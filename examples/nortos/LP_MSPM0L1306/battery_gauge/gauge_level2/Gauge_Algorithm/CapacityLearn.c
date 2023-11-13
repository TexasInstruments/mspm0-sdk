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

#include "CapacityLearn.h"

#define CALI_UNDER_CHARGE_UNFULL
#define OCV_CALIBRATION_FACTOR \
    (1.6)  //The factor will be different at different SOC and Current
#define DELTA_SOC_CALI_THD (0.6)
#define FULLCAP_FILTER_LEVEL (2)
#define CALI_SOC_WEIGHT_BY_CYCLE (0.2)

uint16_t CL_U16ComputeIIR(
    uint16_t u16Raw, uint16_t u16Filtered, uint8_t ui8Beta);
void CL_IQ15CalcCaliSocOcv(int16_t AvgVcell, int16_t i16Icell_Num0,
    _iq15 iq15Rcell, tBattGlobalParams *pBattGlobalParams);

//*****************************************************************************
// Init battery state buffer to eCHGUnfull
//*****************************************************************************
void CL_Init(tBattGlobalParams *pBattGlobalParams)
{
    tBattCapLearnParams *pCapLearnParams;
    tBattParamsConfig *pBattParamsCfg;

    pBattParamsCfg  = pBattGlobalParams->pBattParamsCfg;
    pCapLearnParams = pBattGlobalParams->pCapLearnParams;

    pCapLearnParams->battState          = eCHGUnfull;
    pCapLearnParams->battStateRecord[2] = eCHGUnfull;
    pCapLearnParams->battStateRecord[1] = eCHGUnfull;
    pCapLearnParams->battStateRecord[0] = eCHGUnfull;

    pCapLearnParams->iq15SOH_DEC =
        pBattParamsCfg
            ->iq15DefaultSOH_DEC,  //Set the default battery health is 100%
        pCapLearnParams->u16FilteredNomFullCap_mAh =
            pBattParamsCfg->u16DesignCap_mAh;
    pCapLearnParams->u16RawNomFullCap_mAh = pBattParamsCfg->u16DesignCap_mAh;
    pCapLearnParams->u16MaxNomFullCap_mAh = 0;

    //Flags init
    pCapLearnParams->ocvCaliFlg           = 0;
    pCapLearnParams->capLearnStartFlg     = 0;
    pCapLearnParams->u16TimeExtendCounter = 0;
    pCapLearnParams->iGuageDominationFlg  = 0;
    pCapLearnParams->fullCapCaliFlg       = 0;
}

//*****************************************************************************
// Calculate calibrated SOC and OCV
//*****************************************************************************
void CL_IQ15CalcCaliSocOcv(int16_t AvgVcell, int16_t i16Icell_Num0,
    _iq15 iq15Rcell, tBattGlobalParams *pBattGlobalParams)
{
    _iq15 iq15CaliSoc_DEC;

    tBattCapLearnParams *pCapLearnParams;
    tBattParamsConfig *pBattParamsCfg;
    tIGaugeCoulometerParams *pIGaugeParams;
    tMixingParams *pMixingParams;

    pCapLearnParams = pBattGlobalParams->pCapLearnParams;
    pBattParamsCfg  = pBattGlobalParams->pBattParamsCfg;
    pIGaugeParams   = pBattGlobalParams->pIGaugeParams;
    pMixingParams   = pBattGlobalParams->pMixingParams;

    pCapLearnParams->u16CaliOcv_mV =
        AvgVcell - _IQ15int(_IQ15mpy(_IQ15(i16Icell_Num0), iq15Rcell));
    iq15CaliSoc_DEC = VGuage_BattSocSerach(pCapLearnParams->u16CaliOcv_mV,
        pBattParamsCfg->u8CircuitTableLength,
        pBattParamsCfg->pBattCircuitParams);

    //Before enter iGuageDomination, it should at least have two CaliCycleRecord
    pCapLearnParams->u16CaliCycleRecord[1] =
        pCapLearnParams->u16CaliCycleRecord[0];
    pCapLearnParams->u16CaliCycleRecord[0] = pIGaugeParams->u16DhgCycles;

    pCapLearnParams->iq15CaliSoc_DEC = iq15CaliSoc_DEC;

    pCapLearnParams->ocvCaliFlg = 1;  //Enable to enter capacity learn function
    pCapLearnParams->socCapAccumulationFlg =
        1;  //Soc and Capacity are ready for accumulation
}

