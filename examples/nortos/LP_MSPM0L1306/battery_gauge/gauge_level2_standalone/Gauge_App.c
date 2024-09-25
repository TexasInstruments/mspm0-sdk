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

#include "Gauge_App.h"

extern uint16_t CellVoltage[];
extern float Temperature[];
extern int16_t Pack_Current;

//*****************************************************************************
// Init algorithm used in Gauge
//*****************************************************************************
void App_AlgorithmInit(tGaugeApplication *pGaugeApp)
{
    uint8_t u8BattCount;
    tBattInfoInput *pBattInfoInput;

    App_UartFrameInput();  //Accept measured data from GUI
#if (DETECTION_MODE == DETECTION_DATA_INPUT)
    BSP_CurtCalcFactorsCali(
        &pGaugeApp->curtCalcFactors);  //calibrate current detection module

    //Update average data for gauge algorithm
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        pBattInfoInput =
            &pGaugeApp->pBattGlobalParamList[u8BattCount]->battInfoInput;

        BSP_BattInfoMeasure(pBattInfoInput,
            &pGaugeApp
                 ->curtCalcFactors);  //Measure the battery for the first time

        pBattInfoInput->iq15AvgVcell_mV = pBattInfoInput->iq15Vcell_mV;
        pBattInfoInput->i8AvgTcell_C    = pBattInfoInput->i8Tcell_C;
        pBattInfoInput->i16AvgIcell_mA  = pBattInfoInput->i16Icell_mA;
        pBattInfoInput->CellBalanceFlg  = eFalse;
    }
#endif

    //Init Gauge algorithm
    CL_Init(
        pGaugeApp);  //Step1: Update Capacity learn algorithm (Init battery state)
    IGauge_Init(pGaugeApp);  //Step2: Current gauge init
    Mix_Init(pGaugeApp);     //Step3: Mixing algorithm init
    pGaugeApp->battPackParams.iq15PackSOH_DEC = _IQ15(1);
    App_UartFrameOutput();
}

//*****************************************************************************
// Algorithm Process
//*****************************************************************************
void App_AlgorithmProcess(tGaugeApplication *pGaugeApp)
{
    CL_Handler(pGaugeApp);      //Step1: Calibrate SOC and update NomSOC
    IGauge_Handler(pGaugeApp);  //Step2: Run Coulometer
    Mix_Handler(
        pGaugeApp);  //Step3: Calculate CusSoc and SmoothSoc; Clear flags
    Pack_Handler(pGaugeApp);  //Step4
}

