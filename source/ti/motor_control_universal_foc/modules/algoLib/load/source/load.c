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

#include "load.h"
#include "appDefs.h"
#include "focHALInterface.h"
#include "main.h"

static void brakeTransitionInit(LOAD_T *);
static void brakeTransitionInitOnStart(LOAD_T *);
static void brakeTransitionInitOnStop(LOAD_T *);
static void ipdTransitionInit(LOAD_T *);

void loadInit(LOAD_T *pLoad)
{
    ISD_T *pIsd = &(pLoad->isd);
    SOFT_STOP_T *pSoftStop = &(pLoad->softStop);
    TRISTATE_T *pTriState = &(pLoad->triState);
    BRAKE_T *pBrake = &(pLoad->brake);

    pLoad->flags.b.loadStartReadyStatus = FALSE;
    pLoad->flags.b.loadStopReadyStatus = FALSE;

    pLoad->motorStartAlgorithm = START_ALIGN;

    isdInit(pIsd);
    triStateInit(pTriState);
    brakeInit(pBrake);

    softStopInit(pSoftStop);

    if(pBrake->flags.b.enableOnStop)
    {
        pSoftStop->minimumSpeed = pSoftStop->minimumSpeedBrake;
    }
    else
    {
        pSoftStop->minimumSpeed = pSoftStop->minimumSpeedSoftStop;
    }

}

