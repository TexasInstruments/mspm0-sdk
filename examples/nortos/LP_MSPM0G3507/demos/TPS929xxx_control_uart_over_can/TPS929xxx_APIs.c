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

#include "ti_msp_dl_config.h"

#include "FlexWire.h"
#include "TPS929xxx_APIs.h"
#include "system_info.h"

unsigned int read_offset;

#if TPS92912X
const unsigned int bit_idx_lut[MAX_CHANNEL_CNT] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x04, 0x08};
const unsigned int data_idx_lut[MAX_CHANNEL_CNT] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01};
#elif TPS929160
const unsigned int bit_idx_lut[MAX_CHANNEL_CNT]  = {0x01, 0x02, 0x10, 0x20,
    0x01, 0x02, 0x10, 0x20, 0x01, 0x02, 0x10, 0x20, 0x01, 0x02, 0x10, 0x20};
const unsigned int data_idx_lut[MAX_CHANNEL_CNT] = {0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03};
#else
const unsigned int bit_idx_lut[MAX_CHANNEL_CNT]  = {0x01, 0x02, 0x04, 0x10,
    0x20, 0x40, 0x01, 0x02, 0x04, 0x10, 0x20, 0x40, 0x01, 0x02, 0x04, 0x10,
    0x20, 0x40, 0x01, 0x02, 0x04, 0x10, 0x20, 0x40};
const unsigned int data_idx_lut[MAX_CHANNEL_CNT] = {0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
#endif

/* Function Name: writeRegs
 *
 * Purpose: Write one of more registers.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * reg_addr    -> Address of register to write
 * data        -> Array of value to write to reg_addr
 * length      -> Number of registers to be written
 * enable      -> Need to make value 1 (TRUE) or 0 (FALSE) in register
 * writeWOread -> Do not first read the register before writing
 *
 */
void writeRegs(unsigned int dev_addr_x, unsigned int reg_addr,
    unsigned int *data, unsigned int length, unsigned int enable,
    unsigned int writeWOread)
{
    unsigned int data_internal[MAX_BURST_CNT];
    unsigned int dataLength = 0;
    unsigned int data_idx   = 0;

    if (dev_addr_x != BROADCAST_MODE) {
        if (writeWOread == FALSE) {
            dataLength = DATA_LENGTH(length);

            // Copy data to internal array as it will be needed to be updated
            for (data_idx = 0; data_idx < dataLength; data_idx++) {
                data_internal[data_idx] = data[data_idx];
            }

            // Read registers to retain old settings
            FlexRead(dev_addr_x, reg_addr, length, TRUE);
            //test
            //DL_UART_Main_transmitData(UART_Test_INST, 0x01);
            // Only write register when CRC of read command was correct
            if (rcvCrcError == FALSE) {
                if (enable == TRUE) {
                    // Add new settings to the old settings
                    for (data_idx = 0; data_idx < dataLength; data_idx++) {
                        data_internal[data_idx] |= (unsigned int)
                            ledRcvBuffer[data_idx + read_offset];
                    }
                } else {
                    // Clear new settings to the old settings
                    for (data_idx = 0; data_idx < dataLength; data_idx++) {
                        data_internal[data_idx] =
                            (unsigned int)
                                ledRcvBuffer[data_idx + read_offset] &
                            ~data_internal[data_idx];
                    }
                }
                // Write new settings
                FlexWrite(dev_addr_x, reg_addr, data_internal, length, FALSE);
                //
                // DL_UART_Main_transmitData(UART_Test_INST, 0x02);
            }
        } else {
            // Write new settings
            FlexWrite(dev_addr_x, reg_addr, data, length, FALSE);
        }
    } else {
        // Overwrite existing value of other fields
        FlexWriteBroadcast(reg_addr, data, length);
    }
}

/* Function Name: writeSingleReg
 *
 * Purpose: Write one register.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * reg_addr    -> Address of register to write
 * value       -> Value to write to reg_addr
 * enable      -> Need to make value 1 (TRUE) or 0 (FALSE) in register
 * writeWOread -> Do not first read the register before writing
 *
 */
void writeSingleReg(unsigned int dev_addr_x, unsigned int reg_addr,
    unsigned int value, unsigned int enable, unsigned int writeWOread)
{
    unsigned int data[1];
    const unsigned int length = DATA_LENGTH__1;

    data[0] = value;

    writeRegs(dev_addr_x, reg_addr, data, length, enable, writeWOread);
}

/* Function Name: checkFlag
 *
 * Purpose: Check if a flag has been set.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, return FALSE.
 * reg_addr    -> Address of register to check
 * flag        -> One or multiple flags to check in reg_addr
 *
 */
unsigned int checkFlag(
    unsigned int dev_addr_x, unsigned int reg_addr, unsigned int flag)
{
    if (dev_addr_x != BROADCAST) {
        // Read register
        FlexRead(dev_addr_x, reg_addr, NORMAL_MODE, TRUE);

        if (rcvCrcError == FALSE) {
            return (ledRcvBuffer[read_offset] & flag);
        } else {
            // When CRC error occurred return FALSE
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

#ifndef TPS92912X
/* Function Name: unlockCtrl
 *
 * Purpose: Unlock the access to the CTRL registers.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 *
 */
void unlockCtrl(unsigned int dev_addr_x)
{
    const unsigned int unlock_length = 5;
    const unsigned int ctrlgate_addr = CTRLGATE;
    unsigned int unlock_seq[]        = {0x00, 0x43, 0x4F, 0x44, 0x45};
    unsigned int idx;

    // We write one extra dummy sequence at the beginning to ensure the start status of CTRLGATE is locked
    // Unlock programming sequence
    for (idx = 0; idx < unlock_length; idx++) {
        writeSingleReg(dev_addr_x, ctrlgate_addr, unlock_seq[idx], TRUE, TRUE);
    }
}

/* Function Name: unlockEep
 *
 * Purpose: Unlock the possibility to enter Program State.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 *
 */
void unlockEep(unsigned int dev_addr_x)
{
    const unsigned int unlock_length = 7;
    const unsigned int eepgate_addr  = EEPGATE;
    unsigned int unlock_seq[] = {0xFF, 0x00, 0x04, 0x02, 0x09, 0x02, 0x09};
    unsigned int idx;

    // We write one extra dummy sequence at the beginning to ensure the start status of EEPGATE is locked
    // Unlock programming sequence
    for (idx = 0; idx < unlock_length; idx++) {
        writeSingleReg(dev_addr_x, eepgate_addr, unlock_seq[idx], TRUE, TRUE);
    }
}
#endif

/* Function Name: enableCh
 *
 * Purpose: Turn on specified channels and turn off the unspecified channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * ch_list     -> List of channels that should be enabled
 * channel_cnt -> Count of channels in the ch_list
 *
 */
void enableCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt)
{
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;
    unsigned int ch_idx   = 0;
    unsigned int data_idx = 0;

#if TPS92912X
    reg_addr = CONF_EN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = OUTEN0;
    length   = DATA_LENGTH__4;
#endif

    while (ch_idx < channel_cnt) {
        data_idx = data_idx_lut[ch_list[ch_idx]];
        data[data_idx] |= bit_idx_lut[ch_list[ch_idx]];
        ch_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, TRUE);
}

/* Function Name: enableAllCh
 *
 * Purpose: Turn on all channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 *
 */
void enableAllCh(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0xFF, 0xFF, 0xFF, 0xFF};
    unsigned int length   = DATA_LENGTH__1;

#if TPS92912X
    reg_addr = CONF_EN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = OUTEN0;
    length   = DATA_LENGTH__4;
#endif

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, TRUE);
}

/* Function Name: disableAllCh
 *
 * Purpose: Turn off all channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 *
 */
void disableAllCh(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;

#if TPS92912X
    reg_addr = CONF_EN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = OUTEN0;
    length   = DATA_LENGTH__4;
#endif

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, TRUE);
}

