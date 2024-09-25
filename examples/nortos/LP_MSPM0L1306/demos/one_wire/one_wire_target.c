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

#include "one_wire_target.h"

uint8_t oneWireMemory[ONE_WIRE_MEMORY_SIZE] = {0};

uint8_t oneWireScratchpad[32] = {0};

uint8_t eprom_2502_image[ONE_WIRE_MEMORY_SIZE] = {0x8d, 0x1d, 0x01, 0x01, 0x00,
    0x00, 0xc7, 0x53, 0x5c, 0x04, 0xcd, 0x32, 0xff, 0xff, 0x2b, 0xc0, 0x75,
    0x02, 0x94, 0xff, 0x01, 0xb0, 0xfe, 0xff, 0xff, 0xaf, 0x19, 0x05, 0xdc,
    0x05, 0xdc, 0x3f, 0xff, 0xec, 0x3c, 0x50, 0x4d, 0x4e, 0x4e, 0x34, 0x35,
    0x34, 0x33, 0x41, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0x36, 0x08, 0x02,
    0x5c, 0x01, 0xa9, 0xb4, 0xa9, 0xac, 0xa6, 0x98, 0x0c, 0x01, 0x7b, 0x7a,
    0xff, 0x04, 0x00, 0x7a, 0x00, 0x6a, 0x00, 0x66, 0x00, 0x63, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x05, 0x00, 0xff, 0xff, 0xff, 0x03,
    0xe1, 0x76, 0x04, 0xd0, 0x96, 0xf0, 0x10, 0xa9, 0x32, 0x30, 0x31, 0x37,
    0x4d, 0x4f, 0x54, 0x53, 0x4f, 0x4c, 0x54, 0x4e, 0x53, 0x04, 0x06, 0x01,
    0x19, 0x9c, 0x69};

/*
 * Init function for 1wire target interface
 */
void oneWireTarget_init(OneWireTarget* ow)
{
    ow->state.targetState                = OneWire_Start_State;
    ow->state.activeDevice               = 0;
    ow->state.searchDevice               = 0;
    ow->state.targetMatchSuccessful      = false;
    ow->state.targetResumeFlag           = false;
    ow->state.targetRead                 = false;
    ow->state.targetTargetAddress        = 0;
    ow->state.targetEndAddressDataStatus = 0;

    DL_GPIO_setPins(ow->config.outputPort, ow->config.outputPin);

    DL_TimerG_startCounter(ow->config.timerInst);

    uint16_t i;
    for (i = 0; i < ONE_WIRE_MEMORY_SIZE; i++) {
        oneWireMemory[i] = eprom_2502_image[i];
    }
}

