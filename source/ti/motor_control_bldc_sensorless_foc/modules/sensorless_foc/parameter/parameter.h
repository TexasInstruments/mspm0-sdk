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

#include "ipd.h"
#include "rampgen.h"
#include "rmp_cntl.h"
#include "clarke.h"
#include "park.h"
#include "ipark.h"
#include "pi.h"
#include "volt_calc.h"
#include "estimator.h"
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
    PARA_IDX_USER_RS = 0,
    /*! Index associated to motor stator inductance */
    PARA_IDX_USER_LS,
    /*! Index associated to motor backemf constant */
    PARA_IDX_USER_KE,
    /*! Index associated to motor maximum frequency */
    PARA_IDX_USER_MAX_FREQ,
    /*! Index associated to pwm frequency */
    PARA_IDX_USER_PWMFREQ,
    /*! Index associated to deadband in ns */
    PARA_IDX_USER_DEADBAND,
    /*! Index associated to amplifier gain */
    PARA_IDX_USER_AMP_GAIN,
    /*! Index associated to outer loop */
    PARA_IDX_USER_OUTER_LOOP,
    /*! Index associated to direction reverse */
    PARA_IDX_USER_DIRECTION_REVERSE,
    /*! Index associated to startup method */
    PARA_IDX_USER_STARTUP_METHOD,
    /*! Index associated to slow first cycle frequency */
    PARA_IDX_USER_SLOW_FIRST_CYC_FREQ,
    /*! Index associated to align current in Ampere */
    PARA_IDX_USER_ALIGN_CURRENT,
    /*! Index associated to align time in seconds */
    PARA_IDX_USER_ALIGN_TIME,
    /*! Index associated to IPD current threshold */
    PARA_IDX_USER_IPD_CURRENT_THRESHOLD,
    /*! Index associated to IPD frequency in Hz */
    PARA_IDX_USER_IPD_FREQ,
    /*! Index associated to rampup current in Ampere */
    PARA_IDX_USER_RAMPUP_CURRENT,
    /*! Index associated to rampup rate */
    PARA_IDX_USER_RAMPUP_RATE,
    /*! Index associated to rampup target in Hz/sec */
    PARA_IDX_USER_RAMPUP_TARGET,
    /*! Index associated to speed reference ramp rate in Hz */
    PARA_IDX_USER_SPDREF_RAMP_RATE,
    /*! Index associated to proportional gain for speed pi controller */
    PARA_IDX_USER_PISPD_KP,
    /*! Index associated to integral gain for speed pi controller */
    PARA_IDX_USER_PISPD_KI,
    /*! Index associated to execution divider for speed pi controller */
    PARA_IDX_USER_PISPD_DIV,
    /*! Index associated to proportional gain for iq pi controller */
    PARA_IDX_USER_PIIQ_KP,
    /*! Index associated to integral gain for iq pi controller */
    PARA_IDX_USER_PIIQ_KI,
    /*! Index associated to proportional gain for id pi controller */
    PARA_IDX_USER_PIID_KP,
    /*! Index associated to integral gain for id pi controller */
    PARA_IDX_USER_PIID_KI,
    /*! Index associated to over voltage limit */
    PARA_IDX_USER_OVER_VOLTAGE_LIMIT,
    /*! Index associated to under voltage limit */
    PARA_IDX_USER_UNDER_VOLTAGE_LIMIT,
    /*! Index associated to over current limit */
    PARA_IDX_USER_OVER_CURRENT_LIMIT,
    /*! Total number of user parameters */
    PARA_IDX_USER_MAX,
    /*! Index for system parameter deadband cycles */
    PARA_IDX_SYS_DEADBAND_CYC,
    /*! Index for system parameter PWM period */
    PARA_IDX_SYS_PWMPERIOD,
    /*! Index for system parameter sampling time */
    PARA_IDX_SYS_TS,
    /*! Index for system parameter sampling frequency */
    PARA_IDX_SYS_SAMPLE_FREQ,
    /*! Total number of user and system parameters */
    PARA_IDX_MAX,
}PARA_IDX;

/**
 * @brief     Check for EST parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to est instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkEST(uint32_t *parameter, EST_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PISPD parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pi instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPISPD(uint32_t *parameter, PI_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PIIQ parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pi instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPIIQ(uint32_t *parameter, PI_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PIID parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pi instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPIID(uint32_t *parameter, PI_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for RC parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to rc instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkRC(uint32_t *parameter, RMPCNTL_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for RG parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to rg instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkRG(uint32_t *parameter, RAMPGEN_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for PWMGEN parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to pwmgen instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkPWMGEN(uint32_t *parameter, PWMGEN_Instance *handle, PARA_IDX idx);

/**
 * @brief     Check for IPD parameter changes
 * @param[in] parameter  Pointer to the parameter array.
 * @param[in] handle     A pointer to ipd instance
 * @param[in] idx        Index of the parameter changed. One of @ref PARA_IDX
 */
void PARA_checkIPD(uint32_t *parameter, IPD_Instance *handle, PARA_IDX idx);

#ifdef __cplusplus
}
#endif
#endif /* PARAMETER_H */
/** @}*/

