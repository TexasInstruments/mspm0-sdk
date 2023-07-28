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

#include "DualRaySmokeAFE_App_Config.h"
#include <Src/include/DualRaySmokeAFELib.h>
#include <Src/include/DualRaySmokeAFE_Config_Common.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include <stdlib.h>
#include <string.h>
#include "DualRaySmokeAFE_App.h"

#define __SW_VERSION__ 0x01, 0x00, 0x02, 0x02
#ifdef __AFE_SACL3__
#define __EVM430_ID__ 0x01, 0x00  // FR2355 Dual Ray Demo board v0.5
#elif defined(__AFE_TPS880X__)
#define __EVM430_ID__ 0x02, 0x01  // TPS880x EVM v1.0
#else
#error "Hardware ID not defined."
#endif
#ifdef __MSPM0L1306__
#define __DEV_ID__ \
    0x23, 0x55  //TODO figure out what these numbers mean.  Legacy from FR2355
#else
#error "Device ID not defined."
#endif

#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(app_version, ".version")
#pragma RETAIN(app_version)
const uint8_t app_version[8] = {__SW_VERSION__, __EVM430_ID__, __DEV_ID__};
#endif

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(sDualRaySmokeAFELib_Config)
#pragma PERSISTENT(sDualRaySmokeAFE_AppConfig)
#pragma PERSISTENT(sDualRaySmokeAFE_HAL_AFE_Config)
#pragma PERSISTENT(sDualRaySmokeAFE_HAL_Sounder_Config)
#pragma PERSISTENT(sDualRaySmokeAFE_HALConfig)
#endif

tDualRaySmokeAFE_AppConfig sDualRaySmokeAFE_AppConfig = {
    .u16MeasIntervalms = DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC,
    .u16MeasIntervalms_tempor =
        DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC,
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    .u16TempSensorInterval =
        DUALRAYSMOKEAFE_APP_DEFAULT_TEMPERATURESENSOR_INTERVAL,
#endif
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
    .u16RTCCalibInterval =
        DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_CALIBRATION_INTERVAL,
#endif
    .u16LPPerTimerIntervalms =
        DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC,
    .u16LPPerTimerCyclesAlarmOnHigh =
        ((DUALRAYSMOKEAFE_APP_ALARM_ONHIGH_MS) /
            DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC),
    .u16LPPerTimerCyclesAlarmOnLow =
        ((DUALRAYSMOKEAFE_APP_ALARM_ONLOW_MS) /
            DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC),
    .u16LPPerTimerCyclesBeforeAlarmOff =
        DUALRAYSMOKEAFE_APP_ALARM_ONTOOFF_CYCLES,
    .u16PPerTimerCyclesAlarmSilent =
        ((DUALRAYSMOKEAFE_APP_ALARM_SILENT_SEC * 1000) /
            DUALRAYSMOKEAFE_APP_DEFAULT_LPPERTIMER_INTERVAL_MSEC),
    .APPSTATUS.AlarmOn =
        DUALRAYSMOKEAFE_APP_DEFAULT_ALARMON,  // @suppress("Symbol is not resolved")
    .APPSTATUS.AlarmDetectionEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_ALARMDETECTIONENABLED,  // @suppress("Symbol is not resolved")
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
    .APPSTATUS.HornEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_HORNENABLED,  // @suppress("Symbol is not resolved")
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    .APPSTATUS.TemperatureMeasEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_TEMPERATUREMEASENABLED,  // @suppress("Symbol is not resolved")
#endif
#if (DUALRAYSMOKE_APP_ULP_CALIBRATION_SUPPORTED == 1)
    .APPSTATUS.RTCCalibEnabled = DUALRAYSMOKEAFE_APP_DEFAULT_RTCCALIBENABLED,
#endif
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
    .APPSTATUS.COEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_COENABLED,  // @suppress("Symbol is not resolved")

#endif
    // Note: .APPSTATUS.LPModeEnabled default state is selected by LPEnable pin
    //         using DualRaySmokeAFE_HAL_IO_GetLowPowerEnableStatus()
    .APPCTL.all = 0,  // @suppress("Symbol is not resolved")
};

tDualRaySmokeAFELib_Config sDualRaySmokeAFELib_Config = {
    .bAlarmDetectionEnabled = true,
    .i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_THRESHOLD,
    .i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_THRESHOLD,
};

tDualRaySmokeAFE_HAL_AFE_Config sDualRaySmokeAFE_HAL_AFE_Config = {
    .u16LED_Strength[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDSTRENGTH,
    .u16LED_Strength[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDSTRENGTH,
    .u16PGA_Gain[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_PGAGAIN,
    .u16PGA_Gain[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_PGAGAIN,
    .bPDTIAEn = DUALRAYSMOKEAFELIB_DEFAULT_PD_TIA_EN,
    .u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDHOLDON_US,
    .u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDHOLDON_US,
    .u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDHOLDOFF_US,
    .u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDHOLDOFF_US,
#ifdef __AFE_TPS880X__
    .u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDTEMPCOEFF,
    .u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDTEMPCOEFF,
#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
    .u16COGain = DUALRAYSMOKEAFELIB_DEFAULT_CO_GAIN_KOHM,
    .bCOAmpEn  = DUALRAYSMOKEAFELIB_DEFAULT_CO_AMP_EN,
    .bCOMeasEn = DUALRAYSMOKEAFELIB_DEFAULT_CO_MEAS_EN,
#endif  // DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS
#endif  // __AFE_TPS880X__

#ifdef __AFE_SACL3__
    .u16TIABiasmV[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_TIABIASMV,
    .u16TIABiasmV[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_TIABIASMV,
    .u16PGABiasmV[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_PGABIASMV,
    .u16PGABiasmV[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_PGABIASMV,
#endif
};

tDualRaySmokeAFE_HAL_Sounder_Config sDualRaySmokeAFE_HAL_Sounder_Config = {
    .u16Freq = DUALRAYSMOKEAFELIB_DEFAULT_SOUNDERFREQ_HZ,
#ifdef __AFE_TPS880X__
    .u16Volume = DUALRAYSMOKEAFELIB_DEFAULT_SOUNDERVOLUME,
#endif
};

tDualRaySmokeAFE_HAL_Config sDualRaySmokeAFE_HALConfig = {
    .AFEConfig     = &sDualRaySmokeAFE_HAL_AFE_Config,
    .sounderConfig = &sDualRaySmokeAFE_HAL_Sounder_Config,
};

void DualRaySmokeAFE_App_Config_SetMeasIntervalmsec(uint16_t interval)
{
    // Update measurement intervals
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms        = interval;
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor = interval;

    // Update RTC
    DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(
        sDualRaySmokeAFE_AppConfig.u16MeasIntervalms);
}

void DualRaySmokeAFE_App_Config_SetTempMeasIntervalmsec(uint16_t interval)
{
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor = interval;
    DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(
        sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor);
}

void DualRaySmokeAFE_App_Config_RestoreDefaultIntervalmsec(void)
{
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor =
        sDualRaySmokeAFE_AppConfig.u16MeasIntervalms;
    DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(
        sDualRaySmokeAFE_AppConfig.u16MeasIntervalms);
}
