/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 */

#ifndef SYSTEM_INFO_H_
#define SYSTEM_INFO_H_

#include <stdint.h>
#include "led_driver/TPS929240.h"

#define FALSE (0x00)
#define TRUE (0x01)

// Define if CAN or UART is used
#define CAN_USED FALSE

// When non-broadcast is transmitted, does the CRC need to be checked
#define ALWAYS_CHECK_CRC FALSE

// Total devices on FlexWire bus
#define DEVICE_CNT 1

// When set to 1, the EEPROM programming routine is executed instead of normal program
#define PROG_EEPROM FALSE
// When set to 1, program the EEPROM to the default value
#define PROG_DEFAULT_EEPROM FALSE
// Use external device address settings for EEPROM programming
#define USE_REF_PIN_FOR_EEPROM_PROG FALSE

extern const uint16_t device_address[DEVICE_CNT];

struct chipStatus {
    // Indicates open, short, and/or single-LED-short fault
    uint16_t OUT_flag;
    uint16_t SHORT_channels[MAX_CHANNEL_CNT];
    uint16_t OPEN_channels[MAX_CHANNEL_CNT];
    uint16_t SLS_channels[MAX_CHANNEL_CNT];  // Single-LED-short
    uint16_t EEPCRC;                         // EEPROM CRC fault
    uint16_t TSD;                            // Thermal Shutdown
    uint16_t PRETSD;                         // Pre-thermal shutdown warning
    uint16_t REF;                            // REF-pin fault
    uint16_t LOWSUP;                         // Low supply
    uint16_t POR;                            // Power-on-reset
#ifndef TPS92912X
    uint16_t SUPUV;    // Supply undervoltage
    uint16_t VBAT_mV;  // *1 mV
#endif
    uint16_t VSUPPLY_mV;    // *1 mV
    uint16_t VLDO_mV;       // *1 mV
    uint16_t TEMPSNS_10mC;  // *10 mC
    uint16_t VREF_100uV;    // *100 uV
    uint16_t IREF_10nA;     // *10 nA
};

extern struct chipStatus chip_status[];

#endif /* SYSTEM_INFO_H_ */
