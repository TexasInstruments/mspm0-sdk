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
 *  @file       dali_303_command.h
 *  @brief      DALI Control Device - Occupancy Sensor Command List and Functionality
 *  @defgroup   DALI Library (DALI)
 *
 *  @anchor ti_dali_303_commands_overview
 *  # Overview
 *
 *  The DALI Control Device - Occupancy Sensor Commands and Functionality
 *
 *  <hr>
 ******************************************************************************/

#ifndef DALI_303_COMMAND_H_
#define DALI_303_COMMAND_H_

#include "ti/dali/dali_103/dali_103_variables.h"
#include "ti/dali/dali_103/dali_gpio_comm.h"
#include "ti/dali/dali_103/dali_timings.h"

#include "dali_303_variables.h"

/* Execution Command */

/* Instance Control Commands */
/**
 * @brief Enables Catching
 */
void DALI_303_catchMovement(void);

/**
 * @brief Stops Hold Timer
 */
void DALI_303_cancelHoldTimer(void);

/**
 * @brief Sets Hold Timer Counter
 */
void DALI_303_setHoldTimer(void);

/**
 * @brief Sets Report Timer Counter
 */
void DALI_303_setReportTimer(void);

/**
 * @brief Sets DeadTime Timer Counter
 */
void DALI_303_setDeadtimeTimer(void);

/**
 * @brief Sends Hold Timer Counter Value
 */
void DALI_303_queryHoldTimer(void);

/**
 * @brief Sends Report Timer Counter Value
 */
void DALI_303_queryReportTimer(void);

/**
 * @brief Sends DeadTime Timer Counter Value
 */
void DALI_303_queryDeadtimeTimer(void);

/**
 * @brief Sends YES if catching is enabled
 */
void DALI_303_queryCatching(void);

/**
 * @brief Calculates the counter value for TIMER_303
 *
 * @param[in]   time    Time for which counter value is calculated
 *
 * @retval Corresponding counter value
 */
uint32_t DALI_303_getCounterValue(uint32_t time);

#endif /* DALI_303_COMMAND_H_ */
