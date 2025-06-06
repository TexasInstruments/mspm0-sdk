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
 *
 */


#include "dali_303_command.h"

/**
 * @brief   Configures DALI_Tx for response
 * 
 * @param[in]   respByte    Response Byte   
 */
static inline void DALI_setResponse(uint8_t respByte)
{
    dali_gpio.Tx.frame = BACKWARD_FRAME;
    dali_gpio.Tx.dataArr[0] = respByte;
    dali_gpio.Tx.toSend = true;
    dali_gpio.Tx.settlingTime = BACKWARD_FRAME_SETTLING_MIN;    
}

uint32_t DALI_303_getCounterValue(uint32_t time)
{
    return time/((uint32_t) TIMER_303_PERIOD);
}

void DALI_303_catchMovement(void)
{
    /* If Movement Event is Enabled, Ignore the Instruction */
    if(gControlDeviceVar1.instance[0].eventFilter & DALI_303_EVENT_FILTER_MOVEMENT)
    {
        return ;
    }

    gOccupancySensorVar1.catching = true;

    return;
}

void DALI_303_setHoldTimer(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    /* If DTR0 equals MASK, ignore the command */
    if(gControlDeviceVar1.DTR0 == MASK)
    {
        return;
    }

    gOccupancySensorVar1.tHold = gControlDeviceVar1.DTR0;
    
    g303_TimerCounter.holdCounter = DALI_303_getCounterValue((uint32_t)DALI_303_HOLD_INCREMENT * (uint32_t)gOccupancySensorVar1.tHold);
    
    DL_Timer_startCounter(TIMER_303_INST);

    return;
}

void DALI_303_setReportTimer(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gOccupancySensorVar1.tReport = gControlDeviceVar1.DTR0;
    
    g303_TimerCounter.reportCounter = DALI_303_getCounterValue((uint32_t)DALI_303_REPORT_INCREMENT * (uint32_t)gOccupancySensorVar1.tReport);

    DL_Timer_startCounter(TIMER_303_INST);
    
    return;
}

void DALI_303_setDeadtimeTimer(void)
{
    /* Check for Send Twice Commands */
    if(dali_gpio.Rx.isSendTwiceCmd == false)
    {
        return;
    }

    gOccupancySensorVar1.tDeadtime = gControlDeviceVar1.DTR0;
    
    g303_TimerCounter.deadtimeCounter = DALI_303_getCounterValue((uint32_t)DALI_303_DEADTIME_INCREMENT * (uint32_t)gOccupancySensorVar1.tDeadtime);

    gOccupancySensorVar1.isDeadtimeOn = true;

    DL_Timer_startCounter(TIMER_303_INST);

    return;
}

void DALI_303_cancelHoldTimer(void)
{
    if(g303_TimerCounter.holdCounter > 1)
    {
        g303_TimerCounter.holdCounter = 1;
    }

    return;
}

void DALI_303_queryHoldTimer(void)
{
    DALI_setResponse(gOccupancySensorVar1.tHold);

    return;
}

void DALI_303_queryReportTimer(void)
{
    DALI_setResponse(gOccupancySensorVar1.tReport);

    return;
}

void DALI_303_queryDeadtimeTimer(void)
{
    DALI_setResponse(gOccupancySensorVar1.tDeadtime);

    return;
}

void DALI_303_queryCatching(void)
{
    if(gOccupancySensorVar1.catching)
    {
        DALI_setResponse(YES);
    }

    return;
}
