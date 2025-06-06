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

#include "CRC_LUT.h"

// As UART transmit from LSB to MSB, invert the polynomial 0x31 (0011 0001) to 0x8C (1000 1100)
#define polynomialINV 0x8C
#define LSB 0x01

uint32_t crcArray[256];

// Calculate and store the CRC of data from 0x00 to 0xFF
void crcInitial(void)
{
    uint32_t k, j;
    uint32_t remainder;

    for (k = 0; k < 256; k++) {
        remainder = k;
        for (j = 8; j > 0; j--) {
            if (remainder & LSB) {
                // Right shift 1 bit and do the XOR operation with 0x8C
                remainder = (remainder >> 1) ^ polynomialINV;
            } else {
                remainder = remainder >> 1;
            }
        }
        crcArray[k] = remainder;
    }
}

// Calculate CRC of command frame
uint32_t CRC_LUT(uint32_t commandFrame_withoutCRC[], uint32_t byteLength)
{
    uint32_t k;
    uint32_t remainder, tempData;

    // Assign the initial value 0xFF
    remainder = 0xFF;

    // The first SYNC byte not engage CRC calculation
    for (k = 1; k < byteLength; k++) {
        // Input data byte XOR remainder
        tempData = remainder ^ commandFrame_withoutCRC[k];
        // Use tempData as the index to retrieve its CRC from crcArray
        remainder = crcArray[tempData];
    }

    // We have to reverse the final remainder to get the CRC value, for example, if final remainder = 0010 1100, we need to reverse it to 0011 0100
    remainder = ((remainder & 0x80) >> 7) + ((remainder & 0x40) >> 5) +
                ((remainder & 0x20) >> 3) + ((remainder & 0x10) >> 1) +
                ((remainder & 0x08) << 1) + ((remainder & 0x04) << 3) +
                ((remainder & 0x02) << 5) + ((remainder & 0x01) << 7);
    return remainder;
}
