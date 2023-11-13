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
 *  @file       transit.h
 *  @brief      Sensorless FOC Motor Control Library TRANSIT Module
 *
 *
 *  @anchor transit_h
 *  # Overview
 *
 *  APIs for TRANSIT module
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MATH_TRANSIT FOC_MATH/TRANSIT
 * @{
 */
#ifndef TRANSIT_H
#define TRANSIT_H

#include <stdint.h>
#include "angle_math.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/*! @enum TRANSIT_STATE */
typedef enum{
    /*! Compute angle difference */
    TRANSIT_STATE_COMPUTE_DIFF = 0,
    /*! Transition in process */
    TRANSIT_STATE_IN_TRANSIT,
    /*! Transition complete */
    TRANSIT_STATE_COMPLETE,
} TRANSIT_STATE;

/*! @brief Define transit structure */
typedef struct {
    /*! State of transit */
    TRANSIT_STATE state;
    /*! Angle difference */
    _iq diff;
    /*! Merge factor */
    _iq factor;
    /*! Output angle */
    uint32_t out;
} TRANSIT_Instance;

/*! @brief Merge factor initial value */
#define TRANSIT_FACTOR_DELTA_INIT_VAL_IQ10  _IQ10(1)

/*! @brief Delta change for Merge factor */
#define TRANSIT_FACTOR_DELTA_IQ10           _IQ10(0.001)

/*! @brief Left shift to convert to iq 10 */
#define TRANSIT_ANGLE_SCALE_FACTOR_TO_IQ10 (12)

/**
 * @brief     Reset the transit variables
 * @param[in] handle  A pointer to transit instance
 */
__STATIC_INLINE void TRANSIT_reset(TRANSIT_Instance *handle)
{
    handle->state = TRANSIT_STATE_COMPUTE_DIFF;
    handle->out = 0;
    handle->diff = 0;
}

/**
 * @brief     Run transit
 * @param[in] handle  A pointer to transit instance
 * @param[in] finalVal  Final transition value
 * @param[in] refVal  Current value in transition
 * @return    Returns transit rotor angle
 */
__STATIC_INLINE uint32_t TRANSIT_run(TRANSIT_Instance *handle,
                                             uint32_t finalVal, uint32_t refVal)
{
    uint32_t out = 0;
    int32_t tmp;
    switch (handle->state)
    {
        case TRANSIT_STATE_COMPUTE_DIFF:
            tmp = ((int32_t)finalVal - (int32_t)refVal)
                                           >>TRANSIT_ANGLE_SCALE_FACTOR_TO_IQ10;
            handle->diff = tmp;
            handle->factor = TRANSIT_FACTOR_DELTA_INIT_VAL_IQ10;
            handle->state = TRANSIT_STATE_IN_TRANSIT;
            out = refVal;
            break;
        case TRANSIT_STATE_IN_TRANSIT:
            out = finalVal - (uint32_t)(_IQ10mpy(handle->diff, 
                           handle->factor)<<TRANSIT_ANGLE_SCALE_FACTOR_TO_IQ10);
            handle->factor -= TRANSIT_FACTOR_DELTA_IQ10;
            if(handle->factor < 0)
            {
                handle->state = TRANSIT_STATE_COMPLETE;
            }
            break;
        case TRANSIT_STATE_COMPLETE:
            out = finalVal;
            break;
       default:
            /* This is expected to be empty */
            break;
    }
    handle->out = out;
    return out;
}

#ifdef __cplusplus
}
#endif
#endif /* TRANSIT_H */
/** @}*/
