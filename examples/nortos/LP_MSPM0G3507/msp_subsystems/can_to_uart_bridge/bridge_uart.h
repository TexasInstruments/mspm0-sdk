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
#ifndef BRIDGE_UART_H_
#define BRIDGE_UART_H_

#ifndef UART_ID_LENGTH
/* Define for id length(0, 1, 4) */
/* The format of Uart:
     * if UART_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if UART_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if UART_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...*/
//#define UART_ID_LENGTH  (0)
#define UART_ID_LENGTH  (1)
//#define UART_ID_LENGTH  (4)
#endif

#ifndef UART_TIMEOUT
#define UART_TIMEOUT  (0x4000)      //timeout 250ms
#endif

/* Variables for UART receive */
#ifdef UART_TRANSPARENT     /* UART Transparent transmission */
#define UART_RX_SIZE   (TRANSMIT_DATA_LENGTH)
#else
#define UART_RX_SIZE   (TRANSMIT_DATA_LENGTH + 3 + UART_ID_LENGTH)
#endif
extern Custom_Element gUART_RX_Element;
extern uint8_t gUartReceiveGroup[UART_RX_SIZE];
extern uint16_t gGetUartRxMsg_Count;
bool getUartRxMsg(UART_Regs *uart, uint8_t *UartReceiveGroup);
bool processUartRxMsg(uint8_t *UartReceiveGroup, Custom_Element *gUART_RX_Element);
bool getUartRxMsg_transparent(UART_Regs *uart, uint8_t *UartReceiveGroup);
bool processUartRxMsg_transparent(uint8_t *UartReceiveGroup, Custom_Element *gUART_RX_Element);

/* Variables for UART Transmit */
#ifdef UART_TRANSPARENT     /* UART Transparent transmission */
#define UART_TX_SIZE   (TRANSMIT_DATA_LENGTH)
#else
#define UART_TX_SIZE   (TRANSMIT_DATA_LENGTH + 3 + UART_ID_LENGTH)
#endif
extern Custom_Element gUART_TX_Element;
extern uint8_t gUartTransmitGroup[UART_TX_SIZE];
extern bool gUartTxflag;
bool sendUartTxMsg(UART_Regs *uart, uint8_t *UartTransmitGroup, uint16_t lenth);
bool processUartTxMsg(uint8_t *UartTransmitGroup, Custom_Element *gUART_TX_Element);
void transmitPacketBlocking(UART_Regs *uart, uint8_t *buffer, uint32_t size);
bool processUartTxMsg_transparent(uint8_t *UartTransmitGroup, Custom_Element *gUART_TX_Element);

#endif /* BRIDGE_UART_H_ */
