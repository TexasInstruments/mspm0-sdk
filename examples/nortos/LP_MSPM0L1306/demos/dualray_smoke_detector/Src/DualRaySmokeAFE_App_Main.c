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

#include <stdbool.h>
#include <stdint.h>
#include <Src/include/DualRaySmokeAFELib.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include "DualRaySmokeAFE_App.h"

#include <Src/include/DualRaySmokeAFE_GUI.h>

// Structure with smoke detector data.
tDualRaySmokeAFELib_DualRayData sDualRaySmokeAFELib_Data;
// ID for AlarmOnPeriodicTimerCallback
//static uint16_t u16AlarmOnTimerCallbackID;
// ID for u16AlarmTimerCallbackID
static uint16_t u16AlarmTimerCallbackID;
// Counter used to keep track of periodic events
static uint16_t u16AlarmTimerCounter;
// Counter for Alarm active cycles (alarm is activated)
static uint16_t u16AlarmONCounter;

// Callback for push button to detect long and short presses
static bool TestButtonCallback(bool long_press);
// Callback for ULP Timer used to take new measurements
static bool ULPTimerCallback(void);
// Callback for Low-Power Periodic Timer used to keep track when alarm is ON.
static bool AlarmPeriodicTimerCallback();
// Callback for Low-Power Periodic Timer used to keep track when alarm is Silent.
//static bool AlarmSilentPeriodicTimerCallback();
// Initialization function
static inline int8_t AppInit(void);

