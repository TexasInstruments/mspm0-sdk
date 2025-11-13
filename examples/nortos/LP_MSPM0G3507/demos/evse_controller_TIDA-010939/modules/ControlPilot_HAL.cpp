/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * ControlPilot_HAL.cpp
 *
 *  Created on: 15.10.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#include "ControlPilot_HAL.h"

#include <math.h>
#include <string.h>

#include "includes/EVConfig.h"

ControlPilot_HAL::ControlPilot_HAL(
    GPTIMER_Regs *_pwmTimer, Adc &_adc, PlugLock &_plugLock, Gpio *_cpEnable)
    : pwmTimer(_pwmTimer), adc(_adc), plugLock(_plugLock), cpEnable(_cpEnable)
{
}

ControlPilot_HAL::~ControlPilot_HAL()
{
    DL_Timer_stopCounter(pwmTimer);
}

void ControlPilot_HAL::init()
{
    setPWM(1);
    DL_Timer_startCounter(pwmTimer);
    disableCP();
}

void ControlPilot_HAL::setPWM(float dc)
{
    uint16_t counterTicks = dc * 40000;
    DL_Timer_setCaptureCompareValue(
        pwmTimer, counterTicks, DL_TIMER_CC_0_INDEX);
}

// reads ADC values for lo and hi part of PWM
// returns false if signal is unstable
bool ControlPilot_HAL::readCPSignal()
{
    // note we do not sqrt() the std dev here
    const _iq13 qMaxStdDev = _IQ13(0.01);

    _iq13 cpLoArr[Adc::AVG];
    _iq13 cpHiArr[Adc::AVG];

    adc.getEvseCPLo(cpLoArr);
    adc.getEvseCPHi(cpHiArr);

    // do some statistics to see if signal is stable
    _iq13 avgLo = 0;
    _iq13 avgHi = 0;
    for (int i = 0; i < Adc::AVG; i++) {
        avgLo += cpLoArr[i];
        avgHi += cpHiArr[i];
    }
    avgLo = _IQ13div(avgLo, _IQ13(Adc::AVG));
    avgHi = _IQ13div(avgHi, _IQ13(Adc::AVG));

    _iq13 stdDevLo = 0;
    _iq13 stdDevHi = 0;
    for (int i = 0; i < Adc::AVG; i++) {
        stdDevLo += _IQmpy((cpLoArr[i] - avgLo), (cpLoArr[i] - avgLo));
        stdDevHi += _IQmpy((cpHiArr[i] - avgHi), (cpHiArr[i] - avgHi));
    }
    stdDevLo = _IQ13div(stdDevLo, _IQ13(Adc::AVG));
    stdDevHi = _IQ13div(stdDevHi, _IQ13(Adc::AVG));

    if (stdDevHi > qMaxStdDev || stdDevLo > qMaxStdDev) return false;

    cpLo = _IQ13toF(avgLo);
    cpHi = _IQ13toF(avgHi);

    return true;
}

void ControlPilot_HAL::enableCP()
{
    /*if (cpEnable != nullptr)*/
}

void ControlPilot_HAL::disableCP()
{
    /*if (cpEnable != nullptr)*/
}

void ControlPilot_HAL::startCounter()
{
    DL_Timer_startCounter(pwmTimer);
}

void ControlPilot_HAL::stopCounter()
{
    DL_Timer_stopCounter(pwmTimer);
}

void ControlPilot_HAL::lockMotorUnlock()
{
    plugLock.unlock();
}

void ControlPilot_HAL::lockMotorLock()
{
    plugLock.lock();
}

void ControlPilot_HAL::lockMotorOff()
{
    plugLock.motorOff();
}

bool ControlPilot_HAL::plugLocked()
{
    return plugLock.isLocked();
}

float ControlPilot_HAL::getCPHi()
{
    return cpHi;
}

float ControlPilot_HAL::getCPLo()
{
    return cpLo;
}

uint32_t ControlPilot_HAL::getEvsePP()
{
    return adc.getEvsePP();
}

uint32_t ControlPilot_HAL::getEvseDP1()
{
    return adc.getEvseDP1();
}

uint32_t ControlPilot_HAL::getPlugLockFB()
{
    return adc.getPlugLockFB();
}
