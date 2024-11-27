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
 *  @file       faults.h
 *  @brief      faults Module
 *
 * 
 *  @anchor faults_h
 *  # Overview
 *
 *
 *  <hr>
 ******************************************************************************/


#ifndef ALGOLIB_FAULTSERVICE_INCLUDE_FAULTS_H_
#define ALGOLIB_FAULTSERVICE_INCLUDE_FAULTS_H_

#include "stdint.h"
#include "stallDetect.h"
#include "load.h"
#include "focHALInterface.h"
#include "sourceVoltageLimit.h"
#include "main.h"

/*! @brief Default Controller Fault Report Values */
#define CONTROLLER_FAULT_REPORT_DEFAULT         0x7FFFF800
/*! @brief Default Controller Fault Action Values */
#define CONTROLLER_FAULT_ACTION_DEFAULT         0x3BFF0000
/*! @brief Default Controller Fault Latched Values */
#define CONTROLLER_FAULT_ACTION_LATCHED_DEFAULT 0x03000000

/* bit position for faults in fault status register for external communication and control */

/* Controller fault reporting */
/*! @brief IPD_CLOCK_TIMEOUT_FAULT_INDEX Fault Bit */
#define IPD_CLOCK_TIMEOUT_FAULT_INDEX   0x10000000
/*! @brief IPD_DECAY_TIME_FAULT_INDEX Fault Bit */
#define IPD_DECAY_TIME_FAULT_INDEX      0x08000000

/*! @brief BUS_CURRENT_LIMIT_INDEX Fault Bit */
#define BUS_CURRENT_LIMIT_INDEX     0x04000000
/*! @brief ABN_SPEED_FAULT_INDEX Fault Bit */
#define ABN_SPEED_FAULT_INDEX       0x00800000
/*! @brief ABN_BEMF_FAULT_INDEX Fault Bit */
#define ABN_BEMF_FAULT_INDEX        0x00400000
/*! @brief NO_MOTOR_FAULT_INDEX Fault Bit */
#define NO_MOTOR_FAULT_INDEX        0x00200000
/*! @brief MTR_LOCK_FAULT_INDEX Fault Bit */
#define MTR_LOCK_FAULT_INDEX        0x00F00000
/*! @brief MTR_LOCK_FAULT_STATUS Fault Bit */
#define MTR_LOCK_FAULT_STATUS       0x00100000
/*! @brief HW_LOCK_ILIMIT_FAULT_INDEX Fault Bit */
#define HW_LOCK_ILIMIT_FAULT_INDEX  0x00040000
/*! @brief UNDER_VOLTAGE_FAULT_INDEX Fault Bit */
#define UNDER_VOLTAGE_FAULT_INDEX   0x00020000
/*! @brief OVER_VOLTAGE_FAULT_INDEX Fault Bit */
#define OVER_VOLTAGE_FAULT_INDEX    0x00010000

/*! @brief Application Faults Values */
typedef enum
{
    /*! NO_FAULT Fault Value */
    NO_FAULT = 0,
    /*! MOTOR_STALL Fault Value*/
    MOTOR_STALL = 1,
    /*! VOLTAGE_OUT_OF_BOUNDS  Fault Value*/
    VOLTAGE_OUT_OF_BOUNDS = 2,
    /*! LOAD_STALL Fault Value*/
    LOAD_STALL = 3,
    /*! HARDWARE_OVER_CURRENT Fault Value*/
    HARDWARE_OVER_CURRENT = 4,
    /*! HV_DIE_FAULT Fault Value */
    HV_DIE_FAULT = 5,

}APP_FAULT_TYPES;

/*! @brief Fault flags */
typedef union
{
    /*! @brief faultFlags structure */
    struct faultFlags
    {
        _Bool
            /*! hardwareOverCurrentRetryEnable */
            hardwareOverCurrentRetryEnable: 1,
            /*! noMotorRetryEnable */
            noMotorRetryEnable: 1,
            /*! abnormalSpeedRetryEnable */
            abnormalSpeedRetryEnable: 1,
            /*! lowBemfRetryEnable */
            lowBemfRetryEnable: 1,
            /*! clearFault */
            clearFault: 1,
            /*! retryEnable */
            retryEnable: 1,
            /*! retryStatus */
            retryStatus: 1,
            /*! actionTaken */
            actionTaken: 1;
    }
    /*! Bitwise access */
    b;
    /*! Block access */
    uint16_t w;

}FAULT_FLAGS_T;

/*! @brief Fault Stop Options */
typedef enum
{
    /*! Fault during Hi-Z */
    FAULT_STOP_HI_Z = 0,
    /*! Fault during High Side Brake */
    FAULT_STOP_HIGH_BRAKE = 1,
    /*! Fault during Low Side Brake */
    FAULT_STOP_LOW_BRAKE = 2,

}FAULT_STOP_TYPES;

/*! @brief Fault Service Structure */
typedef struct
{
    /*! Fault Status */
    const APP_FAULT_TYPES
        *pFaultStatus;

    /*! Motor Stall Types */
    const STALL_TYPES
        *pMotorStallStatus;

    /*! Source Fault Types */
    const SOURCE_VOLTAGE_FAULT_TYPES
        *pSourceVoltageFaultStatus;

    /*! Load Fault Types */
    const LOAD_FAULT_TYPES
        *pLoadFaultStatus;

    /*! Load Pointer Value */
    LOAD_T
        *pLoad;

    /*! Fault Stop options */
    FAULT_STOP_TYPES
        /*! HW OCP  */
        hardwareOverCurrent,
        /*! no Motor Fault type */
        noMotor,
        /*! Low BEMF Fault Type */
        lowBemf,
        /*! Abnormal Speed Fault type */
        abnormalSpeed;

    /*! Motor Fault Flags */
    FAULT_FLAGS_T
        flags;

    /*! Fault Counter */
    uint16_t
    /*! Fault Counter Value */
        count,
    /*! Fault Counter Value Max*/
        countMax;

}

FAULT_SERVICE_T;

/**
 * @brief     faultService Initialization
 * @param[in] *pFault Pointer
 */
void faultServiceInit(FAULT_SERVICE_T *pFault);
/**
 * @brief     faultService Run Module
 * @param[in] *pFault Pointer
 */
void faultServiceRun(void *pFault);
/**
 * @brief     faultService Low Priority Run Module
 * @param[in] *pFault Pointer
 */
void faultServiceLowPriorityRun(void *pFault);
/**
 * @brief     faultService Report Clear
 * @param[in] *pFault Pointer
 */
void faultReportClear(FAULT_SERVICE_T *pFault);
/**
 * @brief     FOC Fault Status
 * @param[in] *pFault Pointer
 */
void update_FOC_faultStatus(void *pFault);
/**
 * @brief     Fault retry Status
 * @param[in] *pFault Pointer
 * @return    Returns the status
 */
inline _Bool getFaultRetryStatus(FAULT_SERVICE_T *pFault)
{
    return pFault->flags.b.retryStatus;
}

/**
 * @brief     Fault retry Status
 * @param[in] *pFault Pointer
 * @return    Returns the status
 */
inline _Bool getFaultClearStatus(FAULT_SERVICE_T *pFault)
{
    return pFault->flags.b.clearFault;
}

/** @brief Controller Fault report */
extern uint32_t controllernFaultReport;
/** @brief Controller Fault Action*/
extern uint32_t controllerFaultAction;
/** @brief Controller Latched Fault Status */
extern uint32_t controllerFaultActionLatched;

#endif /* ALGOLIB_FAULTSERVICE_INCLUDE_FAULTS_H_ */

