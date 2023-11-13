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

#include "Gauge_App.h"

//*****************************************************************************
// Init algorithm used in Gauge
//*****************************************************************************
void App_AlgorithmInit(void)
{
    //Init System state and warning flags
    warningFlags.VcellHighWarnFlg         = 0;
    warningFlags.VcellLowWarnFlg          = 0;
    warningFlags.IcellHighWarnFlg         = 0;
    warningFlags.IcellLowWarnFlg          = 0;
    warningFlags.TcellChgHighWarnFlg      = 0;
    warningFlags.TcellChgLowWarnFlg       = 0;
    warningFlags.TcellDhgHighWarnFlg      = 0;
    warningFlags.TcellDhgLowWarnFlg       = 0;
    warningFlags.CircuitTableOverRangeFlg = 0;
    warningFlags.DhgChgEfcyGapFlg         = 0;

    BSP_CurtCalcFactorsCali(
        &curtCalcFactors);  //calibrate current detection module

#if (DETECTION_MODE == COMMUNICATION_DATA_INPUT)
    App_UartFrameInput();
#endif
    BSP_BattInfoMeasure(&battInfoInput,
        &curtCalcFactors);  //Measure the battery for the first time

    //Update average data for gauge algorithm
    battInfoInput.u16AvgVcell_mV = battInfoInput.u16Vcell_mV;
    battInfoInput.i8AvgTcell_C   = battInfoInput.i8Tcell_C;
    battInfoInput.i16AvgIcell_mA = battInfoInput.i16Icell_mA;
    battInfoInput.passiveAvgFlg  = eFalse;

    //Init Gauge algorithm
    CL_Init(
        &battGlobalParams);  //Step1: Update Capacity learn algorithm (Init battery state)
    VGauge_Init(&battGlobalParams);  //Step2: Voltage gauge init
    IGauge_Init(&battGlobalParams);  //Step3: Current gauge init
    Mix_Init(&battGlobalParams);     //Step4: Mixing algorithm init
    App_UartFrameOutput();
}

//*****************************************************************************
// Algorithm Process
//*****************************************************************************
void App_AlgorithmProcess(void)
{
    CL_Handler(&battGlobalParams);  //Step1: Calibrate SOC and update NomSOC
    VGauge_Handler(&battGlobalParams);  //Step2: Get VFSoc
    IGauge_Handler(&battGlobalParams);  //Step3: Run Coulometer
    Mix_Handler(
        &battGlobalParams);  //Step4: Calculate CusSoc and SmoothSoc; Clear flags
}

