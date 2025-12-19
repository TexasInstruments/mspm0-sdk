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

DL_TimerA_backupConfig gTIMER_0Backup;

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
    SYSCFG_DL_LIN_0_init();
    /* Ensure backup structures have no valid state */
	gTIMER_0Backup.backupRdy 	= false;

}
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
SYSCONFIG_WEAK bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_saveConfiguration(TIMER_0_INST, &gTIMER_0Backup);

    return retStatus;
}


SYSCONFIG_WEAK bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_TimerA_restoreConfiguration(TIMER_0_INST, &gTIMER_0Backup, false);

    return retStatus;
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_TimerA_reset(TIMER_0_INST);
    DL_UART_reset(LIN_0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_TimerA_enablePower(TIMER_0_INST);
    DL_UART_enablePower(LIN_0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_CLKOUT_IOMUX, GPIO_CLKOUT_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_CLKOUT_PORT, GPIO_CLKOUT_PIN);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_LIN_0_IOMUX_TX, GPIO_LIN_0_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_LIN_0_IOMUX_RX, GPIO_LIN_0_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_LIN_ENABLE_USER_LIN_ENABLE_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_2_IOMUX);

    DL_GPIO_clearPins(GPIOA, GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_clearPins(GPIOB, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN |
		GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_enableOutput(GPIOB, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN |
		GPIO_LEDS_USER_LED_1_PIN);

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
    DL_SYSCTL_enableExternalClock(DL_SYSCTL_CLK_OUT_SOURCE_SYSOSC, DL_SYSCTL_CLK_OUT_DIVIDE_DISABLE);

}



/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32768 Hz = 32768 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMER_0_INST_LOAD_VALUE = (0 ms * 32768 Hz) - 1
 */
static const DL_TimerA_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_ONE_SHOT,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMER_0_init(void) {

    DL_TimerA_setClockConfig(TIMER_0_INST,
        (DL_TimerA_ClockConfig *) &gTIMER_0ClockConfig);

    DL_TimerA_initTimerMode(TIMER_0_INST,
        (DL_TimerA_TimerConfig *) &gTIMER_0TimerConfig);
    DL_TimerA_enableInterrupt(TIMER_0_INST , DL_TIMERA_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableClock(TIMER_0_INST);





}


static const DL_UART_ClockConfig gLIN_0ClockConfig = {
    .clockSel    = DL_UART_CLOCK_BUSCLK,
    .divideRatio = DL_UART_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Config gLIN_0Config = {
    .mode        = DL_UART_MODE_NORMAL,
    .direction   = DL_UART_DIRECTION_TX_RX,
    .flowControl = DL_UART_FLOW_CONTROL_NONE,
    .parity      = DL_UART_PARITY_NONE,
    .wordLength  = DL_UART_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_LIN_0_init(void)
{
    DL_UART_setClockConfig(LIN_0_INST, (DL_UART_ClockConfig *) &gLIN_0ClockConfig);

    DL_UART_init(LIN_0_INST, (DL_UART_Config *) &gLIN_0Config);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 19200
     *  Actual baud rate: 19199.04
     */
    DL_UART_setOversampling(LIN_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_setBaudRateDivisor(LIN_0_INST, LIN_0_IBRD_32_MHZ_19200_BAUD, LIN_0_FBRD_32_MHZ_19200_BAUD);


    /* Configure Interrupts */
    DL_UART_enableInterrupt(LIN_0_INST,
                                 DL_UART_INTERRUPT_BREAK_ERROR |
                                 DL_UART_INTERRUPT_LINC0_MATCH |
                                 DL_UART_INTERRUPT_LIN_COUNTER_OVERFLOW |
                                 DL_UART_INTERRUPT_LIN_FALLING_EDGE |
                                 DL_UART_INTERRUPT_OVERRUN_ERROR);

    /* Configure FIFOs */
    DL_UART_enableFIFOs(LIN_0_INST);
    DL_UART_setRXFIFOThreshold(LIN_0_INST, DL_UART_RX_FIFO_LEVEL_ONE_ENTRY);
    DL_UART_setTXFIFOThreshold(LIN_0_INST, DL_UART_TX_FIFO_LEVEL_EMPTY);

    /* Configure LIN settings */
    DL_UART_enableLINCounter(LIN_0_INST);
    DL_UART_setLINCounterValue(LIN_0_INST, 0);
    DL_UART_enableLINCounterCompareMatch(LIN_0_INST);
    DL_UART_setLINCounterCompareValue(LIN_0_INST, LIN_0_COUNTER_COMPARE_VALUE);
    DL_UART_enableLINCountWhileLow(LIN_0_INST);
    DL_UART_enableLINCounterClearOnFallingEdge(LIN_0_INST);

    DL_UART_enable(LIN_0_INST);
}

