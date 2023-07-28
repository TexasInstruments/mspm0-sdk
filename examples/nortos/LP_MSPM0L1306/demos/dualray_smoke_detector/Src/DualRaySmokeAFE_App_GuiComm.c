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

#include <stdlib.h>
#include <string.h>
#include <Src/DualRaySmokeAFE_App.h>
#include <Src/include/DualRaySmokeAFELib_Config.h>
#ifdef __ENABLE_GUI__
#include <Src/include/DualRaySmokeAFE_GUI.h>
#endif
#include "DualRaySmokeAFE_App_Config.h"

static bool getSwitchValuefromString(char *string);

static void DualRaySmokeAFE_App_GUIComm_SendAppConfig(char *string);
static void DualRaySmokeAFE_App_GUIComm_RestoreDefaultAppConfig(char *string);
static void DualRaySmokeAFE_App_GUIComm_SendAlarmMode(void);
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
static void DualRaySmokeAFE_App_GUIComm_SendHornEn(void);
static void DualRaySmokeAFE_App_GUIComm_SendHornVol(void);
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
static void DualRaySmokeAFE_App_GUIComm_SendTempSensorMode(void);
#endif
static void DualRaySmokeAFE_App_GUIComm_SendLPEn(void);
static void DualRaySmokeAFE_App_GUIComm_SendSamplingPeriod(void);
static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDStrength(void);
static void DualRaySmokeAFE_App_GUIComm_SendRedREDStrength(void);
static void DualRaySmokeAFE_App_GUIComm_SendPGAGainRed(void);
static void DualRaySmokeAFE_App_GUIComm_SendPGAGainBlue(void);
static void DualRaySmokeAFE_App_GUIComm_SendBlueThreshold(void);
static void DualRaySmokeAFE_App_GUIComm_SendRedThreshold(void);
static void DualRaySmokeAFE_App_GUIComm_SendTIAEn(void);
static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDTempCoeff(void);
static void DualRaySmokeAFE_App_GUIComm_SendRedLEDTempCoeff(void);
static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDOnTime(void);
static void DualRaySmokeAFE_App_GUIComm_SendRedLEDOnTime(void);
static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDOffTime(void);
static void DualRaySmokeAFE_App_GUIComm_SendRedLEDOffTime(void);
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
static void DualRaySmokeAFE_App_GUIComm_SendCOMode(void);
static void DualRaySmokeAFE_App_GUIComm_SendCOAmpEn(void);
static void DualRaySmokeAFE_App_GUIComm_SendCOGain(void);
#endif
static void DualRaySmokeAFE_App_GUIComm_SendLEDMeasurementOverhead(void);

static void GUIComm_SendBoolean(char *cmd, bool flag);
static void GUIComm_SendInt(char *cmd, int16_t val);
static void GUIComm_SendFloat(char *cmd, float val);

static void GUICmdUpdateSamplingPeriod(char *string);
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
static void GUICmdUpdateTempSensorEnable(char *string);
#endif
static void GUICmdUpdateLPEn(char *string);
static void GUICmdUpdateAlarmEn(char *string);
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
static void GUICmdUpdateHornEn(char *string);
static void GUICmdUpdateHornVolume(char *string);
#endif
static void GUICmdUpdateBlueLEDStrength(char *string);
static void GUICmdUpdateRedLEDStrength(char *string);
static void GUICmdUpdatePGAGainRed(char *string);
static void GUICmdUpdatePGAGainBlue(char *string);
static void GUICmdUpdateBlueThreshold(char *string);
static void GUICmdUpdateRedThreshold(char *string);
static void GUICmdUpdateTIAEn(char *string);
static void GUICmdUpdateBlueLEDTempCoeff(char *string);
static void GUICmdUpdateRedLEDTempCoeff(char *string);
static void GUICmdUpdateBlueLEDOnTime(char *string);
static void GUICmdUpdateRedLEDOnTime(char *string);
static void GUICmdUpdateBlueLEDOffTime(char *string);
static void GUICmdUpdateRedLEDOffTime(char *string);

#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
static void GUICmdUpdateCOEn(char *string);
static void GUICmdUpdateCOAmpEn(char *string);
static void GUICmdUpdateCOGain(char *string);
#endif

static char cATXString[20];

