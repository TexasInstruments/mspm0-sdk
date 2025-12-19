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

#ifndef __LOCKABLE_STG_COMMON_H__
#define __LOCKABLE_STG_COMMON_H__

#include <stdint.h>
#include "customer_secure_config.h"
#include "hash_verify.h"
#include "keystore.h"

#define LOCKABLE_STG __attribute__((section(".lockStg"))) __attribute__((used))
#define LOCKABLE_STG_PUBLIC __attribute__((section(".lockStg")))

#define LOCKSTG_BOOT_STATUS_SUCCESS 0xB007600D
#define LOCKSTG_BOOT_STATUS_FAILURE 0x2FF81AAD

#define KEYSTORE_HASH_TABLE_SIZE (4)

typedef struct lockableStorage {
#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
    uint32_t Rollback_securityCount;
    uint32_t Rollback_inverse;  // count inverse
#endif
#ifdef CSC_ENABLE_KEYSTORE
    key_hash_t keystore_hash_table[KEYSTORE_HASH_TABLE_SIZE];
#endif
#ifdef CSC_ENABLE_DEV_ERROR_CODES
    error_code_t error_code;
#endif
#ifdef CSC_ENABLE_UNPRIVILEGED_HASH_VERIFY
    image_hash_t image_hash;
#endif
#ifdef CSC_EXPOSE_SECURITY_PRIMITIVES
    primitive_functions_table_t functionTable;
#endif
    volatile uint32_t bootStatus;
    uint32_t crc_val;
} LockableStorage;

/* This is a read-only copy of the lockable storage struct that can be read from
 * either privileged or unprivileged code in order to perform operations based
 * on its contents.
 */
extern volatile const LockableStorage gLockStgInFlash;

#endif  // __LOCKABLE_STG_COMMON_H__
