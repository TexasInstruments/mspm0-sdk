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

#ifndef ISR_H
#define ISR_H

#include "stdint.h"
#include "application.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Extern global MC app */
extern UNIVERSAL_FOC_APPLICATION_T *g_pMC_App;

/** @brief Extern gate driver raw fault status */
extern uint32_t gateDriverRawFaultStatus;
/** @brief Extern controller raw fault status */
extern uint32_t controllerRawFaultStatus;

/**
 * @brief Initialize user inputs
 */
void userInputsInit(void);

/**
 * @brief Initialize all configs
 */
void updateConfigsInit(void);

/**
 * @brief Update all configs
 */
void updateConfigs(void);

/**
 * @brief FOC ISR called by ADC interrupt
 */
void FOC_ADC_ISR(void);

/**
 * @brief Application configuration initialization
 */
void appConfig(void);

/**
 * @brief Reset application inputs
 */
void appInputReset(void);

/**
 * @brief Motor control app
 * @param[in] gpMC_App  MC App pointer
 */
void applicationRun(void *gpMC_App);

/**
 * @brief Low priority application
 * @param[in] gpMC_App  MC App pointer
 */
void applicationLowPriorityRun(void *gpMC_App);

/**
 * @brief Low priority application
 * @param[in] gpMC_App  MC App pointer
 */
void applicationConfig(void *gpMC_App);

/**
 * @brief Reset all the Application Data variables to Zero
 * @param[in] gpMC_App  MC App pointer
 */
void appReset(void *gpMC_App);

/**
 * @brief IPD current threshold API
 * @param[in] gpMC_App  MC App pointer
 */
void applicationIPDCurrThresh(void *gpMC_App);

/**
 * @brief IPD timer overflow API
 * @param[in] gpMC_App  MC App pointer
 */
void applicationIPDOverFlow(void *gpMC_App);

/**
 * @brief Return size of MC app
 * @return Size of MC app
 */
uint32_t getMCAppSize(void);

#ifdef __cplusplus
}
#endif
#endif /* ISR_H */
