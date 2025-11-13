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
 * Adc.h
 *
 *  Created on: Mar 11, 2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#ifndef SRC_EVDRIVERS_ADC_H_
#define SRC_EVDRIVERS_ADC_H_

#include <ti/iqmath/include/IQmathLib.h>
#include <cstdio>
#include "includes/EVConfig.h"
#include "ti_msp_dl_config.h"

class Adc
{
   public:
    Adc(ADC12_Regs *_adc);

    void getEvseCPHi(_iq13 *out);
    void getEvseCPLo(_iq13 *out);
    uint16_t getEvsePP();
    uint16_t getEvseDP1();
    uint16_t getPlugLockFB();

    void triggerEvseCPHiISR();
    void triggerEvseCPLoISR();

    void HAL_ADC_ConvCpltCallback(ADC12_Regs *hadc);

    static constexpr uint8_t AVG = 50;

   private:
    ADC12_Regs *adc;

    uint32_t evseCPHi[AVG], evseCPLo[AVG];
    volatile uint8_t evseCPHiIdx, evseCPLoIdx;
    volatile uint16_t evsePP, evseDP1, plugLockFB;
    volatile uint8_t evseCPSampleTarget;
    volatile uint16_t evseCPuntriggered;
};

#endif  // SRC_EVDRIVERS_ADC_H_
