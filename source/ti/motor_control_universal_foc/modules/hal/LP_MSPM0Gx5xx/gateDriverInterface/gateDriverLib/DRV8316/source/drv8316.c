/*
 * Copyright (c) 2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>

#include "drv8316.h"
#include "drv8316_spi_hal.h"
#include "main.h"
#include "ISR.h"
#include "focHALInterface.h"

#if defined DRV8316 || defined DRV8316_DRIVER

/*! @brief Over current fault index in gate driver fault reporting register */
#define OCP_FAULT_INDEX             0x103F0000

/*! @brief Over voltage fault index in gate driver fault reporting register */
#define OVP_FAULT_INDEX             0x04000000

/*! @brief Over Temperature warning fault index in gate driver fault reporting 
register */
#define OTW_FAULT_INDEX             0x00800000

/* Gate driver configuration registers */

/*! @brief DRV8316 CNTRL2 config register */
uint16_t drv8316_ctrl2_config = 0x20;
/*! @brief DRV8316 CNTRL3 config register */
uint16_t drv8316_ctrl3_config;
/*! @brief DRV8316 CNTRL4 config register */
uint16_t drv8316_ctrl4_config;
/*! @brief DRV8316 CNTRL5 config register */
uint16_t drv8316_ctrl5_config;
/*! @brief DRV8316 CNTRL6 config register */
uint16_t drv8316_ctrl6_config;
/*! @brief DRV8316 CNTRL10 config register */
uint16_t drv8316_ctrl10_config;

uint8_t 
    /*! @brief DRV8316 ICSTAT register */
    drv8316ICstat = 0 , 
    /*! @brief DRV8316 ICSTAT1 register */
    drv8316Stat1 = 0 , 
    /*! @brief DRV8316 ICSTAT2 register */
    drv8316stat2 = 0;

/* Gate driver and controller faults which are reported out always and go to HI-Z state
 when fault happens, are set to 1 as default in following fault action registers */
/*! @brief Gate driver fault report register, faults with 1 are set to report */
uint32_t gateDrivernFaultReport = 0x00407800;
/*! @brief Gate driver fault action register.
Motor stops when a fault in action occurs */
uint32_t gateDriverFaultAction  = 0x00404800;

void gateDriverConfig(void)
{
    uint16_t otpTest,testConfig6, ctrlReg2 = 0;

    /* No Default values to be written as OTP in place */
    /* Unlock the registers write */

    drv8316SPIWrite(0x03, 0x03);

    ctrlReg2 = drv8316SPIRead(0, DRV8316_CTRL2);

    ctrlReg2 |= 0x1;

    drv8316SPIWrite(DRV8316_CTRL2, ctrlReg2);

    drv8316TestModeEntry();

    testConfig6 = drv8316SPIRead(0, DRV8316_TESTCONFIG6);
    testConfig6 = (testConfig6 |(0x03));

    drv8316SPIWrite(DRV8316_TESTCONFIG6,testConfig6);

    otpTest = drv8316SPIRead(0, DRV8316_OTPTEST);

    otpTest = (otpTest & DRV8316_OTPTEST_WRITE_PROTECT_MASK) |
            (1 << DRV8316_OTPTEST_OTP_RELOAD_POS);

    drv8316SPIWrite(DRV8316_OTPTEST, otpTest);

}

void gateDriverCSACalib(void)
{
    /* No CSA calibration bit */
}

void gateDriverClearFault(void)
{
    volatile uint16_t readVal = 0, writeVal = 0;
    writeVal = drv8316_ctrl2_config | 0x01;
    drv8316SPIWrite(DRV8316_CTRL1, 0x03);

    drv8316SPIWrite(DRV8316_CTRL2,writeVal);
}

