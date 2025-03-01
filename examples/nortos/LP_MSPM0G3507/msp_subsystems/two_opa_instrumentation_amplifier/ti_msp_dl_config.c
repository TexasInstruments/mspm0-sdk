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
    SYSCFG_DL_TIMER0_init();
    SYSCFG_DL_ADC_init();
    SYSCFG_DL_OPA_0_init();
    SYSCFG_DL_OPA_1_init();
}



SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerG_reset(TIMER0_INST);
    DL_ADC12_reset(ADC_INST);
    DL_OPA_reset(OPA_0_INST);
    DL_OPA_reset(OPA_1_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerG_enablePower(TIMER0_INST);
    DL_ADC12_enablePower(ADC_INST);
    DL_OPA_enablePower(OPA_0_INST);
    DL_OPA_enablePower(OPA_1_INST);
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
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

}



/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32768 Hz = 32768 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER0_INST_LOAD_VALUE = (1 ms * 32768 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER0TimerConfig = {
    .period     = TIMER0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER0_init(void) {

    DL_TimerG_setClockConfig(TIMER0_INST,
        (DL_TimerG_ClockConfig *) &gTIMER0ClockConfig);

    DL_TimerG_initTimerMode(TIMER0_INST,
        (DL_TimerG_TimerConfig *) &gTIMER0TimerConfig);
    DL_TimerG_enableClock(TIMER0_INST);


    DL_TimerG_enableEvent(TIMER0_INST, DL_TIMERG_EVENT_ROUTE_1, (DL_TIMERG_EVENT_ZERO_EVENT));

    DL_TimerG_setPublisherChanID(TIMER0_INST, DL_TIMERG_PUBLISHER_INDEX_0, TIMER0_INST_PUB_0_CH);



}


/* ADC Initialization */
static const DL_ADC12_ClockConfig gADCClockConfig = {
    .clockSel       = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio    = DL_ADC12_CLOCK_DIVIDE_4,
    .freqRange      = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};
SYSCONFIG_WEAK void SYSCFG_DL_ADC_init(void)
{
    DL_ADC12_setClockConfig(ADC_INST, (DL_ADC12_ClockConfig *) &gADCClockConfig);
    DL_ADC12_initSingleSample(ADC_INST,
        DL_ADC12_REPEAT_MODE_DISABLED, DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SAMP_CONV_RES_12_BIT, DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);
    DL_ADC12_configConversionMem(ADC_INST, ADC_ADCMEM_0,
        DL_ADC12_INPUT_CHAN_13, DL_ADC12_REFERENCE_VOLTAGE_VDDA, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_setSampleTime0(ADC_INST,1000);
    DL_ADC12_setSubscriberChanID(ADC_INST,ADC_INST_SUB_CH);
    /* Enable ADC12 interrupt */
    DL_ADC12_clearInterruptStatus(ADC_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
    DL_ADC12_enableInterrupt(ADC_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
    DL_ADC12_enableConversions(ADC_INST);
}

static const DL_OPA_Config gOPA_0Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

SYSCONFIG_WEAK void SYSCFG_DL_OPA_0_init(void)
{
    DL_OPA_init(OPA_0_INST, (DL_OPA_Config *) &gOPA_0Config0);

    DL_OPA_enable(OPA_0_INST);
}
static const DL_OPA_Config gOPA_1Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

SYSCONFIG_WEAK void SYSCFG_DL_OPA_1_init(void)
{
    DL_OPA_init(OPA_1_INST, (DL_OPA_Config *) &gOPA_1Config0);

    DL_OPA_enable(OPA_1_INST);
}

