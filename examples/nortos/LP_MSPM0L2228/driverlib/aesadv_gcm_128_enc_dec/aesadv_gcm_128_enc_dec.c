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
#include "customer_secure_config.h"

/* initializes memory in a dedicated region that will be read-execute protected */
#define SECRET_MSG __attribute__((section(".secret"))) __attribute__((used))
#define AES_TRANSACTION_LENGTH (15)
#define AAD_LENGTH (5)

SECRET_MSG const uint32_t  __attribute__((aligned(4))) gKey[] = {0x92e9fffe , 0x1c736586 , 0x948f6a6d, 0x08833067};

static uint32_t gHkey[4] = {0x37533bb8 , 0x5d53bf08 , 0x29e5a60a , 0x783bd580};

static uint32_t gAesInput[AES_TRANSACTION_LENGTH] = {0x253231d9 ,0xe50684f8 ,0xc50959a5 ,0x9a26f5af, 0x53a9a786 , 0xdaf73415 , 0x3d304c2e , 0x728a318a , 0x950c3c1c ,0x53096895 ,0x240ecf2f ,0x25b5a649 ,0xf5ed6ab1 ,0x57e60daa ,0x397b63ba};

static uint32_t gAesAadInput[AAD_LENGTH] = {0xcefaedfe , 0xefbeadde , 0xcefaedfe , 0xefbeadde , 0xd2daadab} ;

static uint32_t gAesTag[4];

/* size is taken as 16 because output is generated as 128 block cipher text in each operation */
static uint32_t gAesEncryptedOutput[16];

static uint32_t gExpectedCiphertext[AES_TRANSACTION_LENGTH] = {0xC21E8342 ,0x24747721   , 0xB721724B ,   0x9CD4D084 ,  0x2F21AAE3 ,0xE0A4022C ,  0x237EC135  ,0x2EA1AC29  ,0xB214D521 ,0x1C936654
                                       ,0x5A6A8F7D  ,0x05AA84AC  ,0x390BA31B  ,0x97AC0A6A  ,0x91E0583D};

static uint32_t gAesDecryptedOutput[AES_TRANSACTION_LENGTH];

static uint32_t gAesIv[4] = {0xbebafeca,0xaddbcefa,0x88f8cade,0x01000000};


volatile uint32_t input_idx = 0;
volatile uint32_t output_idx = 0;
volatile uint32_t operation_in_progress = false;

/* check for whether to load aad or input text for encryption */
volatile uint32_t aad_flag = 0;
volatile bool gCorrectResult = true;

static DL_AESADV_Config gAESADV_config = {
    .mode = DL_AESADV_MODE_GCM_LOAD_HASH_KEY,
    .direction = DL_AESADV_DIR_ENCRYPT,
    .ctr_ctrWidth = DL_AESADV_CTR_WIDTH_32_BIT,
    .cfb_fbWidth = DL_AESADV_FB_WIDTH_128,
    .ccm_ctrWidth = DL_AESADV_CCM_CTR_WIDTH_2_BYTES,
    .ccm_tagWidth = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,
    .iv = (uint8_t *)&gAesIv[0],
    .nonce = NULL,
    .lowerCryptoLength = 60,
    .upperCryptoLength = 0,
    .aadLength = 20,
};

/* configuration for writing key in keystore */
static DL_KEYSTORECTL_KeyWrConfig gKeyWriteConfig ={
     .keySlot = DL_KEYSTORECTL_KEY_SLOT_0,
     .keySize = DL_KEYSTORECTL_KEY_SIZE_128_BITS,
     .key = (uint32_t *)&gKey[0],
};

/* configuration for loading key in AES engine */
static DL_KEYSTORECTL_Config gKeyTransferConfig ={
     .keySlot = DL_KEYSTORECTL_KEY_SLOT_0,
     .keySize = DL_KEYSTORECTL_KEY_SIZE_128_BITS,
     .cryptoSel = DL_KEYSTORECTL_CRYPTO_SEL_AES,
};

int aes_app(void);

int main(void){

    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();

    /* if initdone is not issued load keys to keystore and issue intidone */
    if(!DL_SYSCTL_isINITDONEIssued())
    {
        /*
         * Device in privileged mode, INITDONE has not been issued
         * Customer secure code keys/policies should be configured here
         * then INITDONE will be issued
         */
        DL_KEYSTORECTL_setNumberOf256Keys(KEYSTORECTL, DL_KEYSTORECTL_NUM_256_KEYS_ZERO);
        DL_KEYSTORECTL_writeKey(KEYSTORECTL, (DL_KEYSTORECTL_KeyWrConfig*) &gKeyWriteConfig);

        /* Read/Execute protect key stored in Keystore */
        DL_SYSCTL_setReadExecuteProtectFirewallAddrStart(CSC_SECRET_ADDR);
        DL_SYSCTL_setReadExecuteProtectFirewallAddrEnd(CSC_SECRET_END);
        DL_SYSCTL_enableReadExecuteProtectFirewall();

        /* End of Customer Secure Code */
        DL_SYSCTL_issueINITDONE();
    }

    /* Enables a AES_ADV interrupt in the NVIC interrupt controller */
    NVIC_EnableIRQ(AESADV_INT_IRQn);
    DL_SYSCTL_disableSleepOnExit();

    /* branch to gcm aes encryption application */
    aes_app();

    return 0;
}

