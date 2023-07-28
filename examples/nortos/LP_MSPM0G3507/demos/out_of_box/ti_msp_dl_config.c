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
    DL_GPIO_reset(GPIOB);
    DL_UART_Main_reset(UART_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
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

    /* Initialize S3 on LP */
    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES1_USER_SWITCH_1_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_setUpperPinsPolarity(
        GPIO_SWITCHES1_PORT, DL_GPIO_PIN_21_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(
        GPIO_SWITCHES1_PORT, GPIO_SWITCHES1_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(
        GPIO_SWITCHES1_PORT, GPIO_SWITCHES1_USER_SWITCH_1_PIN);

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
    /* UART clock configuration to use BUSCLK at 32MHz */
    DL_UART_Main_setClockConfig(
        UART_INST, (DL_UART_Main_ClockConfig *) &gUARTClockConfig);

    /* UART configuration to operate in normal mode */
    DL_UART_Main_init(UART_INST, (DL_UART_Main_Config *) &gUARTConfig);

    DL_UART_Main_configBaudRate(UART_INST, 32000000, UART_BAUD);

    /* Configure RX interrupt to trigger when there's 1 entry in the RX FIFO */
    DL_UART_Main_setRXFIFOThreshold(
        UART_INST, DL_UART_MAIN_RX_FIFO_LEVEL_ONE_ENTRY);
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
    /* Don't reset GPIOA/B to maintain UART and switch functionality */

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
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

void blink_led_SYSCTL_init(void)
{
    /* Enable MFCLK for demo 1, blinking an LED at a particular frequency */
    DL_SYSCTL_enableMFCLK();
}

/*
 * Timer clock source is MFCLK running at 4MHz. Ideal for timer peripheral which
 * requires a constant clock source in RUN, SLEEP, and STOP power modes.
 * Start with a default delay of 500ms. This can be changed in GUI Composer.
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
    lightsensor_PWM_RED_GREEN_init();
    lightsensor_PWN_BLUE_init();
    lightsensor_OPA_init();
    lightsensor_TIMER_ADC_init();
    lightsensor_ADC_init();
}

void lightsensor_initPower(void)
{
    /* Don't reset GPIOA/B for UART and switch functionality */

    DL_TimerA_reset(LIGHTSENSOR_PWM_RED_GREEN_INST);
    DL_TimerG_reset(LIGHTSENSOR_PWM_BLUE_INST);
    DL_OPA_reset(LIGHTSENSOR_OPA_INST);
    DL_TimerG_reset(LIGHTSENSOR_TIMER_ADC_INST);
    DL_ADC12_reset(ADC_INST);

    DL_TimerA_enablePower(LIGHTSENSOR_PWM_RED_GREEN_INST);
    DL_TimerG_enablePower(LIGHTSENSOR_PWM_BLUE_INST);
    DL_OPA_enablePower(LIGHTSENSOR_OPA_INST);
    DL_TimerG_enablePower(LIGHTSENSOR_TIMER_ADC_INST);
    DL_ADC12_enablePower(ADC_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void lightsensor_SYSCTL_init(void)
{
    /*
    //__PG1_0_WORKAROUND__ OPA_4
    DL_SYSCTL_setVBOOSTConfig(DL_SYSCTL_VBOOST_ONALWAYS);
    //__PG1_0_WORKAROUND_END__ OPA_4

    //__PG1_0_WORKAROUND__ OPA_5
    DL_SYSCTL_enableMFCLK();
    //__PG1_0_WORKAROUND_END__ OPA_5
*/
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
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

/*static const DL_OPA_Config gLIGHTSENSOR_OPA_INSTConfig0 = {
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1};*/

static const DL_OPA_Config gLIGHTSENSOR_OPA_INSTConfig0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

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
        LIGHTSENSOR_TIMER_ADC_INST, DL_TIMER_PUBLISHER_INDEX_0, 1);
}

static const DL_ADC12_ClockConfig gADC_INSTClockConfig = {
    .clockSel    = DL_ADC12_CLOCK_SYSOSC, /* 32MHz after reset */
    .divideRatio = DL_ADC12_CLOCK_DIVIDE_4,
    /* The ADC clock is 32MHz, but the sampling clock is 8MHz (32 / 4 = 8) */
    .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

void lightsensor_ADC_init(void)
{
    DL_ADC12_setClockConfig(
        ADC_INST, (DL_ADC12_ClockConfig *) &gADC_INSTClockConfig);

    /* Repeatedly sample a particular channel. HW event serves as trigger */
    DL_ADC12_initSingleSample(ADC_INST, DL_ADC12_REPEAT_MODE_ENABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);

    DL_ADC12_configConversionMem(ADC_INST, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_7, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
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

static const DL_TimerA_ClockConfig gLIGHTSENSOR_PWM_RED_GREEN_INSTClockConfig =
    {
        .clockSel    = DL_TIMER_CLOCK_LFCLK,
        .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
        .prescale    = 0,
};

static const DL_TimerA_PWMConfig gLIGHTSENSOR_PWM_RED_GREEN_INSTConfig = {
    .period  = RGB_LED_PERIOD, /* 64Hz PWM signal for visible light */
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .isTimerWithFourCC = false,
    .startTimer        = false,
};

void lightsensor_PWM_RED_GREEN_init(void)
{
    DL_TimerA_setClockConfig(LIGHTSENSOR_PWM_RED_GREEN_INST,
        (DL_TimerA_ClockConfig *) &gLIGHTSENSOR_PWM_RED_GREEN_INSTClockConfig);

    DL_TimerA_initPWMMode(LIGHTSENSOR_PWM_RED_GREEN_INST,
        (DL_TimerA_PWMConfig *) &gLIGHTSENSOR_PWM_RED_GREEN_INSTConfig);

    /* Configure 50% duty cycle */
    DL_TimerA_setCaptureCompareValue(LIGHTSENSOR_PWM_RED_GREEN_INST,
        RGB_LED_PERIOD / 2, DL_TIMER_CC_0_INDEX);

    /* Configure 50% duty cycle */
    DL_TimerA_setCaptureCompareValue(LIGHTSENSOR_PWM_RED_GREEN_INST,
        RGB_LED_PERIOD / 2, DL_TIMER_CC_1_INDEX);

    DL_TimerA_clearInterruptStatus(
        LIGHTSENSOR_PWM_RED_GREEN_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
    DL_TimerA_enableInterrupt(
        LIGHTSENSOR_PWM_RED_GREEN_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

    DL_TimerA_enableClock(LIGHTSENSOR_PWM_RED_GREEN_INST);

    DL_TimerA_setCCPDirection(LIGHTSENSOR_PWM_RED_GREEN_INST,
        (DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT));
}

static const DL_TimerG_ClockConfig gLIGHTSENSOR_PWM_BLUE_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0,
};

static const DL_TimerG_PWMConfig gLIGHTSENSOR_PWM_BLUE_INSTConfig = {
    .period     = RGB_LED_PERIOD, /* 64Hz PWM signal for visible light */
    .pwmMode    = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .startTimer = false,
};

void lightsensor_PWN_BLUE_init(void)
{
    DL_TimerG_setClockConfig(LIGHTSENSOR_PWM_BLUE_INST,
        (DL_TimerG_ClockConfig *) &gLIGHTSENSOR_PWM_BLUE_INSTClockConfig);

    DL_TimerG_initPWMMode(LIGHTSENSOR_PWM_BLUE_INST,
        (DL_TimerG_PWMConfig *) &gLIGHTSENSOR_PWM_BLUE_INSTConfig);

    /* Configure 50% duty cycle */
    DL_TimerG_setCaptureCompareValue(
        LIGHTSENSOR_PWM_BLUE_INST, RGB_LED_PERIOD / 2, DL_TIMER_CC_1_INDEX);

    DL_TimerG_clearInterruptStatus(
        LIGHTSENSOR_PWM_BLUE_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableInterrupt(
        LIGHTSENSOR_PWM_BLUE_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);

    DL_TimerG_enableClock(LIGHTSENSOR_PWM_BLUE_INST);

    DL_TimerG_setCCPDirection(LIGHTSENSOR_PWM_BLUE_INST, DL_TIMER_CC1_OUTPUT);
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
    //__PG1_0_WORKAROUND__ OPA_4
    DL_SYSCTL_setVBOOSTConfig(DL_SYSCTL_VBOOST_ONALWAYS);
    //__PG1_0_WORKAROUND_END__ OPA_4

    //__PG1_0_WORKAROUND__ OPA_5
    DL_SYSCTL_enableMFCLK();
    //__PG1_0_WORKAROUND_END__ OPA_5
}

void thermistor_GPIO_init(void)
{
    /* Clear the LED1 pin that is toggled during IDLE mode */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
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
        DL_ADC12_INPUT_CHAN_5, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
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

//start function generator additions
void functiongenerator_init(DL_OPA_GAIN gain, int nonInverting)
{
    functiongenerator_initPower();
    functiongenerator_SYSCTL_init();
    functiongenerator_OPA_initNINV(gain, 1);
    functiongenerator_DMA_init();
    functiongenerator_VREF_init();
    functiongenerator_DAC_init();
    functiongenerator_TIMER_ADC_init();
    functiongenerator_ADC_init();
}

void functiongenerator_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_VREF_reset(VREF);
    DL_DAC12_reset(FNGEN_DAC_INST);
    DL_OPA_reset(FNGEN_OPA_INST);
    DL_TimerG_reset(FNGEN_TIMER_ADC_INST);
    DL_ADC12_reset(FNGEN_ADC_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_VREF_enablePower(VREF);
    DL_DAC12_enablePower(FNGEN_DAC_INST);
    DL_OPA_enablePower(FNGEN_OPA_INST);
    DL_TimerG_enablePower(FNGEN_TIMER_ADC_INST);
    DL_ADC12_enablePower(FNGEN_ADC_INST);

    delay_cycles(POWER_STARTUP_DELAY);
}

void functiongenerator_SYSCTL_init(void)
{
    DL_SYSCTL_enableMFPCLK();
    DL_SYSCTL_setVBOOSTConfig(DL_SYSCTL_VBOOST_ONALWAYS);
    DL_SYSCTL_enableMFCLK();
}

/*
static const DL_DMA_Config gDMACH0Config = {
    .transferMode  = DL_DMA_REPEAT_SINGLE_TRANSFER_MODE,
    .destIncrement = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement  = DL_DMA_ADDR_INCREMENT,
    .destWidth     = DL_DMA_WIDTH_HALF_WORD,
    .srcWidth      = DL_DMA_WIDTH_HALF_WORD,
    .trigger       = DMA_DAC0_EVT_BD_1_TRIG,
    .extendedMode  = DL_DMA_NORMAL_MODE,
};
*/

static const DL_DMA_Config gDMACH0Config = {
    .transferMode  = DL_DMA_FULL_CH_REPEAT_SINGLE_TRANSFER_MODE,
    .extendedMode  = DL_DMA_NORMAL_MODE,
    .destIncrement = DL_DMA_ADDR_UNCHANGED,
    .srcIncrement  = DL_DMA_ADDR_INCREMENT,
    .destWidth     = DL_DMA_WIDTH_HALF_WORD,
    .srcWidth      = DL_DMA_WIDTH_HALF_WORD,
    .trigger       = DMA_DAC0_EVT_BD_1_TRIG,
    .triggerType   = DL_DMA_TRIGGER_TYPE_EXTERNAL,
};

void functiongenerator_DMA_init(void)
{
    /* Initialize DMA channel 0 */
    DL_DMA_disableChannel(DMA, FNGEN_DMA_CH0_SEL);
    DL_DMA_initChannel(
        DMA, FNGEN_DMA_CH0_SEL, (DL_DMA_Config *) &gDMACH0Config);
    //DL_DMA_setSubscriberChanID(DMA, DL_DMA_SUBSCRIBER_INDEX_0,
    //    12);  // channel 12 used so ADC can also subscribe
}

static const DL_VREF_ClockConfig gVREFClockConfig = {
    .clockSel    = DL_VREF_CLOCK_BUSCLK,
    .divideRatio = DL_VREF_CLOCK_DIVIDE_1,
};

static const DL_VREF_Config gVREFConfig = {
    .bufConfig      = DL_VREF_BUFCONFIG_OUTPUT_2_5V,
    .holdCycleCount = DL_VREF_HOLD_MIN,
    .shCycleCount   = DL_VREF_SH_MIN,
    .shModeEnable   = DL_VREF_SHMODE_DISABLE,
    .vrefEnable     = DL_VREF_ENABLE_ENABLE,
};

void functiongenerator_VREF_init(void)
{
    DL_VREF_setClockConfig(VREF, (DL_VREF_ClockConfig *) &gVREFClockConfig);
    DL_VREF_configReference(VREF, (DL_VREF_Config *) &gVREFConfig);
}

DL_TimerG_ClockConfig gFUNCTION_GENERATOR_TIMER_ADC_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_MFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 1U,
};

DL_TimerG_TimerConfig gFUNCTION_GENERATOR_TIMER_ADC_INSTTimerConfig = {
    .period     = 4095,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

void functiongenerator_TIMER_ADC_init(void)
{
    DL_TimerG_setClockConfig(FNGEN_TIMER_ADC_INST,
        (DL_TimerG_ClockConfig
                *) &gFUNCTION_GENERATOR_TIMER_ADC_INSTClockConfig);

    DL_TimerG_initTimerMode(FNGEN_TIMER_ADC_INST,
        (DL_TimerG_TimerConfig
                *) &gFUNCTION_GENERATOR_TIMER_ADC_INSTTimerConfig);

    DL_TimerG_enableClock(FNGEN_TIMER_ADC_INST);

    DL_TimerG_enableEvent(FNGEN_TIMER_ADC_INST, DL_TIMER_EVENT_ROUTE_1,
        DL_TIMER_EVENT_ZERO_EVENT);

    DL_TimerG_setPublisherChanID(FNGEN_TIMER_ADC_INST,
        DL_TIMER_PUBLISHER_INDEX_0,
        12);  // publishes to start both ADC and DAC conversions

    DL_TimerG_startCounter(FNGEN_TIMER_ADC_INST);
}
static const DL_ADC12_ClockConfig gFUNCTION_GENERATOR_ADC_INSTClockConfig = {
    .clockSel    = DL_ADC12_CLOCK_SYSOSC, /* 32MHz after reset */
    .divideRatio = DL_ADC12_CLOCK_DIVIDE_32,
    /* The ADC clock is 32MHz, but the sampling clock is 8MHz (32 / 4 = 8) */
    .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

void functiongenerator_ADC_init(void)
{
    DL_ADC12_setClockConfig(FNGEN_ADC_INST,
        (DL_ADC12_ClockConfig *) &gFUNCTION_GENERATOR_ADC_INSTClockConfig);

    /* Repeatedly sample a particular channel. HW event serves as trigger */
    DL_ADC12_initSingleSample(FNGEN_ADC_INST, DL_ADC12_REPEAT_MODE_ENABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);

    DL_ADC12_configConversionMem(FNGEN_ADC_INST, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_13, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_TRIGGER_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_ADC12_setSampleTime0(FNGEN_ADC_INST, 26);

    /* Disable automatic power down mode to get non-zero ADC result */
    DL_ADC12_setPowerDownMode(FNGEN_ADC_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);

    /* Set subscriber channel for the timer */
    DL_ADC12_setSubscriberChanID(FNGEN_ADC_INST, 12);

    DL_ADC12_clearInterruptStatus(
        FNGEN_ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    DL_ADC12_enableInterrupt(
        FNGEN_ADC_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);

    DL_ADC12_enableConversions(FNGEN_ADC_INST);
}

static const DL_DAC12_Config gDACConfig = {
    .outputEnable              = DL_DAC12_OUTPUT_ENABLED,
    .resolution                = DL_DAC12_RESOLUTION_12BIT,
    .representation            = DL_DAC12_REPRESENTATION_BINARY,
    .voltageReferenceSource    = DL_DAC12_VREF_SOURCE_VDDA_VSSA,
    .fifoEnable                = DL_DAC12_FIFO_ENABLED,
    .fifoTriggerSource         = DL_DAC12_FIFO_TRIGGER_SAMPLETIMER,
    .dmaTriggerEnable          = DL_DAC12_DMA_TRIGGER_DISABLED,
    .dmaTriggerThreshold       = DL_DAC12_FIFO_THRESHOLD_TWO_QTRS_EMPTY,
    .sampleTimeGeneratorEnable = DL_DAC12_SAMPLETIMER_ENABLE,
    .sampleRate                = DL_DAC12_SAMPLES_PER_SECOND_500,
    //.sampleRate                = DL_DAC12_SAMPLES_PER_SECOND_100K,
    .amplifierSetting = DL_DAC12_AMP_ON,
};

void functiongenerator_DAC_init(void)
{
    DL_DAC12_init(FNGEN_DAC_INST, (DL_DAC12_Config *) &gDACConfig);
    DL_DAC12_enableInterrupt(FNGEN_DAC_INST, DL_DAC12_INTERRUPT_DMA_DONE);
    //DL_DAC12_setSubscriberChanID(
    //    FNGEN_DAC_INST, 0, 12);  // was ch.1 before tweaks
}

volatile DL_OPA_Config gOPA_INSTConfig0NINV = {
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .pselChannel    = DL_OPA_PSEL_DAC_OUT,
    .nselChannel    = DL_OPA_NSEL_RTAP,
    .mselChannel    = DL_OPA_MSEL_GND,
    .gain           = DL_OPA_GAIN_N31_P32};

void functiongenerator_OPA_initNINV(DL_OPA_GAIN gain, int nonInverting)
{
    DL_OPA_disable(FNGEN_OPA_INST);
    DL_VREF_disablePower(VREF);
    DL_OPA_disablePower(FNGEN_OPA_INST);
    delay_cycles(10 * POWER_STARTUP_DELAY);
    DL_VREF_reset(VREF);
    DL_OPA_reset(FNGEN_OPA_INST);
    DL_VREF_enablePower(VREF);
    DL_OPA_enablePower(FNGEN_OPA_INST);
    delay_cycles(POWER_STARTUP_DELAY);

    DL_SYSCTL_setVBOOSTConfig(DL_SYSCTL_VBOOST_ONALWAYS);
    DL_SYSCTL_enableMFCLK();

    functiongenerator_VREF_init();
    gOPA_INSTConfig0NINV.gain = gain;

    if (gain == DL_OPA_GAIN_N0_P1) {
        gOPA_INSTConfig0NINV.nselChannel = DL_OPA_NSEL_RTOP;
        gOPA_INSTConfig0NINV.mselChannel = DL_OPA_MSEL_OPEN;
    } else {
        gOPA_INSTConfig0NINV.nselChannel = DL_OPA_NSEL_RTAP;
        gOPA_INSTConfig0NINV.mselChannel = DL_OPA_MSEL_GND;
    }

    /* change MSEL to make the amplifier inverting */
    if (nonInverting == 0) {
        gOPA_INSTConfig0NINV.nselChannel = DL_OPA_NSEL_RTAP,
        gOPA_INSTConfig0NINV.mselChannel = DL_OPA_MSEL_IN1_NEG;
    }

    DL_OPA_enablePower(FNGEN_OPA_INST);
    delay_cycles(POWER_STARTUP_DELAY);

    DL_OPA_disableRailToRailInput(FNGEN_OPA_INST);
    DL_OPA_setGainBandwidth(FNGEN_OPA_INST, DL_OPA_GBW_HIGH);
    DL_OPA_init(FNGEN_OPA_INST, (DL_OPA_Config *) &gOPA_INSTConfig0NINV);

    DL_OPA_enable(FNGEN_OPA_INST);
}

void functiongenerator_setPeriod(int frequency)
{
    if (frequency != 0) {
        // 0.94 scaling achieved correct frequency  +/- 0.5% in debugging
        gFUNCTION_GENERATOR_TIMER_ADC_INSTTimerConfig.period =
            0.94 * (4095 / (frequency));

        DL_TimerG_disableClock(FNGEN_TIMER_ADC_INST);

        DL_TimerG_initTimerMode(FNGEN_TIMER_ADC_INST,
            (DL_TimerG_TimerConfig
                    *) &gFUNCTION_GENERATOR_TIMER_ADC_INSTTimerConfig);

        DL_TimerG_enableClock(FNGEN_TIMER_ADC_INST);
    } else {
        DL_TimerG_stopCounter(FNGEN_TIMER_ADC_INST);
    }
}
