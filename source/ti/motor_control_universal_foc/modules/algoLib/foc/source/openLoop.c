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

#include "openLoop.h"
#include "appDefs.h"
#include "main.h"
#include "slewRate.h"

#define HALF_CYCLE  _IQ(0.5)

static void openLoopAccelProfileSpeedReferencing(OPEN_LOOP_T *);
static void openLoopCurrentReferenceSet(OPEN_LOOP_T *);
static void openLoopSlewRateRun(OPEN_LOOP_T *);

void openLoopInit(OPEN_LOOP_T *pOpenLoop)
{
    pOpenLoop->velocityReference = 0;

    pOpenLoop->prevVelocityReference = 0;

    pOpenLoop->loopExecutionCount = 0;

    pOpenLoop->velocityReferenceScaled = 0;

    pOpenLoop->idRef = 0;

    pOpenLoop->flags.b.status = FALSE;

    pOpenLoop->transitionToOpenLoop = OPEN_LOOP_TRANSITION_AT_MOTOR_START;

    pOpenLoop->speedReferenceMax = pOpenLoop->speedReferenceMaxForwardSet;

    slowFirstCycleInit(&pOpenLoop->slowFirstCycle);
}

void openLoopRun(OPEN_LOOP_T *pOpenLoop)
{
    int32_t
    targetVelocity;

    targetVelocity = *pOpenLoop->pTargetVelocity;

    /* Calculate slewed speed reference */
    openLoopSlewRateRun(pOpenLoop);

    /* Calculate slewed D-axis current reference */
    slewRate(&pOpenLoop->idRef, 0, pOpenLoop->idRefSlewRate);

    if((pOpenLoop->transitionToOpenLoop ==
            OPEN_LOOP_TRANSITION_AT_MOTOR_START &&
            pOpenLoop->speedReferenceState == OPEN_LOOP_ACCELERATING_PROFILE)
            ||
            (pOpenLoop->transitionToOpenLoop !=
                    OPEN_LOOP_TRANSITION_AT_MOTOR_START))
    {
        /* If motor has started from stationary and is supposed to follow
         * acceleration profile
         * OR
         * if motor has not entered open loop from stationary:
         * Set current reference properly */

        openLoopCurrentReferenceSet(pOpenLoop);
    }

    /* Calculate the angle */
    pOpenLoop->angle +=
            _IQmpy_mathacl(pOpenLoop->velocityReference, *pOpenLoop->pDeltaT);

    /* Decide to enable transition */
    if((targetVelocity > 0 && pOpenLoop->velocityReference >= 0) ||
            (targetVelocity < 0 && pOpenLoop->velocityReference <= 0))
    {
        pOpenLoop->flags.b.transitionEnable = TRUE;
            pOpenLoop->speedReferenceMax =
                    pOpenLoop->speedReferenceMaxForwardSet;
    }

    if(pOpenLoop->flags.b.transitionEnable)
    {
        /* Look for algorithms to enter closed loop earlier */

        /* Decide to go to close loop */
        if((_IQabs(pOpenLoop->velocityReference) >=
                pOpenLoop->speedReferenceMax))
        {
            pOpenLoop->speedReferenceMax = _IQabs(pOpenLoop->velocityReference);


            if(pOpenLoop->flags.b.forceEnable ||
                    pOpenLoop->slowFirstCycle.flags.b.forceEnable)
            {
                pOpenLoop->flags.b.status = FALSE;
            }
            else
            {
                pOpenLoop->flags.b.status = TRUE;
            }

        }
    }

    /* Update Previous velocity reference */
    pOpenLoop->prevVelocityReference = pOpenLoop->velocityReference;
}

