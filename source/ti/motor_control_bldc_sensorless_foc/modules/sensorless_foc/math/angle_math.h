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
 * @brief 0.5 in IQ
 */
#define ANGLE_MATH_0P5_IQ     _IQ(0.5)

/*!
 * @brief 1.0 in IQ
 */
#define  ANGLE_MATH_1P0_IQ    _IQ(1.0)

/**
 * @brief     Wraps angle within _IQ(-0.5) and _IQ(0.5)
 * @param[in] angle   Angle input to wrap within _IQ(-0.5) and _IQ(0.5)
 */
__STATIC_INLINE void ANGLE_WRAP(_iq *angle)
{
  if(*angle > ANGLE_MATH_0P5_IQ)
  {
    *angle -= ANGLE_MATH_1P0_IQ;
  }
  else if(*angle < -ANGLE_MATH_0P5_IQ)
  {
    *angle += ANGLE_MATH_1P0_IQ;
  }
  else
  {
    /* This is expected to be empty */
  }
}

#ifdef __cplusplus
}
#endif
#endif /* ANGLE_MATH_H */
/** @}*/
