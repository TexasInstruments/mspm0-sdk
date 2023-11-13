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
 *  @file       pwmgen.h
 *  @brief      Sensorless FOC Motor Control Library PWM Generation Module
 *
 * 
 *  @anchor pwmgen_h
 *  # Overview
 *
 *  APIs for PWM Generation operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_PWMGEN FOC_MATH/PWMGEN
 * @{
 */
#ifndef PWMGEN_H
#define PWMGEN_H

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#include "user.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define pwmgen structure */
typedef struct {
    /*!  Half of pwm period */
    uint16_t HalfPerMax;
    /*!  Maximum dutycycle */
    uint32_t dutyMax;
    /*!  Minimum dutycycle */
    uint32_t dutyMin;
    /*!  A phase dutycycle in range (-1,1) */
    _iq MfuncC1;
    /*!  B phase dutycycle in range (-1,1) */
    _iq MfuncC2;
    /*!  C phase dutycycle in range (-1,1) */
    _iq MfuncC3;
    /*!  PWM compare value calculated for A phase */
    uint32_t dutyA;
    /*!  PWM compare value calculated for B phase */
    uint32_t dutyB;
    /*!  PWM compare value calculated for C phase */
    uint32_t dutyC;
} PWMGEN_Instance ;

/*! @brief Default values for pwmgen instance */
#define PWMGEN_DEFAULTS    {  0,0,0,0,0,0,0,0,0 }

/*! @brief Define pwmgen parameter structure */
typedef struct {
    /*!  Period for PWM */
    uint32_t pwmPeriod;
    /*!  PWM cycles spend in deadtime */
    uint32_t deadband;
} PWMGEN_PARA;

/**
 * @brief     Update the pwmgen parameters
 * @param[in] handle  A pointer to pwmgen instance
 * @param[in] para    A pointer to parameter array
 */
__STATIC_INLINE void PWMGEN_UpdateParams(PWMGEN_Instance *handle,
                                                              PWMGEN_PARA *para)
{
    handle->HalfPerMax =  para->pwmPeriod>>1;
    handle->dutyMin = para->deadband + USER_DEFAULT_FOC_ADC_SETTLE_DELAY 
                                                + USER_DEFAULT_FOC_PWMADCSAMPLE;
    handle->dutyMax = para->pwmPeriod - handle->dutyMin;
}

/**
 * @brief     Limit to max and min duty limits
 * @param[in] duty     Dutycycle input to limit
 * @param[in] maxduty  Max dutycycle to limit to
 * @param[in] minduty  Min dutycycle to limit to
 */
__STATIC_INLINE void PWMGEN_checkLimits(uint32_t *duty,
                                             uint32_t maxduty, uint32_t minduty)
{
    if(*duty > maxduty)
    {
        *duty = maxduty;
    }

    if(*duty < minduty)
    {
        *duty = minduty;
    }
}

/**
 * @brief     Run pwmgen algorithm
 * @param[in] handle  A pointer to pwmgen instance
 */
__STATIC_INLINE void PWMGEN_run(PWMGEN_Instance *handle)
{
    handle->dutyA = handle->HalfPerMax
                                  - _IQmpy(handle->HalfPerMax, handle->MfuncC1);
    handle->dutyB = handle->HalfPerMax
                                  - _IQmpy(handle->HalfPerMax, handle->MfuncC2);
    handle->dutyC = handle->HalfPerMax
                                  - _IQmpy(handle->HalfPerMax, handle->MfuncC3);

    PWMGEN_checkLimits(&handle->dutyA, handle->dutyMax, handle->dutyMin);
    PWMGEN_checkLimits(&handle->dutyB, handle->dutyMax, handle->dutyMin);
    PWMGEN_checkLimits(&handle->dutyC, handle->dutyMax, handle->dutyMin);
}

#ifdef __cplusplus
}
#endif
#endif /* PWMGEN_H */
/** @}*/
