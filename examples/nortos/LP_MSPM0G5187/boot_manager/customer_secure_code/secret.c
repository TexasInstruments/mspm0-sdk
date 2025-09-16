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

#include "secret.h"
#include "keystore.h"

#include <string.h>
#include <ti/driverlib/driverlib.h>

SecretStorage __attribute__((used)) gSecretStgInRAM;
SecretUpdateVector __attribute__((used)) __attribute__((aligned(4)))
gSecretUpdate;

static uint32_t gSecretStgKeyFlag;

#if defined(CSC_STATIC_SHARED_SECRET_KEY_INTERNAL) || \
    defined(CSC_ENABLE_KEYSTORE_STATIC_KEY)
#ifdef CSC_STATIC_SHARED_SECRET_KEY_INTERNAL
const uint32_t internal_shared_secret[] = {0x16157e2b, 0xa6d2ae28, 0x8815f7ab,
    0x3c4fcf09, 0x16157e2b, 0xa6d2ae28, 0x8815f7ab, 0x3c4fcf09};
#endif

/* can be read but should not be written to */
SECRET_MSG const SecretStorage gSecretStgInFlash = {
#ifdef CSC_STATIC_SHARED_SECRET_KEY_INTERNAL
    .shared_secret = internal_shared_secret;
#endif
#ifdef CSC_ENABLE_KEYSTORE_STATIC_KEY
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
.keystore_staticKey = {
    {0x00, 0x1, 0x16157e2b, 0xa6d2ae28, 0x8815f7ab, 0x3c4fcf09},
    {0x00, 0x1, 0x92e9fffe, 0x1c736586, 0x948f6a6d, 0x08833067}},
#endif
}
;
#endif

static uint32_t Secret_copyOverKeys(void);
#ifdef CSC_ENABLE_CMAC_ACCELERATION
static void Secret_copyOverCMAC(void);
#endif  //CSC_ENABLE_CMAC_ACCELERATION
#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
static void Secret_copyOverRollback(void);
#endif  //CSC_ENABLE_ROLLBACK_PROTECTION

void Secret_init(void)
{
    // Determine the correct secret region to use
    memset(&gSecretStgInRAM, 0x00, sizeof(SecretStorage));
    memset(&gSecretUpdate, 0x00, sizeof(SecretUpdateVector));
}

void Secret_writeOut(void)
{
    /* will write secret into flash region. All components desired to be written
     * should be present in RAM, pointed to by the secret storage region.
     *
     * Upon completion of the secret write it is up to the applications
     * to erase/taint extranneous secret components in RAM that are stored
     * outside of gSecretStgInRAM.
     */

    // First, determine if there was anything that changed such that
    // the secret must be re-written

    // Copy over static keys or other things that the CSC is not expected to
    // change
    uint8_t doUpdate;
    uint32_t keyCopyStatus                            = 0;
    uint8_t update_vector[sizeof(SecretUpdateVector)] = {0};

    if (memcmp(update_vector, &gSecretUpdate, sizeof(SecretUpdateVector)) ==
        0) {
        // No update required. Do not write out

    } else {
#ifdef CSC_ENABLE_CMAC_ACCELERATION
        if (gSecretUpdate.CMAC_Updated != SECRET_IS_UPDATED) {
            Secret_copyOverCMAC();
        }
#endif
#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
        if (gSecretUpdate.RollbackProtection_Updated != SECRET_IS_UPDATED) {
            Secret_copyOverRollback();
        }
#endif
        /* Dynamic Keys will require a copy over either way, so  we take care of
         * update behavior in the copyOverKeys funciton
         */
        keyCopyStatus = Secret_copyOverKeys();

        if (keyCopyStatus == 0) {
            DL_FlashCTL_unprotectSector(
                FLASHCTL, CSC_SECRET_ADDR, DL_FLASHCTL_REGION_SELECT_MAIN);
            /* Erase sector in main memory */
            DL_FlashCTL_eraseMemoryFromRAM(
                FLASHCTL, CSC_SECRET_ADDR, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

            DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
                FLASHCTL, CSC_SECRET_ADDR, (uint32_t *) &gSecretStgInRAM,
                sizeof(SecretStorage) / 4, DL_FLASHCTL_REGION_SELECT_MAIN);
        }

        /* Zero out RAM */
        memset(&gSecretStgInRAM, 0x00, sizeof(SecretStorage));
    }
}

static uint32_t Secret_copyOverKeys(void)
{
    uint32_t status = 0;

    // first copy over existing keys
#ifdef CSC_STATIC_SHARED_SECRET_KEY
    for (int i = 0; i < 8; i++) {
        gSecretStgInRAM.shared_secret[i] = gSecretStgInFlash.shared_secret[i];
    }
#endif

#ifdef CSC_ENABLE_KEYSTORE_STATIC_KEY
    for (int j = 0; j < CSC_NUM_STATIC_KEYS; j++) {
        for (int i = 0; i < 10; i++) {
            gSecretStgInRAM.keystore_staticKey[j][i] =
                gSecretStgInFlash.keystore_staticKey[j][i];
        }
    }
#endif

#ifdef CSC_ENABLE_KEYSTORE_DYNAMIC_KEY
    key_status_t keyStatus;

    for (int j = 0; j < CSC_NUM_DYNAMIC_KEYS; j++) {
        for (int i = 0; i < 10; i++) {
            gSecretStgInRAM.keystore_dynamicKeys[j][i] =
                gSecretStgInFlash.keystore_dynamicKeys[j][i];
        }
    }

    if (Keystore_isPendingRequest()) {
        keyStatus = Keystore_executeKeyRequests();
    }
    if (keyStatus != KEYSTORE_STATUS_SUCCESS) {
        status = keyStatus;
    }
#endif

    return status;
}

#ifdef CSC_ENABLE_CMAC_ACCELERATION

static void Secret_copyOverCMAC(void)
{
    memcpy((void *) &gSecretStgInRAM.CMACKey,
        (const void *) &gSecretStgInFlash.CMACKey, sizeof(CMAC_generated_key));
    memcpy((void *) &gSecretStgInRAM.CMACTag,
        (const void *) &gSecretStgInFlash.CMACTag, sizeof(CMAC_tag));
}

#endif  // CSC_ENABLE_CMAC_ACCELERATION

#ifdef CSC_ENABLE_ROLLBACK_PROTECTION

static void Secret_copyOverRollback(void)
{
    gSecretStgInRAM.Rollback_securityCount =
        gSecretStgInFlash.Rollback_securityCount;
    gSecretStgInRAM.Rollback_inverse = gSecretStgInFlash.Rollback_inverse;
}

#endif  // CSC_ENABLE_ROLLBACK_PROTECTION
