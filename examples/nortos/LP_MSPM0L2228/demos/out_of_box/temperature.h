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
//  temperature.h
//
//  Use the ADC to take a measurement of the internal temperature sensor and
//  update the LCD display with the results.
//
//  The ADC result will also be displayed on the GUI.
//*****************************************************************************

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <math.h>
#include <stdint.h>
#include "callbacks_mpack.h"
#include "demo_mode_declaration.h"
#include "lcd.h"
#include "ti_msp_dl_config.h"

extern volatile uint16_t gTemperatureADCResult;
extern volatile bool gCheckTemperature;
extern volatile bool gTemperatureUnits;
float VTEMP;              // set up the variable for the measured voltage
float TemperatureResult;  // setup the variable for the calculated temperature

/*
 * The following trim parameter are provided in the device datasheet in chapter
 * "Temperature Sensor"
 */
#define TEMP_TS_TRIM_C ((uint32_t) 25)
/*
 * Constant below is (1/TSc). Where TSc is Temperature Sensor coefficient
 * available in the device datasheet
 */
#define TEMP_TS_COEF_mV_C (-571.42f)
#define ADC_VREF_VOLTAGE (1.4f)
#define ADC_BIT_RESOLUTION ((uint32_t)(1) << 12)
/*!
 *  @brief      Runs the temperature example
 *
 *  Periodically reads the ADC measurement of the internal temperature sensor and updates
 *  the LCD based on the result. Temperature units can change through S1 or GUI
 */
void temperature_runExample(void);
static void setupExample(void);
static void cleanupExample(void);
static float calc_temperature(int);

#endif /* TEMPERATURE_H_ */
