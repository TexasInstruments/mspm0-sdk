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


#include "ti_msp_dl_config.h"
#define AES_TRANSACTION_LENGTH (4)

/* AES_ADV configuration for ECB mode in encrypt direction */
static DL_AESADV_Config gAESADV_config = {
      .mode              = DL_AESADV_MODE_ECB,
      .direction         = DL_AESADV_DIR_ENCRYPT,
      .iv                = NULL,
      .nonce             = NULL,
      .lowerCryptoLength = 16,
      .upperCryptoLength = 0,
      .aadLength         = 0,
};


static uint32_t gKey[4] = {0x16157e2b , 0xa6d2ae28 , 0x8815f7ab, 0x3c4fcf09};

static const uint32_t gAesExpectedCiphertext[AES_TRANSACTION_LENGTH] = {0xB47BD73A, 0x60367A0D, 0xF3CA9EA8, 0x97EF6624};

static uint32_t gAesInput[AES_TRANSACTION_LENGTH] = {0xe2bec16b, 0x969f402e, 0x117e3de9, 0x2a179373};

static uint32_t gAesDecryptOutput[AES_TRANSACTION_LENGTH];

static uint32_t gAesEncryptOutput[AES_TRANSACTION_LENGTH];

volatile bool gCorrectResult = true;

int main(void)
{

    /* Perform any initialization needed before using any board APIs*/
    SYSCFG_DL_init();

    /*
     * Set the key size and Load the key to the AES engine. This must be done prior to setting up
     * the AES engine for ECB encryption with SYSCFG_DL_AESADV_init()
     */
    DL_AESADV_setKeySize(AESADV, DL_AESADV_KEY_SIZE_128_BIT);

    DL_AESADV_setKeyAligned(AESADV,&gKey[0],DL_AESADV_KEY_SIZE_128_BIT);


    /* Write the rest of the AES context */
    DL_AESADV_initECB(AESADV, &gAESADV_config);


    /* wait for input register to be ready to filled up with input data */
    while (!DL_AESADV_isInputReady(AESADV)) {
        ;
    }

    /* Load plaintext into engine */
    DL_AESADV_loadInputDataAligned(AESADV, gAesInput);


    /* Wait for engine to complete operation */
    while (!DL_AESADV_isOutputReady(AESADV)) {
        ;
    }

    /* Get encrypted result */
    DL_AESADV_readOutputDataAligned(AESADV, gAesEncryptOutput);


    /* Compare ciphertext to golden data */
    for (int i = 0; i < AES_TRANSACTION_LENGTH; i++) {
        if (gAesEncryptOutput[i] != gAesExpectedCiphertext[i]) {
            gCorrectResult = false;
        }
    }

    /*
       * Stop the debugger to examine the output. At this point,
       * gCorrectResults should be equal to "true" and gAesEncryptOutput
       * should be equal to gAesExpectedCiphertext.
       */
   __BKPT(0);


    /* change the configuration direction decyrption */
    gAESADV_config.direction = DL_AESADV_DIR_DECRYPT;


    /* Re-Write the AES context to signify a new operation */
    DL_AESADV_initECB(AESADV, &gAESADV_config);


    /* wait for input register to be ready to filled up with input data */
    while (!DL_AESADV_isInputReady(AESADV)) {
        ;
    }

    /* Load ciphertext into engine */
    DL_AESADV_loadInputDataAligned(AESADV, gAesEncryptOutput);

    /* Wait for engine to complete operation */
    while (!DL_AESADV_isOutputReady(AESADV)) {
        ;
    }

    /* Get encrypted result */
    DL_AESADV_readOutputDataAligned(AESADV, gAesDecryptOutput);


    /* Compare decrypted text to original input data */
    for (int i = 0; i < AES_TRANSACTION_LENGTH; i++) {
        if (gAesDecryptOutput[i] != gAesInput[i]) {
            gCorrectResult = false;
        }
    }

    /*
   * Stop the debugger to examine the output. At this point,
   * gCorrectResults should be equal to "true" and gAesDecryptOutput
   * should be equal to gAesInput.
   */
   __BKPT(0);


   while (1) {
       __WFI();
   }

}

