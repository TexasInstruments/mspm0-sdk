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
 *  @file       triState.h
 *  @brief      triState Module
 *
 * 
 *  @anchor triState_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LOAD_INCLUDE_TRISTATE_H_
#define ALGOLIB_LOAD_INCLUDE_TRISTATE_H_

#include "stdint.h"

/*! @brief TriState Flags Working Structure */
typedef union
{
    /*! @brief TriState Flags Structure */
    struct triStateFlags
    {
        _Bool
        /*! Status of tri-state */
            status: 1,
        /*! Enables tri-state */
            enable: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint32_t w;

}TRI_STATE_FLAGS_T;

/*! @brief TriState  Working Structure */
typedef struct
{
    /*! Tri-state parameters for processing */
    uint16_t
    /*! Counter to track time in tri-state mode */
        count,
    /*! Maximum counter value in tri-state mode */
        countMax;

    TRI_STATE_FLAGS_T
    /*! Flags */
        flags;

}TRISTATE_T;
/**
 * @brief     TriState Init
 * @param[in] *pTriState Pointer
 */
void triStateInit(TRISTATE_T *pTriState);
/**
 * @brief     TriState Run
 * @param[in] *pTriState Pointer
 */
void triStateRun(TRISTATE_T *pTriState);
/**
 * @brief     TriState Status check
 * @param[in] *pTriState  Pointer
 * @return    Returns the status
 */
inline _Bool getTriStateStatus(TRISTATE_T *pTriState)
{
    return pTriState->flags.b.status;
}
#endif /* ALGOLIB_LOAD_INCLUDE_TRISTATE_H_ */
