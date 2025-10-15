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
#include "ipd.h"

#include <string.h>

static void ipdVectorAnglesInit(IPD_T *);
static void ipdClearTimes(IPD_T *);
static void ipdAdjacentVectorsInit(IPD_T *);

/* Angle interpolation coefficients */
#define ANGLE_LINEAR_COEFF      _IQ(31.25/360.0)
#define ANGLE_CONSTANT_COEFF    _IQ(30.64375/360.0)

/* Vector Positions */
#define IPD_THIRTY_DEGREES          _IQ(30.0/360.0)
#define IPD_NINETY_DEGREES          _IQ(90.0/360.0)
#define IPD_ONE_FIFTY_DEGREES       _IQ(150.0/360.0)
#define IPD_TWO_TEN_DEGREES         _IQ(210.0/360.0)
#define IPD_TWO_SEVENTY_DEGREES     _IQ(270.0/360.0)
#define IPD_THREE_THIRTY_DEGREES    _IQ(330.0/360.0)

/* Frequency of the capture timer used for IPD */
#define IPD_TIMER_FREQ_KHZ (80000)

/* Flag for ipd runnning state */
volatile uint8_t ipdRunning = 0;

/* IPD risetime */
volatile int32_t ipdRiseTime = 0;

/* PWM low duration counter */
uint8_t PWMLCounter = 0;

void ipdInit(IPD_T *pIpd)
{
    pIpd->flags.b.status = FALSE;

    pIpd->cycleCount = 0;

    pIpd->pulseToPulseCount = 0;

    pIpd->minRiseTime = _IQ(1.0);

    pIpd->vector = VOLTAGE_VECTOR_AC;

    ipdRiseTime = 0;

    ipdClearTimes(pIpd);
    ipdAdjacentVectorsInit(pIpd);
    ipdVectorAnglesInit(pIpd);
    HAL_IPDDisableISR();
    
}

uint32_t ipdTick=0;

void ipdRun(IPD_T *pIpd)
{
    ipdTick++;
    int32_t
        angleFunctionRatio,
        angleAdjust;

    switch(pIpd->state)
    {
        case IPD_INIT:
            ipdInit(pIpd);

            HAL_PWMAllForceLow();

            if(++PWMLCounter > 200)
            {
                HAL_PWMAllForceDisable();
                PWMLCounter=0;
                pIpd->state = IPD_VECTOR_CONFIG;
            }
        break;

        case IPD_VECTOR_CONFIG:
            pIpd->waitState = WAIT_GET_T1;
            
            HAL_enterIPDMode();
            ipdRunning = 1;
            pIpd->overflowCnt = 0;
            pIpd->state = IPD_RUN;
            HAL_setIPDPulseConfig(pIpd->vector, pIpd->currentLimitHi, pIpd->pOffset);

            /* CPU kept in loop while IPD runs
            This is done to prevent any other application from using the ADC 
            while the IPD runs */
            while(ipdRunning)
            {
                /* This is intentionally kept empty */
            };

            /* set the pulseToPulse to the riseTime */
            pIpd->pulseToPulseCount = ((ipdRiseTime*IPD_EXECUTION_FREQUENCY_KHZ)/IPD_TIMER_FREQ_KHZ);
        break;

        case IPD_RUN:
            pIpd->pulseToPulseCount++;
        break;

        case IPD_WAIT_COMPLETE:
            pIpd->pulseToPulseCount++;

            /* decay current duration is 2 times the rise time */
            if(pIpd->pulseToPulseCount >= ((3*ipdRiseTime*IPD_EXECUTION_FREQUENCY_KHZ)/IPD_TIMER_FREQ_KHZ))
            {
                pIpd->state = IPD_SET_NEXT_VECTOR;
            }
        break;

        case IPD_SET_NEXT_VECTOR:
            pIpd->pulseToPulseCount++;
            pIpd->vector++;
            if(pIpd->vector >= VOLTAGE_VECTOR_MAX)
            {
                pIpd->vector = 0;
                pIpd->cycleCount++;
            }

            if(pIpd->cycleCount >= pIpd->cycleCountMax)
            {

                HAL_PWMAllForceDisable();
                pIpd->state = IPD_CALC_SECTOR_RISE;
            }
            else
            {
                if(pIpd->pulseToPulseCount < pIpd->pulseToPulseCountMax)
                {
                    pIpd->state = IPD_WAIT_PULSE;
                }
                else
                {
                    if(pIpd->pulseToPulseCount > pIpd->pulseToPulseCountMax)
                    {
                        pIpd->faultStatus = IPD_FAULT_DECAY_TIME;
                        pIpd->state = IPD_FAULT;    
                    }
                    else
                    {
                        pIpd->pulseToPulseCount = 0;
                        pIpd->state = IPD_VECTOR_CONFIG;
                    }
                }
            }
        break;

        case IPD_WAIT_PULSE:
            pIpd->pulseToPulseCount++;

            if(pIpd->pulseToPulseCount >= pIpd->pulseToPulseCountMax)
            {
                pIpd->pulseToPulseCount = 0;
                pIpd->state = IPD_VECTOR_CONFIG;
            }
        break;

        case IPD_CALC_SECTOR_RISE:
            pIpd->minRiseTime = pIpd->timeRise[0];
            pIpd->noRotationVectorCount = 0;
            for(int8_t vector = 0; vector < VOLTAGE_VECTOR_MAX; vector++)
            {
                if( pIpd->timeRise[vector] < pIpd->minRiseTime)
                {
                    pIpd->minRiseTime = pIpd->timeRise[vector];
                    
                    pIpd->noRotationVectorCount = vector;
                }
            }
            pIpd->state = IPD_CALC_ROTOR_POSITION;
        break;

        case IPD_CALC_ROTOR_POSITION:
            pIpd->rotorPosition =
                        pIpd->vectorAngle[pIpd->noRotationVectorCount];

            pIpd->state = IPD_CALC_ANGLE;
        break;

        case IPD_CALC_ANGLE:
            if(*(pIpd->pTargetVelocity) > 0)
            {
                pIpd->angle = pIpd->rotorPosition -
                                    (IPD_NINETY_DEGREES - pIpd->advanceAngle);
            }
            else
            {
                pIpd->angle = pIpd->rotorPosition +
                                    (IPD_NINETY_DEGREES - pIpd->advanceAngle);
            }
            pIpd->state = IPD_COMPLETE;
        break;

        case IPD_COMPLETE:
        
            
            if(!pIpd->flags.b.forceEnable)
            {
                pIpd->flags.b.status = TRUE;
            }
        break;

        case IPD_FAULT:
        /* This is intentionally kept empty */
        break;

        default:
            pIpd->state = IPD_INIT;
        break;
    }
}


