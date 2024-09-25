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

#define PWM_PERIOD_UPDATE_CNT (10U)

const uint32_t gPeriodVal[3] = {1599, 999, 99};
const uint32_t gDutyVal[3]   = {799, 499, 49};

volatile uint32_t gIndex;
volatile uint32_t gUpdateCnt;
int main(void)
{
    SYSCFG_DL_init();

    gIndex     = 0;
    gUpdateCnt = PWM_PERIOD_UPDATE_CNT;

    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);

    DL_TimerA_startCounter(PWM_0_INST);

    /*
     * USER_LED_1 is turned on and USER_TEST_PIN is set low to indicate
     * PWM generation has started
     */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    while (1) {
        __WFI();
    }
}

void PWM_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(PWM_0_INST)) {
        case DL_TIMERA_IIDX_CC0_DN:
            if (gUpdateCnt == 0) {
                gUpdateCnt = PWM_PERIOD_UPDATE_CNT;

                if (gIndex > ((sizeof(gPeriodVal) / sizeof(uint32_t)) - 1)) {
                    gIndex = 0;
                }
                /**
                 * Updates PWM period and duty cycle during a timer capture compare
                 * interrupt in the down counting mode. Since shadow load and shadow
                 * capture compare mode are configured to trigger during a timer
                 * zero event the PWM update is synchronized until timer reaches
                 * zero.
                 */
                DL_TimerA_setLoadValue(PWM_0_INST, gPeriodVal[gIndex]);
                DL_TimerA_setCaptureCompareValue(PWM_0_INST, gDutyVal[gIndex],
                    DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
                gIndex++;
            } else {
                gUpdateCnt--;
            }
        default:
            break;
    }
}
