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
 * This example utilizes OPA0 and ADC0 to properly condition and read from the on board thermistor of the Launchpad
 * associated with the target device. This example does not implement the proper voltage to temperature conversion,
 * as those equations and method vary by thermistor manufacturer. Comments are placed in code for where to add such
 * calculations to get exact temperature readings. To show case actual ADC readings, this example takes an initial
 * reading of the thermistor, then changes LED colors depending on if temperature has drifted above or below the initial reading.
 * LED Color Code:
 * GREEN -- Temperature has not changed
 * RED   -- Temperature has increased
 * BLUE  -- Temperature has decreased
 */

#include "ti_msp_dl_config.h"
#include <math.h>
#define CHANGEFACTOR 10
volatile uint16_t gThermistorADCResult = 0;
volatile bool gCheckThermistor         = false;

volatile uint32_t gCelcius_reading = 0;
volatile uint32_t gInitial_reading = 0;
volatile uint16_t gAlivecheck =0;

int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_clearPins(RGB_PORT,
        (RGB_RED_PIN | RGB_GREEN_PIN | RGB_BLUE_PIN));

    NVIC_EnableIRQ(ADC_INST_INT_IRQN);
    gCheckThermistor = false;
    /* Start timer that triggers ADC sampling */
    DL_TimerG_startCounter(THERMISTOR_TIMER_ADC_INST);


    bool first_reading       = true;

    while (1) {
        while (gCheckThermistor == false) {
                   __WFE();
               }
        //Insert Thermistor Algorithm

               gCelcius_reading = gThermistorADCResult;

               if (first_reading) {
                   gInitial_reading = gCelcius_reading;
                   first_reading   = false;
               }

               /*
                * Change in LEDs is based on current sample compared to previous sample
                *
                * If the new sample is warmer than CHANGEFACTOR from initial temp, turn LED red
                * If the new sample is colder than CHANGEFACTOR  from initial temp, turn LED blue
                * Else, keep LED green
                * Variable gAlivecheck is utilized for debug window to confirm code is executing.
                * It is not needed in final applications.
                *
                */
               gAlivecheck++;
               if(gAlivecheck >= 0xFFF0){gAlivecheck =0;}
               if (gCelcius_reading - CHANGEFACTOR > gInitial_reading) {
                   DL_GPIO_clearPins(
                       RGB_PORT, (RGB_GREEN_PIN | RGB_BLUE_PIN));
                   DL_GPIO_setPins(RGB_PORT, RGB_RED_PIN);

               } else if (gCelcius_reading < gInitial_reading - CHANGEFACTOR) {
                   DL_GPIO_clearPins(
                       RGB_PORT, (RGB_RED_PIN | RGB_BLUE_PIN));
                   DL_GPIO_setPins(RGB_PORT, RGB_BLUE_PIN);

               } else {
                   DL_GPIO_clearPins(
                       RGB_PORT, (RGB_RED_PIN | RGB_BLUE_PIN));
                   DL_GPIO_setPins(RGB_PORT, RGB_GREEN_PIN);
               }

               gCheckThermistor = false;

        __WFI();
    }
}


/*
 * This IRQ is triggered whenever a new ADC conversion result is ready,
 * and the conversion result is stored in gThermistorADCResult.
 */
void ADC_INST_IRQHandler(void)
{
        switch (DL_ADC12_getPendingInterrupt(ADC_INST)) {
            case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
                gThermistorADCResult =
                    DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_0);
                gCheckThermistor = true;
                break;
            default:
                break;
        }
}
