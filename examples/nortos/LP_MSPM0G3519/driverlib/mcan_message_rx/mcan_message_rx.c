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

#include "ti_msp_dl_config.h"

#define LED_STATUS_ON ((uint8_t) 0x00)
#define LED_STATUS_OFF ((uint8_t) 0x01)

#define ID_MODE_STANDARD (0x0U)
#define ID_MODE_EXTENDED (0x1U)

volatile uint32_t gInterruptLine1Status;
volatile bool gServiceInt;

static void processRxMsg(DL_MCAN_RxBufElement *rxMsg);

int main(void)
{
    DL_MCAN_RxBufElement rxMsg;
    DL_MCAN_RxFIFOStatus rxFS;

    SYSCFG_DL_init();

    gServiceInt           = false;
    gInterruptLine1Status = 0;

    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    while (1) {
        /*
         * Wait for flag interrupt to be triggered and flag to check received
         * message is set to true.
         */
        while (false == gServiceInt) {
            __WFE();
        }
        gServiceInt  = false;
        rxFS.fillLvl = 0;

        if ((gInterruptLine1Status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK) {
            rxFS.num = DL_MCAN_RX_FIFO_NUM_0;
            while ((rxFS.fillLvl) == 0) {
                DL_MCAN_getRxFIFOStatus(MCAN0_INST, &rxFS);
            }

            DL_MCAN_readMsgRam(
                MCAN0_INST, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, &rxMsg);

            DL_MCAN_writeRxFIFOAck(MCAN0_INST, rxFS.num, rxFS.getIdx);

            processRxMsg(&rxMsg);

            gInterruptLine1Status &= ~(MCAN_IR_RF0N_MASK);
        }
    }
}

void processRxMsg(DL_MCAN_RxBufElement *rxMsg)
{
    uint32_t idMode;
    uint32_t id;

    idMode = rxMsg->xtd;

    if (ID_MODE_EXTENDED == idMode) {
        id = rxMsg->id;
    } else {
        /* Assuming package is using 11-bit standard ID.
         * When package uses standard id, ID is stored in ID[28:18]*/
        id = ((rxMsg->id & (uint32_t) 0x1FFC0000) >> (uint32_t) 18);
    }

    switch (id) {
        case 0x3:
            if (rxMsg->data[0] == LED_STATUS_ON) {
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            } else {
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            }
            break;
        case 0x4:
            if (rxMsg->data[0] == LED_STATUS_ON) {
                DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            } else {
                DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
            }
            break;
        default:
            /* Don't do anything */
            break;
    }
}

void MCAN0_INST_IRQHandler(void)
{
    switch (DL_MCAN_getPendingInterrupt(MCAN0_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gInterruptLine1Status |= DL_MCAN_getIntrStatus(MCAN0_INST);
            DL_MCAN_clearIntrStatus(MCAN0_INST, gInterruptLine1Status,
                DL_MCAN_INTR_SRC_MCAN_LINE_1);

            gServiceInt = true;
            break;
        default:
            break;
    }
}
