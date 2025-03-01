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
#include "focHALInterface.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>
#include "appDefs.h"
#include "main.h"
#include "gateDriver.h"

/** @brief IQ value of square root of 3 */
#define IQ_SQRT3    _IQ(1.732050807568877)

/** @brief Gain change max limit */
#define IABC_GAIN_CHANGE_MAX_LIMIT      _IQ(3.80)
/** @brief Gain change min limit */
#define IABC_GAIN_CHANGE_MIN_LIMIT      _IQ(0.45)

void HAL_ReadMotorInputs(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_CURRENT_T *pCurrent;
    HAL_MEASURE_VOLTAGE_T *pVoltage;

    pVoltage = &(pMotorInputs->voltage);
    pCurrent = &(pMotorInputs->current);

    HAL_GD_ReadDCVBusVoltage(pVoltage);
     switch(pMotorInputs->measureQuantity)
    {
        case MEASURE_PHASE_CURRENT:

            HAL_ReadCurrents(pMotorInputs);

        break;

        case MEASURE_PHASE_VOLTAGE:

            HAL_ReadPhaseVoltages(pMotorInputs);

        break;
    }

    pCurrent->prevMaxDutyPhase = *(pCurrent->pMaxDutyPhase);
}

void HAL_SetPhaseCurrentChannels(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    pMotorInputs->measureQuantity = MEASURE_PHASE_CURRENT;

    /* Disable Data Sequencer */
   DL_ADC12_disableConversions(FOC_ADC0_INST);
   DL_ADC12_disableConversions(FOC_ADC1_INST);

   HAL_GD_ConfigureCurrentChannels(pMotorInputs->current.currentShunt);

   /* Enable Data Sequencer */
   DL_ADC12_enableConversions(FOC_ADC0_INST);
   DL_ADC12_enableConversions(FOC_ADC1_INST);
}

void HAL_setADCIdxChannel(ADC12_Regs *adc12, DL_ADC12_MEM_IDX idx,
                                                             uint32_t chansel)
{
    adc12->ULLMEM.MEMCTL[idx] &= ~(ADC12_MEMCTL_CHANSEL_MASK);
    adc12->ULLMEM.MEMCTL[idx] |= chansel;
}

void HAL_SetPhaseVoltageChannels(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    pMotorInputs->measureQuantity = MEASURE_PHASE_VOLTAGE;

    /* Disable Data Sequencer */
     DL_ADC12_disableConversions(FOC_ADC0_INST);
     DL_ADC12_disableConversions(FOC_ADC1_INST);

     HAL_GD_ConfigureVoltageChannels();

     /* Configure Data Sequencer Trigger for current sampling */
     HAL_SetADCSamplingFrequency(pMotorInputs->adcSamplingRatio);

     /* Enable Data Sequencer */
     DL_ADC12_enableConversions(FOC_ADC0_INST);
     DL_ADC12_enableConversions(FOC_ADC1_INST);
}

void HAL_ReadPhaseVoltages(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_VOLTAGE_T *pVoltage;

    pVoltage = &(pMotorInputs->voltage);

    HAL_GD_ReadVoltages(pVoltage);

     /* Convert raw voltage values to proper format */
     pVoltage->vabc.a =
             _IQmpy_mathacl(pVoltage->vabcRaw.a,IQ_SQRT3);

     pVoltage->vabc.b =
             _IQmpy_mathacl(pVoltage->vabcRaw.b,IQ_SQRT3) ;

     pVoltage->vabc.c =
             _IQmpy_mathacl(pVoltage->vabcRaw.c,IQ_SQRT3);

}

void HAL_ReadCurrents(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_CURRENT_T *pCurrent = &(pMotorInputs->current);

    int32_t temp, ia, ib, ic, iMagSqr;

    CURRENT_SHUNT_TYPES currentShunt = pCurrent->currentShunt;

    MC_ABC_T
        iabc;

    HAL_GD_ReadCurrents(pCurrent);

    if(getMeasureOffsetStatus(pMotorInputs))
    {
#ifndef __CURRENT_SINGLE_SHUNT
        iabc.a =
                (_IQ11toIQ((pCurrent->iabcRaw.a -
                        pCurrent->pOffset->iaOffset)));

        if(pCurrent->currentShunt == CURRENT_THREE_SHUNT_A_BC ||
                pCurrent->currentShunt == CURRENT_TWO_SHUNT_A_B)
        {

            iabc.b =
                    (_IQ11toIQ((pCurrent->iabcRaw.b -
                            pCurrent->pOffset->ibADC1Offset)));
        }
        else
        {

            iabc.b =
                    (_IQ11toIQ((pCurrent->iabcRaw.b -
                            pCurrent->pOffset->ibADC0Offset)));
        }

        iabc.c =
                (_IQ11toIQ((pCurrent->iabcRaw.c -
                        pCurrent->pOffset->icOffset)));

        if(currentShunt == CURRENT_TWO_SHUNT_A_B)
        {
            iabc.c = -(iabc.a + iabc.b);
        }
        else if(currentShunt == CURRENT_TWO_SHUNT_A_C)
        {
            iabc.b = -(iabc.a + iabc.c);
        }
        else if(currentShunt == CURRENT_TWO_SHUNT_B_C)
        {
            iabc.a = -(iabc.c + iabc.b);
        }

#else
        HAL_currentReconstruction(pCurrent, *pCurrent->pPrevPrevSector);
        iabc.a = pCurrent->iabcRaw.a;
        iabc.b = pCurrent->iabcRaw.b;
        iabc.c = pCurrent->iabcRaw.c;
#endif
        ia = iabc.a;
        ib = iabc.b;
        ic = iabc.c;

        temp = _IQmpy_mathacl(ia, ia) + _IQmpy_mathacl(ib, ib)
                + _IQmpy_mathacl(ic, ic);

        iMagSqr = _IQmpy_mathacl(temp, TWO_BY_THREE);

        pCurrent->iabc = iabc;
        pCurrent->iMagSqr = iMagSqr;
    }
}