/* Function Name: setCh
 *
 * Purpose: Turn on specified channels and keep the other channels
 *          at their current value.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * ch_list     -> List of channels that should be enabled
 * channel_cnt -> Count of channels in the ch_list
 *
 */
void setCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt)
{
    // Loop over channel list
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;
    unsigned int ch_idx   = 0;

#if TPS92912X
    reg_addr = CONF_EN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = OUTEN0;
    length   = DATA_LENGTH__4;
#endif

    while (ch_idx < channel_cnt) {
        data_idx = data_idx_lut[ch_list[ch_idx]];
        data[data_idx] |= bit_idx_lut[ch_list[ch_idx]];
        ch_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, FALSE);
}

/* Function Name: unsetCh
 *
 * Purpose: Turn off specified channels and keep the other channels
 *          at their current value.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * ch_list     -> List of channels that should be disabled
 * channel_cnt -> Count of channels in the ch_list
 *
 */
void unsetCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt)
{
    // Loop over channel list
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;
    unsigned int ch_idx   = 0;

#if TPS92912X
    reg_addr = CONF_EN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = OUTEN0;
    length   = DATA_LENGTH__4;
#endif

    while (ch_idx < channel_cnt) {
        data_idx = data_idx_lut[ch_list[ch_idx]];
        data[data_idx] |= bit_idx_lut[ch_list[ch_idx]];
        ch_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, data, length, FALSE, FALSE);
}

/* Function Name: enableDiagCh
 *
 * Purpose: Turn diagnostics on for specified channels and turn off diagnostics
 *          for unspecified channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * ch_list     -> List of channels for which the diagnostics should be enabled
 * channel_cnt -> Count of channels in the ch_list
 *
 */
void enableDiagCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt)
{
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;
    unsigned int ch_idx   = 0;
    unsigned int data_idx = 0;

#if TPS92912X
    reg_addr = CONF_DIAGEN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = DIAGEN0;
    length   = DATA_LENGTH__4;
#endif

    while (ch_idx < channel_cnt) {
        data_idx = data_idx_lut[ch_list[ch_idx]];
        data[data_idx] |= bit_idx_lut[ch_list[ch_idx]];
        ch_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, TRUE);
}

/* Function Name: enableDiagAllCh
 *
 * Purpose: Turn diagnostics on for all channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 *
 */
void enableDiagAllCh(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0xFF, 0xFF, 0xFF, 0xFF};
    unsigned int length   = DATA_LENGTH__1;

#if TPS92912X
    reg_addr = CONF_DIAGEN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = DIAGEN0;
    length   = DATA_LENGTH__4;
#endif

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, TRUE);
}

/* Function Name: disableDiagAllCh
 *
 * Purpose: Turn diagnostics off for all channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 *
 */
void disableDiagAllCh(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;

#if TPS92912X
    reg_addr = CONF_DIAGEN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = DIAGEN0;
    length   = DATA_LENGTH__4;
#endif

    writeRegs(dev_addr_x, reg_addr, data, length, FALSE, TRUE);
}

/* Function Name: setDiagCh
 *
 * Purpose: Turn diagnostics on for specified channels and keep other channels
 *          at their current value.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * ch_list     -> List of channels for which the diagnostics should be enabled
 * channel_cnt -> Count of channels in the ch_list
 *
 */
void setDiagCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt)
{
    // Loop over channel list
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;
    unsigned int ch_idx   = 0;

#if TPS92912X
    reg_addr = CONF_DIAGEN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = DIAGEN0;
    length   = DATA_LENGTH__4;
#endif

    while (ch_idx < channel_cnt) {
        data_idx = data_idx_lut[ch_list[ch_idx]];
        data[data_idx] |= bit_idx_lut[ch_list[ch_idx]];
        ch_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, data, length, TRUE, FALSE);
}

/* Function Name: unsetDiagCh
 *
 * Purpose: Turn diagnostics off for specified channels.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, all devices are written.
 * ch_list     -> List of channels for which the diagnostics should be disabled
 * channel_cnt -> Count of channels in the ch_list
 *
 */
void unsetDiagCh(
    unsigned int dev_addr_x, unsigned int *ch_list, unsigned int channel_cnt)
{
    // Loop over channel list
    unsigned int reg_addr = 0x00;
    unsigned int data[]   = {0x00, 0x00, 0x00, 0x00};
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;
    unsigned int ch_idx   = 0;

#if TPS92912X
    reg_addr = CONF_DIAGEN0;
    length   = DATA_LENGTH__2;
#else
    reg_addr = DIAGEN0;
    length   = DATA_LENGTH__4;
#endif

    while (ch_idx < channel_cnt) {
        data_idx = data_idx_lut[ch_list[ch_idx]];
        data[data_idx] |= bit_idx_lut[ch_list[ch_idx]];
        ch_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, data, length, FALSE, FALSE);
}

/* Function Name: enableDiagMask
 *
 * Purpose: Enable masking for specified diagnostic faults.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices are written.
 * mask       -> Specify which diagnostics faults are masked
 *
 */
void enableDiagMask(unsigned int dev_addr_x, unsigned int mask)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC8;
#else
    reg_addr = DIAGMASK;
