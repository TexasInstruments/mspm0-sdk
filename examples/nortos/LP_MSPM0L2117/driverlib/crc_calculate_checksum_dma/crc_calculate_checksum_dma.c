/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#define CRC_NUM_OF_WORDS (6)
#define CRC_EXPECTED_RESULT (0x5B56)

uint16_t gData16[CRC_NUM_OF_WORDS] = {
    0x5812, 0x3B4A, 0xFECA, 0x0C74, 0x842F, 0x0000};
const uint16_t gCrcSeed = CRC_SEED;
volatile bool gCheckCRC;
int main(void)
{
    volatile uint16_t crcChecksum;
    __attribute__((unused)) volatile bool crcChecksumMatch;

    SYSCFG_DL_init();

    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gData16[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, DL_CRC_getCRCINAddr(CRC0));
    DL_DMA_setTransferSize(
        DMA, DMA_CH0_CHAN_ID, sizeof(gData16) / sizeof(uint16_t));
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /*
     * Confirm DMA channel is enabled before issuing a SW trigger of the DMA
     * channel
     */
    while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH0_CHAN_ID)) {
        __BKPT(0);
    }

    /* Configure device exception handling */
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(DMA_INT_IRQn);
    gCheckCRC = false;

    /* Trigger DMA via SW request */
    DL_DMA_startTransfer(DMA, DMA_CH0_CHAN_ID);

    /* Wait in SLEEP mode until DMA interrupt is triggered */
    while (false == gCheckCRC) {
        __WFE();
    }

    /* Check CRC checksum */
    crcChecksum = DL_CRC_getResult16(CRC0);

    if (CRC_EXPECTED_RESULT == crcChecksum) {
        crcChecksumMatch = true;
    } else {
        crcChecksumMatch = false;
    }

    /* Stop the debugger to verify crcChecksumMatch == true */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}

void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA)) {
        case DL_DMA_EVENT_IIDX_DMACH0:
            gCheckCRC = true;
            break;
        default:
            break;
    }
}
