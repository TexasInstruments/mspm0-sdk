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

#define INPUT_SIZE 16

static uint32_t aes_input[INPUT_SIZE] = {0xe2bec16b, 0x969f402e, 0x117e3de9,
    0x2a179373, 0x578a2dae, 0x9cac031e, 0xac6fb79e, 0x518eaf45, 0x461cc830,
    0x11e45ca3, 0x19c1fbe5, 0xef520a1a, 0x45249ff6, 0x179b4fdf, 0x7b412bad,
    0x10376ce6};

static const uint32_t aes_expectedCiphertext[INPUT_SIZE] = {0xACAB4976,
    0x46B21981, 0x9B8EE9CE, 0x7D19E912, 0x9BCB8650, 0xEE197250, 0x3A11DB95,
    0xB2787691, 0xB8D6BE73, 0x3B74C1E3, 0x9EE61671, 0x16952222, 0xA1CAF13F,
    0x09AC1F68, 0x30CA0E12, 0xA7E18675};

static const uint32_t key[4] = {
    0x16157e2b, 0xa6d2ae28, 0x8815f7ab, 0x3c4fcf09};

static uint32_t aes_iv[4] = {0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c};

static uint32_t aes_encrypt_output[INPUT_SIZE];

static uint32_t aes_decrypt_output[INPUT_SIZE];

volatile bool outputDMADoneFlag = false;

/* AES_ADV configuration for CBC mode in encrypt direction */
static DL_AESADV_Config AESADV_config = {
    .mode              = DL_AESADV_MODE_CBC,
    .direction         = DL_AESADV_DIR_ENCRYPT,
    .iv                = (uint8_t *) &aes_iv[0],
    .lowerCryptoLength = 64,
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

int main(void)
{
    volatile uint8_t status = 0x00;
    uint32_t transfer_size  = INPUT_SIZE;
    uint32_t cycleDelay     = 10000000;
    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* Setup interrupts on device */
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(DMA_INT_IRQn);

    /* DMA channel 0 is for input data transfer and dma channel 1 is for output data transfer */
    /*Set DMA source and destination address for input data transfer, and set the transfer size*/
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) aes_input);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &AESADV->DATA_IN);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, transfer_size);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /*Set DMA source and destination address for output data transfer, and set the transfer size*/
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &AESADV->DATA_OUT);
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) aes_encrypt_output);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, transfer_size);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    /*
     * Set the key size and Load the key to the AES engine. This must be done prior to setting up
     * the AES engine for ECB encryption with SYSCFG_DL_AESADV_init()
     */
    DL_AESADV_setKeySize(AESADV, DL_AESADV_KEY_SIZE_128_BIT);
    DL_AESADV_setKeyAligned(AESADV, &key[0], DL_AESADV_KEY_SIZE_128_BIT);

    /* Write the rest of the AES context */
    DL_AESADV_initCBC(AESADV, &AESADV_config);

    /* Set the AESADV module to run with Data Inputs using the DMA to read/write
     data rather than using register input/output */
    DL_AESADV_enableDMAOperation(AESADV);

    /* enable the dma input trigger */
    DL_AESADV_enableDMAInputTriggerEvent(AESADV);

    /* enable the dma output trigger */
    DL_AESADV_enableDMAOutputTriggerEvent(AESADV);

    /* wait for transfer */
    while (outputDMADoneFlag == false) {
        __WFE();
    }

    /* Compare ciphertext to golden data */
    for (int i = 0; i < INPUT_SIZE; i++) {
        if (aes_encrypt_output[i] != aes_expectedCiphertext[i]) {
            status = 0x01;
        }
    }

    /* resetting the variable again to false for decryption*/
    outputDMADoneFlag = false;

    /* disable both input and ouput dma channel */
    DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
    DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);

    DL_AESADV_disableDMAOperation(AESADV);
    /* disable the dma input trigger */
    DL_AESADV_disableDMAInputTriggerEvent(AESADV);

    /* disable the dma output trigger */
    DL_AESADV_disableDMAOutputTriggerEvent(AESADV);

    /* DMA channel 0 is for input data transfer and dma channel 1 is for output data transfer */
    /*Set DMA source and destination address for input data transfer, and set the transfer size*/
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) aes_encrypt_output);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &AESADV->DATA_IN);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, transfer_size);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /*Set DMA source and destination address for output data transfer, and set the transfer size*/
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &AESADV->DATA_OUT);
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) aes_decrypt_output);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, transfer_size);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    /* change the direction in configuration to Decryption */
    AESADV_config.direction = DL_AESADV_DIR_DECRYPT;

    /* Write the rest of the AES context */
    DL_AESADV_initCBC(AESADV, &AESADV_config);

    /* Set the AESADV module to run with Data Inputs using the DMA to read/write
       data rather than using register input/output */
    DL_AESADV_enableDMAOperation(AESADV);

    /* enable the dma input trigger */
    DL_AESADV_enableDMAInputTriggerEvent(AESADV);

    /* enable the dma output trigger */
    DL_AESADV_enableDMAOutputTriggerEvent(AESADV);

    /* Wait for transfer */
    while (outputDMADoneFlag == false) {
        __WFE();
    }

    /* Compare decrypted text to original input data */
    for (int i = 0; i < INPUT_SIZE; i++) {
        if (aes_decrypt_output[i] != aes_input[i]) {
            status = 0x01;
        }
    }

    if (status == 0x01) {
        /* if the AES output is not matching expected output value, flash faster to indicate*/
        cycleDelay = cycleDelay / 5;
    }

    while (1) {
        /*
         * Call togglePins API to flip the current value of LED PIN 1
         */

        delay_cycles(cycleDelay);
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_USER_LED_1_PIN);
    }
}

void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA)) {
        case DL_DMA_EVENT_IIDX_DMACH1:
            /* interrupt of output channel is fired */
            outputDMADoneFlag = true;
            DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL1);
            break;
        default:
            break;
    }
}
