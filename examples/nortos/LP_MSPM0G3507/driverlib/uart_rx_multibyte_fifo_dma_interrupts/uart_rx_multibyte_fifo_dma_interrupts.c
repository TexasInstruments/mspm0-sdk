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

/* Number of bytes for UART packet size */
#define UART_PACKET_SIZE (4)

/* Data received from UART */
volatile uint8_t gRxPacket[UART_PACKET_SIZE];

volatile bool gCheckUART;

int main(void)
{
    SYSCFG_DL_init();

    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gRxPacket[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, UART_PACKET_SIZE);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /* Confirm DMA channel is enabled */
    while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH0_CHAN_ID)) {
        __BKPT(0);
    }
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    gCheckUART = false;
    DL_SYSCTL_disableSleepOnExit();
    /* Wait in SLEEP mode until DMA interrupt is triggered */
    while (false == gCheckUART) {
        __WFE();
    }

    DL_SYSCTL_enableSleepOnExit();
    while (1) {
        /* LED will turn ON to indicate example has completed without error */
        DL_GPIO_clearPins(GPIO_LEDS_PORT,
            (GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN));

        /*
        * If this example is used with the uart_tx_multibyte_fifo_dma_interrupts
        * example, the expected data to receive in gRxPacket is:
        * {'M', 'S', 'P', '!'}
        */
        __WFI();
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_DMA_DONE_RX:
            gCheckUART = true;
            break;
        default:
            break;
    }
}
