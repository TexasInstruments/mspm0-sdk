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

#include "Gauge_UserConfig/Gauge_UserConfig.h"
#include "Gauge_Algorithm/Gauge_Type.h"  //A collection of all head files

//The table must go from Target full(100%) to Target empty(0%, consider the influence the battery age influence)
//The Capfactor is calculated from higher resolution Soc and Ocv input, which will be a little different using the Soc and Ocv output
tBattCircuitParams circuitParamsTable[] = {
    {4322, _IQ15(1), _IQ15(0.7807), _IQ15(0.1407)},
    {4176, _IQ15(0.9683), _IQ15(0.7807), _IQ15(0.1407)},
    {4116, _IQ15(0.9366), _IQ15(1.8864), _IQ15(0.1246)},
    {4093, _IQ15(0.9049), _IQ15(5.0278), _IQ15(0.1178)},
    {4081, _IQ15(0.8732), _IQ15(9.7548), _IQ15(0.1181)},
    {4069, _IQ15(0.8415), _IQ15(9.6719), _IQ15(0.1229)},
    {4046, _IQ15(0.8098), _IQ15(4.8939), _IQ15(0.1242)},
    {4013, _IQ15(0.7781), _IQ15(3.4767), _IQ15(0.1275)},
    {3978, _IQ15(0.7464), _IQ15(3.261), _IQ15(0.132)},
    {3946, _IQ15(0.7147), _IQ15(3.6005), _IQ15(0.1368)},
    {3919, _IQ15(0.683), _IQ15(4.2081), _IQ15(0.1426)},
    {3894, _IQ15(0.6514), _IQ15(4.5608), _IQ15(0.1504)},
    {3868, _IQ15(0.6197), _IQ15(4.3398), _IQ15(0.1617)},
    {3839, _IQ15(0.588), _IQ15(3.9351), _IQ15(0.1784)},
    {3806, _IQ15(0.5563), _IQ15(3.4858), _IQ15(0.1939)},
    {3761, _IQ15(0.5246), _IQ15(2.5284), _IQ15(0.1649)},
    {3715, _IQ15(0.4929), _IQ15(2.449), _IQ15(0.1243)},
    {3684, _IQ15(0.4612), _IQ15(3.7173), _IQ15(0.1191)},
    {3661, _IQ15(0.4295), _IQ15(4.9194), _IQ15(0.1217)},
    {3642, _IQ15(0.3979), _IQ15(5.9707), _IQ15(0.1265)},
    {3625, _IQ15(0.3662), _IQ15(6.8702), _IQ15(0.1307)},
    {3609, _IQ15(0.3345), _IQ15(7.2702), _IQ15(0.1355)},
    {3594, _IQ15(0.3028), _IQ15(7.2642), _IQ15(0.1407)},
    {3577, _IQ15(0.2711), _IQ15(6.7087), _IQ15(0.1452)},
    {3555, _IQ15(0.2394), _IQ15(5.2799), _IQ15(0.1429)},
    {3525, _IQ15(0.2077), _IQ15(3.7923), _IQ15(0.1313)},
    {3491, _IQ15(0.176), _IQ15(3.3473), _IQ15(0.1336)},
    {3445, _IQ15(0.1443), _IQ15(2.4866), _IQ15(0.1359)},
    {3404, _IQ15(0.1126), _IQ15(2.7504), _IQ15(0.1426)},
    {3379, _IQ15(0.0809), _IQ15(4.6778), _IQ15(0.1865)},
    {3324, _IQ15(0.0492), _IQ15(2.0623), _IQ15(0.2611)},
    {3090, _IQ15(0.0175), _IQ15(0.4888), _IQ15(0.5505)},
    {2864, _IQ15(0.0), _IQ15(0.2791), _IQ15(1.3375)},
};

const tBattParamsConfig battParamsCfg = {
    //******************************General configuration parameters********************************//
    .pBattCircuitParams = circuitParamsTable,
    .u16DesignCap_mAh   = 3200,
    .u16MinBattVoltThd_mV =
        2500,  //Need to ensure the battery run time voltage all larger that it.
    .u16MaxBattVoltThd_mV =
        4300,  //Need to ensure the battery run time voltage all lower that it.
    .i8MaxTempThd_C             = 60,
    .i8MinTempThd_C             = -20,
    .u8AvgBattParamsUpdateCount = 16,
    .u8SysTikShift              = SYSTEM_TIK_SHIFT,
    .sysTikFreq                 = eSystemTik_1000MS,
    //****************************VoltGuage algorithm related parameters********************************//
    .u8CircuitTableLength =
        CIRCUIT_TABLE_LENGTH,  //Length of OCV-SOC-Rcell table
    .u8CircuitTableTestTemp_C = 25,
    .iq15RcellNegTshift_R     = _IQ15(
        0.005),  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
    .iq15RcellPosTshift_R = _IQ15(
        -0.001),  //Resolution is 0.000030518R or 0.03mR, changed based on u8CircuitTableTestTemp_C
};

tVGaugeFilterParams vGaugeParams;
tBattInfoInput battInfoInput;
tOutputParams outputParams;
tWarningFlags warningFlags;
#if (DETECTION_MODE == COMMUNICATION_DATA_INPUT)
tInputParams inputRarams;  //Only used when in communication data input mode
#endif

tBattGlobalParams battGlobalParams = {.pVGaugeParams = &vGaugeParams,
    .pBattParamsCfg                                  = &battParamsCfg,
    .pBattInfoInput                                  = &battInfoInput,
    .pOutputParams                                   = &outputParams,
    .pWarningFlag                                    = &warningFlags};

tGaugeApplication gaugeApp = {
    .pBattGlobalParams = &battGlobalParams,
};
