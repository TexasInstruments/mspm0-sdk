/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#ifndef PIR_INTERFACE_H_
#define PIR_INTERFACE_H_

#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

/*! PIR Config Register Reserved Value */
#define PIR_CONFIG_RESERVED_VALUE (0x2 << 3)

/* Bit Shifts */
/*! PIR Threshold shift */
#define PIR_CONFIG_THRESHOLD_SHIFT (17)
/*! PIR Blind time shift */
#define PIR_CONFIG_BLINDTIME_SHIFT (13)
/*! PIR Pulse counter shift */
#define PIR_CONFIG_PULSECOUNTER_SHIFT (11)
/*! PIR Window time shift */
#define PIR_CONFIG_WINDOWTIME_SHIFT (9)
/*! PIR Operation mode shift */
#define PIR_CONFIG_OPRMODE_SHIFT (7)
/*! PIR Signal source shift */
#define PIR_CONFIG_SIGNALSOURCE_SHIFT (5)
/*! PIR High pass filter cutoff shift */
#define PIR_CONFIG_HPF_CUTOFF_SHIFT (2)
/*! PIR Count mode shift */
#define PIR_CONFIG_COUNTMODE_SHIFT (0)

/* Bit Masks */
/*! PIR Threshold mask */
#define PIR_CONFIG_THRESHOLD_MASK (0xFF << PIR_CONFIG_THRESHOLD_SHIFT)
/*! PIR Blind time mask */
#define PIR_CONFIG_BLINDTIME_MASK (0xF << PIR_CONFIG_BLINDTIME_SHIFT)
/*! PIR Pulse counter mask */
#define PIR_CONFIG_PULSECOUNTER_MASK (0x3 << PIR_CONFIG_PULSECOUNTER_SHIFT)
/*! PIR Window time mask */
#define PIR_CONFIG_WINDOWTIME_MASK (0x3 << PIR_CONFIG_WINDOWTIME_SHIFT)
/*! PIR Operation mode mask */
#define PIR_CONFIG_OPRMODE_MASK (0x3 << PIR_CONFIG_OPRMODE_SHIFT)
/*! PIR Signal source mask */
#define PIR_CONFIG_SIGNALSOURCE_MASK (0x3 << PIR_CONFIG_SIGNALSOURCE_SHIFT)
/*! PIR High pass filter cutoff mask */
#define PIR_CONFIG_HPF_CUTOFF_MASK (0x1 << PIR_CONFIG_HPF_CUTOFF_SHIFT)
/*! PIR Count mode mask */
#define PIR_CONFIG_COUNTMODE_MASK (0x1 << PIR_CONFIG_COUNTMODE_SHIFT)

/* Operation Modes */
/*! Operation Mode Forced Readout Value */
#define PIR_OPR_MODE_FORCED_READOUT (0)
/*! Operation Mode Interrupt Readout Value */
#define PIR_OPR_MODE_INTERRUPT_READOUT (1)
/*! Operation Mode Wake Up Value */
#define PIR_OPR_MODE_WAKE_UP (2)

/* Signal Source */
/*! Signal Source Band Pass Filtering Value */
#define PIR_SIGNAL_SOURCE_BPF (0)
/*! Signal Source Low Pass Filtering Value */
#define PIR_SIGNAL_SOURCE_LPF (1)
/*! Signal Source Temperature Sensor Value */
#define PIR_SIGNAL_SOURCE_TEMP_SENSOR (3)

/* HPF Cut-Off */
/*! High Pass Cut Off Frquency 0.4 Hz Value */
#define PIR_HPF_CUTOFF_0_4_Hz (0)
/*! High Pass Cut Off Frquency 0.2 Hz Value */
#define PIR_HPF_CUTOFF_0_2_Hz (1)

/* Count Mode */
/*! Count Mode with Zero Crossing Value */
#define PIR_COUNT_MODE_WITH_SIGN_CHANGE (0)
/*! Count Mode without Zero Crossing Value */
#define PIR_COUNT_MODE_WITHOUT_SIGN_CHANGE (1)

/*! Direct Link Frame Size */
#define DIRECT_LINK_FRAME_SIZE (40)

/*! Configuration Register Size */
#define CONFIG_REGISTER_SIZE (24)

/* Delay Time */
/*! Cycles for 1 us delay*/
#define DELAY_1_US (32)

/*!
* @brief variable to store Configuration Register Information
*/
typedef struct {
    /*! Stores the Threshold Value */
    uint8_t threshold;
    /*! Stores the Blind Time Value */
    uint8_t blindTime;
    /*! Stores the Pulse Counter Value */
    uint8_t pulseCounter;
    /*! Stores the Window Time Value */
    uint8_t windowTime;
    /*! Stores the Operation Mode Value */
    uint8_t operationMode;
    /*! Stores the Signal Source Value */
    uint8_t signalSource;
    /*! Stores the High Pass Filter Cut Off Value */
    bool hpfCutOff;
    /*! Stores the Count Mode Value */
    bool countMode;
} PIR_configRegister;

/*!
 * @brief variable to store the Input value from PIR
*/
typedef struct {
    /*! Stores the Configuration Register Value */
    uint32_t Config;
    /*! Stores the ADC Value */
    uint16_t AdcValue;
    /*! Indicates if the ADC Value is out of the range */
    bool OutOfRange;
} PIR_directLink;

PIR_directLink gPIR_directLink;

/*! Definition of global PIR Configuration Register variable */
extern PIR_configRegister gPIR_configRegister;

/**
 * @brief Initializes the PIR Interface
 *
 */
void PIR_init(void);

/**
 * @brief Configures PIR with specified configuration
 *
 */
void PIR_sendConfig(PIR_configRegister *configRegister);

#endif /* PIR_INTERFACE_H_ */
