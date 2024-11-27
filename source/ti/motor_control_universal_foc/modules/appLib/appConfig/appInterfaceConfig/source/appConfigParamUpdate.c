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

#include "appConfigParamUpdate.h"
#include "appUserInputsConfig.h"
#include "appInterfaceConfig.h"
#include "iqNum.h"
#include "appDefs.h"
#include "main.h"
#include "focHALInterface.h"
#include "configTables.h"
#include "gateDriver.h"
#include "services.h"
#include "servicesHAL.h"

#define MAX(A, B)  A>B?A:B

#define PER_MIL_TO_PER_UNIT     0.001
#define PER_10K_TO_PER_UNIT     0.0001

#define KILO_TO_UNIT            1000.0
#define DECI_TO_UNIT            0.1
#define CENTI_TO_UNIT           0.01
#define MILLI_TO_UNIT           0.001
#define MICRO_TO_UNIT           (MILLI_TO_UNIT * MILLI_TO_UNIT)

#define PerMilToPerUnit             _IQ(PER_MIL_TO_PER_UNIT)
#define PerMilToPerUnit_HALCurrent  _IQ(PER_MIL_TO_PER_UNIT)

#define Per10kToPerUnit             _IQ(PER_10K_TO_PER_UNIT)
#define Per10kToPerUnit_HALCurrent  _IQ(PER_10K_TO_PER_UNIT)

#define PIGAIN_COEF                 (0.992185 * KILO_TO_UNIT * 2.0 * PI) / 10.0

#define IQ_ANGLE_CONV           _IQ(1.0f/360.0f)
#define IQ_IPD_ANGLE_CONV       _IQ(30.0f/360.0f)

#define IQ_MOD_INDX_FORCE_CONV  _IQ(1.0/1000.0)

#define IQ_CURRENT_FORCE_CONV   _IQ(8.0/1000.0)

#define IQ_ALGO_CURR_SQR        _IQ(PER_10K_TO_PER_UNIT * PER_10K_TO_PER_UNIT)

extern uint32_t dutyHysteresis;
static void userInputsToAlgoVar_a_mul_b_div_c_IQx(uint32_t,
                                                  float, float, float,
                                                  int32_t, int32_t *);

static void userInputsToAlgoVar_a_mul_b_div_c_IQ(uint16_t,
                                                 float, float, float,
                                                 int32_t *);

static void userInputsToAlgoVar_a_mul_b_div_c_IQNum(int32_t,
                                                    float, float, float,
                                                    IQ_NUM_T *);

static void userInputsCurrentToAlgoCurrSqr(int32_t, int32_t *);

static void userInputsPerMilToAlgoVar(uint16_t, int32_t *);

static void userInputsPerMilToAlgoVar_HALCurrent(uint16_t, int32_t *);

/* Function Declarations */
void updateMotorConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void semiCloseLoopEstimConfig(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
void updateEsmoConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
void updateMotorInputOutputConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateSourceCurrentLimitConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateSourceVoltageLimitConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateOpenLoopConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateRotorAlignConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateFOCStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateLowBemfStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateNoMotorStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateFOCConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateSoftStopConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateIsdConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateBrakeConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateTriStateConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateIpdConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateSlowFirstCycleConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateVoltageGainConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App);
void updateMotorStartConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateDebugTuningConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateOverCurrentConfigParam(UNIVERSAL_FOC_APPLICATION_T *);
void updateFaulServiceConfigParam(UNIVERSAL_FOC_APPLICATION_T *);

/******************************************************************************/
/* Motor Configuration Parameters */
void updateMotorConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    MOTOR_T *pMotor = &(pMC_App->motor);

    int32_t prevKe;

    float temp;

    userInputsToAlgoVar_a_mul_b_div_c_IQx(pUserInputs->rsMilliOhms,
                                          MILLI_TO_UNIT,
                                          1.0f,
                                          pUserInputsInterface->motorImpedanceBase,
                                          RS_IQ_SCALING,
                                          &pMotor->rs);

    userInputsToAlgoVar_a_mul_b_div_c_IQx(pUserInputsInterface->L0MicroHenry,
                                          MILLI_TO_UNIT,
                                          pUserInputsInterface->adcSamplingFrequencyKHz,
                                          pUserInputsInterface->motorImpedanceBase,
                                          LS_IQ_SCALING,
                                          &pMotor->L0Dt);

    userInputsToAlgoVar_a_mul_b_div_c_IQx(pUserInputsInterface->negL1MicroHenry,
                                          MILLI_TO_UNIT,
                                          pUserInputsInterface->adcSamplingFrequencyKHz,
                                          pUserInputsInterface->motorImpedanceBase,
                                          LS_IQ_SCALING,
                                          &pMotor->L1Dt);


    pMotor->L1Dt = -pMotor->L1Dt;

    pMotor->LdDt = pMotor->L0Dt + pMotor->L1Dt;
    pMotor->LqDt = pMotor->L0Dt - pMotor->L1Dt;

    prevKe = pMotor->Ke;

    userInputsToAlgoVar_a_mul_b_div_c_IQx(pUserInputs->KeMilliVoltPhasePkPerHz,
                                          (MILLI_TO_UNIT * 0.1),
                                          pUserInputs->maximumSpeedHz,
                                          pUserInputs->systemDCBusVoltageVolts,
                                          KE_IQ_SCALING,
                                          &pMotor->Ke);

    temp = pUserInputs->systemDCBusVoltageVolts /
            (((float)pUserInputs->KeMilliVoltPhasePkPerHz) *
                    (MILLI_TO_UNIT * 0.1) *
                    pUserInputs->maximumSpeedHz);
    float2IQ(&pMotor->InvKe, temp);

    pMotor->InvKe.scaledNum = pMotor->InvKe.scaledNum >> 2;
    pMotor->InvKe.iqScaling = pMotor->InvKe.iqScaling - 2;
}

/******************************************************************************/
/* ESMO Configuration Parameters */
void updateEsmoConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    MOTOR_T *pMotor = &(pMC_App->motor);

    ESMO_T *pESMO = &(pMC_App->foc.esmo);

    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
                   &(pMC_App->pAppInterface->userInputsInterface);

    float temp;

    temp = 2.0 * PI * pMC_App->pAppInterface->userInputs.maximumSpeedHz;

    temp *= pUserInputsInterface->adcSamplingRate;

    float2IQ(&pESMO->filterGain, temp);

    pESMO->kSlide = _IQ(pUserInputRegs->systemParams.kSlide);

    pUserInputsInterface->FSMO =  expf(-(pUserInputs->rsMilliOhms /(pUserInputsInterface->L0MicroHenry * pUserInputsInterface->adcSamplingFrequencyKHz)));

    float2IQx(&pESMO->FSMO , pUserInputsInterface->FSMO, FSMO_IQ_SCALING);

    pUserInputsInterface->GSMO = (pUserInputsInterface->motorImpedanceBase * 1000 * (1 - pUserInputsInterface->FSMO))/(pUserInputs->rsMilliOhms);

    float2IQx(&pESMO->GSMO , pUserInputsInterface->GSMO, GSMO_IQ_SCALING);

}
/******************************************************************************/

