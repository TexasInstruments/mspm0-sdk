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
 *  @file       rmp_cntl.h
 *  @brief      Sensorless FOC Motor Control Library Ramp Control Module
 *
 * 
 *  @anchor rmp_cntl_h
 *  # Overview
 *
 *  APIs for Ramp Control operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_RMP_CNTL FOC_MATH/RMP_CNTL
 * @{
 */
#ifndef RMP_CNTL_H
#define RMP_CNTL_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

/* Extended IQmath Library for rts */
#include "iqmath_rts.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define rmpcntrl structure */
typedef struct {
    /*! Target value of ramp */
	_iq TargetValue;
    /*! Ramp max delay used to divide the delay rate */
	uint32_t RampDelayMax;
    /*! Lower limit for ramp */
	_iq RampLowLimit;
    /*! High limit for ramp */
	_iq RampHighLimit;
    /*! Ramp delay counter */
	uint32_t RampDelayCount;
	/*! Ramp output value */
	_iq SetpointValue;
    /*! Ramp delta */
    _iq RampDelta;
	/*! Temporary variable to hold the difference of target and current ramp
	output */
	_iq	Tmp;
} RMPCNTL_Instance;

/*! @brief Default values for RMPCNTL instance */       
#define RMPCNTL_DEFAULTS {  0, 		 \
                            1,		 \
                           _IQ(-1),  \
                           _IQ(1),   \
                            0,       \
                          	0,       \
                          	0,       \
                            0,       \
                   		  }

/*! @brief Define ramp control parameter structure */
typedef struct {
    /*! RampDelta in Hz per cycle */
    int32_t RampDelta;
    /*! Maximum frequency in Hz */
    int32_t maxFreq;
    /*!  Sampling time in sec */
    int32_t Ts;
} RMPCNTL_PARA;

/**
 * @brief     Update the ramp control parameters
 * @param[in] handle  A pointer to ramp control instance
 * @param[in] para    A pointer to parameter array
 */
__STATIC_INLINE void RMPCNTL_UpdateParams(RMPCNTL_Instance *handle,
                                                          RMPCNTL_PARA *para)
{
    handle->RampDelta = _IQmpy_rts(para->RampDelta ,para->Ts);
    handle->RampHighLimit = _IQ(para->maxFreq);
    handle->RampLowLimit = 0;
}

/**
 * @brief     Run ramp control
 * @param[in] handle  A pointer to rmpcntrl instance
 */
__STATIC_INLINE void RC_run(RMPCNTL_Instance *handle)
{
	handle->Tmp = handle->TargetValue - handle->SetpointValue;
	if (_IQabs(handle->Tmp) >= handle->RampDelta)
	{
		handle->RampDelayCount++;
		if (handle->RampDelayCount >= handle->RampDelayMax)
		{
			if (handle->TargetValue >= handle->SetpointValue)
			{
				handle->SetpointValue += handle->RampDelta;
			}
			else
			{
				handle->SetpointValue -= handle->RampDelta;
			}
			handle->SetpointValue =_IQsat(handle->SetpointValue,
								   handle->RampHighLimit, handle->RampLowLimit);
			handle->RampDelayCount = 0;
		}
	}
}

/**
 * @brief     Reset ramp control variables
 * @param[in] handle  A pointer to ramp control instance
 */
__STATIC_INLINE void RC_reset(RMPCNTL_Instance *handle)
{
	handle->TargetValue = 0;
	handle->SetpointValue = 0;
}

#ifdef __cplusplus
}
#endif
#endif /* RMP_CNTL_H */
/** @}*/
