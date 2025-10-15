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

#include "lowBemfStallDetect.h"
#include "appDefs.h"
#include "measure.h"

void lowBemfStallDetectInit(LOW_BEMF_STALL_DETECT_T *pLowBemfStall)
{
    pLowBemfStall->count = 0;
    pLowBemfStall->flags.b.enable = FALSE;
    pLowBemfStall->countMax = 65535;
}

void lowBemfStallDetectRun(LOW_BEMF_STALL_DETECT_T *pLowBemfStall)
{
    int32_t
    velocity,
    velocityReference,
    lowBEMFPUValue;


    velocity = *pLowBemfStall->pVelocity;
    velocityReference = *pLowBemfStall->pVelocityReference;

    lowBEMFPUValue = _IQmpy_mathacl(*pLowBemfStall->pInvVdc, *pLowBemfStall->pEmagFiltRef)<<
            (GLOBAL_IQ - INV_VDC_IQ_SCALING);

    /* Check for persistence of low BEMF */
    if(lowBEMFPUValue < pLowBemfStall->lowBEMFThreshold)
    {
        pLowBemfStall->count++;

        if(pLowBemfStall->count >= pLowBemfStall->countMax)
        {
            pLowBemfStall->flags.b.status = TRUE;
        }
    }
    else
    {
        pLowBemfStall->count = 0;
    }

    if((velocityReference > 0 && velocity < 0) ||
            (velocityReference < 0 && velocity > 0))
    {
        pLowBemfStall->flags.b.status = TRUE;
    }
}
