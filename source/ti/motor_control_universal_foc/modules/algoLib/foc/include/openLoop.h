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
 *  @file       openLoop.h
 *  @brief      openLoop Module
 *
 * 
 *  @anchor openLoop_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef FOC_INCLUDE_OPENLOOP_H_
#define FOC_INCLUDE_OPENLOOP_H_

#include "stdint.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "iqNum.h"
#include "slowFirstCycle.h"

/*! @brief Open Loop flags Structure */
typedef union
{
    /*! @brief Open Loop flags  */
    struct openLoopFlags
    {
        _Bool
            /*! Disables exit for open Loop */
            forceEnable: 1,
            /*! Enables the open loop to close loop transition */
            transitionEnable: 1,
            /*! Checks zero crossing of velocity */
            velocityZeroCross: 1,
            /*! Completion Status */
            status: 1,
            /*! Slow first cycle velocity applied as initial velocity */
            fastStartEnable: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}OPEN_LOOP_FLAGS_T;

/*! @brief Open Loop Ramp Enum Values */
typedef enum
{
    /*! Accelerate user on profile */
    OPEN_LOOP_ACCELERATING_PROFILE = 0,
    /*! Slow first cycle */
    OPEN_LOOP_SLOW_FIRST_CYCLE_PROFILE = 1,
}FORCED_SPEED_STATE_TYPES;

/*! @brief Open Loop Transition Enum Values */
typedef enum
{
    /*! Entry: standstill */
    OPEN_LOOP_TRANSITION_AT_MOTOR_START = 0,

    /*! Entry: motoring */
    OPEN_LOOP_TRANSITION_IN_MOTORING = 1,

}OPEN_LOOP_TRANSITION_TYPES;

/*! @brief Open Loop Working Structure */
typedef struct
{
    /* Open Loop Inputs */

    const int32_t
        /*! Target Velocity */    
        *pTargetVelocity,
        /*! Sampling Time */        
        *pDeltaT;

    /* Open Loop parameters for processing */

    int32_t
        /*! Velocity Reference */
        velocityReference,
        /*! Previous sample velocity reference */
        prevVelocityReference,
        /*! Scaled Velocity Reference */
        velocityReferenceScaled,
        /*! Number of times loop has run */
        loopExecutionCount,
        /*! Forced commutation Angle */
        angle,
        /*! D-axis current reference */
        idRef,
        /*! D-axis current reference Slew Rate */
        idRefSlewRate,
        /*! Q-axis current reference */
        iqRef,
        /*! Maximum Q-axis current reference */
        iqRefMax,
        /*! Q-axis current slew rate */
        iqRefSlewRate,
        /*! Q-axis current reference set by user */
        iqRefSet,
        /*! Q-axis current reference startup */
        iqRefSetFastAccel,
        /*! Maximum Forward speed reference - user */
        speedReferenceMaxForwardSet,
        /*! Maximum speed reference in open loop */
        speedReferenceMax;

    IQ_NUM_T
        /*! Slew rate for speed reference */
        *pSpeedRefSlewRate,
        /*! Slew rate for acceleration */
        *pAccelRefSlewRate,

        /*! Speed reference slew rate startup */
        speedRefSlewRateFastAccel,
        /*! Acceleration slew rate startup */
        accelRefSlewRateFastAccel,

        /*! Speed reference slew rate fwd drive */
        speedRefSlewRateSlowAccel,
        /*! Acceleration slew rate forward drive */
        accelRefSlewRateSlowAccel;


    SLOW_FIRST_CYCLE_T
        /*! Slow First Cycle Structure */
        slowFirstCycle;

    FORCED_SPEED_STATE_TYPES
        /*! Speed Reference State */
        speedReferenceState;

    OPEN_LOOP_TRANSITION_TYPES
        /*! Tracks the mode of transition to open loop happened */
        transitionToOpenLoop;

    OPEN_LOOP_FLAGS_T
        /*! Open Loop Flags */
        flags;

}OPEN_LOOP_T;

/**
 * @brief     openLoopInit
 * @param[in] *pOpenLoop Input Pointer
 */
void openLoopInit(OPEN_LOOP_T *pOpenLoop);
/**
 * @brief     openLoop Run
 * @param[in] *pOpenLoop Input Pointer
 */
void openLoopRun(OPEN_LOOP_T *pOpenLoop);

/**
 * @brief     Open Loop Init Velocity Set
 * @param[in] *pOpenLoop Input Pointer
 */
void openLoopSetInitialVelocity(OPEN_LOOP_T *pOpenLoop);

/**
 * @brief     Open Loop Status check
 * @param[in] *pOpenLoop  Pointer
 * @return    Returns the status
 */
inline _Bool getOpenLoopStatus(OPEN_LOOP_T *pOpenLoop)
{
    return pOpenLoop->flags.b.status;
}

/**
 * @brief     Open Loop Velocity zero cross Status
 * @param[in] *pOpenLoop  Pointer
 * @return    Returns the status
 */
inline _Bool getOpenLoopVelocityZeroCross(OPEN_LOOP_T *pOpenLoop)
{
    return pOpenLoop->flags.b.velocityZeroCross;
}

#endif /* FOC_INCLUDE_OPENLOOP_H_ */