static void ipdAdjacentVectorsInit(IPD_T *pIpd)
{
    pIpd->adjacentVectorForward[0] = VOLTAGE_VECTOR_BC;
    pIpd->adjacentVectorForward[1] = VOLTAGE_VECTOR_CB;
    pIpd->adjacentVectorForward[2] = VOLTAGE_VECTOR_BA;
    pIpd->adjacentVectorForward[3] = VOLTAGE_VECTOR_AB;
    pIpd->adjacentVectorForward[4] = VOLTAGE_VECTOR_CA;
    pIpd->adjacentVectorForward[5] = VOLTAGE_VECTOR_AC;

    pIpd->adjacentVectorReverse[0] = VOLTAGE_VECTOR_AB;
    pIpd->adjacentVectorReverse[1] = VOLTAGE_VECTOR_BA;
    pIpd->adjacentVectorReverse[2] = VOLTAGE_VECTOR_AC;
    pIpd->adjacentVectorReverse[3] = VOLTAGE_VECTOR_CA;
    pIpd->adjacentVectorReverse[4] = VOLTAGE_VECTOR_BC;
    pIpd->adjacentVectorReverse[5] = VOLTAGE_VECTOR_CB;
}

static void ipdVectorAnglesInit(IPD_T *pIpd)
{
    pIpd->vectorAngle[0] = IPD_THIRTY_DEGREES;
    pIpd->vectorAngle[1] = IPD_TWO_TEN_DEGREES;
    pIpd->vectorAngle[2] = IPD_NINETY_DEGREES;
    pIpd->vectorAngle[3] = IPD_TWO_SEVENTY_DEGREES;
    pIpd->vectorAngle[4] = IPD_ONE_FIFTY_DEGREES;
    pIpd->vectorAngle[5] = IPD_THREE_THIRTY_DEGREES;
}

static void ipdClearTimes(IPD_T *pIpd)
{
    memset(pIpd->timeRise,0,VOLTAGE_VECTOR_MAX*sizeof(int32_t));
    memset(pIpd->timeFall,0,VOLTAGE_VECTOR_MAX*sizeof(int32_t));
}

void IPDCurrThreshAction(IPD_T *pIpd)
{
    int32_t loadVal = (int32_t)HAL_IPDGetLoadVal();
    int32_t ipdGetTime = HAL_IPDGetTime();
    ipdRiseTime = (loadVal - ipdGetTime)
                    + (loadVal * (int32_t)pIpd->overflowCnt);

    switch(pIpd->waitState)
    {
        case WAIT_GET_T1:
            ipdRunning = 0;
            HAL_IPDDisableISR();
            HAL_DisablePWM(); /* Sets the PWM output to low, but this is done so that when the timer fault is cleared the output still stays low*/
            pIpd->timeRise[pIpd->vector] += ipdRiseTime;
            pIpd->state = IPD_WAIT_COMPLETE;
            pIpd->waitState = WAIT_COMPLETE;
            HAL_exitIPDMode();
            break;
        case WAIT_COMPLETE:
            /* Disable PWM */
            HAL_DisablePWM();
            break;
        default:
            /* This is intentionally kept empty */
            break;
    }
    pIpd->overflowCnt = 0;
}

void IPDOverFlowAction(IPD_T *pIpd)
{
    if(pIpd->state == IPD_RUN)
    {
        ipdRunning = 0;
        pIpd->overflowCnt++;
        if(pIpd->overflowCnt > pIpd->overflowMax)
        {
            pIpd->faultStatus = IPD_FAULT_CLOCK_TIMEOUT;
            pIpd->state = IPD_FAULT;

            /* Disable PWM */
            HAL_DisablePWM();

            HAL_exitIPDMode();
            HAL_IPDDisableISR();

        }
    }
}
