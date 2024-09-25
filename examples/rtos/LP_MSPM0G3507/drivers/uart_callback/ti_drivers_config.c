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

/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */

#include "ti_drivers_config.h"

/*
 *  =============================== DMA ===============================
 */
#include <ti/drivers/dma/DMAMSPM0.h>
const uint_least8_t CONFIG_DMA_0               = 0;
const uint_least8_t DMA_Count                  = CONFIG_DMA_COUNT;
DMAMSPM0_Object DMAObject[CONFIG_DMA_CH_COUNT] = {
    {.dmaTransfer =
            {
                .txTrigger              = DMA_UART0_TX_TRIG,
                .txTriggerType          = DL_DMA_TRIGGER_TYPE_EXTERNAL,
                .rxTrigger              = DMA_UART0_RX_TRIG,
                .rxTriggerType          = DL_DMA_TRIGGER_TYPE_EXTERNAL,
                .transferMode           = DL_DMA_SINGLE_TRANSFER_MODE,
                .extendedMode           = DL_DMA_NORMAL_MODE,
                .destWidth              = DL_DMA_WIDTH_BYTE,
                .srcWidth               = DL_DMA_WIDTH_BYTE,
                .destIncrement          = DL_DMA_ADDR_INCREMENT,
                .dmaChannel             = 0,
                .dmaTransferSource      = NULL,
                .dmaTransferDestination = NULL,
                .enableDMAISR           = false,
            }},
};

static const DMAMSPM0_HWAttrs DMAMSP0HWAttrs[CONFIG_DMA_COUNT] = {
    {
        .dmaIsrFxn          = NULL,
        .intPriority        = DEFAULT_DMA_PRIORITY,
        .roundRobinPriority = 0,
    },
};
const DMAMSPM0_Cfg DMAMSPM0_Config[CONFIG_DMA_COUNT] = {
    {
        &DMAMSP0HWAttrs[CONFIG_DMA_0],
        &DMAObject[CONFIG_DMA_0],
    },
};

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/uart/UARTMSPM0.h>

const uint_least8_t CONFIG_UART_0 = 0;
const uint_least8_t UART_count    = CONFIG_UART_COUNT;

static const UARTMSP_HWAttrs UARTMSPHWAttrs[CONFIG_UART_COUNT] = {
    {
        .regs          = UART0,
        .irq           = UART0_INT_IRQn,
        .rxPin         = IOMUX_PINCM22, /* 17 */
        .rxPinFunction = IOMUX_PINCM22_PF_UART0_RX,
        .txPin         = IOMUX_PINCM21, /* 16 */
        .txPinFunction = IOMUX_PINCM21_PF_UART0_TX,
        .mode          = DL_UART_MODE_NORMAL,
        .direction     = DL_UART_DIRECTION_TX_RX,
        .flowControl   = DL_UART_FLOW_CONTROL_NONE,
        .clockSource   = DL_UART_CLOCK_BUSCLK,
        .clockDivider  = DL_UART_CLOCK_DIVIDE_RATIO_4,
        .rxIntFifoThr  = DL_UART_RX_FIFO_LEVEL_ONE_ENTRY,
        .txIntFifoThr  = DL_UART_TX_FIFO_LEVEL_EMPTY,
    },
};

UART_Data_Object UARTObject[CONFIG_UART_COUNT] = {
    {
        .object =
            {
                .supportFxns        = &UARTMSPSupportFxns,
                .buffersSupported   = true,
                .eventsSupported    = false,
                .callbacksSupported = true,
                .dmaSupported       = true,
                .noOfDMAChannels    = 1,
            },
        .buffersObject =
            {
                .rxBufPtr  = rxBuffer,
                .txBufPtr  = txBuffer,
                .rxBufSize = sizeof(rxBuffer),
                .txBufSize = sizeof(txBuffer),
            },
    },
};

const UART_Config UART_config[CONFIG_UART_COUNT] = {
    {
        &UARTObject[CONFIG_UART_0],
        &UARTMSPHWAttrs[CONFIG_UART_0],
    },
};

/*****************************************************************************************************************/

void UART0_IRQHandler(void)
{
    UARTMSP_interruptHandler((UART_Handle) &UART_config[0]);
}

void UARTMSP_eventCallback(
    UART_Handle handle, uint32_t event, uint32_t data, void *userArg)
{
}
