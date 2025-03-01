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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
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
    SYSCFG_DL_RTC_init();
    SYSCFG_DL_SYSCTL_CLK_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_RTC_reset(RTC);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_RTC_enablePower(RTC);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    /* Configure GPIO/IOMUX for LFXT functionality */
    DL_GPIO_initPeripheralAnalogFunction(GPIO_LFXIN_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_LFXOUT_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES1_USER_SWITCH_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES2_USER_SWITCH_2_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_setUpperPinsPolarity(GPIO_SWITCHES2_PORT, DL_GPIO_PIN_18_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(GPIO_SWITCHES2_PORT, GPIO_SWITCHES2_USER_SWITCH_2_PIN);
    DL_GPIO_enableInterrupt(GPIO_SWITCHES2_PORT, GPIO_SWITCHES2_USER_SWITCH_2_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIO_SWITCHES1_PORT, DL_GPIO_PIN_21_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(GPIO_SWITCHES1_PORT, GPIO_SWITCHES1_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(GPIO_SWITCHES1_PORT, GPIO_SWITCHES1_USER_SWITCH_1_PIN);

    /* Configure RTCOUT pin */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_RTC_IOMUX_RTCOUT, GPIO_RTC_IOMUX_RTCOUT_FUNC);

}


static const DL_SYSCTL_LFCLKConfig gSYSCTLConfig = {
    .lowCap   = false,
    .monitor  = false,
    .xt1Drive = DL_SYSCTL_LFXT_DRIVE_STRENGTH_HIGHEST,
};

SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setLFCLKSourceLFXT((DL_SYSCTL_LFCLKConfig *) &gSYSCTLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_CLK_init(void) {
    while ((DL_SYSCTL_getClockStatus() & (DL_SYSCTL_CLK_STATUS_LFXT_GOOD))
	       != (DL_SYSCTL_CLK_STATUS_LFXT_GOOD))
	{
		/* Ensure that clocks are in default POR configuration before initialization.
		* Additionally once LFXT is enabled, the internal LFOSC is disabled, and cannot
		* be re-enabled other than by executing a BOOTRST. */
		;
	}
}



static const DL_RTC_Calendar gRTCCalendarConfig = {
		.seconds    = 58,   /* Seconds = 58 */
		.minutes    = 32,   /* Minute = 32 */
		.hours      = 10,   /* Hour = 10 */
		.dayOfWeek  = 1,    /* Day of week = 1 (Monday) */
		.dayOfMonth = 5,    /* Day of month = 5*/
		.month      = 4,    /* Month = 4 (April) */
		.year       = 2021, /* Year = 2021 */
};




SYSCONFIG_WEAK void SYSCFG_DL_RTC_init(void)
{
	/* Initialize RTC Calendar */
	DL_RTC_initCalendar(RTC , gRTCCalendarConfig, DL_RTC_FORMAT_BINARY);

	/* Configure the Crystal Offset Error Calibration */
	DL_RTC_setOffsetCalibrationFrequency(RTC, DL_RTC_OFFSET_CALIBRATION_FREQUENCY_512);
}

