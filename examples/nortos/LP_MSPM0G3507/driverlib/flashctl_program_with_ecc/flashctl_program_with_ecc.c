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

/* Address in main memory to write to */
#define MAIN_BASE_ADDRESS (0x00001000)

/* Array to write 64-bits to flash. Data must be loaded 32-bits at a time,
 * but a single word program is executed. */
uint32_t gDataArray64[] = {0xABCDEF00, 0x12345678};

/*
 * Array to write multiple 64-bit data words to flash. Data must be loaded
 * 32-bits at a time, but a single word program is executed.
 */
uint32_t gDataArray64MultipleWords1[] = {0xCAFEBABE, 0xDEADBEEF, 0xCAFEBABE,
    0xDEADBEEF, 0xCAFEBABE, 0xDEADBEEF, 0xCAFEBABE, 0xDEADBEEF, 0xCAFEBABE,
    0xDEADBEEF};

/*
 * Array to write multiple 64-bit data words to flash. Data must be loaded
 * 32-bits at a time, but a single word program is executed.
 */
uint32_t gDataArray64MultipleWords2[] = {0xDEADBEEF, 0xCAFEBABE, 0xDEADBEEF,
    0xCAFEBABE, 0xDEADBEEF, 0xCAFEBABE, 0xDEADBEEF, 0xCAFEBABE, 0xDEADBEEF,
    0xCAFEBABE};

/*
 * 8-bit ECC code array that corresponds with the data in
 * gDataArray64MultipleWords1
 */
uint8_t gECCArray[] = {0x67, 0xF9, 0xF6, 0x68, 0xEB};

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

int main(void)
{
    SYSCFG_DL_init();

    /* Erase sector in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /*
     * Program single flash word (64-bit data) write to flash in main memory
     * with ECC enabled. Data must be loaded 32-bits at a time, but a single
     * word program is executed. The flash controller hardware will handle
     * generating the 8-bit ECC code.
     */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64[0]);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /*
     * Set a breakpoint to check the contents of MAIN_BASE_ADDRESS:
     * AB CD EF 00
     * 12 34 56 78
     */
    __BKPT(0);

    /* Erase sector in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /*
     * Write five, 64-bit flash words, but one flash word is programmed at a
     * time. The flash controller hardware will handle generating the 8-bit
     * ECC code for each flash word.
     */
    gCmdStatus = DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64MultipleWords1[0],
        sizeof(gDataArray64MultipleWords1) / sizeof(uint32_t),
        DL_FLASHCTL_REGION_SELECT_MAIN);

    /*
     * Set a breakpoint to check the contents of MAIN_BASE_ADDRESS:
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     */
    __BKPT(0);

    /* Erase sector in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /*
     * Write five, 64-bit flash words, but one flash word is programmed
     * at a time. The corresponding ECC code for each flash word is
     * manually provided after overriding ECC generated by the flash
     * controller hardware
     */
    DL_FlashCTL_enableOverrideHardwareGeneratedECC(FLASHCTL);
    gCmdStatus = DL_FlashCTL_programMemoryBlockingFromRAM64WithECCManual(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64MultipleWords2[0],
        &gECCArray[0], sizeof(gDataArray64MultipleWords2) / sizeof(uint32_t),
        DL_FLASHCTL_REGION_SELECT_MAIN);

    DL_FlashCTL_disableOverrideHardwareGeneratedECC(FLASHCTL);

    /*
     * Set a breakpoint to check the contents of MAIN_BASE_ADDRESS:
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     * DE AD BE EF
     * CA FE BA BE
     */
    __BKPT(0);

    /* After completion, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}