void test(tDualRaySmokeAFE_GUI_TxCmd temp)
{
}
//! \brief Implements a smoke detector AFE demo.
//!         Initializes the system and libraries and stays in periodic loop
//!         performing measurements and detecting smoke.
//!         If smoke is detected, an alarm will be set.
//!
//! \return none
int main(void)
{
    int alarmDetectionStatus;
    tAlarmStatus alarm_status;

    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();


    // Initialize application
    if (AppInit() < 0) {
        // trap if there is an error during initialization
        while (1)
            ;
    }

#ifdef __ENABLE_GUI__
    // Send Configuration to GUI
    DualRaySmokeAFE_App_GUIComm_SendConfig();
#endif
    //

    sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_Idle;
    while (1) {
        // Check if any of the control flags, except for measurement are set
        if ((sDualRaySmokeAFE_AppConfig.APPCTL.all &
                APPCTL_STARTMEASUREMENT) != APPCTL_STARTMEASUREMENT) {
            if (sDualRaySmokeAFE_AppConfig.APPCTL.AppConfigUpdate == true) {
                // ***AppConfigUpdate:
                //      Set when the application configuration has changed.
                //      The only relevant change is the measurement interval
                //      configuration.
                //
                // clear flag
                sDualRaySmokeAFE_AppConfig.APPCTL.AppConfigUpdate = false;
                // Reconfigure Measurement rate
                DualRaySmokeAFE_App_Config_RestoreDefaultIntervalmsec();
            }  // if (sDualRaySmokeAFE_AppConfig.APPCTL.AppConfigUpdate == true)

            if (sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate == true) {
                // ***LibConfigUpdate:
                //      Set when the Library or HAL configuration has changed.
                //      Call DualRaySmokeAFELib_UpdateConfig to modify
                //      library parameters and HAL configuration.
                //
                // clear flag
                sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = false;

                // Reconfigure AFE and library
                DualRaySmokeAFELib_UpdateConfig(
                    &sDualRaySmokeAFELib_Config, &sDualRaySmokeAFE_HALConfig);

#ifdef __ENABLE_GUI__
                // Send Configuration to GUI
                DualRaySmokeAFE_App_GUIComm_SendConfig();
#endif
            }  // if (sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate == true)
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)

//            if (sDualRaySmokeAFE_AppConfig.APPCTL.StartCalibration == true)
//            {
//                // ***StartCalibration:
//                //      Set to trigger a ULP timer calibration.
//                //      Internal RTC using VLO can be calibrated periodically
//                //      using REFO.
//                //
//                // calibrate VLO every calibration interval time
//                // clear flag
//                sDualRaySmokeAFE_AppConfig.APPCTL.StartCalibration = false;
//
//                // Trigger Calibration
//                DualRaySmokeAFE_HAL_Timing_ULPTimer_calibrate();
//                // blink two fast cycles and one slow cycles to indicate VLO re-calibration
//                DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(4, 50);
//                DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(2, 500);
//            } // if (sDualRaySmokeAFE_AppConfig.APPCTL.StartCalibration == true)
#endif

#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)

            if (sDualRaySmokeAFE_AppConfig.APPCTL.StartTemperatureMeas ==
                true) {
                // ***StartTemperatureMeas:
                //      Triggers a new temperature measurement.
                //
                // Clear flag
                sDualRaySmokeAFE_AppConfig.APPCTL.StartTemperatureMeas = false;

                // Perform temperature measurement every u16TempSensorInterval
                DualRaySmokeAFELib_TemperatureMeasurement(
                    &sDualRaySmokeAFELib_Data);
#ifdef __ENABLE_GUI__
                if (sDualRaySmokeAFE_AppConfig.APPSTATUS.LPModeEnabled ==
                    false) {
                    // Send Temperature Data to GUI
                    DualRaySmokeAFE_App_GUIComm_SendTemperature();
                    __NOP();
                }
#endif
            }  // if (sDualRaySmokeAFE_AppConfig.APPCTL.StartTemperatureMeas == true)
#endif

            if (sDualRaySmokeAFE_AppConfig.APPCTL.StartAlarmTest == true) {
                // ***StartAlarmTest:
                //      Starts an alarm test, triggering the alarm.
                //
                sDualRaySmokeAFE_AppConfig.APPCTL.StartAlarmTest = false;
                if ((sDualRaySmokeAFE_AppConfig.APPSTATUS
                            .AlarmDetectionEnabled == true) &&
                    (sDualRaySmokeAFE_AppConfig.eAlarmStatus ==
                        AlarmStatus_Idle)) {
                    sDualRaySmokeAFE_AppConfig.eAlarmStatus =
                        AlarmStatus_TurnOn;
                    //sDualRaySmokeAFE_AppConfig.APPCTL.TurnAlarmOn = true;
                }
            }  // if (sDualRaySmokeAFE_AppConfig.APPCTL.StartAlarmTest == true)

            if (sDualRaySmokeAFE_AppConfig.APPCTL.TurnAlarmSilentOn == true) {
                // ***TurnAlarmSilentOn:
                //      Turns on silent mode if the alarm is active.
                //      Silent mode will ignore measurements for a predetermined
                //      amount of time.
                //
                sDualRaySmokeAFE_AppConfig.APPCTL.TurnAlarmSilentOn = false;
                if ((sDualRaySmokeAFE_AppConfig.eAlarmStatus ==
                        AlarmStatus_OnHigh) ||
                    (sDualRaySmokeAFE_AppConfig.eAlarmStatus ==
                        AlarmStatus_OnLow)) {
                    sDualRaySmokeAFE_AppConfig.eAlarmStatus =
                        AlarmStatus_TurnSilentOn;
                }
            }  // if (sDualRaySmokeAFE_AppConfig.APPCTL.TurnAlarmSilentOn == true)
        }  // if ((sDualRaySmokeAFE_AppConfig.APPCTL.all & APPCTL_STARTMEASUREMENT)

        alarm_status = sDualRaySmokeAFE_AppConfig.eAlarmStatus;
        // State machine for alarm status
        if (alarm_status >= AlarmStatus_Action) {
            switch (alarm_status) {
                case AlarmStatus_TurnOn:
                    // ***AlarmStatus_TurnOn:
                    //      Enable the alarm in High mode.
                    //      Alarm will sound if sounder is enabled.
                    //
                    sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmOn = true;
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
                    DualRaySmokeAFELib_setAlarmOn(
                        sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled,
                        true);
#else
                    DualRaySmokeAFELib_setAlarmOn(false, false);
#endif
                    DualRaySmokeAFE_HAL_IO_ClearLEDIndicator();
                    DualRaySmokeAFE_HAL_IO_SetAlarmOutputHigh();
                    u16AlarmTimerCounter = 0;  // Count cycles with alarm high
                    u16AlarmONCounter    = 0;  // Count cycles with alarm On
                    // Enable timer callback to keep track of time with alarm ON
                    DualRaySmokeAFE_HAL_Timing_LPPerTimer_enableCallback(
                        u16AlarmTimerCallbackID);
#ifdef __ENABLE_GUI__
                    // Send Alarm status to GUI
                    DualRaySmokeAFE_App_GUIComm_SendAlarmStatus();
#endif
                    alarm_status = AlarmStatus_OnHigh;
                    break;
                case AlarmStatus_Error:
                    // ***AlarmStatus_Error:
                    //      Error in state machine. Indicate and go to off.
                    //
                    DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(6, 250);
                    /* no break */
                case AlarmStatus_TurnSilentOn:
                    // ***AlarmStatus_TurnSilentOn:
                    //      Disable the alarm and wait u16PPerTimerCyclesAlarmSilent
                    //      to set the alarm in idle mode again.
                    //
                    /* no break */
                case AlarmStatus_TurnOff:
                    // ***AlarmStatus_TurnOff:
                    //      Disable the alarm and go to idle mode.
                    //
                    sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmOn = false;
                    DualRaySmokeAFE_App_Config_RestoreDefaultIntervalmsec();
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
                    DualRaySmokeAFELib_setAlarmOff(
                        sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled);
#else
                    DualRaySmokeAFELib_setAlarmOff(false);
#endif
                    DualRaySmokeAFE_HAL_IO_ClearLEDIndicator();
                    DualRaySmokeAFE_HAL_IO_SetAlarmOutputLow();
                    if (alarm_status == AlarmStatus_TurnSilentOn) {
                        sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmSilent =
                            true;
                        DualRaySmokeAFE_HAL_IO_SetLEDWarning();
                        u16AlarmTimerCounter =
                            0;  // Count number of cycles with Silence ON
                        // Enable timer callback to keep track of time with alarm silent
                        DualRaySmokeAFE_HAL_Timing_LPPerTimer_enableCallback(
                            u16AlarmTimerCallbackID);
                        alarm_status = AlarmStatus_Silent;
                    } else {
                        sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmSilent =
                            false;
                        DualRaySmokeAFE_HAL_IO_ClearLEDWarning();
                        // Disable timer callback
                        DualRaySmokeAFE_HAL_Timing_LPPerTimer_disableCallback(
                            u16AlarmTimerCallbackID);
                        alarm_status = AlarmStatus_Idle;
                    }
#ifdef __ENABLE_GUI__
                    // Send Alarm status to GUI
                    DualRaySmokeAFE_App_GUIComm_SendAlarmStatus();
#endif
                    break;

                default:
                    break;
            }
        }  // if (alarm_status >= AlarmStatus_Action)

        // Check if a new measurement should be taken
        if (sDualRaySmokeAFE_AppConfig.APPCTL.StartMeasurement == true) {
            // ***StartMeasurement:
            //      Set to start a new measurement of AFE.
            //      Dual-Ray and CO (if enabled)
            //
            // clear flag
            sDualRaySmokeAFE_AppConfig.APPCTL.StartMeasurement = false;

            // Perform Measurement
            DualRaySmokeAFELib_Measurement(&sDualRaySmokeAFELib_Data);
#ifdef __ENABLE_GUI__
            if (sDualRaySmokeAFE_AppConfig.APPSTATUS.LPModeEnabled == false) {
                __NOP();
                // Send Measurement Data to GUI
                DualRaySmokeAFE_App_GUIComm_SendAppData();
            }
#endif
            if (sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmDetectionEnabled ==
                true) {
                // Check result from running algorithm
                alarmDetectionStatus = DualRaySmokeAFELib_DetectionAlgorithm(
                    &sDualRaySmokeAFELib_Data);
            } else {
                alarmDetectionStatus = false;
            }

            __NOP();
            switch (alarm_status) {
                case AlarmStatus_Idle:
                    // ***AlarmStatus_Idle:
                    //      Takes measurements periodically and go to warning #1
                    //      if any threshold is exceeded.
                    //      Warning 1 measurements are taken at 1/4 the interval.
                    //
                    if (alarmDetectionStatus >
                        DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE) {
                        // sets the sample period 1/4*WAKEUP_INTERVAL
                        DualRaySmokeAFE_App_Config_SetTempMeasIntervalmsec(
                            sDualRaySmokeAFE_AppConfig.u16MeasIntervalms >> 2);
                        alarm_status = AlarmStatus_Warning1;
                    }
                    // 10ms LED toggle to indicate measurement
                    DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(2, 10);
                    break;
                case AlarmStatus_Warning1:
                    // ***AlarmStatus_Warning1:
                    //      If the threshold is still high, go to warning #2
                    //      taking measurements at 1/8 the interval.
                    //      If the treshold is low, go to idle.
                    //
                    if (alarmDetectionStatus ==
                        DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE) {
                        alarm_status = AlarmStatus_Idle;
                    } else if (alarmDetectionStatus >
                               DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE) {
                        // sets the sample period 1/8*WAKEUP_INTERVAL
                        DualRaySmokeAFE_App_Config_SetTempMeasIntervalmsec(
                            sDualRaySmokeAFE_AppConfig.u16MeasIntervalms >> 3);
                        alarm_status = AlarmStatus_Warning2;
                    }
                    // 10ms LED toggle to indicate measurement
                    DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(2, 10);
                    break;
                case AlarmStatus_Warning2:
                    // ***AlarmStatus_Warning2:
                    //      If the threshold is still high, turn on alarm
                    //      and set measurements at 1/16 the interval.
                    //      If the treshold is low, go to idle.
                    //
                    if (alarmDetectionStatus ==
                        DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE) {
                        alarm_status = AlarmStatus_Idle;
                        // 10ms LED toggle to indicate measurement
                        DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(2, 10);
                    } else if (alarmDetectionStatus >
                               DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE) {
                        // sets the sample period 1/16*WAKEUP_INTERVAL
                        DualRaySmokeAFE_App_Config_SetTempMeasIntervalmsec(
                            sDualRaySmokeAFE_AppConfig.u16MeasIntervalms >> 4);
                        alarm_status = AlarmStatus_TurnOn;
                    }
                    break;
                case AlarmStatus_OnLow:
                    // ***AlarmStatus_OnLow:
                    //      Takes a measurement when the alarm is active but
                    //      in a low cycle.
                    //      If the threshold is low, turn Off alarm.
                    //      If the threshold is high, keep alarm ON.
                    //
                    if (alarmDetectionStatus ==
                        DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE) {
                        alarm_status = AlarmStatus_TurnOff;
                    }
                    break;

                default:
                    break;
            }  // switch (alarm_status)

        }  //if (sDualRaySmokeAFE_AppConfig.APPCTL.StartMeasurement == true)

        sDualRaySmokeAFE_AppConfig.eAlarmStatus = alarm_status;

        // If no flags set, go to sleep
        if (sDualRaySmokeAFE_AppConfig.APPCTL.all == 0x00) {
            __NOP();
            if ((sDualRaySmokeAFE_AppConfig.eAlarmStatus ==
                    AlarmStatus_Silent) ||
                (sDualRaySmokeAFE_AppConfig.eAlarmStatus ==
                    AlarmStatus_OnHigh) ||
                (sDualRaySmokeAFE_AppConfig.eAlarmStatus ==
                    AlarmStatus_OnLow)) {

                // Go to LPM3 if the Low Power Timer is enabled
                // I.e. Alarm is ON or the test button was pressed
                __NOP();
                __WFI();
            } else if ((sDualRaySmokeAFE_AppConfig.eAlarmStatus >=
                           AlarmStatus_Idle) &&
                       (sDualRaySmokeAFE_AppConfig.eAlarmStatus <=
                           AlarmStatus_Warning2)) {
                // Go to LPM4 if Low Power Periodic Timer is disabled
                DualRaySmokeAFE_HAL_Timing_ULPTimer_Sleep();

                __NOP();
                __WFI();
            }
        }  // if (sDualRaySmokeAFE_AppConfig.APPCTL.all == 0x00)

    }  // while(1)
}