uint32_t drv8316FaultStatusRead(void)
{
    uint32_t faultStatus = 0;
    uint8_t drv8316ICstatFault = 0,
            drv8316Stat1Fault = 0,
            drv8316stat2Fault = 0;

    drv8316ICstat = drv8316SPIRead(drv8316ICstat, DRV8316_IC_STAT);

    if((drv8316ICstat & 0x08) == 0)
    {
        HAL_DisableINT();
        HAL_DisablePWM();
        HAL_ClearNFault();

        HAL_delayMicroSeconds(100);

        HAL_resetPOR();

    }

    drv8316ICstatFault = drv8316ICstat & DRV8316_ICSTAT0_FAULT_MASK;

    drv8316Stat1 = drv8316SPIRead(drv8316Stat1, DRV8316_STAT1);
    drv8316Stat1Fault = drv8316Stat1 & DRV8316_ICSTAT1_FAULT_MASK;

    drv8316stat2 = drv8316SPIRead(drv8316stat2, DRV8316_STAT2);
    drv8316stat2Fault = drv8316stat2 & DRV8316_ICSTAT2_FAULT_MASK;

    /* Following is the bit position of fault status register */

    /*    IC_STAT: GD_FAULT | BK_FLT  | SPI_FAULT | OCP     | NPOR(UVLO) | OVP       | OT       | RESERVED
      STAT 1 : OTW      | OTS     | OCP_HC    | OCP_LC  | OCP_HB     | OCP_LB    | OCP_HA   | OCP_LA
      STAT 2 : RESERVED | OTP_ERR | BUCK_OCP  | BUCK_UV | VCP_UV     | SPI_PARITY| SPI_SCLK | SPI_ADDR
     */
    faultStatus = ((drv8316ICstatFault << 8 ) | drv8316Stat1Fault);
    faultStatus = ((faultStatus << 16) | (drv8316stat2Fault << 8));

    /* set fault b it in MSB */

    if((drv8316ICstatFault & 0x1) != 0)
        faultStatus |= 0x80000000;
    else
        faultStatus &= 0x7FFFFFFF;

    return faultStatus;
}

void gateDriverSetCSAGain(uint16_t setGain)
{
    uint16_t write, csaGain;

    csaGain = setGain << DRV8316_CSA_GAIN_POS;

    write = (drv8316_ctrl5_config & (~DRV8316_CSA_MASK)) | csaGain;

    drv8316SPIWrite(DRV8316_CTRL5, write);
}

uint16_t gateDriverGetCSAGain(void)
{
    uint16_t gain, read;
    drv8316_ctrl5_config = drv8316SPIRead(drv8316_ctrl5_config, DRV8316_CTRL5);

    read = (drv8316_ctrl5_config & DRV8316_CSA_MASK);

    gain = read >> DRV8316_CSA_GAIN_POS;

    return gain;

}

void drv8316TestModeEntry(void)
{
    drv8316SPIWrite(DRV8316_TESTMODE, 0xBA);
    drv8316SPIWrite(DRV8316_TESTMODE, 0x32);
    drv8316SPIWrite(DRV8316_TESTMODE, 0xED);
    drv8316SPIWrite(DRV8316_TESTMODE, 0x14);
}

uint32_t drv8316FaultBitRead(void)
{
    uint32_t faultStatus = 0;
    faultStatus = drv8316SPIRead(0, DRV8316_IC_STAT);
    return (faultStatus & DRV8316_FAULT_FLG);
}

