/* --COPYRIGHT--,BSD
 * Copyright (c) 2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
#ifndef __DUALRAYSMOKEAFE_HAL_CONFIG_FR235X_TPS880x_H__
#define __DUALRAYSMOKEAFE_HAL_CONFIG_FR235X_TPS880x_H__

#include <Src/include/DualRaySmokeAFE_HAL.h>
#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

//*********** AFE ************************************************************
//! \brief ADC Clock frequency in Hz
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_CLOCKFREQ_HZ (4000000)

//! \brief Number of samples for each LED measurement
//!         Use power of 2 for simpler/faster division/shift
//!
#define DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES (4)

//! \brief Delay in ms after enabling the VBoost and LED LDO
//!         i.e. 10= 10ms
//!
#define DUALRAYSMOKEAFE_HAL_AFE_DELAY_VBOOST_ON_MS (10)

//! \brief Delay in ms before disabling the VBoost
//!         i.e. 10= 10ms
//!
#define DUALRAYSMOKEAFE_HAL_AFE_DELAY_VBOOSTOFF_MS (10)

//! \brief Setting of VREF for LED measurements
//!         i.e. 1300 = 1.3V
//!
#define DUALRAYSMOKEAFE_HAL_AFE_LED_VREF_MV (2300)

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
//! \brief Resistor for CSA measuring IR LED current
//!         i.e. 1 = 1 ohm
//!
#define DUALRAYSMOKEAFE_HAL_AFE_LED_CSA_RESISTANCE (1)

//! \brief Resistor for CSB measuring IR LED current
//!         i.e. 3 = 3 ohm
//!
#define DUALRAYSMOKEAFE_HAL_AFE_LED_CSB_RESISTANCE (3)

//! \brief Number of samples for LED measurement
//!         Use power of 2 for simpler/faster division/shift
//!
#define DUALRAYSMOKEAFE_HAL_AFE_LEDCURRENT_NUM_SAMPLES (1)
#endif

//! \brief ADC S&H for Photodiode measurements.
//!         Sample & Hold depends on external components.
//!         With Rs = 4.7K, Cext = 330pF, Ci = 5.5pF, Ri = 2K
//!         tsample >= (Ri + Rs) x ln(2^(12+2)) x (Ci + Cext)
//!         tsample >= 20.25us
//!         SHT_5 = 96 cycles SMCLK/2(4MHz) = 24us
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SNH (96)

//! \brief ADC cycles for Photodiode measurements.
//!         Total cycles include S&H and conversion time (14 cycles for 12-bit)
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SAMPLECYCLES \
    (DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SNH + 14)

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
//! \brief ADC S&H for LED current measurements.
//!         SHT_0 = 4 cycles SMCLK/2(4MHz) = 4us
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDCURR_SNH (4)

//! \brief ADC cycles for LED current measurements.
//!         Total cycles include S&H and conversion time (14 cycles for 12-bit)
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDCURR_SAMPLECYCLES \
    (DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDCURR_SNH + 14)

//! \brief Total ADC cycles for LED measurements.
//!         Includes photodiode samples and LED current samples.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDMEAS_TOTALCYCLES     \
    ((DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SAMPLECYCLES *         \
         DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES) +         \
        (DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDCURR_SAMPLECYCLES * \
            DUALRAYSMOKEAFE_HAL_AFE_LEDCURRENT_NUM_SAMPLES))
#else
//! \brief Total ADC cycles for LED measurements.
//!         Includes photodiode samples.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDMEAS_TOTALCYCLES \
    (DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SAMPLECYCLES *      \
        DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES)
#endif

//! \brief Total time in us for Photodiode measurements.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDMEAS_US               \
    (1.0 * DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDMEAS_TOTALCYCLES / \
        (DUALRAYSMOKEAFE_HAL_AFE_ADC_CLOCKFREQ_HZ / 1000000))

//! \brief Overhead cycles for turning LED on/off, before taking ADC samples.
//!         This timing was calculated from code using CCS optimization level 4.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_PREOVERHEAD_US (26.5)

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
//! \brief Overhead cycles for turning LED on/off, after taking ADC samples.
//!         This timing was calculated from code using CCS optimization level 4.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_POSTOVERHEAD_US (51.0)
#else
//! \brief Overhead cycles for turning LED on/off, after taking ADC samples.
//!         This timing was calculated from code using CCS optimization level 4.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_POSTOVERHEAD_US (17.625)
#endif

//! \brief Total time for measurement with LED ON, not including delay.
//!         Time includes:
//!         1) Processing pre-ADC measurement,
//!         2) ADC measurements of photodiode [and LED current if enabled],
//!         3) Processing post-ADC measurement.
//!
#define DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_TOTALTIME_US     \
    (DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDMEAS_US +                 \
        DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_PREOVERHEAD_US + \
        DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_POSTOVERHEAD_US)

#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
//! \brief Number of samples for CO measurement
//!         Use power of 2 for simpler/faster division/shift
//!
#define DUALRAYSMOKEAFE_HAL_AFE_CO_NUM_SAMPLES (4)

//! \brief Delay in us after AFE is enabled for CO measurement
//!         i.e. 150 = 150us
//!
#define DUALRAYSMOKEAFE_HAL_AFE_DELAY_MEASUREMENT3 (100)

#endif  // DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1)
//! \brief Setting of internal VREF in mV for Temperature Measurements using
//!         internal sensor.
//!         i.e. 1500 = 1.5V
//!
#define DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_VREF_MV (1500)

//! \brief Delay in us for internal reference for temperature measurement using
//!         internal sensor.
//!         i.e. 100 = 100us
//!
#define DUALRAYSMOKEAFE_HAL_AFE_DELAY_TEMPERATURE_INT_US (100)

//! \brief Number of samples for Temperature measurement using internal sensor.
//!         Use power of 2 for simpler/faster division/shift
//!
#define DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_NUM_SAMPLES (4)
#endif  // DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1)

//! \brief Setting of internal VREF in mV for Temperature Measurements using
//!         external sensor
//!         i.e. 1500 = 1.5V
//!
#define DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_VREF_MV (2300)

//! \brief Delay in us for internal reference for temperature measurement using
//!         external sensor
//!         i.e. 100 = 100us
//!
#define DUALRAYSMOKEAFE_HAL_AFE_DELAY_TEMPERATURE_EXT_US (1900)

//! \brief Number of samples for Temperature measurement using external sensor
//!         Use power of 2 for simpler/faster division/shift
//!
#define DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_NUM_SAMPLES (4)

#endif  // DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS

////*********** I/O **************************************************************

//! \brief Number of supported GPIO interrupt callbacks
//!
#define DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM (2)

//! \brief ID of Test button interrupt
//!
#define DUALRAYSMOKEAFE_HAL_IO_TESTBUTTON_INT_ID (0)

//! \brief ID of TPS880x GPIO interrupt
//!
#define DUALRAYSMOKEAFE_HAL_IO_TPS880X_GPIO_INT_ID (1)

//! \brief Number of timer events to detect long press
//!         I.e. 20 = 20x100ms = 2sec
#define DUALRAYSMOKEAFE_HAL_IO_TESTBUTTONTIMER_LONGPRESSCYCLES (20)

//! \brief Number of timer events to debounce
//!         I.e. 2 = 2x100ms = 200msec
#define DUALRAYSMOKEAFE_HAL_IO_TESTBUTTONTIMER_DEBOUNCECYCLES (2)

//*********** GUI Communication ************************************************
//! \brief baudrate of GUI Communication port
//!         i.e. 19200 = 19,200bps
//!
//#define DUALRAYSMOKEAFE_HAL_GUICOMM_BAUDRATE                (9600)

//*********** Timing ULP Timer (RTC) *******************************************
// Periodic interrupt working at lowest power consumption.
//  Can uses RTC with VLO source.
//  Can be calibrated for higher accuracy using GP Timer.
//  Expected to be ON all the time.
//  Optionally, it can use TPS880x internal timer and a GPIO interrupt.

//! \brief Set to 1 to use TPS880x for ULP timer instead of RTC
//!
// RTC SUPPORT CURRENTLY REMOVED ON M0 PORT
#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x (1)

//! \brief ULP Timer frequency in Hz
//!         RTC is set to VLO/10 for this demo
//!
#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_FREQ_HZ (1000)

//! \brief Source for ULP Timer in HZ
//!         Sourced by VLO
//!
#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_SOURCECLK_HZ (10000)

//! \brief Minimum Interval for ULP Timer in ms
//!
#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MIN_INTERVAL_MS (1)

//! \brief Maximum Interval for ULP Timer in ms
//!
#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MAX_INTERVAL_MS (65535)

//! \brief ULP Timer cycles used to calibrate VLO
//!
#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_CAL_CYCLES (50)

#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 0)
////! \brief Interrupt Vector for ULP Timer
//#define DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_VECTOR          (RTC_VECTOR)
#endif

////*********** Timing GP Timer **************************************************
//// General purpose timer used to create us and ms blocking
////  delays, and calibrate ULP Timer.
////  us delay and calibration use ACLK (REFO).
////  ms delay uses SMCLK (DCO).
////  Independent from ULP and LP since it needs to run
//

//! \brief Maximum number of callback functions supported by LP Periodic Timer.
#define DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX (3)

#endif  //__DUALRAYSMOKEAFE_HAL_CONFIG_FR235X_TPS880x_H__
