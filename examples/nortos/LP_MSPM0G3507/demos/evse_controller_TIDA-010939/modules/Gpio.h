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
 * Gpio.h
 *
 *  Created on: 26.02.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#ifndef SRC_EVDRIVERS_GPIO_H_
#define SRC_EVDRIVERS_GPIO_H_

#include "ti_msp_dl_config.h"

class Gpio
{
   public:
    Gpio(GPIO_Regs *_port, uint32_t _pin);
    void set();
    void reset();
    bool read();

    uint32_t getPin();
    GPIO_Regs *getPort();

   private:
    GPIO_Regs *port;
    uint32_t pin;
};

#endif  // SRC_EVDRIVERS_GPIO_H_
