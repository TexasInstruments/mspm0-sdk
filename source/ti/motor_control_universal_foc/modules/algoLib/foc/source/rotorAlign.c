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

#ifndef FOC_SOURCE_ROTORALIGN_C_
#define FOC_SOURCE_ROTORALIGN_C_

#include "rotorAlign.h"
#include "appDefs.h"
#include "main.h"
#include "slewRate.h"

void rotorAlignInit(ROTOR_ALIGN_T *pAlign)
{
    pAlign->count = 0;

    pAlign->state = ALIGN_INIT;

    pAlign->iqRef = 0;

    pAlign->idRef = 0;

    pAlign->flags.b.status = FALSE;

    pAlign->flags.b.forceAlignStatus = FALSE;

    pAlign->flags.b.enableOnBrake = FALSE;

    pAlign->flags.b.enableOnStop = FALSE;
}

void rotorAlignRun(ROTOR_ALIGN_T *pAlign)
{
    int32_t targetVelocity = *(pAlign->pTargetVelocity);

    pAlign->flags.b.enableOnStart = !(pAlign->flags.b.enableOnBrake ||
            pAlign->flags.b.enableOnStop);

    switch(pAlign->state)
    {
    case ALIGN_INIT:

        pAlign->state = FIRST_ALIGN;

        break;

    case FIRST_ALIGN:

        if(pAlign->flags.b.enableOnStart ||
                pAlign->flags.b.enableOnBrake)
        {
            if(targetVelocity > 0)
            {
                pAlign->angle = pAlign->angleSet -
                        SECOND_ALIGN_ANGLE_OFFSET;
            }
            else
            {
                pAlign->angle = pAlign->angleSet +
                        SECOND_ALIGN_ANGLE_OFFSET;
            }
        }

        if(pAlign->count >= pAlign->countMax)
        {
                if(pAlign->flags.b.forceEnable &&
                        pAlign->flags.b.enableOnStart)
                {
                    pAlign->count = pAlign->countMax;
                    pAlign->flags.b.status = FALSE;
                    pAlign->flags.b.forceAlignStatus = TRUE;
                }
                else
                {
                    if(!pAlign->flags.b.enableOnBrake)
                    {
                        pAlign->flags.b.status = TRUE;
                    }
                }
        }

        break;
    
    default:
        /* This is intentionally kept empty */
        break;
    }

    pAlign->count++;
}

#endif /* FOC_SOURCE_ROTORALIGN_C_ */
