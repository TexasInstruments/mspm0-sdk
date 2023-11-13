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

/* Address in main memory to write to */
#define MAIN_BASE_ADDRESS (0x00001000)

/* 32-bit data to write to flash */
uint32_t gData32 = 0x12345678;

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;


/* Codes to understand where error occured */
#define NO_ERROR 0
#define ERROR_ERASE 1
#define ERROR_PROTECT 2
#define ERROR_UNPROTECT 3
#define ERROR_REPROTECT 4
#define ERROR_UNEXPECTED 5

volatile uint8_t gErrorType = NO_ERROR;

int main(void)
{
    gErrorType = NO_ERROR;
    SYSCFG_DL_init();

    DL_FLASHCTL_FAIL_TYPE failType;

    /*
     * Dynamically protect main memory so it cannot be erased.
     * However, memory is protected by default.
     */
    DL_FlashCTL_protectMainMemory(FLASHCTL);

    /*
     * Try to erase memory.
     * This command is expected to fail, because memory was protected prior to
     * executing an erase operation.
     * Memory must be unprotected before attempting to erase.
     */
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    failType = DL_FlashCTL_getFailureStatus(FLASHCTL);
    if ((gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_FAILED) ||
        (failType != DL_FLASHCTL_FAIL_TYPE_WRITE_ERASE_PROTECT)) {
        gErrorType = ERROR_ERASE;
    }

    if (gErrorType == NO_ERROR) {
        /* Protect a specific sector in main memory */
        DL_FlashCTL_protectSector(
            FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
        /* Try to program main memory */
        gCmdStatus = DL_FlashCTL_programMemoryFromRAM32WithECCGenerated(
            FLASHCTL, MAIN_BASE_ADDRESS, &gData32);
        /*
         * This is expected to fail, because memory was protected prior to
         * executing a program operation.
         * Memory must be unprotected before attempting to program.
         */
        failType = DL_FlashCTL_getFailureStatus(FLASHCTL);
        if ((gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_FAILED)) {
            gErrorType = ERROR_PROTECT;
        }
        if ((failType != DL_FLASHCTL_FAIL_TYPE_WRITE_ERASE_PROTECT)) {
            gErrorType = ERROR_UNEXPECTED;
        }
    }

    if (gErrorType == NO_ERROR) {
        /* Unprotect sector in main memory*/
        DL_FlashCTL_unprotectSector(
            FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
        /* Erase sector in main memory */
        gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
            FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
        /*
         * This is expected to pass, because memory was unprotected
         * prior to executing a program operation.
         */
        if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
            gErrorType = ERROR_UNPROTECT;
        }
    }

    if (gErrorType == NO_ERROR) {
        /*
         * The Flash Controller will automatically re-protect memory after every
         * program and erase operation, so memory must be dynamically unprotected
         * again before the next operation is executed.
         */
        DL_FlashCTL_unprotectSector(
            FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
        /* Program to flash in main memory */
        gCmdStatus = DL_FlashCTL_programMemoryFromRAM32WithECCGenerated(
            FLASHCTL, MAIN_BASE_ADDRESS, &gData32);
        /*
         * This is expected to pass, because memory was unprotected
         * prior to executing a program operation.
         */
        if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
            gErrorType = ERROR_REPROTECT;
        }
    }

    if (gErrorType == NO_ERROR) {
        /* If successful, toggle LED */
        while (1) {
            DL_GPIO_togglePins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
            delay_cycles(16000000);
        }
    }
    /* Unsuccesful example run */
    else {
        /* Check gErrorType value */
        __BKPT(0);
    }
}
