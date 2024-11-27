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

#include "foc.h"
#include "focHALInterface.h"
#include "main.h"
#include "appDefs.h"
#include "measure.h"
#include "slewRate.h"

#define ROTOR_ALIGN_TO_OPEN_LOOP_ANGLE_OFFSET   _IQ(90.0/360.0)

static void focFeedback(FOC_T *);
static void focForward(FOC_T *);

void focInit(FOC_T *pFOC)
{
    piInit(&pFOC->piId);
    piInit(&pFOC->piIq);
    rotorAlignInit(&pFOC->rotorAlign);
    openLoopInit(&pFOC->openLoop);
    closeLoopInit(&pFOC->closeLoop);
    stallDetectInit(&pFOC->stallDetect);
    eSMOInit(&pFOC->esmo);
    pFOC->svm.maxDutyPhase = PHASE_INVALID;
    pFOC->svm.minDutyPhase = PHASE_INVALID;
    pFOC->svm.voltageSector = SECTOR_INVALID;
    pFOC->svm.prevVoltageSector = SECTOR_INVALID;

    pFOC->svm.Dabc.a = 0;
    pFOC->svm.Dabc.b = 0;
    pFOC->svm.Dabc.c = 0;

    pFOC->flags.b.voltageModeEnable = FALSE;

}

