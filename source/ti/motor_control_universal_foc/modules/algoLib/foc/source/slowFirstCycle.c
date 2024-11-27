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

#include "slowFirstCycle.h"
#include "appDefs.h"

#define FULL_CYCLE _IQ(1.0)

void slowFirstCycleInit(SLOW_FIRST_CYCLE_T *pSlowFirstCycle)
{
    pSlowFirstCycle->flags.b.status = FALSE;
    pSlowFirstCycle->flags.b.inProgress = FALSE;
}

void slowFirstCycleRun(SLOW_FIRST_CYCLE_T *pSlowFirstCycle)
{
    int32_t
    targetVelocity, iqRefSetLimited;

    targetVelocity = *pSlowFirstCycle->pTargetVelocity;

    pSlowFirstCycle->flags.b.inProgress = TRUE;

    iqRefSetLimited = pSlowFirstCycle->iqRefSet;

    /* Set speed in slow first cycle based on direction */
    if(targetVelocity > 0)
    {
        pSlowFirstCycle->velocityReference = pSlowFirstCycle->speedReference;
        pSlowFirstCycle->iqRefMax = iqRefSetLimited;
    }
    else
    {
        pSlowFirstCycle->velocityReference = -pSlowFirstCycle->speedReference;
        pSlowFirstCycle->iqRefMax = -iqRefSetLimited;
    }

    if(_IQabs(*pSlowFirstCycle->pAngle) >= FULL_CYCLE)
    {
        if(!pSlowFirstCycle->flags.b.forceEnable)
        {
            pSlowFirstCycle->flags.b.status = TRUE;
            pSlowFirstCycle->flags.b.inProgress = FALSE;
        }
    }

}
