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
//  thermistor.h
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

#ifndef THERMISTOR_H_
#define THERMISTOR_H_

#include <stdbool.h>
#include <stdint.h>

extern volatile uint16_t gThermistorADCResult;
extern volatile bool gCheckThermistor;

/*!
 *  @brief      Runs the thermistor example
 *
 *  Periodically reads the OPA+ADC measurement of the thermistor and drives
 *  and RGB LED based on the result.
 */
void thermistor_runExample(void);

#endif /* THERMISTOR_H_ */
