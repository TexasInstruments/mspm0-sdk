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

uint8_t gWelcomeMsg[] = "\r\n==== MSPM0 Console Test ====\r\n";
uint8_t gTestingMsg[] = "> Testing...0\r\n";

volatile bool gConsoleTxTransmitted, gConsoleTxDMATransmitted, gTransmitReady;
void UART_Console_write(const uint8_t *data, uint16_t size)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(data));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->TXDATA));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, size);

    DL_SYSCTL_disableSleepOnExit();

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    while (false == gConsoleTxDMATransmitted) {
        __WFE();
    }

    while (false == gConsoleTxTransmitted) {
        __WFE();
    }

    gConsoleTxTransmitted    = false;
    gConsoleTxDMATransmitted = false;
}

int main(void)
{
    gTransmitReady           = false;
    gConsoleTxTransmitted    = false;
    gConsoleTxDMATransmitted = false;
    uint32_t increment       = 0;

    SYSCFG_DL_init();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);


    /* Write welcome message */
    UART_Console_write(&gWelcomeMsg[0], sizeof(gWelcomeMsg));

    while (1) {
        if (gTransmitReady == true) {
            /* Will output "Testing...0", "Testing...1", etc. and loop after 9 */
            gTransmitReady  = false;
            gTestingMsg[12] = '0' + increment;
            increment       = (increment + 1) % 10;
            UART_Console_write(&gTestingMsg[0], sizeof(gTestingMsg));
        } else {
            __WFI();
        }
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_EOT_DONE:
            gConsoleTxTransmitted = true;
            break;
        case DL_UART_MAIN_IIDX_DMA_DONE_TX:
            gConsoleTxDMATransmitted = true;
            break;
        default:
            break;
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            gTransmitReady = true;
            DL_GPIO_togglePins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
            break;
        default:
            break;
    }
}
