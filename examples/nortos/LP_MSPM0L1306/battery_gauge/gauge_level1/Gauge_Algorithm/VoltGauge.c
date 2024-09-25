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

#include "Gauge_Algorithm/VoltGauge.h"

void VGuage_BattCircuitParamsUpdate(tVGaugeFilterParams *pVFParams,
    tBattParamsConfig *pBattParamsCfg, tBattInfoInput *pBattInfoInput,
    tWarningFlags *pWarningFlag, uint16_t u16NomFullCap_mAh);

//*****************************************************************************
// Calculate the middle value using liner relationship in IQ15 type
// X1,X2 and Y1, Y2 is the range, we want to calculate Y related to X
// Need to avoid X2 equal to X1
//*****************************************************************************
_iq15 VGauge_IQ15CalMidNum(_iq15 X, _iq15 X1, _iq15 X2, _iq15 Y1, _iq15 Y2)
{
    _iq15 Y2toY1, XtoX1, X2toX1, Y;

    XtoX1  = X - X1;
    X2toX1 = X2 - X1;
    Y2toY1 = Y2 - Y1;
    Y      = Y1 + _IQ15mpy(Y2toY1, _IQ15div(XtoX1, X2toX1));
    return Y;
}

//*****************************************************************************
// VGauge initialization to get the first SOC and OCV from Vcell
//*****************************************************************************
void VGauge_Init(tBattGlobalParams *pBattGlobalParams)
{
    tVGaugeFilterParams *pVGaugeParams;
    tBattParamsConfig *pBattParamsCfg;
    tWarningFlags *pWarningFlag;
    tBattInfoInput *pBattInfoInput;

    pBattInfoInput = pBattGlobalParams->pBattInfoInput;
    pVGaugeParams  = pBattGlobalParams->pVGaugeParams;
    pBattParamsCfg = pBattGlobalParams->pBattParamsCfg;
    pWarningFlag   = pBattGlobalParams->pWarningFlag;

    //Get the initial OCV by taking the terminal voltage as the initial OCV
    pVGaugeParams->iq15CalculatedOcv[0] = _IQ15(pBattInfoInput->u16Vcell_mV);

    //Init VF_SOC
    VGuage_BattCircuitParamsUpdate(pVGaugeParams, pBattParamsCfg,
        pBattInfoInput, pWarningFlag, pBattParamsCfg->u16DesignCap_mAh);
}

//*****************************************************************************
// VGauge handler
//*****************************************************************************
void VGauge_Handler(tBattGlobalParams *pBattGlobalParams)
{
    tVGaugeFilterParams *pVGaugeParams;
    tBattParamsConfig *pBattParamsCfg;
    tWarningFlags *pWarningFlag;
    tBattInfoInput *pBattInfoInput;

    pBattInfoInput = pBattGlobalParams->pBattInfoInput;
    pVGaugeParams  = pBattGlobalParams->pVGaugeParams;
    pBattParamsCfg = pBattGlobalParams->pBattParamsCfg;
    pWarningFlag   = pBattGlobalParams->pWarningFlag;

    //Update Vgauge when the average data is prepared
    //Update frequency = sysTikFreq * u8AvgBattParamsUpdateCount
    if (pBattInfoInput->avgBattParamsUpdateFlg == 1) {
        pVGaugeParams->iq15CalculatedOcv[1] =
            pVGaugeParams->iq15CalculatedOcv[0];

        pVGaugeParams->iq15CalculatedOcv[0] =
            _IQ15mpy(pVGaugeParams->iq15CalculatedOcv[1],
                (_IQ15(1) - pVGaugeParams->iq15SmoothFactor)) +
            _IQ15mpy(_IQ15(pBattInfoInput->u16AvgVcell_mV),
                pVGaugeParams->iq15SmoothFactor);

        //Update previous SOC, Rcell, Capcell,smoothfactor
        VGuage_BattCircuitParamsUpdate(pVGaugeParams, pBattParamsCfg,
            pBattInfoInput, pWarningFlag, pBattParamsCfg->u16DesignCap_mAh);
    }
}

