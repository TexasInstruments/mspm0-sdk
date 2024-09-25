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

#include "Gauge_UserConfig.h"
#include "ti/battery_gauge/gauge_level2/Gauge_Type.h"  //A collection of all head files

//*****************************************************************************
//
//! \var circuitParamsTable
//The table must go from full to empty else it will influence the V-guage
//The Capfactor is calculated from higher resolution Soc and Ocv input, which
//will be a little different using the different resolution Soc and Ocv intput.
//
//*****************************************************************************
tBattCircuitParams circuitParamsTable[CIRCUIT_TABLE_LENGTH] = {
    {_IQ15(3526.3), _IQ15(1), _IQ15(0.6033), _IQ15(0.1649), _IQ20(0.000167)},
    {_IQ15(3330.3), _IQ15(0.9672), _IQ15(0.6033), _IQ15(0.1649),
        _IQ20(0.000167)},
    {_IQ15(3329.9), _IQ15(0.9343), _IQ15(295.6051), _IQ15(0.1699),
        _IQ20(0.082250)},
    {_IQ15(3329.8), _IQ15(0.9015), _IQ15(1182.4151), _IQ15(0.1743),
        _IQ20(0.328000)},
    {_IQ15(3329.7), _IQ15(0.8686), _IQ15(1181.4356), _IQ15(0.179),
        _IQ20(0.329000)},
    {_IQ15(3329.5), _IQ15(0.8358), _IQ15(591.2069), _IQ15(0.1843),
        _IQ20(0.164000)},
    {_IQ15(3329.3), _IQ15(0.803), _IQ15(590.7176), _IQ15(0.1909),
        _IQ20(0.164000)},
    {_IQ15(3329.0), _IQ15(0.7702), _IQ15(393.8124), _IQ15(0.1988),
        _IQ20(0.109333)},
    {_IQ15(3328.6), _IQ15(0.7373), _IQ15(295.6018), _IQ15(0.2083),
        _IQ20(0.082250)},
    {_IQ15(3327.9), _IQ15(0.7045), _IQ15(168.7779), _IQ15(0.2191),
        _IQ20(0.046857)},
    {_IQ15(3326.4), _IQ15(0.6717), _IQ15(78.763), _IQ15(0.2299),
        _IQ20(0.021867)},
    {_IQ15(3320.5), _IQ15(0.6389), _IQ15(20.0246), _IQ15(0.2304),
        _IQ20(0.005559)},
    {_IQ15(3304.5), _IQ15(0.6061), _IQ15(7.384), _IQ15(0.2046),
        _IQ20(0.002050)},
    {_IQ15(3293.9), _IQ15(0.5732), _IQ15(11.155), _IQ15(0.1896),
        _IQ20(0.003104)},
    {_IQ15(3290.8), _IQ15(0.5404), _IQ15(38.1114), _IQ15(0.1885),
        _IQ20(0.010581)},
    {_IQ15(3289.4), _IQ15(0.5076), _IQ15(84.3902), _IQ15(0.1896),
        _IQ20(0.023429)},
    {_IQ15(3288.7), _IQ15(0.4748), _IQ15(168.781), _IQ15(0.1922),
        _IQ20(0.046857)},
    {_IQ15(3288.2), _IQ15(0.4419), _IQ15(236.4859), _IQ15(0.1965),
        _IQ20(0.065800)},
    {_IQ15(3287.6), _IQ15(0.4091), _IQ15(197.0754), _IQ15(0.2019),
        _IQ20(0.054667)},
    {_IQ15(3286.7), _IQ15(0.3762), _IQ15(131.3859), _IQ15(0.2088),
        _IQ20(0.036556)},
    {_IQ15(3284.6), _IQ15(0.3434), _IQ15(56.2599), _IQ15(0.2148),
        _IQ20(0.015619)},
    {_IQ15(3279.2), _IQ15(0.3105), _IQ15(21.8973), _IQ15(0.2159),
        _IQ20(0.006093)},
    {_IQ15(3269.9), _IQ15(0.2777), _IQ15(12.7145), _IQ15(0.2114),
        _IQ20(0.003527)},
    {_IQ15(3259.7), _IQ15(0.2449), _IQ15(11.5831), _IQ15(0.2088),
        _IQ20(0.003216)},
    {_IQ15(3249.8), _IQ15(0.2121), _IQ15(11.9335), _IQ15(0.2141),
        _IQ20(0.003313)},
    {_IQ15(3235.1), _IQ15(0.1792), _IQ15(8.0372), _IQ15(0.2162),
        _IQ20(0.002238)},
    {_IQ15(3218.5), _IQ15(0.1464), _IQ15(7.1171), _IQ15(0.2231),
        _IQ20(0.001976)},
    {_IQ15(3203.7), _IQ15(0.1136), _IQ15(7.9824), _IQ15(0.2204),
        _IQ20(0.002216)},
    {_IQ15(3198.7), _IQ15(0.0808), _IQ15(23.6277), _IQ15(0.2507),
        _IQ20(0.006560)},
    {_IQ15(3118.7), _IQ15(0.048), _IQ15(1.478), _IQ15(0.2665),
        _IQ20(0.000410)},
    {_IQ15(2867.0), _IQ15(0.0151), _IQ15(0.4697), _IQ15(0.3316),
        _IQ20(0.000131)},
    {_IQ15(2588.9), _IQ15(0.0038), _IQ15(0.1464), _IQ15(1.5692),
        _IQ20(0.000041)},
    {_IQ15(2484.8), _IQ15(0.002), _IQ15(0.0631), _IQ15(3.1005),
        _IQ20(0.000017)},
    {_IQ15(2404.2), _IQ15(0.0008), _IQ15(0.0517), _IQ15(6.6487),
        _IQ20(0.000015)},
    {_IQ15(2333.8), _IQ15(0.0), _IQ15(0.0418), _IQ15(11.4237),
        _IQ20(0.000011)},
};

