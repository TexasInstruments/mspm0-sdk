/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include "MPESTI_Target.h"
#include "ti_msp_dl_config.h"

MPESTI_T_Params params;
MPESTI_T_DPR_PKT dprPkt;
void Broadcast_Handler(uint8_t command);
void VWE_Handler(void* target);
uint8_t data[4] = {0x5A, 0x5B, 0x5C, 0x5D};

uint8_t DPR_payload[32] = {0};
uint8_t rxBuffer[32]    = {0};
uint8_t txBuffer[32]    = {[0] = 1, [1] = 2, [2] = 3, [3] = 4, [4] = 5};

int main(void)
{
    params.targetUartBase        = UART_0_INST;
    params.readyToReceiveGpio    = GPIO_PESTI_Ready_PORT;
    params.readyToReceivePinMask = GPIO_PESTI_Ready_PESTI_Ready_PIN;
    params.Broadcast_Callback    = Broadcast_Handler;
    params.VWE_Callback          = VWE_Handler;
    params.rxBuffPtr             = rxBuffer;
    params.targetDPR             = &dprPkt;

    params.targetDPR->DPR_payload      = DPR_payload;
    params.targetDPR->vendorData       = data;
    params.targetDPR->vendorDataSize   = 4;
    params.targetDPR->noOfPaddingBytes = 3;

    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    MPESTI_T_init(&params);

    while (1) {
        MPESTI_T_stateMachine();

        switch (MPESTI_T_getCurrentState()) {
            case MPESTI_T_DPR:
                DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_Red_Led_PIN);
                break;

            case MPESTI_T_VWE:
                DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_Blue_Led_PIN);
                break;

            default:
                DL_GPIO_clearPins(GPIO_LED_PORT,
                    GPIO_LED_Blue_Led_PIN | GPIO_LED_Red_Led_PIN);
                break;
        };
    }
}

void Broadcast_Handler(uint8_t command)
{
    /* The range of the command value is from 0-255 which can be check
     * here and user can perform any action based on the check*/
    /* This broadcast command is being called from ISR so it is expected
     * to be smaller. It can set a flag or a trigger which can execute
     * that expected task*/
    if (command == 0) {
        /* Toggle the green led */
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_Green_Led_PIN);
    }
}

void VWE_Handler(void* target)
{
    /* The receive data can be parsed here and according to that the
     * tx packet can be prepared.
     */
    MPESTI_Target* MPESTITarget = (MPESTI_Target*) target;
    MPESTITarget->nwk.txBuffPtr = txBuffer;
    MPESTITarget->nwk.txLen     = 5;
    MPESTITarget->nwk.txIndex   = 0;
    MPESTITarget->nwk.rxLen     = MPESTITarget->nwk.txLen;
}

void UART_0_INST_IRQHandler(void)
{
    MPESTI_T_uartInterruptHandler();
}

void GROUP1_IRQHandler(void)
{
    uint32_t interrupts = DL_Interrupt_getStatusGroup(DL_INTERRUPT_GROUP_1,
        (DL_INTERRUPT_GROUP1_GPIOA | DL_INTERRUPT_GROUP1_GPIOB));
    if (interrupts & DL_INTERRUPT_GROUP1_GPIOA) {
        DL_Interrupt_clearGroup(
            DL_INTERRUPT_GROUP_1, DL_INTERRUPT_GROUP1_GPIOA);
    } else if (interrupts & DL_INTERRUPT_GROUP1_GPIOB) {
        DL_Interrupt_clearGroup(
            DL_INTERRUPT_GROUP_1, DL_INTERRUPT_GROUP1_GPIOB);
    }
}