/******************************************************************************/
/* Motor Input Output Configuration Parameters */
void updateMotorInputOutputConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                       *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs = &(pMC_App->motorInputs);

    FOC_T *pFOC = &(pMC_App->foc);

    int32_t propagationDelay_nS;

    uint32_t minDutyPerMil, minDutyPerMilDisCont = 0, minPWMCountPerMil,
                 minOnTime_ns;

    float2IQx(&pMotorInputs->deltaT, (pUserInputs->maximumSpeedHz *
            pUserInputsInterface->adcSamplingRate), GLOBAL_IQ);

    /* SVM configuration */
    propagationDelay_nS = gateDriverGetPropagationDelay();

    if(pFOC->modIndexLimit == MOD_INDEX_LIMIT_SINE)
    {
        minOnTime_ns = gateDriverGetMinOTimeSineMod();

    }
    else
    {
        minOnTime_ns = gateDriverGetMinOnTimeOverMod();
    }

    int32_t blankingTime=0,ADCCyc;



    if(HAL_Is_SingleShunt())
    {
        blankingTime = (pUserInputRegs->periphCfg1.b.mcuDeadTime<<2);

        blankingTime += (propagationDelay_nS + minOnTime_ns)* ((int32_t)CPU_FREQUENCY_MHZ)/1000;

        ADCCyc = (ADC_SAMPLING_TIME_ns )* ((int32_t)CPU_FREQUENCY_MHZ);

        pMC_App->foc.svm.blankingTime = blankingTime;

        minPWMCountPerMil = ADCCyc + (blankingTime *1000);

        pMC_App->foc.svm.minPWMdelta = blankingTime + (ADCCyc/1000);

        minDutyPerMil = minPWMCountPerMil/(pMC_App->motorInputs.pwmPeriod);
    }
    else
    {
        blankingTime = (minOnTime_ns)* ((int32_t)CPU_FREQUENCY_MHZ);

        ADCCyc = (ADC_SAMPLING_TIME_ns )* ((int32_t)CPU_FREQUENCY_MHZ);

        pMC_App->foc.svm.blankingTime = 0;

        minPWMCountPerMil = MAX(ADCCyc , blankingTime);

        pMC_App->foc.svm.minPWMdelta = 0;

        minDutyPerMil = minPWMCountPerMil/(pMC_App->motorInputs.pwmPeriod);
    }


    if((pUserInputRegs->closeLoop1.b.pwmMode) &&
            (pFOC->closeLoop.commutationState == COMMUTATION_ALIGNED))
    {
        pMC_App->foc.svm.svmGen = SVM_DISCONTINUOUS;

        minDutyPerMilDisCont = ((DISCONTINUOUS_MIN_DUTY_COUNTS * 1000)/
                pMC_App->motorInputs.pwmPeriod);

    }
    else
    {
        pMC_App->foc.svm.svmGen = SVM_CONTINUOUS;
    }

    pMC_App->motorInputs.propagationDelay =
            (((int32_t)CPU_FREQUENCY_MHZ) * propagationDelay_nS * 2)/1000;

    userInputsPerMilToAlgoVar(minDutyPerMil, &pMC_App->foc.svm.minDuty);
    userInputsPerMilToAlgoVar(minDutyPerMilDisCont,
                              &pMC_App->foc.svm.minDutyDisCont);

}

/******************************************************************************/
/* Source Current Limit Configuration Parameters */

void updateSourceCurrentLimitConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                         *pMC_App)
{
    SOURCE_CURRENT_LIMIT_T *pSourceCurrentLimit =
            &(pMC_App->sourceLimits.sourceCurrentLimit);

    pSourceCurrentLimit->sourceCurrentLimit =
                         tbl_pu[pUserInputRegs->periphCfg1.b.busCurrLimit];

    pSourceCurrentLimit->flags.b.enableSet =
            pUserInputRegs->periphCfg1.b.busCurrLimitEnable;
}

/******************************************************************************/
/* Source Voltage Limit Configuration Parameters */
void updateSourceVoltageLimitConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                         *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    uint16_t temp, underVoltageLimit, overVoltageLimit;

    SOURCE_VOLTAGE_LIMIT_T *pSourceVoltageLimit =
            &(pMC_App->sourceLimits.sourceVoltageLimit);

    pSourceVoltageLimit->minDCBusVoltage = tbl_minVm_pMil[pUserInputRegs->faultCfg2.b.minVmMtr];

    pSourceVoltageLimit->maxDCBusVoltage = tbl_maxVm_pMil[pUserInputRegs->faultCfg2.b.maxVmMtr];

    pSourceVoltageLimit->overVoltageFaultClearThreshold = pSourceVoltageLimit->maxDCBusVoltage -
                                            pUserInputs->overVoltageHystPU;

    pSourceVoltageLimit->underVoltageFaultClearThreshold = pSourceVoltageLimit->minDCBusVoltage +
                                                            pUserInputs->underVoltageHystPU;

    pSourceVoltageLimit->flags.b.overVoltageFaultEnable =
            (pSourceVoltageLimit->maxDCBusVoltage != 0);

    pSourceVoltageLimit->flags.b.underVoltageFaultEnable =
            (pSourceVoltageLimit->minDCBusVoltage != 0);

    pSourceVoltageLimit->flags.b.underVoltageFaultAutoClear =
            pUserInputRegs->faultCfg2.b.minVmMode;

    pSourceVoltageLimit->flags.b.overVoltageFaultAutoClear =
            pUserInputRegs->faultCfg2.b.maxVmMode;

    pSourceVoltageLimit->countMax =
            (int16_t)(VOLTAGE_OUT_OF_BOUNDS_TIME_MSEC);

}