//*****************************************************************************
// IIR filter
//*****************************************************************************
uint16_t CL_U16ComputeIIR(
    uint16_t u16Raw, uint16_t u16Filtered, uint8_t ui8Beta)
{
    uint16_t u16Temp;

    u16Temp = (u16Filtered - (u16Filtered >> ui8Beta)) + (u16Raw >> ui8Beta);
    return u16Temp;
}

//*****************************************************************************
//Battery capacity learn and update
//*****************************************************************************
void CL_Handler(tBattGlobalParams *pBattGlobalParams)
{
    int16_t i16PreDeltaAvgVcell_mV, i16CurDeltaAvgVcell_mV;
    uint16_t u16cyclesShift;
    _iq15 iq15DeltaSoc, iq15DeltaQsum;
    tBattParamsConfig *pBattParamsCfg;
    tBattCapLearnParams *pCapLearnParams;
    tBattInfoInput *pBattInfoInput;
    tIGaugeCoulometerParams *pIGaugeParams;
    tVGaugeFilterParams *pVGaugeParams;

    pBattInfoInput  = pBattGlobalParams->pBattInfoInput;
    pBattParamsCfg  = pBattGlobalParams->pBattParamsCfg;
    pCapLearnParams = pBattGlobalParams->pCapLearnParams;
    pIGaugeParams   = pBattGlobalParams->pIGaugeParams;
    pVGaugeParams   = pBattGlobalParams->pVGaugeParams;

    //Update Capacity learn when the average data is prepared
    //Update frequency = sysTikFreq * u8AvgBattParamsUpdateCount
    if (pBattInfoInput->avgBattParamsUpdateFlg == 1) {
        //******************************Update battery state*********************************************//
        //Current above discharge current limit
        if (pBattInfoInput->i16AvgIcell_mA <
            pBattParamsCfg->i16UnloadCurtLowThd_mA) {
            pCapLearnParams->battState = eDHG;
        }
        //Current above Charge current limit
        else if (pBattInfoInput->i16AvgIcell_mA >
                 pBattParamsCfg->i16UnloadCurtHighThd_mA) {
            //When the charge current drops to lower than i16FullChgCurtThd_mA and the voltage
            //keeps higher than the full Ocv, we assume here it is charged to full
            if (pBattInfoInput->i16AvgIcell_mA <=
                    pBattParamsCfg->i16FullChgCurtThd_mA &&
                pBattInfoInput->u16AvgVcell_mV >=
                    pBattParamsCfg->u16MinFullChgVoltThd_mV) {
                pCapLearnParams->battState = eCHGFull;
            } else {
                pCapLearnParams->battState = eCHGUnfull;
            }
        } else {
            pCapLearnParams->battState = eUnload;
        }

        //******************************Get calibrated OCV and NomSoc*********************************************//
        //Update the battState[0] and battState[1] when the battState changes
        //Ensure there previous state will not equals to current state
        if (pCapLearnParams->battStateRecord[0] !=
            pCapLearnParams->battState) {
            pCapLearnParams->battStateRecord[2] =
                pCapLearnParams->battStateRecord[1];
            pCapLearnParams->battStateRecord[1] =
                pCapLearnParams->battStateRecord[0];
            pCapLearnParams->battStateRecord[0] = pCapLearnParams->battState;
        }

        //Keep run the OCV detection when:
        //Previous state = eCHGFull/eDHG/eCHGunFull, Current state = eUnload
        if (pCapLearnParams->battStateRecord[0] == eUnload) {
            //Ensure big voltage gap by adding long waiting time
            //Update frequency = sysFrq*u8AvgVcellDetectPeriod*u8AvgBattParamsUpdateCount
            if (pCapLearnParams->u16TimeExtendCounter == 0) {
                pCapLearnParams->u16AvgVcellRecord[2] =
                    pCapLearnParams->u16AvgVcellRecord[1];
                pCapLearnParams->u16AvgVcellRecord[1] =
                    pCapLearnParams->u16AvgVcellRecord[0];
                pCapLearnParams->u16AvgVcellRecord[0] =
                    pBattInfoInput->u16AvgVcell_mV;

                //u16AvgVcellRecord will be init in eCHGUnfull, eDCHG
                //Enter the eUnload up to 3 times, then do further calculation
                if (pCapLearnParams->u16AvgVcellRecord[2] != 0) {
                    i16PreDeltaAvgVcell_mV =
                        (int16_t) pCapLearnParams->u16AvgVcellRecord[1] -
                        (int16_t) pCapLearnParams->u16AvgVcellRecord[2];
                    i16CurDeltaAvgVcell_mV =
                        (int16_t) pCapLearnParams->u16AvgVcellRecord[0] -
                        (int16_t) pCapLearnParams->u16AvgVcellRecord[1];

                    //When all the delta voltage changed in the range, update OCV and SOC
                    //Update the battery rest state
                    if (pCapLearnParams->battStateRecord[1] == eCHGFull ||
                        pCapLearnParams->battStateRecord[1] ==
                            eCHGUnfull)  //Charge to full an reset
                    {
                        if (i16PreDeltaAvgVcell_mV >
                                -pBattParamsCfg
                                     ->u16DeltaAvgVellForCaliThd_mV &&
                            i16CurDeltaAvgVcell_mV >
                                -pBattParamsCfg
                                     ->u16DeltaAvgVellForCaliThd_mV) {
                            CL_IQ15CalcCaliSocOcv((int16_t) pCapLearnParams
                                                      ->u16AvgVcellRecord[0],
                                pBattInfoInput->i16AvgIcell_mA +
                                    pBattParamsCfg
                                        ->i16AvgLeckageCurt_mA,  //Add lead current compensation
                                pVGaugeParams->iq15CalRcell_R,
                                pBattGlobalParams);
                        }
                    } else if (pCapLearnParams->battStateRecord[1] ==
                               eDHG)  //DHG current is 0 or not 0
                    {
                        if (i16PreDeltaAvgVcell_mV <
                                pBattParamsCfg->u16DeltaAvgVellForCaliThd_mV &&
                            i16CurDeltaAvgVcell_mV <
                                pBattParamsCfg->u16DeltaAvgVellForCaliThd_mV) {
                            //We assume there is a low constant current load
                            if (i16CurDeltaAvgVcell_mV < 0) {
                                pCapLearnParams->lowDhgOcvCaliFlg =
                                    1;  //Tell the Mixing algorithm, this is a calibration under low current DHG
                            }
                            CL_IQ15CalcCaliSocOcv((int16_t) pCapLearnParams
                                                      ->u16AvgVcellRecord[0],
                                pBattInfoInput->i16AvgIcell_mA +
                                    pBattParamsCfg
                                        ->i16AvgLeckageCurt_mA,  //Add lead current compensation
                                pVGaugeParams->iq15CalRcell_R,
                                pBattGlobalParams);
                        }
                    }

                    //When all the delta voltage changed in the range, update OCV and SOC
                    if (i16PreDeltaAvgVcell_mV <
                            pBattParamsCfg->u16DeltaAvgVellForCaliThd_mV &&
                        i16CurDeltaAvgVcell_mV <
                            pBattParamsCfg->u16DeltaAvgVellForCaliThd_mV &&
                        i16PreDeltaAvgVcell_mV >
                            -pBattParamsCfg->u16DeltaAvgVellForCaliThd_mV &&
                        i16CurDeltaAvgVcell_mV >
                            -pBattParamsCfg->u16DeltaAvgVellForCaliThd_mV) {
                        //Update the battery rest state
                        if (pCapLearnParams->battStateRecord[1] ==
                            eCHGFull)  //Charge to full an reset
                        {
                            CL_IQ15CalcCaliSocOcv((int16_t) pCapLearnParams
                                                      ->u16AvgVcellRecord[0],
                                pBattInfoInput->i16AvgIcell_mA +
                                    pBattParamsCfg
                                        ->i16AvgLeckageCurt_mA,  //Add lead current compensation
                                pVGaugeParams->iq15CalRcell_R,
                                pBattGlobalParams);
                        }
#ifdef CALI_UNDER_CHARGE_UNFULL
                        else if (pCapLearnParams->battStateRecord[1] ==
                                 eCHGUnfull)  //CHG current is 0 or not 0
                        {
                            CL_IQ15CalcCaliSocOcv((int16_t) pCapLearnParams
                                                      ->u16AvgVcellRecord[0],
                                pBattInfoInput->i16AvgIcell_mA +
                                    pBattParamsCfg
                                        ->i16AvgLeckageCurt_mA,  //Add lead current compensation
                                pVGaugeParams->iq15CalRcell_R,
                                pBattGlobalParams);
                        }
#endif
                        else if (pCapLearnParams->battStateRecord[1] ==
                                 eDHG)  //DHG current is 0 or not 0
                        {
                            //We assume there is a low constant current load
                            if (i16CurDeltaAvgVcell_mV < 0) {
                                pCapLearnParams->lowDhgOcvCaliFlg =
                                    1;  //Tell the Mixing algorithm, this is a calibration under low current DHG
                            }

                            CL_IQ15CalcCaliSocOcv((int16_t) pCapLearnParams
                                                      ->u16AvgVcellRecord[0],
                                pBattInfoInput->i16AvgIcell_mA +
                                    pBattParamsCfg
                                        ->i16AvgLeckageCurt_mA,  //Add lead current compensation
                                pVGaugeParams->iq15CalRcell_R,
                                pBattGlobalParams);
                        }
                    }
                }
            }

            //Update TimeCounter
            pCapLearnParams->u16TimeExtendCounter++;
            if (pCapLearnParams->u16TimeExtendCounter >=
                pBattParamsCfg->u8AvgVcellDetectPeriod) {
                pCapLearnParams->u16TimeExtendCounter = 0;
            }
        }

        //******************************Get NomFullCap and SOH*********************************************//
        //Update the capacity learn parameters when get out from rest andpPrevious state != unload and not charge to rest
        //When first come out from the rest status and get the OCV, do the capacity learn functions
        else if (pCapLearnParams->battStateRecord[1] == eUnload &&
#ifndef CALI_UNDER_CHARGE_UNFULL
                 pCapLearnParams->battStateRecord[2] != eCHGUnfull &&
#endif
                 pCapLearnParams->socCapAccumulationFlg == 1) {

            //Update current SocSum and QSum
            pCapLearnParams->iq15CaliAbsSocRecord[1] =
                pCapLearnParams->iq15CaliAbsSocRecord[0];
            pCapLearnParams->iq15CaliAbsSocRecord[0] =
                pCapLearnParams->iq15CaliSoc_DEC;
            //The system has get the second real OCV and run into this judgment for at least 2 times
            if (pCapLearnParams->capLearnStartFlg == 1) {
                iq15DeltaSoc = pCapLearnParams->iq15CaliAbsSocRecord[0] -
                               pCapLearnParams->iq15CaliAbsSocRecord[1];
                iq15DeltaQsum = IGauge_UpdateDeltaQsum(pIGaugeParams);

                if (iq15DeltaSoc > _IQ15(0)) {
                    pCapLearnParams->iq15SumAbsDeltaSoc_DEC += iq15DeltaSoc;
                    pCapLearnParams->iq15SumDeltaCap_mAh += iq15DeltaQsum;
                } else {
                    pCapLearnParams->iq15SumAbsDeltaSoc_DEC -= iq15DeltaSoc;
                    pCapLearnParams->iq15SumDeltaCap_mAh -= iq15DeltaQsum;
                }

                //When Soc above 60%, do capacity calibration and calculate SOH
                if (pCapLearnParams->iq15SumAbsDeltaSoc_DEC >
                    _IQ15(DELTA_SOC_CALI_THD)) {
                    pCapLearnParams->u16RawNomFullCap_mAh =
                        _IQ15int(_IQ15div(pCapLearnParams->iq15SumDeltaCap_mAh,
                            pCapLearnParams->iq15SumAbsDeltaSoc_DEC));
                    pCapLearnParams->iq15SumDeltaCap_mAh    = _IQ15(0);
                    pCapLearnParams->iq15SumAbsDeltaSoc_DEC = _IQ15(0);

                    pCapLearnParams->DHGCycleRecord[1] =
                        pCapLearnParams->DHGCycleRecord[0];
                    pCapLearnParams->DHGCycleRecord[0] =
                        pIGaugeParams->u16DhgCycles;

                    //Update SOH  and NomFullCap_mAh
                    //It is the first time to get the NomFullCap_mAh
                    if (pCapLearnParams->iGuageDominationFlg == 0) {
                        //Update MixFullcap
                        pCapLearnParams->u16FilteredNomFullCap_mAh =
                            pCapLearnParams->u16RawNomFullCap_mAh;

                    } else {
                        //Update NomFullcap, using IIR filter
                        pCapLearnParams->u16FilteredNomFullCap_mAh =
                            CL_U16ComputeIIR(
                                pCapLearnParams->u16RawNomFullCap_mAh,
                                pCapLearnParams->u16FilteredNomFullCap_mAh,
                                FULLCAP_FILTER_LEVEL);
                        u16cyclesShift = pCapLearnParams->DHGCycleRecord[0] -
                                         pCapLearnParams->DHGCycleRecord[1];
                        if (u16cyclesShift >
                            (pBattParamsCfg->u8SOHCalcCycleThd - 1)) {
                            pCapLearnParams->iq15SOH_DEC = _IQ15div(
                                _IQ15(pCapLearnParams->u16RawNomFullCap_mAh),
                                _IQ15(pCapLearnParams->u16MaxNomFullCap_mAh));
                        }
                    }

                    //Update MaxAbsFullCap
                    if (pCapLearnParams->u16MaxNomFullCap_mAh <
                        pCapLearnParams->u16FilteredNomFullCap_mAh) {
                        pCapLearnParams->u16MaxNomFullCap_mAh =
                            pCapLearnParams->u16FilteredNomFullCap_mAh;
                    }

                    pCapLearnParams->iGuageDominationFlg =
                        1;  //Tell Mixing SOC calculation change from VGauge to IGauge
                    pCapLearnParams->fullCapCaliFlg =
                        1;  //Tell Mixing the FullCap finish calibration
                }
            }
            pCapLearnParams->capLearnStartFlg =
                1;  // Tell CurtGauge to start record deltaQsum
            pCapLearnParams->socCapAccumulationFlg = 0;
        } else {
            //Init parameters
            pCapLearnParams->u16TimeExtendCounter = 0;
            pCapLearnParams->u16AvgVcellRecord[0] = 0;
            pCapLearnParams->u16AvgVcellRecord[1] = 0;
            pCapLearnParams->u16AvgVcellRecord[2] = 0;
        }
    }
}
