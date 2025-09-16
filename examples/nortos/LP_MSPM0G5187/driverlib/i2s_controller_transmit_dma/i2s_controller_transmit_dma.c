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

#include "ti_msp_dl_config.h"

#define NUM_SAMPLES (32)
const int16_t DATA[NUM_SAMPLES] = {
    6392,
    12539,
    18204,
    23169,
    27244,
    30272,
    32137,
    32767,
    32137,
    30272,
    27244,
    23169,
    18204,
    12539,
    6392,
    0,
    -6393,
    -12540,
    -18205,
    -23170,
    -27245,
    -30273,
    -32138,
    -32767,
    -32138,
    -30273,
    -27245,
    -23170,
    -18205,
    -12540,
    -6393,
    -1,
};

volatile bool gDMATXDataTransferred;

int main(void)
{
    SYSCFG_DL_init();

    gDMATXDataTransferred = false;

    NVIC_ClearPendingIRQ(I2S_INST_IRQn);
    NVIC_EnableIRQ(I2S_INST_IRQn);

    /* Configure the DMA  */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &DATA[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&I2S_INST->TXDATA));
    DL_DMA_setTransferSize(
        DMA, DMA_CH0_CHAN_ID, sizeof(DATA) / sizeof(DATA[0]));
    /* Enable the DMA Channel  */
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /* Enable the DAI module to start transferring the data */
    DL_I2S_enable(I2S_INST);

    /* Wait till DMA transfers are done */
    while (false == gDMATXDataTransferred) {
        __WFE();
    }

    while (1) {
    }
}

void I2S_INST_IRQHandler(void)
{
    switch (DL_I2S_getPendingInterrupt(I2S_INST)) {
        case DL_I2S_IIDX_DMA_DONE_TX:
            /* DMA is done transferring data from buffer to TXFIFO */
            gDMATXDataTransferred = true;
            break;
        default:
            break;
    }
}
