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

/* Counter for sawtooth signal, it will increment to 4096, then restart. */
uint16_t gSawCounter = 0;
/* Increment for sawtooth signal. 4096/8 = 512 steps */
uint16_t gSawIncrement = 8;

int main(void)
{
    SYSCFG_DL_init();

    /* Start timer to trigger DAC */
    DL_TimerG_startCounter(TIMER_0_INST);
    /* Enable DAC interrupt to update output */
    NVIC_EnableIRQ(DAC12_INT_IRQN);

    /*
     * Send the signal to DAC output.
     *   The timing of the timer trigger can be configured in SysConfig.
     *   The signal frequency will depend on sample time generator and number
     *   of samples:
     *      Output signal frequency =     Timer frequency
     *                                  -------------------
     *                                    Number of samples
     */
    DL_SYSCTL_enableSleepOnExit();
    while (1) {
        __WFI();
    }
}
void DAC12_IRQHandler(void)
{
    switch (DL_DAC12_getPendingInterrupt(DAC0)) {
        case DL_DAC12_IIDX_FIFO_1_2_EMPTY:
            DL_DAC12_output12(DAC0, gSawCounter);
            gSawCounter += gSawIncrement;
            if (gSawCounter >= 4096) {
                gSawCounter = 0;
            }
            break;
        /* unused */
        case DL_DAC12_IIDX_FIFO_3_4_EMPTY:
        case DL_DAC12_IIDX_NO_INT:
        case DL_DAC12_IIDX_MODULE_READY:
        case DL_DAC12_IIDX_FIFO_FULL:
        case DL_DAC12_IIDX_FIFO_1_4_EMPTY:
        case DL_DAC12_IIDX_FIFO_EMPTY:
        case DL_DAC12_IIDX_FIFO_UNDERRUN:
        case DL_DAC12_IIDX_DMA_DONE:
            break;
    }
}
