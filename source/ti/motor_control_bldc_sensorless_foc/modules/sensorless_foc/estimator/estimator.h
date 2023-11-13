/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  @brief      Sensorless FOC Motor Control Library EST Module
 *
 * 
 *  @anchor estimator_h
 *  # Overview
 *
 *  APIs for Estimator math operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_EST FOC_EST/EST
 * @{
 */
#ifndef EST_H
#define EST_H

#include <ti/devices/msp/msp.h>
#include <math.h>
#include "pi.h"
#include "angle_math.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Extended IQmath Library for rts */
#include "iqmath_rts.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define estimator structure */
typedef struct {  
    /*! Stationary alfa-axis stator voltage */
    _iq Valpha;
    /*! Stationary beta-axis stator voltage */
    _iq Vbeta;
    /*! Stationary alfa-axis back EMF */
    _iq Ealpha;
    /*! Stationary beta-axis back EMF */
    _iq Ebeta;
    /*! Filtered Ealpha */
    _iq Ealphaflt;
    /*! Filtered Ebeta */
    _iq Ebetaflt;
    /*! Motor dependent gain */
    _iq Fsmopos;
    /*! Motor dependent gain */
    _iq Gsmopos;
    /*! Stationary alfa-axis stator current */
    _iq Ialpha;
    /*! Stationary beta-axis stator current */
    _iq Ibeta;
    /*! Scaled Valpha */
    _iq Valpha1;
    /*! Scaled Vbeta */
    _iq Vbeta1;
    /*! Scaled Ialpha */
    _iq Ialpha1;
    /*! Scaled Ibeta */
    _iq Ibeta1;
    /*! Previous Ialpha1 */
    _iq pIalpha1;
    /*! Previous Ibeta1 */
    _iq pIbeta1;
    /*! Estimated rotor angle */
    uint32_t estTheta;
    /*! Compensated rotor angle */
    uint32_t Theta;
    /*! Theta scale factor */
    _iq thetaSF;
    /*! PLL proportionality constant */
    _iq pllKp;
    /*! PLL integral constant */
    _iq pllKi;
    /*! Ki scale factor */
    _iq kiSF;
    /*! PLL integral */
    _iq pllInt;
    /*! PLL output */
    _iq pllOut;
    /*! Rotor Omega */
    _iq omega;
    /*! Rotor Omega scaled */
    _iq omegaScaled;
    /*! Computed rotor frequency in hz*/
    _iq freq;
    /*! Sine value */
    _iq SinTheta;
    /*! Cosine value */
    _iq CosTheta;
    /*! Error */
    _iq dE;
    /*! Sampling time */
    _iq Ts;
    /*! PLL output scale factor */
    _iq pllOutSF;
    /*! Omega scale factor */
    _iq omegaSF;
    /*! Counter */
    uint32_t cnt;
    /*! Angle compensation */
    _iq cycleComp;
} EST_Instance;

/*! @brief Default values for estimator instance */
#define EST_DEFAULTS {  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\
                        0,0,0\
                      }

/*! @brief Define estimator parameter structure */
typedef struct {
    /*! Stator resistor in ohms */
    int32_t Rs;
    /*! Stator inductance in H */
    int32_t Ls;
    /*! Base frequency in hz */
    int32_t baseFreq;
    /*! Sampling time in sec */
    int32_t Ts;
    /*! Backemf constant */
    int32_t ke;
} EST_PARA;

/**
 * @brief     Update the estimator parameters
 * @param[in] handle  A pointer to estimator instance
 * @param[in] para    A pointer to parameter array
 */
void EST_UpdateParams(EST_Instance *handle, EST_PARA *para);

/**
 * @brief     Run estimator algorithm
 * @param[in] handle  A pointer to estimator instance
 */
void EST_run(EST_Instance *handle);

/**
 * @brief     Reset the estimator variables
 * @param[in] handle  A pointer to estimator instance
 */
void EST_reset(EST_Instance *handle);

#ifdef __cplusplus
}
#endif
#endif /* EST_H */
/** @}*/
