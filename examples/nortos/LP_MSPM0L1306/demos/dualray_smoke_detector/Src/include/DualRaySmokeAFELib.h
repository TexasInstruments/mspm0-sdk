/* --COPYRIGHT--,BSD
 * Copyright (c) 2019, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//*****************************************************************************
//         Dual Ray Smoke Sensor Software Library
//
// This library implements functions to read TI smoke sensors

#ifndef __DUALRAYSMOKEAFELIB_H__
#define __DUALRAYSMOKEAFELIB_H__

#include <Src/include/DualRaySmokeAFELib_Config.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

//! \brief Configuration of SW Library
typedef struct {
    //!< Indicates if alarm detection algorithm is enabled or disabled.
    //!     Set by application dynamically if needed.
    bool bAlarmDetectionEnabled;
    //!< Reflection delta for each LED before triggering an alarm
    //!     Set by application dynamically if needed, or as a default.
    int16_t i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_TOTAL];
} tDualRaySmokeAFELib_Config;

//! \brief Main structure with Data for Dual Ray Demo
typedef struct {
    //!< Smoke measurements.
    tDualRaySmokeAFE_HAL_AFE_SmokeMeasurement sSmoke;
    //!< Temperature measurement.
    tDualRaySmokeAFE_HAL_AFE_TemperatureMeasurement sTemperature;
} tDualRaySmokeAFELib_DualRayData;

#ifdef UNDO
//! \brief Alarm state machine
typedef enum {
    DUALRAYSMOKEAFE_ALARM_OFF = 0,
    DUALRAYSMOKEAFE_ALARM_WARNING1,
    DUALRAYSMOKEAFE_ALARM_WARNING2,
    DUALRAYSMOKEAFE_ALARM_ON,
    DUALRAYSMOKEAFE_ALARM_ON_THRESHOLDHIGH,
    DUALRAYSMOKEAFE_ALARM_ON_THRESHOLDLOW,
    DUALRAYSMOKEAFE_ALARM_TURN_ON,
    DUALRAYSMOKEAFE_ALARM_TURN_OFF,
    DUALRAYSMOKEAFE_ALARM_SILENT,
} tDualRaySmokeAFE_AlarmDetectionState;
#endif

#define DUALRAYSMOKEAFE_ALARM_DETECTION_ERROR (-1)
#define DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE (0x00)
#define DUALRAYSMOKEAFE_ALARM_DETECTION_IR (0x01)
#define DUALRAYSMOKEAFE_ALARM_DETECTION_BLUE (0x02)
#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
#define DUALRAYSMOKEAFE_ALARM_DETECTION_CO (0x04)
#endif

//************* Library and Detection Algorithm ********************************
//! \brief Initializes the AFE Library and Detection Algorithm.
//!     This function initializes both the library and the corresponding HAL
//!     functions for smoke detector AFE.
//!
//! \param[in] LibConfig is the pointer to the library configuration structure.
//! \param[in] HALConfig is the pointer to the HAL configuration structure.
//!
//! \return 0 if OK, -1 if error
extern int8_t DualRaySmokeAFELib_Init(tDualRaySmokeAFELib_Config *LibConfig,
    tDualRaySmokeAFE_HAL_Config *HALConfig);

//! \brief Updates the configuration of library.
//!     This function should be called if parameters are modified dynamically
//!     by the application.
//!
//! \param[in] LibConfig is the pointer to the library configuration structure.
//! \param[in] HALConfig is the pointer to the HAL configuration structure.
//!
//! \return 0 if OK, -1 if error
extern int8_t DualRaySmokeAFELib_UpdateConfig(
    tDualRaySmokeAFELib_Config *LibConfig,
    tDualRaySmokeAFE_HAL_Config *HALConfig);

//! \brief Runs basic algorithm to check if light was detected for either
//!     LED and decides if the alarm should be turned on or off.
//!
//! \param[in] Data is the pointer to the library data structure.
//!
//! \return DUALRAYSMOKEAFE_ALARM_DETECTION_ERROR if an error occurred.
//          DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE if no detection.
//          DUALRAYSMOKEAFE_ALARM_DETECTION_IR with IR threshold detection.
//          DUALRAYSMOKEAFE_ALARM_DETECTION_BLUE with blue threshold detection.
//          DUALRAYSMOKEAFE_ALARM_DETECTION_CO with CO threshold detection (if enabled).

extern int8_t DualRaySmokeAFELib_DetectionAlgorithm(
    tDualRaySmokeAFELib_DualRayData *Data);

//! \brief Turn ON the alarm.
//!
//! \param[in] sounderEn enables sounder when true.
//! \param[in] sounderHigh sets sounder High when true, otherwise is low.
//!
//! \return none
extern void DualRaySmokeAFELib_setAlarmOn(bool sounderEn, bool sounderHigh);

//! \brief Turn OFF the alarm.
//
//! \param[in] sounderEn enables sounder when true.
//!
//! \return none
extern void DualRaySmokeAFELib_setAlarmOff(bool sounderEn);

//************* Measurement ***************************************************
//! \brief Perform a measurement of both LEDs
//!
//! \param[out] SmokeData is the pointer to the data structure which will be
//!                 updated with LED information.
//! \return none
extern void DualRaySmokeAFELib_Measurement(
    tDualRaySmokeAFELib_DualRayData *SmokeData);

//! \brief Perform a temperature measurement
//!
//! \param[out] SmokeData is the pointer to the data structure which will be
//!                 updated with temperature information.
//! \return none
extern void DualRaySmokeAFELib_TemperatureMeasurement(
    tDualRaySmokeAFELib_DualRayData *SmokeData);

#endif  //__DUALRAYSMOKEAFELIB_H__
