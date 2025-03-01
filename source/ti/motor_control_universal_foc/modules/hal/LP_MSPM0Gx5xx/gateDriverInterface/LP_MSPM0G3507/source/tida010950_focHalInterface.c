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
 */
#include "main.h"
#include "appDefs.h"
#include "measure.h"
#include "focHALInterface.h"
#include "gateDriver.h"
/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#ifdef TIDA010950

void HAL_GD_ReadDCVBusVoltage(HAL_MEASURE_VOLTAGE_T *pVoltage)
{
    pVoltage->vdc = _IQ12toIQ(DL_ADC12_getMemResult(FOC_ADC_VOLT_DC_INST, ADC_VOLT_DC_IDX))>>
                                 pVoltage->voltageGain;
}

void HAL_GD_ReadCurrents(HAL_MEASURE_CURRENT_T *pCurrent)
{
       int32_t adc0Idx0,adc0Idx1,adc1Idx0,adc1Idx1;

       MC_ABC_T* iabcRaw = &pCurrent->iabcRaw;

       CURRENT_SHUNT_TYPES currentShunt = pCurrent->currentShunt;

       iabcRaw->a = DL_ADC12_getMemResult(FOC_CURR_ADC1_INST, FOC_CURR_ADC1_IDX_0);
       iabcRaw->b = DL_ADC12_getMemResult(FOC_CURR_ADC1_INST, FOC_CURR_ADC1_IDX_1);
       iabcRaw->c = DL_ADC12_getMemResult(FOC_CURR_ADC1_INST, FOC_CURR_ADC1_IDX_2);
}

void HAL_GD_ConfigureCurrentChannels(CURRENT_SHUNT_TYPES currentShunt)
{
    /* Configure the ADC channels based on the ADC pin configurations */

      HAL_setADCIdxChannel(FOC_CURR_ADC1_INST, ADC1_IDX_0, ADC1_CURRENT_U_CH);
      HAL_setADCIdxChannel(FOC_CURR_ADC1_INST, ADC1_IDX_1, ADC1_CURRENT_V_CH);
      HAL_setADCIdxChannel(FOC_CURR_ADC1_INST, ADC1_IDX_2, ADC1_CURRENT_W_CH);
}

#endif
