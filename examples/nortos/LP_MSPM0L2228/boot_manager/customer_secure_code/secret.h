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

#ifndef __SECRET_STG__
#define __SECRET_STG__

#include <stdint.h>
#include "aes_cmac.h"
#include "customer_secure_config.h"

#define SECRET_MSG __attribute__((section(".secret"))) __attribute__((used))

#define SECRET_IS_UPDATED (0xAA)
/* all elements that should be in secret storage should be defined here to be written */
typedef struct secretStorage {
#ifdef CSC_ENABLE_CMAC_ACCELERATION
    CMAC_generated_key CMACKey;
    CMAC_tag CMACTag;
#endif
#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
    uint32_t Rollback_securityCount;
    uint32_t Rollback_inverse;  // count inverse
#endif
#ifdef CSC_STATIC_SHARED_SECRET_KEY
    uint32_t shared_secret[8];
#endif
#ifdef CSC_ENABLE_KEYSTORE
#ifdef CSC_ENABLE_KEYSTORE_STATIC_KEY

#define CSC_NUM_STATIC_KEYS (2)
    /* Note that the keys below are in the following format:
     * uint32_t: shortened hash for revocation if applicable, 0x00 if
     *           non-revocable
     * uint32_t: key size, one of DL_KEYSTORECTL_KEY_SIZE
     * uint32_t key[8]: key array in little-endian notation, thus if a 128-bit
     *           key in 8-byte notation is aabbccddeeff00112233445566778899,
     *           the key would be written into the array as:
     *           { 0xddccbbaa, 0x1100ffee, 0x55443322, 0x99887766 }
     *           and the last 4 words of the array, key[4]-key[7], will
     *           remain empty.
     */
    uint32_t keystore_staticKey[CSC_NUM_STATIC_KEYS][10];

#endif
#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY
    /* Note that the keys below are in the following format:
     * uint32_t: first 4-bytes of hash (little-endian) for revocation.
     * uint32_t: key size, one of DL_KEYSTORECTL_KEY_SIZE
     * uint32_t key[8]: key array in little-endian notation, thus if a 128-bit
     *           key in 8-byte notation is aabbccddeeff00112233445566778899,
     *           the key would be written into the array as:
     *           { 0xddccbbaa, 0x1100ffee, 0x55443322, 0x99887766 }
     *           and the last 4 words of the array, key[4]-key[7], will
     *           remain empty.
     */
    uint32_t keystore_dynamicKeys[CSC_NUM_DYNAMIC_KEYS][10];
#endif
#endif  // CSC_ENABLE_KEYSTORE
    uint32_t secretRegionVersion __attribute__((aligned(8)));
    uint32_t crc_val;  // Check to ensure unmodified
} SecretStorage;

typedef enum {
    SECRET_NO_UPDATE = 0x00,
    SECRET_UPDATE    = 0xAA,
} SecretUpdate_t;

/* The update vector is used by the different processes to notify the secret
 * region whether a new update is required. */
typedef struct update_vector {
#ifdef CSC_ENABLE_CMAC_ACCELERATION
    SecretUpdate_t CMAC_Updated;
#endif
#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
    SecretUpdate_t RollbackProtection_Updated;
#endif
#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY
    SecretUpdate_t DynamicKeys_Updated;
#endif
    SecretUpdate_t reserved;
} SecretUpdateVector;

#ifdef CSC_STATIC_SHARED_SECRET_KEY_INTERNAL
extern uint32_t internal_shared_secret[8];
#endif

/* This is a working copy of the secret storage struct that can be written to flash
 * in one go. It is for editing and making changes to the struct before writing.
 * It must be cleared/tainted before exiting the privileged phase.
 *
 * If there are any elements that are desired to be retained from the original
 * secret storage, then it must be loaded with the flash contents upon initialization
 */
extern SecretStorage gSecretStgInRAM;

/* This is an update vector showing through the process of running the CSC if
 * a new secret region in flash needs to be created because new information
 * has been obtained such that the old secret region is no longer up-to-date.
 */
extern SecretUpdateVector gSecretUpdate;

/* This is a read-only copy of the secret storage struct that can be read from
 * privileged mode in order to perform operations based on its contents.
 *
 * It is not recommended to copy any elements out of this struct to anything
 * but the gSecretStgInRAM for editing
 */
extern const SecretStorage gSecretStgInFlash;

/* Clears regions of memory such that the information in RAM is correct for
 * whether or not to update.
 */
void Secret_init(void);

void Secret_writeOut(void);

#endif  // __SECRET_STG__
