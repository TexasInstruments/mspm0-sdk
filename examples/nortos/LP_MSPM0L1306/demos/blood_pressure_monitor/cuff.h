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
//*****************************************************************************
//  cuff.h
//*****************************************************************************

#ifndef BPM_H_
#define BPM_H_

#include <stdbool.h>
#include <stdint.h>

#define MAX_CUFF_PRESSURE \
    3277  // Maximum adc value for cuff pressure before cuff deflates
#define VALVE_OPEN_MOD 14  // Modulus to time when to open valve
#define MIN_PEAK_PRESSURE 2800

#define OPEN_VALVE 0
#define PULSE_OPEN_VALVE 1
#define ENABLE_ADC 4

/* ADC */
extern volatile uint16_t ADCResult_CuffPressure;
extern volatile uint16_t ADCResult_CuffOscPressure;
extern volatile bool checkADC;

/* Valve timer */
extern volatile uint16_t valveOpenCounter;

/* Peak detection */
extern volatile uint16_t prevOsc;
extern volatile uint16_t peakOsc;
extern volatile uint16_t
    decCounterOsc;  // Count the number of times the ADC result decreases in a row
extern volatile uint16_t peakValuesOsc[5];  // Store peak values
extern volatile uint16_t peaksFoundOsc;     // Number of peaks found
extern volatile uint16_t
    peaksAfterMax;  // Number of peaks after max peak for oscillations
extern volatile bool peakFlag;
extern volatile bool
    ignoreFirstPeak;  // For this particular application, first peak ignored

extern volatile bool deflateCuff;  // Flag for when cuff should deflate

extern int startupFlag;  // Flag to indicate program is restarting

/*!
 *  @brief      Start controlling cuff using motor
 *
 *  Inflates cuff until a certain pressure or until a max peak followed by three decreasing peaks found.
 *  Then deflate cuff by opening value and turning off motor.
 */
void setupExample(void);

/*!
 *  @brief      Start ADC to get cuff pressure and oscillation readings
 */
void run_activeMode(void);

/*!
 *  @brief      Reset peripherals
 */
void cleanupExample(void);

#endif /* BPM_H_ */
