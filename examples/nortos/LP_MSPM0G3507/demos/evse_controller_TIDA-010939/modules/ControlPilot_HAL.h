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
 *  Created on: 25.10.2021
 *  Author: cornelius
 *	Modified: Texas Instruments Incorporated, 2025
 *
 * ControlPilot_HAL.h
 * IEC 61851-1 compliant Control Pilot state machine
 *
 * This class provides abstraction for CP and PP signals:
 * 1) timer interrupt triggered ADC readings of PWM signal on CP
 * 2) PWM output on CP
 * 3) Lock motor control
 */

#ifndef SRC_EVDRIVERS_CONTROLPILOT_HAL_H_
#define SRC_EVDRIVERS_CONTROLPILOT_HAL_H_

#include <ti/iqmath/include/IQmathLib.h>
#include "Adc.h"
#include "Gpio.h"
#include "PlugLock.h"
#include "includes/EVConfig.h"

class ControlPilot_HAL
{
   public:
    ControlPilot_HAL(GPTIMER_Regs *_pwmTimer, Adc &_adc, PlugLock &_plugLock,
        Gpio *_cpEnable);
    virtual ~ControlPilot_HAL();

    void init();
    bool readCPSignal();
    float getCPHi();
    float getCPLo();

    void lockMotorLock();
    void lockMotorUnlock();
    void lockMotorOff();
    bool plugLocked();

    uint32_t getEvsePP();
    uint32_t getEvseDP1();
    uint32_t getPlugLockFB();

    void setPWM(float dc);
    void disableCP();
    void enableCP();
    void startCounter();
    void stopCounter();

   private:
    GPTIMER_Regs *pwmTimer;
    Adc &adc;
    PlugLock &plugLock;
    Gpio *cpEnable;

    float cpLo, cpHi;
};

#endif  // SRC_EVDRIVERS_CONTROLPILOT_HAL_H_
