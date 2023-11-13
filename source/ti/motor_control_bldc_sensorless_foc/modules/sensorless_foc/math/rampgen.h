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
 *  @file       rampgen.h
 *  @brief      Sensorless FOC Motor Control Library Ramp Generation Module
 *
 * 
 *  @anchor rampgen_h
 *  # Overview
 *
 *  APIs for Ramp Generation operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_RAMPGEN FOC_MATH/RAMPGEN
 * @{
 */
#ifndef RAMPGEN_H
#define RAMPGEN_H

#include "angle_math.h"

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define rampgen structure */
typedef struct { 
	/*!  Ramp frequency Hz*/
	_iq  Freq;
	/*!  Maximum step angle*/
	_iq  StepAngleMax;
	/*!  Step angle pu */
	uint32_t  Angle;
	/*!  Ramp output pu */
	uint32_t  Out;
} RAMPGEN_Instance;

/*! @brief Default values for RAMPGEN instance */        
#define RAMPGEN_DEFAULTS {0, 0, 0, 0 }

/*! @brief Define ramp generator estimator parameter structure */
typedef struct {
    /*! Sampling time */
    int32_t Ts;
} RAMPGEN_PARA;

/**
 * @brief     Update the ramp generator parameters
 * @param[in] handle  A pointer to ramp generator instance
 * @param[in] para    A pointer to parameter array
 */
__STATIC_INLINE void RAMPGEN_UpdateParams(RAMPGEN_Instance *handle,
															 RAMPGEN_PARA *para)
{
    handle->StepAngleMax = para->Ts;
}

/*!
 * @brief Run RAMP(Sawtooth) Generator
 * @param[in] handle  A pointer to ramp generator instance
 */
__STATIC_INLINE void RG_run(RAMPGEN_Instance *handle)
{
	/* Compute the angle rate */
	handle->Angle += (uint32_t)_ANGLE_IQtoIQ32(_IQmpy(handle->StepAngleMax,
																 handle->Freq));

    handle->Out = handle->Angle;
}

/*!
 * @brief Reset RAMP(Sawtooth) Generator
 * @param[in] handle  A pointer to ramp generator instance
 */
void RG_reset(RAMPGEN_Instance *handle);

#ifdef __cplusplus
}
#endif
#endif /* RAMPGEN_H */
/** @}*/
