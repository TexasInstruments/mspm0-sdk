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

#include "appDefs.h"
#include "measure.h"
#include "focHALInterface.h"

/**
 * @brief    Measures current offset gain
 * @param[in] pMotorInputs   Pointer to motor input
 */
static void measureCurrentOffsetGain(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

void inputsInit(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_CURRENT_T *pCurrent;

    pCurrent = &(pMotorInputs->current);

    pCurrent->iaSum = 0;
    pCurrent->ibADC0Sum = 0;
    pCurrent->ibADC1Sum = 0;
    pCurrent->icSum = 0;

    pCurrent->i1Sum = 0;
    pCurrent->i2Sum = 0;

    pCurrent->offsetMeasCount = 0;
    pCurrent->flags.b.offsetStatus = FALSE;
    pCurrent->flags.b.offsetGainStatus = FALSE;

    pCurrent->flags.b.offsetAB_CStatus = FALSE;

    pMotorInputs->current.currentShunt = CURRENT_SHUNT_CONFIG;

    pCurrent->flags.b.threeShuntDynamic =
            pCurrent->flags.b.threeShuntDynamicSet;
}

void measureCurrentOffset(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_CURRENT_T *pCurrent;

    pCurrent = &(pMotorInputs->current);

    switch(pCurrent->state)
    {
    case OFFSET_INIT:

        inputsInit(pMotorInputs);
        
        pMotorInputs->current.currentShunt = CURRENT_SHUNT_CONFIG;

        HAL_SetPhaseCurrentChannels(pMotorInputs);

        pCurrent->state = COMPUTE_OFFSET;

        break;


    case COMPUTE_OFFSET:
        measureCurrentOffsetGain(pMotorInputs);

        if(pCurrent->flags.b.offsetGainStatus)
        {
            if(pCurrent->currentShuntSet == CURRENT_THREE_SHUNT_DYNAMIC)
            {
                pCurrent->offsetGainIMaxBy1.iaOffset =
                        pCurrent->iaSum >> (HAL_CURRENT_OFFSET_MEAS_BITS + 1);

                pCurrent->offsetGainIMaxBy1.ibADC0Offset =
                        pCurrent->ibADC0Sum >> HAL_CURRENT_OFFSET_MEAS_BITS;

                pCurrent->offsetGainIMaxBy1.ibADC1Offset =
                        pCurrent->ibADC1Sum >> HAL_CURRENT_OFFSET_MEAS_BITS;

                pCurrent->offsetGainIMaxBy1.icOffset =
                        pCurrent->icSum >> (HAL_CURRENT_OFFSET_MEAS_BITS + 1);
            }
            else
            {
                pCurrent->offsetGainIMaxBy1.iaOffset =
                        pCurrent->iaSum >> (HAL_CURRENT_OFFSET_MEAS_BITS);

                pCurrent->offsetGainIMaxBy1.ibADC0Offset =
                        pCurrent->ibADC0Sum >> (HAL_CURRENT_OFFSET_MEAS_BITS);

                pCurrent->offsetGainIMaxBy1.ibADC1Offset =
                        pCurrent->ibADC1Sum >> (HAL_CURRENT_OFFSET_MEAS_BITS);

                pCurrent->offsetGainIMaxBy1.icOffset =
                        pCurrent->icSum >> (HAL_CURRENT_OFFSET_MEAS_BITS);
            }

            pCurrent->offsetGainIMaxBy1.i1Offset =
                pCurrent->i1Sum >> (HAL_CURRENT_OFFSET_MEAS_BITS);

            pCurrent->offsetGainIMaxBy1.i2Offset =
                pCurrent->i2Sum >> (HAL_CURRENT_OFFSET_MEAS_BITS);

            pCurrent->state = OFFSET_COMPLETE;
        }
        break;

    case OFFSET_COMPLETE:
        pCurrent->flags.b.offsetStatus = TRUE;

        break;

    }
}

void measureCurrentOffsetGain(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_CURRENT_T *pCurrent = &(pMotorInputs->current);
    
#ifndef __CURRENT_SINGLE_SHUNT
    if(pCurrent->currentShuntSet == CURRENT_THREE_SHUNT_DYNAMIC)
    {
        if(!pCurrent->flags.b.offsetAB_CStatus)
        {
            pCurrent->offsetMeasCount++;

            if(pCurrent->offsetMeasCount > HAL_CURRENT_OFFSET_MEAS_COUNT)
            {
                pCurrent->offsetMeasCount = 0;

                pCurrent->currentShunt = CURRENT_THREE_SHUNT_A_BC;

                HAL_SetPhaseCurrentChannels(pMotorInputs);

                pCurrent->flags.b.offsetAB_CStatus = TRUE;
            }
            else
            {
                pCurrent->iaSum += pCurrent->iabcRaw.a;
                pCurrent->ibADC0Sum += pCurrent->iabcRaw.b;
                pCurrent->icSum += pCurrent->iabcRaw.c;
            }
        }
        else
        {
            pCurrent->offsetMeasCount++;

            if(pCurrent->offsetMeasCount > HAL_CURRENT_OFFSET_MEAS_COUNT)
            {
                pCurrent->currentShunt = CURRENT_THREE_SHUNT_AB_C;

                HAL_SetPhaseCurrentChannels(pMotorInputs);

                pCurrent->flags.b.offsetGainStatus = TRUE;
            }
            else
            {
                pCurrent->iaSum += pCurrent->iabcRaw.a;
                pCurrent->ibADC1Sum += pCurrent->iabcRaw.b;
                pCurrent->icSum += pCurrent->iabcRaw.c;
            }
        }
    }
    else
    {
        pCurrent->offsetMeasCount++;
        if(pCurrent->offsetMeasCount > HAL_CURRENT_OFFSET_MEAS_COUNT)
        {
            pCurrent->flags.b.offsetGainStatus = TRUE;
        }
        else
        {
            pCurrent->iaSum += pCurrent->iabcRaw.a;
            if(pCurrent->currentShunt == CURRENT_TWO_SHUNT_A_B || pCurrent->currentShunt == CURRENT_THREE_SHUNT_A_BC)
            {
                pCurrent->ibADC1Sum += pCurrent->iabcRaw.b;
            }
            else if (pCurrent->currentShunt == CURRENT_TWO_SHUNT_B_C || pCurrent->currentShunt == CURRENT_THREE_SHUNT_AB_C)
            {
                pCurrent->ibADC0Sum += pCurrent->iabcRaw.b;
            }
            pCurrent->icSum += pCurrent->iabcRaw.c;
        }
    }
#endif

#ifdef __CURRENT_SINGLE_SHUNT
if(pCurrent->currentShunt == CURRENT_SINGLE_SHUNT)
    {
        pCurrent->offsetMeasCount++;
        if(pCurrent->offsetMeasCount > HAL_CURRENT_OFFSET_MEAS_COUNT)
        {
            pCurrent->flags.b.offsetGainStatus = TRUE;
        }
        else
        {
            pCurrent->i1Sum += pCurrent->iFirstSample;
            pCurrent->i2Sum += pCurrent->iSecondSample;
        }
    }
#endif

}