//*****************************************************************************
// Update gauge input data
//*****************************************************************************
void App_BattInfoParamsUpdate(void)
{
    int16_t i16CurtTemp, i16TempGap;

    BSP_SysTikCali();  //Calibrate system tick

    BSP_BattInfoMeasure(&battInfoInput, &curtCalcFactors);

    //Reserve for passive average function
    //When the passive average is enabled for this battery, the current will be replaced
    if (battInfoInput.passiveAvgFlg == eTrue)
        battInfoInput.i16Icell_mA = PASSIVE_DHG_CURT_MA;

    //Update Avg used for rest and full judgement
    battInfoInput.i16SumTcell += battInfoInput.i8Tcell_C;
    battInfoInput.i32SumIcell += battInfoInput.i16Icell_mA;
    battInfoInput.u32SumVcell += battInfoInput.u16Vcell_mV;

    battInfoInput.u8SumCount++;

    if (battInfoInput.u8SumCount >= battParamsCfg.u8AvgBattParamsUpdateCount) {
        battInfoInput.u16AvgVcell_mV =
            battInfoInput.u32SumVcell /
            battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i8AvgTcell_C = battInfoInput.i16SumTcell /
                                     battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i16AvgIcell_mA =
            battInfoInput.i32SumIcell /
            battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i16SumTcell            = 0;
        battInfoInput.i32SumIcell            = 0;
        battInfoInput.u32SumVcell            = 0;
        battInfoInput.u8SumCount             = 0;
        battInfoInput.avgBattParamsUpdateFlg = 1;
    } else {
        battInfoInput.avgBattParamsUpdateFlg = 0;
    }

    //Temperature flag judgement
    if (capLearnParams.battState == eCHGUnfull ||
        capLearnParams.battState == eCHGFull) {
        if (battInfoInput.i8Tcell_C > battParamsCfg.i8MaxChgTempThd_C) {
            warningFlags.TcellChgHighWarnFlg = eTrue;
        } else if (battInfoInput.i8Tcell_C < battParamsCfg.i8MinChgTempThd_C) {
            warningFlags.TcellChgLowWarnFlg = eTrue;
        }
    }
    if (capLearnParams.battState == eDHG) {
        if (battInfoInput.i8Tcell_C > battParamsCfg.i8MaxDhgTempThd_C) {
            warningFlags.TcellDhgHighWarnFlg = eTrue;
        } else if (battInfoInput.i8Tcell_C < battParamsCfg.i8MinChgTempThd_C) {
            warningFlags.TcellDhgLowWarnFlg = eTrue;
        }
    }
    //voltage flag judgement
    if (battInfoInput.u16Vcell_mV > battParamsCfg.u16MaxBattVoltThd_mV) {
        warningFlags.VcellHighWarnFlg = eTrue;
    } else if (battInfoInput.u16Vcell_mV <
               battParamsCfg.u16MinBattVoltThd_mV) {
        warningFlags.VcellLowWarnFlg = eTrue;
    }
    //current flag judgement
    if (battInfoInput.i16Icell_mA > battParamsCfg.i16MaxChgCurtThd_mA) {
        warningFlags.IcellHighWarnFlg = eTrue;
    } else if (battInfoInput.i16Icell_mA < battParamsCfg.i16MinDhgCurtThd_mA) {
        warningFlags.IcellLowWarnFlg = eTrue;
    }
}

//*****************************************************************************
// System init
//*****************************************************************************
void App_SystemInit(void)
{
    BSP_PeripheralsInit();  //Hardware init
    App_AlgorithmInit();    //Algorithm init
}

