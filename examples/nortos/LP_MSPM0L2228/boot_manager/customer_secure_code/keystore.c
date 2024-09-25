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

#include "keystore.h"
#include <string.h>
#include "lockable_storage_private.h"
#include "msp-crypto/msp_crypto.h"
#include "secret.h"

#ifdef CSC_ENABLE_KEYSTORE

key_hash_t gInputHashTable[4];

key_hash_t *gOutputHashTablePtr = gLockStgInRAM.keystore_hash_table;

#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY
typedef enum {
    KEYSTORE_ACTION_ADD       = 0x55,
    KEYSTORE_ACTION_REVOKE    = 0xAA,
    KEYSTORE_ACTION_NO_ACTION = 0x00
} key_action_t;

typedef struct key_request {
    key_action_t action;
    uintptr_t keyHashPtr;
    uintptr_t keyMaterialPtr;
    key_size_t keySize;
    bool isEncrypted;
} key_request_t;

static key_request_t Keystore_RequestQueue[CSC_KEYSTORE_MAX_REQUESTS];
static uint8_t Keystore_RQIndex = 0;

#endif

// Helper functions
static void Keystore_outputKeyHash(
    const uint32_t *KeyMaterialPtr, uint8_t index, key_size_t keySize);
static uint32_t Keystore_generateKeyHash(
    uint32_t *KeyMaterialPtr, key_size_t keySize, uint32_t *output);
static int16_t minimum(int16_t, int16_t);
static uint32_t Keystore_getKeySlotFromSlotIndex(uint8_t keySlotIndex);

#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY

key_status_t Keystore_revokeExistingKeys(uintptr_t hashPtr);
key_status_t Keystore_getNewKeyInput(
    uintptr_t hashPtr, uintptr_t keyMaterialPtr);

int8_t Keystore_findHashMatch(uint32_t *hashPtr);
int8_t Keystore_findEmptySlot(void);

#endif

#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY

int Keystore_resetRequestQueue(void)
{
    Keystore_RQIndex = 0;
    for (int i = 0; i < CSC_KEYSTORE_MAX_REQUESTS; i++) {
        Keystore_RequestQueue[i].action         = KEYSTORE_ACTION_NO_ACTION;
        Keystore_RequestQueue[i].keyHashPtr     = (uintptr_t) NULL;
        Keystore_RequestQueue[i].keyMaterialPtr = (uintptr_t) NULL;
        Keystore_RequestQueue[i].keySize        = KEY_SIZE_256;
        Keystore_RequestQueue[i].isEncrypted    = false;
    }
    return 0;
}

int Keystore_newKeyRequest(uint32_t hashPtr)
{
    if (Keystore_RQIndex >= CSC_KEYSTORE_MAX_REQUESTS) {
        // insufficient room to take the request. Request ignored.
        // TODO: Add error condition
    } else {
        Keystore_RequestQueue[Keystore_RQIndex].action = KEYSTORE_ACTION_ADD;
        Keystore_RequestQueue[Keystore_RQIndex].keyHashPtr =
            (uintptr_t) hashPtr;
        // Will need to be changed and not hardcoded for more complicated modules
        Keystore_RequestQueue[Keystore_RQIndex].keyMaterialPtr =
            CSC_KEY_EXCHANGE_REGION_BASE_ADDR;
        // TODO: come back and add encryption
        //
        // Keystore_RequestQueue[Keystore_RQIndex].isEncrypted = true;
        Keystore_RQIndex++;
    }
    // TODO: consider a different way to do this
    gSecretUpdate.DynamicKeys_Updated = SECRET_UPDATE;
    return 0;
}

bool Keystore_isPendingRequest(void)
{
    return (Keystore_RQIndex > 0);
}

int Keystore_revokeKeyRequest(uint32_t hashPtr)
{
    if (Keystore_RQIndex >= CSC_KEYSTORE_MAX_REQUESTS) {
        // insufficient room to take the request. Request ignored.
        // TODO: Add error condition
    } else {
        Keystore_RequestQueue[Keystore_RQIndex].action =
            KEYSTORE_ACTION_REVOKE;
        Keystore_RequestQueue[Keystore_RQIndex].keyHashPtr =
            (uintptr_t) hashPtr;
        // Other information in the queue is a don't care for revocation
        Keystore_RQIndex++;
    }
    // Todo: consider a different way to do this
    gSecretUpdate.DynamicKeys_Updated = SECRET_UPDATE;
    return 0;
}

