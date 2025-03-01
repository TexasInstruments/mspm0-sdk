/* --COPYRIGHT--,BSD
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
#ifndef __PMBUS_H__
#define __PMBUS_H__

//*****************************************************************************
//
//! \addtogroup pmbus
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Include files
//
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ti/smbus/smbus.h"

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

// The PMBus commands below are are supported by the PMBus_cmdRead and
// PMBus_cmdWrite APIs.
//
// The PMBus commands which are not listed below are manufacturer-specific, or
// have variable data packets.
//
// Use the appropriate lower level API.(i.e. PM PMBus_cmdSendByte,
// PMBus_cmdWriteByte, PMBus_cmdWriteWord, PMBus_cmdBlockWrite
// PMBus_cmdReadByte, PMBus_cmdReadWord, PMBus_cmdBlockRead,
// PMBus_cmdBlockWriteBlockReadProcessCall for these commands.
//
// Some information about these commands is in the comment section of pmbus.h.
// For more details on exact usage of the manufacture-specific commands, see
// the device documentation and the PMBus specification.

/*! @brief PMBus Command PMB_PAGE */
#define PMB_PAGE                            0x00
/*! @brief PMBus Command PMB_OPERATION */
#define PMB_OPERATION                       0x01
/*! @brief PMBus Command PMB_ON_OFF_CONFIG */
#define PMB_ON_OFF_CONFIG                   0x02
/*! @brief PMBus Command PMB_CLEAR_FAULTS */
#define PMB_CLEAR_FAULTS                    0x03
/*! @brief PMBus Command PMB_PHASE */
#define PMB_PHASE                           0x04
/*! @brief PMBus Command PMB_PAGE_PLUS_WRITE */
#define PMB_PAGE_PLUS_WRITE                 0x05
/*! @brief PMBus Command PMB_ZONE_ACTIVE */
#define PMB_ZONE_ACTIVE                     0x08
/*! @brief PMBus Command PMB_WRITE_PROTECT */
#define PMB_WRITE_PROTECT                   0x10
/*! @brief PMBus Command PMB_STORE_DEFAULT_ALL */
#define PMB_STORE_DEFAULT_ALL               0x11
/*! @brief PMBus Command PMB_RESTORE_DEFAULT_ALL */
#define PMB_RESTORE_DEFAULT_ALL             0x12
/*! @brief PMBus Command PMB_STORE_DEFAULT_CODE */
#define PMB_STORE_DEFAULT_CODE              0x13
/*! @brief PMBus Command PMB_RESTORE_DEFAULT_CODE */
#define PMB_RESTORE_DEFAULT_CODE            0x14
/*! @brief PMBus Command PMB_STORE_USER_ALL */
#define PMB_STORE_USER_ALL                  0x15
/*! @brief PMBus Command PMB_RESTORE_USER_ALL */
#define PMB_RESTORE_USER_ALL                0x16
/*! @brief PMBus Command PMB_STORE_USER_CODE */
#define PMB_STORE_USER_CODE                 0x17
/*! @brief PMBus Command PMB_RESTORE_USER_CODE */
#define PMB_RESTORE_USER_CODE               0x18
/*! @brief PMBus Command PMB_CAPABILITY */
#define PMB_CAPABILITY                      0x19
/*! @brief PMBus Command PMB_VOUT_MODE */
#define PMB_VOUT_MODE                       0x20
/*! @brief PMBus Command PMB_VOUT_COMMAND */
#define PMB_VOUT_COMMAND                    0x21
/*! @brief PMBus Command PMB_VOUT_TRIM */
#define PMB_VOUT_TRIM                       0x22
/*! @brief PMBus Command PMB_VOUT_CAL_OFFSET */
#define PMB_VOUT_CAL_OFFSET                 0x23
/*! @brief PMBus Command PMB_VOUT_MAX */
#define PMB_VOUT_MAX                        0x24
/*! @brief PMBus Command PMB_VOUT_MARGIN_HIGH */
#define PMB_VOUT_MARGIN_HIGH                0x25
/*! @brief PMBus Command PMB_VOUT_MARGIN_LOW */
#define PMB_VOUT_MARGIN_LOW                 0x26
/*! @brief PMBus Command PMB_VOUT_TRANSITION_RATE */
#define PMB_VOUT_TRANSITION_RATE            0x27
/*! @brief PMBus Command PMB_VOUT_DROOP */
#define PMB_VOUT_DROOP                      0x28
/*! @brief PMBus Command PMB_VOUT_SCALE_LOOP */
#define PMB_VOUT_SCALE_LOOP                 0x29
/*! @brief PMBus Command PMB_VOUT_SCALE_MONITOR */
#define PMB_VOUT_SCALE_MONITOR              0x2A
/*! @brief PMBus Command PMB_VOUT_MIN */
#define PMB_VOUT_MIN                        0x2B
/*! @brief PMBus Command PMB_POUT_MAX */
#define PMB_POUT_MAX                        0x31
/*! @brief PMBus Command PMB_MAX_DUTY */
#define PMB_MAX_DUTY                        0x32
/*! @brief PMBus Command PMB_FREQUENCY_SWITCH */
#define PMB_FREQUENCY_SWITCH                0x33
/*! @brief PMBus Command PMB_POWER_MODE */
#define PMB_POWER_MODE                      0x34
/*! @brief PMBus Command PMB_VIN_ON */
#define PMB_VIN_ON                          0x35
/*! @brief PMBus Command PMB_VIN_OFF */
#define PMB_VIN_OFF                         0x36
/*! @brief PMBus Command PMB_INTERLEAVE */
#define PMB_INTERLEAVE                      0x37
/*! @brief PMBus Command PMB_IOUT_CAL_GAIN */
#define PMB_IOUT_CAL_GAIN                   0x38
/*! @brief PMBus Command PMB_IOUT_CAL_OFFSET */
#define PMB_IOUT_CAL_OFFSET                 0x39
/*! @brief PMBus Command PMB_FAN_CONFIG_1_2 */
#define PMB_FAN_CONFIG_1_2                  0x3A
/*! @brief PMBus Command PMB_FAN_COMMAND_1 */
#define PMB_FAN_COMMAND_1                   0x3B
/*! @brief PMBus Command PMB_FAN_COMMAND_2 */
#define PMB_FAN_COMMAND_2                   0x3C
/*! @brief PMBus Command PMB_FAN_CONFIG_3_4 */
#define PMB_FAN_CONFIG_3_4                  0x3D
/*! @brief PMBus Command PMB_FAN_COMMAND_3 */
#define PMB_FAN_COMMAND_3                   0x3E
/*! @brief PMBus Command PMB_FAN_COMMAND_4 */
#define PMB_FAN_COMMAND_4                   0x3F
/*! @brief PMBus Command PMB_VOUT_OV_FAULT_LIMIT */
#define PMB_VOUT_OV_FAULT_LIMIT             0x40
/*! @brief PMBus Command PMB_VOUT_OV_FAULT_RESPONSE */
#define PMB_VOUT_OV_FAULT_RESPONSE          0x41
/*! @brief PMBus Command PMB_VOUT_OV_WARN_LIMIT */
#define PMB_VOUT_OV_WARN_LIMIT              0x42
/*! @brief PMBus Command PMB_VOUT_UV_WARN_LIMIT */
#define PMB_VOUT_UV_WARN_LIMIT              0x43
/*! @brief PMBus Command PMB_VOUT_UV_FAULT_LIMIT */
#define PMB_VOUT_UV_FAULT_LIMIT             0x44
/*! @brief PMBus Command PMB_VOUT_UV_FAULT_RESPONSE */
#define PMB_VOUT_UV_FAULT_RESPONSE          0x45
/*! @brief PMBus Command PMB_IOUT_OC_FAULT_LIMIT */
#define PMB_IOUT_OC_FAULT_LIMIT             0x46
/*! @brief PMBus Command PMB_IOUT_OC_FAULT_RESPONSE */
#define PMB_IOUT_OC_FAULT_RESPONSE          0x47
/*! @brief PMBus Command PMB_IOUT_OC_LV_FAULT_LIMIT */
#define PMB_IOUT_OC_LV_FAULT_LIMIT          0x48
/*! @brief PMBus Command PMB_IOUT_OC_LV_FAULT_RESPONSE */
#define PMB_IOUT_OC_LV_FAULT_RESPONSE       0x49
/*! @brief PMBus Command PMB_IOUT_OC_WARN_LIMIT */
#define PMB_IOUT_OC_WARN_LIMIT              0x4A
/*! @brief PMBus Command PMB_IOUT_UC_FAULT_LIMIT */
#define PMB_IOUT_UC_FAULT_LIMIT             0x4B
/*! @brief PMBus Command PMB_IOUT_UC_FAULT_RESPONSE */
#define PMB_IOUT_UC_FAULT_RESPONSE          0x4C
/*! @brief PMBus Command PMB_OT_FAULT_LIMIT */
#define PMB_OT_FAULT_LIMIT                  0x4F
/*! @brief PMBus Command PMB_OT_FAULT_RESPONSE */
#define PMB_OT_FAULT_RESPONSE               0x50
/*! @brief PMBus Command PMB_OT_WARN_LIMIT */
#define PMB_OT_WARN_LIMIT                   0x51
/*! @brief PMBus Command PMB_UT_WARN_LIMIT */
#define PMB_UT_WARN_LIMIT                   0x52
/*! @brief PMBus Command PMB_UT_FAULT_LIMIT */
#define PMB_UT_FAULT_LIMIT                  0x53
/*! @brief PMBus Command PMB_UT_FAULT_RESPONSE */
#define PMB_UT_FAULT_RESPONSE               0x54
/*! @brief PMBus Command PMB_VIN_OV_FAULT_LIMIT */
#define PMB_VIN_OV_FAULT_LIMIT              0x55
/*! @brief PMBus Command PMB_VIN_OV_FAULT_RESPONSE */
#define PMB_VIN_OV_FAULT_RESPONSE           0x56
/*! @brief PMBus Command PMB_VIN_OV_WARN_LIMIT */
#define PMB_VIN_OV_WARN_LIMIT               0x57
/*! @brief PMBus Command PMB_VIN_UV_WARN_LIMIT */
#define PMB_VIN_UV_WARN_LIMIT               0x58
/*! @brief PMBus Command PMB_VIN_UV_FAULT_LIMIT */
#define PMB_VIN_UV_FAULT_LIMIT              0x59
/*! @brief PMBus Command PMB_VIN_UV_FAULT_RESPONSE */
#define PMB_VIN_UV_FAULT_RESPONSE           0x5A
/*! @brief PMBus Command PMB_IIN_OC_FAULT_LIMIT */
#define PMB_IIN_OC_FAULT_LIMIT              0x5B
/*! @brief PMBus Command PMB_IIN_OC_FAULT_RESPONSE */
#define PMB_IIN_OC_FAULT_RESPONSE           0x5C
/*! @brief PMBus Command PMB_IIN_OC_WARN_LIMIT */
#define PMB_IIN_OC_WARN_LIMIT               0x5D
/*! @brief PMBus Command PMB_POWER_GOOD_ON */
#define PMB_POWER_GOOD_ON                   0x5E
/*! @brief PMBus Command PMB_POWER_GOOD_OFF */
#define PMB_POWER_GOOD_OFF                  0x5F
/*! @brief PMBus Command PMB_TON_DELAY */
#define PMB_TON_DELAY                       0x60
/*! @brief PMBus Command PMB_TON_RISE */
#define PMB_TON_RISE                        0x61
/*! @brief PMBus Command PMB_TON_MAX_FAULT_LIMIT */
#define PMB_TON_MAX_FAULT_LIMIT             0x62
/*! @brief PMBus Command PMB_TON_MAX_FAULT_RESPONSE */
#define PMB_TON_MAX_FAULT_RESPONSE          0x63
/*! @brief PMBus Command PMB_TOFF_DELAY */
#define PMB_TOFF_DELAY                      0x64
/*! @brief PMBus Command PMB_TOFF_FALL */
#define PMB_TOFF_FALL                       0x65
/*! @brief PMBus Command PMB_TOFF_MAX_WARN_LIMIT */
#define PMB_TOFF_MAX_WARN_LIMIT             0x66
/*! @brief PMBus Command PMB_POUT_OP_FAULT_LIMIT */
#define PMB_POUT_OP_FAULT_LIMIT             0x68
/*! @brief PMBus Command PMB_POUT_OP_FAULT_RESPONSE */
#define PMB_POUT_OP_FAULT_RESPONSE          0x69
/*! @brief PMBus Command PMB_POUT_OP_WARN_LIMIT */
#define PMB_POUT_OP_WARN_LIMIT              0x6A
/*! @brief PMBus Command PMB_PIN_OP_WARN_LIMIT */
#define PMB_PIN_OP_WARN_LIMIT               0x6B
/*! @brief PMBus Command PMB_STATUS_BYTE */
#define PMB_STATUS_BYTE                     0x78
/*! @brief PMBus Command PMB_STATUS_WORD */
#define PMB_STATUS_WORD                     0x79
/*! @brief PMBus Command PMB_STATUS_VOUT */
#define PMB_STATUS_VOUT                     0x7A
/*! @brief PMBus Command PMB_STATUS_IOUT */
#define PMB_STATUS_IOUT                     0x7B
/*! @brief PMBus Command PMB_STATUS_INPUT */
#define PMB_STATUS_INPUT                    0x7C
/*! @brief PMBus Command PMB_STATUS_TEMPERATURE */
#define PMB_STATUS_TEMPERATURE              0x7D
/*! @brief PMBus Command PMB_STATUS_CML */
#define PMB_STATUS_CML                      0x7E
/*! @brief PMBus Command PMB_STATUS_OTHER */
#define PMB_STATUS_OTHER                    0x7F
/*! @brief PMBus Command PMB_STATUS_MFR_SPECIFIC */
#define PMB_STATUS_MFR_SPECIFIC             0x80
/*! @brief PMBus Command PMB_STATUS_FANS_1_2 */
#define PMB_STATUS_FANS_1_2                 0x81
/*! @brief PMBus Command PMB_STATUS_FANS_3_4 */
#define PMB_STATUS_FANS_3_4                 0x82
/*! @brief PMBus Command PMB_READ_KWH_CONFIG */
#define PMB_READ_KWH_CONFIG                 0x85
/*! @brief PMBus Command PMB_READ_EIN */
#define PMB_READ_EIN                        0x86
/*! @brief PMBus Command PMB_READ_EOUT */
#define PMB_READ_EOUT                       0x87
/*! @brief PMBus Command PMB_READ_VIN */
#define PMB_READ_VIN                        0x88
/*! @brief PMBus Command PMB_READ_IIN */
#define PMB_READ_IIN                        0x89
/*! @brief PMBus Command PMB_READ_VCAP */
#define PMB_READ_VCAP                       0x8A
/*! @brief PMBus Command PMB_READ_VOUT */
#define PMB_READ_VOUT                       0x8B
/*! @brief PMBus Command PMB_READ_IOUT */
#define PMB_READ_IOUT                       0x8C
/*! @brief PMBus Command PMB_READ_TEMPERATURE_1 */
#define PMB_READ_TEMPERATURE_1              0x8D
/*! @brief PMBus Command PMB_READ_TEMPERATURE_2 */
#define PMB_READ_TEMPERATURE_2              0x8E
/*! @brief PMBus Command PMB_READ_TEMPERATURE_3 */
#define PMB_READ_TEMPERATURE_3              0x8F
/*! @brief PMBus Command PMB_READ_FAN_SPEED_1 */
#define PMB_READ_FAN_SPEED_1                0x90
/*! @brief PMBus Command PMB_READ_FAN_SPEED_2 */
#define PMB_READ_FAN_SPEED_2                0x91
/*! @brief PMBus Command PMB_READ_FAN_SPEED_3 */
#define PMB_READ_FAN_SPEED_3                0x92
/*! @brief PMBus Command PMB_READ_FAN_SPEED_4 */
#define PMB_READ_FAN_SPEED_4                0x93
/*! @brief PMBus Command PMB_READ_DUTY_CYCLE */
#define PMB_READ_DUTY_CYCLE                 0x94
/*! @brief PMBus Command PMB_READ_FREQUENCY */
#define PMB_READ_FREQUENCY                  0x95
/*! @brief PMBus Command PMB_READ_POUT */
#define PMB_READ_POUT                       0x96
/*! @brief PMBus Command PMB_READ_PIN */
#define PMB_READ_PIN                        0x97
/*! @brief PMBus Command PMB_PMBUS_REVISION */
#define PMB_PMBUS_REVISION                  0x98
/*! @brief PMBus Command PMB_MFR_ID */
#define PMB_MFR_ID                          0x99
/*! @brief PMBus Command PMB_MFR_MODEL */
#define PMB_MFR_MODEL                       0x9A
/*! @brief PMBus Command PMB_MFR_REVISION */
#define PMB_MFR_REVISION                    0x9B
/*! @brief PMBus Command PMB_MFR_LOCATION */
#define PMB_MFR_LOCATION                    0x9C
/*! @brief PMBus Command PMB_MFR_DATE */
#define PMB_MFR_DATE                        0x9D
/*! @brief PMBus Command PMB_MFR_SERIAL */
#define PMB_MFR_SERIAL                      0x9E
/*! @brief PMBus Command PMB_APP_PROFILE_SUPPORT */
#define PMB_APP_PROFILE_SUPPORT             0x9F
/*! @brief PMBus Command PMB_MFR_VIN_MIN */
#define PMB_MFR_VIN_MIN                     0xA0
/*! @brief PMBus Command PMB_MFR_VIN_MAX */
#define PMB_MFR_VIN_MAX                     0xA1
/*! @brief PMBus Command PMB_MFR_IIN_MAX */
#define PMB_MFR_IIN_MAX                     0xA2
/*! @brief PMBus Command PMB_MFR_PIN_MAX */
#define PMB_MFR_PIN_MAX                     0xA3
/*! @brief PMBus Command PMB_MFR_VOUT_MIN */
#define PMB_MFR_VOUT_MIN                    0xA4
/*! @brief PMBus Command PMB_MFR_VOUT_MAX */
#define PMB_MFR_VOUT_MAX                    0xA5
/*! @brief PMBus Command PMB_MFR_IOUT_MAX */
#define PMB_MFR_IOUT_MAX                    0xA6
/*! @brief PMBus Command PMB_MFR_POUT_MAX */
#define PMB_MFR_POUT_MAX                    0xA7
/*! @brief PMBus Command PMB_MFR_TAMBIENT_MAX */
#define PMB_MFR_TAMBIENT_MAX                0xA8
/*! @brief PMBus Command PMB_MFR_TAMBIENT_MIN */
#define PMB_MFR_TAMBIENT_MIN                0xA9
/*! @brief PMBus Command PMB_MFR_EFFICIENCY_LL */
#define PMB_MFR_EFFICIENCY_LL               0xAA
/*! @brief PMBus Command PMB_MFR_EFFICIENCY_HL */
#define PMB_MFR_EFFICIENCY_HL               0xAB
/*! @brief PMBus Command PMB_MFR_PIN_ACCURACY */
#define PMB_MFR_PIN_ACCURACY                0xAC
/*! @brief PMBus Command PMB_IC_DEVICE_ID */
#define PMB_IC_DEVICE_ID                    0xAD
/*! @brief PMBus Command PMB_IC_DEVICE_REV */
#define PMB_IC_DEVICE_REV                   0xAE
/*! @brief PMBus Command PMB_USER_DATA_00 */
#define PMB_USER_DATA_00                    0xB0
/*! @brief PMBus Command PMB_USER_DATA_01 */
#define PMB_USER_DATA_01                    0xB1
/*! @brief PMBus Command PMB_USER_DATA_02 */
#define PMB_USER_DATA_02                    0xB2
/*! @brief PMBus Command PMB_USER_DATA_03 */
#define PMB_USER_DATA_03                    0xB3
/*! @brief PMBus Command PMB_USER_DATA_04 */
#define PMB_USER_DATA_04                    0xB4
/*! @brief PMBus Command PMB_USER_DATA_05 */
#define PMB_USER_DATA_05                    0xB5
/*! @brief PMBus Command PMB_PMB_USER_DATA_06PAGE */
#define PMB_USER_DATA_06                    0xB6
/*! @brief PMBus Command PMB_USER_DATA_07 */
#define PMB_USER_DATA_07                    0xB7
/*! @brief PMBus Command PMB_USER_DATA_08 */
#define PMB_USER_DATA_08                    0xB8
/*! @brief PMBus Command PMB_USER_DATA_09 */
#define PMB_USER_DATA_09                    0xB9
/*! @brief PMBus Command PMB_USER_DATA_10 */
#define PMB_USER_DATA_10                    0xBA
/*! @brief PMBus Command PMB_USER_DATA_11 */
#define PMB_USER_DATA_11                    0xBB
/*! @brief PMBus Command PMB_USER_DATA_12 */
#define PMB_USER_DATA_12                    0xBC
/*! @brief PMBus Command PMB_USER_DATA_13 */
#define PMB_USER_DATA_13                    0xBD
/*! @brief PMBus Command PMB_USER_DATA_14 */
#define PMB_USER_DATA_14                    0xBE
/*! @brief PMBus Command PMB_USER_DATA_15 */
#define PMB_USER_DATA_15                    0xBF
/*! @brief PMBus Command PMB_MFR_MAX_TEMP_1 */
#define PMB_MFR_MAX_TEMP_1                  0xC0
/*! @brief PMBus Command PMB_PMB_MFR_MAX_TEMP_2PAGE */
#define PMB_MFR_MAX_TEMP_2                  0xC1
/*! @brief PMBus Command PMB_MFR_MAX_TEMP_3 */
#define PMB_MFR_MAX_TEMP_3                  0xC2

