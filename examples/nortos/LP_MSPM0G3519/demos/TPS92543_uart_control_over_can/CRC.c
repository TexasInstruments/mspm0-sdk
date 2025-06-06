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

#include "CRC.h"

//CRC-16-IBM for 32-Bytes of Data
uint16_t CRC_16_IBM(uint8_t commandFrame_withoutCRC[], uint8_t byteLength)
{
    uint16_t crc = 0;
    uint8_t i, j;
    for (i = 0; i < byteLength; i++) {
        crc ^= commandFrame_withoutCRC[i];
        for (j = 0; j < 8; j++)
            if (crc & 0x0001) {
                // Right shift 1 bit and do the XOR operation with 0xA001
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
    }
    return crc;
}

//CRC-FlexRay Frame for 64-Bytes of Data
uint32_t CRC_24_IBM(uint8_t commandFrame_withoutCRC[], uint8_t byteLength)
{
    uint32_t crc = 0x5D3B7F;
    uint8_t i, j;
    for (i = 0; i < byteLength; i++) {
        crc ^= commandFrame_withoutCRC[i];
        for (j = 0; j < 8; j++)
            if (crc & 0x000001) {
                // Right shift 1 bit and do the XOR operation with 0xD3B6BA
                crc = (crc >> 1) ^ 0xD3B6BA;
            } else {
                crc = crc >> 1;
            }
    }
    return crc;
}
