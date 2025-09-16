/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#ifndef SMARTRF_SETTINGS_H
#define SMARTRF_SETTINGS_H

#include "spi_cc11xL.h"

// Address Config = No address check
// Base Frequency = 867.999939
// CRC Autoflush = false
// CRC Enable = true
// Carrier Frequency = 867.999939
// Channel Spacing = 199.951172
// Data Format = Normal mode
// Data Rate = 38.3835
// Deviation = 20.629883
// Device Address = 0
// Manchester Enable = false
// Modulated = true
// Modulation Format = GFSK
// Packet Length = 255
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word
// Preamble Count = 4
// RX Filter BW = 101.562500
// Sync Word Qualifier Mode = 30/32 sync word bits detected
// TX Power = 0
// PA table
#define PA_TABLE {0x50,0x00}

// Address Config = No address check
// Base Frequency = 867.999939
// CRC Autoflush = false
// CRC Enable = true
// Carrier Frequency = 867.999939
// Channel Spacing = 199.951172
// Data Format = Normal mode
// Data Rate = 38.3835
// Deviation = 20.629883
// Device Address = 0
// Manchester Enable = false
// Modulated = true
// Modulation Format = GFSK
// Packet Length = 255
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word
// Preamble Count = 4
// RX Filter BW = 101.562500
// Sync Word Qualifier Mode = 30/32 sync word bits detected
// TX Power = 0
// PA table
#define PA_TABLE {0x50,0x00}

static const registerSetting_t preferredSettings[]=
{
  {CC11xL_IOCFG0,           0x06},
  {CC11xL_FIFOTHR,          0x47},
  {CC11xL_PKTCTRL0,         0x05},
  {CC11xL_FSCTRL1,          0x06},
  {CC11xL_FREQ2,            0x21},
  {CC11xL_FREQ1,            0x62},
  {CC11xL_FREQ0,            0x76},
  {CC11xL_MDMCFG4,          0xCA},
  {CC11xL_MDMCFG3,          0x83},
  {CC11xL_MDMCFG2,          0x13},
  {CC11xL_DEVIATN,          0x35},
  {CC11xL_MCSM0,            0x18},
  {CC11xL_FOCCFG,           0x16},
  {CC11xL_AGCCTRL2,         0x43},
  {CC11xL_RESERVED_0X20,    0xFB},
  {CC11xL_FSCAL3,           0xE9},
  {CC11xL_FSCAL2,           0x2A},
  {CC11xL_FSCAL1,           0x00},
  {CC11xL_FSCAL0,           0x1F},
  {CC11xL_TEST2,            0x81},
  {CC11xL_TEST1,            0x35},
  {CC11xL_TEST0,            0x09},
};


#endif
