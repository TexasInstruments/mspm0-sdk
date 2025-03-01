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
/*!****************************************************************************
 *  @file       drv8316.h
 *  @brief      DRV8316 gate driver Module
 *
 * 
 *  @anchor drv8316_h
 *  # Overview
 *
 *  defines APIs for drv8316 gate driver
 *
 *  <hr>
 ******************************************************************************/

#ifndef DRV8316_H
#define DRV8316_H

#include "stdint.h"
#include "main.h"
#include "appInputCtrlInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined DRV8316 || defined DRV8316_DRIVER

/*! @brief Defines the address mask */
#define DRV8316_ADDR_MASK                   (0x7E00)

/*! @brief Defines the data mask */
#define DRV8316_DATA_MASK                   (0xFF)

/*! @brief Defines the fault bit mask */
#define DRV8316_FAULT_FLG                   0x01


/*! @brief Defines the ICSTAT0 fault mask */
#define DRV8316_ICSTAT0_FAULT_MASK 0x56
/* IC_STAT: RESERVED | BK_FLT  | RESERVED | OCP     | NPOR(UVLO) | OVP       | OT       | FAULT */

/*! @brief Defines the ICSTAT1 fault mask */
#define DRV8316_ICSTAT1_FAULT_MASK 0xFF
/* STAT 1 : OTW      | OTS     | OCP_HC    | OCP_LC  | OCP_HB     | OCP_LB    | OCP_HA   | OCP_LA */

/*! @brief Defines the ICSTAT2 fault mask */
#define DRV8316_ICSTAT2_FAULT_MASK 0x78
/* STAT 2 : OTW MCE  | OTP_ERR | BUCK_OCP  | BUCK_UV | VCP_UV     | SPI_PARITY| SPI_SCLK | SPI_ADDR */

/*! @brief Defines the R/W mask for IC STAT */
#define DRV8316_IC_STAT              0x00
/*! @brief Defines the R/W mask for IC STAT 1 */
#define DRV8316_STAT1                0x01
/*! @brief Defines the R/W mask for IC STAT 2 */
#define DRV8316_STAT2                0x02
/*! @brief Defines the R/W mask for CTRL 1 */
#define DRV8316_CTRL1                0x03
/*! @brief Defines the R/W mask for CTRL 2 */
#define DRV8316_CTRL2                0x04
/*! @brief Defines the R/W mask for CTRL 3 */
#define DRV8316_CTRL3                0x05
/*! @brief Defines the R/W mask for CTRL 4 */
#define DRV8316_CTRL4                0x06
/*! @brief Defines the R/W mask for CTRL 5 */
#define DRV8316_CTRL5                0x07
/*! @brief Defines the R/W mask for CTRL 6 */
#define DRV8316_CTRL6                0x08
/*! @brief Defines the R/W mask for CTRL 7 */
#define DRV8316_CTRL7                0x09
/*! @brief Defines the R/W mask for CTRL 8 */
#define DRV8316_CTRL8                0x0A
/*! @brief Defines the R/W mask for CTRL 9 */
#define DRV8316_CTRL9                0x0B
/*! @brief Defines the R/W mask for CTRL 10 */
#define DRV8316_CTRL10               0x0C
/*! @brief Defines the R/W mask for TESTMODE */
#define DRV8316_TESTMODE             0x3F
/*! @brief Defines the R/W mask for TESTCONFIG 0 */
#define DRV8316_TESTCONFIG0          0x10
/*! @brief Defines the R/W mask for TESTCONFIG 1 */
#define DRV8316_TESTCONFIG1          0x11
/*! @brief Defines the R/W mask for TESTCONFIG 2 */
#define DRV8316_TESTCONFIG2          0x12
/*! @brief Defines the R/W mask for TESTCONFIG 3 */
#define DRV8316_TESTCONFIG3          0x13
/*! @brief Defines the R/W mask for TESTCONFIG 4 */
#define DRV8316_TESTCONFIG4          0x14
/*! @brief Defines the R/W mask for TESTCONFIG 5 */
#define DRV8316_TESTCONFIG5          0x15
/*! @brief Defines the R/W mask for TESTCONFIG 6 */
#define DRV8316_TESTCONFIG6          0x16
/*! @brief Defines the R/W mask for TESTCONFIG 7 */
#define DRV8316_TESTCONFIG7          0x17
/*! @brief Defines the R/W mask for TESTCONFIG 8 */
#define DRV8316_TESTCONFIG8          0x18
/*! @brief Defines the R/W mask for TESTCONFIG 9 */
#define DRV8316_TESTCONFIG9          0x19
/*! @brief Defines the R/W mask for TESTCONFIG 10 */
#define DRV8316_TESTCONFIG10         0x1A
/*! @brief Defines the R/W mask for TESTCONFIG 11 */
#define DRV8316_TESTCONFIG11         0x1B
/*! @brief Defines the R/W mask for TESTCONFIG 12 */
#define DRV8316_TESTCONFIG12         0x1C
/*! @brief Defines the R/W mask for TESTCONFIG 13 */
#define DRV8316_TESTCONFIG13         0x1D
/*! @brief Defines the R/W mask for OTPCONFIG 1 */
#define DRV8316_OTPCONFIG1           0x21
/*! @brief Defines the R/W mask for OTPCONFIG 2 */
#define DRV8316_OTPCONFIG2           0x22
/*! @brief Defines the R/W mask for OTPCONFIG 3 */
#define DRV8316_OTPCONFIG3           0x23
/*! @brief Defines the R/W mask for OTPCONFIG 4 */
#define DRV8316_OTPCONFIG4           0x24
/*! @brief Defines the R/W mask for OTPCONFIG 5 */
#define DRV8316_OTPCONFIG5           0x25
/*! @brief Defines the R/W mask for OTPCONFIG 6 */
#define DRV8316_OTPCONFIG6           0x26
/*! @brief Defines the R/W mask for OTPCONFIG 13 */
#define DRV8316_OTPCONFIG13          0x2D
/*! @brief Defines the R/W mask for OTPTEST */
#define DRV8316_OTPTEST              0x3A
/*! @brief Defines the R/W mask for CSA MASK */
#define DRV8316_CSA_MASK             0x00000003

