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
#include <stdio.h>

uint8_t gMessageTableIndex = 0;

#if defined(__MSPM0_HAS_UART_EXTD__)

void LIN_Commander_receiveMessage(UART_Regs *uart, uint8_t rxByte,
    uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    static uint8_t byteCounter = 0;
    static LIN_word_t tempChksum;
    static LIN_RX_STATE LIN_state = LIN_RX_STATE_DATA;
    uint8_t checksum;
    uint8_t rxChecksum;
    LIN_function_ptr_t callbackFunction;

    switch (LIN_state) {
        case LIN_RX_STATE_DATA:
            /* Save the received byte to the RX message buffer */
            msgBuffer[byteCounter] = rxByte;
            /* Use received byte to add to checksum calculation */
            tempChksum.word += msgBuffer[byteCounter];

            /*
            * Check if the number of received bytes is greater than the expected
            * number of data bytes. If yes, then the checksum byte was just
            * just received.
            */
            byteCounter++;
            if (byteCounter >= messageTable[gMessageTableIndex].msgSize) {
                LIN_state = LIN_RX_STATE_CHECKSUM;
            }
            break;

        case LIN_RX_STATE_CHECKSUM:
            rxChecksum = rxByte;

            /* Calculate the checksum based on the received data */
            tempChksum.word += messageTable[gMessageTableIndex].msgID;
            tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];
            checksum        = tempChksum.byte[0];
            checksum += tempChksum.byte[1];
            checksum = 0xFF - checksum;

            /* Compare the received checksum to the calculated checksum */
            if (rxChecksum == checksum) {
                /* If they match, call the corresponding callback function */
                callbackFunction =
                    messageTable[gMessageTableIndex].callbackFunction;
                callbackFunction();
            }

            /* Reset the data */
            byteCounter     = 0;
            tempChksum.word = 0;
            LIN_state       = LIN_RX_STATE_DATA;
            break;

        default:
            LIN_state = LIN_RX_STATE_DATA;
            break;
    }
}

void LIN_Commander_transmitMessage(UART_Regs *uart, uint8_t tableIndex,
    uint8_t *buffer, LIN_table_record_t *messageTable)
{
    uint8_t locIndex;
    uint8_t checksum;
    LIN_word_t tempChksum;

    tempChksum.word = 0;

    /* Disable LIN message reception */
    DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

    /* Transmit BREAK, SYNC byte, and PID */
    DL_UART_enableLINSendBreak(uart);
    delay_cycles(LIN_BREAK_LENGTH); /* Set break for ~1ms */
    DL_UART_disableLINSendBreak(uart);
    DL_UART_Extend_transmitDataBlocking(uart, LIN_SYNC_BYTE);
    DL_UART_Extend_transmitDataBlocking(uart, messageTable[tableIndex].msgID);

    tempChksum.word = messageTable[tableIndex].msgID;

    /*
     * If a callback function is present corresponding to the message PID that
     * was transmitted, then we do not transmit data, but expect to receive
     * data back from the Responder.
     */
    if (messageTable[tableIndex].callbackFunction != NULL) {
        /* Clear pending RX Interrupts and the RX buffer */
        DL_UART_Extend_receiveData(uart);
        DL_UART_Extend_clearInterruptStatus(uart, DL_UART_EXTEND_INTERRUPT_RX);

        /* Keep track of which message PID we are using */
        gMessageTableIndex = tableIndex;

        DL_UART_Extend_enableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

    }

    else {
        /*
         * There is no callback associated with the message PID, so we
         * transmit a data packet to the Responder
         */
        for (locIndex = 0; locIndex < messageTable[tableIndex].msgSize;
             locIndex++) {
            DL_UART_Extend_transmitDataBlocking(uart, buffer[locIndex]);
            tempChksum.word += buffer[locIndex];
        }

        /* Calculate and then transmit the checksum*/
        tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];

        checksum = tempChksum.byte[0];
        checksum += tempChksum.byte[1];
        checksum = 0xFF - checksum;

        DL_UART_Extend_transmitDataBlocking(uart, checksum);
    }
}

#endif

#if defined(__MCU_HAS_UNICOMMUART__)

