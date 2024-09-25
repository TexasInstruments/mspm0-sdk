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

#define LIN_TABLE_INDEX_PID_39 (0)
#define LIN_TABLE_INDEX_PID_3A (1)
#define LIN_TABLE_INDEX_PID_3B (2)
#define LIN_TABLE_INDEX_PID_08 (3)
#define LIN_TABLE_INDEX_PID_09 (4)
#define LIN_TABLE_INDEX_PID_0D (5)

volatile LIN_STATE gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
volatile bool gSendMessage1      = false;
volatile bool gSendMessage2      = false;
volatile bool gDataReceived      = false;

uint8_t gCommanderRXBuffer[LIN_DATA_MAX_BUFFER_SIZE] = {0};
uint8_t gCommanderTXPacket[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

static void LIN_processMessage_Rx(void);

LIN_table_record_t gCommanderMessageTable[LIN_COMMANDER_NUM_MSGS] = {
    {0x39, /* PID 0x39 */
        8, NULL},
    {0xBA, /* PID 0x3A */
        8, NULL},
    {0xFB, /* PID 0x3B */
        8, NULL},
    {0x08, /* PID 0x08 */
        5, LIN_processMessage_Rx},
    {0x49, /* PID 0x09 */
        5, LIN_processMessage_Rx},
    {0x0D, /* PID 0x0D */
        5, LIN_processMessage_Rx}};

int main(void)
{
    SYSCFG_DL_init();

    /* Enable transceiver */
    DL_GPIO_setPins(GPIO_LIN_ENABLE_PORT, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN);

    NVIC_ClearPendingIRQ(LIN_0_INST_INT_IRQN);
    NVIC_EnableIRQ(LIN_0_INST_INT_IRQN);

    NVIC_EnableIRQ(GPIO_SWITCHES1_INT_IRQN);

    while (1) {
        __WFI();

        if (gSendMessage1 == true) { /* Send LIN message PID_39 */
            gSendMessage1 = false;
            gDataReceived = false;
            LIN_Commander_transmitMessage(LIN_0_INST, LIN_TABLE_INDEX_PID_39,
                gCommanderTXPacket, gCommanderMessageTable);

            /* Increment first and last bytes of the data */
            gCommanderTXPacket[0] += 1;
            gCommanderTXPacket[7] += 1;

            /* Toggle LED1 with TX packet */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

        } else if (gSendMessage2 == true) { /* Send LIN message PID_08 */
            gSendMessage2 = false;
            gDataReceived = false;
            LIN_Commander_transmitMessage(LIN_0_INST, LIN_TABLE_INDEX_PID_08,
                gCommanderRXBuffer, gCommanderMessageTable);
        }
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case DL_INTERRUPT_GROUP1_IIDX_GPIOA:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES1_PORT)) {
                case GPIO_SWITCHES1_USER_SWITCH_1_IIDX:
                    /* When USER_SWITCH_1_INST is pressed, send PID 0x39 */
                    gSendMessage1 = true;
                    break;
                case GPIO_SWITCHES1_USER_SWITCH_2_IIDX:
                    /* When USER_SWITCH_2_INST is pressed, send PID 0x08 */
                    gSendMessage2 = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

static void LIN_processMessage_Rx(void)
{
    /* Message received */
    gDataReceived = true;

    /* Toggle LED2 with RX packet */
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
}

void LIN_0_INST_IRQHandler(void)
{
    uint8_t data = 0;
    switch (DL_UART_Extend_getPendingInterrupt(LIN_0_INST)) {
        case DL_UART_EXTEND_IIDX_RX:
            /* Process data received from the Responder */
            data = DL_UART_Extend_receiveData(LIN_0_INST);
            LIN_Commander_receiveMessage(
                LIN_0_INST, data, gCommanderRXBuffer, gCommanderMessageTable);
            break;
        default:
            break;
    }
}
