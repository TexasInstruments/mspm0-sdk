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
 *  @file       ipd.h
 *  @brief      Sensorless FOC Motor Control Library IPD Module
 * Module
 *
 *
 *  @anchor ipd_h
 *  # Overview
 *
 *  APIs for IPD
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_IPD FOC_IPD/IPD
 * @{
 */
#ifndef IPD_H
#define IPD_H

#include <stdint.h>
#include "angle_math.h"
#include "foc_types.h"

/* Include the IQmath Library */
#include "ti/iqmath/include/IQmathLib.h"

/* Extended IQmath Library for rts */
#include "iqmath_rts.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief * bit mask for comparator input */
#define IPD_MAX_COMP_VAL    (255)

/*! @enum IPD_STATE */
typedef enum
{
    /*! Init state */
    IPD_INIT = 0,
    /*! Configure state */
    IPD_CONFIG,
    /*! Waiting for pulse */
    IPD_WAIT,
    /*! State to wait for period to complete */
    IPD_PERIOD_WAIT,
    /*! Angle compute state */
    IPD_COMPUTE,
    /*! IPD completed state */
    IPD_CMPLT
} IPD_STATE;

/*! @enum IPD_VECTOR */
typedef enum
{
    /*! IPD vector AC */
    IPD_VECTOR_AC = 0,
    /*! IPD vector CA */
    IPD_VECTOR_CA,
    /*! IPD vector BC */
    IPD_VECTOR_BC,
    /*! IPD vector BC */
    IPD_VECTOR_CB,
    /*! IPD vector BA */
    IPD_VECTOR_BA,
    /*! IPD vector AB */
    IPD_VECTOR_AB,
    /*! Max number of IPD vectors */
    IPD_VECTOR_MAX
}IPD_VECTOR;

/*! @brief Define ipd structure */
typedef struct {
    /*! Rotor position output angle */
    uint32_t outAngle;
    /*! IPD state */
    IPD_STATE ipdState;
    /*! IPD vector */
    IPD_VECTOR ipdVector;
    /*! Wait counter for pulse */
    uint32_t waitCnt;
    /*! Period wait counter */
    uint32_t pulsePeriodCnt;
    /*! Period wait count limit */
    uint32_t pulsePeriodLimit;
    /*! IPD current threshold */
    uint8_t ipdThr;
    /*! Array for storing IPD time */
    uint32_t ipdT[IPD_VECTOR_MAX];
    /*! Minimum time vector */
    IPD_VECTOR minTimeVector;
    /*! IPD min time */
    uint32_t minTime;
    /*! Time difference between minTimeVector and its opposite */
    uint32_t deltaT;
} IPD_Instance;

/*! @brief Define ipd parameter structure */
typedef struct {
    /*! IPD threshold */
    uint32_t ipdThr;
    /*! IPD frequency selection */
    uint32_t ipdFreq;
    /*! Sampling frequency in Hz */
    uint32_t sampleFreq;
} IPD_PARA;

/* Extern variables */
extern uint32_t ipdAngle[IPD_VECTOR_MAX];

/**
 * @brief     Update the ipd parameters
 * @param[in] handle  A pointer to ipd instance
 * @param[in] para  A pointer to parameter array
 */
void IPD_UpdateParams(IPD_Instance *handle, IPD_PARA *para);

/**
 * @brief     Reset the ipd variables
 * @param[in] handle  A pointer to ipd instance
 */
void IPD_reset(IPD_Instance *handle);

/**
 * @brief     Run IPD
 * @param[in] handle A pointer to ipd instance
 * @param[in] hal    A pointer to foc hal instance
 * @param[in] offsetA 8 bit value of the A phase offset
 * @param[in] offsetB 8 bit value of the B phase offset
 * @param[in] offsetC 8 bit value of the C phase offset
 */
void IPD_run(IPD_Instance *handle, FOC_HAL *hal, 
                        uint8_t offsetA , uint8_t offsetB , uint8_t offsetC);

/**
 * @brief     IPD ISR
 * @param[in] handle A pointer to ipd instance
 * @param[in] hal    A pointer to foc hal instance
 */
void IPD_ISR(IPD_Instance *handle, FOC_HAL *hal);

#ifdef __cplusplus
}
#endif
#endif /* SSPID_H */
/** @}*/
