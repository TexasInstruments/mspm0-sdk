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

#include "stdint.h"

#define BSL_INVOKE_DELAY (1000000)
#define DELAY_BUTTON_PRESS (2000)
#define DELAY_BSL_OP (50000)
#define DELAY_BSL_OP_LONG (500000)
#define DELAY_LED_TOGGLE (16000000)

#define BSL_CRC (CRC)
#define BSL_CRC_SEED ((uint32_t) 0xFFFFFFFFU)

#define MAX_PAYLOAD_DATA_SIZE (128)
//MAX_PACKET_SIZE = MAX_PAYLOAD_DATA_SIZE + HDR_LEN_CMD_BYTES + CRC_BYTES = 128 + 8 = 136
#define MAX_PACKET_SIZE (256)

//#define Hardware_Invoke
#define Software_Invoke  //This just work when the code "Application_demo_with_software_trigger_LP_MSPM0G3507_0_address" exist on the device

/* BSL Packet definitions */
#define BSL_PACKET_HEADER ((uint8_t) 0x80)

/* Sizes for Fields */
#define BSL_ACK_SIZE (1)
#define BSL_CMD_SIZE (1)
#define BSL_HED_SIZE (1)
#define BSL_LEN_SIZE (2)
#define BSL_HED_LEN_CMD_SIZE (4)
#define BSL_CRC_SIZE (4)
#define BSL_WRAPPER_SIZE (7)  // Header + Length + CRC
#define BSL_PASSWORD_SIZE (32)
#define BLS_DEVICE_INFO_SIZE (24)
#define BSL_ADDRS_SIZE (4)
#define BSL_BITRATE_CONFIG_SIZE (7)

// ! Define BSL CORE commands
#define NO_ACTIVE_COMMAND ((uint8_t) 0x00)
#define CMD_CONNECTION ((uint8_t) 0x12)
#define CMD_UNLOCK_BOOTLOADER ((uint8_t) 0x21)
#define CMD_FLASH_RANGE_ERASE ((uint8_t) 0x23)
#define CMD_MASS_ERASE ((uint8_t) 0x15)
#define CMD_PROGRAM_DATA ((uint8_t) 0x20)
#define CMD_PROGRAM_DATA_FAST ((uint8_t) 0x24)
#define CMD_MEMORY_READBACK ((uint8_t) 0x29)
#define CMD_FACTORY_RESET ((uint8_t) 0x30)
#define CMD_GET_DEVICE_INFO ((uint8_t) 0x19)
#define CMD_STANDALONE_VERIFICATION ((uint8_t) 0x26)
#define CMD_START_APPLICATION ((uint8_t) 0x40)
#define CMD_CHANGE_BAUDRATE ((uint8_t) 0x52)

/* BSL Acknowledgment responses */
#define BSL_ERROR_HEADER_INCORRECT ((uint8_t) 0x51)
#define BSL_ERROR_CHECKSUM_INCORRECT ((uint8_t) 0x52)
#define BSL_ERROR_PACKET_SIZE_ZERO ((uint8_t) 0x53)
#define BSL_ERROR_PACKET_SIZE_TOO_BIG ((uint8_t) 0x54)
#define BSL_ERROR_UNKNOWN_ERROR ((uint8_t) 0x55)
#define BSL_ERROR_UNKNOWN_BAUDRATE ((uint8_t) 0x56)
#define BSL_ACK ((uint8_t) 0x0)

/* BSL Core Responses */
#define BSL_RESP_HEADER ((uint8_t) 0x08)
#define BSL_RESP_MEMORY_READBACK ((uint8_t) 0x30)
#define BSL_RESP_GET_IDENTITY ((uint8_t) 0x31)
#define BSL_RESP_STANDALONE_VERIFICATION ((uint8_t) 0x32)
#define BSL_RESP_MESSAGE ((uint8_t) 0x3B)
#define BSL_RESP_DETAILED_ERROR ((uint8_t) 0x3A)

/* BSL Message Meaning */
#define BSL_OPERATION_SUCCESFUL ((uint8_t) 0x00)
#define BSL_LOCKED_ERROR ((uint8_t) 0x01)
#define BSL_PASSWORD_ERROR ((uint8_t) 0x02)
#define BSL_MULTIPLE_PASSWORD_ERROR ((uint8_t) 0x03)
#define BSL_UNKNOWN_COMMAND ((uint8_t) 0x04)
#define BSL_INVALID_MEMORY_RANGE ((uint8_t) 0x05)
#define BSL_INVALID_COMMAND ((uint8_t) 0x06)
#define BSL_FACTORY_RESET_DISABLED ((uint8_t) 0x07)
#define BSL_FACTORY_RESET_PASSWORD_ERROR ((uint8_t) 0x08)
#define BSL_READ_OUT_ERROR ((uint8_t) 0x09)
#define BSL_INVALID_ADDRESS_ALIGNMENT ((uint8_t) 0x0A)
#define BSL_INVALID_LENGTH_VERIFICATION (uint8_t) 0x0B)

#define CORE_INDEX 3

enum {
    /*! No Error Occurred! The operation was successful.*/
    eBSL_success = 0,

    /*! Flash write check failed. After programming, a CRC is run on the programmed data
     *! If the CRC does not match the expected result, this error is returned.*/
    eBSL_flashWriteCheckFailed = 1,

    /*! BSL locked.  The correct password has not yet been supplied to unlock the BSL.*/
    eBSL_locked = 4,

    /*! BSL password error. An incorrect password was supplied to the BSL when attempting an unlock.*/
    eBSL_passwordError = 5,

    /*! Unknown error.  The command given to the BSL was not recognized*/
    eBSL_unknownError = 7,

    eBSL_responseCommand = 0x3B

};
typedef uint8_t BSL_error_t;
/*BSL ERROR CODES*/
#define NO_RESPONSE_RECEIVED 0xE0;
#define CHECKSUM_FAIL 0xEC;

//================================================================================
// ! Conversion MACROS
#define LSB(x) (x & 0x00FF)
#define MSB(x) ((x & 0xFF00) >> 8)

void Host_BSL_entry_sequence(void);

void Host_BSL_software_trigger(void);

void TurnOnErrorLED(void);
extern volatile uint8_t BSL_TX_buffer[MAX_PACKET_SIZE + 2];
extern volatile uint8_t BSL_RX_buffer[MAX_PACKET_SIZE + 2];
extern volatile uint16_t BSL_maxBufferSize;
uint8_t Host_BSL_Connection(void);
uint8_t Host_BSL_GetID(void);
uint8_t Host_BSL_loadPassword(uint8_t* pPassword);
uint8_t Host_BSL_MassErase(void);
uint8_t Host_BSL_writeMemory(uint32_t addr, const uint8_t* data, uint32_t len);
uint8_t Host_BSL_StartApp(void);

uint32_t softwareCRC(const uint8_t* data, uint8_t length);
uint8_t Host_BSL_getResponse(void);
