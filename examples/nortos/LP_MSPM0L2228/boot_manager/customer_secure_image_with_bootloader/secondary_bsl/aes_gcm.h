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

#ifndef __AES_GCM_H__
#define __AES_GCM_H__

#include <stdint.h>
#include <ti/driverlib/driverlib.h>

#define ENCRYPTION_ENABLED

#define ENCRYPTION_MAGIC_ADDR (0x0010)
#define ENCRYPTION_HEADER_MAGIC (0x9999)
#define ENCRYPTION_TYPE_OPERATION_GCM (0x40)
#define ENCRYPTION_HEADER_END_OF_HEADER (0xAAAA)

typedef enum gcm_status {
    GCM_OK                  = 0x6C31600D,
    GCM_OPERATION_CONTINUES = 5,
    GCM_INVALID_TAG         = 2,
    GCM_NEW_KEY             = 3,
    GCM_NEW_TAG             = 4,
    GCM_INIT_FAILED         = -1,
    GCM_OPERATION_FAILED    = -2,
    GCM_INVALID_HEADER      = -3,
} GCM_status;

GCM_status GCM_init(uintptr_t headerPtr);
GCM_status GCM_streamDataToFlash(
    uint32_t addrStart, uint32_t* inputPtr, uint32_t lengthBytes);
GCM_status GCM_finishOperation(void);

#endif  // __AES_GCM_H__
