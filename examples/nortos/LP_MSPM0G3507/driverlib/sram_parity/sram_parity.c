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

/* SRAM will be initialized if value is 1 */
#ifndef SRAM_PARITY_INITIALIZE
#define SRAM_PARITY_INITIALIZE (1)
#endif
/* Start of SRAM which will be initialized manually */
#define SRAM_PARITY_START (0x20100000)
/* Start of SRAM Parity codes */
#define SRAM_PARITY_CODE_START (0x20300000)
/* Size of SRAM to initialize in bytes. Avoid corrupting the stack */
#define SRAM_PARITY_SIZE (0x7E00)
/* Offset of SRAM being tested */
#define SRAM_PARITY_TEST_ADDRESS_OFFSET (0x5000)

/*
 * This function is called during initialization, and provides a place to
 * perform application-specific initialization immediately after reset.
 * It is invoked after the stack pointer is initialized but before any C
 * environment setup is performed.
 * The default C environment setup is bypassed if this function returns 0.
 */
#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
uint32_t _system_pre_init(void)
#elif defined(__IAR_SYSTEMS_ICC__)
int __low_level_init(void)
#else
#error "Compiler not supported for this example"
#endif
{
#if (SRAM_PARITY_INITIALIZE == 1)
    uint32_t *sramPtr = (uint32_t *) SRAM_PARITY_START;
    uint32_t i;

    /* Memory should be aligned to 32-bits using this implementation */
    for (i = 0; i < SRAM_PARITY_SIZE / 4; i++) {
        *sramPtr++ = 0x00;
    }
#endif

    return 1;
}

volatile uint32_t gValue;
volatile bool gParityFault;


int main(void)
{
    SYSCFG_DL_init();
    gParityFault = false;

    /*
     * If SRAM_PARITY_INITIALIZE is set to 0, then parity SRAM will not be
     * initialized. Attempting to read parity SRAM will result in a NMI
     * triggering with parity fault. Otherwise code will run normally.
     */
    gValue =
        *(uint32_t *) (SRAM_PARITY_START + SRAM_PARITY_TEST_ADDRESS_OFFSET);

    /*
     * If SRAM was initialized by setting SRAM_PARITY_INITIALIZE to 1,
     * then breakpoint should be reached and gValue should be 0x00000000.
     */
    __BKPT(0);

    /* Write to parity region */
    *(uint32_t *) (SRAM_PARITY_CODE_START + SRAM_PARITY_TEST_ADDRESS_OFFSET) =
        0xAA55AA55;

    /*
     * Reading the same location will result in a fault since parity was modified
     */
    gValue =
        *(uint32_t *) (SRAM_PARITY_START + SRAM_PARITY_TEST_ADDRESS_OFFSET);

    while (1) {
        __WFI();
    }
}

void NMI_Handler(void)
{
    switch (DL_SYSCTL_getPendingNonMaskableInterrupt()) {
        case DL_SYSCTL_NMI_IIDX_SRAM_DED:
            /* NMI will trigger with parity fault */
            gParityFault = true;

            DL_GPIO_clearPins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

            break;
        default:
            break;
    }
}