#endif

    writeSingleReg(dev_addr_x, reg_addr, mask, TRUE, TRUE);
}

/* Function Name: enableOutMask
 *
 * Purpose: Enable masking for specified OUT faults.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices are written.
 * mask       -> Specify if open, short, and/or SLS faults are masked
 *
 */
void enableOutMask(unsigned int dev_addr_x, unsigned int mask)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC8;
#else
    reg_addr = OUTMASK;
#endif

    writeSingleReg(dev_addr_x, reg_addr, mask, TRUE, TRUE);
}

/* Function Name: enableSLS
 *
 * Purpose: Enable Single-LED-Short diagnostics.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices are written.
 *
 */
void enableSLS(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC8;
    value    = CONF_SSSTART__1;
#else
    reg_addr = REFERENCE;
    value    = SLSEN_1;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, FALSE);
}

/* Function Name: disableSLS
 *
 * Purpose: Disable Single-LED-Short diagnostics.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices are written.
 *
 */
void disableSLS(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC0;
    value    = CONF_AUTOSS__1;
#else
    reg_addr = REFERENCE;
    value    = SLSEN_1;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, FALSE);
}

/* Function Name: setSLSth
 *
 * Purpose: Set the Single-LED-Short Threshold value.
 *
 * Parameters:
 * dev_addr_x    -> Flexwire address of the device.
 *                  When use BROADCAST, all devices are written.
 * threshold_val -> Value for Single-LED-Short Threshold
 * threshold_set -> Set threshold 0 or 1 (ignored for TPS92912x)
 *
 */
void setSLSth(unsigned int dev_addr_x, unsigned int threshold_val,
    unsigned int threshold_set)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC5;
#else
    if (threshold_set == 0) {
        reg_addr = SLSDAC0;
    } else {
        reg_addr = SLSDAC1;
    }
#endif

    writeSingleReg(dev_addr_x, reg_addr, threshold_val, TRUE, TRUE);
}

/* Function Name: setLowSupplyTh
 *
 * Purpose: Set the Low Supply Threshold value.
 *
 * Parameters:
 * dev_addr_x    -> Flexwire address of the device.
 *                  When use BROADCAST, all devices are written.
 * threshold_val -> Value for Low Supply Threshold
 *
 */
void setLowSupplyTh(unsigned int dev_addr_x, unsigned int threshold_val)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC2;
#else
    reg_addr = REFERENCE;
#endif

    writeSingleReg(dev_addr_x, reg_addr, threshold_val, TRUE, FALSE);
}

/* Function Name: checkErrFlag
 *
 * Purpose: Check if the ERR error flag is set.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, FALSE is returned.
 *
 */
unsigned int checkErrFlag(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int flag     = 0x00;

#if TPS92912X
    reg_addr = FLAG0;
    flag     = FLAG_ERR;
#else
    reg_addr = FLAG_ERR;
    flag     = FLAGERR;
#endif

    return checkFlag(dev_addr_x, reg_addr, flag);
}

/* Function Name: getErrFlags
 *
 * Purpose: Return all error flags in FLAG0 (TPS92912x) or FLAG_ERR (TPS929160/240).
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, FALSE is returned.
 *
 */
unsigned int getErrFlags(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int flag     = 0xFF;

#if TPS92912X
    reg_addr = FLAG0;
#else
    reg_addr = FLAG_ERR;
#endif

    return checkFlag(dev_addr_x, reg_addr, flag);
}

/* Function Name: checkPorFlag
 *
 * Purpose: Check if the POR error flag is set.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, FALSE is returned.
 *
 */
unsigned int checkPorFlag(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int flag     = FLAG_POR;

#if TPS92912X
    reg_addr = FLAG0;
#else
    reg_addr = FLAG_STATUS;
#endif

    return checkFlag(dev_addr_x, reg_addr, flag);
}

/* Function Name: checkSupFlags
 *
 * Purpose: Check if the Low Supply error flag is set.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, FALSE is returned.
 *
 */
unsigned int checkSupFlags(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int flag     = 0x00;

#if TPS92912X
    reg_addr = FLAG1;
    flag     = FLAG_ADCLOWSUP;
#else
    reg_addr = FLAG_ERR;
    flag     = FLAG_LOWSUP | FLAG_SUPUV;
#endif

    return checkFlag(dev_addr_x, reg_addr, flag);
}

/* Function Name: checkEepcrcFlag
 *
 * Purpose: Check if the EEPROM CRC error flag is set.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, FALSE is returned.
 *
 */
unsigned int checkEepcrcFlag(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int flag     = FLAG_EEPCRC;

#if TPS92912X
    reg_addr = FLAG1;
#else
    reg_addr = FLAG_ERR;
    flag     = FLAG_EEP_CRC;
#endif

    return checkFlag(dev_addr_x, reg_addr, flag);
}

/* Function Name: getADCvalue
 *
 * Purpose: Perform ADC conversion for single device. When BROADCAST is selected 0 will be returned.
 *
 * Parameters:
 * dev_addr_x  -> Flexwire address of the device.
 *                When use BROADCAST, no device is checked.
 * adc_in      -> ADC channel (CONF_ADCCH) to read.
 * convert_res -> When FALSE, return ADC_OUT digital value. When TRUE, convert result to voltage/current/degrees.
 *
 */
unsigned int getADCvalue(
    unsigned int dev_addr_x, unsigned int adc_in, unsigned int convert_res)
{
    unsigned int reg_addr = 0x00;
    unsigned int k_factor = 0x00;
    unsigned int a_factor = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC6;
#else
    reg_addr = ADCCH;
#endif

    if (dev_addr_x != BROADCAST) {
        writeSingleReg(dev_addr_x, reg_addr, adc_in, TRUE, TRUE);

#if TPS92912X
        reg_addr = FLAG1;
#else
        reg_addr = FLAG_STATUS;
#endif

        // Check if ADC is done - Need timeout??
        while (!(checkFlag(dev_addr_x, reg_addr, FLAG_ADCDONE))) {
        }

#if TPS92912X
        reg_addr = FLAG3;
#else
        reg_addr = FLAG_ADC;
#endif
        // Get result
        FlexRead(dev_addr_x, reg_addr, NORMAL_MODE, TRUE);

        if (rcvCrcError == TRUE) {
            // When CRC error occurs, return 0
            return 0;
        } else {
            // Check if conversion is required
            if (convert_res == FALSE) {
                return (unsigned int) ledRcvBuffer[read_offset];
            } else {
                switch (adc_in) {
                    case (ADCCHSEL__REF):
                        // In 100 uV
                        k_factor = 101;
                        a_factor = 70;
                        break;
                    case (ADCCHSEL__VLDO):
                        // In 1 mV
                        k_factor = 22;
                        a_factor = 47;
                        break;
                    case (ADCCHSEL__TEMPSNS):
                        // In 10 mC
#if TPS929121
                        k_factor = 246;
                        a_factor = 30470;
#else
                        k_factor = 215;
                        a_factor = 24235;
#endif
                        break;
                    case (ADCCHSEL__IREF):
                        // In 10 nA
                        k_factor = 75;
                        a_factor = 76;
                        break;
                    default:
                        // All output channels, SUPPLY, MAXOUT, and VBAT
                        // In 1 mV
#if TPS929120
                        k_factor = 80;
                        a_factor = 67;
#elif TPS929121
                        k_factor = 158;
                        a_factor = 288;
#else
                        k_factor = 161;
                        a_factor = 135;
#endif
                }
                if (adc_in == ADCCHSEL__TEMPSNS) {
                    return (
                        (k_factor * (unsigned int) ledRcvBuffer[read_offset]) -
                        a_factor);
                } else {
                    return (
                        (k_factor * (unsigned int) ledRcvBuffer[read_offset]) +
                        a_factor);
                }
            }
        }
    } else {
        return 0;
    }
}

