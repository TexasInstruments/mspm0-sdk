/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "MSPM0_TIMER.h"

bool timeOutFlag = false;

void watchDog(uint32_t time)
{
    timeOutFlag = false;
    DL_Timer_setLoadValue(TIME_OUT_INST, time);
    DL_Timer_startCounter(TIME_OUT_INST);
    NVIC_ClearPendingIRQ(TIME_OUT_INST_INT_IRQN);
    NVIC_EnableIRQ(TIME_OUT_INST_INT_IRQN);
}

void watchDogStop(void)
{
    DL_Timer_stopCounter(TIME_OUT_INST);
    NVIC_DisableIRQ(TIME_OUT_INST_INT_IRQN);
}

void TIME_OUT_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIME_OUT_INST)) {
        case DL_TIMER_IIDX_ZERO:
            timeOutFlag = true;
            break;
        default:
            break;
    }
}
