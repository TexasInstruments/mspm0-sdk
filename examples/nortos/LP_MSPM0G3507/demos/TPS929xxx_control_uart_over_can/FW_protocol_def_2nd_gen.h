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

#ifndef FW_PROTOCOL_DEF_2ND_GEN_H_
#define FW_PROTOCOL_DEF_2ND_GEN_H_

#define MAX_BURST_CNT 24

// Burst mode data length - (Bit 4-5)
#define DATA_LENGTH__1 (0x00)  /* Single-byte mode with 1 byte of data */
#define DATA_LENGTH__4 (0x10)  /* Burst mode with 4 bytes of data */
#define DATA_LENGTH__16 (0x20) /* Burst mode with 16 bytes of data */
#define DATA_LENGTH__24 (0x30) /* Burst mode with 8 bytes of data */
#define NORMAL_MODE (DATA_LENGTH__1)
#define BURST_MODE_SIZE_4 (DATA_LENGTH__4)
#define BURST_MODE_SIZE_16 (DATA_LENGTH__16)
#define BURST_MODE_SIZE_24 (DATA_LENGTH__24)
#define DATA_LENGTH_1 (DATA_LENGTH__1)
#define DATA_LENGTH_4 (DATA_LENGTH__4)
#define DATA_LENGTH_16 (DATA_LENGTH__16)
#define DATA_LENGTH_24 (DATA_LENGTH__24)
// 0 --> 1
// 1 --> 4
// 2 --> 16
// 3 --> 24
#define DATA_LENGTH(x)                 \
    (((x) == DATA_LENGTH__1)           \
            ? 1                        \
            : (((x) == DATA_LENGTH__4) \
                      ? 4              \
                      : (((x) == DATA_LENGTH__16) ? 16 : 24)))

#endif /* FW_PROTOCOL_DEF_2ND_GEN_H_ */
