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
 
#include "appInterfaceConfig.h"
#include "appUserInputsConfig.h"
#include "appUserInputsInterfaceConfig.h"
#include "main.h"
#include "services.h"
#include "appDefs.h"
#include "filter.h"
#include "servicesHAL.h"

void appInterfaceConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_STATUS_INTERFACE_T *pUserOutputs = (pMC_App->pAppInterface->pUserOutputs);

    MC_DQ_T *pDdq = &(pMC_App->foc.ddq);
    
    if((pMC_App->state == APP_INIT) ||
        (pMC_App->state == APP_IDLE) ||
        (pMC_App->state == APP_BRAKE) ||
        (pMC_App->state == APP_FAULT))
    {
    }

    filter(&pUserOutputs->VdqFilt.d, pDdq->d,
           VOLTAGE_MAGNITUDE_FILTER_CONST);

    filter(&pUserOutputs->VdqFilt.q, pDdq->q,
           VOLTAGE_MAGNITUDE_FILTER_CONST);

}

void appInterfaceEEpromConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    appUserInputRegsConfig(pUserInputs);

    peri1Config();

    appUserInputsInterfaceConfig(pMC_App);
}
