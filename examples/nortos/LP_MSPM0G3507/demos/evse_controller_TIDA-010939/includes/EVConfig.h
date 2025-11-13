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
 * EVConfig.h
 *
 *  Created on: Mar 11, 2021
 *      Author: cornelius
 *		Modified : Texas Instruments Incorporated, 2025
 */

#ifndef SRC_EVDRIVERS_EVCONFIG_H_
#define SRC_EVDRIVERS_EVCONFIG_H_

#define YETI_RELEASE 1

// define to compile for self test mode instead of normal working mode.
// Do NOT connect car or AC voltage in self test mode!
// #define SELFTEST_MODE
// #define EMITEST_relays_PWM

// define this to enable workarounds for HW bugs in R0 devboard
// #define EVDEVBOARD_R0_WORKAROUNDS

// NOTE: In the following countries automatic reclosing of protection means is
// not allowed: DK, UK, FR, CH uncomment for these countries
// #define DISABLE_RCD_RECLOSING

// NOTE: This should be disabled in production environment and replaced with a
// secure boot loader
#define ALLOW_ROM_BOOTLOADER

// Ignore TESLA's CP sequence B->C->DF->B 5 times.
// That helps is SLAC is too slow to respond to CM_SLAC_PARM.REQ on the first
// try. Note that this may cause other problems as events in the first 16
// seconds are delayed by 1.5 seconds
//#define IGNORE_TESLA_SPECIAL_SEQUENCE

/*
 * Enable debug printf's in the threads:
 * Be careful with enableing printf's in multiple threads. This can hard fault
 * probably due to still buggy malloc implementation that is called in printf.
 * Disable all for release!
 * */

#define REMOTECONTROL_CPP_ENABLE_PRINTF
// #define ADE7978_CPP_ENABLE_PRINTF
// #define CHARGER_CPP_ENABLE_PRINTF
// #define MGMTLINK_CPP_ENABLE_PRINTF
// #define RCD_CPP_ENABLE_PRINTF
// #define SPIBUS_CPP_ENABLE_PRINTF
// #define FINE_GRAIN_DEBUG_PRINTF

#endif  // SRC_EVDRIVERS_EVCONFIG_H_