#ifdef __ENABLE_GUI__
//! \brief RX Command structure.
//!         The corresponding callback will be called when the command is
//!         received from GUI.
static const tDualRaySmokeAFE_GUI_RxCmd GUI_RXCommands[] = {
    {"bReadConfig", DualRaySmokeAFE_App_GUIComm_SendAppConfig},
    {"bRestoreConfig", DualRaySmokeAFE_App_GUIComm_RestoreDefaultAppConfig},
    {"tSPt", GUICmdUpdateSamplingPeriod},
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    {"TSEnt", GUICmdUpdateTempSensorEnable},
#endif
    {"LPEnt", GUICmdUpdateLPEn},
    {"alEnt", GUICmdUpdateAlarmEn},
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
    {"hoEnt", GUICmdUpdateHornEn},
    {"SVt", GUICmdUpdateHornVolume},
#endif
    {"bLSt", GUICmdUpdateBlueLEDStrength},
    {"irLSt", GUICmdUpdateRedLEDStrength},
    {"bPGAt", GUICmdUpdatePGAGainBlue},
    {"irPGAt", GUICmdUpdatePGAGainRed},
    {"bThrt", GUICmdUpdateBlueThreshold},
    {"irThrt", GUICmdUpdateRedThreshold},
    {"TIAEnt", GUICmdUpdateTIAEn},
    {"bTeCot", GUICmdUpdateBlueLEDTempCoeff},
    {"irTeCot", GUICmdUpdateRedLEDTempCoeff},
    {"bONt", GUICmdUpdateBlueLEDOnTime},
    {"irONt", GUICmdUpdateRedLEDOnTime},
    {"bOFFt", GUICmdUpdateBlueLEDOffTime},
    {"irOFFt", GUICmdUpdateRedLEDOffTime},
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
    {"COEnt", GUICmdUpdateCOEn},
    {"CoAEnt", GUICmdUpdateCOAmpEn},
    {"COGt", GUICmdUpdateCOGain},
#endif
};

//! \brief TX Command structure.
//!         The device will all commands and corresponding formatted data
//!         when DualRaySmokeAFE_App_GUIComm_SendAppData is called.
/*static */ const tDualRaySmokeAFE_GUI_TxCmd GUI_TXCmdData[] = {
    {"{\"dR\":%d}\n",
        (void *) &(
            sDualRaySmokeAFELib_Data.sSmoke.u16Dark[DUALRAYSMOKEAFE_LED_RED])},
    {"{\"lR\":%d}\n", (void *) &(sDualRaySmokeAFELib_Data.sSmoke
                                     .u16Light[DUALRAYSMOKEAFE_LED_RED])},
    {"{\"fR\":%d}\n",
        (void *) &(
            sDualRaySmokeAFELib_Data.sSmoke.i16Diff[DUALRAYSMOKEAFE_LED_RED])},
    {"{\"dB\":%d}\n", (void *) &(sDualRaySmokeAFELib_Data.sSmoke
                                     .u16Dark[DUALRAYSMOKEAFE_LED_BLUE])},
    {"{\"lB\":%d}\n", (void *) &(sDualRaySmokeAFELib_Data.sSmoke
                                     .u16Light[DUALRAYSMOKEAFE_LED_BLUE])},
    {"{\"fB\":%d}\n", (void *) &(sDualRaySmokeAFELib_Data.sSmoke
                                     .i16Diff[DUALRAYSMOKEAFE_LED_BLUE])},
    {"{\"iB\":%d}\n",
        (void *) &(sDualRaySmokeAFELib_Data.sSmoke
                       .u16LEDCurrent[DUALRAYSMOKEAFE_LED_BLUE])},
    {"{\"iR\":%d}\n", (void *) &(sDualRaySmokeAFELib_Data.sSmoke
                                     .u16LEDCurrent[DUALRAYSMOKEAFE_LED_RED])},
#if ((DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1) && \
     (DUALRAYSMOKE_APP_CO_SUPPORTED == 1))
    {"{\"CO\":%d}\n", (void *) &(sDualRaySmokeAFELib_Data.sSmoke.u16CO)},
#endif
};

void DualRaySmokeAFE_App_GUIComm_Init(void)
{
    DualRaySmokeAFE_GUI_Init(&GUI_RXCommands[0],
        sizeof(GUI_RXCommands) / sizeof(GUI_RXCommands[0]));
}

void DualRaySmokeAFE_App_GUIComm_SendAppData(void)
{
    DualRaySmokeAFE_GUI_TransmitData(
        &GUI_TXCmdData[0], sizeof(GUI_TXCmdData) / sizeof(GUI_TXCmdData[0]));
}

