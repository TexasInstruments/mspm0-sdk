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

#include "loadConfig.h"
#include "isdConfig.h"
#include "ipdConfig.h"
#include "softStopConfig.h"
#include "brakeConfig.h"

void loadConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    LOAD_T *pLoad = &(pMC_App->load);

    /* Configure Load Inputs */
    pLoad->pMotorInputs = &(pMC_App->motorInputs);
    pLoad->pPWMPeriod = &(pMC_App->motorInputs.pwmPeriod);
    pLoad->pPropagationDelay = &(pMC_App->motorInputs.propagationDelay);
    pLoad->pADCSamplingRatio = &(pMC_App->motorInputs.adcSamplingRatio);

    pLoad->pSlowFirstCycleFlags =
                    &(pMC_App->foc.openLoop.slowFirstCycle.flags);

    pLoad->pVoltage = &(pMC_App->motorInputs.voltage);

    pLoad->pRotorAlignFlags = &(pMC_App->foc.rotorAlign.flags);

    /* Configure ISD */
    isdConfig(pMC_App);

    /* Configure the soft stop */
    softStopConfig(pMC_App);

    /* Configure Brake */
    brakeConfig(pMC_App);

    /* Configure IPD */
    ipdConfig(pMC_App);

}