void focRun(FOC_T *pFOC)
{
    _Bool focForwardEnable = FALSE, avsStatus;

    int32_t angleOffset;

    focFeedback(pFOC);

    ROTOR_ALIGN_T *pRotorAlign = &(pFOC->rotorAlign);
    OPEN_LOOP_T *pOpenLoop = &(pFOC->openLoop);
    CLOSE_LOOP_T *pCloseLoop = &(pFOC->closeLoop);
    STALL_DETECT_T *pStallDetect = &(pFOC->stallDetect);

    int32_t targetVelocity = *(pRotorAlign->pTargetVelocity), iqRefSetLimited;

    switch(pFOC->state)
    {
    case FOC_INIT:

        focInit(pFOC);

        pRotorAlign->countMax = pRotorAlign->countMaxStart;

        /* Select rotor align IdRef, IqRef and angle */
        pFOC->piId.pReference = &(pRotorAlign->idRef);
        pFOC->piIq.pReference = &(pRotorAlign->iqRef);

        pFOC->pFOCAngle = &(pRotorAlign->angle);

        pFOC->state = FOC_ROTOR_ALIGN;

        break;

    case FOC_ROTOR_ALIGN:

        focForwardEnable = TRUE;

        iqRefSetLimited = pRotorAlign->iqRefSet;

        if(targetVelocity > 0)
        {
            pRotorAlign->iqRefMax = iqRefSetLimited;
        }
        else
        {
            pRotorAlign->iqRefMax = -iqRefSetLimited;
        }

        /* Slew rate for current reference */
        slewRate(&pFOC->rotorAlign.iqRef,
                 pFOC->rotorAlign.iqRefMax,
                 pFOC->rotorAlign.iqRefSlewRate);

        if(getStallStatusForAction(&pFOC->stallDetect))
        {
            pFOC->state = FOC_STALL;
        }

        if(getRotorAlignStatus(&pFOC->rotorAlign))
        {
            if(pRotorAlign->flags.b.enableOnStop)
            {
                /* Align on Stop is completed - Disable PWM */
                HAL_DisablePWM();
            }
            else
            {
                /* Align at Startup is completed - go to open loop */

                /* Initialize Open Loop */
                openLoopInit(&pFOC->openLoop);

                pOpenLoop->iqRef = pRotorAlign->iqRef;

                /* Set Open Loop initial velocity */
                openLoopSetInitialVelocity(&pFOC->openLoop);

                pOpenLoop->angle = pRotorAlign->angle;

                /* Initialize acceleration parameters */
                pOpenLoop->pSpeedRefSlewRate =
                        &(pOpenLoop->speedRefSlewRateFastAccel);

                pOpenLoop->pAccelRefSlewRate =
                        &(pOpenLoop->accelRefSlewRateFastAccel);

                pOpenLoop->iqRefSet =
                        pOpenLoop->iqRefSetFastAccel;


                /* Select open loop IdRef, IqRef and angle */
                pFOC->piId.pReference = &(pOpenLoop->idRef);
                pFOC->piIq.pReference = &(pOpenLoop->iqRef);

                pFOC->pFOCAngle = &(pOpenLoop->angle);

                pFOC->pVelocityReference =
                        &(pOpenLoop->velocityReference);

                pFOC->state = FOC_OPEN_LOOP;
            }
        }

        if(getRotorForceAlignStatus(&pFOC->rotorAlign))
        {
            pFOC->state = FOC_STOP;
        }

        break;

    case FOC_OPEN_LOOP:

        focForwardEnable = TRUE;
#ifdef ESMO_ESTIMATOR
        /* esmo estimator angle offset */
        angleOffset = pOpenLoop->angle -
                        pFOC->esmo.pAngleTrackingPLLEstim->fluxAngle;
#else
        /* Finite difference Bemf estimator angle offset */
        angleOffset = pOpenLoop->angle -
                pFOC->estim.pAngleTrackingPLLEstim->fluxAngle;
#endif

        if(getStallStatusForAction(&pFOC->stallDetect))
        {
            pFOC->state = FOC_STALL;
        }
        else
        {
            openLoopRun(&pFOC->openLoop);
        }

        /* Ensure proper voltage vector is applied at zero cross */
        if(getOpenLoopVelocityZeroCross(&pFOC->openLoop))
        {
            pFOC->piId.kiTerm = -pFOC->piId.kiTerm;
            pFOC->piIq.kiTerm = -pFOC->piIq.kiTerm;
        }

        if(getOpenLoopStatus(&pFOC->openLoop))
        {
            /* Initialize Close Loop */
            closeLoopInit(&pFOC->closeLoop);

            if(pCloseLoop->controlMode == CONTROL_SPEED)
            {
                /* Initialize speed reference */
                pCloseLoop->velocityReference =
                        pOpenLoop->velocityReference;
            }
            else if(pCloseLoop->controlMode == CONTROL_TORQUE)
            {   /* Initialize speed reference */
                pCloseLoop->velocityReference = _IQdiv_mathacl(pOpenLoop->iqRef,
                                        pCloseLoop->torqRefScalingFactor);

            }
            else
            {
                /* This is intentionally kept empty */
            }

            /* Calculate the angle offset */
            pCloseLoop->angleOffset = angleOffset;

            /* Initialize the speed PI integrator */
            pCloseLoop->piSpeed.kiTerm = pOpenLoop->iqRef;

            pCloseLoop->idqRef.q = pOpenLoop->iqRef;
            pCloseLoop->idqRef.d = pOpenLoop->idRef;
            pCloseLoop->angle = pOpenLoop->angle;

            /* Set the Max Torque Limit in Openloop */
            pCloseLoop->iqRefMax = pCloseLoop->iRefMax;

            /* Disable No Motor stall */
            pStallDetect->noMotorStall.flags.b.enable = FALSE;

            /* Select close loop IdRef, IqRef and angle */
            pFOC->piId.pReference = &(pCloseLoop->idqRef.d);
            pFOC->piIq.pReference = &(pCloseLoop->idqRef.q);

            pFOC->pFOCAngle = &(pCloseLoop->angle);

            pFOC->pVelocityReference =
                    &(pCloseLoop->velocityReference);

            pFOC->state = FOC_CLOSE_LOOP;
        }

        /* Align Brake */
        if((pRotorAlign->flags.b.enableOnStop ||
                pRotorAlign->flags.b.enableOnBrake))
        {
            pRotorAlign->count = 0;
            pRotorAlign->flags.b.status = FALSE;
            pRotorAlign->iqRef = pOpenLoop->iqRef;
            pRotorAlign->idRef = pOpenLoop->idRef;
            pRotorAlign->state = ALIGN_INIT;

            pRotorAlign->angle = pOpenLoop->angle;

            pRotorAlign->countMax = pRotorAlign->countMaxStop;

            /* Disable No Motor Stall Detection */
            pStallDetect->noMotorStall.flags.b.enable = FALSE;

            /* Disable Low BEMF Stall Detection */
            pStallDetect->lowBemfStall.flags.b.enable = FALSE;

            /* Disable Abnormal Speed Stall Detection */
            pStallDetect->abnormalSpeedStall.flags.b.enable = FALSE;

            /* Select rotor align IdRef, IqRef and angle */
            pFOC->piId.pReference = &(pRotorAlign->idRef);
            pFOC->piIq.pReference = &(pRotorAlign->iqRef);

            pFOC->pFOCAngle = &(pRotorAlign->angle);

            pFOC->state = FOC_ROTOR_ALIGN;
        }

        break;

    case FOC_CLOSE_LOOP:

        focForwardEnable = TRUE;

        /* Stop the motor in case of stall, otherwise, continue running */
        if(getStallStatusForAction(&pFOC->stallDetect))
        {
            pFOC->state = FOC_STALL;
        }
        else
        {
            closeLoopRun(&pFOC->closeLoop);

            /* AVS: Disable PWM if power is being supplied to dc bus */
            avsStatus = getAvsStatus(pCloseLoop);

            if(avsStatus && pCloseLoop->sourceCurrentFeedback <
                    AVS_MIN_SOURCE_CURRENT_PU)
            {
                pFOC->flags.b.voltageModeEnable = FALSE;

                pFOC->vdqForce.d =
                        pFOC->estim.pAngleTrackingPLLEstim->Edq.d;

                pFOC->vdqForce.q =
                        pFOC->estim.pAngleTrackingPLLEstim->Edq.q;
            }
            else
            {
                pFOC->flags.b.voltageModeEnable = FALSE;
            }
        }

        /* Align Brake */
        if((pRotorAlign->flags.b.enableOnStop ||
                pRotorAlign->flags.b.enableOnBrake))
        {
            pRotorAlign->count = 0;
            pRotorAlign->flags.b.status = FALSE;
            pRotorAlign->iqRef = pCloseLoop->idqRef.q;
            pRotorAlign->idRef = pCloseLoop->idqRef.d;
            pRotorAlign->state = ALIGN_INIT;

            pRotorAlign->angle = pCloseLoop->angle;
            pRotorAlign->countMax = pRotorAlign->countMaxStop;

            /* Disable No Motor Stall Detection */
            pStallDetect->noMotorStall.flags.b.enable = FALSE;

            /* Disable Low BEMF Stall Detection */
            pStallDetect->lowBemfStall.flags.b.enable = FALSE;

            /* Disable Abnormal Speed Stall Detection */
            pStallDetect->abnormalSpeedStall.flags.b.enable = FALSE;

            /* Select rotor align IdRef, IqRef and angle */
            pFOC->piId.pReference = &(pRotorAlign->idRef);
            pFOC->piIq.pReference = &(pRotorAlign->iqRef);

            pFOC->pFOCAngle = &(pRotorAlign->angle);

            pFOC->state = FOC_ROTOR_ALIGN;
        }

        break;

    case FOC_STALL:
        /* This is intentionally kept empty */
        break;

    case FOC_STOP:
        HAL_DisablePWM();
        break;

    default:
        pFOC->state = FOC_INIT;
        break;
    }

    if(focForwardEnable)
    {
        focForward(pFOC);
    }

}

