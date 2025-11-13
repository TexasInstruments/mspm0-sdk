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
 * PowerSwitch.h
 *
 *  Created on: 26.02.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 * Driver for the power relay.
 *
 * switchOn() enables power output to the car.
 *
 * The driver monitors the mirror contacts for both relays and sets
 * relaysHealthy to false if the mirror contacts do not work as expected. The
 * same happens if trying to switch on from on state. There is no way to recover
 * from this error except power cycling the whole system for safety reasons.
 *
 * emergencySwitchOff() switches off and sets relaysHealthy to false with
 * no way to recover. Used to switch off from RCD.
 *
 *
 */

#ifndef SRC_EVDRIVERS_POWERSWITCH_H_
#define SRC_EVDRIVERS_POWERSWITCH_H_

#include <stdio.h>

//#include "cmsis_os.h"

#include "Gpio.h"

class PowerSwitch
{
   public:
    PowerSwitch(
        GPTIMER_Regs *_pwmTimer, Gpio &_relayStatus, Gpio &_relayWeldDetect);
    ~PowerSwitch();

    void init();
    bool switchOn();
    bool switchOff();
    bool isOn();

    bool executeSelfTest();
    void emergencySwitchOff();
    void resetEmergencySwitchOff();
    bool relaysHealthy;

   private:
    bool relaysOn;
    float relaysHoldingPercent = 0.35;

    GPTIMER_Regs *pwmTimer;

    void setPWM1(float dc);
    void setPWM2(float dc);

    Gpio &relayStatus;
    Gpio &relayWeldDetect;

    // time relays takes to switch on/off in cycles (80MHz clock)
    const uint32_t relaysDelayCycles = 8000000;
};

#endif  // SRC_EVDRIVERS_POWERSWITCH_H_
