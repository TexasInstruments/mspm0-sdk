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
    volatile DL_SYSCTL_RESET_CAUSE rstCause;
    uint8_t counter;
    uint8_t blink;

    /*
     * Initialize GPIOs after reset, or re-initialize after waking-up from
     * SHUTDOWN.
     *
     * GPIOs will retain their last state prior to entry into SHUTDOWN and
     * they need to be re-configured before releasing them by calling
     * DL_SYSCTL_enableShutdownIORelease
     */
    SYSCFG_DL_init();

    rstCause = DL_SYSCTL_getResetCause();

    if (DL_SYSCTL_RESET_CAUSE_BOR_WAKE_FROM_SHUTDOWN == rstCause)
    /* Device was woken up from a SHUTDOWN state from this GPIO pin */
    {
        /* Release IO after Shutdown before initializing any peripherals */
        SYSCFG_DL_GPIO_init();
        DL_SYSCTL_releaseShutdownIO();

        /* Load save state after wake from SHUTDNSTORE */
        counter = 1 + DL_SYSCTL_getShutdownStorageByte(
                          DL_SYSCTL_SHUTDOWN_STORAGE_BYTE_0);

        /* Blink LED a number of times equal to counter */
        for (blink = 0; blink < (2 * counter); blink++) {
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            delay_cycles(16000000);
        }
    } else
    /* Device was NOT woken up from a SHUTDOWN state from this GPIO pin */
    {
        counter = 0;
    }

    /**
     * Resolve which Power policy to enable based on GPIO_INPUT_CONFIG_0_PIN and
     * GPIO_INPUT_CONFIG_1_PIN state.
     */
    switch (DL_GPIO_readPins(GPIO_INPUT_PORT,
        (GPIO_INPUT_CONFIG_0_PIN | GPIO_INPUT_CONFIG_1_PIN))) {
        /**
         * GPIO_INPUT_CONFIG_0_PIN and GPIO_INPUT_CONFIG_1_PIN are not connected
         * (since inputs are internally pulled high the pins will be at Vcc).
         */
        case (GPIO_INPUT_CONFIG_0_PIN | GPIO_INPUT_CONFIG_1_PIN):
            DL_SYSCTL_setPowerPolicySHUTDOWN();
            break;
        /**
         *  GPIO_INPUT_CONFIG_0_PIN  is not connected and
         *  GPIO_INPUT_CONFIG_1_PIN is connected to ground.
         */
        case (GPIO_INPUT_CONFIG_0_PIN):
            DL_SYSCTL_setPowerPolicySTOP0();
            break;
        /**
         *  GPIO_INPUT_CONFIG_0_PIN is connected to ground and
         *  GPIO_INPUT_CONFIG_1_PIN is not connected.
         */
        case (GPIO_INPUT_CONFIG_1_PIN):
            DL_SYSCTL_setPowerPolicySTANDBY0();
            break;
        /**
         * GPIO_INPUT_CONFIG_0_PIN and GPIO_INPUT_CONFIG_1_PIN are connected to
         * ground
         */
        default:
            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();
            break;
    }

    /* Save application state before shutdown using SHUTDNSTORE */
    DL_SYSCTL_setShutdownStorageByte(
        DL_SYSCTL_SHUTDOWN_STORAGE_BYTE_0, counter);

    while (1) {
        __WFI(); /* Enter selected power policy */
    }
}
