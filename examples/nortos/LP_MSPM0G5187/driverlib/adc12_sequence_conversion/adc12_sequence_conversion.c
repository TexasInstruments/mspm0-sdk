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

volatile bool gCheckADC;

#define RESULT_SIZE (64)
volatile uint16_t gAdcResult0[RESULT_SIZE];
volatile uint16_t gAdcResult1[RESULT_SIZE];
volatile uint16_t gAdcResult2[RESULT_SIZE];
volatile uint16_t gAdcResult3[RESULT_SIZE];

int main(void)
{
    /* Initialize peripherals and enable interrupts */
    SYSCFG_DL_init();
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    gCheckADC  = false;
    uint16_t i = 0;

    while (1) {
        DL_ADC12_startConversion(ADC12_0_INST);

        /* Wait until all data channels have been loaded. */
        while (gCheckADC == false) {
            __WFE();
        }

        /* Store ADC Results into their respective buffer */
        gAdcResult0[i] =
            DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        gAdcResult1[i] =
            DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);
        gAdcResult2[i] =
            DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_2);
        gAdcResult3[i] =
            DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_3);

        i++;
        gCheckADC = false;
        /* Reset index of buffers, set breakpoint to check buffers. */
        if (i >= RESULT_SIZE) {
            __BKPT(0);
            i = 0;
        } else {
            ; /*No action required*/
        }
        DL_ADC12_enableConversions(ADC12_0_INST);
    }
}

/* Check for the last result to be loaded then change boolean */
void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM3_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