/******************************************************************************/
/* Close Loop Configuration Parameters */
void updateCloseLoopConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    CLOSE_LOOP_T *pCloseLoop = &(pMC_App->foc.closeLoop);

    float temp,temp1;

    int32_t maxCurr = tbl_pu[pUserInputRegs->closeLoop1.b.iLimit];

    userInputsCurrentToAlgoCurrSqr(maxCurr,
                                   &pCloseLoop->currentReferenceSqrMaxSet);

    if(pUserInputRegs->closeLoop1.b.controlMode == CONTROL_SPEED)
    {
        temp = (DECI_TO_UNIT * SYSTEM_EXECUTION_RATE) /
                    pUserInputs->maximumSpeedHz;
    }
    else if(pUserInputRegs->closeLoop1.b.controlMode == CONTROL_POWER)
    {
        temp1 = (float)pUserInputRegs->systemParams.maxMotorPower;

        temp = (DECI_TO_UNIT * SYSTEM_EXECUTION_RATE) /temp1;

    }
    else if(pUserInputRegs->closeLoop1.b.controlMode == CONTROL_TORQUE)
    {
        temp1 =  _IQtoF(maxCurr) * pUserInputs->fullScalePhaseCurrentAmp;

        temp = (CENTI_TO_UNIT * SYSTEM_EXECUTION_RATE) /temp1;

    }
    else
    {
        temp = (DECI_TO_UNIT * SYSTEM_EXECUTION_RATE * PER_MIL_TO_PER_UNIT);
    }

    temp1 = temp * (float)pUserInputs->closeLoopSlowAccelSpeedSlewRateDeciHzPerSec;
    float2IQx(&pCloseLoop->slowAccelSpeedRefSlewRate,temp1,GLOBAL_IQ);

    temp1 = temp * (float)pUserInputs->closeLoopFastAccelSpeedSlewRateDeciHzPerSec;
    float2IQx(&pCloseLoop->fastAccelSpeedRefSlewRate,temp1,GLOBAL_IQ);

    temp1 = temp * (float)pUserInputs->closeLoopFastDecelSpeedSlewRateDeciHzPerSec;
    float2IQx(&pCloseLoop->fastDecelSpeedRefSlewRate,temp1,GLOBAL_IQ);

    pCloseLoop->slowDecelSpeedRefSlewRate =
            pCloseLoop->slowAccelSpeedRefSlewRate;

    if(pCloseLoop->fastAccelSpeedRefSlewRate <= 0)
    {
        pCloseLoop->fastAccelSpeedRefSlewRate = _IQ(1.0);
    }

    if(pCloseLoop->fastDecelSpeedRefSlewRate <= 0)
    {
        pCloseLoop->fastDecelSpeedRefSlewRate = _IQ(1.0);
    }

    temp = (pUserInputs->maximumSpeedHz) /
            pUserInputsInterface->phaseCurrentBase;
    temp1 = pUserInputs->kpSpeed * temp;
    float2IQx(&pCloseLoop->piSpeed.kp, temp1, KP_IQ_SCALING);

    temp1 = (pUserInputs->kiSpeed * temp *
            pUserInputsInterface->adcSamplingRate);
    float2IQx(&pCloseLoop->piSpeed.ki, temp1, GLOBAL_IQ);

    userInputsToAlgoVar_a_mul_b_div_c_IQ(pUserInputs->
                                         angleErrorSlewRateMilliDegreesPerMsec,
                                         1.0,
                                         pUserInputsInterface->adcSamplingRate,
                                         360.0,
                                         &pCloseLoop->angleAlignStep);

    pCloseLoop->flags.b.avsEnable = pUserInputRegs->closeLoop1.b.avsEn;

    pCloseLoop->modIndexSqrLimit = 0;


}

/******************************************************************************/
/* Current Control Configuration Parameters */

void updateCurrentControlConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                     *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_STATUS_INTERFACE_T *pUserOutputs = pMC_App->pAppInterface->pUserOutputs;
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    FOC_T *pFOC = &(pMC_App->foc);

    float
    L0,
    PIGain,
    kpCurrent,
    kiCurrent;

    /* Current PI constants calculation */
    if((pUserInputs->kpCurrent != 0) && (pUserInputs->kiCurrent != 0))
    {
        kpCurrent = pUserInputs->kpCurrent;
        kiCurrent = pUserInputs->kiCurrent;
    }
    else
    {
        L0 = ((float)(pUserInputsInterface->L0MicroHenry)) * MICRO_TO_UNIT;

        PIGain = PIGAIN_COEF * (pUserInputsInterface->adcSamplingFrequencyKHz);

        kpCurrent = L0 * PIGain;

        kiCurrent = ((float)pUserInputs->rsMilliOhms) *
                MILLI_TO_UNIT * PIGain;
    }

    pUserOutputs->currentPI.kp = kpCurrent;

    pUserOutputs->currentPI.ki = kiCurrent;


    float temp = pUserInputsInterface->phaseCurrentBase /
            (pUserInputs->systemDCBusVoltageVolts);

    temp = temp/3.0;
    pUserInputsInterface->kpCurrent =
            (kpCurrent * temp);

    pUserInputsInterface->kiCurrent =
            (kiCurrent * temp *
                    pUserInputsInterface->adcSamplingRate);

    float2IQx(&pFOC->piId.kp, pUserInputsInterface->kpCurrent, KP_IQ_SCALING);
    float2IQx(&pFOC->piId.ki, pUserInputsInterface->kiCurrent, GLOBAL_IQ);

    pFOC->piIq.kp = pFOC->piId.kp;
    pFOC->piIq.ki = pFOC->piId.ki;

}


/******************************************************************************/
/* Open Loop Configuration Parameters */
void updateOpenLoopConfigParam(UNIVERSAL_FOC_APPLICATION_T
                               *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    OPEN_LOOP_T *pOpenLoop = &(pMC_App->foc.openLoop);

    float temp,
    slewRateDenominatorForward,
    accelSlewRateNumerator;

    int32_t
    openLoopFastAccelCurrentReferencePU;

    uint16_t
    slowFirstCycleSpeedPerMil,
    OLtemp;

    int32_t
    openLoopSpeedSlewRateCentiHzPerSec,
    openLoopSpeedReferencePerMil;

    OLtemp = tbl_alignSlowRampRate[pUserInputRegs->mtrStartUp1.b.
                                   alignSlowRampRate];
    userInputsToAlgoVar_a_mul_b_div_c_IQ(OLtemp,
                                         DECI_TO_UNIT,
                                         pUserInputsInterface->adcSamplingRate,
                                         pUserInputsInterface->phaseCurrentBase,
                                         &pOpenLoop->iqRefSlewRate);

    slowFirstCycleSpeedPerMil =
            tbl_slowFirstCycFreqPerMil[pUserInputRegs->mtrStartUp2.b.
                                       slowFirstCycFreq];

    if(pOpenLoop->iqRefSlewRate <= 0)
    {
        pOpenLoop->iqRefSlewRate = _IQ(8.0);
    }

    /* Fast Acceleration Current Reference */
    if(pUserInputRegs->mtrStartUp1.b.olILimitCfg == 0)
    {
        OLtemp = pUserInputRegs->mtrStartUp2.b.olILimit;
        openLoopFastAccelCurrentReferencePU = tbl_pu[OLtemp];
    }
    else
    {
        OLtemp =  pUserInputRegs->closeLoop1.b.iLimit;
        openLoopFastAccelCurrentReferencePU = tbl_pu[OLtemp];
    }

        openLoopSpeedReferencePerMil =
                pUserInputs->forwardTransitionSpeedPerMil;


    pOpenLoop->iqRefSetFastAccel = openLoopFastAccelCurrentReferencePU;

    if(pOpenLoop->slowFirstCycle.flags.b.enable &&
            slowFirstCycleSpeedPerMil > openLoopSpeedReferencePerMil)
    {
        openLoopSpeedReferencePerMil = slowFirstCycleSpeedPerMil;
    }

    userInputsPerMilToAlgoVar(openLoopSpeedReferencePerMil,
                              &pOpenLoop->speedReferenceMaxForwardSet);

    temp = PER_MIL_TO_PER_UNIT *
            pUserInputs->maximumSpeedHz;

    /* Fast Acceleration Slew Rate */
    slewRateDenominatorForward =
            ((float)openLoopSpeedReferencePerMil) * temp;

    accelSlewRateNumerator = (pUserInputsInterface->adcSamplingRate *
            pUserInputsInterface->adcSamplingRate *
            ((float)_IQ(1.0)));

    /* Fast Acceleration Speed Slew Rate */
    OLtemp = pUserInputRegs->mtrStartUp2.b.olAcc1;

    int32_t openLoopAccelSlewRateCentiHzPerSecPerSec;
    openLoopAccelSlewRateCentiHzPerSecPerSec = pUserInputs->
            openLoopFastAccelAccelSlewRateCentiHzPerSecPerSec;
    openLoopSpeedSlewRateCentiHzPerSec =
            openLoopAccelDecelRate(OLtemp);

    /* Fast Acceleration Acceleration Slew Rate */
    userInputsToAlgoVar_a_mul_b_div_c_IQNum(openLoopAccelSlewRateCentiHzPerSecPerSec,
                                            CENTI_TO_UNIT,
                                            accelSlewRateNumerator,
                                            slewRateDenominatorForward,
                                            &pOpenLoop->accelRefSlewRateFastAccel);

    userInputsToAlgoVar_a_mul_b_div_c_IQNum(openLoopSpeedSlewRateCentiHzPerSec,
                                            CENTI_TO_UNIT,
                                            pUserInputsInterface->adcSamplingRate,
                                            slewRateDenominatorForward,
                                            &pOpenLoop->speedRefSlewRateFastAccel);

    pOpenLoop->speedRefSlewRateSlowAccel =
                pOpenLoop->speedRefSlewRateFastAccel;

    pOpenLoop->accelRefSlewRateSlowAccel =
                pOpenLoop->accelRefSlewRateFastAccel;

    pOpenLoop->idRefSlewRate = 0;

    pOpenLoop->flags.b.fastStartEnable =
            pUserInputRegs->mtrStartUp2.b.FirstCycFreqSel;

}

