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


#include "watchdog_hal.h"
#include <stdio.h>


void watchdog_start(void)
{
    /* disable WDT feed pin until WDT started */
    DL_GPIO_disableInterrupt(GPIOA, WDT_Start_PIN);
    DL_GPIO_clearInterruptStatus(GPIOA, WDT_WDI_PIN |WDT_Start_PIN);

    /* Enable WDT Feed pin */
    DL_GPIO_enableInterrupt(GPIOA, WDT_WDI_PIN);
    NVIC_EnableIRQ(watchdog_timer_INST_INT_IRQN);

    DL_Timer_setLoadValue(watchdog_timer_INST, WATCHDOG_PERIOD-1);
    DL_TimerG_startCounter(watchdog_timer_INST);
}

void watchdog_timeout(void)
{
    DL_TimerG_stopCounter(watchdog_timer_INST);
    DL_GPIO_disableInterrupt(GPIOA, WDT_WDI_PIN);

    /* Toggle WDT output fail signal */
    DL_GPIO_clearPins(WDT_PORT, WDT_WDO_PIN); //output low
    delay_cycles(WDO_RESET_TIME);
    DL_GPIO_setPins(WDT_PORT, WDT_WDO_PIN);   //output high

    /* enable WDT start pin */
    DL_GPIO_clearInterruptStatus(GPIOA, WDT_WDI_PIN |WDT_Start_PIN);
    DL_GPIO_enableInterrupt(GPIOA, WDT_Start_PIN);
}

void watchdog_reload(void)
{
    DL_TimerG_stopCounter(watchdog_timer_INST);
    DL_Timer_setLoadValue(watchdog_timer_INST, WATCHDOG_PERIOD-1);
    DL_TimerG_startCounter(watchdog_timer_INST);
}

