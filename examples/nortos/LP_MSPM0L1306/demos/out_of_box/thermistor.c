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
//*****************************************************************************
//
//  thermistor.c
//
//  Use OPA0 in general purpose mode along with the on-board thermistor, to
//  send a signal directly to the ADC. The ADC result is used to set the color
//  of the RGB LED.
//
//  Temperature drops of over 2 degrees Celcius will result in a blue LED while
//  temperature increases of over 2 degrees Celcius will result in a red LED.
//  Temperature changes less than 2 degrees Celcius will result in a green LED.
//  The ADC result will also be displayed on the GUI.
//*****************************************************************************

#include <ti/gui_composer/HAL.h>
#include "ti_msp_dl_config.h"

#include "demo_mode_declaration.h"
#include "thermistor.h"

#include <math.h>
#include "callbacks_mpack.h"

volatile uint16_t gThermistorADCResult = 0;
volatile bool gCheckThermistor         = false;
float VBias                            = 3.30;  // set the VBIAS voltage
unsigned int ADC_BITS =
    4096;  // set the number of bits based on you ADC (2^# of ADC Bit Value)
float VTEMP     = 0;  // set up the variable for the measured voltage
float THRM_TEMP = 0;  // setup the variable for the calculated temperature

static void setupExample(void);
static void cleanupExample(void);
static float thermistor_calc_temperature(int raw_ADC);

static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableThermistor) {
        GUIComm_sendBool("thermEnable", STR_LEN_ELEVEN, true);

        bEnableThermistor = true;
    }

    thermistor_init();

    /* Thermistor uses RGB LED as GPIOs, not PWMs. Manually set up GPIOs here */
    DL_GPIO_initDigitalOutput(GPIO_IOMUX_RGB_RED);
    DL_GPIO_initDigitalOutput(GPIO_IOMUX_RGB_GREEN);
    DL_GPIO_initDigitalOutput(GPIO_IOMUX_RGB_BLUE);

    DL_GPIO_enableOutput(GPIO_RGB_PORT,
        (GPIO_RGB_RED_PIN | GPIO_RGB_GREEN_PIN | GPIO_RGB_BLUE_PIN));

    DL_GPIO_clearPins(GPIO_RGB_PORT,
        (GPIO_RGB_RED_PIN | GPIO_RGB_GREEN_PIN | GPIO_RGB_BLUE_PIN));

    NVIC_EnableIRQ(ADC_INST_INT_IRQN);
    gCheckThermistor = false;

    /* Start timer that triggers ADC sampling */
    DL_TimerG_startCounter(THERMISTOR_TIMER_ADC_INST);
}

void thermistor_runExample(void)
{
    uint16_t celcius_reading = 0;
    uint16_t initial_reading = 0;
    bool first_reading       = true;

    setupExample();

    while (gDemoMode == THERMISTOR_MODE) {
        while (gCheckThermistor == false && gDemoMode == THERMISTOR_MODE) {
            __WFE();
        }

        celcius_reading =
            (uint32_t) thermistor_calc_temperature(gThermistorADCResult);

        if (first_reading) {
            initial_reading = celcius_reading;
            first_reading   = false;
        }

        GUIComm_sendUInt32("thADC", STR_LEN_FIVE, celcius_reading);

        /*
         * Change in LEDs is based on current sample compared to previous sample
         *
         * If the new sample is warmer than 2* from initial temp, turn LED red
         * If the new sample is colder than 2* from initial temp, turn LED blue
         * Else, keep LED green
         */

        if (celcius_reading - 64 > initial_reading) {
            DL_GPIO_clearPins(
                GPIO_RGB_PORT, (GPIO_RGB_GREEN_PIN | GPIO_RGB_BLUE_PIN));
            DL_GPIO_setPins(GPIO_RGB_PORT, GPIO_RGB_RED_PIN);
        } else if (celcius_reading < initial_reading - 64) {
            DL_GPIO_clearPins(
                GPIO_RGB_PORT, (GPIO_RGB_RED_PIN | GPIO_RGB_BLUE_PIN));
            DL_GPIO_setPins(GPIO_RGB_PORT, GPIO_RGB_BLUE_PIN);
        } else {
            DL_GPIO_clearPins(
                GPIO_RGB_PORT, (GPIO_RGB_RED_PIN | GPIO_RGB_BLUE_PIN));
            DL_GPIO_setPins(GPIO_RGB_PORT, GPIO_RGB_GREEN_PIN);
        }

        gCheckThermistor = false;
    }

    cleanupExample();
}

static void cleanupExample(void)
{
    /* Reset peripherals */
    DL_TimerG_reset(THERMISTOR_TIMER_ADC_INST);
    DL_OPA_reset(THERMISTOR_OPA_INST);
    DL_ADC12_reset(ADC_INST);

    /* Power off peripherals */
    DL_TimerG_disablePower(THERMISTOR_TIMER_ADC_INST);
    DL_OPA_disablePower(THERMISTOR_OPA_INST);
    DL_ADC12_disablePower(ADC_INST);

    NVIC_DisableIRQ(ADC_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ADC_INST_INT_IRQN);

    /* Reset flags and thermistor sensor enable widget */
    gCheckThermistor = false;

    DL_GPIO_clearPins(GPIO_RGB_PORT,
        (GPIO_RGB_RED_PIN | GPIO_RGB_GREEN_PIN | GPIO_RGB_BLUE_PIN));

    GUIComm_sendBool("thermEnable", STR_LEN_ELEVEN, false);
    bEnableThermistor = false;

    /*
     * To fix M0L device sometimes getting stuck switching between
     * light sensor mode and thermistor mode.
     */
    HAL_System_Init();
}

float thermistor_calc_temperature(int raw_ADC)
{
    // THRM calculations via regression
    // Copied from TI Thermistor Design Tool Excel Doc
    VTEMP          = 0.0;
    float THRM_ADC = raw_ADC;

    float THRM_A0 = -4.232811E+02;
    float THRM_A1 = 4.728797E+02;
    float THRM_A2 = -1.988841E+02;
    float THRM_A3 = 4.869521E+01;
    float THRM_A4 = -1.158754E+00;

    VTEMP =
        (VBias / ADC_BITS) *
        THRM_ADC;  // calculate volts per bit then multiply that times the ADV value
    THRM_TEMP = (THRM_A4 * powf(VTEMP, 4)) + (THRM_A3 * powf(VTEMP, 3)) +
                (THRM_A2 * powf(VTEMP, 2)) + (THRM_A1 * VTEMP) +
                THRM_A0;    // 4th order regression to get temperature
    return THRM_TEMP * 32;  // multiply by 32 for GC to divide later
}
