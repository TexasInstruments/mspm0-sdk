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

#include "isd.h"

#include "appDefs.h"
#include "focHALInterface.h"
#include "main.h"

void isdInit(ISD_T *pIsd)
{
    pIsd->flags.b.status = FALSE;
    pIsd->direction = DIRECTION_UNIDENTIFIED;
    pIsd->motorRunCount = 0;
    pIsd->motorStopCount = 0;
    pIsd->motorSpeedPositive = 0;
    pIsd->motorSpeedNegative = 0;
}

void isdRun(ISD_T *pIsd)
{
    /* Process BEMF */
    MC_ClarkeTransform(&pIsd->eAlphaBeta, pIsd->pEabc);

    /* Calculate speed of motor */
    angleTrackingPLLRun(pIsd->pAngleTrackingPLLEstim);

    switch(pIsd->state)
    {
    case ISD_INIT:
        isdInit(pIsd);
        pIsd->state = ISD_MOTOR_STOP_CHECK;

        break;

    case ISD_MOTOR_STOP_CHECK:

        /* Check if motor has definitely stopped */
        if(pIsd->bemfMag < pIsd->maxStopBemf)
        {
            pIsd->motorStopCount++;

            /* Check for persistence of stop */
            if(pIsd->motorStopCount >= pIsd->motorStopCountMax)
            {
                pIsd->motorRunCount = 0;

                pIsd->direction = DIRECTION_STOP;
                pIsd->state = ISD_COMPLETE;
            }
        }
        else
        {
            pIsd->state = ISD_MOTOR_DIRECTION_CHECK;
        }

        break;

    case ISD_MOTOR_DIRECTION_CHECK:

            pIsd->motorRunCount++;

            if(pIsd->pAngleTrackingPLLEstim->velocity > 0)
            {
                pIsd->motorSpeedPositive++;
            }
            else if(pIsd->pAngleTrackingPLLEstim->velocity < 0)
            {
                pIsd->motorSpeedNegative++;
            }

            if(pIsd->motorRunCount > pIsd->motorRunCountMax)
            {
                if(_IQabs(pIsd->motorSpeedPositive - pIsd->motorSpeedNegative) < (pIsd->motorRunCountMax >> 4))
                {
                    pIsd->direction = DIRECTION_UNIDENTIFIED;
                }
                else if((pIsd->motorSpeedPositive > pIsd->motorSpeedNegative) && (*pIsd->pTargetVelocity > 0))
                {
                    pIsd->direction = DIRECTION_FORWARD;
                }

                else if((pIsd->motorSpeedPositive < pIsd->motorSpeedNegative) && (*pIsd->pTargetVelocity < 0))
                {
                    pIsd->direction = DIRECTION_REVERSE;
                }
                else
                {
                    pIsd->direction = DIRECTION_UNINTENDED; // If Direction Reversal is Commanded.
                }
                pIsd->state = ISD_COMPLETE;
            }

        break;


    case ISD_RUN_MOTOR_CHECK:
        /* This is intentionally kept empty */
        break;


    case ISD_COMPLETE:
        if(!pIsd->flags.b.forceEnable)
        {
            pIsd->flags.b.status = TRUE;
        }
        break;

    case ISD_FAULT:
        /* This is intentionally kept empty */
        break;

    default:
        pIsd->state = ISD_INIT;
        break;

    }
}
