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

#include "drv8323.h"
#include "drv8323_spi_hal.h"
#include "main.h"
#include "ISR.h"
#include "focHALInterface.h"

#ifdef DRV8323

/*! @brief VDS Over current fault index in gate driver fault reporting register */
#define VDS_OCP_FAULT_INDEX         0x47E00000
/*! @brief SNS Over current fault index in gate driver fault reporting register */
#define SNS_OCP_FAULT_INDEX         0x001C0000
/*! @brief OTW fault index in gate driver fault reporting register */
#define OTW_FAULT_INDEX             0x00020000
/*! @brief CPUV fault index in gate driver fault reporting register */
#define CPUV_FAULT_INDEX            0x00010000
/*! @brief Gate driver fault index in gate driver fault reporting register */
#define GDF_FAULT_INDEX             0x2000FC00

/*! @brief  DRV8323_CTRL2 config default value */
#define DRV8323_CTRL2_CONFIG_DEFAULT  (0x00  & DRV_REG2_WRITE_PROTECT_MASK)
/*! @brief  DRV8323_CTRL3 config default value */
#define DRV8323_CTRL3_CONFIG_DEFAULT  (0x3FF & DRV_REG3_WRITE_PROTECT_MASK)
/*! @brief  DRV8323_CTRL4 config default value */
#define DRV8323_CTRL4_CONFIG_DEFAULT  (0x3FF & DRV_REG4_WRITE_PROTECT_MASK)
/*! @brief  DRV8323_CTRL5 config default value */
#define DRV8323_CTRL5_CONFIG_DEFAULT  (0x119 & DRV_REG5_WRITE_PROTECT_MASK)
/*! @brief  DRV8323_CTRL6 config default value */
#define DRV8323_CTRL6_CONFIG_DEFAULT  (0x283 & DRV_REG6_WRITE_PROTECT_MASK)

/* Gate driver configuration registers */

/*! @brief  DRV8323_CTRL2 config default value */
uint16_t drv8323_ctrl2_config = 0x00;
/*! @brief  DRV8323_CTRL3 config default value */
uint16_t drv8323_ctrl3_config = 0x3FF;
/*! @brief  DRV8323_CTRL4 config default value */
uint16_t drv8323_ctrl4_config = 0x3FF;
/*! @brief  DRV8323_CTRL5 config default value */
uint16_t drv8323_ctrl5_config = 0x119;
/*! @brief  DRV8323_CTRL6 config default value */
uint16_t drv8323_ctrl6_config = 0x283;

/*! @brief  DRV8323 fault status 1 */
uint32_t faultStatus = 0;
/*! @brief  DRV8323 fault status 2 */
uint16_t drvDataNewFaultStatus = 0;

/*! @brief Gate driver fault report register, faults with 1 are set to report */
uint32_t gateDrivernFaultReport = 0x18000000;
/*! @brief Gate driver fault action register, faults with 1 are set to clear if reported */
uint32_t gateDriverFaultAction = 0x18000000;

/**************************************************************************/

void gateDriverConfig(void)
{
    volatile uint16_t
    count = 0,
    dataw_8323[16] =
    {0, 0, 0, 0x3FF, 0x07FF, 0x119, 0x283,
     0x001, 0, 0, 0, 0, 0, 0x102, 0x010, 0x700};

    /* One Time Programmable memory (OTP) is not written.
     * Write OTP shadow registers to emulate OTP data */

    drv8323SPIWrite(0x07, 0x01);

    drv8323SPIWrite(0x08, 0xBA);
    drv8323SPIWrite(0x08, 0x32);
    drv8323SPIWrite(0x08, 0xED);
    drv8323SPIWrite(0x08, 0x14);

    for(count = 0; count < 16; count++)
    {
        if(count != 8)
        {
            drv8323SPIWrite(count, dataw_8323[count]);
        }
    }

    drv8323SPIWrite(0x08, 0x0);

}