void oneWireTarget_checkState(OneWireTarget* ow)
{
    if ((false == ow->state.lowPulseDetected) &&
        (false == ow->state.skipPulse)) {
        return;
    }
    ow->state.lowPulseDetected = false;
    ow->state.skipPulse        = false;  // Disable skip state flag

    if ((ow->state.tLow >= onewireResetmin_delay) &&
        (ow->state.tLow <= onewireResetmax_delay)) {
        // Reset pulse detected. Send a presence pulse and remain in Start state
        DL_TimerG_disableInterrupt(
            ow->config.timerInst, DL_TIMERG_INTERRUPT_CC0_DN_EVENT);
        oneWireTarget_sendPresence(ow);  // Send Presence Pulse
        ow->state.targetState = OneWire_Start_State;
        DL_TimerG_startCounter(
            ow->config
                .timerInst);  //Incase timer was disabled during an aborted read
        DL_TimerG_clearInterruptStatus(
            ow->config.timerInst, DL_TIMERG_INTERRUPT_CC0_DN_EVENT);
        DL_TimerG_enableInterrupt(
            ow->config.timerInst, DL_TIMERG_INTERRUPT_CC0_DN_EVENT);
    } else {
        switch (ow->state.targetState) {
            oneWireTarget_CMD_return_t cmdRet;

            case OneWire_Start_State:
                // Reset working variables
                ow->state.writeValue            = 0;      // Reset write value
                ow->state.bitCount              = 0;      // Reset bit count
                ow->state.targetMatchSuccessful = false;  // Reset match flag
                if (ow->state.tLow > onewirewrite1min_delay) {
                    // Receive a bit and set next state to OneWire_ProcessROM_State
                    ow->state.writeValue |=
                        ow->state.targetRcvBit
                        << ow->state.bitCount++;  // Receive a bit
                    ow->state.targetState = OneWire_ProcessROM_State;
                }
                break;

            case OneWire_ProcessROM_State:
                if (ow->state.tLow > onewirewrite1min_delay) {
                    // Receive a bit and set next state to OneWire_ProcessROM_State
                    ow->state.writeValue |=
                        ow->state.targetRcvBit
                        << ow->state.bitCount++;  // Receive a bit
                    if (ow->state.bitCount == 8) {
                        if (!ow->state
                                 .targetMatchSuccessful)  // If no match condition then process the ROM command
                        {
                            cmdRet = oneWireTarget_processROM(
                                ow, ow->state.writeValue);
                            if (OneWire_CMD_pass == cmdRet) {
                                ow->state.targetState =
                                    OneWire_ProcessROM_State;  // Regular speed
                            } else if (OneWire_CMD_OD_pass == cmdRet) {
                                ow->state.targetState =
                                    OneWire_ODProcessROM_State;  // Overdrive speed
                            } else if (
                                OneWire_CMD_Reset ==
                                cmdRet)  // if False returned then we need to reset.
                            {
                                // Advance to Start state
                                ow->state.skipPulse = true;
                                ow->state.targetState =
                                    OneWire_Start_State;  // For now assume regular speed only
                            }
                            ow->state.writeValue = 0;
                            ow->state.bitCount   = 0;
                        } else {
                            // Advance to ProcessMEM state
                            ow->state.skipPulse   = true;
                            ow->state.targetState = OneWire_ProcessMEM_State;
                        }
                    }
                }
                break;
            case OneWire_ProcessMEM_State:
                ow->state.skipPulse = false;  // Disable skip state flag

                cmdRet = oneWireTarget_processMEM(ow,
                    ow->state
                        .writeValue);  // If match condition successful then process the memory function command
                if (OneWire_CMD_Reset ==
                    cmdRet)  // if False returned then we need to reset.
                {
                    ow->state.skipPulse   = true;  // Advance to Start state
                    ow->state.targetState = OneWire_Start_State;  //
                } else {
                    ow->state.targetState = OneWire_ProcessROM_State;
                    ow->state.writeValue  = 0;
                    ow->state.bitCount    = 0;
                }
                break;
            case OneWire_ODStart_State:
                // Reset working variables
                ow->state.writeValue            = 0;      // Reset write value
                ow->state.bitCount              = 0;      // Reset bit count
                ow->state.targetMatchSuccessful = false;  // Reset match flag

                if (ow->state.tLow > onewireODwrite1min_delay) {
                    // Receive a bit and set next state to OneWire_ProcessROM_State
                    ow->state.writeValue |=
                        ow->state.targetRcvBit
                        << ow->state.bitCount++;  // Receive a bit
                    ow->state.targetState = OneWire_ODProcessROM_State;
                }
                break;
            case OneWire_ODProcessROM_State:
                if ((ow->state.tLow >= onewireODResetmin_delay) &&
                    (ow->state.tLow <= onewireODResetmax_delay)) {
                    // OD Reset pulse detected. Send a presence pulse and remain in OD Start state
                    oneWireTarget_ODsendPresence(ow);  // Send Presence Pulse
                    ow->state.targetState = OneWire_ODStart_State;
                }

                break;
            case OneWire_ODProcessMEM_State:
                break;
        }
    }
    if (true == ow->state.skipPulse) {
        oneWireTarget_checkState(ow);
    }
}

/*
 * void oneWireTarget_sendPresence(void) - Used to send a presence pulse on 1-wire line
 * in response to a reset pulse.
 * Currently uses a separate GPIO in OD mode that must be externally tied with timer pin.
 *
 */
void oneWireTarget_sendPresence(OneWireTarget* ow)
{
    DL_TimerG_stopCounter(ow->config.timerInst);
    DL_GPIO_clearPins(ow->config.outputPort, ow->config.outputPin);
    delay_us(150);
    DL_GPIO_setPins(ow->config.outputPort, ow->config.outputPin);
    DL_TimerG_startCounter(ow->config.timerInst);
}

