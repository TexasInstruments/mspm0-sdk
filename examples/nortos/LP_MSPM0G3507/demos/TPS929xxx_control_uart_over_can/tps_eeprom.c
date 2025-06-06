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
#include <stdint.h>

#include "mspm0_timer.h"

#include "FlexWire.h"
#include "TPS929xxx_APIs.h"
#include "system_info.h"
#include "tps_eeprom.h"
#include "tps_eeprom_data.h"
#include "tps_eeprom_default.h"

unsigned int programCore(unsigned int *dev_idx, unsigned int regCnt,
    struct eepromField *eeprom_def, unsigned int useRefPin)
{
    unsigned int idx = 0;
    unsigned int result;
    unsigned int dev_idx_temp;

    if ((useRefPin == FALSE) | (*dev_idx == BROADCAST)) {
        dev_idx_temp = *dev_idx;
    } else {
        dev_idx_temp = DEVICE_ADDR__0;
    }

    enableEepMode(dev_idx_temp);

    // Write all EEPROM registers
    for (idx = 0; idx < regCnt; idx++) {
        writeSingleReg(dev_idx_temp, eeprom_def[idx].Addr,
            eeprom_def[idx].Data, TRUE, TRUE);
        // For TPS92912x the device address will only be updated after a power cycle
#ifndef TPS92912X
        if (eeprom_def[idx].Addr == FLEXWIRE1) {
            if (useRefPin == FALSE) {
                if (dev_idx_temp != BROADCAST) {
                    // internal address is enabled
                    if (eeprom_def[idx].Data & INTADDR_1) {
                        dev_idx_temp = eeprom_def[idx].Data & 0x0F;
                        // Check if register was written correctly
                        FlexRead(dev_idx_temp, eeprom_def[idx].Addr,
                            NORMAL_MODE, TRUE);
                    }
#if TPS929240
                    else {
                        // Clear MSB of temp address and set to the current data
                        dev_idx_temp &= 0x07;
                        dev_idx_temp |=
                            (eeprom_def[idx].Data & DEVICE_ADDR__8);
                        // Check if register was written correctly
                        FlexRead(dev_idx_temp, eeprom_def[idx].Addr,
                            NORMAL_MODE, TRUE);
                    }
#endif
                    // Update device address
                    *dev_idx = dev_idx_temp;
                }
            } else {
                if (dev_idx_temp != BROADCAST) {
                    // Only update device address and not the temp address
                    if (eeprom_def[idx].Data & INTADDR_1) {
                        *dev_idx = eeprom_def[idx].Data & 0x0F;
                    }
#if TPS929240
                    else {
                        // Clear MSB of address and set to the current data
                        *dev_idx &= 0x07;
                        *dev_idx |= (eeprom_def[idx].Data & DEVICE_ADDR__8);
                    }
#endif
                }
            }
        }
#endif
    }

    // Start EEPROM programming
    progEepStart(dev_idx_temp);

    delay(25000);  //delay 200ms

    if (dev_idx_temp == BROADCAST) {
#if TPS92912X
        result = FLAG_PROGREADY;
#else
        result = FLAG_PROGDONE;
#endif
    } else {
        result = checkEepProgReady(dev_idx_temp);
    }

    // Check if programming is done and return result
    return result;
}