/* Function Name: getOpenCh
 *
 * Purpose: Get the list of channels that have opens. The list
 *          is ended with 0xFF, i.e. when index 0 is 0xFF no channels
 *          have opens.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, no device is checked.
 * ch_list    -> Return value of array with channels that have opens.
 *
 */
void getOpenCh(unsigned int dev_addr_x, unsigned int *ch_list)
{
    unsigned int reg_addr    = 0x00;
    unsigned int length      = DATA_LENGTH__1;
    unsigned int ch_idx      = 0;
    unsigned int ch_list_idx = 0;

#if TPS92912X
    reg_addr = FLAG11;
    length   = DATA_LENGTH__2;
#else
    reg_addr      = FLAG_OPEN0;
    length        = DATA_LENGTH__4;
#endif

    if (dev_addr_x != BROADCAST) {
        // Read register
        FlexRead(dev_addr_x, reg_addr, length, TRUE);

        // If read resulted in CRC error do not set any channel as fail
        if (rcvCrcError == FALSE) {
            // Run over channels
            for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
                if (ledRcvBuffer[data_idx_lut[ch_idx] + read_offset] &
                    bit_idx_lut[ch_idx]) {
                    ch_list[ch_list_idx] = ch_idx;
                    ch_list_idx++;
                }
            }
        }
    }

    // Make last index 0xFF to be able to know the edge of the array
    ch_list[ch_list_idx] = 0xFF;
}

/* Function Name: getShortCh
 *
 * Purpose: Get the list of channels that have shorts. The list
 *          is ended with 0xFF, i.e. when index 0 is 0xFF no channels
 *          have shorts.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, no device is checked.
 * ch_list    -> Return value of array with channels that have shorts.
 *
 */
void getShortCh(unsigned int dev_addr_x, unsigned int *ch_list)
{
    unsigned int reg_addr    = 0x00;
    unsigned int length      = DATA_LENGTH__1;
    unsigned int ch_idx      = 0;
    unsigned int ch_list_idx = 0;

#if TPS92912X
    reg_addr = FLAG13;
    length   = DATA_LENGTH__2;
#else
    reg_addr      = FLAG_SHORT0;
    length        = DATA_LENGTH__4;
#endif

    if (dev_addr_x != BROADCAST) {
        // Read register
        FlexRead(dev_addr_x, reg_addr, length, TRUE);

        // If read resulted in CRC error do not set any channel as fail
        if (rcvCrcError == FALSE) {
            // Run over channels
            for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
                if (ledRcvBuffer[data_idx_lut[ch_idx] + read_offset] &
                    bit_idx_lut[ch_idx]) {
                    ch_list[ch_list_idx] = ch_idx;
                    ch_list_idx++;
                }
            }
        }
    }

    // Make last index 0xFF to be able to know the end of the array
    ch_list[ch_list_idx] = 0xFF;
}

/* Function Name: getSLSCh
 *
 * Purpose: Get the list of channels that have Single-LED-Short (SLS). The list
 *          is ended with 0xFF, i.e. when index 0 is 0xFF no channels have SLS.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, no device is checked.
 * ch_list    -> Return value of array with channels that have SLS.
 *
 */
void getSLSCh(unsigned int dev_addr_x, unsigned int *ch_list)
{
    unsigned int reg_addr    = 0x00;
    unsigned int length      = DATA_LENGTH__1;
    unsigned int ch_idx      = 0;
    unsigned int ch_list_idx = 0;

#if TPS92912X
    reg_addr = FLAG4;
    length   = DATA_LENGTH__2;
#else
    reg_addr      = FLAG_SLS0;
    length        = DATA_LENGTH__4;
#endif

    if (dev_addr_x != BROADCAST) {
        // Read register
        FlexRead(dev_addr_x, reg_addr, length, TRUE);

        // If read resulted in CRC error do not set any channel as fail
        if (rcvCrcError == FALSE) {
            // Run over channels
            for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
                if (ledRcvBuffer[data_idx_lut[ch_idx] + read_offset] &
                    bit_idx_lut[ch_idx]) {
                    ch_list[ch_list_idx] = ch_idx;
                    ch_list_idx++;
                }
            }
        }
    }

    // Make last index 0xFF to be able to know the edge of the array
    ch_list[ch_list_idx] = 0xFF;
}

/* Function Name: setPWMAllCh
 *
 * Purpose: Set 12-bit value of PWM for all channels.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_val    -> Array with 1 or multiple items to specify 12-bit PWM (possible values are in the range of 0x00 to 0xFFF).
 * same_val   -> Specify if all channels use the same or different value.
 *
 */
void setPWMAllCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int same_val)
{
    unsigned int reg_high_addr = 0x00;
    unsigned int reg_low_addr  = 0x00;
    unsigned int data_high[MAX_CHANNEL_CNT];
    unsigned int data_low[MAX_CHANNEL_CNT];
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;

#if TPS92912X
    unsigned int dataLength = 0;

    reg_high_addr = PWM0;
    reg_low_addr  = PWML0;
    length        = DATA_LENGTH__8;

    dataLength = DATA_LENGTH(length);
#elif TPS929160
    reg_high_addr = PWMM0;
    reg_low_addr  = PWML0;
    length        = DATA_LENGTH__16;
#else
    reg_high_addr = PWMM0;
    reg_low_addr  = PWML0;
    length        = DATA_LENGTH__24;
#endif

    for (data_idx = 0; data_idx < MAX_CHANNEL_CNT; data_idx++) {
        if (same_val == TRUE) {
            data_high[data_idx] = (pwm_val[0] & 0xFF0) >> 4;
            data_low[data_idx]  = pwm_val[0] & 0xF;
        } else {
            data_high[data_idx] = (pwm_val[data_idx] & 0xFF0) >> 4;
            data_low[data_idx]  = pwm_val[data_idx] & 0xF;
        }
    }

    writeRegs(dev_addr_x, reg_low_addr, data_low, length, TRUE, TRUE);
    writeRegs(dev_addr_x, reg_high_addr, data_high, length, TRUE, TRUE);
#if TPS92912X
    writeRegs(dev_addr_x, reg_low_addr + dataLength, &data_low[dataLength],
        DATA_LENGTH__4, TRUE, TRUE);
    writeRegs(dev_addr_x, reg_high_addr + dataLength, &data_high[dataLength],
        DATA_LENGTH__4, TRUE, TRUE);
#endif
}

/* Function Name: setPWMhighAllCh
 *
 * Purpose: Set 8-bit MSB value of PWM for all channels.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_val    -> Array with 1 or multiple items to specify 8-bit MSB PWM (possible values are in the range of 0x00 to 0xFF).
 * same_val   -> Specify if all channels use the same or different value.
 *
 */
void setPWMhighAllCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int same_val)
{
    unsigned int reg_high_addr = 0x00;
    unsigned int data_high[MAX_CHANNEL_CNT];
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;

#if TPS92912X
    unsigned int dataLength = 0;

    reg_high_addr = PWM0;
    length        = DATA_LENGTH__8;

    dataLength = DATA_LENGTH(length);
#elif TPS929160
    reg_high_addr = PWMM0;
    length        = DATA_LENGTH__16;
#else
    reg_high_addr = PWMM0;
    length        = DATA_LENGTH__24;
#endif

    for (data_idx = 0; data_idx < MAX_CHANNEL_CNT; data_idx++) {
        if (same_val == TRUE) {
            data_high[data_idx] = pwm_val[0] & 0xFF;
        } else {
            data_high[data_idx] = pwm_val[data_idx] & 0xFF;
        }
    }

    writeRegs(dev_addr_x, reg_high_addr, &data_high[0], length, TRUE, TRUE);
#if TPS92912X
    writeRegs(dev_addr_x, reg_high_addr + dataLength, &data_high[dataLength],
        DATA_LENGTH__4, TRUE, TRUE);
#endif
}

/* Function Name: setPWMlowAllCh
 *
 * Purpose: Set 4-bit LSB value of PWM for all channels.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_val    -> Array with 1 or multiple items to specify 4-bit LSB PWM (possible values are in the range of 0x00 to 0xF).
 * same_val   -> Specify if all channels use the same or different value.
 *
 */
void setPWMlowAllCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int same_val)
{
    unsigned int reg_low_addr = 0x00;
    unsigned int data_low[MAX_CHANNEL_CNT];
    unsigned int length   = DATA_LENGTH__1;
    unsigned int data_idx = 0;

#if TPS92912X
    unsigned int dataLength = 0;

    reg_low_addr = PWML0;
    length       = DATA_LENGTH__8;

    dataLength = DATA_LENGTH(length);
#elif TPS929160
    reg_low_addr  = PWML0;
    length        = DATA_LENGTH__16;
#else
    reg_low_addr  = PWML0;
    length        = DATA_LENGTH__24;
#endif

    for (data_idx = 0; data_idx < MAX_CHANNEL_CNT; data_idx++) {
        if (same_val == TRUE) {
            data_low[data_idx] = pwm_val[0] & 0x0F;
        } else {
            data_low[data_idx] = pwm_val[data_idx] & 0x0F;
        }
    }

    writeRegs(dev_addr_x, reg_low_addr, &data_low[0], length, TRUE, TRUE);
#if TPS92912X
    writeRegs(dev_addr_x, reg_low_addr + dataLength, &data_low[dataLength],
        DATA_LENGTH__4, TRUE, TRUE);
#endif
}

/* Function Name: setPWMoneCh
 *
 * Purpose: Set 12-bit value of PWM for 1 channel.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_val    -> Array with 1 item to specify 12-bit PWM (possible values are in the range of 0x00 to 0xFFF).
 * channel    -> Specify which channel to use in range from 0 to MAX_CHANNEL_CNT-1
 *
 */
void setPWMoneCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int channel)
{
    unsigned int reg_high_addr = 0x00;
    unsigned int reg_low_addr  = 0x00;
    unsigned int data_high;
    unsigned int data_low;

#if TPS92912X
    reg_high_addr = PWM0;
    reg_low_addr  = PWML0;
#else
    reg_high_addr = PWMM0;
    reg_low_addr  = PWML0;
#endif

    data_high = (pwm_val[0] & 0xFF0) >> 4;
    data_low  = pwm_val[0] & 0xF;

    writeSingleReg(dev_addr_x, reg_low_addr + channel, data_low, TRUE, TRUE);
    writeSingleReg(dev_addr_x, reg_high_addr + channel, data_high, TRUE, TRUE);
}

/* Function Name: setPWMhighOneCh
 *
 * Purpose: Set 8-bit MSB value of PWM for 1 channel.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_val    -> Array with 1 item to specify 8-bit MSB PWM (possible values are in the range of 0x00 to 0xFF).
 * channel    -> Specify which channel to use in range from 0 to MAX_CHANNEL_CNT-1
 *
 */
void setPWMhighOneCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int channel)
{
    unsigned int reg_high_addr = 0x00;
    unsigned int data_high;

#if TPS92912X
    reg_high_addr = PWM0;
#else
    reg_high_addr = PWMM0;
#endif

    data_high = pwm_val[0] & 0xFF;

    writeSingleReg(dev_addr_x, reg_high_addr + channel, data_high, TRUE, TRUE);
}

/* Function Name: setPWMlowOneCh
 *
 * Purpose: Set 4-bit LSB value of PWM for 1 channel.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_val    -> Array with 1 item to specify 4-bit LSB PWM (possible values are in the range of 0x00 to 0xF).
 * channel    -> Specify which channel to use in range from 0 to MAX_CHANNEL_CNT-1
 *
 */
