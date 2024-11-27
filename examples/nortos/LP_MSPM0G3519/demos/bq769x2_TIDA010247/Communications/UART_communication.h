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

#ifndef UART_COMMUNICATION_H_
#define UART_COMMUNICATION_H_

#include "ti_msp_dl_config.h"

#define RS485_INST UART_0_INST
#define RS485_TX_ENABLE
#define RS485_TX_DISABLE
#define RS485_RX_ENABLE
#define RS485_RX_DISABLE

/* Maximum size of TX packet */
#define RS485_TX_MAX_PACKET_SIZE (34)

/* Number of bytes to send to target device */
#define RS485_TX_PACKET_SIZE (34)

/* Maximum size of RX packet */
#define RS485_RX_MAX_PACKET_SIZE RS485_TX_MAX_PACKET_SIZE

/* Number of bytes to received from target */
#define RS485_RX_PACKET_SIZE RS485_TX_PACKET_SIZE

/* Indicates status of RS485 */
typedef enum {
    RS485_STATUS_IDLE = 0,
    RS485_STATUS_TX_STARTED,
    RS485_STATUS_TX_INPROGRESS,
    RS485_STATUS_TX_COMPLETE,
    RS485_STATUS_RX_STARTED,
    RS485_STATUS_RX_INPROGRESS,
    RS485_STATUS_RX_COMPLETE,
    RS485_STATUS_ERROR
} RS485Status_t;

/* RS485 structure */
typedef struct {
    uint8_t txPacket[RS485_TX_MAX_PACKET_SIZE];
    uint32_t txLen;
    volatile uint32_t txCount;
    uint8_t rxPacket[RS485_RX_MAX_PACKET_SIZE];
    uint32_t rxLen;
    volatile uint32_t rxCount;
    volatile RS485Status_t status;
} RS485_t;

extern void RS485_Send(
    uint8_t BQid, uint8_t cmd, uint16_t *data, uint32_t length);
extern void RS485_Receive(uint8_t *RS485_data, uint32_t RS485_count);

#endif /* UART_COMMUNICATION_H_ */
