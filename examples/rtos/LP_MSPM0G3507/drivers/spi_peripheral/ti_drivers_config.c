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
 *  ============ ti_drivers_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */

#include "ti_drivers_config.h"
#include <stddef.h>
#include <stdint.h>

/*
 *  ============================= Display =============================
 */

#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

#define CONFIG_Display_COUNT 1

const uint_least8_t CONFIG_UART_0 = 0;
const uint_least8_t UART_count    = CONFIG_UART_COUNT;

#define Display_UARTBUFFERSIZE 128
static char displayUARTBuffer[Display_UARTBUFFERSIZE];

DisplayUart_Object displayUartObject;

const DisplayUart_HWAttrs displayUartHWAttrs = {.uartIdx = CONFIG_UART_0,
    .baudRate                                            = 115200,
    .mutexTimeout                                        = (unsigned int) (-1),
    .strBuf                                              = displayUARTBuffer,
    .strBufLen = Display_UARTBUFFERSIZE};

const Display_Config Display_config[CONFIG_Display_COUNT] = {
    /* CONFIG_Display_0 */
    /* XDS110 UART */
    {.fxnTablePtr = &DisplayUartMin_fxnTable,
        .object   = &displayUartObject,
        .hwAttrs  = &displayUartHWAttrs},
};

const uint_least8_t Display_count = CONFIG_Display_COUNT;

/*
 *  =============================== DMA ===============================
 */