void setPWMlowOneCh(
    unsigned int dev_addr_x, unsigned int *pwm_val, unsigned int channel)
{
    unsigned int reg_low_addr = 0x00;
    unsigned int data_low;

#if TPS92912X
    reg_low_addr = PWML0;
#else
    reg_low_addr  = PWML0;
#endif

    data_low = pwm_val[0] & 0x0F;

    writeSingleReg(dev_addr_x, reg_low_addr + channel, data_low, TRUE, TRUE);
}

/* Function Name: enableExpPWM
 *
 * Purpose: Enable exponential dimming curve.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 *
 */
void enableExpPWM(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value;

#if TPS92912X
    reg_addr = CONF_MISC0;
    value    = CONF_EXPEN__1;
#else
    reg_addr      = DIM;
    value         = EXPEN_1;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, FALSE);
}

/* Function Name: disableExpPWM
 *
 * Purpose: Disable exponential dimming curve.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 *
 */
void disableExpPWM(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value;

#if TPS92912X
    reg_addr = CONF_MISC0;
    value    = CONF_EXPEN__1;
#else
    reg_addr      = DIM;
    value         = EXPEN_1;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, FALSE);
}

/* Function Name: setPWMfreq
 *
 * Purpose: Set PWM dimming frequency.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * pwm_freq   -> PWM frequency to use
 *
 */
void setPWMfreq(unsigned int dev_addr_x, unsigned int pwm_freq)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC1;
#else
    reg_addr      = DIM;
#endif

    writeSingleReg(dev_addr_x, reg_addr, pwm_freq, TRUE, FALSE);
}

/* Function Name: enablePWMshare
 *
 * Purpose: Enable that all output PWM duty-cycle share the same as channel 0.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 *
 */
void enablePWMshare(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value;

#if TPS92912X
    reg_addr = CONF_MISC7;
    value    = CONF_SHAREPWM__1;
#else
    reg_addr      = PWMSHARE;
    value         = SHAREPWM_15;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, FALSE);
}

/* Function Name: disablePWMshare
 *
 * Purpose: Disable that all output PWM duty-cycle share the same as channel 0.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 *
 */
void disablePWMshare(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value;

#if TPS92912X
    reg_addr = CONF_MISC7;
    value    = CONF_SHAREPWM__1;
#else
    reg_addr      = PWMSHARE;
    value         = SHAREPWM_15;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, FALSE);
}

/* Function Name: setRefRange
 *
 * Purpose: Set reference current ratio.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * ref_range  -> Reference current ratio to use. Possible options are REFRANGE__512, REFRANGE__256, REFRANGE__128, and REFRANGE__64
 *
 */
void setRefRange(unsigned int dev_addr_x, unsigned int ref_range)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC1;
#else
    reg_addr      = REFERENCE;
#endif

    writeSingleReg(dev_addr_x, reg_addr, ref_range, TRUE, FALSE);
}

/* Function Name: setIOUTAllCh
 *
 * Purpose: Set IOUT value of all channels which can be different or same value.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * iout_val   -> Array with 1 or multiple items to specify IOUT (possible values are in the range of 0x00 to 0x3F).
 * same_val   -> Specify if all channels use the same or different value.
 *
 */
void setIOUTAllCh(
    unsigned int dev_addr_x, unsigned int *iout_val, unsigned int same_val)
{
    unsigned int reg_addr = 0x00;
    // Currently support 24 channels maximum
    unsigned int data[24];
    unsigned int length = DATA_LENGTH__1;
    int data_idx        = 0;
    int loop_idx        = 0;

#if TPS92912X
    unsigned int dataLength = 0;

    reg_addr = IOUT0;
    length   = DATA_LENGTH__8;

    dataLength = DATA_LENGTH(length);
#elif TPS929160
    reg_addr      = IOUTA0;
    length        = DATA_LENGTH__16;
#else
    reg_addr      = IOUTA0;
    length        = DATA_LENGTH__24;
#endif

    loop_idx = 0;
    data_idx = loop_idx;

    while (loop_idx < MAX_CHANNEL_CNT) {
        if (same_val == TRUE) {
            data[data_idx] = iout_val[0] & 0xFF;
        } else {
            data[data_idx] = iout_val[loop_idx] & 0xFF;
        }
        loop_idx++;
        data_idx++;
    }

    writeRegs(dev_addr_x, reg_addr, &data[0], length, TRUE, TRUE);

#if TPS92912X
    writeRegs(dev_addr_x, reg_addr + dataLength, &data[dataLength],
        DATA_LENGTH__4, TRUE, TRUE);
#endif
}

/* Function Name: setIOUToneCh
 *
 * Purpose: Set IOUT value of one channel.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be written.
 * iout_val   -> Array with 1 item to specify IOUT (possible values are in the range of 0x00 to 0x3F).
 * channel    -> Specify which channel to use in range from 0 to MAX_CHANNEL_CNT-1
 *
 */
void setIOUToneCh(
    unsigned int dev_addr_x, unsigned int *iout_val, unsigned int channel)
{
    unsigned int reg_addr = 0x00;
    unsigned int data;

#if TPS92912X
    reg_addr = IOUT0 + channel;
#else
    reg_addr      = IOUTA0 + channel;
#endif

    data = iout_val[0] & 0xFF;

    writeSingleReg(dev_addr_x, reg_addr, data, TRUE, TRUE);
}

/* Function Name: setLock
 *
 * Purpose: Lock register ranges.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be locked.
 * lock       -> Possible values depend on TPS92912x or TPS929160/240.
 *
 */
void setLock(unsigned int dev_addr_x, unsigned int lock)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_LOCK;
#else
    // This function assumes that CTRLGATE is already unlocked
    reg_addr = _LOCK;
#endif

    writeSingleReg(dev_addr_x, reg_addr, lock, TRUE, FALSE);
}

/* Function Name: clearLock
 *
 * Purpose: Unlock specified register ranges.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be unlocked.
 * lock       -> Possible values depend on TPS92912x or TPS929160/240.
 *
 */
void clearLock(unsigned int dev_addr_x, unsigned int lock)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0xFF;

#if TPS92912X
    reg_addr = CONF_LOCK;
#else
    // This function assumes that CTRLGATE is already unlocked
    reg_addr = _LOCK;
#endif
    if (dev_addr_x == BROADCAST) {
        value &= ~lock;
    } else {
        value = lock;
    }
    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, FALSE);
}

/* Function Name: clearLockAll
 *
 * Purpose: Clear all locked registers.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will be unlocked.
 *
 */
