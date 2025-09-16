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

volatile bool gTogglePolicy;

int main(void)
{
    SYSCFG_DL_init();

    gTogglePolicy = false;

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);

    DL_TimerG_startCounter(TIMER_0_INST);
    DL_TimerG_startCounter(TIMER_1_INST);

    while (1) {
        while (false == gTogglePolicy) {
            __WFE();
        }

        gTogglePolicy = false;

        DL_SYSCTL_setPowerPolicySTANDBY0();

        while (false == gTogglePolicy) {
            __WFE();
        }
        gTogglePolicy = false;

        DL_SYSCTL_setPowerPolicyRUN0SLEEP0();
    }
}

/**
 * TIMG2 is sourced from the SYSOSC, and can run in SLEEP but not STANDBY.
 * TIMG2 will toggle LED1 every 250ms during the SLEEP policy, and not toggle
 * when the device is in STANDBY.
 */
void TIMER_1_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST)) {
        case DL_TIMERG_IIDX_ZERO:
            DL_GPIO_togglePins(GPIO_LEDS1_PORT, GPIO_LEDS1_USER_LED_1_PIN);
            break;
        default:
            break;
    }
}

/**
 * TIMG1 is sourced from the low-frequency LFCLK, and therefore can run in all
 * states but SHUTDOWN. TIMG1 will toggle the LED2 constantly every 1000ms
 * during both SLEEP0 and STANDBY0.
 */
void TIMER_0_INST_IRQHandler(void)
{
    static uint32_t count = 5;
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMERG_IIDX_ZERO:
            DL_GPIO_togglePins(GPIO_LEDS2_PORT, GPIO_LEDS2_USER_LED_2_PIN);
            if (count == 1) {
                gTogglePolicy = true;
                count         = 5;
            } else {
                count--;
            }
            break;
        default:
            break;
    }
}
