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
#include "application.h"
#include "focHALInterface.h"
#include "main.h"
#include "appDefs.h"
#include "appConfigParamUpdate.h"
#include "gateDriver.h"
#include "motorInputOutputConfig.h"
#include "focConfig.h"
#include "sourceLimitsConfig.h"
#include "loadConfig.h"
#include "semiCloseLoopEstimConfig.h"
#include "services.h"
#include "faultsConfig.h"
#include "faults.h"
#include "appUserInputsConfig.h"
#include "sourceVoltageLimit.h"
#include "filter.h"
#include "speedProfile.h"
#include <string.h>
#include "ISR.h"
#include "angleTrackingPLL.h"
#include "appConfigParamUpdate.h"

static void focStartTransition(UNIVERSAL_FOC_APPLICATION_T *);

static void loadStartTransition(UNIVERSAL_FOC_APPLICATION_T *);

static void loadStopTransition(UNIVERSAL_FOC_APPLICATION_T *);

static void loadRunTransition(UNIVERSAL_FOC_APPLICATION_T *);

static void appBrakeCheck(UNIVERSAL_FOC_APPLICATION_T *);

static void motorStateSet(UNIVERSAL_FOC_APPLICATION_T *);

static void controllerFaultsClear(UNIVERSAL_FOC_APPLICATION_T *);

static void updateStatusVariables(UNIVERSAL_FOC_APPLICATION_T *);

void applicationConfig(void *gpMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*)gpMC_App;
    if(pMC_App->state != APP_FAULT)
    {
        pMC_App->state = APP_INIT;
    }
    sourceLimitsConfig(pMC_App);
    motorInputOutputConfig(pMC_App);
    focConfig(pMC_App);
    loadConfig(pMC_App);
    semiCloseLoopEstimConfig(pMC_App);
    faultsConfig(pMC_App);
    CurrSenseAmpConfigInit(&pMC_App->motorInputs);
}

