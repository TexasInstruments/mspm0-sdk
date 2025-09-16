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
 * Check if adcResult corresponds to voltage level less than 2.5V
 * in MCU voltage supply.
 *
 * Supply/Monitor voltage monitor channel is (VDD / 3)
 *
 * ((2^ (ADC resolution bits))*(Voltage_to_monitor / (3 * Vref) )) =
 * ADC_value_to_monitor
 *
 */

/* clang-format off */
#define ADC12_BIT_RESOLUTION                (12)
#define ADC12_REF_VOLTAGE                   (2.5)
#define ADC12_SUPPLY_MONITOR_VOLTAGE        (2.5)
 #define ADC12_SUPPLY_MONITOR_VALUE         ( (1 << ADC12_BIT_RESOLUTION) * \
                                              (ADC12_SUPPLY_MONITOR_VOLTAGE / \
                                               (3 * ADC12_REF_VOLTAGE)))
/* clang-format on */

volatile bool gCheckADC;
volatile float gAdcResultVolts;

int main(void)
{
    uint16_t adcResult;
    SYSCFG_DL_init();

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    gCheckADC = false;

    DL_ADC12_startConversion(ADC12_0_INST);

    while (1) {
        while (false == gCheckADC) {
            __WFE();
        }

        gCheckADC = false;

        /* Result in integer for efficient processing */
        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        /* Result in float for simpler reading */
        gAdcResultVolts =
            (adcResult * ADC12_REF_VOLTAGE) / (1 << ADC12_BIT_RESOLUTION) * 3;

        if (adcResult > ADC12_SUPPLY_MONITOR_VALUE) {
            DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        } else {
            DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        }
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
