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
 *  @file       motor_params.h
 *  @brief      Sensorless FOC Motor Control Library Motor Module
 *
 *
 *  @anchor motor_params_h
 *  # Overview
 *
 *  Motor related APIs
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup MOTOR_MODULE Motor_Module
 * @{
 */
#ifndef MOTOR_PARAMS_H
#define MOTOR_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#if (LVSERVOMTR)
/* Define the electrical motor parameters (Teknic 2310 Servomotor) */
/* @brief Stator resistance (ohm) */
#define MOTOR_PARA_RS               0.38
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               0.000169
/* @brief Number of poles */
#define MOTOR_PARA_POLES            8
/* @brief Base RPM */
#define MOTOR_PARA_BASE_RPM         6000

#elif (LVBLDCMTR)
/* Define the electrical motor parameters (Anaheim motor) */
/* @brief Stator resistance (ohm) */
#define MOTOR_PARA_RS               0.8 
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               0.001
/* @brief Number of poles */
#define MOTOR_PARA_POLES            8
/* @brief Base RPM */
#define MOTOR_PARA_BASE_RPM         6000

#elif (USER_MOTOR)
/* Define the electrical motor parameters (User motor) */
/* @brief Stator resistance (ohm) */
#define MOTOR_PARA_RS               1.7
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               0.004
/* @brief Number of poles */
#define MOTOR_PARA_POLES            8
/* @brief Base RPM */
#define MOTOR_PARA_BASE_RPM         5000

#endif

/** @brief Pole pairs of the motor */
#define MOTOR_PARA_NUM_POLE_PAIRS                  (MOTOR_PARA_POLES/2)

/**
 * @brief     Calculates the base frequency from base rpm and number of poles
 * @param[in] baseRpm   Base rpm of motor
 * @param[in] poles     Number of poles of motor 
 * @return    Returns the calculated base frequency
 */
float MOTOR_getBaseFreq(float baseRpm, float poles);

#ifdef __cplusplus
}
#endif
#endif /* MOTOR_PARAMS_H */
/** @}*/
