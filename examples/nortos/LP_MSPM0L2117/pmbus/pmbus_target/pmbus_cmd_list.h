/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

#ifndef __PMBUS_CMD_LIST_H__
#define __PMBUS_CMD_LIST_H__

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
#include "ti/smbus/smbus.h"


/*
 * PMBus 1.2 Standard Command Codes
 */
/* Commented codes are not supported in this example    */

//#define PMB_PAGE                            0x00
//#define PMB_OPERATION                       0x01
//#define PMB_ON_OFF_CONFIG                   0x02
#define PMB_CLEAR_FAULTS                    0x03
//#define PMB_PHASE                           0x04
//#define PMB_PAGE_PLUS_WRITE                 0x05
//#define PMB_PAGE_PLUS_READ                  0x06
//#define PMB_ZONE_CONFIG                     0x07
//#define PMB_ZONE_ACTIVE                     0x08
//#define PMB_P2_PLUS_WRITE                   0x09
//#define PMB_P2_PLUS_READ                    0x0A
//#define PMB_WRITE_PROTECT                   0x10
//#define PMB_STORE_DEFAULT_ALL               0x11
//#define PMB_RESTORE_DEFAULT_ALL             0x12
//#define PMB_STORE_DEFAULT_CODE              0x13
//#define PMB_RESTORE_DEFAULT_CODE            0x14
//#define PMB_STORE_USER_ALL                  0x15
//#define PMB_RESTORE_USER_ALL                0x16
//#define PMB_STORE_USER_CODE                 0x17
//#define PMB_RESTORE_USER_CODE               0x18
//#define PMB_CAPABILITY                      0x19
//#define PMB_QUERY                           0x1A
//#define PMB_SMBALERT_MASK                   0x1B
#define PMB_VOUT_MODE                       0x20
//#define PMB_VOUT_COMMAND                    0x21
//#define PMB_VOUT_TRIM                       0x22
//#define PMB_VOUT_CAL_OFFSET                 0x23
//#define PMB_VOUT_MAX                        0x24
//#define PMB_VOUT_MARGIN_HIGH                0x25
//#define PMB_VOUT_MARGIN_LOW                 0x26
//#define PMB_VOUT_TRANSITION_RATE            0x27
//#define PMB_VOUT_DROOP                      0x28
//#define PMB_VOUT_SCALE_LOOP                 0x29
//#define PMB_VOUT_SCALE_MONITOR              0x2A
//#define PMB_VOUT_MIN                        0x2B
//#define PMB_COEFFICIENTS                    0x30
//#define PMB_POUT_MAX                        0x31
//#define PMB_MAX_DUTY                        0x32
//#define PMB_FREQUENCY_SWITCH                0x33
//#define PMB_POWER_MODE                      0x34
//#define PMB_VIN_ON                          0x35
//#define PMB_VIN_OFF                         0x36
//#define PMB_INTERLEAVE                      0x37
//#define PMB_IOUT_CAL_GAIN                   0x38
//#define PMB_IOUT_CAL_OFFSET                 0x39
//#define PMB_FAN_CONFIG_1_2                  0x3A
//#define PMB_FAN_COMMAND_1                   0x3B
//#define PMB_FAN_COMMAND_2                   0x3C
//#define PMB_FAN_CONFIG_3_4                  0x3D
//#define PMB_FAN_COMMAND_3                   0x3E
//#define PMB_FAN_COMMAND_4                   0x3F
//#define PMB_VOUT_OV_FAULT_LIMIT             0x40
//#define PMB_VOUT_OV_FAULT_RESPONSE          0x41
#define PMB_VOUT_OV_WARN_LIMIT              0x42
//#define PMB_VOUT_UV_WARN_LIMIT              0x43
//#define PMB_VOUT_UV_FAULT_LIMIT             0x44
//#define PMB_VOUT_UV_FAULT_RESPONSE          0x45
//#define PMB_IOUT_OC_FAULT_LIMIT             0x46
//#define PMB_IOUT_OC_FAULT_RESPONSE          0x47
//#define PMB_IOUT_OC_LV_FAULT_LIMIT          0x48
//#define PMB_IOUT_OC_LV_FAULT_RESPONSE       0x49
//#define PMB_IOUT_OC_WARN_LIMIT              0x4A
//#define PMB_IOUT_UC_FAULT_LIMIT             0x4B
//#define PMB_IOUT_UC_FAULT_RESPONSE          0x4C
//#define PMB_OT_FAULT_LIMIT                  0x4F
//#define PMB_OT_FAULT_RESPONSE               0x50
//#define PMB_OT_WARN_LIMIT                   0x51
//#define PMB_UT_WARN_LIMIT                   0x52
//#define PMB_UT_FAULT_LIMIT                  0x53
//#define PMB_UT_FAULT_RESPONSE               0x54
//#define PMB_VIN_OV_FAULT_LIMIT              0x55
//#define PMB_VIN_OV_FAULT_RESPONSE           0x56
//#define PMB_VIN_OV_WARN_LIMIT               0x57
//#define PMB_VIN_UV_WARN_LIMIT               0x58
//#define PMB_VIN_UV_FAULT_LIMIT              0x59
//#define PMB_VIN_UV_FAULT_RESPONSE           0x5A
//#define PMB_IIN_OC_FAULT_LIMIT              0x5B
//#define PMB_IIN_OC_FAULT_RESPONSE           0x5C
//#define PMB_IIN_OC_WARN_LIMIT               0x5D
//#define PMB_POWER_GOOD_ON                   0x5E
//#define PMB_POWER_GOOD_OFF                  0x5F
//#define PMB_TON_DELAY                       0x60
//#define PMB_TON_RISE                        0x61
//#define PMB_TON_MAX_FAULT_LIMIT             0x62
//#define PMB_TON_MAX_FAULT_RESPONSE          0x63
//#define PMB_TOFF_DELAY                      0x64
//#define PMB_TOFF_FALL                       0x65
//#define PMB_TOFF_MAX_WARN_LIMIT             0x66
//#define PMB_POUT_OP_FAULT_LIMIT             0x68
//#define PMB_POUT_OP_FAULT_RESPONSE          0x69
//#define PMB_POUT_OP_WARN_LIMIT              0x6A
//#define PMB_PIN_OP_WARN_LIMIT               0x6B
//#define PMB_STATUS_BYTE                     0x78
//#define PMB_STATUS_WORD                     0x79
//#define PMB_STATUS_VOUT                     0x7A
//#define PMB_STATUS_IOUT                     0x7B
//#define PMB_STATUS_INPUT                    0x7C
//#define PMB_STATUS_TEMPERATURE              0x7D
//#define PMB_STATUS_CML                      0x7E
//#define PMB_STATUS_OTHER                    0x7F
//#define PMB_STATUS_MFR_SPECIFIC             0x80
//#define PMB_STATUS_FANS_1_2                 0x81
//#define PMB_STATUS_FANS_3_4                 0x82
//#define PMB_READ_KWH_IN                     0x83
//#define PMB_READ_KWH_OUT                    0x84
//#define PMB_READ_KWH_CONFIG                 0x85
//#define PMB_READ_EIN                        0x86
//#define PMB_READ_EOUT                       0x87
//#define PMB_READ_VIN                        0x88
//#define PMB_READ_IIN                        0x89
//#define PMB_READ_VCAP                       0x8A
#define PMB_READ_VOUT                       0x8B
//#define PMB_READ_IOUT                       0x8C
//#define PMB_READ_TEMPERATURE_1              0x8D
//#define PMB_READ_TEMPERATURE_2              0x8E
//#define PMB_READ_TEMPERATURE_3              0x8F
//#define PMB_READ_FAN_SPEED_1                0x90
//#define PMB_READ_FAN_SPEED_2                0x91
//#define PMB_READ_FAN_SPEED_3                0x92
//#define PMB_READ_FAN_SPEED_4                0x93
//#define PMB_READ_DUTY_CYCLE                 0x94
//#define PMB_READ_FREQUENCY                  0x95
//#define PMB_READ_POUT                       0x96
//#define PMB_READ_PIN                        0x97
//#define PMB_PMBUS_REVISION                  0x98
#define PMB_MFR_ID                          0x99
//#define PMB_MFR_MODEL                       0x9A
//#define PMB_MFR_REVISION                    0x9B
//#define PMB_MFR_LOCATION                    0x9C
//#define PMB_MFR_DATE                        0x9D
//#define PMB_MFR_SERIAL                      0x9E
//#define PMB_APP_PROFILE_SUPPORT             0x9F
//#define PMB_MFR_VIN_MIN                     0xA0
//#define PMB_MFR_VIN_MAX                     0xA1
//#define PMB_MFR_IIN_MAX                     0xA2
//#define PMB_MFR_PIN_MAX                     0xA3
//#define PMB_MFR_VOUT_MIN                    0xA4
//#define PMB_MFR_VOUT_MAX                    0xA5
//#define PMB_MFR_IOUT_MAX                    0xA6
//#define PMB_MFR_POUT_MAX                    0xA7
//#define PMB_MFR_TAMBIENT_MAX                0xA8
//#define PMB_MFR_TAMBIENT_MIN                0xA9
//#define PMB_MFR_EFFICIENCY_LL               0xAA
//#define PMB_MFR_EFFICIENCY_HL               0xAB
//#define PMB_MFR_PIN_ACCURACY                0xAC
#define PMB_IC_DEVICE_ID                    0xAD
//#define PMB_IC_DEVICE_REV                   0xAE
//#define PMB_MFR_STATUS_0                    0xB0
//#define PMB_MFR_STATUS_1                    0xB1
//#define PMB_MFR_STATUS_2                    0xB2
//#define PMB_MFR_STATUS_3                    0xB3
//#define PMB_MFR_STATUS_4                    0xB4
//#define PMB_BLACK_BOX_LOG_FAULT_FIFO        0xB5
//#define PMB_BLACK_BOX_LOG_RAILS_WARNING     0xB6
//#define PMB_BLACK_BOX_LOG_RAILS_VALUE       0xB7
//#define PMB_RAIL_PROFILE                    0xB8
//#define PMB_RAIL_STATE                      0xB9
//#define PMB_MFR_STATUS_5                    0xBA
//#define PMB_USER_DATA_11                    0xBB
//#define PMB_USER_DATA_12                    0xBC
//#define PMB_USER_DATA_13                    0xBD
//#define PMB_USER_DATA_14                    0xBE
//#define PMB_USER_DATA_15                    0xBF
//#define PMB_MFR_MAX_TEMP_1                  0xC0
//#define PMB_MFR_MAX_TEMP_2                  0xC1
//#define PMB_MFR_MAX_TEMP_3                  0xC2