void gateDriverParamsUpdate(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig)
{
    uint32_t temp;

    uint16_t targetDelay;

    drv8316_ctrl2_config = (DRV8316_REG2_DEFAULT &
            GD1_REG2_WRITE_PROTECT_MASK) |
                    ((pGateDriverConfig->gateDrvCfg1.w & GD1_REG2_MASK) >>
                            (GD1_REG2_GD_POS - GD1_REG2_DRV_POS));
    drv8316SPIWrite(DRV8316_CTRL2, drv8316_ctrl2_config);

    drv8316_ctrl3_config = (DRV8316_REG3_DEFAULT &
            GD1_REG3_WRITE_PROTECT_MASK) |
                    ((pGateDriverConfig->gateDrvCfg1.w & GD1_REG3_MASK) >>
                            (GD1_REG3_GD_POS - GD1_REG3_DRV_POS));
    drv8316SPIWrite(DRV8316_CTRL3, drv8316_ctrl3_config);

    drv8316_ctrl4_config = (DRV8316_REG4_DEFAULT &
            GD1_REG4_WRITE_PROTECT_MASK) |
                    ((pGateDriverConfig->gateDrvCfg1.w & GD1_REG4_MASK) >>
                            (GD1_REG4_GD_POS - GD1_REG4_DRV_POS));
    drv8316SPIWrite(DRV8316_CTRL4, drv8316_ctrl4_config);


    drv8316_ctrl6_config = (DRV8316_REG6_DEFAULT &
            GD2_REG6_WRITE_PROTECT_MASK) |
                    ((pGateDriverConfig->gateDrvCfg2.w & GD2_REG6_MASK) >>
                            (GD2_REG6_GD_POS - GD2_REG6_DRV_POS));
    drv8316SPIWrite(DRV8316_CTRL6, drv8316_ctrl6_config);

    if(pGateDriverConfig->gateDrvCfg2.b.targetDelay == 0)
    {
        switch(pGateDriverConfig->gateDrvCfg1.b.slewRate)
        {
        case 0:
            targetDelay = 0xF;
            break;
        case 1:
            targetDelay = 0xB;
            break;
        case 2:
            targetDelay = 0x8;
            break;
        case 3:
            targetDelay = 0x5;
            break;
        default:
            targetDelay = 0xF;
            break;
        }
    }
    else
    {
        targetDelay = pGateDriverConfig->gateDrvCfg2.b.targetDelay;
    }

    drv8316_ctrl10_config = (DRV8316_REG10_DEFAULT &
            GD2_REG10_WRITE_PROTECT_MASK) |
                    ((pGateDriverConfig->gateDrvCfg2.w & GD2_REG10_MASK) >>
                        (GD2_REG10_GD_POS - GD2_REG10_DRV_POS)) | targetDelay;

    drv8316SPIWrite(DRV8316_CTRL10, drv8316_ctrl10_config);

    /* fault configuration for OTW */
    if(pGateDriverConfig->gateDrvCfg1.b.otwRep)
    {
        gateDrivernFaultReport  |=  OTW_FAULT_INDEX;
    }
    else
    {
        gateDrivernFaultReport  &= ~(OTW_FAULT_INDEX);
    }

    /* fault configuration for OCP */

    temp = (pGateDriverConfig->gateDrvCfg1.b.ocpMode);

    if(temp <= 2)
    {
        gateDrivernFaultReport  |= (OCP_FAULT_INDEX);
    }
    else
    {
        gateDrivernFaultReport  &= ~(OCP_FAULT_INDEX);
    }

    if(temp <= 1)
    {
        gateDriverFaultAction   |= OCP_FAULT_INDEX ;
    }
    else
    {
        gateDriverFaultAction   &= ~(OCP_FAULT_INDEX);
    }

    /* fault configuration for OVP */
    if(pGateDriverConfig->gateDrvCfg1.b.ovpEn)
    {
        gateDrivernFaultReport  |= (OVP_FAULT_INDEX);
        gateDriverFaultAction   |= (OVP_FAULT_INDEX);
    }
    else
    {
        gateDrivernFaultReport  &= ~(OVP_FAULT_INDEX);
        gateDriverFaultAction   &= ~(OVP_FAULT_INDEX);
    }

}

void gateDriverFaultResponse(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig)
{

    /* if NPOR, VCP_UV, OTP_ERR and OTS faults, disable PWM
       for OCP and OVP, move to HI_Z based on config, disable PWM */

    if(((gateDriverRawFaultStatus & 0x00404800) != 0) ||  /* VCP_UV, OTP_ERR and OTS */
            (((gateDriverRawFaultStatus & 0x103F0000) != 0) 
            && (pGateDriverConfig->gateDrvCfg1.b.ocpMode < 2 )) ||
            (((gateDriverRawFaultStatus & 0x04000000) != 0) 
            && (pGateDriverConfig->gateDrvCfg1.b.ovpEn == 1)))
    {
        HAL_DisablePWM();

        /* Delay before Faults are cleared from HV die */
        HAL_delayMicroSeconds(25);

    }
}

uint32_t gateDriverGetFaultStatus(void)
{
    uint32_t drv8316Status;

    drv8316Status = drv8316FaultStatusRead();

    return  (drv8316Status & 0xFFFF7F00);
}

void gateDriverOffsetCalibReset(void)
{
    drv8316SPIWrite(19, 0);
}

void gateDriverOffsetCalibSet(void)
{
    drv8316SPIWrite(19, 0x94);
}

void gateDriverInit(void)
{
    gateDrivernFaultReport = 0x00407800;
    gateDriverFaultAction  = 0x00404800;
}

uint16_t gateDriverRegRead(int16_t regAddr)
{
    return drv8316SPIRead(0, regAddr);
}

#endif
