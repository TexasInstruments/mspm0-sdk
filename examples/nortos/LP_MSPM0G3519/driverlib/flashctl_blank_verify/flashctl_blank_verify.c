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

/*
 * Array to write 64-bits to flash. Data must be loaded 32-bits at a time,
 * but a single word program is executed.
 */
uint32_t gDataArray64[] = {0xABCDEF00, 0x12345678};

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

/* Codes to understand where error occured */
#define NO_ERROR 0
#define ERROR_ERASE 1
#define ERROR_BLANK_VERIFY 2
#define ERROR_64BIT_W 3
#define ERROR_UNEXPECTED 4

volatile uint8_t gErrorType = NO_ERROR;

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
        /* If command was not successful, set error flag */
        gErrorType = ERROR_ERASE;
    }

    if (gErrorType == NO_ERROR) {
        /*
         * Check if the flash word at MAIN_BASE_ADDRESS is blank. Since this sector
         * was just erased, this is a blank flash word and the blank verify command
         * will be successful and return no failure.
         */
        gCmdStatus =
            DL_FlashCTL_blankVerifyFromRAM(FLASHCTL, MAIN_BASE_ADDRESS);
        if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
            /* If command was not successful, set error flag */
            gErrorType = ERROR_BLANK_VERIFY;
        }
    }

    if (gErrorType == NO_ERROR) {
        /* 64-bit write to flash in main memory */
        DL_FlashCTL_unprotectSector(
            FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
        gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
            FLASHCTL, MAIN_BASE_ADDRESS, &gDataArray64[0]);
        if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
            /* If command was not successful, set error flag */
            gErrorType = ERROR_64BIT_W;
        }
    }

    if (gErrorType == NO_ERROR) {
        /*
        * Check again if the flash word at MAIN_BASE_ADDRESS is blank. Since a
        * flash word was just programmed to MAIN_BASE_ADDRESS, this is not a
        * blank flash word. Therefore the command will fail and the failure type
        * will be a  DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR.
        */
        gCmdStatus =
            DL_FlashCTL_blankVerifyFromRAM(FLASHCTL, MAIN_BASE_ADDRESS);
        if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
            failType = DL_FlashCTL_getFailureStatus(FLASHCTL);
            if (failType != DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR) {
                /* If fail type is unexpected, set error flag */
                gErrorType = ERROR_UNEXPECTED;
            }
        }
    }

    /* After successful completion, toggle LED and USER_TEST pin */
    if (gErrorType == NO_ERROR) {
        while (1) {
            DL_GPIO_togglePins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
            delay_cycles(16000000);
        }
    }
    /* Unsuccessful example run */
    else {
        /* Check gErrorType value */
        __BKPT(0);
    }
}