/* Manufacturer specific commands   */
#define PMB_PROCESS_CALL_CMD                0xD0
#define PMB_BLOCK_PROCESS_CALL_CMD          0xD1
#define PMB_MFR_SPECIFIC_COMMAND_EXT        0xFE
#define PMB_PMBUS_COMMAND_EXT               0xFF

/* Extended Commands    */
#define PMB_EXTENDED_BYTE_COMMAND           0x01FF
#define PMB_EXTENDED_WORD_COMMAND           0x02FF

/*
 * PMBus command lengths in bytes
 */
/* SEND_BYTE only send the command code, making data payload length 0 */
#define PMBUS_SEND_BYTE_CMD_LENGTH_BYTES        0
#define PMBUS_RW_BYTE_CMD_LENGTH_BYTES          1
#define PMBUS_RW_WORD_CMD_LENGTH_BYTES          2
/* M0 SMBus library takes care of block length by examining data payload */
#define PMBUS_RW_BLOCK_CMD_LENGTH_BYTES         SMBUS_BLOCK_LENGTH

/*! Handler called when a command matches one in our list */
typedef int8_t (*cmdHandler)(SMBus *);

/*! PMBus command structure*/
typedef struct
{
    /*! Command address */
    uint16_t cmd;
    /*! Length of command payload (SMBUS_BLOCK_LENGTH if Block command) */
    uint16_t length;
    /*! Handler called if command is received */
    cmdHandler handler;
} PMBusCommand;

/* Global variables */
extern const PMBusCommand PMBus_cmdList[];

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //__PMBUS_CMD_LIST_H__
