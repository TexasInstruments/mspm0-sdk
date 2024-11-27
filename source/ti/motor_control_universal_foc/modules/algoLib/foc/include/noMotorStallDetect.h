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
 *  @file       noMotorStallDetect.h
 *  @brief      noMotorStallDetect Module
 *
 * 
 *  @anchor noMotorStallDetect_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_FOC_INCLUDE_NOMOTORSTALLDETECT_H_
#define ALGOLIB_FOC_INCLUDE_NOMOTORSTALLDETECT_H_

#include "stdint.h"
#include "mcLib.h"

/*! @brief No Motor Stall Working Structure */
typedef union
{
    /*! @brief No Motor Stall Flags Struct     */
    struct noMotorStallDetectFlags
    {
        _Bool
        /*! Enables no motor stall detection */
            enable: 1,
            /*! Enable set by user */
            enableSet: 1,
            /*! no Motor stall status */
            status: 1,
            /*! Report only - no action */
            reportOnly: 1,
            /*! Ph A disconnect */
            phaseADisconnect: 1,
            /*! Ph B disconnect */
            phaseBDisconnect: 1,
            /*! Ph C disconnect */
            phaseCDisconnect: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}NO_MOTOR_STALL_DETECT_FLAGS_T;

/*! @brief No Motor Detect Working Structure */
typedef struct
{
    const MC_ABC_T
    /*! Three phase current */
        *pIabc;

    int32_t
    /*! Minimum current */
        minimumCurrent;

    uint16_t
    /*! Persistence counter for no motor */
        countA,
        /*! Persistence counter for no motor */
        countB,
        /*! Persistence counter for no motor */
        countC,
        /*! Persistence limit - no motor */
        countMax;

    NO_MOTOR_STALL_DETECT_FLAGS_T
    /*! Flags */
        flags;

}NO_MOTOR_STALL_DETECT_T;

/**
 * @brief     No Motor Stall Init
 * @param[in] *pNoMotorStall Input Pointer
 */
void noMotorStallDetectInit(NO_MOTOR_STALL_DETECT_T *pNoMotorStall);
/**
 * @brief     No Motor Stall Run
 * @param[in] *pNoMotorStall Input Pointer
 */
void noMotorStallDetectRun(NO_MOTOR_STALL_DETECT_T *pNoMotorStall);

/**
 * @brief     No Motor Stall Status check
 * @param[in] *pNoMotorStall  Pointer
 * @return    Returns the status
 */
inline _Bool getNoMotorStallDetectStatus(NO_MOTOR_STALL_DETECT_T
                                                   *pNoMotorStall)
{
    return pNoMotorStall->flags.b.status;
}


#endif /* ALGOLIB_FOC_INCLUDE_NOMOTORSTALLDETECT_H_ */
