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

#include "appInputCtrlInterface.h"
#include "appUserInputsConfig.h"
#include "gateDriver.h"
#include "main.h"
#include "appDefs.h"
#include "configTables.h"
#include "services.h"


int32_t openLoopAccelDecelRate(uint16_t index)
{
    int32_t accelDecelRate;
    if(index < 8)
    {
        accelDecelRate = ((int32_t)tbl1_olAccA1A2_centiHzPerSec[index]);
    }
    else
    {
        accelDecelRate = ((int32_t)tbl2_olAccA1A2_HzPerSec[index - 8]) * 100;
    }

    return accelDecelRate;
}

int32_t closeLoopAccelDecelRate(uint16_t index)
{
    int32_t accelDecelRate;

    if(index < 16)
    {
        accelDecelRate = ((int32_t)tbl1_clDecClAcc_deciHzPerSec[index]);
    }
    else
    {
        if(index < 30)
        {

            accelDecelRate = ((int32_t)tbl2_clDecClAcc_HzPerSec[index - 16]) * 10;
        }
        else
        {
            if(index == 30)
            {
                accelDecelRate = 700000;
            }
            else
            {
                accelDecelRate = 0;
            }
        }
    }

    return accelDecelRate;
}

void update_ISDConfig(USER_INPUTS_T *pUserInputs)
{
    /* ISD Config      */
    pUserInputs->isdMaxStopBemfPU =
            tbl_StatDetectThr_pu[ pUserInputRegs->isdCfg.b.statDetectThr];

}


void update_MotorStartUp1Config(USER_INPUTS_T *pUserInputs)
{
    pUserInputs->ipdPulseToPulseTimeMsec = 1000.0/
            ((float)(tbl_ipdClkFreq_Hz[pUserInputRegs->
                                       mtrStartUp1.b.ipdClkFreq]));
}

void update_MotorStartUp2Config(USER_INPUTS_T *pUserInputs)
{
    uint16_t temp =0;

    temp = pUserInputRegs->mtrStartUp2.b.olAcc2;
    if(temp == 0)
    {
        pUserInputs->openLoopFastAccelAccelSlewRateCentiHzPerSecPerSec = 0;
    }
    else
    {
        pUserInputs->openLoopFastAccelAccelSlewRateCentiHzPerSecPerSec =
                openLoopAccelDecelRate(temp);
    }

    temp = pUserInputRegs->mtrStartUp2.b.olClHandOffThr;
    if(temp <=19 )
    {
        pUserInputs->forwardTransitionSpeedPerMil = 10 + temp*10;
    }
    else
    {
        pUserInputs->forwardTransitionSpeedPerMil = 225 + (temp-20)*25;
    }

    pUserInputs->angleErrorSlewRateMilliDegreesPerMsec =
            tbl_thetaErrRampRate_mili[pUserInputRegs->
                                      mtrStartUp2.b.thetaErrRampRate];

}

void update_CloseLoop1Config(USER_INPUTS_T *pUserInputs)
{
    uint16_t temp = 0;

    temp = pUserInputRegs->closeLoop1.b.clAcc;

    pUserInputs->closeLoopFastAccelSpeedSlewRateDeciHzPerSec =
            closeLoopAccelDecelRate(temp);

    if(pUserInputRegs->closeLoop1.b.clDecCfg)
    {
        pUserInputs->closeLoopFastDecelSpeedSlewRateDeciHzPerSec =
                pUserInputs->closeLoopFastAccelSpeedSlewRateDeciHzPerSec;
    }
    else
    {
        temp = pUserInputRegs->closeLoop1.b.clDec;
        pUserInputs->closeLoopFastDecelSpeedSlewRateDeciHzPerSec =
                closeLoopAccelDecelRate(temp);
    }
    
}

