/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/
//*****************************************************************************
//         callbacks_mpack.c
//
// Application level callbacks using MessagePack
// Texas Instruments, Inc.
//*****************************************************************************

#include "callbacks_mpack.h"

/* Global variables for the callbacks */
volatile bool bEnableBlinkLED;
volatile bool bChangeBlinkFreq;
volatile float fBlinkLEDDelay;

volatile bool bEnableLightsensor;
volatile bool bEnableThermistor;

volatile bool bEnableDAC;
volatile bool bChangeType;
volatile bool bChangeFreq;
volatile bool bChangeAmp;
volatile bool bChangeInversion;
volatile bool bChangeGain;
volatile int iWaveType;
volatile int iFrequency;
volatile int iAmplitude;
volatile int iGain;
volatile bool bInversion;
volatile bool bDisplay;

/* Application Callback Functions to Process data */
void callback_blinkEnable(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bEnableBlinkLED = GUIComm_ReadBool(tag);

    if (bEnableBlinkLED) {
        gDemoMode = BLINK_LED_MODE;
    } else {
        gDemoMode = IDLE;
    }
}

void callback_setBlinkDelay(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bChangeBlinkFreq = true;

    /* Convert Q value to float for use in timer period calculation */
    fBlinkLEDDelay = GUIComm_ReadUInt16(tag) / 256.0;
}

void callback_lightEnable(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bEnableLightsensor = GUIComm_ReadBool(tag);

    if (bEnableLightsensor) {
        gDemoMode = LIGHTSENSOR_MODE;
    } else {
        gDemoMode = IDLE;
    }
}

void callback_thermistorEnable(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bEnableThermistor = GUIComm_ReadBool(tag);

    if (bEnableThermistor) {
        gDemoMode = THERMISTOR_MODE;
    } else {
        gDemoMode = IDLE;
    }
}

/* Function Generator Additions */
void callback_DACEnable(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bEnableDAC = GUIComm_ReadBool(tag);

    if (bEnableDAC) {
        gDemoMode = DAC_MODE;
    } else {
        gDemoMode = IDLE;
    }
}

void callback_setWaveType(mpack_tag_t *tag, mpack_reader_t *reader)
{
    iWaveType   = GUIComm_ReadUInt8(tag);
    bChangeType = true;
}

void callback_setInversion(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bInversion       = GUIComm_ReadBool(tag);
    bChangeInversion = true;
}

void callback_setFrequency(mpack_tag_t *tag, mpack_reader_t *reader)
{
    iFrequency  = GUIComm_ReadUInt16(tag);
    bChangeFreq = true;
}

void callback_setGain(mpack_tag_t *tag, mpack_reader_t *reader)
{
    iGain       = GUIComm_ReadUInt16(tag);
    bChangeGain = true;
}

void callback_setAmplitude(mpack_tag_t *tag, mpack_reader_t *reader)
{
    iAmplitude = GUIComm_ReadUInt16(tag);
    bChangeAmp = true;
}

void callback_ADCEnable(mpack_tag_t *tag, mpack_reader_t *reader)
{
    bDisplay = GUIComm_ReadBool(tag);
}