/*! @brief Defines the CSA GAIN POS */
#define DRV8316_CSA_GAIN_POS         0

/*! @brief Defines the mask for WRITE PROTECT */
#define DRV8316_OTPTEST_WRITE_PROTECT_MASK  0xDF
/*! @brief Defines the OTP RELOAD POS */
#define DRV8316_OTPTEST_OTP_RELOAD_POS      5

/******************************************************************************/
/*! @brief GD1 REG2 mask */
#define GD1_REG2_MASK                       0x0C000000

/*! @brief GD1 REG2 POS in gate driver register */
#define GD1_REG2_GD_POS                     23

/*! @brief GD1 REG2 POS in DRV register */
#define GD1_REG2_DRV_POS                    0

/*! @brief GD1 REG2 write protect mask */
#define GD1_REG2_WRITE_PROTECT_MASK         0xE7

/*! @brief REG2 default value */
#define DRV8316_REG2_DEFAULT                0x38

/******************************************************************************/
/*! @brief GD1 REG3 mask */
#define GD1_REG3_MASK                       0x000C0000

/*! @brief GD1 REG3 POS in gate driver register */
#define GD1_REG3_GD_POS                     16

/*! @brief GD1 REG3 POS in DRV register */
#define GD1_REG3_DRV_POS                    0

/*! @brief GD1 REG3 write protect mask */
#define GD1_REG3_WRITE_PROTECT_MASK         0xF2

/*! @brief REG3 default value */
#define DRV8316_REG3_DEFAULT                0x42

/******************************************************************************/
/*! @brief GD1 REG4 mask */
#define GD1_REG4_MASK                       0x00003F00

/*! @brief GD1 REG4 POS in gate driver register */
#define GD1_REG4_GD_POS                     8

/*! @brief GD1 REG4 POS in DRV register */
#define GD1_REG4_DRV_POS                    0

/*! @brief GD1 REG4 write protect mask */
#define GD1_REG4_WRITE_PROTECT_MASK         0xC8

/*! @brief REG4 default value */
#define DRV8316_REG4_DEFAULT                0x08

/******************************************************************************/
/*! @brief GD1 REG5 mask */
#define GD1_REG5_MASK                       0x00000000

/*! @brief GD1 REG5 POS in gate driver register */
#define GD1_REG5_GD_POS                     0

/*! @brief GD1 REG5 POS in DRV register */
#define GD1_REG5_DRV_POS                    0

/*! @brief GD1 REG5 write protect mask */
#define GD1_REG5_WRITE_PROTECT_MASK         0xFC

/*! @brief REG5 default value */
#define DRV8316_REG5_DEFAULT                0x00

/******************************************************************************/
/*! @brief GD2 REG6 mask */
#define GD2_REG6_MASK                       0x03F00000

/*! @brief GD2 REG6 POS in gate driver register */
#define GD2_REG6_GD_POS                     20

