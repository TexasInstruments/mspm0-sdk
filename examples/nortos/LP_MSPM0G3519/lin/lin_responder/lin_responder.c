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
#include <stdio.h>
#include <string.h>
#include "lin_config.h"
#include "ti_msp_dl_config.h"

volatile LIN_STATE gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
volatile uint8_t gNumCycles      = 0;
volatile LIN_Sync_Bits gBitTimes[LIN_RESPONDER_SYNC_CYCLES];
uint8_t gResponderRXBuffer[LIN_DATA_MAX_BUFFER_SIZE]      = {0};
uint8_t gResponderTXMessageData[LIN_DATA_MAX_BUFFER_SIZE] = {0};

/* Variables for autobaud feature */

volatile uint8_t gNumSyncErrors        = 0;
volatile bool gFirstSyncBit            = true;
volatile uint16_t gTotalBitTime        = 0;
volatile uint16_t gLinResponseLapseVar = LIN_RESPONSE_LAPSE;
volatile bool gAutoBaudUsed            = false;
volatile uint32_t gLin0TbitWidthVar    = LIN_0_TBIT_WIDTH;
volatile uint16_t gPrevBaudRate        = LIN_0_BAUD_RATE;
volatile uint16_t gCurrBaudRate        = LIN_0_BAUD_RATE;

