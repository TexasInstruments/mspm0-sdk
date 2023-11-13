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

#include "Gauge_Algorithm/CurtGauge.h"

void IGauge_CaliNomRemCap(_iq15 iq15Soc_DEC, uint16_t NomFullCap_mAh,
    tIGaugeCoulometerParams *pIGaugeParams);
void IGauge_CoulombCountUpdate(
    tQsum *pQsum, tQsumType Qsumtype, tBattGlobalParams *pBattGlobalParams);

//*****************************************************************************
// Calibrate the normalized Remain Cap
//*****************************************************************************
void IGauge_CaliNomRemCap(_iq15 iq15Soc_DEC, uint16_t NomFullCap_mAh,
    tIGaugeCoulometerParams *pIGaugeParams)
{
    _iq15 iq15NomRemCap_mAh;

    //u16NomFullCap_mAh has already been updated in CL_Handler
    iq15NomRemCap_mAh = _IQ15mpy(_IQ15(NomFullCap_mAh), iq15Soc_DEC);
    pIGaugeParams->i16IF_NomRemCap_mAh = _IQ15int(iq15NomRemCap_mAh);
    //Update TempQsum
    pIGaugeParams->tempQsum.i16Qsum_mAh = 0;
    pIGaugeParams->tempQsum.i16Qsum_mAs = _IQ15int(
        _IQ15mpy(iq15NomRemCap_mAh - _IQ15(pIGaugeParams->i16IF_NomRemCap_mAh),
            _IQ15(3600)));
    pIGaugeParams->tempQsum.i16Qsum_mA = 0;
}

//*****************************************************************************
// Update CoulombCount parameters
//*****************************************************************************
void IGauge_CoulombCountUpdate(
    tQsum *pQsum, tQsumType Qsumtype, tBattGlobalParams *pBattGlobalParams)
{
    tQsum *pTempQsum = pQsum;
    tBattInfoInput *pBattInfoInput;
    tBattParamsConfig *pBattParamsCfg;

    pBattInfoInput = pBattGlobalParams->pBattInfoInput;
    pBattParamsCfg = pBattGlobalParams->pBattParamsCfg;

    if (pBattInfoInput->i16Icell_mA > 0 &&
        Qsumtype !=
            eDhgQsum)  // When Icell>0, it will not calculated into DhgQsum
        pTempQsum->i16Qsum_mA += pBattInfoInput->i16Icell_mA;
    if (pBattInfoInput->i16Icell_mA < 0 &&
        Qsumtype !=
            eChgQsum)  // When Icell<0, it will not calculated into ChgQsum
        pTempQsum->i16Qsum_mA += pBattInfoInput->i16Icell_mA;

    //Fit for different sysFreq
    while (pTempQsum->i16Qsum_mA > pBattParamsCfg->u8SysTikShift) {
        pTempQsum->i16Qsum_mA -= pBattParamsCfg->u8SysTikShift;
        pTempQsum->i16Qsum_mAs += (pBattParamsCfg->sysTikFreq);
    }

    while (pTempQsum->i16Qsum_mA < -pBattParamsCfg->u8SysTikShift) {
        pTempQsum->i16Qsum_mA += pBattParamsCfg->u8SysTikShift;
        pTempQsum->i16Qsum_mAs -= (pBattParamsCfg->sysTikFreq);
    }

    while (pTempQsum->i16Qsum_mAs > 3600) {
        pTempQsum->i16Qsum_mAs -= 3600;
        pTempQsum->i16Qsum_mAh += 1;
    }

    while (pTempQsum->i16Qsum_mAs < -3600) {
        pTempQsum->i16Qsum_mAs += 3600;
        pTempQsum->i16Qsum_mAh -= 1;
    }
}

