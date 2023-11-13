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
 *  @file       park.h
 *  @brief      Sensorless FOC Motor Control Library Park Module
 *
 * 
 *  @anchor park_h
 *  # Overview
 *
 *  APIs for Park math operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_PARK FOC_MATH/PARK
 * @{
 */
#ifndef PARK_H
#define PARK_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define park structure */
typedef struct {  
	/*!  Stationary d-axis stator variable  */
	_iq  Alpha;
	/*!  Stationary q-axis stator variable  */
	_iq  Beta;
	/*!  Rotating angle (pu)  */
	uint32_t  Angle;
	/*!  Rotating d-axis stator variable  */
	_iq  Ds;
	/*!  Rotating q-axis stator variable */
	_iq  Qs;
	/*!  Sine of rotor angle */
	_iq  Sine;
	/*!  Cosine of rotor angle */
	_iq  Cosine; 	 
} PARK_Instance;

/*! @brief Default values for park instance */                  
#define PARK_DEFAULTS { 0, 0, 0, 0, 0, 0, 0 }

/**
 * @brief     Run park
 * @param[in] handle  A pointer to park instance
 */
__STATIC_INLINE void PARK_run(PARK_Instance *handle)
{
    handle->Ds = _IQmpy(handle->Alpha,handle->Cosine)
										    + _IQmpy(handle->Beta,handle->Sine);
    handle->Qs = _IQrepeat(handle->Beta,handle->Cosine)
										- _IQrepeat(handle->Alpha,handle->Sine);
}

/**
 * @brief     Reset park variables
 * @param[in] handle  A pointer to park instance
 */
__STATIC_INLINE void PARK_reset(PARK_Instance *handle)
{
	handle->Ds = 0;
	handle->Qs = 0;
}

#ifdef __cplusplus
}
#endif
#endif /* PARK_H */
/** @}*/