key_status_t Keystore_executeKeyRequests(void)
{
    // TODO: maybe do all revokes first
    key_status_t status = KEYSTORE_STATUS_ERROR;
    for (int i = 0; i < Keystore_RQIndex; i++) {
        switch (Keystore_RequestQueue[i].action) {
            case KEYSTORE_ACTION_ADD:
                status = Keystore_getNewKeyInput(
                    Keystore_RequestQueue[i].keyHashPtr,
                    Keystore_RequestQueue[i].keyMaterialPtr);
                break;
            case KEYSTORE_ACTION_REVOKE:
                status = Keystore_revokeExistingKeys(
                    Keystore_RequestQueue[i].keyHashPtr);
                break;
            default:
                status = KEYSTORE_STATUS_ERROR;
                break;
        }
        if (status != KEYSTORE_STATUS_SUCCESS) {
            break;
        }
    }
    return status;
}

key_status_t Keystore_revokeExistingKeys(uintptr_t hashPtr)
{
    // This method can take input from the application header to revoke a
    // previously provisioned static or dynamic key. These keys will be wiped
    // from future versions of the key code and will no longer be available
    // It is possible for a future application with valid version to
    // re-provision the same key. But this application must have a higher version
    // than that which revoked the key in order for the key to be re-instated.

    // In the current implementation, there will not be a blacklist of key hashes
    // that cannot be used.

    // TLVs that contain keys to be revoked will be kept in the image.
    int8_t matchIndex = Keystore_findHashMatch((uint32_t *) hashPtr);
    if (matchIndex >= 0) {
        // In this case the key should be revoked. This means that
        // a secret_writeOut should be performed, and that the key in question
        // should not be copied over.
        memset(&gSecretStgInRAM.keystore_dynamicKeys[matchIndex][0], 0x00, 40);
    }

    return KEYSTORE_STATUS_SUCCESS;
}

key_status_t Keystore_getNewKeyInput(
    uintptr_t hashPtr, uintptr_t keyMaterialPtr)
{
    // This is the method called for getting a key input from the header of the
    // candidate application image that has been verified.

    // Proposal. During loading, the candidate application image will write
    // any key hashes that it wishes to use into a TLV stating it's desired
    // input hashes

    // If the input hash matches an existing hash that is in the secret region
    // it will be marked for transfer and is ok.

    // If there is still another hash that hasn't been listed by the applciation
    // at this point, the CSC will check a designated region of flash where the
    // new key material will reside (either encrypted or in plaintext).
    // plaintext key material option is provided only for debugging.

    // This key material is then copied into the secret key region.

    // If the key cannot be found through this search, then the hash is marked
    // as not found.
    int8_t matchIndex = Keystore_findHashMatch((uint32_t *) hashPtr);
    int8_t emptyIndex = Keystore_findEmptySlot();
    uint32_t hashBuffer[8];
    uint32_t status = KEYSTORE_STATUS_ERROR;

    /* check if the key doesn't already exist and there is an empty slot */
    if (matchIndex < 0 && emptyIndex >= 0) {
        // Decrypt if needed
        uint32_t *keyPtr = (uint32_t *) keyMaterialPtr;

        // check key pointer against provided hash for integrity
        Keystore_generateKeyHash(keyPtr, KEY_SIZE_256, hashBuffer);

        if (memcmp((void *) hashBuffer, (void *) hashPtr,
                sizeof(hashBuffer)) == 0) {
            // Hashes match, key is legitimate.
            // Copy the appropriate information into the dynamic keySlot
            gSecretStgInRAM.keystore_dynamicKeys[emptyIndex][0] =
                *((uint32_t *) hashPtr);
            /* TODO: update to any key size detected by the size of the key */
            gSecretStgInRAM.keystore_dynamicKeys[emptyIndex][1] = KEY_SIZE_256;
            for (int i = 0; i < 8; i++) {
                gSecretStgInRAM.keystore_dynamicKeys[emptyIndex][2 + i] =
                    *(keyPtr + i);
            }
            status = KEYSTORE_STATUS_SUCCESS;
        } else {
            status = KEYSTORE_STATUS_FAIL;
        }
    } else {
        if (matchIndex > 0) {
            /* Key already exists in storage. */
            status = KEYSTORE_STATUS_SUCCESS;
        } else {
            /* No empty slots remaining */
            status = KEYSTORE_STATUS_SLOTS_FULL;
        }
    }
    return status;
}