static void focFeedback(FOC_T *pFOC)
{
    MC_ClarkeTransformNoZeroSeq(&pFOC->iAlphaBeta, pFOC->pIabc);
    MC_ParkTransform(&pFOC->idq, &pFOC->iAlphaBeta, &pFOC->sinCos);
#ifdef ESMO_ESTIMATOR
    /* eSMO estimator */
    eSMORun(&pFOC->esmo);
#else
    /* Finite difference Bemf estimator */
    estimatorRun(&pFOC->estim);
#endif
    stallDetectRun(&pFOC->stallDetect);

}

static void focForward(FOC_T *pFOC)
{

    int32_t mqMaxSqr, vqMax, mqMax;

    MC_ALPHABETA_T mAlphaBetaEstim;

    if(!(pFOC->flags.b.voltageModeEnable ||
            pFOC->flags.b.voltageModeEnableSet ||
            (pFOC->closeLoop.controlMode == CONTROL_VOLTAGE&&
                    pFOC->state == FOC_CLOSE_LOOP &&
                    pFOC->closeLoop.commutationState ==
                            COMMUTATION_ALIGNED)))
    {
        /* Current control is expected */

        piRun(&pFOC->piId);
        pFOC->vdq.d = pFOC->piId.out;
        pFOC->mdq.d = _IQmpy_mathacl(pFOC->vdq.d, *pFOC->pInvVdc) <<
                (GLOBAL_IQ - INV_VDC_IQ_SCALING);

        if(pFOC->modIndexLimit == MOD_INDEX_LIMIT_SINE)
        {
            mqMaxSqr = _IQ(1.0) - _IQmpy_mathacl(pFOC->mdq.d, pFOC->mdq.d);

            mqMax = _IQsqrt_mathacl(mqMaxSqr);
            vqMax = _IQmpy_mathacl(mqMax, *pFOC->pVdc);

            pFOC->piIq.outMax = vqMax;
            pFOC->piIq.outMin = -vqMax;
        }

        piRun(&pFOC->piIq);
        pFOC->vdq.q = pFOC->piIq.out;
        pFOC->mdq.q = _IQmpy_mathacl(pFOC->vdq.q, *pFOC->pInvVdc) <<
                (GLOBAL_IQ - INV_VDC_IQ_SCALING);

        if(pFOC->closeLoop.controlMode == CONTROL_VOLTAGE)
        {
            pFOC->closeLoop.velocityReference = _IQdiv_mathacl(pFOC->mdq.q,
                                    pFOC->closeLoop.mdqForceSinCos.cos);
        }
    }
    else
    {
        /* Voltage mode is expected */
        if(pFOC->flags.b.voltageModeEnable)
        {
            /* Voltage mode set by algorithm */
            pFOC->vdq.d = pFOC->vdqForce.d;
            pFOC->mdq.d = _IQmpy_mathacl(pFOC->vdq.d, *pFOC->pInvVdc) <<
                    (GLOBAL_IQ - INV_VDC_IQ_SCALING);

            pFOC->vdq.q = pFOC->vdqForce.q;
            pFOC->mdq.q = _IQmpy_mathacl(pFOC->vdq.q, *pFOC->pInvVdc) <<
                    (GLOBAL_IQ - INV_VDC_IQ_SCALING);
        }
        else if(pFOC->closeLoop.controlMode == CONTROL_VOLTAGE &&
                pFOC->state == FOC_CLOSE_LOOP && (pFOC->closeLoop.commutationState ==
                        COMMUTATION_ALIGNED) )
        {
            pFOC->mdq.q = _IQmpy_mathacl(pFOC->closeLoop.velocityReference,
                                 pFOC->closeLoop.mdqForceSinCos.cos);
            pFOC->mdq.d = - (_IQmpy_mathacl(pFOC->closeLoop.speedReference,
                                 pFOC->closeLoop.mdqForceSinCos.sin));
        }
        else
        {
            /* Voltage mode set by user */
            pFOC->mdq.d = pFOC->mdqForceSet.d;
            pFOC->mdq.q = pFOC->mdqForceSet.q;
        }

        pFOC->piId.kiTerm = _IQmpy_mathacl(pFOC->mdq.d, *pFOC->pVdc);
        pFOC->piIq.kiTerm = _IQmpy_mathacl(pFOC->mdq.q, *pFOC->pVdc);
    }

    MC_SinCos(&pFOC->sinCos, *pFOC->pFOCAngle);

    MC_InverseParkTransform(&pFOC->mAlphaBeta, &pFOC->mdq, &pFOC->sinCos);

    MC_InverseClarkeTransform(&pFOC->mabc, &pFOC->mAlphaBeta);

    MC_SpaceVectorGen(&pFOC->svm, &pFOC->mabc);

    if(pFOC->modIndexLimit == MOD_INDEX_LIMIT_SINE)
    {
        mAlphaBetaEstim = pFOC->mAlphaBeta;
        pFOC->ddq = pFOC->mdq;
    }
    else
    {
        MC_SVM_ClarkeTransform(&pFOC->dAlphaBeta, &pFOC->svm.Dabc);

        mAlphaBetaEstim = pFOC->dAlphaBeta;

        MC_ParkTransform(&pFOC->ddq, &pFOC->dAlphaBeta, &pFOC->sinCos);
    }

    pFOC->vAlphaBeta.alpha = _IQmpy_mathacl(mAlphaBetaEstim.alpha, *pFOC->pVdc);
    pFOC->vAlphaBeta.beta = _IQmpy_mathacl(mAlphaBetaEstim.beta, *pFOC->pVdc);
}

