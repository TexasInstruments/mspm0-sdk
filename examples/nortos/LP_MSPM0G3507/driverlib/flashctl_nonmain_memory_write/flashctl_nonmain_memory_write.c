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

#include "boot_config.h"
#include "ti_msp_dl_config.h"

/* clang-format off */

/* Base address of nonmain memory */
#define NONMAIN_BASE_ADDRESS                                      (0x41C00000U)

/* Base address of the BCR configuration structure in nonmain memory */
#define BCR_USER_CFG_BASE_ADDRESS                                 (0x41C00000U)

/* Base address of the BSL configuration structure in nonmain memory */
#define BSL_USER_CFG_BASE_ADDRESS                                 (0x41C00100U)

/* Size in bytes of the BSL and BCR configuration structures */
#define BCR_CONFIG_SIZE_BYTES                                             (96U)
#define BSL_CONFIG_SIZE_BYTES                                             (88U)

/* The calculated CRC based on the default configuration values */
#define BCR_CFG_DEFAULT_CRC                                        (0x1879dac3)
#define BSL_CFG_DEFAULT_CRC                                         (0xf7df416)

/* clang-format on */

/* The default configuration of the BCR config struct */
BCR_Config gBCRConfig = {
    .bcrConfigID                   = 0x1,
    .debugAccess                   = BCR_CFG_DEBUG_ACCESS_EN,
    .swdpMode                      = BCR_CFG_SWDP_EN,
    .tifaMode                      = BCR_CFG_TIFA_EN,
    .bslPinInvokeEnable            = BCR_CFG_BSL_PIN_INVOKE_EN,
    .passwordDebugLock             = {CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE,
        CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE},
    .fastBootMode                  = BCR_CFG_FAST_BOOT_DIS,
    .bootloaderMode                = BCR_CFG_BOOTLOADER_MODE_EN,
    .massEraseMode                 = BCR_CFG_MASS_ERASE_EN,
    .factoryResetMode              = BCR_CFG_FACTORY_RESET_EN,
    .passwordMassErase             = {CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE,
        CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE},
    .passwordFactoryReset          = {CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE,
        CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE},
    .staticWriteProtectionMainLow  = CFG_DEFAULT_VALUE,
    .staticWriteProtectionMainHigh = CFG_DEFAULT_VALUE,
    .staticWriteProtectionNonMain  = BCR_CFG_NON_MAIN_STATIC_PROT_DIS,
    .secureBootMode                = BCR_CFG_SECURE_BOOT_DIS,
    .userSecureAppStartAddr        = CFG_DEFAULT_VALUE,
    .userSecureAppLength           = CFG_DEFAULT_VALUE,
    .userSecureAppCrc              = CFG_DEFAULT_VALUE,
    .userCfgCRC                    = BCR_CFG_DEFAULT_CRC,
};

/* The default configuration of the BSL config struct */
BSL_Config gBSLConfig = {
    .configID                          = 0x1,
    .interfacePins.UART_RXD_pad_num    = DEF_UART_RXD_PAD,
    .interfacePins.UART_RXD_PF_mux_sel = DEF_UART_RXD_MUX,
    .interfacePins.UART_TXD_pad_num    = DEF_UART_TXD_PAD,
    .interfacePins.UART_TXD_PF_mux_sel = DEF_UART_TXD_MUX,
    .interfacePins.I2C_SDA_pad_num     = DEF_I2C_SDA_PAD,
    .interfacePins.I2C_SDA_PF_mux_sel  = DEF_I2C_SDA_MUX,
    .interfacePins.I2C_SCL_pad_num     = DEF_I2C_SCL_PAD,
    .interfacePins.I2C_SCL_PF_mux_sel  = DEF_I2C_SCL_MUX,
    .pin.pinData0                      = DEFAULT_BSL_PIN_INVOCATION_DATA0,
    .pin.pinData1                      = DEFAULT_BSL_PIN_INVOCATION_DATA1,
    .memoryRead                        = BSL_CFG_MEMORY_READOUT_DISABLE,
    .password   = {CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE,
        CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE,
        CFG_DEFAULT_VALUE, CFG_DEFAULT_VALUE},
    .pluginType = BSL_CFG_PLUGIN_TYPE_ANY,
    .flashPluginEnable  = BSL_CFG_PLUGIN_FLASH_NOT_EXIST,
    .pluginSramSize     = 0xFF,
    .pluginHook[0]      = CFG_DEFAULT_VALUE,
    .pluginHook[1]      = CFG_DEFAULT_VALUE,
    .pluginHook[2]      = CFG_DEFAULT_VALUE,
    .pluginHook[3]      = CFG_DEFAULT_VALUE,
    .BSLAlternateConfig = BSL_CFG_FLASH_BSL_NOT_EXIST,
    .reserved           = 0xFFFF,
    .BSLAlternateAddr   = CFG_DEFAULT_VALUE,
    .appRev             = (uint32_t *) CFG_DEFAULT_VALUE,
    .securityAlert      = BSL_CFG_SECURITY_IGNORE,
    .i2cSlaveAddress    = 0x48,
    .userCfgCRC         = BSL_CFG_DEFAULT_CRC,
};

uint32_t calcUserConfigCRC(uint8_t *dataPointer, uint32_t size)
{
    static uint32_t i;

    uint32_t calculatedCRC;

    DL_CRC_init(CRC, DL_CRC_32_POLYNOMIAL, DL_CRC_BIT_REVERSED,
        DL_CRC_INPUT_ENDIANESS_LITTLE_ENDIAN, DL_CRC_OUTPUT_BYTESWAP_DISABLED);

    /* Set the Seed value to reset the calculation */
    DL_CRC_setSeed32(CRC, CRC_SEED);

    for (i = (uint32_t) 0U; i < size; i++) {
        DL_CRC_feedData8(CRC, dataPointer[i]);
    }
    calculatedCRC = DL_CRC_getResult32(CRC);

    return calculatedCRC;
}

int main(void)
{
    SYSCFG_DL_init();

    /*
     * Calculate the CRC for the BCR and BSL configuration structs
     * The CRC is 4 bytes in each struct, this should not be included in the calculation
     */
    gBCRConfig.userCfgCRC = calcUserConfigCRC((uint8_t *) &gBCRConfig,
        BCR_CONFIG_SIZE_BYTES - ((uint32_t) sizeof(gBCRConfig.userCfgCRC)));
    gBSLConfig.userCfgCRC = calcUserConfigCRC((uint8_t *) &gBSLConfig,
        BSL_CONFIG_SIZE_BYTES - ((uint32_t) sizeof(gBSLConfig.userCfgCRC)));

    /* Unprotect and then erase NONMAIN memory */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
    DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

    /*
     * Program the BCR config structure to NONMAIN memory.
     * The BCR config struct has a size of 96 bytes, and 4 bytes are
     * programmed at a time, resulting in 96 / 4 = 24 writes to Flash
     */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        BCR_USER_CFG_BASE_ADDRESS, (uint32_t *) &gBCRConfig,
        (BCR_CONFIG_SIZE_BYTES / 4), DL_FLASHCTL_REGION_SELECT_NONMAIN);

    /*
     * Program the BSL config structure to NONMAIN memory.
     * The BSL config struct has a size of 88 bytes, and 4 bytes are
     * programmed at a time, resulting in 88 / 4 = 22 writes to Flash
     */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, NONMAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        BSL_USER_CFG_BASE_ADDRESS, (uint32_t *) &gBSLConfig,
        (BSL_CONFIG_SIZE_BYTES / 4), DL_FLASHCTL_REGION_SELECT_NONMAIN);

    /* After all writes, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}
