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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L2117
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
    SYSCFG_DL_LCD_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_LCD_reset(LCD);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_LCD_enablePower(LCD);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}


static const DL_LCD_Config gLCDConfig = {
    .frequencyDivider   = DL_LCD_FREQ_DIVIDE_4,
    .muxRate            = DL_LCD_MUX_RATE_4,
    .lowPowerWaveform   = DL_LCD_WAVEFORM_POWERMODE_STANDARD
};

SYSCONFIG_WEAK void SYSCFG_DL_LCD_init(void)
{
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_33);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_34);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_7);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_35);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_36);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_37);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_38);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_18);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_26);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_27);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_28);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_29);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_30);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_48);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_47);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_45);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_31);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_32);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_19);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_20);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_23);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_39);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_40);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_41);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_24);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_42);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_43);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_44);
    DL_LCD_init(LCD, (DL_LCD_Config *) &gLCDConfig);
    DL_LCD_turnSegmentsOn(LCD);
    DL_LCD_setBiasVoltageSource(LCD, DL_LCD_BIAS_VOLTAGE_SOURCE_AVDD);
    DL_LCD_setR33source(LCD, DL_LCD_R33_SOURCE_INTERNAL);
    DL_LCD_enableInternalBias(LCD);
    DL_LCD_setInternalBiasPowerMode(LCD, DL_LCD_POWER_MODE_LOW);

    DL_LCD_clearAllMemoryRegs(LCD); //
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_33, DL_LCD_COM_0);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_34, DL_LCD_COM_1);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_7, DL_LCD_COM_2);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_35, DL_LCD_COM_3);
    DL_LCD_enable(LCD);
}

