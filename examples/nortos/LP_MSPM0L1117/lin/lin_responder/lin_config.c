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

volatile LIN_STATE gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
uint8_t gMessageTableIndex = 0;
LIN_RX_STATE LIN_state = LIN_RX_STATE_DATA;
uint8_t byteCounter   = 0;
LIN_word_t tempChksum;

#if defined(__MSPM0_HAS_UART_EXTD__)

void LIN_Responder_receivePID(UART_Regs *uart, uint8_t rxByte, uint8_t *TXmsgBuffer, LIN_table_record_t *messageTable)
{
    static uint8_t tableIndex = 0;
    /*Find PID in table*/
    for (tableIndex = 0; tableIndex < LIN_RESPONDER_NUM_MSGS;tableIndex++) {
        if (messageTable[tableIndex].msgID == rxByte) {
            /* Keep track of which message PID we are using */
            gMessageTableIndex = tableIndex;
            break;
        }
    }
    if (tableIndex >= LIN_RESPONDER_NUM_MSGS) {

        /*No PID matched*/
        gStateMachine = LIN_STATE_WAIT_FOR_BREAK;

        /* Disable LIN RX */
        DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);
    }
    else
    {
        /*receive message*/
        if (messageTable[tableIndex].callbackFunction != NULL) {
            gStateMachine  = LIN_STATE_DATA_FIELD;
            /*Enable Timer to detect timeout issue */
            DL_Timer_setLoadValue(TIMER_0_INST, TIMEOUT);
            DL_Timer_startCounter(TIMER_0_INST);
        }
        /*Send message*/
        else
        {
            /* Disable LIN RX */
            DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

#ifdef Transmit_INT
            gStateMachine  = LIN_STATE_DATA_FIELD;
            DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);
            DL_UART_Extend_transmitData(uart, TXmsgBuffer[0]);
            DL_UART_Extend_clearInterruptStatus(uart, DL_UART_EXTEND_INTERRUPT_TX);
            DL_UART_Extend_enableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_TX);

#else
            LIN_Responder_transmitMessage(uart, TXmsgBuffer, messageTable);
            gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
#endif
        }
    }
}



void LIN_Responder_receiveMessage(uint8_t rxByte, uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    uint8_t checksum;
    uint8_t rxChecksum;
    LIN_function_ptr_t callbackFunction;

    switch (LIN_state) {
        case LIN_RX_STATE_DATA:
            /* Save the received byte to the RX message msgBuffer */
            msgBuffer[byteCounter] = rxByte;
            /* Use received byte to add to checksum calculation */
            tempChksum.word += msgBuffer[byteCounter++];

            /*
            * Check if the number of received bytes is greater than the expected
            * number of data bytes. If yes, then the checksum byte was just
            * just received.
            */
            if (byteCounter >= messageTable[gMessageTableIndex].msgSize) {
                LIN_state = LIN_RX_STATE_CHECKSUM;
            }
            DL_Timer_startCounter(TIMER_0_INST);
            break;

        case LIN_RX_STATE_CHECKSUM:
            rxChecksum = rxByte;
            /* Disable LIN RX */
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);

            /* Calculate the checksum based on the received data */
            tempChksum.word += messageTable[gMessageTableIndex].msgID;
            tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];
            checksum = tempChksum.byte[0];
            checksum += tempChksum.byte[1];
            checksum = 0xFF - checksum;



            /* Compare the received checksum to the calculated checksum */
            if (rxChecksum == checksum) {
                /* If they match, call the corresponding callback function */
                callbackFunction =
                        messageTable[gMessageTableIndex].callbackFunction;
                callbackFunction();
            }

            /* Reset state */
            byteCounter     = 0;
            tempChksum.word = 0;
            LIN_state       = LIN_RX_STATE_DATA;
            gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
            /* Disable LIN RX */
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
            DL_UART_Extend_enableLINCounter(LIN_0_INST);
            DL_UART_Extend_enableLINCountWhileLow(LIN_0_INST);
            DL_UART_Extend_enableLINCounterCompareMatch(LIN_0_INST); 
            DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
            DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
            break;

        default:
            LIN_state = LIN_RX_STATE_DATA;
            break;
    }
}



