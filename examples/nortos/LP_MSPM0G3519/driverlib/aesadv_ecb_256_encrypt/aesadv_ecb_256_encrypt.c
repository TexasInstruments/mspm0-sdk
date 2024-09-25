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

#include "ti_msp_dl_config.h"

/*
 * The following vectors are from example 1.5 ECB-AES256.Encrypt of
 * NIST SP 800-38A Appendix F: Example Vectors for Modes of Operation of the
 * AES
 */
uint8_t gKey[AES_KEY_SIZE] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07,
    0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

uint8_t gPlaintext[AES_BLOCK_SIZE] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f,
    0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};

uint8_t gCiphertext[AES_BLOCK_SIZE] = {0};

const uint8_t gGoldenData[AES_BLOCK_SIZE] = {0xf3, 0xee, 0xd1, 0xbd, 0xb5,
    0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8};

volatile bool gCorrectResults                  = false;
volatile DL_AESADV_STATUS gSuccessfulOperation = true;

int main(void)
{
    uint_fast8_t i;

    SYSCFG_DL_init();

    /* Wait for AES engine to indicate that an input context can be written */
    while (!DL_AESADV_isInputContextWriteable(AESADV)) {
        ;
    }

    /*
     * Load the key to the AES engine. This must be done prior to setting up
     * the AES engine for ECB encryption with SYSCFG_DL_AESADV_init()
     */
    gSuccessfulOperation =
        DL_AESADV_setKey(AESADV, &gKey[0], DL_AESADV_KEY_SIZE_256_BIT);

    /* Write the rest of the AES context */
    SYSCFG_DL_AESADV_init();

    while (!DL_AESADV_isInputReady(AESADV)) {
        ;
    }

    /* Load plaintext into engine */
    gSuccessfulOperation &= DL_AESADV_loadInputData(AESADV, &gPlaintext[0]);

    /* Wait for engine to complete operation */
    while (!DL_AESADV_isOutputReady(AESADV)) {
        ;
    }

    /* Get encrypted result */
    gSuccessfulOperation &= DL_AESADV_readOutputData(AESADV, &gCiphertext[0]);

    /* Compare ciphertext to golden data */
    gCorrectResults = true;
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        gCorrectResults &= (gGoldenData[i] == gCiphertext[i]);
    }

    /*
     * Stop the debugger to examine the output. At this point,
     * gCorrectResults should be equal to "true" and gCiphertext
     * should be equal to gGoldenData.
     *
     * gSuccessfulOperation should be "true". If not, the buffers i.e. gKey,
     * gPlaintext, or gCiphertext are not 32-bit aligned which is required for
     * a successful AES operation.
     */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}
