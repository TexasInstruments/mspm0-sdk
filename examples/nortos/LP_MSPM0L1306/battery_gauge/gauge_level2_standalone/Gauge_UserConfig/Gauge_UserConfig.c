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
    {_IQ15(4322.2), _IQ15(1), _IQ15(0.7807), _IQ15(0.1407), _IQ20(0.000217)},
    {_IQ15(4176.0), _IQ15(0.9683), _IQ15(0.7807), _IQ15(0.1407),
        _IQ20(0.000217)},
    {_IQ15(4115.5), _IQ15(0.9366), _IQ15(1.8864), _IQ15(0.1246),
        _IQ20(0.000524)},
    {_IQ15(4092.8), _IQ15(0.9049), _IQ15(5.0278), _IQ15(0.1178),
        _IQ20(0.001396)},
    {_IQ15(4081.1), _IQ15(0.8732), _IQ15(9.7548), _IQ15(0.1181),
        _IQ20(0.002709)},
    {_IQ15(4069.3), _IQ15(0.8415), _IQ15(9.6719), _IQ15(0.1229),
        _IQ20(0.002686)},
    {_IQ15(4046.0), _IQ15(0.8098), _IQ15(4.8939), _IQ15(0.1242),
        _IQ20(0.001361)},
    {_IQ15(4013.2), _IQ15(0.7781), _IQ15(3.4767), _IQ15(0.1275),
        _IQ20(0.000966)},
    {_IQ15(3978.2), _IQ15(0.7464), _IQ15(3.261), _IQ15(0.132),
        _IQ20(0.000906)},
    {_IQ15(3946.5), _IQ15(0.7147), _IQ15(3.6005), _IQ15(0.1368),
        _IQ20(0.001000)},
    {_IQ15(3919.4), _IQ15(0.683), _IQ15(4.2081), _IQ15(0.1426),
        _IQ20(0.001170)},
    {_IQ15(3894.4), _IQ15(0.6514), _IQ15(4.5608), _IQ15(0.1504),
        _IQ20(0.001264)},
    {_IQ15(3868.1), _IQ15(0.6197), _IQ15(4.3398), _IQ15(0.1617),
        _IQ20(0.001205)},
    {_IQ15(3839.1), _IQ15(0.588), _IQ15(3.9351), _IQ15(0.1784),
        _IQ20(0.001093)},
    {_IQ15(3806.4), _IQ15(0.5563), _IQ15(3.4858), _IQ15(0.1939),
        _IQ20(0.000969)},
    {_IQ15(3761.3), _IQ15(0.5246), _IQ15(2.5284), _IQ15(0.1649),
        _IQ20(0.000703)},
    {_IQ15(3714.7), _IQ15(0.4929), _IQ15(2.449), _IQ15(0.1243),
        _IQ20(0.000680)},
    {_IQ15(3684.0), _IQ15(0.4612), _IQ15(3.7173), _IQ15(0.1191),
        _IQ20(0.001033)},
    {_IQ15(3660.8), _IQ15(0.4295), _IQ15(4.9194), _IQ15(0.1217),
        _IQ20(0.001366)},
    {_IQ15(3641.7), _IQ15(0.3979), _IQ15(5.9707), _IQ15(0.1265),
        _IQ20(0.001654)},
    {_IQ15(3625.1), _IQ15(0.3662), _IQ15(6.8702), _IQ15(0.1307),
        _IQ20(0.001910)},
    {_IQ15(3609.4), _IQ15(0.3345), _IQ15(7.2702), _IQ15(0.1355),
        _IQ20(0.002019)},
    {_IQ15(3593.7), _IQ15(0.3028), _IQ15(7.2642), _IQ15(0.1407),
        _IQ20(0.002019)},
    {_IQ15(3576.7), _IQ15(0.2711), _IQ15(6.7087), _IQ15(0.1452),
        _IQ20(0.001865)},
    {_IQ15(3555.1), _IQ15(0.2394), _IQ15(5.2799), _IQ15(0.1429),
        _IQ20(0.001468)},
    {_IQ15(3525.0), _IQ15(0.2077), _IQ15(3.7923), _IQ15(0.1313),
        _IQ20(0.001053)},
    {_IQ15(3490.9), _IQ15(0.176), _IQ15(3.3473), _IQ15(0.1336),
        _IQ20(0.000930)},
    {_IQ15(3445.0), _IQ15(0.1443), _IQ15(2.4866), _IQ15(0.1359),
        _IQ20(0.000691)},
    {_IQ15(3403.5), _IQ15(0.1126), _IQ15(2.7504), _IQ15(0.1426),
        _IQ20(0.000764)},
    {_IQ15(3379.1), _IQ15(0.0809), _IQ15(4.6778), _IQ15(0.1865),
        _IQ20(0.001299)},
    {_IQ15(3323.8), _IQ15(0.0492), _IQ15(2.0623), _IQ15(0.2611),
        _IQ20(0.000573)},
    {_IQ15(3090.5), _IQ15(0.0175), _IQ15(0.4888), _IQ15(0.5505),
        _IQ20(0.000136)},
    {_IQ15(2864.3), _IQ15(0.0), _IQ15(0.2791), _IQ15(1.3375), _IQ20(0.000077)},
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
    .u16DesignCap_mAh   = 3200,
    .u16MinBattVoltThd_mV =
        2600,  //Need to ensure the battery run time voltage all larger that it.
    //When voltage bellow than this, the SmoothSOC will turns to 0% at
    // once and the warning flag will be set.
    .u16MaxBattVoltThd_mV =
        4250,  //Need to ensure the battery run time voltage all lower that it.
    .i16MaxChgCurtThd_mA =
        2180,  //Influenced by hardware setting and application requirement
    .i16MinDhgCurtThd_mA =
        -2000,  //Influenced by hardware setting and application requirement
    .i8MaxChgTempThd_C = 60,
    .i8MinChgTempThd_C = 0,
    .i8MaxDhgTempThd_C = 70,
    .i8MinDhgTempThd_C = -20,
    .u16MinFullChgVoltThd_mV =
        4150,  //We advise to set the value a little lower than the real charge voltage range,
    //as the voltage will drop because of line resistance.
    //This will be used to judge when the battery charge full, to do calibration
    .u16MaxFullChgVoltThd_mV =
        4200,  //We need to make sure the charge voltage range should always higher than this value
    .u16EmptyDhgVoltThd_mV =
        3000,  //When voltage reaches this u16EmptyDhgVoltThd_mV, we treat it as empty
    //Should be higher than the real terminate voltage,Suggest value is above 50mV
    .i16FullChgCurtThd_mA = 150,  //The real charge terminate current
    //Should be higher than the real full charge current,Suggest value is above 10mA

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
    .i16UnloadCurtLowThd_mA  = -5,
    .i16UnloadCurtHighThd_mA = 5,
    .iq15DefaultSOH_DEC      = _IQ15(1),
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

//*****************************************************************************
//
//! \var g_pCaptivateSensorArray
//! This array allows for indexed access to any
//! battery parameters in the configuration.
//
//*****************************************************************************
tBattGlobalParams* battGlobalParamsArray[CELL_NUMBER] = {
    &battGlobalParams_00,
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
