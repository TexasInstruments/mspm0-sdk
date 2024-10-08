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
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, plaintext, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_aes.h>
#include "ti_msp_dl_config.h"


uint8_t g_plaintext[16] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
    0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};

uint8_t g_iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

uint8_t g_key[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b,
    0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b,
    0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

const uint8_t g_goldenData[16] = {0xf5, 0x8c, 0x4c, 0x04, 0xd6, 0xe5, 0xf1,
    0xba, 0x77, 0x9e, 0xab, 0xfb, 0x5f, 0x7b, 0xfb, 0xd6};

volatile bool gCorrectResults = false;

int main(void)
{
    uint8_t ciphertext[16];
    uint8_t buffer[16];
    uint_fast8_t i;

    /* Set up the AES engine for CBC encryption */
    SYSCFG_DL_init();

    /* Load a cipher key to module */
    DL_AES_setKey(AES, &g_key[0], DL_AES_KEY_LENGTH_256);

    /* Load the plaintext into the buffer to be XOR'd with the IV */
    DL_AES_xorData(&g_plaintext[0], &g_iv[0], &buffer[0]);

    /* Load buffer data plaintext ^ IV and encrypt it */
    DL_AES_loadDataIn(AES, &buffer[0]);

    /* Wait for engine to complete */
    while (DL_AES_isBusy(AES))
        ;

    /* Get ciphertext */
    DL_AES_getDataOut(AES, &ciphertext[0]);

    /* Compare ciphertext to golden data set */
    gCorrectResults = true;
    for (i = 0; i < 16; i++) {
        gCorrectResults &= (g_goldenData[i] == ciphertext[i]);
    }

    /* Set up the AES engine for CBC decryption with the same key */
    DL_AES_init(
        AES, DL_AES_MODE_DECRYPT_SAME_KEY_CBC_MODE, DL_AES_KEY_LENGTH_256);

    /* Load a cipher key to module */
    DL_AES_setKey(AES, &g_key[0], DL_AES_KEY_LENGTH_256);

    /* Load buffer with encrypted data */
    DL_AES_loadDataIn(AES, &ciphertext[0]);

    /* Wait for engine to complete */
    while (DL_AES_isBusy(AES))
        ;

    /* Get decrypted result result */
    DL_AES_getDataOut(AES, &buffer[0]);

    /* XOR block cipher result with IV to get plaintext */
    DL_AES_xorData(&buffer[0], &g_iv[0], &buffer[0]);

    /* Compare plaintext to original set */
    gCorrectResults = true;
    for (i = 0; i < 16; i++) {
        gCorrectResults &= (buffer[i] == g_plaintext[i]);
    }

    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    /*
     * Stop the debugger to examine the output. At this point,
     * gCorrectResults should be equal to "true" and buffer
     * should be equal to plaintext
     */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}
