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
 * RCD.h
 *
 *  Created on: 01.03.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 * RCD driver
 *
 * If RCD triggers due to too high residual current isrRCDFired is set to true
 * and relays are switched off directly in interrupt handler.
 *
 * isrRCDEnabled is a flag that can be modified also by high level to enable or
 * disable RCD usage (config option).
 *
 * isrRCDActive is an yeti-internal flag used to (de-)activate the RCD in sync
 * with relays switching to prevent erroneous firing directly during switch
 * on/off.
 *
 * Requires one timer with input capture pin and 2 CC units for PWM decoding.
 * CC CH1 must trigger on rising edge (direct capture of input pin)
 * and CC CH2 on falling edge (indirect capture of input pin)
 * The timer must be configured before running this driver (incl. enable of
 * interrupt)
 *
 * executeSelfTest() runs an actual HW self test with induced residual current.
 * While running the test emergency switch off is disabled. This test should be
 * run before every charging cycle to ensure the RCD is still functional. It
 * could also be run during charging at regular intervals, but be aware that RCD
 * is inactive for a maximum of 10 seconds.
 *
 */

#ifndef SRC_EVDRIVERS_RCD_H_
#define SRC_EVDRIVERS_RCD_H_

#include "Gpio.h"
#include "PowerSwitch.h"
#include "includes/EVConfig.h"

// Global variables for error GPIOs (as they are needed in interrupt handler!)s
extern uint16_t rcd_dc_errorin_gpio_pin;
extern GPIO_Regs *rcd_dc_errorin_gpio_port;
extern uint16_t rcd_ac_errorin_gpio_pin;
extern GPIO_Regs *rcd_ac_errorin_gpio_port;

class Rcd
{
   public:
    Rcd(Gpio &_testout, Gpio &_errorin, PowerSwitch *_powerSwitch);
    ~Rcd();
    bool executeSelfTest();

    void enable();
    void disable();
    void activate();
    void deactivate();
    bool getEnabled();
    // void setUnused(bool u);
    bool getRcdFired();
    void reset();

    void HAL_GPIO_EXTI_Callback(uint32_t GPIO_Pin);

   private:
    Gpio &testout;
    Gpio &errorin;
    PowerSwitch *powerSwitch;

    // Interrupt access only variables
    uint16_t isrLastRising, isrRising, isrFalling = 0;
    bool isrRcdTestModeError;
    bool isrRcdEnabled;
    bool isrRcdActive;
    bool isrRcdFired;
    // bool isrRcdUnused;
};

#endif  // SRC_EVDRIVERS_RCD_H_
