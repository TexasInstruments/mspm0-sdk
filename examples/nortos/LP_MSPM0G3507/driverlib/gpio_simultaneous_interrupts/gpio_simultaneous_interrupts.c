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

int main(void)
{
    SYSCFG_DL_init();

    /* Enable Interrupt for both GPIOA and GPIOB ports */
    NVIC_EnableIRQ(GPIO_SWITCHES_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_SWITCHES_GPIOB_INT_IRQN);

    while (1) {
        __WFI();
    }
}

void GROUP1_IRQHandler(void)
{
    /*
     * Get the pending interrupt for the GPIOA port and store for
     * comparisons later
     */
    uint32_t gpioA = DL_GPIO_getEnabledInterruptStatus(GPIOA,
        GPIO_SWITCHES_USER_SWITCH_1_PIN | GPIO_SWITCHES_USER_SWITCH_3_PIN);

    /*
     * Bitwise AND the pending interrupt with the pin you want to check,
     * then check if it is equal to the pins. Clear the interrupt status.
     */
    if ((gpioA & GPIO_SWITCHES_USER_SWITCH_1_PIN) ==
        GPIO_SWITCHES_USER_SWITCH_1_PIN) {
        DL_GPIO_togglePins(
            GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
        DL_GPIO_clearInterruptStatus(GPIOA, GPIO_SWITCHES_USER_SWITCH_1_PIN);
    }

    if ((gpioA & GPIO_SWITCHES_USER_SWITCH_3_PIN) ==
        GPIO_SWITCHES_USER_SWITCH_3_PIN) {
        DL_GPIO_togglePins(
            GPIO_LEDS_USER_LED_3_PORT, GPIO_LEDS_USER_LED_3_PIN);
        DL_GPIO_clearInterruptStatus(GPIOA, GPIO_SWITCHES_USER_SWITCH_3_PIN);
    }

    /* Repeat with GPIOB Port */
    uint32_t gpioB = DL_GPIO_getEnabledInterruptStatus(
        GPIOB, GPIO_SWITCHES_USER_SWITCH_2_PIN);

    if ((gpioB & GPIO_SWITCHES_USER_SWITCH_2_PIN) ==
        GPIO_SWITCHES_USER_SWITCH_2_PIN) {
        DL_GPIO_togglePins(
            GPIO_LEDS_USER_LED_2_PORT, GPIO_LEDS_USER_LED_2_PIN);
        DL_GPIO_clearInterruptStatus(GPIOB, GPIO_SWITCHES_USER_SWITCH_2_PIN);
    }
}
