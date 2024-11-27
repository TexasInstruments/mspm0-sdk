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
 *  @file       pi.h
 *  @brief      pi Module
 *
 * 
 *  @anchor pi_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LIBRARIES_CONTROLLER_INCLUDE_PI_H_
#define ALGOLIB_LIBRARIES_CONTROLLER_INCLUDE_PI_H_

#include "stdint.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "iqNum.h"

/*! @brief KP IQ Scaling */
#define KP_IQ_SCALING       24
/*! @brief KP Term Saturation Lim */
#define KP_TERM_SAT         (((int32_t)1) << (30 - GLOBAL_IQ + KP_IQ_SCALING))

/*! @brief PI Flags Working Structure */
typedef union
{
    /*! @brief PI flags Structure */
    struct piFlags
    {
        _Bool
        /*! Indicates controller saturation */
            saturation: 1,
        /*! Consider external saturation  */
            externalSaturation: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}PI_FLAGS_T;

/*! @brief PI Working Structure */
typedef struct
{
    const int32_t
    /*! Reference */
        *pReference,
        /*! Feedback */
        *pFeedback;

    int32_t
    /*! Accumulation of Error * Ki */
        kiTerm,
        /*! Low Limit of Output */
        outMin,
        /*! High Limit of Output */
        outMax,
        /*! Output */
        out,
        /*! Proportional Controller Constant */
        kp,
        /*! Integral Controller Constant */
        ki;

    PI_FLAGS_T
    /*! Flags */
        flags;

}PI_T;

/**
 * @brief    PI Init
 * @param[in] *pPI Input Pointer
 */
void piInit(PI_T *pPI);
/**
 * @brief    PI Run
 * @param[in] *pPI Input Pointer
 */
void piRun(PI_T *pPI);
/**
 * @brief    PI Low Priority Run
 * @param[in] *pPI Input Pointer
 */
void piRun_LowPriority(PI_T *pPI);

#endif /* ALGOLIB_LIBRARIES_CONTROLLER_INCLUDE_PI_H_ */
