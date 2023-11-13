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

#include "boot_config.h"
#include "ti_msp_dl_config.h"

/* clang-format off */

/* Base address of nonmain memory */
#define NONMAIN_BASE_ADDRESS                                      (0x41C00000U)

/* Base address of the BCR configuration structure in nonmain memory */
#define BCR_USER_CFG_BASE_ADDRESS                                 (0x41C00000U)

/* Size in bytes of the BCR configuration structure */
#define BCR_CONFIG_SIZE_BYTES                                             (24U)

/* clang-format on */

/* The default configuration of the BCR config struct */
BCR_Config gBCRConfig = {
    .bcrConfigID                   = 0x3,
    .debugAccess                   = BCR_CFG_DEBUG_ACCESS_EN,
    .swdpMode                      = BCR_CFG_SWDP_EN,
    .factoryResetMode              = BCR_CFG_FACTORY_RESET_EN,
    .staticWriteProtectionNonMain  = BCR_CFG_NON_MAIN_STATIC_PROT_DIS,
    .staticWriteProtectionMainLow  = CFG_DEFAULT_VALUE,
    .staticWriteProtectionMainHigh = CFG_DEFAULT_VALUE,
    .reserved                      = 0xFFFFFFFFU,
};

int main(void)
{
    SYSCFG_DL_init();

    /* Unprotect and then erase NONMAIN memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
    DL_FlashCTL_eraseMemory(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

    /*
     * Program the BCR config structure to NONMAIN memory.
     * The BCR config struct has a size of 24 bytes, and 4 bytes are
     * programmed at a time, resulting in 24 / 4 = 6 writes to Flash
     */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
    DL_FlashCTL_programMemoryBlocking(FLASHCTL, BCR_USER_CFG_BASE_ADDRESS,
        (uint32_t *) &gBCRConfig, (BCR_CONFIG_SIZE_BYTES / 4),
        DL_FLASHCTL_REGION_SELECT_NONMAIN);

    /* After all writes, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}
