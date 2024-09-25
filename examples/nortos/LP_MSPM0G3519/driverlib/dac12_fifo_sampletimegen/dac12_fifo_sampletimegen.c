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

/* Repetitive triangle signal */
const uint16_t gOutputSignalTriangle4[] = {0, 2048, 4095, 2048};

/* Repetitive sine wave */
const uint16_t gOutputSignalSine32[] = {2048, 2447, 2831, 3185, 3496, 3750,
    3940, 4056, 4095, 4056, 3940, 3750, 3496, 3185, 2831, 2447, 2048, 1648,
    1264, 910, 599, 345, 155, 39, 0, 39, 155, 345, 599, 910, 1264, 1648};

#define DAC12_REF_VOLTAGE_mV (3300)

/* Configure the signal to output on DAC */
#define OUTPUT_SIGNAL gOutputSignalSine32
#define OUTPUT_SIGNAL_SIZE (sizeof(OUTPUT_SIGNAL) / sizeof(uint16_t))

volatile uint32_t gCounter;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(DAC12_INT_IRQN);

    /*
     * Send the signal to DAC output.
     *   The timing of sample time generator can be configured in SysConfig.
     *   The signal frequency will depend on sample time generator and number
     *   of samples:
     *      Output signal frequency =  Sample Time Generator frequency
     *                                    ------------------------
     *                                      Number of samples
     */
    gCounter = 0;

    DL_SYSCTL_enableSleepOnExit();
    while (1) {
        __WFI();
    }
}

void DAC12_IRQHandler(void)
{
    switch (DL_DAC12_getPendingInterrupt(DAC0)) {
        case DL_DAC12_IIDX_FIFO_1_2_EMPTY:
            DL_DAC12_output12(DAC0, OUTPUT_SIGNAL[gCounter++]);
            if (gCounter >= OUTPUT_SIGNAL_SIZE) {
                gCounter = 0;
            }
            break;
        default:

            break;
    }
}
