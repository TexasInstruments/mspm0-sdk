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

#include "Gauge_Algorithm/Mixing.h"

#define CONVERGE_STRAT_CURT_TIMES (2)
#define CONVERGE_VOLT_STEP_MV (5)
#define CONVERGE_START_VOLT_MV \
    (3300)  //When the run time voltage below than this parameter,the converging algorithm will start to use
#define SOC_GAP (0.002)
#define FULL_CURRENT_STEP_SHIFT (4)  //Divide by 16
#define FULL_VOLTAGE_SHIFT_LIMIT (-5)
#define EMPTY_CURRENT_DIVIDER (4)  //Divide by 4

//*****************************************************************************
// Init Mixing function
//*****************************************************************************
void Mix_Init(tBattGlobalParams *pBattGlobalParams)
{
    uint8_t u8Count;
    _iq15 iq15DefaultFullSoc;
    _iq15 iq15DefaultEmptySoc;
    tMixingParams *pMixingParams;
    tBattParamsConfig *pBattParamsCfg;
    tBattInfoInput *pBattInfoInput;

    pMixingParams  = pBattGlobalParams->pMixingParams;
    pBattParamsCfg = pBattGlobalParams->pBattParamsCfg;
    pBattInfoInput = pBattGlobalParams->pBattInfoInput;

    //Init NomFullCap and NomAbsSoc
    pMixingParams->u16NomFullCap_mAh = pBattParamsCfg->u16DesignCap_mAh;
    pMixingParams->iq15NomAbsSoc_DEC = VGuage_BattSocSerach(
        pBattInfoInput->u16Vcell_mV, pBattParamsCfg->u8CircuitTableLength,
        pBattParamsCfg->pBattCircuitParams);

    //Init FullSoc
    iq15DefaultFullSoc =
        VGuage_BattSocSerach(pBattParamsCfg->u16MaxFullChgVoltThd_mV,
            pBattParamsCfg->u8CircuitTableLength,
            pBattParamsCfg->pBattCircuitParams);
    //For VGauge domination AbsFullSoc will only be updated with MixAbsFullSoc
    //For IGuage domination AbsFullSoc will be updated with MixAbsFullSoc and AbsFullSocMatrix
    pMixingParams->iq15AbsFullSoc[1] = iq15DefaultFullSoc;
    pMixingParams->iq15AbsFullSoc[0] = iq15DefaultFullSoc;

    //Init EmptySoc
    iq15DefaultEmptySoc =
        VGuage_BattSocSerach(pBattParamsCfg->u16EmptyDhgVoltThd_mV,
            pBattParamsCfg->u8CircuitTableLength,
            pBattParamsCfg->pBattCircuitParams);
    for (u8Count = 0; u8Count < CT_LEGTH; u8Count++) {
        if (pBattParamsCfg->iq15AbsEmptySocMatrixInput[u8Count] != 0) {
            pMixingParams->iq15AbsEmptySocMatrix[0][u8Count] =
                pBattParamsCfg->iq15AbsEmptySocMatrixInput[u8Count];
        } else {
            pMixingParams->iq15AbsEmptySocMatrix[0][u8Count] =
                iq15DefaultEmptySoc;
        }
    }
    //For VGauge domination AbsEmptySoc will only be updated with MixAbsEmptySoc
    //For IGuage domination AbsEmptySoc will be updated with MixAbsEmptySoc and AbsEmptySocMatrix
    pMixingParams->iq15AbsEmptySoc[1] = iq15DefaultEmptySoc;
    pMixingParams->iq15AbsEmptySoc[0] = iq15DefaultEmptySoc;

    //Init CusSoc
    pMixingParams->iq15CusAbsFullSocRange_DEC =
        pMixingParams->iq15AbsFullSoc[0] - pMixingParams->iq15AbsEmptySoc[0];
    pMixingParams->iq15CusRltSoc[0] = _IQ15div(
        pMixingParams->iq15NomAbsSoc_DEC - pMixingParams->iq15AbsEmptySoc[0],
        pMixingParams->iq15CusAbsFullSocRange_DEC);
    //Init SmoothSoc
    pMixingParams->iq15SmoothRltSoc_DEC = pMixingParams->iq15CusRltSoc[0];
}

