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


#include "closeLoopConfig.h"
#include "fluxWeakConfig.h"
#include "mtpaConfig.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#include "main.h"
#include "iqNum.h"

void closeLoopConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{

    CLOSE_LOOP_T *pCloseLoop = &(pMC_App->foc.closeLoop);

    pCloseLoop->velocityReferenceMax = _IQ(1.0);
    pCloseLoop->velocityReferenceMin = _IQ(-1.0);

    pCloseLoop->pTargetVelocity = &(pMC_App->targetVelocity);

    pCloseLoop->pVelocity = &(pMC_App->angleTrackingPLLEstim.velocityFilt);
    pCloseLoop->pSpeed = &(pMC_App->angleTrackingPLLEstim.speedFilt);

    pCloseLoop->pMabc = &(pMC_App->foc.mabc);

    pCloseLoop->pDabc = &(pMC_App->foc.svm.Dabc);

    pCloseLoop->pIabc = (pMC_App->foc.pIabc);

    pCloseLoop->piSpeed.pReference = &(pCloseLoop->velocityReference);
    pCloseLoop->piSpeed.pFeedback =
            &(pMC_App->angleTrackingPLLEstim.velocityFilt);

    pCloseLoop->pFluxAngle = &(pMC_App->angleTrackingPLLEstim.fluxAngle);
    
    pCloseLoop->psourceCurrentFeedbackFilt = &(pMC_App->sourceLimits.sourceCurrentEstim.sourceCurrentFilt);

    /* Configure Flux Weakening Parameters */
    fluxWeakConfig(pMC_App);

    /* Configure MTPA Parameters */
    mtpaConfig(pMC_App);
}
