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
    SYSCFG_DL_LIN_0_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_UART_Extend_reset(LIN_0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_UART_Extend_enablePower(LIN_0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_LIN_0_IOMUX_TX, GPIO_LIN_0_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_LIN_0_IOMUX_RX, GPIO_LIN_0_IOMUX_RX_FUNC);

    DL_GPIO_initDigitalOutput(GPIO_LIN_ENABLE_USER_LIN_ENABLE_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES1_USER_SWITCH_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_SWITCHES2_USER_SWITCH_2_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_2_IOMUX);

    DL_GPIO_setUpperPinsPolarity(GPIO_SWITCHES2_PORT, DL_GPIO_PIN_18_EDGE_RISE);
    DL_GPIO_clearInterruptStatus(GPIO_SWITCHES2_PORT, GPIO_SWITCHES2_USER_SWITCH_2_PIN);
    DL_GPIO_enableInterrupt(GPIO_SWITCHES2_PORT, GPIO_SWITCHES2_USER_SWITCH_2_PIN);
    DL_GPIO_clearPins(GPIOB, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN |
		GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_enableOutput(GPIOB, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN |
		GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOB, DL_GPIO_PIN_21_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(GPIOB, GPIO_SWITCHES1_USER_SWITCH_1_PIN);
    DL_GPIO_enableInterrupt(GPIOB, GPIO_SWITCHES1_USER_SWITCH_1_PIN);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

    
	DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();
	DL_SYSCTL_disableSYSPLL();

}


static const DL_UART_Extend_ClockConfig gLIN_0ClockConfig = {
    .clockSel    = DL_UART_EXTEND_CLOCK_BUSCLK,
    .divideRatio = DL_UART_EXTEND_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Extend_Config gLIN_0Config = {
    .mode        = DL_UART_EXTEND_MODE_NORMAL,
    .direction   = DL_UART_EXTEND_DIRECTION_TX_RX,
    .flowControl = DL_UART_EXTEND_FLOW_CONTROL_NONE,
    .parity      = DL_UART_EXTEND_PARITY_NONE,
    .wordLength  = DL_UART_EXTEND_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_EXTEND_STOP_BITS_ONE
};

SYSCONFIG_WEAK void SYSCFG_DL_LIN_0_init(void)
{
    DL_UART_Extend_setClockConfig(LIN_0_INST, (DL_UART_Extend_ClockConfig *) &gLIN_0ClockConfig);

    DL_UART_Extend_init(LIN_0_INST, (DL_UART_Extend_Config *) &gLIN_0Config);
    /*
     * Configure baud rate by setting oversampling and baud rate divisors.
     *  Target baud rate: 19200
     *  Actual baud rate: 19199.04
     */
    DL_UART_Extend_setOversampling(LIN_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Extend_setBaudRateDivisor(LIN_0_INST, LIN_0_IBRD_32_MHZ_19200_BAUD, LIN_0_FBRD_32_MHZ_19200_BAUD);


    /* Configure Interrupts */
    DL_UART_Extend_enableInterrupt(LIN_0_INST,
                                 DL_UART_EXTEND_INTERRUPT_RX);


    /* Configure LIN settings */
    DL_UART_Extend_setLINCounterValue(LIN_0_INST, 0);

    DL_UART_Extend_enable(LIN_0_INST);
}