void HAL_SelectShuntMeasure(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    HAL_MEASURE_CURRENT_T *pCurrent = &(pMotorInputs->current);

    MC_PHASE_TYPES maxDutyPhase = *(pCurrent->pMaxDutyPhase);

#ifdef __CURRENT_THREE_SHUNT_DYNAMIC
    if(maxDutyPhase != pCurrent->prevMaxDutyPhase &&
        pCurrent->flags.b.threeShuntDynamic)
    {
        /* Max duty phase has changed and dynamic update on 3
         * shunt is enabled */
        switch(maxDutyPhase)
        {
            case PHASE_A:
                pCurrent->currentShunt = CURRENT_TWO_SHUNT_B_C;
            break;

            case PHASE_B:
                pCurrent->currentShunt = CURRENT_TWO_SHUNT_A_C;
            break;

            case PHASE_C:
                pCurrent->currentShunt = CURRENT_TWO_SHUNT_A_B;
            break;

            case PHASE_INVALID:
                pCurrent->currentShunt = pCurrent->currentShuntSet;
            break;
        }

        HAL_SetPhaseCurrentChannels(pMotorInputs);
    }
#endif
}

void HAL_EnablePWM(void)
{
    /* sets forced PWM generation to disabled */
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_U_IDX);
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_V_IDX);
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_W_IDX);

    DL_Timer_setCCPOutputDisabledAdv(FOC_PWMA0_INST,(
        DL_TIMER_CCP0_DIS_OUT_ADV_SET_BY_OCTL | 
        DL_TIMER_CCP1_DIS_OUT_ADV_SET_BY_OCTL |
        DL_TIMER_CCP2_DIS_OUT_ADV_SET_BY_OCTL | 
        DL_TIMER_CCP3_DIS_OUT_ADV_SET_BY_OCTL
        ));
}

void HAL_DisablePWM(void)
{
    /* sets the CC outputs low */
    DL_Timer_setCCPOutputDisabledAdv(FOC_PWMA0_INST,(
        DL_TIMER_CCP0_DIS_OUT_ADV_FORCE_LOW | 
        DL_TIMER_CCP1_DIS_OUT_ADV_FORCE_LOW |
        DL_TIMER_CCP2_DIS_OUT_ADV_FORCE_LOW | 
        DL_TIMER_CCP3_DIS_OUT_ADV_FORCE_LOW
        ));

    /* sets forced PWM generation to disabled */
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_U_IDX);
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_V_IDX);
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_W_IDX);

#ifdef __CURRENT_SINGLE_SHUNT
    /* Set ADC Trigger to be at cc value of 0 */
    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, (DL_Timer_getLoadValue(FOC_TIMA1_INST)>>1), FOC_ADC_TRIG_1_IDX);
    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, 0, FOC_ADC_TRIG_0_IDX);
#endif
}

void HAL_EnableLowSideBrake(void)
{
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                            DL_TIMER_FORCE_CMPL_OUT_HIGH, FOC_PWMA0_U_IDX);
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                            DL_TIMER_FORCE_CMPL_OUT_HIGH, FOC_PWMA0_V_IDX);
    HAL_TimerOverRideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                            DL_TIMER_FORCE_CMPL_OUT_HIGH, FOC_PWMA0_W_IDX);
}

#ifndef __CURRENT_SINGLE_SHUNT

/**
 * @brief     Set PWM dutycycles for three shunt or two shunts
 * @param[in] pABC      Pointer to dutycycle
 * @param[in] FullDuty   Full dutycycle
 */
__STATIC_INLINE void HAL_UpdateDuty_3S(const MC_ABC_T *pABC, const int32_t FullDuty)
{
    int32_t duty1, duty2, duty3;
    _Bool pwmULow = false,pwmVLow = false,pwmWLow = false;

    /* Full Duty == 100% */
    duty1 = _IQmpy_mathacl(pABC->a, FullDuty);
    duty2 = _IQmpy_mathacl(pABC->b, FullDuty);
    duty3 = _IQmpy_mathacl(pABC->c, FullDuty);

    if(duty1 < DISCONTINUOUS_MIN_DUTY_COUNTS)
    {
        pwmULow = true;
    }
    else
    {
        /* Disable Force PWM for U phase */
        HAL_PWMForceDisable(FOC_PWMA0_U_IDX);

        /* duty divided by 2 for up-down counter mode */
        HAL_setCaptureCompareValue(
                          FOC_PWMA0_INST, duty1>>1, FOC_PWMA0_U_IDX);
    }

    if(duty2 < DISCONTINUOUS_MIN_DUTY_COUNTS)
    {
        pwmVLow = true;
    }
    else
    {
        /* Disable Force PWM for V phase */
        HAL_PWMForceDisable(FOC_PWMA0_V_IDX);

        /* duty divided by 2 for up-down counter mode */
        HAL_setCaptureCompareValue(
                          FOC_PWMA0_INST, duty2>>1, FOC_PWMA0_V_IDX);
    }

    if(duty3 < DISCONTINUOUS_MIN_DUTY_COUNTS)
    {
        pwmWLow = true;
    }
    else
    {
        /* Disable Force PWM for W phase */
        HAL_PWMForceDisable(FOC_PWMA0_W_IDX);

        /* duty divided by 2 for up-down counter mode */
        HAL_setCaptureCompareValue(
                          FOC_PWMA0_INST, duty3>>1, FOC_PWMA0_W_IDX);
    }

    if(pwmULow)
    {
        HAL_PWMForceLow(FOC_PWMA0_U_IDX);
    }

    if(pwmVLow)
    {
        HAL_PWMForceLow(FOC_PWMA0_V_IDX);
    }

    if(pwmWLow)
    {
        HAL_PWMForceLow(FOC_PWMA0_W_IDX);
    }
}
#endif