void LIN_Responder_transmitMessage(UART_Regs *uart, uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    uint8_t checksum;

#ifdef Transmit_INT

    static uint8_t locIndex = 1;
    static LIN_word_t tempChksum_TX;

    tempChksum_TX.word += msgBuffer[locIndex];
    DL_UART_Extend_transmitData(uart, msgBuffer[locIndex++]);
    if(locIndex >= messageTable[gMessageTableIndex].msgSize)
    {

        /* Calculate and then transmit the checksum*/
        tempChksum_TX.word = tempChksum_TX.word + messageTable[gMessageTableIndex].msgID + msgBuffer[0];
        tempChksum_TX.word = tempChksum_TX.byte[0] + tempChksum_TX.byte[1];

        checksum = tempChksum_TX.byte[0];
        checksum += tempChksum_TX.byte[1];
        checksum = 0xFF - checksum;
        DL_UART_Extend_transmitDataBlocking(uart, checksum);
        DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_TX);
        DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
        /* Call TX callback for LED indication */
        LIN_processMessage_Tx();

        /*Reset state*/
        locIndex = 1;
        tempChksum_TX.word = 0;
        DL_UART_Extend_receiveDataBlocking(uart);
        gStateMachine  = LIN_STATE_WAIT_FOR_BREAK;
        DL_UART_Extend_enableLINCounter(LIN_0_INST);
        DL_UART_Extend_enableInterrupt(LIN_0_INST,DL_UART_EXTEND_INTERRUPT_LINC0_MATCH);
        DL_UART_Extend_enableLINCountWhileLow(LIN_0_INST);
        DL_UART_Extend_enableLINCounterCompareMatch(LIN_0_INST);  
        while (DL_UART_Extend_isBusy(uart));
        DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
        DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
    }

#else

    uint8_t locIndex;
    LIN_word_t tempChksum_TX;

    tempChksum_TX.word = 0;

    /* Disable LIN RX */
    for (locIndex = 0; locIndex < messageTable[gMessageTableIndex].msgSize; locIndex++)
    {
        DL_UART_Extend_transmitDataBlocking(uart, msgBuffer[locIndex]);
        tempChksum_TX.word += msgBuffer[locIndex];
    }
    /* Calculate and then transmit the checksum*/
    tempChksum_TX.word += messageTable[gMessageTableIndex].msgID;
    tempChksum_TX.word = tempChksum_TX.byte[0] + tempChksum_TX.byte[1];

    checksum = tempChksum_TX.byte[0];
    checksum += tempChksum_TX.byte[1];
    checksum = 0xFF - checksum;
    DL_UART_Extend_transmitDataBlocking(uart, checksum);
    /* Call TX callback for LED indication */
    LIN_processMessage_Tx();

    /*clear Buffer*/
    DL_UART_Extend_receiveDataBlocking(uart);
    DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
    while (DL_UART_Extend_isBusy(uart));
    DL_UART_Extend_enableLINCounter(LIN_0_INST);
    DL_UART_Extend_enableInterrupt(LIN_0_INST,DL_UART_EXTEND_INTERRUPT_LINC0_MATCH);
    DL_UART_Extend_enableLINCountWhileLow(LIN_0_INST);
    DL_UART_Extend_enableLINCounterCompareMatch(LIN_0_INST);
    DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
    DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
#endif
}

#endif

#if defined(__MCU_HAS_UNICOMMUART__)