static void focStartTransition(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    FOC_T *pFOC = &(pMC_App->foc);
    ISD_T *pIsd = &(pMC_App->load.isd);
    IPD_T *pIpd = &(pMC_App->load.ipd);

    switch(pMC_App->load.motorStartAlgorithm)
    {
    case START_ALIGN:

        pFOC->state = FOC_INIT;

        if(pMC_App->targetVelocity > 0)
        {
            pFOC->rotorAlign.angle = pFOC->rotorAlign.angleSet -
                    SECOND_ALIGN_ANGLE_OFFSET;
        }
        else
        {
            pFOC->rotorAlign.angle = pFOC->rotorAlign.angleSet +
                    SECOND_ALIGN_ANGLE_OFFSET;
        }

        /* Start the motor with open loop profile mode */
        pFOC->openLoop.speedReferenceState =
                OPEN_LOOP_ACCELERATING_PROFILE;

        break;

    case START_IPD:

            /* Initialize Open Loop */
            openLoopInit(&pFOC->openLoop);

            /* Set Initial current reference */
            pFOC->openLoop.iqRef = 0;

            /* Set Open Loop initial velocity */
            openLoopSetInitialVelocity(&pFOC->openLoop);

            pFOC->openLoop.angle = pIpd->angle;

            /* Start the motor with open loop profile mode */
            pFOC->openLoop.speedReferenceState =
                        OPEN_LOOP_ACCELERATING_PROFILE;

            /* Initialize acceleration parameters */
            pFOC->openLoop.pSpeedRefSlewRate =
                    &(pFOC->openLoop.speedRefSlewRateFastAccel);

            pFOC->openLoop.pAccelRefSlewRate =
                    &(pFOC->openLoop.accelRefSlewRateFastAccel);

            pFOC->openLoop.iqRefSet =
                    pFOC->openLoop.iqRefSetFastAccel;

            /* Select open loop IdRef, IqRef and angle */

            pFOC->piId.pReference = &(pFOC->openLoop.idRef);
            pFOC->piIq.pReference = &(pFOC->openLoop.iqRef);

            pFOC->pVelocityReference =
                            &(pFOC->openLoop.velocityReference);

            pFOC->pFOCAngle = &(pFOC->openLoop.angle);

            pFOC->state = FOC_OPEN_LOOP;

        break;
    case START_ISD:


            /* Motor can definitely enter close loop */

            /* Initialize Close Loop */
            closeLoopInit(&pFOC->closeLoop);

            /* Initialize current integrators */
            pFOC->piId.kiTerm = 0;
            pFOC->piIq.kiTerm =  pMC_App->angleTrackingPLLEstim.EdqFilt.d;

            /* Initialize close loop parameters */
            pFOC->closeLoop.piSpeed.kiTerm = 0;

            pFOC->closeLoop.angleOffset = 0;

            /* Initialize Velocity Reference */
            if(pFOC->closeLoop.controlMode == CONTROL_SPEED)
            {
            /* Initialize Velocity Reference */
                pFOC->closeLoop.velocityReference =
                        pMC_App->angleTrackingPLLEstim.velocityFilt;

            }
            else if(pFOC->closeLoop.controlMode == CONTROL_VOLTAGE)
            {

                pFOC->closeLoop.velocityReference = _IQmpy_mathacl(pFOC->piIq.kiTerm, *pFOC->pInvVdc) <<
                                                    (GLOBAL_IQ - INV_VDC_IQ_SCALING);
            }

            pFOC->closeLoop.targetVelocityLimited =
                    pMC_App->angleTrackingPLLEstim.velocityFilt;

            pFOC->closeLoop.targetSpeedLimited =
                    pMC_App->angleTrackingPLLEstim.speedFilt;

            pFOC->closeLoop.speedReference =
                    pMC_App->angleTrackingPLLEstim.speedFilt;

            pFOC->closeLoop.angle = pMC_App->angleTrackingPLLEstim.fluxAngle;

            /* Select close loop IdRef, IqRef and angle */
            pFOC->piId.pReference = &(pFOC->closeLoop.idqRef.d);
            pFOC->piIq.pReference = &(pFOC->closeLoop.idqRef.q);

            pFOC->pFOCAngle = &(pFOC->closeLoop.angle);

            pFOC->pVelocityReference =
                    &(pFOC->closeLoop.velocityReference);

            pFOC->state = FOC_CLOSE_LOOP;

        pFOC->vAlphaBeta = pIsd->eAlphaBeta;

        pFOC->mAlphaBeta.alpha =
                _IQmpy_mathacl(pFOC->vAlphaBeta.alpha, *pFOC->pInvVdc) <<
                (GLOBAL_IQ - INV_VDC_IQ_SCALING);

        pFOC->mAlphaBeta.beta =
                _IQmpy_mathacl(pFOC->vAlphaBeta.beta, *pFOC->pInvVdc) <<
                (GLOBAL_IQ - INV_VDC_IQ_SCALING);

        MC_InverseClarkeTransform(&pFOC->mabc, &pFOC->mAlphaBeta);

        MC_SpaceVectorGen(&pFOC->svm, &pFOC->mabc);

        break;

    case START_SLOW_FIRST_CYCLE:

        /* Initialize Open Loop */
        openLoopInit(&pFOC->openLoop);

        pFOC->openLoop.iqRef = 0;

        /* Start the motor with open loop profile mode */
        pFOC->openLoop.speedReferenceState =
                OPEN_LOOP_SLOW_FIRST_CYCLE_PROFILE;

        /* Initialize acceleration parameters */
        pFOC->openLoop.pSpeedRefSlewRate =
                &(pFOC->openLoop.speedRefSlewRateFastAccel);

        pFOC->openLoop.pAccelRefSlewRate =
                &(pFOC->openLoop.accelRefSlewRateFastAccel);

        pFOC->openLoop.iqRefSet =
                pFOC->openLoop.iqRefSetFastAccel;

        /* Select open loop IdRef, IqRef and angle */

        pFOC->piId.pReference = &(pFOC->openLoop.idRef);
        pFOC->piIq.pReference = &(pFOC->openLoop.iqRef);

        pFOC->pVelocityReference =
                &(pFOC->openLoop.velocityReference);

        pFOC->pFOCAngle = &(pFOC->openLoop.angle);

        pFOC->state = FOC_OPEN_LOOP;
        break;

    }

    HAL_UpdateDuty(pMC_App->pDabc,
                    pMC_App->motorInputs.pwmPeriod,
                    *pMC_App->pVoltageSector,
                    *pMC_App->pMinPWMdelta,
                    *pMC_App->pBlankingTime
                    );

    //Todo: Analyze the Applied Voltage Instance, Observed BEMF instance & current Rotor Angle
#ifdef ESMO_ESTIMATOR
    pMC_App->angleTrackingPLLEstim.pEAlphaBeta = &(pFOC->esmo.prevEAlphaBeta);
#else
    pMC_App->angleTrackingPLLEstim.pEAlphaBeta = &(pFOC->estim.eAlphaBeta);
#endif
}

static void loadStartTransition(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    LOAD_T *pLoad = &(pMC_App->load);

    pMC_App->angleTrackingPLLEstim.pEAlphaBeta = &(pLoad->isd.eAlphaBeta);

    pLoad->state = LOAD_INIT;

}

static void loadStopTransition(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    pMC_App->load.state = LOAD_STOP_INIT;
}

static void loadRunTransition(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    pMC_App->load.state = LOAD_RUN;
}

