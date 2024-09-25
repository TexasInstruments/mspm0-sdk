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

#define NUM_CAPTURES (50)

volatile uint32_t gTRNGBuffer[NUM_CAPTURES];


int main(void)
{
    uint32_t i;
    /*
     * Initialize the peripherals and set the TRNG to the NORM_FUNC state
     * The LED (USER_LED_1) should be turned off
     */
    SYSCFG_DL_init();

    /* Setup and start a capture, then wait for the result */
    for (i = 0; i < NUM_CAPTURES; i++) {
        while (!DL_TRNG_isCaptureReady(TRNG))
            ;
        DL_TRNG_clearInterruptStatus(
            TRNG, DL_TRNG_INTERRUPT_CAPTURE_RDY_EVENT);
        gTRNGBuffer[i] = DL_TRNG_getCapture(TRNG);
    }

    /* Power off the peripheral */
    DL_TRNG_disablePower(TRNG);


    /*
     * Program completed. Since USER_LED_1 is PA0 and active low on M0G LP,
     * clearing the pin turns on the LED
     */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    /* Set a SW breakpoint. Check gTRNGBuffer is filled with random numbers */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}
