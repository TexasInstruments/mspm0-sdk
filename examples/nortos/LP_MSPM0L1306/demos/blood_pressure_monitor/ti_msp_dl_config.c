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

#include "ti_msp_dl_config.h"

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_UART_init();
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_OPA_0_init();
    SYSCFG_DL_OPA_1_init();
    SYSCFG_DL_CAPTURE_1_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_UART_Main_reset(UART_INST);
    DL_OPA_reset(OPA_0_INST);
    DL_OPA_reset(OPA_1_INST);
    DL_TimerG_reset(CAPTURE_1_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_UART_Main_enablePower(UART_INST);
    DL_OPA_enablePower(OPA_0_INST);
    DL_OPA_enablePower(OPA_1_INST);
    DL_TimerG_enablePower(CAPTURE_1_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

/* GPIO init */
void SYSCFG_DL_GPIO_init(void)
{
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_IOMUX_TX, GPIO_UART_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_IOMUX_RX, GPIO_UART_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_PINS_VALVE_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_PINS_MOTOR_IOMUX);

    DL_GPIO_clearPins(
        GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN | GPIO_PINS_MOTOR_PIN);
    DL_GPIO_enableOutput(
        GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN | GPIO_PINS_MOTOR_PIN);
}

/* UART init */
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
    DL_UART_Main_setClockConfig(
        UART_INST, (DL_UART_Main_ClockConfig *) &gUARTClockConfig);

    DL_UART_Main_init(UART_INST, (DL_UART_Main_Config *) &gUARTConfig);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 115200
     *  Actual baud rate: 115211.52
     */
    DL_UART_Main_setOversampling(UART_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(
        UART_INST, UART_IBRD_32_MHZ_115200_BAUD, UART_FBRD_32_MHZ_115200_BAUD);
    /* Configure Interrupts */
    DL_UART_Main_enableInterrupt(
        UART_INST, DL_UART_MAIN_INTERRUPT_RX | DL_UART_MAIN_INTERRUPT_TX);

    DL_UART_Main_enable(UART_INST);
}

/* ADC init */
void startADC(void)
{
    ADC_initPower();
    TIMER_ADC_init();
    ADC_init();
}

void ADC_initPower(void)
{
    DL_TimerG_reset(TIMER_ADC_INST);
    DL_ADC12_reset(ADC_INST);

    DL_TimerG_enablePower(TIMER_ADC_INST);
    DL_ADC12_enablePower(ADC_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

/* OPA init */
static const DL_OPA_Config gOPA_0Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

void SYSCFG_DL_OPA_0_init(void)
{
    DL_OPA_init(OPA_0_INST, (DL_OPA_Config *) &gOPA_0Config0);
    DL_OPA_enableRailToRailInput(OPA_0_INST);

    DL_OPA_enable(OPA_0_INST);
}

static const DL_OPA_Config gOPA_1Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN1_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

void SYSCFG_DL_OPA_1_init(void)
{
    DL_OPA_init(OPA_1_INST, (DL_OPA_Config *) &gOPA_1Config0);
    DL_OPA_enableRailToRailInput(OPA_1_INST);

    DL_OPA_enable(OPA_1_INST);
}

static const DL_TimerG_ClockConfig gTIMER_ADC_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

static const DL_TimerG_TimerConfig gTIMER_ADC_INSTTimerConfig = {
    .period     = 163,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

/* Init timer to trigger ADC conversions */
void TIMER_ADC_init(void)
{
    DL_TimerG_setClockConfig(
        TIMER_ADC_INST, (DL_TimerG_ClockConfig *) &gTIMER_ADC_INSTClockConfig);

    DL_TimerG_initTimerMode(
        TIMER_ADC_INST, (DL_TimerG_TimerConfig *) &gTIMER_ADC_INSTTimerConfig);

    DL_TimerG_enableClock(TIMER_ADC_INST);

    DL_TimerG_enableEvent(
        TIMER_ADC_INST, DL_TIMER_EVENT_ROUTE_1, DL_TIMER_EVENT_ZERO_EVENT);

    DL_TimerG_setPublisherChanID(
        TIMER_ADC_INST, DL_TIMER_PUBLISHER_INDEX_0, 1);
}

static const DL_ADC12_ClockConfig gADC_INSTClockConfig = {
    .clockSel    = DL_ADC12_CLOCK_SYSOSC, /* 32Mhz after reset */
    .divideRatio = DL_ADC12_CLOCK_DIVIDE_4,
    /* The ADC clock is 32MHz, but the sampling clock is 8MHz (32 / 4 = 8) */
    .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

void ADC_init(void)
{
    DL_ADC12_setClockConfig(
        ADC_INST, (DL_ADC12_ClockConfig *) &gADC_INSTClockConfig);

    DL_ADC12_initSeqSample(ADC_INST, DL_ADC12_REPEAT_MODE_DISABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SEQ_START_ADDR_00, DL_ADC12_SEQ_END_ADDR_01,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);

    DL_ADC12_configConversionMem(ADC_INST, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_0, DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_ADC12_configConversionMem(ADC_INST, DL_ADC12_MEM_IDX_1,
        DL_ADC12_INPUT_CHAN_1, DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    /* 26 / 8MHz (of the sample clock, SYSOSC / 4) = 3.25us sampling period */
    DL_ADC12_setSampleTime0(ADC_INST, 100);
    DL_ADC12_setSampleTime1(ADC_INST, 50);

    /* Disable automatic power down mode to get non-zero ADC result */
    DL_ADC12_setPowerDownMode(ADC_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);

    DL_ADC12_clearInterruptStatus(
        ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED |
                      DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED);
    DL_ADC12_enableInterrupt(
        ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED |
                      DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED);

    /* Set subscriber channel for the timer */
    DL_ADC12_setSubscriberChanID(ADC_INST, 1);

    DL_ADC12_enableConversions(ADC_INST);
}

/* SYSCTL init */
void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    DL_SYSCTL_enableMFCLK();

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setVBOOSTConfig(DL_SYSCTL_VBOOST_ONALWAYS);
}

/* Valve timer init */
/*
 * Timer clock configuration to be sourced by BUSCLK /  (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   125000 Hz = 32000000 Hz / (1 * (255 + 1))
 */
static const DL_TimerG_ClockConfig gCAPTURE_1ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * CAPTURE_1_INST_LOAD_VALUE = (156 ms * 125000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gCAPTURE_1TimerConfig = {
    .period     = CAPTURE_1_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_START,
};

void SYSCFG_DL_CAPTURE_1_init(void)
{
    DL_TimerG_setClockConfig(
        CAPTURE_1_INST, (DL_TimerG_ClockConfig *) &gCAPTURE_1ClockConfig);

    DL_TimerG_initTimerMode(
        CAPTURE_1_INST, (DL_TimerG_TimerConfig *) &gCAPTURE_1TimerConfig);
    DL_TimerG_enableInterrupt(CAPTURE_1_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);

    DL_TimerG_enableClock(CAPTURE_1_INST);
}