//*****************************************************************************
// Update DeltaQsum used in capacity learn function
//*****************************************************************************
_iq15 IGauge_UpdateDeltaQsum(tIGaugeCoulometerParams *pIGaugeParams)
{
    _iq15 iq15DeltaQsum;

    //Use backupDeltaQsum instead of DeltaQsum is to avoid the unwanted added DeltaQsum
    //When it gets out of rest to run mode.
    pIGaugeParams->deltaQsum.i16Qsum_mA -=
        pIGaugeParams->backupDeltaQsum.i16Qsum_mA;
    pIGaugeParams->deltaQsum.i16Qsum_mAs -=
        pIGaugeParams->backupDeltaQsum.i16Qsum_mAs;
    pIGaugeParams->deltaQsum.i16Qsum_mAh -=
        pIGaugeParams->backupDeltaQsum.i16Qsum_mAh;

    //As FullCap is calculated when battery state changes, need to use backupDeltaQsum
    //to avoid the error caused from the CHG or DHG current
    iq15DeltaQsum = _IQ15(pIGaugeParams->backupDeltaQsum.i16Qsum_mAh) +
                    _IQ15div(_IQ15(pIGaugeParams->backupDeltaQsum.i16Qsum_mAs),
                        _IQ15(3600));
    return iq15DeltaQsum;
}

//*****************************************************************************
// IGauge Handler
//*****************************************************************************
void IGauge_Handler(tBattGlobalParams *pBattGlobalParams)
{
    int16_t i16CycleGap;
    int16_t i16LeakCurtComp;
    tIGaugeCoulometerParams *pIGaugeParams;
    tBattParamsConfig *pBattParamsCfg;
    tBattCapLearnParams *pCapLearnParams;
    tWarningFlags *pWarningFlag;

    pIGaugeParams   = pBattGlobalParams->pIGaugeParams;
    pBattParamsCfg  = pBattGlobalParams->pBattParamsCfg;
    pWarningFlag    = pBattGlobalParams->pWarningFlag;
    pCapLearnParams = pBattGlobalParams->pCapLearnParams;

    IGauge_CoulombCountUpdate(
        &pIGaugeParams->tempQsum, eTempQsum, pBattGlobalParams);
    IGauge_CoulombCountUpdate(&pIGaugeParams->chgQsum, eChgQsum,
        pBattGlobalParams);  //actually, not used in the following algorithm
    IGauge_CoulombCountUpdate(
        &pIGaugeParams->dhgQsum, eDhgQsum, pBattGlobalParams);
    IGauge_CoulombCountUpdate(
        &pIGaugeParams->deltaQsum, eDeltaQsum, pBattGlobalParams);

    //Do leakage current compensation
    i16LeakCurtComp = pBattParamsCfg->i16AvgLeckageCurt_mA *
                      pBattParamsCfg->sysTikFreq /
                      pBattParamsCfg->u8SysTikShift;
    pIGaugeParams->dhgQsum.i16Qsum_mAs += i16LeakCurtComp;
    pIGaugeParams->tempQsum.i16Qsum_mAs += i16LeakCurtComp;
    pIGaugeParams->deltaQsum.i16Qsum_mAs += i16LeakCurtComp;

    //Update Absolute Remain Capacity
    //Update BackupDeltaQsum, prepare for capacity learn
    if (pCapLearnParams->ocvCaliFlg == 1) {
        //Calibrate the NomRemCap according to the detected OCV
        //As the OCV is correlated with current RemCap, need to update the tempQsum as well
        IGauge_CaliNomRemCap(pCapLearnParams->iq15CaliSoc_DEC,
            pCapLearnParams->u16FilteredNomFullCap_mAh, pIGaugeParams);

        pIGaugeParams->backupDeltaQsum = pIGaugeParams->deltaQsum;

        //1. Init the deltaQsum before OCV is calibrated for the first time.
        //2. Stop init deltaQsum after getting the first calibrated OCV and going from rest to run mode.
        //   because we want the DeltaQsum and DeltaSoc get more close to the real one
        if (pCapLearnParams->capLearnStartFlg == 0) {
            pIGaugeParams->deltaQsum.i16Qsum_mA  = 0;
            pIGaugeParams->deltaQsum.i16Qsum_mAs = 0;
            pIGaugeParams->deltaQsum.i16Qsum_mAh = 0;
        }
    }

    //As the OCV is correlated with last times current RemCap, update the tempQsum and add the deltaQsum
    if (pCapLearnParams->fullCapCaliFlg == 1) {
        //Update RemCap when AbsFullCap changes, using the iq15CaliSoc_DEC for the second time
        IGauge_CaliNomRemCap(pCapLearnParams->iq15CaliSoc_DEC,
            pCapLearnParams->u16FilteredNomFullCap_mAh, pIGaugeParams);

        pIGaugeParams->tempQsum.i16Qsum_mA +=
            pIGaugeParams->deltaQsum.i16Qsum_mA;
        pIGaugeParams->tempQsum.i16Qsum_mAs +=
            pIGaugeParams->deltaQsum.i16Qsum_mAs;
        pIGaugeParams->tempQsum.i16Qsum_mAh +=
            pIGaugeParams->deltaQsum.i16Qsum_mAh;
    }

    //Update NomRemCap_mAh, clear temQsum_mAh
    pIGaugeParams->i16IF_NomRemCap_mAh += pIGaugeParams->tempQsum.i16Qsum_mAh;
    pIGaugeParams->tempQsum.i16Qsum_mAh = 0;

    //Update Cycles, update DhgQsum_mAh, use Dhg to calculate the cycles
    if (pIGaugeParams->dhgQsum.i16Qsum_mAh <
        0 - (int16_t) pCapLearnParams->u16FilteredNomFullCap_mAh) {
        pIGaugeParams->dhgQsum.i16Qsum_mAh +=
            pCapLearnParams->u16FilteredNomFullCap_mAh;
        pIGaugeParams->u16DhgCycles++;
    }

    //Update Cycles, update ChgQsum_mAh
    if (pIGaugeParams->chgQsum.i16Qsum_mAh >
        pCapLearnParams->u16FilteredNomFullCap_mAh) {
        pIGaugeParams->chgQsum.i16Qsum_mAh -=
            pCapLearnParams->u16FilteredNomFullCap_mAh;
        pIGaugeParams->u16ChgCycles++;
    }

    //Normally the efficiency should be above 99%.The Gap between ChgCycles and DhgCycles should be 1
    //When this flag set, it means:1.high internal discharge 2.Low current sampling frequency
    if (pIGaugeParams->u16DhgCycles != pIGaugeParams->u16ChgCycles) {
        i16CycleGap =
            (pIGaugeParams->u16DhgCycles - pIGaugeParams->u16ChgCycles);
        if (i16CycleGap > 2 || i16CycleGap < -2) {
            pWarningFlag->DhgChgEfcyGapFlg = 1;
        }
    }
}

