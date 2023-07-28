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
#include "parameter.h"

void PARA_checkESMO(float *parameter, ESMO_Instance *handle, PARA_IDX idx)
{
    ESMO_PARA ESMO_para;
    switch (idx)
    {
        case PARA_IDX_RS:
        case PARA_IDX_LS:
        case PARA_IDX_PWMFREQ:
        case PARA_IDX_CNTRLDIV:
        case PARA_IDX_VOLT_RATIO:
        case PARA_IDX_AMP_GAIN:
        case PARA_IDX_RSHUNT:
        case PARA_IDX_KSLIDE:
        case PARA_IDX_BASE_RPM:
        case PARA_IDX_POLES:
            ESMO_para.Rs = parameter[PARA_IDX_RS];
            ESMO_para.Ls = parameter[PARA_IDX_LS];
            ESMO_para.Ts = (parameter[PARA_IDX_CNTRLDIV]/
                                                   parameter[PARA_IDX_PWMFREQ]);
            ESMO_para.baseSF = (2 * parameter[PARA_IDX_VOLT_RATIO] * 
                     parameter[PARA_IDX_AMP_GAIN] * parameter[PARA_IDX_RSHUNT]);
            ESMO_para.kslide = parameter[PARA_IDX_KSLIDE];
            ESMO_para.baseFreq = MOTOR_getBaseFreq(parameter[PARA_IDX_BASE_RPM],
                                                     parameter[PARA_IDX_POLES]);
            ESMO_UpdateParams(handle, &ESMO_para);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

void PARA_checkSPDEST(float *parameter, SPDEST_Instance *handle, PARA_IDX idx)
{
    SPDEST_PARA SPDEST_para;
    switch (idx)
    {
        case PARA_IDX_BASE_RPM:
        case PARA_IDX_POLES:
        case PARA_IDX_PWMFREQ:
        case PARA_IDX_CNTRLDIV:
            SPDEST_para.baseFreq = MOTOR_getBaseFreq(
                       parameter[PARA_IDX_BASE_RPM], parameter[PARA_IDX_POLES]);
            SPDEST_para.Ts = (parameter[PARA_IDX_CNTRLDIV]/
                                                   parameter[PARA_IDX_PWMFREQ]);
            SPDEST_para.poles = parameter[PARA_IDX_POLES];
            SPDEST_para.baseRpm = parameter[PARA_IDX_BASE_RPM];
            SPDEST_UpdateParams(handle, &SPDEST_para);
            break;
        default:
             /* This is expected to be empty */
             break;
    }
}

void PARA_checkPISPD(float *parameter, PI_Instance *handle, PARA_IDX idx)
{
   PI_PARA PI_para;
   switch (idx)
   {
       case PARA_IDX_PISPD_KP:
       case PARA_IDX_PISPD_KI:
       case PARA_IDX_PISPD_MAX:
       case PARA_IDX_PISPD_MIN:
            PI_para.Kp = parameter[PARA_IDX_PISPD_KP];
            PI_para.Ki = parameter[PARA_IDX_PISPD_KI];
            PI_para.Max = parameter[PARA_IDX_PISPD_MAX];
            PI_para.Min = parameter[PARA_IDX_PISPD_MIN];
            PI_UpdateParams(handle, &PI_para);
            break;
       default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkPIIQ(float *parameter, PI_Instance *handle, PARA_IDX idx)
{
   PI_PARA PI_para;
   switch (idx)
   {
       case PARA_IDX_PIIQ_KP:
       case PARA_IDX_PIIQ_KI:
       case PARA_IDX_PIIQ_MAX:
       case PARA_IDX_PIIQ_MIN:
            PI_para.Kp = parameter[PARA_IDX_PIIQ_KP];
            PI_para.Ki = parameter[PARA_IDX_PIIQ_KI];
            PI_para.Max = parameter[PARA_IDX_PIIQ_MAX];
            PI_para.Min = parameter[PARA_IDX_PIIQ_MIN];
            PI_UpdateParams(handle, &PI_para);
            break;
       default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkPIID(float *parameter, PI_Instance *handle, PARA_IDX idx)
{
   PI_PARA PI_para;
   switch (idx)
   {
       case PARA_IDX_PIID_KP:
       case PARA_IDX_PIID_KI:
       case PARA_IDX_PIID_MAX:
       case PARA_IDX_PIID_MIN:
            PI_para.Kp = parameter[PARA_IDX_PIID_KP];
            PI_para.Ki = parameter[PARA_IDX_PIID_KI];
            PI_para.Max = parameter[PARA_IDX_PIID_MAX];
            PI_para.Min = parameter[PARA_IDX_PIID_MIN];
            PI_UpdateParams(handle, &PI_para);
            break;
        default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkRG(float *parameter, RAMPGEN_Instance *handle, PARA_IDX idx)
{
   RAMPGEN_PARA RG_para;
   switch (idx)
   {
        case PARA_IDX_PWMFREQ:
        case PARA_IDX_CNTRLDIV:
        case PARA_IDX_BASE_RPM:
        case PARA_IDX_POLES:
            RG_para.Ts = (parameter[PARA_IDX_CNTRLDIV]/
                                                   parameter[PARA_IDX_PWMFREQ]);
            RG_para.baseFreq = MOTOR_getBaseFreq(parameter[PARA_IDX_BASE_RPM],
                                                     parameter[PARA_IDX_POLES]);
            RAMPGEN_UpdateParams(handle, &RG_para);
            break;
        default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkPWMGEN(float *parameter, PWMGEN_Instance *handle, PARA_IDX idx)
{
   PWMGEN_PARA PWMGEN_para;
    switch (idx)
    {
        case PARA_IDX_PWMFREQ:
        case PARA_IDX_DEADBAND:
        case PARA_IDX_PWMADCSAMPLE:
            PWMGEN_para.pwmPeriod = parameter[PARA_IDX_PWMPERIOD];
            PWMGEN_para.adcSampleCycles = parameter[PARA_IDX_PWMADCSAMPLE];
            PWMGEN_para.deadband = parameter[PARA_IDX_DEADBAND_CYC];
            PWMGEN_UpdateParams(handle, &PWMGEN_para);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