//*****************************************************************************
//
//! \var battParamsCfg
//This includes the setting of battery working condiction and algorithm setting
//
//*****************************************************************************
const tBattParamsConfig battParamsCfg = {
    /*General configuration parameters*/
    .pBattCircuitParams = circuitParamsTable,
    .u16DesignCap_mAh   = 3800,
    .u16MinBattVoltThd_mV =
        2000,  //Need to ensure the battery run time voltage all larger that it.
    //When voltage bellow than this, the SmoothSOC will turns to 0% at
    // once and the warning flag will be set.
    .u16MaxBattVoltThd_mV =
        3800,  //Need to ensure the battery run time voltage all lower that it.
    .i16MaxChgCurtThd_mA =
        2180,  //Influenced by hardware setting and application requirement
    .i16MinDhgCurtThd_mA =
        -2000,  //Influenced by hardware setting and application requirement
    .i8MaxChgTempThd_C = 60,
    .i8MinChgTempThd_C = 0,
    .i8MaxDhgTempThd_C = 70,
    .i8MinDhgTempThd_C = -20,
    .u16MinFullChgVoltThd_mV =
        3400,  //We advise to set the value a little lower than the real charge voltage range,
    //as the voltage will drop because of line resistance.
    //This will be used to judge when the battery charge full, to do calibration
    .u16MaxFullChgVoltThd_mV =
        3600,  //We need to make sure the charge voltage range should always higher than this value
    .i16FullChgCurtThd_mA = 160,  //The real charge terminate current
    //Should be higher than the real full charge current,Suggest value is above 10mA
    .u16EmptyDhgVoltThd_mV =
        2300,  //When voltage reaches this u16EmptyDhgVoltThd_mV, we treat it as empty
    //Should be higher than the real terminate voltage,Suggest value is above 50mV
    .u8AvgBattParamsUpdateCount = 16,
    /*Mix algorithm related parameters*/
    .iq15SmSocIIRFactor =
        _IQ15(1),  //When it =1 disable IIR filter for smoothSoc
    .u16ConvergeStartVolt_mV =
        3200,  //When the run time voltage below than this parameter,the converging algorithm will start to use
    .i8TempThd1_C   = 70,
    .i8TempThd2_C   = 55,
    .i8TempThd3_C   = 40,
    .i8TempThd4_C   = 25,
    .i8TempThd5_C   = 10,
    .i8TempThd6_C   = 5,
    .i16CurtThd1_mA = -300,
    .i16CurtThd2_mA = -600,
    .i16CurtThd3_mA = -1500,
    .i16CurtThd4_mA = -3000,
    //i: means matrix row
    //              70           55            40             25            10           5
    //         TempThd1_C    TempThd2_C    TempThd3_C    TempThd4_C    TempThd5_C    TempThd6_C
    //             |             |             |             |             |             |
    //Matrix[7*i+0] Matrix[7*i+1] Matrix[7*i+2] Matrix[7*i+3] Matrix[7*i+4] Matrix[7*i+5] Matrix[7*i+6]
    //
    //j: means matrix column
    //             -300         -600         -1500          -3000
    //        CurtThd1_mA    CurtThd2_mA   CurtThd3_mA   CurtThd4_mA
    //             |             |             |             |
    //Matrix[7*0+j] Matrix[7*1+j] Matrix[7*2+j] Matrix[7*3+j]  Matrix[7*4+j]
    // TempThd and CurtThd table should be saved from high to low!!!
    .iq15AbsEmptySocMatrixInput[0]  = 0,
    .iq15AbsEmptySocMatrixInput[1]  = 0,
    .iq15AbsEmptySocMatrixInput[2]  = 0,
    .iq15AbsEmptySocMatrixInput[3]  = 0,
    .iq15AbsEmptySocMatrixInput[4]  = 0,
    .iq15AbsEmptySocMatrixInput[5]  = 0,
    .iq15AbsEmptySocMatrixInput[6]  = 0,
    .iq15AbsEmptySocMatrixInput[7]  = 0,
    .iq15AbsEmptySocMatrixInput[8]  = 0,
    .iq15AbsEmptySocMatrixInput[9]  = 0,
    .iq15AbsEmptySocMatrixInput[10] = 0,
    .iq15AbsEmptySocMatrixInput[11] = 0,
    .iq15AbsEmptySocMatrixInput[12] = 0,
    .iq15AbsEmptySocMatrixInput[13] = 0,
    .iq15AbsEmptySocMatrixInput[14] = 0,
    .iq15AbsEmptySocMatrixInput[15] = 0,
    .iq15AbsEmptySocMatrixInput[16] = 0,
    .iq15AbsEmptySocMatrixInput[17] = 0,
    .iq15AbsEmptySocMatrixInput[18] = 0,
    .iq15AbsEmptySocMatrixInput[19] = 0,
    .iq15AbsEmptySocMatrixInput[20] = 0,
    .iq15AbsEmptySocMatrixInput[21] = 0,
    .iq15AbsEmptySocMatrixInput[22] = 0,
    .iq15AbsEmptySocMatrixInput[23] = 0,
    .iq15AbsEmptySocMatrixInput[24] = 0,
    .iq15AbsEmptySocMatrixInput[25] = 0,
    .iq15AbsEmptySocMatrixInput[26] = 0,
    .iq15AbsEmptySocMatrixInput[27] = 0,
    .iq15AbsEmptySocMatrixInput[28] = 0,
    .iq15AbsEmptySocMatrixInput[29] = 0,
    .iq15AbsEmptySocMatrixInput[30] = 0,
    .iq15AbsEmptySocMatrixInput[31] = 0,
    .iq15AbsEmptySocMatrixInput[32] = 0,
    .iq15AbsEmptySocMatrixInput[33] = 0,
    .iq15AbsEmptySocMatrixInput[34] = 0,
    /*Capacity learn algorithm related parameters*/
    .i16UnloadCurtLowThd_mA =
        -20,  //Need to consider the noise of current detection
    .i16UnloadCurtHighThd_mA =
        20,  //Need to consider the noise of current detection, bellow than i16FullChgCurtThd_mA
    .iq15DefaultSOH_DEC = _IQ15(1),
    /*VoltGuage algorithm related parameters*/
    .u8CircuitTableLength =
        CIRCUIT_TABLE_LENGTH,  //Length of OCV-SOC-Rcell table
    .u8CircuitTableTestTemp_C =
        25,  //The OCV-SOC-Rcell table tested temperature
    .iq15RcellNegTshift_R = _IQ15(
        0.005),  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
    .iq15RcellPosTshift_R = _IQ15(
        -0.001),  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
                  /*CurrentGuage algorithm related parameters*/
    .i16AvgLeakageCurt_mA = -4,
    .i16PassiveDhgCurt_mA = 100,
};

