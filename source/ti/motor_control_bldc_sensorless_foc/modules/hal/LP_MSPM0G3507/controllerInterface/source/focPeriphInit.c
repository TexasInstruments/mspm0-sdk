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
    /* Set the action for the capture compare channels */
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH),
        DL_TIMER_CC_0_INDEX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH),
        DL_TIMER_CC_1_INDEX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH),
        DL_TIMER_CC_2_INDEX);
    DL_Timer_setCaptureCompareAction(FOC_PWMA0_INST,
        (DL_TIMER_CC_CUACT_CCP_LOW | DL_TIMER_CC_CDACT_CCP_HIGH),
        DL_TIMER_CC_3_INDEX);
        
    /* suppress normal events, this is done to get the events 
        using the repeat counter */
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_0_INDEX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_1_INDEX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_2_INDEX);
    DL_TimerA_enableSuppressionOfCompEvent(FOC_PWMA0_INST,DL_TIMER_CC_3_INDEX);
}

/**
 * @brief     Interrupt configurations for FOC
 */
static void interruptsConfig(void)
{
    /* Enable interrupts */
    NVIC_EnableIRQ(ADC0_INST_INT_IRQN);
    NVIC_EnableIRQ(FOC_TIMG0_INST_INT_IRQN);
    NVIC_EnableIRQ(DMA_INT_IRQn);

    /* Set the priority for interrupts */
    NVIC_SetPriority(ADC0_INST_INT_IRQN, 1);
    NVIC_SetPriority(FOC_TIMG0_INST_INT_IRQN, 3);
    NVIC_SetPriority(DMA_INT_IRQn, 4);
    NVIC_SetPriority(CAPTURE_INST_INT_IRQN, 0);
}

void focPeriphInit(void)
{
   PWMConfig();
   interruptsConfig();
}