void loadRun(LOAD_T *pLoad)
{
    ISD_T *pIsd = &(pLoad->isd);
    IPD_T *pIpd = &(pLoad->ipd);
    SOFT_STOP_T *pSoftStop = &(pLoad->softStop);
    TRISTATE_T *pTriState = &(pLoad->triState);
    BRAKE_T *pBrake = &(pLoad->brake);
    HAL_MEASURE_CURRENT_T *pCurrent = &(pLoad->pMotorInputs->current);

    _Bool slowFirstCycleEnable = pLoad->pSlowFirstCycleFlags->b.enable;

    switch(pLoad->state)
    {

    case LOAD_INIT:
        loadInit(pLoad);

        if(pIsd->flags.b.enable)
        {
            HAL_SetPhaseVoltageChannels(pLoad->pMotorInputs);

            if(HAL_GetPhasesFloatingStatus(pLoad->pVoltage))
            {
                pIsd->state = ISD_INIT;
                pLoad->state = LOAD_ISD;
            }
        }
        else if(pBrake->flags.b.enableOnStart)
        {
            brakeTransitionInit(pLoad);
            brakeTransitionInitOnStart(pLoad);
        }
        else if(pIpd->flags.b.enable)
        {
            ipdTransitionInit(pLoad);
        }
        else if(slowFirstCycleEnable)
        {
            HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

            pLoad->motorStartAlgorithm = START_SLOW_FIRST_CYCLE;

            pLoad->state = LOAD_START;
        }
        else
        {
            HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

            pLoad->motorStartAlgorithm = START_ALIGN;

            pLoad->state = LOAD_START;
        }

        break;

    case LOAD_ISD:

        /* Execute ISD */
        isdRun(pIsd);

        if(getIsdStatus(pIsd))
        {
            switch(pIsd->direction)
            {
            case DIRECTION_FORWARD:

                if(pIsd->flags.b.forwardSyncEnable)
                {
                    /* Motor running at sufficient speed.
                     * Can be started with ISD */

                    if(pIsd->pAngleTrackingPLLEstim->speedFilt >= pIsd->minCloseLoopSpeed)
                    {
                        pLoad->motorStartAlgorithm = START_ISD;
                        pLoad->state = LOAD_START;
                    }
                    /* Motor running at Less speed.
                      * Apply Brakes and Restart */
                    else
                    {
                        brakeTransitionInit(pLoad);
                        brakeTransitionInitOnStart(pLoad);
                        pLoad->state = LOAD_BRAKE;
                    }
                }
                else
                {
                    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                    if(pTriState->flags.b.enable)
                    {
                        pLoad->motorStartAlgorithm = START_ALIGN;
                        pLoad->state = LOAD_TRISTATE;
                    }
                    else if(pBrake->flags.b.enableOnStart)
                    {
                        brakeTransitionInit(pLoad);
                        brakeTransitionInitOnStart(pLoad);
                    }
                    else if(pIpd->flags.b.enable)
                    {
                        ipdTransitionInit(pLoad);
                    }
                    else if(slowFirstCycleEnable)
                    {
                        HAL_SetPhaseCurrentChannels(pLoad->
                                                    pMotorInputs);

                        pLoad->motorStartAlgorithm =
                                START_SLOW_FIRST_CYCLE;

                        pLoad->state = LOAD_START;
                    }
                    else
                    {
                        pLoad->motorStartAlgorithm = START_ALIGN;

                        pLoad->state = LOAD_START;
                    }
                }
                break;

            case DIRECTION_REVERSE:

                if(pIsd->flags.b.reverseSyncEnable)
                {
                    /* Motor running at sufficient speed.
                     * Can be started with ISD */

                    if(pIsd->pAngleTrackingPLLEstim->speedFilt >= pIsd->minCloseLoopSpeed)
                    {
                        pLoad->motorStartAlgorithm = START_ISD;
                        pLoad->state = LOAD_START;
                    }
                    /* Motor running at Less speed.
                     * Apply Brakes and Restart */
                    else
                    {
                        brakeTransitionInit(pLoad);
                        brakeTransitionInitOnStart(pLoad);
                        pLoad->state = LOAD_BRAKE;
                    }
                }
                else
                {
                    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                    if(pTriState->flags.b.enable)
                    {
                        pLoad->motorStartAlgorithm = START_ALIGN;
                        pLoad->state = LOAD_TRISTATE;
                    }
                    else if(pBrake->flags.b.enableOnStart)
                    {
                        brakeTransitionInit(pLoad);
                        brakeTransitionInitOnStart(pLoad);
                    }
                    else if(pIpd->flags.b.enable)
                    {
                        ipdTransitionInit(pLoad);
                    }
                    else if(slowFirstCycleEnable)
                    {
                        HAL_SetPhaseCurrentChannels(pLoad->
                                                    pMotorInputs);

                        pLoad->motorStartAlgorithm =
                                START_SLOW_FIRST_CYCLE;

                        pLoad->state = LOAD_START;
                    }
                    else
                    {
                        pLoad->motorStartAlgorithm = START_ALIGN;

                        pLoad->state = LOAD_START;
                    }
                }
                break;

                /* Motor is Spinning in opposite to Intended direction */
            case  DIRECTION_UNINTENDED:
                    brakeTransitionInit(pLoad);
                    brakeTransitionInitOnStart(pLoad);
                    pLoad->state = LOAD_BRAKE;
                break;
            case DIRECTION_UNIDENTIFIED:

            case DIRECTION_STOP:

                if(pBrake->flags.b.enableOnStart)
                {
                    brakeTransitionInit(pLoad);
                    brakeTransitionInitOnStart(pLoad);
                }
                else if(pIpd->flags.b.enable)
                {
                    ipdTransitionInit(pLoad);
                }
                else if(slowFirstCycleEnable)
                {
                    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                    pLoad->motorStartAlgorithm = START_SLOW_FIRST_CYCLE;

                    pLoad->state = LOAD_START;
                }
                else
                {
                    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                    pLoad->motorStartAlgorithm = START_ALIGN;

                    pLoad->state = LOAD_START;
                }

                break;

            default:

                break;
            }
        }

        break;

    case LOAD_TRISTATE:

        if(getTriStateStatus(pTriState))
        {
            if(pBrake->flags.b.enableOnStart)
            {
                brakeTransitionInit(pLoad);
                brakeTransitionInitOnStart(pLoad);
            }
            else if(pIpd->flags.b.enable)
            {
                ipdTransitionInit(pLoad);
            }
            else if(slowFirstCycleEnable)
            {
                HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                pLoad->motorStartAlgorithm = START_SLOW_FIRST_CYCLE;

                pLoad->state = LOAD_START;
            }
            else
            {
                HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                pLoad->motorStartAlgorithm = START_ALIGN;

                pLoad->state = LOAD_START;
            }
        }

        break;

    case LOAD_BRAKE:

        brakeRun(pBrake);

        if((getBrakeStatusCurrent(pBrake) &&
                pBrake->flags.b.brakeCheckOnCurrent) ||
                (getBrakeStatusTimeOut(pBrake) &&
                        !pBrake->flags.b.brakeCheckOnCurrent))
        {
            if(pBrake->flags.b.brakeOnStart)
            {
                /* Brake mode entered during motor start */
                pLoad->pMotorInputs->current.flags.b.threeShuntDynamic =
                        pLoad->pMotorInputs->current.
                        flags.b.threeShuntDynamicSet;

                if(pIpd->flags.b.enable)
                {
                    ipdTransitionInit(pLoad);
                }
                else if(slowFirstCycleEnable)
                {
                    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                    pLoad->motorStartAlgorithm = START_SLOW_FIRST_CYCLE;

                    pLoad->state = LOAD_START;
                }
                else
                {
                    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

                    pLoad->motorStartAlgorithm = START_ALIGN;

                    pLoad->state = LOAD_START;
                }
            }
            else
            {
                /* Brake state entered during motor stop */
                pLoad->state = LOAD_STOP;
            }
        }

        break;

    case LOAD_IPD:

        /* Execute IPD */
        ipdRun(pIpd);

       if(getIpdFaultStatus(pIpd) != IPD_NO_FAULT)
       {
           pLoad->faultStatus = LOAD_IPD_FAULT;

           HAL_SetPWMFrequency(*pLoad->pPWMPeriod,
                               *pLoad->pPropagationDelay);
           HAL_SetADCSamplingFrequency(*pLoad->pADCSamplingRatio);
           pLoad->state = LOAD_FAULT;
       }

        if(getIpdStatus(pIpd))
        {
           pLoad->motorStartAlgorithm = START_IPD;

            HAL_SetPWMFrequency(*pLoad->pPWMPeriod,
                                *pLoad->pPropagationDelay);
            HAL_SetADCSamplingFrequency(*pLoad->pADCSamplingRatio);
            
           pLoad->state = LOAD_START;
        }
        break;

    case LOAD_START:
        pLoad->flags.b.loadStartReadyStatus = TRUE;
        pLoad->state = LOAD_RUN;
        break;

    case LOAD_RUN:
        /* This is intentionally kept empty */
        break;

    case LOAD_STOP_INIT:

        if(pLoad->brake.flags.b.enableOnFault)
        {
            brakeTransitionInit(pLoad);
            brakeTransitionInitOnStop(pLoad);
        }
        else if(pLoad->softStop.flags.b.enable ||
                pLoad->softStop.flags.b.enableOnBrake)
        {
            /* Soft stop */
            pLoad->state = LOAD_SOFT_STOP;
        }
        else
        {
            /* Hi-z mode */
            pLoad->state = LOAD_STOP;
        }

        break;

    case LOAD_SOFT_STOP:

        softStopRun(pSoftStop);

        if(getSoftStopStatus(pSoftStop))
        {
            if(!pSoftStop->flags.b.enableOnBrake)
            {
                if(pLoad->brake.flags.b.enableOnStop)
                {
                    brakeTransitionInit(pLoad);
                    brakeTransitionInitOnStop(pLoad);
                }
                else if(pLoad->flags.b.loadStopAlignEnable)
                {
                    HAL_EnablePWM();
                    pLoad->pRotorAlignFlags->b.enableOnStop = TRUE;
                    pLoad->state = LOAD_ALIGN_STOP;
                }
                else
                {
                    pLoad->state = LOAD_STOP;
                }
            }
        }

        break;

    case LOAD_ALIGN_STOP:

        if(pLoad->pRotorAlignFlags->b.status)
        {
            pLoad->state = LOAD_STOP;
        }

        break;

    case LOAD_STOP:
        pLoad->flags.b.loadStopReadyStatus = TRUE;

        break;

    case LOAD_FAULT:
        /* This is intentionally kept empty */
        break;

    default:
        pLoad->state = LOAD_INIT;
        break;
    }
}

