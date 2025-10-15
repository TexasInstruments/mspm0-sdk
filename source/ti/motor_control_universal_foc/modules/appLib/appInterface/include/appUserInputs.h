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
 *  @file       appUserInputs.h
 *  @brief      Application Inputs Module
 *
 * 
 *  @anchor appUserInputs_h
 *  # Overview
 *
 *  defines user input structure
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPUSERINPUTS_H_
#define APPUSERINPUTS_H_

#include "stdint.h"
#include "measure.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif
/*! @brief Structure for module enable flags */
typedef struct
{
    uint16_t
        /*! Enables comparator based Over Current Detect */
        hardwareOverCurrentFaultEnable: 1,
        /*! Enables Low BEMF Stall Detect */
        lowBemfStallDetectEnable: 1,
        /*! Enables No Motor Stall Detect */
        noMotorStallDetectEnable: 1,
        /*! Enables Abnormal Speed Stall Detect */
        abnormalSpeedStallDetectEnable: 1,
        /*! Only report the fault*/
        motorStallReportOnlyEnable: 1,
        /*! Enables AVS by plugging */
        plugAVSEnable: 1,
        /*! Reserved */
        reserved: 10;
}
moduleEnableFlags;

/*! @brief Module enable flags */
typedef union
{
    /*! Module enables flags */
    moduleEnableFlags b;
    /*! Block access */
    uint16_t w;
}MODULE_ENABLE_FLAGS_T;

/*! @brief Structure for debug enable flags */
typedef struct
{
    uint16_t
        /*! Flag to updated register data */
        updatedData;


}debugEnableFlags;

/*! @brief Debug enable flags */
typedef union
{
    /*! Debug enable flags */
    debugEnableFlags b;
    /*! Block access */
    uint16_t w;
}DEBUG_ENABLE_FLAGS_T;

/*! @brief Motor start options */
typedef enum
{
    /*! Align and go method */
    MOTOR_START_ALIGN_AND_GO = 0,
    /*! Double align method */
    MOTOR_START_DOUBLE_ALIGN = 1,
    /*! IPD startup method */
    MOTOR_START_IPD = 2,
    /*! Slow first cycle method */
    MOTOR_START_SLOW_FIRST_CYCLE = 3,
}MOTOR_START_OPTIONS_TYPES;

/*! @brief Motor stop options */
typedef enum
{
    /*! High-Z or Tristate method */
    MOTOR_STOP_HI_Z = 0,
    /*! Active spin down method */
    MOTOR_STOP_ACTIVE_SPIN_DOWN = 1,
    /*! Hard stop method */
    MOTOR_STOP_HARD_STOP = 2,
}MOTOR_STOP_OPTIONS_TYPES;

/*! @brief User Input structure */
typedef struct
{
    /* Variables Expected from User. These include application inputs and
     * tuning parameters */

    /* Application Limits */
    int32_t
        /*! Over current limit in hardware */
        hardwareOverCurrentLimitPU,
        /*! No Motor Current Limit */
        noMotorLimitPU;
        
    /* Motor Parameters */
    uint32_t
        /*! Phase Resistance in milliOhms */
        rsMilliOhms,
        /*! D-Axis Inductance in uH */            
        LdMicroHenry,                     
        /*! Q-Axis Inductance in uH */
        LqMicroHenry,
        /*! BEMF Constant Units: Phase Peak MilliVolt per Hz */
        KeMilliVoltPhasePkPerHz,            

    /* Transition Parameters */
        /*! Forward Transition Speed */
        forwardTransitionSpeedPerMil,
        /*! Rate of aligning of angle */
        angleErrorSlewRateMilliDegreesPerMsec,

    /* Close Loop Parameters */
        /*! Close Loop Accel slew rate */
        closeLoopSlowAccelSpeedSlewRateDeciHzPerSec,

    /* Stall Detection Parameters */
        /*! Low BEMF Threshold */
        lowBemfEmagThresholdPerMil,
        /*! Abnormal Speed Limit */
        abnormalSpeedLimitPerMil,

    /* ISD Parameters */
        /*! BEMF threshold for motor stop */
        isdMaxStopBemfPU,

    /* Fault Parameters */
        /*! Retry time after fault */
        faultRetryTimeMsec,
        /*! Undervoltage hysteresis */
        underVoltageHystPU,
        /*! Overvoltage hysteresis */
        overVoltageHystPU;
        
    /* Module Enable Flags */
        /*! Module Flags */
    MODULE_ENABLE_FLAGS_T
        moduleFlags;                        

    /* Debug Enable Flags */
        /*! Debug Flags */
    DEBUG_ENABLE_FLAGS_T
        debugFlags;                         

    float
        /*! System DC bus voltage */
        systemDCBusVoltageVolts,            
        /*! Full scale phase current */
        fullScalePhaseCurrentAmp,           

    /* Motor Limits */
        /*! Motor Maximum Speed */
        maximumSpeedHz,
        /*! Base power */
        basePower,
        
    /* Controller Parameters */
        /*! Proportional - Current */
        kpCurrent,
        /*! Integral - Current */
        kiCurrent,
        /*! Proportional - Speed */
        kpSpeed,
        /*! Integral - Speed */
        kiSpeed,
        /*! Proportional - Flux Weakening */
        kpFluxWeak,
        /*! Integral - Flux Weakening */
        kiFluxWeak,
    /* IPD Parameters */
        /*! Time between two IPD pulses */
        ipdPulseToPulseTimeMsec;
        
    int32_t
        /*! Speed Input */
        speedInput,
        /*! Startup Open Loop Accel rate */
        openLoopFastAccelAccelSlewRateCentiHzPerSecPerSec,
        /*! Close Loop Accel slew rate */
        closeLoopFastAccelSpeedSlewRateDeciHzPerSec,
        /*! Close Loop Decel slew rate */
        closeLoopFastDecelSpeedSlewRateDeciHzPerSec;
        
    uint8_t
        /*! Maximum number of timer overflows allowed during IPD */
        ipdMaxOverflow;
}USER_INPUTS_T;

#ifdef __cplusplus
}
#endif
#endif /* APPUSERINPUTS_H_ */

