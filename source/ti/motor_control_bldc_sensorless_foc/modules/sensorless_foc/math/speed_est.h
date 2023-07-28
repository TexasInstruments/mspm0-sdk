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
 *  @file       speed_est.h
 *  @brief      Sensorless FOC Motor Control Library Speed Estimator Module
 *
 * 
 *  @anchor speed_est_h
 *  # Overview
 *
 *  APIs for Speed Estimator operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_SPEED_EST FOC_MATH/SPEED_EST
 * @{
 */
#ifndef SPEED_EST_H
#define SPEED_EST_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#include "angle_math.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define speed estimator structure */
#define SPDEST_PI                           (3.14159265358979)

/*! @brief Cutodf frequency for the low pass filter in speed estimator */
#define SPDEST_FILTER_CUTOFF_FREQ          (5)

/*! @brief Define speed estimator structure */
typedef struct {
    /*! Estimated rotor angle electrical  */
    _iq EstimatedTheta;
    /*! Previous estimated rotor angle electrical  */
    _iq OldEstimatedTheta;
    /*! Estimated speed in pu */
    _iq EstimatedSpeed;
    /*! Base rpm */
    uint32_t BaseRpm;
    /*! Constant for differentiator */
    _iq21 K1;
    /*! Constant for low-pass filter */
    _iq21 K2;
    /*! Constant for low-pass filter */
    _iq21 K3;
    /*! Estimated speed in rpm */
    int32_t EstimatedSpeedRpm;
    /*! Temporary variable */
    _iq21 Temp;
} SPDEST_Instance;

 /*! @brief Default values for SPEED_ESTIMATION object */
 #define SPDEST_DEFAULTS   { 0, 0, 0, 0, 0, 0, 0, 0, 0 }

/*! @brief Define speed estimator parameter structure */
typedef struct {
    /*! Base frequency in hz */
    float baseFreq;
    /*! Base rpm */
    float baseRpm;
    /*! Sampling time */
    float Ts;
    /*! Number of poles */
    float poles;
} SPDEST_PARA;

/**
 * @brief     Update the speed estimator parameters
 * @param[in] handle  A pointer to speed estimator instance
 * @param[in] para    A pointer to parameter array
 */
__STATIC_INLINE void SPDEST_UpdateParams(SPDEST_Instance *handle,
                                                              SPDEST_PARA *para)
{
    handle->K1 = _IQ21(1/(para->baseFreq*para->Ts));
    handle->K2 = _IQ21(1/(1+para->Ts*2*SPDEST_PI*SPDEST_FILTER_CUTOFF_FREQ));
    handle->K3 = _IQ21(1)-(handle->K2);
    handle->BaseRpm = para->baseRpm;
}

/**
 * @brief     Run speed estimator
 * @param[in] handle  A pointer to speed estimator instance
 */
__STATIC_INLINE void SPDEST_run(SPDEST_Instance *handle)
{
    /* Synchronous speed computation   */
    handle->Temp = handle->EstimatedTheta - handle->OldEstimatedTheta;
    ANGLE_WRAP(&handle->Temp);
	handle->Temp = _IQ21mpy(handle->K1, _IQtoIQ21(handle->Temp));

    /* Low-pass filter */
    /* Q21 = Q21*Q21 + Q21*Q21 */
	handle->Temp = _IQ21mpy(handle->K2, _IQtoIQ21(handle->EstimatedSpeed))
                                              +_IQ21mpy(handle->K3, handle->Temp);

    /* Saturate the output */
	handle->Temp=_IQsat(handle->Temp,_IQ21(1),_IQ21(-1));
	handle->EstimatedSpeed = _IQ21toIQ(handle->Temp);

    /* Update the electrical angle */
	handle->OldEstimatedTheta = handle->EstimatedTheta;

    /* Change motor speed from pu value to rpm value (GLOBAL_Q -> Q0)*/
    /* Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q*/
    handle->EstimatedSpeedRpm = _IQmpy(handle->BaseRpm, handle->EstimatedSpeed);
}

#ifdef __cplusplus
}
#endif
#endif /* SPEED_EST_H */
/** @}*/
