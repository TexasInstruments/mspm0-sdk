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
 *  @file       dali_103_variables.h
 *  @brief      DALI Control Device Variable Definition
 *  @defgroup   DALI Library (DALI)
 *
 *  @anchor ti_dali_103_variables_overview
 *  # Overview
 *
 *  The DALI Control Device Variable Definition
 *
 *  <hr>
 ******************************************************************************/

#ifndef DALI_103_VARIABLES_H_
#define DALI_103_VARIABLES_H_

#include <stdbool.h>
#include <stdint.h>

/*! Mask byte */
#define MASK (uint8_t)(0xFF)
/*! Yes byte */
#define YES (uint8_t)(0xFF)

/*  ENUM Definitions            */

/*!
* @brief DALI states
*/
typedef enum {
    /*! Enabled State */
    ENABLED,
    /*! Disabled State */
    DISABLED,
    /*! Only Specific to InitialisationState, Device has been initialized */
    WITHDRAWN,
} DALI_stateType;

/* Flash Vars Definitions               */

/*! Device Instance count */
#define DEVICE_INSTANCE_COUNT (1)
/*! Application Controller Present */
#define APP_CONTROLLER_PRESENT (1)
/*! Device Operating Mode */
#define OPERATING_MODE (0x00)
/* Control Device Default Definitions           */

/*! Default Short Address */
#define SHORT_ADDRESS_DEFAULT MASK
/*! Default Device Group Assignment */
#define DEVICE_GROUP_DEFAULT (uint16_t)(0x0000)
/*! Default Random Address */
#define RANDOM_ADDRESS_DEFAULT (uint32_t)(0x00FFFFFF)
/*! Default Power Cycle Notification */
#define POWER_CYC_NOTIFICATION_DEFAULT DISABLED

/*! Default Reset State*/
#define RESET_STATE_DEFAULT false
/*! Default Instance Group0 */
#define INST_GROUP0_DEFAULT MASK
/*! Default Instance Group1 */
#define INST_GROUP1_DEFAULT MASK
/*! Default Instance Group2 */
#define INST_GROUP2_DEFAULT MASK
/*! Default Instance Active */
#define INST_ACTIVE_DEFAULT true
/*! Default Event Filter */
#define EVENT_FILTER_DEFAULT (uint32_t)(0x00FFFFFF)
/*! Default Event Scheme */
#define EVENT_SCHEME_DEFAULT 0
/*! Default Event Priority */
#define EVENT_PRIORITY_DEFAULT 4

/*  Reset Definitions              */
/*! Device Groups Reset Value */
#define DEVICE_GROUP_RESET (uint16_t)(0x0000)
/*! Search Address Reset Value */
#define SEARCH_ADDR_RESET (uint32_t)(0x00FFFFFF)
/*! Random Address Reset Value */
#define RANDOM_ADDRESS_RESET (uint32_t)(0x00FFFFFF)
/*! Quiescent Mode Reset Value */
#define QUIESCENT_MODE_RESET DISABLED
/*! Write Enable state Reset Value */
#define WRITE_ENABLE_RESET DISABLED
/*! Power Cycle Seen Reset Value */
#define POWER_CYC_SEEN_RESET false
/*! Initialisation State Reset Value */
#define INIT_STATE_RESET false
/*! Application Controller Error Reset Value */
#define APP_CONTROLLER_ERROR_RESET false
/*! Input Device Error Reset Value */
#define INPUT_DEVICE_ERROR_RESET false
/*! Reset State Reset Value*/
#define RESET_STATE_RESET true
/*! Instance Group0 Reset Value */
#define INST_GROUP0_RESET MASK
/*! Instance Group1 Reset Value */
#define INST_GROUP1_RESET MASK
/*! Instance Group2 Reset Value */
#define INST_GROUP2_RESET MASK
/*! Event Filter Reset Value */
#define EVENT_FILTER_RESET (uint32_t)(0x00FFFFFF)
/*! Event Scheme Reset Value */
#define EVENT_SCHEME_RESET 0
/*! Instance Error Reset Value */
#define INST_ERROR_RESET false

/*!
 * @brief variable to store all instance information
*/
typedef struct {
    /*! Stores the primary instance group number */
    uint8_t instanceGroups0;
    /*! Stores the additional instance group1 number*/
    uint8_t instanceGroups1;
    /*! Stores the additional instance group2 number*/
    uint8_t instanceGroups2;
    /*! Indicates if Event messages are enabled */
    bool instanceActive;
    /*! Stores the type of instance implemented */
    uint8_t instanceType;
    /*! Stores the instance resolution */
    uint8_t resolution;
    /*! Stores the instance input value */
    uint32_t inputValue;
    /*! Stores the instance Number */
    uint8_t instanceNumber;
    /*! Stores the instance Event Filter */
    uint32_t eventFilter;
    /*! Stores the instance event source addressing scheme */
    uint8_t eventScheme;
    /*! Stores the event priority */
    uint8_t eventPriority;
    /*! Indicates if an error occured in Instance */
    bool instanceError;
} dali_instanceVar;

/*!
 * @brief variable to store all control device information
*/
typedef struct {
    /*! Stores the short address of the control device */
    uint8_t shortAddress;
    /*! Stores the group that the device is a member of */
    uint32_t deviceGroups;
    /*! Stores the search address of the control device */
    uint32_t searchAddress;
    /*! Stores the random address of the control device */
    uint32_t randomAddress;
    /*! Stores the contents of Data Transfer Register 0 */
    uint8_t DTR0;
    /*! Stores the contents of Data Transfer Register 1 */
    uint8_t DTR1;
    /*! Stores the contents of Data Transfer Register 2 */
    uint8_t DTR2;
    /*! Stores the number of instances present in the device */
    uint8_t numberOfInstances;
    /*! Stores the operating mode of the control device */
    uint8_t operatingMode;
    /*! Indicates if quiescent mode is enabled */
    DALI_stateType quiescentMode;
    /*! Indicates if App Controller is enabled */
    bool applicationActive;
    /*! Indicates if Addressed memory bank can be written */
    DALI_stateType writeEnableState;
    /*! Indicates if App Controller is present in the device */
    bool applicationControllerPresent;
    /*! Indicates if Power Cycle has been seen */
    bool powerCycleSeen;
    /*! Indicates if Power Cycle Notification is enabled */
    DALI_stateType powerCycleNotification;
    /*! Stores the initialisation state of the control device */
    DALI_stateType initialisationState;
    /*! Indicates if an error occured in App Controller */
    bool applicationControllerError;
    /*! Indicates if an error occured in Input Device */
    bool inputDeviceError;
    /*! Indicates if device is in reset */
    bool resetState;
    /*! Stores instance information for each instance present */
    dali_instanceVar instance[DEVICE_INSTANCE_COUNT];

} dali_controlDeviceVar;

/*!
 * @brief variable to store user command information
*/
typedef struct {
    /*! Flag to indicate if command is to be transmitted */
    bool toSend;
    /*! Address Byte */
    uint8_t addrByte;
    /*! Instance Byte */
    uint8_t instanceByte;
    /*! OpCode Byte */
    uint8_t opcodeByte;
    /*! Flag to indicate if command is to be sent Twice */
    bool isSendTwiceCmd;
} userVars;

/*! Definition of global user variable */
extern volatile userVars gUserVar;

/*! Definition of global control device variable */
extern volatile dali_controlDeviceVar gControlDeviceVar1;

#endif /* DALI_103_VARIABLES_H_ */
