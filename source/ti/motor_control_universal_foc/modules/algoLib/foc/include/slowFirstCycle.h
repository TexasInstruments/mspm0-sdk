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
 *  @file       slowFirstCycle.h
 *  @brief      slowFirstCycle Module
 *
 * 
 *  @anchor slowFirstCycle_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_FOC_INCLUDE_SLOWFIRSTCYCLE_H_
#define ALGOLIB_FOC_INCLUDE_SLOWFIRSTCYCLE_H_

#include "stdint.h"

/*! @brief Slow first cycle flags union */
typedef union
{
    /*! @brief Slow first cycle flags */
    struct slowFirstCycleFlags
    {
        _Bool
            /*! Disables exit of slow first cycle */
            forceEnable: 1,
            /*! Enables slow first cycle */
            enable: 1,
            /*! Slow first cycle Inprogress*/
            inProgress: 1,
            /*! Completion status */
            status: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}SLOW_FIRST_CYCLE_FLAGS_T;

/*! @brief Slow first cycle structure */
typedef struct
{
    /* Slow First Cycle Inputs */

    const int32_t
        /*! Target Velocity */
        *pTargetVelocity,
        /*! Commutation angle */
        *pAngle;

    /* Slow First Cycle parameters for processing */

    int32_t
        /*! Slow first cycle speed reference */
        speedReference,
        /*! Slow first cycle velocity reference */
        velocityReference,
        /*! Q-axis current reference */
        iqRef,
        /*! Maximum value for Q-axis current reference */
        iqRefMax,
        /*! Q-axis current reference set by user */
        iqRefSet;

    SLOW_FIRST_CYCLE_FLAGS_T
        /*! Flags */
        flags;

}SLOW_FIRST_CYCLE_T;

/**
 * @brief     slow first cycle init
 * @param[in] *pSlowFirstCycle Pointer
 */
void slowFirstCycleInit(SLOW_FIRST_CYCLE_T *pSlowFirstCycle);

/**
 * @brief     slow first cycle run
 * @param[in] *pSlowFirstCycle Pointer
 */
void slowFirstCycleRun(SLOW_FIRST_CYCLE_T *pSlowFirstCycle);

/**
 * @brief     slow first cycle Status
 * @param[in] *pSlowFirstCycle  Pointer
 * @return    Returns the status
 */
inline _Bool getSlowFirstCycleStatus(SLOW_FIRST_CYCLE_T *pSlowFirstCycle)
{
    return pSlowFirstCycle->flags.b.status;
}

#endif /* ALGOLIB_FOC_INCLUDE_SLOWFIRSTCYCLE_H_ */
