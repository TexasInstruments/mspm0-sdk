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
 *  @file       pi.h
 *  @brief      Sensorless FOC Motor Control Library PI Controller Module
 *
 * 
 *  @anchor pi_h
 *  # Overview
 *
 *  APIs for PI controller operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_PI FOC_MATH/PI
 * @{
 */
#ifndef PI_H
#define PI_H

#include "angle_math.h"

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define pi structure */
typedef struct {  
	/*!  Input: reference set-point */
	_iq  ref;
	/*!  Input: feedback */
	_iq  fbk;
	/*!  Data: computed error */
	_iq  error;
	/*!  Parameter: proportional loop gain */
	_iq  Kp;
	/*!  Parameter: integral gain */
	_iq  Ki;
	/*!  Parameter: upper saturation limit */
	_iq  outMax;
	/*!  Parameter: lower saturation limit */
	_iq  outMin;
	/*!  Parameter: upper saturation limit for integral */
	_iq  outIMax;
	/*!  Parameter: lower saturation limit for integral */
	_iq  outIMin;
	/*!  Data: proportional term */
	_iq  outP;
	/*!  Data: integral term */
	_iq  outI;
	/*!  Data: pre-saturated controller output */
	_iq  preOut;
    /*!  Output: controller output  */
    _iq  out;
} PI_Instance;

/*! @brief Default values for pi instance */
#define PI_DEFAULTS {											   			   \
						   0, 												   \
                           0, 												   \
                           _IQ(1),                                             \
                           0,										   		   \
                           0,										   		   \
                           _IQ(1),										   	   \
                           -_IQ(1), 										   \
                           _IQ(1),										   	   \
                           -_IQ(1), 										   \
                           0,										   		   \
                           0,										   		   \
                           0,										   		   \
                           0,										   		   \
              			  }

/*! @brief Define pi parameter structure */						  
typedef struct {
    /*!  PI proportionality constant */
    int32_t Kp;
    /*!  PI integral constant */
    int32_t Ki;
} PI_PARA;

/**
 * @brief     Update the pi parameters
 * @param[in] handle  A pointer to pi instance
 * @param[in] para  A pointer to parameter array
 */
__STATIC_INLINE void PI_UpdateParams(PI_Instance *handle, PI_PARA *para)
{
    handle->Kp = para->Kp;
    handle->Ki = para->Ki;
}

/**
 * @brief     Run pi controller
 * @param[in] handle  A pointer to pi instance
 */
__STATIC_INLINE void PI_run(PI_Instance *handle)
{
    handle->error = (handle->ref - handle->fbk);
	/* proportional term */
    handle->outP = _IQmpy(handle->Kp, handle->error);

	/* integral term */
	handle->outI += _IQrepeat(handle->Ki, handle->error);
	handle->outI = _IQsat(handle->outI, handle->outIMax, handle->outIMin);

	/* control output */
	handle->preOut = handle->outP + handle->outI;
	handle->out = _IQsat(handle->preOut , handle->outMax, handle->outMin);
}

/*!
 * @brief Resets the PI variables
 * @param[in] handle  A pointer to pi instance
 */
void PI_reset(PI_Instance *handle);

#ifdef __cplusplus
}
#endif
#endif /* PI_H */
/** @}*/
