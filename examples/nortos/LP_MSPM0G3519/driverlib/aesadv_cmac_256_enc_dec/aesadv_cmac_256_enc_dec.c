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
#define INPUT_SIZE 12


SECRET_MSG const uint32_t  __attribute__((aligned(4))) gKey[8] = {0x10eb3d60, 0xbe71ca15, 0xf0ae732b, 0x81777d85,
                                                                 0x072c351f, 0xd708613b, 0xa310982d, 0xf4df1409};

static uint32_t gAesInput[INPUT_SIZE] = {0xe2bec16b, 0x969f402e, 0x117e3de9, 0x2a179373, 0x578a2dae ,
                                          0x9cac031e, 0xac6fb79e, 0x518eaf45, 0x461cc830, 0x11e45ca3, 0x00000080 ,0x00000000};

static const uint32_t gAesExpectedTagOut[4] = {0xf1d8f3aa, 0xc24056de, 0x69b1f532, 0xe611c9b9};

//static const uint32_t gKey[8] = {0x10eb3d60, 0xbe71ca15, 0xf0ae732b, 0x81777d85,
//                                 0x072c351f, 0xd708613b, 0xa310982d, 0xf4df1409};

static uint32_t gAesIv[4] = {0x0, 0x0, 0x0, 0x0};

static uint32_t gAesTagOut[4];

static uint32_t gKey_1[4] = {0x03edd1ca, 0xaced9e29, 0x80999a2e, 0x2f502186};
static uint32_t gKey_2[4] = {0x06daa395, 0x58db3d53, 0x0133355d, 0xd9a0420c};

volatile bool gOutputDMADoneFlag = false;
volatile bool gOperationIP= false;
volatile bool gCorrectResult = true;
volatile uint32_t num = 0;

/* AES_ADV configuration for CMAC mode in encrypt direction */
static DL_AESADV_Config gAESADV_config = {
    .mode = DL_AESADV_MODE_CMAC,  // XCBC-MAC + CBC -> together that's CMAC
    .direction         = DL_AESADV_DIR_ENCRYPT,
    .ctr_ctrWidth      = DL_AESADV_CTR_WIDTH_32_BIT, /* Dont care */
    .cfb_fbWidth       = DL_AESADV_FB_WIDTH_128,     /* Dont care */
    .ccm_ctrWidth      = DL_AESADV_CCM_CTR_WIDTH_2_BYTES,
    .ccm_tagWidth      = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,
    .iv                = (uint8_t *) &gAesIv[0],
    .nonce             = NULL,
    .k1                = (uint8_t *) &gKey_1[0],
    .k2                = (uint8_t *) &gKey_2[0],
    .lowerCryptoLength = 40, /* updated by the run below */
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

/* configuration for writing key in keystore */
static DL_KEYSTORECTL_KeyWrConfig gKeyWriteConfig ={
     .keySlot = DL_KEYSTORECTL_KEY_SLOT_0,
     .keySize = DL_KEYSTORECTL_KEY_SIZE_256_BITS,
     .key = (uint32_t *)&gKey[0],
};

/* configuration for loading key in AES engine */
static DL_KEYSTORECTL_Config gKeyTransferConfig ={
     .keySlot = DL_KEYSTORECTL_KEY_SLOT_0,
     .keySize = DL_KEYSTORECTL_KEY_SIZE_256_BITS,
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
        DL_KEYSTORECTL_setNumberOf256Keys(KEYSTORECTL, DL_KEYSTORECTL_NUM_256_KEYS_ONE);
        DL_KEYSTORECTL_writeKey(KEYSTORECTL, (DL_KEYSTORECTL_KeyWrConfig*) &gKeyWriteConfig);

        /* Read/Execute protect key stored in Keystore */
        DL_SYSCTL_setReadExecuteProtectFirewallAddrStart(CSC_SECRET_ADDR);
        DL_SYSCTL_setReadExecuteProtectFirewallAddrEnd(CSC_SECRET_END);
        DL_SYSCTL_enableReadExecuteProtectFirewall();

        /* End of Customer Secure Code */
        DL_SYSCTL_issueINITDONE();
    }

    /* Enables a AES_ADV and DMA interrupt in the NVIC interrupt controller */
    NVIC_EnableIRQ(DMA_INT_IRQn);
    NVIC_EnableIRQ(AESADV_INT_IRQn);
    DL_SYSCTL_disableSleepOnExit();

    /* branch to cmac aes encryption application */
    aes_app();

    return 0;
}

int aes_app(void)
{
    uint32_t transfer_size  = INPUT_SIZE;



    /* DMA channel 0 is for input data transfer and dma channel 1 is for output tag data transfer */
    /*Set DMA source and destination address for input data transfer, and set the transfer size*/
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) gAesInput);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &AESADV->DATA_IN);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, transfer_size);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);


    /*Set DMA source and destination address for output tag data transfer, and set the transfer size*/
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &AESADV->TAG0);
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) gAesTagOut);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, 4);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    /*
     * Set the key size and Load the key to the AES engine.
     */

    DL_AESADV_setKeySize(AESADV, DL_AESADV_KEY_SIZE_256_BIT);
    DL_KEYSTORECTL_transferKey(KEYSTORECTL, (DL_KEYSTORECTL_Config*) &gKeyTransferConfig);

    /* Write the rest of the AES context */
    DL_AESADV_initCMAC(AESADV, &gAESADV_config);


    /* Set the AESADV module to run with Data Inputs using the DMA to read/write
     data rather than using register input/output */
    DL_AESADV_enableDMAOperation(AESADV);

    /* enable the dma input trigger */
    DL_AESADV_enableDMAInputTriggerEvent(AESADV);


    /* wait for transfer of output tag*/
    while (gOutputDMADoneFlag == false) {
        __WFE();
    }

    /* Compare output tag to expected data */
    for (int i = 0; i < 4; i++) {
        if (gAesTagOut[i] != gAesExpectedTagOut[i]) {
            gCorrectResult = false;
        }
    }

    /*
    * Stop the debugger to examine the output. At this point,
    * gCorrectResults should be equal to "true" and gAesTagOut
    * should be equal to gAesExpectedTagOut.
    */
    __BKPT(0);



    while (1) {
        __WFI();
    }
}

void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA)) {
        case  DL_DMA_EVENT_IIDX_DMACH0:
            DL_AESADV_clearInterruptStatus(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);
            DL_AESADV_enableInterrupt(AESADV,DL_AESADV_INTERRUPT_SAVED_OUTPUT_CONTEXT_READY);
            break;
        case  DL_DMA_EVENT_IIDX_DMACH1:
            /* interrupt of output channel is fired */
            gOutputDMADoneFlag = true;
            DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL1 );
            DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL0 );
            break;
        default:
            break;
    }
}

void AESADV_IRQHandler(void)
{
    switch(DL_AESADV_getPendingInterrupt(AESADV)){
        case DL_AESADV_IIDX_SAVED_OUTPUT_CONTEXT_READY:
            /* read output tag */
            DL_DMA_startTransfer(DMA, DMA_CH1_CHAN_ID);
        default :
            break;
    }
}
