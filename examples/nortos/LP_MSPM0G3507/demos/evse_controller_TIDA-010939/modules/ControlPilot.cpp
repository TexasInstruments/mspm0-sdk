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
 *
 * ControlPilot.cpp
 *
 *  Created on: 15.10.2021
 *      Author: cornelius
 *		Modified: Texas Instruments Incorporated, 2025
 *
 */

#include "ControlPilot.h"

#include <math.h>
#include <string.h>

#include "includes/EVConfig.h"

static bool compare_error_flags(const ErrorFlags &a, const ErrorFlags &b)
{
    return a.diode_fault == b.diode_fault and
           a.rcd_selftest_failed == b.rcd_selftest_failed and
           a.rcd_triggered == b.rcd_triggered and
           a.ventilation_not_available == b.ventilation_not_available and
           a.connector_lock_failed == b.connector_lock_failed and
           a.cp_signal_fault == b.cp_signal_fault and
           a.over_current == b.over_current;
}

ControlPilot::ControlPilot(ControlPilot_HAL &_control_pilot_hal,
    RemoteControlTX &_remote_tx, PowerSwitch &_power_switch, Rcd &_rcd)
    : control_pilot_hal(_control_pilot_hal),
      remote_tx(_remote_tx),
      power_switch(_power_switch),
      rcd(_rcd)
{
}

ControlPilot::~ControlPilot()
{
    pwm_F();
}

void ControlPilot::init()
{
    enable();
    //pwm_F();

    rcd.enable();  // Default is RCD is enabled
}

void ControlPilot::main()
{
    // Enable CP output
    enable();

    int cnt = 0;

    while (true) {
        //delay_ms(50);
        delay_cycles(4000000);

        /* Update EVerest every 4th iteration*/
        if (cnt++ % 4 == 0) {
            Telemetry t;
            t.cp_voltage_hi = cp_hi;
            t.cp_voltage_lo = cp_lo;
            t.temp0         = temp0;
            t.temp1         = temp1;
            remote_tx.send_telemetry(t);

            remote_tx.send_keep_alive();
        }

        // Run low level state machine update
        run_state_machine();

        // Send updates to EVerest if CP state changed
        if (last_state not_eq current_state) {
            last_state = current_state;
            remote_tx.send_cp_state(to_CpState(current_state));
        }

        // Send updates to EVerest if PP state changed
        if (last_pp_state not_eq current_pp_state) {
            last_pp_state = current_pp_state;
            remote_tx.send_pp_state(current_pp_state);
        }

        current_relays_state = power_switch.isOn();
        // Send updates to EVerest if relays state changed
        if (last_relays_state not_eq current_relays_state) {
            last_relays_state = current_relays_state;
            remote_tx.send_relays_state(current_relays_state);
        }

        // Send updates to EVerest if error flags changed
        if (not compare_error_flags(last_error_flags, error_flags)) {
            last_error_flags = error_flags;
            remote_tx.send_error_flags(error_flags);
        }
    }
}

