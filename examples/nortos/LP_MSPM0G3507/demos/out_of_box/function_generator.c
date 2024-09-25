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
//******************************************************************************
//
//  function_generator.c
//
//  Generate a signal using the DAC. The DMA will load data into the DAC since
//  the DAC is operating at max sampling rate. The user can view the signal on
//  the OPA output pin, or they can have the signal be captured with the internal
//  ADC connection and displayed on the GUI.
//*****************************************************************************

#include "ti_msp_dl_config.h"

#include "demo_mode_declaration.h"
#include "function_generator.h"

#include "callbacks_mpack.h"
#include "functions.h"

uint16_t gOutputSignal512[512];

volatile uint32_t gidealData;

static void setupExample(void);
static void cleanupExample(void);

bool enableDAC;
bool changeType;
bool changeFreq;
bool changeAmp;
bool changeInversion;
bool changeGain;
bool dispADC;
volatile int gain;

volatile uint16_t gFunctionGeneratorADCResult;
volatile bool bCheckADCVal = false;

void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableDAC) {
        GUIComm_sendBool("dacEnable", STR_LEN_NINE, true);

        bEnableDAC = true;
    }

    functiongenerator_init(iGain, !bInversion);

    changeWaveType();  // Must be called before starting transfer so the LUT is populated

    DL_DMA_setSrcAddr(DMA, FNGEN_DMA_CH0_SEL, (uint32_t) &gOutputSignal512[0]);
    DL_DMA_setDestAddr(
        DMA, FNGEN_DMA_CH0_SEL, (uint32_t) & (FNGEN_DAC_INST->DATA0));
    DL_DMA_setTransferSize(
        DMA, FNGEN_DMA_CH0_SEL, sizeof(gOutputSignal512) / sizeof(uint16_t));

    DL_DMA_enableChannel(DMA, FNGEN_DMA_CH0_SEL);

    NVIC_EnableIRQ(FNGEN_DAC_INST_INT_IRQN);
    DL_DAC12_enable(FNGEN_DAC_INST);
    DL_ADC12_startConversion(FNGEN_ADC_INST);

    DL_DAC12_enableDMATrigger(FNGEN_DAC_INST);

    NVIC_ClearPendingIRQ(FNGEN_ADC_INST_INT_IRQN);
    NVIC_EnableIRQ(FNGEN_ADC_INST_INT_IRQN);
}

