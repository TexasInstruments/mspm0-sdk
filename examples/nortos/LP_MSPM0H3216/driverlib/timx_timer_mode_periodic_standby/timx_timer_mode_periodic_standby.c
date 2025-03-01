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

/* ((32KHz / (32+1)) * 0.5s) = 45 - 1 = 495 due to N+1 ticks */
#define TIMER_500_MILLISECONDS_TICKS (495)
/* ((32KHz / (32+1)) * 0.05s) = 50 */
#define TIMER_50_MILLISECONDS_TICKS (50)

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_SYSCTL_enableSleepOnExit();

    DL_TimerG_startCounter(TIMER_0_INST);

    while (1) {
        __WFI();
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    static uint32_t count = TIMER_500_MILLISECONDS_TICKS;
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            /*
             * Counter stopped to avoid a conflict with the timer reading
             * the LOAD value while it's being set
             */
                DL_TimerG_stopCounter(TIMER_0_INST);

            /*
             * Count progressively gets smaller in 0.05 s increments until
             * reset with 0.5s
             */
                if (count > (TIMER_500_MILLISECONDS_TICKS / 5)) {
                    count = count - TIMER_50_MILLISECONDS_TICKS;
                } else {
                    count = TIMER_500_MILLISECONDS_TICKS;
                }

                DL_Timer_setLoadValue(TIMER_0_INST, count);
            /*
             * By default, this should load the new count value and count down
             * from there (CVAE = 0)
             */
                DL_TimerG_startCounter(TIMER_0_INST);

            DL_GPIO_togglePins(GPIO_LEDS_PORT,
                (GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN));
            break;
        default:
            break;
    }
}
