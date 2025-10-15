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

#ifdef TIDA010265

/** @brief IQ value of Current scale factor for TIDA010265 in single shunt
 * For TIDA010265 range 0<-->(3.3-offset) is scaled to 0<-->3.3
*/
#define TIDA010265_1S_CURRENT_SF_IQ  (_IQ(2))

void HAL_GD_ReadDCVBusVoltage(HAL_MEASURE_VOLTAGE_T *pVoltage)
{
    pVoltage->vdc = _IQ12toIQ(DL_ADC12_getMemResult(FOC_ADC_VOLT_DC_INST, ADC_VOLT_DC_IDX))>>
                                 pVoltage->voltageGain;
}

void HAL_GD_ReadVoltages(HAL_MEASURE_VOLTAGE_T *pVoltage)
{
    MC_ABC_T *pVabcRaw = &pVoltage->vabcRaw;
    /* Read Phase voltage raw values */
    pVabcRaw->a =
            _IQ12toIQ(DL_ADC12_getMemResult(ADC_VOLTAGE_U_INST, ADC_VOLTAGE_U_IDX)) >>
                pVoltage->voltageGain;

    pVabcRaw->b =
            _IQ12toIQ(DL_ADC12_getMemResult(ADC_VOLTAGE_V_INST, ADC_VOLTAGE_V_IDX)) >>
                pVoltage->voltageGain;

    pVabcRaw->c =
            _IQ12toIQ(DL_ADC12_getMemResult(ADC_VOLTAGE_W_INST, ADC_VOLTAGE_W_IDX)) >>
                pVoltage->voltageGain;
}

void HAL_GD_ConfigureVoltageChannels(void)
{
    /* Configure the ADC channels based on the ADC pin configurations */
    HAL_setADCIdxChannel(ADC_VOLTAGE_U_INST, ADC_VOLTAGE_U_IDX, ADC_VOLTAGE_U_CH);
    HAL_setADCIdxChannel(ADC_VOLTAGE_V_INST, ADC_VOLTAGE_V_IDX, ADC_VOLTAGE_V_CH);
    HAL_setADCIdxChannel(ADC_VOLTAGE_W_INST, ADC_VOLTAGE_W_IDX, ADC_VOLTAGE_W_CH);

}

void HAL_currentReconstruction(HAL_MEASURE_CURRENT_T *pCurrent, MC_SECTOR_TYPES sector)
{
    MC_ABC_T* iabcRaw = &pCurrent->iabcRaw;
#ifdef _INVERT_ISEN
    int32_t i1 = _IQ12toIQ((pCurrent->pOffset->i1Offset - pCurrent->iFirstSample));
    int32_t i2 =  _IQ12toIQ((pCurrent->pOffset->i2Offset - pCurrent->iSecondSample));
#elif defined _NONINVERT_ISEN
    int32_t i1 = _IQ12toIQ((pCurrent->iFirstSample - pCurrent->pOffset->i1Offset));
    int32_t i2 =  _IQ12toIQ((pCurrent->iSecondSample - pCurrent->pOffset->i2Offset));
#endif
    i1 = _IQmpy_mathacl(i1, TIDA010265_1S_CURRENT_SF_IQ);
    i2 = _IQmpy_mathacl(i2, TIDA010265_1S_CURRENT_SF_IQ);
    switch(sector)
    {
        case SECTOR_1:
            /* Max A, Min C */
            iabcRaw->a = i1;
            iabcRaw->c = -i2;
            iabcRaw->b = -(iabcRaw->a + iabcRaw->c);
            break;
        case SECTOR_2:
            /* Max B, Min C */
            iabcRaw->b = i1;
            iabcRaw->c = -i2;
            iabcRaw->a = -(iabcRaw->b + iabcRaw->c);
            break;
        case SECTOR_3:
            /* Max B, Min A */
            iabcRaw->b = i1;
            iabcRaw->a = -i2;
            iabcRaw->c = -(iabcRaw->a + iabcRaw->b);
            break;
        case SECTOR_4:
            /* Max C, Min A */
            iabcRaw->c = i1;
            iabcRaw->a = -i2;
            iabcRaw->b = -(iabcRaw->a + iabcRaw->c);
            break;
        case SECTOR_5:
            /* Max C, Min B */
            iabcRaw->c = i1;
            iabcRaw->b = -i2;
            iabcRaw->a = -(iabcRaw->b + iabcRaw->c);
            
            break;
        case SECTOR_6:
            /* Max A, Min B */
            iabcRaw->a = i1;
            iabcRaw->b = -i2;
            iabcRaw->c = -(iabcRaw->a + iabcRaw->b);
            break;
        default:
            /* Max A, Min B */
            iabcRaw->a = i1;
            iabcRaw->b = -i2;
            iabcRaw->c = -(iabcRaw->a + iabcRaw->b);
            break;
    }


}

void HAL_GD_ReadCurrents(HAL_MEASURE_CURRENT_T *pCurrent)
{
        int32_t adc0Idx0, adc1Idx0;

       MC_ABC_T* iabcRaw = &pCurrent->iabcRaw;

       CURRENT_SHUNT_TYPES currentShunt = pCurrent->currentShunt;

        adc0Idx0 = DL_ADC12_getMemResult(FOC_ADC0_INST, ADC0_IDX_0);
        adc1Idx0 = DL_ADC12_getMemResult(FOC_ADC1_INST, ADC1_IDX_0);

       switch(pCurrent->currentShunt)
        {
#ifndef __CURRENT_SINGLE_SHUNT        
            case CURRENT_TWO_SHUNT_B_C:
                iabcRaw->b = adc0Idx0;
                iabcRaw->c = adc1Idx0;
                break;
#else           
           case CURRENT_SINGLE_SHUNT:
                pCurrent->iFirstSample = adc1Idx0;
                pCurrent->iSecondSample = adc0Idx0;
                break;
#endif           
           default:
                break;
       }
}

void HAL_GD_ConfigureCurrentChannels(CURRENT_SHUNT_TYPES currentShunt)
{
    /* Configure the ADC channels based on the ADC pin configurations */
    switch(currentShunt)
    {
#ifndef __CURRENT_SINGLE_SHUNT        
       case CURRENT_TWO_SHUNT_B_C:
           HAL_setADCIdxChannel(FOC_ADC0_INST, ADC0_IDX_0, ADC0_CURRENT_V_CH);
           HAL_setADCIdxChannel(FOC_ADC1_INST, ADC1_IDX_0, ADC1_CURRENT_W_CH);
           break;
#else           
        case CURRENT_SINGLE_SHUNT:
            HAL_setADCIdxChannel(FOC_CURR_ADC1_INST, ADC1_IDX_0, ADC1_DC_CURRENT_CH);
            HAL_setADCIdxChannel(FOC_CURR_ADC0_INST, ADC0_IDX_0, ADC0_DC_CURRENT_CH);
           break;
#endif           

       default:
           break;
    }
}

#endif