/******************************************************************************/
void applicationRun(void *gpMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T *)gpMC_App;
    HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs = &(pMC_App->motorInputs);
    HAL_MEASURE_CURRENT_T *pCurrent = &(pMC_App->motorInputs.current);
    FOC_T *pFOC = &(pMC_App->foc);
    LOAD_T *pLoad = &(pMC_App->load);
    ISD_T *pIsd = &(pMC_App->load.isd);

    int32_t EmagSqr;

    switch(pMC_App->state)
    {
    case APP_INIT:
        HAL_DisablePWM();
        HAL_SetPhaseVoltageChannels(pMotorInputs);

        pMC_App->flags.b.runCmd = 0;

        pMC_App->targetVelocity = 0;

        if(pMC_App->pAppInterface->userInputs.debugFlags.b.updatedData)
        {
            sourceLimitsInit(&pMC_App->sourceLimits);

            inputsInit(&pMC_App->motorInputs);
            focInit(&pMC_App->foc);
            loadInit(&pMC_App->load);
            angleTrackingPLLInit(&pMC_App->angleTrackingPLLEstim);
            faultServiceInit(&pMC_App->faultService);
            HAL_pwmCountZero();

            pMC_App->state = APP_IDLE;
        }

        pMC_App->faultService.flags.b.actionTaken = FALSE;

        break;

    case APP_IDLE:

        /* Check if brake command is received */
        appBrakeCheck(pMC_App);

        if(pMotorInputs->measureQuantity != MEASURE_PHASE_VOLTAGE)
        {
            HAL_SetPhaseVoltageChannels(pMotorInputs);
        }

        if(HAL_GetPhasesFloatingStatus(&pMC_App->motorInputs.voltage))
        {
            /* Process BEMF */
            MC_ClarkeTransform(&pIsd->eAlphaBeta, pIsd->pEabc);

            EmagSqr = _IQmpy_mathacl(pIsd->eAlphaBeta.alpha,
                             pIsd->eAlphaBeta.alpha) +
                                     _IQmpy_mathacl(pIsd->eAlphaBeta.beta,
                                            pIsd->eAlphaBeta.beta);

            pIsd->bemfMag = _IQsqrt_mathacl(EmagSqr);

            /* Wait for run command.
             * If received, calculate current offset */
            if(pMC_App->flags.b.runCmd)
            {
                HAL_SetPhaseCurrentChannels(pMotorInputs);

                HAL_setGateDriveOffsetCalib();

                pCurrent->state = OFFSET_INIT;

                pMC_App->state = APP_OFFSET_CALIBRATION;
            }
        }

        break;

    case APP_OFFSET_CALIBRATION:

        /* Calculate current offset */
        measureCurrentOffset(pMotorInputs);

        /* Wait for offset calculation to complete.
         * If complete, start the load */
        if(getMeasureOffsetStatus(&pMC_App->motorInputs))
        {
            HAL_resetGateDriveOffsetCalib();

            pCurrent->currentShunt = pCurrent->currentShuntSet;

            HAL_SetPhaseCurrentChannels(pMotorInputs);

            loadStartTransition(pMC_App);

            pMC_App->state = APP_START;
        }
        break;

    case APP_START:

        /* Check if brake command is received */
        appBrakeCheck(pMC_App);

        /* Run the load */
        loadRun(pLoad);

        /* Check for load fault */
        if(getLoadStallStatus(pLoad))
        {
            pMC_App->faultStatus = LOAD_STALL;
            pMC_App->state = APP_FAULT;
        }

        /* Wait till load is ready and motor can be started */
        if(getLoadStartReadyStatus(pLoad))
        {
            pCurrent->currentShunt = pCurrent->currentShuntSet;

            HAL_SetPhaseCurrentChannels(pMotorInputs);

            focStartTransition(pMC_App);

            HAL_EnablePWM();

            pMC_App->state = APP_RUN;
        }


        /* Stop the application if run command withdrawn */
        if(!pMC_App->flags.b.runCmd)
        {
            loadStopTransition(pMC_App);
            pMC_App->state = APP_STOP;
        }

        break;

    case APP_RUN:
        /* Check if brake command is received */
        appBrakeCheck(pMC_App);

        /* Measure phase currents.
         * Execute the motor and load control algorithms */
        focRun(pFOC);

        HAL_UpdateDuty(pMC_App->pDabc,
                            pMC_App->motorInputs.pwmPeriod,
                            *pMC_App->pVoltageSector,
                            *pMC_App->pMinPWMdelta,
                            *pMC_App->pBlankingTime
                            );

        loadRun(pLoad);

        /* Check if motor is stalled */
        if(getFOCStallStatusForAction(pFOC))
        {
            pMC_App->faultStatus = MOTOR_STALL;
            pMC_App->state = APP_FAULT;
        }

        if(getFOCStallStatusForReport(pFOC))
        {
            pMC_App->faultStatus = MOTOR_STALL;
        }

        /* Check for load fault */
        if(getLoadStallStatus(pLoad))
        {
            pMC_App->faultStatus = LOAD_STALL;
            pMC_App->state = APP_FAULT;
        }

        /* Stop the application if run command withdrawn */
        if(!pMC_App->flags.b.runCmd)
        {
            loadStopTransition(pMC_App);
            pMC_App->state = APP_STOP;
        }

        break;

    case APP_STOP:

        /* Check if brake command is received */
        appBrakeCheck(pMC_App);

        if(runMotorDuringStop(pLoad))
        {
            focRun(pFOC);

            HAL_UpdateDuty(pMC_App->pDabc,
                    pMC_App->motorInputs.pwmPeriod,
                    *pMC_App->pVoltageSector,
                    *pMC_App->pMinPWMdelta,
                    *pMC_App->pBlankingTime
                    );
        }

        loadRun(pLoad);

        /* Check if motor is stalled */
        if(getFOCStallStatusForAction(pFOC))
        {
            pMC_App->faultStatus = MOTOR_STALL;
            pMC_App->state = APP_FAULT;
        }

        /* Check for load fault */
        if(getLoadStallStatus(pLoad))
        {
            pMC_App->faultStatus = LOAD_STALL;
            pMC_App->state = APP_FAULT;
        }

        /* Check for run command
         * If re-issued, start running the motor and load */
        if(pMC_App->flags.b.runCmd && (pLoad->state == LOAD_SOFT_STOP))
        {
            loadRunTransition(pMC_App);
            pMC_App->state = APP_RUN;
        }

        /* Wait till load is ready and motor can be stopped
         * If stopped, re-initialize the application */
        if(getLoadStopReadyStatus(pLoad) && !pMC_App->flags.b.brakeCmd)
        {
            HAL_DisablePWM();
            pMC_App->state = APP_INIT;
        }

        break;

    case APP_BRAKE:

        /* Check if brake command is withdrawn.
         * If withdrawn, re-initialize the application */
        if(!pMC_App->flags.b.brakeCmd)
        {
            HAL_DisablePWM();

            HAL_SetPhaseVoltageChannels(pMotorInputs);

            pMC_App->state = APP_INIT;
        }
        else
        {

                if(pFOC->rotorAlign.flags.b.enableOnBrake)
                {
                    HAL_EnablePWM();
                    pMC_App->foc.state = FOC_ROTOR_ALIGN;
                    focRun(pFOC);
                    
                    HAL_UpdateDuty(pMC_App->pDabc,
                    pMC_App->motorInputs.pwmPeriod,
                    *pMC_App->pVoltageSector,
                    *pMC_App->pMinPWMdelta,
                    *pMC_App->pBlankingTime
                    );
                }
                else
                {
                    HAL_EnableLowSideBrake();
                }
        }

        break;

    case APP_FAULT:

        /* Fault house keeping */
        if(pMC_App->faultStatus == LOAD_STALL ||
                pMC_App->faultStatus == VOLTAGE_OUT_OF_BOUNDS)
        {
            HAL_DisablePWM();
        }

        pMC_App->flags.b.runCmd = FALSE;
        pMC_App->targetVelocity = 0;

        faultServiceRun(pMC_App);

        if(pMC_App->faultService.flags.b.actionTaken)
        {
            loadRun(pLoad);
        }

        break;

    default:
        pMC_App->state = APP_FAULT;
        break;
    }

}

