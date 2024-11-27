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
 *  @file       estimator.h
 *  @brief      estimator Module
 *
 * 
 *  @anchor estimator_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/

#ifndef FOC_INCLUDE_ESTIMATOR_H_
#define FOC_INCLUDE_ESTIMATOR_H_

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "iqNum.h"
#include "motor.h"
#include "mcLib.h"
#include "angleTrackingPLL.h"

/*! @brief Structure for estimator */
typedef struct
{
    /* Estimator Inputs */
    const MC_ALPHABETA_T
        /*! Voltage */
        *pVAlphaBeta,
        /*! Current */
        *pIAlphaBeta;

    const MOTOR_T
        /*! Motor Parameters */
        *pMotor;

    /* Estimator parameters for processing */
    MC_ALPHABETA_T
        /*! Previous sample Flux */
        prevFluxAlphaBeta,
        /*! BEMF in stator reference frame */
        eAlphaBeta;
    AT_PLL_ESTIM_T
        /*! AT PLL estimator */
        *pAngleTrackingPLLEstim;
}ESTIMATOR_T;

/**
 * @brief     estimator run
 * @param[in] *pEstim
 */
void estimatorRun(ESTIMATOR_T *pEstim);

#endif /* FOC_INCLUDE_ESTIMATOR_H_ */