unsigned int progEEPROMdata(unsigned int *dev_addr_x, unsigned int useRefPin,
    unsigned int regCnt, struct eepromField *eeprom_def)
{
    unsigned int idx = 0;
    // We write one extra dummy sequence at the beginning to ensure the start status is locked
    const unsigned int unlock_length = 7;
    unsigned int result              = 0;
    unsigned int dev_addr_temp;
#if TPS92912X
    unsigned int unlock_seq_ref[] = {0xFF, 0x09, 0x02, 0x09, 0x01, 0x02, 0x00};
    unsigned int unlock_seq_dev[] = {0xFF, 0x00, 0x02, 0x01, 0x09, 0x02, 0x09};
    const unsigned int eepgate_addr = CONF_MISC9;
#else
    unsigned int unlock_seq_ref[] = {0xFF, 0x09, 0x02, 0x09, 0x02, 0x04, 0x00};
    unsigned int unlock_seq_dev[] = {0xFF, 0x00, 0x04, 0x02, 0x09, 0x02, 0x09};
    const unsigned int eepgate_addr = EEPGATE;
#endif

    if ((useRefPin == FALSE) | (*dev_addr_x == BROADCAST)) {
        dev_addr_temp = *dev_addr_x;
    } else {
        dev_addr_temp = DEVICE_ADDR__0;
    }

    // It is assumed that REF-pin for selected device is already pulled high when this function is called
    //
    // When REF-pin is used, 2 steps will be skipped and need to be executed after end of this function
    //    - Write 0h to CONF_STAYINEEP to Normal mode
    //    - set 1 to CLR_REG

    if (useRefPin == FALSE) {
        // Clear POR, FAULT, Fail-safe for device
        setClr(dev_addr_temp, CLRFS | CLRFAULT | CLRPOR);

        // Unlock programming sequence
        for (idx = 0; idx < unlock_length; idx++) {
            writeSingleReg(
                dev_addr_temp, eepgate_addr, unlock_seq_dev[idx], TRUE, TRUE);
        }
    } else {
        for (idx = 0; idx < unlock_length; idx++) {
            writeSingleReg(
                dev_addr_temp, eepgate_addr, unlock_seq_ref[idx], TRUE, TRUE);
        }
    }

    result = programCore(dev_addr_x, regCnt, eeprom_def, useRefPin);

#ifndef TPS92912X
    if ((useRefPin == FALSE) | (*dev_addr_x == BROADCAST)) {
        dev_addr_temp = *dev_addr_x;
    } else {
        dev_addr_temp = DEVICE_ADDR__0;
    }
#endif

    // Disable EEPROM mode
    disableEepMode(dev_addr_temp);

    // Set CLR_REG
    defaultReg(dev_addr_temp);

#if TPS92912X
    if ((useRefPin == FALSE) | (*dev_addr_x == BROADCAST)) {
        dev_addr_temp = *dev_addr_x;
    } else {
        dev_addr_temp = DEVICE_ADDR__0;
    }
#endif

    if (useRefPin == FALSE) {
        // Unlock device
#if TPS92912X
        clearLock(dev_addr_temp, CLRLOCK | CONFLOCK | IOUTLOCK | PWMLOCK);
#else
        clearLock(dev_addr_temp, BRTLOCK | CONFLOCK | IOUTLOCK);
#endif

        // Clear POR
        setClr(dev_addr_temp, CLRPOR);
    }

    return result;
}

unsigned int programEEPROM(unsigned int *dev_addr_x, unsigned int useRefPin)
{
    unsigned int result = 0;

    result = progEEPROMdata(
        dev_addr_x, useRefPin, (unsigned int) EEPROM_REGS, eeprom);

    return result;
}

unsigned int programDefaultEEPROM(
    unsigned int *dev_addr_x, unsigned int useRefPin)
{
    unsigned int result = 0;

    result = progEEPROMdata(dev_addr_x, useRefPin,
        (unsigned int) EEPROM_DEFAULT_REGS, eeprom_default);

    return result;
}

void readEEPROM(
    unsigned int dev_addr_x, unsigned int *rcvBytes, unsigned int shadowReg)
{
    unsigned int idx = 0;

    // If broadcast mode is used, skip reading
    if (dev_addr_x != BROADCAST) {
        if (shadowReg == TRUE) {
            // This will only work for TPS929120/1; for TPS929160 and TPS929240 nothing will happen
            enableEepSR(dev_addr_x);
        }

        // Read back EEPROM (shadowReg = FALSE) or shadow register of EEPROM (shadowReg = TRUE)
        for (idx = 0; idx < EEPROM_DEFAULT_REGS; idx++) {
            FlexRead(dev_addr_x, eeprom_default[idx].Addr, NORMAL_MODE, FALSE);
            rcvBytes[idx] = ledRcvBuffer[read_offset];
        }

        if (shadowReg == TRUE) {
            // This will only work for TPS929120/1; for TPS929160 and TPS929240 nothing will happen
            disableEepSR(dev_addr_x);
        }
    }
}