static void ipdTransitionInit(LOAD_T *pLoad)
{
    IPD_T *pIpd;

    pIpd = &(pLoad->ipd);

    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

    /* Setting the PWM freq to IPD run rate */
    HAL_SetPWMFrequency(IPD_PWM_PERIOD, *pLoad->pPropagationDelay);

    HAL_SetADCSamplingFrequency(ADC_SAMPLE_EVERY_PWM);

    pIpd->state = IPD_INIT;
    pLoad->state = LOAD_IPD;
}

static void brakeTransitionInit(LOAD_T *pLoad)
{
    BRAKE_T *pBrake;

    pBrake = &(pLoad->brake);

    brakeInit(pBrake);

    pLoad->pMotorInputs->current.flags.b.threeShuntDynamic = FALSE;

    pLoad->pMotorInputs->current.currentShunt = CURRENT_SHUNT_CONFIG;

    HAL_SetPhaseCurrentChannels(pLoad->pMotorInputs);

    pLoad->state = LOAD_BRAKE;
}

static void brakeTransitionInitOnStart(LOAD_T *pLoad)
{
    BRAKE_T *pBrake;

    pBrake = &(pLoad->brake);

    pBrake->timeOutCountMax = pBrake->timeOutCountMaxOnStart;
    pBrake->flags.b.brakeCheckOnCurrent =
            pBrake->flags.b.brakeCheckCurrentStart;
    pBrake->flags.b.brakeOnStart = TRUE;

    pLoad->motorStartAlgorithm = START_ALIGN;

    HAL_EnableLowSideBrake();  // Only Low Side Braking applied
}

static void brakeTransitionInitOnStop(LOAD_T *pLoad)
{
    BRAKE_T *pBrake;

    pBrake = &(pLoad->brake);

    pBrake->timeOutCountMax = pBrake->timeOutCountMaxOnStop;

    pBrake->flags.b.brakeOnStart = FALSE;

    if(pBrake->flags.b.enableOnFault)
    {
        pBrake->flags.b.brakeCheckOnCurrent =
                pBrake->flags.b.brakeCheckCurrentFault;
    }
    else
    {
        pBrake->flags.b.brakeCheckOnCurrent =
                pBrake->flags.b.brakeCheckCurrentStop;
    }

    HAL_EnableLowSideBrake();

}

void loadLowPriorityRun(LOAD_T *pLoad)
{
    TRISTATE_T *pTriState = &pLoad->triState;
    BRAKE_T *pBrake = &pLoad->brake;

    switch(pLoad->state)
    {
    case LOAD_TRISTATE:
        triStateRun(pTriState);
        break;

    case LOAD_BRAKE:
        brakeLowPriorityRun(pBrake);
        break;
    default:
        break;

    }
}
