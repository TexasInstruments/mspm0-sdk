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
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);

    DL_GPIO_enablePower(GPIOA);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{
    const uint8_t unusedPinIndexes[] =
    {
        IOMUX_PINCM1, IOMUX_PINCM2, IOMUX_PINCM3, IOMUX_PINCM5,
        IOMUX_PINCM6, IOMUX_PINCM7, IOMUX_PINCM8, IOMUX_PINCM9,
        IOMUX_PINCM10, IOMUX_PINCM11, IOMUX_PINCM12, IOMUX_PINCM13,
        IOMUX_PINCM14, IOMUX_PINCM16, IOMUX_PINCM17, IOMUX_PINCM18,
        IOMUX_PINCM22, IOMUX_PINCM23, IOMUX_PINCM24, IOMUX_PINCM25,
        IOMUX_PINCM26
    };

    for(int i = 0; i < sizeof(unusedPinIndexes)/sizeof(unusedPinIndexes[0]); i++)
    {
        DL_GPIO_initDigitalOutput(unusedPinIndexes[i]);
    }

    DL_GPIO_clearPins(GPIOA,
        (DL_GPIO_PIN_0 | DL_GPIO_PIN_1 | DL_GPIO_PIN_2 | DL_GPIO_PIN_4 |
        DL_GPIO_PIN_5 | DL_GPIO_PIN_6 | DL_GPIO_PIN_7 | DL_GPIO_PIN_8 |
        DL_GPIO_PIN_9 | DL_GPIO_PIN_10 | DL_GPIO_PIN_11 | DL_GPIO_PIN_12 |
        DL_GPIO_PIN_13 | DL_GPIO_PIN_15 | DL_GPIO_PIN_16 | DL_GPIO_PIN_17 |
        DL_GPIO_PIN_21 | DL_GPIO_PIN_22 | DL_GPIO_PIN_23 | DL_GPIO_PIN_24 |
        DL_GPIO_PIN_25));
    DL_GPIO_enableOutput(GPIOA,
        (DL_GPIO_PIN_0 | DL_GPIO_PIN_1 | DL_GPIO_PIN_2 | DL_GPIO_PIN_4 |
        DL_GPIO_PIN_5 | DL_GPIO_PIN_6 | DL_GPIO_PIN_7 | DL_GPIO_PIN_8 |
        DL_GPIO_PIN_9 | DL_GPIO_PIN_10 | DL_GPIO_PIN_11 | DL_GPIO_PIN_12 |
        DL_GPIO_PIN_13 | DL_GPIO_PIN_15 | DL_GPIO_PIN_16 | DL_GPIO_PIN_17 |
        DL_GPIO_PIN_21 | DL_GPIO_PIN_22 | DL_GPIO_PIN_23 | DL_GPIO_PIN_24 |
        DL_GPIO_PIN_25));

    DL_GPIO_initDigitalInput(GPIO_SWITCH_USER_SWITCH_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_1_IOMUX);

    DL_GPIO_initDigitalOutput(GPIO_LEDS_USER_LED_2_IOMUX);

    DL_GPIO_initDigitalInputFeatures(GPIO_INPUT_CONFIG_0_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(GPIO_INPUT_CONFIG_1_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(GPIOA, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_enableOutput(GPIOA, GPIO_LEDS_USER_LED_1_PIN |
		GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_setUpperPinsPolarity(GPIOA, DL_GPIO_PIN_18_EDGE_RISE);

}



SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);

	//Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);

}