//*****************************************************************************
// Mixing algorithm handler
//*****************************************************************************
void Mix_Handler(tBattGlobalParams *pBattGlobalParams)
{
    uint8_t u8YCount;
    uint8_t u8XCount;
    uint8_t u8Count;
    int16_t i16ConvergeStartCurt;
    int16_t i16ConvergeShiftCurtStep;
    int16_t i16VoltGap;
    int16_t i16CurtGap;
    _iq15 iq15TempWeight_DEC;
    _iq15 iq15EvalAbsFullSoc_DEC;
    _iq15 iq15MixAbsFullSoc_DEC;
    _iq15 iq15EvalAbsEmptySoc_DEC;
    _iq15 iq15MixAbsEmptySoc_DEC;
    _iq15 iq15DeltaSoc_DEC;

    tVGaugeFilterParams *pVGaugeParams;
    tBattParamsConfig *pBattParamsCfg;
    tMixingParams *pMixingParams;
    tBattCapLearnParams *pCapLearnParams;
    tBattInfoInput *pBattInfoInput;
    tIGaugeCoulometerParams *pIGaugeParams;

    pBattInfoInput  = pBattGlobalParams->pBattInfoInput;
    pVGaugeParams   = pBattGlobalParams->pVGaugeParams;
    pBattParamsCfg  = pBattGlobalParams->pBattParamsCfg;
    pMixingParams   = pBattGlobalParams->pMixingParams;
    pCapLearnParams = pBattGlobalParams->pCapLearnParams;
    pIGaugeParams   = pBattGlobalParams->pIGaugeParams;

    //******************************Update u16NomFullCap_mAh*********************************************//
    if (pCapLearnParams->fullCapCaliFlg ==
        1)  //It is the first time enter capacity calibration
    {
        //Update MixFullcap
        pMixingParams->u16NomFullCap_mAh =
            pCapLearnParams->u16FilteredNomFullCap_mAh;
    }

    //******************************Update NomAbsSoc*********************************************//
    //The battery dedicated NomFullCap is not created, use VGauge for Soc calculation
    if (pCapLearnParams->iGuageDominationFlg == 0) {
        pMixingParams->iq15NomAbsSoc_DEC = pVGaugeParams->iq15VFAbsSoc_DEC;
        pMixingParams->i16Mix_NomRemCap_mAh =
            _IQ15int(_IQ15mpy(pMixingParams->iq15NomAbsSoc_DEC,
                _IQ15(pMixingParams->u16NomFullCap_mAh)));
    }
    //The battery dedicated NomFullCap is created, use IGauge for Soc calculation
    else {
        //NomFullCap has been updated in the CapacityLerarn function
        pMixingParams->i16Mix_NomRemCap_mAh =
            pIGaugeParams->i16IF_NomRemCap_mAh;
        pMixingParams->iq15NomAbsSoc_DEC =
            _IQ15div(_IQ15(pMixingParams->i16Mix_NomRemCap_mAh),
                _IQ15(pMixingParams->u16NomFullCap_mAh));
    }

    //******************Update Count used for FullSoc and emptySoc evaluation *************************//
    //Get count for Y axis
    for (u8Count = 0; u8Count < CT_Y_THD_NUM; u8Count++) {
        if (pBattInfoInput->i8AvgTcell_C >=
            pBattParamsCfg->i8TempThd_C[u8Count]) {
            u8YCount = u8Count;
        }
    }
    if (pBattInfoInput->i8AvgTcell_C <
        pBattParamsCfg->i8TempThd_C[CT_Y_THD_NUM - 1]) {
        u8YCount = CT_Y_THD_NUM;
    }

    //Get count for Y axis
    for (u8Count = 0; u8Count < CT_X_THD_NUM; u8Count++) {
        if (pBattInfoInput->i16AvgIcell_mA >=
            pBattParamsCfg->i16CurtThd_mA[u8Count]) {
            u8XCount = u8Count;
            break;
        }
    }
    if (pBattInfoInput->i16AvgIcell_mA <
        pBattParamsCfg->i16CurtThd_mA[CT_X_THD_NUM - 1]) {
        u8XCount = CT_X_THD_NUM;
    }

    //******************************Update AbsFullSoc*********************************************//
    pMixingParams->iq15AbsFullSoc[1] = pMixingParams->iq15AbsFullSoc[0];
    i16ConvergeStartCurt =
        pBattParamsCfg->i16FullChgCurtThd_mA * CONVERGE_STRAT_CURT_TIMES;
    //When battery goes to constant voltage charge, AbsFullSoc will be updated

    if (pCapLearnParams->battStateRecord[0] == eUnload &&
        pCapLearnParams->battStateRecord[1] == eCHGFull) {
        if (pCapLearnParams->ocvCaliFlg == 1) {
            //We assume the full capacity is getting smaller and smaller
            if (pMixingParams->iq15AbsFullSoc[0] >
                pCapLearnParams->iq15CaliSoc_DEC) {
                pMixingParams->iq15AbsFullSoc[0] =
                    pCapLearnParams->iq15CaliSoc_DEC;
            }
        }
    }

    //******************************Update AbsEmptySoc*********************************************//
    pMixingParams->iq15AbsEmptySoc[1] = pMixingParams->iq15AbsEmptySoc[0];
    pMixingParams->iq15AbsEmptySoc[0] =
        pMixingParams
            ->iq15AbsEmptySocMatrix[0][pMixingParams->u8EmptySocMatrixCount];
    pMixingParams->u8EmptySocMatrixCount =
        u8YCount * (CT_X_THD_NUM + 1) + u8XCount;

    //AbsEmptySoc only changed in DHG
    if (pCapLearnParams->battState == eDHG) {
        if (pBattInfoInput->u16AvgVcell_mV >
                pBattParamsCfg->u16MinBattVoltThd_mV &&
            pBattInfoInput->u16AvgVcell_mV <
                pBattParamsCfg->u16EmptyDhgVoltThd_mV) {
            //When it changes from VGauge to IGauge
            if (pCapLearnParams->iGuageDominationFlg == 1) {
                //Update the backup matrix if the emptySOC is smaller than current NomAbsSoc
                if (pMixingParams->iq15AbsEmptySocMatrix
                        [0][pMixingParams->u8EmptySocMatrixCount] <
                    pMixingParams->iq15NomAbsSoc_DEC) {
                    pMixingParams->iq15AbsEmptySocMatrix
                        [1][pMixingParams->u8EmptySocMatrixCount] =
                        pMixingParams->iq15NomAbsSoc_DEC;
                }
            }
        }
    } else {
        //Update Matrix when in charge or rest state
        for (u8Count = 0; u8Count < CT_LEGTH; u8Count++) {
            if (pMixingParams->iq15AbsEmptySocMatrix[0][u8Count] <
                pMixingParams->iq15AbsEmptySocMatrix[1][u8Count])
                pMixingParams->iq15AbsEmptySocMatrix[0][u8Count] =
                    pMixingParams->iq15AbsEmptySocMatrix[1][u8Count];
        }
    }
    //******************************Update CusAbsSoc*********************************************//
    pMixingParams->iq15CusAbsFullSocRange_DEC =
        pMixingParams->iq15AbsFullSoc[0] - pMixingParams->iq15AbsEmptySoc[0];

    iq15DeltaSoc_DEC =
        pMixingParams->iq15CusRltSoc[1] - pMixingParams->iq15CusRltSoc[0];
    if (iq15DeltaSoc_DEC < _IQ15(-SOC_GAP) ||
        iq15DeltaSoc_DEC > _IQ15(SOC_GAP)) {
        pMixingParams->iq15CusRltSoc[1] = pMixingParams->iq15CusRltSoc[0];
    }

    pMixingParams->iq15CusRltSoc[0] = _IQ15div(
        pMixingParams->iq15NomAbsSoc_DEC - pMixingParams->iq15AbsEmptySoc[0],
        pMixingParams->iq15CusAbsFullSocRange_DEC);
    pMixingParams->u16CusFullCap_mAh =
        _IQ15int(_IQ15mpy(_IQ15(pMixingParams->u16NomFullCap_mAh),
            pMixingParams->iq15CusAbsFullSocRange_DEC));
    pMixingParams->i16CusRemCap_mAh =
        _IQ15int(_IQ15mpy(pMixingParams->iq15CusRltSoc[0],
            _IQ15(pMixingParams->u16CusFullCap_mAh)));

    //******************************Update SmoothSoc*********************************************//
    //Keep SmoothSoc to be same in calibration mode except low discharge calibration
    if ((pCapLearnParams->ocvCaliFlg == 0 ||
            pCapLearnParams->lowDhgOcvCaliFlg == 1) &&
        pCapLearnParams->fullCapCaliFlg == 0) {
        // Avoid the divisor to be 0
        if (pMixingParams->iq15CusRltSoc[0] > _IQ15(0.999)) {
            pMixingParams->iq15SmoothRltSoc_DEC = _IQ15(1);
        }
        //Avoid the divisor to be 0
        else if (pMixingParams->iq15CusRltSoc[0] < _IQ15(0.001)) {
            pMixingParams->iq15SmoothRltSoc_DEC = _IQ15(0);
        } else {
            if (pBattInfoInput->u16AvgVcell_mV >
                pBattParamsCfg->u16MinBattVoltThd_mV) {
                //Avoid SmoothSoc updated when FullSoc or EmptySoc Change
                if (pMixingParams->iq15AbsFullSoc[1] ==
                        pMixingParams->iq15AbsFullSoc[0] &&
                    pMixingParams->iq15AbsEmptySoc[1] ==
                        pMixingParams->iq15AbsEmptySoc[0]) {
                    //SOC is going up
                    if (pMixingParams->iq15CusRltSoc[0] -
                            pMixingParams->iq15CusRltSoc[1] >
                        _IQ15(SOC_GAP)) {
                        //Avoid pMixingParams->iq15CusRltSoc[1] = 1
                        pMixingParams->iq15SmoothRltSoc_DEC =
                            VGauge_IQ15CalMidNum(
                                pMixingParams->iq15CusRltSoc[0], _IQ15(1),
                                pMixingParams->iq15CusRltSoc[1], _IQ15(1),
                                pMixingParams->iq15SmoothRltSoc_DEC);
                    } else if (pMixingParams->iq15CusRltSoc[0] -
                                   pMixingParams->iq15CusRltSoc[1] <
                               _IQ15(-SOC_GAP)) {
                        //Avoid pMixingParams->iq15CusRltSoc[1] = 0
                        pMixingParams->iq15SmoothRltSoc_DEC =
                            VGauge_IQ15CalMidNum(
                                pMixingParams->iq15CusRltSoc[0],
                                pMixingParams->iq15CusRltSoc[1], _IQ15(0),
                                pMixingParams->iq15SmoothRltSoc_DEC, _IQ15(0));
                    }
                }
            } else {
                pMixingParams->iq15SmoothRltSoc_DEC = 0;
            }
        }
    }

    //******************************Clear the global flags generated after finishing one algorithm cycle*******************************//
    pCapLearnParams->ocvCaliFlg            = 0;
    pBattInfoInput->avgBattParamsUpdateFlg = 0;
    pCapLearnParams->fullCapCaliFlg        = 0;
    pCapLearnParams->lowDhgOcvCaliFlg      = 0;
}
