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
 *  @file       angleTrackingPLL.h
 *  @brief      angleTrackingPLL Module
 *
 * 
 *  @anchor angleTrackingPLL_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LIBRARIES_SEMICLOSELOOPESTIM_INCLUDE_ATPLL_H
#define ALGOLIB_LIBRARIES_SEMICLOSELOOPESTIM_INCLUDE_ATPLL_H

#include "stdint.h"
#include "mcLib.h"
#include "motor.h"
#include "appDefs.h"
#include "pi.h"

/* Estimator Parameters */

/*! @brief Bemf filter constant */
#define BEMF_FILT_CONST                             0.1
/*! @brief Bemf filter cutoff frequency */
#define BEMF_CUT_OFF_FREQ                           1 //Hz
/*! @brief Bemf filter cutoff constant */
#define BEMF_FILT_CONST_FACTOR                      _IQ(BEMF_FILT_CONST * 2 * PI)

/*! @brief EMAG_COMP_OPTIM definition to Optimize Cycles for EMAG */
#define EMAG_COMP_OPTIM

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

/*! @brief AT PLL structure */
typedef struct
{
    /* Semi Close Loop Estimator Inputs */
    const MC_ALPHABETA_T
        /*! BEMF in stator reference frame */
        *pEAlphaBeta;

    const MOTOR_T
        /*! Motor parameters */
        *pMotor;

    const int32_t
        /*! Sampling Time */
        *pDeltaT;

    /* ATPLL parameters for processing */
    MC_DQ_T
        /*! BEMF in rotor reference frame */
        Edq,
        /*! Filtered BEMF in rotor reference frame */
        EdqFilt;

    MC_SINCOS_T
        /*! Sin and Cos of Flux Angle */
        sinCosFluxAngle;

    int32_t
        /*! Normalized E reference */
        normEqReference,
        /*! Normalized E Feedback */
        normEqfeedback,
        /*! Estimated Velocity */
        velocity,
        /*! Filtered Velocity */
        velocityFilt,
        /*! Filtered Speed */
        speedFilt,
        /*! Estimated bemf angle */
        bemfAngle,
        /*! Phase Angle Difference between BEMF & Flux */
        phaseDiff,
        /*! Estimated flux angle */
        fluxAngle,
        /*! Change in Angle for deltaT */
        deltaTheta,
        /*! BEMF filter constant */
        bemfFiltConst,
        /*! invEqMagNorm */
        invEqMagNorm,
        /*! bemfMag */
        bemfMag;
    PI_T
        /*! AT PLL PI Parameters */
        piATPLL;

   const int32_t
        /*! Velocity reference pointer */
       *pVelocityReference;

}AT_PLL_ESTIM_T;

/**
 * @brief     Angle tracking PLL Init
 * @param[in] *pATPLL
 */
void angleTrackingPLLInit(AT_PLL_ESTIM_T *pATPLL);

/**
 * @brief     Angle tracking PLL run
 * @param[in] *pATPLL
 */
void angleTrackingPLLRun(AT_PLL_ESTIM_T *pATPLL);

/**
 * @brief     Angle tracking filter constant update
 * @param[in] *pATPLL
 */
void angleTrackingFiltConstUpdate(AT_PLL_ESTIM_T *pATPLL);

/**
 * @brief     Angle tracking Emag Update
 * @param[in] *pATPLL
 */
__STATIC_INLINE void angleTrackingEmagUpdate(AT_PLL_ESTIM_T *pATPLL)
{

    /* Calculate BEMF */
    /* Define EMAG_COMP_OPTIM  to optimize the Emag Computation cycles */
#ifdef EMAG_COMP_OPTIM
    pATPLL->bemfMag = _IQmpy_mathacl(pATPLL->pMotor->Ke,pATPLL->speedFilt);
#else
    int32_t EmagSqr;

    EmagSqr = _IQmpy_mathacl(pATPLL->pEAlphaBeta->alpha, pATPLL->pEAlphaBeta->alpha) +
              _IQmpy_mathacl(pATPLL->pEAlphaBeta->beta,pATPLL->pEAlphaBeta->beta);

    pATPLL->bemfMag = _IQsqrt_mathacl(EmagSqr);

#endif

     pATPLL->invEqMagNorm = _IQdiv_mathacl(pATPLL->Edq.q,pATPLL->bemfMag);
}
#endif /* ALGOLIB_LIBRARIES_SEMICLOSELOOPESTIM_INCLUDE_ATPLL_H */