uint32_t drv8323StatusRead(void)
{

    faultStatus = drv8323SPIRead(faultStatus, 0);
    faultStatus = faultStatus << 21;
    drvDataNewFaultStatus  = drv8323SPIRead(drvDataNewFaultStatus, 1);
    faultStatus = faultStatus | (((uint32_t)drvDataNewFaultStatus) << 10);
    return faultStatus;
}

void gateDriverClearFault(void)
{
    volatile uint16_t writeVal_SPI = 0;

    writeVal_SPI = drv8323_ctrl2_config | 0x01;

    drv8323SPIWrite(DRV8323_REG_2,writeVal_SPI);
}

uint32_t gateDriverGetFaultStatus(void)
{
    uint32_t drv8323Status;

    drv8323Status = drv8323StatusRead();

    return  (drv8323Status & 0xFFFFFC00);
}

void gateDriverParamsUpdate(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig)
{
    uint32_t temp;

    if(!pGateDriverConfig->gateDrvCfg2.b.repOTW)
    {
        gateDrivernFaultReport  &= ~(OTW_FAULT_INDEX);
    }
    else
    {
        gateDrivernFaultReport  |=  OTW_FAULT_INDEX;
    }

    if(!pGateDriverConfig->gateDrvCfg2.b.disGDF)
    {
        gateDrivernFaultReport  |= GDF_FAULT_INDEX;
      gateDriverFaultAction   |= GDF_FAULT_INDEX;
    }
    else
    {
      gateDrivernFaultReport  &= ~(GDF_FAULT_INDEX);
      gateDriverFaultAction   &= ~(GDF_FAULT_INDEX);
    }

    if(!pGateDriverConfig->gateDrvCfg2.b.disCPUV)
    {
        gateDrivernFaultReport  |= CPUV_FAULT_INDEX;
      gateDriverFaultAction   |= CPUV_FAULT_INDEX;
    }
    else
    {
        gateDrivernFaultReport  &= ~(CPUV_FAULT_INDEX);
      gateDriverFaultAction   &= ~(CPUV_FAULT_INDEX);
    }

    temp = (pGateDriverConfig->gateDrvCfg1.b.ocpMode);

    if(temp <= 2)
    {
        gateDrivernFaultReport  |= ((VDS_OCP_FAULT_INDEX) | (SNS_OCP_FAULT_INDEX)) ;
    }
    else
    {
        gateDrivernFaultReport  &= ~((VDS_OCP_FAULT_INDEX) | (SNS_OCP_FAULT_INDEX)) ;
    }

    if(temp < 1)
    {
        gateDriverFaultAction   |= ((VDS_OCP_FAULT_INDEX) | (SNS_OCP_FAULT_INDEX)) ;
    }
    else
    {
        gateDriverFaultAction   &= ~((VDS_OCP_FAULT_INDEX) | (SNS_OCP_FAULT_INDEX)) ;
    }

    drv8323_ctrl2_config = (DRV8323_CTRL2_CONFIG_DEFAULT |
            (~DRV_REG2_WRITE_PROTECT_MASK &((pGateDriverConfig->gateDrvCfg2.w & GD2_REG2_MASK) >> GD2_REG2_POS )));

    drv8323SPIWrite(DRV8323_REG_2, drv8323_ctrl2_config) ;

    /* register 3 */

    drv8323_ctrl3_config = (DRV8323_CTRL3_CONFIG_DEFAULT |
                (~DRV_REG3_WRITE_PROTECT_MASK &((pGateDriverConfig->gateDrvCfg2.w & GD2_REG3_MASK) >> GD2_REG3_POS )));

        drv8323SPIWrite(DRV8323_REG_3, drv8323_ctrl3_config) ;

    /* register 4 */

    drv8323_ctrl4_config = (DRV8323_CTRL4_CONFIG_DEFAULT |
                   (~DRV_REG4_WRITE_PROTECT_MASK &(((pGateDriverConfig->gateDrvCfg2.w & GD2_REG4_MASK) >> (GD2_REG4_POS - GD2_REG4_DRV_POS)) |
                                                   ((pGateDriverConfig->gateDrvCfg1.w & GD1_REG4_MASK) >> GD1_REG4_POS ))));

   drv8323SPIWrite(DRV8323_REG_4, drv8323_ctrl4_config) ;

       /* register 5 */

   drv8323_ctrl5_config = (DRV8323_CTRL5_CONFIG_DEFAULT |
                  (~DRV_REG5_WRITE_PROTECT_MASK &((pGateDriverConfig->gateDrvCfg1.w & GD1_REG5_MASK) >> GD1_REG5_POS )));


   drv8323SPIWrite(DRV8323_REG_5, drv8323_ctrl5_config) ;

       /* register 6 */

   drv8323_ctrl6_config = (DRV8323_CTRL6_CONFIG_DEFAULT |
                    (~DRV_REG6_WRITE_PROTECT_MASK &((pGateDriverConfig->gateDrvCfg1.w & GD1_REG6_MASK) >> GD1_REG6_POS )));


   drv8323SPIWrite(DRV8323_REG_6, drv8323_ctrl6_config) ;
}

