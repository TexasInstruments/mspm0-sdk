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
 *  @file       angle_math.h
 *  @brief      Sensorless FOC Motor Control Library Angle Math Module
 *
 * 
 *  @anchor angle_math_h
 *  # Overview
 *
 *  APIs for angle math operations
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_ANGLE_MATH FOC_MATH/ANGLE_MATH
 * @{
 */
#ifndef ANGLE_MATH_H
#define ANGLE_MATH_H

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#include <ti/devices/msp/msp.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief 0 in angle perunit IQ 32
 */
#define ANGLE_MATH_0_PU                 (0)

/*!
 * @brief PI/3 in angle perunit IQ 32
 */
#define ANGLE_MATH_PIp3_PU              (715827882)

/*!
 * @brief PI/2 in angle perunit IQ 32
 */
#define ANGLE_MATH_PIp2_PU              (1073741824)

/*!
 * @brief 2PI/3 in angle perunit IQ 32
 */
#define ANGLE_MATH_2PIp3_PU             (1431655765)

/*!
 * @brief PI in angle perunit IQ 32
 */
#define ANGLE_MATH_PI_PU                (2147483648)

/*!
 * @brief 4PI/3 in angle perunit IQ 32
 */
#define ANGLE_MATH_4PIp3_PU             (2863311530)

/*!
 * @brief 5PI/3 in angle perunit IQ 32
 */
#define ANGLE_MATH_5PIp3_PU             (3579139413)


/** @brief Macro to convert IQ32 to IQ 24 */
#define _ANGLE_toIQ24(X)                    (X >> 8)

/** @brief Macro to convert IQ32 to IQ 21 */
#define _ANGLE_toIQ21(X)                    (X >> 11)

/** @brief Macro to convert IQ17 to IQ 32 */
#define _ANGLE_IQ17toIQ32(X)                (X << 15)

/** @brief Macro to convert IQ to IQ 32 */
#define _ANGLE_IQtoIQ32(X)                  (X << (32 - GLOBAL_IQ))

/** @brief Macro to convert IQ24 to IQ 32 */
#define _ANGLE_IQ21toIQ32(X)                (X << 11)

/** @brief Macro to convert IQ24 to IQ 32 */
#define _ANGLE_IQ24toIQ32(X)                (X << 8)

/**
 * @brief     Gets the sine of the angle
 * @param[in] phase  Angle for sine in 31 fixed point
 * @return    Returns the sine of the angle in fixed point 24
 */
int32_t ANGLE_getSine( uint32_t phase );

#ifdef __cplusplus
}
#endif
#endif /* ANGLE_MATH_H */
/** @}*/