/*! @brief PMBus Command PMB_PROCESS_CALL_WORD */
#define PMB_PROCESS_CALL_WORD               0xD0
/*! @brief PMBus Command PMB_PROCESS_CALL_BLOCK */
#define PMB_PROCESS_CALL_BLOCK              0xD1
/*! @brief PMBus Command MFR_SPECIFIC_COMMAND_EXT */
#define MFR_SPECIFIC_COMMAND_EXT            0xFE
/*! @brief PMBus Command PMBUS_COMMAND_EXT */
#define PMBUS_COMMAND_EXT                   0xFF
/*! @brief PMBus Command PMB_QUICK_COMMAND */
#define PMB_QUICK_COMMAND                   0xFD

//*****************************************************************************
//
//! Return value when successful
//
//*****************************************************************************
#define PMBUS_RET_OK                            (1)

//*****************************************************************************
//
//! Return value when an error ocurred
//
//*****************************************************************************
#define PMBUS_RET_ERROR                         (-1)

//*****************************************************************************
//
//! Max packet size == SMBus max packet size
//
//*****************************************************************************
#define PMB_MAX_PACKET_SIZE                     SMB_MAX_PACKET_SIZE

//*****************************************************************************
//
//! Alert Response Address
//
//*****************************************************************************
#define PMBUS_ARA                               (0x0C)

