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
 *  @file       cirlimit.h
 *  @brief      Sensorless FOC Motor Control Library Circle limitation Module
 *
 * 
 *  @anchor cirlimit_h
 *  # Overview
 *
 *  APIs for circle limitation
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_CIRLIMIT FOC_MATH/CIRLIMIT
 * @{
 */
#ifndef CIRLIMIT_H
#define CIRLIMIT_H

#include <ti/devices/msp/msp.h>

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Define circle limit structure */
typedef struct {
    /*!  Direct axis voltage input in pu */
    _iq inVd;
    /*!  Quadrature axis voltage input in pu */
    _iq inVq;
    /*!  Direct axis voltage input in pu */
    _iq outVd;
    /*!  Quadrature axis voltage output in pu */
    _iq outVq;
    /*!  Magnitude of the input voltage */
    _iq vmag;
    /*!  Magnitude to limit the output */
    _iq vmax;
} CIRLMT_Instance;

/*! @brief Default values for the circle limit instance */
#define CIRLMT_DEFAULTS     { 0,0,0,0,0,_IQ(0.866) }

/**
 * @brief     Limits the Vq and Vd in pu within the circle of set radius
 * @param[in] handle   A handle to the Circle limit instance
 */
__STATIC_INLINE void CIRLIMIT_run(CIRLMT_Instance *handle)
{
    handle->vmag = _IQmag(handle->inVq, handle->inVd);
    if( handle->vmag > handle->vmax )
    {
        handle->outVd = _IQmpy(handle->vmax,
                                            _IQdiv(handle->inVd, handle->vmag));
        handle->outVq = _IQmpy(handle->vmax,
                                            _IQdiv(handle->inVq, handle->vmag));
    }
    else
    {
        handle->outVd = handle->inVd;
        handle->outVq = handle->inVq;
    }
}

#ifdef __cplusplus
}
#endif
#endif /* CIRLIMIT_H */
/** @}*/