//*****************************************************************************
// Receive bit function. Reused from TIDM-1WIREEEPROM
// Receives a bit from the controller device
// \return  Value of the receive bit
//*****************************************************************************
inline unsigned char oneWireTarget_receiveBit(OneWireTarget* ow)
{
    unsigned char bit = 0;
    if (ow->state.tLow <= onewirewrite1max_delay) {
        bit = 1;  // Write logic 1 detected
    }
    return bit;
}

//*****************************************************************************
// Process ROM function. Reused from TIDM-1WIREEEPROM
// Contains all of the necessary ROM function commands as well as a few flag updates
// \romCommand  Value of the ROM function command to execute
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_processROM(
    OneWireTarget* ow, uint8_t romCommand)
{
    oneWireTarget_CMD_return_t retVal = OneWire_CMD_pass;
    switch (romCommand) {
        case 0x33:  // Read ROM
            retVal = oneWireTarget_readROM(ow);
            break;
        case 0x55:  // Match ROM
            retVal = oneWireTarget_matchROM(ow);
            if (retVal == OneWire_CMD_pass) {
                ow->state.targetMatchSuccessful = true;
            } else {
                ow->state.targetMatchSuccessful = false;
                ow->state.targetResumeFlag      = false;
            }
            break;
        case 0xF0:  // Search ROM
            retVal = oneWireTarget_searchROM(ow);
            break;
        case 0xCC:  // Skip ROM, mark as successful
            ow->state.targetMatchSuccessful = true;
            break;
        case 0xA5:  // Resume, mark status according to previous match command
            if (ow->state.targetResumeFlag) {
                ow->state.targetMatchSuccessful = true;
            } else {
                ow->state.targetMatchSuccessful = false;
                retVal                          = OneWire_CMD_fail;
            }
            break;
        case 0x69:  // OD Match ROM
            retVal = oneWireTarget_ODmatchROM(ow);

            if (retVal == OneWire_CMD_pass) {
                ow->state.targetMatchSuccessful = true;
            } else {
                ow->state.targetMatchSuccessful = false;
                ow->state.targetResumeFlag      = false;
            }
            break;
        default:
            break;
    }
    return (retVal);
}

//*****************************************************************************
// Process memory function.  Reused from TIDM-1WIREEEPROM
// Contains all of the necessary memory function commands
// \memoryCommand   Value of the memory function command to execute
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_processMEM(
    OneWireTarget* ow, uint8_t memoryCommand)
{
    ow->state.targetMatchSuccessful = false;  // No longer a match
    ow->state.targetResumeFlag =
        true;  // Could sustain match with resume command
    oneWireTarget_CMD_return_t retVal = OneWire_CMD_pass;
    switch (memoryCommand) {
        case 0x0F:  // Write Scratchpad
            retVal = oneWireTarget_writeScratchpad(ow);
            break;
        case 0xAA:  // Read Scratchpad
            retVal = oneWireTarget_readScratchpad(ow);
            break;
        case 0x55:  // Copy Scratchpad
            retVal = oneWireTarget_copyScratchpad(ow);
            break;
        case 0xF0:  // Read Memory
            retVal = oneWireTarget_readMemory(ow);
            break;
        default:
            break;
    }
    return (retVal);
}

//*****************************************************************************
// Read ROM function.  Reused from TIDM-1WIREEEPROM
// Sends the 8-byte ROM value to the controller device
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_readROM(OneWireTarget* ow)
{
    uint8_t loop, charBitMask, bitSend;
    for (
        loop = 0; loop < 8;
        loop++) {  // Loop to read the 8 byte ROM value if only one target is present
        for (charBitMask = 0x01; charBitMask; charBitMask <<= 1) {
            bitSend = (charBitMask &
                          ow->config.regNums[ow->state.activeDevice][loop])
                          ? 1
                          : 0;
            if (oneWireTarget_sendBit(ow, bitSend) == 1) {
                return (OneWire_CMD_Reset);
            }
        }
    }
    return (OneWire_CMD_pass);
}