//! \brief Application initialization.
//!         Initializes the system and libraries.
//!
//! \return 0 if OK; -1 if an error occurred
static inline int8_t AppInit(void)
{
    // Initialize flags
    sDualRaySmokeAFE_AppConfig.APPCTL.all   = 0;
    sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_Inactive;

    // Initialize System
    DualRaySmokeAFE_HAL_System_Init();

    // Set a callback function for Test button
    DualRaySmokeAFE_HAL_IO_RegisterTestButtonCallback(TestButtonCallback);
    // Initialize Timing
    DualRaySmokeAFE_HAL_Timing_Init();
    // Initialize Smoke Library
    if (DualRaySmokeAFELib_Init(
            &sDualRaySmokeAFELib_Config, &sDualRaySmokeAFE_HALConfig) < 0) {
        DualRaySmokeAFE_HAL_IO_SetLEDIndicator();
        DualRaySmokeAFE_HAL_IO_SetAlarmOutputHigh();
        return -1;
    }

    // Initialize the Low Power Periodic Timer
    DualRaySmokeAFE_HAL_Timing_LPPerTimer_setIntervalms(
        sDualRaySmokeAFE_AppConfig.u16LPPerTimerIntervalms);
    // Register a callback function to keep timing when alarm is ON
    u16AlarmTimerCallbackID =
        DualRaySmokeAFE_HAL_Timing_LPPerTimer_registerCallback(
            AlarmPeriodicTimerCallback);
    // Set a callback function for the ultra-Low power timer
    DualRaySmokeAFE_HAL_Timing_ULPTimer_registerCallback(ULPTimerCallback);
    // Start ULP timer
    DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(
        sDualRaySmokeAFE_AppConfig.u16MeasIntervalms);
    DualRaySmokeAFE_HAL_Timing_ULPTimer_calibrate();

#ifdef __ENABLE_GUI__
    DualRaySmokeAFE_App_GUIComm_Init();
#endif
    // Toggle LED ON/OFF 3 times to indicate start
    DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(6, 100);

    // Check status of Low-Power enable pin
    // If High set low-power mode disabling GUI transmissions
    sDualRaySmokeAFE_AppConfig.APPSTATUS.LPModeEnabled =
        DualRaySmokeAFE_HAL_IO_GetLowPowerEnableStatus();

    u16AlarmTimerCounter = 0;
    u16AlarmONCounter    = 0;

    return 0;
}

