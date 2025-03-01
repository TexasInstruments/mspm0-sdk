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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L2228
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
        /* Ensure VBAT Power domain status is good before any initialization */
    while ((DL_SYSCTL_getStatus() & (DL_SYSCTL_STATUS_VBAT_GOOD)) !=
        (DL_SYSCTL_STATUS_VBAT_GOOD));
    SYSCFG_DL_RTC_A_init();
    SYSCFG_DL_TAMPERIO_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_GPIO_reset(GPIOC);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_GPIO_enablePower(GPIOC);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    /* Configure GPIO/IOMUX for LFXT functionality */
    DL_GPIO_initPeripheralAnalogFunction(GPIO_LFXIN_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_LFXOUT_IOMUX);

    /* Configure RTC_A OUT pin */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_RTC_A_IOMUX_RTCOUT, GPIO_RTC_A_IOMUX_RTCOUT_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_TEST_IOMUX);

    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_TEST_PIN);
    DL_GPIO_enableOutput(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_TEST_PIN);

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
    DL_SYSCTL_setLFCLKSourceLFXT((DL_SYSCTL_LFCLKConfig *) &gSYSCTLConfig);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}


static const DL_RTC_A_Calendar gRTCCalendarConfig = {
		.seconds    = 0,   /* Seconds = 0 */
		.minutes    = 0,   /* Minute = 0 */
		.hours      = 0,   /* Hour = 0 */
		.dayOfWeek  = 0,    /* Day of week = 0 (Sunday) */
		.dayOfMonth = 1,    /* Day of month = 1*/
		.month      = 1,    /* Month = 1 (January) */
		.year       = 2025, /* Year = 2025 */
};

SYSCONFIG_WEAK void SYSCFG_DL_RTC_A_init(void)
{
	/* Initialize RTC Calendar */
	DL_RTC_A_initCalendar(RTC_A , gRTCCalendarConfig, DL_RTC_A_FORMAT_BINARY);


	/* Configure sources for Time Stamp Event */
	DL_RTC_A_setTimeStampEventSource(RTC_A, (DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_0));
	DL_RTC_A_setTimeStampEventCapture(RTC_A, DL_RTC_A_TIME_STAMP_EVENT_CAPTURE_FIRST);
	/* Configure the Crystal Offset Error Calibration */
	DL_RTC_A_setOffsetCalibrationFrequency(RTC_A, DL_RTC_A_OFFSET_CALIBRATION_FREQUENCY_32_KHZ);
}

SYSCONFIG_WEAK void SYSCFG_DL_TAMPERIO_init(void)
{
	/* Tamper I/O 0 Configuration Begin */
	DL_TamperIO_setMode(TAMPERIO, DL_TAMPERIO_0, 
		DL_TAMPERIO_MODE_TAMPER);
	DL_TamperIO_enableInput(TAMPERIO, DL_TAMPERIO_0);
	DL_TamperIO_setEdgeDetectionPolarity(TAMPERIO, DL_TAMPERIO_0, 
		DL_TAMPERIO_EDGE_DETECTION_POLARITY_RISE);
	DL_TamperIO_setGlitchFilterWidth(TAMPERIO, DL_TAMPERIO_0, 
		DL_TAMPERIO_GLITCH_FILTER_WIDTH_30_USEC);

	DL_TamperIO_enableInterrupt(TAMPERIO, (DL_TAMPERIO_INTERRUPT_TAMPERIO_0));
}

