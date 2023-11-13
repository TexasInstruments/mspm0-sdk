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
#define MOTOR_PARA_RS               (0.38)
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               (0.000169)
/* @brief Number of poles */
#define MOTOR_PARA_POLES            (8)
/* @brief Back Emf constant in V/Hz */
#define MOTOR_PARA_KE               (0.069)
/* @brief Maximum Frequency */
#define MOTOR_PARA_MAX_FREQ        (400)

#elif (LVBLDCMTR)
/* Define the electrical motor parameters (Anaheim motor) */
/* @brief Stator resistance (ohm) */
#define MOTOR_PARA_RS               (0.8) 
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               (0.001)
/* @brief Number of poles */
#define MOTOR_PARA_POLES            (8)
/* @brief Back Emf constant in V/Hz */
#define MOTOR_PARA_KE               (0.06)
/* @brief Maximum Frequency */
#define MOTOR_PARA_MAX_FREQ         (400)

#elif (HVPMSMMTR)
/* Define the electrical motor parameters (Estun EMJ-04APB22 motor) */
/* @brief Stator resistance (ohm) */
#define MOTOR_PARA_RS               (2.35) 
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               (0.00065)
/* @brief Number of poles */
#define MOTOR_PARA_POLES            (8)
/* @brief Back Emf constant in V/Hz */
#define MOTOR_PARA_KE               (1.0)
/* @brief Maximum Frequency */
#define MOTOR_PARA_MAX_FREQ         (200)

#elif (USER_MOTOR)
/* Define the electrical motor parameters (User motor) */
/* @brief Stator resistance (ohm) */
#define MOTOR_PARA_RS               (8)
/* @brief Stator inductance (H) */
#define MOTOR_PARA_LS               (0.0032)
/* @brief Number of poles */
#define MOTOR_PARA_POLES            (6)
/* @brief Back Emf constant in V/Hz */
#define MOTOR_PARA_KE               (0.1)
/* @brief Maximum Frequency */
#define MOTOR_PARA_MAX_FREQ         (125)

#endif

#ifdef __cplusplus
}
#endif
#endif /* MOTOR_PARAMS_H */
/** @}*/
