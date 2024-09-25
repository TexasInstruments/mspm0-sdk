/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include <stdint.h>
/*!****************************************************************************
 *  @file       one_wire_target.h
 *  @brief      One Wire Target Interface hardware abstraction layer (HAL)
 *  @defgroup   OneWireTargetInterface
 *
 *  # Overview
 *  The  one wire target Interface hal layer provides a layer of abstraction
 *  above the base MSPM0 timers and GPIO calls.
 *
 *  <hr>
 ******************************************************************************
 */
#ifndef ONE_WIRE_TARGET_H_
#define ONE_WIRE_TARGET_H_

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
/**
 * Enum for OneWireTargetInterface state machine states
**/
typedef enum {
    OneWire_Start_State, /*!< OneWire Start State. Initial state in "regular speed" after reset */
    OneWire_ProcessROM_State, /*!< OneWire ProcessROM State. Process a ROM command in "regular speed"*/
    OneWire_ProcessMEM_State, /*!< OneWire ProcessMEM State. Process a MEMORY command in "regular speed"*/
    OneWire_ODStart_State, /*!< OneWire Start State. Initial state in "Overdrive speed" after reset */
    OneWire_ODProcessROM_State, /*!< OneWire ProcessROM State. Process a ROM command in "Overdrive speed"*/
    OneWire_ODProcessMEM_State, /*!< OneWire ProcessMEM State. Process a MEMORY command in "Overdrive speed"*/
    OneWire_Stop_State /*!< Not currently used */
} OneWireStave_State_t;

/**
 * Enum for oneWireTarget command return value
**/
typedef enum {
    OneWire_CMD_pass,     /*!< OneWire command success */
    OneWire_CMD_OD_pass,  /*!< OneWire overdrive command success */
    OneWire_CMD_fail,     /*!< OneWire command failed */
    OneWire_CMD_OD_Reset, /*!< OneWire overdrive reset detected */
    OneWire_CMD_Reset,    /*!< OneWire normal speed reset detected */
} oneWireTarget_CMD_return_t;

/**
 * @brief OneWireTargetInterface pulse width constants based on 0.25uS timer period
**/
#define microseconds_0 0 /* Not needed but added for completeness */
#define microseconds_2 8 /* 8 ticks * 0.25uS per tick = 2 microseconds */
#define microseconds_6 24
#define microseconds_15 60
#define microseconds_16 64
#define microseconds_48 192
#define microseconds_60 240
#define microseconds_80 320
#define microseconds_120 480
#define microseconds_480 1800
#define microseconds_960 3840
/* One wire target regular speed timings */
#define onewirewrite1min_delay microseconds_0
#define onewirewrite1max_delay microseconds_15
#define onewirewrite0min_delay microseconds_60
#define onewirewrite0max_delay microseconds_120
#define onewireResetmin_delay microseconds_480
#define onewireResetmax_delay microseconds_960
/* One wire target Overdrive speed timings */
#define onewireODwrite1min_delay microseconds_0
#define onewireODwrite1max_delay microseconds_2
#define onewireODwrite0min_delay microseconds_6
#define onewireODwrite0max_delay microseconds_16
#define onewireODResetmin_delay microseconds_48
#define onewireODResetmax_delay microseconds_80

#define delay_us(x) \
    delay_cycles((x * 32) - 1)  //Crude delay for now. Assumes 32MHz CPU clock

#define ONE_WIRE_MEMORY_SIZE 512

#define ONE_WIRE_NUM_DEVICES 4
#define ONE_WIRE_MAX_NUM_OF_DEVICES 4

typedef struct OneWireTargetConfig {
    uint8_t deviceCount;

    unsigned char regNums[ONE_WIRE_MAX_NUM_OF_DEVICES][8];

    /***********************************************************************************
 * This is important when ONE_WIRE_NUM_DEVICES > 1
 * Arrays: config.conflicts[ONE_WIRE_NUM_DEVICES][8]
 * -------------------------------------------------------------------------------
 *
 * When the MSPM0 emulates multiple target addresses the search ROM command response
 * must properly signal the arbitration losses at the correct bit positions so that
 * the controller knows when to continue or terminate the search.
 * Created these arrays to manually specify bit locations where conflicts occur
 * between the device addresses during each search ROM run. This information is
 * Used within the oneWireTarget_searchROM() function to determine when to signal
 * a conflict on the bus. Attempted to handle this automatically but algorithm
 * requires further development and refinement.
 * These arrays have to be manually initialized with a 1 in the bit position
 * corresponding to a loss of arbitration between a device ROM code# and the
 * subsequent device.
 * The first and last devices on the list always have a single conflict, but the
 * devices in the middle (e.g. device 2, 3...) have two conflicts; one with each
 * adjacent device.
 *
 ***********************************************************************************/
    unsigned char conflicts[ONE_WIRE_MAX_NUM_OF_DEVICES][7];

    // Currently this library uses two pins (connected externally) for 1-wire
    // communication

    // Input is connected to the timer instance
    GPTIMER_Regs* timerInst;

    // Output is controlled by the GPIO
    GPIO_Regs* outputPort;
    uint32_t outputPin;

} OneWireTargetConfig;

typedef struct OneWireTargetState {
    uint8_t targetState;
    uint8_t activeDevice;  // Used to identify matched device
    uint8_t searchDevice;  // Used in search ROM algo
    bool targetMatchSuccessful;
    bool targetResumeFlag;
    bool lowPulseDetected;  // 1-wire bus idle state is high
    bool targetRead;
    uint8_t targetRcvBit;
    uint32_t
        tLow;  // For now use a global variable to confirm pulse width measurements
    // Address registers
    uint16_t targetTargetAddress;
    uint16_t targetEndAddressDataStatus;
    uint8_t writeValue;
    uint8_t bitCount;
    bool skipPulse;
} OneWireTargetState;

typedef struct OneWireTarget {
    OneWireTargetConfig config;
    OneWireTargetState state;
} OneWireTarget;

void oneWireTarget_init(OneWireTarget* ow);
void oneWireTarget_checkState(OneWireTarget* ow);
void oneWireTarget_sendPresence(OneWireTarget* ow);
unsigned char oneWireTarget_receiveBit(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_processROM(
    OneWireTarget* ow, uint8_t romCommand);
oneWireTarget_CMD_return_t oneWireTarget_processMEM(
    OneWireTarget* ow, uint8_t memoryCommand);
oneWireTarget_CMD_return_t oneWireTarget_readROM(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_searchROM(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_matchROM(OneWireTarget* ow);
unsigned char oneWireTarget_sendBit(OneWireTarget* ow, unsigned char bit);
oneWireTarget_CMD_return_t oneWireTarget_ODmatchROM(OneWireTarget* ow);  //0x69
unsigned char oneWireTarget_ODsendBit(OneWireTarget* ow, unsigned char bit);
unsigned char oneWireTarget_ODreceiveBit(OneWireTarget* ow);
void oneWireTarget_ODsendPresence(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_readMemory(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_writeScratchpad(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_readScratchpad(OneWireTarget* ow);
oneWireTarget_CMD_return_t oneWireTarget_copyScratchpad(OneWireTarget* ow);
void oneWireTarget_pulseStarted(OneWireTarget* ow);
void oneWireTarget_pulseFinished(OneWireTarget* ow);
void oneWireTarget_computeRegNumCRC(OneWireTarget* ow, uint8_t idx);
unsigned char crc8(unsigned char addr[], unsigned char length);
unsigned int crc16(unsigned char input, unsigned int crc16);

#endif /* ONE_WIRE_TARGET_H_ */