/******************************************************************************/
/* Rotor Align Configuration Parameters */
void updateRotorAlignConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                 *pMC_App)
{
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    ROTOR_ALIGN_T *pRotorAlign = &(pMC_App->foc.rotorAlign);

    uint16_t rotorAlignCurrentSlewRate, rotorAlignTime;

    uint16_t rotorTemp;

    rotorAlignCurrentSlewRate = tbl_alignSlowRampRate[pUserInputRegs->mtrStartUp1.b.
                                                      alignSlowRampRate];
    rotorAlignTime = tbl_alignTime_msec[pUserInputRegs->mtrStartUp1.b.alignTime];

    pRotorAlign->iqRefSet =
            tbl_pu[pUserInputRegs->mtrStartUp1.b.alignOrSlowCurrLimit];

    userInputsToAlgoVar_a_mul_b_div_c_IQ(rotorAlignCurrentSlewRate,
                                         DECI_TO_UNIT,
                                         pUserInputsInterface->adcSamplingRate,
                                         pUserInputsInterface->
                                         phaseCurrentBase,
                                         &pRotorAlign->iqRefSlewRate);

    if(pRotorAlign->iqRefSlewRate <= 0)
    {
        pRotorAlign->iqRefSlewRate = _IQ(8.0);
    }

    pRotorAlign->countMaxStart = rotorAlignTime;

    rotorTemp = tbl_alignAngle[pUserInputRegs->mtrStartUp2.b.alignAngle];
    if(pUserCtrlRegs->algoDebugCtrl1.b.forceAlignEn &&
            pUserCtrlRegs->algoDebugCtrl1.b.forceAlignAngleSrcSelect)
    {
        rotorTemp = pUserCtrlRegs->algoDebugCtrl1.b.forcedAlignAngle;
    }

    pRotorAlign->angleSet = rotorTemp * IQ_ANGLE_CONV;

}

/******************************************************************************/
/* Slow First Cycle Configuration Parameters */
void updateSlowFirstCycleConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                     *pMC_App)
{

    SLOW_FIRST_CYCLE_T  *pSlowFirstCycle =
            &(pMC_App->foc.openLoop.slowFirstCycle);
    uint16_t sfcTemp;

    sfcTemp = tbl_slowFirstCycFreqPerMil[pUserInputRegs->mtrStartUp2.b.slowFirstCycFreq];

    userInputsPerMilToAlgoVar(sfcTemp,&pSlowFirstCycle->speedReference);

    pSlowFirstCycle->iqRefSet = 
            tbl_pu[pUserInputRegs->mtrStartUp1.b.alignOrSlowCurrLimit];
}

/******************************************************************************/
/* Over Current Stall Detection Configuration Parameters */
void updateFOCStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    STALL_DETECT_T *pStallDetect = &(pMC_App->foc.stallDetect);

    pStallDetect->abnormalSpeedStall.flags.b.reportOnly =
            pUserInputs->moduleFlags.b.motorStallReportOnlyEnable;

    pStallDetect->lowBemfStall.flags.b.reportOnly =
            pUserInputs->moduleFlags.b.motorStallReportOnlyEnable;

    pStallDetect->noMotorStall.flags.b.reportOnly =
            pUserInputs->moduleFlags.b.motorStallReportOnlyEnable;
}

/******************************************************************************/
/* Low BEMF Stall Detection Configuration Parameters */
void updateLowBemfStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                         *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    LOW_BEMF_STALL_DETECT_T *pLowBemfStall =
            &(pMC_App->foc.stallDetect.lowBemfStall);

    int32_t bemfMagTresholdPU;

    pLowBemfStall->flags.b.enableSet =
            pUserInputs->moduleFlags.b.lowBemfStallDetectEnable;

    userInputsPerMilToAlgoVar(pUserInputs->lowBemfEmagThresholdPerMil,
                              &bemfMagTresholdPU);

    pLowBemfStall->lowBEMFThreshold = bemfMagTresholdPU;

}

/******************************************************************************/
/* No Motor Stall Detection Configuration Parameters */
void updateNoMotorStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                              *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    NO_MOTOR_STALL_DETECT_T *pNoMotorStall =
            &(pMC_App->foc.stallDetect.noMotorStall);

    pNoMotorStall->minimumCurrent = pUserInputs->noMotorLimitPU;

    pNoMotorStall->countMax =
            NO_MOTOR_TIME_MSEC *
            pUserInputsInterface->adcSamplingFrequencyKHz;

    pNoMotorStall->flags.b.enableSet =
            pUserInputs->moduleFlags.b.noMotorStallDetectEnable;
}

/******************************************************************************/
/* Abnormal Speed Stall Detection Configuration Parameters */
void updateAbnormalSpeedStallDetectConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                               *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    ABNORMAL_SPEED_STALL_DETECT_T *pAbnormalSpeedStall =
            &(pMC_App->foc.stallDetect.abnormalSpeedStall);

    userInputsPerMilToAlgoVar(
            pUserInputs->abnormalSpeedLimitPerMil,
            &pAbnormalSpeedStall->maximumSpeed);

    pAbnormalSpeedStall->countMax =
            (uint16_t)(ABNORMAL_SPEED_TIME_MSEC);


    pAbnormalSpeedStall->flags.b.enableSet =
            pUserInputs->moduleFlags.b.abnormalSpeedStallDetectEnable;
}
/******************************************************************************/
/* FOC Configuration Parameters */
void updateFOCConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    if(pUserInputRegs->closeLoop1.b.overModEnable)
    {
        pMC_App->foc.modIndexLimit = MOD_INDEX_LIMIT_OVERMODULATION;
    }
    else
    {
        pMC_App->foc.modIndexLimit = MOD_INDEX_LIMIT_SINE;
    }

}

