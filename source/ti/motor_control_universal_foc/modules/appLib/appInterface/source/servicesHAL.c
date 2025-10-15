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

#include "servicesHAL.h"
#include "focPeriphInit.h"
#include "gateDriver.h"

void HAL_setDeadTime(uint32_t value)
{
    /* Configure Dead-time for PWM */
    uint16_t deadTimeVal;

    deadTimeVal = (value) << 2;

    DL_TimerA_setDeadBand(FOC_PWMA0_INST, deadTimeVal, deadTimeVal,
                            DL_TIMER_DEAD_BAND_MODE_0);
}

#ifdef DRV8316

int32_t gateDriverGetMinOTimeSineMod(void)
{
    int32_t minONtime_ns;

    if(pGateDriverConfig->gateDrvCfg2.b.minOnTime == 0)
     {
         minONtime_ns = 167;
     }
     else if(pGateDriverConfig->gateDrvCfg2.b.minOnTime == 1)
     {
        switch(pGateDriverConfig->gateDrvCfg1.b.slewRate)
        {
        case 0:
            minONtime_ns = 4800;
            break;
        case 1:
            minONtime_ns = 2200;
            break;
        case 2:
            minONtime_ns = 1400;
            break;
        case 3:
            minONtime_ns = 1100;
            break;
        default:
            minONtime_ns = DRIVER_PROPAGATION_DELAY_nS;
            break;
        }
     }
     else if(pGateDriverConfig->gateDrvCfg2.b.minOnTime <= 6)
     {
        minONtime_ns = (pGateDriverConfig->gateDrvCfg2.b.minOnTime-2)*250 + 500;
     }
     else
     {
        minONtime_ns = 2000;
     }
    return minONtime_ns;
}

int32_t gateDriverGetPropagationDelay(void)
{
    int32_t propagationDelay_nS;
    if(pGateDriverConfig->gateDrvCfg2.b.delayCompEn)
    {
        switch(pGateDriverConfig->gateDrvCfg1.b.slewRate)
        {
        case 0:
            propagationDelay_nS = 3000;
            break;
        case 1:
            propagationDelay_nS = 1300;
            break;
        case 2:
            propagationDelay_nS = 950;
            break;
        case 3:
            propagationDelay_nS = 700;
            break;
        default:
            propagationDelay_nS = DRIVER_PROPAGATION_DELAY_nS;
            break;
        }
    }
    else
    {
        propagationDelay_nS = DRIVER_PROPAGATION_DELAY_nS;
    }
    return propagationDelay_nS;
}

#endif
#if !defined(DRV8316)

int32_t gateDriverGetPropagationDelay(void)
{
    return DRIVER_PROPAGATION_DELAY_nS;
}

int32_t gateDriverGetMinOTimeSineMod(void)
{

    return DRIVER_MIN_ON_TIME_nS;
}

#endif

int32_t gateDriverGetMinOnTimeOverMod(void)
{                  
    return ((DISCONTINUOUS_MIN_DUTY_COUNTS * 1000) / CPU_FREQUENCY_MHZ);
}