void update_SystemParams(USER_INPUTS_T *pUserInputs)
{
    uint32_t L0, L1;

    pUserInputs->rsMilliOhms = pUserInputRegs->systemParams.mtrResist;

    L0 = pUserInputRegs->systemParams.mtrInductance;

    L1 = (pUserInputRegs->systemParams.mtrSaliency * L0);

    pUserInputs->LdMicroHenry = L0 - L1;

    pUserInputs->LqMicroHenry = L0 + L1;

    pUserInputs->KeMilliVoltPhasePkPerHz = pUserInputRegs->systemParams.mtrBemfConst;

    pUserInputs->systemDCBusVoltageVolts = pUserInputRegs->systemParams.voltageBase;

    pUserInputs->fullScalePhaseCurrentAmp = pUserInputRegs->systemParams.currentBase;

    pUserInputs->maximumSpeedHz = pUserInputRegs->systemParams.maxMotorSpeed;

    pUserInputs->kpCurrent = pUserInputRegs->systemParams.currLoopKp;

    pUserInputs->kiCurrent = pUserInputRegs->systemParams.currLoopKi;

    pUserInputs->kpSpeed = pUserInputRegs->systemParams.speedLoopKp;

    pUserInputs->kiSpeed = pUserInputRegs->systemParams.speedLoopKi;

    pUserInputs->kiFluxWeak = pUserInputRegs->systemParams.fluxWeakeningKi;

    pUserInputs->kpFluxWeak = pUserInputRegs->systemParams.fluxWeakeningKp;
}

void update_FaultCfg1Config(USER_INPUTS_T *pUserInputs)
{
    /* This is intentionally kept empty */
}

void update_FaultCfg2Config(USER_INPUTS_T *pUserInputs)
{
    uint16_t temp =0;
    pUserInputs->moduleFlags.b.abnormalSpeedStallDetectEnable =
            pUserInputRegs->faultCfg2.b.abnSpdFaultEn;
    pUserInputs->moduleFlags.b.lowBemfStallDetectEnable =
            pUserInputRegs->faultCfg2.b.lowBemfFaultEn;
    pUserInputs->moduleFlags.b.noMotorStallDetectEnable =
            pUserInputRegs->faultCfg2.b.noMtrFaultEn;

    pUserInputs->abnormalSpeedLimitPerMil =
            (pUserInputRegs->faultCfg2.b.lockAbnSpeed *100)+1300;
    pUserInputs->lowBemfEmagThresholdPerMil =
            tbl_AbnormalBEMFThr_pMil[pUserInputRegs->faultCfg2.b.abnBemfThr];

    temp = pUserInputRegs->faultCfg2.b.noMtrThr;
    pUserInputs->noMotorLimitPU = tbl_pu[temp];
}

void update_MiscAlgoConfig(USER_INPUTS_T *pUserInputs)
{
    /* Closeloop Parameters */
    pUserInputs->closeLoopSlowAccelSpeedSlewRateDeciHzPerSec =
            tbl_clSlowAcc_deci[pUserInputRegs->miscAlgo.b.clSlowAcc];

    pUserInputs->ipdMaxOverflow =  
                    tbl_ipdMaxOverflow[pUserInputRegs->miscAlgo.b.ipdMaxOverflow];
}

void appUserInputsInit(USER_INPUTS_T *pUserInputs)
{
    pUserInputs->underVoltageHystPU = _IQ(UNDER_VOLTAGE_HYSTERESIS_PU);
    pUserInputs->overVoltageHystPU = _IQ(OVER_VOLTAGE_HYSTERESIS_PU);
}

void appUserInputRegsConfig(USER_INPUTS_T *pUserInputs)
{
    update_SystemParams(pUserInputs);

    update_ISDConfig(pUserInputs);

    update_MotorStartUp1Config(pUserInputs);

    update_MotorStartUp2Config(pUserInputs);

    update_CloseLoop1Config(pUserInputs);

    update_FaultCfg1Config(pUserInputs);

    update_FaultCfg2Config(pUserInputs);

    update_MiscAlgoConfig(pUserInputs);
}
