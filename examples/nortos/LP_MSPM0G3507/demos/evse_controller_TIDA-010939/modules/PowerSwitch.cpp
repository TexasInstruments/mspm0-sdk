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
 * PowerSwitch.cpp
 *
 *  Created on: 26.02.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 */

#include "PowerSwitch.h"

PowerSwitch::PowerSwitch(
    GPTIMER_Regs *_pwmTimer, Gpio &_relayStatus, Gpio &_relayWeldDetect)
    : pwmTimer(_pwmTimer),
      relayStatus(_relayStatus),
      relayWeldDetect(_relayWeldDetect)
{
}

PowerSwitch::~PowerSwitch()
{
    switchOff();
}

void PowerSwitch::init()
{
    //DL_Timer_startCounter(pwmTimer);
    switchOff();
}

bool PowerSwitch::isOn()
{
    return relaysOn;
}

bool PowerSwitch::switchOn()
{
    if (relaysHealthy) {
        DL_Timer_startCounter(pwmTimer);
        setPWM1(100);
        setPWM2(100);
#ifdef FINE_GRAIN_DEBUG_PRINTF
        printf("switchOn 1: On");
#endif
        //delay_ms(relaysDelay);
        delay_cycles(relaysDelayCycles);
        relaysOn = true;
        setPWM1(relaysHoldingPercent);
        setPWM2(relaysHoldingPercent);
#ifdef FINE_GRAIN_DEBUG_PRINTF
        printf("switchOn 2: PWM mode");
#endif
        if (relayStatus.read())
            relaysHealthy = true;
        else
            relaysHealthy = false;
    }
    return relaysHealthy;
}

bool PowerSwitch::switchOff()
{
    //setPWM1(0);
    //setPWM2(0);
    DL_Timer_stopCounter(pwmTimer);
#ifdef FINE_GRAIN_DEBUG_PRINTF
    printf("switchOn 1: Off");
#endif
    //delay_ms(relaysDelay);
    delay_cycles(relaysDelayCycles);
#ifdef FINE_GRAIN_DEBUG_PRINTF
    printf("switchOn 2: Off after delay");
#endif
    relaysOn = false;
    if (!relayStatus.read())
        relaysHealthy = true;
    else
        relaysHealthy = false;
    return relaysHealthy;
}

void PowerSwitch::setPWM1(float dc)
{
    uint16_t counterTicks = dc * 100;
    DL_Timer_setCaptureCompareValue(
        pwmTimer, counterTicks, DL_TIMER_CC_0_INDEX);
}

void PowerSwitch::setPWM2(float dc)
{
    uint16_t counterTicks = dc * 100;
    DL_Timer_setCaptureCompareValue(
        pwmTimer, counterTicks, DL_TIMER_CC_1_INDEX);
}

bool PowerSwitch::executeSelfTest()
{
    bool success = true;
    printf("   relays self test...\n");
    if (switchOn()) {
        printf("OK PowerSwitch: on\n");
    } else {
        printf("FAIL PowerSwitch: on\n");
        success = false;
    }
    // delay_ms(100);
    delay_cycles(8000000);

    if (switchOff()) {
        printf("OK PowerSwitch: off\n");
    } else {
        printf("FAIL PowerSwitch: off\n");
        success = false;
    }
    return success;
}

void PowerSwitch::emergencySwitchOff()
{
    setPWM1(0);
    setPWM2(0);
    relaysHealthy = false;
    relaysOn      = false;
}

void PowerSwitch::resetEmergencySwitchOff()
{
    // NOTE In the following countries automatic reclosing of protection means
    // is not allowed: DK, UK, FR, CH.
    relaysHealthy = true;
    setPWM1(0);
    setPWM2(0);
    relaysOn = false;
}