int8_t Keystore_findHashMatch(uint32_t *hashPtr)
{
    int8_t matchingHashIndex = -1;
    uint32_t truncatedHash;
    uint32_t hashBuffer[8];
    for (int i = 0; i < CSC_NUM_DYNAMIC_KEYS; i++) {
        truncatedHash = gSecretStgInRAM.keystore_dynamicKeys[i][0];
        if (truncatedHash == *hashPtr) {
            // compute full hash of key material
            Keystore_generateKeyHash(
                &gSecretStgInRAM.keystore_dynamicKeys[i][2],
                gSecretStgInFlash.keystore_dynamicKeys[i][1], hashBuffer);
            // compare entire hash
            if (memcmp(hashBuffer, hashPtr, 32) == 0) {
                matchingHashIndex = i;
                break;
            }
        }
    }
    return matchingHashIndex;
}

int8_t Keystore_findEmptySlot(void)
{
    int8_t emptyIndex = -1;
    for (int i = 0; i < CSC_NUM_DYNAMIC_KEYS; i++) {
        if (gSecretStgInRAM.keystore_dynamicKeys[i][0] == 0x00000000) {
            emptyIndex = i;
            break;
        }
    }
    return emptyIndex;
}

#else

/* Requests not supported */
int Keystore_newKeyRequest(uint32_t hashPtr)
{
    return 0;
}

int Keystore_revokeKeyRequest(uint32_t hashPtr)
{
    return 0;
}

#endif  // CSC_ENABLE_KEYSTORE_DYNAMIC_KEY

static DL_KEYSTORECTL_KeyWrConfig keyWriteConfig = {
    .keySlot = DL_KEYSTORECTL_KEY_SLOT_0,
    .keySize = DL_KEYSTORECTL_KEY_SIZE_128_BITS,
    .key     = (uint32_t *) &gSecretStgInFlash.keystore_staticKey[2],
};

void Keystore_storeKeys(void)
{
    // Determine the positioning algorithm to transfer the most important keys
    // into the keyStore.

    // The Keys are taken entirely from the shared secret in RAM, which
    // can be updated by the keystore_getNewKeyInput and is a pre-requisite
    // if keys are to be dynamically loaded.

    /* The order of storing the keys shall be as follows:
     * 1. Static Keys that haven't been revoked.
     * 2. Dynamic Keys Explicitly Referenced by the App Header for use, stored
     *    in gInputHashTable
     *
     */

    /* First cycle through the keys that are present to determine number and
     * type
     */

    /* configure the keystore */
    DL_KEYSTORECTL_KeyWrConfig keyWriteConfig;
    DL_KEYSTORECTL_STATUS keyStrStatus;
    uint8_t num256Keys = 0;
    uint8_t num128Keys = 0;
    const uint32_t *num256KeyPtrs[2];
    const uint32_t *num128KeyPtrs[4];

#ifdef CSC_ENABLE_KEYSTORE_STATIC_KEY
    // check if static key is populated with keyMaterial
    for (uint8_t j = 0; j < CSC_NUM_STATIC_KEYS; j++) {
        if (gSecretStgInFlash.keystore_staticKey[j][2] != 0x00) {
            if (gSecretStgInFlash.keystore_staticKey[j][1] == KEY_SIZE_128) {
                num128KeyPtrs[num128Keys] =
                    &gSecretStgInFlash.keystore_staticKey[j][2];
                num128Keys++;
            } else {
                num256KeyPtrs[num256Keys] =
                    &gSecretStgInFlash.keystore_staticKey[j][2];
                num256Keys++;
            }
        }
    }
#endif

#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY
    for (int j = 0; j < CSC_NUM_DYNAMIC_KEYS; j++) {
        if (gSecretStgInFlash.keystore_dynamicKeys[j][2] != 0x00) {
            if (gSecretStgInFlash.keystore_dynamicKeys[j][1] == KEY_SIZE_128) {
                num128KeyPtrs[num128Keys] =
                    &gSecretStgInFlash.keystore_dynamicKeys[j][2];
                num128Keys++;
            } else {
                num256KeyPtrs[num256Keys] =
                    &gSecretStgInFlash.keystore_dynamicKeys[j][2];
                num256Keys++;
            }
        }
    }
#endif

    switch (num256Keys) {
        case 0:
            DL_KEYSTORECTL_setNumberOf256Keys(
                KEYSTORECTL, DL_KEYSTORECTL_NUM_256_KEYS_ZERO);
            break;
        case 1:
            DL_KEYSTORECTL_setNumberOf256Keys(
                KEYSTORECTL, DL_KEYSTORECTL_NUM_256_KEYS_ONE);
            break;
        case 2:
        default:
            DL_KEYSTORECTL_setNumberOf256Keys(
                KEYSTORECTL, DL_KEYSTORECTL_NUM_256_KEYS_TWO);
            break;
    }

    uint8_t keySlotIndex = 0;
    // Iterate over all keys

    for (int i = 0; i < minimum(num256Keys, 2); i++) {
        keyWriteConfig.keySlot = (keySlotIndex == 0)
                                     ? DL_KEYSTORECTL_KEY_SLOT_0_AND_1
                                     : DL_KEYSTORECTL_KEY_SLOT_2_AND_3;
        keyWriteConfig.keySize = DL_KEYSTORECTL_KEY_SIZE_256_BITS;
        keyWriteConfig.key     = (uint32_t *) num256KeyPtrs[i];

        keyStrStatus = DL_KEYSTORECTL_writeKey(KEYSTORECTL, &keyWriteConfig);

        if (keyStrStatus == DL_KEYSTORECTL_STATUS_VALID) {
            Keystore_outputKeyHash(
                keyWriteConfig.key, keySlotIndex, keyWriteConfig.keySize);
        }
        keySlotIndex += 2;
    }
    for (int i = 0; (i < minimum(num128Keys, 4 - (num256Keys * 2))) &&
                    (keyStrStatus == DL_KEYSTORECTL_STATUS_VALID);
         i++) {
        keyWriteConfig.keySlot =
            Keystore_getKeySlotFromSlotIndex(keySlotIndex);
        keyWriteConfig.keySize = DL_KEYSTORECTL_KEY_SIZE_128_BITS;
        keyWriteConfig.key     = (uint32_t *) num128KeyPtrs[i];

        keyStrStatus = DL_KEYSTORECTL_writeKey(KEYSTORECTL, &keyWriteConfig);

        if (keyStrStatus == DL_KEYSTORECTL_STATUS_VALID) {
            Keystore_outputKeyHash(
                keyWriteConfig.key, keySlotIndex, keyWriteConfig.keySize);
        }
        keySlotIndex++;
    }

    if (keyStrStatus != DL_KEYSTORECTL_STATUS_VALID) {
        while (1)
            ;
    }
}

