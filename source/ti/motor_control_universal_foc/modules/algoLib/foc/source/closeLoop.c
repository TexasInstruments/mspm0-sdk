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

#include "iqTrig.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#include "closeLoop.h"
#include "appDefs.h"
#include "measure.h"
#include "mcLib.h"
#include "main.h"
#include "focHALInterface.h"
#include "slewRate.h"

void closeLoopInit(CLOSE_LOOP_T *pCloseLoop)
{
    pCloseLoop->angleOffset = 0;

    pCloseLoop->commutationState = COMMUTATION_UNALIGNED;

    pCloseLoop->accelSpeedRefSlewRate = pCloseLoop->slowAccelSpeedRefSlewRate;
    pCloseLoop->decelSpeedRefSlewRate = pCloseLoop->slowDecelSpeedRefSlewRate;

    pCloseLoop->velocityReference = 0;

    pCloseLoop->PowerReference = 0;

    pCloseLoop->PowerFeedback = 0;
    
    pCloseLoop->flags.b.transitionEnable = FALSE;

    pCloseLoop->currentReferenceSqrMax = pCloseLoop->currentReferenceSqrMaxSet;

    piInit(&pCloseLoop->piSpeed);
    idRefInit(&pCloseLoop->idRefGen);

    pCloseLoop->idRefGen.idRef = 0;

    pCloseLoop->sourceCurrentFeedback = 0;
}

void closeLoopRun(CLOSE_LOOP_T *pCloseLoop)
{
    int32_t
    velocity,
    speedError,
    speedErrorMax,
    sinAngleOffset,
    sourceCurrentFeedback;

    velocity = *(pCloseLoop->pVelocity);

    speedError = _IQabs(pCloseLoop->velocityReference - *pCloseLoop->pVelocity);

    speedErrorMax = _IQmpy_mathacl(pCloseLoop->speedReference,
                           _IQ(SPEED_ERROR_THRESHOLD_PU));

    sourceCurrentFeedback =
            _IQmpy_mathacl(pCloseLoop->pDabc->a, pCloseLoop->pIabc->a) +
            _IQmpy_mathacl(pCloseLoop->pDabc->b, pCloseLoop->pIabc->b) +
            _IQmpy_mathacl(pCloseLoop->pDabc->c, pCloseLoop->pIabc->c);

    pCloseLoop->sourceCurrentFeedback = sourceCurrentFeedback;

    /* Close the loop slowly */
    switch(pCloseLoop->commutationState)
    {
    case COMMUTATION_UNALIGNED:

        /* Calculate id and iq required for motor based on angle offset */
        sinAngleOffset = sine(pCloseLoop->angleOffset);

        if(sinAngleOffset > 0)
        {
            /* Positive angle offset. Decrease the angle offset */
            pCloseLoop->angleOffset =
                    pCloseLoop->angleOffset - pCloseLoop->angleAlignStep;
        }
        else
        {
            /* Negative angle offset. Increase the angle offset */
            pCloseLoop->angleOffset =
                    pCloseLoop->angleOffset + pCloseLoop->angleAlignStep;
        }

        /* Commutation angle is estimated angle with offset */
        pCloseLoop->angle =
                *(pCloseLoop->pFluxAngle) + pCloseLoop->angleOffset;


        if(_IQabs(sinAngleOffset) <= TRANSITION_SIN_ANGLE_ALIGN_THRESHOLD)
        {
            /* Motor commutation close to estimator.
             * Can enter fully aligned state */
            pCloseLoop->angleOffset = 0;

            pCloseLoop->accelSpeedRefSlewRate =
                    pCloseLoop->fastAccelSpeedRefSlewRate;
            pCloseLoop->decelSpeedRefSlewRate =
                    pCloseLoop->fastDecelSpeedRefSlewRate;

            pCloseLoop->commutationState = COMMUTATION_ALIGNED;
        }

        break;

    case COMMUTATION_ALIGNED:

        /* Commutation angle is estimated angle */
        pCloseLoop->angle = *(pCloseLoop->pFluxAngle);

        break;

    default:
        pCloseLoop->commutationState = COMMUTATION_UNALIGNED;
        break;
    }

    /* D-axis current generation, current limit and Q-axis current generation */

    /* D-axis Current Reference Generation */
    if((pCloseLoop->commutationState == COMMUTATION_UNALIGNED) ||
            (pCloseLoop->commutationState == COMMUTATION_ALIGNED))
    {
         /* Slew the d-axis current reference */
         slewRate(&pCloseLoop->idqRef.d, pCloseLoop->idRefGen.idRef, _IQ(0.001));
    }


    /* Speed loop current limit */
    /* Motor in aligned state */

    /* Passive AVS Current Limit */
    if((pCloseLoop->commutationState == COMMUTATION_ALIGNED) &&
            (pCloseLoop->flags.b.avsEnable))
    {
        if(velocity > 0)
        {
            /* Motor running in positive direction.
             * Limit negative current to avoid DC bus kick-back */
            pCloseLoop->piSpeed.outMax = pCloseLoop->iqRefMax;
            pCloseLoop->piSpeed.outMin = 0;
        }
        else
        {
            /* Motor running in negative direction.
             * Limit positive current to avoid DC bus kick-back */
            pCloseLoop->piSpeed.outMax = 0;
            pCloseLoop->piSpeed.outMin = -pCloseLoop->iqRefMax;
        }
    }
    else
    {
        /* DC bus kick-back allowed */
        pCloseLoop->piSpeed.outMax = pCloseLoop->iqRefMax;
        pCloseLoop->piSpeed.outMin = -pCloseLoop->iqRefMax;
    }

    /* Execute Speed Loop */
    if((pCloseLoop->controlMode == CONTROL_POWER)||(pCloseLoop->controlMode == CONTROL_SPEED))
    {
        piRun(&(pCloseLoop->piSpeed));
        pCloseLoop->idqRef.q = pCloseLoop->piSpeed.out;
    }
}