void DualRaySmokeAFE_App_GUIComm_SendTemperature(void)
{
    //TODO Not sure if I can just cast number to a float or not...
#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1)
    GUIComm_SendFloat("tempC",
        (float) (sDualRaySmokeAFELib_Data.sTemperature.iq15TemperatureIntC));
#endif
#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1)
    GUIComm_SendFloat("etempC",
        (float) (sDualRaySmokeAFELib_Data.sTemperature.iq15TemperatureExtC));
#endif
}

void DualRaySmokeAFE_App_GUIComm_SendConfig(void)
{
    // Send the configuration
    DualRaySmokeAFE_App_GUIComm_SendAlarmStatus();
    DualRaySmokeAFE_App_GUIComm_SendAlarmMode();
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
    DualRaySmokeAFE_App_GUIComm_SendHornEn();
    DualRaySmokeAFE_App_GUIComm_SendHornVol();
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    DualRaySmokeAFE_App_GUIComm_SendTempSensorMode();
#endif
    DualRaySmokeAFE_App_GUIComm_SendLPEn();
    DualRaySmokeAFE_App_GUIComm_SendSamplingPeriod();
    DualRaySmokeAFE_App_GUIComm_SendBlueLEDStrength();
    DualRaySmokeAFE_App_GUIComm_SendRedREDStrength();
    DualRaySmokeAFE_App_GUIComm_SendPGAGainRed();
    DualRaySmokeAFE_App_GUIComm_SendPGAGainBlue();
    DualRaySmokeAFE_App_GUIComm_SendBlueThreshold();
    DualRaySmokeAFE_App_GUIComm_SendRedThreshold();
    DualRaySmokeAFE_App_GUIComm_SendTIAEn();
    DualRaySmokeAFE_App_GUIComm_SendBlueLEDTempCoeff();
    DualRaySmokeAFE_App_GUIComm_SendRedLEDTempCoeff();
    DualRaySmokeAFE_App_GUIComm_SendBlueLEDOnTime();
    DualRaySmokeAFE_App_GUIComm_SendRedLEDOnTime();
    DualRaySmokeAFE_App_GUIComm_SendBlueLEDOffTime();
    DualRaySmokeAFE_App_GUIComm_SendRedLEDOffTime();
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
    DualRaySmokeAFE_App_GUIComm_SendCOMode();
    DualRaySmokeAFE_App_GUIComm_SendCOAmpEn();
    DualRaySmokeAFE_App_GUIComm_SendCOGain();
#endif
    DualRaySmokeAFE_App_GUIComm_SendLEDMeasurementOverhead();
}

void DualRaySmokeAFE_App_GUIComm_SendAlarmStatus(void)
{
    GUIComm_SendBoolean("aSt", sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmOn);
    GUIComm_SendBoolean(
        "aQt", sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmSilent);
}

//  ***** Local Functions to Send Data to GUI *****     //

