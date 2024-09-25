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
//  lightsensor.c
//
//  Use OPA0 in general purpose mode along with the on-board photodiode,
//  capacitor, and resistor to implement a light sensor circuit.
//  The resulting voltage is measured by the ADC0 module.
//  The ADC result is used to control TimerG1's and TimerG0's PWMs
//  duty-cycles to modulate the brightness of the RGB LED. The ADC result will
//  also be displayed on the GUI.
//*****************************************************************************

#include <ti/gui_composer/HAL.h>
#include "ti_msp_dl_config.h"

#include "demo_mode_declaration.h"
#include "lightsensor.h"

#include "callbacks_mpack.h"

volatile uint16_t gLightsensorADCResult = 0;
volatile bool gCheckLightsensor         = false;
volatile uint16_t gPWMDutyCycle         = 0;

static void setupExample(void);
static void cleanupExample(void);

static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableLightsensor) {
        GUIComm_sendBool("lightEnable", STR_LEN_ELEVEN, true);

        bEnableLightsensor = true;
    }

    lightsensor_init();

    /* Start the PWMs for the RGB LED */
    DL_TimerG_startCounter(LIGHTSENSOR_PWM_RED_BLUE_INST);
    DL_TimerG_startCounter(LIGHTSENSOR_PWM_GREEN_INST);

    NVIC_EnableIRQ(ADC_INST_INT_IRQN);
    gCheckLightsensor = false;

    /* Start timer that triggers ADC sampling */
    DL_TimerG_startCounter(LIGHTSENSOR_TIMER_ADC_INST);

    NVIC_EnableIRQ(LIGHTSENSOR_PWM_RED_BLUE_INST_INT_IRQN);
    NVIC_EnableIRQ(LIGHTSENSOR_PWM_GREEN_INST_INT_IRQN);
}

void lightsensor_runExample(void)
{
    setupExample();

    while (gDemoMode == LIGHTSENSOR_MODE) {
        while (gCheckLightsensor == false && gDemoMode == LIGHTSENSOR_MODE) {
            __WFE();
        }

        /*
         * Scale measured ADC value to a PWM duty cycle.
         * The PWM period is 512 cycles (RGB_LED_PERIOD), so ADC measurements
         * should be divided by 8 (4095 / 8 ~= 512,
         * within range of the period).
         */
        gPWMDutyCycle = gLightsensorADCResult / 8;

        GUIComm_sendUInt16("lsADC", STR_LEN_FIVE, gLightsensorADCResult);

        gCheckLightsensor = false;
    }

    cleanupExample();
}

void LIGHTSENSOR_PWM_RED_BLUE_INST_IRQHandler(void)
{
    /*
     * This IRQ changes the PWM duty cycles of the red and blue portions of
     * the RGB LED (controlled by TIMG1) whenever the timer counts down to
     * zero (zero event).
     */
    switch (DL_TimerG_getPendingInterrupt(LIGHTSENSOR_PWM_RED_BLUE_INST)) {
        case DL_TIMER_IIDX_ZERO:
            DL_TimerG_setCaptureCompareValue(LIGHTSENSOR_PWM_RED_BLUE_INST,
                RGB_LED_PERIOD - gPWMDutyCycle - 1, DL_TIMER_CC_0_INDEX);
            DL_TimerG_setCaptureCompareValue(LIGHTSENSOR_PWM_RED_BLUE_INST,
                RGB_LED_PERIOD - gPWMDutyCycle - 1, DL_TIMER_CC_1_INDEX);

            break;
        default:
            break;
    }
}

void LIGHTSENSOR_PWM_GREEN_INST_IRQHandler(void)
{
    /*
     * This IRQ changes the PWM duty cycle of the green portion of the RGB LED
     * (controlled by TIMG0) whenever the timer counts down to zero
     * (zero event).
     */
    switch (DL_TimerG_getPendingInterrupt(LIGHTSENSOR_PWM_GREEN_INST)) {
        case DL_TIMER_IIDX_ZERO:
            DL_TimerG_setCaptureCompareValue(LIGHTSENSOR_PWM_GREEN_INST,
                RGB_LED_PERIOD - gPWMDutyCycle - 1, DL_TIMER_CC_1_INDEX);

            break;
        default:
            break;
    }
}

static void cleanupExample(void)
{
    /* Reset peripherals */
    DL_TimerG_reset(LIGHTSENSOR_TIMER_ADC_INST);
    DL_TimerG_reset(LIGHTSENSOR_PWM_RED_BLUE_INST);
    DL_TimerG_reset(LIGHTSENSOR_PWM_GREEN_INST);
    DL_OPA_reset(LIGHTSENSOR_OPA_INST);
    DL_ADC12_reset(ADC_INST);

    /* Power off peripherals */
    DL_TimerG_disablePower(LIGHTSENSOR_TIMER_ADC_INST);
    DL_TimerG_disablePower(LIGHTSENSOR_PWM_RED_BLUE_INST);
    DL_TimerG_disablePower(LIGHTSENSOR_PWM_GREEN_INST);
    DL_OPA_disablePower(LIGHTSENSOR_OPA_INST);
    DL_ADC12_disablePower(ADC_INST);

    NVIC_DisableIRQ(ADC_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ADC_INST_INT_IRQN);

    NVIC_DisableIRQ(LIGHTSENSOR_PWM_RED_BLUE_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(LIGHTSENSOR_PWM_RED_BLUE_INST_INT_IRQN);

    NVIC_DisableIRQ(LIGHTSENSOR_PWM_GREEN_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(LIGHTSENSOR_PWM_GREEN_INST_INT_IRQN);

    /* Reset flags and light sensor enable widget */
    gCheckLightsensor = false;

    GUIComm_sendBool("lightEnable", STR_LEN_ELEVEN, false);
    bEnableLightsensor = false;

    /*
     * To fix M0L device sometimes getting stuck switching between
     * light sensor mode and thermistor mode.
     */
    HAL_System_Init();
}
