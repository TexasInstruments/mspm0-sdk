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

/* The address space in memory containing the programmed ECC code */
#define ECC_CODE_ADDRESS_SPACE (0x41801000)

/* The address space in memory containing the ECC-corrected data */
#define CORRECTED_ADDRESS_SPACE (0x00001000)

/* The address space in memory containing the uncorrected data */
#define UNCORRECTED_ADDRESS_SPACE (0x00401000)

/*
 * Array to write 64-bits to flash. Data must be loaded 32-bits at a time,
 * but a single word program is executed.
 */
uint32_t gDataArray64[] = {0xABCDEF00, 0x12345678};

/* Array with "single-bit error" of gDataArray64 */
uint32_t gDataArray64SEC[] = {0xABCDEF01, 0x12345678};

/* Array with "double-bit error" of gDataArray64 */
uint32_t gDataArray64DED[] = {0xABCDEF11, 0x12345678};

volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

volatile bool gDEDFault;

uint32_t* gECCCodeAddress     = (void*) ECC_CODE_ADDRESS_SPACE;
uint32_t* gUncorrectedAddress = (void*) UNCORRECTED_ADDRESS_SPACE;
uint32_t* gCorrectedAddress   = (void*) CORRECTED_ADDRESS_SPACE;
uint32_t gUncorrectedData[2] __attribute__((unused));
uint32_t gCorrectedData[2] __attribute__((unused));
uint32_t gFaultStatus __attribute__((unused));
uint8_t gECCCode;

/*
 *  Helper function to perform erase of sector at the specified
 *  address. Function will block if command does not pass.
 *
 *  address   The address of sector to erase
 */
void FlashCtl_eraseSectorBlocking(uint32_t address)
{
    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    /* Erase memory before programming */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, address, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }
}

/*
 *  Helper function to set buffer either gUncorrectedData or gCorrectedData
 *  with data passed in from CORRECTED_ADDRESS_SPACE and
 *  UNCORRECTED_ADDRESS_SPACE.
 *
 *  buffer  The array being passed in to store data from read
 *  address The address of memory being read from
 *
 */
void FlashCtl_readMemoryAndSetBuffer(uint32_t buffer[], uint32_t* address)
{
    buffer[0] = *address;
    buffer[1] = *(address + 1);
}
int main(void)
{
    SYSCFG_DL_init();

    FlashCtl_eraseSectorBlocking(MAIN_BASE_ADDRESS);

    /*
     * Program single flash word (64-bit data) with ECC enabled.
     * The flash controller hardware will handle generating the 8-bit ECC code.
     */

    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCGenerated(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64[0]);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }
    /* Get the 8-bit ECC code that was generated for this 64-bit data */
    gECCCode = *(gECCCodeAddress);

    FlashCtl_eraseSectorBlocking(MAIN_BASE_ADDRESS);

    /*
     * Enable override of hardware generated ECC. This allows the user to
     * manually set the ECC code to be programmed.
     */
    DL_FlashCTL_enableOverrideHardwareGeneratedECC(FLASHCTL);

    /*
     * Demonstrate a SEC (single bit error correction) error.
     * Program 64-bit data with ECC enabled, but the data in gDataArray64SEC
     * has a one bit difference from the data in gDataArray64.
     * Additionally, the ECC code being programmed corresponds with the data
     * in gDataArray64. Since we are using the ECC code corresponding with
     * gDataArray64, and gDataArray64SEC has a bit difference in the data,
     * this will trigger a SEC error.
     */
    DL_SYSCTL_clearECCErrorStatus();
    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCManual(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64SEC[0], &gECCCode);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }

    /*
     * The uncorrected address space has the original raw data of
     * gDataArray64SEC. The corrected address space has the ECC-corrected data.
     * Since there was only a single bit error, this is a correctable error, so
     * the 'corrected' data that matched the manually provided ECC code was
     * programmed. The 'corrected' data of gDataArray64SEC will match the data
     * of gDataArray64
     */
    FlashCtl_readMemoryAndSetBuffer(gUncorrectedData, gUncorrectedAddress);
    FlashCtl_readMemoryAndSetBuffer(gCorrectedData, gCorrectedAddress);
    /* Delay in order for register to update gFaultStatus accordingly */
    delay_cycles(1000000);
    gFaultStatus = DL_SYSCTL_getStatus() & DL_SYSCTL_STATUS_FLASH_SEC;

    /*
     * Set a breakpoint to confirm the values of:
     * gFaultStatus = DL_SYSCTL_STATUS_FLASH_SEC
     * gUncorrectedData = {0xABCDEF01, 0x12345678}
     * gCorrectedData = {0xABCDEF00, 0x12345678}
     * User can also check the memory browser at addresses
     * CORRECTED_ADDRESS_SPACE and UNCORRECTED_ADDRESS_SPACE
     */
    __BKPT(0);

    FlashCtl_eraseSectorBlocking(MAIN_BASE_ADDRESS);

    /*
     * Demonstrate a DED (double bit error correction) error.
     * A DED error will result in an NMI (Non-Maskable Interrupt) triggering with a DED fault.
     * Program 64-bit data with ECC enabled, but the data in gDataArray64DED
     * has a two bit difference from the data in gDataArray64.
     * Additionally, the ECC code being programmed corresponds with the data in
     * gDataArray64. Since we are using the ECC code corresponding with
     * gDataArray64, and gDataArray64DED has a two bit difference in the data,
     * this will trigger a DED error.
    */
    DL_SYSCTL_clearECCErrorStatus();
    /*
     * Ensure proper flash command execution by clearing the STATCMD
     * register before executing a flash operation
     */
    DL_FlashCTL_executeClearStatus(FLASHCTL);
    DL_FlashCTL_unprotectSector(
        FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    gCmdStatus = DL_FlashCTL_programMemoryFromRAM64WithECCManual(
        FLASHCTL, (MAIN_BASE_ADDRESS), &gDataArray64DED[0], &gECCCode);
    if (gCmdStatus != DL_FLASHCTL_COMMAND_STATUS_PASSED) {
        /* If command did not pass, set a SW breakpoint. */
        __BKPT(0);
    }
    /* Reading the location will result in NMI triggered since ECC is incorrect */
    FlashCtl_readMemoryAndSetBuffer(gCorrectedData, gCorrectedAddress);

    while (1) {
        __WFI();
    }
}

void NMI_Handler(void)
{
    switch (DL_SYSCTL_getPendingNonMaskableInterrupt()) {
        case DL_SYSCTL_NMI_IIDX_FLASH_DED:
            /*
            * NMI will trigger with DED fault.
            * The uncorrected address space has the original raw data of gDataArray64DED.
            * The corrected address space has the ECC-corrected data, but since there was
            * a double bit error and this type of error is uncorrectable, the data at
            * the corrected address space will also be the data of gDataArray64DED.
            * Confirm the values of:
            * gFaultStatus = DL_SYSCTL_STATUS_FLASH_DED
            * gUncorrectedData = {0xABCDEF11, 0x12345678}
            * gCorrectedData = {0xABCDEF11, 0x12345678}
            */
            FlashCtl_readMemoryAndSetBuffer(
                gUncorrectedData, gUncorrectedAddress);
            FlashCtl_readMemoryAndSetBuffer(gCorrectedData, gCorrectedAddress);
            gFaultStatus = DL_SYSCTL_getStatus() & DL_SYSCTL_STATUS_FLASH_DED;
            gDEDFault    = true;
            DL_GPIO_clearPins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
            __BKPT(0);
            break;
        default:
            break;
    }
}