/******************************************************************************/

static void appBrakeCheck(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    FOC_T *pFOC = &(pMC_App->foc);
    LOAD_T *pLoad = &(pMC_App->load);
    ROTOR_ALIGN_T *pRotorAlign = &(pMC_App->foc.rotorAlign);

    if(pMC_App->flags.b.brakeCmd)
    {
        /* Brake command is issued */
        if(pMC_App->state == APP_IDLE)
        {
            if(pRotorAlign->flags.b.enableOnBrakeSet)
            {
                pMC_App->flags.b.runCmd = TRUE;
            }
            else
            {
                pMC_App->state = APP_BRAKE;
            }
        }
        else if(pMC_App->state == APP_START)
        {
            pLoad->state = LOAD_SOFT_STOP;

            pMC_App->state = APP_STOP;

        }
        else
        {
            pMC_App->flags.b.runCmd = FALSE;

            pMC_App->targetVelocity = 0;

            pLoad->softStop.flags.b.enableOnBrake = TRUE;
            pLoad->softStop.minimumSpeed = pLoad->softStop.minimumSpeedBrake;

            if(getSoftStopStatus(&pLoad->softStop))
            {
                pRotorAlign->flags.b.enableOnBrake =
                        pRotorAlign->flags.b.enableOnBrakeSet;

                /* Select rotor align IdRef, IqRef and angle */
                pFOC->piId.pReference = &(pFOC->rotorAlign.idRef);
                pFOC->piIq.pReference = &(pFOC->rotorAlign.iqRef);

                pFOC->pFOCAngle = &(pFOC->rotorAlign.angle);
                pMC_App->state = APP_BRAKE;

            }
        }
    }
    else
    {
        pLoad->softStop.flags.b.enableOnBrake = FALSE;

        if(pLoad->brake.flags.b.enableOnStop ||
                pLoad->flags.b.loadStopAlignEnable)
        {
            pLoad->softStop.minimumSpeed =
                    pLoad->softStop.minimumSpeedBrake;
        }
        else
        {
            pLoad->softStop.minimumSpeed =
                    pLoad->softStop.minimumSpeedSoftStop;
        }
    }
}

/******************************************************************************/

