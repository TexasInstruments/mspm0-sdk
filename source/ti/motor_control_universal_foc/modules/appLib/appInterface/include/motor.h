/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @file       motor.h
 *  @brief      Motor Module
 *
 * 
 *  @anchor motor
 *  # Overview
 *
 *  defines motor structure
 *
 *  <hr>
 ******************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include "iqNum.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The below scaling factors should not be changed by user */

/** @brief Resistance IQ scaling */
#define RS_IQ_SCALING   23

/** @brief Inductance IQ scaling */
#define LS_IQ_SCALING   23

/** @brief Backemf constant Ke IQ scaling */
#define KE_IQ_SCALING   23

/*! @brief Motor structure */
typedef struct
{
    int16_t
        /*! Pole Pairs */
        polePairs;

    int32_t
        /*! Phase Resistance */
        rs,
        /*! D-Axis Inductance / Sampling Time */
        LdDt,
        /*! Q-Axis Inductance / Sampling Time */ 
        LqDt,
        /*! Common mode Inductance / Sampling Time */
        L0Dt,
        /*! Differential mode Inductance / Sampling Time */
        L1Dt,
        /*! BEMF Constant */
        Ke;

    IQ_NUM_T
        /*! Inverse of BEMF Constant */
        InvKe;

    _Bool
        /*! Saliency: Defines SPMSM or IPMSM */
        saliency;
}MOTOR_T;

#ifdef __cplusplus
}
#endif
#endif /* MOTOR_H */
