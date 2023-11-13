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
/*!****************************************************************************
 *  @file       hal_timer.c
 *  @brief      SENT HAL TIMER source file
 ******************************************************************************/

#include "hal_timer.h"

/*
 * Timer clock configuration to be sourced by  / 1 (4000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   4000000 Hz = 4000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_MFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 0U};

static const DL_TimerG_PWMConfig gPWM_0Config = {
    .pwmMode    = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period     = 0,
    .startTimer = DL_TIMER_STOP,
};

void HAL_Timer_init(uint16_t load, uint16_t capture)
{
    DL_TimerG_reset(PWM_0_INST);
    DL_TimerG_enablePower(PWM_0_INST);

    DL_TimerG_setClockConfig(
        PWM_0_INST, (DL_TimerG_ClockConfig *) &gPWM_0ClockConfig);

    DL_TimerG_initPWMMode(PWM_0_INST, (DL_TimerG_PWMConfig *) &gPWM_0Config);

    DL_TimerG_setLoadValue(PWM_0_INST, load);

    DL_TimerG_setCaptureCompareValue(PWM_0_INST, capture, PWM_0_CHAN_SEL);

    DL_TimerG_setCaptureCompareOutCtl(PWM_0_INST,
        DL_TIMER_CC_OCTL_INIT_VAL_HIGH, DL_TIMER_CC_OCTL_INV_OUT_ENABLED,
        DL_TIMER_CC_OCTL_SRC_FUNCVAL, PWM_0_CHAN_SEL);

    DL_TimerG_setCaptCompUpdateMethod(
        PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, PWM_0_CHAN_SEL);

    DL_TimerG_setCCPDirection(PWM_0_INST, PWM_0_CHAN_OUT);

    DL_TimerG_enableInterrupt(PWM_0_INST, (DL_TIMER_INTERRUPT_ZERO_EVENT));

    DL_TimerG_enableClock(PWM_0_INST);
}

void HAL_Timer_start(void)
{
    DL_TimerG_clearInterruptStatus(PWM_0_INST, 0xFFFFFFFF);

    NVIC_EnableIRQ(PWM_0_INST_INT_IRQN);

    DL_TimerG_startCounter(PWM_0_INST);
}

void HAL_Timer_updateLoadVal(uint16_t loadVal)
{
    DL_TimerG_setLoadValue(PWM_0_INST, loadVal);
}
