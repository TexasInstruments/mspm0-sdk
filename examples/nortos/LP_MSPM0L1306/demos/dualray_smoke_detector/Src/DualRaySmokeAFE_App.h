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


#ifndef __DUALRAYSMOKEAFE_APP_H__
#define __DUALRAYSMOKEAFE_APP_H__

#include <Src/include/DualRaySmokeAFELib.h>
#include <Src/include/DualRaySmokeAFE_Config_Common.h>
#include <stdbool.h>
#include <stdint.h>
#include "DualRaySmokeAFE_App_Config.h"

//! \brief Alarm Status
typedef enum {
    AlarmStatus_Inactive = 0,  //!< Not configured.
    AlarmStatus_Idle,          //!< Alarm off, system is taking measurements.
    AlarmStatus_Warning1,      //!< Alarm off, warning 1 after 1st detection.
    AlarmStatus_Warning2,      //!< Alarm off, warning 2 after 2nd detection.
    AlarmStatus_Silent,        //!< Alarm is in silent mode.
    AlarmStatus_OnHigh,        //!< Alarm is ON, high cycle
    AlarmStatus_OnLow,         //!< Alarm is OFF, low cycle
    AlarmStatus_Action,        //!< Take an Alarm action and don't go to sleep
    AlarmStatus_TurnSilentOn,  //!< Turn Silent Mode On, don't go to sleep
    AlarmStatus_TurnOn,        //!< Turn alarm ON, don't go to sleep
    AlarmStatus_TurnOff,       //!< Turn alarm OFF, don't go to sleep
    AlarmStatus_Error,         //!< Error in state machine
} tAlarmStatus;

//! \brief Configuration of Dual Ray AFE demo application.
typedef struct {
    //!< Measurement interval in ms.
    uint16_t u16MeasIntervalms;
    //!< Temporary measurement interval in ms. Used when the interval
    //!     is modified temporarily (i.e. alarm warning).
    uint16_t u16MeasIntervalms_tempor;
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    //!< Temperature sensor measurement interval in measurement cycles.
    //!<    Set to 0 to disable.
    uint16_t u16TempSensorInterval;
#endif
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
    //!< RTC calibration interval in measurement cycles. Set to 0 to disable.
    uint16_t u16RTCCalibInterval;
#endif
    //!< Low Power Periodic Timer interval in ms.
    //!     Used to keep track of test button and alarm timings.
    uint16_t u16LPPerTimerIntervalms;
    //!< Number of Low-Power Periodic Timer cycles before turning OFF alarm
    //!     if no smoke is detected.
    uint16_t u16LPPerTimerCyclesBeforeAlarmOff;
    //!< Number of Low-Power Periodic Timer cycles with Alarm ON high
    uint16_t u16LPPerTimerCyclesAlarmOnHigh;
    //!< Number of Low-Power Periodic Timer cycles with Alarm ON Low
    uint16_t u16LPPerTimerCyclesAlarmOnLow;
    //!< Number of Low-Power Periodic Timer cycles counting when alarm is silent.
    uint16_t u16PPerTimerCyclesAlarmSilent;
    //!< Status of the Alarm state machine.
    tAlarmStatus eAlarmStatus;
    //!< Application status flags.
    union _status_flags {
        uint8_t all;
        struct {
            //!< true if alarm is ON.
            uint8_t AlarmOn : 1;
            //!< true if alarm detection is enabled, false if not.
            uint8_t AlarmDetectionEnabled : 1;
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
            //!< true if alarm horn is enabled, false if not.
            uint8_t HornEnabled : 1;
#endif
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
            //!< true if RTC Calibration is enabled, false if not.
            uint8_t RTCCalibEnabled : 1;
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
            //!< true if temperature measurement is enabled.
            uint8_t TemperatureMeasEnabled : 1;
#endif
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
            //!< true CO measurement is enabled.
            uint8_t COEnabled : 1;
#endif
            //!< true if alarm is in silent
            uint8_t AlarmSilent : 1;
            //!< Low-Power mode, disables GUI transmissions when set
            uint8_t LPModeEnabled : 1;
        };
    } APPSTATUS;
    //!< Application Control flags
    union _ctl_flags {
        uint8_t all;
        struct {
            //!< Set to start a new measurement
            //   Placed in BIT0 for easier masking using APPCTL_STARTMEASUREMENT
            uint8_t StartMeasurement : 1;
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
            //!< Set to enable RTC calibration.
            uint8_t StartCalibration : 1;
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
            //!< Set to do temperature sensor measurement.
            uint8_t StartTemperatureMeas : 1;
#endif
            //!< Start alarm test.
            uint8_t StartAlarmTest : 1;
            //!< Turn Alarm Silent Mode On.
            uint8_t TurnAlarmSilentOn : 1;
            //!< Set to trigger a Library/HAL configuration update.
            uint8_t LibConfigUpdate : 1;
            //!< Set to trigger an Application configuration update.
            uint8_t AppConfigUpdate : 1;
        };
    } APPCTL;
} tDualRaySmokeAFE_AppConfig;

