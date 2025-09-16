/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  DO NOT EDIT - This file is generated for the MSPM0L111X
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
    SYSCFG_DL_I2S_init();
    SYSCFG_DL_DMA_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_I2S_reset(I2S_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_I2S_enablePower(I2S_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_I2S_IOMUX_WCLK, GPIO_I2S_IOMUX_WCLK_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_I2S_IOMUX_BCLK, GPIO_I2S_IOMUX_BCLK_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_I2S_IOMUX_AD0, GPIO_I2S_IOMUX_AD0_FUNC);

    /*
     * Custom function that enables readback of output pins for loopback
     * Recommended to loopback clock signals for better timing closure
     */
    DL_GPIO_inena_Function(GPIO_I2S_IOMUX_WCLK, IOMUX_PINCM_INENA_ENABLE);
    DL_GPIO_inena_Function(GPIO_I2S_IOMUX_BCLK, IOMUX_PINCM_INENA_ENABLE);
}

void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
}

/* Configurations for DMA channel 0 */
static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode  = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode  = DL_DMA_NORMAL_MODE,
    .destIncrement = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement  = DL_DMA_ADDR_INCREMENT,
    .destWidth     = DL_DMA_WIDTH_HALF_WORD,
    .srcWidth      = DL_DMA_WIDTH_HALF_WORD,
    .trigger       = I2S_INST_DMA_TRIGGER_0,
    .triggerType   = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

SYSCONFIG_WEAK void SYSCFG_DL_DMA_init(void)
{
    DL_DMA_initChannel(
        DMA, DMA_CH0_CHAN_ID, (DL_DMA_Config *) &gDMA_CH0Config);
}

static const DL_I2S_ClockConfig gI2S_Clk_config = {
    .clockSel            = DL_I2S_CLOCK_SOURCE_SYSOSC,
    .wordBaudClockSource = DL_I2S_WORD_BAUD_CLOCK_SOURCE_INTERNAL,
    .wclkPhase           = DL_I2S_WCLK_PHASE_DUAL,
    /* Fs = BCLK / (2 * WCLKDIV) = 4MHz / (2 * 64) ~ 32KHz (freq slightly off) */
    /* WCLK freq IS audio sampling frequency */
    .wclkDivider = 64,
    /* 32MHz (SYSOSC) / 8 = 4MHz */
    .bclkDivider = 8,
};

static const DL_I2S_Config gI2S_0_config = {
    .mode         = DL_I2S_MODE_CONTROLLER,
    .wclkInvert   = DL_I2S_WCLK_INVERSION_ENABLED,
    .phase        = DL_I2S_PHASE_DUAL,
    .samplingEdge = DL_I2S_SAMPLE_EDGE_POS,
    /* Data is signed int16_t. Register mask requires this to be 1 off */
    .sampleWordLength   = 15,
    .dataDelay          = DL_I2S_DATA_DELAY_ONE,
    .emptySlotOutput    = DL_I2S_EMPTY_SLOT_OUTPUT_ZERO,
    .memoryAccessLength = DL_I2S_MEMORY_LENGTH_16_BIT,
    .dataPin0Direction  = DL_I2S_DATA_PIN_DIRECTION_OUTPUT,
    /* Channel 0 and 1 bits should always be set for stereo */
    .dataPin0ValidChannelMask = 0x03,
    .dataPin1Direction        = DL_I2S_DATA_PIN_DIRECTION_UNUSED,
    .dataPin1ValidChannelMask = 0x03,
    .mclkDivider              = 0U,
    .enableMCLK               = false,
};

void SYSCFG_DL_I2S_init(void)
{
    DL_I2S_setClockConfig(I2S_INST, (DL_I2S_ClockConfig *) &gI2S_Clk_config);

    DL_I2S_init(I2S_INST, (DL_I2S_Config *) &gI2S_0_config);

    DL_I2S_setTXFIFOThreshold(I2S_INST, DL_I2S_TX_FIFO_LEVEL_1_2_EMPTY);
    DL_I2S_setRXFIFOThreshold(I2S_INST, DL_I2S_RX_FIFO_LEVEL_1_2_FULL);

    DL_I2S_enableDMATransmitEvent(I2S_INST, DL_I2S_DMA_INTERRUPT_TX_TRIGGER);
    DL_I2S_enableInterrupt(I2S_INST, DL_I2S_INTERRUPT_DMA_DONE_TX);
}