#ifdef __CURRENT_SINGLE_SHUNT

/**
 * @brief     Set PWM dutycycles for single shunt
 * @param[in] pABC      Pointer to dutycycle
 * @param[in] FullDuty   Full dutycycle
 * @param[in] voltageSector   Voltage sector
 * @param[in] minDelta   minimum PWM delta
 * @param[in] blankingTime   Blanking time
 */
__STATIC_INLINE void HAL_UpdateDuty_1S(const MC_ABC_T *pABC, const int32_t FullDuty,
                                                 MC_SECTOR_TYPES voltageSector,
                                                 int32_t minDelta,
                                                 int32_t blankingTime)
{
    int32_t dutyA, dutyB, dutyC;
    int32_t AcmpA, AcmpB;
    int32_t BcmpA, BcmpB;
    int32_t CcmpA, CcmpB;
    int16_t delta;
    int32_t pwm3cmpA,pwm3cmpB;

    /* Full Duty == 100% */
    dutyA = _IQmpy_mathacl(pABC->a, FullDuty);
    dutyB = _IQmpy_mathacl(pABC->b, FullDuty);
    dutyC = _IQmpy_mathacl(pABC->c, FullDuty);

    AcmpB = (dutyA>>1);
    AcmpA = FullDuty - AcmpB;
    BcmpB = (dutyB>>1);
    BcmpA = FullDuty - BcmpB;
    CcmpB = (dutyC>>1);
    CcmpA = FullDuty - CcmpB;

    switch(voltageSector)
    {
    case SECTOR_1:
            /* Max A, Min C */

            /* Mid Duty Phase Compensation */
            if(BcmpB < minDelta)
            {
                delta = minDelta - BcmpB;
                BcmpA+=delta;
                BcmpB+=delta;
            }
            /* Min Duty Phase Compensation */
            if((BcmpB - CcmpB) < minDelta)
            {
                delta = minDelta - (BcmpB - CcmpB);
                CcmpA-=delta;
                CcmpB-=delta;
            }
            /* Max Duty Phase Compensation */
            if((AcmpB - BcmpB) < minDelta)
            {
                delta = minDelta - (AcmpB - BcmpB);
                AcmpA+=delta;
                AcmpB+=delta;
            }

            /* Max Duty Phase */
            pwm3cmpA = AcmpB;
            /* Mid Duty Phase */
            pwm3cmpB = BcmpB;
            break;
        case SECTOR_2:
            /* Max B, Min C */

            /* Mid Duty Phase Compensation */
            if(AcmpB < minDelta)
            {
                delta = minDelta - AcmpB;
                AcmpA+=delta;
                AcmpB+=delta;
            }
            /* Min Duty Phase Compensation */
            if((AcmpB - CcmpB) < minDelta)
            {
                delta = minDelta - (AcmpB - CcmpB);
                CcmpA-=delta;
                CcmpB-=delta;
            }
            /* Max Duty Phase Compensation */
            if((BcmpB - AcmpB) < minDelta)
            {
                delta = minDelta - (BcmpB - AcmpB);
                BcmpA+=delta;
                BcmpB+=delta;
            }

            /* Max Duty Phase */
            pwm3cmpA = BcmpB;
            /* Mid Duty Phase */
            pwm3cmpB = AcmpB;
            break;
        case SECTOR_3:
            /* Max B, Min A */

            /* Mid Duty Phase Compensation */
            if(CcmpB < minDelta)
            {
                delta = minDelta - CcmpB;
                CcmpA+=delta;
                CcmpB+=delta;
            }
            /* Min Duty Phase Compensation */
            if((CcmpB - AcmpB) < minDelta)
            {
                delta = minDelta - (CcmpB - AcmpB);
                AcmpA-=delta;
                AcmpB-=delta;
            }
            /* Max Duty Phase Compensation */
            if((BcmpB - CcmpB) < minDelta)
            {
                delta = minDelta - (BcmpB - CcmpB);
                BcmpA+=delta;
                BcmpB+=delta;
            }

            /* Max Duty Phase */
            pwm3cmpA = BcmpB;
            /* Mid Duty Phase */
            pwm3cmpB = CcmpB;
            break;
        case SECTOR_4:
            /* Max C, Min A */

            /* Mid Duty Phase Compensation */
            if(BcmpB < minDelta)
            {
                delta = minDelta - BcmpB;
                BcmpA+=delta;
                BcmpB+=delta;
            }
            /* Min Duty Phase Compensation */
            if((BcmpB - AcmpB) < minDelta)
            {
                delta = minDelta - (BcmpB - AcmpB);
                AcmpA-=delta;
                AcmpB-=delta;
            }
            /* Max Duty Phase Compensation */
            if((CcmpB - BcmpB) < minDelta)
            {
                delta = minDelta - (CcmpB - BcmpB);
                CcmpA+=delta;
                CcmpB+=delta;
            }

            /* Max Duty Phase */
            pwm3cmpA = CcmpB;
            /* Mid Duty Phase */
            pwm3cmpB = BcmpB;
            break;
        case SECTOR_5:
            /* Max C, Min B */

            /* Mid Duty Phase Compensation */
            if(AcmpB < minDelta)
            {
                delta = minDelta - AcmpB;
                AcmpA+=delta;
                AcmpB+=delta;
            }
            /* Min Duty Phase Compensation */
            if((AcmpB - BcmpB) < minDelta)
            {
                delta = minDelta - (AcmpB - BcmpB);
                BcmpA-=delta;
                BcmpB-=delta;
            }
            /* Max Duty Phase Compensation */
            if((CcmpB - AcmpB) < minDelta)
            {
                delta = minDelta - (CcmpB - AcmpB);
                CcmpA+=delta;
                CcmpB+=delta;
            }

            /* Max Duty Phase */
            pwm3cmpA = CcmpB;
            /* Mid Duty Phase */
            pwm3cmpB = AcmpB;
            break;
        case SECTOR_6:
            /* Max A, Min B */

            /* Mid Duty Phase Compensation */
            if(CcmpB < minDelta)
            {
                delta = minDelta - CcmpB;
                CcmpA+=delta;
                CcmpB+=delta;
            }
            /* Min Duty Phase Compensation */
            if((CcmpB - BcmpB) < minDelta)
            {
                delta = minDelta - (CcmpB - BcmpB);
                BcmpA-=delta;
                BcmpB-=delta;
            }
            /* Max Duty Phase Compensation */
            if((AcmpB - CcmpB) < minDelta)
            {
                delta = minDelta - (AcmpB - CcmpB);
                AcmpA+=delta;
                AcmpB+=delta;
            }

            /* Max Duty Phase */
            pwm3cmpA = AcmpB;
            /* Mid Duty Phase */
            pwm3cmpB = CcmpB;
            break;
        default:
            pwm3cmpA = 400;
            pwm3cmpB = 200;
            break;
    }

    /* Limit cmpA */
    if(AcmpA >= FullDuty)
    {
        AcmpA = FullDuty - 1;
    }
    if(BcmpA >= FullDuty)
    {
        BcmpA = FullDuty - 1;
    }
    if(CcmpA >= FullDuty)
    {
        CcmpA = FullDuty - 1;
    }

    /* Limit cmpB */
    if(AcmpB <= 0)
    {
        AcmpB = 1;
    }
    if(BcmpB <= 0)
    {
        BcmpB = 1;
    }
    if(CcmpB <= 0)
    {
        CcmpB = 1;
    }

    HAL_setCaptureCompareValue(
                    FOC_PWMA0_INST, AcmpA, FOC_PWMA0_U_IDX);
    HAL_setCaptureCompareValue(
                    FOC_PWMA0_INST, AcmpB, FOC_PWMA0_U_B_IDX);

//    /* Disable Force PWM for U phase */
//    HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
    

    HAL_setCaptureCompareValue(
                    FOC_PWMA0_INST, BcmpA, FOC_PWMA0_V_IDX);
    HAL_setCaptureCompareValue(
                    FOC_PWMA0_INST, BcmpB, FOC_PWMA0_V_B_IDX);
                    
//    /* Disable Force PWM for V phase */
//    HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
    

    HAL_setCaptureCompareValue(
                    FOC_PWMA0_INST, CcmpA, FOC_PWMA0_W_IDX);
    HAL_setCaptureCompareValue(
                    FOC_PWMA0_INST, CcmpB, FOC_PWMA0_W_B_IDX);

//    /* Disable Force PWM for W phase */
//    HAL_PWMForceDisable(FOC_PWMA0_W_IDX);

    pwm3cmpA = pwm3cmpA - blankingTime;
    pwm3cmpB = pwm3cmpB - blankingTime;
    if(pwm3cmpA<=0)
    {
        pwm3cmpA = 1;
    }
    if(pwm3cmpB<=0)
    {
        pwm3cmpB = 1;
    }

    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, pwm3cmpA, FOC_ADC_TRIG_1_IDX);
    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, pwm3cmpB, FOC_ADC_TRIG_0_IDX);

}

