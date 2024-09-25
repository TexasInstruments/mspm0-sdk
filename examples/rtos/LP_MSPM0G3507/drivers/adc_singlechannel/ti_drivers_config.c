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

/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G350X
 *  by the SysConfig tool.
 */

#include <stddef.h>
#include <stdint.h>

#include <ti/devices/DeviceFamily.h>
#include "ti_drivers_config.h"

/*
 *  ============================= Display =============================
 */

#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

#define CONFIG_Display_COUNT 1

const uint_least8_t CONFIG_UART_0 = 0;
const uint_least8_t UART_count    = CONFIG_UART_COUNT;

#define DISPLAY_UARTBUFFERSIZE 50
static char displayUARTBuffer[DISPLAY_UARTBUFFERSIZE];

DisplayUart_Object displayUartObject;

const DisplayUart_HWAttrs displayUartHWAttrs = {.uartIdx = CONFIG_UART_0,
    .baudRate                                            = 115200,
    .mutexTimeout                                        = (unsigned int) (-1),
    .strBuf                                              = displayUARTBuffer,
    .strBufLen = DISPLAY_UARTBUFFERSIZE};

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
    GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH, /* PA10:CONFIG_GPIO_UART2_0_TX */
    GPIO_CFG_INPUT | GPIO_CFG_IN_INT_NONE |
        GPIO_CFG_IN_PD,                  /* PA11:CONFIG_GPIO_UART2_0_RX */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA12 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA13 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA14 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA15 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA16 */
    /* Owned by CONFIG_ADC_1, channel 2 as ADC Pin */
    GPIO_CFG_INPUT | GPIO_CFG_IN_INT_NONE |
        GPIO_CFG_IN_NOPULL,              /* PA17:CONFIG_GPIO_ADC_1_AIN */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA18 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA19 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA20 */
    GPIO_CFG_INPUT | GPIO_DO_NOT_CONFIG, /* PA21 */
    /* Owned by CONFIG_ADC_0, channel 7 as ADC Pin */
    GPIO_CFG_INPUT | GPIO_CFG_IN_INT_NONE |
        GPIO_CFG_IN_NOPULL,              /* PA22:CONFIG_GPIO_ADC_0_AIN */
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

const uint_least8_t CONFIG_GPIO_LED_0_CONST     = CONFIG_GPIO_LED_0;
const uint_least8_t CONFIG_GPIO_ADC_0_AIN_CONST = CONFIG_GPIO_ADC_0_AIN;
const uint_least8_t CONFIG_GPIO_ADC_1_AIN_CONST = CONFIG_GPIO_ADC_1_AIN;
const uint_least8_t CONFIG_GPIO_UART_0_TX_CONST = CONFIG_GPIO_UART_0_TX;
const uint_least8_t CONFIG_GPIO_UART_0_RX_CONST = CONFIG_GPIO_UART_0_RX;

/*
 *  ======== GPIO_config ========
 */
const GPIO_Config GPIO_config = {.configs = (GPIO_PinConfig *) gpioPinConfigs,
    .callbacks   = (GPIO_CallbackFxn *) gpioCallbackFunctions,
    .userArgs    = gpioUserArgs,
    .intPriority = (~0)};

/*
 *  =============================== ADC ===============================
 */

#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSPM0.h>

#define CONFIG_ADC_COUNT 2

/*
 *  ======== adcMSPM0Objects ========
 */
ADCMSPM0_Object adcObjects[CONFIG_ADC_COUNT];

/*
 *  ======== adcMSPM0HWAttrs ========
 */
