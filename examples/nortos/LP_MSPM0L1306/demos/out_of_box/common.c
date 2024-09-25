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
//  common.c
//
//  This file includes common functions between the OOB examples, namely
//  ADC_INST_IRQHandler(). This is the IRQHandler for the ADC, and since the
//  light sensor and thermistor examples both use ADC0, the IRQHandler must
//  ensure that the correct action is performed for the currently enabled demo.
//*****************************************************************************

#include "ti_msp_dl_config.h"

#include "demo_mode_declaration.h"
#include "lightsensor.h"
#include "thermistor.h"

#include "callbacks_mpack.h"

/*
 * This IRQ is triggered whenever a new ADC conversion result is ready,
 * and the conversion result is stored in either gLightsensorADCResult or
 * gThermistorADCResult.
 */
void ADC_INST_IRQHandler(void)
{
    if (gDemoMode == THERMISTOR_MODE) {
        switch (DL_ADC12_getPendingInterrupt(ADC_INST)) {
            case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
                gThermistorADCResult =
                    DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_0);

                gCheckThermistor = true;

                break;
            default:
                break;
        }
    } else if (gDemoMode == LIGHTSENSOR_MODE) {
        switch (DL_ADC12_getPendingInterrupt(ADC_INST)) {
            case DL_ADC12_IIDX_MEM0_RESULT_LOADED:

                gLightsensorADCResult =
                    DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_0);

                gCheckLightsensor = true;

                break;
            default:
                break;
        }
    }
}