void applicationLowPriorityRun(void *gpMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T *)gpMC_App;
    SOURCE_LIMITS_T *pSourceLimits;
    SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit;
    SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit;
    HAL_MEASURE_VOLTAGE_T *pVoltage;
    HAL_MEASURE_CURRENT_T *pCurrent;
    FOC_T *pFOC;
    OPEN_LOOP_T *pOpenLoop;
    LOAD_T *pLoad;
    FAULT_SERVICE_T *pFaultService;
    USER_INPUTS_T *pUserInputs;

    int32_t targetSpeed = 0, temp, maxCurr;

    pSourceLimits = &(pMC_App->sourceLimits);
    pSourceVoltageLimit = &(pSourceLimits->sourceVoltageLimit);
    pSourceCurrentLimit = &(pSourceLimits->sourceCurrentLimit);
    pVoltage = &(pMC_App->motorInputs.voltage);
    pCurrent = &(pMC_App->motorInputs.current);
    pFOC = &(pMC_App->foc);
    pOpenLoop = &(pMC_App->foc.openLoop);
    pLoad = &(pMC_App->load);
    pFaultService = &(pMC_App->faultService);
    pUserInputs = &(pMC_App->pAppInterface->userInputs);

    /* Incrementing 1 msec counter */
    pMC_App->pAppInterface->servicesCount++;

    if(HAL_getRawHVdieFaultStatus())
    {
        HAL_DisablePWM();
        pMC_App->state = APP_FAULT; /* Move FOC algo to fault state */
    }
    
    pVoltage->voltageGain = pVoltage->voltageGainSet;

    pMC_App->foc.closeLoop.idRefGen.mtpa.idFiltConst =
               (pMC_App->angleTrackingPLLEstim.bemfFiltConst *
               (uint16_t)pMC_App->pAppInterface->
                   userInputsInterface.adcSamplingFrequencyKHz);

    /* Calculate inverse of dc bus voltage */
    if((pMC_App->state == APP_RUN) || (pMC_App->state == APP_STOP))
    {
        if(pUserInputRegs->pinCfg.b.vdcFiltDis)
        {
            pVoltage->vdcFilt = pVoltage->vdc;
        }
        else
        {
            filter(&pVoltage->vdcFilt, pVoltage->vdc,
                   pMC_App->foc.closeLoop.idRefGen.mtpa.idFiltConst);
        }
    }
    else
    {
        pVoltage->vdcFilt = pVoltage->vdc;
    }


    pVoltage->invVdcFilt = _IQdiv(_IQ(1.0), pVoltage->vdcFilt) >>
            (GLOBAL_IQ - INV_VDC_IQ_SCALING);

    pMC_App->flags.b.brakeCmd = update_BrakeStatus();

    /* Execute the source limits */
    sourceLimitsRun(&pMC_App->sourceLimits);

    /* Use Speed from speed block */
    if(!pMC_App->flags.b.brakeCmd)
    {
        targetSpeed = speedProfile(pMC_App);

        if((targetSpeed < MIN_TARGET_SPEED) ||
                (pMC_App->maximumSpeed == 0))
        {
            targetSpeed = 0;
        }

        /* Check for Direction reversal command */
        if(pMC_App->flags.b.reverseDirectionCmd !=
                pMC_App->flags.b.prevDirectionCmd)
        {
            /* Upon Direction reversal Motor should
             * stop and restart */
            pMC_App->flags.b.directionActionTaken = FALSE;
            pMC_App->flags.b.runCmd = FALSE;
            pMC_App->targetVelocity = 0;
        }

        if(pMC_App->flags.b.directionActionTaken &&
                (pSourceCurrentLimit->state == SOURCE_CURRENT_BELOW_LIMIT))
        {
            /* Action based on direction reversal is taken */
            if(pMC_App->flags.b.reverseDirectionCmd)
            {
                pMC_App->targetVelocity = -targetSpeed;
            }
            else
            {
                pMC_App->targetVelocity = targetSpeed;
            }
        }
        else
        {
            if(pMC_App->state == APP_IDLE)
            {
                pMC_App->flags.b.directionActionTaken = TRUE;
            }
        }

    }

    /* Set minimum and maximum speed based on direction */
    if(pMC_App->targetVelocity > 0)
    {
        pMC_App->foc.closeLoop.velocityReferenceMax = pMC_App->maximumSpeed;

        pMC_App->foc.closeLoop.velocityReferenceMin = pMC_App->minimumSpeed;

#ifdef ESMO_ESTIMATOR
        pMC_App->angleTrackingPLLEstim.phaseDiff = _IQ(0.125);
#else
        pMC_App->angleTrackingPLLEstim.phaseDiff = _IQ(0.25);
#endif
    }
    else if(pMC_App->targetVelocity < 0)
    {
        pMC_App->foc.closeLoop.velocityReferenceMax = -pMC_App->minimumSpeed;

        pMC_App->foc.closeLoop.velocityReferenceMin = -pMC_App->maximumSpeed;

#ifdef ESMO_ESTIMATOR
        pMC_App->angleTrackingPLLEstim.phaseDiff = -_IQ(0.125);
#else
        pMC_App->angleTrackingPLLEstim.phaseDiff = -_IQ(0.25);
#endif
    }

    if((pMC_App->state == APP_RUN) ||
            (pMC_App->state == APP_STOP && runMotorDuringStop(pLoad)) ||
            (pMC_App->state == APP_BRAKE && pFOC->rotorAlign.flags.b.enableOnBrake))
    {
        /* Check for source current limit */
        if(pSourceLimits->sourceCurrentLimit.flags.b.enable)
        {
            if((getCurrentLimitTransition(pSourceCurrentLimit) ==
                    SOURCE_CURRENT_TRANSITION_TO_CURRENT_LIMIT) ||
                    getCurrentLimitState(pSourceCurrentLimit) ==
                            SOURCE_CURRENT_ABOVE_LIMIT)
            {

                pFOC->closeLoop.fastDecelSpeedRefSlewRate =
                        (pFOC->closeLoop.slowDecelSpeedRefSlewRate >> 1);

                pFOC->closeLoop.flags.b.avsEnable = FALSE;
                temp = _IQdiv(pSourceCurrentLimit->sourceCurrentLimit,
                              *pSourceCurrentLimit->pSourceCurrent);

                if(temp > _IQ(1.0))
                {
                    temp = _IQ(1.0);
                }

                pMC_App->targetVelocity =
                        _IQmpy(temp, pMC_App->targetVelocity);

                if(pMC_App->targetVelocity > 0)
                {
                    if(pMC_App->targetVelocity >= targetSpeed)
                    {
                        pMC_App->targetVelocity = targetSpeed;
                    }

                    else if(pMC_App->targetVelocity <= pMC_App->minimumSpeed)
                    {
                        pMC_App->targetVelocity = pMC_App->minimumSpeed;
                    }
                }
                else
                {
                    if(pMC_App->targetVelocity <= -targetSpeed)
                    {
                        pMC_App->targetVelocity = -targetSpeed;
                    }

                    else if(pMC_App->targetVelocity >= -pMC_App->minimumSpeed)
                    {
                        pMC_App->targetVelocity = -pMC_App->minimumSpeed;
                    }
                }

            }
        }
        /* Apply slew rates in FOC only if application is running or stopping */
        focLowPriorityRun(pFOC);
    }

    if((pMC_App->state == APP_START) ||
            (pMC_App->state == APP_STOP) ||
            (pMC_App->state == APP_BRAKE) ||
            (pMC_App->state == APP_FAULT))
    {
        loadLowPriorityRun(&pMC_App->load);
    }

    /* Check for voltage out of bounds fault */
    if((getSourceVoltageFaultStatus(pSourceVoltageLimit) != NO_VOLTAGE_FAULT))
    {
        pMC_App->faultStatus = VOLTAGE_OUT_OF_BOUNDS;

        pMC_App->state = APP_FAULT;
    }
    else
    {

        if(pMC_App->targetVelocity == 0)
        {
            if(!pMC_App->flags.b.brakeCmd)
            {
                pMC_App->flags.b.runCmd = FALSE;
            }
        }
        else
        {
            if(pUserInputs->debugFlags.b.updatedData)
            {
                pMC_App->flags.b.runCmd = TRUE;
                pMC_App->flags.b.motorRun = TRUE;
            }
        }
    }