void LIN_Responder_receivePID(UNICOMM_Inst_Regs *unicomm, uint8_t rxByte, uint8_t *TXmsgBuffer, LIN_table_record_t *messageTable)
{
    static uint8_t tableIndex = 0;
    /*Find PID in table*/
    for (tableIndex = 0; tableIndex < LIN_RESPONDER_NUM_MSGS;tableIndex++) {
        if (messageTable[tableIndex].msgID == rxByte) {
            /* Keep track of which message PID we are using */
            gMessageTableIndex = tableIndex;
            break;
        }
    }
    if (tableIndex >= LIN_RESPONDER_NUM_MSGS) {

        /*No PID matched*/
        gStateMachine = LIN_STATE_WAIT_FOR_BREAK;

        /* Disable LIN RX */
        DL_UART_Extend_disableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);
    }
    else
    {
        /*receive message*/
        if (messageTable[tableIndex].callbackFunction != NULL) {
            gStateMachine  = LIN_STATE_DATA_FIELD;
            /*Enable Timer to detect timeout issue */
            DL_Timer_setLoadValue(TIMER_0_INST, TIMEOUT);
            DL_Timer_startCounter(TIMER_0_INST);
        }
        /*Send message*/
        else
        {
            /* Disable LIN RX */
            DL_UART_Extend_disableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);

#ifdef Transmit_INT
            gStateMachine  = LIN_STATE_DATA_FIELD;
            DL_UART_Extend_disableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_RX);
            DL_UART_Extend_transmitData(unicomm, TXmsgBuffer[0]);
            DL_UART_Extend_clearInterruptStatus(unicomm, DL_UART_EXTEND_INTERRUPT_TX);
            DL_UART_Extend_enableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_TX);

#else
            LIN_Responder_transmitMessage(unicomm, TXmsgBuffer, messageTable);
            gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
#endif
        }
    }
}



void LIN_Responder_receiveMessage(uint8_t rxByte, uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    uint8_t checksum;
    uint8_t rxChecksum;
    LIN_function_ptr_t callbackFunction;

    switch (LIN_state) {
        case LIN_RX_STATE_DATA:
            /* Save the received byte to the RX message msgBuffer */
            msgBuffer[byteCounter] = rxByte;
            /* Use received byte to add to checksum calculation */
            tempChksum.word += msgBuffer[byteCounter++];

            /*
            * Check if the number of received bytes is greater than the expected
            * number of data bytes. If yes, then the checksum byte was just
            * just received.
            */
            if (byteCounter >= messageTable[gMessageTableIndex].msgSize) {
                LIN_state = LIN_RX_STATE_CHECKSUM;
            }
            DL_Timer_startCounter(TIMER_0_INST);
            break;

        case LIN_RX_STATE_CHECKSUM:
            rxChecksum = rxByte;
            /* Disable LIN RX */
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);

            /* Calculate the checksum based on the received data */
            tempChksum.word += messageTable[gMessageTableIndex].msgID;
            tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];
            checksum = tempChksum.byte[0];
            checksum += tempChksum.byte[1];
            checksum = 0xFF - checksum;



            /* Compare the received checksum to the calculated checksum */
            if (rxChecksum == checksum) {
                /* If they match, call the corresponding callback function */
                callbackFunction =
                        messageTable[gMessageTableIndex].callbackFunction;
                callbackFunction();
            }

            /* Reset state */
            byteCounter     = 0;
            tempChksum.word = 0;
            LIN_state       = LIN_RX_STATE_DATA;
            gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
            /* Disable LIN RX */
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
            DL_UART_Extend_enableLINCounter(LIN_0_INST);
            DL_UART_Extend_enableLINCountWhileLow(LIN_0_INST);
            DL_UART_Extend_enableLINCounterCompareMatch(LIN_0_INST); 
            DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
            DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
            break;

        default:
            LIN_state = LIN_RX_STATE_DATA;
            break;
    }
}



