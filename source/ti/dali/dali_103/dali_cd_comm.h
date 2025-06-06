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
 *  @file       dali_cd_comm.h
 *  @brief      DALI Control Device Communication and Execution
 *  @defgroup   DALI Library (DALI)
 *
 *  @anchor ti_dali_cd_overview
 *  # Overview
 *
 *  The DALI Control Device Communication and Execution Functions
 *
 *  <hr>
 ******************************************************************************/

#ifndef DALI_CD_COMM_H_
#define DALI_CD_COMM_H_

#include <ti/eeprom/emulation_type_a/eeprom_emulation_type_a.h>
#include "dali_103_command.h"
#include "dali_103_variables.h"
#include "dali_gpio_comm.h"

#ifdef DALI_303_SUPPORT
#include "dali_303/dali_303_command.h"
#include "dali_303/dali_303_variables.h"
#include "dali_303/pir_interface.h"

#endif /* DALI_303_SUPPORT */

/**
 * @brief Initializes the DALI Control Device by enabling DALI Tx and loading default values.
 *
 */
void DALI_ControlDevice_init(void);

/**
 * @brief Handles incoming DALI control device commands.
 *
 */
void DALI_ControlDevice_receive(void);

/**
 * @brief Execute Device Specific Commands
 *
 * @param[in]  opCode   Command byte
 */
void DALI_ControlDevice_execDeviceCommand(uint8_t opCode);

/**
 * @brief Executes instance specific Commands
 *
 * @param[in] opCode      Command byte
 * @param[in] instMask    Target Instances
 */
void DALI_ControlDevice_execInstanceCommand(uint8_t opCode, uint32_t instMask);

/**
 * @brief Execute Device Special Commands
 *
 */
void DALI_ControlDevice_execSpecialCommand(void);

/**
 * @brief Transmits a forward frame , sending Control Device Commands.
 *
 */
void DALI_ControlDevice_testTransmit(void);

/**
 * @brief Transmit frame with the data stored in DALI Tx
 *
 */
void DALI_ControlDevice_transmit(void);

/**
 * @brief Check if command is addressed to this device
 *
 * @param[in]  AddrByte   Address byte
 * @return True if the command is addressed to this device, false otherwise.
 */
bool DALI_ControlDevice_AddrCheck(uint8_t AddrByte);

/**
 * @brief Checks the instance byte and returns the corresponding instance mask.
 *
 * @param[in]   InstByte    Instance byte
 * @return The instance mask corresponding to the instance byte.
 */
uint32_t DALI_ControlDevice_InstCheck(uint8_t InstByte);

/**
 * @brief Load Default Values to Control Device Variables
 *
 */
void DALI_ControlDevice_loadDefaults(void);

/**
 * @brief Prepares Rx Buffer with Event Message
 *
 * @param[in]  instanceIndex   Index of Instance whose event message has to be sent
 */
void DALI_ControlDevice_EventMessages(uint8_t instanceIndex);

#ifdef DALI_303_SUPPORT
/**
 * @brief Initializes the DALI 303 module.
 *
 * @param[in] configRegister Pointer to the PIR configuration register.
 */
void DALI_303_init(PIR_configRegister *configRegister);

/**
 * @brief Reads Input from PIR Sensor
 *
 */
void DALI_303_PIR_readInput(void);
#endif /* DALI_303_SUPPORT */

#endif /* DALI_CD_COMM_H_ */
