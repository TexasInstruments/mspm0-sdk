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
 *  @file       abnormalSpeedStallDetect.h
 *  @brief      abnormalSpeedStallDetect Module
 *
 * 
 *  @anchor abnormalSpeedStallDetect_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_FOC_INCLUDE_ABNORMALSPEEDSTALLDETECT_H_
#define ALGOLIB_FOC_INCLUDE_ABNORMALSPEEDSTALLDETECT_H_

#include "stdint.h"

/*! @brief Abnormal Speed Detect flags Structure */
typedef union
{
    /*! @brief Abnormal Speed Detect flags */
    struct abnormalSpeedStallDetectFlags
    {
        _Bool
        /*! Enables Abnormal Speed Stall Detection */
        enable: 1,
        /*! Stall Detection enable set by user */
        enableSet: 1,
        /*! Stall Detection Status */
        status: 1,
        /*! Report only - no action */
        reportOnly: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}ABNORMAL_SPEED_STALL_DETECT_FLAGS_T;

/*! @brief Abnormal Speed Detect Working Structure */
typedef struct
{
    const int32_t
    /*! Velocity Feedback */
        *pSpeed;

    int32_t
    /*! Maximum speed to declare stall */
        maximumSpeed;

    uint16_t
    /*! Persistence counter for abnormal speed fault */
        count,
        /*! Persistence limit - abnormal speed fault */
        countMax;

    ABNORMAL_SPEED_STALL_DETECT_FLAGS_T
    /*! Flags */
        flags;

}ABNORMAL_SPEED_STALL_DETECT_T;

/**
 * @brief     Abnormal Speed Detect Init
 * @param[in] *pAbnormalSpeed Input Pointer
 */
void abnormalSpeedStallDetectInit(ABNORMAL_SPEED_STALL_DETECT_T *pAbnormalSpeed);
/**
 * @brief     Abnormal Speed Detect Run
 * @param[in] *pAbnormalSpeed Input Pointer
 */
void abnormalSpeedStallDetectRun(ABNORMAL_SPEED_STALL_DETECT_T *pAbnormalSpeed);
/**
 * @brief     Abnormal Speed Detect Status check
 * @param[in] *pAbnormalSpeedStall  Pointer
 * @return    Returns the status
 */
inline _Bool getAbnormalSpeedStallDetectStatus(ABNORMAL_SPEED_STALL_DETECT_T
                                                   *pAbnormalSpeedStall)
{
    return pAbnormalSpeedStall->flags.b.status;
}

#endif /* ALGOLIB_FOC_INCLUDE_ABNORMALSPEEDSTALLDETECT_H_ */
