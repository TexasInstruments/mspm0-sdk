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

/* Configuration for GUI connection */
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
    DL_GPIO_reset(GPIOC);
    DL_UART_Main_reset(UART_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_GPIO_enablePower(GPIOC);
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

    /* Initialize S1 on LP */
    DL_GPIO_initDigitalInput(GPIO_SWITCHES1_USER_SWITCH_1_IOMUX);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_18_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_SWITCHES1_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(GPIOA, GPIO_SWITCHES1_USER_SWITCH_1_PIN);

    /* Initialize S3 on LP */
    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES_USER_SWITCH_3_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_setLowerPinsPolarity(GPIO_SWITCHES_PORT, DL_GPIO_PIN_8_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(
        GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_3_PIN);
    DL_GPIO_enableInterrupt(
        GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_3_PIN);

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

/*
 * Blink Example Configuration
 *
 */
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

/*
 * Temperature Example Configuration
 *
 */
void temperature_init(void)
{
    temperature_initPower();
    temperature_SYSCTL_init();
    temperature_GPIO_init();
    temperature_TIMER_ADC_init();
    VREF_init();
    temperature_ADC_init();
    LCD_init();
}

void temperature_initPower(void)
{
    /* Don't reset GPIOA due to UART communication */
    DL_TimerG_reset(TEMPERATURE_TIMER_ADC_INST);
    DL_ADC12_reset(ADC12_0_INST);
    DL_LCD_reset(LCD);

    DL_TimerG_enablePower(TEMPERATURE_TIMER_ADC_INST);
    DL_ADC12_enablePower(ADC12_0_INST);
    DL_LCD_enablePower(LCD);
    DL_VREF_enablePower(VREF);

    delay_cycles(POWER_STARTUP_DELAY);
}

void temperature_SYSCTL_init(void)
{
}

void temperature_GPIO_init(void)
{
    /* Clear the LED1 pin that is toggled during IDLE mode */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

static const DL_TimerG_ClockConfig gTEMPERATURE_TIMER_ADC_INST_ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 8U,
};

static const DL_TimerG_TimerConfig gTEMPERATURE_TIMER_ADC_INST_TimerConfig = {
    .period     = 4095U,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

/* Init timer to trigger ADC conversions */
void temperature_TIMER_ADC_init(void)
{
    DL_TimerG_setClockConfig(TEMPERATURE_TIMER_ADC_INST,
        (DL_TimerG_ClockConfig *) &gTEMPERATURE_TIMER_ADC_INST_ClockConfig);

    DL_TimerG_initTimerMode(TEMPERATURE_TIMER_ADC_INST,
        (DL_TimerG_TimerConfig *) &gTEMPERATURE_TIMER_ADC_INST_TimerConfig);

    DL_TimerG_enableClock(TEMPERATURE_TIMER_ADC_INST);

    DL_TimerG_enableEvent(TEMPERATURE_TIMER_ADC_INST, DL_TIMER_EVENT_ROUTE_1,
        DL_TIMER_EVENT_ZERO_EVENT);

    DL_TimerG_setPublisherChanID(
        TEMPERATURE_TIMER_ADC_INST, DL_TIMER_PUBLISHER_INDEX_0, 1);
}

static const DL_VREF_ClockConfig gVREFClockConfig = {
    .clockSel    = DL_VREF_CLOCK_BUSCLK,
    .divideRatio = DL_VREF_CLOCK_DIVIDE_1,
};

static const DL_VREF_Config gVREFConfig = {
    .vrefEnable     = DL_VREF_ENABLE_ENABLE,
    .bufConfig      = DL_VREF_BUFCONFIG_OUTPUT_1_4V,
    .shModeEnable   = DL_VREF_SHMODE_DISABLE,
    .holdCycleCount = DL_VREF_HOLD_MIN,
    .shCycleCount   = DL_VREF_SH_MIN,
};

void VREF_init(void)
{
    DL_VREF_setClockConfig(VREF, (DL_VREF_ClockConfig *) &gVREFClockConfig);
    DL_VREF_configReference(VREF, (DL_VREF_Config *) &gVREFConfig);
}

static const DL_ADC12_ClockConfig gTEMPERATURE_ADC12_0_ClockConfig = {
    .clockSel    = DL_ADC12_CLOCK_ULPCLK,
    .divideRatio = DL_ADC12_CLOCK_DIVIDE_8,
    .freqRange   = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

void temperature_ADC_init(void)
{
    DL_ADC12_setClockConfig(ADC12_0_INST,
        (DL_ADC12_ClockConfig *) &gTEMPERATURE_ADC12_0_ClockConfig);

    DL_ADC12_initSingleSample(ADC12_0_INST, DL_ADC12_REPEAT_MODE_DISABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_EVENT,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);

    DL_ADC12_configConversionMem(ADC12_0_INST, DL_ADC12_MEM_IDX_0,
        DL_ADC12_INPUT_CHAN_29, DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
        DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    DL_ADC12_setPowerDownMode(ADC12_0_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);
    DL_ADC12_setSampleTime0(ADC12_0_INST, 100);
    DL_ADC12_setSubscriberChanID(ADC12_0_INST, 1);

    /* Enable ADC12 interrupt */
    DL_ADC12_clearInterruptStatus(
        ADC12_0_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    DL_ADC12_enableInterrupt(
        ADC12_0_INST, DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED);
    DL_ADC12_enableConversions(ADC12_0_INST);
}

/*
 * Tamper Example Configuration
 *
 */

void tamper_init(void)
{
    tamper_initPower();
    tamper_RTC_A_init();
    tamper_TAMPERIO_init();
    LCD_init();
}

void tamper_initPower(void)
{
    DL_LCD_enablePower(LCD);
    delay_cycles(POWER_STARTUP_DELAY);
}

static const DL_RTC_A_Calendar gRTCCalendarConfig = {
    .seconds    = 0x0,    /* Seconds = 0 */
    .minutes    = 0x0,    /* Minute = 0 */
    .hours      = 0x0,    /* Hour = 0 */
    .dayOfWeek  = 0x1,    /* Day of week = 1 (Monday) */
    .dayOfMonth = 0x1,    /* Day of month = 1*/
    .month      = 0x1,    /* Month = 1 (January) */
    .year       = 0x2024, /* Year = 2024 */
};

void tamper_RTC_A_init(void)
{
    /* Initialize RTC Calendar */
    DL_RTC_A_initCalendar(RTC_A, gRTCCalendarConfig, DL_RTC_A_FORMAT_BCD);

    /* Configure Interrupts */
    DL_RTC_A_enableInterrupt(RTC_A, DL_RTC_A_INTERRUPT_TSEVT);

    /* Configure sources for Time Stamp Event */
    DL_RTC_A_setTimeStampEventSource(
        RTC_A, (DL_RTC_A_TIME_STAMP_EVENT_SOURCE_TIO_0));
    DL_RTC_A_setTimeStampEventCapture(
        RTC_A, DL_RTC_A_TIME_STAMP_EVENT_CAPTURE_LAST);
    /* Configure the Crystal Offset Error Calibration */
    DL_RTC_A_setOffsetCalibrationFrequency(
        RTC_A, DL_RTC_A_OFFSET_CALIBRATION_FREQUENCY_32_KHZ);
}

void tamper_TAMPERIO_init(void)
{
    DL_TamperIO_setMode(TAMPERIO, DL_TAMPERIO_0, DL_TAMPERIO_MODE_TAMPER);
    DL_TamperIO_enableInput(TAMPERIO, DL_TAMPERIO_0);
    DL_TamperIO_setEdgeDetectionPolarity(
        TAMPERIO, DL_TAMPERIO_0, DL_TAMPERIO_EDGE_DETECTION_POLARITY_RISE);
}

static const DL_LCD_Config gLCDConfig = {
    .frequencyDivider = DL_LCD_FREQ_DIVIDE_4,
    .muxRate          = DL_LCD_MUX_RATE_4,
    .lowPowerWaveform = DL_LCD_WAVEFORM_POWERMODE_STANDARD};

/*
 * LCD Configuration
 *
 */

void LCD_init(void)
{
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_24);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_42);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_43);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_44);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_58);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_57);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_56);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_55);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_36);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_37);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_38);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_18);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_19);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_20);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_23);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_39);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_40);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_41);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_54);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_53);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_52);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_51);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_50);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_49);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_48);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_47);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_46);
    DL_LCD_setPinAsLCDFunction(LCD, DL_LCD_SEGMENT_LINE_45);
    DL_LCD_init(LCD, (DL_LCD_Config *) &gLCDConfig);
    DL_LCD_turnSegmentsOn(LCD);
    DL_LCD_setBiasVoltageSource(LCD, DL_LCD_BIAS_VOLTAGE_SOURCE_AVDD);
    DL_LCD_setR33source(LCD, DL_LCD_R33_SOURCE_EXTERNAL);
    DL_LCD_enableInternalBias(LCD);
    DL_LCD_setInternalBiasPowerMode(LCD, DL_LCD_POWER_MODE_LOW);

    DL_LCD_clearAllMemoryRegs(LCD);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_24, DL_LCD_COM_0);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_42, DL_LCD_COM_1);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_43, DL_LCD_COM_2);
    DL_LCD_setPinAsCommon(LCD, DL_LCD_SEGMENT_LINE_44, DL_LCD_COM_3);
    DL_LCD_enable(LCD);
}