static void Keystore_outputKeyHash(
    const uint32_t *KeyMaterialPtr, uint8_t index, key_size_t keySize)
{
    // take the SHA256 Hash of the key (seed it or XOR it in any way?)
    int_fast16_t result;
    MSPCrypto_sha256_init();
    uint32_t keySizeInBytes = (keySize == KEY_SIZE_128) ? 16 : 32;

    result =
        MSPCrypto_sha256_update((const void *) KeyMaterialPtr, keySizeInBytes);

    // TODO: determine which part of the hash array to update

    result =
        MSPCrypto_sha256_finish((uint8_t *) gOutputHashTablePtr[index].hash);

    if (result == MSP_CRYPTO_SHA256_STATUS_SUCCESS) {
        gOutputHashTablePtr[index].keySlot =
            Keystore_getKeySlotFromSlotIndex(index);
        gOutputHashTablePtr[index].keySize = keySize;
    } else {
        gOutputHashTablePtr[index].keySlot = 0x1BAD2BAD;
    }
}

static uint32_t Keystore_generateKeyHash(
    uint32_t *KeyMaterialPtr, key_size_t keySize, uint32_t *output)
{
    // take the SHA256 Hash of the key (seed it or XOR it in any way?)
    int_fast16_t result;
    MSPCrypto_sha256_init();
    uint32_t keySizeInBytes = (keySize == KEY_SIZE_128) ? 16 : 32;

    result =
        MSPCrypto_sha256_update((const void *) KeyMaterialPtr, keySizeInBytes);

    // TODO: determine which part of the hash array to update

    result = MSPCrypto_sha256_finish((uint8_t *) output);

    if (result == MSP_CRYPTO_SHA256_STATUS_SUCCESS) {
        return 0;
    } else {
        return 0x1BAD;
    }
}

static uint32_t Keystore_getKeySlotFromSlotIndex(uint8_t keySlotIndex)
{
    uint32_t slot;
    switch (keySlotIndex) {
        case 0:
            slot = DL_KEYSTORECTL_KEY_SLOT_0;
            break;
        case 1:
            slot = DL_KEYSTORECTL_KEY_SLOT_1;
            break;
        case 2:
            slot = DL_KEYSTORECTL_KEY_SLOT_2;
            break;
        case 3:
            slot = DL_KEYSTORECTL_KEY_SLOT_3;
            break;
        default:
            slot = DL_KEYSTORECTL_KEY_SLOT_0;
            break;
    }
    return slot;
}

static int16_t minimum(int16_t x, int16_t y)
{
    return (x < y) ? x : y;
}

#endif  // CSC_ENABLE_KEYSTORE
