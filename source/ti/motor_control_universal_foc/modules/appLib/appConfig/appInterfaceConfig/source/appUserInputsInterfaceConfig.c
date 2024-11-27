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

#include "appInputCtrlInterface.h"
#include "appUserInputs.h"
#include "appUserInputsInterface.h"
#include "appUserInputsInterfaceConfig.h"
#include "main.h"
#include "appDefs.h"
#include "measure.h"
#include "configTables.h"
#include "faults.h"

void appUserInputsInterfaceConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    BRAKE_T *pBrake = &(pMC_App->load.brake);

    OVER_CURRENT_T *pOverCurrent = &(pMC_App->sourceLimits.overCurrent);

    uint16_t pwmFrequencyKHz;

    uint32_t adcSamplingTime0p1uS;

    float adcSamplingRatio;

    /* System setup */

    /* Set CSA gain for Motor running */
    pUserInputsInterface->phaseCurrentBase =
            pUserInputs->fullScalePhaseCurrentAmp;

    pUserInputsInterface->overCurrentBase = pUserInputsInterface->phaseCurrentBase;


    if((pMC_App->state == APP_INIT) || (pMC_App->state == APP_IDLE) ||
            (pMC_App->state == APP_FAULT))
    {
        pwmFrequencyKHz = ((int32_t)tbl1_clPWMFreqKHz[pUserInputRegs->closeLoop1.b.pwmFreqOut]);

        /* PWM and System period counts */
        pMC_App->motorInputs.pwmPeriod = (int32_t)(CPU_FREQUENCY_MHZ * 1000UL /
                                                pwmFrequencyKHz);

        /* ADC Sampling Frequency in kHz */
        if(pwmFrequencyKHz <= 16)
        {
            pMC_App->motorInputs.adcSamplingRatio = ADC_SAMPLE_EVERY_PWM;
        }
        else if(pwmFrequencyKHz <= 32)
        {
            pMC_App->motorInputs.adcSamplingRatio = ADC_SAMPLE_EVERY_2_PWM;
        }
        else if(pwmFrequencyKHz <= 48)
        {
            pMC_App->motorInputs.adcSamplingRatio = ADC_SAMPLE_EVERY_3_PWM;

        }
        else if(pwmFrequencyKHz <= 64)
        {
            pMC_App->motorInputs.adcSamplingRatio = ADC_SAMPLE_EVERY_4_PWM;

        }
        else
        {
            pMC_App->motorInputs.adcSamplingRatio = ADC_SAMPLE_EVERY_5_PWM;
        }

        if(pUserInputRegs->closeLoop1.b.highFreqFOCEn)
        {
            pMC_App->motorInputs.adcSamplingRatio <<= 1;
            pMC_App->motorInputs.adcSamplingRatio +=1;
        }

        adcSamplingRatio = (float)(1 + pMC_App->motorInputs.adcSamplingRatio);

        pUserInputsInterface->adcSamplingFrequencyKHz =
                pwmFrequencyKHz /
                    (adcSamplingRatio);

        /* ADC Sampling rate in sec */
        pUserInputsInterface->adcSamplingRate = 0.001 /
                (pUserInputsInterface->adcSamplingFrequencyKHz);
    }

    adcSamplingTime0p1uS =
            (uint32_t)(10000.0/pUserInputsInterface->adcSamplingFrequencyKHz);

    if(adcSamplingTime0p1uS >= 1000)
    {
        adcSamplingTime0p1uS = 1000;
    }

    pUserInputs->basePower =
                  pUserInputsInterface->phaseCurrentBase * DC_VOLTAGE_BASE;
                  
    /* Motor base impedance for estimator */
    pUserInputsInterface->motorImpedanceBase =
            pUserInputs->systemDCBusVoltageVolts / pUserInputsInterface->phaseCurrentBase;

    /* Inductances */
    pUserInputsInterface->L0MicroHenry =
            ((pUserInputs->LqMicroHenry) +
            (pUserInputs->LdMicroHenry)) >> 1;

    pUserInputsInterface->negL1MicroHenry =
            ((pUserInputs->LqMicroHenry) -
            (pUserInputs->LdMicroHenry)) >> 1;
}