static void openLoopAccelProfileSpeedReferencing(OPEN_LOOP_T *pOpenLoop)
{
    int32_t
    targetVelocity,
    velocityReferenceMax,
    linearTerm,
    quadraticTerm;

    targetVelocity = *pOpenLoop->pTargetVelocity;

    /* Set maximum speed in open loop based on direction */
    if(targetVelocity > 0)
    {
        velocityReferenceMax = pOpenLoop->speedReferenceMax;
    }
    else
    {
        if(targetVelocity < 0)
        {
            velocityReferenceMax = -pOpenLoop->speedReferenceMax;
        }
        else
        {
            velocityReferenceMax = 0;
        }
    }

    /* Speed Reference slew rate */
    quadraticTerm = (_IQmpy_mathacl(pOpenLoop->loopExecutionCount,
                            pOpenLoop->pAccelRefSlewRate->scaledNum) <<
            (GLOBAL_IQ - pOpenLoop->pAccelRefSlewRate->iqScaling));

    linearTerm = (pOpenLoop->pSpeedRefSlewRate->scaledNum <<
            (GLOBAL_IQ - pOpenLoop->pSpeedRefSlewRate->iqScaling));

    if(quadraticTerm < 0 && linearTerm >= 0)
    {
        quadraticTerm = _IQ(1.0);
    }
    else if(quadraticTerm >= 0 && linearTerm < 0)
    {
        linearTerm = _IQ(1.0);
    }
    else if(quadraticTerm <= 0 && linearTerm < 0)
    {
        quadraticTerm = 0;
        linearTerm = _IQ(1.0);
    }
    else
    {
        /* This is intentionally kept empty */
    }

    if(pOpenLoop->velocityReference < velocityReferenceMax)
    {
        /* Velocity less than max velocity. Need to increase velocity */
        if(pOpenLoop->flags.b.transitionEnable)
        {
            /* Open Loop Acceleration in positive direction */
            pOpenLoop->loopExecutionCount++;
        }
        else
        {
            /* Open Loop Deceleration in negative direction */
            pOpenLoop->loopExecutionCount--;

            if(pOpenLoop->loopExecutionCount < 0)
            {
                pOpenLoop->loopExecutionCount = 0;
            }
        }
        pOpenLoop->velocityReferenceScaled += (linearTerm + quadraticTerm);
    }
    else if(pOpenLoop->velocityReference > velocityReferenceMax)
    {
        /* Velocity more than max velocity. Need to decrease velocity */
        if(pOpenLoop->flags.b.transitionEnable)
        {
            /* Open Loop Acceleration in negative direction */
            pOpenLoop->loopExecutionCount++;
        }
        else
        {
            /* Open Loop Deceleration in positive direction */
            pOpenLoop->loopExecutionCount--;

            if(pOpenLoop->loopExecutionCount < 0)
            {
                pOpenLoop->loopExecutionCount = 0;
            }
        }
        pOpenLoop->velocityReferenceScaled -= (linearTerm + quadraticTerm);
    }
    else
    {
        /* This is intentionally kept empty */
    }

    pOpenLoop->velocityReference =
            _IQmpy_mathacl(pOpenLoop->velocityReferenceScaled,
                   pOpenLoop->speedReferenceMax);

    /* Remove Chattering in reference due to slew rate granularity */
    if(_IQabs(pOpenLoop->velocityReference - velocityReferenceMax) <
            _IQmpy_mathacl((linearTerm + quadraticTerm), pOpenLoop->speedReferenceMax))
    {
        pOpenLoop->velocityReference = velocityReferenceMax;
    }

}

