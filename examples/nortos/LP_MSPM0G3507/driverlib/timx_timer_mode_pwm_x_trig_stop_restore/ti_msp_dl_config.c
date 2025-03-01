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

DL_TimerA_backupConfig gPWM_0Backup;
DL_TimerG_backupConfig gPWM_1Backup;

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
    SYSCFG_DL_PWM_0_init();
    SYSCFG_DL_PWM_1_init();
    SYSCFG_DL_TIMER_0_init();
    SYSCFG_DL_PWM_Cross_Trigger_init();
    /* Ensure backup structures have no valid state */
	gPWM_0Backup.backupRdy 	= false;
	gPWM_1Backup.backupRdy 	= false;


}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(PWM_0_INST, &gPWM_0Backup);
	retStatus &= DL_TimerG_saveConfiguration(PWM_1_INST, &gPWM_1Backup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(PWM_0_INST, &gPWM_0Backup, false);
	retStatus &= DL_TimerG_restoreConfiguration(PWM_1_INST, &gPWM_1Backup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(PWM_0_INST);
    DL_TimerG_reset(PWM_1_INST);
    DL_TimerG_reset(TIMER_0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(PWM_0_INST);
    DL_TimerG_enablePower(PWM_1_INST);
    DL_TimerG_enablePower(TIMER_0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{
    const uint8_t unusedPinIndexes[] =
    {
        IOMUX_PINCM30, IOMUX_PINCM31, IOMUX_PINCM32, IOMUX_PINCM33,
        IOMUX_PINCM34, IOMUX_PINCM35, IOMUX_PINCM36, IOMUX_PINCM38,
        IOMUX_PINCM39, IOMUX_PINCM40, IOMUX_PINCM43, IOMUX_PINCM44,
        IOMUX_PINCM45, IOMUX_PINCM46, IOMUX_PINCM47, IOMUX_PINCM48,
        IOMUX_PINCM49, IOMUX_PINCM50, IOMUX_PINCM51, IOMUX_PINCM52,
        IOMUX_PINCM53, IOMUX_PINCM54, IOMUX_PINCM55, IOMUX_PINCM56,
        IOMUX_PINCM57, IOMUX_PINCM58, IOMUX_PINCM59, IOMUX_PINCM60,
        IOMUX_PINCM2, IOMUX_PINCM4, IOMUX_PINCM5, IOMUX_PINCM7,
        IOMUX_PINCM8, IOMUX_PINCM9, IOMUX_PINCM10, IOMUX_PINCM11,
        IOMUX_PINCM12, IOMUX_PINCM14, IOMUX_PINCM15, IOMUX_PINCM16,
        IOMUX_PINCM18, IOMUX_PINCM19, IOMUX_PINCM20, IOMUX_PINCM21,
        IOMUX_PINCM22, IOMUX_PINCM23, IOMUX_PINCM24, IOMUX_PINCM25,
        IOMUX_PINCM26, IOMUX_PINCM27, IOMUX_PINCM28, IOMUX_PINCM29
    };

    for(int i = 0; i < sizeof(unusedPinIndexes)/sizeof(unusedPinIndexes[0]); i++)
    {
        DL_GPIO_initDigitalOutput(unusedPinIndexes[i]);
    }

    DL_GPIO_clearPins(GPIOA,
        (DL_GPIO_PIN_12 | DL_GPIO_PIN_13 | DL_GPIO_PIN_14 | DL_GPIO_PIN_16 |
        DL_GPIO_PIN_17 | DL_GPIO_PIN_18 | DL_GPIO_PIN_21 | DL_GPIO_PIN_22 |
        DL_GPIO_PIN_23 | DL_GPIO_PIN_24 | DL_GPIO_PIN_25 | DL_GPIO_PIN_26 |
        DL_GPIO_PIN_27 | DL_GPIO_PIN_1 | DL_GPIO_PIN_29 | DL_GPIO_PIN_30 |
        DL_GPIO_PIN_2 | DL_GPIO_PIN_3 | DL_GPIO_PIN_4 | DL_GPIO_PIN_5 |
        DL_GPIO_PIN_6 | DL_GPIO_PIN_7 | DL_GPIO_PIN_8 | DL_GPIO_PIN_9 |
        DL_GPIO_PIN_10 | DL_GPIO_PIN_11));
    DL_GPIO_enableOutput(GPIOA,
        (DL_GPIO_PIN_12 | DL_GPIO_PIN_13 | DL_GPIO_PIN_14 | DL_GPIO_PIN_16 |
        DL_GPIO_PIN_17 | DL_GPIO_PIN_18 | DL_GPIO_PIN_21 | DL_GPIO_PIN_22 |
        DL_GPIO_PIN_23 | DL_GPIO_PIN_24 | DL_GPIO_PIN_25 | DL_GPIO_PIN_26 |
        DL_GPIO_PIN_27 | DL_GPIO_PIN_1 | DL_GPIO_PIN_29 | DL_GPIO_PIN_30 |
        DL_GPIO_PIN_2 | DL_GPIO_PIN_3 | DL_GPIO_PIN_4 | DL_GPIO_PIN_5 |
        DL_GPIO_PIN_6 | DL_GPIO_PIN_7 | DL_GPIO_PIN_8 | DL_GPIO_PIN_9 |
        DL_GPIO_PIN_10 | DL_GPIO_PIN_11));
    DL_GPIO_clearPins(GPIOB,
        (DL_GPIO_PIN_13 | DL_GPIO_PIN_14 | DL_GPIO_PIN_15 | DL_GPIO_PIN_16 |
        DL_GPIO_PIN_17 | DL_GPIO_PIN_18 | DL_GPIO_PIN_19 | DL_GPIO_PIN_20 |
        DL_GPIO_PIN_21 | DL_GPIO_PIN_22 | DL_GPIO_PIN_23 | DL_GPIO_PIN_24 |
        DL_GPIO_PIN_25 | DL_GPIO_PIN_26 | DL_GPIO_PIN_27 | DL_GPIO_PIN_0 |
        DL_GPIO_PIN_2 | DL_GPIO_PIN_3 | DL_GPIO_PIN_5 | DL_GPIO_PIN_6 |
        DL_GPIO_PIN_7 | DL_GPIO_PIN_8 | DL_GPIO_PIN_9 | DL_GPIO_PIN_10 |
        DL_GPIO_PIN_11 | DL_GPIO_PIN_12));
    DL_GPIO_enableOutput(GPIOB,
        (DL_GPIO_PIN_13 | DL_GPIO_PIN_14 | DL_GPIO_PIN_15 | DL_GPIO_PIN_16 |
        DL_GPIO_PIN_17 | DL_GPIO_PIN_18 | DL_GPIO_PIN_19 | DL_GPIO_PIN_20 |
        DL_GPIO_PIN_21 | DL_GPIO_PIN_22 | DL_GPIO_PIN_23 | DL_GPIO_PIN_24 |
        DL_GPIO_PIN_25 | DL_GPIO_PIN_26 | DL_GPIO_PIN_27 | DL_GPIO_PIN_0 |
        DL_GPIO_PIN_2 | DL_GPIO_PIN_3 | DL_GPIO_PIN_5 | DL_GPIO_PIN_6 |
        DL_GPIO_PIN_7 | DL_GPIO_PIN_8 | DL_GPIO_PIN_9 | DL_GPIO_PIN_10 |
        DL_GPIO_PIN_11 | DL_GPIO_PIN_12));

    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C0_IOMUX,GPIO_PWM_0_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C0_PORT, GPIO_PWM_0_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C1_IOMUX,GPIO_PWM_0_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C1_PORT, GPIO_PWM_0_C1_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C0_IOMUX,GPIO_PWM_1_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C0_PORT, GPIO_PWM_1_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C1_IOMUX,GPIO_PWM_1_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C1_PORT, GPIO_PWM_1_C1_PIN);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutputFeatures(GPIO_LEDS_USER_TEST_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
		 DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_TEST_PIN);
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_enableOutput(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_TEST_PIN);

}


SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}


/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gPWM_0ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerA_PWMConfig gPWM_0Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1600,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_0_init(void) {

    DL_TimerA_setClockConfig(
        PWM_0_INST, (DL_TimerA_ClockConfig *) &gPWM_0ClockConfig);

    DL_TimerA_initPWMMode(
        PWM_0_INST, (DL_TimerA_PWMConfig *) &gPWM_0Config);

    // Set Counter control to the smallest CC index being used
    DL_TimerA_setCounterControl(PWM_0_INST,DL_TIMER_CZC_CCCTL0_ZCOND,DL_TIMER_CAC_CCCTL0_ACOND,DL_TIMER_CLC_CCCTL0_LCOND);

    DL_TimerA_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 399, DL_TIMER_CC_0_INDEX);

    DL_TimerA_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_1_INDEX);

    DL_TimerA_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_1_INDEX);
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 880, DL_TIMER_CC_1_INDEX);

    DL_TimerA_enableClock(PWM_0_INST);


    
    DL_TimerA_setCCPDirection(PWM_0_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );



     /* DL_TIMER_CROSS_TRIG_SRC is a Don't Care field when Cross Trigger Source is set to Software */
    DL_TimerA_configCrossTrigger(PWM_0_INST, DL_TIMER_CROSS_TRIG_SRC_FSUB0,
	DL_TIMER_CROSS_TRIGGER_INPUT_DISABLED, DL_TIMER_CROSS_TRIGGER_MODE_ENABLED
		);

    DL_TimerA_setCaptureCompareInput(PWM_0_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_0_INDEX);

    DL_TimerA_setCaptureCompareInput(PWM_0_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_1_INDEX);

    /*
     * Determines the external triggering event to trigger the module (self-triggered in main configuration)
     * and triggered by specific timer in secondary configuration
     */
    DL_TimerA_setExternalTriggerEvent(PWM_0_INST,DL_TIMER_EXT_TRIG_SEL_TRIG_1);
    DL_TimerA_enableExternalTrigger(PWM_0_INST);
    uint32_t temp;
    temp = DL_TimerA_getCaptureCompareCtl(PWM_0_INST, DL_TIMER_CC_0_INDEX);
    DL_TimerA_setCaptureCompareCtl(PWM_0_INST, DL_TIMER_CC_MODE_COMPARE, temp | (uint32_t) DL_TIMER_CC_LCOND_TRIG_RISE, DL_TIMER_CC_0_INDEX);

    temp = DL_TimerA_getCaptureCompareCtl(PWM_0_INST, DL_TIMER_CC_1_INDEX);
    DL_TimerA_setCaptureCompareCtl(PWM_0_INST, DL_TIMER_CC_MODE_COMPARE, temp | (uint32_t) DL_TIMER_CC_LCOND_TRIG_RISE, DL_TIMER_CC_1_INDEX);

}
/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_1ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_1Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1600,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_PWM_1_init(void) {

    DL_TimerG_setClockConfig(
        PWM_1_INST, (DL_TimerG_ClockConfig *) &gPWM_1ClockConfig);

    DL_TimerG_initPWMMode(
        PWM_1_INST, (DL_TimerG_PWMConfig *) &gPWM_1Config);

    // Set Counter control to the smallest CC index being used
    DL_TimerG_setCounterControl(PWM_1_INST,DL_TIMER_CZC_CCCTL0_ZCOND,DL_TIMER_CAC_CCCTL0_ACOND,DL_TIMER_CLC_CCCTL0_LCOND);

    DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 479, DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

    DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 639, DL_TIMER_CC_1_INDEX);

    DL_TimerG_enableClock(PWM_1_INST);


    
    DL_TimerG_setCCPDirection(PWM_1_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );



    DL_TimerG_setCaptureCompareInput(PWM_1_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_0_INDEX);

    DL_TimerG_setCaptureCompareInput(PWM_1_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_1_INDEX);

    /*
     * Determines the external triggering event to trigger the module (self-triggered in main configuration)
     * and triggered by specific timer in secondary configuration
     */
    DL_TimerG_setExternalTriggerEvent(PWM_1_INST,DL_TIMER_EXT_TRIG_SEL_TRIG_1);
    DL_TimerG_enableExternalTrigger(PWM_1_INST);
    uint32_t temp;
    temp = DL_TimerG_getCaptureCompareCtl(PWM_1_INST, DL_TIMER_CC_0_INDEX);
    DL_TimerG_setCaptureCompareCtl(PWM_1_INST, DL_TIMER_CC_MODE_COMPARE, temp | (uint32_t) DL_TIMER_CC_LCOND_TRIG_RISE, DL_TIMER_CC_0_INDEX);

    temp = DL_TimerG_getCaptureCompareCtl(PWM_1_INST, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareCtl(PWM_1_INST, DL_TIMER_CC_MODE_COMPARE, temp | (uint32_t) DL_TIMER_CC_LCOND_TRIG_RISE, DL_TIMER_CC_1_INDEX);

}

SYSCONFIG_WEAK void SYSCFG_DL_PWM_Cross_Trigger_init(void) {
}


/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32768 Hz = 32768 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (1 * 32768 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_ONE_SHOT,
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


