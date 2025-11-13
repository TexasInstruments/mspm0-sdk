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
 * Rcd.cpp
 *
 *  Created on: 01.03.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 */

#include "Rcd.h"

#include <stdio.h>

Rcd::Rcd(Gpio &_testout, Gpio &_errorin, PowerSwitch *_powerSwitch)
    : testout(_testout), errorin(_errorin), powerSwitch(_powerSwitch)
{
    testout.set();  // Note logic is inverted. HIGH means no self test.

    // residualCurrentPercentPerMa = 2; // -X921 UL version
    // residualCurrentPercentPerMa=3.33; // -X920 IEC version

    // 96MHz timer clock base, 8KHz PWM expected from sensor
    // FIXME: this clock base could be retreived via HAL_RCC_Get*Freq
    // expectedPwmPeriod = 96000 / 8;

    // isrPwmUpdateTimeout = 0;
    // isrPwmUpdated = false;
    isrRcdTestModeError = false;
    isrRcdEnabled       = false;
    isrRcdFired         = false;
    isrRcdActive        = false;

    // HAL_TIM_Base_Start(pwmTimer);
    // HAL_TIM_IC_Start_IT(pwmTimer, TIM_CHANNEL_1);
    // HAL_TIM_IC_Start_IT(pwmTimer, TIM_CHANNEL_2);
}

Rcd::~Rcd()
{
}

bool Rcd::executeSelfTest()
{
    // bool pass = false;

    // testout.set();
    // delay_cycles(8000000);

    // if (errorin.read())
    //     pass = false;
    // else
    //     pass = true;

    // testout.reset();
    // delay_cycles(8000000);

    // reset();
    // return pass;
    return false;
}

void Rcd::HAL_GPIO_EXTI_Callback(uint32_t GPIO_Pin)
{
    if (GPIO_Pin == errorin.getPin()) {
        isrRcdTestModeError = true;
        if (isrRcdEnabled && isrRcdActive) {
            // emergency switch off relays
            if (powerSwitch) powerSwitch->emergencySwitchOff();

            isrRcdFired  = true;
            isrRcdActive = false;

            reset();
        }
#ifdef RCD_CPP_ENABLE_PRINTF
        else
            printf("RCD Error interrupt ignored!\n");
#endif
    }
}

void Rcd::enable()
{
    isrRcdEnabled = true;
}
void Rcd::disable()
{
    isrRcdEnabled = false;
}

void Rcd::activate()
{
    isrRcdActive        = true;
    isrRcdTestModeError = false;
    isrRcdFired         = false;
}
void Rcd::deactivate()
{
    isrRcdActive        = false;
    isrRcdTestModeError = false;
    isrRcdFired         = false;
}

/* Toggles reset pin with delay of 100ms */
void Rcd::reset()
{
    isrRcdFired = false;
    testout.set();
    delay_cycles(8000000);
    testout.reset();
}

/*
 void Rcd::setUnused(bool u) {
 //taskENTER_CRITICAL();
 isrRcdUnused = u;
 //taskEXIT_CRITICAL();
 }*/

bool Rcd::getEnabled()
{
    return isrRcdEnabled;
}

bool Rcd::getRcdFired()
{
    return isrRcdFired;
}
