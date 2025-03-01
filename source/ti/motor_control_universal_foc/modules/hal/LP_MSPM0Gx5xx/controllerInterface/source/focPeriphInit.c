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
#include "focPeriphInit.h"
#include "gateDriver.h"
#include "main.h"

/**
 * @brief     PWM configurations for FOC
 */
static void PWMConfig(void)
{

    #ifndef __CURRENT_SINGLE_SHUNT

    /* suppress normal events, this is done to get the events 
        using the repeat counter */
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_0_INDEX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_1_INDEX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_2_INDEX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_3_INDEX);
    DL_Timer_enableLZEventSuppression(FOC_PWMA0_INST);

    /* Set the action for the capture compare channels */
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH | DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        DL_TIMER_CC_0_INDEX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH| DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        DL_TIMER_CC_1_INDEX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH| DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        DL_TIMER_CC_2_INDEX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH| DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        DL_TIMER_CC_3_INDEX);
        
    #else

    /* Fix for cross trigger bug */
    DL_TimerA_setCaptureCompareInput(PWMA0_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_0_INDEX);
    DL_TimerA_setCaptureCompareCtl(FOC_PWMA0_INST,DL_TIMER_CC_MODE_COMPARE,DL_TIMER_CC_LCOND_TRIG_RISE,DL_TIMER_CC_0_INDEX);

    DL_TimerA_setCaptureCompareInput(FOC_TIMA1_INST, DL_TIMER_CC_INPUT_INV_NOINVERT, DL_TIMER_CC_IN_SEL_TRIG, DL_TIMER_CC_0_INDEX);
    DL_TimerA_setCaptureCompareCtl(FOC_TIMA1_INST,DL_TIMER_CC_MODE_COMPARE,DL_TIMER_CC_LCOND_TRIG_RISE,DL_TIMER_CC_0_INDEX);

    /* Set the action for the capture compare channels */
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_LACT_DISABLED | DL_TIMER_CC_CDACT_CCP_LOW | DL_TIMER_CC_CUACT_DISABLED| DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        FOC_PWMA0_W_IDX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_LACT_DISABLED | DL_TIMER_CC_CDACT_CCP_LOW | DL_TIMER_CC_CUACT_DISABLED| DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        FOC_PWMA0_U_IDX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_LACT_DISABLED |DL_TIMER_CC_CDACT_CCP_LOW | DL_TIMER_CC_CUACT_DISABLED| DL_TIMER_CC_FEXACT_LOW | DL_TIMER_CC_FENACT_CCP_LOW),
        FOC_PWMA0_V_IDX);

    DL_Timer_setSecondCompActionDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_ACT_SEL_HIGH,FOC_PWMA0_U_IDX);
    DL_Timer_setSecondCompActionDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_ACT_SEL_HIGH,FOC_PWMA0_V_IDX);
    DL_Timer_setSecondCompActionDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_ACT_SEL_HIGH,FOC_PWMA0_W_IDX);

    /* configuration for second capture compare  */
    if (FOC_PWMA0_U_B_IDX == DL_TIMER_CC_0_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC0,FOC_PWMA0_U_IDX);
    }
    else if (FOC_PWMA0_U_B_IDX == DL_TIMER_CC_1_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC1,FOC_PWMA0_U_IDX);
    }
    else if (FOC_PWMA0_U_B_IDX == DL_TIMER_CC_2_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC2,FOC_PWMA0_U_IDX);
    }
    else if (FOC_PWMA0_U_B_IDX == DL_TIMER_CC_3_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC3,FOC_PWMA0_U_IDX);
    }
    else if (FOC_PWMA0_U_B_IDX == DL_TIMER_CC_4_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC4,FOC_PWMA0_U_IDX);
    }
    else if (FOC_PWMA0_U_B_IDX == DL_TIMER_CC_5_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC5,FOC_PWMA0_U_IDX);
    }

    if (FOC_PWMA0_V_B_IDX == DL_TIMER_CC_0_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC0,FOC_PWMA0_V_IDX);
    }
    else if (FOC_PWMA0_V_B_IDX == DL_TIMER_CC_1_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC1,FOC_PWMA0_V_IDX);
    }
    else if (FOC_PWMA0_V_B_IDX == DL_TIMER_CC_2_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC2,FOC_PWMA0_V_IDX);
    }
    else if (FOC_PWMA0_V_B_IDX == DL_TIMER_CC_3_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC3,FOC_PWMA0_V_IDX);
    }
    else if (FOC_PWMA0_V_B_IDX == DL_TIMER_CC_4_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC4,FOC_PWMA0_V_IDX);
    }
    else if (FOC_PWMA0_V_B_IDX == DL_TIMER_CC_5_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC5,FOC_PWMA0_V_IDX);
    }

    if (FOC_PWMA0_W_B_IDX == DL_TIMER_CC_0_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC0,FOC_PWMA0_W_IDX);
    }
    else if (FOC_PWMA0_W_B_IDX == DL_TIMER_CC_1_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC1,FOC_PWMA0_W_IDX);
    }
    else if (FOC_PWMA0_W_B_IDX == DL_TIMER_CC_2_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC2,FOC_PWMA0_W_IDX);
    }
    else if (FOC_PWMA0_W_B_IDX == DL_TIMER_CC_3_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC3,FOC_PWMA0_W_IDX);
    }
    else if (FOC_PWMA0_W_B_IDX == DL_TIMER_CC_4_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC4,FOC_PWMA0_W_IDX);
    }
    else if (FOC_PWMA0_W_B_IDX == DL_TIMER_CC_5_INDEX)
    {
        DL_Timer_setSecondCompSrcDn(FOC_PWMA0_INST,DL_TIMER_SEC_COMP_DOWN_EVT_SEL_CC5,FOC_PWMA0_W_IDX);
    }

    DL_Timer_setCaptCompUpdateMethod(FOC_PWMA0_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_PWMA0_W_IDX);
    DL_Timer_setCaptCompUpdateMethod(FOC_PWMA0_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_PWMA0_U_IDX);
    DL_Timer_setCaptCompUpdateMethod(FOC_PWMA0_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_PWMA0_V_IDX);

    DL_Timer_setCaptCompUpdateMethod(FOC_PWMA0_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_PWMA0_V_B_IDX);
    DL_Timer_setCaptCompUpdateMethod(FOC_PWMA0_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_PWMA0_W_B_IDX);
    DL_Timer_setCaptCompUpdateMethod(FOC_PWMA0_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_PWMA0_U_B_IDX);

    DL_TimerA_enableSuppressionOfCompEvent(FOC_TIMA1_INST,FOC_ADC_TRIG_0_IDX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_TIMA1_INST,FOC_ADC_TRIG_1_IDX);
    
    DL_Timer_setCaptCompUpdateMethod(FOC_TIMA1_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_ADC_TRIG_0_IDX);
    DL_Timer_setCaptCompUpdateMethod(FOC_TIMA1_INST,DL_TIMER_CC_UPDATE_METHOD_ZERO_RC_EVT,FOC_ADC_TRIG_1_IDX);

    DL_TimerA_setCaptureCompareValue(
                    FOC_TIMA1_INST, 1, FOC_ADC_TRIG_0_IDX);
    DL_TimerA_setCaptureCompareValue(
                    FOC_TIMA1_INST, (DL_Timer_getLoadValue(FOC_TIMA1_INST)>>1), FOC_ADC_TRIG_1_IDX);

   /* Generate cross trigger */
   DL_TimerA_generateCrossTrigger(FOC_PWMA0_INST);

    #endif        

}

/**
 * @brief     Interrupt configurations for FOC
 */
static void interruptsConfig(void)
{
    /* Enable interrupts */
#ifndef FOC_ISR_ADC1
    NVIC_EnableIRQ(ADC0_INST_INT_IRQN);
#else
    NVIC_EnableIRQ(ADC1_INST_INT_IRQN);
#endif
    NVIC_EnableIRQ(FOC_TIMG0_INST_INT_IRQN);
    NVIC_EnableIRQ(DMA_INT_IRQn);

    /* Set the priority for interrupts */
#ifndef FOC_ISR_ADC1
    NVIC_SetPriority(ADC0_INST_INT_IRQN, 1);
#else
    NVIC_SetPriority(ADC1_INST_INT_IRQN, 1);
#endif
    NVIC_SetPriority(FOC_TIMG0_INST_INT_IRQN, 3);
    NVIC_SetPriority(DMA_INT_IRQn, 4);
#ifdef __IPD_ENABLE
    NVIC_SetPriority(CAPTURE_INST_INT_IRQN, 0);
#endif
}

void focPeriphInit(void)
{
   PWMConfig();
   interruptsConfig();
}
