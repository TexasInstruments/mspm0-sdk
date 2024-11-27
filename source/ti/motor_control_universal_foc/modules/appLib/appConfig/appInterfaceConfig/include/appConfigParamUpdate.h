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
 *  @file       appConfigParamUpdate.h
 *  @brief      appConfigParamUpdate Module
 *
 * 
 *  @anchor appConfigParamUpdate_h
 *  # Overview
 *
 *  APIs for updating the application configurations
 *
 *  <hr>
 ******************************************************************************/

#ifndef ALGOLIB_APPINTERFACE_ALGORITHM_INCLUDE_APPCONFIGPARAMUPDATE_H_
#define ALGOLIB_APPINTERFACE_ALGORITHM_INCLUDE_APPCONFIGPARAMUPDATE_H_

#include "application.h"

/*! @brief mod index limit */
#define MOD_INDEX_LIMIT_FW                          _IQ(0.80)

/*! @brief 0.5/15 in IQ */
#define POINT_FIVE_BY_FIFTEEN                       _IQ(0.51/15)

/*! @brief Second align angle slew rate */
#define SECOND_ALIGN_ANGLE_SLEW_RATE                _IQ(9.0/360.0)

/*! @brief minimum target duty in float */
#define MIN_TARGET_DUTY_FLOAT                       0.01
/*! @brief minimum speed */
#define MIN_TARGET_SPEED                            _IQ(MIN_TARGET_DUTY_FLOAT)

/* Stall Detection Parameters */
/*! @brief voltage out of bound time */
#define VOLTAGE_OUT_OF_BOUNDS_TIME_MSEC             50.0

/*! @brief no motor time */
#define NO_MOTOR_TIME_MSEC                         500
/*! @brief abnormal speed time */
#define ABNORMAL_SPEED_TIME_MSEC                    10

/**
 * @brief     update Configuration Parameters
 * @param[in] *pMC_App Input Pointer
 */
void updateConfigurationParameters(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
/**
 * @brief     update RAM Configuration Parameters
 * @param[in] *pMC_App Input Pointer
 */
void updateRAMConfigurationParameters(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
/**
 * @brief     update Motor Stop Config Param
 * @param[in] *pMC_App Input Pointer
 */
void updateMotorStopConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
/**
 * @brief      user config faults
 * @param[in] *pMC_App Input Pointer
 */
void user_config_faults(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
/**
 * @brief      update closeloop Config Param
 * @param[in] *pMC_App Input Pointer
 */
void updateCloseLoopConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
/**
 * @brief      update current control Config Param
 * @param[in] *pMC_App Input Pointer
 */
void updateCurrentControlConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App);

/**
 * @brief pi RAM Input
 * @param[in] piGain Input
 * @return Size of MC app
 */
uint16_t piRAMInput(float piGain);

#endif /* ALGOLIB_APPINTERFACE_ALGORITHM_INCLUDE_APPCONFIGPARAMUPDATE_H_ */
