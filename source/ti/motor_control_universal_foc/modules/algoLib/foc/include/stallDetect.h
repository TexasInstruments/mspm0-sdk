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
 *  @file       stallDetect.h
 *  @brief      stallDetect Module
 *
 * 
 *  @anchor stallDetect_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef FOC_INCLUDE_STALLDETECT_H_
#define FOC_INCLUDE_STALLDETECT_H_

#include "iqNum.h"
#include "stdint.h"
#include "mcLib.h"

#include "lowBemfStallDetect.h"
#include "noMotorStallDetect.h"
#include "abnormalSpeedStallDetect.h"

/**
 * @brief Stall Type Enums
 */
typedef enum
{
    /*! Motor is not stalled */
    NO_STALL = 0,

    /*! Low BEMF stall detected */
    LOW_BEMF_STALL = 1,

    /*! No motor stall detected */
    NO_MOTOR_STALL = 2,

    /*! Abnormal speed stall detected */
    ABNORMAL_SPEED_STALL = 3,

}STALL_TYPES;

/*! @brief Stall detect structure */
typedef struct
{
     LOW_BEMF_STALL_DETECT_T
        /*! Low BEMF Stall Detection Structure */
        lowBemfStall;

    NO_MOTOR_STALL_DETECT_T
        /*! no Motor Stall Detection Structure */
        noMotorStall;

    ABNORMAL_SPEED_STALL_DETECT_T
        /*! Abnormal Speed Stall Detection Structure */
        abnormalSpeedStall;

    STALL_TYPES
        /*! Stall Status */
        status;

}STALL_DETECT_T;

/**
 * @brief     Stall Detect Init
 * @param[in] *pStall Input Pointer
 */
void stallDetectInit(STALL_DETECT_T *pStall);
/**
 * @brief    Stall Detect Run
 * @param[in] *pStall Input Pointer
 */
void stallDetectRun(STALL_DETECT_T *pStall);
/**
 * @brief     Stall Detect Low Priority
 * @param[in] *pStall Input Pointer
 */
void stallDetectLowPriorityRun(STALL_DETECT_T *pStall);

/**
 * @brief    Stall Status check
 * @param[in] *pStall  Pointer
 * @return    Returns the status
 */
inline _Bool getStallStatusForAction(STALL_DETECT_T *pStall)
{
    _Bool
        statusLowBemf,
        statusAbnormalStallSpeed,
        statusNoMotor;

    statusLowBemf = (pStall->lowBemfStall.flags.b.status &&
                        !pStall->lowBemfStall.flags.b.reportOnly);

    statusAbnormalStallSpeed = (pStall->abnormalSpeedStall.flags.b.status &&
                            !pStall->abnormalSpeedStall.flags.b.reportOnly);

    statusNoMotor = (pStall->noMotorStall.flags.b.status &&
                            !pStall->noMotorStall.flags.b.reportOnly);

    return (statusLowBemf || statusAbnormalStallSpeed ||
                statusNoMotor);
}

/**
 * @brief     Stall Report Status check
 * @param[in] *pStall  Pointer
 * @return    Returns the status
 */
inline _Bool getStallStatusForReport(STALL_DETECT_T *pStall)
{
    return (pStall->status != NO_STALL);
}

#endif /* FOC_INCLUDE_STALLDETECT_H_ */
