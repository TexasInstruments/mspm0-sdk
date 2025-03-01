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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L1306
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
    SYSCFG_DL_TIMG_DELAY_BLOCKING_init();
    SYSCFG_DL_TIMG_DELAY_NO_BLOCKING_init();
    SYSCFG_DL_I2C_CONTROLLER_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_TimerG_reset(TIMG_DELAY_BLOCKING_INST);
    DL_TimerG_reset(TIMG_DELAY_NO_BLOCKING_INST);
    DL_I2C_reset(I2C_CONTROLLER_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_TimerG_enablePower(TIMG_DELAY_BLOCKING_INST);
    DL_TimerG_enablePower(TIMG_DELAY_NO_BLOCKING_INST);
    DL_I2C_enablePower(I2C_CONTROLLER_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{
    const uint8_t unusedPinIndexes[] =
    {
        IOMUX_PINCM3, IOMUX_PINCM4, IOMUX_PINCM5, IOMUX_PINCM6,
        IOMUX_PINCM7, IOMUX_PINCM8, IOMUX_PINCM9, IOMUX_PINCM10,
        IOMUX_PINCM11, IOMUX_PINCM12, IOMUX_PINCM13, IOMUX_PINCM16,
        IOMUX_PINCM17, IOMUX_PINCM18, IOMUX_PINCM22, IOMUX_PINCM23,
        IOMUX_PINCM25, IOMUX_PINCM26
    };

    for(int i = 0; i < sizeof(unusedPinIndexes)/sizeof(unusedPinIndexes[0]); i++)
    {
        DL_GPIO_initDigitalOutput(unusedPinIndexes[i]);
    }

    DL_GPIO_clearPins(GPIOA,
        (DL_GPIO_PIN_2 | DL_GPIO_PIN_3 | DL_GPIO_PIN_4 | DL_GPIO_PIN_5 |
        DL_GPIO_PIN_6 | DL_GPIO_PIN_7 | DL_GPIO_PIN_8 | DL_GPIO_PIN_9 |
        DL_GPIO_PIN_10 | DL_GPIO_PIN_11 | DL_GPIO_PIN_12 | DL_GPIO_PIN_15 |
        DL_GPIO_PIN_16 | DL_GPIO_PIN_17 | DL_GPIO_PIN_21 | DL_GPIO_PIN_22 |
        DL_GPIO_PIN_24 | DL_GPIO_PIN_25));
    DL_GPIO_enableOutput(GPIOA,
        (DL_GPIO_PIN_2 | DL_GPIO_PIN_3 | DL_GPIO_PIN_4 | DL_GPIO_PIN_5 |
        DL_GPIO_PIN_6 | DL_GPIO_PIN_7 | DL_GPIO_PIN_8 | DL_GPIO_PIN_9 |
        DL_GPIO_PIN_10 | DL_GPIO_PIN_11 | DL_GPIO_PIN_12 | DL_GPIO_PIN_15 |
        DL_GPIO_PIN_16 | DL_GPIO_PIN_17 | DL_GPIO_PIN_21 | DL_GPIO_PIN_22 |
        DL_GPIO_PIN_24 | DL_GPIO_PIN_25));

    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_CONTROLLER_IOMUX_SDA,
        GPIO_I2C_CONTROLLER_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_CONTROLLER_IOMUX_SCL,
        GPIO_I2C_CONTROLLER_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_I2C_CONTROLLER_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_CONTROLLER_IOMUX_SCL);

    DL_GPIO_initDigitalOutput(GPIO_A_LED_GREEN_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_A_LED_RED_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_A_LED_BLUE_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_A_SW_S2_USER_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_A_IRQ_IN_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_A_SW_S1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIO_A_PORT, GPIO_A_LED_GREEN_PIN |
		GPIO_A_LED_RED_PIN |
		GPIO_A_LED_BLUE_PIN);
    DL_GPIO_enableOutput(GPIO_A_PORT, GPIO_A_LED_GREEN_PIN |
		GPIO_A_LED_RED_PIN |
		GPIO_A_LED_BLUE_PIN);
    DL_GPIO_setLowerPinsPolarity(GPIO_A_PORT, DL_GPIO_PIN_14_EDGE_RISE_FALL);
    DL_GPIO_setUpperPinsPolarity(GPIO_A_PORT, DL_GPIO_PIN_23_EDGE_FALL |
		DL_GPIO_PIN_18_EDGE_RISE_FALL);
    DL_GPIO_setLowerPinsInputFilter(GPIO_A_PORT, DL_GPIO_PIN_14_INPUT_FILTER_8_CYCLES);
    DL_GPIO_setUpperPinsInputFilter(GPIO_A_PORT, DL_GPIO_PIN_23_INPUT_FILTER_3_CYCLES);
    DL_GPIO_clearInterruptStatus(GPIO_A_PORT, GPIO_A_SW_S2_USER_PIN |
		GPIO_A_IRQ_IN_PIN |
		GPIO_A_SW_S1_PIN);
    DL_GPIO_enableInterrupt(GPIO_A_PORT, GPIO_A_SW_S2_USER_PIN |
		GPIO_A_IRQ_IN_PIN |
		GPIO_A_SW_S1_PIN);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be STANDBY1
    DL_SYSCTL_setPowerPolicySTANDBY1();
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);

}



