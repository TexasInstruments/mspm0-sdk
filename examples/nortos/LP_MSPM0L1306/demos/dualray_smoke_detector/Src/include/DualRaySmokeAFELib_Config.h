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
//  Dual Ray AFE Library Configuration
//
//  Static and default configuration of Library
#ifndef __DUALRAYSMOKEAFELIB_CONFIG_H__
#define __DUALRAYSMOKEAFELIB_CONFIG_H__

#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

//*********** Static Configuration *********************************************
//! \brief Types of LEDs supported for measurement
typedef enum {
    DUALRAYSMOKEAFE_LED_RED   = 0,
    DUALRAYSMOKEAFE_LED_BLUE  = 1,
    DUALRAYSMOKEAFE_LED_TOTAL = 2,
} tDualRaySmokeAFE_LED;

//! \brief Set to zero to disable infrared LED detection.
//!         Any other value enables this functionality.
//!
#define DUALRAYSMOKEAFELIB_ENABLE_INFRAREDLED_DETECTION (1)
//! \brief Set to zero to disable blue LED detection.
//!         Any other value enables this functionality.
//!
#define DUALRAYSMOKEAFELIB_ENABLE_BLUELED_DETECTION (1)

//! \brief Set to zero to disable LED current measurement
//!         Any other value enables this functionality.
//!
#define DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS (1)

//! \brief Set to zero to disable internal temperature measurement.
//!         Any other value enables this functionality.
//!
#define DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS (0)

//! \brief Set to zero to disable external temperature measurement.
//!         Any other value enables this functionality.
//!
#define DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS (1)

//! \brief Set to zero to disable CO measurement.
//!         Any other value enables this functionality.
//!
#define DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS (1)

//*********** Default Dynamic Configuration ************************************
//! \brief Default strength for IR LED.
//!     For TPS880x, this setting together with Temperature coefficient will
//!     indicate the LED current.
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDSTRENGTH (128)

//! \brief Default strength for Blue LED.
//!     For TPS880x, this setting together with Temperature coefficient will
//!     indicate the LED current.
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDSTRENGTH (100)

//! \brief Default threshold for IR LED delta
//!         I.e. 400 = 400 ADC counts compared to (IR light - IR dark)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_THRESHOLD (2000)

//! \brief Default threshold for blue LED delta
//!         I.e. 30 = 30 ADC counts compared to (IR light - IR dark)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_THRESHOLD (1000)

//! \brief Default gain for PGA when driving IR LED
//!         I.e. 5 = Gain of 5x
//!     Possible values: 5,11,20,35 (check datasheet)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_PGAGAIN (5)

//! \brief Default gain for PGA when driving Blue LED
//!         I.e. 5 = Gain of 5x
//!     Possible values: 5,11,20,35 (check datasheet)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_PGAGAIN (20)

//! \brief Default Hold time with LED ON before taking IR measurement
//!         I.e. 100 for 100us
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDHOLDON_US (100)

//! \brief Default Hold time with LED OFF before taking IR measurement
//!         I.e. 100 for 100us
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDHOLDOFF_US (100)

//! \brief Default Hold time with LED ON before taking blue measurement
//!         I.e. 100 for 100us
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDHOLDON_US (100)

//! \brief Default Hold time with LED OFF before taking blue measurement
//!         I.e. 100 for 100us
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDHOLDOFF_US (100)

//! \brief Default setting to enable Photodiode TIA
//!         I.e. true = Enabled
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_PD_TIA_EN (true)

//! \brief Default temperature coefficient for IR LED
//!     For TPS880x, this setting together with LED strength will
//!     indicate the LED current.
//!     I.e. 347 for 0.347mV/C
//!     Possible values: 347, 416, 693, 1040 (check datasheet)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_LEDTEMPCOEFF (1040)

//! \brief Default temperature coefficien for Blue LED.
//!     For TPS880x, this setting together with LED strength will
//!     indicate the LED current.
//!     I.e. 347 for 0.347mV/C
//!     Possible values: 347, 416, 693, 1040 (check datasheet)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_LEDTEMPCOEFF (1040)

#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
//! \brief Default setting to enable CO Measurements
//!         I.e. true = Enabled
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_CO_MEAS_EN (true)

//! \brief Default gain for CO
//!         I.e. 1100 = 1.1M
//!     Possible values: 1100, 300, 500, 800 (check datasheet)
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_CO_GAIN_KOHM (1100)

//! \brief Default setting to enable CO Amplifier
//!         I.e. true = Enabled
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_CO_AMP_EN (true)
#endif

#ifdef __AFE_SACL3__
//! \brief Default bias voltage for TIA using Red LED in mV
//!         I.e. 100 = 100mV
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_TIABIASMV (100)

//! \brief Default bias voltage for TIA using Red LED in mV
//!         I.e. 100 = 100mV
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_TIABIASMV (100)

//! \brief Default bias voltage for PGA using Red LED in mV
//!         I.e. 850 = 850mV
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_IR_PGABIASMV (850)

//! \brief Default bias voltage for PGA using Blue LED in mV
//!         I.e. 850 = 850mV
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_BLUE_PGABIASMV (850)
#endif

//! \brief Default sounder Frequency in HZ
//!         I.e. 496 = 496HzmA
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_SOUNDERFREQ_HZ (496)

#ifdef __AFE_TPS880X__
//! \brief Default setting for sounder volume controlled by VBOOST
//!         I.e. 15000 = 15V
//!     Possible values: 2700, 3800, 4700, 6000, 9000, 10000, 10500,
//!                      11000, 11500, 15000
//!
#define DUALRAYSMOKEAFELIB_DEFAULT_SOUNDERVOLUME (15000)
#endif

#endif  //__DUALRAYSMOKEAFELIB_CONFIG_H__
