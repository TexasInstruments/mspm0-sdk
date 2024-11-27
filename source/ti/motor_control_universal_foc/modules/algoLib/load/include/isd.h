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
 *  @file       isd.h
 *  @brief      isd Module
 *
 * 
 *  @anchor isd_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LOAD_INCLUDE_ISD_H_
#define ALGOLIB_LOAD_INCLUDE_ISD_H_

#include "stdint.h"
#include "mcLib.h"
#include "angleTrackingPLL.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

/* ISD Parameters */
/*! @brief Isd Flags Working Structure */
typedef union
{
    /*! @brief Isd Flags */
    struct isdFlags
    {
        _Bool
            /*! Enables ISD */
            enable: 1,
            /*! Enables on-the-fly pickup of motor in reverse direction */
            reverseSyncEnable: 1,
            /*! Enables on-the-fly pickup of motor in forward direction */
            forwardSyncEnable: 1,
            /*! Status of ISD */
            status: 1,
            /*! Holds ISD in complete state */
            forceEnable: 1;

    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}ISD_FLAGS_T;

/*! @brief isd direction types enum */
typedef enum
{
    /* Unidentified direction */
    DIRECTION_UNIDENTIFIED = 0,
    /* Motor is stopped */
    DIRECTION_STOP = 1,
    /* Motor is rotating in forward direction */
    DIRECTION_FORWARD = 2,
    /* Motor is rotating in reverse direction */
    DIRECTION_REVERSE = 3,
    /* Motor is rotating in Opposite to Intended direction */
    DIRECTION_UNINTENDED = 4,
}ISD_DIRECTION_TYPES;

/*! @brief isd state types enum */
typedef enum
{
    /*! Initialize ISD */
    ISD_INIT = 0,
    /*! Check if motor is stopped */
    ISD_MOTOR_STOP_CHECK = 1,
    /*! Check if motor is run */
    ISD_RUN_MOTOR_CHECK = 2,
    /*! Check if motor is running stably */
    ISD_MOTOR_DIRECTION_CHECK = 3,
    /*! ISD Completed */
    ISD_COMPLETE = 4,
    /*! Maximum number of retries reached */
    ISD_FAULT = 5,
}ISD_STATE_TYPES;

/*! @brief Isd  Structure */
typedef struct
{
    /* ISD Inputs */
    const MC_ABC_T
        /*! 3 Phase Phase voltage */
        *pEabc;

    const int32_t
        /*! Target Velocity */
        *pTargetVelocity,
        /*! Velocity of motor */
        *pVelocity;

    /* ISD parameters for processing */
    ISD_STATE_TYPES
        /*! State of ISD */
        state;

    MC_ALPHABETA_T
        /*! Stationary frame phase voltages */
        eAlphaBeta;

    ISD_DIRECTION_TYPES
        /*! Motor direction detected by ISD */
        direction;

    AT_PLL_ESTIM_T
        /*! AT PLL estimator */
        *pAngleTrackingPLLEstim;

    int32_t
        /*! Detected Speed */
        speed,
        /*! Minimum speed to enter close loop */
        minCloseLoopSpeed,
        /*! Per unit BEMF error to declare motor run */
        bemfMagRunThresholdPU,
        /*! Maximum BEMF to declare motor has stopped */
        maxStopBemf,
        /*! Magnitude of BEMF */
        bemfMag;

    uint16_t
        /*! Counter to track motor is stopped or not */
        motorStopCount,
        /*! Maximum count to declare motor is stopped */
        motorStopCountMax,
        /*! Counter to track motor is running or not */
        motorRunCount,
        /*! Maximum count to declare motor is running */
        motorRunCountMax;

    int16_t
        /*! Number of positive direction detected */
        motorSpeedPositive,
        /*! Number of negative direction deetected */
        motorSpeedNegative;
    ISD_FLAGS_T
        /*! Flags */
        flags;

}ISD_T;

/**
 * @brief     Isd  Init
 * @param[in] *pIsd Pointer
 */
void isdInit(ISD_T *pIsd);

/**
 * @brief     Isd  run
 * @param[in] *pIsd Pointer
 */
void isdRun(ISD_T *pIsd);

/**
 * @brief     Isd status
 * @param[in] *pIsd Pointer
 * @return    Returns the status
 */
inline _Bool getIsdStatus(ISD_T *pIsd)
{
    return pIsd->flags.b.status;
}


#endif /* ALGOLIB_LOAD_INCLUDE_ISD_H_ */