/*! @brief GD2 REG6 POS in DRV register */
#define GD2_REG6_DRV_POS                    0

/*! @brief GD2 REG6 write protect mask */
#define GD2_REG6_WRITE_PROTECT_MASK         0x00

/*! @brief REG6 default value */
#define DRV8316_REG6_DEFAULT                0x00

/******************************************************************************/
/*! @brief GD2 REG10 mask */
#define GD2_REG10_MASK                      0x7C000000

/*! @brief GD2 REG10 POS in gate driver register */
#define GD2_REG10_GD_POS                    26

/*! @brief GD2 REG10 POS in DRV register */
#define GD2_REG10_DRV_POS                   0

/*! @brief GD2 REG10 write protect mask */
#define GD2_REG10_WRITE_PROTECT_MASK        0xE0

/*! @brief REG10 default value */
#define DRV8316_REG10_DEFAULT               0x00

/******************************************************************************/
/*! @brief DRV8316_CFG1 register */
typedef union
{
    struct drv8316Cfg1
    {
        uint32_t
        /*! Current Sense Amplifier (CSA) gain */
        csaGain:            2,
        /*! Active Synchronous Rectification Enable Bit */
        enASR:              1,
        /*! Active Asynshronous Rectification Enable Bit */
        enAAR:              1,
        /*! Reserved */
        reserved1:          1,
        /*! Reserved */
        reserved2:          1,
        /*! OCP Fault Options */
        IlimRecir:          1,
        /*! OCP Retry Time Settings */
        bemfThr:            1,
        /*! OCP Fault Options */
        ocpMode:            2,
        /*! Overcurrent Level Setting */
        ocpLvl:             1,
        /*! OCP Retry Time Settings */
        ocpRetry:           1,
        /*! OCP Deglitch Time Settings */
        cbcDeg:             2,
        /*! OCP PWM Cycle Operation Bit */
        ocpCBC:             1,
        /*! Driver OFF Bit */
        drvOff:             1,
        /*! Overtemperature Warning Reporting Bit */
        otwRep:             1,
        /*! Reserved */
        reserved3:          1,
        /*! Overvoltage Enable Bit */
        ovpEn:              1,
        /*! Overvoltage Level Setting */
        ovpSel:             1,
        /*! Frequency of PWM at 100% Duty Cycle */
        pwm100DutySel:      1,
        /*! Device Mode Selection */
        disRR:              1,
        /*! Device Mode Selection */
        halfRR:             1,
        /*! Clear Fault */
        clrFLT:             1,
        /*! Device Mode Selection */
        pwmMode:            2,
        /*! Slew Rate Settings */
        slewRate:           2,
        /*! Reserved */
        reserved:           4;
   }b;
    uint32_t w;
}GATE_DRIVE_DRV8316_CFG1_T;

/*! @brief DRV8316_CFG2 register */
typedef union
{
    struct drv8316Cfg2
    {
        uint32_t
        /*! Reserved */
        reserved:           17,
        /*! Minimum on time */
        minOnTime:          3,
        /*! Buck Disable Bit */
        buckDis:            1,
        /*! Buck Voltage Selection */
        buckSel:            2,
        /*! Buck Current Limit Setting */
        buckCl:             1,
        /*! Buck Power Sequencing Disable Bit */
        buckPsDis:          1,
        /*! Buck slew rate */
        buckSr:             1,
        /*! Target delay */
        targetDelay:        4,
        /*! Driver delay compensation enable */
        delayCompEn:        1,
        /*! Reserved */
        reserved1:          1;
   }b;
   uint32_t w;
}GATE_DRIVE_DRV8316_CFG2_T;

/*/*! @brief Interface Registers to configure Gate Driver DRV8316 */
typedef struct hvDie8316Interface
{
    /*! gate driver config 1 */
    GATE_DRIVE_DRV8316_CFG1_T    gateDrvCfg1;
    /*! gate driver config 2 */
    GATE_DRIVE_DRV8316_CFG2_T    gateDrvCfg2;
} HV_DIE_EEPROM_INTERFACE_T;

extern HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig;

void drv8316ParamsInit(void);

/**
 * @brief     Read fault status from DRV8316
 * @return    Returns fault status
 */
uint32_t drv8316FaultStatusRead(void);

/**
 * @brief     Read fault bit from DRV8316
 * @return    Returns fault bit
 */
uint32_t drv8316FaultBitRead(void);

/**
 * @brief     Enters DRV8316 test mode
 */
void drv8316TestModeEntry(void);

#endif

#ifdef __cplusplus
}
#endif
#endif /* DRV8316_H */