/*! @enum PMBUS_GRP_CMD */
typedef enum
{
    /*! @brief PMBUS_GRP_CMD_DISABLE */
    PMBUS_GRP_CMD_DISABLE = 0,
    /*! @brief PMBUS_GRP_CMD_ENABLE */
    PMBUS_GRP_CMD_ENABLE,
    /*! @brief PMBUS_GRP_CMD_LAST_TARGET */
    PMBUS_GRP_CMD_LAST_TARGET
}PMBUS_GRP_CMD;

//*****************************************************************************
//
//! \brief   Initialize the PMBus Interface
//
//! \param SMB       Pointer to SMBus structure
//! \param i2cAddr   Base address of I2C module. For MSP430G2xxx devices,
//!                  this parameter is ignored.
//! \param busClk    SMCLK Frequency
//
// \return  None
//
//*****************************************************************************
extern void PMBus_controllerInit(SMBus *SMB,
                                 I2C_Regs* i2cAddr,
                                 uint32_t busClk);

//*****************************************************************************
//
//! \brief   Initialize the PMBus Interface
//
//! \param SMB       Pointer to SMBus structure
//! \param i2cAddr     Base address of I2C module. For MSP430G2xxx devices,
//!                    this parameter is ignored.
//! \param targetAddr  Target Address
//
// \return  None
//
//*****************************************************************************
extern void PMBus_targetInit(SMBus *SMB, I2C_Regs* i2cAddr, uint8_t targetAddr);