#endif

void HAL_UpdateDuty(const MC_ABC_T *pABC, const int32_t FullDuty,
                                                 MC_SECTOR_TYPES voltageSector,
                                                 int32_t minDelta,
                                                 int32_t blankingTime)
{
    #ifdef __CURRENT_SINGLE_SHUNT
        HAL_UpdateDuty_1S( pABC, FullDuty, 
                            voltageSector, minDelta, blankingTime);
    #else
        HAL_UpdateDuty_3S(pABC, FullDuty);

    #endif
}

void HAL_SetPWMFrequency(const int32_t pwmPeriod,
                         const int32_t propagationDelay)
{
    
#ifndef __CURRENT_SINGLE_SHUNT

    uint32_t triggerPoint;

    /* period is divided by 2 for up-down counter mode */
    DL_Timer_setLoadValue(FOC_PWMA0_INST, pwmPeriod >> 1);

    /* Set 3 shunt adc trigger point */
    /* period is divided by 2 for up-down counter mode */
    HAL_setCaptureCompareValue(
        FOC_PWMA0_INST, ((pwmPeriod >> 1) - _IQabs(propagationDelay)),
        FOC_PWMA0_ADC_TRIG_IDX);

    if(propagationDelay > 0)
    {
        /* Trigger after the Counter Load */
        triggerPoint = FOC_PWMA0_ADC_TRIG_DN_EVENT;
    }
    else if(propagationDelay < 0)
    {
        /* Trigger before the Counter Load */
        triggerPoint = FOC_PWMA0_ADC_TRIG_UP_EVENT;
    }
    else
    {
        /* Trigger at the Counter Load */
        triggerPoint = FOC_PWMA0_ADC_TRIG_LOAD_EVENT;
    }

    /* Disable all trigger points */
    DL_TimerA_disableEvent(FOC_PWMA0_INST, DL_TIMERA_EVENT_ROUTE_1,
            0xFFFF);
            
    /* Generate Trigger for ADC Conversion on PWMA0 at Trigger Point */
    DL_TimerA_enableEvent(FOC_PWMA0_INST, DL_TIMERA_EVENT_ROUTE_1,
                        triggerPoint);
    DL_TimerA_setPublisherChanID(FOC_PWMA0_INST, DL_TIMERA_PUBLISHER_INDEX_0,
                        ADC_PWM_TRIG_CH);

#else
    DL_Timer_setLoadValue(FOC_PWMA0_INST, pwmPeriod);
    DL_Timer_setLoadValue(FOC_TIMA1_INST, pwmPeriod);
    
    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, (DL_Timer_getLoadValue(FOC_TIMA1_INST)>>1), FOC_ADC_TRIG_1_IDX);
    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, 0, FOC_ADC_TRIG_0_IDX);
