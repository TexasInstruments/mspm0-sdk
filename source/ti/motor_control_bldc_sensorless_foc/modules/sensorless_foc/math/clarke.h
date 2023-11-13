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
 *  @file       clarke.h
 *  @brief      Sensorless FOC Motor Control Library Clarke Module
 *
 * 
 *  @anchor clarke_h
 *  # Overview
 *
 *  APIs for Clarke math operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_CLARKE FOC_MATH/CLARKE
 * @{
 */
#ifndef CLARKE_H
#define CLARKE_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define clarke structure */
typedef struct {  
	/*!  Phase-a stator variable */
	_iq  As;
	/*!  Phase-b stator variable */
	_iq  Bs;
	/*!  Phase-c stator variable */
	_iq  Cs;
	/*!  Stationary d-axis stator variable */
	_iq  Alpha;
	/*!  Stationary q-axis stator variable */
	_iq  Beta;
} CLARKE_Instance;


/*! @brief Default values for the clarke instance */                    
#define CLARKE_DEFAULTS					{ 0, 0, 0, 0, 0 } 

/*!
 * @brief 1/sqrt(3) in IQ format
 */
#define  ONEbySQRT3_IQ				_IQ(0.57735)

/*!
 * @brief Clarke transform macro (with 2 currents)
 * @param[in] handle   A handle to the clarke instance
 */
__STATIC_INLINE void CLARKE_run(CLARKE_Instance *handle)
{
	handle->Alpha = handle->As;
	handle->Beta = _IQmpy((handle->As + _IQmpy2(handle->Bs)), ONEbySQRT3_IQ);
}

/**
 * @brief     Reset clarke variables
 * @param[in] handle  A pointer to clarke instance
 */
__STATIC_INLINE void CLARKE_reset(CLARKE_Instance *handle)
{
	handle->Alpha = 0;
	handle->Beta = 0;
}

#ifdef __cplusplus
}
#endif
#endif /* CLARKE_H */
/** @}*/
