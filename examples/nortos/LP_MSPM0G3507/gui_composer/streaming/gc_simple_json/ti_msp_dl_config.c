/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "ti_msp_dl_config.h"

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_UART_init();
    SYSCFG_DL_TIMER_init();
    SYSCFG_DL_SYSCTL_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_UART_Main_reset(UART_INST);
    DL_TimerG_reset(TIMER_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_UART_Main_enablePower(UART_INST);
    DL_TimerG_enablePower(TIMER_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{
    /* Configure UART_INST */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_IOMUX_TX, GPIO_UART_IOMUX_TX_FUNCTION);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_IOMUX_RX, GPIO_UART_IOMUX_RX_FUNCTION);

    /* Initialize USER_SWITCH_1_INST pins */
    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES1_USER_SWITCH_1_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_setUpperPinsPolarity(
        GPIO_SWITCHES1_PORT, DL_GPIO_PIN_21_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(
        GPIO_SWITCHES1_PORT, GPIO_SWITCHES1_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(
        GPIO_SWITCHES1_PORT, GPIO_SWITCHES1_USER_SWITCH_1_PIN);

    /* Initialize USER_SWITCH_2_INST pins */
    DL_GPIO_initDigitalInput(GPIO_SWITCHES2_USER_SWITCH_1_IOMUX);
    DL_GPIO_setUpperPinsPolarity(
        GPIO_SWITCHES2_PORT, DL_GPIO_PIN_18_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(
        GPIO_SWITCHES2_PORT, GPIO_SWITCHES2_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(
        GPIO_SWITCHES2_PORT, GPIO_SWITCHES2_USER_SWITCH_1_PIN);
}

void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
}

static const DL_UART_Main_ClockConfig gUARTClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1};

static const DL_UART_Main_Config gUARTConfig = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE};

void SYSCFG_DL_UART_init(void)
{
    /* UART clock configuration to use BUSCLK at 8MHz */
    DL_UART_Main_setClockConfig(
        UART_INST, (DL_UART_Main_ClockConfig *) &gUARTClockConfig);

    /* UART configuration to operate in normal mode */
    DL_UART_Main_init(UART_INST, (DL_UART_Main_Config *) &gUARTConfig);

    DL_UART_Main_configBaudRate(UART_INST, 32000000, 9600);

    /* Configure RX interrupt to trigger when there is one entry in the RX FIFO */
    DL_UART_Main_setRXFIFOThreshold(
        UART_INST, DL_UART_MAIN_RX_FIFO_LEVEL_ONE_ENTRY);
    DL_UART_Main_enableInterrupt(UART_INST, DL_UART_MAIN_INTERRUPT_RX);

    DL_UART_Main_enable(UART_INST);
}

/*
 * Timer clock configuration to be sourced by BUSCLK running at 32 Mhz
 * This configures timer to generate 500msec delay
 * ((32MHz / divideRatio / prescale) * TARGET DELAY) = period
 * ((32MHz / 1 / 255) * 0.5s) = 62745 - 1 = 62744 due to N+1 ticks in down mode
 */
static const DL_TimerG_ClockConfig gTIMER_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U};

static const DL_TimerG_TimerConfig gTIMER_INSTTimerConfig = {.period = 62744,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP};

void SYSCFG_DL_TIMER_init(void)
{
    DL_TimerG_setClockConfig(
        TIMER_INST, (DL_TimerG_ClockConfig *) &gTIMER_INSTClockConfig);

    DL_TimerG_initTimerMode(
        TIMER_INST, (DL_TimerG_TimerConfig *) &gTIMER_INSTTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

    DL_TimerG_enableClock(TIMER_INST);
}
