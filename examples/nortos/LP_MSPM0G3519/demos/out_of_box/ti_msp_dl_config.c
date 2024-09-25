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

   /* Initialize S2 on LP */
   DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES_USER_SWITCH_2_IOMUX,
       DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
       DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
   DL_GPIO_setLowerPinsPolarity(GPIO_SWITCHES_PORT, DL_GPIO_PIN_3_EDGE_FALL);
   DL_GPIO_clearInterruptStatus(
       GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_2_PIN);
   DL_GPIO_enableInterrupt(
       GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_2_PIN);

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

}

void temperature_initPower(void)
{
   /* Don't reset GPIOA due to UART communication */
   DL_TimerG_reset(TEMPERATURE_TIMER_ADC_INST);
   DL_ADC12_reset(ADC12_0_INST);


   DL_TimerG_enablePower(TEMPERATURE_TIMER_ADC_INST);
   DL_ADC12_enablePower(ADC12_0_INST);

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
       DL_ADC12_INPUT_CHAN_11, DL_ADC12_REFERENCE_VOLTAGE_INTREF,
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

void functiongenerator_init(void)
{
    functiongenerator_initPower();
    functiongenerator_SYSCTL_init();

    functiongenerator_DMA_init();
    functiongenerator_VREF_init();
    functiongenerator_DAC_init();
    functiongenerator_TIMER_ADC_init();
    functiongenerator_ADC_init();
}

void functiongenerator_initPower(void)
{

    DL_VREF_reset(VREF);
    DL_DAC12_reset(FNGEN_DAC_INST);
    DL_TimerG_reset(FNGEN_TIMER_ADC_INST);
    DL_ADC12_reset(FNGEN_ADC_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_VREF_enablePower(VREF);
    DL_DAC12_enablePower(FNGEN_DAC_INST);
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
}



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
        DL_ADC12_INPUT_CHAN_0, DL_ADC12_REFERENCE_VOLTAGE_VDDA,
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
}



void functiongenerator_setPeriod(int frequency)
{
    if (frequency != 0) {
        /* 0.94 scaling achieved correct frequency  +/- 0.5% in debugging */
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