//*****************************************************************************
// Search the Soc, using the Ocv input
//*****************************************************************************
_iq15 VGuage_BattSocSerach(
    uint16_t u16Ocv, uint8_t u8TableLength, tBattCircuitParams *pCircuitTable)
{
    uint8_t u8HighNum, u8LowNum, u8Count;
    _iq15 iq15X1, iq15X2, iq15Y1, iq15Y2;
    _iq15 iq15Soc;

    //Avoid OCV out of circuit table range
    if (u16Ocv > pCircuitTable[0].u16ChgOcv_mV) {
        iq15Soc = pCircuitTable[0].iq15Soc_DEC;
    } else if (u16Ocv < pCircuitTable[u8TableLength - 1].u16ChgOcv_mV) {
        iq15Soc = pCircuitTable[u8TableLength - 1].iq15Soc_DEC;
    } else {
        for (u8Count = 0; u8Count < u8TableLength; u8Count++) {
            if (pCircuitTable[u8Count].u16ChgOcv_mV < u16Ocv) {
                u8HighNum = u8Count - 1;
                u8LowNum  = u8Count;
                break;
            }
        }

        iq15X1  = _IQ15(pCircuitTable[u8LowNum].u16ChgOcv_mV);
        iq15X2  = _IQ15(pCircuitTable[u8HighNum].u16ChgOcv_mV);
        iq15Y1  = pCircuitTable[u8LowNum].iq15Soc_DEC;
        iq15Y2  = pCircuitTable[u8HighNum].iq15Soc_DEC;
        iq15Soc = VGauge_IQ15CalMidNum(
            _IQ15(u16Ocv), iq15X1, iq15X2, iq15Y1, iq15Y2);
    }
    return iq15Soc;
}

//*****************************************************************************
// Search the Soc using the Ocv input
//*****************************************************************************
uint16_t VGuage_BattOcvSerach(
    _iq15 iq15Soc, uint8_t u8TableLength, tBattCircuitParams *pCircuitTable)
{
    uint8_t u8HighNum, u8LowNum, u8Count;
    _iq15 iq15X1, iq15X2, iq15Y1, iq15Y2;
    uint16_t u16Ocv;

    //Avoid SOC out of circuit table range
    if (iq15Soc > pCircuitTable[0].iq15Soc_DEC) {
        u16Ocv = pCircuitTable[0].u16ChgOcv_mV;
    } else if (iq15Soc < pCircuitTable[u8TableLength - 1].iq15Soc_DEC) {
        u16Ocv = pCircuitTable[u8TableLength - 1].u16ChgOcv_mV;
    } else {
        for (u8Count = 0; u8Count < u8TableLength; u8Count++) {
            if (pCircuitTable[u8Count].iq15Soc_DEC < iq15Soc) {
                u8HighNum = u8Count - 1;
                u8LowNum  = u8Count;
                break;
            }
        }

        iq15Y1 = _IQ15(pCircuitTable[u8LowNum].u16ChgOcv_mV);
        iq15Y2 = _IQ15(pCircuitTable[u8HighNum].u16ChgOcv_mV);
        iq15X1 = pCircuitTable[u8LowNum].iq15Soc_DEC;
        iq15X2 = pCircuitTable[u8HighNum].iq15Soc_DEC;
        u16Ocv = _IQ15int(
            VGauge_IQ15CalMidNum(iq15Soc, iq15X1, iq15X2, iq15Y1, iq15Y2));
    }

    return u16Ocv;
}

