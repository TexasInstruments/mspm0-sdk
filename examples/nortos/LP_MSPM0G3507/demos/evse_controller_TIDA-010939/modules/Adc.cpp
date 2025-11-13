/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 *
 * Adc.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#include "Adc.h"
#include <cstring>

#include "includes/EVConfig.h"

Adc::Adc(ADC12_Regs *_adc) : adc(_adc)
{
    evseCPHiIdx = 0;
    evseCPLoIdx = 0;

    evsePP     = 0;
    evseDP1    = 0;
    plugLockFB = 0;
}

void Adc::getEvseCPHi(_iq13 *out)
{
    int i = 0;
    uint32_t o[AVG];

    memcpy(o, evseCPHi, AVG * sizeof(uint32_t));

    //Pre-div voltage = ADC code * 0.00655 - 12.4
    for (i = 0; i < AVG; i++) {
        out[i] =
            (_IQ18mpy(_IQ18(0.006554555), _IQ18(o[i])) - _IQ18(12.40677966)) >>
            5;
    }
}

void Adc::getEvseCPLo(_iq13 *out)
{
    int i = 0;
    uint32_t o[AVG];

    memcpy(o, evseCPLo, AVG * sizeof(uint32_t));

    //Pre-div voltage = ADC code * 0.00655 - 12.4
    for (i = 0; i < AVG; i++) {
        out[i] =
            (_IQ18mpy(_IQ18(0.006554555), _IQ18(o[i])) - _IQ18(12.40677966)) >>
            5;
    }
}

uint16_t Adc::getEvsePP()
{
    return evsePP;
    //return _IQ13mpy(_IQ13div(_IQ13(v), _IQ13(4095)), _IQ13(3.3)) >> 13;
}

uint16_t Adc::getEvseDP1()
{
    return evseDP1;
    //return _IQ13mpy(_IQ13div(_IQ13(v), _IQ13(4095)), _IQ13(3.3)) >> 13;
}

uint16_t Adc::getPlugLockFB()
{
    return plugLockFB;
    //return _IQ13mpy(_IQ13div(_IQ13(v), _IQ13(4095)), _IQ13(3.3)) >> 13;
}

/* Take a sample of the high portion of the feedback signal */
void Adc::triggerEvseCPHiISR()
{
    evseCPSampleTarget = 2;
    DL_ADC12_startConversion(adc);
}

/* Take a sample of the low portion of the feedback signal */
void Adc::triggerEvseCPLoISR()
{
    evseCPSampleTarget = 1;
    DL_ADC12_startConversion(adc);
}

void Adc::HAL_ADC_ConvCpltCallback(ADC12_Regs *hadc)
{
    if (hadc == adc) {
        if (evseCPSampleTarget == 1) {
            evseCPLo[evseCPLoIdx] =
                DL_ADC12_getMemResult(ADC0_INST, DL_ADC12_MEM_IDX_0);
            if (++evseCPLoIdx >= AVG) {
                evseCPLoIdx = 0;
            }
        } else if (evseCPSampleTarget == 2) {
            evseCPHi[evseCPHiIdx] =
                DL_ADC12_getMemResult(ADC0_INST, DL_ADC12_MEM_IDX_0);
            if (++evseCPHiIdx >= AVG) {
                evseCPHiIdx = 0;
            }
        }
        evsePP     = DL_ADC12_getMemResult(ADC0_INST, DL_ADC12_MEM_IDX_1);
        evseDP1    = DL_ADC12_getMemResult(ADC0_INST, DL_ADC12_MEM_IDX_2);
        plugLockFB = DL_ADC12_getMemResult(ADC0_INST, DL_ADC12_MEM_IDX_3);
        evseCPSampleTarget = 0;
    }
}