#endif
}


void HAL_enterIPDMode()
{
    /* Stop the 1ms ISR */
    DL_Timer_stopCounter(FOC_TIMG0_INST);

    /* Disable the ADC interrupt */
    DL_ADC12_disableInterrupt(FOC_ADC_ISR_INST, 1<<(FOC_ADC_MEM_RES_LOAD-1));
    HAL_SetADCSamplingFrequency(ADC_SAMPLE_EVERY_PWM);

}

void HAL_exitIPDMode()
{
    /* Disable the trigger */
    DL_Timer_configCrossTriggerInputCond(FOC_PWMA0_INST, DL_TIMER_CROSS_TRIGGER_INPUT_DISABLED);
    HAL_disableTriggerFault(FOC_PWMA0_INST);

    /* Clear the timer fault */
    HAL_clearFaultStatus();
    
    /* Start the 1ms timer */
    DL_Timer_startCounter(FOC_TIMG0_INST);

    /* Reset the Timer A0 , Timer A1
     * -> Stop the Timers
     * - > Reset the Counters
     * Start the timers with cross trigger enabled*/

#ifdef __CURRENT_SINGLE_SHUNT

    DL_Timer_stopCounter(FOC_PWMA0_INST);
    DL_Timer_stopCounter(FOC_TIMA1_INST);

    DL_Timer_setTimerCount(FOC_PWMA0_INST,0);
    DL_Timer_setTimerCount(FOC_TIMA1_INST,0);
#endif


#if  defined ADC0_INST
    DL_ADC12_reset(ADC0_INST);
    DL_ADC12_enablePower(ADC0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
    SYSCFG_DL_ADC0_init();
#endif

#if defined ADC1_INST
    DL_ADC12_reset(ADC1_INST);
    DL_ADC12_enablePower(ADC1_INST);
    delay_cycles(POWER_STARTUP_DELAY);
    SYSCFG_DL_ADC1_init();
#endif
    
#ifdef __CURRENT_SINGLE_SHUNT

    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, (DL_Timer_getLoadValue(FOC_TIMA1_INST)>>1), FOC_ADC_TRIG_1_IDX);
    HAL_setCaptureCompareValue(
                    FOC_TIMA1_INST, 0, FOC_ADC_TRIG_0_IDX);
#endif

    /* Run foc peripheral init */
   focPeriphInit();

    /* Enable the ADC interrupt */
    DL_ADC12_enableInterrupt(FOC_ADC_ISR_INST, 1<<(FOC_ADC_MEM_RES_LOAD-1));

}

void HAL_SetADCSamplingFrequency(const ADC_SAMPLING_RATIO_TYPES samplingRatio)
{
#ifndef __CURRENT_SINGLE_SHUNT
    DL_TimerA_setRepeatCounter(FOC_PWMA0_INST, samplingRatio);
   DL_ADC12_setSubscriberChanID(FOC_ADC0_INST, ADC_PWM_TRIG_CH);
   DL_ADC12_setSubscriberChanID(FOC_ADC1_INST, ADC_PWM_TRIG_CH);
#else
    DL_TimerA_setRepeatCounter(FOC_PWMA0_INST, samplingRatio);
    DL_TimerA_setRepeatCounter(FOC_TIMA1_INST, samplingRatio);
#endif
}

void HAL_SetSystemFrequency(const int32_t systemPeriod)
{
    DL_Timer_setLoadValue(FOC_TIMER_INST, systemPeriod);
}

_Bool HAL_GetPhasesFloatingStatus(const HAL_MEASURE_VOLTAGE_T *pVoltage)
{
    _Bool status, vaMid, vbMid, vcMid;

    MC_ABC_T Vabc = pVoltage->vabcRaw;

    int32_t lowLimit, highLimit;

    lowLimit = _IQmpy_mathacl(pVoltage->vdc, _IQ(MIN_FLOAT_VOLTAGE));
    highLimit = _IQmpy_mathacl(pVoltage->vdc, _IQ(MAX_FLOAT_VOLTAGE));

    vaMid = (Vabc.a >= lowLimit) && (Vabc.a < highLimit);
    vbMid = (Vabc.b >= lowLimit) && (Vabc.b < highLimit);
    vcMid = (Vabc.c >= lowLimit) && (Vabc.c < highLimit);

    if(((vaMid && vbMid) ||
        (vbMid && vcMid) ||
        (vcMid && vaMid)) ||
        (Vabc.a <= lowLimit && Vabc.b <= lowLimit && Vabc.c <= lowLimit) ||
        (Vabc.a >= highLimit && Vabc.b >= highLimit && Vabc.c >= highLimit))

    {
        /* All three phase voltages are either pulled up, down or in between
         * If current flows through phases, at least one phase voltage will be
         * pulled up, other will be pulled down (+ve curr, -ve curr) */
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    return status;
}


void HAL_ClearNFault()
{
    DL_GPIO_clearPins(FOC_GPIO_NFAULT_PORT, FOC_GPIO_NFAULT_PIN);
}

void HAL_SetNFault()
{
    DL_GPIO_setPins(FOC_GPIO_NFAULT_PORT, FOC_GPIO_NFAULT_PIN);
}

uint32_t HAL_getRawHVdieFaultStatus(void)
{
    return (DL_Timer_getRawInterruptStatus(FOC_PWMA0_INST,
                                                GPTIMER_CPU_INT_IMASK_F_MASK));
}

uint32_t HAL_getRawFaultStatus(void)
{
    return (DL_Timer_getRawInterruptStatus(FOC_PWMA0_INST,
                                                GPTIMER_CPU_INT_IMASK_F_MASK));
}

void HAL_clearFaultStatus(void)
{
    DL_Timer_clearInterruptStatus(FOC_PWMA0_INST, GPTIMER_CPU_INT_IMASK_F_MASK);
}

void HAL_setGateDriveOffsetCalib(void)
{
    gateDriverOffsetCalibSet();
}

void HAL_resetGateDriveOffsetCalib(void)
{
    gateDriverOffsetCalibReset();
}

void HAL_pwmCountZero(void)
{
    HAL_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_U_IDX);
    HAL_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_V_IDX);
    HAL_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_W_IDX);

    #ifdef __CURRENT_SINGLE_SHUNT
    HAL_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_U_B_IDX);
    HAL_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_V_B_IDX);
    HAL_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_W_B_IDX);

    #endif
}

