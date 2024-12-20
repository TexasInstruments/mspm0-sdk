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

#include <stdint.h>

#define App1_SIZE   1208

const uint32_t App1_Addr[] = {
    0x1810,
    0x1800,
};

const uint32_t App1_Size[] = {
    1192,
    16,
};

const uint8_t App1_0[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5B,0x19,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5B,0x19,0x00,0x00,0x5B,0x19,0x00,0x00,
0x5B,0x19,0x00,0x00,0x01,0x1C,0x00,0x00,0x5B,0x19,0x00,0x00,0x00,0x00,0x00,0x00,
0x5B,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x5B,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5B,0x19,0x00,0x00,
0x5B,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x5B,0x19,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xD5,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5B,0x19,0x00,0x00,
0x00,0x2A,0x49,0xD0,0x84,0x46,0x8B,0x07,0x1C,0xD1,0x83,0x07,0x22,0xD1,0x10,0x2A,
0x08,0xD3,0x70,0xB4,0x10,0x3A,0x78,0xC9,0x78,0xC0,0x10,0x3A,0xFB,0xD2,0x70,0xBC,
0x10,0x32,0x38,0xD0,0x04,0x2A,0x2C,0xD3,0x08,0x2A,0x05,0xD3,0x0C,0x2A,0x01,0xD3,
0x08,0xC9,0x08,0xC0,0x08,0xC9,0x08,0xC0,0x08,0xC9,0x08,0xC0,0x92,0x07,0x2A,0xD0,
0x92,0x0F,0x22,0xE0,0x0B,0x78,0x03,0x70,0x49,0x1C,0x40,0x1C,0x52,0x1E,0x22,0xD0,
0x8B,0x07,0xF7,0xD1,0xC3,0x07,0x14,0xD1,0x83,0x07,0xD8,0xD0,0x12,0x1F,0x12,0xD3,
0x08,0xC9,0x03,0x80,0x1B,0x0C,0x43,0x80,0x00,0x1D,0x12,0x1F,0xF8,0xD2,0x0A,0xE0,
0x08,0xC9,0x03,0x70,0x1B,0x0A,0x43,0x70,0x1B,0x0A,0x83,0x70,0x1B,0x0A,0xC3,0x70,
0x00,0x1D,0x12,0x1F,0xF4,0xD2,0x12,0x1D,0x05,0xD0,0x0B,0x78,0x03,0x70,0x49,0x1C,
0x40,0x1C,0x52,0x1E,0xF9,0xD1,0x60,0x46,0x70,0x47,0xFE,0xE7,0xF0,0xB5,0x44,0x1C,
0x00,0x78,0x00,0x23,0x08,0x2B,0x30,0xD0,0x25,0x78,0xC6,0x07,0x0F,0xD1,0x66,0x78,
0x2D,0x01,0x37,0x09,0x7F,0x19,0x0F,0x25,0x35,0x40,0xED,0x1C,0x12,0x2D,0x0B,0xD1,
0x02,0x25,0x66,0x57,0xF5,0xB2,0x00,0x2E,0x08,0xD4,0xE6,0x1C,0x0C,0xE0,0x64,0x1C,
0x4E,0x1C,0x0D,0x70,0x31,0x46,0x15,0xE0,0xA6,0x1C,0x06,0xE0,0x26,0x1D,0xE4,0x78,
0x7F,0x22,0x15,0x40,0xE2,0x01,0x55,0x19,0x12,0x35,0x09,0x4A,0x97,0x42,0x0E,0xD0,
0xFC,0x43,0x00,0x2D,0x05,0xD0,0x0A,0x5D,0x4F,0x1C,0x0A,0x70,0x6D,0x1E,0x39,0x46,
0xF7,0xE7,0x34,0x46,0x40,0x08,0x5B,0x1C,0xCC,0xE7,0x20,0x46,0xC7,0xE7,0xF0,0xBD,
0xFF,0x0F,0x00,0x00,0x15,0x48,0x00,0x68,0xC0,0xB2,0x13,0x28,0x24,0xD1,0x14,0x48,
0x01,0x68,0x09,0x07,0x11,0xD0,0x13,0x49,0x02,0x46,0x10,0x3A,0x13,0x68,0x0B,0x70,
0x03,0x68,0x1B,0x07,0x09,0xD0,0x13,0x68,0x0B,0x70,0x03,0x68,0x1B,0x07,0x04,0xD0,
0x12,0x68,0x0A,0x70,0x02,0x68,0x12,0x07,0xEE,0xD1,0x0A,0x48,0x00,0x78,0x22,0x28,
0x0A,0xD1,0x09,0x48,0x01,0x68,0x01,0x22,0x11,0x43,0xFE,0x23,0x99,0x43,0x01,0x60,
0x06,0x48,0x02,0x60,0x06,0x49,0x41,0x60,0x70,0x47,0xC0,0x46,0x20,0x10,0x0F,0x40,
0x70,0x12,0x0F,0x40,0x00,0x00,0x00,0x20,0x00,0x04,0x0B,0x40,0x00,0x03,0x0B,0x40,
0x01,0x00,0x00,0xE4,0x10,0xB5,0x14,0x49,0x14,0x48,0x00,0xF0,0xC5,0xF8,0x14,0x48,
0x01,0x68,0x03,0x22,0x52,0x02,0x0A,0x43,0x02,0x60,0x02,0x68,0x01,0x21,0x0B,0x02,
0x13,0x43,0x03,0x60,0x02,0x6D,0x0F,0x4B,0x13,0x40,0x48,0x33,0x03,0x65,0xC2,0x6E,
0x07,0x23,0x9A,0x43,0xC2,0x66,0xC2,0x6E,0x1B,0x02,0x9A,0x43,0xC2,0x66,0x82,0x6D,
0x04,0x23,0x13,0x43,0x83,0x65,0x08,0x4A,0x13,0x68,0x8C,0x04,0x1C,0x43,0x14,0x60,
0x82,0x6D,0x0A,0x43,0x82,0x65,0x10,0xBD,0x98,0x1C,0x00,0x00,0x00,0x00,0x0F,0x40,
0x00,0x12,0x0F,0x40,0x00,0xFC,0xFF,0xFF,0x28,0x10,0x0F,0x40,0x11,0x49,0x12,0x48,
0x08,0x60,0x40,0x1C,0xC8,0x62,0x0A,0x68,0x01,0x20,0x43,0x06,0x1A,0x43,0x0A,0x60,
0xCA,0x6A,0x1A,0x43,0xCA,0x62,0x0D,0x4A,0x0A,0x64,0x81,0x22,0x8A,0x65,0x81,0x05,
0x0B,0x4A,0x11,0x60,0x11,0x64,0x0B,0x49,0x0A,0x68,0x02,0x23,0x13,0x43,0x0B,0x60,
0x09,0x49,0x0A,0x6A,0x00,0x04,0x02,0x43,0x0A,0x62,0x0A,0x68,0x02,0x43,0x0A,0x60,
0x70,0x47,0xC0,0x46,0x04,0x80,0x42,0x40,0x83,0x00,0x04,0x00,0x81,0x00,0x06,0x00,
0x90,0x12,0x0A,0x40,0xA0,0x13,0x0A,0x40,0x28,0x10,0x0A,0x40,0x0B,0x4D,0x0C,0x4C,
0x01,0x27,0xBE,0x05,0x00,0xF0,0x9E,0xF8,0x0A,0x48,0x02,0x21,0x01,0x60,0x39,0x06,
0x01,0x60,0x20,0x46,0x00,0xF0,0xA9,0xF8,0x2E,0x60,0x20,0x46,0x00,0xF0,0xA5,0xF8,
0x2E,0x60,0x20,0x46,0x00,0xF0,0xA1,0xF8,0x2E,0x60,0xF2,0xE7,0xB0,0x12,0x0A,0x40,
0x00,0x1B,0xB7,0x00,0x00,0xE1,0x00,0xE0,0x70,0xB5,0x0A,0x48,0x0A,0x4C,0x84,0x42,
0x0C,0xD0,0x0A,0x4D,0x0A,0x4E,0xB5,0x42,0x08,0xD0,0x2A,0x68,0x69,0x68,0x50,0x1C,
0x12,0x78,0x92,0x00,0xA2,0x58,0x90,0x47,0x08,0x35,0xF4,0xE7,0x00,0xBF,0x00,0xBF,
0x70,0xBD,0xC0,0x46,0xA8,0x1C,0x00,0x00,0xA0,0x1C,0x00,0x00,0xB0,0x1C,0x00,0x00,
0xB8,0x1C,0x00,0x00,0x80,0xB5,0x06,0x48,0x06,0x49,0x41,0x60,0x06,0x4A,0x51,0x60,
0x06,0x49,0x01,0x60,0x11,0x60,0x10,0x20,0x00,0xF0,0x6F,0xF8,0x80,0xBD,0xC0,0x46,
0x00,0x08,0x0A,0x40,0x03,0x00,0x00,0xB1,0x00,0x08,0x0F,0x40,0x01,0x00,0x00,0x26,
0x08,0x48,0x80,0xF3,0x08,0x88,0x00,0xBF,0x00,0xBF,0x00,0xF0,0x67,0xF8,0x00,0x28,
0x01,0xD0,0xFF,0xF7,0xC1,0xFF,0x00,0x20,0xFF,0xF7,0xA0,0xFF,0x01,0x20,0x00,0xF0,
0x5F,0xF8,0xC0,0x46,0x00,0x04,0x00,0x20,0x70,0xB5,0x01,0x22,0x12,0x03,0x83,0x18,
0x0C,0x78,0x5D,0x68,0x0C,0x26,0xB5,0x43,0x34,0x40,0x2C,0x19,0x5C,0x60,0x49,0x78,
0x83,0x58,0x07,0x24,0xA3,0x43,0x21,0x40,0x59,0x18,0x81,0x50,0x70,0xBD,0x00,0x00,
0x05,0x48,0x01,0x68,0x01,0x22,0x11,0x43,0xFE,0x23,0x99,0x43,0x01,0x60,0x03,0x48,
0x02,0x60,0x03,0x49,0x41,0x60,0x70,0x47,0x00,0x04,0x0B,0x40,0x00,0x03,0x0B,0x40,
0x01,0x00,0x00,0xE4,0x04,0x48,0x00,0x21,0x01,0x60,0x04,0x48,0x01,0x68,0x03,0x22,
0x91,0x43,0x01,0x60,0x70,0x47,0xC0,0x46,0x08,0x03,0x0B,0x40,0x00,0x01,0x0B,0x40,
0x02,0x48,0x03,0x49,0x08,0x60,0xB3,0xE7,0x70,0x47,0xC0,0x46,0x00,0x18,0x00,0x00,
0x08,0xED,0x00,0xE0,0x80,0xB5,0xFF,0xF7,0x95,0xFF,0xFF,0xF7,0x27,0xFF,0xFF,0xF7,
0xE1,0xFF,0xFF,0xF7,0xEF,0xFE,0x80,0xBD,0x80,0xB5,0x0B,0x46,0x03,0x21,0x42,0x58,
0xC1,0x1D,0x18,0x46,0x00,0xF0,0x06,0xF8,0x80,0xBD,0x81,0x1E,0x09,0x1F,0x00,0xBF,
0xFC,0xD2,0x70,0x47,0x80,0xB5,0xFF,0xF7,0x1B,0xFE,0x80,0xBD,0x01,0x20,0x70,0x47,
0x00,0xBF,0xFE,0xE7,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x5D,0x19,0x00,0x00,0x69,0x1C,0x00,0x00,0x00,0x01,0x00,0xFF,0xF0,0x00,0x00,0x00,
0xA8,0x1C,0x00,0x00,0x00,0x00,0x00,0x20,};

const uint8_t App1_1[] = {
0x00,0x04,0x00,0x20,0x41,0x1C,0x00,0x00,0x5B,0x19,0x00,0x00,0x5B,0x19,0x00,0x00,
};

const uint8_t *App1_Ptr[2] = {
    App1_0,
    App1_1,
};

