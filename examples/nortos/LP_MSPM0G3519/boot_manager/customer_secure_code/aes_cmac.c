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

#include "aes_cmac.h"
#include <string.h>
#include "secret.h"

static CMAC_status gStatus;

SecretUpdate_t *CMACUpdate     = &(gSecretUpdate.CMAC_Updated);
CMAC_generated_key *CMACKeyRAM = &(gSecretStgInRAM.CMACKey);
CMAC_tag *CMACTagRAM           = &(gSecretStgInRAM.CMACTag);

volatile const CMAC_generated_key *const CMACKeyFlash =
    &(gSecretStgInFlash.CMACKey);
volatile const CMAC_tag *const CMACTagFlash = &(gSecretStgInFlash.CMACTag);

static uint32_t all_zeros[] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};

void CMAC_generateNewKey(void);
void CMAC_deriveSubkeys(void);
static void CMAC_run(
    uint32_t *dataPtr, uint8_t *tagOut, DL_AESADV_Config *config);

static inline uint32_t swap_bytes(uint32_t);

CMAC_status CMAC_init(uint32_t isForceNewKey)
{
    gStatus = CMAC_INIT_FAILED;
    /* this is designed to see if there is a valid key in the secret storage */
    if (CMACKeyFlash->validKey == CMAC_VALID_KEY &&
        isForceNewKey != CMAC_FORCE_NEW_KEY) {
        if (CMACTagFlash->validTag == CMAC_VALID_TAG) {
            gStatus = CMAC_OK;
        } else {
            gStatus = CMAC_INVALID_TAG;
        }
    } else {
        /* There is not a valid key present on the device. A key must be generated */
        CMAC_generateNewKey();
        CMAC_deriveSubkeys();
        gStatus = CMAC_NEW_KEY;
    }

    /* only a CMAC_OK should prompt a full pre-run for a previously-verified image. */
    return gStatus;
}

