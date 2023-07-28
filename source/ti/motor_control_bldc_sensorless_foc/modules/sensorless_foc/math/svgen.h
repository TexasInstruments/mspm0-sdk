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
 *  @file       svgen.h
 *  @brief      Sensorless FOC Motor Control Library Space Vector Generation 
 * Module
 *
 * 
 *  @anchor svgen_h
 *  # Overview
 *
 *  APIs for Space Vector Generation operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_SVGEN FOC_MATH/SVGEN
 * @{
 */
#ifndef SVGEN_H
#define SVGEN_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define svgen structure */
typedef struct 	{ 
	/*!  Input: reference alpha-axis phase voltage  */
	_iq  Ualpha;
	/*!  Input: reference beta-axis phase voltage  */
	_iq  Ubeta;
	/*!  Output: reference phase-a switching function		 */
	_iq  Ta;
	/*!  Output: reference phase-b switching function  */
	_iq  Tb;
	/*!  Output: reference phase-c switching function */
	_iq  Tc;
	/*!  Variable: temp variable */
	_iq  tmp1;
	/*!  Variable: temp variable */
	_iq  tmp2;
	/*!  Variable: temp variable */
	_iq  tmp3;
	/*!  Space vector sector */
	uint16_t VecSector;
} SVGEN_Instance;

/*! @brief Default values for SVGEN object */                
#define SVGEN_DEFAULTS { 0,0,0,0,0 }                       

/*! @brief 0.866 in IQ24 is 14529069 */  
#define _0P866_IQ24			(14529069)

/**
 * @brief     Run Space vector generation algorithm
 * @param[in] handle  A pointer to esmo instance
 */
__STATIC_INLINE void SVGEN_run(SVGEN_Instance *handle)
{
	handle->tmp1 = handle->Ubeta;
	handle->tmp2 = _IQdiv2(handle->Ubeta)
									  + (_IQ24mpy(_0P866_IQ24, handle->Ualpha));
    handle->tmp3 = handle->tmp2 - handle->tmp1;

	handle->VecSector = 3;
	handle->VecSector = (handle->tmp2 > 0)?
									  (handle->VecSector - 1):handle->VecSector;
	handle->VecSector = (handle->tmp3 > 0)?
									  (handle->VecSector - 1):handle->VecSector;
	handle->VecSector = (handle->tmp1 < 0)?
									    (7-handle->VecSector):handle->VecSector;
	
	if (handle->VecSector == 1 || handle->VecSector == 4)
	{
		handle->Ta = handle->tmp2;
		handle->Tb = handle->tmp1 - handle->tmp3;
		handle->Tc =-handle->tmp2;
	}
    else if(handle->VecSector == 2 || handle->VecSector == 5)
	{
		handle->Ta = handle->tmp3 + handle->tmp2;
		handle->Tb = handle->tmp1;
		handle->Tc =-handle->tmp1;
	}
    else
	{
		handle->Ta = handle->tmp3;
		handle->Tb =-handle->tmp3;
		handle->Tc =-(handle->tmp1 + handle->tmp2);
	}
}

#ifdef __cplusplus
}
#endif
#endif /* SVGEN_H */
/** @}*/