static void DualRaySmokeAFE_App_GUIComm_SendAppConfig(char *string)
{
    // Set flag so that configuration is sent in main loop
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void DualRaySmokeAFE_App_GUIComm_RestoreDefaultAppConfig(char *string)
{
    sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmOn =
        DUALRAYSMOKEAFE_APP_DEFAULT_ALARMON;
    sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmDetectionEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_ALARMDETECTIONENABLED;
#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
    sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_HORNENABLED;
    sDualRaySmokeAFE_HALConfig.sounderConfig->u16Volume =
        DUALRAYSMOKEAFELIB_DEFAULT_SOUNDERVOLUME;
#endif
#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
    sDualRaySmokeAFE_AppConfig.APPSTATUS.TemperatureMeasEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_TEMPERATUREMEASENABLED;
#endif
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
    sDualRaySmokeAFE_AppConfig.APPSTATUS.COEnabled =
        DUALRAYSMOKEAFE_APP_DEFAULT_COENABLED;
    sDualRaySmokeAFE_HALConfig.AFEConfig->bCOAmpEn =
        DUALRAYSMOKEAFELIB_DEFAULT_CO_AMP_EN;
    sDualRaySmokeAFE_HALConfig.AFEConfig->u16COGain =
        DUALRAYSMOKEAFELIB_DEFAULT_CO_GAIN_KOHM;
#endif
    sDualRaySmokeAFE_HALConfig.AFEConfig->bPDTIAEn =
        DUALRAYSMOKEAFELIB_DEFAULT_PD_TIA_EN;
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms =
        DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC;
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor =
        DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_Strength[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDSTRENGTH;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_Strength[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDSTRENGTH;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16PGA_Gain[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_PGAGAIN;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16PGA_Gain[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_PGAGAIN;
    sDualRaySmokeAFELib_Config
        .i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_THRESHOLD;
    sDualRaySmokeAFELib_Config.i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_THRESHOLD;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDTEMPCOEFF;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDTEMPCOEFF;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDHOLDON_US;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDHOLDON_US;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_BLUE] =
        DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDHOLDOFF_US;
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_RED] =
        DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDHOLDOFF_US;

    // Update Configuration
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms =
        DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC;
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor =
        DUALRAYSMOKEAFE_APP_DEFAULT_ULPTIMER_INTERVAL_MSEC;
    // Set flag so that configuration is updated and sent to GUI in main loop
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
    sDualRaySmokeAFE_AppConfig.APPCTL.AppConfigUpdate = true;
}

static void DualRaySmokeAFE_App_GUIComm_SendAlarmMode(void)
{
    GUIComm_SendBoolean(
        "alEnt", sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmDetectionEnabled);
}

#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
static void DualRaySmokeAFE_App_GUIComm_SendHornEn(void)
{
    GUIComm_SendBoolean(
        "hoEnt", sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled);
}

static void DualRaySmokeAFE_App_GUIComm_SendHornVol(void)
{
    GUIComm_SendInt(
        "SVt", sDualRaySmokeAFE_HALConfig.sounderConfig->u16Volume);
}

#endif

#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
static void DualRaySmokeAFE_App_GUIComm_SendTempSensorMode(void)
{
    GUIComm_SendBoolean(
        "TSEnt", sDualRaySmokeAFE_AppConfig.APPSTATUS.TemperatureMeasEnabled);
}
#endif

static void DualRaySmokeAFE_App_GUIComm_SendLPEn(void)
{
    GUIComm_SendBoolean(
        "LPEnt", sDualRaySmokeAFE_AppConfig.APPSTATUS.LPModeEnabled);
}

static void DualRaySmokeAFE_App_GUIComm_SendSamplingPeriod(void)
{
    GUIComm_SendInt("tSPt", sDualRaySmokeAFE_AppConfig.u16MeasIntervalms);
}

static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDStrength(void)
{
    GUIComm_SendInt("bLSt", sDualRaySmokeAFE_HALConfig.AFEConfig
                                ->u16LED_Strength[DUALRAYSMOKEAFE_LED_BLUE]);
}

static void DualRaySmokeAFE_App_GUIComm_SendRedREDStrength(void)
{
    GUIComm_SendInt("irLSt", sDualRaySmokeAFE_HALConfig.AFEConfig
                                 ->u16LED_Strength[DUALRAYSMOKEAFE_LED_RED]);
}

static void DualRaySmokeAFE_App_GUIComm_SendPGAGainRed(void)
{
    GUIComm_SendInt("irPGAt", sDualRaySmokeAFE_HALConfig.AFEConfig
                                  ->u16PGA_Gain[DUALRAYSMOKEAFE_LED_RED]);
}

static void DualRaySmokeAFE_App_GUIComm_SendPGAGainBlue(void)
{
    GUIComm_SendInt("bPGAt", sDualRaySmokeAFE_HALConfig.AFEConfig
                                 ->u16PGA_Gain[DUALRAYSMOKEAFE_LED_BLUE]);
}

static void DualRaySmokeAFE_App_GUIComm_SendBlueThreshold(void)
{
    GUIComm_SendInt(
        "bThrt", sDualRaySmokeAFELib_Config
                     .i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_BLUE]);
}

static void DualRaySmokeAFE_App_GUIComm_SendRedThreshold(void)
{
    GUIComm_SendInt(
        "irThrt", sDualRaySmokeAFELib_Config
                      .i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_RED]);
}

static void DualRaySmokeAFE_App_GUIComm_SendTIAEn(void)
{
    GUIComm_SendBoolean(
        "TIAEnt", sDualRaySmokeAFE_HALConfig.AFEConfig->bPDTIAEn);
}

static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDTempCoeff(void)
{
    GUIComm_SendInt(
        "bTeCot", sDualRaySmokeAFE_HALConfig.AFEConfig
                      ->u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_BLUE]);
}

static void DualRaySmokeAFE_App_GUIComm_SendRedLEDTempCoeff(void)
{
    GUIComm_SendInt(
        "irTeCot", sDualRaySmokeAFE_HALConfig.AFEConfig
                       ->u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_RED]);
}

