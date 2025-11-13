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
 *  Created on: 12.03.2024
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#include "RemoteControlRX.h"
#include "FirmwareUpdater.h"
//#include "version_autogen.h"

//extern int reset_cause_int;

RemoteControlRX::RemoteControlRX(
    ControlPilot &_control_pilot, RemoteControlTX &_tx, MgmtLink &_link)
    : control_pilot(_control_pilot), tx(_tx), link(_link)
{
}

RemoteControlRX::~RemoteControlRX()
{
}

void RemoteControlRX::sendResetReason()
{
    // Announce that we are done resetting
    // if (reset_cause_int == 2) {
    // 	tx.send_reset_reason(ResetReason::ResetReason_WATCHDOG);
    // } else {
    // 	tx.send_reset_reason(ResetReason::ResetReason_USER);
    // }

    tx.send_reset_reason(ResetReason::ResetReason_USER);
}

void RemoteControlRX::processPacket()
{
    if (link.read(&msg_in)) {
        switch (msg_in.which_payload) {
            case EverestToMcu_firmware_update_tag:
                if (msg_in.payload.firmware_update.invoke_rom_bootloader) {
                    restartInBootLoaderMode();
                }
                break;

            case EverestToMcu_keep_alive_tag:
                resetTimer();
                break;

            case EverestToMcu_connector_lock_tag:
                control_pilot.connector_lock(msg_in.payload.connector_lock);
                break;

            case EverestToMcu_pwm_duty_cycle_tag:
                if (msg_in.payload.pwm_duty_cycle == 0) {
                    control_pilot.pwm_F();
                } else if (msg_in.payload.pwm_duty_cycle >= 10000) {
                    control_pilot.pwm_off();
                } else {
                    control_pilot.pwm_on(
                        msg_in.payload.pwm_duty_cycle / 10000.);
                }
                break;

            case EverestToMcu_allow_power_on_tag:
                control_pilot.allow_power_on(msg_in.payload.allow_power_on);
                break;

            case EverestToMcu_reset_tag:
#ifdef REMOTECONTROL_CPP_ENABLE_PRINTF
                printf("Received reset\n");
#endif
                reset_flags = 0;
                NVIC_SystemReset();
                break;
        }
    }
}

void RemoteControlRX::startTimer(uint32_t ticks)
{
    timer_countdown = ticks;
    timer_tick      = DL_SYSTICK_getValue();
}

void RemoteControlRX::resetTimer()
{
    DL_SYSTICK_resetValue();
    timer_tick = DL_SYSTICK_getValue();
}

bool RemoteControlRX::timerElapsed()
{
    if (timer_tick != 0 &&
        DL_SYSTICK_getValue() - timer_tick > timer_countdown)
        return true;
    else
        return false;
}
