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

#define TIMER_CAPTURE_DURATION (CAPTURE_0_INST_LOAD_VALUE)
volatile bool gCheckEdgeCaptureTime;
int main(void)
{
    __attribute__((unused)) volatile static uint32_t edgeCapture;
    SYSCFG_DL_init();

    NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);

    gCheckEdgeCaptureTime = false;
    DL_TimerG_startCounter(CAPTURE_0_INST);

    /*
    * LED_1 is turned on to indicate edge capture has started
    */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    while (1) {
        while (false == gCheckEdgeCaptureTime) {
            __WFE();
        }
        gCheckEdgeCaptureTime = false;

        /*
         * When the timer is operating in DL_TIMER_CAPTURE_MODE_EDGE_TIME mode,
         * the timer counts from TIMER_CAPTURE_DURATION to 0 (timer is
         * configured in count down mode)
         * When an edge is detected the current timer count is loaded in to the
         * timer CaptureCompare register.
         */
        edgeCapture =
            TIMER_CAPTURE_DURATION - (DL_Timer_getCaptureCompareValue(
                                         CAPTURE_0_INST, DL_TIMER_CC_0_INDEX));

        /*
         * LED_1 is toggled every time a capture is detected
         */
        DL_GPIO_togglePins(GPIO_LEDS_PORT,
            GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

        __BKPT(0);

        /* Setting timer count manually due to TIMER_ERR_01 errata */
        DL_TimerG_setTimerCount(CAPTURE_0_INST, TIMER_CAPTURE_DURATION);
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_0_INST)) {
        case DL_TIMERG_IIDX_CC0_DN:
            gCheckEdgeCaptureTime = true;
            break;
        default:
            break;
    }
}
