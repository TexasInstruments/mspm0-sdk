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

volatile uint32_t gCaptureCnt;
volatile bool gSynced;
volatile bool gCheckCaptures;
uint32_t gLoadValue;

int main(void)
{
    volatile static uint32_t pwmPeriod;
    __attribute__((unused)) volatile static uint32_t pwmDuty;

    SYSCFG_DL_init();

    /*
     * This value is used to reload timer manually. Due to timer capture
     * limitation
     */
    gLoadValue = DL_TimerG_getLoadValue(CAPTURE_0_INST);

    /* Initialize capture global states */
    gSynced        = false;
    gCheckCaptures = false;

    /*
     * Forcing timers to halt immediately to prevent timers getting out of sync
     * when code is halted
     */
    DL_TimerG_setCoreHaltBehavior(
        CAPTURE_0_INST, DL_TIMER_CORE_HALT_IMMEDIATE);
    DL_TimerG_setCoreHaltBehavior(PWM_0_INST, DL_TIMER_CORE_HALT_IMMEDIATE);

    NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);
    DL_TimerG_startCounter(CAPTURE_0_INST);
    DL_TimerG_startCounter(PWM_0_INST);

    while (1) {
        while (false == gCheckCaptures) {
            __WFE();
        }
        gCheckCaptures = false;

        /*
         * Calculate PWM period and PWM duty cycle. IMPORTANT: These calculation
         * assume timer is running in DOWN counting mode
         */
        pwmPeriod = gLoadValue - gCaptureCnt;
        pwmDuty   = ((gLoadValue - DL_TimerG_getCaptureCompareValue(
                                     CAPTURE_0_INST, DL_TIMER_CC_0_INDEX)) *
                      100) /
                  pwmPeriod;

        __BKPT(0);
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_0_INST)) {
        case DL_TIMERG_IIDX_CC1_DN:
            if (gSynced == true) {
                gCaptureCnt = DL_TimerG_getCaptureCompareValue(
                    CAPTURE_0_INST, DL_TIMER_CC_1_INDEX);
                gCheckCaptures = true;
            } else {
                gSynced = true;
            }
            /* Manual reload is needed to workaround timer capture limitation */
            DL_TimerG_setTimerCount(CAPTURE_0_INST, gLoadValue);
            break;
        case DL_TIMERG_IIDX_ZERO:
            /* If Timer reaches zero then no PWM signal is detected and it
             * requires re-synchronization
             */
            gSynced = false;
            break;
        default:
            break;
    }
}
