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



/**
 * main.c
 */
#include "ti_msp_dl_config.h"

#define INITIALIZATION_VECTOR_WORDS (4)
#define CTR_TRANSACTION_LENGTH  (8)

static uint32_t gKey[4] = {0x16157e2b , 0xa6d2ae28 , 0x8815f7ab, 0x3c4fcf09};

static uint32_t gAesInput[CTR_TRANSACTION_LENGTH] = {0x91614d87, 0x26e320b6, 0x6468ef1b, 0xceb60d99 , 0x6bf60698 , 0xfffd7079 ,
                                       0x7b181786 , 0xfffdffb9};

static uint32_t gAesDecryptedOutput[CTR_TRANSACTION_LENGTH];

static uint32_t gAesEncryptedOutput[CTR_TRANSACTION_LENGTH];

volatile uint32_t gAesIv[INITIALIZATION_VECTOR_WORDS];


/* AES_ADV configuration for ECB mode in encrypt direction */
static DL_AESADV_Config gAESADV_config = {
    .mode = DL_AESADV_MODE_CTR,
    .direction = DL_AESADV_DIR_ENCRYPT,
    .ctr_ctrWidth = DL_AESADV_CTR_WIDTH_32_BIT,
    .cfb_fbWidth = DL_AESADV_FB_WIDTH_128,
    .ccm_ctrWidth = DL_AESADV_CCM_CTR_WIDTH_2_BYTES,
    .ccm_tagWidth = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,
    .iv = (uint8_t *)&gAesIv[0],
    .nonce = NULL,
    .lowerCryptoLength = 32,
    .upperCryptoLength = 0,
    .aadLength = 0,
};

volatile uint32_t input_idx = 0;
volatile uint32_t output_idx = 0;
volatile uint32_t operation_in_progress = false;
volatile bool gCorrectResult = true;


int main(void)
{

    /* Perform any initialization(TRNG here) needed before using any board APIs*/
    SYSCFG_DL_init();

    uint32_t i = 0;

    /* Enables a AES_ADV interrupt in the NVIC interrupt controller */
    NVIC_EnableIRQ(AESADV_INT_IRQn);
    DL_SYSCTL_disableSleepOnExit();

    /* Setup and start a capture, then wait for the result */
    for (i = 0; i < INITIALIZATION_VECTOR_WORDS; i++) {
        while (!DL_TRNG_isCaptureReady(TRNG))
            ;
        DL_TRNG_clearInterruptStatus(
            TRNG, DL_TRNG_INTERRUPT_CAPTURE_RDY_EVENT);
        gAesIv[i] = DL_TRNG_getCapture(TRNG);
    }

    /* Power off the peripheral */
    DL_TRNG_disablePower(TRNG);

    /*
     * Set the key size and Load the key to the AES engine. This must be done prior to setting up
     * the AES engine for CTR encryption with SYSCFG_DL_AESADV_init()
     */

    DL_AESADV_setKeySize(AESADV, DL_AESADV_KEY_SIZE_128_BIT);

    DL_AESADV_setKeyAligned(AESADV,&gKey[0],DL_AESADV_KEY_SIZE_128_BIT);


    /* Write the rest of the AES context */
    DL_AESADV_initCTR(AESADV, (DL_AESADV_Config *) &gAESADV_config);


    /*set all index again to 0 for encryption*/
    input_idx = 0;
    output_idx = 0;

    /* operation in progress signifies AES engine is encrypting/decrypting */
    operation_in_progress = true;

    /* enabling all 2 interrupts, INPUT_READY and OUTPUT_READY*/
    DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_enableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);


    /* wait till encryption gets complete */
    while(operation_in_progress){
        __WFE();
    }


    /* change the configuration direction decryption */
    gAESADV_config.direction = DL_AESADV_DIR_DECRYPT;


    /* Re-Write the AES context to signify a new operation */
    DL_AESADV_initCTR(AESADV, (DL_AESADV_Config *) &gAESADV_config);

    input_idx = 0;
    output_idx = 0;

    /* operation in progress signifies AES engine is encrypting/decrypting */
    operation_in_progress = true;

    /* enabling all 3 interrupts, INPUT_READY and OUTPUT_READY */
    DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_enableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);

    /* wait till decryption gets complete */
    while(operation_in_progress){
        __WFE();
    }

    /* Compare decrypted text to input data */
    for (int i = 0; i < CTR_TRANSACTION_LENGTH; i++) {
        if (gAesDecryptedOutput[i] != gAesInput[i]) {
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

void AESADV_IRQHandler(void)
{
    switch(DL_AESADV_getPendingInterrupt(AESADV)){
        case DL_AESADV_IIDX_INPUT_READY:
            if(gAESADV_config.direction == DL_AESADV_DIR_ENCRYPT){
                /* Load plaintext into engine */
                DL_AESADV_loadInputDataAligned(AESADV, &gAesInput[input_idx]);
                /* increment input index for next block of input */
                input_idx = input_idx + 4;
                if(input_idx == CTR_TRANSACTION_LENGTH){
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
                }
            }
            else{
                /* Load ciphertext into engine */
                DL_AESADV_loadInputDataAligned(AESADV, &gAesEncryptedOutput[input_idx]);
                /* increment input index for next block of input */
                input_idx = input_idx + 4;
                if(input_idx == CTR_TRANSACTION_LENGTH){
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
                }
            }
            break;

        case DL_AESADV_IIDX_OUTPUT_READY:
            if(gAESADV_config.direction == DL_AESADV_DIR_ENCRYPT){
                /* Get encrypted result */
                DL_AESADV_readOutputDataAligned(AESADV, &gAesEncryptedOutput[output_idx]);
                /* increment output index for next block of output */
                output_idx = output_idx + 4;
                DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                if(output_idx == CTR_TRANSACTION_LENGTH){
                    /* change the operation state to false */
                    operation_in_progress = false;
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                }

            }
            else{
                /* Get decrypted result */
                DL_AESADV_readOutputDataAligned(AESADV, &gAesDecryptedOutput[output_idx]);
                /* increment output index for next block of output */
                output_idx = output_idx + 4;
                DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                if(output_idx == CTR_TRANSACTION_LENGTH){
                    /* change the operation state to false */
                    operation_in_progress = false;
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                }
            }
            break;

        default :
            break;
    }
}
