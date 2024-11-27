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
 *  @file       overCurrent.h
 *  @brief      overCurrent Module
 *
 * 
 *  @anchor overCurrent_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/

#ifndef ALGOLIB_SOURCELIMITS_INCLUDE_OVERCURRENT_H_
#define ALGOLIB_SOURCELIMITS_INCLUDE_OVERCURRENT_H_

#include "stdint.h"
#include "measure.h"

/*! @brief Over Current Working Structure */
typedef union
{
    /*! @brief Over Current Structure */
    struct overCurrentFlags
    {
        _Bool
        /*! Enable Over Currents*/
            enable: 1,
            /*! Enable Set */
            enableSet: 1,
            /*! OC Status */
            status: 1,
            /*! OC Report only */
            reportOnly: 1;
    }
    /*! Bitwise Access */
    b;
    /*! Block Access*/
    uint16_t w;

}OVER_CURRENT_FLAGS_T;

/*! @brief Over Current Structure */
typedef struct
{

    int32_t
    /*! OC Limit Set */
        overCurrentLimitSet,
        /*! OC Limit */
        overCurrentLimit;

    OVER_CURRENT_FLAGS_T
    /*! OC Flags*/
        flags;

}OVER_CURRENT_T;

/**
 * @brief    over Current Init
 * @param[in] *pOverCurrent Input Pointer
 */
void overCurrentInit(OVER_CURRENT_T *pOverCurrent);
/**
 * @brief      over Current Run
 * @param[in] *pOverCurrent Input Pointer
 */
void overCurrentRun(OVER_CURRENT_T *pOverCurrent);
/**
 * @brief     get Over Current Status
 * @param[in] *pOverCurrent  Pointer
 * @return    Returns the status
 */
inline _Bool getOverCurrentStatus(OVER_CURRENT_T *pOverCurrent)
{
    return (pOverCurrent->flags.b.status);
}

#endif /* ALGOLIB_SOURCELIMITS_INCLUDE_OVERCURRENT_H_ */