void HAL_delayMicroSeconds(uint32_t num_us)
{
    if(num_us == 0)
    {
        return;
    }
    DL_Common_delayCycles(num_us*HAL_CYCLES_TO_1_US_DELAY);
}

void HAL_DisableINT(void)
{
    __disable_irq();
}

void HAL_resetPOR(void)
{
    DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_POR);
}

_Bool HAL_getDirPinStatus(void)
{
    return DL_GPIO_readPins(FOC_GPIO_DIR_PORT, FOC_GPIO_DIR_PIN)?
                        HAL_GPIO_HIGH : HAL_GPIO_LOW;
}

_Bool HAL_getBrakePinStatus(void)
{
    return DL_GPIO_readPins(FOC_GPIO_BRAKE_PORT, FOC_GPIO_BRAKE_PIN)?
                        HAL_GPIO_HIGH : HAL_GPIO_LOW;
}

static const DL_ADC12_ClockConfig gADC_IPDClockConfig = {
    .clockSel       = DL_ADC12_CLOCK_ULPCLK,
    .divideRatio    = DL_ADC12_CLOCK_DIVIDE_1,
    .freqRange      = DL_ADC12_CLOCK_FREQ_RANGE_32_TO_40,
};

static void IPD_ADC_init(ADC12_Regs *adc12Inst, uint32_t chansel)
{
    DL_ADC12_setClockConfig(adc12Inst, (DL_ADC12_ClockConfig *) &gADC_IPDClockConfig);
    DL_ADC12_initSingleSample(adc12Inst,
        DL_ADC12_REPEAT_MODE_ENABLED, DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_SOFTWARE,
        DL_ADC12_SAMP_CONV_RES_12_BIT, DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);
    DL_ADC12_configConversionMem(adc12Inst, DL_ADC12_MEM_IDX_0,
        chansel, DL_ADC12_REFERENCE_VOLTAGE_VDDA, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_ENABLED);
    DL_ADC12_setPowerDownMode(adc12Inst,DL_ADC12_POWER_DOWN_MODE_MANUAL);
    DL_ADC12_setSampleTime0(adc12Inst,2);
    DL_ADC12_setPublisherChanID(adc12Inst,FOC_IPD_EVENT_CH);
#ifdef _NONINVERT_ISEN
    DL_ADC12_enableEvent(adc12Inst,DL_ADC12_EVENT_WINDOW_COMP_HIGH);
#else
    DL_ADC12_enableEvent(adc12Inst,DL_ADC12_EVENT_WINDOW_COMP_LOW);
#endif
    DL_ADC12_enableConversions(adc12Inst);
}

#ifdef __CURRENT_SINGLE_SHUNT
/**
 * @brief     Set IPD Pulse configuration
 * @param[in] vector voltage vector to set
 * @param[in] CompVal Comparator threshold value
 * @param[in] pOffset ADC offset pointer
 */
__STATIC_INLINE void HAL_setIPDPulseConfig_1S(VOLTAGE_VECTOR vector, uint8_t CompVal, CURRENT_OFFSET_T* pOffset)
{
    #ifdef __IPD_ENABLE
    int16_t WCompTh;
    
    /* setting minimum compare value to 2 */
    if(CompVal <= 2)
    {
        CompVal = 2;
    }
    
    /* Convert 8 bit to 12 bit adc range*/
    WCompTh = ((uint16_t)CompVal)<<4;

    /* Stop the ADCs */
    HAL_ADCDisConv(ADC0_INST);
    HAL_ADCDisConv(ADC1_INST);
    
    DL_ADC12_reset(ADC0_INST);
    DL_ADC12_reset(ADC1_INST);
    DL_ADC12_enablePower(ADC0_INST);
    DL_ADC12_enablePower(ADC1_INST);
    delay_cycles(POWER_STARTUP_DELAY);

    /* set the publisher channels to disabled and clear all events */
    DL_ADC12_setPublisherChanID(ADC0_INST,0);
    DL_ADC12_setPublisherChanID(ADC1_INST,0);

    DL_Timer_stopCounter(FOC_CAPTURE_IPD_INST);
    /* clear Capture interrupt status */
    DL_Timer_clearInterruptStatus(FOC_CAPTURE_IPD_INST, 
                DL_TIMER_INTERRUPT_CC0_DN_EVENT|DL_TIMER_INTERRUPT_ZERO_EVENT);
    /* clear the Fault interrupt */
    HAL_clearFaultStatus();

    /* Set timer A0 to trigger using event configuration */
    /*
    CTTRIGCTL.CTEN=1 ,DL_TIMER_CROSS_TRIGGER_MODE_ENABLED
    CTTRIGCTL.EVTCTEN=1 ,DL_TIMER_CROSS_TRIGGER_INPUT_ENABLED
    CTTRIGCTL.EVTCTTRIGSEL=1 ,DL_TIMER_CROSS_TRIG_SRC_FSUB0
    */
    DL_Timer_configCrossTrigger(FOC_PWMA0_INST,DL_TIMER_CROSS_TRIG_SRC_FSUB0,
    DL_TIMER_CROSS_TRIGGER_INPUT_ENABLED,DL_TIMER_CROSS_TRIGGER_MODE_ENABLED);

    /* TSEL.ETSEL =0*/
    /* TSEL.TE =1 */
    DL_Timer_setExternalTriggerEvent(FOC_PWMA0_INST,DL_TIMER_EXT_TRIG_SEL_TRIG_0);
    DL_Timer_enableExternalTrigger(FOC_PWMA0_INST);

    /* FCTL.TFIM =1 */
    HAL_enableTriggerFault(FOC_PWMA0_INST);

    IPD_ADC_init(FOC_CURR_ADC_INSTANCE, ADC_DC_CURRENT_CH);

#ifdef _NONINVERT_ISEN
    DL_ADC12_configWinCompHighThld(FOC_CURR_ADC_INSTANCE, pOffset->i2Offset +WCompTh);
#else
    DL_ADC12_configWinCompLowThld(FOC_CURR_ADC_INSTANCE, pOffset->i2Offset -WCompTh);
#endif
    DL_ADC12_startConversion(FOC_CURR_ADC_INSTANCE);/* ADC conversion started */

    switch(vector)
    {
        case VOLTAGE_VECTOR_AC:

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
            HAL_PWMForceLow(FOC_PWMA0_W_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_U_IDX);
            break;
        case VOLTAGE_VECTOR_CA:

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
            HAL_PWMForceLow(FOC_PWMA0_U_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_W_IDX);
            break;
        case VOLTAGE_VECTOR_BC:
            
            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
            HAL_PWMForceLow(FOC_PWMA0_W_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_V_IDX);
            break;
        case VOLTAGE_VECTOR_CB:
             
            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
            HAL_PWMForceLow(FOC_PWMA0_V_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_W_IDX);
            break;
        case VOLTAGE_VECTOR_BA:
        
            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
            HAL_PWMForceLow(FOC_PWMA0_U_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_V_IDX);
            break;
        case VOLTAGE_VECTOR_AB:

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
            HAL_PWMForceLow(FOC_PWMA0_V_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_U_IDX);
            break;
        default:
            /* This is expected to be empty */
            break;
    }

    HAL_IPDEnableISR();
    #endif
}

