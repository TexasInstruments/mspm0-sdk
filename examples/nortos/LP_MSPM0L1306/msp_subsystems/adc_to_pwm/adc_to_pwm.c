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

#define  ADC_MINIMUM_VALUE 500  

#include "ti_msp_dl_config.h"

volatile bool gCheckADC;
volatile uint32_t gCCV;
volatile uint16_t gAdcResult;

int main(void) {

    SYSCFG_DL_init();
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_Timer_setCaptCompUpdateMethod(PWM_0_INST, GPTIMER_CCCTL_01_CCUPD_ZERO_EVT, DL_TIMER_CC_0_INDEX);

    /* Start our ADC conversion and PWM Timers */
    DL_Timer_startCounter(TIMER_0_INST);
    DL_Timer_startCounter(PWM_0_INST);

    gCheckADC = false;

    while (1) 
    {
    __WFI();

    if (gCheckADC) {
    
        gCheckADC = false;
        gAdcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        /* Check if the gAdcResult is above the minmum threshold */
            if (gAdcResult > ADC_MINIMUM_VALUE)
            {
                /* Start counter in case it was previously stopped. */
                DL_Timer_startCounter(PWM_0_INST);

                /* Do necessary ADC-to-PWM scaling here: */

                /* Here we just map the ADC Result from 0 to 100% of the Load Value */
                gCCV = DL_Timer_getLoadValue(PWM_0_INST)*gAdcResult/4096;
                /* CC Value will update on the next zero event */
                DL_TimerG_setCaptureCompareValue(PWM_0_INST, DL_Timer_getLoadValue(PWM_0_INST)-gCCV, DL_TIMER_CC_0_INDEX);
            }
            else 
            {
                /* If gAdcResult is not above minmum value then disable timer */
                DL_TimerG_stopCounter(PWM_0_INST);
            }
        }
    }
}

void ADC12_0_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
  case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
    gCheckADC = true;
    break;
  default:
    break;
  }
  DL_ADC12_enableConversions(ADC12_0_INST);
}
