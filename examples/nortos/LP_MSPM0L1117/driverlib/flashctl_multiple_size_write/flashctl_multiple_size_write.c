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

/* 8-bit data to write to flash */
uint8_t gData8 = 0x11;

/* 16-bit data to write to flash */
uint16_t gData16 = 0x2222;

/* 32-bit data to write to flash */
uint32_t gData32 = 0x33333333;

/* Array to write 64-bits to flash */
uint32_t gDataArray64[] = {0xABCDEF00, 0x12345678};

/* 32-bit data array to write to flash */
uint32_t gDataArray32[] = {0x00000000, 0x11111111, 0x22222222, 0x33333333,
    0x4444, 0x55555, 0x66, 0x77, 0x8, 0x9};

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

int main(void)
{
    SYSCFG_DL_init();

    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    /* Unprotect sector in main memory*/
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    /* Erase sector in main memory */
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
        /* If command was not successful, set a breakpoint */
        __BKPT(0);
    }

    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    /* 8-bit write to flash in main memory without ECC */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM8(
        FLASHCTL, MAIN_BASE_ADDRESS, &gData8);
    if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
        /* If command was not successful, set a breakpoint */
        __BKPT(0);
    }

    /*
     * 16-bit write to flash in main memory without ECC.
     * The target program address must be a flash word address
     * (8-byte aligned), so we increase the address in increments of 8.
     */

    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM16(
        FLASHCTL, (MAIN_BASE_ADDRESS + 8), &gData16);
    if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
        /* If command was not successful, set a breakpoint */
        __BKPT(0);
    }

    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    /* 32-bit write to flash in main memory without ECC */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM32(
        FLASHCTL, (MAIN_BASE_ADDRESS + 16), &gData32);
    if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
        /* If command was not successful, set a breakpoint */
        __BKPT(0);
    }

    /*
     * 64-bit write to flash in main memory without ECC.
     * Data must be loaded 32-bits at a time, but a single word program
     * is executed
     */

    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64(
        FLASHCTL, (MAIN_BASE_ADDRESS + 24), &gDataArray64[0]);
    if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
        __BKPT(0);
    }

    /* If writes successful, set LED */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    /*
     * Set a breakpoint to check the contents of MAIN_BASE_ADDRESS:
     * FF FF FF 11
     * FF FF FF FF
     * FF FF 22 22
     * FF FF FF FF
     * 33 33 33 33
     * FF FF FF FF
     * AB CD EF 00
     * 12 34 56 78
     */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}