#else
/**
 * @brief     Set IPD Pulse configuration
 * @param[in] vector voltage vector to set
 * @param[in] CompVal Comparator threshold value
 * @param[in] pOffset ADC offset pointer
 */
__STATIC_INLINE void HAL_setIPDPulseConfig_3S(VOLTAGE_VECTOR vector, uint8_t CompVal, CURRENT_OFFSET_T* pOffset)
{
    #ifdef __IPD_ENABLE
    int16_t WCompTh;
    
    /* setting minimum compare value to 2 */
    if(CompVal <= 2)
    {
        CompVal = 2; 
    }
    
    /* Convert 8 bit to 12 bit adc range*/
    WCompTh = ((uint16_t)CompVal)<<4;

    /* Stop the ADCs */
    HAL_ADCDisConv(ADC0_INST);
    HAL_ADCDisConv(ADC1_INST);
    
    DL_ADC12_reset(ADC0_INST);
    DL_ADC12_reset(ADC1_INST);
    DL_ADC12_enablePower(ADC0_INST);
    DL_ADC12_enablePower(ADC1_INST);
    delay_cycles(POWER_STARTUP_DELAY);

    /* set the publisher channels to disabled and clear all events */
    DL_ADC12_setPublisherChanID(ADC0_INST,0);
    DL_ADC12_setPublisherChanID(ADC1_INST,0);

    DL_Timer_stopCounter(FOC_CAPTURE_IPD_INST);
    /* clear Capture interrupt status */
    DL_Timer_clearInterruptStatus(FOC_CAPTURE_IPD_INST, 
                DL_TIMER_INTERRUPT_CC0_DN_EVENT|DL_TIMER_INTERRUPT_ZERO_EVENT);
    /* clear the Fault interrupt */
    HAL_clearFaultStatus();

    /* Set timer A0 to trigger using event configuration */
    /*
    CTTRIGCTL.CTEN=1 ,DL_TIMER_CROSS_TRIGGER_MODE_ENABLED
    CTTRIGCTL.EVTCTEN=1 ,DL_TIMER_CROSS_TRIGGER_INPUT_ENABLED
    CTTRIGCTL.EVTCTTRIGSEL=1 ,DL_TIMER_CROSS_TRIG_SRC_FSUB0
    */
    DL_Timer_configCrossTrigger(FOC_PWMA0_INST,DL_TIMER_CROSS_TRIG_SRC_FSUB0,
    DL_TIMER_CROSS_TRIGGER_INPUT_ENABLED,DL_TIMER_CROSS_TRIGGER_MODE_ENABLED);

    /* TSEL.ETSEL =0*/
    /* TSEL.TE =1 */
    DL_Timer_setExternalTriggerEvent(FOC_PWMA0_INST,DL_TIMER_EXT_TRIG_SEL_TRIG_0);
    DL_Timer_enableExternalTrigger(FOC_PWMA0_INST);

    /* FCTL.TFIM =1 */
    HAL_enableTriggerFault(FOC_PWMA0_INST);

    switch(vector)
    {
        case VOLTAGE_VECTOR_AC:

#if defined ADC0_CURRENT_W_CH
            IPD_ADC_init(ADC0_INST, ADC0_CURRENT_W_CH);
            DL_ADC12_configWinCompLowThld(ADC0_INST, pOffset->icOffset -WCompTh);
            DL_ADC12_startConversion(ADC0_INST);/* ADC conversion started */
#else
            IPD_ADC_init(ADC1_INST, ADC1_CURRENT_W_CH);
            DL_ADC12_configWinCompLowThld(ADC1_INST, pOffset->icOffset -WCompTh);
            DL_ADC12_startConversion(ADC1_INST);/* ADC conversion started */
#endif

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
            HAL_PWMForceLow(FOC_PWMA0_W_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_U_IDX);
            break;
        case VOLTAGE_VECTOR_CA:

#if defined ADC0_CURRENT_U_CH
            IPD_ADC_init(ADC0_INST, ADC0_CURRENT_U_CH);
            DL_ADC12_configWinCompLowThld(ADC0_INST, pOffset->iaOffset -WCompTh);
            DL_ADC12_startConversion(ADC0_INST);/* ADC conversion started */
#else
            IPD_ADC_init(ADC1_INST, ADC1_CURRENT_U_CH);
            DL_ADC12_configWinCompLowThld(ADC1_INST, pOffset->iaOffset -WCompTh);
            DL_ADC12_startConversion(ADC1_INST);/* ADC conversion started */
#endif

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
            HAL_PWMForceLow(FOC_PWMA0_U_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_W_IDX);
            break;
        case VOLTAGE_VECTOR_BC:

#if defined ADC0_CURRENT_W_CH
            IPD_ADC_init(ADC0_INST, ADC0_CURRENT_W_CH);
            DL_ADC12_configWinCompLowThld(ADC0_INST, pOffset->icOffset -WCompTh);
            DL_ADC12_startConversion(ADC0_INST);/* ADC conversion started */
#else
            IPD_ADC_init(ADC1_INST, ADC1_CURRENT_W_CH);
            DL_ADC12_configWinCompLowThld(ADC1_INST, pOffset->icOffset -WCompTh);
            DL_ADC12_startConversion(ADC1_INST);/* ADC conversion started */
#endif

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
            HAL_PWMForceLow(FOC_PWMA0_W_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_V_IDX);
            break;
        case VOLTAGE_VECTOR_CB:
                    
#if defined ADC0_CURRENT_V_CH
            IPD_ADC_init(ADC0_INST, ADC0_CURRENT_V_CH);
            DL_ADC12_configWinCompLowThld(ADC0_INST, pOffset->ibADC0Offset -WCompTh);
            DL_ADC12_startConversion(ADC0_INST);/* ADC conversion started */
#else
            IPD_ADC_init(ADC1_INST, ADC1_CURRENT_V_CH);
            DL_ADC12_configWinCompLowThld(ADC1_INST, pOffset->ibADC0Offset -WCompTh);
            DL_ADC12_startConversion(ADC1_INST);/* ADC conversion started */
#endif

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
            HAL_PWMForceLow(FOC_PWMA0_V_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_W_IDX);
            break;
        case VOLTAGE_VECTOR_BA:
        
#if defined ADC0_CURRENT_U_CH
            IPD_ADC_init(ADC0_INST, ADC0_CURRENT_U_CH);
            DL_ADC12_configWinCompLowThld(ADC0_INST, pOffset->iaOffset -WCompTh);
            DL_ADC12_startConversion(ADC0_INST);/* ADC conversion started */
#else
            IPD_ADC_init(ADC1_INST, ADC1_CURRENT_U_CH);
            DL_ADC12_configWinCompLowThld(ADC1_INST, pOffset->iaOffset -WCompTh);
            DL_ADC12_startConversion(ADC1_INST);/* ADC conversion started */
#endif

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
            HAL_PWMForceLow(FOC_PWMA0_U_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_V_IDX);
            break;
        case VOLTAGE_VECTOR_AB:

#if defined ADC0_CURRENT_V_CH
            IPD_ADC_init(ADC0_INST, ADC0_CURRENT_V_CH);
            DL_ADC12_configWinCompLowThld(ADC0_INST, pOffset->ibADC0Offset -WCompTh);
            DL_ADC12_startConversion(ADC0_INST);/* ADC conversion started */
#else
            IPD_ADC_init(ADC1_INST, ADC1_CURRENT_V_CH);
            DL_ADC12_configWinCompLowThld(ADC1_INST, pOffset->ibADC0Offset -WCompTh);
            DL_ADC12_startConversion(ADC1_INST);/* ADC conversion started */
#endif

            /* Disable PWM and Set the PWM to low */
            HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
            HAL_PWMForceLow(FOC_PWMA0_V_IDX);
            /* Start the timer */
            DL_Timer_startCounter(FOC_CAPTURE_IPD_INST);
            /*Set PWM high*/
            HAL_PWMForceHigh(FOC_PWMA0_U_IDX);
            break;
        default:
            /* This is expected to be empty */
            break;
    }

    HAL_IPDEnableISR();
    #endif
}
#endif