void LIN_Commander_receiveMessage(UNICOMM_Inst_Regs *unicomm, uint8_t rxByte,
    uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    static uint8_t byteCounter = 0;
    static LIN_word_t tempChksum;
    static LIN_RX_STATE LIN_state = LIN_RX_STATE_DATA;
    uint8_t checksum;
    uint8_t rxChecksum;
    LIN_function_ptr_t callbackFunction;

    switch (LIN_state) {
        case LIN_RX_STATE_DATA:
            /* Save the received byte to the RX message buffer */
            msgBuffer[byteCounter] = rxByte;
            /* Use received byte to add to checksum calculation */
            tempChksum.word += msgBuffer[byteCounter];

            /*
            * Check if the number of received bytes is greater than the expected
            * number of data bytes. If yes, then the checksum byte was just
            * just received.
            */
            byteCounter++;
            if (byteCounter >= messageTable[gMessageTableIndex].msgSize) {
                LIN_state = LIN_RX_STATE_CHECKSUM;
            }
            break;

        case LIN_RX_STATE_CHECKSUM:
            rxChecksum = rxByte;

            /* Calculate the checksum based on the received data */
            tempChksum.word += messageTable[gMessageTableIndex].msgID;
            tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];
            checksum        = tempChksum.byte[0];
            checksum += tempChksum.byte[1];
            checksum = 0xFF - checksum;

            /* Compare the received checksum to the calculated checksum */
            if (rxChecksum == checksum) {
                /* If they match, call the corresponding callback function */
                callbackFunction =
                    messageTable[gMessageTableIndex].callbackFunction;
                callbackFunction();
            }

            /* Reset the data */
            byteCounter     = 0;
            tempChksum.word = 0;
            LIN_state       = LIN_RX_STATE_DATA;
            break;

        default:
            LIN_state = LIN_RX_STATE_DATA;
            break;
    }
}

void LIN_Commander_transmitMessage(UNICOMM_Inst_Regs *unicomm, uint8_t tableIndex,
    uint8_t *buffer, LIN_table_record_t *messageTable)
{
    uint8_t locIndex;
    uint8_t checksum;
    LIN_word_t tempChksum;

    tempChksum.word = 0;

    /* Disable LIN message reception */
    DL_UART_Extend_disableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);

    /* Transmit BREAK, SYNC byte, and PID */
    DL_UART_enableLINSendBreak(unicomm);
    delay_cycles(LIN_BREAK_LENGTH); /* Set break for ~1ms */
    DL_UART_disableLINSendBreak(unicomm);
    DL_UART_Extend_transmitDataBlocking(unicomm, LIN_SYNC_BYTE);
    
    /* Clear RXFIFO */
    while (!DL_UART_isRXFIFOEmpty(unicomm)) {
        DL_UART_Extend_receiveData(unicomm);
    }
    
    DL_UART_Extend_transmitDataBlocking(unicomm, messageTable[tableIndex].msgID);
    
    /* Clear RXFIFO */
    while (!DL_UART_isRXFIFOEmpty(unicomm)) {
        DL_UART_Extend_receiveData(unicomm);
    }
 
    tempChksum.word = messageTable[tableIndex].msgID;

    /*
     * If a callback function is present corresponding to the message PID that
     * was transmitted, then we do not transmit data, but expect to receive
     * data back from the Responder.
     */
    if (messageTable[tableIndex].callbackFunction != NULL) {
        /* Clear pending RX Interrupts and the RX buffer */
        DL_UART_Extend_receiveData(unicomm);
        DL_UART_Extend_clearInterruptStatus(unicomm, DL_UART_EXTEND_INTERRUPT_RX);

        /* Keep track of which message PID we are using */
        gMessageTableIndex = tableIndex;

        DL_UART_Extend_enableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);

    }

    else {
        /*
         * There is no callback associated with the message PID, so we
         * transmit a data packet to the Responder
         */
        for (locIndex = 0; locIndex < messageTable[tableIndex].msgSize;
             locIndex++) {
            DL_UART_Extend_transmitDataBlocking(unicomm, buffer[locIndex]);
            
            /* Clear RXFIFO */
            while (!DL_UART_isRXFIFOEmpty(unicomm)) {
               DL_UART_Extend_receiveData(unicomm);
            }
            tempChksum.word += buffer[locIndex];
        }

        /* Calculate and then transmit the checksum*/
        tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];

        checksum = tempChksum.byte[0];
        checksum += tempChksum.byte[1];
        checksum = 0xFF - checksum;

        DL_UART_Extend_transmitDataBlocking(unicomm, checksum);
       
        /* Clear RXFIFO */
        while (!DL_UART_isRXFIFOEmpty(unicomm)) {
          DL_UART_Extend_receiveData(unicomm);
        }
    }
}

#endif

