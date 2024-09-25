/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#ifndef __BSL_CI_H__
#define __BSL_CI_H__

#include "ti_msp_dl_config.h"

/* BSL version information */

#define BSL_VERSION_BUILD_ID ((uint16_t) 0x0001)
#define BSL_VERSION_CMD_INTERPRETER ((uint16_t) 0x0001)

/*
 * BSL protocol defines
 */

/* BSL Commands */
#define CMD_CONNECTION ((uint8_t) 0x12)
#define CMD_UNLOCK_BSL ((uint8_t) 0x21)
#define CMD_FLASH_RANGE_ERASE ((uint8_t) 0x23)
#define CMD_MASS_ERASE ((uint8_t) 0x15)
#define CMD_PROGRAM_DATA ((uint8_t) 0x20)
#define CMD_PROGRAM_DATA_FAST ((uint8_t) 0x24)
#define CMD_MEMORY_READ_BACK ((uint8_t) 0x29)
#define CMD_FACTORY_RESET ((uint8_t) 0x30)
#define CMD_GET_IDENTITY ((uint8_t) 0x19)
#define CMD_STANDALONE_VERIFICATION ((uint8_t) 0x26)
#define CMD_START_APPLICATION ((uint8_t) 0x40)

/* BSL Responses */
#define BSL_MEMORY_READ_BACK ((uint8_t) 0x30)
#define GET_IDENTITY ((uint8_t) 0X31)
#define STANDALONE_VERIFICATION ((uint8_t) 0X32)
#define BSL_MESSAGE_REPLY ((uint8_t) 0x3B)

/* BSL Core Message responses */
#define BSL_SUCCESSFUL_OPERATION ((uint8_t) 0x00)
#define BSL_LOCKED ((uint8_t) 0x01)
#define BSL_PASSWORD_ERROR ((uint8_t) 0x02)
#define BSL_MULTIPLE_PASSWORD_ERROR ((uint8_t) 0x03)
#define BSL_UNKNOWN_COMMAND ((uint8_t) 0x04)
#define BSL_INVALID_MEMORY_RANGE ((uint8_t) 0x05)
#define BSL_INVALID_COMMAND ((uint8_t) 0x06)
#define BSL_FACTORY_RESET_DISABLED ((uint8_t) 0x07)
#define BSL_FACTORY_RESET_PASSWORD_ERROR ((uint8_t) 0x08)
#define BSL_READOUT_DISABLED ((uint8_t) 0x09)
#define BSL_INVALID_ADDR_LEN_ALIGNMENT ((uint8_t) 0x0A)
#define BSL_STANDALONE_VERIFICATION_INVALID_LEN ((uint8_t) 0x0B)
#define BSL_FLASH_PROGRAM_FAILED ((uint8_t) 0xF1)
#define BSL_MASS_ERASE_FAIL ((uint8_t) 0xF2)
#define BSL_FLASH_ERASE_FAILED ((uint8_t) 0xF3)
#define BSL_FACTORY_RESET_FAILED ((uint8_t) 0xF4)

/*
 * Packet structure defines
 */
/* Header */
#define PACKET_HEADER_UART_I2C_SPI ((uint8_t) 0x80)
#define PACKET_HEADER_RESPONSE ((uint8_t) 0x08)

/* Packet element's index */
#define HEADER_INDEX ((uint16_t) 0U)
#define LEN_BYTE_L_INDEX ((uint16_t) 1U)
#define LEN_BYTE_H_INDEX ((uint16_t) 2U)
#define CMD_INDEX ((uint16_t) 3U)
#define START_ADDRESS_INDEX ((uint16_t) 4U)
#define END_ADDRESS_INDEX ((uint16_t) 8U)
#define DATA_LENGTH_INDEX ((uint16_t) 8U)
#define PASSWORD_INDEX ((uint16_t) 4U)

#define MESSAGE_INDEX ((uint16_t) 4U)
#define VERIFICATION_CRC_INDEX ((uint16_t) 4U)

/* Packet Lengths */

/*
 * Packet Header length includes the PI wrapper bytes length
 * paced at the beginning of the packet
 * It includes "Header (1 Byte) and Length (2 Bytes)"
 */
#define PACKET_HEADER_LEN ((uint16_t) 3U)

#define UNLOCK_BSL_CMD_LEN ((uint16_t) 0x21U)
#define FACTORY_RESET_CMD_LEN ((uint16_t) 0x11U)

#define GET_IDENTITY_RSP_LEN ((uint8_t) 0x19)
#define MESSAGE_RSP_LEN ((uint8_t) 0x02)
#define STANDALONE_VERIFY_RSP_LEN ((uint8_t) 0x05)

#define BSL_PASSWORD_LEN ((uint8_t) 8U)

/* Flash Addresses */
#define MSPM0_MAIN_FLASH_START_ADDRESS (0x24800)
#define MSPM0_MAIN_FLASH_END_ADDRESS (0x27000)

/*
 * Maximum possible Memory that can be used as Data buffer for
 * BSL communication
 */
#define BSL_MAX_BUF_SIZE_ALLOWED (0xFFFFU)

/*
 * BSL lock status defines
 */
#define BSL_STATUS_LOCKED ((uint8_t) 0x01)
#define BSL_STATUS_UNLOCKED ((uint8_t) 0xA5)

/*
 * @brief   Defining the Plugin interface
 *
 * This structure acts as the bridge between the BSL core operations and the
 * Interface operations.
 */
typedef struct {
    /* Plugin interface type, if UART, I2C, SPI, etc.. */
    uint32_t plugin_type;

    /* Init function initializes the interface and data buffer */
    uint16_t (*init)(uint8_t* buffer, uint16_t size);

    /* Receive function handles packet reception from BSL Host */
    uint32_t (*receive)(void);

    /* Send function transmits the BSL core response to the Host */
    uint8_t (*send)(uint8_t* data, uint16_t len);

    /* Deinit disables the Interface */
    bool (*deinit)(void);
} BSLPluginInfo;

static inline uint32_t BSL_CI_getRAMEndAddress(void)
{
    return 0x20208000;
}

static inline uint32_t BSL_CI_getFlashEndAddress(void)
{
    return MSPM0_MAIN_FLASH_END_ADDRESS;
}

/**
 * @brief       BSL Command Interpreter
 *
 * Interprets the command contained in the data buffer and
 * branches to the appropriate BSL API
 *
 * @return      None
 */
void BSL_CI_interpretCommand(void);

#endif  // __BSL_CI_H__
