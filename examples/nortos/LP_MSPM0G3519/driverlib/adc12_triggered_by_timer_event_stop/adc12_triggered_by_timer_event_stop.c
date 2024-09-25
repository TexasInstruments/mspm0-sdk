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

/*
 * Check if adcResult corresponds to voltage level less 1.5V
 *
 * ((2^ (ADC resolution bits))*(1.5 / (Vref) )) =
 * ADC_value_to_monitor
 *
 */
#define ADC12_BIT_RESOLUTION (12)
#define ADC12_REF_VOLTAGE (3.3)
#define ADC12_MONITOR_VOLTAGE (1.5)
#define ADC12_MONITOR_VALUE \
    ((1 << ADC12_BIT_RESOLUTION) * (ADC12_MONITOR_VOLTAGE / ADC12_REF_VOLTAGE))

int main(void)
{
    SYSCFG_DL_init();

    /* Configure window comparators to detect changes above and below VDDA/2 */
    DL_ADC12_configWinCompHighThld(
        ADC12_0_INST, (uint16_t) ADC12_MONITOR_VALUE);

    DL_ADC12_configWinCompLowThld(
        ADC12_0_INST, (uint16_t) ADC12_MONITOR_VALUE);

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_TimerG_startCounter(TIMER_0_INST);

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
