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

/*
 * This function is called during initialization, and provides a place to
 * perform application-specific initialization immediately after reset.
 * It is invoked after the stack pointer is initialized but before any C
 * environment setup is performed.
 * The default C environment setup is bypassed if this function returns 0.
 */
#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
uint32_t _system_pre_init(void)
{
    return 0;
}
#elif defined(__IAR_SYSTEMS_ICC__)
int __low_level_init(void)
{
    return 0;
}
#else
#error "Compiler not supported for this example"
#endif

/*
 * Length-10 array in global SRAM. This array is *not* initialized to 0
 * via the .bss table as the C standard calls for, due to pre-init function
 * returning 0 and therefore bypassing C init, so gByteArray will have
 * retained whatever state was previously in SRAM.
 */
uint8_t gByteArray[10];


int main(void)
{
    /* Initialize the peripherals. */
    SYSCFG_DL_init();

    /* Manually initialize gByteArray */
    for (int i = 0; i < 10; i++) {
        gByteArray[i] = i;
    }


    /* Program completed. The LED should turn on and USER_TEST pin cleared */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    while (1) {
        __WFI();
    }
}
