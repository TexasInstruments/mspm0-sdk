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
 * RemoteControl.cpp
 *
 *  Created on: 31.03.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#include "RemoteControlTX.h"
#include "FirmwareUpdater.h"
//#include "version_autogen.h"

extern int reset_cause_int;

RemoteControlTX::RemoteControlTX(MgmtLink &_link) : link(_link)
{
}

void RemoteControlTX::send_reset_reason(ResetReason r)
{
    //printf ("------------------------------- RESET TAG SENT ------------------");
    McuToEverest msg_out  = McuToEverest_init_default;
    msg_out.which_payload = McuToEverest_reset_tag;
    msg_out.payload.reset = r;
    link.write(&msg_out);
}

void RemoteControlTX::send_cp_state(CpState cp_state)
{
    McuToEverest msg_out     = McuToEverest_init_default;
    msg_out.which_payload    = McuToEverest_cp_state_tag;
    msg_out.payload.cp_state = cp_state;
    link.write(&msg_out);
}

void RemoteControlTX::send_relays_state(bool on)
{
    McuToEverest msg_out         = McuToEverest_init_default;
    msg_out.which_payload        = McuToEverest_relays_state_tag;
    msg_out.payload.relays_state = on;
    link.write(&msg_out);
}

void RemoteControlTX::send_error_flags(ErrorFlags e)
{
    McuToEverest msg_out        = McuToEverest_init_default;
    msg_out.which_payload       = McuToEverest_error_flags_tag;
    msg_out.payload.error_flags = e;
    link.write(&msg_out);
}

void RemoteControlTX::send_telemetry(Telemetry t)
{
    McuToEverest msg_out      = McuToEverest_init_default;
    msg_out.which_payload     = McuToEverest_telemetry_tag;
    msg_out.payload.telemetry = t;
    link.write(&msg_out);
}

void RemoteControlTX::send_pp_state(PpState pp_state)
{
    McuToEverest msg_out     = McuToEverest_init_default;
    msg_out.which_payload    = McuToEverest_pp_state_tag;
    msg_out.payload.pp_state = pp_state;
    link.write(&msg_out);
}

void RemoteControlTX::send_lock_state(LockState lock_state)
{
    McuToEverest msg_out       = McuToEverest_init_default;
    msg_out.which_payload      = McuToEverest_lock_state_tag;
    msg_out.payload.lock_state = lock_state;
    link.write(&msg_out);
}

void RemoteControlTX::send_keep_alive()
{
    McuToEverest msg_out  = McuToEverest_init_default;
    msg_out.which_payload = McuToEverest_keep_alive_tag;
    //msg_out.payload.keep_alive.time_stamp = DL_SYSTICK_getValue();
    msg_out.payload.keep_alive.hw_type                                  = 0;
    msg_out.payload.keep_alive.hw_revision                              = 0;
    msg_out.payload.keep_alive.protocol_version_major                   = 0;
    msg_out.payload.keep_alive.protocol_version_minor                   = 1;
    msg_out.payload.keep_alive.hwcap_min_current                        = 6;
    msg_out.payload.keep_alive.hwcap_max_current                        = 32;
    msg_out.payload.keep_alive.hwcap_min_phase_count                    = 1;
    msg_out.payload.keep_alive.hwcap_max_phase_count                    = 3;
    msg_out.payload.keep_alive.supports_changing_phases_during_charging = true;

    //strncpy(msg_out.payload.keep_alive.sw_version_string, VERSION_STRING, 50);
    msg_out.payload.keep_alive.sw_version_string[50] = 0;
    link.write(&msg_out);
    //printf ("Keep alive sent %i.\n", (int)msg_out.payload.keep_alive.time_stamp);
}
