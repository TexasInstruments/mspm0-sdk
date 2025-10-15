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

#include "motorInputOutputConfig.h"
#include "iqNum.h"
#include "main.h"

void motorInputOutputConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    g_pMotorInputs = &(pMC_App->motorInputs);
    pMC_App->pAppInterface->pUserOutputs = pUserStatusRegs;

    g_pMotorInputs->current.pMaxDutyPhase = &(pMC_App->foc.svm.maxDutyPhase);
    g_pMotorInputs->current.pPrevPrevSector = &(pMC_App->foc.svm.prevPrevVoltageSector);

    pMC_App->pDabc = &(pMC_App->foc.svm.Dabc);
    pMC_App->pVoltageSector = &(pMC_App->foc.svm.voltageSector);
    pMC_App->pBlankingTime = &(pMC_App->foc.svm.blankingTime);
    pMC_App->pMinPWMdelta = &(pMC_App->foc.svm.minPWMdelta);

    g_pMotorInputs->current.pSourceCurrentFilt = &(pMC_App->sourceLimits.sourceCurrentEstim.sourceCurrentFilt);
}

