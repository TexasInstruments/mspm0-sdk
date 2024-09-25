/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

volatile bool gCheckRXMessage;
volatile uint8_t gMsgCount;
volatile bool gError;
volatile bool gTransmitComplete;

#define DL_MCAN_MSG_INT (0x80200)


int main(void)
{
    DL_MCAN_TxBufElement txMsg;
    DL_MCAN_RxBufElement rxMsg;
    DL_MCAN_RxNewDataStatus newData;

    SYSCFG_DL_init();

    /* Initialize message to transmit. */

    /* Identifier Value. */
    txMsg.id = ((uint32_t)(0x4)) << 18U;
    /* Transmit data frame. */
    txMsg.rtr = 0U;
    /* 11-bit standard identifier. */
    txMsg.xtd = 0U;
    /* ESI bit in CAN FD format depends only on gError passive flag. */
    txMsg.esi = 0U;
    /* Transmitting 4 bytes. */
    txMsg.dlc = 4U;
    /* CAN FD frames transmitted with bit rate switching. */
    txMsg.brs = 1U;
    /* Frame transmitted in CAN FD format. */
    txMsg.fdf = 1U;
    /* Store Tx events. */
    txMsg.efc = 1U;
    /* Message Marker. */
    txMsg.mm = 0xAAU;

    /* Data bytes. */
    txMsg.data[0] = 0x00;
    txMsg.data[1] = 0x34;
    txMsg.data[2] = 0x56;
    txMsg.data[3] = 0x78;

    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);

    gMsgCount = 0;

    while (1) {
        gError = false;

        /* Write Tx Message to the Message RAM. */
        DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 1U, &txMsg);

        /* Enable Transmission interrupt.*/
        DL_MCAN_TXBufTransIntrEnable(MCAN0_INST, 1U, 1U);

        /* Add request for transmission. */
        DL_MCAN_TXBufAddReq(MCAN0_INST, 1U);

        /*
         * Wait for flag interrupt to be triggered and flag to check received
         * message is set to true.
         */
        gCheckRXMessage = false;
        while (false == gCheckRXMessage) {
            __WFE();
        }

        /*
         * Extract received message from MCAN RAM RX buffer and copy content to
         * rxMsg
         */
        DL_MCAN_readMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0U,
            DL_MCAN_RX_FIFO_NUM_1, &rxMsg);

        DL_MCAN_getNewDataStatus(MCAN0_INST, &newData);

        DL_MCAN_clearNewDataStatus(MCAN0_INST, &newData);

        /*
         * Check that received data matches sent data.
         * Device will halt here during debug if data doesn't match.
         */
        if ((txMsg.data[0] != rxMsg.data[0]) ||
            (txMsg.data[1] != rxMsg.data[1]) ||
            (txMsg.data[2] != rxMsg.data[2]) ||
            (txMsg.data[3] != rxMsg.data[3])) {
            /* Trap code execution */
            gError = true;
        } else {
            /* Increment message count if message is received. */
            gMsgCount++;
            txMsg.data[0] = gMsgCount;
        }

        if (gError == true) {
            /* Clear pins on error */
            DL_GPIO_clearPins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
            while (gError == true) {
                __BKPT(0);
            }
        } else {
            /* Set pins on success */
            DL_GPIO_setPins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
        }
    }
}

void MCAN0_INST_IRQHandler(void)
{
    uint32_t intrStatus;

    switch (DL_MCAN_getPendingInterrupt(MCAN0_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            intrStatus =
                DL_MCAN_getIntrStatus(MCAN0_INST) & MCAN0_INST_MCAN_INTERRUPTS;
            DL_MCAN_clearIntrStatus(
                MCAN0_INST, intrStatus, DL_MCAN_INTR_SRC_MCAN_LINE_1);

            /*
             * Check if MCAN interrupts set correspond to a complete
             * transmission and reception of a CAN package.
             */
            if (DL_MCAN_MSG_INT != intrStatus) {
                if (gMsgCount == MCAN0_INST_MCAN_FIFO_1_NUM) {
                    gTransmitComplete = true;
                } else {
                    gError = true;
                }
            } else {
                gCheckRXMessage = true;
            }

            break;
        default:
            gError = true;
            break;
    }
}
