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
 *  @file       ipark.h
 *  @brief      Sensorless FOC Motor Control Library Inverse Park Module
 *
 * 
 *  @anchor ipark_h
 *  # Overview
 *
 *  APIs for Ipark math operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_IPARK FOC_MATH/IPARK
 * @{
 */
#ifndef IPARK_H
#define IPARK_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define ipark structure */
typedef struct {  
    /*!  Output: stationary d-axis stator variable */
    _iq  Alpha;
    /*!  Output: stationary q-axis stator variable */
    _iq  Beta;
    /*!  Input: rotating angle (pu) */
    _iq  Angle;
    /*!  Input: rotating d-axis stator variable */
    _iq  Ds;
    /*!  Input: rotating q-axis stator variable */
    _iq  Qs;
    /*!  Input: Sine term */
    _iq  Sine;
    /*!  Input: Cosine term */
    _iq  Cosine;
} IPARK_Instance;

/*! @brief Default values for ipark instance*/                   
#define IPARK_DEFAULTS {  0, 0, 0, 0, 0, 0, 0 }

/**
 * @brief     Run ipark
 * @param[in] handle  A pointer to ipark instance
 */
__STATIC_INLINE void IPARK_run(IPARK_Instance *handle)
{
	handle->Alpha = _IQmpy(handle->Ds,handle->Cosine) 
											  - _IQmpy(handle->Qs,handle->Sine);
	handle->Beta  = _IQrepeat(handle->Qs,handle->Cosine) 
										   + _IQrepeat(handle->Ds,handle->Sine);
}

/**
 * @brief     Reset ipark variables
 * @param[in] handle  A pointer to ipark instance
 */
__STATIC_INLINE void IPARK_reset(IPARK_Instance *handle)
{
	handle->Alpha = 0;
	handle->Beta = 0;
}

#ifdef __cplusplus
}
#endif
#endif /* IPARK_H */
/** @}*/