//*****************************************************************************
// Use OCV to search battery parameters:iq15VFAbsSoc_DEC,iq15CalCcell_F,iq15CalRcell_R
//*****************************************************************************
void VGuage_BattCircuitParamsUpdate(tVGaugeFilterParams *pVFParams,
    tBattParamsConfig *pBattParamsCfg, tBattInfoInput *pBattInfoInput,
    tWarningFlags *pWarningFlag, uint16_t u16NomFullCap_mAh)
{
    uint8_t u8HighNum, u8LowNum;
    uint8_t u8Count;
    uint8_t u8TableLength;
    _iq15 iq15Ocv, iq15SysTik, iq15TempTc, iq15TempRcellShift;
    _iq15 iq15Y1, iq15Y2, iq15X1, iq15X2;
    tBattCircuitParams *pCircuitTable;

    pCircuitTable = pBattParamsCfg->pBattCircuitParams;
    iq15Ocv       = pVFParams->iq15CalculatedOcv[0];
    u8TableLength = pBattParamsCfg->u8CircuitTableLength;

    if (iq15Ocv <
        _IQ15(pBattParamsCfg->pBattCircuitParams[u8TableLength - 1]
                  .u16ChgOcv_mV))  // Over the minimum pCircuitTable range
    {
        pWarningFlag->CircuitTableOverRangeFlg = 1;
        u8HighNum                              = u8TableLength - 1;
        u8LowNum                               = u8TableLength - 2;
    } else if (iq15Ocv >
               _IQ15(
                   pBattParamsCfg->pBattCircuitParams[0]
                       .u16ChgOcv_mV))  // Over the maximum pCircuitTable range
    {
        pWarningFlag->CircuitTableOverRangeFlg = 1;
        u8HighNum                              = 1;
        u8LowNum                               = 0;
    } else  // OCV in the range
    {
        for (u8Count = 0; u8Count < u8TableLength; u8Count++) {
            if (pCircuitTable[u8Count].u16ChgOcv_mV < _IQ15int(iq15Ocv)) {
                u8HighNum = u8Count - 1;
                u8LowNum  = u8Count;
                break;
            }
        }
    }

    iq15X1 = _IQ15(pCircuitTable[u8LowNum].u16ChgOcv_mV);
    iq15X2 = _IQ15(pCircuitTable[u8HighNum].u16ChgOcv_mV);

    // Calculate Battery SOC
    iq15Y1 = pCircuitTable[u8LowNum].iq15Soc_DEC;
    iq15Y2 = pCircuitTable[u8HighNum].iq15Soc_DEC;
    pVFParams->iq15VFAbsSoc_DEC =
        VGauge_IQ15CalMidNum(iq15Ocv, iq15X1, iq15X2, iq15Y1, iq15Y2);

    //Calculate Battery capacitor
    iq15Y1 = pCircuitTable[u8LowNum].iq15CapFactor;
    iq15Y2 = pCircuitTable[u8HighNum].iq15CapFactor;

    //Calculate Cap factor
    //When exceed the parameter table range, CapFactor will be negative
    if (iq15Ocv < _IQ15(pBattParamsCfg->pBattCircuitParams[u8TableLength - 1]
                            .u16ChgOcv_mV))
        pVFParams->iq15CalCcell_F = _IQ15mpy(_IQ15(u16NomFullCap_mAh), iq15Y1);
    else if (iq15Ocv >
             _IQ15(pBattParamsCfg->pBattCircuitParams[0].u16ChgOcv_mV))
        pVFParams->iq15CalCcell_F = _IQ15mpy(_IQ15(u16NomFullCap_mAh), iq15Y2);
    else
        pVFParams->iq15CalCcell_F = _IQ15mpy(_IQ15(u16NomFullCap_mAh),
            VGauge_IQ15CalMidNum(iq15Ocv, iq15X1, iq15X2, iq15Y1, iq15Y2));

    //Calculate Battery resistor
    if (pBattInfoInput->i8Tcell_C > pBattParamsCfg->u8CircuitTableTestTemp_C) {
        iq15TempRcellShift =
            _IQ15mpy(_IQ15(pBattInfoInput->i8Tcell_C -
                           pBattParamsCfg->u8CircuitTableTestTemp_C),
                pBattParamsCfg->iq15RcellPosTshift_R);
    } else {
        iq15TempRcellShift =
            _IQ15mpy(_IQ15(pBattParamsCfg->u8CircuitTableTestTemp_C -
                           pBattInfoInput->i8Tcell_C),
                pBattParamsCfg->iq15RcellNegTshift_R);
    }
    if (pVFParams->iq15CalculatedOcv[0] > pVFParams->iq15CalculatedOcv[1]) {
        //Assume in charge mode, the rcell equals to the pCircuitTable[0]'s rcell
        pVFParams->iq15CalRcell_R =
            pCircuitTable[0].iq15DhgNomRcell_R + iq15TempRcellShift;
    } else  //battState = eDHG or eRest or Error
    {
        iq15Y1 = pCircuitTable[u8LowNum].iq15DhgNomRcell_R;
        iq15Y2 = pCircuitTable[u8HighNum].iq15DhgNomRcell_R;
        pVFParams->iq15CalRcell_R =
            VGauge_IQ15CalMidNum(iq15Ocv, iq15X1, iq15X2, iq15Y1, iq15Y2) +
            iq15TempRcellShift;
    }

    iq15SysTik =
        _IQ15(pBattParamsCfg->u8AvgBattParamsUpdateCount *
              pBattParamsCfg->sysTikFreq / pBattParamsCfg->u8SysTikShift);
    iq15TempTc =
        _IQ15mpy(pVFParams->iq15CalRcell_R, pVFParams->iq15CalCcell_F);
    pVFParams->iq15SmoothFactor =
        _IQ15div(iq15SysTik, (iq15TempTc + iq15SysTik));  //Ts/(Ts + RC)
}
