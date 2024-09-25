/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#ifndef FACTORY_CONFIG_H
#define FACTORY_CONFIG_H

/* Structure to store the Bootloader invocation Pin details.
 * This structure is followed in TRIM memory where default BSL invocation
 * pin details are stored and also in Bootloader configuration memory.
 *
 * Pin_data_0
 * BIT [0-5]  :pincmIndex   -> IOMUX_SECCFG_PINCM index for the pin (0 to 63)
 * BIT [7]    :pinLevel     -> Pin state High(1)/ Low(0) which should be
 *                             considered as BSL invocation condition
 * Pin_data_1
 * BIT [0-4]  :gpioNum      -> GPIO Pin number (0 to 31)
 * BIT [5]    :gpioBase     -> GPIO Instance number or GPIO Base Index (0 or 1)
 */
typedef struct {
    uint8_t pinData0;
    uint8_t pinData1;
} BSLInvokePin_FactoryConfig;

typedef struct {
    uint8_t UART_RXD_pad_num;
    uint8_t UART_RXD_PF_mux_sel;
    uint8_t UART_TXD_pad_num;
    uint8_t UART_TXD_PF_mux_sel;

    uint8_t I2C_SDA_pad_num;
    uint8_t I2C_SDA_PF_mux_sel;
    uint8_t I2C_SCL_pad_num;
    uint8_t I2C_SCL_PF_mux_sel;

} BSLPluginPin;

typedef struct {
    uint32_t traceID;
    uint32_t deviceID;
    uint32_t userID;
    BSLPluginPin bslPluginPins;
    BSLInvokePin_FactoryConfig bslInvokePin;
    uint16_t reserved;
    uint16_t szFlash;
    uint16_t szSRAM;
    uint32_t pllTrims[8];
    uint32_t tempsense0;
    uint32_t reserved1[15];
    uint32_t bootCRC;
} Factory_Config;

/*
 * TRIM configuration defines
 */
#define TRIM_MAIN_FLASH_SIZE_MASK ((uint16_t) 0x0FFF)
#define TRIM_SRAM_SIZE_MASK ((uint16_t) 0x03FF)

#endif
