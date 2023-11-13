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
 *  @file       param_drv8323rs.h
 *  @brief      Sensorless FOC Motor Control Library drv8323rs tuning parameters
 *
 * 
 *  @anchor param_drv8323rs_h
 *  # Overview
 *
 *  tuning parameters
 *
 *  <hr>
 ******************************************************************************/
#ifndef PARAM_DRV8323RS_H
#define PARAM_DRV8323RS_H

#include <stdint.h>
#include "foc_types.h"
#include "motor_params.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief DC bus voltage divider ratio */
#define USER_DEFAULT_FOC_VOLT_RATIO             DRV8323RS_DEFAULT_VOLT_R_DIVIDER

/** @brief DRV shunt resistor in ohms */
#define USER_DEFAULT_DRV_RSHUNT                 DRV8323RS_DEFAULT_R_SHUNT

/** @brief DRV gain to map to FOC_CSA_GAIN_0 */
#define  DRV_CSA_GAIN_MAP_0                     DRV8323RS_CSA_CONTROL_CSA_GAIN_5VPV

/** @brief DRV gain to map to FOC_CSA_GAIN_1 */
#define  DRV_CSA_GAIN_MAP_1                     DRV8323RS_CSA_CONTROL_CSA_GAIN_10VPV

/** @brief DRV gain to map to FOC_CSA_GAIN_2 */
#define  DRV_CSA_GAIN_MAP_2                     DRV8323RS_CSA_CONTROL_CSA_GAIN_20VPV

 /** @brief DRV gain to map to FOC_CSA_GAIN_3 */
 #define  DRV_CSA_GAIN_MAP_3                     DRV8323RS_CSA_CONTROL_CSA_GAIN_40VPV

/** @brief CSA Gain selected */
#define USER_DEFAULT_CSA_GAIN                   FOC_CSA_GAIN_0

/** @brief Enable gain change */
#define USER_DEFAULT_ENABLE_GAIN_CHANGE

/** @brief Define api for csa gain */
#define DRV_setCsaGain                          DRV8323RS_setCsaGain

/** @brief Define drv handle */
#define DRV_HANDLE                              DRV8323RS_Instance

/** @brief Define drv csa gain enumerations */
#define DRV_CSA_GAIN                            DRV8323RS_CSA_CONTROL_CSA_GAIN

/** @brief Enable fault input check */
#define USER_DEFAULT_ENABLE_FAULT_INPUT_CHECK

/** @brief Define api for fault input status, should return 1 in drv fault */
#define FAULT_INPUT_STATUS                      DRV8323RS_isFaultOccurred

/** @brief Define api for clear drv fault */
#define FAULT_INPUT_CLEAR                       DRV8323RS_clearFaults

/** @brief Initial value of stator resistance */
#define USER_DEFAULT_FOC_RS                     MOTOR_PARA_RS

/** @brief Initial value of stator inductance */
#define USER_DEFAULT_FOC_LS                     MOTOR_PARA_LS

/** @brief Initial value of motor back emf constant */
#define USER_DEFAULT_FOC_KE                     MOTOR_PARA_KE

/** @brief IQ value of motor max frequency */
#define USER_DEFAULT_FOC_MAX_FREQ               MOTOR_PARA_MAX_FREQ

/** @brief Initial value of pwm frequency */
#define USER_DEFAULT_FOC_PWMFREQ                FOC_PWM_FREQ_10000

/** @brief Initial value of deadband in ns */
#define USER_DEFAULT_FOC_DEADBAND               (400)

/** @brief Initial motor startup method */
#define USER_DEFAULT_FOC_STARTUP_METHOD         FOC_STARTUP_ALIGN

/** @brief Initial motor slow first cycle frequency in Hz */
#define USER_DEFAULT_FOC_SLOW_FIRST_CYC_FREQ    (5)

/** @brief Initial value of rampup current */
#define USER_DEFAULT_FOC_RAMPUP_CURRENT         (2)

/** @brief Initial value of rampup rate in Hz/s */
#define USER_DEFAULT_FOC_RAMPUP_RATE            (40)

/** @brief Initial value of ramp target in Hz */
#define USER_DEFAULT_FOC_RAMPUP_TARGET          (80)

/** @brief Initial value of align time in seconds */
#define USER_DEFAULT_FOC_ALIGN_TIME             (0.5)

/** @brief Initial value of align current in Ampere */
#define USER_DEFAULT_FOC_ALIGN_CURRENT          (2)

/** @brief Initial value of ipd current threshold between 1-127 */
#define USER_DEFAULT_IPD_THRESHOLD_COUNT        FOC_IPD_THRES_COUNT_16

/** @brief Selection of IPD frequency */
#define USER_DEFAULT_FOC_IPD_FREQ               FOC_IPD_FREQ_4000

/** @brief Initial value of speed reference ramp rate in Hz/s */
#define USER_DEFAULT_FOC_SPEED_REF_RAMP_RATE    (80)

/** @brief Initial value of KP for PI controller for speed */
#define USER_DEFAULT_FOC_PISPD_KP               (0.1)

/** @brief Initial value of KI for PI controller for speed */
#define USER_DEFAULT_FOC_PISPD_KI               (0.0009)

/** @brief Initial value of divider to derive PI speed execution rate from 
 * foc frequency */
#define USER_DEFAULT_FOC_PISPD_DIV              FOC_SPD_DIV_10

/** @brief Initial value of KP for PI controller for Iq */
#define USER_DEFAULT_FOC_PIIQ_KP                (0.053)

/** @brief Initial value of KI for PI controller for Iq */
#define USER_DEFAULT_FOC_PIIQ_KI                (0.012)

/** @brief Initial value of KP for PI controller for Id */
#define USER_DEFAULT_FOC_PIID_KP                (0.053)

/** @brief Initial value of KI for PI controller for Id */
#define USER_DEFAULT_FOC_PIID_KI                (0.012)

/** @brief Initial selection for the outer loop */
#define USER_DEFAULT_FOC_OUTER_LOOP             FOC_OUTER_LOOP_SPEED

/** @brief Initial state for the direction selection */
#define USER_DEFAULT_FOC_DIRECTION_REVERSAL     FOC_DIRECTION_CW

/** @brief Initial value of pwm cycles spend in adc sampling */
#define USER_DEFAULT_FOC_PWMADCSAMPLE           (40)

/** @brief Delay for the ADC value to settle all low side turn on */
#define USER_DEFAULT_FOC_ADC_SETTLE_DELAY       (400)

/** @brief Initial value of over voltage threshold in voltage */
#define USER_DEFAULT_FOC_OVER_VOLT              (40)

/** @brief Initial value of under voltage threshold in voltage */
#define USER_DEFAULT_FOC_UNDER_VOLT             (7)

/** @brief Initial value of over current threshold in Ampere */
#define USER_DEFAULT_FOC_OVER_CURRENT           (12.0)

#ifdef __cplusplus
}
#endif
#endif /* PARAM_DRV8323RS_H */
