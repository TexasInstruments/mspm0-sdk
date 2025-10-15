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
 *  @file       softStop.h
 *  @brief      softStop Module
 *
 * 
 *  @anchor softStop_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LOAD_INCLUDE_SOFTSTOP_H_
#define ALGOLIB_LOAD_INCLUDE_SOFTSTOP_H_

#include "stdint.h"

/*! @brief Soft Stop Flags Working Structure */
typedef union
{
    /*! @brief Soft Stop Flags Structure */
    struct softStopFlags
    {
        _Bool
        /*! Enables soft stop */
            enable: 1,
            /*! Enables soft stop on brake */
            enableOnBrake: 1,
            /*! Status of soft stop */
            status: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}SOFT_STOP_FLAGS_T;

/*! @brief Soft Stop Working Structure */
typedef struct
{
    /*! Soft Stop Inputs */
    const int32_t
    /*! Velocity Reference */
        **ppVelocityReference,
        /*! Speed of Motor */
        *pSpeed;

    /*! Soft Stop parameters for processing */
    int32_t
    /*! Minimum Speed */
        minimumSpeed,
        /*! Minimum Speed on Brake */
        minimumSpeedBrake,
        /*! Minimum Speed on soft stop */
        minimumSpeedSoftStop;

    SOFT_STOP_FLAGS_T
    /*! Flags */
        flags;

}SOFT_STOP_T;

/**
 * @brief     Soft Stop Init
 * @param[in] *pSoftStop Input Pointer
 */
void softStopInit(SOFT_STOP_T *pSoftStop);
/**
 * @brief     Soft Stop Run
 * @param[in] *pSoftStop Input Pointer
 */
void softStopRun(SOFT_STOP_T *pSoftStop);
/**
 * @brief     Soft Stop Status check
 * @param[in] *pSoftStop  Pointer
 * @return    Returns the status
 */
inline _Bool getSoftStopStatus(SOFT_STOP_T *pSoftStop)
{
    return pSoftStop->flags.b.status;
}

#endif /* ALGOLIB_LOAD_INCLUDE_SOFTSTOP_H_ */