void gateDriverFaultResponse(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig)
{
    uint32_t temp_ocp = 0, temp_cpuv =0, temp_gdf =0, temp_uvlo = 0, temp_otsd =0;

    temp_cpuv = pGateDriverConfig->gateDrvCfg2.b.disCPUV;
    temp_ocp  = pGateDriverConfig->gateDrvCfg1.b.ocpMode;
    temp_gdf  = pGateDriverConfig->gateDrvCfg2.b.disGDF;
    temp_uvlo = (gateDriverRawFaultStatus & GD_UVLO_FAULT_MASK) >> GD_UVLO_FAULT_POS;
    temp_otsd = (gateDriverRawFaultStatus & GD_OTSD_FAULT_MASK) >> GD_OTSD_FAULT_POS;

    if((gateDriverRawFaultStatus & 0x781D0000) != 0)  /* VDS_OCP , GDF,UVLO,OTSD, SA_OC,SB_OC,SC_OC,,CPUV, */
    {
        if ((temp_ocp < 2) || (temp_cpuv == 0) || (temp_gdf == 0) || (temp_uvlo == 1) || (temp_otsd == 1)) /* OTSD */
        {
            HAL_DisablePWM();
        }

        /* Delay before Faults are cleared from HV die */
        HAL_delayMicroSeconds(25);
    }

}

void gateDriverCSACalib(void)
{
    /* Configure the DRV8323 Registers */
    drv8323_ctrl6_config = drv8323SPIRead(drv8323_ctrl6_config, 0x6);
    drv8323_ctrl6_config |= 0x001C;

    drv8323SPIWrite(0x6, drv8323_ctrl6_config);

    /*wait for 100us As per review comments dated 6/13/19 */
    HAL_delayMicroSeconds(100);

    /* Reset , Set and Reset the DRV8x Calibration ,
     *  Do the Calib twice to get the consistent WCOMP Trim o/p's */

    drv8323_ctrl6_config &= ~0x001C;

    drv8323SPIWrite(0x6, drv8323_ctrl6_config);

    HAL_delayMicroSeconds(100);

}

void gateDriverOffsetCalibSet(void)
{

}

void gateDriverOffsetCalibReset(void)
{

}

void gateDriverInit(void)
{
    drv8323_ctrl2_config = 0x00;
    drv8323_ctrl3_config = 0x3FF;
    drv8323_ctrl4_config = 0x3FF;
    drv8323_ctrl5_config = 0x119;
    drv8323_ctrl6_config = 0x283;

    gateDrivernFaultReport = 0x18000000;
    gateDriverFaultAction = 0x18000000;
}

uint16_t gateDriverRegRead(int16_t regAddr)
{
    return drv8323SPIRead(0, regAddr);
}

#endif