void HAL_setIPDPulseConfig(VOLTAGE_VECTOR vector, uint8_t CompVal, CURRENT_OFFSET_T* pOffset)
{
    #ifdef __CURRENT_SINGLE_SHUNT
        HAL_setIPDPulseConfig_1S( vector, CompVal, pOffset);
    #else
        HAL_setIPDPulseConfig_3S( vector, CompVal, pOffset);
    #endif
}

void HAL_IPDEnableISR(void)
{
#ifdef __IPD_ENABLE
    NVIC_EnableIRQ(FOC_CAPTURE_IRQN);
#endif
}

void HAL_IPDDisableISR(void)
{
#ifdef __IPD_ENABLE
    NVIC_DisableIRQ(FOC_CAPTURE_IRQN);
#endif
}

uint16_t HAL_IPDGetTime(void)
{
#ifdef __IPD_ENABLE
    return DL_Timer_getCaptureCompareValue(FOC_CAPTURE_IPD_INST, FOC_CAPTURE_IPD_IDX);
#else
    return 0;
#endif
}

uint16_t HAL_IPDGetLoadVal(void)
{
#ifdef __IPD_ENABLE
    return FOC_CAPTURE_IPD_LOAD_VALUE;
#else
    return 0;
#endif
}
uint16_t HAL_Is_SingleShunt(void)
{
#ifdef __CURRENT_SINGLE_SHUNT
    return TRUE;
#else
    return FALSE;
#endif
}