#include <ti/drivers/dma/DMAMSPM0.h>
const uint_least8_t CONFIG_DMA_0               = 0;
const uint_least8_t DMA_Count                  = CONFIG_DMA_COUNT;
DMAMSPM0_Object DMAObject[CONFIG_DMA_CH_COUNT] = {
    {.dmaTransfer =
            {
                .txTrigger              = SPI_0_INST_DMA_TRIGGER_0,
                .txTriggerType          = DL_DMA_TRIGGER_TYPE_EXTERNAL,
                .transferMode           = DL_DMA_SINGLE_TRANSFER_MODE,
                .extendedMode           = DL_DMA_NORMAL_MODE,
                .destWidth              = DL_DMA_WIDTH_BYTE,
                .srcWidth               = DL_DMA_WIDTH_BYTE,
                .srcIncrement           = DL_DMA_ADDR_INCREMENT,
                .dmaChannel             = DMA_CH0_CHAN_ID,
                .dmaTransferSource      = NULL,
                .dmaTransferDestination = NULL,
                .enableDMAISR           = false,
            }},
    {.dmaTransfer =
            {
                .rxTrigger              = SPI_0_INST_DMA_TRIGGER_1,
                .rxTriggerType          = DL_DMA_TRIGGER_TYPE_EXTERNAL,
                .transferMode           = DL_DMA_SINGLE_TRANSFER_MODE,
                .extendedMode           = DL_DMA_NORMAL_MODE,
                .destWidth              = DL_DMA_WIDTH_BYTE,
                .srcWidth               = DL_DMA_WIDTH_BYTE,
                .destIncrement          = DL_DMA_ADDR_INCREMENT,
                .dmaChannel             = DMA_CH1_CHAN_ID,
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
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSPM0.h>

/* The range of pins available on this device */
const uint_least8_t GPIO_pinLowerBound = 0;
const uint_least8_t GPIO_pinUpperBound = 60;

/*
 *  ======== gpioPinConfigs ========
 *  Array of Pin configurations
 */
GPIO_PinConfig gpioPinConfigs[60] = {
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA0 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA1 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA2 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA3 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA4 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA5 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA6 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA7 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA8 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA9 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA10 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA11 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA12 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA13 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA14 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA15 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA16 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA17 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA18 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA19 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA20 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA21 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA22 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA23 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA24 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA25 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA26 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA27 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA28 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA29 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA30 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA31 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB0 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB1 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB2 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB3 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB4 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB5 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB6 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB7 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB8 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB9 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB10 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB11 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB12 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB13 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB14 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB15 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB16 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB17 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB18 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB19 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB20 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB21 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB22 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB23 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB24 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB25 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB26 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PB27 */
};

/*
 *  ======== gpioCallbackFunctions ========
 *  Array of callback function pointers
 *  Change at runtime with GPIO_setCallback()
 */
GPIO_CallbackFxn gpioCallbackFunctions[60];

/*
 *  ======== gpioUserArgs ========
 *  Array of user argument pointers
 *  Change at runtime with GPIO_setUserArg()
 *  Get values with GPIO_getUserArg()
 */
void *gpioUserArgs[60];

const uint_least8_t CONFIG_GPIO_LED_0_CONST = CONFIG_GPIO_LED_0;

/*
 *  ======== GPIO_config ========
 */
const GPIO_Config GPIO_config = {.configs = (GPIO_PinConfig *) gpioPinConfigs,
    .callbacks   = (GPIO_CallbackFxn *) gpioCallbackFunctions,
    .userArgs    = gpioUserArgs,
    .intPriority = (~0)};

/*
 *  =============================== SPI ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/dma/DMAMSPM0.h>
#include <ti/drivers/spi/SPIMSPM0.h>

#include <ti/devices/DeviceFamily.h>

#define CONFIG_SPI_COUNT 1

/*
 *  ======== SPIMSPM0_objects ========
 */
SPIMSPM0_Object SPIMSPM0_objects[CONFIG_SPI_COUNT];

/*
 *  ======== SPIMSPM0_hwAttrs ========
 */
const SPIMSPM0_HWAttrs SPIMSPM0_hwAttrs[CONFIG_SPI_COUNT] = {
    {
        .spi               = SPI_1_INST,
        .intNum            = SPI_1_INST_INT_IRQN,
        .intPriority       = (~0),
        .clockSource       = DL_SPI_CLOCK_BUSCLK,
        .clockDivider      = DL_SPI_CLOCK_DIVIDE_RATIO_1,
        .defaultTxBufValue = 0xFF,

        .pociPin    = GPIO_SPI_1_POCI_PIN,
        .pociPincm  = GPIO_SPI_1_IOMUX_POCI,
        .pociPinMux = GPIO_SPI_1_IOMUX_POCI_FUNC,

        .picoPin    = GPIO_SPI_1_PICO_PIN,
        .picoPincm  = GPIO_SPI_1_IOMUX_PICO,
        .picoPinMux = GPIO_SPI_1_IOMUX_PICO_FUNC,

        .sclkPin    = GPIO_SPI_1_SCLK_PIN,
        .sclkPincm  = GPIO_SPI_1_IOMUX_SCLK,
        .sclkPinMux = GPIO_SPI_1_IOMUX_SCLK_FUNC,

        .csnPin    = GPIO_SPI_1_CS0_PIN,
        .csnPincm  = GPIO_SPI_1_IOMUX_CS0,
        .csnPinMux = GPIO_SPI_1_IOMUX_CS0_FUNC,

        .txFifoThreshold = DL_SPI_TX_FIFO_LEVEL_ONE_FRAME,
        .rxFifoThreshold = DL_SPI_RX_FIFO_LEVEL_ONE_FRAME,
        .cssel           = 0,
        .noOfDMAChannels = 2,
    },
};

/*
 *  ======== SPI_config ========
 */
const SPI_Config SPI_config[CONFIG_SPI_COUNT] = {
    /* CONFIG_SPI_PERIPHERAL */
    /* LaunchPad SPI Bus with Chip Select */
    {.fxnTablePtr = &SPIMSPM0_fxnTable,
        .object   = &SPIMSPM0_objects[CONFIG_SPI_PERIPHERAL],
        .hwAttrs  = &SPIMSPM0_hwAttrs[CONFIG_SPI_PERIPHERAL]},
};

const uint_least8_t CONFIG_SPI_PERIPHERAL_CONST = CONFIG_SPI_PERIPHERAL;
const uint_least8_t SPI_count                   = CONFIG_SPI_COUNT;

/*
 *  =============================== UART0 ===============================
 */
#include <ti/drivers/uart/UARTMSPM0.h>

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
                .callbacksSupported = false,
                .dmaSupported       = false,
                .noOfDMAChannels    = 0,
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

void UART0_IRQHandler(void)
{
    UARTMSP_interruptHandler((UART_Handle) &UART_config[0]);
}

void UARTMSP_eventCallback(
    UART_Handle handle, uint32_t event, uint32_t data, void *userArg)
{
}