/* Clear Faults if in report only mode */
if((pMC_App->state != APP_FAULT) &&
        getFaultClearStatus(pFaultService))
{
    controllerFaultsClear(pMC_App);
}

if(pMC_App->state == APP_FAULT)
{
    faultServiceLowPriorityRun(pMC_App);

    if(getFaultClearStatus(pFaultService))
    {
        controllerFaultsClear(pMC_App);
        faultReportClear(pFaultService);

        pMC_App->state = APP_INIT;

        pFOC->state = FOC_INIT;
        pLoad->state = LOAD_INIT;
    }
    else if(pFaultService->flags.b.retryStatus)
    {
        if(((pMC_App->pAppInterface->pUserOutputs->controllerFaultStatus & controllerFaultActionLatched) == 0) &&
                (*pFaultService->pSourceVoltageFaultStatus ==
                        NO_VOLTAGE_FAULT))
        {
            pFaultService->flags.b.clearFault = TRUE;
        }
    }
}
else if((pMC_App->state == APP_RUN) || (pMC_App->state == APP_STOP) ||
        (pMC_App->state == APP_BRAKE))
{
    /* Motor is running */
    pSourceLimits->overCurrent.flags.b.enable =
            pSourceLimits->overCurrent.flags.b.enableSet;

    if((pMC_App->foc.closeLoop.commutationState ==
            COMMUTATION_ALIGNED) &&
            (pMC_App->foc.state == FOC_CLOSE_LOOP))
    {
        /* Motor is running in close loop and
         * commutation aligned to estimator */
        pSourceLimits->sourceCurrentLimit.flags.b.enable =
                pSourceLimits->sourceCurrentLimit.flags.b.enableSet;
    }
    else
    {
        /* Motor is not running in close loop or
         * commutation is not yet aligned to estimator */

        /* Disable the source limit enable flag*/
        pSourceLimits->sourceCurrentLimit.flags.b.enable =  FALSE;
    }

}


pCurrent->pOffset = &(pCurrent->offsetGainIMaxBy1);

/* Over current limit protection */
if(pSourceLimits->overCurrent.flags.b.enable &&
        !pSourceLimits->overCurrent.flags.b.status)
{
    overCurrentRun(&pSourceLimits->overCurrent);
}
else
{
/*    HAL_DisableWindowComparator(); */
}

/* State set for output - debug purposes */
motorStateSet(pMC_App);

if(pMC_App->pAppInterface->servicesCount >= SERVICE_INTERVAL)
{
    pMC_App->pAppInterface->servicesCount = 0;
    pMC_App->pAppInterface->flags.b.shadowToAlgorithmSet = TRUE;
}

/* faults processing once in 100msec */
if(pMC_App->pAppInterface->servicesCount == FAULT_SERVICE_COUNT)
{
    pMC_App->pAppInterface->flags.b.faultsResponseSet = TRUE;
}

if(pMC_App->pAppInterface->servicesCount == HVDIE_SERVICE_COUNT)
{
    pMC_App->pAppInterface->flags.b.hvDieConfigSet = TRUE;
}

pMC_App->flags.b.prevDirectionCmd = pMC_App->flags.b.reverseDirectionCmd;

updateMotorStopConfigParam(pMC_App);

