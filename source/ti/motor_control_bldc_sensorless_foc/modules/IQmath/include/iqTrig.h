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
 *  @file       iqTrig.h
 *  @brief      IQ Trignometry Module
 *
 * 
 *  @anchor iqTrig_h
 *  # Overview
 *
 *  IQ Trignometry APIs
 *
 *  <hr>
 ******************************************************************************/

#ifndef IQTRIG_H_
#define IQTRIG_H_

#include "stdint.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Number of lookup points */
#define LOOKUP_POINTS               16

/*! @brief Delta lookup */
#define DELX_LOOKUP                 _IQ(0.25)/(LOOKUP_POINTS - 1)
/*! @brief Inverted Delta lookup */
#define INV_DELX_LOOKUP              (int32_t)(((LOOKUP_POINTS - 1) / 0.25))

/**
 * @brief sine of angle
 * @param[in] orig_angle  Angle input in pu
 * @return  Sine of the angle
 */
int32_t sine(int32_t orig_angle);

/**
 * @brief cosine of angle
 * @param[in] orig_angle  Angle input in pu
 * @return  Cosine of the angle
 */
int32_t cosine(int32_t orig_angle);

/**
 * @brief Finds tan inverse angle
 * @param[in] cosVal  Cosine value of angle
 * @param[in] sinVal  Sine value of angle
 * @param[in] mag  Magnitude
 * @return  Tan inverse angle
 */
int32_t arcTan2(int32_t cosVal, int32_t sinVal, int32_t mag);

#ifdef __cplusplus
}
#endif
#endif /* IQTRIG_H_ */
