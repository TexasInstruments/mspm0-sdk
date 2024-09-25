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

#define DMA_FILL_SIZE_WORDS (16)

const uint32_t gFillPattern = 0xF711BEEF;
uint32_t gDstData[DMA_FILL_SIZE_WORDS];

volatile bool gChannel0InterruptTaken = false;
volatile bool gVerifyResult           = false;


int main(void)
{
    /*
     * Initialize the DMA peripheral and set up a transaction according to
     * the parameters defined in ti_msp_dl_config.h. The LED (USER_LED_1)
     * should be off.
     */
    SYSCFG_DL_init();

    /* Setup interrupts on device */
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(DMA_INT_IRQn);

    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, gFillPattern);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gDstData[0]);
    DL_DMA_setTransferSize(
        DMA, DMA_CH0_CHAN_ID, sizeof(gDstData) / sizeof(uint32_t));
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /* Start fill data */
    gChannel0InterruptTaken = false;
    DL_DMA_startTransfer(DMA, DMA_CH0_CHAN_ID);

    /* Wait for transfer */
    while (gChannel0InterruptTaken == false) {
        __WFE();
    }

    gVerifyResult = true;
    for (int i = 0; i < DMA_FILL_SIZE_WORDS; i++) {
        gVerifyResult &= (gDstData[i] == gFillPattern);
    }


    /* Program completed. The LED should turn on */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    /* Breakpoint to inspect verification result */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}

void DMA_IRQHandler(void)
{
    /* Example interrupt code -- just used to break the WFI in this example */
    switch (DL_DMA_getPendingInterrupt(DMA)) {
        case DL_DMA_EVENT_IIDX_DMACH0:
            gChannel0InterruptTaken = true;
            break;
        default:
            break;
    }
}