// Flags for APPCTL soft-register
#define APPCTL_STARTMEASUREMENT (0x01)

//! \brief Configuration structure of Dual Ray demo application.
extern tDualRaySmokeAFE_AppConfig sDualRaySmokeAFE_AppConfig;
//! \brief Configuration structure of Dual Ray AFE library.
extern tDualRaySmokeAFELib_Config sDualRaySmokeAFELib_Config;
//! \brief Structure with measurement data of dual Ray AFE.
extern tDualRaySmokeAFELib_DualRayData sDualRaySmokeAFELib_Data;
//! \brief Structure with HAL Configuration.
extern tDualRaySmokeAFE_HAL_Config sDualRaySmokeAFE_HALConfig;

//************* Application Control ********************************************
//! \brief Turns Alarm On
//!
//! \return none
extern void DualRaySmokeAFE_App_TurnAlarmOn(void);

//! \brief Turns Alarm Off
//!
//! \return none
extern void DualRaySmokeAFE_App_TurnAlarmOff(void);

//************* Configuration *************************************************
//! \brief Sets the measurement interval in msec.
//!
//! \param[in] interval is the measurement interval in milliseconds.
//!
//! \return none
extern void DualRaySmokeAFE_App_Config_SetMeasIntervalmsec(uint16_t interval);

//! \brief Sets a temporary measurement interval in msec.
//!         A temporary interval can be used to increase or decrease the
//!         measurement frequency temporarily.
//!
//! \param[in] interval is the temporary measurement interval in milliseconds.
//!
//! \return none
extern void DualRaySmokeAFE_App_Config_SetTempMeasIntervalmsec(
    uint16_t interval);

//! \brief Restores the default measurement interval.
//!         Used after setting a temporary interval.
//!
//! \return none
extern void DualRaySmokeAFE_App_Config_RestoreDefaultIntervalmsec(void);

//************* GUI Communication **********************************************
//! \brief Initializes the GUI Communication interface and protocol.
//!         The HAL will be initialized and RX Command structure will be passed
//!         to the drivers so that the corresponding callback functions are
//!         called.
//!
//! \return none
extern void DualRaySmokeAFE_App_GUIComm_Init(void);

//! \brief Sends Application data to the GUI.
//!         The device will send the contents of GUI_TXCmdData structure.
//!
//! \return none
extern void DualRaySmokeAFE_App_GUIComm_SendAppData(void);

//! \brief Sends temperatue info to the GUI.
//!         Note that this function uses floating point.
//!
//! \return none
extern void DualRaySmokeAFE_App_GUIComm_SendTemperature(void);

//! \brief Sends the alarm status to the GUI
//!
//! \return none
extern void DualRaySmokeAFE_App_GUIComm_SendAlarmStatus(void);

//! \brief Sends the current configuration to GUI.
//!
//! \return none
extern void DualRaySmokeAFE_App_GUIComm_SendConfig(void);

#endif  //__DUALRAYSMOKEAFE_APP_H__
