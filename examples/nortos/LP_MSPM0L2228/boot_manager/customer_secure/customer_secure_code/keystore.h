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

#ifndef __CSC_KEYSTORE_H__
#define __CSC_KEYSTORE_H__

#include <ti/driverlib/driverlib.h>
#include "customer_secure_config.h"

#ifdef CSC_ENABLE_KEYSTORE

#define KEYSTORE_HASH_CODE_ADD (0x1ADDC0DE)
#define KEYSTORE_HASH_CODE_REVOKE (0xBADC0DE7)
#define KEYSTORE_HASH_CODE_NOT_FOUND (0xC0DE0404)
#define KEYSTORE_HASH_CODE_GOOD (0x600DC0DE)
#define KEYSTORE_HASH_CODE_SLOT_0 (0x000000AA)
#define KEYSTORE_HASH_CODE_SLOT_1 (0x0000AA00)
#define KEYSTORE_HASH_CODE_SLOT_2 (0x00AA0000)
#define KEYSTORE_HASH_CODE_SLOT_3 (0xAA000000)

#define KEYSTORE_MAX_SLOTS (4)

#define KEY_SIZE_128 (DL_KEYSTORECTL_KEY_SIZE_128_BITS)
#define KEY_SIZE_256 (DL_KEYSTORECTL_KEY_SIZE_256_BITS)

typedef DL_KEYSTORECTL_KEY_SIZE key_size_t;

typedef enum {
    KEYSTORE_STATUS_SUCCESS    = 0x600D,
    KEYSTORE_STATUS_FAIL       = 0x1BAD,
    KEYSTORE_STATUS_ERROR      = 0xE9909,
    KEYSTORE_STATUS_SLOTS_FULL = 0xF011
} key_status_t;

typedef struct key_hash {
    uint32_t hash[8];
    uint32_t keySlot;
    union {
        uint32_t reserved;
        key_size_t keySize;
    };
} key_hash_t;

int Keystore_newKeyRequest(uint32_t hashPtr);
int Keystore_revokeKeyRequest(uint32_t hashPtr);
uint32_t Keystore_executeKeyRequests(void);
bool Keystore_isPendingRequest(void);

void Keystore_storeKeys(void);

#else

/* Requests not supported because CSC not supported */
int Keystore_newKeyRequest(uint32_t hashPtr)
{
    return 0;
}

int Keystore_revokeKeyRequest(uint32_t hashPtr)
{
    return 0;
}

#endif  // CSC_ENABLE_KEYSTORE

#endif  // __CSC_KEYSTORE_H__
