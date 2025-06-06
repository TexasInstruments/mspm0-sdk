/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *
 */

#include "pir_interface.h"

/* Global Config Register and its Index */
uint32_t gConfig     = 0;
uint8_t gConfigIndex = 0;

void PIR_init()
{
    /* Clear the Direct Link Pin if it was pulled high by PIR */
    DL_GPIO_enableOutput(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);
    DL_GPIO_clearPins(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);
    /* 300 us delay*/
    delay_cycles(300 * DELAY_1_US);
    DL_GPIO_disableOutput(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);
}

void PIR_sendConfig(PIR_configRegister *configRegister)
{
    gConfig = PIR_CONFIG_RESERVED_VALUE;
    gConfig |=
        ((uint32_t) configRegister->threshold << PIR_CONFIG_THRESHOLD_SHIFT) &
        PIR_CONFIG_THRESHOLD_MASK;
    gConfig |=
        ((uint16_t) configRegister->blindTime << PIR_CONFIG_BLINDTIME_SHIFT) &
        PIR_CONFIG_BLINDTIME_MASK;
    gConfig |= ((uint16_t) configRegister->pulseCounter
                   << PIR_CONFIG_PULSECOUNTER_SHIFT) &
               PIR_CONFIG_PULSECOUNTER_MASK;
    gConfig |= ((uint16_t) configRegister->windowTime
                   << PIR_CONFIG_WINDOWTIME_SHIFT) &
               PIR_CONFIG_WINDOWTIME_MASK;
    gConfig |=
        ((uint8_t) configRegister->operationMode << PIR_CONFIG_OPRMODE_SHIFT) &
        PIR_CONFIG_OPRMODE_MASK;
    gConfig |= ((uint8_t) configRegister->signalSource
                   << PIR_CONFIG_SIGNALSOURCE_SHIFT) &
               PIR_CONFIG_SIGNALSOURCE_MASK;
    gConfig |=
        ((uint8_t) configRegister->hpfCutOff << PIR_CONFIG_HPF_CUTOFF_SHIFT) &
        PIR_CONFIG_HPF_CUTOFF_MASK;
    gConfig |=
        ((uint8_t) configRegister->countMode << PIR_CONFIG_COUNTMODE_SHIFT) &
        PIR_CONFIG_COUNTMODE_MASK;

    gConfigIndex = CONFIG_REGISTER_SIZE;

    DL_Timer_startCounter(TIMER_PIR_SERIN_INST);
}

void PIR_readDirectLink(void)
{
    /* PIR setup delay */
    delay_cycles(120 * DELAY_1_US);

    gPIR_directLink.OutOfRange = 0;
    gPIR_directLink.AdcValue   = 0;
    gPIR_directLink.Config     = 0;

    for (int i = 0; i < DIRECT_LINK_FRAME_SIZE; i++) {
        DL_GPIO_enableOutput(
            GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

        DL_GPIO_clearPins(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

        delay_cycles(DELAY_1_US);

        DL_GPIO_setPins(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

        DL_GPIO_disableOutput(
            GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

        DL_GPIO_initDigitalInput(GPIO_PIR_DIRECT_LINK_IOMUX);

        delay_cycles(DELAY_1_US);

        uint8_t currBit = (DL_GPIO_readPins(GPIO_PIR_DIRECT_LINK_PORT,
                               GPIO_PIR_DIRECT_LINK_PIN) > 0)
                              ? 1
                              : 0;

        if (i == 0) {
            gPIR_directLink.OutOfRange = currBit;
        }

        else if (i > 0 && i < 15) {
            gPIR_directLink.AdcValue =
                (gPIR_directLink.AdcValue << 1) | currBit;
        }

        else if (i > 14 && i < 40) {
            gPIR_directLink.Config = (gPIR_directLink.Config << 1) | currBit;
        }
    }

    DL_GPIO_clearPins(GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);
    DL_GPIO_clearInterruptStatus(
        GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);
    DL_GPIO_enableInterrupt(
        GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);
}

void TIMER_PIR_SERIN_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_PIR_SERIN_INST)) {
        case DL_TIMER_IIDX_ZERO:
            DL_GPIO_clearPins(GPIO_PIR_PIN_SERIN_PORT, GPIO_PIR_PIN_SERIN_PIN);
            if (gConfigIndex < 0) {
                DL_Timer_stopCounter(TIMER_PIR_SERIN_INST);
                return;
            }

            DL_GPIO_setPins(GPIO_PIR_PIN_SERIN_PORT, GPIO_PIR_PIN_SERIN_PIN);

            if (((gConfig >> gConfigIndex) & 0x1) == 0) {
                DL_GPIO_clearPins(
                    GPIO_PIR_PIN_SERIN_PORT, GPIO_PIR_PIN_SERIN_PIN);
            }

            gConfigIndex--;
            break;

        default:
            break;
    }
}