const ADCMSPM0_HWAttrs adcHWAttrs[CONFIG_ADC_COUNT] = {
    /* CONFIG_ADC_0 */
    {.adc                     = ADC_0_INST,
        .adcInputDIO          = CONFIG_GPIO_ADC_0_AIN,
        .adcInputPincm        = CONFIG_GPIO_ADC_0_AIN_PINCM,
        .adcInputPinMux       = CONFIG_GPIO_ADC_0_AIN_PINMUX,
        .adcPosRefDIO         = GPIO_INVALID_INDEX,
        .adcNegRefDIO         = GPIO_INVALID_INDEX,
        .adcChannel           = 7,
        .refSource            = ADCMSPM0_VDDA_REFERENCE,
        .samplingDuration     = 16,
        .refVoltage           = 3300000, /* uV */
        .resolutionBits       = ADCMSPM0_RESOLUTION_12_BIT,
        .adcClkkDivider       = ADCMSPM0_CLKDIV_8,
        .adcClkSelect         = ADCMSPM0_CLK_ULPCLK,
        .adcClkFreqRange      = ADCMSPM0_CLK_FREQ_RANGE_24TO32,
        .conversionMode       = ADCMSPM0_SINGLE_CH_SINGLE_CONV,
        .conversionStartAddr  = 0,
        .conversionEndAddr    = 0,
        .repeatConversionMode = ADCMSPM0_REPEAT_MODE_ENABLED,
        .samplingMode         = ADCMSPM0_SAMPLING_MODE_AUTO,
        .sampleTrigger        = ADCMSPM0_SAMPLING_TRIG_SW,
        .conversionDataFormat = ADCMSPM0_CONV_DATA_FORMAT_UNSIGNED,
        .sampleTimerSrc       = ADCMSPM0_SAMP_TMR_SOURCE_SCOMP0,
        .conversionTrigger    = ADCMSPM0_NEXT_CONV_WITH_TRIG,
        .adcHWAveraging       = ADCMSPM0_HW_AVG_DISABLED,
        .idxMEMCTLx           = 0},
    /* CONFIG_ADC_1 */
    {.adc                     = ADC_1_INST,
        .adcInputDIO          = CONFIG_GPIO_ADC_1_AIN,
        .adcInputPincm        = CONFIG_GPIO_ADC_1_AIN_PINCM,
        .adcInputPinMux       = CONFIG_GPIO_ADC_1_AIN_PINMUX,
        .adcPosRefDIO         = GPIO_INVALID_INDEX,
        .adcNegRefDIO         = GPIO_INVALID_INDEX,
        .adcChannel           = 2,
        .refSource            = ADCMSPM0_VDDA_REFERENCE,
        .samplingDuration     = 16,
        .refVoltage           = 3300000, /* uV */
        .resolutionBits       = ADCMSPM0_RESOLUTION_12_BIT,
        .adcClkkDivider       = ADCMSPM0_CLKDIV_8,
        .adcClkSelect         = ADCMSPM0_CLK_SYSOSC,
        .adcClkFreqRange      = ADCMSPM0_CLK_FREQ_RANGE_24TO32,
        .conversionMode       = ADCMSPM0_SINGLE_CH_SINGLE_CONV,
        .conversionStartAddr  = 0,
        .conversionEndAddr    = 0,
        .repeatConversionMode = ADCMSPM0_REPEAT_MODE_ENABLED,
        .samplingMode         = ADCMSPM0_SAMPLING_MODE_AUTO,
        .sampleTrigger        = ADCMSPM0_SAMPLING_TRIG_SW,
        .conversionDataFormat = ADCMSPM0_CONV_DATA_FORMAT_UNSIGNED,
        .sampleTimerSrc       = ADCMSPM0_SAMP_TMR_SOURCE_SCOMP0,
        .conversionTrigger    = ADCMSPM0_NEXT_CONV_WITH_TRIG,
        .adcHWAveraging       = ADCMSPM0_HW_AVG_DISABLED,
        .idxMEMCTLx           = 0},
};

/*
 *  ======== ADC_config ========
 */
const ADC_Config ADC_config[CONFIG_ADC_COUNT] = {
    /* CONFIG_ADC_0 */
    {.fxnTablePtr = &ADCMSPM0_fxnTable,
        .object   = &adcObjects[CONFIG_ADC_0],
        .hwAttrs  = &adcHWAttrs[CONFIG_ADC_0]},
    /* CONFIG_ADC_1 */
    {.fxnTablePtr = &ADCMSPM0_fxnTable,
        .object   = &adcObjects[CONFIG_ADC_1],
        .hwAttrs  = &adcHWAttrs[CONFIG_ADC_1]},
};

const uint_least8_t CONFIG_ADC_0_CONST = CONFIG_ADC_0;
const uint_least8_t CONFIG_ADC_1_CONST = CONFIG_ADC_1;
const uint_least8_t ADC_count          = CONFIG_ADC_COUNT;

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