static DL_AESADV_Config gAESADV_config = {
    .mode = DL_AESADV_MODE_CMAC,  // XCBC-MAC + CBC -> together that's CMAC
    .direction         = DL_AESADV_DIR_ENCRYPT,
    .ctr_ctrWidth      = DL_AESADV_CTR_WIDTH_32_BIT, /* Dont care */
    .cfb_fbWidth       = DL_AESADV_FB_WIDTH_128,     /* Dont care */
    .ccm_ctrWidth      = DL_AESADV_CCM_CTR_WIDTH_2_BYTES,
    .ccm_tagWidth      = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,
    .iv                = (uint8_t *) all_zeros,
    .nonce             = NULL,
    .k1                = (uint8_t *) CMACKeyFlash->k1,
    .k2                = (uint8_t *) CMACKeyFlash->k2,
    .lowerCryptoLength = 0x2800, /* updated by the run below */
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

CMAC_status CMAC_compareTag(uint32_t *offset, struct image_version *version)
{
    uint8_t candidateTag[16];
    uint32_t status = 0x00;
    if (boot_compare_version(version, &CMACTagFlash->version) == 0) {
        while (!DL_AESADV_isInputContextWriteable(AESADV)) {
            ;
        }
        DL_AESADV_setKeyAligned(AESADV, (const uint32_t *) CMACKeyFlash->key,
            DL_AESADV_KEY_SIZE_256_BIT);
        gAESADV_config.lowerCryptoLength = CMACTagFlash->image_length;
        CMAC_run(
            CMACTagFlash->image_start_address, candidateTag, &gAESADV_config);

        for (uint8_t j = 0; j < 16; j++) {
            if (candidateTag[j] != CMACTagFlash->tag[j]) {
                status |= 0x01;
            } else {
                status |= 0x10;
            }
        }
        if (status == 0x10) {
            /* assign the image offset to the bootheader
             * such that the customer secure code can jump to the
             * correct bank before. */
            *offset = (uint32_t) CMACTagFlash->image_start_address;
            return CMAC_OK;
        }
    }
    return CMAC_INVALID_TAG;
}

static DL_AESADV_Config gAESADV_genConfig = {
    .mode              = DL_AESADV_MODE_CMAC,
    .direction         = DL_AESADV_DIR_ENCRYPT,
    .ctr_ctrWidth      = DL_AESADV_CTR_WIDTH_32_BIT, /* Dont care */
    .cfb_fbWidth       = DL_AESADV_FB_WIDTH_128,     /* Dont care */
    .ccm_ctrWidth      = DL_AESADV_CCM_CTR_WIDTH_2_BYTES,
    .ccm_tagWidth      = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,
    .iv                = (uint8_t *) all_zeros,
    .nonce             = NULL,
    .k1                = (uint8_t *) CMACKeyFlash->k1,
    .k2                = (uint8_t *) CMACKeyFlash->k2,
    .lowerCryptoLength = 0x2800, /* updated by the run below */
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

CMAC_status CMAC_generateTag(
    uint32_t *dataPtr, uint32_t length, const struct image_version *version)
{
    /* only generating tags should be allowed to use a different key than stored
     * in Flash. This is to prevent FI attacks where new key can be loaded for comparison */
    while (!DL_AESADV_isInputContextWriteable(AESADV)) {
        ;
    }

    if (gStatus == CMAC_NEW_KEY) {
        DL_AESADV_setKeyAligned(
            AESADV, CMACKeyRAM->key, DL_AESADV_KEY_SIZE_256_BIT);
        gAESADV_genConfig.k1 = (uint8_t *) CMACKeyRAM->k1;
        gAESADV_genConfig.k2 = (uint8_t *) CMACKeyRAM->k2;
    } else {
        DL_AESADV_setKeyAligned(AESADV, (const uint32_t *) CMACKeyFlash->key,
            DL_AESADV_KEY_SIZE_256_BIT);

        /* If we are generating a new tag to write, we must also make the copy
         * over the existing key to the RAM space */
        memcpy(CMACKeyRAM, (const void *) CMACKeyFlash,
            sizeof(CMAC_generated_key));
    }

    gAESADV_genConfig.lowerCryptoLength = length;

    CMAC_run(dataPtr, CMACTagRAM->tag, &gAESADV_genConfig);

    CMACTagRAM->image_length        = length;
    CMACTagRAM->image_start_address = dataPtr;

    memcpy(
        &CMACTagRAM->version, (void *) version, sizeof(struct image_version));
    CMACTagRAM->validTag = CMAC_VALID_TAG;
    CMACKeyRAM->validKey = CMAC_VALID_KEY;

    /* Notify the secret region service to create a new secret region and
     * preserve changes that this function has made to the Secret Region of RAM
     */
    *CMACUpdate = SECRET_UPDATE;

    return CMAC_NEW_TAG;
}

static void CMAC_run(
    uint32_t *dataPtr, uint8_t *tagOut, DL_AESADV_Config *config)
{
    uint32_t i;

    /*
     * The key must be written prior to the init call since the key can
     * come from plaintext or from the key store controller
     */
    DL_AESADV_initCMAC(AESADV, (DL_AESADV_Config *) config);

    uint32_t iterations = config->lowerCryptoLength / 16;

    /* Currently, no bounds checking on the length, so this could hard fault if not careful */
    for (i = 0; i < iterations; i++) {
        while (!DL_AESADV_isInputReady(AESADV)) {
            ;
        }
        DL_AESADV_loadInputDataAligned(
            AESADV, (uint32_t *) ((uint32_t) dataPtr + i * 4));
    }

    /* Wait for engine to complete operation */
    while (!DL_AESADV_isSavedOutputContextReady(AESADV)) {
        ;
    }

    DL_AESADV_readTAG(AESADV, tagOut);

    return;
}

void CMAC_generateNewKey(void)
{
    /*
     * generates new 256-bit key based on the TRNG. Should only need to happen once for
     * the device or if a key is considered compromised (having run too many times, for
     * example)
     */
#ifndef CSC_OMIT_TRNG
    DL_TRNG_reset(TRNG);

    DL_TRNG_enablePower(TRNG);

    delay_cycles(60);

    DL_TRNG_setClockDivider(TRNG, DL_TRNG_CLOCK_DIVIDE_2);

    DL_TRNG_sendCommand(TRNG, DL_TRNG_CMD_NORM_FUNC);
    while (!DL_TRNG_isCommandDone(TRNG))
        ;
    DL_TRNG_clearInterruptStatus(TRNG, DL_TRNG_INTERRUPT_CMD_DONE_EVENT);

    DL_TRNG_setDecimationRate(TRNG, DL_TRNG_DECIMATION_RATE_4);

    for (uint8_t i = 0; i < 8; i++) {
        while (!DL_TRNG_isCaptureReady(TRNG))
            ;
        CMACKeyRAM->key[i] = DL_TRNG_getCapture(TRNG);
    }

    /* Power off the peripheral */
    DL_TRNG_disablePower(TRNG);
#else
    /* Pseudorandom for the purposes of testing. Does not represent a uniquely generated
     * key in a secure application due to fixed seed */
    srand(0xC0DEF00D);
    for (uint8_t i = 0; i < 8; i++) {
        CMACKeyRAM->key[i] = ((0xFF & rand()) << 24) |
                             ((0xFF & rand()) << 16) | ((0xFF & rand()) << 8) |
                             ((0xFF & rand()));
    }

#endif
}

static DL_AESADV_Config gAESADV_ecbConfig = {
    .mode              = DL_AESADV_MODE_ECB,
    .direction         = DL_AESADV_DIR_ENCRYPT,
    .ctr_ctrWidth      = DL_AESADV_CTR_WIDTH_32_BIT,      /* Dont care */
    .cfb_fbWidth       = DL_AESADV_FB_WIDTH_128,          /* Dont care */
    .ccm_ctrWidth      = DL_AESADV_CCM_CTR_WIDTH_2_BYTES, /* Dont care*/
    .ccm_tagWidth      = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,  /* Dont care*/
    .iv                = (uint8_t *) all_zeros,
    .nonce             = NULL,
    .k1                = NULL,
    .k2                = NULL,
    .lowerCryptoLength = 0x4, /* one 128-bit block is 4 bytes */
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

void CMAC_deriveSubkeys(void)
{
    /* FROM RFC-4493 Key derivation algorithm */
    DL_AESADV_setKeyAligned(
        AESADV, CMACKeyRAM->key, DL_AESADV_KEY_SIZE_256_BIT);
    DL_AESADV_initECB(AESADV, (DL_AESADV_Config *) &gAESADV_ecbConfig);

    uint32_t const_Rb =
        0x87000000;  // put into uint32_t version where the actual LSB is perceived as MSB (due to little endianness)
    uint32_t swapped = 0x00;
    uint32_t shifted = 0x00;
    uint32_t carry   = 0x00;
    uint32_t L[4];

    while (!DL_AESADV_isInputReady(AESADV)) {
        ;
    }
    DL_AESADV_loadInputDataAligned(AESADV, (uint32_t *) all_zeros);

    while (!DL_AESADV_isOutputReady(AESADV)) {
        ;
    }

    DL_AESADV_readOutputDataAligned(AESADV, L);

    /* left shift L by 1 */
    carry = 0;
    for (int8_t i = 3; i >= 0; i--) {
        swapped           = swap_bytes(L[i]);
        CMACKeyRAM->k1[i] = swap_bytes(((swapped << 1) + carry));
        carry             = ((0x80000000 & swapped) == 0x80000000) ? 1 : 0;
    }

    /* if MSB(L) is not 0 */
    if (carry != 0) {
        CMACKeyRAM->k1[3] = (CMACKeyRAM->k1[3] ^ const_Rb);
    }

    /* left shift K1 by 1 */
    carry = 0;
    for (int8_t i = 3; i >= 0; i--) {
        swapped           = swap_bytes(CMACKeyRAM->k1[i]);
        CMACKeyRAM->k2[i] = swap_bytes(((swapped << 1) + carry));
        carry             = ((0x80000000 & swapped) == 0x80000000) ? 1 : 0;
    }

    /* if MSB(K1) is not 0 */
    if (carry != 0) {
        CMACKeyRAM->k2[3] = (CMACKeyRAM->k2[3] ^ const_Rb);
    }

    /* remove any remnants of the key material, such as L */
    memset(L, 0x00, sizeof(L));
    swapped = 0x00;
}

static inline uint32_t swap_bytes(uint32_t val)
{
    /* REV is a assembly command that swaps byte order */
    asm volatile("REV %0, %0" : "+r"(val));
    return val;
}
