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

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include "main.h"
#include "system/uart.h"
#include "ti_msp_dl_config.h"

#define UART_CMD_IBRD_4_MHZ_115200_BAUD (2)
#define UART_CMD_FBRD_4_MHZ_115200_BAUD (11)
#define UART_CMD_IBRD_32_MHZ_115200_BAUD (17)
#define UART_CMD_FBRD_32_MHZ_115200_BAUD (23)
#define UART_CMD_IBRD_40_MHZ_115200_BAUD (21)
#define UART_CMD_FBRD_40_MHZ_115200_BAUD (45)

/**
 * @brief Initializes system to run at 80 MHz after startup
 *
 * @return STATUS_OK always
 */
enum status_enum cpu_clock_init_80m(void)
{
    static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
        .inputFreq   = DL_SYSCTL_SYSPLL_INPUT_FREQ_16_32_MHZ,
        .rDivClk2x   = 1,
        .rDivClk1    = 0,
        .rDivClk0    = 0,
        .enableCLK2x = DL_SYSCTL_SYSPLL_CLK2X_DISABLE,
        .enableCLK1  = DL_SYSCTL_SYSPLL_CLK1_DISABLE,
        .enableCLK0  = DL_SYSCTL_SYSPLL_CLK0_ENABLE,
        .sysPLLMCLK  = DL_SYSCTL_SYSPLL_MCLK_CLK0,
        .sysPLLRef   = DL_SYSCTL_SYSPLL_REF_SYSOSC,
        .qDiv        = 9,
        .pDiv        = DL_SYSCTL_SYSPLL_PDIV_2};

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);
    DL_UART_Main_setBaudRateDivisor(UART_CMD_INST,
        UART_CMD_IBRD_40_MHZ_115200_BAUD, UART_CMD_FBRD_40_MHZ_115200_BAUD);
    DL_SYSTICK_config(80000);
    uart_printf("Running at 80 MHz\r\n");
    return STATUS_OK;
}

/**
 * @brief Initializes system to run at 32 MHz after startup
 *
 * @return STATUS_OK always
 */
enum status_enum cpu_clock_init_32m(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSTICK_config(32000);
    DL_UART_Main_setBaudRateDivisor(UART_IO_INST,
        UART_CMD_IBRD_32_MHZ_115200_BAUD, UART_CMD_FBRD_32_MHZ_115200_BAUD);
    uart_printf("Running at 32 MHz\r\n");
    return STATUS_OK;
}

/**
 * @brief Initializes system to run at 4 MHz after startup
 *
 * @return STATUS_OK always
 */
enum status_enum cpu_clock_init_4m(void)
{
    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    DL_SYSCTL_enableMFCLK();
    DL_SYSTICK_config(32000);  //something is odd here
    DL_UART_Main_setBaudRateDivisor(UART_IO_INST,
        UART_CMD_IBRD_4_MHZ_115200_BAUD, UART_CMD_FBRD_4_MHZ_115200_BAUD);
    uart_printf("Running at 4 MHz\r\n");
    return STATUS_OK;
}