void LIN_Responder_transmitMessage(UNICOMM_Inst_Regs *unicomm, uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    uint8_t checksum;

#ifdef Transmit_INT

    static uint8_t locIndex = 1;
    static LIN_word_t tempChksum_TX;

    tempChksum_TX.word += msgBuffer[locIndex];
    DL_UART_Extend_transmitData(unicomm, msgBuffer[locIndex++]);
    if(locIndex >= messageTable[gMessageTableIndex].msgSize)
    {

        /* Calculate and then transmit the checksum*/
        tempChksum_TX.word = tempChksum_TX.word + messageTable[gMessageTableIndex].msgID + msgBuffer[0];
        tempChksum_TX.word = tempChksum_TX.byte[0] + tempChksum_TX.byte[1];

        checksum = tempChksum_TX.byte[0];
        checksum += tempChksum_TX.byte[1];
        checksum = 0xFF - checksum;
        DL_UART_Extend_transmitDataBlocking(unicomm, checksum);

        while (!DL_UART_isRXFIFOEmpty(unicomm)) {
          DL_UART_Extend_receiveData(unicomm);
        }
        
        DL_UART_Extend_disableInterrupt(unicomm, DL_UART_EXTEND_INTERRUPT_TX);
        DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
        /* Call TX callback for LED indication */
        LIN_processMessage_Tx();

        /*Reset state*/
        locIndex = 1;
        tempChksum_TX.word = 0;
        DL_UART_Extend_receiveData(unicomm);
        gStateMachine  = LIN_STATE_WAIT_FOR_BREAK;
        DL_UART_Extend_enableLINCounter(LIN_0_INST);
        DL_UART_Extend_enableInterrupt(LIN_0_INST,DL_UART_EXTEND_INTERRUPT_LINC0_MATCH);
        DL_UART_Extend_enableLINCountWhileLow(LIN_0_INST);
        DL_UART_Extend_enableLINCounterCompareMatch(LIN_0_INST);  
        while (DL_UART_Extend_isBusy(unicomm));
        DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
        DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
    }

#else

    uint8_t locIndex;
    LIN_word_t tempChksum_TX;

    tempChksum_TX.word = 0;

    /* Disable LIN RX */
    for (locIndex = 0; locIndex < messageTable[gMessageTableIndex].msgSize; locIndex++)
    {
        DL_UART_Extend_transmitDataBlocking(unicomm, msgBuffer[locIndex]);
        tempChksum_TX.word += msgBuffer[locIndex];
    }
    /* Calculate and then transmit the checksum*/
    tempChksum_TX.word += messageTable[gMessageTableIndex].msgID;
    tempChksum_TX.word = tempChksum_TX.byte[0] + tempChksum_TX.byte[1];

    checksum = tempChksum_TX.byte[0];
    checksum += tempChksum_TX.byte[1];
    checksum = 0xFF - checksum;
    DL_UART_Extend_transmitDataBlocking(unicomm, checksum);

    while (!DL_UART_isRXFIFOEmpty(unicomm)) {
      DL_UART_Extend_receiveData(unicomm);

    }
    /* Call TX callback for LED indication */
    LIN_processMessage_Tx();

    /*clear Buffer*/
    DL_UART_Extend_receiveData(unicomm);
    DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
    while (DL_UART_Extend_isBusy(unicomm));
    DL_UART_Extend_enableLINCounter(LIN_0_INST);
    DL_UART_Extend_enableInterrupt(LIN_0_INST,DL_UART_EXTEND_INTERRUPT_LINC0_MATCH);
    DL_UART_Extend_enableLINCountWhileLow(LIN_0_INST);
    DL_UART_Extend_enableLINCounterCompareMatch(LIN_0_INST);
    DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
    DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_FALLING_EDGE);
#endif
}

#endif
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            if(gStateMachine == LIN_STATE_DATA_FIELD)
            {
                gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
                LIN_state = LIN_RX_STATE_DATA;
                byteCounter     = 0;
                tempChksum.word = 0;

                /* Disable LIN RX */
                DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
            }
            break;
        default:
            break;
    }
}
