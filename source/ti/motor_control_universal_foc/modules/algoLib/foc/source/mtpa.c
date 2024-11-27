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

#include "mtpa.h"
#include "filter.h"
/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

void mtpaInit(MTPA_T *pMtpa)
{
    pMtpa->idRefFilt = 0;
}

void mtpaRun(MTPA_T *pMtpa)
{
    int32_t
    FourL1InvKe,
    FourL1InvKeIq,
    sqrtTerm,
    numerator,
    tempSqrt;

    FourL1InvKe = _IQmpy(pMtpa->L1.scaledNum, *pMtpa->pInvKe) <<
            (2 + GLOBAL_IQ + GLOBAL_IQ -
                    (pMtpa->L1.iqScaling + *pMtpa->pInvKeScaling));

    FourL1InvKeIq = _IQmpy(FourL1InvKe, *pMtpa->pIq);

    tempSqrt = _IQ(1.0) + _IQmpy(FourL1InvKeIq, FourL1InvKeIq);

    sqrtTerm = _IQsqrt(tempSqrt);

    numerator = _IQ(1.0) - sqrtTerm;

    if(FourL1InvKe < _IQabs((numerator >> 3)))
    {
        pMtpa->idRef = -_IQ(8.0);
    }
    else
    {
    /* Take the current so that maximum torque for
     * given current is generated */
        pMtpa->idRef = _IQdiv(numerator,FourL1InvKe);
        if(FourL1InvKe == 0)
        {
            pMtpa->idRef = 0;
        }
    }

    filter(&pMtpa->idRefFilt, pMtpa->idRef,
           pMtpa->idFiltConst);

    /* Apply limits on current reference */
    if(pMtpa->idRefFilt >= 0)
    {
        pMtpa->idRefFilt = 0;
    }
    else if(pMtpa->idRefFilt <= - *pMtpa->pidRefMin)
    {
        pMtpa->idRefFilt = - *pMtpa->pidRefMin;
    }
}
