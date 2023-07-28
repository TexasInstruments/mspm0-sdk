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
 *  @file       esmopos.h
 *  @brief      Sensorless FOC Motor Control Library ESMO Module
 *
 * 
 *  @anchor esmopos_h
 *  # Overview
 *
 *  APIs for Estimator math operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_ESMO FOC_MATH/ESMO
 * @{
 */
#ifndef ESMOPOS_H
#define ESMOPOS_H

#include <ti/devices/msp/msp.h>
#include <math.h>
#include "pi.h"
#include "angle_math.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief 2PI is 6.283185307179586, this is 105414357 in IQ24 format
 */
#define TWO_PI_IQ24                     (105414357)

/*! @brief Define esmo structure */
typedef struct {  
    /*!  Input: Stationary alfa-axis stator voltage */
    _iq  Valpha;   	 
    /*!  Variable: Stationary alfa-axis back EMF */
    _iq  Ealpha;
    /*!  Output: Stationary alfa-axis sliding control */
    _iq  Zalpha;
    /*!  Parameter: Motor dependent control gain */
    _iq  Gsmopos;
    /*!  Variable: Estimated stationary alfa-axis stator current*/
    _iq  EstIalpha;
    /*!  Parameter: Motor dependent plant matrix */
    _iq  Fsmopos;
    /*!  Input: Stationary beta-axis stator voltage */
    _iq  Vbeta;
    /*!  Variable: Stationary beta-axis back EMF */
    _iq  Ebeta;
    /*!  Output: Stationary beta-axis sliding control */
    _iq  Zbeta;
    /*!  Variable: Estimated stationary beta-axis stator current*/
    _iq  EstIbeta;
    /*!  Input: Stationary alfa-axis stator current */
    _iq  Ialpha;
    /*!  Variable: Stationary alfa-axis current error */
    _iq  IalphaError;                
    /*!  Parameter: Sliding control gain */
    _iq  Kslide;
    /*!  Input: Stationary beta-axis stator current */
    _iq  Ibeta;
    /*!  Variable: Stationary beta-axis current error */
    _iq  IbetaError;                
    /*!  Parameter: Sliding control filter gain */
    _iq  Kslf;
    /*!  Output: Compensated rotor angle */
    _iq  Theta;
    /*!  Output:  rotor angle */
    _iq  EstTheta;
    /*!  Parameter: 0.5 */
    _iq  E0;
    /*!  smo filter frequency */
    _iq  smoFreq;
    /*!  smo filter Minimum frequency */
    _iq  smoFreqMin;
    /*!  phase shift due to smo filter */
    _iq  smoShift;
    /*!  running electrical speed of motor */
    _iq  runSpeed;
    /*!  commanded speed of motor */
    _iq  cmdSpeed;
    /*!  base_wTs = basefreq * sampling time */
    _iq  base_wTs;
    /*!  Negative direction compensation */
    _iq  ThetaRevComp;
} ESMO_Instance;

/*! @brief Default values for esmo instance */
#define ESMO_DEFAULTS {  0,0,0,0,0,0,0,0,0,0,0,                                \
                        0,0,0,0,0,0,0,_IQ(0.5),                                \
                        0,_IQ(0.2),0,0,0,0,_IQ(0.5)                            \
                      }

/*! @brief Define esmo parameter structure */
typedef struct {
    /*!  Stator resistor in ohms */
    float Rs;
    /*!  Stator inductance in H */
    float Ls;
    /*!  Base scale factor */
    float baseSF;
    /*!  Base frequency in hz */
    float baseFreq;
    /*!  Sampling time in sec */
    float Ts;
    /*!  Kslide constant */
    float kslide;
    /*!  Calculated constant F */
    float Fsmopos;
    /*!  Calculated constant G */
    float Gsmopos;
} ESMO_PARA;

/**
 * @brief     Update the esmo parameters
 * @param[in] handle  A pointer to esmo instance
 * @param[in] para    A pointer to parameter array
 */
void ESMO_UpdateParams(ESMO_Instance *handle, ESMO_PARA *para);


/**
 * @brief     Run esmo algorithm
 * @param[in] handle  A pointer to esmo instance
 */
void ESMO_run(ESMO_Instance *handle);

#ifdef __cplusplus
}
#endif
#endif /* ESMOPOS_H */
/** @}*/