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
 *  @file       parameter.h
 *  @brief      Sensorless FOC Motor Control Library Parameter Module
 *
 * 
 *  @anchor parameter_h
 *  # Overview
 *
 *  APIs for FOC parameter operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_PARAMETER_PARAMETER FOC_PARAMETER/PARAMETER
 * @{
 */
#ifndef PARAMETER_H
#define PARAMETER_H

#include <stdint.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#include "hal.h"

#include "rampgen.h"
#include "rmp_cntl.h"
#include "clarke.h"
#include "park.h"
#include "ipark.h"
#include "pi.h"
#include "cirlimit.h"
#include "volt_calc.h"
#include "esmo/esmopos.h"
#include "speed_est.h"
#include "svgen.h"
#include "pwmgen.h"
#include "hal.h"
#include "motor_params.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Enumeration for the FOC parameter
 */
typedef enum {
    /*! Index associated to motor stator resistance */
    PARA_IDX_RS = 0,
    /*! Index associated to motor stator inductance */
    PARA_IDX_LS,
    /*! Index associated to motor poles */
    PARA_IDX_POLES,
    /*! Index associated to motor base rpm */
    PARA_IDX_BASE_RPM,
    /*! Index associated to pwm frequency */
    PARA_IDX_PWMFREQ,
    /*! Index associated to pwm period */
    PARA_IDX_PWMPERIOD,
    /*! Index associated to pwm cycles for adc sampling */
    PARA_IDX_PWMADCSAMPLE,
    /*! Index associated to control frequency divider */
    PARA_IDX_CNTRLDIV,
    /*! Index associated to deadband in ns */
    PARA_IDX_DEADBAND,
    /*! Index associated to deadband in pwm cycles */
    PARA_IDX_DEADBAND_CYC,
    /*! Index associated to factor for converting adc voltage to bus voltage */
    PARA_IDX_VOLT_RATIO,
    /*! Index associated to amplifier gain for current sensing */
    PARA_IDX_AMP_GAIN,
    /*! Index associated to shunt resistor in ohms */
    PARA_IDX_RSHUNT,
    /*! Index associated to kslide constant */
    PARA_IDX_KSLIDE,
    /*! Index associated to proportional gain for speed pi controller */
    PARA_IDX_PISPD_KP,
    /*! Index associated to integral gain for speed pi controller */
    PARA_IDX_PISPD_KI,
    /*! Index associated to max output value for speed pi controller */
    PARA_IDX_PISPD_MAX,
    /*! Index associated to min output value for speed pi controller */
    PARA_IDX_PISPD_MIN,
    /*! Index associated to execution divider for speed pi controller */
    PARA_IDX_PISPD_DIV,
    /*! Index associated to proportional gain for iq pi controller */
    PARA_IDX_PIIQ_KP,
    /*! Index associated to integral gain for iq pi controller */
    PARA_IDX_PIIQ_KI,
    /*! Index associated to min output value for iq pi controller */
    PARA_IDX_PIIQ_MAX,
    /*! Index associated to min output value for iq pi controller */
    PARA_IDX_PIIQ_MIN,
    /*! Index associated to proportional gain for id pi controller */
    PARA_IDX_PIID_KP,
    /*! Index associated to integral gain for id pi controller */
    PARA_IDX_PIID_KI,
    /*! Index associated to max output value for id pi controller */
    PARA_IDX_PIID_MAX,
    /*! Index associated to max output value for id pi controller */
    PARA_IDX_PIID_MIN,
    /*! Total number of parameters */
    PARA_IDX_MAX,
}PARA_IDX;

/**
 * @brief     Check for ESMO parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to esmo instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkESMO(float *parameter, ESMO_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for SPDEST parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to spdest instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkSPDEST(float *parameter, SPDEST_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PISPD parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pi instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPISPD(float *parameter, PI_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PIIQ parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pi instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPIIQ(float *parameter, PI_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PIID parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pi instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPIID(float *parameter, PI_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for RG parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to rg instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkRG(float *parameter, RAMPGEN_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PWMGEN parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pwmgen instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPWMGEN(float *parameter, PWMGEN_Instance *handle, PARA_IDX idx);

#ifdef __cplusplus
}
#endif
#endif /* PARAMETER_H */
/** @}*/

