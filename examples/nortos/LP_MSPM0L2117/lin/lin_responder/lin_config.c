/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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

#include "lin_config.h"

#if defined(__MSPM0_HAS_UART_EXTD__)

void sendLINResponderTXMessage(UART_Regs *uart, uint8_t tableIndex,
    uint8_t *msgBuffer, LIN_table_record_t *responderMessageTable)
{
    uint8_t locIndex;
    uint8_t checksum;
    LIN_word_t tempChksum;

    /* Disable LIN RX */
    DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

    tempChksum.word = responderMessageTable[tableIndex].msgID;
    tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];

    for (locIndex = 0; locIndex < responderMessageTable[tableIndex].msgSize;
         locIndex++) {
        DL_UART_Extend_transmitDataBlocking(uart, msgBuffer[locIndex]);
        tempChksum.word += msgBuffer[locIndex];
    }
    /* Calculate and send checksum */
    checksum = tempChksum.byte[0];
    checksum += tempChksum.byte[1];
    checksum = 0xFF - checksum;

    DL_UART_Extend_transmitDataBlocking(uart, checksum);
    while (DL_UART_Extend_isBusy(uart)) {
        ;
    }

    DL_UART_Extend_receiveDataBlocking(uart);

    /* Enable LIN RX */
    DL_UART_Extend_clearInterruptStatus(uart, DL_UART_EXTEND_INTERRUPT_RX);
    DL_UART_Extend_enableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);
}

#endif

#if defined(__MCU_HAS_UNICOMMUART__)

void sendLINResponderTXMessage(UNICOMM_Inst_Regs *unicomm, uint8_t tableIndex,
    uint8_t *msgBuffer, LIN_table_record_t *responderMessageTable)
{
    uint8_t locIndex;
    uint8_t checksum;
    LIN_word_t tempChksum;

    /* Disable LIN RX */
    DL_UART_Extend_disableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);

    tempChksum.word = responderMessageTable[tableIndex].msgID;
    tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];

    for (locIndex = 0; locIndex < responderMessageTable[tableIndex].msgSize;
         locIndex++) {
        DL_UART_Extend_transmitDataBlocking(unicomm, msgBuffer[locIndex]);
        tempChksum.word += msgBuffer[locIndex];
        
        /* Clear RXFIFO */
        while (!DL_UART_isRXFIFOEmpty(unicomm)) {
          DL_UART_Extend_receiveData(unicomm);
    }
    }
    /* Calculate and send checksum */
    checksum = tempChksum.byte[0];
    checksum += tempChksum.byte[1];
    checksum = 0xFF - checksum;

    DL_UART_Extend_transmitDataBlocking(unicomm, checksum);
    while (DL_UART_Extend_isBusy(unicomm)) {
        ;
    }

    DL_UART_Extend_receiveDataBlocking(unicomm);

    /* Enable LIN RX */
    DL_UART_Extend_clearInterruptStatus(unicomm, DL_UART_EXTEND_INTERRUPT_RX);
    DL_UART_Extend_enableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);
}

#endif