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

/*
 * This example setups OPA0 to dynamically change gain via internal PGA of input signal. The ADC12's Window Comparator mode is utilized
 * to bound the ADC results for GAIN changes. All settings initialized via Sysconfig. ADC120 is used along with OPA0 in non-inverting configuration.
 * Application specific code needs to be added  appropriate maths on ADC results.
 */

#include "ti_msp_dl_config.h"

#define HIGHMARGIN 3890 // 4095*0.75 = 75% of max ADC value
#define LOWMARGIN  1638 // 4095*0.25 = 25% of max ADC value
#define MAXGAIN  DL_OPA_GAIN_N7_P8 // Maximum GAIN level of OPA wanted
#define MINGAIN  DL_OPA_GAIN_N1_P2 // Minimum GAIN level of OPA wanted.
//For non-inverting PGA mode this is an OPA GAIN of 2x. See advisory in TRM for MIN GAIN.

int main(void)
{
    SYSCFG_DL_init();

    //Set ADC Window Compare thresholds
    DL_ADC12_disableConversions(ADC12_0_INST);
    DL_ADC12_configWinCompHighThld(ADC12_0_INST, HIGHMARGIN);
    DL_ADC12_configWinCompLowThld(ADC12_0_INST, LOWMARGIN);
    DL_ADC12_enableConversions(ADC12_0_INST);

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_ADC12_startConversion(ADC12_0_INST);

    DL_SYSCTL_enableSleepOnExit();

    while (1) {
        __WFI();
    }
}


void ADC12_0_INST_IRQHandler(void)
{
    uint32_t tempGain = 0;

    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_WINDOW_COMP_HIGH:
            // Entered high side margin window. Decrease OPA GAIN if possible.
            tempGain = DL_OPA_getGain(OPA_0_INST);
            if(tempGain > MINGAIN){
                //Update OPA gain.
                DL_OPA_decreaseGain(OPA_0_INST);
               //For full applications, at this point you would want to adjust any math factors or
               //look up tables to the new voltage ranges being captured by the ADC, or set a flag to do so in main while loop.
            }
            break;
        case DL_ADC12_IIDX_WINDOW_COMP_LOW:
            // Entered low side margin window. Increase OPA GAIN if possible.
            tempGain = DL_OPA_getGain(OPA_0_INST);
            if(tempGain < MAXGAIN){
               //Update OPA gain.
               DL_OPA_increaseGain(OPA_0_INST);
               //For full applications, at this point you would want to adjust any math factors or
               //look up tables to the new voltage ranges being captured by the ADC, or set a flag to do so in main while loop.
            }
            break;
        default:
            break;
    }
}
