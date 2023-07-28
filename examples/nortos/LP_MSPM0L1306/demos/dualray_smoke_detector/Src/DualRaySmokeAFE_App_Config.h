/* --COPYRIGHT--,BSD
 * Copyright (c) 2023, Texas Instruments Incorporated
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
//  Dual Ray AFE Application Configuration
//
//  Static and default configuration of Demo Application

#ifndef __DUALRAYSMOKEAFE_APP_CONFIG_H__
#define __DUALRAYSMOKEAFE_APP_CONFIG_H__
#include <stdbool.h>
#include <stdint.h>

//*********** Static Configuration *********************************************
#ifdef __AFE_TPS880X__
#define DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED (0)
#define DUALRAYSMOKE_APP_CO_SUPPORTED (1)
#define DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED (1)
#define DUALRAYSMOKE_APP_HORN_SUPPORTED (1)
#elif defined(__AFE_SACL3__)
#define DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED (1)
#define DUALRAYSMOKE_APP_CO_SUPPORTED (0)
#define DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED (1)
#define DUALRAYSMOKE_APP_HORN_SUPPORTED (1)
#endif

//*********** Default Dynamic Configuration ************************************
//! \brief Default interval of Ultra Low Power Timer in ms.
//!         This timer defines the period at which mesurements are taken.
//!         I.e. 1000 = 1000ms
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC (1000)

#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
//! \brief Default calibration interval of Ultra Low Power Timer.
//!         Set in measurement cycles.
//!         I.e. 60 = 60secs if ULP Timer interval is 1sec.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_CALIBRATION_INTERVAL (60)
#endif

#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
//! \brief Default Temperature sensor measurement interval.
//!         Set in measurement cycles.
//!         I.e. 5 = 5secs if ULP Timer interval is 1sec.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_TEMPERATURESENSOR_INTERVAL (5)
#endif

//! \brief Interval of Low Power Timer in ms.
//!         This timer is used to trigger periodic functions such as tracking
//!         test button debounce, and alarm timings
//!         I.e. 100 = 100ms
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC (100)

//! \brief Number of ms with Alarm ON in high cycle.
//!         Must be a multiple of DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC
//!         I.e. 500 = 500 ms
//!
#define DUALRAYSMOKEAFE_APP_ALARM_ONHIGH_MS (500)

//! \brief Number of ms with Alarm ON in LOW cycle.
//!         Must be a multiple of DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC
//!         I.e. 500 = 500 ms
//!
#define DUALRAYSMOKEAFE_APP_ALARM_ONLOW_MS (500)

//! \brief Number of Alarm Low cycles before turning OFF alarm after being activated
//!         and if no smoke is detected.
//!         I.e. if set to 5 with high cycle is 500ms, low cycle is 500ms
//!               the result will be 5x1000ms = 5 seconds
//!
#define DUALRAYSMOKEAFE_APP_ALARM_ONTOOFF_CYCLES (3)

//! \brief Number of seconds with the alarm desactivated (silent) when test
//!         button is pressed after being ON.
//!         I.e. 6 = 6 secs
//!
#define DUALRAYSMOKEAFE_APP_ALARM_SILENT_SEC (6)

//! \brief Default state of alarm.
//!         true if alarm is ON, false if not.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_ALARMON (false)

//! \brief Default state of alarm detection.
//!         true if alarm detection is enabled, false if not.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_ALARMDETECTIONENABLED (false)

#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
//! \brief Default state of horn/sounder enable.
//!         true if horn/sounder is enabled, false if not.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_HORNENABLED (false)
#endif

#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
//! \brief Default state of temperature measurement enable.
//!         true if temperature measurement is enabled, false if not.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_TEMPERATUREMEASENABLED (true)
#endif

#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
//! \brief Default state of RTC calibration.
//!         true if RTC calibration is enabled, false if not.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_RTCCALIBENABLED (false)
#endif

#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
//! \brief Default state of CO enable.
//!         true if CO measurements are enabled, false if not.
//!
#define DUALRAYSMOKEAFE_APP_DEFAULT_COENABLED (true)
#endif

#endif  //__DUALRAYSMOKEAFE_APP_CONFIG_H__
