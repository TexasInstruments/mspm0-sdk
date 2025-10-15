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
 *  @file       appUserInputsConfig.h
 *  @brief      Application input config apis
 *
 * 
 *  @anchor appUserInputsConfig_h
 *  # Overview
 *
 *  defines application input configuration apis
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPUSERINPUTSCONFIG_H
#define APPUSERINPUTSCONFIG_H

#include "appInputCtrlInterface.h"
#include "appUserInputs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Gets the open loop acceleration and deceleration rate
 * @param[in] index   Set Index
 * @return    Returns acceleration and deceleration rate from the config table
 */
int32_t openLoopAccelDecelRate(uint16_t index);

/**
 * @brief     Gets the close loop acceleration and deceleration rate
 * @param[in] index   Set Index
 * @return    Returns acceleration and deceleration rate from the config table
 */
int32_t closeLoopAccelDecelRate(uint16_t index);

/**
 * @brief     Updates ISD configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_ISDConfig(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates startup1 configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_MotorStartUp1Config(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates startup2 configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_MotorStartUp2Config(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates close loop 1 configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_CloseLoop1Config(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates motor configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_SystemParams(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates fault config 1 configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_FaultCfg1Config(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates fault config 2 configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_FaultCfg2Config(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates Misc algo configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void update_MiscAlgoConfig(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Updates all Input Registers configurations
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void appUserInputRegsConfig(USER_INPUTS_T *pUserInputs);

/**
 * @brief     Initializes user inputs
 * @param[in] pUserInputs   Pointer to user Inputs
 */
void appUserInputsInit(USER_INPUTS_T *pUserInputs);

#ifdef __cplusplus
}
#endif
#endif /* APPUSERINPUTSCONFIG_H */
