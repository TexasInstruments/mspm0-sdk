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
    SYSCFG_DL_VREF_init();
    SYSCFG_DL_COMP_init();
    /* Ensure backup structures have no valid state */
	gPWM_0Backup.backupRdy 	= false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(PWM_0_INST, &gPWM_0Backup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(PWM_0_INST, &gPWM_0Backup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(PWM_0_INST);
    DL_VREF_reset(VREF);
    DL_COMP_reset(COMP_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(PWM_0_INST);
    DL_VREF_enablePower(VREF);
    DL_COMP_enablePower(COMP_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C0_IOMUX,GPIO_PWM_0_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C0_PORT, GPIO_PWM_0_C0_PIN);

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
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   125000 Hz = 32000000 Hz / (1 * (255 + 1))
 */
static const DL_TimerA_ClockConfig gPWM_0ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 255U
};

static const DL_TimerA_PWMConfig gPWM_0Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 2000,
    .isTimerWithFourCC = true,
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
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 500, DL_TIMER_CC_0_INDEX);

    DL_TimerA_enableClock(PWM_0_INST);


    
    DL_TimerA_setCCPDirection(PWM_0_INST , DL_TIMER_CC0_OUTPUT );


    DL_TimerA_setFaultSourceConfig(PWM_0_INST, (DL_TIMERA_FAULT_SOURCE_COMP0_SENSE_HIGH));
    DL_TimerA_setFaultConfig(PWM_0_INST, DL_TIMERA_FAULT_CONFIG_TFIM_DISABLED
		 | DL_TIMERA_FAULT_CONFIG_FL_NO_LATCH
		 | DL_TIMERA_FAULT_CONFIG_FI_DEPENDENT
		 | DL_TIMERA_FAULT_CONFIG_FIEN_ENABLED);
    DL_TimerA_setFaultInputFilterConfig(PWM_0_INST,
        DL_TIMERA_FAULT_FILTER_FILTERED,
        DL_TIMERA_FAULT_FILTER_CPV_CONSEC_PER,
        DL_TIMERA_FAULT_FILTER_FP_PER_3);
    DL_TimerA_configFaultOutputAction(PWM_0_INST,
        DL_TIMERA_FAULT_ENTRY_CCP_HIGHZ,
        DL_TIMERA_FAULT_EXIT_CCP_DISABLED,
        DL_TIMER_CC_0_INDEX);
    DL_TimerA_configFaultCounter(PWM_0_INST,
        DL_TIMERA_FAULT_ENTRY_CTR_SUSP_COUNT, DL_TIMERA_FAULT_EXIT_CTR_CVAE_ACTION);
    DL_TimerA_enableFaultInput(PWM_0_INST);
    DL_TimerA_disableClockFaultDetection(PWM_0_INST);
}



static const DL_VREF_Config gVREFConfig = {
    .vrefEnable     = DL_VREF_ENABLE_ENABLE,
    .bufConfig      = DL_VREF_BUFCONFIG_OUTPUT_1_4V,
    .shModeEnable   = DL_VREF_SHMODE_DISABLE,
    .holdCycleCount = DL_VREF_HOLD_MIN,
    .shCycleCount   = DL_VREF_SH_MIN,
};

SYSCONFIG_WEAK void SYSCFG_DL_VREF_init(void) {
    DL_VREF_configReference(VREF,
        (DL_VREF_Config *) &gVREFConfig);
}


/* COMP Initialization */
static const DL_COMP_Config gCOMPConfig = {
    .channelEnable = DL_COMP_ENABLE_CHANNEL_POS,
    .mode          = DL_COMP_MODE_FAST,
    .negChannel    = DL_COMP_IMSEL_CHANNEL_0,
    .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
    .hysteresis    = DL_COMP_HYSTERESIS_30,
    .polarity      = DL_COMP_POLARITY_NON_INV
};
static const DL_COMP_RefVoltageConfig gCOMPVRefConfig = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = DL_COMP_REF_SOURCE_VDDA_DAC,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_NEG,
    .controlSelect  = DL_COMP_DAC_CONTROL_SW,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0
};

SYSCONFIG_WEAK void SYSCFG_DL_COMP_init(void)
{
    DL_COMP_init(COMP_INST, (DL_COMP_Config *) &gCOMPConfig);
    DL_COMP_enableOutputFilter(COMP_INST,DL_COMP_FILTER_DELAY_1200);
    DL_COMP_refVoltageInit(COMP_INST, (DL_COMP_RefVoltageConfig *) &gCOMPVRefConfig);
    DL_COMP_setDACCode0(COMP_INST, COMP_DACCODE0);

    DL_COMP_enable(COMP_INST);

}