void closeLoopLowPriorityRun(CLOSE_LOOP_T *pCloseLoop)
{
    /* Slew Rate */
    int32_t
    modIndexSqr;

    pCloseLoop->targetVelocityLimited = *(pCloseLoop->pTargetVelocity);

    /* Limit target velocity in close loop */
    if(pCloseLoop->targetVelocityLimited < pCloseLoop->velocityReferenceMin)
    {
        pCloseLoop->targetVelocityLimited = pCloseLoop->velocityReferenceMin;
    }
    else
    {
        if(pCloseLoop->targetVelocityLimited >
    pCloseLoop->velocityReferenceMax)
        {
            pCloseLoop->targetVelocityLimited =
                    pCloseLoop->velocityReferenceMax;
        }
    }

    pCloseLoop->targetSpeedLimited = _IQabs(pCloseLoop->targetVelocityLimited);
    pCloseLoop->speedReference = _IQabs(pCloseLoop->velocityReference);

    /* Target velocity and velocity reference are of same signs
     * Acceleration or deceleration will depend upon
     * speed difference */
    if(pCloseLoop->targetSpeedLimited > pCloseLoop->speedReference)
    {
        if(pCloseLoop->controlMode == CONTROL_SPEED)
        {
            if((pCloseLoop->speedChangeState == CLOSE_LOOP_STEADY_STATE) &&
                    (*pCloseLoop->pSpeed > pCloseLoop->speedReference )&&
                    ((pCloseLoop->targetSpeedLimited - pCloseLoop->speedReference) >= SPEED_CMD_THRESHOLD))
            {
                pCloseLoop->velocityReference = *pCloseLoop->pVelocity;
            }
        }

        pCloseLoop->speedChangeState = CLOSE_LOOP_ACCELERATE;
    }
    else
    {
        if(pCloseLoop->targetSpeedLimited < pCloseLoop->speedReference)
        {
            if(pCloseLoop->controlMode == CONTROL_SPEED)
            {
                if((pCloseLoop->speedChangeState == CLOSE_LOOP_STEADY_STATE) &&
                        (*pCloseLoop->pSpeed < pCloseLoop->speedReference )&&
                        ((pCloseLoop->speedReference - pCloseLoop->targetSpeedLimited) >= SPEED_CMD_THRESHOLD))
                {
                    pCloseLoop->velocityReference = *pCloseLoop->pVelocity;
                }
            }

            pCloseLoop->speedChangeState = CLOSE_LOOP_DECELERATE;
        }
        else
        {
            pCloseLoop->speedChangeState = CLOSE_LOOP_STEADY_STATE;
        }
    }


    /* Slew the Speed Reference */
    switch(pCloseLoop->speedChangeState)
    {
    case CLOSE_LOOP_STEADY_STATE:
        /* This is intentionally kept empty */
        break;

    case CLOSE_LOOP_ACCELERATE:

        slewRate(&pCloseLoop->velocityReference,
                 pCloseLoop->targetVelocityLimited,
                 pCloseLoop->accelSpeedRefSlewRate);

        if(pCloseLoop->velocityReference ==
                pCloseLoop->targetVelocityLimited)
        {
            pCloseLoop->speedChangeState = CLOSE_LOOP_STEADY_STATE;
        }

        break;

    case CLOSE_LOOP_DECELERATE:

        if(pCloseLoop->flags.b.avsEnable)
        {
            /* AVS is enabled. Ignore slew rate and Coast motor */

            if(pCloseLoop->controlMode == CONTROL_SPEED)
            {
                /* Motor is not expected to change direction */
                pCloseLoop->velocityReference =
                        pCloseLoop->targetVelocityLimited;
            }
            else
            {
                slewRate(&pCloseLoop->velocityReference,
                            pCloseLoop->targetVelocityLimited,
                            pCloseLoop->decelSpeedRefSlewRate);

                if(pCloseLoop->velocityReference ==
                        pCloseLoop->targetVelocityLimited)
                {
                    pCloseLoop->speedChangeState = CLOSE_LOOP_STEADY_STATE;
                }
            }
        }
        else
        {
            /* AVS and plugging are disabled. Apply slew rate */
            slewRate(&pCloseLoop->velocityReference,
                     pCloseLoop->targetVelocityLimited,
                     pCloseLoop->decelSpeedRefSlewRate);

            if(pCloseLoop->velocityReference ==
                    pCloseLoop->targetVelocityLimited)
            {
                pCloseLoop->speedChangeState = CLOSE_LOOP_STEADY_STATE;
            }
        }
        break;
    default:
        break;
    }

    /* Use the updated speed slew rates */
    if(pCloseLoop->commutationState == COMMUTATION_UNALIGNED)
    {
        pCloseLoop->accelSpeedRefSlewRate =
                pCloseLoop->slowAccelSpeedRefSlewRate;

        pCloseLoop->decelSpeedRefSlewRate =
                pCloseLoop->slowDecelSpeedRefSlewRate;
    }
    else
    {
        pCloseLoop->accelSpeedRefSlewRate =
                pCloseLoop->fastAccelSpeedRefSlewRate;

        pCloseLoop->decelSpeedRefSlewRate =
                pCloseLoop->fastDecelSpeedRefSlewRate;
    }

    modIndexSqr =
            _IQmpy(pCloseLoop->pMabc->a, pCloseLoop->pMabc->a) +
            _IQmpy(pCloseLoop->pMabc->b, pCloseLoop->pMabc->b) +
            _IQmpy(pCloseLoop->pMabc->c, pCloseLoop->pMabc->c);

    pCloseLoop->modIndexSqr = _IQmpy(modIndexSqr, TWO_BY_THREE);

    /* D-axis Current Reference Generation with MTPA or FW */
       if(pCloseLoop->commutationState == COMMUTATION_ALIGNED)
       {
           /* Execute Id Reference Generation */
           idRefLowPriorityRun(&pCloseLoop->idRefGen);
       }

       pCloseLoop->iqRefMax = (_IQsqrt(pCloseLoop->currentReferenceSqrMax -
        _IQmpy(pCloseLoop->idqRef.d, pCloseLoop->idqRef.d)));

}
