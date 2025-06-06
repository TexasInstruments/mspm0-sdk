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

int pwm_count = 1800; // initial ccr count based on 10% duty cycle
int dc = 10;          // initialized to 10 % duty cycle
int mode = 1; // 1 = up (will indicate you need to increase dc), 0 = down (will
              // decrease dc)

int main(void) {
  SYSCFG_DL_init();

  NVIC_EnableIRQ(PWM_1_INST_INT_IRQN);
  DL_TimerA_startCounter(PWM_1_INST);

  while (1) {
    __WFI();
  }
}

void PWM_1_INST_IRQHandler(void) {
  switch (DL_TimerA_getPendingInterrupt(PWM_1_INST)) {
  case DL_TIMER_IIDX_LOAD:
    if (dc <= 10) {
      mode = 1;
    } // if reached lowest dc (10%), increase dc
    else if (dc >= 90) {
      mode = 0;
    } // if reached highest dc (90%), decrease dc
    if (mode) {
      pwm_count -= 20;
      dc += 1;
    } // up
    if (!mode) {
      pwm_count += 20;
      dc -= 1;
    } // down
    DL_TimerA_setCaptureCompareValue(PWM_1_INST, pwm_count,
                                     DL_TIMER_CC_0_INDEX); // update ccr0 value
    break;
  default:
    break;
  }
}
