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

void PARA_checkEST(uint32_t *parameter, EST_Instance *handle, PARA_IDX idx)
{
    EST_PARA EST_para;
    switch (idx)
    {
        case PARA_IDX_USER_RS:
        case PARA_IDX_USER_LS:
        case PARA_IDX_USER_PWMFREQ:
        case PARA_IDX_USER_KE:
            EST_para.Rs = parameter[PARA_IDX_USER_RS];
            EST_para.Ls = parameter[PARA_IDX_USER_LS];
            EST_para.Ts = parameter[PARA_IDX_SYS_TS];
            EST_para.ke = parameter[PARA_IDX_USER_KE];
            EST_UpdateParams(handle, &EST_para);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

void PARA_checkPISPD(uint32_t *parameter, PI_Instance *handle, PARA_IDX idx)
{
   PI_PARA PI_para;
   switch (idx)
   {
       case PARA_IDX_USER_PISPD_KP:
       case PARA_IDX_USER_PISPD_KI:
            PI_para.Kp = parameter[PARA_IDX_USER_PISPD_KP];
            PI_para.Ki = parameter[PARA_IDX_USER_PISPD_KI];
            PI_UpdateParams(handle, &PI_para);
            break;
       default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkPIIQ(uint32_t *parameter, PI_Instance *handle, PARA_IDX idx)
{
   PI_PARA PI_para;
   switch (idx)
   {
       case PARA_IDX_USER_PIIQ_KP:
       case PARA_IDX_USER_PIIQ_KI:
            PI_para.Kp = parameter[PARA_IDX_USER_PIIQ_KP];
            PI_para.Ki = parameter[PARA_IDX_USER_PIIQ_KI];
            PI_UpdateParams(handle, &PI_para);
            break;
       default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkPIID(uint32_t *parameter, PI_Instance *handle, PARA_IDX idx)
{
   PI_PARA PI_para;
   switch (idx)
   {
       case PARA_IDX_USER_PIID_KP:
       case PARA_IDX_USER_PIID_KI:
            PI_para.Kp = parameter[PARA_IDX_USER_PIID_KP];
            PI_para.Ki = parameter[PARA_IDX_USER_PIID_KI];
            PI_UpdateParams(handle, &PI_para);
            break;
        default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkRC(uint32_t *parameter, RMPCNTL_Instance *handle, PARA_IDX idx)
{
   RMPCNTL_PARA RC_para;
   switch (idx)
   {
        case PARA_IDX_USER_SPDREF_RAMP_RATE:
        case PARA_IDX_USER_MAX_FREQ:
        case PARA_IDX_USER_PWMFREQ:
            RC_para.RampDelta = parameter[PARA_IDX_USER_SPDREF_RAMP_RATE];
            RC_para.Ts = parameter[PARA_IDX_SYS_TS];
            RC_para.maxFreq = parameter[PARA_IDX_USER_MAX_FREQ];
            RMPCNTL_UpdateParams(handle, &RC_para);
            break;
        default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkRG(uint32_t *parameter, RAMPGEN_Instance *handle, PARA_IDX idx)
{
   RAMPGEN_PARA RG_para;
   switch (idx)
   {
        case PARA_IDX_USER_PWMFREQ:
            RG_para.Ts = parameter[PARA_IDX_SYS_TS];
            RAMPGEN_UpdateParams(handle, &RG_para);
            break;
        default:
            /* This is expected to be empty */
            break;
   }
}

void PARA_checkPWMGEN(uint32_t *parameter, PWMGEN_Instance *handle, 
                                                                   PARA_IDX idx)
{
   PWMGEN_PARA PWMGEN_para;
    switch (idx)
    {
        case PARA_IDX_USER_PWMFREQ:
        case PARA_IDX_USER_DEADBAND:
            PWMGEN_para.pwmPeriod = parameter[PARA_IDX_SYS_PWMPERIOD];
            PWMGEN_para.deadband = parameter[PARA_IDX_SYS_DEADBAND_CYC];
            PWMGEN_UpdateParams(handle, &PWMGEN_para);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

void PARA_checkIPD(uint32_t *parameter, IPD_Instance *handle, PARA_IDX idx)
{
   IPD_PARA IPD_para;
    switch (idx)
    {
        case PARA_IDX_USER_IPD_CURRENT_THRESHOLD:
        case PARA_IDX_USER_IPD_FREQ:
        case PARA_IDX_USER_PWMFREQ:
            IPD_para.sampleFreq = parameter[PARA_IDX_SYS_SAMPLE_FREQ];
            IPD_para.ipdFreq = parameter[PARA_IDX_USER_IPD_FREQ];
            IPD_para.ipdThr = parameter[PARA_IDX_USER_IPD_CURRENT_THRESHOLD];
            IPD_UpdateParams(handle, &IPD_para);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

