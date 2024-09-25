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

volatile bool gWaitForNormalResult, gWaitForBcsResult;
volatile uint16_t gAdcNormalResult, gAdcBcsResult;

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    while (1) {
        /* Start a normal conversion of external signal and wait for result */
        DL_ADC12_disableConversions(ADC12_0_INST);
        DL_ADC12_configConversionMem(ADC12_0_INST, DL_ADC12_MEM_IDX_0,
            DL_ADC12_INPUT_CHAN_13, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
            DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
            DL_ADC12_AVERAGING_MODE_DISABLED,
            DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
            DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
        DL_ADC12_enableConversions(ADC12_0_INST);
        gWaitForNormalResult = true;
        DL_ADC12_startConversion(ADC12_0_INST);

        while (true == gWaitForNormalResult) {
            __WFE();
        }

        /* Now do a conversion enabling burnout current source */
        DL_ADC12_disableConversions(ADC12_0_INST);
        DL_ADC12_configConversionMem(ADC12_0_INST, DL_ADC12_MEM_IDX_0,
            DL_ADC12_INPUT_CHAN_13, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
            DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
            DL_ADC12_AVERAGING_MODE_DISABLED, DL_ADC12_BURN_OUT_SOURCE_ENABLED,
            DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
            DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
        DL_ADC12_enableConversions(ADC12_0_INST);
        gWaitForBcsResult = true;
        DL_ADC12_startConversion(ADC12_0_INST);

        while (true == gWaitForBcsResult) {
            __WFE();
        }

        __BKPT(0);
    }
}

void ADC0_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            if (gWaitForNormalResult == true) {
                gAdcNormalResult =
                    DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);
                gWaitForNormalResult = false;
            } else if (gWaitForBcsResult == true) {
                gAdcBcsResult =
                    DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);
                gWaitForBcsResult = false;
            }
            break;
        default:
            break;
    }
}