/******************************************************************************/
/* Soft Stop Configuration Parameters */
void updateSoftStopConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    SOFT_STOP_T *pSoftStop = &(pMC_App->load.softStop);

    uint16_t SPDtemp;


    SPDtemp =
            tbl_brkDutyActSPinThr_pMil[pUserInputRegs->closeLoop2.b.brkSpeedThr];

    userInputsPerMilToAlgoVar(SPDtemp,
                              &pSoftStop->minimumSpeedBrake);

    if(pSoftStop->minimumSpeedBrake < pMC_App->minimumSpeed)
    {
        pSoftStop->minimumSpeedBrake = pMC_App->minimumSpeed;
    }

    SPDtemp =
            tbl_brkDutyActSPinThr_pMil[pUserInputRegs->closeLoop2.b.actSpinThr];

    userInputsPerMilToAlgoVar(SPDtemp,
                              &pSoftStop->minimumSpeedSoftStop);

    if(pSoftStop->minimumSpeedSoftStop <= pMC_App->minimumSpeed)
    {
        pSoftStop->minimumSpeedSoftStop = pMC_App->minimumSpeed;
    }

}
/******************************************************************************/
/* ISD Configuration Parameters */
void updateIsdConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    ISD_T *pIsd = &(pMC_App->load.isd);
    uint16_t ISDtemp;

    pIsd->flags.b.enable = pUserInputRegs->isdCfg.b.isdEn;

    pIsd->maxStopBemf = pUserInputs->isdMaxStopBemfPU;
    ISDtemp = tbl_fwRevDrv_pMil[pUserInputRegs->isdCfg.b.fwDrvResyncThr];

    userInputsPerMilToAlgoVar(ISDtemp,
                              &pIsd->minCloseLoopSpeed);

    ISDtemp = tbl_isdStopTime_msec[pUserInputRegs->miscAlgo.b.isdStopTime];
    pIsd->motorStopCountMax =
            (ISDtemp *
                    (uint16_t)pUserInputsInterface->adcSamplingFrequencyKHz);

    ISDtemp = tbl_isdRunTime_msec[pUserInputRegs->miscAlgo.b.isdRunTime];
    pIsd->motorRunCountMax =
            (ISDtemp *
                    (uint16_t)pUserInputsInterface->adcSamplingFrequencyKHz);

    pIsd->flags.b.reverseSyncEnable =
            pUserInputRegs->isdCfg.b.rvsDrEn;
    pIsd->flags.b.forwardSyncEnable =
            pUserInputRegs->isdCfg.b.resyncEn;
}
/******************************************************************************/
/* Brake Configuration Parameters */
void updateBrakeConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{

    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    BRAKE_T *pBrake = &(pMC_App->load.brake);

    uint16_t BRKtemp;
    int32_t brakeCurrentThresholdPU;

    pBrake->flags.b.enableOnStart =
            pUserInputRegs->isdCfg.b.brakeEn;

    brakeCurrentThresholdPU =
             tbl_pu[pUserInputRegs->closeLoop2.b.brkCurrThr];

    BRKtemp = tbl_hiZ_brk_Time_ms[pUserInputRegs->isdCfg.b.brkTime];
    pBrake->timeOutCountMaxOnStart = BRKtemp;

    BRKtemp = tbl_brakeCurrPersist_msec[pUserInputRegs->
                                        miscAlgo.b.brkCurrPersist];
    pBrake->motorStopCountMax =
            (int16_t)(BRKtemp *
                    pUserInputsInterface->adcSamplingFrequencyKHz);

    pBrake->flags.b.brakeCheckCurrentStart =
            pUserInputRegs->isdCfg.b.brkConfig;

    userInputsCurrentToAlgoCurrSqr(brakeCurrentThresholdPU,
                                   &pBrake->currentThresholdSqr);

    pBrake->timeOutCountMaxOnStop = 1;

       pMC_App->foc.rotorAlign.countMaxStop = pBrake->timeOutCountMaxOnStop;

}
/******************************************************************************/
/* Tristate Configuration Parameters */
void updateTriStateConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    TRISTATE_T *pTriState = &(pMC_App->load.triState);

    pTriState->flags.b.enable =
            pUserInputRegs->isdCfg.b.hiZEn;
    uint16_t temp = tbl_hiZ_brk_Time_ms[pUserInputRegs->isdCfg.b.hiZTime];

    pTriState->countMax  = temp;
}

/******************************************************************************/
/* IPD Configuration Parameters */
void updateIpdConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    IPD_T *pIpd = &(pMC_App->load.ipd);
    
    pIpd->pulseToPulseCountMax =
                (int32_t)(pUserInputs->ipdPulseToPulseTimeMsec *
                        ((float)IPD_EXECUTION_FREQUENCY_KHZ));
	
	/* IPD delay corrections */
	if(pIpd->pulseToPulseCountMax > 1)
	{
			/* compensating 2 cycles for delays in ipd process */
			pIpd->pulseToPulseCountMax-=2; 
	}

    pIpd->overflowMax = pUserInputs->ipdMaxOverflow;

    pIpd->currentLimitHi = pUserInputRegs->mtrStartUp1.b.ipdCurrThresh;

    pIpd->cycleCountMax = pUserInputRegs->mtrStartUp1.b.ipdRepeat + 1;

    pIpd->advanceAngle = pUserInputRegs->mtrStartUp1.b.ipdAdvAngle *
                            IQ_IPD_ANGLE_CONV;
}

/******************************************************************************/
void updateVoltageGainConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    HAL_MEASURE_VOLTAGE_T *pVoltage = &(pMC_App->motorInputs.voltage);
    pVoltage->voltageGainSet = VOLTAGE_GAIN_VMAX_BY_1;
}
/******************************************************************************/
/* Motor Start Configuration Parameters */
void updateMotorStartConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    MOTOR_START_OPTIONS_TYPES temp =
            (MOTOR_START_OPTIONS_TYPES)(pUserInputRegs->mtrStartUp1.b.mtrStartUpOption);

    pMC_App->foc.openLoop.slowFirstCycle.flags.b.enable =
            (temp == MOTOR_START_SLOW_FIRST_CYCLE);
	pMC_App->load.ipd.flags.b.enable =
			(temp == MOTOR_START_IPD);
}

