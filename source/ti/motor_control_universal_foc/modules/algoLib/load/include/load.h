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
 *  @file       load.h
 *  @brief      load Module
 *
 * 
 *  @anchor load_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_LOAD_INCLUDE_LOAD_H_
#define ALGOLIB_LOAD_INCLUDE_LOAD_H_

#include "brake.h"
#include "stdint.h"
#include "isd.h"
#include "softStop.h"
#include "measure.h"
#include "triState.h"
#include "ipd.h"

#include "rotorAlign.h"
#include "slowFirstCycle.h"

/*! @brief Load Flags Working Structure */
typedef union
{
    /*! @brief Load Flags Struct */
    struct loadFlags
    {
        _Bool
            /*! Load is ready. Motor can be started */
            loadStartReadyStatus: 1,
            /*! Load is ready. Motor can be stopped */
            loadStopReadyStatus: 1,
            /*! Motor stop by align */
            loadStopAlignEnable: 1;
    }
    /*! Bitwise Access */
    b;
    /*! Block Access */
    uint16_t w;

}LOAD_FLAGS_T;

/*! @brief Load state types enum */
typedef enum
{
    /*! Initialize load */    
    LOAD_INIT = 0,
    /*! Initial Speed Detect */
    LOAD_ISD = 1,
    /*! Open all switches  */
    LOAD_TRISTATE = 2,
    /*! Brake the motor */
    LOAD_BRAKE = 3,
    /*! Initial Position */
    LOAD_IPD = 4,
    /*! Start the load */
    LOAD_START = 5,
    /*! Running of load */
    LOAD_RUN = 6,
    /*! Stop Initialization */
    LOAD_STOP_INIT = 7,
    /*! Soft stop the load */
    LOAD_SOFT_STOP = 8,
    /*! Load stopped */
    LOAD_STOP = 9,
    /*! Load Fault */
    LOAD_FAULT = 10,
    /*! Align Stop */
    LOAD_ALIGN_STOP = 11,
}LOAD_STATE_TYPES;

/*!  @brief Motor start types enum */
typedef enum
{
    /*! Start the motor with Align */
    START_ALIGN = 0,
    /*! Start the motor with IPD */
    START_IPD = 1,
    /*! Start the motor with ISD */
    START_ISD = 2,
    /*! Start the motor with slow first cycle */
    START_SLOW_FIRST_CYCLE = 3,
}MOTOR_START_TYPES;

/*!  @brief Load fault types enum */
typedef enum
{
    /*! No fault on load */
    LOAD_NO_FAULT = 0,
    /*! ISD Fault */
    LOAD_ISD_FAULT = 1,
    /*! IPD Fault */
    LOAD_IPD_FAULT = 2,
}LOAD_FAULT_TYPES;

/*!  @brief Load Working Structure */

typedef struct
{
    /*! Load Inputs */
    const int32_t
    /*! PWM Period to Run Motor */
        *pPWMPeriod,
        /*! Propagation Delay for PWM */
        *pPropagationDelay;

    const SLOW_FIRST_CYCLE_FLAGS_T
        /*! Slow first Cycle Enable */
        *pSlowFirstCycleFlags;

    const HAL_MEASURE_VOLTAGE_T
        /*! Measured voltages */
        *pVoltage;

    const ADC_SAMPLING_RATIO_TYPES
        /*! ADC Sampling Ratio */
        *pADCSamplingRatio;

    ROTOR_ALIGN_FLAGS_T
        /*! Rotor Align Flags */
        *pRotorAlignFlags;

    /* Load parameters for processing */
    LOAD_STATE_TYPES
        /*! Load State */
        state;

    HAL_MEASURE_MOTOR_INPUTS_T
        /*! Motor Inputs */
        *pMotorInputs;

    MOTOR_START_TYPES
        /*! Motor Start Algorithm */
        motorStartAlgorithm;

    LOAD_FAULT_TYPES
        /*! Load fault status */
        faultStatus;

    ISD_T
        /*! Initial Speed Detection Structure */
        isd;

    TRISTATE_T
        /*! Tri-State Structure */
        triState;

    BRAKE_T
        /*! Brake Structure */
        brake;

    IPD_T
        /*! Initial Position Detection Structure */
        ipd; 

    SOFT_STOP_T
        /*! Soft Stop Structure */
        softStop;

    LOAD_FLAGS_T
        /*! Flags */
        flags;

}LOAD_T;

/**
 * @brief     Load Init
 * @param[in] *pLoad Input Pointer
 */
void loadInit(LOAD_T *pLoad);
/**
 * @brief    Load Run
 * @param[in] *pLoad Input Pointer
 */
void loadRun(LOAD_T *pLoad);
/**
 * @brief     Load Low Priority Run
 * @param[in] *pLoad Input Pointer
 */
void loadLowPriorityRun(LOAD_T *pLoad);

/**
 * @brief    LoadStartReadyStatus Status check
 * @param[in] *pLoad  Pointer
 * @return    Returns the status
 */
inline _Bool getLoadStartReadyStatus(LOAD_T *pLoad)
{
    return pLoad->flags.b.loadStartReadyStatus;
}

/**
 * @brief     Load Stop Status check
 * @param[in] *pLoad  Pointer
 * @return    Returns the status
 */
inline _Bool getLoadStopReadyStatus(LOAD_T *pLoad)
{
    return pLoad->flags.b.loadStopReadyStatus;
}

/**
 * @brief     Load Stall Status check
 * @param[in] *pLoad  Pointer
 * @return    Returns the status
 */
inline _Bool getLoadStallStatus(LOAD_T *pLoad)
{
    return (pLoad->faultStatus != LOAD_NO_FAULT);
}

/**
 * @brief    Motor Run  Status check
 * @param[in] *pLoad  Pointer
 * @return    Returns the status
 */
inline _Bool runMotorDuringStop(LOAD_T *pLoad)
{
    _Bool status;
    status = ((pLoad->state == LOAD_SOFT_STOP) ||
                (pLoad->state == LOAD_STOP_INIT) ||
                (pLoad->state == LOAD_ALIGN_STOP));
    return status;
}
#endif /* ALGOLIB_LOAD_INCLUDE_LOAD_H_ */
