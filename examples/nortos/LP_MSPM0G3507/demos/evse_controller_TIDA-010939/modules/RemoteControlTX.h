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
 * RemoteControl.h
 *
 *  Created on: 31.03.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 */

#ifndef SRC_EVDRIVERS_REMOTECONTROLTX_H_
#define SRC_EVDRIVERS_REMOTECONTROLTX_H_

#include "MgmtLink.h"

class RemoteControlTX
{
   public:
    RemoteControlTX(MgmtLink &_link);

    void send_reset_reason(ResetReason r);
    void send_cp_state(CpState s);
    void send_relays_state(bool on);
    void send_error_flags(ErrorFlags f);
    void send_telemetry(Telemetry t);
    void send_pp_state(PpState p);
    void send_lock_state(LockState l);
    void send_keep_alive();

    void test_send();

   private:
    MgmtLink &link;

    uint32_t *txTimerArg;
    uint8_t txCnt;
};

#endif  // SRC_EVDRIVERS_REMOTECONTROLTX_H_
