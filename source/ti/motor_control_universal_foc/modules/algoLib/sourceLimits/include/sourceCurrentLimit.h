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
 *  @file       sourceCurrentLimit.h
 *  @brief      sourceCurrentLimit Module
 *
 * 
 *  @anchor sourceCurrentLimit_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef APPLIMITS_INCLUDE_SOURCECURRENTLIMIT_H_
#define APPLIMITS_INCLUDE_SOURCECURRENTLIMIT_H_

#include "stdint.h"
#include "mcLib.h"

/*! @brief Source Current Limit Threshold */
#define SOURCE_CURRENT_LIMIT_THRESHHOLD             _IQ(0.95)

/*! @brief Source Current Working Structure */
typedef union
{
    /*! @brief Source Current Structure */
    struct sourceCurrentLimitFlags
    {
        _Bool
        /*! Enables Source Current Limit */
        enable: 1,
        /*! Enables Source Current Limit Set Value*/
        enableSet: 1;
    }
    /*! Bitwise Access */
    b;
    /*! Block Access */
    int16_t w;

}SOURCE_CURRENT_LIMIT_FLAGS_T;

/*! @brief Source Current Limit Range Values */
typedef enum
{
    /*! Source current in limits */
    SOURCE_CURRENT_BELOW_LIMIT = 0,
    /*! Source current out of limit */
    SOURCE_CURRENT_ABOVE_LIMIT = 1,

}SOURCE_CURRENT_LIMIT_STATE_TYPES;

/*! @brief Source Current Limit Transition Values */
typedef enum
{
    /*! No transition */
    SOURCE_CURRENT_NO_TRANSITION = 0,
    /*! Normal current */
    SOURCE_CURRENT_TRANSITION_TO_NORMAL_CURRENT = 1,
    /*! Current limit */
    SOURCE_CURRENT_TRANSITION_TO_CURRENT_LIMIT = 2,

}SOURCE_CURRENT_LIMIT_STATE_TRANSITION_TYPES;

/*! @brief Source Current Limit Structure */
typedef struct
{
    /*! Current Limit Inputs */
    int32_t
    /*! Source Current */
        *pSourceCurrent;

    /*! Current Limit parameters for processing */

    SOURCE_CURRENT_LIMIT_STATE_TYPES
    /*! Current Limit State */
        state,
        /*! Previous Sample Current Limit State */
        prevState;

    SOURCE_CURRENT_LIMIT_STATE_TRANSITION_TYPES
    /*! Transition between current limit states */
        transition;

    SOURCE_CURRENT_LIMIT_FLAGS_T
    /*! Flags */
        flags;

    int32_t
    /*! Source Current Limit */
        sourceCurrentLimit;

}SOURCE_CURRENT_LIMIT_T;
/**
 * @brief     source Current Limit Init
 * @param[in] *pSourceCurrentLimit Input Pointer
 */
void sourceCurrentLimitInit(SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit);
/**
 * @brief     source Current Limit Run
 * @param[in] *pSourceCurrentLimit Input Pointer
 */
void sourceCurrentLimitRun(SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit);

/**
 * @brief    get Current Limit Transition
 * @param[in] *pSourceCurrentLimit  Pointer
 * @return    Returns the Transition status
 */
inline SOURCE_CURRENT_LIMIT_STATE_TRANSITION_TYPES
        getCurrentLimitTransition(SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit)
{
    return pSourceCurrentLimit->transition;
}

/**
 * @brief    get Current Limit State
 * @param[in] *pSourceCurrentLimit  Pointer
 * @return    Returns the State
 */
inline SOURCE_CURRENT_LIMIT_STATE_TYPES
        getCurrentLimitState(SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit)
{
    return pSourceCurrentLimit->state;
}

#endif /* APPLIMITS_INCLUDE_SOURCECURRENTLIMIT_H_ */