int aes_app(void)
{

    /*
     * Set the key size and Load the key to the AES engine. This must be done prior to setting up
     * the AES engine for GCM encryption with SYSCFG_DL_AESADV_init()
     */
    DL_AESADV_setKeySize(AESADV, DL_AESADV_KEY_SIZE_128_BIT);


    DL_KEYSTORECTL_transferKey(KEYSTORECTL, (DL_KEYSTORECTL_Config*) &gKeyTransferConfig);

    /* load Hash-key to G-hash register */
    DL_AESADV_setGCMHashKeyAligned(AESADV, &gHkey[0]);

    /* Write the rest of the AES context */
    DL_AESADV_initGCM(AESADV, (DL_AESADV_Config *) &gAESADV_config);

    /* loading aad to aes */
    aad_flag = 1;

    /* clear the pending input ready interrupt and enable the input ready interrupt */
    DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_enableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);

    /* wait for input register to be ready to filled up with aad input data */
    while(input_idx < 8){
        __WFE();
    }

    /*set all index again to 0 for encryption */
    input_idx = 0;
    output_idx = 0;
    aad_flag =0;

    /* operation in progress signifies AES engine is encrypting/decrypting */
    operation_in_progress = true;

    /* enabling all 3 interrupts, INPUT_READY, OUTPUT_READY, and SAVED_OUTPUT_CONTEXT_READY */
    DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_enableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);
    DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);


    /* wait till encryption gets complete */
    while(operation_in_progress){
        __WFE();
    }


    /* Compare encrypted text to expected cipher text */
    for (int i = 0; i < AES_TRANSACTION_LENGTH; i++) {
        if (gAesEncryptedOutput[i] != gExpectedCiphertext[i]) {
            gCorrectResult = false;
        }
    }

    /*
    * Stop the debugger to examine the output. At this point,
    * gCorrectResults should be equal to "true" and gAesEncryptOutput
    * should be equal to gAesExpectedCiphertext.
    */
    __BKPT(0);


    /*set all index again to 0 for encryption */
    input_idx = 0;
    output_idx = 0;

    /* change the configuration direction decyrption */
    gAESADV_config.direction = DL_AESADV_DIR_DECRYPT;


    /* Re-Write the AES context to signify a new operation */
    DL_AESADV_initGCM(AESADV, (DL_AESADV_Config *) &gAESADV_config);


    /* loading aad to aes */
    aad_flag = 1;

    /* clear the pending input ready interrupt and enable the input ready interrupt */
    DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_enableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);

    /* wait for input register to be ready to filled up with aad data */
    while(input_idx < 8){
        __WFE();
    }

    input_idx = 0;
    output_idx = 0;
    aad_flag =0;

    /* operation in progress signifies AES engine is encrypting/decrypting */
    operation_in_progress = true;

    /* enabling all 3 interrupts, INPUT_READY, OUTPUT_READY, and SAVED_OUTPUT_CONTEXT_READY */
    DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_OUTPUT_READY);
    DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_enableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
    DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);
    DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);

    /* wait till decryption gets complete */
    while(operation_in_progress){
        __WFE();
    }

    /* Compare decrypted text to input data */
    for (int i = 0; i < AES_TRANSACTION_LENGTH; i++) {
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


    return 0;


}


void AESADV_IRQHandler(void)
{
    switch(DL_AESADV_getPendingInterrupt(AESADV)){
        case DL_AESADV_IIDX_INPUT_READY:
            if(aad_flag == 1){
                /* load aad (additional authentication data) into engine */
                DL_AESADV_loadInputDataAligned(AESADV, &gAesAadInput[input_idx]);
                input_idx = input_idx + 4;
                if(input_idx >= AAD_LENGTH){
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
                }
            }
            else if(gAESADV_config.direction == DL_AESADV_DIR_ENCRYPT){
                /* Load plaintext into engine */
                DL_AESADV_loadInputDataAligned(AESADV, &gAesInput[input_idx]);
                /* increment input index for next block of input */
                input_idx = input_idx + 4;
                if(input_idx >= AES_TRANSACTION_LENGTH){
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_INPUT_READY);
                }
            }
            else{
                /* Load ciphertext into engine */
                DL_AESADV_loadInputDataAligned(AESADV, &gAesEncryptedOutput[input_idx]);
                /* increment input index for next block of input */
                input_idx = input_idx + 4;
                if(input_idx >= AES_TRANSACTION_LENGTH){
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
                if(output_idx >= AES_TRANSACTION_LENGTH){
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                }

            }
            else{
                /* Get decrypted result */
                DL_AESADV_readOutputDataAligned(AESADV, &gAesDecryptedOutput[output_idx]);
                /* increment output index for next block of output */
                output_idx = output_idx + 4;
                DL_AESADV_clearInterruptStatus(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                if(output_idx >= AES_TRANSACTION_LENGTH){
                    DL_AESADV_disableInterrupt(AESADV, DL_AESADV_INTERRUPT_OUTPUT_READY);
                }
            }
            break;

        case DL_AESADV_IIDX_SAVED_OUTPUT_CONTEXT_READY:
            /* read output tag */
            DL_AESADV_readTAGAligned(AESADV, gAesTag);
            DL_AESADV_disableInterrupt(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);
            /* change the operation state to false */
            operation_in_progress = false;
        default :
            break;
    }
}
