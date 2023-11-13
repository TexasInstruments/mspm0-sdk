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
 *
 **************************************************************************************
 * 
 * Shown is the external circuitry necessary for this code:
 * *Rg can be changed for a different gain*
 *                                                                                        
 *                                            |--------Rg (3.9k)--------|          (Vo)
 *                                            |                         |--R2(20k)--|
 *                                            |--R3(5.1k)--|--R1(5.1k)--|           |
 *               MSPM0G         GND--R4(20k)--|            |            |           |
 *       ----------------------               |            |            |           |
 *      |                      |              |            |            |           |
 *      |        OPA0.IN0+/PA26|<----<        |            |            |           |
 *      |        OPA0.IN0-/PA27|-------------->            |            |           |
 *      |         OPA0.OUT/PA22|--------------------------->            |           |
 *      |        OPA1.IN0-/PB20|---------------------------------------->           |
 *      |   v---< OPA1.OUT/PA16|---------------------------------------------------->
 *      |   |    OPA1.IN0+/PB19|<----<
 *      |   >--------> ADC/PA17|
 *      |                      |
 *       ----------------------
 * 
 ***************************************************************************************
 */
 
#include "ti_msp_dl_config.h"
volatile uint16_t gCircuitADCResult = 0;


int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(ADC_INST_INT_IRQN);

    /* This timer will initialize the ADC reading of OPA1 output */
    DL_TimerG_startCounter(TIMER0_INST);

    while (1) {
    }
}

void ADC_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_INST)){
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            /* This variable is collecting the ADC results */
            gCircuitADCResult = 
                DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_0);
            DL_ADC12_enableConversions(ADC_INST);
            break;
        default:
            break;
    }
}