/******************************************************************************/
/* Motor Stop Configuration Parameters */
void updateMotorStopConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{

    LOAD_T *pLoad = &(pMC_App->load);

    MOTOR_STOP_OPTIONS_TYPES temp;

    temp = (MOTOR_STOP_OPTIONS_TYPES)pUserInputRegs->closeLoop1.b.mtrStopOption;

    if(pMC_App->foc.rotorAlign.flags.b.forceEnable ||
                  pMC_App->load.isd.flags.b.forceEnable)
    {
        temp = MOTOR_STOP_HI_Z;
    }

    if(temp == MOTOR_STOP_ACTIVE_SPIN_DOWN)
    {

        /* Brake or Active Spindown */
        pLoad->softStop.flags.b.enable = TRUE;

        /* Brake specific flags */
        pLoad->brake.flags.b.enableOnStop = FALSE;
        pLoad->brake.flags.b.brakeCheckCurrentStop = FALSE;

        pLoad->flags.b.loadStopAlignEnable = FALSE;

        pLoad->softStop.minimumSpeed =
                pLoad->softStop.minimumSpeedSoftStop;
    }
    else
    {
        /* Hi-z */

        /* Brake or Active Spin down specific flags */
        pLoad->softStop.flags.b.enable = FALSE;
        pLoad->brake.flags.b.enableOnStop = FALSE;
        pLoad->brake.flags.b.brakeCheckCurrentStop = FALSE;
        pLoad->flags.b.loadStopAlignEnable = FALSE;

    }

    /* Brake Align Enable */
    pMC_App->foc.rotorAlign.flags.b.enableOnBrakeSet =
            pUserInputRegs->pinCfg.b.brakePinMode;

}
/******************************************************************************/
/* Debug and tuning Configuration Parameters */
void updateDebugTuningConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    FOC_T *pFOC = &(pMC_App->foc);

    LOAD_T *pLoad = &(pMC_App->load);

    int32_t temp;

    temp = pUserCtrlRegs->algoDebugCtrl2.b.forceVDCurrLoopDis << 6;
    temp = temp >> 5;

    pFOC->mdqForceSet.d = temp * IQ_MOD_INDX_FORCE_CONV;

    temp = pUserCtrlRegs->algoDebugCtrl2.b.forceVQCurrLoopDis << 6;
    temp = temp >> 5;

    pFOC->mdqForceSet.q = temp * IQ_MOD_INDX_FORCE_CONV;

    if(pUserCtrlRegs->algoDebugCtrl3.b.fluxModeReference != 0)
    {
        int32_t refFlux;

        refFlux = (pUserCtrlRegs->algoDebugCtrl3.b.fluxModeReference & 0x1FF);

        refFlux <<= (GLOBAL_IQ -9);

       if(pUserCtrlRegs->algoDebugCtrl3.b.fluxModeReference & 0x200)
           pFOC->closeLoop.idRefGen.idRefFluxModeSet = -refFlux;
       else
           pFOC->closeLoop.idRefGen.idRefFluxModeSet = refFlux;
       pFOC->closeLoop.idRefGen.flags.b.fluxModeEnable = 1;

    }
    else
    {
        pFOC->closeLoop.idRefGen.flags.b.fluxModeEnable = 0;
    }
    pFOC->rotorAlign.flags.b.forceEnable =
            pUserCtrlRegs->algoDebugCtrl1.b.forceAlignEn;

    pFOC->openLoop.flags.b.forceEnable =
            pUserCtrlRegs->algoDebugCtrl1.b.closeLoopDis;

    /* Input Reference Mode = 1 => Torque Mode */
    pFOC->closeLoop.flags.b.torqueModeEnable =
            (pUserInputRegs->closeLoop1.b.controlMode == 1);

pFOC->flags.b.voltageModeEnableSet =
        pUserCtrlRegs->algoDebugCtrl2.b.currLoopDis;

pFOC->openLoop.slowFirstCycle.flags.b.forceEnable =
        pUserCtrlRegs->algoDebugCtrl1.b.forceSlowCycleFirstCycleEn;

pLoad->ipd.flags.b.forceEnable =
        pUserCtrlRegs->algoDebugCtrl1.b.forceIPDEn;

pLoad->isd.flags.b.forceEnable =
        pUserCtrlRegs->algoDebugCtrl1.b.forceISDEn;
}
/******************************************************************************/
/* Over Current Configuration Parameters */
void updateOverCurrentConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    OVER_CURRENT_T *pOverCurrent = &(pMC_App->sourceLimits.overCurrent);

    /* 
    pOverCurrent->overCurrentLimitSet = pUserInputs->hardwareOverCurrentLimitpu;
    pOverCurrent->flags.b.enableSet =
            pUserInputs->moduleFlags.b.hardwareOverCurrentFaultEnable;
    */
}
/******************************************************************************/
/* Fault Services Configuration Parameters */
void updateFaulServiceConfigParam(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    FAULT_SERVICE_T *pFault = &(pMC_App->faultService);

    pFault->countMax = pUserInputs->faultRetryTimeMsec;

}
/*  This function reads user configuration from EEPROM and multiple registers
 * updated with user specific configuration on fault reporting, recovery and
 * clearing fault state */

