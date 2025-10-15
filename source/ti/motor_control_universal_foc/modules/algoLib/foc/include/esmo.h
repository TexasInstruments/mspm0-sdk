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
 *  @file       esmo.h
 *  @brief      esmo Module
 *
 * 
 *  @anchor esmo_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/

#ifndef ALGOLIB_FOC_INCLUDE_ESMO_H_
#define ALGOLIB_FOC_INCLUDE_ESMO_H_

#include "stdint.h"
#include "mcLib.h"
#include "angleTrackingPLL.h"
#include "iqNum.h"

/*! @brief Minimum SMO Filter frequency */
#define MIN_SMO_FILTER_FREQ _IQ(0.1)

/*! @brief FSMO IQ scaling */
#define FSMO_IQ_SCALING  27

/*! @brief GSMO IQ scaling */
#define GSMO_IQ_SCALING  21

/*! @brief Structure for esmo */
typedef struct
{
    const MC_ALPHABETA_T
        /*! Applied Voltages */
        *pVAlphaBeta,
        /*! Current */
        *pIAlphaBeta;

    const int32_t
        /*! Delta time */
        *pDeltaT;


    int32_t
        /*! FSMO */
        FSMO,
        /*! GSMO */
        GSMO,
        /*! kslide */
        kSlide,
        /*! Filter constant */
        kSlf;

    MC_ALPHABETA_T
        /*! Estimated current */
        iAlphaBetaEstim,
        /*! Estimated z AlphaBeta */
        zAlphaBeta,
        /*! Estimated z AlphaBeta Filtered */
        zAlphaBetaFilt,
        /*! Estimated backemf AlphaBeta */
        EAlphaBeta,
        /*! Previous Estimated backemf AlphaBeta */
        prevEAlphaBeta,
        /*! Current AlphaBeta  error */
        iAlphaBetaError;
    AT_PLL_ESTIM_T
        /*! AT PLL estimator */
        *pAngleTrackingPLLEstim;           

    IQ_NUM_T
        /*! Filter gain */
        filterGain;

    const MOTOR_T
        /*! Motor Parameters */
        *pMotor;

}ESMO_T;

/**
 * @brief     esmo Init
 * @param[in] *pESMO
 */
void eSMOInit(ESMO_T *pESMO);

/**
 * @brief     Runs esmo estimator
 * @param[in] *pESMO
 */
void eSMORun(ESMO_T *pESMO);

#endif /* ALGOLIB_FOC_INCLUDE_ESMO_H_ */
