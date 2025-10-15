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

#include "speedProfile.h"
#include "appDefs.h"
#include "services.h"

uint16_t duty;
uint32_t dutyHysteresis;

int32_t speedProfile(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    /* NOTE: Changing to for loop doesn't affect memory usage */


    int32_t targetSpeedProcessed = 0, speedReferenceModeDuty = 0;

    speedReferenceModeDuty = ((uint32_t)duty << (GLOBAL_IQ - 15));

    if(_IQabs(speedReferenceModeDuty -
              pMC_App->pAppInterface->userInputs.speedInput) >=
            dutyHysteresis)
    {
        speedReferenceModeDuty =
                pMC_App->pAppInterface->userInputs.speedInput;
    }

    if((pMC_App->pAppInterface->userInputs.speedInput) >= (_IQ(1.0) - dutyHysteresis))
    {
        speedReferenceModeDuty =
                pMC_App->pAppInterface->userInputs.speedInput;
    }

    else if(pMC_App->pAppInterface->userInputs.speedInput < dutyHysteresis)
    {
        speedReferenceModeDuty = 0;
    }

    duty = (uint16_t)(speedReferenceModeDuty >> (GLOBAL_IQ - 15));

    targetSpeedProcessed = speedReferenceModeDuty;

    pMC_App->flags.b.reverseDirectionCmd = !update_DirectionStatus();

    return targetSpeedProcessed;
}
