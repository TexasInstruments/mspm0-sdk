/*
 * Copyright (c) 2022, Texas Instruments Incorporated
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
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_UART_Main_reset(UART_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_UART_Main_enablePower(UART_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{
    /* Configure UART_INST */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_IOMUX_TX, GPIO_UART_IOMUX_TX_FUNCTION);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_IOMUX_RX, GPIO_UART_IOMUX_RX_FUNCTION);

    /* Configure S2 on the LP */
    DL_GPIO_setLowerPinsPolarity(GPIO_SWITCHES_PORT, DL_GPIO_PIN_14_EDGE_FALL);
    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES_USER_SWITCH_1_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_clearInterruptStatus(
        GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(
        GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN);

    /* Configure LED1 to be toggled in IDLE demo mode */
    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_enableOutput(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
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
    DL_UART_Main_enableInterrupt(UART_INST, DL_UART_MAIN_INTERRUPT_RX);

    DL_UART_Main_enable(UART_INST);
}

void blink_led_init(void)
{
    blink_led_initPower();
    blink_led_SYSCTL_init();
    blink_led_GPIO_init();
    blink_led_TIMER_init();
}

void blink_led_initPower(void)
{
    /* Don't reset GPIOA due to UART communication with GUI */

    DL_TimerG_reset(BLINK_LED_TIMER_INST);

    DL_TimerG_enablePower(BLINK_LED_TIMER_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void blink_led_GPIO_init(void)
{
    DL_GPIO_initDigitalOutput(GPIO_IOMUX_RGB_GREEN);

    DL_GPIO_clearPins(GPIO_RGB_PORT, GPIO_RGB_GREEN_PIN);

    DL_GPIO_enableOutput(GPIO_RGB_PORT, GPIO_RGB_GREEN_PIN);

    /* Clear the LED1 pin that is toggled during IDLE mode */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

void blink_led_SYSCTL_init(void)
{
    /* Enable MFCLK for demo 1, blinking an LED at a particular frequency */
    DL_SYSCTL_enableMFCLK();
}

/*
 * Timer clock source is MFCLK running at 4MHz. Ideal for timer peripheral
 * which requires a constant clock source in RUN, SLEEP, and STOP power modes.
 * Starts with a default delay of 500ms. This can be changed in GUI Composer.
 * ((4MHz / divideRatio / prescale) * TARGET DELAY) = TIM_TIMEOUT
 * ((4MHz / 255) * 0.5s) = 7843 - 1 = 7842 due to N+1 ticks in down mode
 */
static const DL_TimerG_ClockConfig gBLINK_LED_TIMER_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_MFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U};

static const DL_TimerG_TimerConfig gBLINK_LED_TIMER_INSTTimerConfig = {
    .period     = 7842U,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP};

void blink_led_TIMER_init(void)
{
    DL_TimerG_setClockConfig(BLINK_LED_TIMER_INST,
        (DL_TimerG_ClockConfig *) &gBLINK_LED_TIMER_INSTClockConfig);

    DL_TimerG_initTimerMode(BLINK_LED_TIMER_INST,
        (DL_TimerG_TimerConfig *) &gBLINK_LED_TIMER_INSTTimerConfig);

    DL_TimerG_clearInterruptStatus(
        BLINK_LED_TIMER_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableInterrupt(
        BLINK_LED_TIMER_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

    DL_TimerG_enableClock(BLINK_LED_TIMER_INST);
}

void lightsensor_init(void)
{
    lightsensor_initPower();
    lightsensor_SYSCTL_init();
    lightsensor_GPIO_init();
    lightsensor_PWM_RED_BLUE_init();
    lightsensor_PWN_GREEN_init();
    lightsensor_OPA_init();
    lightsensor_TIMER_ADC_init();
    lightsensor_ADC_init();
}

void lightsensor_initPower(void)
{
    /* Don't reset GPIOA due to UART communication */

    DL_TimerG_reset(LIGHTSENSOR_PWM_RED_BLUE_INST);
    DL_TimerG_reset(LIGHTSENSOR_PWM_GREEN_INST);
    DL_OPA_reset(LIGHTSENSOR_OPA_INST);
    DL_TimerG_reset(LIGHTSENSOR_TIMER_ADC_INST);
    DL_ADC12_reset(ADC_INST);

    DL_TimerG_enablePower(LIGHTSENSOR_PWM_RED_BLUE_INST);
    DL_TimerG_enablePower(LIGHTSENSOR_PWM_GREEN_INST);
    DL_OPA_enablePower(LIGHTSENSOR_OPA_INST);
    DL_TimerG_enablePower(LIGHTSENSOR_TIMER_ADC_INST);
    DL_ADC12_enablePower(ADC_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void lightsensor_SYSCTL_init(void)
{
}

void lightsensor_GPIO_init(void)
{
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_IOMUX_RGB_RED, GPIO_IOMUX_FUNC_RED_PWM);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_IOMUX_RGB_GREEN, GPIO_IOMUX_FUNC_GREEN_PWM);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_IOMUX_RGB_BLUE, GPIO_IOMUX_FUNC_BLUE_PWM);

    /* Clear the LED1 pin that is toggled during IDLE mode */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

static const DL_OPA_Config gLIGHTSENSOR_OPA_INSTConfig0 = {
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1};

void lightsensor_OPA_init(void)
{
    DL_OPA_disableRailToRailInput(LIGHTSENSOR_OPA_INST);
    DL_OPA_setGainBandwidth(LIGHTSENSOR_OPA_INST, DL_OPA_GBW_HIGH);
    DL_OPA_init(
        LIGHTSENSOR_OPA_INST, (DL_OPA_Config *) &gLIGHTSENSOR_OPA_INSTConfig0);
    DL_OPA_enable(LIGHTSENSOR_OPA_INST);
}

static const DL_TimerG_ClockConfig gLIGHTSENSOR_TIMER_ADC_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

static const DL_TimerG_TimerConfig gLIGHTSENSOR_TIMER_ADC_INSTTimerConfig = {
    .period     = 4095U,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

/* Init timer to trigger ADC conversions */
void lightsensor_TIMER_ADC_init(void)
{
    DL_TimerG_setClockConfig(LIGHTSENSOR_TIMER_ADC_INST,
        (DL_TimerG_ClockConfig *) &gLIGHTSENSOR_TIMER_ADC_INSTClockConfig);

    DL_TimerG_initTimerMode(LIGHTSENSOR_TIMER_ADC_INST,
        (DL_TimerG_TimerConfig *) &gLIGHTSENSOR_TIMER_ADC_INSTTimerConfig);

    DL_TimerG_enableClock(LIGHTSENSOR_TIMER_ADC_INST);

    DL_TimerG_enableEvent(LIGHTSENSOR_TIMER_ADC_INST, DL_TIMER_EVENT_ROUTE_1,
        DL_TIMER_EVENT_ZERO_EVENT);

    DL_TimerG_setPublisherChanID(
        LIGHTSENSOR_TIMER_ADC_INST, DL_TIMER_PUBLISHER_INDEX_0, 2);
}

static const DL_ADC12_ClockConfig gADC_INSTClockConfig = {
    .clockSel    = DL_ADC12_CLOCK_SYSOSC, /* 32Mhz after reset */
    .divideRatio = DL_ADC12_CLOCK_DIVIDE_4,
    /* The ADC clock is 32MHz, but the sampling clock is 8MHz (32 / 4 = 8) */
    .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

void lightsensor_ADC_init(void)
{
    DL_ADC12_setClockConfig(
        ADC_INST, (DL_ADC12_ClockConfig *) &gADC_INSTClockConfig);

    /* Repeatedly sample a particular channel */
    DL_ADC12_initSingleSample(ADC_INST, DL_ADC12_REPEAT_MODE_ENABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);

    DL_ADC12_configConversionMem(ADC_INST, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_4, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    /* 26 / 8MHz (of the sample clock, SYSOSC / 4) = 3.25us sampling period */
    DL_ADC12_setSampleTime0(ADC_INST, 26);

    /* Disable automatic power down mode to get non-zero ADC result */
    DL_ADC12_setPowerDownMode(ADC_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);

    DL_ADC12_clearInterruptStatus(
        ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    DL_ADC12_enableInterrupt(ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);

    /* Set subscriber channel for the timer */
    DL_ADC12_setSubscriberChanID(ADC_INST, 2);

    DL_ADC12_enableConversions(ADC_INST);
}

static const DL_TimerG_ClockConfig gLIGHTSENSOR_PWM_RED_BLUE_INSTClockConfig =
    {
        .clockSel    = DL_TIMER_CLOCK_LFCLK,
        .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
        .prescale    = 0,
};

static const DL_TimerG_PWMConfig gLIGHTSENSOR_PWM_RED_BLUE_INSTConfig = {
    .period     = RGB_LED_PERIOD, /* 64Hz PWM signal for visible light */
    .pwmMode    = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .startTimer = false,
};

void lightsensor_PWM_RED_BLUE_init(void)
{
    DL_TimerG_setClockConfig(LIGHTSENSOR_PWM_RED_BLUE_INST,
        (DL_TimerG_ClockConfig *) &gLIGHTSENSOR_PWM_RED_BLUE_INSTClockConfig);

    DL_TimerG_initPWMMode(LIGHTSENSOR_PWM_RED_BLUE_INST,
        (DL_TimerG_PWMConfig *) &gLIGHTSENSOR_PWM_RED_BLUE_INSTConfig);

    /* Configure 50% duty cycle */
    DL_TimerG_setCaptureCompareValue(LIGHTSENSOR_PWM_RED_BLUE_INST,
        RGB_LED_PERIOD / 2, DL_TIMER_CC_0_INDEX);

    /* Configure 50% duty cycle */
    DL_TimerG_setCaptureCompareValue(LIGHTSENSOR_PWM_RED_BLUE_INST,
        RGB_LED_PERIOD / 2, DL_TIMER_CC_1_INDEX);

    DL_TimerG_clearInterruptStatus(
        LIGHTSENSOR_PWM_RED_BLUE_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableInterrupt(
        LIGHTSENSOR_PWM_RED_BLUE_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

    DL_TimerG_enableClock(LIGHTSENSOR_PWM_RED_BLUE_INST);

    DL_TimerG_setCCPDirection(LIGHTSENSOR_PWM_RED_BLUE_INST,
        (DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT));
}

static const DL_TimerG_ClockConfig gLIGHTSENSOR_PWM_GREEN_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0,
};

static const DL_TimerG_PWMConfig gLIGHTSENSOR_PWM_GREEN_INSTConfig = {
    .period     = RGB_LED_PERIOD, /* 64Hz PWM signal for visible light */
    .pwmMode    = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .startTimer = false,
};

void lightsensor_PWN_GREEN_init(void)
{
    DL_TimerG_setClockConfig(LIGHTSENSOR_PWM_GREEN_INST,
        (DL_TimerG_ClockConfig *) &gLIGHTSENSOR_PWM_GREEN_INSTClockConfig);

    DL_TimerG_initPWMMode(LIGHTSENSOR_PWM_GREEN_INST,
        (DL_TimerG_PWMConfig *) &gLIGHTSENSOR_PWM_GREEN_INSTConfig);

    /* Configure 50% duty cycle */
    DL_TimerG_setCaptureCompareValue(
        LIGHTSENSOR_PWM_GREEN_INST, RGB_LED_PERIOD / 2, DL_TIMER_CC_1_INDEX);

    DL_TimerG_clearInterruptStatus(
        LIGHTSENSOR_PWM_GREEN_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableInterrupt(
        LIGHTSENSOR_PWM_GREEN_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

    DL_TimerG_enableClock(LIGHTSENSOR_PWM_GREEN_INST);

    DL_TimerG_setCCPDirection(LIGHTSENSOR_PWM_GREEN_INST, DL_TIMER_CC1_OUTPUT);
}

void thermistor_init(void)
{
    thermistor_initPower();
    thermistor_SYSCTL_init();
    thermistor_GPIO_init();
    thermistor_OPA_init();
    thermistor_TIMER_ADC_init();
    thermistor_ADC_init();
}

void thermistor_initPower(void)
{
    /* Don't reset GPIOA due to UART communication */

    DL_OPA_reset(THERMISTOR_OPA_INST);
    DL_TimerG_reset(THERMISTOR_TIMER_ADC_INST);
    DL_ADC12_reset(ADC_INST);

    DL_OPA_enablePower(THERMISTOR_OPA_INST);
    DL_TimerG_enablePower(THERMISTOR_TIMER_ADC_INST);
    DL_ADC12_enablePower(ADC_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void thermistor_SYSCTL_init(void)
{
}

void thermistor_GPIO_init(void)
{
    /* Clear the LED1 pin that is toggled during IDLE mode */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

static const DL_OPA_Config gTHERMISTOR_OPA_INSTConfig0 = {
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1};

void thermistor_OPA_init(void)
{
    DL_OPA_disableRailToRailInput(THERMISTOR_OPA_INST);
    DL_OPA_setGainBandwidth(THERMISTOR_OPA_INST, DL_OPA_GBW_HIGH);
    DL_OPA_init(
        THERMISTOR_OPA_INST, (DL_OPA_Config *) &gTHERMISTOR_OPA_INSTConfig0);
    DL_OPA_enable(THERMISTOR_OPA_INST);
}

static const DL_TimerG_ClockConfig gTHERMISTOR_TIMER_ADC_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

static const DL_TimerG_TimerConfig gTHERMISTOR_TIMER_ADC_INSTTimerConfig = {
    .period     = 4095U,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

/* Init timer to trigger ADC conversions */
void thermistor_TIMER_ADC_init(void)
{
    DL_TimerG_setClockConfig(THERMISTOR_TIMER_ADC_INST,
        (DL_TimerG_ClockConfig *) &gTHERMISTOR_TIMER_ADC_INSTClockConfig);

    DL_TimerG_initTimerMode(THERMISTOR_TIMER_ADC_INST,
        (DL_TimerG_TimerConfig *) &gTHERMISTOR_TIMER_ADC_INSTTimerConfig);

    DL_TimerG_enableClock(THERMISTOR_TIMER_ADC_INST);

    DL_TimerG_enableEvent(THERMISTOR_TIMER_ADC_INST, DL_TIMER_EVENT_ROUTE_1,
        DL_TIMER_EVENT_ZERO_EVENT);

    DL_TimerG_setPublisherChanID(
        THERMISTOR_TIMER_ADC_INST, DL_TIMER_PUBLISHER_INDEX_0, 1);
}

void thermistor_ADC_init(void)
{
    DL_ADC12_setClockConfig(
        ADC_INST, (DL_ADC12_ClockConfig *) &gADC_INSTClockConfig);

    /* Repeatedly sample a particular channel */
    DL_ADC12_initSingleSample(ADC_INST, DL_ADC12_REPEAT_MODE_ENABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);

    DL_ADC12_configConversionMem(ADC_INST, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_9, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    /* 26 / 8MHz (of the sample clock, SYSOSC / 4) = 3.25us sampling period */
    DL_ADC12_setSampleTime0(ADC_INST, 26);

    /* Disable automatic power down mode to get non-zero ADC result */
    DL_ADC12_setPowerDownMode(ADC_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);

    DL_ADC12_clearInterruptStatus(
        ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    DL_ADC12_enableInterrupt(ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);

    /* Set subscriber channel for the timer */
    DL_ADC12_setSubscriberChanID(ADC_INST, 1);

    DL_ADC12_enableConversions(ADC_INST);
}