void ControlPilot::run_state_machine()
{
    // check if RCD fired in the meantime (actual emergency switch off happens
    // in interrupt)
    if (rcd.getRcdFired()) {
        rcd.reset();  // Note this does NOT reset the fault flag in the
            // powerSwitch, just the flag that we do not keep on
            // sending Error_RCD events!
        error_flags.rcd_triggered = true;
    }

    // check if we need to change lock state
    check_lock();

    // Check to make sure PP and PL feedback is present
    // If not, we prevent/disable charging until present
    check_proximity_pilot();
    check_plug_lock();

    if (not power_on_allowed and last_power_on_allowed) {
        power_off();
    }

    last_power_on_allowed = power_on_allowed;

    // update currentState from Car reading if signal is stable
    if (read_from_car(&current_state)) {
        if (replugging_start) {
            replugging_start          = false;
            replugging_in_progress    = true;
            replugging_in_progress_F  = true;
            replugging_in_progress_HI = false;
            pwm_F();
        }
        if (replugging_in_progress) {
            if (replugging_in_progress_F && (replugging_timer_F -= 50) <= 0) {
                disable();  // After F we disable (high impedance) for a while
                replugging_in_progress_F  = false;
                replugging_in_progress_HI = true;
            }
            if (replugging_in_progress_HI &&
                (replugging_timer_HI -= 50) <= 0) {
                enable();
                replugging_in_progress_HI = false;
                replugging_in_progress    = false;
            }
        }

        if (current_state not_eq InternalCPState::C and
            current_state not_eq InternalCPState::D) {
            stop_timer();
        }

        switch (current_state) {
            case InternalCPState::Disabled:
                // simply wait until someone enables us...
                power_on_allowed        = false;
                error_flags.diode_fault = false;
                power_off();
                break;

            case InternalCPState::A:
                error_flags.diode_fault = false;
                //power_on_allowed = false;
                error_flags.rcd_triggered = false;
                pwm_off();
                power_off();
                power_switch.resetEmergencySwitchOff();
                rcd.reset();
                break;

            case InternalCPState::B:
                error_flags.diode_fault = false;
                power_off();
                break;

            case InternalCPState::D:
            case InternalCPState::C:

                error_flags.diode_fault = false;

                if (not pwm_running) {  // C1
                    // Table A.6 Sequence 10.2: EV does not stop drawing power
                    // even if PWM stops. Stop within 7 seconds (E.g. Kona1!)
                    // EVerest should stop after 6 seconds already

                    if (last_pwm_running) {
                        start_timer(7000);
                    }

                    if (timer_elapsed()) {
                        // force power off under load
                        power_off();
                    }

                } else {  // C2
                    if (power_on_allowed) {
                        // Table A.6: Sequence 4 EV ready to charge.
                        // Must enable power within 3 seconds.
                        power_on();
                    }
                }
                break;

            case InternalCPState::E:
            case InternalCPState::F:
                pwm_off();

                error_flags.diode_fault = false;
                power_off();
                break;

            case InternalCPState::DF:
                error_flags.diode_fault = true;
                power_off();
                break;
        }

        last_pwm_running = pwm_running;
    }
}

void ControlPilot::pwm_off()
{
    control_pilot_hal.setPWM(1);
    pwm_duty_cycle = 1.;
    pwm_running    = false;
}

void ControlPilot::pwm_on(float dc)
{
    control_pilot_hal.setPWM(dc);
    pwm_duty_cycle = dc;
    pwm_running    = true;
}

void ControlPilot::replug(unsigned int t)
{
    replugging_timer_F  = t;
    replugging_timer_HI = t;
    replugging_start    = true;
}

/*
 * Note that F can be exited by pwmOff or pwmOn only.
 * */
void ControlPilot::pwm_F()
{
    // EVSE error - constant -12V signal on CP
    control_pilot_hal.setPWM(0);
    pwm_duty_cycle = 0.;
    current_state  = InternalCPState::F;
    pwm_running    = false;
    //power_on_allowed = false;
}

void ControlPilot::enable()
{
    current_state = InternalCPState::A;
    pwm_off();
    control_pilot_hal.enableCP();
}

void ControlPilot::disable()
{
    current_state = InternalCPState::Disabled;
    pwm_off();
    control_pilot_hal.disableCP();
}

void ControlPilot::check_proximity_pilot()
{
    // If PP is connected can be set in the ControlPilot.h
    if (proximity_pilot_connected == false) {
        current_pp_state = PpState::PpState_STATE_NC;
        return;
    }

    uint16_t pp = control_pilot_hal.getEvsePP();

    if (pp >= 2295 && pp <= 2592) {
        current_pp_state = PpState::PpState_STATE_13A;
        return;
    } else if (pp >= 1500 && pp <= 1797) {
        current_pp_state = PpState::PpState_STATE_20A;
        return;
    } else if (pp >= 645 && pp <= 827) {
        current_pp_state = PpState::PpState_STATE_32A;
        return;
    } else if (pp >= 321 && pp <= 422) {
        current_pp_state = PpState::PpState_STATE_70A;
        return;
    } else {
        current_pp_state = PpState::PpState_STATE_FAULT;
    }
}

