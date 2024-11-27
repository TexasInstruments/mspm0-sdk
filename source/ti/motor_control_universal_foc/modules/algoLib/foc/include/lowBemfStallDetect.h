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
 *  @file       lowBemfStallDetect.h
 *  @brief      lowBemfStallDetect Module
 *
 * 
 *  @anchor lowBemfStallDetect_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_FOC_INCLUDE_LOWBEMFSTALLDETECT_H_
#define ALGOLIB_FOC_INCLUDE_LOWBEMFSTALLDETECT_H_

#include "stdint.h"
#include "iqNum.h"
#include "mcLib.h"
#include "motor.h"

/*! @brief Low Bemf stall detect Flags Structure */
typedef union
{
    /*! @brief Low Bemf stall detect Flags Structure */
    struct lowBemfStallDetectFlags
    {
        _Bool
            /*! Stall Detection Enable Set by User */
            enableSet: 1,
            /*! Low BEMF Stall Detection Enable */
            enable: 1,
            /*! Low BEMF Stall Status */
            status: 1,
            /*! Report only - no action */
            reportOnly: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}LOW_BEMF_STALL_DETECT_FLAGS_T;

/*! @brief Low Bemf stall detect Structure */
typedef struct
{
    /* Stall Detect Inputs */

    const int32_t
        /*! Velocity Reference */
        *pVelocityReference,
        /*! Velocity */
        *pVelocity,
        /*! Inverse Vdc */
        *pInvVdc,
        /*! Reference BEMF */
        *pEmagFiltRef;

    uint16_t
        /*! Persistence counter for low BEMF fault */
        count,
        /*! Persistence limit - low BEMF */
        countMax;

    int32_t
        /*! threshold below which Low BEMF stall is detected */
        lowBEMFThreshold;
    LOW_BEMF_STALL_DETECT_FLAGS_T
        /*! Flags */
        flags;

}LOW_BEMF_STALL_DETECT_T;

/**
 * @brief     Low Bemf stall detect init
 * @param[in] *pLowBemfStall Pointer
 */
void lowBemfStallDetectInit(LOW_BEMF_STALL_DETECT_T *pLowBemfStall);

/**
 * @brief     Low Bemf stall detect run
 * @param[in] *pLowBemfStall Pointer
 */
void lowBemfStallDetectRun(LOW_BEMF_STALL_DETECT_T *pLowBemfStall);

/**
 * @brief     Low Bemf stall status check
 * @param[in] *pLowBemfStall  Pointer
 * @return    Returns the status
 */
inline _Bool getLowBemfStallStatus(LOW_BEMF_STALL_DETECT_T *pLowBemfStall)
{
    return pLowBemfStall->flags.b.status;
}

#endif /* ALGOLIB_FOC_INCLUDE_LOWBEMFSTALLDETECT_H_ */
