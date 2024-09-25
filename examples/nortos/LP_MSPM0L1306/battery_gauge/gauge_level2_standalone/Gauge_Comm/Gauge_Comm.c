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

#include "Gauge_Comm/Gauge_Comm.h"

//*****************************************************************************
// Communication functions
//*****************************************************************************
void App_UartFrameInput(void)
{
    uint16_t u16RxData;
    uint32_t u32RxData;
    uint8_t u8Count;

    tBattInfoInput* pBattInfoInput;
    tBattParamsConfig* pBattParamsCfg;
    tBattCapLearnParams* pCapLearnParams;
    tWarningFlags* pWarningFlag;
    uint16_t u16Vcell_mV;

#if (DETECTION_MODE == COMMUNICATION_DATA_INPUT)
    Driver_UartInit();

    //Will receive:
    //0x5a5a (header) + Vcell(unit16_t) + Icell(int16_t) + Tcell(int8_t)
    u32RxData = Driver_UartRxU32Data();
    //Header 0x5a5aa5a5
    if (u32RxData == 0x5a5aa5a5) {
        for (u8Count = 0; u8Count < CELL_NUMBER; u8Count++) {
            pBattInfoInput =
                &gaugeApp.pBattGlobalParamList[u8Count]->battInfoInput;
            pBattParamsCfg =
                gaugeApp.pBattGlobalParamList[u8Count]->pBattParamsCfg;
            pCapLearnParams =
                &gaugeApp.pBattGlobalParamList[u8Count]->capLearnParams;
            pWarningFlag =
                &gaugeApp.pBattGlobalParamList[u8Count]->warningFlag;

            u16RxData                    = Driver_UartRxU16Data();
            pBattInfoInput->iq15Vcell_mV = _IQ15(u16RxData);
            u16Vcell_mV                  = u16RxData;
            u16RxData                    = Driver_UartRxU16Data();
            pBattInfoInput->i16Icell_mA  = (int16_t) u16RxData;
            u16RxData                    = Driver_UartRxU16Data();
            pBattInfoInput->i8Tcell_C    = (int8_t) u16RxData;

            //When the passive average is enabled for this battery, the current will be replaced
            if (pBattInfoInput->CellBalanceFlg == eTrue)
                pBattInfoInput->i16Icell_mA =
                    pBattParamsCfg->i16PassiveDhgCurt_mA;

            //Update Avg used for rest and full judgment
            pBattInfoInput->i16SumTcell += pBattInfoInput->i8Tcell_C;
            pBattInfoInput->i32SumIcell += pBattInfoInput->i16Icell_mA;
            pBattInfoInput->u32SumVcell += u16Vcell_mV;

            pBattInfoInput->u8SumCount++;

            if (pBattInfoInput->u8SumCount >=
                pBattParamsCfg->u8AvgBattParamsUpdateCount) {
                pBattInfoInput->iq15AvgVcell_mV =
                    _IQ15div(pBattInfoInput->u32SumVcell,
                        pBattParamsCfg->u8AvgBattParamsUpdateCount);
                pBattInfoInput->i8AvgTcell_C =
                    pBattInfoInput->i16SumTcell /
                    pBattParamsCfg->u8AvgBattParamsUpdateCount;
                pBattInfoInput->i16AvgIcell_mA =
                    pBattInfoInput->i32SumIcell /
                    pBattParamsCfg->u8AvgBattParamsUpdateCount;
                pBattInfoInput->i16SumTcell            = 0;
                pBattInfoInput->i32SumIcell            = 0;
                pBattInfoInput->u32SumVcell            = 0;
                pBattInfoInput->u8SumCount             = 0;
                pBattInfoInput->avgBattParamsUpdateFlg = 1;
            } else {
                pBattInfoInput->avgBattParamsUpdateFlg = 0;
            }

            //Temperature flag judgment
            if (pCapLearnParams->battState == eCHG) {
                if (pBattInfoInput->i8Tcell_C >
                    pBattParamsCfg->i8MaxChgTempThd_C) {
                    pWarningFlag->TcellChgHighWarnFlg = eTrue;
                } else if (pBattInfoInput->i8Tcell_C <
                           pBattParamsCfg->i8MinChgTempThd_C) {
                    pWarningFlag->TcellChgLowWarnFlg = eTrue;
                }
            }
            if (pCapLearnParams->battState == eDHG) {
                if (pBattInfoInput->i8Tcell_C >
                    pBattParamsCfg->i8MaxDhgTempThd_C) {
                    pWarningFlag->TcellDhgHighWarnFlg = eTrue;
                } else if (pBattInfoInput->i8Tcell_C <
                           pBattParamsCfg->i8MinDhgTempThd_C) {
                    pWarningFlag->TcellDhgLowWarnFlg = eTrue;
                }
            }
            //voltage flag judgment
            if (u16Vcell_mV > pBattParamsCfg->u16MaxBattVoltThd_mV) {
                pWarningFlag->VcellHighWarnFlg = eTrue;
            } else if (u16Vcell_mV < pBattParamsCfg->u16MinBattVoltThd_mV) {
                pWarningFlag->VcellLowWarnFlg = eTrue;
            }
            //current flag judgment
            if (pBattInfoInput->i16Icell_mA >
                pBattParamsCfg->i16MaxChgCurtThd_mA) {
                pWarningFlag->IcellHighWarnFlg = eTrue;
            } else if (pBattInfoInput->i16Icell_mA <
                       pBattParamsCfg->i16MinDhgCurtThd_mA) {
                pWarningFlag->IcellLowWarnFlg = eTrue;
            }
        }
    }

#endif
}

