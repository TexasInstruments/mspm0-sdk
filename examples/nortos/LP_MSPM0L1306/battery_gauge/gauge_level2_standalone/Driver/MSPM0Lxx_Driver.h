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

#ifndef DRIVER_MSPM0LXX_DRIVER_H_
#define DRIVER_MSPM0LXX_DRIVER_H_

#include <Gauge_UserConfig/Gauge_UserConfig.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/iqmath/include/IQmathLib.h>
#include "ti_msp_dl_config.h"

//**********************************Hardware configuration settings*********************************************//
#define VCC_2V_ADC_VALUE (1905)
#define VCC_3V_ADC_VALUE (2409)
#define VCELL_FACTOR \
    (3.200)  //Voltage detection resistor divider (22k+10k)/10k
#define SHUNT_RESISTOR_VALUE_R \
    (0.02)                   //20mR. Need to choose a low TCR resistor
#define INTERNAL_REF (1400)  //1400mV
#define NTC_PAIR_RESISTOR_VALUE (10000)  //10k
#define NTC_SHIFT (-40)
#define NTC_TABLE_LENGTH (126)

//**********************************Low level configuration settings*********************************************//
#define ADC_AVERAGE_TIMES (2)  //The total sample time is about 2.56ms
#define ADC_SAMPLE_TIME \
    (10)  //Max is 1023, only affect ADC sampling time, ADC conversion time is fixed
#define POWER_STARTUP_DELAY (16)

/* clang-format on */
void SYSCFG_DL_initPower(void);
void Driver_TempMeassure(void);
void Driver_BattVoltMeassure(void);
void Driver_UartInit(void);
void Driver_DACBufferSet(void);
void Driver_DiffAmpSet(void);
void Driver_DetectionPowerEnable(void);
void Driver_DetectionPowerDisable(void);
void Driver_VrefInit(void);
_iq5 Driver_ADCInitAndSampling(
    uint32_t adcChannel, uint32_t vrefSource, tStatus highAvg);
void Driver_DACInit(uint32_t u32DacValue);
void Driver_OPA0Init(void);
void Driver_OPA1Init(uint32_t u32Gain, tOPACali opaCali);
void Driver_UartTxU16Data(uint16_t u16Data);
void Driver_UartTxU32Data(uint32_t u32Data);
uint32_t Driver_UartRxU32Data(void);
uint16_t Driver_UartRxU16Data(void);
void Driver_TimerInit(void);
int16_t BSP_CalcuMcuTemp(void);
#endif /* DRIVER_MSPM0LXX_DRIVER_H_ */
