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

#include "angleTrackingPLL.h"
#include "appDefs.h"
#include "filter.h"
#include "main.h"

#define ATPLL_KP _IQ24(1.0)
#define ATPLL_KI_SF _IQ(1.0)

void angleTrackingPLLInit(AT_PLL_ESTIM_T *pATPLL)
{
    pATPLL->piATPLL.kp = ATPLL_KP;
    pATPLL->piATPLL.ki  = _IQmpy_mathacl(*pATPLL->pDeltaT,ATPLL_KI_SF);
    pATPLL->normEqReference = 0;
    pATPLL->piATPLL.pReference = &pATPLL->normEqReference;
    pATPLL->piATPLL.pFeedback = &pATPLL->normEqfeedback;
    pATPLL->piATPLL.outMax = _IQ(1.0);
    pATPLL->piATPLL.outMin = -_IQ(1.0);
    pATPLL->fluxAngle = 0;
    pATPLL->velocityFilt = 0;
    pATPLL->velocity = _IQ(0.1);
}
void angleTrackingFiltConstUpdate(AT_PLL_ESTIM_T *pATPLL)
{
    int32_t
    omegaTs , tempbemfFiltConst  ;

    omegaTs = _IQmpy_mathacl(_IQabs(pATPLL->velocity),
                        *pATPLL->pDeltaT);

    tempbemfFiltConst = _IQmpy_mathacl(omegaTs, BEMF_FILT_CONST_FACTOR);

    pATPLL->bemfFiltConst = _IQmpy_mathacl(tempbemfFiltConst, (_IQ(1.0) - tempbemfFiltConst));

}
void angleTrackingPLLRun(AT_PLL_ESTIM_T *pATPLL)
{
    int32_t
    omegaTs,
    temp,
    velTemp;


    /* Calculate Sin and Cosine of Flux Angle */
    MC_SinCos(&pATPLL->sinCosFluxAngle,
              pATPLL->bemfAngle);

    /* Calculate Ed and Eq and filter them */
    MC_ParkTransform(&pATPLL->Edq,
                     pATPLL->pEAlphaBeta,
                     &pATPLL->sinCosFluxAngle);


    angleTrackingEmagUpdate(pATPLL);

#ifndef ESMO_ESTIMATOR
    filter_mathacl(&(pATPLL->EdqFilt.d),
                   pATPLL->Edq.d,pATPLL->bemfFiltConst);
#else
    pATPLL->EdqFilt.d = pATPLL->Edq.d;
#endif
    pATPLL->normEqfeedback = -pATPLL->invEqMagNorm;

    piRun(&pATPLL->piATPLL);

    pATPLL->velocity = pATPLL->piATPLL.out + (_IQmpy_mathacl((pATPLL->EdqFilt.d), pATPLL->pMotor->InvKe.scaledNum) <<
                        (GLOBAL_IQ - pATPLL->pMotor->InvKe.iqScaling));

    pATPLL->deltaTheta =  (_IQmpy_mathacl(pATPLL->velocity,
                   *pATPLL->pDeltaT));

    pATPLL->bemfAngle += pATPLL->deltaTheta;
    pATPLL->fluxAngle = pATPLL->bemfAngle - pATPLL->phaseDiff + pATPLL->deltaTheta ; // Angle Compensation for Previous Instance BEMF Angle , Filter Delay


    angleTrackingFiltConstUpdate(pATPLL);

    /* Filter Velocity */
    filter_mathacl(&(pATPLL->velocityFilt),
           pATPLL->velocity,
           pATPLL->bemfFiltConst);

    pATPLL->speedFilt = _IQabs(pATPLL->velocityFilt);

}
