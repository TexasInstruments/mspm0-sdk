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
 *  @file       ipd.h
 *  @brief      ipd Module
 *
 * 
 *  @anchor ipd_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef IPD_H
#define IPD_H

#include <stdint.h>

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "focHALInterface.h"
#include "appDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! @brief Ipd Flags Working Structure */
typedef union
{
    /*! @brief Ipd Flags */
    struct ipdFlags
    {
        _Bool
            /*! Angle Adjust Direction */        
            angleAdjustinForward: 1,
            /*! Enables IPD */            
            enable: 1,
            /*! Status of IPD */            
            status: 1,
            /*! Disables transition out of IPD */            
            forceEnable: 1,
            /*! Enables Timeout fault */            
            clockFaultEnable: 1,
            /*! Enables too low pulse-to-pulse time fault */            
            currentDecayFaultEnable: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}IPD_FLAGS_T;

/*! @brief Ipd states enum */
typedef enum
{
    /*! Initialize IPD */    
    IPD_INIT = 0,
    /*! Vector Configuration */
    VOLTAGE_VECTOR_CONFIG = 1,
    /*! IPD is running */
    IPD_RUN = 2,
    /*! configure the voltage vector */
    IPD_VECTOR_CONFIG = 3,
    /*! Wait for pulse time to complete */
    IPD_WAIT_PULSE = 5,
    /*! Waiting for T1 and T2 is complete */
    IPD_WAIT_COMPLETE = 6,
    /*! Set next vector to be applied */
    IPD_SET_NEXT_VECTOR = 7,
    /*! Calculate sector on rise times */
    IPD_CALC_SECTOR_RISE = 8,
    /*! Calculate rotor position */
    IPD_CALC_ROTOR_POSITION = 9,
    /*! Calculate angle to be applied */
    IPD_CALC_ANGLE = 10,
    /*! IPD is complete */
    IPD_COMPLETE = 11,
    /*! Fault in IPD */
    IPD_FAULT = 12,
}IPD_STATE_TYPES;

/*! @brief Ipd wait states enum */
typedef enum
{
    /*! Get T1 state */    
    WAIT_GET_T1 = 0,
    /*! Wait complete */    
    WAIT_COMPLETE = 1,
}IPD_WAIT_TYPES;

/*! @brief Ipd wait states enum */
typedef enum
{
    /*! No Fault */
    IPD_NO_FAULT = 0,
    /*! Capture Clock reached overflow limit */    
    IPD_FAULT_CLOCK_TIMEOUT = 1,
    /*! Insufficient IPD frequency set */    
    IPD_FAULT_DECAY_TIME = 2,
}IPD_FAULT_TYPES;

/*! @brief Ipd structure */
typedef struct
{
    /* IPD Inputs */
    const int32_t
        /*! Target Velocity */
        *pTargetVelocity,
        /*! Inverse DC Bus Voltage */
        *pInvVdc;

    /* IPD Parameters for processing */
    IPD_STATE_TYPES
        /*! IPD State */
        state;

    IPD_WAIT_TYPES
        /*! IPD Wait states */
        waitState;

    VOLTAGE_VECTOR
        /*! Voltage Vector */
        vector;

    uint16_t
        /*! Counter to keep track of electrical cycles */
        cycleCount,
        /*! Maximum number of electrical cycles */
        cycleCountMax;

    uint16_t
        /*! Counter to keep track of vectors */
        vectorCount,
        /*! Counter for No reverse rotation */
        noRotationVectorCount,
        /*! Adjacent Vector for forward rotation */
        adjacentVectorForward
                [VOLTAGE_VECTOR_MAX],
        /*! Adjacent Vector for reverse rotation */
        adjacentVectorReverse
                [VOLTAGE_VECTOR_MAX];

    IPD_FAULT_TYPES
        /*! IPD Fault Status */
        faultStatus;

    int32_t
        /*! Rise time of current */
        timeRise[VOLTAGE_VECTOR_MAX], 
        /*! Fall time of current */
        timeFall[VOLTAGE_VECTOR_MAX], 
        /*! Minimum Rise Time */
        minRiseTime,
        /*! Rotor Position */
        rotorPosition,
        /*! Angle to be set for open loop */
        angle,
        /*! Advance angle for IPD */
        advanceAngle,
        /*! Vector Angles for initializations */
        vectorAngle
                [VOLTAGE_VECTOR_MAX],
        /*! Count between two pulses */
        pulseToPulseCount,
        /*! Maximum Count between two pulses */
        pulseToPulseCountMax;

    uint8_t 
        /*! Upper current limit */
        currentLimitHi;

    uint8_t
        /*! Timer overflow counter */
        overflowCnt,
        /*! Maximum Timer overflow allowed */
        overflowMax;

    IPD_FLAGS_T
        /*! Flags */
        flags;

    CURRENT_OFFSET_T
        /*! Current offset */
        *pOffset;

}IPD_T;

/**
 * @brief     Ipd  Init
 * @param[in] *pIpd Pointer
 */
void ipdInit(IPD_T *pIpd);

/**
 * @brief     Ipd  run
 * @param[in] *pIpd Pointer
 */
void ipdRun(IPD_T *pIpd);

/**
 * @brief     Ipd Status check
 * @param[in] *pIpd  Pointer
 * @return    Returns the status
 */
inline _Bool getIpdStatus(IPD_T *pIpd)
{
    return pIpd->flags.b.status;
}

/**
 * @brief     Ipd complete Status check
 * @param[in] *pIpd  Pointer
 * @return    Returns the status
 */
inline _Bool getIpdComplete(IPD_T *pIpd)
{
    return (pIpd->state == IPD_COMPLETE);
}

/**
 * @brief     Ipd Fault Status
 * @param[in] *pIpd  Pointer
 * @return    Returns the status
 */
inline IPD_FAULT_TYPES getIpdFaultStatus(IPD_T *pIpd)
{
    return pIpd->faultStatus;
}

/**
 * @brief     Ipd  current threshold action
 * @param[in] *pIpd Pointer
 */
void IPDCurrThreshAction(IPD_T *pIpd);

/**
 * @brief     Ipd timer overflow action
 * @param[in] *pIpd Pointer
 */
void IPDOverFlowAction(IPD_T *pIpd);

#ifdef __cplusplus
}
#endif
#endif /* IPD_H */