void focLowPriorityRun(FOC_T *pFOC)
{
    ABNORMAL_SPEED_STALL_DETECT_T *pAbnormalSpeedStall;
    LOW_BEMF_STALL_DETECT_T *pLowBemfStall;
    NO_MOTOR_STALL_DETECT_T *pNoMotorStall;

    OPEN_LOOP_T *pOpenLoop;
    CLOSE_LOOP_T *pCloseLoop;

    int32_t vdqMax, speedReference = 0, currentReferenceSqrMax = 0;

    pAbnormalSpeedStall = &(pFOC->stallDetect.abnormalSpeedStall);
    pLowBemfStall = &(pFOC->stallDetect.lowBemfStall);
    pNoMotorStall = &(pFOC->stallDetect.noMotorStall);
    pOpenLoop = &(pFOC->openLoop);
    pCloseLoop = &(pFOC->closeLoop);

    vdqMax = _IQmpy(pFOC->modIndexLimit, *pFOC->pVdc);

    pCloseLoop->currentReferenceSqrMax =
                pCloseLoop->currentReferenceSqrMaxSet;

    /* Limit motor current */
    pCloseLoop->iRefMax = (_IQsqrt(pCloseLoop->currentReferenceSqrMax));

    pFOC->piId.outMax = vdqMax;
    pFOC->piId.outMin = -vdqMax;

    if(pFOC->modIndexLimit == MOD_INDEX_LIMIT_OVERMODULATION)
    {
        pFOC->piIq.outMax = vdqMax;
        pFOC->piIq.outMin = -vdqMax;
    }

    getStallStatusForReport(&pFOC->stallDetect);

    switch(pFOC->state)
    {
    case FOC_INIT:
        /* This is intentionally kept empty */
        break;

    case FOC_ROTOR_ALIGN:

        rotorAlignRun(&pFOC->rotorAlign);

        /* Calculate number of samples in 1 electrical cycle */
        pFOC->electricalCycleCountMax = _IQ(1.0);

        /* Disable No Motor Stall Detection */
        pNoMotorStall->flags.b.enable = FALSE;

        /* Disable Low BEMF Stall Detection */
        pLowBemfStall->flags.b.enable = FALSE;

        /* Disable Abnormal Speed Stall Detection */
        pAbnormalSpeedStall->flags.b.enable = FALSE;

        /* Check for low priority stalls */
        stallDetectLowPriorityRun(&pFOC->stallDetect);

        break;

    case FOC_OPEN_LOOP:

        /* Calculate number of samples in 1 electrical cycle */
        speedReference = _IQabs(pOpenLoop->velocityReference);

        /* Disable Abnormal Speed Stall Detection */
        pAbnormalSpeedStall->flags.b.enable = FALSE;

        /* Check for low priority stalls */
        stallDetectLowPriorityRun(&pFOC->stallDetect);



        /* Disable Low BEMF Stall Detection */
        pLowBemfStall->flags.b.enable = FALSE;

        /* Set No Motor Stall Detection as set by user */
        if(pOpenLoop->slowFirstCycle.flags.b.inProgress)
        {
            pNoMotorStall->flags.b.enable = FALSE;
        }
        else
        {
            if(_IQabs(pOpenLoop->iqRef) >
            pNoMotorStall->minimumCurrent)
            {
                pNoMotorStall->flags.b.enable =
                        pNoMotorStall->flags.b.enableSet;
            }
        }


        break;

    case FOC_CLOSE_LOOP:

        /* Disable No Motor Stall Detection during Close Loop */
        pNoMotorStall->flags.b.enable = FALSE;


        /* Calculate number of samples in 1 electrical cycle */
        if(pCloseLoop->flags.b.torqueModeEnable)
        {
            speedReference =
                    pFOC->estim.pAngleTrackingPLLEstim->speedFilt;
        }
        else
        {
            speedReference = pCloseLoop->speedReference;
        }

        closeLoopLowPriorityRun(&pFOC->closeLoop);

        if(pCloseLoop->controlMode == CONTROL_TORQUE)
        {
            /* Torque Mode current referencing */
            pCloseLoop->idqRef.q = _IQmpy(pCloseLoop->velocityReference, pCloseLoop->torqRefScalingFactor);
            if(pCloseLoop->idqRef.q >= pCloseLoop->iqRefMax)
            {
                pCloseLoop->idqRef.q = pCloseLoop->iqRefMax;
            }
            else if(pCloseLoop->idqRef.q <= -pCloseLoop->iqRefMax)
            {
                pCloseLoop->idqRef.q = -pCloseLoop->iqRefMax;
            }

            pCloseLoop->piSpeed.kiTerm = pCloseLoop->idqRef.q;
        }
        else if(pCloseLoop->controlMode == CONTROL_POWER)
        {
            pCloseLoop->PowerReference = _IQmpy(pCloseLoop->velocityReference,pCloseLoop->maximumPower);
            if(*(pCloseLoop->pVelocity) >= 0)
            {
                pCloseLoop->PowerFeedback = _IQmpy(*pCloseLoop->psourceCurrentFeedbackFilt,*pFOC->pVdc);
            }
            else
            {
                pCloseLoop->PowerFeedback = -_IQmpy(*pCloseLoop->psourceCurrentFeedbackFilt,*pFOC->pVdc);
            }
        }
        else if(pCloseLoop->controlMode == CONTROL_VOLTAGE)
        {
            pCloseLoop->piSpeed.kiTerm = pFOC->idq.q;
        }
        else
        {
            /* This is intentionally kept empty */
        }

        if(getCloseLoopAlignedStatus(&pFOC->closeLoop))
        {
            /* Set Low BEMF Stall Detection as set by user */
            pLowBemfStall->flags.b.enable =
                    pLowBemfStall->flags.b.enableSet;

            /* Set Abnormal Speed Stall Detection as set by user */
            pAbnormalSpeedStall->flags.b.enable =
                    pAbnormalSpeedStall->flags.b.enableSet;

        }
        else
        {
            /* Disable Low BEMF stall detection in active brake */
            pLowBemfStall->flags.b.enable = FALSE;
        }

        /* Check for low priority stalls */
        stallDetectLowPriorityRun(&pFOC->stallDetect);

        break;

    case FOC_STOP:
        /* This is intentionally kept empty */
        break;
    default:
        break;
    }

    pFOC->electricalCycleCountMax =
            _IQdiv(1, _IQmpy(speedReference,
                          *pFOC->estim.pAngleTrackingPLLEstim->pDeltaT));


    /* Low Bemf cycles */
    pLowBemfStall->countMax = (10 * pFOC->electricalCycleCountMax);

}