//*****************************************************************************
// IGauge Init to get the remain capacity
//*****************************************************************************
void IGauge_Init(tBattGlobalParams *pBattGlobalParams)
{
    _iq15 iq15Soc;
    uint8_t u8Counter;
    uint16_t *u16ArrayPoint;
    tIGaugeCoulometerParams *pIGaugeParams;
    tBattInfoInput *pBattInfoInput;
    tBattParamsConfig *pBattParamsCfg;
    tBattCapLearnParams *pCapLearnParams;

    pBattInfoInput  = pBattGlobalParams->pBattInfoInput;
    pIGaugeParams   = pBattGlobalParams->pIGaugeParams;
    pCapLearnParams = pBattGlobalParams->pCapLearnParams;
    pBattParamsCfg  = pBattGlobalParams->pBattParamsCfg;

    //Init Raw Coulomb Count
    u16ArrayPoint = (uint16_t *) &(pIGaugeParams->tempQsum);
    for (u8Counter = 0; u8Counter < 9; u8Counter++) {
        u16ArrayPoint[u8Counter] = 0;
    }

    //Ask the IGauge to update the NomRemCap Based on the OCV (=Vcell)
    iq15Soc = VGuage_BattSocSerach(pBattInfoInput->u16Vcell_mV,
        pBattParamsCfg->u8CircuitTableLength,
        pBattParamsCfg->pBattCircuitParams);
    IGauge_CaliNomRemCap(iq15Soc, pCapLearnParams->u16FilteredNomFullCap_mAh,
        pIGaugeParams);  //Init Remain cap
}
