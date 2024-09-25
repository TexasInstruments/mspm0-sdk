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
}

void HAL_EnablePWM(void)
{
    /* sets forced PWM generation to disabled */
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_U_IDX);
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_V_IDX);
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
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
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_U_IDX);
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_V_IDX);
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                    DL_TIMER_FORCE_CMPL_OUT_DISABLED, FOC_PWMA0_W_IDX);

}

void HAL_EnableLowSideBrake(void)
{
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                            DL_TIMER_FORCE_CMPL_OUT_HIGH, FOC_PWMA0_U_IDX);
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                            DL_TIMER_FORCE_CMPL_OUT_HIGH, FOC_PWMA0_V_IDX);
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                            DL_TIMER_FORCE_CMPL_OUT_HIGH, FOC_PWMA0_W_IDX);
}

void HAL_UpdateDuty(const MC_ABC_T *pABC, const int32_t FullDuty)
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
        DL_TimerA_setCaptureCompareValue(
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
        DL_TimerA_setCaptureCompareValue(
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
        DL_TimerA_setCaptureCompareValue(
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

void HAL_SetPWMFrequency(const int32_t pwmPeriod,
                         const int32_t propagationDelay)
{
    uint32_t triggerPoint;

    /* period is divided by 2 for up-down counter mode */
    DL_Timer_setLoadValue(FOC_PWMA0_INST, pwmPeriod >> 1);

    /* period is divided by 2 for up-down counter mode */
    DL_TimerA_setCaptureCompareValue(
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

    /* Generate Trigger for ADC Conversion on PWMA0 at Trigger Point */
    DL_TimerA_enableEvent(FOC_PWMA0_INST, DL_TIMERA_EVENT_ROUTE_1,
                        triggerPoint);
    DL_TimerA_setPublisherChanID(FOC_PWMA0_INST, DL_TIMERA_PUBLISHER_INDEX_0,
                        ADC_PWM_TRIG_CH);
}

void HAL_SetADCSamplingFrequency(const ADC_SAMPLING_RATIO_TYPES samplingRatio)
{
    DL_TimerA_setRepeatCounter(FOC_PWMA0_INST, samplingRatio);
   DL_ADC12_setSubscriberChanID(FOC_ADC0_INST, ADC_PWM_TRIG_CH);
   DL_ADC12_setSubscriberChanID(FOC_ADC1_INST, ADC_PWM_TRIG_CH);
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
    DL_TimerA_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_U_IDX);
    DL_TimerA_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_V_IDX);
    DL_TimerA_setCaptureCompareValue(
                        FOC_PWMA0_INST, 0, FOC_PWMA0_W_IDX);
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

void HAL_setIPDPulseConfig(VOLTAGE_VECTOR vector, uint8_t CompVal)
{
    /* setting minimum compare value to 2 */
    if(CompVal <= 2)
    {
        CompVal += 2; 
    }
    CompVal = 127 - CompVal;
    DL_Timer_clearInterruptStatus(FOC_CAPTURE_IPD_INST, 
                                            DL_TIMER_INTERRUPT_CC0_DN_EVENT);
    
    switch(vector)
    {
        case VOLTAGE_VECTOR_AC:
            DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST,
                                DL_TIMER_SUBSCRIBER_INDEX_0, FOC_COMP_W_PUB_CH);
            DL_COMP_setOutputInterruptEdge(COMP_W_INST, 
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
            DL_COMP_setDACCode0(FOC_COMP_W_INST, CompVal);
            DL_COMP_setDACCode0(FOC_COMP_U_INST, 0);
            DL_COMP_setDACCode0(FOC_COMP_V_INST, 0);
            HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
            HAL_PWMForceLow(FOC_PWMA0_W_IDX);
            DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
            HAL_PWMForceHigh(FOC_PWMA0_U_IDX);
            break;
        case VOLTAGE_VECTOR_CA:
            DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                            DL_TIMER_SUBSCRIBER_INDEX_0,
                                         FOC_COMP_U_PUB_CH);
            DL_COMP_setOutputInterruptEdge(COMP_U_INST, 
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
            DL_COMP_setDACCode0(FOC_COMP_U_INST, CompVal);
            DL_COMP_setDACCode0(FOC_COMP_V_INST, 0);
            DL_COMP_setDACCode0(FOC_COMP_W_INST, 0);
            HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
            HAL_PWMForceLow(FOC_PWMA0_U_IDX);
            DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
            HAL_PWMForceHigh(FOC_PWMA0_W_IDX);
            break;
        case VOLTAGE_VECTOR_BC:
            DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                            DL_TIMER_SUBSCRIBER_INDEX_0,
                                         FOC_COMP_W_PUB_CH);
            DL_COMP_setOutputInterruptEdge(COMP_W_INST, 
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
            DL_COMP_setDACCode0(FOC_COMP_W_INST, CompVal);
            DL_COMP_setDACCode0(FOC_COMP_V_INST, 0);
            DL_COMP_setDACCode0(FOC_COMP_U_INST, 0);
            HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
            HAL_PWMForceLow(FOC_PWMA0_W_IDX);
            DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
            HAL_PWMForceHigh(FOC_PWMA0_V_IDX);
            break;
        case VOLTAGE_VECTOR_CB:
            DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                            DL_TIMER_SUBSCRIBER_INDEX_0,
                                         FOC_COMP_V_PUB_CH);
            DL_COMP_setOutputInterruptEdge(COMP_V_INST, 
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
            DL_COMP_setDACCode0(FOC_COMP_V_INST, CompVal);
            DL_COMP_setDACCode0(FOC_COMP_U_INST, 0);
            DL_COMP_setDACCode0(FOC_COMP_W_INST, 0);
            HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
            HAL_PWMForceLow(FOC_PWMA0_V_IDX);
            DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
            HAL_PWMForceHigh(FOC_PWMA0_W_IDX);
            break;
        case VOLTAGE_VECTOR_BA:
            DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                            DL_TIMER_SUBSCRIBER_INDEX_0,
                                         FOC_COMP_U_PUB_CH);
            DL_COMP_setOutputInterruptEdge(COMP_U_INST, 
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
            DL_COMP_setDACCode0(FOC_COMP_U_INST, CompVal);
            DL_COMP_setDACCode0(FOC_COMP_V_INST, 0);
            DL_COMP_setDACCode0(FOC_COMP_W_INST, 0);
            HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
            HAL_PWMForceLow(FOC_PWMA0_U_IDX);
            DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
            HAL_PWMForceHigh(FOC_PWMA0_V_IDX);
            break;
        case VOLTAGE_VECTOR_AB:
            DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                            DL_TIMER_SUBSCRIBER_INDEX_0,
                                         FOC_COMP_V_PUB_CH);
            DL_COMP_setOutputInterruptEdge(COMP_V_INST, 
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
            DL_COMP_setDACCode0(FOC_COMP_V_INST, CompVal);
            DL_COMP_setDACCode0(FOC_COMP_U_INST, 0);
            DL_COMP_setDACCode0(FOC_COMP_W_INST, 0);
            HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
            HAL_PWMForceLow(FOC_PWMA0_V_IDX);
            DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
            HAL_PWMForceHigh(FOC_PWMA0_U_IDX);
            break;
        default:
            /* This is expected to be empty */
            break;
    }
    HAL_IPDEnableISR();
}

void HAL_IPDT1Action(VOLTAGE_VECTOR vector, uint8_t CompVal, 
                                                    uint8_t fastDecayEnable)
{
    /* setting minimum compare value to 1 */
    if(CompVal == 0)
    {
        CompVal = 1; 
    }
    
    if(fastDecayEnable)
    {
        CompVal = 127 + CompVal;
        switch(vector)
        {
            case VOLTAGE_VECTOR_AC:
                DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                                DL_TIMER_SUBSCRIBER_INDEX_0, FOC_COMP_U_PUB_CH);
                DL_COMP_setOutputInterruptEdge(COMP_U_INST,
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
                DL_COMP_setDACCode0(FOC_COMP_U_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceLow(FOC_PWMA0_U_IDX);
                HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
                break;
            case VOLTAGE_VECTOR_CA:
                DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                                DL_TIMER_SUBSCRIBER_INDEX_0, FOC_COMP_W_PUB_CH);
                DL_COMP_setOutputInterruptEdge(COMP_W_INST,
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
                DL_COMP_setDACCode0(FOC_COMP_W_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceLow(FOC_PWMA0_W_IDX);
                HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
                break;
            case VOLTAGE_VECTOR_BC:
                DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                                DL_TIMER_SUBSCRIBER_INDEX_0, FOC_COMP_V_PUB_CH);
                DL_COMP_setOutputInterruptEdge(COMP_V_INST,
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
                DL_COMP_setDACCode0(FOC_COMP_V_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceLow(FOC_PWMA0_V_IDX);
                HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
                break;
            case VOLTAGE_VECTOR_CB:
                DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                                DL_TIMER_SUBSCRIBER_INDEX_0, FOC_COMP_W_PUB_CH);
                DL_COMP_setOutputInterruptEdge(COMP_W_INST,
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
                DL_COMP_setDACCode0(FOC_COMP_W_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceLow(FOC_PWMA0_W_IDX);
                HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
                break;
            case VOLTAGE_VECTOR_BA:
                DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                                DL_TIMER_SUBSCRIBER_INDEX_0, FOC_COMP_V_PUB_CH);
                DL_COMP_setOutputInterruptEdge(COMP_V_INST,
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
                DL_COMP_setDACCode0(FOC_COMP_V_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceLow(FOC_PWMA0_V_IDX);
                HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
                break;
            case VOLTAGE_VECTOR_AB:
                DL_Timer_setSubscriberChanID(FOC_CAPTURE_IPD_INST, 
                                DL_TIMER_SUBSCRIBER_INDEX_0,FOC_COMP_U_PUB_CH);
                DL_COMP_setOutputInterruptEdge(COMP_U_INST,
                                            DL_COMP_OUTPUT_INT_EDGE_FALLING);
                DL_COMP_setDACCode0(FOC_COMP_U_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceLow(FOC_PWMA0_U_IDX);
                HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
                break;
            default:
                /* This is expected to be empty */
                break;
        }
    }
    else
    {
        CompVal = 127 - CompVal;
        switch(vector)
        {
            case VOLTAGE_VECTOR_AC:
                DL_COMP_setOutputInterruptEdge(COMP_W_INST,
                                                DL_COMP_OUTPUT_INT_EDGE_RISING);
                DL_COMP_setDACCode0(FOC_COMP_W_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
                break;
            case VOLTAGE_VECTOR_CA:
                DL_COMP_setOutputInterruptEdge(COMP_U_INST,
                                                DL_COMP_OUTPUT_INT_EDGE_RISING);
                DL_COMP_setDACCode0(FOC_COMP_U_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
                break;
            case VOLTAGE_VECTOR_BC:
                DL_COMP_setOutputInterruptEdge(COMP_W_INST,
                                                DL_COMP_OUTPUT_INT_EDGE_RISING);
                DL_COMP_setDACCode0(FOC_COMP_W_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
                break;
            case VOLTAGE_VECTOR_CB:
                DL_COMP_setOutputInterruptEdge(COMP_V_INST,
                                                DL_COMP_OUTPUT_INT_EDGE_RISING);
                DL_COMP_setDACCode0(FOC_COMP_V_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceDisable(FOC_PWMA0_W_IDX);
                break;
            case VOLTAGE_VECTOR_BA:
                DL_COMP_setOutputInterruptEdge(COMP_U_INST,
                                                DL_COMP_OUTPUT_INT_EDGE_RISING);
                DL_COMP_setDACCode0(FOC_COMP_U_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceDisable(FOC_PWMA0_V_IDX);
                break;
            case VOLTAGE_VECTOR_AB:
                DL_COMP_setOutputInterruptEdge(COMP_V_INST,
                                                DL_COMP_OUTPUT_INT_EDGE_RISING);
                DL_COMP_setDACCode0(FOC_COMP_V_INST, CompVal);
                DL_Timer_setTimerCount(FOC_CAPTURE_IPD_INST,
                                                    FOC_CAPTURE_IPD_LOAD_VALUE);
                HAL_PWMForceDisable(FOC_PWMA0_U_IDX);
                break;
            default:
                /* This is expected to be empty */
                break;
        }
    }
}

void HAL_IPDEnableISR(void)
{
    NVIC_EnableIRQ(FOC_CAPTURE_IRQN);
}

void HAL_IPDDisableISR(void)
{
    NVIC_DisableIRQ(FOC_CAPTURE_IRQN);
}

uint16_t HAL_IPDGetTime(void)
{
    return DL_Timer_getCaptureCompareValue(FOC_CAPTURE_IPD_INST, FOC_CAPTURE_IPD_IDX);
}

void HAL_COMPInit(void)
{
    DL_COMP_setDACCode0(FOC_COMP_W_INST, 0);
    DL_COMP_setDACCode0(FOC_COMP_U_INST, 0);
    DL_COMP_setDACCode0(FOC_COMP_V_INST, 0);
}
