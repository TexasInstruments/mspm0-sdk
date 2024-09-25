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

#include "customer_secure_config.h"
#include "lockable_storage_public.h"
#include "secondary_bsl/bsl_main.h"
#include "ti_msp_dl_config.h"

#ifdef GREEN
/* Primary slot will toggle the green LED by default */
#define TOGGLED_PORT GPIO_LEDS_USER_LED_1_PORT
#define TOGGLED_PIN GPIO_LEDS_USER_LED_1_PIN

#else /* BLUE */
/* Secondary slot will toggle the blue LED by default*/
#define TOGGLED_PORT GPIO_LEDS_USER_LED_3_PORT
#define TOGGLED_PIN GPIO_LEDS_USER_LED_3_PIN

#endif

#ifdef CSC_ENABLE_KEYSTORE_STATIC_KEY

static DL_AESADV_Config gAESADV_config = {
    .mode              = DL_AESADV_MODE_ECB,
    .direction         = DL_AESADV_DIR_DECRYPT,
    .lowerCryptoLength = 32,
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

/* Hash of the static key pre-calculated and public */
static uint32_t aes_keyHash[] = {0xb7b8ffd4, 0x266b7d7f, 0x079a6e19,
    0x673f980e, 0x2dc4a401, 0x4d3d81ec, 0xd235a5e1, 0x36f57d0a};

static uint32_t aes_input[] = {0xb47bd73a, 0x60367a0d, 0xf3ca9ea8, 0x97ef6624};

static const uint32_t aes_expectedOutput[] = {
    0xe2bec16b, 0x969f402e, 0x117e3de9, 0x2a179373};

static uint32_t aes_output[4];

#endif  // CSC_ENABLE_KEYSTORE_STATIC_KEY

uint8_t gPerformUpdate = 0;

int main(void)
{
    volatile uint8_t status = 0x00;
    uint32_t cycleDelay     = 10000000;
    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_2_PORT, GPIO_LEDS_USER_LED_2_PIN);
    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_3_PORT, GPIO_LEDS_USER_LED_3_PIN);

    DL_GPIO_setPins(TOGGLED_PORT, TOGGLED_PIN);

#ifdef CSC_ENABLE_KEYSTORE_STATIC_KEY
    DL_KEYSTORECTL_Config keyStrConfig;
    uint32_t aesKeySize;

    if (Lock_keySearch(aes_keyHash, &keyStrConfig) == 0) {
        if (keyStrConfig.keySize == DL_KEYSTORECTL_KEY_SIZE_128_BITS) {
            aesKeySize = DL_AESADV_KEY_SIZE_128_BIT;
        } else {
            aesKeySize = DL_AESADV_KEY_SIZE_256_BIT;
        }
        DL_AESADV_setKeySize(AESADV, aesKeySize);
        DL_KEYSTORECTL_transferKey(KEYSTORECTL, &keyStrConfig);
    } else {
        while (1)
            ;
    }

    DL_AESADV_initECB(AESADV, &gAESADV_config);

    while (!DL_AESADV_isInputReady(AESADV)) {
        ;
    }

    DL_AESADV_loadInputDataAligned(AESADV, aes_input);

    while (!DL_AESADV_isOutputReady(AESADV)) {
        ;
    }

    DL_AESADV_readOutputDataAligned(AESADV, aes_output);

    for (int i = 0; i < 4; i++) {
        if (aes_output[i] != aes_expectedOutput[i]) {
            status = 0x01;
        }
    }

    if (status == 0x01) {
        // failure in the AES expected output value, flash faster to indicate
        // failure
        cycleDelay = cycleDelay / 5;
    }
#endif  // CSC_ENABLE_KEYSTORE_STATIC_KEY

    while (gPerformUpdate == 0) {
        /*
         * Call togglePins API to flip the current value of LEDs 1-3. This
         * API causes the corresponding HW bits to be flipped by the GPIO HW
         * without need for additional R-M-W cycles by the processor.
         */
        DL_GPIO_togglePins(TOGGLED_PORT, TOGGLED_PIN);
        delay_cycles(cycleDelay);
    }

    DL_AESADV_reset(AESADV);
    DL_AESADV_enablePower(AESADV);
    BSL_Update();

    while (1) {
        DL_GPIO_togglePins(
            GPIO_LEDS_USER_LED_2_PORT, GPIO_LEDS_USER_LED_2_PIN);
        delay_cycles(cycleDelay);
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            DL_GPIO_setPins(
                GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
            DL_GPIO_setPins(
                GPIO_LEDS_USER_LED_3_PORT, GPIO_LEDS_USER_LED_3_PIN);
            gPerformUpdate = 1;
            break;
    }
}
