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

/* Array of 128 samples to select duty cycles to generate sine waveform for 1024 Period count (10 bit DAC resolution) */
#define ARRAY_SIZE 128

uint16_t gSine128[ARRAY_SIZE] = {
     512,537,562,587,611,636,660,684,
     707,730,753,774,796,816,836,855,
     873,890,907,922,937,950,963,974,
     984,993,1001,1008,1013,1017,1021,1022,
     1023,1022,1021,1017,1013,1008,1001,993,
     984,974,963,950,937,922,907,890,
     873,855,836,816,796,774,753,730,
     707,684,660,636,611,587,562,537,
     512,486,461,436,412,387,363,339,
     316,293,270,249,227,207,187,168,
     150,133,116,101,86,73,60,49,
     39,30,22,15,10,6,2,1,
     0,1,2,6,10,15,22,30,
     39,49,60,73,86,101,116,133,
     150,168,187,207,227,249,270,293,
     316,339,363,387,412,436,461,486};

/* Counter used to cycle through sine array */
volatile uint32_t gSineCounter = 0;


int main(void)
{
    SYSCFG_DL_init();

    /* Enable PWM interrupts */
    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);

    /* Start PWM counter */
    DL_TimerG_startCounter(PWM_0_INST);

    while(1){
        __WFI();
    }
}


void PWM_0_INST_IRQHandler(void){
    switch (DL_TimerG_getPendingInterrupt(PWM_0_INST)){
        case DL_TIMERG_IIDX_CC0_DN: /* Interrupt on CC0 Down Event */
            /* Set new Duty Cycle based on sine array sample value */
            DL_TimerG_setCaptureCompareValue(PWM_0_INST, gSine128[gSineCounter%128], DL_TIMER_CC_0_INDEX);

            /* Increment gSineCounter value */
            gSineCounter++;

            break;
        default:
            break;
    }
}

