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
 *  @file       sourceLimits.h
 *  @brief      sourceLimits Module
 *
 * 
 *  @anchor sourceLimits_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPLIMITS_INCLUDE_APPLIMITS_H_
#define APPLIMITS_INCLUDE_APPLIMITS_H_

#include "sourceCurrentEstim.h"
#include "sourceCurrentLimit.h"
#include "sourceVoltageLimit.h"
#include "overCurrent.h"

/*! @brief Source Limit Structure Values */
typedef struct
{
    SOURCE_CURRENT_ESTIMATOR_T
    /*! Source Current Estimation Structure */
        sourceCurrentEstim;

    SOURCE_CURRENT_LIMIT_T
    /*! Source Current Limit Structure */
        sourceCurrentLimit;

    SOURCE_VOLTAGE_LIMIT_T
    /*! Source Voltage Limit Structure */
        sourceVoltageLimit;

    OVER_CURRENT_T
    /*! Source Voltage Limit Structure */
        overCurrent;

}SOURCE_LIMITS_T;
/**
 * @brief     source Limits Init
 * @param[in] *pSourceLimits Input Pointer
 */
void sourceLimitsInit(SOURCE_LIMITS_T *pSourceLimits);
/**
 * @brief     source Limits Run
 * @param[in] *pSourceLimits Input Pointer
 */
void sourceLimitsRun(SOURCE_LIMITS_T *pSourceLimits);

#endif /* SOURCELIMITS_INCLUDE_APPLIMITS_H_ */
