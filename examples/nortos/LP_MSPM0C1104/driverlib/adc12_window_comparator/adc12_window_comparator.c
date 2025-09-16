/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

volatile int16_t gADCOffset;

int main(void)
{
    int16_t adcRaw;
    uint16_t adcWinCompLowThld, adcWinCompHighThld;

    SYSCFG_DL_init();

    /* Get calibrated ADC offset - workaround for ADC_ERR_06 */
    gADCOffset =
        DL_ADC12_getADCOffsetCalibration(ADC12_0_ADCMEM_0_REF_VOLTAGE_V);

    /* Apply calibrated ADC offset - workaround for ADC_ERR_06 */
    adcRaw = (int16_t) ADC12_0_WIN_COMP_LOW_THLD_VAL - gADCOffset;
    if (adcRaw < 0) {
        adcRaw = 0;
    }
    if (adcRaw > 4095) {
        adcRaw = 4095;
    }
    adcWinCompLowThld = (uint16_t) adcRaw;

    adcRaw = (int16_t) ADC12_0_WIN_COMP_HIGH_THLD_VAL - gADCOffset;
    if (adcRaw < 0) {
        adcRaw = 0;
    }
    if (adcRaw > 4095) {
        adcRaw = 4095;
    }
    adcWinCompHighThld = (uint16_t) adcRaw;

    /* Configure window comparators to detect changes greater than 1.5V */
    DL_ADC12_configWinCompLowThld(ADC12_0_INST, adcWinCompLowThld);
    DL_ADC12_configWinCompHighThld(ADC12_0_INST, adcWinCompHighThld);

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_ADC12_startConversion(ADC12_0_INST);

    /* Calling WFI after calling DL_SYSCTL_enableSleepOnExit will result in
     * only ISR code to be executed. This is done to showcase the device's
     * low power consumption when sleeping.
     */
    DL_SYSCTL_enableSleepOnExit();

    while (1) {
        __WFI();
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_WINDOW_COMP_HIGH:
            DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            break;
        case DL_ADC12_IIDX_WINDOW_COMP_LOW:
            DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            break;
        default:
            break;
    }
}
