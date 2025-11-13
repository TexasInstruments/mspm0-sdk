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
 * FirmwareUpdater.h
 *
 *  Created on: 20.08.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 */

#ifndef EVDRIVERS_FIRMWAREUPDATER_H_
#define EVDRIVERS_FIRMWAREUPDATER_H_

#include "includes/EVConfig.h"
//#include "cmsis_os.h"
#include "ti_msp_dl_config.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile int reset_flags;

void restartInBootLoaderMode();
void restartInBootLoaderMode_Step2();

#ifdef __cplusplus
}
#endif

#endif /* EVDRIVERS_FIRMWAREUPDATER_H_ */
