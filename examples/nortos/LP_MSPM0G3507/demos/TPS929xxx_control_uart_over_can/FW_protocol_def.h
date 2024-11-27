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

#ifndef FW_PROTOCOL_DEF_H_
#define FW_PROTOCOL_DEF_H_

/* FlexWire definition */
#define SYNC (0x55)

/* DEV_ADDR byte */
#define DEVICE_ADDR__0 (0x00)
#define DEVICE_ADDR__1 (0x01)
#define DEVICE_ADDR__2 (0x02)
#define DEVICE_ADDR__3 (0x03)
#define DEVICE_ADDR__4 (0x04)
#define DEVICE_ADDR__5 (0x05)
#define DEVICE_ADDR__6 (0x06)
#define DEVICE_ADDR__7 (0x07)
#define DEVICE_ADDR__8 (0x08)
#define DEVICE_ADDR__9 (0x09)
#define DEVICE_ADDR__10 (0x0A)
#define DEVICE_ADDR__11 (0x0B)
#define DEVICE_ADDR__12 (0x0C)
#define DEVICE_ADDR__13 (0x0D)
#define DEVICE_ADDR__14 (0x0E)
#define DEVICE_ADDR__15 (0x0F)

// Broadcast mode - (Bit 6)
#define BROADCAST (0x40) /* Broadcast (DEVICE_ADDR = 0x00) */
#define SINGLE_DEVICE_MODE (0x00)
#define BROADCAST_MODE (BROADCAST)
// Read/Write mode - (Bit 7)
#define READ (0x00)  /* Read mode */
#define WRITE (0x80) /* Write mode */
#define READ_MODE (READ)
#define WRITE_MODE (WRITE)

#endif /* FW_PROTOCOL_DEF_H_ */
