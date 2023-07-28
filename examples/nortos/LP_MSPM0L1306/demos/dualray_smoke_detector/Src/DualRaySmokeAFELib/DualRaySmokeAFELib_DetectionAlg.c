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

#include <Src/include/DualRaySmokeAFELib.h>
#include <Src/include/DualRaySmokeAFELib_Config.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>

/**** Local Variables *********************************************************/
// Pointer to HAL Configuration structure
tDualRaySmokeAFE_HAL_Config *pHALConfig;
// Pointer to Library Configuration structure
tDualRaySmokeAFELib_Config *pLibConfig;

/**** Functions ***************************************************************/
int8_t DualRaySmokeAFELib_Init(tDualRaySmokeAFELib_Config *LibConfig,
    tDualRaySmokeAFE_HAL_Config *HALConfig)
{
    return (DualRaySmokeAFELib_UpdateConfig(LibConfig, HALConfig));
}

int8_t DualRaySmokeAFELib_UpdateConfig(tDualRaySmokeAFELib_Config *LibConfig,
    tDualRaySmokeAFE_HAL_Config *HALConfig)
{
    pHALConfig = HALConfig;
    pLibConfig = LibConfig;
    if ((DualRaySmokeAFE_HAL_AFE_Init(pHALConfig->AFEConfig) < 0) ||
        (DualRaySmokeAFE_HAL_Sounder_Init(pHALConfig->sounderConfig) < 0)) {
        return -1;
    }

    return 0;
}

int8_t DualRaySmokeAFELib_DetectionAlgorithm(
    tDualRaySmokeAFELib_DualRayData *Data)
{
    uint8_t ret = DUALRAYSMOKEAFE_ALARM_DETECTION_FALSE;

    if (pLibConfig->bAlarmDetectionEnabled == false) {
        // Do not run the alarm detection algorithm if disabled
        return DUALRAYSMOKEAFE_ALARM_DETECTION_ERROR;
    }

#if (DUALRAYSMOKEAFELIB_ENABLE_INFRAREDLED_DETECTION != 0)
    // Check if photodiode detected light with infrared LED
    if (Data->sSmoke.i16Diff[DUALRAYSMOKEAFE_LED_RED] >
        pLibConfig->i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_RED]) {
        ret |= DUALRAYSMOKEAFE_ALARM_DETECTION_IR;
    }
#endif
#if (DUALRAYSMOKEAFELIB_ENABLE_BLUELED_DETECTION != 0)
    // Check if photodiode detected light with blue LED
    if (Data->sSmoke.i16Diff[DUALRAYSMOKEAFE_LED_BLUE] >
        pLibConfig->i16RefDelta_Threshold[DUALRAYSMOKEAFE_LED_BLUE]) {
        ret |= DUALRAYSMOKEAFE_ALARM_DETECTION_BLUE;
    }
#endif
    return ret;
}

void DualRaySmokeAFELib_setAlarmOn(bool sounderEn, bool sounderHigh)
{
    if (sounderEn == true) {
        if (sounderHigh == true) {
            DualRaySmokeAFE_HAL_Sounder_SetOn();
        } else {
            DualRaySmokeAFE_HAL_Sounder_SetOff();
        }
    }
}

void DualRaySmokeAFELib_setAlarmOff(bool sounderEn)
{
    if (sounderEn == true) {
        DualRaySmokeAFE_HAL_Sounder_SetOff();
    }
}
