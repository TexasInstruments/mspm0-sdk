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

#define MAIN_BASE_ADDRESS (0x00001000)

/* 8-bit data to write to flash */
uint8_t gData8 = 0x11;

/* 16-bit data to write to flash */
uint16_t gData16 = 0x2222;

/* 32-bit data to write to flash */
uint32_t gData32 = 0x33333333;

/* Array to write 64-bits to flash */
uint32_t gDataArray64[] = {0xABCDEF00, 0x12345678};

/* "Incorrect" 64-bit data array to demonstrate read verify error */
uint32_t gDataArray64Error[] = {0x00FEDCBA, 0x87654321};

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

int main(void)
{
    SYSCFG_DL_init();

    DL_FLASHCTL_FAIL_TYPE failType;

    /* Erase sector in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* 8-bit write to flash in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM8(
        FLASHCTL, MAIN_BASE_ADDRESS, &gData8);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* Read verify on the 8-bit data */
    gCmdStatus =
        DL_FlashCTL_readVerifyFromRAM8(FLASHCTL, MAIN_BASE_ADDRESS, &gData8);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* 16-bit write to flash in main memory  */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM16(
        FLASHCTL, (MAIN_BASE_ADDRESS + 8), &gData16);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* Read verify on the 16-bit data */
    DL_FlashCTL_readVerifyFromRAM16(
        FLASHCTL, (MAIN_BASE_ADDRESS + 8), &gData16);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* 32-bit write to flash in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM32(
        FLASHCTL, (MAIN_BASE_ADDRESS + 16), &gData32);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* Read verify on the 32-bit data */
    gCmdStatus = DL_FlashCTL_readVerifyFromRAM32(
        FLASHCTL, (MAIN_BASE_ADDRESS + 16), &gData32);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* 64-bit write to flash in main memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64(
        FLASHCTL, (MAIN_BASE_ADDRESS + 24), &gDataArray64[0]);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* Read verify on the 64-bit data */
    DL_FlashCTL_readVerifyFromRAM64(
        FLASHCTL, (MAIN_BASE_ADDRESS + 24), &gDataArray64[0]);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /* Demonstrate a failed read verify when data does not match */
    gCmdStatus = DL_FlashCTL_readVerifyFromRAM64(
        FLASHCTL, (MAIN_BASE_ADDRESS + 24), &gDataArray64Error[0]);
    failType = DL_FlashCTL_getFailureStatus(FLASHCTL);
    if ((gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_FAILED) ||
        (failType != DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR)) {
        /* If command did not fail, or if there is an unexpected failure, set
         * a SW breakpoint.
         */
        __BKPT(0);
    }

    /* After completion, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}
