/*
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
 */

#ifndef GAUGE_ALGORITHM_GAUGE_TYPE_H_
#define GAUGE_ALGORITHM_GAUGE_TYPE_H_

#include <stdbool.h>
#include <stdint.h>
#include <ti/iqmath/include/IQmathLib.h>

#define NO_OUTPUT (0)
#define UART_OUTPUT (1)

//*****************************************************************************
// Describe OPA test mode
//*****************************************************************************
enum {
    eNormalMode = 0,
    eOffsetCali = 1,
};
typedef uint8_t tOPACali;

//*****************************************************************************
// Describe the status
//*****************************************************************************
enum {
    eFalse = 0,
    eTrue  = 1,
};
typedef uint8_t tStatus;

//*****************************************************************************
// Describe the algorithm one running cycle time
//*****************************************************************************
enum {
    eSystemTik_32000MS = 64,
    eSystemTik_16000MS = 32,
    eSystemTik_8000MS  = 16,
    eSystemTik_4000MS  = 8,
    eSystemTik_2000MS  = 4,
    eSystemTik_1000MS  = 2,
    eSystemTik_e500MS  = 1,
};
typedef uint8_t tSystemTikFreq;

//*****************************************************************************
// Describe different OPA setting
//*****************************************************************************
enum {
    eGAIN16,
    eGAIN32,
};
typedef uint8_t eOpaGain;

//*****************************************************************************
// Battery input message
//*****************************************************************************
typedef struct {
    tStatus avgBattParamsUpdateFlg : 1;  //The average data is prepared
    tStatus passiveAvgFlg : 1;
    uint16_t u16Vcell_mV;  //Battery voltage, unit is mV
    int16_t
        i16Icell_mA;  //Battery current, unit is mA, discharge is negative charge is positive
    int8_t i8Tcell_C;      //Battery temperature
    uint32_t u32SumVcell;  //Temp value used to calculate u16AvgVcell_mV
    int32_t i32SumIcell;   //Temp value used to calculate u16AvgIcell_mV
    int16_t i16SumTcell;   //Temp value used to calculate u16AvgTcell_mV
    uint8_t u8SumCount;    //Used to count the sum
    uint16_t u16AvgVcell_mV;
    int16_t i16AvgIcell_mA;
    int8_t i8AvgTcell_C;
} tBattInfoInput;

//*****************************************************************************
// Current calibration factors
//*****************************************************************************
typedef struct {
    uint8_t u8R12toR2FactorCaliValue;
    uint8_t u8R34toR3FactorCaliValue_Gain32;
    uint8_t u8R34toR3FactorCaliValue_Gain16;
    uint8_t u8DefaultValue_Gain32;
    uint8_t u8DefaultValue_Gain16;
} tDacSetValue;

typedef struct {
    tDacSetValue dacSetValue;
    eOpaGain opaGainSet : 2;
    _iq15 iq15R34toR3Factor_Gain32;   //1/GAIN
    _iq5 iq5VadcinOffsetCode_Gain32;  //ADC in code with current equals to 0mA
    _iq5 iq5OpaOffsetVolt_Gain32;     //OPA output offset with Gain 32x
    _iq15 iq15ResFactor_Gain32;
    _iq15 iq15R34toR3Factor_Gain16;
    _iq5 iq5VadcinOffsetCode_Gain16;
    _iq5 iq5OpaOffsetVolt_Gain16;  //OPA output offset with Gain 16x
    _iq15 iq15ResFactor_Gain16;
    _iq15 iq15R12toR2Factor;
    _iq15 iq15R12toR1Factor;
    int16_t i16McuTemp;  //Use to tell MCU when to do run time calibration
} tCurtCalcFactors;

//*****************************************************************************
// Parameters inputed by customer
//*****************************************************************************
typedef const struct {
    uint8_t
        u8AvgBattParamsUpdateCount;  //Average battery parameters update time:u8AvgBattParamsUpdateCount*sysTikFreq
    uint8_t u8SysTikShift;  //Real sysTik time = sysTikFreq/u8SysTikShift
    tSystemTikFreq
        sysTikFreq;  //Used as a clock to run algorithm, can be changed by customer to reduce power consumption
} tBattParamsConfig;

//*****************************************************************************
// Output Parameters
//*****************************************************************************
typedef struct {
    //Battery input parameters
    uint16_t u16Dr_Vcell_mV;  //Battery voltage, unit is mV
    int16_t i16Dr_Icell_mA;   //Battery current, unit is mA
        //Discharge current is negative. Charge current is positive.
    int8_t i8Dr_Tcell_C;  //Battery temperature
    uint16_t u16Dr_AvgVcell_mV;
    int16_t i16Dr_AvgIcell_mA;
    int8_t i8Dr_AvgTcell_C;
} tOutputParams;

//*****************************************************************************
// One battery related parameters
//*****************************************************************************
typedef struct {
    tOutputParams* pOutputParams;
    tBattParamsConfig* pBattParamsCfg;
} tBattGlobalParams;

//*****************************************************************************
// Global parameters
//*****************************************************************************
typedef struct {
    tStatus sysTikFlag : 1;
    tBattGlobalParams* pBattGlobalParams;
    tCurtCalcFactors* pCurtCalcFactors;
} tGaugeApplication;

#endif