static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDOnTime(void)
{
    // ON time includes the overhead + ON time
    GUIComm_SendInt("bONt",
        sDualRaySmokeAFE_HALConfig.AFEConfig
                ->u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_BLUE] +
            sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
}

static void DualRaySmokeAFE_App_GUIComm_SendRedLEDOnTime(void)
{
    // ON time includes the overhead + ON time
    GUIComm_SendInt("irONt",
        sDualRaySmokeAFE_HALConfig.AFEConfig
                ->u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_RED] +
            sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
}

static void DualRaySmokeAFE_App_GUIComm_SendBlueLEDOffTime(void)
{
    // OFF time includes the overhead + ON time
    GUIComm_SendInt("bOFFt",
        sDualRaySmokeAFE_HALConfig.AFEConfig
                ->u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_BLUE] +
            sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
}

static void DualRaySmokeAFE_App_GUIComm_SendRedLEDOffTime(void)
{
    // OFF time includes the overhead + ON time
    GUIComm_SendInt("irOFFt",
        sDualRaySmokeAFE_HALConfig.AFEConfig
                ->u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_RED] +
            sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
}
#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
static void DualRaySmokeAFE_App_GUIComm_SendCOMode(void)
{
    GUIComm_SendBoolean(
        "COEnt", sDualRaySmokeAFE_AppConfig.APPSTATUS.COEnabled);
}

static void DualRaySmokeAFE_App_GUIComm_SendCOAmpEn(void)
{
    GUIComm_SendBoolean(
        "CoAEnt", sDualRaySmokeAFE_HALConfig.AFEConfig->bCOAmpEn);
}

static void DualRaySmokeAFE_App_GUIComm_SendCOGain(void)
{
    GUIComm_SendInt("COGt", sDualRaySmokeAFE_HALConfig.AFEConfig->u16COGain);
}

#endif

