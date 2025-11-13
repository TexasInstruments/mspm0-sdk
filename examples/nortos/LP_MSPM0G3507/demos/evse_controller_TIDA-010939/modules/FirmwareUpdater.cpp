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
 * FirmwareUpdater.cpp
 *
 *  Created on: 20.08.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 */

#include "FirmwareUpdater.h"

// only works if the no_init section is correctly defined in .ld file
volatile int reset_flags __attribute__((section(".no_init")));

void restartInBootLoaderMode()
{
#ifdef ALLOW_ROM_BOOTLOADER
    reset_flags = 0x12ABCDEF;
    NVIC_SystemReset();
#endif
}

void restartInBootLoaderMode_Step2()
{
#ifdef ALLOW_ROM_BOOTLOADER
#if YETI_RELEASE == 1
    reset_flags = 0;
    void (*SysMemBootJump)(void);
    volatile uint32_t addr = 0x1FFF0000;  //bootloader addr?

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;
  
    SysMemBootJump = (void (*)(void))(*((uint32_t *) (addr + 4)));
    __set_MSP(*(uint32_t *) addr);
    SysMemBootJump();
#endif
#endif
}