void clearLockAll(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0xFF;

#if TPS92912X
    reg_addr = CONF_LOCK;
    value    = CLRLOCK | CONFLOCK | IOUTLOCK | PWMLOCK;
#else
    // Need to unlock LOCK register
    unlockCtrl(dev_addr_x);

    reg_addr = _LOCK;
    value    = BRTLOCK | CONFLOCK | IOUTLOCK;
#endif
    if (dev_addr_x == BROADCAST) {
        value &= ~value;
    }

    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, FALSE);
}

/* Function Name: setClr
 *
 * Purpose: Write CLR register with specified value(s). Normally use to clear
 *          Fail Safe, Fault, and/or POR.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will load default values.
 * clr_val    -> Values that should be cleared; possible options are CLRFS, CLRFAULT, and/or CLRPOR.
 *
 */
void setClr(unsigned int dev_addr_x, unsigned int clr_val)
{
    unsigned int reg_addr = 0x00;

    reg_addr = CLR;

#if TPS92912X
    unsigned int clrLockSet = 0x00;

    if (dev_addr_x != BROADCAST_MODE) {
        // Check if CLR register was locked
        FlexRead(dev_addr_x, CONF_LOCK, DATA_LENGTH__1, TRUE);
        if (rcvCrcError == FALSE) {
            clrLockSet = ledRcvBuffer[read_offset] & CLRLOCK;
        } else {
            // If read CRC was not correct, assume that CLRLOCK was set
            clrLockSet = CLRLOCK;
        }

        if (clrLockSet) {
            clearLock(dev_addr_x, CLRLOCK);
        }
    }
#endif

    writeSingleReg(dev_addr_x, reg_addr, clr_val, FALSE, TRUE);

#if TPS92912X
    if (dev_addr_x != BROADCAST_MODE) {
        // If CLR was locked, lock it again
        if (clrLockSet) {
            setLock(dev_addr_x, CLRLOCK);
        }
    }
#endif
}

/* Function Name: defaultReg
 *
 * Purpose: Load the register settings to default values.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will load default values.
 */
void defaultReg(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

#if TPS92912X
    unsigned int clrLockSet = 0x00;

    reg_addr = CLR;
    value    = CLR_REG__1;
#else
    // Need to unlock CLRREG register
    unlockCtrl(dev_addr_x);

    reg_addr = CLRREG;
    value    = REGDEFAULT_1;
#endif

#if TPS92912X
    if (dev_addr_x != BROADCAST_MODE) {
        // Check if CLR register was locked
        FlexRead(dev_addr_x, CONF_LOCK, DATA_LENGTH__1, TRUE);
        if (rcvCrcError == FALSE) {
            clrLockSet = ledRcvBuffer[read_offset] & CLRLOCK;
        } else {
            // If read CRC was not correct, assume that CLRLOCK was set
            clrLockSet = CLRLOCK;
        }

        if (clrLockSet) {
            clearLock(dev_addr_x, CLRLOCK);
        }
    }
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, TRUE);

#if TPS92912X
    if (dev_addr_x != BROADCAST_MODE) {
        // If CLR was locked, lock it again
        if (clrLockSet) {
            setLock(dev_addr_x, CLRLOCK);
        }
    }
#endif
}

/* Function Name: enableEepMode
 *
 * Purpose: Bring the device from Normal State to Program State.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will go to Program State.
 */
void enableEepMode(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC7;
    value    = CONF_EEPMODE__1;
    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, TRUE);
#endif

#if TPS92912X
    reg_addr = CONF_MISC6;
    value    = CONF_STAYINEEP__1;
#else
    // This function assumes that EEPGATE is already unlocked
    reg_addr = EEP;
    value    = EEPMODE_1;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, TRUE);
}

/* Function Name: forceFailSafe
 *
 * Purpose: Force the device for Fail Safe state.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will go to Fail Safe state.
 */
void forceFailSafe(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = CONF_FORCEFS__1;

#if TPS92912X
    reg_addr = CLR;
#else
    reg_addr = DEBUG;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, FALSE);
}

/* Function Name: forceErr
 *
 * Purpose: Force the ERR pin to pulse down for 50us.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will report error.
 */
void forceErr(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = CONF_FORCEERR__1;

#if TPS92912X
    reg_addr = CLR;
#else
    reg_addr = DEBUG;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, FALSE);
}

/* Function Name: disableEepMode
 *
 * Purpose: Leave Program State and go back to Normal State.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will go back to normal state.
 */
void disableEepMode(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC6;
    value    = CONF_STAYINEEP__1;
#else
    // This function assumes that EEPGATE is already unlocked
    reg_addr = EEP;
    value    = EEPMODE_1;
#endif

    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, TRUE);
}

/* Function Name: progEepStart
 *
 * Purpose: Start EEPROM programming.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, all devices will start programming.
 */
void progEepStart(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;

#if TPS92912X
    reg_addr = CONF_MISC8;
#else
    // This function assumes that EEPGATE is already unlocked
    reg_addr = EEP;
#endif

    writeSingleReg(dev_addr_x, reg_addr, EEPPROG, TRUE, TRUE);
}

/* Function Name: checkEepProgReady
 *
 * Purpose: Check if EEPROM programming is ready.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, automatically return FALSE.
 */
unsigned int checkEepProgReady(unsigned int dev_addr_x)
{
    unsigned int reg_addr = 0x00;
    unsigned int flag     = 0x00;

#if TPS92912X
    reg_addr = FLAG1;
    flag     = FLAG_PROGREADY;
#else
    reg_addr = FLAG_STATUS;
    flag     = FLAG_PROGDONE;
#endif

    return checkFlag(dev_addr_x, reg_addr, flag);
}

/* Function Name: enableEepSR
 *
 * Purpose: Enable access to the EEPROM shadow registers.
 *          Only for TPS92912x
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, enable all devices.
 */
void enableEepSR(unsigned int dev_addr_x)
{
#if TPS92912X
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

    reg_addr = CONF_MISC7;
    value    = CONF_READSHADOW__1;
    writeSingleReg(dev_addr_x, reg_addr, value, TRUE, FALSE);
#endif
}

/* Function Name: disableEepSR
 *
 * Purpose: Disable access to the EEPROM shadow registers.
 *          Only for TPS92912x
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, disable all devices.
 */
void disableEepSR(unsigned int dev_addr_x)
{
#if TPS92912X
    unsigned int reg_addr = 0x00;
    unsigned int value    = 0x00;

    reg_addr = CONF_MISC7;
    value    = CONF_READSHADOW__1;
    writeSingleReg(dev_addr_x, reg_addr, value, FALSE, FALSE);
#endif
}

/* Function Name: LED_Update_Chip_Status
 *
 * Purpose: Update the chip status struct for one device on the FlexWire bus.
 *
 * Parameters:
 * dev_addr_x -> Flexwire address of the device.
 *               When use BROADCAST, no device is updated.
 */
