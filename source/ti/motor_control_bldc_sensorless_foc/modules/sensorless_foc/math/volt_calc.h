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
 *  @file       volt_calc.h
 *  @brief      Sensorless FOC Motor Control Library Phase Voltage Calculation 
 * Module
 *
 * 
 *  @anchor volt_calc_h
 *  # Overview
 *
 *  APIs for Phase Voltage Calculation operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_VOLT_CALC FOC_MATH/VOLT_CALC
 * @{
 */
#ifndef VOLT_CALC_H
#define VOLT_CALC_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief 1/3 in global IQ
 */
#define ONE_THIRD  (_IQ(0.33333333333333))

/*!
 * @brief 1/sqrt(3) in global IQ
 */
#define INV_SQRT3  (_IQ(0.57735026918963))

/*! @brief Define phase voltage structure */
typedef struct 	{ 
	/*!  Input: DC-bus voltage (pu) */
	_iq  DcBusVolt;
	/*!  Input: Modulation voltage phase A (pu) */
	_iq  MfuncV1;
	/*!  Input: Modulation voltage phase B (pu)	 */
	_iq  MfuncV2;
	/*!  Input: Modulation voltage phase C (pu)  */
	_iq  MfuncV3;
	/*!  Output: Phase voltage phase A (pu) */
	_iq  VphaseA;
	/*!  Output: Phase voltage phase B (pu)  */
	_iq  VphaseB;
	/*!  Output: Stationary d-axis phase voltage (pu) */
	_iq  Valpha;
	/*!  Output: Stationary q-axis phase voltage (pu) */
	_iq  Vbeta;
	/*!  Variable: temp variable */
	_iq  temp;
} PHASEVOLT_Instance;	                   

/*! @brief Default values for PHASEVOLTAGE object */                  
#define PHASEVOLT_DEFAULTS { 0, 0, 0, 0, 1, 0, 0, 0 }

/**
 * @brief     Run phase voltage calculation
 * @param[in] handle  A pointer to phase voltage instance
 */
__STATIC_INLINE void PHASEVOLT_run(PHASEVOLT_Instance *handle)
{
	/* Scale the incoming Modulation functions with the DC bus voltage value */
	/* and calculate the 3 Phase voltages */								   
	handle->temp = _IQmpy(handle->DcBusVolt, ONE_THIRD);                                    
	handle->VphaseA = _IQmpy(handle->temp,
				(_IQmpy2(handle->MfuncV1) - handle->MfuncV2 - handle->MfuncV3));       
	handle->VphaseB = _IQmpy(handle->temp,
			   (_IQmpy2(handle->MfuncV2)  - handle->MfuncV1 - handle->MfuncV3)); 
	      
	/* Voltage transformation (a,b,c)  ->  (Alpha,Beta)	*/					   
	handle->Valpha = handle->VphaseA;													   
	handle->Vbeta = _IQmpy((handle->VphaseA
										 + _IQmpy2(handle->VphaseB)),INV_SQRT3);
}

#ifdef __cplusplus
}
#endif
#endif /* VOLT_CALC_H */
/** @}*/
