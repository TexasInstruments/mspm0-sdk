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

#include "noMotorStallDetect.h"
#include "appDefs.h"

void noMotorStallDetectInit(NO_MOTOR_STALL_DETECT_T
                                 *pNoMotorStall)
{
    pNoMotorStall->countA = 0;
    pNoMotorStall->countB = 0;
    pNoMotorStall->countC = 0;

    pNoMotorStall->flags.b.enable = FALSE;
}
void noMotorStallDetectRun(NO_MOTOR_STALL_DETECT_T
                                *pNoMotorStall)
{
    MC_ABC_T iabc = *pNoMotorStall->pIabc;

    if(_IQabs(iabc.a) < pNoMotorStall->minimumCurrent)
    {
        pNoMotorStall->countA++;
        if(pNoMotorStall->countA >= pNoMotorStall->countMax)
        {
            pNoMotorStall->flags.b.phaseADisconnect = TRUE;
        }
    }
    else
    {
        pNoMotorStall->countA = 0;
        pNoMotorStall->flags.b.phaseADisconnect = FALSE;
    }

    if(_IQabs(iabc.b) < pNoMotorStall->minimumCurrent)
    {
        pNoMotorStall->countB++;
        if(pNoMotorStall->countB >= pNoMotorStall->countMax)
        {
            pNoMotorStall->flags.b.phaseBDisconnect = TRUE;
        }
    }
    else
    {
        pNoMotorStall->countB = 0;
        pNoMotorStall->flags.b.phaseBDisconnect = FALSE;
    }

    if(_IQabs(iabc.c) < pNoMotorStall->minimumCurrent)
    {
        pNoMotorStall->countC++;
        if(pNoMotorStall->countC >= pNoMotorStall->countMax)
        {
            pNoMotorStall->flags.b.phaseCDisconnect = TRUE;
        }
    }
    else
    {
        pNoMotorStall->countC = 0;
        pNoMotorStall->flags.b.phaseCDisconnect = FALSE;
    }

    pNoMotorStall->flags.b.status =
            pNoMotorStall->flags.b.phaseADisconnect ||
            pNoMotorStall->flags.b.phaseBDisconnect ||
            pNoMotorStall->flags.b.phaseCDisconnect;
}