//! \brief Callback function for Ultra Low Power Timer (RTC).
//!         Called by HAL ISR to trigger a new measurement.
//!
//! \return true to wake-up the MCU.
static bool ULPTimerCallback(void)
{
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
//    static uint16_t count_calibration = 0;
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    static uint16_t count_temperature = 0;
#endif

    if ((sDualRaySmokeAFE_AppConfig.eAlarmStatus >= AlarmStatus_Idle) &&
        (sDualRaySmokeAFE_AppConfig.eAlarmStatus <= AlarmStatus_Warning2)) {
        // Trigger a measurement if in idle or warning mode
        sDualRaySmokeAFE_AppConfig.APPCTL.StartMeasurement = true;
    } else {
        // Unexpected state, go to error
        sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_Error;
    }

#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
//    // If calibration is enabled (!=0), check for interval and set flag
//    if (sDualRaySmokeAFE_AppConfig.APPSTATUS.RTCCalibEnabled == true)
//    {
//        count_calibration++;                   // count for RTC interval
//        if (count_calibration >= sDualRaySmokeAFE_AppConfig.u16RTCCalibInterval)
//        {
//            sDualRaySmokeAFE_AppConfig.APPCTL.StartCalibration = true;
//            count_calibration = 0;
//        }
//    }
#endif

#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    if (sDualRaySmokeAFE_AppConfig.APPSTATUS.TemperatureMeasEnabled == true) {
        count_temperature++;  // count for temperature sensor
        if (count_temperature >=
            sDualRaySmokeAFE_AppConfig.u16TempSensorInterval) {
            sDualRaySmokeAFE_AppConfig.APPCTL.StartTemperatureMeas = true;
            count_temperature                                      = 0;
        }
    }
