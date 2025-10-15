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


/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#include "pi.h"
#include "appDefs.h"

void piInit(PI_T *pPI)
{
    pPI->out = 0;
    pPI->kiTerm = 0;
    pPI->flags.b.saturation = FALSE;
    pPI->flags.b.externalSaturation = FALSE;
}

void piRun(PI_T *pPI)
{
    int32_t kiTermUnsat, out, kpTerm, error;

    error = *(pPI->pReference) - *(pPI->pFeedback);

    kpTerm = _IQsat((_IQmpy_mathacl(error, pPI->kp)),
                    KP_TERM_SAT,
                    -KP_TERM_SAT);

    kpTerm = kpTerm << (GLOBAL_IQ - KP_IQ_SCALING);

    kiTermUnsat = pPI->kiTerm + _IQmpy_mathacl(error, pPI->ki);

    out = kpTerm + kiTermUnsat;

    if(out >= pPI->outMax)
    {
        pPI->out = pPI->outMax;

        if(error > 0)
        {
            pPI->flags.b.saturation = TRUE;
        }
        else
        {
            pPI->flags.b.saturation = FALSE;
        }
    }
    else if(out <= pPI->outMin)
    {
        pPI->out = pPI->outMin;

        if(error < 0)
        {
            pPI->flags.b.saturation = TRUE;
        }
        else
        {
            pPI->flags.b.saturation = FALSE;
        }
    }
    else
    {
        pPI->out = out;
        pPI->flags.b.saturation = FALSE;
    }

    if(!(pPI->flags.b.externalSaturation || pPI->flags.b.saturation))
    {
        pPI->kiTerm = kiTermUnsat;
    }
}

void piRun_LowPriority(PI_T *pPI)
{
    int32_t kiTermUnsat, out, kpTerm, error;

    error = *(pPI->pReference) - *(pPI->pFeedback);

    kpTerm = _IQsat((_IQmpy(error, pPI->kp)),
                    KP_TERM_SAT,
                    -KP_TERM_SAT);

    kpTerm = kpTerm << (GLOBAL_IQ - KP_IQ_SCALING);

    kiTermUnsat = pPI->kiTerm + _IQmpy(error, pPI->ki);

    out = kpTerm + kiTermUnsat;

    if(out >= pPI->outMax)
    {
        pPI->out = pPI->outMax;

        if(error > 0)
        {
            pPI->flags.b.saturation = TRUE;
        }
        else
        {
            pPI->flags.b.saturation = FALSE;
        }
    }
    else if(out <= pPI->outMin)
    {
        pPI->out = pPI->outMin;

        if(error < 0)
        {
            pPI->flags.b.saturation = TRUE;
        }
        else
        {
            pPI->flags.b.saturation = FALSE;
        }
    }
    else
    {
        pPI->out = out;
        pPI->flags.b.saturation = FALSE;
    }

    if(!(pPI->flags.b.externalSaturation || pPI->flags.b.saturation))
    {
        pPI->kiTerm = kiTermUnsat;
    }
}