void LIN_Message_1_Handler(void)
{
    memcpy(&gResponderTXMessageData[0], &gResponderRXBuffer[0], 0x08);
    /* Toggle LED1 with RX packet */
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

LIN_table_record_t responderMessageTable[LIN_RESPONDER_NUM_MSGS] = {
    /* Important, connection number matches the order of this table */
    {
        0x08, /* PID 0x08 - connection #0 */
        5,
    },
    {
        0x49, /* PID 0x09 */
        5,
    },
    {
        0x0D, /* PID 0x0D */
        5,
    },
    {0x39, /* PID 0x39 */
        8, LIN_Message_1_Handler},
    {0xBA, /* PID 0x3A */
        8, LIN_Message_1_Handler},
    {0xFB, /* PID 0x3B */
        8, LIN_Message_1_Handler},
};

void setLINResponderRXMessage(
    UART_Regs *uart, uint8_t data, volatile LIN_STATE *gStateMachine)
{
    static uint8_t msgTableIndex = 0;
    static uint8_t byteCounter   = 0;
    static LIN_word_t tempChksum;
    static LIN_RX_STATE LIN_state = LIN_RX_STATE_ID;
    volatile uint8_t checksum;
    uint8_t rxChecksum;
    LIN_function_ptr_t callbackFunction;

    switch (LIN_state) {
        case LIN_RX_STATE_ID:
            /* Find ID message */
            for (msgTableIndex = 0; msgTableIndex < LIN_RESPONDER_NUM_MSGS;
                 msgTableIndex++) {
                if (responderMessageTable[msgTableIndex].msgID == data) {
                    break;
                }
            }

            if (msgTableIndex >= LIN_RESPONDER_NUM_MSGS) {
                data = LIN_MESSAGE_NOT_FOUND;
            }

            switch (data) {
                case 0x39: /* ID 0x39 */
                case 0xBA: /* ID 0x3A */
                case 0xFB: /* ID 0x3B */
                    tempChksum.word = data;
                    byteCounter     = 0;
                    LIN_state       = LIN_RX_STATE_DATA;
                    *gStateMachine  = LIN_STATE_DATA_FIELD;
                    break;

                case 0x08: /* ID 0x08 */
                case 0x49: /* ID 0x09 */
                case 0x0D: /* ID 0x0D */
                    /*
                     * Delay is used to ensure STOP bit after PID is completely
                     * received before data transmitted.
                     */
                    delay_cycles(gLinResponseLapseVar);
                    sendLINResponderTXMessage(uart, msgTableIndex,
                        gResponderTXMessageData, responderMessageTable);
                    /* Toggle LED2 with TX packet */
                    DL_GPIO_togglePins(
                        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
                    *gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
                    LIN_state      = LIN_RX_STATE_ID;
                    break;
                default:
                    *gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
                    break;
            }
            break;

        case LIN_RX_STATE_DATA:
            gResponderRXBuffer[byteCounter] = data;
            tempChksum.word += gResponderRXBuffer[byteCounter];
            byteCounter++;

            if (byteCounter >= responderMessageTable[msgTableIndex].msgSize) {
                LIN_state = LIN_RX_STATE_CHECKSUM;
            }
            break;

        case LIN_RX_STATE_CHECKSUM:
            rxChecksum = data;

            tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];
            checksum        = tempChksum.byte[0];
            checksum += tempChksum.byte[1];
            checksum = 0xFF - checksum;

            if (rxChecksum == checksum) {
                callbackFunction =
                    responderMessageTable[msgTableIndex].callbackFunction;
                callbackFunction();
            }

            /* Clear break field and sync byte flags */
            *gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
            LIN_state      = LIN_RX_STATE_ID;
            break;
        default:
            LIN_state = LIN_RX_STATE_ID;
            break;
    }
}

int main(void)
{
    SYSCFG_DL_init();

    /* Enable transceiver */
    DL_GPIO_setPins(GPIO_LIN_ENABLE_PORT, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN);

    NVIC_ClearPendingIRQ(LIN_0_INST_INT_IRQN);
    NVIC_EnableIRQ(LIN_0_INST_INT_IRQN);
    DL_SYSCTL_enableSleepOnExit();

    while (1) {
        __WFI();
    }
}

void LIN_0_INST_IRQHandler(void)
{
    uint16_t counterVal       = 0;
    uint8_t data              = 0;
    uint16_t averageBitTime   = 0;
    uint16_t measuredBaudRate = 0;
    switch (DL_UART_Extend_getPendingInterrupt(LIN_0_INST)) {
        /* LIN Minimum Break Field Width Interrupt. */
        case DL_UART_EXTEND_IIDX_LIN_FALLING_EDGE:
            /* Signals the start of the break field. */
            if (gStateMachine == LIN_STATE_WAIT_FOR_BREAK) {
                gStateMachine = LIN_STATE_BREAK_FIELD;

                counterVal = DL_UART_Extend_getLINCounterValue(LIN_0_INST);
                /* Validation check of the length of the break field. */
                if (counterVal < (gLin0TbitWidthVar * 13.5) &&
                    counterVal >
                        (gLin0TbitWidthVar * LIN_0_TBIT_COUNTER_COEFFICIENT))

                {
                    gStateMachine = LIN_STATE_SYNC_FIELD_NEG_EDGE;
                    DL_UART_Extend_enableLINCounterClearOnFallingEdge(
                        LIN_0_INST);
                    DL_UART_Extend_enableInterrupt(
                        LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_NEG_EDGE);
                    DL_UART_Extend_enableLINSyncFieldValidationCounterControl(
                        LIN_0_INST);
                } else {
                    gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
                }

                DL_UART_Extend_disableInterrupt(
                    LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
            }
            break;
        /* Rising Edge Detection Interrupt on UARTxRXD. */
        case DL_UART_EXTEND_IIDX_RXD_POS_EDGE:
            /* Signals the positive edge of a sync field segment. */
            if (gStateMachine == LIN_STATE_SYNC_FIELD_POS_EDGE) {
                gBitTimes[gNumCycles].posEdge =
                    DL_UART_Extend_getLINRisingEdgeCaptureValue(LIN_0_INST);
                /* Validation check of the timing of the sync field segment.
                 * Finding an invalid sync bit stores each bit time to
                 * calculate new baud rate */
                if (gBitTimes[gNumCycles].posEdge >
                        ((gLin0TbitWidthVar * 95) / 100) &&
                    gBitTimes[gNumCycles].posEdge <
                        ((gLin0TbitWidthVar * 105) / 100)) {
                    gNumCycles++;
                } else if (!gFirstSyncBit) {
                    gTotalBitTime =
                        gTotalBitTime + gBitTimes[gNumCycles].posEdge;
                    gNumSyncErrors++;
                } else {
                    gFirstSyncBit = false;
                }
                /* Only 5 segments of a sync field. */
                if ((gNumSyncErrors + gNumCycles) ==
                    LIN_RESPONDER_SYNC_CYCLES) {
                    DL_UART_Extend_enableInterrupt(
                        LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
                    DL_UART_Extend_disableInterrupt(
                        LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_NEG_EDGE);

                    /* Track new and previous baud rate for validation when
                     * increasing baud rate. Ensures that resets to deal with
                     * overrun errors happen on the appropriate frame.
                     * Reset all variables relevant to sync field */
                    if (gNumCycles == LIN_RESPONDER_SYNC_CYCLES) {
                        gPrevBaudRate = gCurrBaudRate;
                        gAutoBaudUsed = false;
                    }
                    gNumCycles     = 0;
                    gNumSyncErrors = 0;
                    gTotalBitTime  = 0;
                    gFirstSyncBit  = true;

                    /* If 4 sync errors are detected, update baud rate given
                 * autobaud is enabled*/

                } else if ((gNumSyncErrors == AUTO_BAUD_THRESHOLD) &&
                           AUTO_BAUD_ENABLED) {
                    averageBitTime   = gTotalBitTime / gNumSyncErrors;
                    measuredBaudRate = LIN_0_INST_FREQUENCY / averageBitTime;

                    DL_UART_disable(LIN_0_INST);
                    delay_cycles(AUTO_BAUD_CONFIG_DELAY);
                    DL_UART_disableFIFOs(LIN_0_INST);
                    gLinResponseLapseVar =
                        LIN_0_INST_FREQUENCY / (2 * measuredBaudRate);
                    gLin0TbitWidthVar = averageBitTime;
                    DL_UART_configBaudRate(
                        LIN_0_INST, LIN_0_INST_FREQUENCY, measuredBaudRate);

                    DL_UART_Extend_setLINCounterCompareValue(LIN_0_INST,
                        gLin0TbitWidthVar * LIN_0_TBIT_COUNTER_COEFFICIENT);
                    DL_UART_Extend_enable(LIN_0_INST);
                    gPrevBaudRate = gCurrBaudRate;
                    gCurrBaudRate = measuredBaudRate;

                    gAutoBaudUsed = true;
                    gStateMachine = LIN_STATE_SYNC_FIELD_NEG_EDGE;
                }

                else {
                    gStateMachine = LIN_STATE_SYNC_FIELD_NEG_EDGE;
                }
            }
            break;
        /* Counter Overflow Interrupt. */
        case DL_UART_EXTEND_IIDX_LIN_COUNTER_OVERFLOW:
            gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
            break;
        /* Negative Edge Detection Interrupt on UARTxRXD. */
        case DL_UART_EXTEND_IIDX_RXD_NEG_EDGE:
            /* Signals the negative edge of a sync field segment. */
            if (gStateMachine == LIN_STATE_SYNC_FIELD_NEG_EDGE) {
                /* If flag for overrun has gone off and previous
                 * baud rate < current baud rate, turn counter off and on. */
                if ((gCurrBaudRate > gPrevBaudRate) &&
                    DL_UART_getErrorStatus(
                        LIN_0_INST, DL_UART_ERROR_OVERRUN)) {
                    DL_UART_disableLINCounter(LIN_0_INST);
                    DL_UART_enableLINCounter(LIN_0_INST);
                }
                gBitTimes[gNumCycles].negEdge =
                    DL_UART_Extend_getLINFallingEdgeCaptureValue(LIN_0_INST);
                gStateMachine = LIN_STATE_SYNC_FIELD_POS_EDGE;
            }
            break;
        /* UARTxRX Data Detection Interrupt. */
        case DL_UART_EXTEND_IIDX_RX:
            /* Determine if SYNC byte was received */
            if (gStateMachine == LIN_STATE_SYNC_FIELD_POS_EDGE) {
                data = DL_UART_Extend_receiveData(LIN_0_INST);
                if (data == LIN_SYNC_BYTE) {
                    /* First received byte is SYNC. Ignore it and wait for PID */
                    gStateMachine = LIN_STATE_PID_FIELD;
                } else if ((data != LIN_SYNC_BYTE) && gAutoBaudUsed) {
                    /* Incorrect baud rate can cause an incorrect sync byte to be received.
                     * If autobaud was used and the sync byte was incorrect, the state machine
                     * proceeds as usual.*/
                    gStateMachine = LIN_STATE_PID_FIELD;
                } else {
                    /* Unexpected byte, return to idle status */
                    gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
                }
            } else if (gStateMachine == LIN_STATE_PID_FIELD) {
                data = DL_UART_Extend_receiveData(LIN_0_INST);

                /* Process the PID. The state machine will be updated */
                setLINResponderRXMessage(LIN_0_INST, data, &gStateMachine);
            }
            /* Signals the data field */
            else if (gStateMachine == LIN_STATE_DATA_FIELD) {
                data = DL_UART_Extend_receiveData(LIN_0_INST);

                /*
                 *  Process data byte.
                 *  If data byte matches ID byte from "responderMessageTable"
                 *  then performs the matching function. Otherwise does nothing.
                 */
                setLINResponderRXMessage(LIN_0_INST, data, &gStateMachine);
            }
            /* Flushes any data that comes after an invalid PID*/
            else if (gStateMachine == LIN_STATE_WAIT_FOR_BREAK) {
                data = DL_UART_Extend_receiveData(LIN_0_INST);
            }
        default:
            break;
    }
}