//*****************************************************************************
// Communicaiton functions
//*****************************************************************************
void App_UartFrameInput(void)
{
    uint8_t u8RxDataCount = 0;
    uint16_t u16RxData    = 0;

    Driver_Uart1Init();

    //Will receive:
    //0x5a5a (header) + Vcell(unit16_t) + Icell(int16_t) + Tcell(int8_t)
    while (DL_UART_getRawInterruptStatus(UART1, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART1, DL_UART_INTERRUPT_RX);
    u16RxData += DL_UART_Main_receiveData(UART1);
    while (DL_UART_getRawInterruptStatus(UART1, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART1, DL_UART_INTERRUPT_RX);
    u16RxData += (DL_UART_Main_receiveData(UART1) << 8);

    if (u16RxData == 0x5a5a) {
        while (u8RxDataCount < 5) {
            while (DL_UART_getRawInterruptStatus(
                       UART1, DL_UART_INTERRUPT_RX) == 0)
                ;
            DL_UART_clearInterruptStatus(UART1, DL_UART_INTERRUPT_RX);
            inputRarams.u8Table[u8RxDataCount] =
                DL_UART_Main_receiveData(UART1);
            u8RxDataCount++;
        }
        battInfoInput.u16Vcell_mV = inputRarams.sInputParams.u16Vcell_mV;
        battInfoInput.i16Icell_mA = inputRarams.sInputParams.i16Icell_mA;
        battInfoInput.i8Tcell_C   = inputRarams.sInputParams.i8Tcell_C;
    }

    //Disable UART
    DL_UART_Main_reset(UART1);
    DL_UART_Main_disablePower(UART1);
}

void App_UartFrameOutput(void)
{
    int16_t i16SmoothSoc, i16AbsSoc, i16NomSoc, i16SOH, i16DeltaSoc,
        i16DeltaCap, i16Efcy, i16Data;
    int16_t temp;
    uint16_t u16Data;
    uint16_t *p_u16Flags;
    static uint16_t counter = 0;

    Driver_Uart1Init();

    outputParams.u16Dr_Vcell_mV    = battInfoInput.u16Vcell_mV;
    outputParams.i16Dr_Icell_mA    = battInfoInput.i16Icell_mA;
    outputParams.i8Dr_Tcell_C      = battInfoInput.i8Tcell_C;
    outputParams.u16Dr_AvgVcell_mV = battInfoInput.u16AvgVcell_mV;
    outputParams.i16Dr_AvgIcell_mA = battInfoInput.i16AvgIcell_mA;
    outputParams.i8Dr_AvgTcell_C   = battInfoInput.i8AvgTcell_C;

    // VGauge parameters
    outputParams.iq15VF_CurCalcOcv_mV = vGaugeParams.iq15CalculatedOcv[0];
    ;
    outputParams.iq15VFAbsSoc_DEC = vGaugeParams.iq15VFAbsSoc_DEC;

    // IGauge parameters
    outputParams.i16IF_NomRemCap_mAh = iGaugeParams.i16IF_NomRemCap_mAh;
    outputParams.u16IF_Cycles        = iGaugeParams.u16DhgCycles;

    // Capacity learn parameters
    outputParams.u16CL_CaliOcv_mV        = capLearnParams.u16CaliOcv_mV;
    outputParams.iq15CL_CaliSoc_DEC      = capLearnParams.iq15CaliSoc_DEC;
    outputParams.CL_BattState            = capLearnParams.battStateRecord[0];
    outputParams.u16CL_RawNomFullCap_mAh = capLearnParams.u16RawNomFullCap_mAh;
    outputParams.iq15CL_SumAbsDeltaSoc_DEC =
        capLearnParams.iq15SumAbsDeltaSoc_DEC;
    outputParams.i16IF_DeltaQsum_mAh =
        _IQ15int(capLearnParams.iq15SumDeltaCap_mAh);
    outputParams.iq15CL_SOH_DEC = capLearnParams.iq15SOH_DEC;

    // Mix algorithm
    outputParams.iq15Mix_AbsEmptySoc_DEC  = mixingParams.iq15AbsEmptySoc[0];
    outputParams.u8Mix_CurFullMatrixCount = 0;
    outputParams.u8Mix_CurEmptyMatrixCount =
        mixingParams.u8EmptySocMatrixCount;
    outputParams.iq15Mix_AbsFullSoc_DEC   = mixingParams.iq15AbsFullSoc[0];
    outputParams.iq15Mix_SmoothRltSoc_DEC = mixingParams.iq15SmoothRltSoc_DEC;
    outputParams.iq15Mix_CusRltSoc_DEC    = mixingParams.iq15CusRltSoc[0];
    outputParams.i16Mix_CusRemCap_mAh     = mixingParams.i16CusRemCap_mAh;
    outputParams.u16Mix_CusFullCap_mAh    = mixingParams.u16CusFullCap_mAh;
    outputParams.iq15Mix_NomAbsSoc_DEC    = mixingParams.iq15NomAbsSoc_DEC;
    outputParams.i16Mix_NomRemCap_mAh     = mixingParams.i16Mix_NomRemCap_mAh;
    outputParams.u16Mix_NomFullCap_mAh    = mixingParams.u16NomFullCap_mAh;

    // App
    outputParams.App_WarningFlag = warningFlags;

#if OUTPUT_MODE == UART_OUTPUT

    //Used to show the system is working
    counter++;
    Driver_Uart1TxU16Data(counter);

    Driver_Uart1TxU16Data(outputParams.u16Dr_Vcell_mV);
    Driver_Uart1TxU16Data(outputParams.i16Dr_Icell_mA);
    Driver_Uart1TxU16Data(outputParams.u16Dr_AvgVcell_mV);
    Driver_Uart1TxU16Data(outputParams.i16Dr_AvgIcell_mA);
    Driver_Uart1TxU16Data(outputParams.i8Dr_AvgTcell_C);

    //Amply 1000 times
    i16NomSoc =
        _IQ15int(_IQ15mpy(outputParams.iq15Mix_NomAbsSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16NomSoc);

    //Amply 1000 times
    i16AbsSoc =
        _IQ15int(_IQ15mpy(outputParams.iq15Mix_CusRltSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16AbsSoc);

    //Amply 1000 times
    i16SmoothSoc =
        _IQ15int(_IQ15mpy(outputParams.iq15Mix_SmoothRltSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16SmoothSoc);

    //Amply 1000 times
    i16SOH = _IQ15int(_IQ15mpy(outputParams.iq15CL_SOH_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16SOH);

    Driver_Uart1TxU16Data(outputParams.u16Mix_NomFullCap_mAh);
    Driver_Uart1TxU16Data(outputParams.u16Mix_CusFullCap_mAh);
    Driver_Uart1TxU16Data(outputParams.i16Mix_NomRemCap_mAh);
    Driver_Uart1TxU16Data(outputParams.i16Mix_CusRemCap_mAh);
    Driver_Uart1TxU16Data(outputParams.u16CL_RawNomFullCap_mAh);
    Driver_Uart1TxU16Data(outputParams.u16CL_CaliOcv_mV);

    i16Data =
        _IQ15int(_IQ15mpy(outputParams.iq15Mix_AbsEmptySoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16Data);
    i16Data =
        _IQ15int(_IQ15mpy(outputParams.iq15Mix_AbsFullSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16Data);

    Driver_Uart1TxU16Data((uint16_t) outputParams.u8Mix_CurEmptyMatrixCount);
    Driver_Uart1TxU16Data((uint16_t) outputParams.u8Mix_CurFullMatrixCount);
    Driver_Uart1TxU16Data(outputParams.i16IF_DeltaQsum_mAh);

    i16DeltaSoc = _IQ15int(
        _IQ15mpy(outputParams.iq15CL_SumAbsDeltaSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16DeltaSoc);

    i16Efcy = _IQ15int(_IQ15mpy(outputParams.iq15CL_CaliSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16Efcy);

    Driver_Uart1TxU16Data(outputParams.u16IF_Cycles);

    u16Data = *((uint16_t *) &outputParams.App_WarningFlag);
    Driver_Uart1TxU16Data(u16Data);

    Driver_Uart1TxU16Data(0);
    Driver_Uart1TxU16Data(outputParams.CL_BattState);

    //Frame end
    Driver_Uart1TxU16Data(0x5a5a);
#endif

    //Disable UART
    DL_UART_Main_reset(UART1);
    DL_UART_Main_disablePower(UART1);

#if DETECTION_MODE == FLASH_DATA_INPUT
    delay_cycles(1600000);  //Avoid data transmition too fast
#endif
}

//*****************************************************************************
// MOSFET Control
//*****************************************************************************
tStatus App_BattInfoInRange(int8_t i8tempLowThd, int8_t i8tempHighThd)
{
    //Use average parameters as the judgment conditions to make sure the battery has get out of warning states
    if ((battInfoInput.i8AvgTcell_C < i8tempHighThd &&
            battInfoInput.i8AvgTcell_C > i8tempLowThd) &&
        (battInfoInput.i16AvgIcell_mA < battParamsCfg.i16MaxChgCurtThd_mA &&
            battInfoInput.i16AvgIcell_mA >
                battParamsCfg.i16MinDhgCurtThd_mA) &&
        (battInfoInput.u16AvgVcell_mV < battParamsCfg.u16MaxBattVoltThd_mV &&
            battInfoInput.u16AvgVcell_mV > battParamsCfg.u16MinBattVoltThd_mV))
        return eTrue;
    else
        return eFalse;
}