void user_config_faults(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    uint32_t temp;
    FAULT_SERVICE_T *pFault = &(pMC_App->faultService);

    _Bool
    abnormalSpeedStallEnable,
    lowBemfStallEnable,
    noMotorStallEnable;

    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);

    temp = pUserInputRegs->faultCfg1.b.lockRetry;
    if(temp == 0)
    {
        pUserInputs->faultRetryTimeMsec = 500;
    }
    else
    {
        /* Load count max with retry time in ms */
        pUserInputs->faultRetryTimeMsec = temp * 1000;
    }

	controllernFaultReport  &= ~(HW_LOCK_ILIMIT_FAULT_INDEX);
	controllerFaultAction   &= ~(HW_LOCK_ILIMIT_FAULT_INDEX);
	pUserInputs->moduleFlags.b.hardwareOverCurrentFaultEnable  = FALSE;

    /* fault configuration for MOTOR_LOCK faults including NO_MOTOR,
     * ABNORMAL_SPEED and ABNORMAL_BEMF */

    abnormalSpeedStallEnable = pUserInputRegs->faultCfg2.b.abnSpdFaultEn;
    lowBemfStallEnable = pUserInputRegs->faultCfg2.b.lowBemfFaultEn;
    noMotorStallEnable = pUserInputRegs->faultCfg2.b.noMtrFaultEn;

    /* Configure Fault Stop and Recovery Mechanisms */
    temp = pUserInputRegs->faultCfg1.b.mtrLckMode;
    if (temp < 3)  /* if motor lock mode is not set to no report, no action mode */
    {
        pUserInputs->moduleFlags.b.abnormalSpeedStallDetectEnable  =
                abnormalSpeedStallEnable;

        pUserInputs->moduleFlags.b.lowBemfStallDetectEnable  =
                lowBemfStallEnable;

        pUserInputs->moduleFlags.b.noMotorStallDetectEnable  =
                noMotorStallEnable;

        /* Setting of nFault */
        if(abnormalSpeedStallEnable)
        {
            controllernFaultReport  |= (ABN_SPEED_FAULT_INDEX);
        }
        else
        {
            controllernFaultReport  &= ~(ABN_SPEED_FAULT_INDEX);
        }

        if(lowBemfStallEnable)
        {
            controllernFaultReport  |= (ABN_BEMF_FAULT_INDEX);
        }
        else
        {
            controllernFaultReport  &= ~(ABN_BEMF_FAULT_INDEX);
        }

        if(noMotorStallEnable)
        {
            controllernFaultReport  |= (NO_MOTOR_FAULT_INDEX);
        }
        else
        {
            controllernFaultReport  &= ~(NO_MOTOR_FAULT_INDEX);
        }

        if ((abnormalSpeedStallEnable ||
                lowBemfStallEnable ||
                noMotorStallEnable))
        {
            controllernFaultReport  |= (MTR_LOCK_FAULT_INDEX);
        }
        else
        {
            controllernFaultReport  &= ~(MTR_LOCK_FAULT_INDEX);
        }

        if (temp < 2)  /* if motor lock mode is set to retry or latched mode */
        {
            pUserInputs->moduleFlags.b.motorStallReportOnlyEnable = FALSE;

            if(temp == 1)
            {
                pFault->flags.b.abnormalSpeedRetryEnable = TRUE;
                pFault->flags.b.lowBemfRetryEnable = TRUE;
                pFault->flags.b.noMotorRetryEnable = TRUE;
                controllerFaultActionLatched &=
                        ~((MTR_LOCK_FAULT_INDEX) | (ABN_SPEED_FAULT_INDEX) |
                                (ABN_BEMF_FAULT_INDEX) | (NO_MOTOR_FAULT_INDEX));
            }
            else
            {
                pFault->flags.b.abnormalSpeedRetryEnable = FALSE;
                pFault->flags.b.lowBemfRetryEnable = FALSE;
                pFault->flags.b.noMotorRetryEnable = FALSE;
                controllerFaultActionLatched |=
                        ((MTR_LOCK_FAULT_INDEX) | (ABN_SPEED_FAULT_INDEX) |
                                (ABN_BEMF_FAULT_INDEX) | (NO_MOTOR_FAULT_INDEX));
            }

        }
        else /* if motor lock mode is set to report only mode */
        {
            pUserInputs->moduleFlags.b.motorStallReportOnlyEnable = TRUE;

            controllerFaultAction   &=
                    ~((MTR_LOCK_FAULT_INDEX) | (ABN_SPEED_FAULT_INDEX) |
                            (ABN_BEMF_FAULT_INDEX) | (NO_MOTOR_FAULT_INDEX));
        }

    }
    else
    {
        /* Fault Disabled */
        controllernFaultReport  &=
                ~((MTR_LOCK_FAULT_INDEX) | (ABN_SPEED_FAULT_INDEX) |
                        (ABN_BEMF_FAULT_INDEX) | (NO_MOTOR_FAULT_INDEX));

        controllerFaultAction   &=
                ~((MTR_LOCK_FAULT_INDEX) | (ABN_SPEED_FAULT_INDEX) |
                        (ABN_BEMF_FAULT_INDEX) | (NO_MOTOR_FAULT_INDEX));

        pUserInputs->moduleFlags.b.abnormalSpeedStallDetectEnable  = FALSE;

        pUserInputs->moduleFlags.b.lowBemfStallDetectEnable  = FALSE;

        pUserInputs->moduleFlags.b.noMotorStallDetectEnable  = FALSE;

    }

    if(pUserInputRegs->faultCfg2.b.maxVmMode == 0)
    {
        controllerFaultActionLatched |= OVER_VOLTAGE_FAULT_INDEX;
    }
    else
    {
        controllerFaultActionLatched &= ~OVER_VOLTAGE_FAULT_INDEX;
    }
    if(pUserInputRegs->faultCfg2.b.minVmMode == 0)
    {
        controllerFaultActionLatched |= UNDER_VOLTAGE_FAULT_INDEX;
    }
    else
    {
        controllerFaultActionLatched &= ~UNDER_VOLTAGE_FAULT_INDEX;
    }
}
/******************************************************************************/
/* Flux Weakening Configuration Parameters */
void updateFluxWeakConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                                *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    FLUX_WEAK_T *pFluxWeak = &(pMC_App->foc.closeLoop.idRefGen.fluxWeak);

    float temp;

    int32_t tempRatio;
    int32_t outMin;

    pMC_App->foc.closeLoop.idRefGen.flags.b.fluxWeakeningEnable =
            pUserInputRegs->fieldCtrl.b.fluxWeakeningEn;

    temp = pUserInputs->kpFluxWeak / pUserInputsInterface->phaseCurrentBase;
    float2IQx(&pFluxWeak->piFluxWeak.kp, temp, KP_IQ_SCALING);

    temp = (pUserInputs->kiFluxWeak * SYSTEM_EXECUTION_RATE) /
            pUserInputsInterface->phaseCurrentBase;
    float2IQx(&pFluxWeak->piFluxWeak.ki,temp, GLOBAL_IQ);

    outMin = -(_IQsqrt(pMC_App->foc.closeLoop.currentReferenceSqrMaxSet));

    if(pUserInputRegs->fieldCtrl.b.fluxWeakCurrRatio == 0)
    {
        pFluxWeak->piFluxWeak.outMin = outMin;
    }
    else
    {
        tempRatio = (9 -  pUserInputRegs->fieldCtrl.b.fluxWeakCurrRatio) * _IQ(0.1);
        pFluxWeak->piFluxWeak.outMin = _IQmpy(outMin,tempRatio);
    }

    pFluxWeak->mSqrRef = tbl_mSqrRef[pUserInputRegs->fieldCtrl.b.fluxWeakeningReference];

    pFluxWeak->piFluxWeak.outMax = 0;
}
/******************************************************************************/
/* MTPA Configuration Parameters */
void updateMtpaConfigParam(UNIVERSAL_FOC_APPLICATION_T
                                                *pMC_App)
{
    USER_INPUTS_T *pUserInputs = &(pMC_App->pAppInterface->userInputs);
    USER_INPUTS_INTERFACE_T *pUserInputsInterface =
            &(pMC_App->pAppInterface->userInputsInterface);

    MTPA_T *pMtpa = &(pMC_App->foc.closeLoop.idRefGen.mtpa);


    userInputsToAlgoVar_a_mul_b_div_c_IQNum(pUserInputsInterface->
                                            negL1MicroHenry,
                                            MICRO_TO_UNIT,
                                            (pUserInputs->maximumSpeedHz * 2*PI *
                                                    pUserInputsInterface->phaseCurrentBase),
                                                    MOTOR_VOLTAGE_BASE,
                                                    &pMtpa->L1);

    pMC_App->foc.closeLoop.idRefGen.flags.b.mtpaEnable =
                    pUserInputRegs->fieldCtrl.b.mtpaEnable;

}
/******************************************************************************/
/* Control mode update */
void updateControlModeConfigParam(UNIVERSAL_FOC_APPLICATION_T
                             *pMC_App)
{
    CLOSE_LOOP_T *pCloseLoop = &(pMC_App->foc.closeLoop);

    float temp;
    int32_t tempAngle;
    
    pCloseLoop->controlMode = (APP_CONTROL_TYPES)(pUserInputRegs->closeLoop1.b.controlMode);

    if(pCloseLoop->controlMode == CONTROL_SPEED)
    {
        pCloseLoop->piSpeed.pReference = &(pCloseLoop->velocityReference);
        pCloseLoop->piSpeed.pFeedback =
                          &(pMC_App->angleTrackingPLLEstim.velocityFilt);
//        pCloseLoop->piSpeed.pFeedback =
//                          &(pMC_App->semiCloseLoopEstim.velocityFilt);


    }
    else if(pCloseLoop->controlMode == CONTROL_POWER)
    {
        pCloseLoop->piSpeed.pReference = &(pCloseLoop->PowerReference);
        pCloseLoop->piSpeed.pFeedback = &(pCloseLoop->PowerFeedback);

        temp = (float)pUserInputRegs->systemParams.maxMotorPower;

        temp = temp/pMC_App->pAppInterface->userInputs.basePower;

        pCloseLoop->maximumPower = _IQ(temp);
    }
    else if(pCloseLoop->controlMode == CONTROL_TORQUE)
    {
        pCloseLoop->torqRefScalingFactor = _IQsqrt(pCloseLoop->currentReferenceSqrMaxSet);
    }
    else
    {

        if(pUserInputRegs->closeLoop2.b.leadAngle <= 15)
        {
            tempAngle = pUserInputRegs->closeLoop2.b.leadAngle;
        }
        else
        {
            tempAngle = (pUserInputRegs->closeLoop2.b.leadAngle - 15)*2 + 15;
        }
        tempAngle = tempAngle * IQ_ANGLE_CONV;
        MC_SinCos(&pCloseLoop->mdqForceSinCos, tempAngle);

    }
}
/******************************************************************************/
/* All Configuration Parameters */
void updateConfigurationParameters(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    appInterfaceEEpromConfig(pMC_App);

    if(pUserInputRegs->systemParams.maxMotorSpeed == 0)
    {
        pMC_App->maximumSpeed = 0;
    }
    else
    {
        pMC_App->maximumSpeed = _IQ(1.0);
    }

    updateSourceCurrentLimitConfigParam(pMC_App);
    updateSourceVoltageLimitConfigParam(pMC_App);

    updateCloseLoopConfigParam(pMC_App);
    updateCurrentControlConfigParam(pMC_App);
    updateFOCStallDetectConfigParam(pMC_App);
    updateFluxWeakConfigParam(pMC_App);
    updateMtpaConfigParam(pMC_App);
    updateControlModeConfigParam(pMC_App);
    updateNoMotorStallDetectConfigParam(pMC_App);
    updateAbnormalSpeedStallDetectConfigParam(pMC_App);

    updateFOCConfigParam(pMC_App);

    updateBrakeConfigParam(pMC_App);

    updateVoltageGainConfigParam(pMC_App);

    updateMotorStartConfigParam(pMC_App);
    updateMotorStopConfigParam(pMC_App);

    updateDebugTuningConfigParam(pMC_App);

    updateOverCurrentConfigParam(pMC_App);

    updateFaulServiceConfigParam(pMC_App);

    updateMotorInputOutputConfigParam(pMC_App);

    if((pMC_App->state == APP_INIT) || (pMC_App->state == APP_IDLE) ||
            (pMC_App->state == APP_FAULT))
    {
        HAL_SetPWMFrequency(pMC_App->motorInputs.pwmPeriod,
                            pMC_App->motorInputs.propagationDelay);

        HAL_SetADCSamplingFrequency(pMC_App->motorInputs.
                                    adcSamplingRatio);

        HAL_SetSystemFrequency((int32_t)(SYSTEM_TIMER_FREQUENCY_MHZ *
                SYSTEM_EXECUTION_FREQ_KHZ * 1000));

        updateMotorConfigParam(pMC_App);
        semiCloseLoopEstimConfig(pMC_App);
        updateEsmoConfigParam(pMC_App);

        updateOpenLoopConfigParam(pMC_App);
        updateRotorAlignConfigParam(pMC_App);

        updateSlowFirstCycleConfigParam(pMC_App);

        updateIsdConfigParam(pMC_App);

        updateTriStateConfigParam(pMC_App);

        updateIpdConfigParam(pMC_App);
        
        pMC_App->minimumSpeed = MIN_TARGET_SPEED;

    }

    updateLowBemfStallDetectConfigParam(pMC_App);
    updateSoftStopConfigParam(pMC_App);

}