if(pUserCtrlRegs->algoDebugCtrl2.b.statusUpdateEn)
{
    updateStatusVariables(pMC_App);
}
}
/******************************************************************************/
static void motorStateSet(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_STATUS_INTERFACE_T *pOutput = pMC_App->pAppInterface->pUserOutputs;
    LOAD_T *pLoad = &(pMC_App->load);

        switch(pMC_App->state)
        {
        case APP_INIT:
        case APP_IDLE:
        case APP_OFFSET_CALIBRATION:
            pOutput->motorState = MOTOR_IDLE;

            if(pLoad->isd.bemfMag < pLoad->isd.maxStopBemf)
            {
                pMC_App->flags.b.motorRun = FALSE;
            }

            if(pLoad->isd.bemfMag > (pLoad->isd.maxStopBemf << 1))
            {
                pMC_App->flags.b.motorRun = TRUE;
            }

            break;

        case APP_START:

            switch(pMC_App->load.state)
            {
            case LOAD_ISD:
                pOutput->motorState = MOTOR_ISD;

                break;

            case LOAD_TRISTATE:
                pOutput->motorState = MOTOR_TRISTATE;

                break;

            case LOAD_BRAKE:
                pOutput->motorState = MOTOR_BRAKE_ON_START;

                break;
            default:
                break;

            }

            break;

            case APP_RUN:

                switch(pMC_App->foc.state)
                {
                case FOC_ROTOR_ALIGN:
                    pOutput->motorState = MOTOR_ALIGN;

                    break;

                case FOC_OPEN_LOOP:
                    if(pMC_App->foc.openLoop.slowFirstCycle.
                            flags.b.enable &&
                            !pMC_App->foc.openLoop.slowFirstCycle.
                            flags.b.status)
                    {
                        pOutput->motorState = MOTOR_SLOW_FIRST_CYCLE;
                    }
                    else
                    {
                        pOutput->motorState = MOTOR_OPEN_LOOP;
                    }

                    break;

                case FOC_CLOSE_LOOP:
                    switch(pMC_App->foc.closeLoop.commutationState)
                    {
                    case COMMUTATION_UNALIGNED:
                        pOutput->motorState =
                                MOTOR_CLOSE_LOOP_UNALIGNED;
                        break;

                    case COMMUTATION_ALIGNED:
                        pOutput->motorState =
                                MOTOR_CLOSE_LOOP_ALIGNED;
                        break;

                    }
                    break;
                    default:
                        break;
                }

                break;

                case APP_STOP:

                    switch(pMC_App->load.state)
                    {
                    case LOAD_SOFT_STOP:
                        pOutput->motorState = MOTOR_SOFT_STOP;
                        break;

                    case LOAD_BRAKE:
                        pOutput->motorState = MOTOR_BRAKE_ON_STOP;
                        break;
                    default:
                        break;
                    }
                    break;

                    case APP_BRAKE:
                        if(pMC_App->foc.rotorAlign.flags.b.enableOnBrake)
                        {
                            pOutput->motorState = MOTOR_BRAKE_ON_STOP;
                        }
                        else
                        {
                            pOutput->motorState = MOTOR_BRAKE_ON_STOP;
                        }

                        break;

                    case APP_FAULT:

                        pOutput->motorState = MOTOR_FAULT;
                        break;
                    default:
                        break;
        }

}

static void controllerFaultsClear(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    STALL_DETECT_T *pStallDetect = &(pMC_App->foc.stallDetect);
    LOAD_T *pLoad = &(pMC_App->load);
    SOURCE_LIMITS_T *pSourceLimits = &(pMC_App->sourceLimits);
    FAULT_SERVICE_T *pFaultService = &(pMC_App->faultService);
    SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit =
            &(pMC_App->sourceLimits.sourceVoltageLimit);

    /* Clear FOC faults */
    pStallDetect->abnormalSpeedStall.flags.b.status = FALSE;
    pStallDetect->lowBemfStall.flags.b.status = FALSE;
    pStallDetect->noMotorStall.flags.b.status = FALSE;

    pStallDetect->noMotorStall.flags.b.phaseADisconnect = FALSE;
    pStallDetect->noMotorStall.flags.b.phaseBDisconnect = FALSE;
    pStallDetect->noMotorStall.flags.b.phaseCDisconnect = FALSE;

    pStallDetect->status = NO_STALL;

    /* Clear Load Faults */
    pLoad->ipd.faultStatus = IPD_NO_FAULT;

    pLoad->faultStatus = LOAD_NO_FAULT;

    /* Clear enables on faults */
    pLoad->brake.flags.b.enableOnFault = FALSE;

    /* Clear source limits faults */
    if(pMC_App->motorInputs.voltage.vdcFilt <
            pSourceVoltageLimit->overVoltageFaultClearThreshold)
    {
        pSourceVoltageLimit->flags.b.overVoltageFaultStatus = FALSE;
    }
    if(pMC_App->motorInputs.voltage.vdcFilt >
    pSourceVoltageLimit->underVoltageFaultClearThreshold)
    {
        pSourceVoltageLimit->flags.b.underVoltageFaultStatus = FALSE;
    }

    pSourceLimits->overCurrent.flags.b.status = FALSE;

    pMC_App->faultStatus = NO_FAULT;

    pFaultService->flags.b.clearFault = FALSE;
}
void updateConfigsInit(void)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T *)g_pMC_App;
    USER_STATUS_INTERFACE_T *pUserOutputs = (pMC_App->pAppInterface->pUserOutputs);
    FOC_T *pFOC = &(pMC_App->foc);
    CLOSE_LOOP_T *pCloseLoop = &(pFOC->closeLoop);

    while(!g_pAppInterface->userInputs.debugFlags.b.updatedData)
    {
        gateDriverParamsUpdate(pGateDriverConfig);

        user_config_faults(pMC_App);
        updateConfigurationParameters(pMC_App);
        updateRAMConfigurationParameters(pMC_App);
        g_pAppInterface->userInputs.debugFlags.b.updatedData = TRUE;
        g_pAppInterface->flags.b.shadowToAlgorithmSet = FALSE;
    }   
}

