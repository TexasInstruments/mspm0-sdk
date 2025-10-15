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
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G5187
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_UART_0_init();
    SYSCFG_DL_DMA_init();
    SYSCFG_DL_SYSTICK_init();
    SYSCFG_DL_USB_0_init();
    SYSCFG_DL_I2S_0_init();
}



SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_UART_reset(UART_0_INST);



    DL_I2S_reset(I2S_0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_UART_enablePower(UART_0_INST);



    DL_I2S_enablePower(I2S_0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_0_IOMUX_TX, GPIO_UART_0_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_0_IOMUX_RX, GPIO_UART_0_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_GRP_0_PIN_0_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_1_PIN_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIOA, GPIO_GRP_0_PIN_0_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_GRP_0_PIN_0_PIN);

    DL_GPIO_initPeripheralInputFunction(
        GPIO_I2S_0_IOMUX_AD0, GPIO_I2S_0_IOMUX_AD0_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_I2S_0_IOMUX_BCLK, GPIO_I2S_0_IOMUX_BCLK_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_I2S_0_IOMUX_WCLK, GPIO_I2S_0_IOMUX_WCLK_FUNC);


}


static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
    .inputFreq              = DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ,
	.rDivClk2x              = 1,
	.rDivClk1               = 0,
	.rDivClk0               = 0,
	.enableCLK2x            = DL_SYSCTL_SYSPLL_CLK2X_DISABLE,
	.enableCLK1             = DL_SYSCTL_SYSPLL_CLK1_DISABLE,
	.enableCLK0             = DL_SYSCTL_SYSPLL_CLK0_ENABLE,
	.sysPLLMCLK             = DL_SYSCTL_SYSPLL_MCLK_CLK0,
	.sysPLLRef              = DL_SYSCTL_SYSPLL_REF_SYSOSC,
	.qDiv                   = 4,
	.pDiv                   = DL_SYSCTL_SYSPLL_PDIV_1
};
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);
    DL_SYSCTL_configUSBFLL(DL_SYSCTL_USBFLL_REFERENCE_SOF);
    DL_SYSCTL_setUSBCLKSource(DL_SYSCTL_USBCLK_SOURCE_USBFLL);

}


static const DL_UART_ClockConfig gUART_0ClockConfig = {
    .clockSel    = DL_UART_CLOCK_BUSCLK,
    .divideRatio = DL_UART_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Config gUART_0Config = {
    .mode        = DL_UART_MODE_NORMAL,
    .direction   = DL_UART_DIRECTION_TX_RX,
    .flowControl = DL_UART_FLOW_CONTROL_NONE,
    .parity      = DL_UART_PARITY_NONE,
    .wordLength  = DL_UART_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_UART_0_init(void)
{
    DL_UART_setClockConfig(UART_0_INST, (DL_UART_ClockConfig *) &gUART_0ClockConfig);

    DL_UART_init(UART_0_INST, (DL_UART_Config *) &gUART_0Config);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 115200
     *  Actual baud rate: 115190.78
     */
    DL_UART_setOversampling(UART_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_setBaudRateDivisor(UART_0_INST, UART_0_IBRD_40_MHZ_115200_BAUD, UART_0_FBRD_40_MHZ_115200_BAUD);


    /* Configure FIFOs */
    DL_UART_setRXFIFOThreshold(UART_0_INST, DL_UART_RX_FIFO_LEVEL_ONE_ENTRY);
    DL_UART_setTXFIFOThreshold(UART_0_INST, DL_UART_TX_FIFO_LEVEL_ONE_ENTRY);

    DL_UART_enable(UART_0_INST);
}

static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode   = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode   = DL_DMA_NORMAL_MODE,
    .destIncrement  = DL_DMA_ADDR_INCREMENT,
    .srcIncrement   = DL_DMA_ADDR_UNCHANGED,
    .destWidth      = DL_DMA_WIDTH_WORD,
    .srcWidth       = DL_DMA_WIDTH_WORD,
    .trigger        = I2S_0_INST_DMA_TRIGGER,
    .triggerType    = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_CH0_init(void)
{
    DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
}
SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void){
    SYSCFG_DL_DMA_CH0_init();
}


SYSCONFIG_WEAK void SYSCFG_DL_SYSTICK_init(void)
{
    /*
     * Initializes the SysTick period to 1.00 ms,
     * enables the interrupt, and starts the SysTick Timer
     */
    DL_SYSTICK_config(80000);
}

SYSCONFIG_WEAK void SYSCFG_DL_USB_0_init(void)
{

}

static const DL_I2S_Config gI2S_0_config = {
    .mode        = DL_I2S_MODE_CONTROLLER,
    .wclkInvert  = DL_I2S_WCLK_INVERSION_ENABLED,
    .phase       = DL_I2S_PHASE_DUAL,
    .samplingEdge = DL_I2S_SAMPLE_EDGE_POS,
    .sampleWordLength = 24,
    .dataDelay   = DL_I2S_DATA_DELAY_ONE,
    .emptySlotOutput = DL_I2S_EMPTY_SLOT_OUTPUT_ZERO,
    .memoryAccessLength = DL_I2S_MEMORY_LENGTH_32_BIT,
    .dataPin0Direction = DL_I2S_DATA_PIN_DIRECTION_INPUT,
    .dataPin0ValidChannelMask = 1,
    .dataPin1Direction = DL_I2S_DATA_PIN_DIRECTION_UNUSED,
    .dataPin1ValidChannelMask = 1,
    .mclkDivider = 2,
    .enableMCLK = false,
};

static const DL_I2S_ClockConfig gI2S_0_clockConfig = {
    .clockSel    = DL_I2S_CLOCK_SOURCE_SYSOSC,
    .wordBaudClockSource = DL_I2S_WORD_BAUD_CLOCK_SOURCE_INTERNAL,
    .wclkPhase   = DL_I2S_WCLK_PHASE_DUAL,
    .wclkDivider = 48,
    .bclkDivider = 7,
};

SYSCONFIG_WEAK void SYSCFG_DL_I2S_0_init(void) {
    DL_I2S_setClockConfig(I2S_0_INST, (DL_I2S_ClockConfig *) &gI2S_0_clockConfig);
    DL_I2S_init(I2S_0_INST, (DL_I2S_Config *) &gI2S_0_config);
    DL_I2S_enableFreeRun(I2S_0_INST);
    DL_I2S_disableMCLKGeneration(I2S_0_INST);
    DL_I2S_setTXFIFOThreshold(I2S_0_INST, DL_I2S_TX_FIFO_LEVEL_1_2_EMPTY);
    DL_I2S_setRXFIFOThreshold(I2S_0_INST, DL_I2S_RX_FIFO_LEVEL_1_2_FULL);

    /* Enable SPI RX interrupt as a trigger for DMA */
    DL_I2S_enableDMAReceiveEvent(I2S_0_INST, DL_I2S_DMA_INTERRUPT_RX_TRIGGER);
    DL_I2S_enableInterrupt(I2S_0_INST, (DL_I2S_INTERRUPT_DMA_DONE_RX));

    /* Enable module */
    DL_I2S_enable(I2S_0_INST);
}

