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
//  tempsensor.c
//
//  Display the collected ADC values of temperature sensor on the graph.
//
//*****************************************************************************
#include "ti_msp_dl_config.h"
#include "demo_mode_declaration.h"
#include "callbacks_mpack.h"
#include "temp_sensor.h"

/* ADC Raw Data */
static volatile uint16_t gTempsensorADCResult;
/* Temperature Sensor ADC Complete Flag*/
static volatile bool gCheckTempsensor = false;
/* Current Temperature in Celsius */
static volatile float curTemp;

static void setupExample(void);
static void cleanupExample(void);
/* Temperature Conversion by ADC Raw Data */
static float tempCal(uint16_t adcValue);

static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableTempsensor) {
        GUIComm_sendBool("tempEnable", STR_LEN_TEN, true);
        bEnableTempsensor = true;
    }

    /* Initialize peripherals */
    temp_sensor_init();

    bEnableTempsensor = true;

    /* Enable the ADC Interrupt */
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    gCheckTempsensor = false;
}

void temp_sensor_runExample(void)
{
    setupExample();

    while (gDemoMode == TEMP_MODE) {
        DL_ADC12_startConversion(ADC12_0_INST);

        while (gCheckTempsensor == false && gDemoMode == TEMP_MODE) {
            __WFE();
        }

        gTempsensorADCResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
        curTemp = tempCal(gTempsensorADCResult);
        uint16_t tempInt = (uint16_t)curTemp;
        /* Round results to two decimal */
        uint16_t tempDec = (uint16_t)((curTemp - tempInt) * 100);

        /* Send Data to GUI */
        GUIComm_sendUInt16("tempADC", STR_LEN_SEVEN, gTempsensorADCResult);
        GUIComm_sendUInt16("tempInt", STR_LEN_SEVEN, tempInt);
        GUIComm_sendUInt16("tempDec", STR_LEN_SEVEN, tempDec);

        gCheckTempsensor = false;
        /* Every 1s send value to GUI */
        delay_cycles(32000000);
        DL_ADC12_enableConversions(ADC12_0_INST);
    }

    cleanupExample();
}

static void cleanupExample(void)
{
    /* Reset peripherals */
    DL_ADC12_reset(ADC12_0_INST);

    /* Power off peripherals */
    DL_ADC12_disablePower(ADC12_0_INST);

    NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ADC12_0_INST_INT_IRQN);

    /* Reset flag */
    gCheckTempsensor = false;
    GUIComm_sendBool("tempEnable", STR_LEN_TEN, false);
    bEnableTempsensor = false;
}

/* Temperature Conversion by ADC Raw Data */
float tempCal(uint16_t adcValue) {
    float v_sample = 5.0/4096*(adcValue-0.5);
    float v_trim   = 1.4/4096*(TEMP_SENSOR_DATA-0.5);
    float temp     = (1.0/TS_C)*(v_sample-v_trim)+TS_TRIM;
    return temp;
}

/* ADC Interrupt */
void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckTempsensor = true;
            break;
        default:
            break;
    }
}