void updateRAMConfigurationParameters(UNIVERSAL_FOC_APPLICATION_T *pMC_App)
{
    appInterfaceConfig(pMC_App);

    updateMotorInputOutputConfigParam(pMC_App);

    updateCurrentControlConfigParam(pMC_App);

    updateDebugTuningConfigParam(pMC_App);

    updateRotorAlignConfigParam(pMC_App);

}

static void userInputsToAlgoVar_a_mul_b_div_c_IQx(uint32_t input,
                                                  float scaling,
                                                  float numerator,
                                                  float denominator,
                                                  int32_t Q_SCALING,
                                                  int32_t *pAlgoVar)
{
    float temp;

    temp = (((float)input) * scaling * numerator) / denominator;

    float2IQx(pAlgoVar, temp, Q_SCALING);
}

static void userInputsToAlgoVar_a_mul_b_div_c_IQ(uint16_t input,
                                                 float scaling,
                                                 float numerator,
                                                 float denominator,
                                                 int32_t *pAlgoVar)
{
    float temp;

    temp = (((float)input) * scaling * numerator) / denominator;

    float2IQx(pAlgoVar, temp, GLOBAL_IQ);
}

static void userInputsToAlgoVar_a_mul_b_div_c_IQNum(int32_t input,
                                                    float scaling,
                                                    float numerator,
                                                    float denominator,
                                                    IQ_NUM_T *pAlgoVar)
{
    float temp;

    temp = (((float)input) * scaling * numerator) / denominator;

    float2IQ(pAlgoVar, temp);
}

static void userInputsCurrentToAlgoCurrSqr(int32_t input,
                                           int32_t *pCurrentSqr)
{
    *pCurrentSqr = _IQmpy(input, input);
}

static void userInputsPerMilToAlgoVar(uint16_t input, int32_t *pAlgoVar)
{
    *pAlgoVar = (input * PerMilToPerUnit);
}

static void userInputsPerMilToAlgoVar_HALCurrent(uint16_t input, int32_t *pAlgoVar)
{
    *pAlgoVar = (input * Per10kToPerUnit_HALCurrent);
}

uint16_t piRAMInput(float piGain)
{
    uint16_t piScale, piVal, piRAM;

    if(piGain >= 25.5)
    {
        piScale = 0;
        piVal = piGain;
    }
    else if(piGain >= 2.55)
    {
        piScale = 1;
        piVal = (piGain * 10.0);
    }
    else if(piGain >= 0.255)
    {
        piScale = 2;
        piVal = (piGain * 100.0);
    }
    else
    {
        piScale = 3;
        piVal = (piGain * 1000.0);
    }

    if(piVal == 0)
    {
        piVal = 1;
    }

    piRAM = ((piScale << 8) | piVal);

    return piRAM;
}
