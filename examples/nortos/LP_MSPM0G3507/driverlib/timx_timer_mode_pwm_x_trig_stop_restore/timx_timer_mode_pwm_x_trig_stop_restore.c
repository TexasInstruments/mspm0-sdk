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

/**
 * Timer clock configuration to be sourced by LFCLK running at 32.768 KHz
 * This configures timer to generate 1 sec delay
 * ((32768 Hz / divideRatio / prescale) * TARGET DELAY) = period
 * ((32768 Hz / 1 / 1) * 1s) = 32768 - 1 = 32767 due to N+1 ticks in down mode
 */
#define TIMER_1_SEC_DELAY (32767)

volatile bool gIsTimerExpired;
int main(void)
{
    bool isRetentionError;
    SYSCFG_DL_init();

    /* Enable TIM0 interrupts on device */
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

    gIsTimerExpired  = false;
    isRetentionError = true;

    /* Start 1 sec timeout and PWM signals */
    DL_TimerG_startCounter(TIMER_0_INST);

    /* Turn User Test Pin On */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);

    DL_TimerA_generateCrossTrigger(PWM_0_INST);

    while (false == gIsTimerExpired) {
        __WFE();
    }
    /* Turn User Test Pin Off */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);

    /* Make sure instances to be saved are in IDLE state */
    DL_TimerA_stopCounter(PWM_0_INST);
    DL_TimerG_stopCounter(PWM_1_INST);

    if (true == SYSCFG_DL_saveConfiguration()) {
        gIsTimerExpired = false;

        /**
         * Allow device to go to STOP mode. This will cause PWM to stop being
         * generated */
        DL_SYSCTL_setPowerPolicySTOP0();

        /**
         *  Generate a 1 second delay to confirm actual transition to
         *  STOP mode */
        DL_TimerG_setTimerCount(TIMER_0_INST, TIMER_1_SEC_DELAY);
        DL_TimerG_startCounter(TIMER_0_INST);

        /* Turn User Test Pin On */
        DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
        while (false == gIsTimerExpired) {
            __WFE();
        }
        /* Turn User Test Pin Off */
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);

        if (true == SYSCFG_DL_restoreConfiguration()) {
            isRetentionError = false;

            DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

            /**
             * Allow device to go to SLEEP mode to allow generation of PWMs in
             * SLEEP mode */
            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

            DL_TimerA_generateCrossTrigger(PWM_0_INST);
        }
    }

    if (true == isRetentionError) {
        /* This breakpoint is reached if timer save/restore failed */
        __BKPT(0);
    }

    while (1) {
        __WFI();
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            gIsTimerExpired = true;
            break;
        default:
            break;
    }
}