// Translate ADC readings for lo and hi part of PWM to IEC61851 states.
// returns false if signal is unstable/invalid and cp argument was not
// updated.
bool ControlPilot::read_from_car(InternalCPState *cp)
{
    bool cp_signal_valid = false;

    if (control_pilot_hal.readCPSignal()) {
        cp_lo = control_pilot_hal.getCPLo();
        cp_hi = control_pilot_hal.getCPHi();
        if (*cp == InternalCPState::Disabled)
            return true;  // stay in Disabled independent of measurement
        else
            cp_signal_valid = true;
    }

    if (cp_signal_valid) {
        // CP-PE short or signal somehow gone. According to the norm both high and low voltage should be 0 (full short before diode).
        // Some test equipment implement that wrong however, shorting CP after the diode resulting in an undefined state (it would be E2).
        // This means we only check the hi part here.
        //if (isVoltageInRange(cpLo, 0.) && isVoltageInRange(cpHi, 0.))
        if (is_voltage_in_range(cp_hi, 0.)) {
            *cp = InternalCPState::E;
            return true;
        }
        // sth is wrong with negative signal
        if (pwm_running && !is_voltage_in_range(cp_lo, -12.)) {
            if (is_voltage_in_range(cp_hi + cp_lo, 0.)) {
                // Diode fault
                *cp = InternalCPState::DF;
            } else {
                return false;
            }
        } /*else if (!proximity_pilot_good || (!plug_lock_good && plug_lock_set) || overtemp) {
			// Soft fail conditions that hold EVSE in State A:
			// proximity pilot or plug lock disconnected
			// overtemperature condition (>90C)
			*cp = InternalCPState::A;
		}*/
        else if (is_voltage_in_range(cp_hi, 12.)) {
            // +12V State A IDLE (open circuit)
            *cp = InternalCPState::A;
        } else if (is_voltage_in_range(cp_hi, 9.)) {
            *cp = InternalCPState::B;
        } else if (is_voltage_in_range(cp_hi, 6.)) {
            *cp = InternalCPState::C;
        } else if (is_voltage_in_range(cp_hi, 3.)) {
            *cp = InternalCPState::D;
        } else if (is_voltage_in_range(cp_hi, -12.)) {
            *cp = InternalCPState::F;

        } else {
            return false;
        }
        return true;
    }
    return false;
}

// checks if voltage is within center+-interval
bool ControlPilot::is_voltage_in_range(float voltage, float center)
{
    const float interval = 1.1;

    return ((voltage > center - interval) && (voltage < center + interval));
}

bool ControlPilot::power_on()
{
    bool success = true;
    if (!power_switch.isOn()) {
        rcd.deactivate();
        success = power_switch.switchOn();
        rcd.activate();
        // lock_sw_off_Tick = DL_SYSTICK_getValue();
    }
    return success;
}

bool ControlPilot::power_off()
{
    bool success = true;
    if (power_switch.isOn()) {
        // disable RCD
        rcd.deactivate();
        // actually switch off relays
        success = power_switch.switchOff();
        // lock_sw_off_Tick = DL_SYSTICK_getValue();
    }
    return success;
}

/* Check for discrepancy between lock command from AM62L and actual lock state */
void ControlPilot::check_lock()
{
    /* Lock if currently unlocked */
    if (plug_lock_set && !control_pilot_hal.plugLocked()) {
        control_pilot_hal.lockMotorLock();

        /* Unlock if currently locked */
    } else if (!plug_lock_set && control_pilot_hal.plugLocked()) {
        control_pilot_hal.lockMotorUnlock();
    }
}

// Checks if plug lock feedback is present
// This assumes the feedback is digital high
void ControlPilot::check_plug_lock()
{
    uint16_t pl    = control_pilot_hal.getPlugLockFB();
    plug_lock_good = (pl > 3000) ? true : false;
}

void ControlPilot::rcd_enable()
{
    rcd.enable();
}

void ControlPilot::rcd_disable()
{
    rcd.disable();
}

void ControlPilot::start_timer(uint32_t msecs)
{
    timer_countdown = msecs;
    // timer_tick = DL_SYSTICK_getValue();
}

void ControlPilot::stop_timer()
{
    timer_tick = 0;
}

bool ControlPilot::timer_elapsed()
{
    if (timer_tick != 0 &&
        DL_SYSTICK_getValue() - timer_tick > timer_countdown)
        return true;
    else
        return false;
    return false;
}

void ControlPilot::allow_power_on(bool p)
{
    power_on_allowed = p;
}

void ControlPilot::connector_lock(bool lock)
{
    if (lock) {
        plug_lock_set = true;
        control_pilot_hal.lockMotorLock();
        lock_sw_off_Tick = DL_SYSTICK_getValue();
    } else {
        plug_lock_set = false;
        control_pilot_hal.lockMotorUnlock();
        lock_sw_off_Tick = DL_SYSTICK_getValue();
    }
}

/* Check if temp > 90C. If so, state machine will reset back to and hold in state A. */
void ControlPilot::overtemp_check(float *temps)
{
    temp0    = temps[0];
    temp1    = temps[1];
    overtemp = (temps[0] > 90 || temps[1] > 90) ? true : false;
}
