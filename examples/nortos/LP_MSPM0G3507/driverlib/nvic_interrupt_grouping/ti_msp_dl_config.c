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

DL_TimerG_backupConfig gTIMER_1Backup;

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
    SYSCFG_DL_TIMER_0_init();
    SYSCFG_DL_TIMER_1_init();
    SYSCFG_DL_WWDT0_init();
    /* Ensure backup structures have no valid state */
	gTIMER_1Backup.backupRdy 	= false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerG_saveConfiguration(TIMER_1_INST, &gTIMER_1Backup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerG_restoreConfiguration(TIMER_1_INST, &gTIMER_1Backup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerG_reset(TIMER_0_INST);
    DL_TimerG_reset(TIMER_1_INST);
    DL_WWDT_reset(WWDT0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerG_enablePower(TIMER_0_INST);
    DL_TimerG_enablePower(TIMER_1_INST);
    DL_WWDT_enablePower(WWDT0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_2_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_3_IOMUX);

    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_LED_2_PIN |
		GPIO_LEDS_USER_LED_3_PIN);
    DL_GPIO_enableOutput(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_LED_2_PIN |
		GPIO_LEDS_USER_LED_3_PIN);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();

}



/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   125000 Hz = 32000000 Hz / (1 * (255 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (500 ms * 125000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_0_init(void) {

    DL_TimerG_setClockConfig(TIMER_0_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_0ClockConfig);

    DL_TimerG_initTimerMode(TIMER_0_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_0TimerConfig);
    DL_TimerG_enableInterrupt(TIMER_0_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_0_INST);





}

/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   125000 Hz = 32000000 Hz / (1 * (255 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_1ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_1_INST_LOAD_VALUE = (500 ms * 125000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_1TimerConfig = {
    .period     = TIMER_1_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_1_init(void) {

    DL_TimerG_setClockConfig(TIMER_1_INST,
        (DL_TimerG_ClockConfig *) &gTIMER_1ClockConfig);

    DL_TimerG_initTimerMode(TIMER_1_INST,
        (DL_TimerG_TimerConfig *) &gTIMER_1TimerConfig);
    DL_TimerG_enableInterrupt(TIMER_1_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMER_1_INST);





}


SYSCONFIG_WEAK void SYSCFG_DL_WWDT0_init(void)
{
    /*
     * Initialize WWDT0 in Interval Timer mode with following settings
     *   Watchdog Source Clock = (LFCLK Freq) / (WWDT Clock Divider)
     *                         = 32768Hz / 4 = 8.19 kHz
     *   Watchdog Period       = (WWDT Clock Divider) ∗ (WWDT Period Count) / 32768Hz
     *                         = 4 * 2^12 / 32768Hz = 500.00 ms
     */
    DL_WWDT_initIntervalTimerMode(WWDT0_INST, DL_WWDT_CLOCK_DIVIDE_4,
        DL_WWDT_TIMER_PERIOD_12_BITS, DL_WWDT_RUN_IN_SLEEP);

    /* Enable WWDT Interval Timer interrupt */
    DL_WWDT_enableInterrupt(WWDT0_INST);

}


