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
/*!****************************************************************************
 *  @file       closeLoop.h
 *  @brief      closeLoop Module
 *
 * 
 *  @anchor closeLoop_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/

#ifndef FOC_INCLUDE_CLOSELOOP_H_
#define FOC_INCLUDE_CLOSELOOP_H_

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "pi.h"
#include "stdint.h"
#include "idReferencing.h"
#include "mcLib.h"
#include "iqNum.h"
#include "measure.h"
#include "main.h"

/*! @brief Speed Error Thresold */
#define SPEED_ERROR_THRESHOLD_PU        0.05
/*! @brief Speed command Threshold */
#define SPEED_CMD_THRESHOLD             _IQ(0.04)
/*! @brief  Transition Parameter */
#define TRANSITION_SIN_ANGLE_ALIGN_THRESHOLD        _IQ(0.08715574275)

/*! @brief Close Loop Flags */
typedef union
{
    /*! @brief Close Loop Flags Structure */

    struct closeLoopFlags
    {
        _Bool
            /*! Enables torque mode */        
            torqueModeEnable: 1,
            /*! Enables the close loop to open loop transition */            
            transitionEnable: 1,
            /*! Enables Anti Voltage Surge */            
            avsEnable: 1;

    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}CLOSE_LOOP_FLAGS_T;

/*! @brief Commutation States */
typedef enum
{
    /*! Commutation not aligned to estimator */
    COMMUTATION_UNALIGNED = 0,
    /*! Commutation aligned to estimator */
    COMMUTATION_ALIGNED = 1,

}COMMUTATION_STATE_TYPES;

/*! @brief Transition States */
typedef enum
{
    /*! Speed reference: undefined */
    CLOSE_LOOP_UNDEFINED_STATE = 0,

    /*! Speed reference reached steady state */
    CLOSE_LOOP_STEADY_STATE = 1,

    /*! Speed reference: accelerate */
    CLOSE_LOOP_ACCELERATE = 2,

    /*! Speed reference: decelerate */
    CLOSE_LOOP_DECELERATE = 3,

}SPEED_CHANGE_STATE_TYPES;

/*! @brief Control types */
typedef enum
{
    /*! Speed Control*/
    CONTROL_SPEED = 0,
    /*! Power Control*/
    CONTROL_POWER = 1,
    /*! Torque Control*/
    CONTROL_TORQUE = 2,
    /*! Voltage Control*/
    CONTROL_VOLTAGE = 3,
}APP_CONTROL_TYPES;

/*! @brief Closed Loop Working variables */
typedef struct
{
    /*! Closed Loop inputs */

    const int32_t
    /*! Target Speed including direction */
        *pTargetVelocity,
        /*! Estimator Angle */
        *pFluxAngle,
        /*! Estimator Velocity */
        *pVelocity,
        /*! Estimator Speed */
        *pSpeed,
        /*! DC bus Current */
        *psourceCurrentFeedbackFilt;

    const MC_ABC_T
    /*! Modulation Index */
        *pMabc,
        /*! Duty */
        *pDabc,
        /*! Current */
        *pIabc;

    /*! Closed Loop parameters for processing */

    COMMUTATION_STATE_TYPES
    /*! Commutation state */
        commutationState;

    SPEED_CHANGE_STATE_TYPES
    /*! Speed change state */
        speedChangeState;

    int32_t
        /*! Maximum Phase Current Peak Reference*/
        iRefMax,
        /*! Maximum Q-axis current reference */
        iqRefMax,
        /*! Bounded Target Velocity */
        targetVelocityLimited,
        /*! Bounded Target Speed */
        targetSpeedLimited,
        /*! Slewed velocity reference */
        velocityReference,
        /*! Slewed speed reference */
        speedReference,
        /*! Slewed Power Reference*/
        PowerReference,
        /*! Power Feedback */
        PowerFeedback,
        /*! Maximum Power*/
        maximumPower,
        /*! Acceleration slew rate */
        accelSpeedRefSlewRate,
        /*! Deceleration slew rate */
        decelSpeedRefSlewRate,
        /*! Slow Acceleration slew rate: unaligned */
        slowAccelSpeedRefSlewRate,
        /*! Slow Deceleration slew rate: unaligned */
        slowDecelSpeedRefSlewRate,
        /*! Fast Acceleration slew rate: aligned */
        fastAccelSpeedRefSlewRate,
        /*! Fast Deceleration slew rate: aligned */
        fastDecelSpeedRefSlewRate,
        /*! Upper limit on speed reference */
        velocityReferenceMax,
        /*! Lower limit on speed reference */
        velocityReferenceMin,
        /*! Commutation angle */
        angle,
        /*! Angle difference between open loop angle and estimator angle */
        angleOffset,
        /*! Step size for angle alignment */
        angleAlignStep,
        /*! Maximum current reference squared set */
        currentReferenceSqrMaxSet,
        /*! Maximum Current Reference Squared */
        currentReferenceSqrMax,
        /*! Source current feedback calculated */
        sourceCurrentFeedback,
        /*! Modulation Index Squared */
        modIndexSqr,
        /*! Modulation Index Squared Limit for active brake */
        modIndexSqrLimit,
        /*! Scaling for target reference in torque control mode*/
        torqRefScalingFactor;

    MC_DQ_T
    /*! DQ domain current reference */
        idqRef;

    ID_REF_T
    /*! Id reference generator */
        idRefGen;

    PI_T
    /*! PI for speed loop */
        piSpeed;

    CLOSE_LOOP_FLAGS_T
    /*! Flags */
        flags;

    APP_CONTROL_TYPES
    /*! Control mode */
        controlMode;

    MC_SINCOS_T
    /*! Sin,Cos of Lead angle used in modulation index control mode*/
        mdqForceSinCos;

}CLOSE_LOOP_T;

/* Close Loop Interface Functions */

/**
 * @brief     Close Loop Init
 * @param[in] *pCloseLoop Input Pointer
 */
void closeLoopInit(CLOSE_LOOP_T *pCloseLoop);
/**
 * @brief     Close Loop Run
 * @param[in] *pCloseLoop Input Pointer
 */
void closeLoopRun(CLOSE_LOOP_T *pCloseLoop);
/**
 * @brief     Close Loop Low Priority run
 * @param[in] *pCloseLoop Input Pointer
 */
void closeLoopLowPriorityRun(CLOSE_LOOP_T *pCloseLoop);


/**
 * @brief     get Closed loop transition enable status
 * @param[in] *pCloseLoop  Pointer
 * @return    Returns the status
 */
inline _Bool getCloseLoopTransitionEnableStatus(CLOSE_LOOP_T *pCloseLoop)
{
    return pCloseLoop->flags.b.transitionEnable;
}

/**
 * @brief     get Closed loop align status
 * @param[in] *pCloseLoop  Pointer
 * @return    Returns the status
 */
inline _Bool getCloseLoopAlignedStatus(CLOSE_LOOP_T *pCloseLoop)
{
    return (pCloseLoop->commutationState == COMMUTATION_ALIGNED);
}


/**
 * @brief     get AVS Status
 * @param[in] *pCloseLoop
 * @return    Returns the status
 */
inline _Bool getAvsStatus(CLOSE_LOOP_T *pCloseLoop)
{
    _Bool decelExpected, speedDecelExpected;

    speedDecelExpected = *pCloseLoop->pSpeed > (pCloseLoop->targetSpeedLimited +
                            _IQmpy(pCloseLoop->targetSpeedLimited,
                                   _IQ(SPEED_ERROR_THRESHOLD_PU)));

    decelExpected = (pCloseLoop->flags.b.transitionEnable ||
                    (!pCloseLoop->flags.b.transitionEnable &&
                            speedDecelExpected));

    return (pCloseLoop->flags.b.avsEnable && decelExpected &&
                (pCloseLoop->idqRef.q == 0));
}

#endif /* FOC_INCLUDE_CLOSELOOP_H_ */
