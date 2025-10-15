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

#include "esmo.h"

void eSMOInit(ESMO_T *pESMO)
{
    pESMO->zAlphaBeta.alpha = 0;
    pESMO->zAlphaBeta.beta = 0;
    pESMO->iAlphaBetaEstim.alpha = 0;
    pESMO->iAlphaBetaEstim.beta = 0;

}

void eSMORun(ESMO_T *pESMO)
{
    int32_t temp;
    /*  Sliding mode current observer   */
    pESMO->iAlphaBetaEstim.alpha =
            (_IQmpy_mathacl(pESMO->FSMO, pESMO->iAlphaBetaEstim.alpha)<< (GLOBAL_IQ - FSMO_IQ_SCALING)) +
            (_IQmpy_mathacl(pESMO->GSMO,(pESMO->pVAlphaBeta->alpha - pESMO->zAlphaBetaFilt.alpha- pESMO->zAlphaBeta.alpha)) << (GLOBAL_IQ - GSMO_IQ_SCALING));


    pESMO->iAlphaBetaEstim.beta =
            (_IQmpy_mathacl(pESMO->FSMO, pESMO->iAlphaBetaEstim.beta)<< (GLOBAL_IQ - FSMO_IQ_SCALING)) +
            (_IQmpy_mathacl(pESMO->GSMO,(pESMO->pVAlphaBeta->beta - pESMO->zAlphaBetaFilt.beta - pESMO->zAlphaBeta.beta))<< (GLOBAL_IQ - GSMO_IQ_SCALING));

    /* Calculate current error */
    pESMO->iAlphaBetaError.alpha =
                pESMO->iAlphaBetaEstim.alpha - pESMO->pIAlphaBeta->alpha;

    pESMO->iAlphaBetaError.beta =
                pESMO->iAlphaBetaEstim.beta - pESMO->pIAlphaBeta->beta;

    /* Maximum of Z to be Vdc */
    pESMO->zAlphaBeta.alpha =
            _IQmpy_mathacl(_IQsat((pESMO->iAlphaBetaError.alpha),_IQ(1.0),-_IQ(1.0)),
                           pESMO->kSlide);

    pESMO->zAlphaBeta.beta =
            _IQmpy_mathacl(_IQsat((pESMO->iAlphaBetaError.beta),_IQ(1.0),-_IQ(1.0)),
                           pESMO->kSlide);

    /* Sliding Control Filter Frequency calculator */
    if(pESMO->pAngleTrackingPLLEstim->velocityFilt < MIN_SMO_FILTER_FREQ)
    {
        temp = MIN_SMO_FILTER_FREQ;
    }
    else
    {
        temp = pESMO->pAngleTrackingPLLEstim->velocityFilt;
    }

    pESMO->kSlf = _IQmpy_mathacl(pESMO->filterGain.scaledNum, temp) <<
                            (GLOBAL_IQ - pESMO->filterGain.iqScaling);


    /* Filter Z to get E */
    pESMO->zAlphaBetaFilt.alpha += _IQmpy_mathacl(pESMO->kSlf, (pESMO->zAlphaBeta.alpha -  pESMO->zAlphaBetaFilt.alpha));

    pESMO->zAlphaBetaFilt.beta += _IQmpy_mathacl(pESMO->kSlf, (pESMO->zAlphaBeta.beta - pESMO->zAlphaBetaFilt.beta));

    pESMO->prevEAlphaBeta.alpha = pESMO->EAlphaBeta.alpha;
    pESMO->prevEAlphaBeta.beta = pESMO->EAlphaBeta.beta;

    pESMO->EAlphaBeta.alpha = _IQmpy_mathacl(pESMO->zAlphaBetaFilt.alpha, _IQ(1.414));
    pESMO->EAlphaBeta.beta = _IQmpy_mathacl(pESMO->zAlphaBetaFilt.beta, _IQ(1.414));

   angleTrackingPLLRun(pESMO->pAngleTrackingPLLEstim);

}