void App_UartFrameOutput(void)
{
    uint8_t u8Temp;
    int16_t i16Temp;
    tBattGlobalParams* pBattGlobalParams;

#if OUTPUT_MODE == UART_OUTPUT
    Driver_UartInit();
    //******************Send pack data****************************//
    Driver_UartTxU16Data(0x5a5a);
    Driver_UartTxU16Data(0xa5a5);
    Driver_UartTxU16Data(gaugeApp.ui8NrOfCell);
    Driver_UartTxU32Data(gaugeApp.battPackParams.u32PackVolt_mV);
    Driver_UartTxU32Data(gaugeApp.battPackParams.i32PackCurt_mA);
    Driver_UartTxU32Data(gaugeApp.battPackParams.u32PackCusFullCap_mAh);
    Driver_UartTxU32Data(gaugeApp.battPackParams.i32PackRemCap_mAh);
    i16Temp = _IQ15int(_IQ15mpy(
        gaugeApp.battPackParams.iq15PackSmoothRltSoc_DEC, _IQ15(1000)));
    Driver_UartTxU16Data(i16Temp);
    i16Temp = _IQ15int(
        _IQ15mpy(gaugeApp.battPackParams.iq15PackSOH_DEC, _IQ15(1000)));
    Driver_UartTxU16Data(i16Temp);
    Driver_UartTxU16Data(gaugeApp.battPackParams.u16PackCycles);
    Driver_UartTxU16Data(
        *((uint16_t*) &gaugeApp.battPackParams.packWarningFlag) & WARN_FILTER);

    for (u8Temp = 0; u8Temp < gaugeApp.ui8NrOfCell; u8Temp++) {
        pBattGlobalParams = gaugeApp.pBattGlobalParamList[u8Temp];

        Driver_UartTxU16Data(
            _IQ15int(pBattGlobalParams->battInfoInput.iq15Vcell_mV));
        Driver_UartTxU16Data(pBattGlobalParams->battInfoInput.i16Icell_mA);
        Driver_UartTxU16Data(pBattGlobalParams->battInfoInput.i8Tcell_C);
        Driver_UartTxU16Data(
            _IQ15int(pBattGlobalParams->battInfoInput.iq15AvgVcell_mV));
        Driver_UartTxU16Data(pBattGlobalParams->battInfoInput.i16AvgIcell_mA);

        Driver_UartTxU16Data(0);
        i16Temp = _IQ15int(_IQ15mpy(
            pBattGlobalParams->mixingParams.iq15NomAbsSoc_DEC, _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        i16Temp = _IQ15int(_IQ15mpy(
            pBattGlobalParams->mixingParams.iq15CusRltSoc[0], _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        i16Temp = _IQ15int(
            _IQ15mpy(pBattGlobalParams->mixingParams.iq15SmoothRltSoc_DEC,
                _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        Driver_UartTxU16Data(
            pBattGlobalParams->capLearnParams.u16MaxNomFullCap_mAh);
        Driver_UartTxU16Data(
            pBattGlobalParams->capLearnParams.u16NomFullCap_mAh);
        Driver_UartTxU16Data(
            pBattGlobalParams->mixingParams.u16CusFullCap_mAh);
        Driver_UartTxU16Data(
            _IQ15int(pBattGlobalParams->capLearnParams.iq15CaliOcv_mV));
        i16Temp = _IQ15int(_IQ15mpy(
            pBattGlobalParams->capLearnParams.iq15CaliSoc_DEC, _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        i16Temp = _IQ15int(_IQ15mpy(
            pBattGlobalParams->mixingParams.iq15AbsEmptySoc, _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        Driver_UartTxU16Data(
            pBattGlobalParams->mixingParams.u8EmptySocMatrixCount[0]);
        i16Temp = _IQ15int(_IQ15mpy(
            pBattGlobalParams->mixingParams.iq15AbsFullSoc, _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        i16Temp = _IQ15int(_IQ15mpy(
            pBattGlobalParams->capLearnParams.iq15SOH_DEC, _IQ15(1000)));
        Driver_UartTxU16Data(i16Temp);
        Driver_UartTxU16Data(*((uint16_t*) &pBattGlobalParams->iGaugeParams
                                   .deltaQsum.i16Qsum_mAh));
        //        i16Temp = _IQ15int(_IQ15mpy(pBattGlobalParams->capLearnParams.iq15AbsDeltaSoc_DEC, _IQ15(1000)));
        Driver_UartTxU16Data(0);
        Driver_UartTxU16Data(pBattGlobalParams->iGaugeParams.u16DhgCycles);
        Driver_UartTxU16Data(pBattGlobalParams->battInfoInput.CellBalanceFlg);
        Driver_UartTxU16Data(
            *((uint16_t*) &pBattGlobalParams->warningFlag) & WARN_FILTER);
        Driver_UartTxU16Data(pBattGlobalParams->capLearnParams.battState);
    }

#endif

    //Disable UART
    DL_UART_Main_reset(UART_INST);
    DL_UART_Main_disablePower(UART_INST);
}
