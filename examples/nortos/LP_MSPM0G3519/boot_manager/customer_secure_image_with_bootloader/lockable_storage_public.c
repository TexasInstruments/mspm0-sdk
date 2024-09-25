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

#include "lockable_storage_public.h"
#include <string.h>
#include <ti/driverlib/driverlib.h>

LOCKABLE_STG_PUBLIC volatile const LockableStorage gLockStgInFlash;

uint32_t Lock_keySearch(uint32_t* hashPtr, DL_KEYSTORECTL_Config* config)
{
    /* compare hash to outputs in the lockStg data type */
    uint32_t status = 0xFF;
    for (int i = 0; i < KEYSTORE_HASH_TABLE_SIZE; i++) {
        if (memcmp(gLockStgInFlash.keystore_hash_table[i].hash, hashPtr, 32) ==
            0) {
            config->keySlot   = gLockStgInFlash.keystore_hash_table[i].keySlot;
            config->keySize   = gLockStgInFlash.keystore_hash_table[i].keySize;
            config->cryptoSel = DL_KEYSTORECTL_CRYPTO_SEL_AES;
            status            = 0;
            break;
        }
    }
    return status;
}
