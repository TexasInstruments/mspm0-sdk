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
//  temperature.c
//
//  Using the internal temperature sensor and ADC, the ambient temperature will
//  be measured and displayed on the LCD screen. When S3 is pressed,
//  the units for the temperature will change from Celsius to Fahrenheit
//  and vice versa.
//*****************************************************************************

#include <temperature.h>

static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableTemperature) {
        GUIComm_sendBool("temperatureEnable", STR_LEN_ELEVEN, true);

        bEnableTemperature = true;
    }

    temperature_init();

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    gCheckTemperature = false;

    /* Start timer that triggers ADC sampling */
}

void temperature_runExample(void)
{
    uint16_t temperature_reading = 0;
    char firstDigit              = 0;
    char secondDigit             = 0;
    char thirdDigit              = 0;
    char units;

    setupExample();

    while (gDemoMode == TEMPERATURE_MODE) {
        DL_TimerG_startCounter(TEMPERATURE_TIMER_ADC_INST);
        while (gCheckTemperature == false && gDemoMode == TEMPERATURE_MODE) {
            __WFE();
        }

        /* Temperature Reading is intially c  */
        temperature_reading =
            (uint16_t) calc_temperature(gTemperatureADCResult);

        if (gTemperatureUnits) {
            //converts C to F
            temperature_reading = 9.0 / 5.0 * temperature_reading + 32;
            units               = 'F';

        } else {
            /*Display in C*/
            units = 'C';
        }

        /* Convert from decimal to ASCII */
        firstDigit  = 48 + temperature_reading % 10;
        secondDigit = 48 + (temperature_reading / 10) % 10;
        thirdDigit  = 48 + temperature_reading / 100;

        /* Change LCD to display the temperature */
        LCD_showChar(LCD, units, gLCDPinPosition6);
        LCD_showChar(LCD, firstDigit, gLCDPinPosition5);
        LCD_showChar(LCD, secondDigit, gLCDPinPosition4);
        LCD_showChar(LCD, thirdDigit, gLCDPinPosition3);

        GUIComm_sendUInt16("tValue", STR_LEN_SIX, temperature_reading);

        DL_ADC12_enableConversions(ADC12_0_INST);
        gCheckTemperature = false;
    }

    cleanupExample();
}

static void cleanupExample(void)
{
    /* Reset peripherals */
    DL_ADC12_reset(ADC12_0_INST);
    DL_LCD_reset(LCD);
    DL_VREF_reset(VREF);
    /* Power off peripherals */
    DL_ADC12_disablePower(ADC12_0_INST);
    DL_LCD_disablePower(LCD);
    DL_VREF_disablePower(VREF);

    NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ADC12_0_INST_INT_IRQN);

    /* Reset flags and temperature sensor enable widget */
    gCheckTemperature = false;

    GUIComm_sendBool("temperatureEnable", STR_LEN_ELEVEN, false);

    bEnableTemperature = false;
}

float calc_temperature(int raw_ADC)
{
    //Gets the voltage for the trim temp, this is done at 25 C
    float VTrim = ADC_VREF_VOLTAGE / ADC_BIT_RESOLUTION *
                  (DL_SYSCTL_getTempCalibrationConstant() - 0.5);

    //Gets the voltage for the sample from the ADC
    float Vsample = ADC_VREF_VOLTAGE / ADC_BIT_RESOLUTION * (raw_ADC - 0.5);

    //Takes the voltage from trim and ADC to get temperature in C
    float TemperatureDegC =
        (1 / TEMP_TS_COEF_mV_C * 1000) * (Vsample - VTrim) + TEMP_TS_TRIM_C;

    return TemperatureDegC;
}