static void updateStatusVariables(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    pUserStatusRegs->systemFaultStatus = (USER_FAULT_TYPES )pMC_App->faultStatus;

    pUserStatusRegs->piSpeed.feedback =
                            pMC_App->angleTrackingPLLEstim.velocityFilt;

    pUserStatusRegs->piSpeed.reference = *(pMC_App->foc.closeLoop.piSpeed.pReference);

    if(pMC_App->foc.closeLoop.controlMode == CONTROL_POWER)
    {
        pUserStatusRegs->piPower.feedback =
                                pMC_App->foc.closeLoop.PowerFeedback;

        pUserStatusRegs->piPower.reference =
                                pMC_App->foc.closeLoop.PowerReference;
    }
    else
    {
        pUserStatusRegs->piPower.feedback = 0;
        pUserStatusRegs->piPower.reference = 0;
    }
    pUserStatusRegs->piIq.feedback = (pMC_App->foc.idq.q);
    pUserStatusRegs->piIq.reference = *(pMC_App->foc.piIq.pReference);

    pUserStatusRegs->piId.feedback = pMC_App->foc.idq.d;
    pUserStatusRegs->piId.reference = *(pMC_App->foc.piId.pReference);

    pUserStatusRegs->ipdIdentifiedSector = (IPD_IDENTIFIED_SECTOR_T)pMC_App->load.ipd.noRotationVectorCount;

    pUserStatusRegs->estimatedSpeed = pMC_App->angleTrackingPLLEstim.velocityFilt;

    pUserStatusRegs->dcBusVoltage = pMC_App->motorInputs.voltage.vdcFilt;

    pUserStatusRegs->torqueLimit = pMC_App->foc.closeLoop.piSpeed.outMax;
    
    pUserStatusRegs->appVersion.w = APP_FW_VERSION;
}

void updateConfigs(void)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T *)g_pMC_App;

    if(pMC_App->pAppInterface->flags.b.shadowToAlgorithmSet)
    {
        if((pMC_App->state == APP_IDLE) || (pMC_App->state == APP_FAULT) ||
                            (pMC_App->state == APP_INIT))
        {
            updateConfigurationParameters(pMC_App);
            pUserCtrlRegs->algoDebugCtrl2.b.updateConfigs = 0;
        }

        /* Update the System Parameters dynamically*/
        if(pUserCtrlRegs->algoDebugCtrl2.b.updateSysParams)
        {
            update_SystemParams(&pMC_App->pAppInterface->userInputs);
            updateCloseLoopConfigParam(pMC_App);
            updateCurrentControlConfigParam(pMC_App);
        }

        updateRAMConfigurationParameters(pMC_App);

        pMC_App->pAppInterface->userInputs.debugFlags.b.updatedData = TRUE;
        pMC_App->pAppInterface->flags.b.shadowToAlgorithmSet = FALSE;
    }

    if(pMC_App->pAppInterface->flags.b.hvDieConfigSet)
    {
        if((pMC_App->state == APP_IDLE) || (pMC_App->state == APP_FAULT) ||
                (pMC_App->state == APP_INIT))
        {

            gateDriverParamsUpdate(pGateDriverConfig);
        }

        pMC_App->pAppInterface->flags.b.hvDieConfigSet = FALSE;
    }

    if(pMC_App->pAppInterface->flags.b.faultsResponseSet)
    {
        /* read fault status from HVDie and controller */

        /* Update Gate Driver Faults Raw Fault Status */
        gateDriverRawFaultStatus = gateDriverGetFaultStatus();

        /* fault response if there is any active HVDie fault */
        gateDriverFaultResponse(pGateDriverConfig);

        update_FOC_faultStatus(pMC_App);  /* Verify fault status */

        /* report fault status based on user configuration */
        FOC_faultReport(pMC_App, &pMC_App->faultService);

        /* faults recovery flow (clear faults and fault retry) */
        FOC_fault_Recovery(pMC_App, &pMC_App->faultService);

        /* update fault configuration once in 100ms  */
        user_config_faults(pMC_App);

        /* Reset fault if not active */
        if(((pMC_App->pAppInterface->pUserOutputs->controllerFaultStatus & 0x7FFFFFFF) == 0) &&
               ((pMC_App->pAppInterface->pUserOutputs->gateDriverFaultStatus & 0x7FFFFFFF) == 0))
        {
           pMC_App->faultStatus = NO_FAULT;
        }
        pMC_App->pAppInterface->flags.b.faultsResponseSet = FALSE;

    }
}
void appReset(void *gpMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*)gpMC_App;

    memset(pMC_App, 0, sizeof(UNIVERSAL_FOC_APPLICATION_T));

    pMC_App->pAppInterface = g_pAppInterface;

    /* Report all controller faults */
    controllernFaultReport = CONTROLLER_FAULT_REPORT_DEFAULT;

    controllerFaultAction = CONTROLLER_FAULT_ACTION_DEFAULT;

    controllerFaultActionLatched = CONTROLLER_FAULT_ACTION_LATCHED_DEFAULT;
}

void applicationIPDCurrThresh(void *gpMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*)gpMC_App;
    IPD_T *pIpd = &(pMC_App->load.ipd);
    if(pIpd->state == IPD_RUN)
    {
        IPDCurrThreshAction(pIpd);
    }
}

void applicationIPDOverFlow(void *gpMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*)gpMC_App;
    IPD_T *pIpd = &(pMC_App->load.ipd);
    IPDOverFlowAction(pIpd);
}

uint32_t getMCAppSize(void)
{
    return (sizeof(UNIVERSAL_FOC_APPLICATION_T));
}

/******************************************************************************/

