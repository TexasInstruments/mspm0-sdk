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
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_16_EDGE_RISE);
    DL_GPIO_initDigitalInputFeatures(SWITCHES_USER_SWITCH_1_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_clearInterruptStatus(GPIOA, SWITCHES_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(GPIOA, SWITCHES_USER_SWITCH_1_PIN);

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
        UART_INST, UART_IBRD_24_MHZ_115200_BAUD, UART_FBRD_24_MHZ_115200_BAUD);

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
    /* Clear the LED1 pin that is toggled during IDLE mode */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

void blink_led_SYSCTL_init(void)
{
    /* Enable MFCLK for demo 1, blinking an LED at a particular frequency */
    DL_SYSCTL_enableMFCLK();

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
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
    .period     = 7812U,
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
