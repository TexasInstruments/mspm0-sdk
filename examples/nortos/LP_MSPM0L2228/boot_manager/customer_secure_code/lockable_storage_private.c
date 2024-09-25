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

#include "lockable_storage_private.h"

#include <ti/driverlib/driverlib.h>

LOCKABLE_STG const LockableStorage gLockStgInFlash;

LockableStorage __attribute__((used)) gLockStgInRAM;

extern void mcubootFail(void);

void Lock_writeOut(void)
{
    /* Store the resultant information in flash lock sector for BOTH BANKS */
    DL_FLASHCTL_COMMAND_STATUS flashStat;

    DL_FlashCTL_unprotectSector(
        FLASHCTL, CSC_LOCK_STORAGE_ADDR, DL_FLASHCTL_REGION_SELECT_MAIN);
    /* Erase sector in main memory */
    DL_FlashCTL_eraseMemoryFromRAM(
        FLASHCTL, CSC_LOCK_STORAGE_ADDR, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

    flashStat = DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
        FLASHCTL, CSC_LOCK_STORAGE_ADDR, (uint32_t *) &gLockStgInRAM,
        sizeof(gLockStgInRAM) / 4, DL_FLASHCTL_REGION_SELECT_MAIN);

    /* It is necessary for the CSC to program lockable flash in both
     * sectors for successful boots in the future. If this has not been
     * successful, we force a failure. */
    if (!flashStat) {
        mcubootFail();
    }

    /* Store the resultant information in flash lock sector for BOTH BANKS */
    DL_FlashCTL_unprotectSector(FLASHCTL,
        (CSC_LOCK_STORAGE_ADDR + CSC_BANK_SIZE),
        DL_FLASHCTL_REGION_SELECT_MAIN);
    /* Erase sector in main memory */
    DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL,
        (CSC_LOCK_STORAGE_ADDR + CSC_BANK_SIZE),
        DL_FLASHCTL_COMMAND_SIZE_SECTOR);

    flashStat = DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
        FLASHCTL, (CSC_LOCK_STORAGE_ADDR + CSC_BANK_SIZE),
        (uint32_t *) &gLockStgInRAM, sizeof(gLockStgInRAM) / 4,
        DL_FLASHCTL_REGION_SELECT_MAIN);

    /* It is necessary for the CSC to program lockable flash in both
     * sectors for successful boots in the future. If this has not been
     * successful, we force a failure. */
    if (!flashStat) {
        mcubootFail();
    }
}
