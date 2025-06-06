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
 *  @file       dali_103_command.h
 *  @brief      DALI Control Device General Command List and Functionality
 *  @defgroup   DALI Library (DALI)
 *
 *  @anchor ti_dali_103_commands_overview
 *  # Overview
 *
 *  The DALI Control Device General Commands and Functionality
 *
 *  <hr>
 ******************************************************************************/

#ifndef DALI_103_COMMAND_H_
#define DALI_103_COMMAND_H_

#include <ti/eeprom/emulation_type_a/eeprom_emulation_type_a.h>
#include "dali_103_variables.h"
#include "dali_gpio_comm.h"
#include "dali_timings.h"

/* Execution Command */

/* Device Configuration Commands */
/**
 * @brief define procedure to identify device
 */
void DALI_103_identifyDevice(void);

/**
 * @brief reset Power Cycle Seen flag
 */
void DALI_103_resetPowerCycleSeen(void);

/**
 * @brief reset control Device Vars
 */
void DALI_103_reset(void);

/**
 * @brief reset Memory Bank
 */
void DALI_103_resetMemBank(void);

/**
 * @brief assign a short address to a device based on command information
 */
void DALI_103_setShortAddr(void);

/**
 * @brief enable 'writeEnableState' flag
 */
void DALI_103_enableWriteMemory(void);

/**
 * @brief Enable Application Controller
 */
void DALI_103_enableAppController(void);

/**
 * @brief Disable Application Controller
 */
void DALI_103_disableAppController(void);

/**
 * @brief Set Operating Mode
 */
void DALI_103_setOperatingMode(void);

/**
 * @brief Add Device Groups - bits 0 to 15
 */
void DALI_103_addDeviceGroups_0_15(void);

/**
 * @brief Add Device Groups - bits 16 to 31
 */
void DALI_103_addDeviceGroups_16_31(void);

/**
 * @brief Remove Device Groups - bits 0 to 15
 */
void DALI_103_removeDeviceGroups_0_15(void);

/**
 * @brief Remove Device Groups - bits 16 to 31
 */
void DALI_103_removeDeviceGroups_16_31(void);

/**
 * @brief Enable Quiescent Mode
 */
void DALI_103_startQuiescentMode(void);

/**
 * @brief Disable Quiescent Mode
 */
void DALI_103_stopQuiescentMode(void);

/**
 * @brief Enable Power Cycle Notification
 */
void DALI_103_enablePowerCycNotification(void);

/**
 * @brief Disable Power Cycle Notification
 */
void DALI_103_disablePowerCycNotification(void);

/**
 * @brief Store NVM variables in flash
 */
void DALI_103_savePersistentVariables(void);

/* Special Commands */
/**
 * @brief Terminate Initialisation process
 */
void DALI_103_terminate(void);

/**
 * @brief Enable Initialisation state
 */
void DALI_103_initialise(void);

/**
 * @brief Set a Random value for randomAddress
 */
void DALI_103_randomise(void);

/**
 * @brief Compares if searchAddress is greater or equal to randomAddress
 */
void DALI_103_compare(void);

/**
 * @brief Withdraw Initialisation State
 */
void DALI_103_withdraw(void);

/**
 * @brief Set searchAddress bits 23 to 16
 */
void DALI_103_searchAddrH(void);

/**
 * @brief Set searchAddress bits 15 to 8
 */
void DALI_103_searchAddrM(void);

/**
 * @brief Set searchAddress bits 7 to 0
 */
void DALI_103_searchAddrL(void);

/**
 * @brief Set ShortAddress to data
 */
void DALI_103_programShortAddr(void);

/**
 * @brief Check if ShortAddress equals data
 */
void DALI_103_verifyShortAddr(void);

/**
 * @brief Send response as ShortAddr
 */
void DALI_103_queryShortAddr(void);

/**
 * @brief Write data into the memory location and send Data as the response
 */
void DALI_103_writeMemLocation(void);

/**
 * @brief Write data into the memory location
 */
void DALI_103_writeMemLocation_NoReply(void);

/**
 * @brief Set DTR0 to data
 */
void DALI_103_DTR0(void);

/**
 * @brief Set DTR1 to data
 */
void DALI_103_DTR1(void);

/**
 * @brief Set DTR2 to data
 */
void DALI_103_DTR2(void);

/**
 * @brief Write data to memory location indentified by DTR0 in Memory Bank DTR1
 */
void DALI_103_directWriteMem(void);