//*****************************************************************************
// Update gauge input data
//*****************************************************************************
void App_BattInfoParamsUpdate(tGaugeApplication *pGaugeApp)
{
    int16_t i16CurtTemp;
    int16_t i16TempGap;
    uint8_t u8BattCount;
    tBattInfoInput *pBattInfoInput;
    tBattParamsConfig *pBattParamsCfg;
    tBattCapLearnParams *pCapLearnParams;
    tWarningFlags *pWarningFlag;

    BSP_SysTikCali();  //Calibrate system tick

    App_UartFrameInput();  //Accept measured data from GUI
#if (DETECTION_MODE == DETECTION_DATA_INPUT)

    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        pBattInfoInput =
            &pGaugeApp->pBattGlobalParamList[u8BattCount]->battInfoInput;
        pBattParamsCfg =
            pGaugeApp->pBattGlobalParamList[u8BattCount]->pBattParamsCfg;
        pCapLearnParams =
            &pGaugeApp->pBattGlobalParamList[u8BattCount]->capLearnParams;
        pWarningFlag =
            &pGaugeApp->pBattGlobalParamList[u8BattCount]->warningFlag;

        BSP_BattInfoMeasure(pBattInfoInput,
            &pGaugeApp
                 ->curtCalcFactors);  //Measure the battery for the first time

        //When the passive average is enabled for this battery, the current will be replaced
        if (pBattInfoInput->CellBalanceFlg == eTrue)
            pBattInfoInput->i16Icell_mA = pBattParamsCfg->i16PassiveDhgCurt_mA;

        //Update Avg used for rest and full judgment
        pBattInfoInput->i16SumTcell += pBattInfoInput->i8Tcell_C;
        pBattInfoInput->i32SumIcell += pBattInfoInput->i16Icell_mA;
        pBattInfoInput->u32SumVcell += _IQ15int(pBattInfoInput->iq15Vcell_mV);

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
        if (_IQ15int(pBattInfoInput->iq15Vcell_mV) >
            pBattParamsCfg->u16MaxBattVoltThd_mV) {
            pWarningFlag->VcellHighWarnFlg = eTrue;
        } else if (_IQ15int(pBattInfoInput->iq15Vcell_mV) <
                   pBattParamsCfg->u16MinBattVoltThd_mV) {
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
#endif
}

//*****************************************************************************
// System init
//*****************************************************************************
void App_SystemInit(tGaugeApplication *pGaugeApp)
{
    BSP_PeripheralsInit();         //Hardware init
    App_AlgorithmInit(pGaugeApp);  //Algorithm init
}

//*****************************************************************************
// Pack Init
//*****************************************************************************
void Pack_Init(tGaugeApplication *pGaugeApp)
{
    tBattPackParams *pBbattPackParams;

    pBbattPackParams = &pGaugeApp->battPackParams;

    pBbattPackParams->u16PackCycles   = 0;
    pBbattPackParams->iq15PackSOH_DEC = _IQ15(1);
}

//*****************************************************************************
// Pack Handler
//*****************************************************************************
void Pack_Handler(tGaugeApplication *pGaugeApp)
{
    uint8_t u8BattCount;
    uint16_t u16WarningFlg;
    uint16_t *u16PackWarningFlg;
    uint16_t u16PackCycles;
    _iq15 iq15PackSOH_DEC;
    tBattPackParams *pBbattPackParams;
    _iq15 iq15PackSmoothRltSoc_DEC;  //The final Soc value send to customer
    uint32_t u32PackCusFullCap_mAh;
    uint32_t i32PackRemCap_mAh;

    pBbattPackParams = &pGaugeApp->battPackParams;

    //******************Update Pack voltage and current****************************//
    pBbattPackParams->u32PackVolt_mV = 0;
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        pBbattPackParams->u32PackVolt_mV +=
            _IQ15int(pGaugeApp->pBattGlobalParamList[u8BattCount]
                         ->battInfoInput.iq15AvgVcell_mV);
        if (pGaugeApp->pBattGlobalParamList[u8BattCount]
                ->battInfoInput.CellBalanceFlg == 0) {
            pBbattPackParams->i32PackCurt_mA =
                pGaugeApp->pBattGlobalParamList[u8BattCount]
                    ->battInfoInput.i16AvgIcell_mA;
        }
    }

    //******************Update Warning Flag****************************//
    u16WarningFlg            = 0x0;
    iq15PackSmoothRltSoc_DEC = 0;
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        u16WarningFlg |=
            (*((uint16_t *) (&pGaugeApp->pBattGlobalParamList[u8BattCount]
                                  ->warningFlag)) &
                WARN_FILTER);
    }
    u16PackWarningFlg  = (uint16_t *) &pBbattPackParams->packWarningFlag;
    *u16PackWarningFlg = u16WarningFlg;

    //******************Update cycles****************************//
    //Use the max battery cycles as the pack cycles
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        u16PackCycles = pGaugeApp->pBattGlobalParamList[u8BattCount]
                            ->iGaugeParams.u16DhgCycles;
        if (pBbattPackParams->u16PackCycles < u16PackCycles) {
            pBbattPackParams->u16PackCycles = u16PackCycles;
        }
    }

    //******************Update SOH****************************//
    //Use the min battery cycles as the pack cycles
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        iq15PackSOH_DEC = pGaugeApp->pBattGlobalParamList[u8BattCount]
                              ->capLearnParams.iq15SOH_DEC;
        if (pBbattPackParams->iq15PackSOH_DEC > iq15PackSOH_DEC) {
            pBbattPackParams->iq15PackSOH_DEC = iq15PackSOH_DEC;
        }
    }

    //******************Update SmoothSoc****************************//
    //Use the min battery SmoothSoc as the pack SmoothSoc
    iq15PackSmoothRltSoc_DEC = _IQ15(1);
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        if (iq15PackSmoothRltSoc_DEC >
            pGaugeApp->pBattGlobalParamList[u8BattCount]
                ->mixingParams.iq15SmoothRltSoc_DEC) {
            iq15PackSmoothRltSoc_DEC =
                pGaugeApp->pBattGlobalParamList[u8BattCount]
                    ->mixingParams.iq15SmoothRltSoc_DEC;
        }
    }
    pBbattPackParams->iq15PackSmoothRltSoc_DEC = iq15PackSmoothRltSoc_DEC;

    //******************Update PackCusFullCap_mAh,PackSmoothRemCap_mAh****************************//
    u32PackCusFullCap_mAh = 0;
    i32PackRemCap_mAh     = 0;
    for (u8BattCount = 0; u8BattCount < pGaugeApp->ui8NrOfCell;
         u8BattCount++) {
        u32PackCusFullCap_mAh += pGaugeApp->pBattGlobalParamList[u8BattCount]
                                     ->mixingParams.u16CusFullCap_mAh;
        i32PackRemCap_mAh += pGaugeApp->pBattGlobalParamList[u8BattCount]
                                 ->mixingParams.i16CusRemCap_mAh;
    }
    pBbattPackParams->u32PackCusFullCap_mAh = u32PackCusFullCap_mAh;
    pBbattPackParams->i32PackRemCap_mAh     = i32PackRemCap_mAh;
}