#endif

    // Return true to wake-up MCU
    return true;
}

//! \brief Callback function for Test button.
//!         Called by HAL ISR when test button is pressed.
//!
//! \param[in] long_press is true if a long press is detected.
//!             false if short press is detected.
//!
//! \return true to wake-up the MCU.
static bool TestButtonCallback(bool long_press)
{
    if (long_press == false) {
        // Short press, turn OFF alarm if enabled
        if ((sDualRaySmokeAFE_AppConfig.eAlarmStatus == AlarmStatus_OnHigh) ||
            (sDualRaySmokeAFE_AppConfig.eAlarmStatus == AlarmStatus_OnLow)) {
            sDualRaySmokeAFE_AppConfig.APPCTL.TurnAlarmSilentOn = true;
            return true;  // wake-up MCU
        }
    } else {
        // long press, enable alarm test
        if ((sDualRaySmokeAFE_AppConfig.eAlarmStatus >= AlarmStatus_Idle) &&
            (sDualRaySmokeAFE_AppConfig.eAlarmStatus <=
                AlarmStatus_Warning2)) {
            sDualRaySmokeAFE_AppConfig.APPCTL.StartAlarmTest = true;
            return true;  // wake-up MCU
        }
    }
    return false;
}

//! \brief Callback function for the alarm.
//!         Uses the Low-Power Periodic timer at an interval defined by
//!         sDualRaySmokeAFE_AppConfig.u16LPPerTimerIntervalms.
//!         Enabled by state machine to keep count of some events.
//!
//! \return true to wake-up the MCU.
static bool AlarmPeriodicTimerCallback(void)
{
    bool ret = false;
    switch (sDualRaySmokeAFE_AppConfig.eAlarmStatus) {
        case AlarmStatus_Silent:
            // Silent: Wait u16PPerTimerCyclesAlarmSilent to return alarm
            //           to idle mode.
            //
            if (u16AlarmTimerCounter++ >=
                sDualRaySmokeAFE_AppConfig.u16PPerTimerCyclesAlarmSilent) {
                sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_TurnOff;
                ret                                     = true;  // Wake-up MCU
            }
            break;
        case AlarmStatus_OnHigh:
            // On High: Wait u16LPPerTimerCyclesAlarmOnHigh cycles before going
            //          to On Low state.
            //
            if (u16AlarmTimerCounter++ >=
                sDualRaySmokeAFE_AppConfig.u16LPPerTimerCyclesAlarmOnHigh) {
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
                DualRaySmokeAFELib_setAlarmOn(
                    sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled, false);
#else
                DualRaySmokeAFELib_setAlarmOn(false, false);
#endif
                sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_OnLow;
                u16AlarmTimerCounter = 0;  // restart counter
            }
            break;
        case AlarmStatus_OnLow:
            // OnLow : Trigger a new measurement after the
            //          u16LPPerTimerCyclesBeforeAlarmOff expires
            //          Wait u16LPPerTimerCyclesAlarmOnLow cycles before going
            //          to Off High state.
            //
            if (u16AlarmTimerCounter++ == 0) {
                if (u16AlarmONCounter <
                    sDualRaySmokeAFE_AppConfig
                        .u16LPPerTimerCyclesBeforeAlarmOff) {
                    u16AlarmONCounter++;
                }
                if (u16AlarmONCounter >=
                    sDualRaySmokeAFE_AppConfig
                        .u16LPPerTimerCyclesBeforeAlarmOff) {
                    // Trigger a measurement after the wait period
                    sDualRaySmokeAFE_AppConfig.APPCTL.StartMeasurement = true;
                    ret = true;  // Wake-up MCU
                }
            } else if (u16AlarmTimerCounter++ >=
                       sDualRaySmokeAFE_AppConfig
                           .u16LPPerTimerCyclesAlarmOnLow) {
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
                DualRaySmokeAFELib_setAlarmOn(
                    sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled, true);
#else
                DualRaySmokeAFELib_setAlarmOn(false, true);
#endif
                sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_OnHigh;
                u16AlarmTimerCounter = 0;  // restart counter
            }
            break;

        default:
            break;
    }

    return ret;  // true to wake-up MCU, false to stay sleeping
}
