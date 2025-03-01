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

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include "ti_msp_dl_config.h"

/* UART LIN responder value for the number of messages */
#define LIN_RESPONDER_NUM_MSGS (0x07)

/* Max data buffer size for LIN RX and TX */
#define LIN_DATA_MAX_BUFFER_SIZE (8)

/* UART LIN value for the number of cycles in a sync validation */
#define LIN_RESPONDER_SYNC_CYCLES (5)

/* UART LIN value for the message not found */
#define LIN_MESSAGE_NOT_FOUND (0xFF)

/* UART LIN value for the sync byte */
#define LIN_SYNC_BYTE (0x55)

/* Number of delay cycles between PID STOP bit and data transmission START bit*/
#define LIN_RESPONSE_LAPSE (LIN_0_INST_FREQUENCY / (2 * LIN_0_BAUD_RATE))

/*Defaults the system to enable the automatic baud rate synchronization feature*/
#define AUTO_BAUD_ENABLED (true)

/*Number of cycles in a sync validation that must fail to utilize the automatic baud rate synchronization feature*/
#define AUTO_BAUD_THRESHOLD (4)

/*Number of clock cycles to delay when configuring the new baud rate*/
#define AUTO_BAUD_CONFIG_DELAY (10)

/*! @enum LIN_RX_STATE */
typedef enum {
    /*! UART LIN RX state for the ID byte of the RX packet */
    LIN_RX_STATE_ID = 0,
    /*! UART LIN RX state for the data bytes of the RX packet */
    LIN_RX_STATE_DATA = 1,
    /*! UART LIN RX state for the checksum byte of the RX packet */
    LIN_RX_STATE_CHECKSUM = 2
} LIN_RX_STATE;

/*! @enum LIN_STATE */
typedef enum {
    /*! UART LIN state of waiting for the break field */
    LIN_STATE_WAIT_FOR_BREAK = 0,
    /*! UART LIN state of entering the break field */
    LIN_STATE_BREAK_FIELD = 1,
    /*! UART LIN state of waiting for the negative edge of the sync field */
    LIN_STATE_SYNC_FIELD_NEG_EDGE = 2,
    /*! UART LIN state of waiting for the positive edge of the sync field */
    LIN_STATE_SYNC_FIELD_POS_EDGE = 3,
    /*! UART LIN state of waiting for the PID byte */
    LIN_STATE_PID_FIELD = 4,
    /*! UART LIN state of waiting for the data bytes */
    LIN_STATE_DATA_FIELD = 5
} LIN_STATE;

/*!
 *  @brief  Union of word and bytes to form word_t
 */
typedef union {
    /*! The word value */
    uint16_t word;

    /*! The byte array */
    uint8_t byte[2];
} LIN_word_t;

/*!
 *  @brief  LIN callback function type
 */
typedef void (*LIN_function_ptr_t)(void);

/*!
 *  @brief  LIN table record struct
 */
typedef struct {
    /*! The PID of the message */
    uint8_t msgID;

    /*! The number of bytes in the data field */
    uint8_t msgSize;

    /*! The callback function of the specified PID */
    LIN_function_ptr_t callbackFunction;
} LIN_table_record_t;

/*!
 *  @brief  LIN sync bit struct
 */
typedef struct {
    /*! The counter value of the negative edge */
    uint16_t negEdge;

    /*! The counter value of the positive edge */
    uint16_t posEdge;
} LIN_Sync_Bits;

void sendLINResponderTXMessage(UART_Regs *uart, uint8_t tableIndex,
    uint8_t *msgBuffer, LIN_table_record_t *responderMessageTable);