/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32768 Hz = 32768 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gTIMG_DELAY_BLOCKINGClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMG_DELAY_BLOCKING_INST_LOAD_VALUE = (10 ms * 32768 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMG_DELAY_BLOCKINGTimerConfig = {
    .period     = TIMG_DELAY_BLOCKING_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_ONE_SHOT,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMG_DELAY_BLOCKING_init(void) {

    DL_TimerG_setClockConfig(TIMG_DELAY_BLOCKING_INST,
        (DL_TimerG_ClockConfig *) &gTIMG_DELAY_BLOCKINGClockConfig);

    DL_TimerG_initTimerMode(TIMG_DELAY_BLOCKING_INST,
        (DL_TimerG_TimerConfig *) &gTIMG_DELAY_BLOCKINGTimerConfig);
    DL_TimerG_enableInterrupt(TIMG_DELAY_BLOCKING_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMG_DELAY_BLOCKING_INST);





}

/*
 * Timer clock configuration to be sourced by LFCLK /  (32768 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   128 Hz = 32768 Hz / (1 * (255 + 1))
 */
static const DL_TimerG_ClockConfig gTIMG_DELAY_NO_BLOCKINGClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 255U,
};

/*
 * Timer load value (where the counter starts from) is calculated as (timerPeriod * timerClockFreq) - 1
 * TIMG_DELAY_NO_BLOCKING_INST_LOAD_VALUE = (10 ms * 128 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMG_DELAY_NO_BLOCKINGTimerConfig = {
    .period     = TIMG_DELAY_NO_BLOCKING_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_ONE_SHOT,
    .startTimer = DL_TIMER_STOP,
};

SYSCONFIG_WEAK void SYSCFG_DL_TIMG_DELAY_NO_BLOCKING_init(void) {

    DL_TimerG_setClockConfig(TIMG_DELAY_NO_BLOCKING_INST,
        (DL_TimerG_ClockConfig *) &gTIMG_DELAY_NO_BLOCKINGClockConfig);

    DL_TimerG_initTimerMode(TIMG_DELAY_NO_BLOCKING_INST,
        (DL_TimerG_TimerConfig *) &gTIMG_DELAY_NO_BLOCKINGTimerConfig);
    DL_TimerG_enableInterrupt(TIMG_DELAY_NO_BLOCKING_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);
    DL_TimerG_enableClock(TIMG_DELAY_NO_BLOCKING_INST);





}


static const DL_I2C_ClockConfig gI2C_CONTROLLERClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

SYSCONFIG_WEAK void SYSCFG_DL_I2C_CONTROLLER_init(void) {

    DL_I2C_setClockConfig(I2C_CONTROLLER_INST,
        (DL_I2C_ClockConfig *) &gI2C_CONTROLLERClockConfig);
    DL_I2C_setAnalogGlitchFilterPulseWidth(I2C_CONTROLLER_INST,
        DL_I2C_ANALOG_GLITCH_FILTER_WIDTH_50NS);
    DL_I2C_enableAnalogGlitchFilter(I2C_CONTROLLER_INST);

    /* Configure Controller Mode */
    DL_I2C_resetControllerTransfer(I2C_CONTROLLER_INST);
    /* Set frequency to 100000 Hz*/
    DL_I2C_setTimerPeriod(I2C_CONTROLLER_INST, 31);
    DL_I2C_setControllerTXFIFOThreshold(I2C_CONTROLLER_INST, DL_I2C_TX_FIFO_LEVEL_BYTES_1);
    DL_I2C_setControllerRXFIFOThreshold(I2C_CONTROLLER_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_disableControllerClockStretching(I2C_CONTROLLER_INST);

    /* Configure Interrupts */
    DL_I2C_enableInterrupt(I2C_CONTROLLER_INST,
                           DL_I2C_INTERRUPT_CONTROLLER_NACK |
                           DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_CONTROLLER_RX_DONE |
                           DL_I2C_INTERRUPT_CONTROLLER_START |
                           DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);


    /* Enable module */
    DL_I2C_enableController(I2C_CONTROLLER_INST);


}

