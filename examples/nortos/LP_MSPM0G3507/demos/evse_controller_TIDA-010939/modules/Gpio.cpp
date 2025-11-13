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
 * Gpio.cpp
 *
 *  Created on: 26.02.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 */

#include "Gpio.h"
#include <stdio.h>

Gpio::Gpio(GPIO_Regs *_port, uint32_t _pin)
{
    port = _port;
    pin  = _pin;
}

void Gpio::set()
{
    DL_GPIO_setPins(port, pin);
}

void Gpio::reset()
{
    DL_GPIO_clearPins(port, pin);
}

bool Gpio::read()
{
    return DL_GPIO_readPins(port, pin);
}

uint32_t Gpio::getPin()
{
    return pin;
}

GPIO_Regs *Gpio::getPort()
{
    return port;
}
