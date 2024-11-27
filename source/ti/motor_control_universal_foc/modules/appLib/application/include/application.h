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
 *  @file       application.h
 *  @brief      application Module
 *
 * 
 *  @anchor application_h
 *  # Overview
 *
 *  Implements FOC state machine
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPLICATION_H_
#define APPLICATION_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "appInterface.h"
#include "measure.h"
#include "foc.h"
#include "motor.h"
#include "sourceLimits.h"
#include "load.h"
#include "faults.h"
#include "angleTrackingPLL.h"

 /*! @brief App State type */
typedef enum
{
    /*! Initialize Application */
    APP_INIT = 0,
    /*! Idle State - waits for run command */
    APP_IDLE = 1,
    /*! Calibrate sensor offsets */
    APP_OFFSET_CALIBRATION = 2,
    /*! Start the application */
    APP_START = 3,
    /*! Run the motor */
    APP_RUN = 4,
    /*! Stop the motor */
    APP_STOP = 5,
    /*! Brake the motor - enabled by brake pin */
    APP_BRAKE = 6,
    /*! Application Fault */
    APP_FAULT = 7,
}APP_STATE_TYPES;

/*! @brief Application Flags Structure */
typedef union
{
    /*! @brief Application Flags */
    struct appFlags
    {
        _Bool
            /*! Run Command */
            runCmd: 1,
            /*! Brake Command */
            brakeCmd: 1,
            /*! Motor to run in forward direction */
            reverseDirectionCmd: 1,
            /*! Previous direction command */
            prevDirectionCmd: 1,
            /*! Action taken */
            directionActionTaken: 1,
            /*! reserved */
            reserved: 1,
            /*! Motor Running */
            motorRun: 1;

    }
    /*! Bitwise Access*/
    b;
    /*! Block Access */
    uint16_t w;

}APP_FLAGS_T;

/*! @brief Application Working Structure */
typedef struct
{

    APP_STATE_TYPES
        /*! Application State */
        state;

    APP_FAULT_TYPES
        /*! Fault Status */
        faultStatus;

    APP_FLAGS_T
        /*! Flags */
        flags;

    APP_INTERFACE_T
        /*! Application Interface */
        *pAppInterface;

    int32_t
        /*! Target Speed considering system limits */
        targetVelocity,
        /*! Minimum Speed */
        minimumSpeed,
        /*! Maximum Speed */
        maximumSpeed;

    SOURCE_LIMITS_T
        /*! Interface for limits */
        sourceLimits;

    HAL_MEASURE_MOTOR_INPUTS_T
        /*! Motor Inputs */
        motorInputs;

    MOTOR_T
        /*! Motor Parameters */
        motor;

    FOC_T
        /*! FOC Structure */
        foc;

    LOAD_T
        /*! Load Structure */
        load;

    AT_PLL_ESTIM_T
        /*! AT PLL estimator */
        angleTrackingPLLEstim;

    const MC_ABC_T
        /*! Duty Cycle */
        *pDabc;

    MC_SECTOR_TYPES
        /*! Voltage Sector information */
        *pVoltageSector;
        
    int32_t
        /*! Blanking time */
        *pBlankingTime;
        
    int32_t
        /*! Min PWM count delta */
        *pMinPWMdelta;

    FAULT_SERVICE_T
        /*! Servicing of fault */
        faultService;

}UNIVERSAL_FOC_APPLICATION_T;

/*! @brief Motor Control Application Variable  */
extern UNIVERSAL_FOC_APPLICATION_T *g_pMC_App;


/* faults prototype */
/**
 * @brief     FOC_fault Report
 * @param[in] *pMC_App Input Pointer
 * @param[in] *pFault Input Pointer
 *
 */
void FOC_faultReport(UNIVERSAL_FOC_APPLICATION_T *pMC_App, FAULT_SERVICE_T *pFault);

/**
 * @brief     FOC_fault Recovery
 * @param[in] *pMC_App Input Pointer
 * @param[in] *pFault Input Pointer
 */
void FOC_fault_Recovery(UNIVERSAL_FOC_APPLICATION_T *pMC_App, FAULT_SERVICE_T *pFault);

#endif /* APPLICATION_H_ */
