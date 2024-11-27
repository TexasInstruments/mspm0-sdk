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
 *  @file       sourceCurrentEstim.h
 *  @brief      sourceCurrentEstim Module
 *
 * 
 *  @anchor sourceCurrentEstim_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPLIMITS_INCLUDE_SOURCECURRENTESTIM_H_
#define APPLIMITS_INCLUDE_SOURCECURRENTESTIM_H_

#include "stdint.h"
#include "mcLib.h"

/* Source Current Limit Parameters */
/*! @brief Source Current Filt Const */
#define SOURCE_CURRENT_FILT_CONST                   _IQ(0.1)

/*! @brief Source Current Working Struct*/
typedef struct
{
    /*! Source Current Estimation Inputs */

    const int32_t
    /*! Source Current Pointer*/
       *pSourceCurrent;

    /*! Source Current Estimation parameters processing */

    int32_t
    /*! Filtered source current */
       sourceCurrentFilt;

}SOURCE_CURRENT_ESTIMATOR_T;

/**
 * @brief     source Current Estim Init
 * @param[in] *pSourceCurrentEstim Input Pointer
 */
void sourceCurrentEstimInit(SOURCE_CURRENT_ESTIMATOR_T *pSourceCurrentEstim);
/**
 * @brief     source Current Estim Run
 * @param[in] *pSourceCurrentEstim Input Pointer
 */
void sourceCurrentEstimRun(SOURCE_CURRENT_ESTIMATOR_T *pSourceCurrentEstim);

#endif /* APPLIMITS_INCLUDE_SOURCECURRENTESTIM_H_ */