//*****************************************************************************
//
//! \brief   I2C Interrupt Service routine (PMBus Controller)
//
//! \param SMB       Pointer to SMBus structure
//! Handles the interrupts for PMBus controller . Should be called by
//! application when USCI interrupt is detected
//
//! \return  None
//
//*****************************************************************************
extern void PMBus_controllerProcessInt(SMBus *SMB);

//*****************************************************************************
//
//! \brief   I2C Interrupt Service routine (PMBus Target)
//
//! \param SMB       Pointer to SMBus structure
//! Handles the interrupts for PMBus target. Should be called by application
//! when USCI interrupt is detected
//
//! \return  None
//
//*****************************************************************************
extern SMBus_State PMBus_targetProcessInt(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Enables PEC support
//
//! \param SMB       Pointer to SMBus structure
//! \return  None
//
//*****************************************************************************
extern void PMBus_enablePEC(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Disables PEC support
//
//! \param SMB       Pointer to SMBus structure
//! \return  None
//
//*****************************************************************************
extern void PMBus_disablePEC(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Return the current command received by the target
//
//! \param SMB       Pointer to SMBus structure
//
//! \return  Current command
//
//*****************************************************************************
extern uint16_t PMBus_getTargetCommand(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Return the extended command received by the target
//
//! \param SMB       Pointer to SMBus structure
//
//! \return  extended command
//
//*****************************************************************************
extern uint16_t PMBus_getTargetExtendedCommand(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Clears the current state of PMBus
//
//!  Must be called by application in order to clear the state machine
//!  when a byte/packet was processed
//
//! \param SMB     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void PMBus_processDone(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Report to hardware that the command type is a block command
//
//! \param SMB       Pointer to SMBus structure
//
//*****************************************************************************
extern void PMBus_targetReportBlock(SMBus *SMB);

//*****************************************************************************
//
//! \brief   Report to hardware that the command type is a block command
//
//! \param SMB       Pointer to SMBus structure
//! \param length      Length of payload (without PEC byte), max supported is 0xFF
//
//*****************************************************************************
extern void PMBus_targetReportLength(SMBus *SMB, uint16_t length);

//*****************************************************************************
//
//! \brief   Perform a PMBus read for the given PMBus command id
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandId    One of the PMBus command id's defined in pmbus.h
//! \param rxData       RX data buffer
//! \param rxSize       Number of bytes received
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdRead(SMBus *SMB,
                            uint8_t targetAddress,
                            uint8_t commandId,
                            uint8_t* rxData,
                            uint8_t* rxSize,
                            uint32_t timeout);

//*****************************************************************************
//
//! \brief   Perform a PMBus write for the given PMBus command id
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandId    One of the PMBus command id's defined in pmbus.h
//! \param txData       TX data buffer
//! \param txSize       Number of bytes to be transferred
//! \param groupCmd     Enable or disable the group command
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdWrite(SMBus *SMB,
                             uint8_t targetAddress,
                             uint8_t commandId,
                             uint8_t* txData,
                             uint8_t txSize,
                             PMBUS_GRP_CMD groupCmd,
                             uint32_t timeout);

//*****************************************************************************
//
//! \brief   Sends byte to the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param write        write a quick command
//! \param groupCmd     Enable or disable the group command
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_QuickCommand(SMBus *SMB,
                                 uint8_t targetAddress,
                                 bool write,
                                 PMBUS_GRP_CMD groupCmd,
                                 uint32_t timeout);

//*****************************************************************************
//
//! \brief   Sends byte to the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param txData       TX data buffer
//! \param groupCmd     Enable or disable the group command
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdSendByte(SMBus *SMB,
                                uint8_t targetAddress,
                                uint8_t txData,
                                PMBUS_GRP_CMD groupCmd,
                                uint32_t timeout);

//*****************************************************************************
//
//! \brief   Writes a data byte to the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte   One of the PMBus command id's defined in pmbus.h
//! \param txData       TX data buffer
//! \param txSize       Number of bytes to transfer
//! \param groupCmd     Enable or disable the group command
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdWriteByte(SMBus *SMB,
                                 uint8_t targetAddress,
                                 uint8_t commandByte,
                                 uint8_t *txData,
                                 uint8_t txSize,
                                 PMBUS_GRP_CMD groupCmd,
                                 uint32_t timeout);

//*****************************************************************************
//
//! \brief   Writes a word to the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte   One of the PMBus command id's defined in pmbus.h
//! \param txData       TX data buffer
//! \param txSize       Number of bytes to transfer
//! \param groupCmd     Enable or disable the group command
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdWriteWord(SMBus *SMB,
                                 uint8_t targetAddress,
                                 uint8_t commandByte,
                                 uint8_t *txData,
                                 uint8_t txSize,
                                 PMBUS_GRP_CMD groupCmd,
                                 uint32_t timeout);

//*****************************************************************************
//
//! \brief   Writes a data block the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte   One of the PMBus command id's defined in pmbus.h
//! \param txData       TX data buffer
//! \param txSize       Number of bytes to transfer
//! \param groupCmd     Enable or disable the group command
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdBlockWrite(SMBus *SMB,
                                  uint8_t targetAddress,
                                  uint8_t commandByte,
                                  uint8_t *txData,
                                  uint16_t txSize,
                                  PMBUS_GRP_CMD groupCmd,
                                  uint32_t timeout);

//*****************************************************************************
//
//! \brief   Receives a data byte from the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param rxData       RX data buffer
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdReceiveByte(SMBus *SMB,
                                   uint8_t targetAddress,
                                   uint8_t* rxData,
                                   uint32_t timeout);

//*****************************************************************************
//
//! \brief   Reads a data byte from the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandId    One of the PMBus command id's defined in pmbus.h
//! \param rxData       RX data buffer
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdReadByte(SMBus *SMB,
                                uint8_t targetAddress,
                                uint8_t commandId,
                                uint8_t* rxData,
                                uint32_t timeout);

//*****************************************************************************
//
//! \brief   Reads a data word from the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandId    One of the PMBus command id's defined in pmbus.h
//! \param rxData       RX data buffer
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdReadWord(SMBus *SMB,
                                uint8_t targetAddress,
                                uint8_t commandId,
                                uint8_t* rxData,
                                uint32_t timeout);

//*****************************************************************************
//
//! \brief   Reads a block of data from the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandId    One of the PMBus command id's defined in pmbus.h
//! \param rxData       RX data buffer
//! \param rxSize       Number of bytes received
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdBlockRead(SMBus *SMB,
                                 uint8_t targetAddress,
                                 uint8_t commandId,
                                 uint8_t* rxData,
                                 uint8_t* rxSize,
                                 uint32_t timeout);

//*****************************************************************************
//
//! \brief   Sends a Process call command to the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte   One of the PMBus command id's defined in pmbus.h
//! \param txData       TX data buffer
//! \param rxData       RX data buffer
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdProcessCall(SMBus *SMB,
                                   uint8_t targetAddress,
                                   uint8_t commandByte,
                                   uint8_t* txData,
                                   uint8_t* rxData,
                                   uint32_t timeout);

//*****************************************************************************
//
//! \brief   Sends a Block-Write, Block-Read, Process Call to the PMBus target
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandId    One of the PMBus command id's defined in pmbus.h
//! \param txData       TX data buffer
//! \param txSize       Number of bytes to transfer
//! \param rxData       RX data buffer
//! \param rxSize       Number of bytes received
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdBlockWriteBlockReadProcessCall(SMBus *SMB,
                                                      uint8_t targetAddress,
                                                      uint8_t commandId,
                                                      uint8_t* txData,
                                                      uint8_t txSize,
                                                      uint8_t* rxData,
                                                      uint8_t* rxSize,
                                                      uint32_t timeout);

//*****************************************************************************
//
//! \brief   Executes a group command
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddr   Ptr to Target addresses
//! \param command      Ptr to PMBus command id's defined in pmbus.h
//! \param txData        Ptr to TX data buffers
//! \param txSize        Ptr to Number of bytes to transfer
//! \param length        Number of targets
//! \param timeout       Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdGroupCommand(SMBus *SMB,
                                    uint8_t *targetAddr,
                                    uint8_t *command,
                                    uint8_t **txData,
                                    uint8_t *txSize,
                                    uint8_t length,
                                    uint32_t timeout);

//*****************************************************************************
//
//! \brief   Starts a Tx transfer
//
//! \param SMB          Pointer to SMBus structure
//! \param groupCmd     group command status
//
//*****************************************************************************
extern void PMBus_startTxTransfer(SMBus *SMB, PMBUS_GRP_CMD groupCmd);

//*****************************************************************************
//
//! \brief   Writes a data byte to the PMBus target with extended command
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte1  extended command 0xFF or 0xFE
//! \param commandByte2  One of the PMBus command id's defined in pmbus.h
//! \param txData        TX data buffer
//! \param txSize        Number of bytes to transfer
//! \param timeout       Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdExtendedWriteByte(SMBus *SMB,
                                         uint8_t targetAddress,
                                         uint8_t commandByte1,
                                         uint8_t commandByte2,
                                         uint8_t *txData,
                                         uint8_t txSize,
                                         uint32_t timeout);

//*****************************************************************************
//
//! \brief   Writes a data word to the PMBus target with extended command
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte1  extended command 0xFF or 0xFE
//! \param commandbyte2  One of the PMBus command id's defined in pmbus.h
//! \param txData        TX data buffer
//! \param txSize        Number of bytes to transfer
//! \param timeout       Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdExtendedWriteWord(SMBus *SMB,
                                         uint8_t targetAddress,
                                         uint8_t commandByte1,
                                         uint8_t commandByte2,
                                         uint8_t *txData,
                                         uint8_t txSize,
                                         uint32_t timeout);

//*****************************************************************************
//
//! \brief   Reads a data byte to the PMBus target with extended command
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte1  extended command 0xFF or 0xFE
//! \param commandbyte2  One of the PMBus command id's defined in pmbus.h
//! \param rxData        TX data buffer
//! \param rxSize        Number of bytes to transfer
//! \param timeout       Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdExtendedReadByte(SMBus *SMB,
                                        uint8_t targetAddress,
                                        uint8_t commandByte1,
                                        uint8_t commandByte2,
                                        uint8_t* rxData,
                                        uint8_t rxSize,
                                        uint32_t timeout);

//*****************************************************************************
//
//! \brief   Reads a data word to the PMBus target with extended command
//
//! \param SMB           Pointer to SMBus structure
//! \param targetAddress Target address
//! \param commandByte1  extended command 0xFF or 0xFE
//! \param commandbyte2  One of the PMBus command id's defined in pmbus.h
//! \param rxData        TX data buffer
//! \param rxSize        Number of bytes to transfer
//! \param timeout       Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_cmdExtendedReadWord(SMBus *SMB,
                                        uint8_t targetAddress,
                                        uint8_t commandByte1,
                                        uint8_t commandByte2,
                                        uint8_t* rxData,
                                        uint8_t rxSize,
                                        uint32_t timeout);

//*****************************************************************************
//
//! \brief   Sends a Receive Byte to Alert Response Address to request
//
//! \param SMB          Pointer to SMBus structure
//! \param rxData       RX buffer returning address of device invoking the alert
//! \param timeout      Software timeout waiting for a response
//
//! \return  \b PMBUS_RET_ERROR, or \b PMBUS_RET_OK
//
//*****************************************************************************
extern int8_t PMBus_ReceiveByteARA(SMBus *SMB,
                                   uint8_t *rxData,
                                   uint32_t timeout);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif //__PMBUS_H__