//*****************************************************************************
//
//! \var battGlobalParams
//This includes the all the run time parameters for one battery
//
//*****************************************************************************
tBattGlobalParams battGlobalParams_00 = {
    .pBattParamsCfg = &battParamsCfg,
};
tBattGlobalParams battGlobalParams_01 = {
    .pBattParamsCfg = &battParamsCfg,
};
tBattGlobalParams battGlobalParams_02 = {
    .pBattParamsCfg = &battParamsCfg,
};

tBattGlobalParams battGlobalParams_03 = {
    .pBattParamsCfg = &battParamsCfg,
};

//*****************************************************************************
//
//! \var g_pCaptivateSensorArray
//! This array allows for indexed access to any
//! battery parameters in the configuration.
//
//*****************************************************************************
tBattGlobalParams* battGlobalParamsArray[CELL_NUMBER] = {
    &battGlobalParams_00,
    &battGlobalParams_01,
    &battGlobalParams_02,
    &battGlobalParams_03,
};

//*****************************************************************************
//
//! \var gaugeApp
//! This structure stores the global settings for this application.
//
//*****************************************************************************
tGaugeApplication gaugeApp = {
    .pBattGlobalParamList = &battGlobalParamsArray[0],
    .ui8NrOfCell          = CELL_NUMBER,
    .u8SysTikShift =
        2,  //As the least sys clock is 500ms, we multiply it with 2
    .sysTikFreq = eSystemTik_1000MS,
};