static void DualRaySmokeAFE_App_GUIComm_SendLEDMeasurementOverhead(void)
{
    GUIComm_SendInt(
        "MOt", sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
}

static void GUIComm_SendBoolean(char *cmd, bool flag)
{
    if (flag == true) {
        sprintf(cATXString, "{\"%s\":%s}\n", cmd, "true");
    } else {
        sprintf(cATXString, "{\"%s\":%s}\n", cmd, "false");
    }
    DualRaySmokeAFE_GUI_TransmitStringBlocking(cATXString);
}

static void GUIComm_SendInt(char *cmd, int16_t val)
{
    sprintf(cATXString, "{\"%s\":%d}\n", cmd, val);
    DualRaySmokeAFE_GUI_TransmitStringBlocking(cATXString);
}

static void GUIComm_SendFloat(char *cmd, float val)
{
    sprintf(cATXString, "{\"%s\":%.2f}\n", cmd, val);
    DualRaySmokeAFE_GUI_TransmitStringBlocking(cATXString);
}

//  ***** Local Functions to get Data from GUI and update App config *****     //
static void GUICmdUpdateSamplingPeriod(char *string)
{
    uint16_t interval;

    interval                                     = strtol(string, NULL, 10);
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms = interval;
    sDualRaySmokeAFE_AppConfig.u16MeasIntervalms_tempor = interval;
    sDualRaySmokeAFE_AppConfig.APPCTL.AppConfigUpdate   = true;
}

#if (DUALRAYSMOKE_APP_TEMPERATURE_SUPPORTED == 1)
static void GUICmdUpdateTempSensorEnable(char *string)
{
    bool flag;

    flag = getSwitchValuefromString(string);
    sDualRaySmokeAFE_AppConfig.APPSTATUS.TemperatureMeasEnabled = flag;
}
#endif

static void GUICmdUpdateLPEn(char *string)
{
    bool flag;

    flag = getSwitchValuefromString(string);
    sDualRaySmokeAFE_AppConfig.APPSTATUS.LPModeEnabled = flag;
}

static void GUICmdUpdateAlarmEn(char *string)
{
    bool flag;

    flag = getSwitchValuefromString(string);
    // Turn Off Horn if alarm was active
    if ((flag == false) &&
        ((sDualRaySmokeAFE_AppConfig.eAlarmStatus == AlarmStatus_OnHigh) ||
            (sDualRaySmokeAFE_AppConfig.eAlarmStatus == AlarmStatus_OnLow))) {
        sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_TurnSilentOn;
    }

    sDualRaySmokeAFE_AppConfig.APPSTATUS.AlarmDetectionEnabled = flag;
}

#if (DUALRAYSMOKE_APP_HORN_SUPPORTED == 1)
static void GUICmdUpdateHornEn(char *string)
{
    bool flag;

    // Turn Off Horn if alarm was active
    flag = getSwitchValuefromString(string);
    if ((flag == false) &&
        ((sDualRaySmokeAFE_AppConfig.eAlarmStatus == AlarmStatus_OnHigh) ||
            (sDualRaySmokeAFE_AppConfig.eAlarmStatus == AlarmStatus_OnLow))) {
        sDualRaySmokeAFE_AppConfig.eAlarmStatus = AlarmStatus_TurnSilentOn;
    }
    sDualRaySmokeAFE_AppConfig.APPSTATUS.HornEnabled = flag;
}

static void GUICmdUpdateHornVolume(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.sounderConfig->u16Volume = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate   = true;
}

#endif

static void GUICmdUpdateBlueLEDStrength(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_Strength[DUALRAYSMOKEAFE_LED_BLUE]   = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateRedLEDStrength(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_Strength[DUALRAYSMOKEAFE_LED_RED]    = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdatePGAGainRed(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16PGA_Gain[DUALRAYSMOKEAFE_LED_RED]        = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdatePGAGainBlue(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16PGA_Gain[DUALRAYSMOKEAFE_LED_BLUE]       = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateBlueThreshold(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFELib_Config
        .i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_BLUE] = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate    = true;
}

static void GUICmdUpdateRedThreshold(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFELib_Config.i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_RED] =
        value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateTIAEn(char *string)
{
    bool flag;

    flag = getSwitchValuefromString(string);
    sDualRaySmokeAFE_HALConfig.AFEConfig->bPDTIAEn    = flag;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateBlueLEDTempCoeff(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_BLUE]  = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateRedLEDTempCoeff(char *string)
{
    uint16_t value;

    value = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_RED]   = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateBlueLEDOnTime(char *string)
{
    uint16_t value;

    // GUI sends the total ON time
    value = strtol(string, NULL, 10);

    if (value < sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus) {
        // Minimum time will be the overhead
        value = sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus;
    }
    // Extra on time
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_BLUE] =
        (value - sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateRedLEDOnTime(char *string)
{
    uint16_t value;

    // GUI sends the total ON time
    value = strtol(string, NULL, 10);

    if (value < sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus) {
        // Minimum time will be the overhead
        value = sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus;
    }
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_RED] =
        (value - sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateBlueLEDOffTime(char *string)
{
    uint16_t value;

    // GUI sends the total ON time
    value = strtol(string, NULL, 10);

    if (value < sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus) {
        // Minimum time will be the overhead
        value = sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus;
    }
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_BLUE] =
        (value - sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateRedLEDOffTime(char *string)
{
    uint16_t value;

    // GUI sends the total ON time
    value = strtol(string, NULL, 10);

    if (value < sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus) {
        // Minimum time will be the overhead
        value = sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus;
    }
    sDualRaySmokeAFE_HALConfig.AFEConfig
        ->u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_RED] =
        (value - sDualRaySmokeAFE_HALConfig.AFEConfig->u16LEDMeasOverheadus);
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

#if (DUALRAYSMOKE_APP_CO_SUPPORTED == 1)
static void GUICmdUpdateCOEn(char *string)
{
    bool flag;

    flag = getSwitchValuefromString(string);
    sDualRaySmokeAFE_AppConfig.APPSTATUS.COEnabled    = flag;
    sDualRaySmokeAFE_HALConfig.AFEConfig->bCOMeasEn   = flag;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateCOAmpEn(char *string)
{
    bool flag;

    flag = getSwitchValuefromString(string);
    sDualRaySmokeAFE_HALConfig.AFEConfig->bCOAmpEn    = flag;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}

static void GUICmdUpdateCOGain(char *string)
{
    uint16_t value;

    value                                           = strtol(string, NULL, 10);
    sDualRaySmokeAFE_HALConfig.AFEConfig->u16COGain = value;
    sDualRaySmokeAFE_AppConfig.APPCTL.LibConfigUpdate = true;
}
#endif  // DUALRAYSMOKE_APP_CO_SUPPORTED

static bool getSwitchValuefromString(char *string)
{
    if (strncmp(string, "true", 4) == 0) {
        return true;
    } else {
        return false;
    }
}

#endif  // __ENABLE_GUI__
