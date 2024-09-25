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

#ifndef DRIVER_MSPM0LXX_BSP_H_
#define DRIVER_MSPM0LXX_BSP_H_

#include <Gauge_UserConfig/Gauge_UserConfig.h>
#include "Driver/MSPM0Lxx_Driver.h"

void BSP_BattInfoMeasure(
    tBattInfoInput* battInfoInput, tCurtCalcFactors* curtCalcFactors);
void BSP_DiffGainFactorCali(DL_OPA_GAIN opaGain, _iq15* pIq15R34toR3FactorTemp,
    uint8_t u8R34toR3CaliDacValue, uint8_t u8DefaultDacValue,
    tCurtCalcFactors* pCurtCalcFactors);
void BSP_CurtCalcFactorsCali(tCurtCalcFactors* pCurtCalcFactors);
void BSP_PeripheralsInit(void);
void BSP_SysTikCali(void);
#endif /* DRIVER_MSPM0LXX_BSP_H_ */
