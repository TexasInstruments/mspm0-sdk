/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
#include "ti/devices/msp/msp.h"
#include "ti/driverlib/driverlib.h"
#include "stdint.h"
#include "string.h"


#define SHA_API_EXTERNAL    (uint32_t)(0x01003bc1)
#define SRAM_DATA_LOCATION  (uint32_t)(0x20000900)
#define ONE_WORD_LEN        (uint32_t)(0x4)
#define SHA_DIGEST_LEN      (uint32_t)(0x8)
#define INPUT_SIZE          (uint32_t)(0x16)


typedef enum {
    SHA2SW_HASH_TYPE_256 = 0,
#if SHA2SW_SUPPORT_SHA2_224 == 1
    SHA2SW_HASH_TYPE_224 = 1,
#endif
#if SHA2SW_SUPPORT_SHA2_384 == 1
    SHA2SW_HASH_TYPE_384 = 2,
#endif
    SHA2SW_HASH_TYPE_512 = 3
} SHA2SW_HashType;

int_fast16_t result;
typedef int_fast16_t (*sha)(SHA2SW_HashType hashType,const void* data, size_t length, uint32_t digest[8]);


uint8_t inputData[INPUT_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

uint32_t goldenDigest[SHA_DIGEST_LEN] = { 0x9C94A623, 0xC45BD25A,
                                          0x3E3E1772, 0x7BF36316,
                                          0xF38FAC2F, 0xC5D6E923,
                                          0xCB60D2DB, 0x3990311A};
volatile bool validDigest = false;
volatile uint32_t shaDigest[SHA_DIGEST_LEN] = {0};

// Main function
int main()
{
    SYSCFG_DL_init();

    // Pointer to SHA function located at specific address
    sha shaFunction = (sha *)(SHA_API_EXTERNAL);

    // Run the SHA function with specified parameters
    int result = shaFunction(SHA2SW_HASH_TYPE_256, inputData, INPUT_SIZE, shaDigest);

    validDigest = true;
    for(int i=0;i<SHA_DIGEST_LEN;i++)
    {
        validDigest &= (goldenDigest[i] == shaDigest[i]);
    }

    /*
     * Stop the debugger to examine the output. At this point,
     * validDigest should be equal to "true" and shaDigest
     * should be equal to goldenDigest.
     */
    __BKPT(0);

    while(1) {
        __WFI();
    }
    return 0;
}


