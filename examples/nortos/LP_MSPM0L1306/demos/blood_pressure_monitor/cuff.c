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
//  cuff.c
//*****************************************************************************

#include <cuff.h>
#include "callbacks_mpack.h"
#include "demo_mode_declaration.h"
#include "ti_msp_dl_config.h"

/* Variable to track the current state of demo */
volatile demoMode gDemoMode = IDLE;

/* ADC */
volatile uint16_t ADCResult_CuffPressure    = 0;
volatile uint16_t ADCResult_CuffOscPressure = 0;
volatile bool checkADC                      = false;

/* Valve timer */
volatile uint16_t valveOpenCounter = 0;

volatile bool deflateCuff = false;  // Flag for when cuff should deflate

int startupFlag = 0;

/* Function declarations */
void cleanupExample(void);

/* Start ADC */
void setupExample(void)
{
    startADC();
    NVIC_EnableIRQ(ADC_INST_INT_IRQN);
    checkADC = false;

    /* Start timer that triggers ADC sampling */
    DL_TimerG_startCounter(TIMER_ADC_INST);
}

/* Function to inflate and deflate cuff */
void run_activeMode(void)
{
    delay_cycles(16000000);
    setupExample();
    delay_cycles(16000000);  // Delays for ADC

    DL_GPIO_setPins(GPIO_PINS_PORT, GPIO_PINS_MOTOR_PIN);
    DL_GPIO_clearPins(GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN);

    /* Start timer to open valve aka deflate cuff */
    NVIC_EnableIRQ(CAPTURE_1_INST_INT_IRQN);
    DL_TimerG_startCounter(CAPTURE_1_INST);

    while (gDemoMode == CUFF_ACTIVE_MODE) {
        while (checkADC == false && gDemoMode == CUFF_ACTIVE_MODE) {
            __WFE();
        }
        /* Deflate cuff once pressure is high enough */
        if (deflateCuff) {
            switch (valveOpenCounter % VALVE_OPEN_MOD) {
                case OPEN_VALVE:
                    DL_ADC12_disableInterrupt(
                        ADC_INST, DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED);
                    /* Open valve */
                    DL_GPIO_clearPins(GPIO_PINS_PORT,
                        GPIO_PINS_MOTOR_PIN);  // decrease pressure
                    DL_GPIO_setPins(GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN);
                    break;
                case PULSE_OPEN_VALVE:
                    DL_GPIO_clearPins(GPIO_PINS_PORT,
                        GPIO_PINS_VALVE_PIN);  // hold pressure steady
                    break;
                case ENABLE_ADC:
                    DL_ADC12_enableInterrupt(
                        ADC_INST, DL_ADC12_INTERRUPT_MEM1_RESULT_LOADED);
                    break;
                default:
                    break;
            }
        }
    }
}

void cleanupExample(void)
{
    /* Reset variables */
    /* Valve timer */
    valveOpenCounter = 0;

    /* Flatline detection */
    deflateCuff = false;

    /* Deflate cuff completely */
    DL_GPIO_clearPins(GPIO_PINS_PORT, GPIO_PINS_MOTOR_PIN);
    DL_GPIO_setPins(GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN);

    /* Disable ADC, UART, and transmitting buffer data */
    NVIC_DisableIRQ(ADC_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ADC_INST_INT_IRQN);
    NVIC_DisableIRQ(UART_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(UART_INST_INT_IRQN);

    /* Reset variables to be used in main to tell program to start over */
    startupFlag = 1;

    DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_CPU);
}

/*
 * This IRQ is triggered whenever a new ADC conversion result is ready,
 * and the conversion results are stored in ADCResult_CuffPressure and
 * ADCResult_CuffOscPressure
 */
void ADC_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            /* PRESSURE */
            ADCResult_CuffPressure =
                DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_0);
            checkADC = true;
            if (ADCResult_CuffPressure >= MAX_CUFF_PRESSURE) {
                /* Cuff has reached max pressure */
                deflateCuff = true;
            }
            if (gDemoMode != DEFLATE && gDemoMode != CLEAR) {
                GUIComm_sendUInt16(
                    "ADC", STR_LEN_THREE, ADCResult_CuffPressure);
            }
            break;
        case DL_ADC12_IIDX_MEM1_RESULT_LOADED:
            /* OSCILLATIONS */
            ADCResult_CuffOscPressure =
                DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_1);
            /* Can add peak detection algorithm here after getting ADC value */
            checkADC = true;
            DL_ADC12_enableConversions(ADC_INST);
            if (gDemoMode != DEFLATE && gDemoMode != CLEAR) {
                GUIComm_sendUInt16(
                    "ADC2", STR_LEN_FOUR, ADCResult_CuffOscPressure);
            }
            break;
        default:
            break;
    }
}

/* Handler for open valve timing */
void CAPTURE_1_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_1_INST)) {
        case DL_TIMER_IIDX_ZERO:
            valveOpenCounter++;
            break;
        default:
            break;
    }
}