static void openLoopCurrentReferenceSet(OPEN_LOOP_T *pOpenLoop)
{
    int32_t
    targetVelocity,
    iqRefSetLimited;

    targetVelocity = *(pOpenLoop->pTargetVelocity);

    iqRefSetLimited = pOpenLoop->iqRefSet;

    /* Slew rate for current reference */
    slewRate(&pOpenLoop->iqRef, pOpenLoop->iqRefMax, pOpenLoop->iqRefSlewRate);

    /* Set current reference */
    if(pOpenLoop->transitionToOpenLoop == OPEN_LOOP_TRANSITION_AT_MOTOR_START ||
            pOpenLoop->transitionToOpenLoop == OPEN_LOOP_TRANSITION_IN_MOTORING)
    {
        /*  Motor has entered open loop while motoring */

        /* Set Q-axis current reference based on direction for motoring.
         * The sign of current reference should change at zero crossing
         * of speed */

        if(pOpenLoop->transitionToOpenLoop ==
                OPEN_LOOP_TRANSITION_AT_MOTOR_START)
        {
            /* Current slew rate to be applied only at startup */
            if(pOpenLoop->velocityReference > 0)
            {
                pOpenLoop->iqRefMax = iqRefSetLimited;
            }
            else
            {
                pOpenLoop->iqRefMax = -iqRefSetLimited;
            }
        }
        else
        {
            if(pOpenLoop->velocityReference > 0)
            {
                pOpenLoop->iqRef = iqRefSetLimited;
            }
            else
            {
                pOpenLoop->iqRef = -iqRefSetLimited;
            }
        }

        if((pOpenLoop->velocityReference >= 0 &&
                pOpenLoop->prevVelocityReference < 0) ||
                (pOpenLoop->velocityReference <= 0 &&
                        pOpenLoop->prevVelocityReference > 0))
        {
            /* The current feedback is of incorrect sign w.r.t. reference
             * if it is intended to go to close loop.
             * Hence, need to flip the commutation angle at 0 speed */
            pOpenLoop->flags.b.velocityZeroCross = TRUE;
            pOpenLoop->angle += HALF_CYCLE;
            pOpenLoop->loopExecutionCount = 0;

            pOpenLoop->iqRef = -pOpenLoop->iqRef;

            /* Change from deceleration to acceleration. But since it is in
             * reverse drive, the acceleration should be slow */
            pOpenLoop->pSpeedRefSlewRate =
                    &(pOpenLoop->speedRefSlewRateSlowAccel);

            pOpenLoop->pAccelRefSlewRate =
                    &(pOpenLoop->accelRefSlewRateSlowAccel);
        }
        else
        {
            pOpenLoop->flags.b.velocityZeroCross = FALSE;
        }

    }
}

static void openLoopSlewRateRun(OPEN_LOOP_T *pOpenLoop)
{
    if(pOpenLoop->transitionToOpenLoop == OPEN_LOOP_TRANSITION_AT_MOTOR_START)
    {
        /* Motor has entered open loop blindly. Hence, need to enter slow
         * first cycle if commanded */

        switch(pOpenLoop->speedReferenceState)
        {
        case OPEN_LOOP_SLOW_FIRST_CYCLE_PROFILE:
            slowFirstCycleRun(&pOpenLoop->slowFirstCycle);

            pOpenLoop->iqRefMax = pOpenLoop->slowFirstCycle.iqRefMax;
            pOpenLoop->velocityReference =
                    pOpenLoop->slowFirstCycle.velocityReference;

            /* Slew rate for current reference */
            slewRate(&pOpenLoop->iqRef, pOpenLoop->iqRefMax,
                     pOpenLoop->iqRefSlewRate);

            if(getSlowFirstCycleStatus(&pOpenLoop->slowFirstCycle))
            {
                pOpenLoop->velocityReferenceScaled =
                        _IQdiv_mathacl(pOpenLoop->velocityReference,
                               pOpenLoop->speedReferenceMax);

                pOpenLoop->speedReferenceState =
                        OPEN_LOOP_ACCELERATING_PROFILE;
            }

            break;

        case OPEN_LOOP_ACCELERATING_PROFILE:
            openLoopAccelProfileSpeedReferencing(pOpenLoop);
            break;
        default:
            /* This is intentionally kept empty */
            break;
        }
    }
    else
    {
        /* Slow first cycle is not applicable if entering open loop
         * when motor is not stationary */
        openLoopAccelProfileSpeedReferencing(pOpenLoop);
    }
}

void openLoopSetInitialVelocity(OPEN_LOOP_T *pOpenLoop)
{
    int32_t
    targetVelocity,
    initVelocity;

    targetVelocity = *(pOpenLoop->pTargetVelocity);

    if(pOpenLoop->flags.b.fastStartEnable)
    {
        if(targetVelocity > 0)
        {
            initVelocity = pOpenLoop->slowFirstCycle.speedReference;
        }
        else
        {
            initVelocity = -pOpenLoop->slowFirstCycle.speedReference;
        }

        pOpenLoop->velocityReference = initVelocity;

        pOpenLoop->velocityReferenceScaled =
                _IQdiv_mathacl(initVelocity, pOpenLoop->speedReferenceMax);
    }
    else
    {
        pOpenLoop->velocityReference = 0;
        pOpenLoop->velocityReferenceScaled = 0;
    }
}
