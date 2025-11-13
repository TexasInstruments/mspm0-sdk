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
 * ControlPilot.h
 *
 *  Created on: 12.03.2024
 *  Author: cornelius
 *	Modified: Texas Instruments Incorporated, 2025
 *
 *  IEC 61851-1 compliant Control Pilot
 */

#ifndef SRC_EVDRIVERS_CONTROLPILOT_H_
#define SRC_EVDRIVERS_CONTROLPILOT_H_

#include "ControlPilot_HAL.h"
#include "PowerSwitch.h"
#include "Rcd.h"

#include <queue>
#include "DataTypes.h"
#include "RemoteControlTX.h"

class ControlPilot
{
   public:
    ControlPilot(ControlPilot_HAL &_control_pilot_hal,
        RemoteControlTX &_remote_tx, PowerSwitch &_power_switch, Rcd &_rcd);
    virtual ~ControlPilot();

    void init();
    void pwm_on(float dc);
    void pwm_off();
    void pwm_F();
    void replug(unsigned int t);

    // Allow to power on relays. The ControlPilot class decides when to actually
    // switch on and off. Switiching off is always allowed independent of this
    // setting. The flag will be reset e.g. if car unplugs or if pwmOff()/pwmF()
    // is called
    void allow_power_on(bool p);

    void connector_lock(bool lock);

    void overtemp_check(float *temps);

    void rcd_enable();
    void rcd_disable();

    virtual void main();

   private:
    // References to external objects we use here
    ControlPilot_HAL &control_pilot_hal;
    RemoteControlTX &remote_tx;
    PowerSwitch &power_switch;
    Rcd &rcd;

    void enable();
    void disable();

    void run_state_machine();

    InternalCPState current_state{InternalCPState::Disabled};
    InternalCPState last_state{InternalCPState::Disabled};

    bool current_relays_state{false};
    bool last_relays_state{false};

    bool tesla_last_pwm_running{false};
    uint32_t teslaPwmTimer{0};
    bool teslaPwmTimerStarted{false};

    bool read_from_car(InternalCPState *cp);

    bool power_on();
    bool power_off();
    void check_lock();

    void check_proximity_pilot();
    void check_plug_lock();

    bool proximity_pilot_connected{true};  //Can be changed depending on the hardware setup
    bool proximity_pilot_good{false};
    bool plug_lock_good{false};
    bool plug_lock_set{false};
    bool overtemp{false};

    bool is_voltage_in_range(float voltage, float center);

    void start_timer(uint32_t msecs);
    void stop_timer();
    bool timer_elapsed();
    uint32_t timer_countdown{0};
    uint32_t timer_tick{0};

    // Telemetry packet variables
    float cp_hi{0.};
    float cp_lo{0.};
    float temp0;
    float temp1;

    bool rcd_reclosing_allowed{true};

    bool replugging_in_progress{false};
    bool replugging_in_progress_F{false};
    bool replugging_in_progress_HI{false};
    bool replugging_start{false};
    int replugging_timer_F{0};
    int replugging_timer_HI{0};

    uint32_t lock_sw_off_Tick{0};
    float pwm_duty_cycle{0.};

    bool pwm_running{false};
    bool last_pwm_running{false};
    bool power_on_allowed{false};

    PpState last_pp_state{PpState::PpState_STATE_NC};
    PpState current_pp_state{PpState::PpState_STATE_NC};

    ErrorFlags error_flags;
    ErrorFlags last_error_flags;

    bool last_power_on_allowed{false};
};

#endif  // SRC_EVDRIVERS_CONTROLPILOT_H_
