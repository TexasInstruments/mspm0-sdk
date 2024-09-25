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

#ifndef __AES_CMAC_H__
#define __AES_CMAC_H__

#include <stdint.h>
#include <ti/driverlib/driverlib.h>
#include "bootutil/image.h"  // for version only
#include "customer_secure_config.h"

#ifdef CSC_OMIT_TRNG
#include <stdlib.h>
#endif

#define CMAC_VALID_KEY (0xC0DE600D)
#define CMAC_VALID_TAG (0x7A61600D)
#define CMAC_FORCE_NEW_KEY (0x123F09CE)

typedef struct cmac_generated_key {
    uint32_t key[8];
    uint32_t k1[4];
    uint32_t k2[4];
    uint32_t validKey;
} CMAC_generated_key;

typedef struct cmac_tag {
    uint8_t tag[16];
    uint32_t validTag;
    uint32_t image_length;
    uint32_t *image_start_address;
    uint32_t reserved;
    struct image_version version; /* a space for the version struct */
} CMAC_tag;

typedef enum cmac_status {
    CMAC_OK               = 0xC3AC600D,
    CMAC_INVALID_TAG      = 2,
    CMAC_NEW_KEY          = 3,
    CMAC_NEW_TAG          = 4,
    CMAC_INIT_FAILED      = -1,
    CMAC_OPERATION_FAILED = -2,
} CMAC_status;

CMAC_status CMAC_init(uint32_t forceNewKey);
CMAC_status CMAC_compareTag(uint32_t *, struct image_version *);
CMAC_status CMAC_generateTag(
    uint32_t *dataPtr, uint32_t length, const struct image_version *);

#endif  // __AES_CMAC_H__
