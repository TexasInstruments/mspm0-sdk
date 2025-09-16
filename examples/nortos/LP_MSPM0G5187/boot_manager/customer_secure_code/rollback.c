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

#include "rollback.h"
#include "lockable_storage_private.h"
#include "secret.h"

#ifdef CSC_ENABLE_ROLLBACK_PROTECTION

extern LockableStorage gLockStgInRAM;

uint32_t *securityCountRAM = &(gLockStgInRAM.Rollback_securityCount);
uint32_t *inverseCountRAM  = &(gLockStgInRAM.Rollback_inverse);

volatile const uint32_t *const countFlash =
    &(gLockStgInFlash.Rollback_securityCount);
volatile const uint32_t *const countInvFlash =
    &(gLockStgInFlash.Rollback_inverse);

void Rollback_storeCount(uint32_t count)
{
    *securityCountRAM = count;
    *inverseCountRAM  = ~count;
}

Rollback_status Rollback_compareCount(uint32_t count)
{
    if ((((*countFlash ^ *countInvFlash) + 1) == 0) &&
        (*countFlash <= count)) {
        return ROLLBACK_OK;
    }
    return ROLLBACK_INVALID;
}

#endif  // CSC_ENABLE_ROLLBACK_PROTECTION