/**
 * @brief Set DTR1 to data1 and DTR0 to data0
 */
void DALI_103_DTR1_DTR0(void);

/**
 * @brief Set DTR2 to data1 and DTR1 to data0
 */
void DALI_103_DTR2_DTR1(void);

/**
 * @brief For testing Collision detection in multi master app controller
 */
void DALI_103_sendTestFrame(void);

/* Instance Configuration Commands */
/**
 * @brief Set Event Priority to DTR0
 */
void DALI_103_setEventPriority(uint32_t instMask);

/**
 * @brief Set 'InstanceActive' to true
 */
void DALI_103_enableInstance(uint32_t instMask);

/**
 * @brief Set 'InstanceActive' to false
 */
void DALI_103_disableInstance(uint32_t instMask);

/**
 * @brief Set Primary Instance Group to DTR0
 */
void DALI_103_setPrimaryInstGroup(uint32_t instMask);

/**
 * @brief Set Instance Group 1 to DTR0
 */
void DALI_103_setInstGroup1(uint32_t instMask);

/**
 * @brief Set Instance Group 2 to DTR0
 */
void DALI_103_setInstGroup2(uint32_t instMask);

/**
 * @brief Set Event Scheme to DTR0
 */
void DALI_103_setEventScheme(uint32_t instMask);

/**
 * @brief Set Event Filter to [DTR2:DTR1:DTR0]
 */
void DALI_103_setEventFilter(uint32_t instMask);

/* Device Query Instructions */
/**
 * @brief Transmits the current device status
 */
void DALI_103_query_deviceStatus(void);

/**
 * @brief Transmits YES if an application controller error has occurred
 */
void DALI_103_query_appControllerError(void);

/**
 * @brief Transmits YES if an input device error has occurred
 */
void DALI_103_query_inputDeviceError(void);

/**
 * @brief Transmits YES if short Address is equal to MASK
 */
void DALI_103_query_missingShortAddr(void);

/**
 * @brief Transmits the device version number
 */
void DALI_103_query_versionNumber(void);

/**
 * @brief Transmits the number of instances supported by the device
 */
void DALI_103_query_numberOfInstances(void);

/**
 * @brief Transmits the current value of DTR0
 */
void DALI_103_query_DTR0(void);

/**
 * @brief Transmits the current value of DTR1
 */
void DALI_103_query_DTR1(void);

/**
 * @brief Transmits the current value of DTR2
 */
void DALI_103_query_DTR2(void);

/**
 * @brief Transmits random address bits 23 to 16
 */
void DALI_103_query_randomAddrH(void);

/**
 * @brief Transmits random address bits 15 to 8
 */
void DALI_103_query_randomAddrM(void);

/**
 * @brief Transmits random address bits 7 to 0
 */
void DALI_103_query_randomAddrL(void);

/**
 * @brief Transmits data from a memory location.
 */
void DALI_103_query_readMemLocation(void);

/**
 * @brief Transmits YES, if application controller is enabled
 */
void DALI_103_query_appControlEnabled(void);

/**
 * @brief Transmits the current operating mode of the device.
 */
void DALI_103_query_operatingMode(void);

/**
 * @brief Transmits YES, if operatingMode is within manufacturer specific mode range
 */
void DALI_103_query_manufacturerSpecificMode(void);

/**
 * @brief Transmits YES, if quiescent mode is enabled
 */
void DALI_103_query_quiescentMode(void);

/**
 * @brief Transmits deviceGroups bits 0 to 7
 */
void DALI_103_query_deviceGroups_0_7(void);

/**
 * @brief Transmits deviceGroups bits 8 to 15
 */
void DALI_103_query_deviceGroups_8_15(void);

/**
 * @brief Transmits deviceGroups bits 16 to 23
 */
void DALI_103_query_deviceGroups_16_23(void);

/**
 * @brief Transmits deviceGroups bits 24 to 31
 */
void DALI_103_query_deviceGroups_24_31(void);

/**
 * @brief Transmits YES if power cycle notification is enabled
 */
void DALI_103_query_powerCycNotification(void);

/**
 * @brief Transmits the capabilities of the device
 */
void DALI_103_query_deviceCapabilites(void);

/**
 * @brief Transmits the extended Version number
 */
void DALI_103_query_extendedVersionNumber(void);

/**
 * @brief Transmits YES if device is in reset
 */
void DALI_103_query_resetState(void);

#endif /* DALI_103_COMMAND_H_ */