void functiongenerator_runExample(void)
{
    // When function generator is enabled in GUI, reset all parameters to default sine wave
    GUIComm_sendUInt16("setAmp", STR_LEN_SIX, 100);
    iAmplitude = 100;
    GUIComm_sendUInt16("setType", STR_LEN_SEVEN, 0);
    iWaveType = 0;
    GUIComm_sendUInt16("setFreq", STR_LEN_SEVEN, 0);
    iFrequency = 0;
    GUIComm_sendUInt16("setGain", STR_LEN_SEVEN, 0);
    iGain = DL_OPA_GAIN_N0_P1;
    GUIComm_sendUInt16("inversion", STR_LEN_NINE, 0);
    bInversion = 0;

    setupExample();

    bool bChange = bChangeType | bChangeFreq | bChangeAmp | bChangeInversion |
                   bChangeGain;

    int currDACOutput = 0;

    while (gDemoMode == DAC_MODE) {
        if (bDisplay && bCheckADCVal) {
            GUIComm_sendUInt16("dacADC", STR_LEN_SIX, gADCResult);
            bCheckADCVal = false;
        }

        // conditionals to update the settings of the peripherals
        if (bChangeType || bChangeAmp) {
            setupExample();
            bChangeFreq = true;
        }

        if (bChangeFreq) {
            if (iFrequency == 0) {
                DL_DAC12_setSampleRate(
                    FNGEN_DAC_INST, DL_DAC12_SAMPLES_PER_SECOND_500);
            } else if (iFrequency == 1) {
                DL_DAC12_setSampleRate(
                    FNGEN_DAC_INST, DL_DAC12_SAMPLES_PER_SECOND_1K);
            } else if (iFrequency == 2) {
                DL_DAC12_setSampleRate(
                    FNGEN_DAC_INST, DL_DAC12_SAMPLES_PER_SECOND_2K);
            }
            bChangeFreq = false;
        }

        if (bChangeInversion) {
            if (!bInversion) {
                functiongenerator_OPA_initNINV(iGain, 1);
            } else if (bInversion) {
                functiongenerator_OPA_initNINV(iGain, 0);
            }
            setupExample();
            bChangeInversion = false;
        }

        if (bChangeGain) {
            // Conditional to set gain equal to an existing macro
            if (iGain == 0) {
                iGain = DL_OPA_GAIN_N0_P1;
            } else if (iGain == 1) {
                iGain = DL_OPA_GAIN_N1_P2;
            } else if (iGain == 2) {
                iGain = DL_OPA_GAIN_N3_P4;
            } else if (iGain == 3) {
                iGain = DL_OPA_GAIN_N7_P8;
            } else if (iGain == 4) {
                iGain = DL_OPA_GAIN_N15_P16;
            } else if (iGain == 5) {
                iGain = DL_OPA_GAIN_N31_P32;
            }

            // Configure OPA module to new gain setting
            if (!bInversion) {
                functiongenerator_OPA_initNINV(iGain, 1);
            } else if (bInversion) {
                functiongenerator_OPA_initNINV(iGain, 0);
            }
            bChangeGain = false;
        }
    }

    cleanupExample();
}

static void cleanupExample(void)
{
    DL_VREF_reset(VREF);
    DL_DAC12_reset(FNGEN_DAC_INST);
    DL_OPA_reset(FNGEN_OPA_INST);
    DL_TimerG_reset(FNGEN_TIMER_ADC_INST);
    DL_ADC12_reset(FNGEN_ADC_INST);

    DL_DMA_disableChannel(DMA, FNGEN_DMA_CH0_SEL);
    DL_VREF_disablePower(VREF);
    DL_DAC12_disablePower(FNGEN_DAC_INST);
    DL_OPA_disablePower(FNGEN_OPA_INST);
    DL_TimerG_disablePower(FNGEN_TIMER_ADC_INST);
    DL_ADC12_disablePower(FNGEN_ADC_INST);

    GUIComm_sendBool("dacEnable", STR_LEN_NINE, false);
    bEnableDAC = false;
}

void changeWaveType(void)
{
    DL_DMA_disableChannel(DMA, FNGEN_DMA_CH0_SEL);

    // TODO: remove once Qmath library is updated
    for (int i = 0; i < 512; i++) {
        switch (iWaveType) {
            case 0:
                gOutputSignal512[i] =
                    (iAmplitude * gOutputSignalSine512[i]) / 100;
                break;
            case 1:
                gOutputSignal512[i] =
                    (iAmplitude * gOutputSignalSquare512[i]) / 100;
                break;
            case 2:
                gOutputSignal512[i] =
                    (iAmplitude * gOutputSignalSawtooth512[i]) / 100;
                break;
            case 3:
                gOutputSignal512[i] =
                    (iAmplitude * gOutputSignalTriangle512[i]) / 100;
                break;
        }
    }

    DL_DMA_setSrcAddr(DMA, FNGEN_DMA_CH0_SEL, (uint32_t) &gOutputSignal512[0]);
    DL_DMA_setDestAddr(
        DMA, FNGEN_DMA_CH0_SEL, (uint32_t) & (FNGEN_DAC_INST->DATA0));
    DL_DMA_setTransferSize(
        DMA, FNGEN_DMA_CH0_SEL, sizeof(gOutputSignal512) / sizeof(uint16_t));

    DL_DMA_enableChannel(DMA, FNGEN_DMA_CH0_SEL);

    bChangeType = false;
    bChangeAmp  = false;
}