void LED_Update_Chip_Status(unsigned int dev_addr_x)
{
    unsigned int err_flags;
    unsigned int failing_ch_list[MAX_CHANNEL_CNT + 1];
    unsigned int ch_idx;

    if (dev_addr_x != BROADCAST) {
        // Clear all channel fails
        for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
            chip_status[dev_addr_x].OPEN_channels[ch_idx]  = 0U;
            chip_status[dev_addr_x].SHORT_channels[ch_idx] = 0U;
            chip_status[dev_addr_x].SLS_channels[ch_idx]   = 0U;
        }
        chip_status[dev_addr_x].OUT_flag = 0U;
        chip_status[dev_addr_x].EEPCRC   = 0U;
        chip_status[dev_addr_x].TSD      = 0U;
        chip_status[dev_addr_x].PRETSD   = 0U;
        chip_status[dev_addr_x].REF      = 0U;
        chip_status[dev_addr_x].LOWSUP   = 0U;
        chip_status[dev_addr_x].POR      = 0U;
#ifndef TPS92912X
        chip_status[dev_addr_x].SUPUV   = 0U;
        chip_status[dev_addr_x].VBAT_mV = 0U;
#endif
        chip_status[dev_addr_x].VSUPPLY_mV   = 0U;
        chip_status[dev_addr_x].VLDO_mV      = 0U;
        chip_status[dev_addr_x].TEMPSNS_10mC = 0U;
        chip_status[dev_addr_x].VREF_100uV   = 0U;
        chip_status[dev_addr_x].IREF_10nA    = 0U;

        if (checkErrFlag(dev_addr_x)) {
            err_flags = getErrFlags(dev_addr_x);

            // Run over all errors
            // Check OUT errors
            //
            // failing_ch_list contains the list of channels that fail the particular
            // condition (open, short, single-led-open) with channel numbers ranging from
            // 0 to MAX_CHANNEL_CNT. The end of the list is indicated with 0xFF (255 in decimal).
            //
            if (err_flags & FLAG_OUT) {
                chip_status[dev_addr_x].OUT_flag = 1U;
                getOpenCh(dev_addr_x, failing_ch_list);
                ch_idx = 0;
                while (failing_ch_list[ch_idx] < 255) {
                    chip_status[dev_addr_x]
                        .OPEN_channels[failing_ch_list[ch_idx]] = 1U;
                    ch_idx++;
                }
                getShortCh(dev_addr_x, failing_ch_list);
                ch_idx = 0;
                while (failing_ch_list[ch_idx] < 255) {
                    chip_status[dev_addr_x]
                        .SHORT_channels[failing_ch_list[ch_idx]] = 1U;
                    ch_idx++;
                }
                getSLSCh(dev_addr_x, failing_ch_list);
                ch_idx = 0;
                while (failing_ch_list[ch_idx] < 255) {
#if TPS92912X
                    if (chip_status[dev_addr_x]
                            .SHORT_channels[failing_ch_list[ch_idx]]) {
                        chip_status[dev_addr_x]
                            .SLS_channels[failing_ch_list[ch_idx]] = 0U;
                    } else {
                        chip_status[dev_addr_x]
                            .SLS_channels[failing_ch_list[ch_idx]] = 1U;
                    }
#else
                    chip_status[dev_addr_x]
                        .SLS_channels[failing_ch_list[ch_idx]] = 1U;
#endif
                    ch_idx++;
                }
            }

            // Check REF errors
            if (err_flags & FLAG_REF) {
                chip_status[dev_addr_x].REF = 1U;
                //
                // When conversion is TRUE, the result is *100 uV
                //
                chip_status[dev_addr_x].VREF_100uV =
                    getADCvalue(dev_addr_x, ADCCHSEL__REF, TRUE);
                //
                // When conversion is TRUE, the result is *10 nA
                //
                chip_status[dev_addr_x].IREF_10nA =
                    getADCvalue(dev_addr_x, ADCCHSEL__IREF, TRUE);
            }

            // Check (pre-) Thermal Shutdown errors
            if ((err_flags & FLAG_PRETSD) | (err_flags & FLAG_TSD)) {
                //
                // When conversion is TRUE, the result is *10 mC
                //
                chip_status[dev_addr_x].TEMPSNS_10mC =
                    getADCvalue(dev_addr_x, ADCCHSEL__TEMPSNS, TRUE);
                if (err_flags & FLAG_PRETSD) {
                    chip_status[dev_addr_x].PRETSD = 1U;
                }
                if (err_flags & FLAG_TSD) {
                    chip_status[dev_addr_x].TSD = 1U;
                }
            }

            // POR fault --> In TPS929240/160 is different register
            // TPS929120/121 --> SUPPLY or LDO
            // TPS929240/160 --> VBAT or LDO
            if (checkPorFlag(dev_addr_x)) {
                chip_status[dev_addr_x].POR = 1U;
                //
                // When conversion is TRUE, the result is *1 mV
                //
                chip_status[dev_addr_x].VLDO_mV =
                    getADCvalue(dev_addr_x, ADCCHSEL__VLDO, TRUE);
#if TPS92912X
                //
                // When conversion is TRUE, the result is *1 mV
                //
                chip_status[dev_addr_x].VSUPPLY_mV =
                    getADCvalue(dev_addr_x, ADCCHSEL__SUPPLY, TRUE);
#else
                //
                // When conversion is TRUE, the result is *1 mV
                //
                chip_status[dev_addr_x].VBAT_mV =
                    getADCvalue(dev_addr_x, ADCCHSEL__VBAT, TRUE);
#endif
            }

            // Check low supply error and supply undervoltage error
            if (checkSupFlags(dev_addr_x)) {
                //
                // When conversion is TRUE, the result is *1 mV
                //
                chip_status[dev_addr_x].VSUPPLY_mV =
                    getADCvalue(dev_addr_x, ADCCHSEL__SUPPLY, TRUE);
#if TPS92912X
                chip_status[dev_addr_x].LOWSUP = 1U;
#else
                if (err_flags & FLAG_LOWSUP) {
                    chip_status[dev_addr_x].LOWSUP = 1U;
                }
                if (err_flags & FLAG_SUPUV) {
                    chip_status[dev_addr_x].SUPUV = 1U;
                }
#endif
            }

            // Check EEPROM CRC flag
            if (checkEepcrcFlag(dev_addr_x)) {
                chip_status[dev_addr_x].EEPCRC = 1U;
            }
        }
    }
}
