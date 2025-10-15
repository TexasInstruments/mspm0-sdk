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

#include "idReferencing.h"

void idRefInit(ID_REF_T *pIdRef)
{
    pIdRef->idRef = 0;
    fluxWeakInit(&pIdRef->fluxWeak);
    mtpaInit(&pIdRef->mtpa);

}

void idRefLowPriorityRun(ID_REF_T *pIdRef)
{
    int32_t
    idRef,
    idRefFluxWeak,      /* Flux Weakening current reference */
    idRefMTPA;          /* MTPA current reference */

    if(pIdRef->flags.b.fluxWeakeningEnable)
    {
        fluxWeakRun(&pIdRef->fluxWeak);
        idRefFluxWeak = _IQabs(pIdRef->fluxWeak.idRef);
    }
    else
    {
        pIdRef->fluxWeak.idRef = 0;
        pIdRef->fluxWeak.piFluxWeak.kiTerm = 0;
        idRefFluxWeak = 0;
    }

    if(pIdRef->flags.b.mtpaEnable)
    {
        mtpaRun(&pIdRef->mtpa);
        idRefMTPA = _IQabs(pIdRef->mtpa.idRefFilt);
    }
    else
    {
        pIdRef->mtpa.idRef = 0;
        pIdRef->mtpa.idRefFilt = 0;
        idRefMTPA = 0;
    }

    if(idRefMTPA >= idRefFluxWeak)
    {
        idRef = idRefMTPA;
    }
    else
    {
        idRef = idRefFluxWeak;
    }
    if(pIdRef->flags.b.fluxModeEnable)
    {
        pIdRef->idRef = pIdRef->idRefFluxModeSet;
    }
    else
    {
        pIdRef->idRef = -idRef;
    }
}

