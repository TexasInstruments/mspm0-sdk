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
 *  @file       appUserOutputs.h
 *  @brief      Application Outputs Module
 *
 * 
 *  @anchor appUserOutputs
 *  # Overview
 *
 *  Handles the output variables for user to understand
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPUSEROUTPUTS_H
#define APPUSEROUTPUTS_H

#include "stdint.h"
#include "mcLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Motor states */
typedef enum
{
    /*! Motor Idle state */
    MOTOR_IDLE = 0,
    /*! Motor in ISD state */
    MOTOR_ISD = 1,
    /*! Motor in Tristate state */
    MOTOR_TRISTATE = 2,
    /*! Motor in Brake on start state */
    MOTOR_BRAKE_ON_START = 3,
    /*! Motor in IPD state */
    MOTOR_IPD = 4,
    /*! Motor in Slow first cycle state */
    MOTOR_SLOW_FIRST_CYCLE = 5,
    /*! Motor in align state */
    MOTOR_ALIGN = 6,
    /*! Motor in open loop state */
    MOTOR_OPEN_LOOP = 7,
    /*! Motor in close loop unaligned state */
    MOTOR_CLOSE_LOOP_UNALIGNED = 8,
    /*! Motor in close loop aligned state */
    MOTOR_CLOSE_LOOP_ALIGNED = 9,
    /*! Motor in active brake state */
    MOTOR_CLOSE_LOOP_ACTIVE_BRAKING = 10,
    /*! Motor in soft stop state */
    MOTOR_SOFT_STOP = 11,
    /*! Motor in brake on stop state */
    MOTOR_BRAKE_ON_STOP = 12,
    /*! Motor in fault state */
    MOTOR_FAULT = 13,
    /*! Motor Max State */
    MOTOR_MAX = 0xFFFF,
}MOTOR_STATE_TYPES_T;

/*! @brief Fault types */
typedef enum
{
    /*! No Fault */
    NO_FAULTS = 0,
    /*! Motor Stall detected */
    FAULT_MOTOR_STALL = 1,
    /*! Voltage out of bounds detected */
    FAULT_VOLTAGE_OUT_OF_BOUNDS = 2,
    /*! Motor startup fault detected */
    FAULT_LOAD_STALL = 3,
    /*! Over current fault detected */
    FAULT_HARDWARE_OVER_CURRENT = 4,
    /*! HVDie Fault */
    FAULT_HV_DIE = 5,
    /*! Fault Max */
    FAULT_NONE = 0xFFFF,
}USER_FAULT_TYPES;

/*! @brief PI Current output structure */
typedef struct
{
    /*! PI current Kp */
    float     kp;
    /*! PI current Ki */
    float     ki;

}OUTPUTS_CURRENT_PI_T;

/*! @brief PI output structure */
typedef struct
{
    /*! PI reference */
    int32_t        reference;
    /*! PI Feedback */
    int32_t        feedback;
}OUTPUTS_PI_T;

/*! @brief DQ structure */
typedef struct
{
        /*! D axis */
    int32_t d;
        /*! Q axis */
    int32_t q;

}OUTPUT_DQ_T;


/*! @brief IPD identified sector types */
typedef enum
{
    /*! IPD sector Ac */
    Ac = 0,
    /*! IPD sector aC */
    aC = 1,
    /*! IPD sector Bc */
    Bc = 2,
    /*! IPD sector bC */
    bC = 3,
    /*! IPD sector aB */
    aB = 4,
    /*! IPD sector Ab */
    Ab = 5,
    /*! Max number of IPD sector */
    IPD_IDENTIFIED_SECTOR_MAX =  0xFFFFFFFF
}IPD_IDENTIFIED_SECTOR_T;

/*! @brief appVersion structure */
typedef struct
{
    uint32_t
    /*! Patch version */
    patchVersion:       8,
    /*! Minor version */
    minorVersion:       8,
    /*! Major version */
    majorVersion:       8,
    /*! Reserved */
    reserved:           8;
}appVersion;

/*! @brief appVersion register */
typedef union
{
    /*! Bit wise access */
    appVersion b;
    /*! Block access */
    uint32_t w;
}APP_VERSION_T;

/*! @brief User output structure */
typedef struct
{
    /*! Fault status */
    /*Field Length: 1*/
    USER_FAULT_TYPES systemFaultStatus;
    /*! Motor state */
    MOTOR_STATE_TYPES_T motorState;
    /*! Filtered dq voltage */
    OUTPUT_DQ_T VdqFilt;
    /*! PI current outputs */
    OUTPUTS_CURRENT_PI_T currentPI;
    /*! PI speed */
    OUTPUTS_PI_T piSpeed;
    /*! PI Power */
    OUTPUTS_PI_T piPower;
    /*! PI ID */
    OUTPUTS_PI_T piId;
    /*! PI IQ */
    OUTPUTS_PI_T piIq;
    /*! IPD identified sector */
    IPD_IDENTIFIED_SECTOR_T ipdIdentifiedSector;
    /*! Estimated speed */
    int32_t estimatedSpeed;
    /*! DC bus voltage */
    int32_t dcBusVoltage;
    /*! Torque limit(Q axis current limit) */
    int32_t torqueLimit;
    /*! @brief Gate driver faults status register for external reporting  */
    uint32_t gateDriverFaultStatus;
    /*! @brief controller fault status register for external reporting */
    uint32_t controllerFaultStatus;
    /*! Application FW version */
    APP_VERSION_T appVersion;
}USER_STATUS_INTERFACE_T;

/* Extern parameter */

/** @brief pointer to userOutputs */
extern USER_STATUS_INTERFACE_T *pUserStatusRegs;

#ifdef __cplusplus
}
#endif
#endif /* APPUSEROUTPUTS_H */