//*****************************************************************************
// Search ROM function.  Reused from TIDM-1WIREEEPROM
// MSP430 device sends the ROM value so long as the controller keeps requesting it.
// Otherwise, it halts to allow other one-wire devices to send their ROM value.
// Multi-target support: See comments at def of ow->config.conflicts[][]
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_searchROM(OneWireTarget* ow)
{
    volatile uint8_t loop, charBitMask, bitSend;
    volatile uint8_t x;

    for (loop = 0; loop < 8;
         loop++)  // Loop to read 8 byte ROM value of multiple 1-wire targets
    {
        for (charBitMask = 0x01; charBitMask; charBitMask <<= 1) {
            bitSend = (charBitMask &
                          ow->config.regNums[ow->state.searchDevice][loop])
                          ? 1
                          : 0;

            if ((ow->config.deviceCount > 1) && (loop < 7) &&
                (charBitMask &
                    ow->config.conflicts
                        [ow->state.searchDevice]
                        [loop]))  // If there is a conflict send 0 - 0 to signal a conflict (don't do this for CRC byte)
            {
                if (oneWireTarget_sendBit(ow, 0) == 1) {
                    return (OneWire_CMD_Reset);
                }

                if (oneWireTarget_sendBit(ow, 0) == 1) {
                    return (OneWire_CMD_Reset);
                }
            } else  // Otherwise if there are no conflicts we send bit followed by compliment
            {
                if (oneWireTarget_sendBit(ow, bitSend) == 1) {
                    return (OneWire_CMD_Reset);
                }

                if (oneWireTarget_sendBit(ow, !bitSend) == 1) {
                    return (OneWire_CMD_Reset);
                }
            }
            // Now receive bit value back from controller
            ow->state.lowPulseDetected = false;
            while (false == ow->state.lowPulseDetected) {
                __WFE();
            }
            if ((ow->state.targetRcvBit !=
                    bitSend)) {  //We have lost arbitration to another target (external or internal)
                return (OneWire_CMD_fail);
            }
        }
    }
    if (++(ow->state.searchDevice) == ow->config.deviceCount) {
        ow->state.searchDevice =
            0;  //Reset to first device incase another searchrom command occurs
    }

    return (OneWire_CMD_pass);
}

