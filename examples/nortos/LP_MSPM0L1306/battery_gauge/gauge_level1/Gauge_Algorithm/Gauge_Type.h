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

#define FLASH_DATA_INPUT (0)
#define COMMUNICATION_DATA_INPUT (1)
#define DETECTION_DATA_INPUT (2)
#define NO_OUTPUT (0)
#define UART_OUTPUT (1)
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
// Warning flags
//*****************************************************************************
typedef struct {
    tStatus VcellHighWarnFlg : 1;
    tStatus VcellLowWarnFlg : 1;
    tStatus TcellHighWarnFlg : 1;
    tStatus TcellLowWarnFlg : 1;
    tStatus CircuitTableOverRangeFlg : 1;
} tWarningFlags;

//*****************************************************************************
// Elements used in the battery circuit model, which is used to characterize the battery
// These parameters need to input ahead.
// The most care the accurate of discharging parameters, especially iq15DhgNomRcell_R.
// For other parameters we can input by experience.
//*****************************************************************************
typedef struct {
    uint16_t u16ChgOcv_mV;  //Open circuit voltage, unit is mV
    _iq15 iq15Soc_DEC;      //State of charge, unit is decimal
    _iq15
        iq15CapFactor;  //C = dSoc(%)/dOcv(mV)*Qmax(As) = dSoc(%)/dOcv(V)*3.6*Qmax(mAh)
                        //capFactor = dSoc/dOcv*3.6
    _iq15 iq15DhgNomRcell_R;
} tBattCircuitParams;

//*****************************************************************************
// Parameters used in VGauge
//*****************************************************************************
typedef struct {
    // TimeConstant factor Ts/(Ts + R*C)
    _iq15 iq15SmoothFactor;
    _iq15 iq15CalculatedOcv[2];
    _iq15 iq15CalRcell_R;
    _iq15 iq15CalCcell_F;  //Calculated battery Capacitor
    _iq15 iq15VFAbsSoc_DEC;
} tVGaugeFilterParams;

typedef const struct {
    //******************************Generation configuration parameters********************************//
    tBattCircuitParams *pBattCircuitParams;
    uint16_t
        u16DesignCap_mAh;  //Battery default battery Cell Size, unit is  mAh
    uint16_t
        u16MinBattVoltThd_mV;  //Need to ensure the battery run time voltage all larger that it.
    uint16_t
        u16MaxBattVoltThd_mV;  //Need to ensure the battery run time voltage all lower that it.
    int8_t i8MaxTempThd_C;
    int8_t i8MinTempThd_C;
    uint8_t
        u8AvgBattParamsUpdateCount;  //Average battery parameters update time:u8AvgBattParamsUpdateCount*sysTikFreq
    uint8_t u8SysTikShift;  //Real sysTik time = sysTikFreq/u8SysTikShift
    tSystemTikFreq
        sysTikFreq;  //Used as a clock to run algorithm, can be changed by customer to reduce power consumption

    //****************************VoltGuage algorithm related parameters********************************//
    uint8_t u8CircuitTableLength;  //Battery circuit table length
    uint8_t
        u8CircuitTableTestTemp_C;  //Battery circuit table test temperature, used to evaluate runtime Rcell
    // paired with RcellNegTshift and RcellPosTshift
    _iq15
        iq15RcellNegTshift_R;  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
    _iq15
        iq15RcellPosTshift_R;  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
} tBattParamsConfig;

//*****************************************************************************
// Gauge  detection parameters(V, T)
//*****************************************************************************
typedef struct {
    tStatus avgBattParamsUpdateFlg : 1;  //The average data is prepared
    uint16_t u16Vcell_mV;                //Battery voltage, unit is mV
    int8_t i8Tcell_C;                    //Battery temperature
    uint32_t u32SumVcell;  //Temp value used to calculate u16AvgVcell_mV
    int16_t i16SumTcell;   //Temp value used to calculate u16AvgTcell_mV
    uint8_t u8SumCount;    //Used to count the sum
    uint16_t u16AvgVcell_mV;
    int8_t i8AvgTcell_C;
} tBattInfoInput;

//*****************************************************************************
// Output Parameters
//*****************************************************************************
typedef struct {
    //Bttery input parameters
    uint16_t u16Dr_Vcell_mV;  //Battery voltage, unit is mV
    int16_t i16Dr_Icell_mA;   //Battery current, unit is mA.
        //Discharge current is negative. Charge current is positive.
    int8_t i8Dr_Tcell_C;  //Battery temperature
    uint16_t u16Dr_AvgVcell_mV;
    int16_t i16Dr_AvgIcell_mA;
    int8_t i8Dr_AvgTcell_C;

    //VGauge parameters
    _iq15 iq15VF_CurCalcOcv_mV;  //Current calculated Ocv
    _iq15 iq15VFAbsSoc_DEC;

    //APP
    tWarningFlags App_WarningFlag;
} tOutputParams;

//*****************************************************************************
// Input Parameters
//*****************************************************************************
typedef union {
    struct {
        //battery input parameters
        uint16_t u16Vcell_mV;  //Battery voltage, unit is mV
        int16_t i16Icell_mA;   //Battery current, unit is mA.
            //Discharge current is negative. Charge current is positive.
        int8_t i8Tcell_C;  //Battery temperature
    } sInputParams;
    uint8_t u8Table[5];
} tInputParams;

//*****************************************************************************
// One battery related parameters
//*****************************************************************************
typedef struct {
    tBattParamsConfig *pBattParamsCfg;
    tBattInfoInput *pBattInfoInput;
    tVGaugeFilterParams *pVGaugeParams;
    tOutputParams *pOutputParams;
    tWarningFlags *pWarningFlag;
} tBattGlobalParams;

//*****************************************************************************
// Global parameters
//*****************************************************************************
typedef struct {
    tStatus sysTikFlag : 1;
    tBattGlobalParams *pBattGlobalParams;
} tGaugeApplication;

#endif
