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

#define LED0_STATUS_ON ((uint8_t) 0x01)
#define LED0_STATUS_OFF ((uint8_t) 0x00)
#define LED1_STATUS_ON ((uint8_t) 0x01)
#define LED1_STATUS_OFF ((uint8_t) 0x00)

volatile bool gTXMsg;
volatile bool error;

int main(void)
{
    DL_MCAN_TxBufElement txMsg0;
    DL_MCAN_TxBufElement txMsg1;

    SYSCFG_DL_init();

    /* Initialize message0 to transmit. */

    /* Identifier Value. */
    txMsg0.id = ((uint32_t)(0x4)) << 18U;
    /* Transmit data frame. */
    txMsg0.rtr = 0U;
    /* 11-bit standard identifier. */
    txMsg0.xtd = 0U;
    /* ESI bit in CAN FD format depends only on error passive flag. */
    txMsg0.esi = 0U;
    /* Transmitting 4 bytes. */
    txMsg0.dlc = 1U;
    /* CAN FD frames transmitted with bit rate switching. */
    txMsg0.brs = 1U;
    /* Frame transmitted in CAN FD format. */
    txMsg0.fdf = 1U;
    /* Store Tx events. */
    txMsg0.efc = 1U;
    /* Message Marker. */
    txMsg0.mm = 0xAAU;
    /* Data bytes. */
    txMsg0.data[0] = LED0_STATUS_ON;

    /* Initialize message1 to transmit. */

    /* Identifier Value. */
    txMsg1.id = ((uint32_t)(0x3)) << 18U;
    /* Transmit data frame. */
    txMsg1.rtr = 0U;
    /* 11-bit standard identifier. */
    txMsg1.xtd = 0U;
    /* ESI bit in CAN FD format depends only on error passive flag. */
    txMsg1.esi = 0U;
    /* Transmitting 4 bytes. */
    txMsg1.dlc = 1U;
    /* CAN FD frames transmitted with bit rate switching. */
    txMsg1.brs = 1U;
    /* Frame transmitted in CAN FD format. */
    txMsg1.fdf = 1U;
    /* Store Tx events. */
    txMsg1.efc = 1U;
    /* Message Marker. */
    txMsg1.mm = 0xAAU;
    /* Data bytes. */
    txMsg1.data[0] = LED1_STATUS_ON;

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    while (1) {
        /* Waits until button is pressed to send the message*/
        while (gTXMsg == false) {
            __WFE();
        }
        gTXMsg = false;

        if (txMsg0.data[0] == LED0_STATUS_ON) {
            txMsg0.data[0] = LED0_STATUS_OFF;
        } else {
            txMsg0.data[0] = LED0_STATUS_ON;
        }

        if (txMsg1.data[0] == LED1_STATUS_ON) {
            txMsg1.data[0] = LED1_STATUS_OFF;
        } else {
            txMsg1.data[0] = LED1_STATUS_ON;
        }

        /* Write Tx Message to the Message RAM. */
        DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0, &txMsg0);

        /* Write Tx Message to the Message RAM. */
        DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 1, &txMsg1);

        /* Add request for transmission. */
        DL_MCAN_TXBufAddReq(MCAN0_INST, 0);

        /* Add request for transmission. */
        DL_MCAN_TXBufAddReq(MCAN0_INST, 1);
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case DL_GPIO_IIDX_DIO21:
                    gTXMsg = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