//*****************************************************************************
// Match ROM function.  Reused from TIDM-1WIREEEPROM
// If the ROM value sent by the controller matches the MSP430 device's ROM number,
// get ready to receive memory commands.  Otherwise ignore the incoming commands
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_matchROM(OneWireTarget* ow)
{
    uint8_t byteCounter     = 0;
    uint8_t bits            = 0;
    uint8_t device          = 0;
    bool match              = false;
    unsigned char valROM[8] = {0};
    while (byteCounter < 8) {
        // Wait for timer pulse
        ow->state.lowPulseDetected = false;
        while (false == ow->state.lowPulseDetected) {
            __WFE();
        }
        if ((ow->state.tLow >= onewireResetmin_delay) &&
            (ow->state.tLow <=
                onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
        {
            return (OneWire_CMD_Reset);
        } else if (
            ow->state.tLow >= onewirewrite0min_delay &&
            ow->state.tLow <=
                onewirewrite0max_delay)  // Write logic 0 detected between 50 and 120 us @ 16 MHz
            bits++;  // No need to set a bit, move to next MSB
        else if (
            ow->state.tLow <=
            onewirewrite1max_delay)  // Write logic 1 detected between 0 and 15 us @ 16 MHz
        {
            valROM[byteCounter] |= 1
                                   << bits;  // Set 1 to proper writeValue bit
            bits++;                          // Move to next MSB
        }
        if (bits == 8)  // Has a full byte been sent?
        {
            bits = 0;  // Reset bit counter
            while ((device < ow->config.deviceCount) &&
                   (match == false))  // Check remaining devices
            {
                if (valROM[byteCounter] !=
                    ow->config.regNums[device][byteCounter]) {
                    device++;  // Check next device
                } else {
                    match = true;
                }
            }
            if (match == false)  // No device ROM IDs matched
            {
                ow->state.activeDevice = 0;  // Reset active device
                return (OneWire_CMD_fail);
            }
            ow->state.activeDevice =
                device;     // Update active device index and continue
            match = false;  // reset for next byte check
            byteCounter++;
        }
    }
    ow->state.lowPulseDetected = false;
    return (OneWire_CMD_pass);
}

//*****************************************************************************
// Send bit function.  Reused from TIDM-1WIREEEPROM
// Sends a bit value to the controller device
// \bit     Bit value to be send to the controller
// \return  Bit was either sucessfully sent or a reset occurred
// Currently uses a separate GPIO in OD mode that must be externally tied with timer pin.
//*****************************************************************************
inline unsigned char oneWireTarget_sendBit(
    OneWireTarget* ow, unsigned char bit)
{
    // Wait for timer pulse
    ow->state.lowPulseDetected = false;
    if (bit == 0) {
        ow->state.targetRead = true;
        while (true == ow->state.targetRead) {
            __WFE();
        }
        delay_us(60);
        DL_GPIO_setPins(ow->config.outputPort, ow->config.outputPin);
    }
    while (false == ow->state.lowPulseDetected) {
        __WFE();
    }
    if ((ow->state.tLow >= onewireResetmin_delay) &&
        (ow->state.tLow <=
            onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
    {
        return 1;
    }
    return 0;
}

//*****************************************************************************
// CRC8 function.  Reused from TIDM-1WIREEEPROM
// Calculates the CRC8 value for the MSB of the registration number
// \addr[]  The least significant 7 bytes of the registration number
// \length  Number of bytes to be checked by the CRC
// \return  CRC8 byte added to the end of the registration number
//*****************************************************************************
unsigned char crc8(unsigned char addr[], unsigned char length)
{
    unsigned char loop, crc8 = 0;

    while (length--) {
        unsigned char inbyte = *addr++;
        for (loop = 8; loop; loop--) {
            unsigned char mix = (crc8 ^ inbyte) & 0x01;
            crc8 >>= 1;
            if (mix) crc8 ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc8;
}

//*****************************************************************************
// CRC16 function.   Reused from TIDM-1WIREEEPROM
// Calculates the next iteration of the CRC16 value based on the previous input
// \input   New byte to be appended to the CRC16
// \crc16   Previous CRC16 value
// \return  Updated CRC16 value
//*****************************************************************************
unsigned int crc16(unsigned char input, unsigned int crc16)
{
    static const unsigned char oddparity[16] = {
        0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};

    unsigned int cdata = input;
    cdata              = (cdata ^ crc16) & 0xff;
    crc16 >>= 8;

    if (oddparity[cdata & 0x0F] ^ oddparity[cdata >> 4]) crc16 ^= 0xC001;

    cdata <<= 6;
    crc16 ^= cdata;
    cdata <<= 1;
    crc16 ^= cdata;

    return crc16;
}

//*****************************************************************************
// Read memory function. Reused from TIDM-1WIREEEPROM
// Sends the memory contents to the controller, the length of which depends on the
// settings determined by the type of power sourcing the MSP430
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_readMemory(OneWireTarget* ow)
{
    unsigned int startAddress = 0;
    unsigned int byteCounter  = 0;
    unsigned int loop;
    unsigned char bitCount = 0;
    unsigned char bitSend, charBitMask;

    while (1) {
        // Wait for timer pulse
        ow->state.lowPulseDetected = false;
        while (false == ow->state.lowPulseDetected) {
            __WFE();
        }
        if ((ow->state.tLow >= onewireResetmin_delay) &&
            (ow->state.tLow <=
                onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
        {
            return (OneWire_CMD_Reset);
        }

        if (ow->state.targetRcvBit ==
            1)  // Write logic 1 detected between 0 and 15 us @ 16 MHz
        {
            startAddress |=
                1 << (bitCount +
                      byteCounter *
                          8);  // If first two bytes set 1 to proper startAddress bit
        }
        // else logic 0 detected
        bitCount++;  // Move to next MSB

        if (bitCount == 8)  // Has a full byte been sent?
        {
            bitCount = 0;  // Reset bit counter
            byteCounter++;
        }
        if (byteCounter ==
            2)  // Past the startAddress value, ready to send data
        {
            for (
                loop = 0; loop < ONE_WIRE_MEMORY_SIZE - startAddress;
                loop++)  // Loop to send memory contents from start to end of memory
            {
                for (charBitMask = 0x01; charBitMask; charBitMask <<= 1) {
                    bitSend =
                        (charBitMask & oneWireMemory[startAddress + loop]) ? 1
                                                                           : 0;
                    if (oneWireTarget_sendBit(ow, bitSend) == 1) {
                        return (OneWire_CMD_Reset);
                    }
                }
            }
        }
    }
}

//*****************************************************************************
// Write Scratchpad function. Reused from TIDM-1WIREEEPROM
// Receives data bytes from the controller and stores them into the scratchpad.
// Computes a CRC16 value if the scratchpad has been filled
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_writeScratchpad(OneWireTarget* ow)
{
    unsigned char bytesSent[34] = {0};
    unsigned int crc16Value     = 0;
    unsigned int byteCounter    = 0;
    unsigned char bitCount      = 0;
    unsigned char bitSend, charBitMask;
    uint16_t intBitMask;

    ow->state.targetEndAddressDataStatus = 1 << 6;  // Set PF bit to start off
    ow->state.targetTargetAddress        = 0;
    crc16Value =
        crc16(0x0F, crc16Value);  // 0x0F is command code for writeScratchpad

    while (1) {
        // Wait for timer pulse
        ow->state.lowPulseDetected = false;
        while (false == ow->state.lowPulseDetected) {
            __WFE();
        }
        if ((ow->state.tLow >= onewireResetmin_delay) &&
            (ow->state.tLow <=
                onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
        {
            if (bitCount == 0 &&
                byteCounter >
                    1)  // New byte and past the target address? Update end address
                ow->state.targetEndAddressDataStatus =
                    (ow->state.targetTargetAddress & 0x01F) +
                    (byteCounter - 2);
            else if (
                bitCount != 0 &&
                byteCounter >
                    1)  // Past target address but interrupted byte? Update end address with partial byte flag set
                ow->state.targetEndAddressDataStatus =
                    (ow->state.targetTargetAddress & 0x01F) +
                    (byteCounter - 2) + 0x020;

            return (OneWire_CMD_Reset);
        }

        if (ow->state.targetRcvBit ==
            1)  // Write logic 1 detected between 0 and 15 us @ 16 MHz
        {
            if (byteCounter <= 1)
                ow->state.targetTargetAddress |=
                    1 << (bitCount +
                          byteCounter *
                              8);  // If first two bytes set 1 to proper targetAddress bit
            else
                oneWireScratchpad[byteCounter - 2] |=
                    1 << bitCount;  // Else set 1 to proper scratchpad bit
            bytesSent[byteCounter] |=
                1 << bitCount;  // This is for the CRC16 generation
        }
        // else logic 0 detected
        bitCount++;  // Move to next MSB

        if (bitCount == 8)  // Has a full byte been sent?
        {
            bitCount = 0;  // Reset bit counter
            byteCounter++;
            crc16Value = crc16(bytesSent[byteCounter - 1],
                crc16Value);  // Calculate next crc16 iteration
        }
        if ((ow->state.targetTargetAddress & 0x01F) + (byteCounter - 2) ==
            32)  // Has the scratchpad filled up?
        {
            ow->state.targetEndAddressDataStatus =
                0x01F;  // Set end address value
            for (intBitMask = 0x01; intBitMask; intBitMask <<= 1) {
                bitSend = (intBitMask & crc16Value) ? 1 : 0;
                if (oneWireTarget_sendBit(ow, !bitSend) == 1) {
                    return (OneWire_CMD_Reset);
                }
            }
            return (OneWire_CMD_pass);
        }
    }
}

//*****************************************************************************
// Read scratchpad function. Reused from TIDM-1WIREEEPROM
// Sends the scratchpad contents to the controller device for verification
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_readScratchpad(OneWireTarget* ow)
{
    unsigned char bitSend, charBitMask, loop;

    uint16_t intBitMask;

    for (intBitMask = 0x01; intBitMask;
         intBitMask <<= 1)  // Send the target address
    {
        if (oneWireTarget_sendBit(ow,
                (intBitMask & ow->state.targetTargetAddress) ? 1 : 0) == 1) {
            return (OneWire_CMD_Reset);
        }
    }
    for (charBitMask = 0x01; charBitMask;
         charBitMask <<= 1)  // Send the end address and data status flags
    {
        if (oneWireTarget_sendBit(
                ow, (charBitMask & ow->state.targetEndAddressDataStatus)
                        ? 1
                        : 0) == 1) {
            return (OneWire_CMD_Reset);
        }
    }
    for (loop = 0; loop < (0x020 - (ow->state.targetTargetAddress & 0x01F));
         loop++)  // Send the scratchpad contents
    {
        for (charBitMask = 0x01; charBitMask; charBitMask <<= 1) {
            bitSend = (charBitMask & oneWireScratchpad[loop]) ? 1 : 0;
            if (oneWireTarget_sendBit(ow, bitSend) == 1) {
                return (OneWire_CMD_Reset);
            }
        }
    }
    return (OneWire_CMD_pass);
}

//*****************************************************************************
// Copy scratchpad function. Reused from TIDM-1WIREEEPROM
// Saves the scratchpad contents into allocated FRAM memory if the controller sends
// the correct start and end addresses.  Then returns alternating 0 and 1 bits
// to indicate that the save has been completed
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_copyScratchpad(OneWireTarget* ow)
{
    unsigned int byteCounter        = 0;
    unsigned char bitCount          = 0;
    unsigned int checkTargetAddress = 0;
    unsigned int checkEndAddress    = 0;
    unsigned char loop;

    while (1) {
        // Wait for timer pulse
        ow->state.lowPulseDetected = false;
        while (false == ow->state.lowPulseDetected) {
            __WFE();
        }
        if ((ow->state.tLow >= onewireResetmin_delay) &&
            (ow->state.tLow <=
                onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
        {
            return (OneWire_CMD_Reset);
        }

        if (ow->state.targetRcvBit ==
            1)  // Write logic 1 detected between 0 and 15 us @ 16 MHz
        {
            if (byteCounter <= 1)
                checkTargetAddress |=
                    1 << (bitCount +
                          byteCounter *
                              8);  // If first two bytes set 1 to proper targetAddress bit
            else
                checkEndAddress |=
                    1 << bitCount;  // Else set 1 to proper scratchpad bit
        }
        // else logic 0 detected
        bitCount++;  // Move to next MSB

        if (bitCount == 8)  // Has a full byte been sent?
        {
            bitCount = 0;  // Reset bit counter
            byteCounter++;
        }
        if (byteCounter == 3) {
            if (checkTargetAddress != ow->state.targetTargetAddress ||
                checkEndAddress != ow->state.targetEndAddressDataStatus)
                return (OneWire_CMD_fail);  //Addresses don't match
            else  // Only copy from scratchpad to memory if addresses align correctly
            {
                for (loop = 0;
                     loop <= ((ow->state.targetEndAddressDataStatus & 0x01F) -
                                 (ow->state.targetTargetAddress & 0x01F));
                     loop++)
                    oneWireMemory[ow->state.targetTargetAddress + loop] =
                        oneWireScratchpad[loop];

                ow->state.targetEndAddressDataStatus |=
                    0x80;  // Update authorization accepted bit
                for (;;) {
                    if (oneWireTarget_sendBit(ow, 0) == 1) {
                        return (OneWire_CMD_Reset);
                    }  //Send 0xAA repeatedly until reset condition present
                    if (oneWireTarget_sendBit(ow, 1) == 1) {
                        return (OneWire_CMD_Reset);
                    }
                }
            }
        }
    }
}

//*****************************************************************************
// Match ROM function.  Reused from TIDM-1WIREEEPROM. Modified for overdrive
// If the ROM value sent by the controller matches the MSP430 device's ROM number,
// get ready to receive memory commands.  Otherwise ignore the incoming commands
//*****************************************************************************
oneWireTarget_CMD_return_t oneWireTarget_ODmatchROM(OneWireTarget* ow)
{
    uint8_t byteCounter     = 0;
    uint8_t bits            = 0;
    uint8_t device          = 0;
    bool match              = false;
    unsigned char valROM[8] = {0};
    while (byteCounter < 8) {
        // Wait for timer pulse
        ow->state.lowPulseDetected = false;
        while (false == ow->state.lowPulseDetected) {
            __WFE();
        }
        if ((ow->state.tLow >= onewireResetmin_delay) &&
            (ow->state.tLow <=
                onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
        {
            return (OneWire_CMD_Reset);
        }

        if (ow->state.tLow <=
            onewireODwrite1max_delay)  // Write logic 1 detected between 0 and 2 us
        {
            valROM[byteCounter] |= 1
                                   << bits;  // Set 1 to proper writeValue bit
        }
        bits++;  // Move to next MSB

        if (bits == 8)  // Has a full byte been sent?
        {
            bits = 0;  // Reset bit counter
            while ((device < ow->config.deviceCount) &&
                   (match == false))  // Check remaining devices
            {
                if (valROM[byteCounter] !=
                    ow->config.regNums[device][byteCounter]) {
                    device++;  // Check next device
                } else {
                    match = true;
                }
            }
            if (match == false)  // No device ROM IDs matched
            {
                ow->state.activeDevice = 0;  // Reset active device
                return (OneWire_CMD_fail);
            }
            ow->state.activeDevice =
                device;     // Update active device index and continue
            match = false;  // reset for next byte check
            byteCounter++;
        }
    }
    return (OneWire_CMD_OD_pass);
}

//*****************************************************************************
// Send bit function.  Reused from TIDM-1WIREEEPROM. Modified for OD
// Sends a bit value to the controller device
// \bit     Bit value to be send to the controller
// \return  Bit was either sucessfully sent or a reset occurred
// Currently uses a separate GPIO in OD mode that must be externally tied with timer pin.
//*****************************************************************************
inline unsigned char oneWireTarget_ODsendBit(
    OneWireTarget* ow, unsigned char bit)
{
    // Wait for timer pulse
    ow->state.lowPulseDetected = false;
    while (false == ow->state.lowPulseDetected) {
        __WFE();
    }
    if ((ow->state.tLow >= onewireResetmin_delay) &&
        (ow->state.tLow <=
            onewireResetmax_delay))  // Line has been low for too long, must be a reset condition
    {
        return 1;
    }
    if ((ow->state.tLow >= onewireODResetmin_delay) &&
        (ow->state.tLow <=
            onewireODResetmax_delay))  // Line has been low for too long, must be a reset condition
    {
        return 2;
    } else if (bit == 0)  // If a zero value, drive pin low
    {
        DL_TimerG_stopCounter(ow->config.timerInst);
        DL_GPIO_clearPins(ow->config.outputPort, ow->config.outputPin);
        delay_us(6);
        DL_GPIO_setPins(ow->config.outputPort, ow->config.outputPin);
        DL_TimerG_startCounter(ow->config.timerInst);
        delay_cycles(5);
    }
    return 0;
}

//*****************************************************************************
// Receive bit function. Reused from TIDM-1WIREEEPROM. Modified for OD
// Receives a bit from the controller device
// \return  Value of the receive bit
//*****************************************************************************
inline unsigned char oneWireTarget_ODreceiveBit(OneWireTarget* ow)
{
    unsigned char bit = 0;
    if (ow->state.tLow <= onewireODwrite1max_delay) {
        bit = 1;  // Write logic 1 detected
    }
    return bit;
}

void oneWireTarget_computeRegNumCRC(OneWireTarget* ow, uint8_t idx)
{
    ow->config.regNums[idx][7] =
        crc8((unsigned char*) ow->config.regNums[idx], 7);
}

/*
 * void oneWireTarget_ODsendPresence(void) - Used to send a presence pulse on 1-wire line
 * in response to a reset pulse.
 * Currently uses a separate GPIO in OD mode that must be externally tied with timer pin.
 *
 */

void oneWireTarget_pulseStarted(OneWireTarget* ow)
{
    if (ow->state.targetRead)  //Used to drive read data-out early for a 0
    {
        DL_GPIO_clearPins(ow->config.outputPort, ow->config.outputPin);
        ow->state.targetRead = false;
    }
}

void oneWireTarget_pulseFinished(OneWireTarget* ow)
{
    /*
             * In Capture Combined Mode the number of timer clocks that the signal
             * remained low is captured in capture compare register 1.
             */
    uint32_t lv = DL_Timer_getLoadValue(ow->config.timerInst);
    uint32_t cc = DL_Timer_getCaptureCompareValue(
        ow->config.timerInst, DL_TIMER_CC_0_INDEX);
    ow->state.tLow = lv - cc;

    if (ow->state.tLow <= onewirewrite1max_delay) {
        ow->state.targetRcvBit = 1;  // Write logic 1 detected
    } else {
        ow->state.targetRcvBit = 0;
    }
    ow->state.lowPulseDetected = true;
}
void oneWireTarget_ODsendPresence(OneWireTarget* ow)
{
    DL_TimerG_stopCounter(ow->config.timerInst);
    DL_GPIO_clearPins(ow->config.outputPort, ow->config.outputPin);
    delay_us(16);
    DL_GPIO_setPins(ow->config.outputPort, ow->config.outputPin);
    DL_TimerG_startCounter(ow->config.timerInst);
}
