/*
 * Copyright (c) 2021-2025, Texas Instruments Incorporated
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

#include <ti/eeprom/emulation_type_a/eeprom_emulation_type_a.h>
#include "ti_msp_dl_config.h"

/* The starting emulation address is defined in the eeprom_emulation_type_a.h
 * header file. It is 0x2000. If the example needs to change its location, it is
 * necessary to define EEPROM_EMULATION_ADDRESS at the project level or change
 * the value in the header file.
 */

#define SECTOR_SIZE (1024)

#define NUMBER_OF_WRITES ((SECTOR_SIZE / EEPROM_EMULATION_DATA_SIZE) + 1)

/* element headers */
const uint32_t active_header[]    = {0x0000ffff, 0x0000ffff};
const uint32_t recording_header[] = {0x0000ffff, 0xffffffff};
const uint32_t used_header[]      = {0x00000000, 0x0000ffff};

const uint32_t recording_data[] = {0x9ec09d16, 0xda7abbbb};
const uint32_t used_data[]      = {0x012345ed, 0xda7acccc};

#if (EEPROM_EMULATION_RECORD_SIZE == 256)
/* 248bytes data for 256bytes record size*/
const uint32_t DataArray[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)] = {
    0x12340000, 0x43210fff, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678};

#elif (EEPROM_EMULATION_RECORD_SIZE == 128)
/* 120bytes data for 128bytes record size*/
const uint32_t DataArray[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)] = {
    0x12340000, 0x43210fff, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678};

#elif (EEPROM_EMULATION_RECORD_SIZE == 64)
/* 56bytes data for 64bytes record size*/
const uint32_t DataArray[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)] = {
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678};

#else
#error "invalid EEPROM_EMULATION_RECORD_SIZE. Must be 64, 128, or 256"
#endif

uint32_t EEPROMEmulationBuffer[EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t)];

/* 64bytes data for unexpected case*/
const uint32_t UnexpectedDataArray[16] = {0xABCDEF00, 0x12345678, 0x00FEDCBA,
    0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00,
    0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA,
    0x87654321};

int main(void)
{
    uint32_t EEPROMEmulationState;

    SYSCFG_DL_init();

    /*
     *
     * 1. Test EEPROM_TypeA_init with previous unused data present.
     *
     */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_eraseMemory(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

    /* seed with old used data */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        EEPROM_EMULATION_ADDRESS, (uint32_t *) &used_header[0], 2,
        DL_FLASHCTL_REGION_SELECT_MAIN);

    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        EEPROM_EMULATION_ADDRESS + 8, (uint32_t *) &used_data[0], 2,
        DL_FLASHCTL_REGION_SELECT_MAIN);

    /* seed with active data */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        EEPROM_EMULATION_ADDRESS + EEPROM_EMULATION_RECORD_SIZE,
        (uint32_t *) &active_header[0], 2, DL_FLASHCTL_REGION_SELECT_MAIN);

    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        EEPROM_EMULATION_ADDRESS + EEPROM_EMULATION_RECORD_SIZE + 8,
        (uint32_t *) &DataArray[0],
        EEPROM_EMULATION_DATA_SIZE / sizeof(uint32_t),
        DL_FLASHCTL_REGION_SELECT_MAIN);

    /* seed with recording data */
    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        EEPROM_EMULATION_ADDRESS + (EEPROM_EMULATION_RECORD_SIZE * 2),
        (uint32_t *) &recording_header[0], 2, DL_FLASHCTL_REGION_SELECT_MAIN);

    DL_FlashCTL_unprotectSector(
        FLASHCTL, EEPROM_EMULATION_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        EEPROM_EMULATION_ADDRESS + (EEPROM_EMULATION_RECORD_SIZE * 2) + 8,
        (uint32_t *) &recording_data[0], 2, DL_FLASHCTL_REGION_SELECT_MAIN);

    __BKPT(0);
    /*
     * In memory browser, search the address 0x00002000
     * Before EEPROM_TypeA_init, flash is set as:
     * Sector 1:
     *      record 1: Used
     *      record 2: Active
     *      record 3: Recording
     *      rest:     Erased
     * Sector 2: all erased
     * EEPROM_TypeA_init will find the active record, and repair the format
     */

    EEPROMEmulationState = EEPROM_TypeA_init(&EEPROMEmulationBuffer[0]);
    if (EEPROMEmulationState != EEPROM_EMULATION_INIT_OK) {
        __BKPT(0);
    }
    __BKPT(0);
    /* After EEPROM_TypeA_init, flash will be:
     * Sector 1: all erased
     * Sector 2:
     *      record 1: Active
     *      rest:     Erased
     * EEPROMEmulationBuffer will also copy the active record's data
     * */

    /* Test for EEPROM_TypeA_writeData */

    /* Write NUMBER_OF_WRITE times to the EEPROM region specified in memory */
    for (uint32_t counter = 0; counter < NUMBER_OF_WRITES; counter++) {
        /* Change the content of EEPROMEmulationBuffer */
        EEPROMEmulationBuffer[0] += 1;
        EEPROMEmulationBuffer[1] += 0x1000;
        /*
         * Before EEPROM_TypeA_writeData, the EEPROMEmulationBuffer is
         * updated by user
         */

        /* Store the EEPROMEmulationBuffer to the flash to be a new record*/
        EEPROMEmulationState =
            EEPROM_TypeA_writeData(&EEPROMEmulationBuffer[0]);
        if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
            __BKPT(0);
        }
        if (gEEPROMTypeAEraseFlag == 1) {
            /* In this demo, when the sector is full, it will be erased immediately*/
            EEPROM_TypeA_eraseLastSector();
            gEEPROMTypeAEraseFlag = 0;
        }
        __BKPT(0);
        /*
         * After EEPROM_TypeA_writeData, there will be a new record in
         * flash, and the last record's header is updated to the used state.
         * If the write triggers a new sector, the previous sector should be
         * all erased, with one active entry in the new sector.
         */
    }

    while (1) {
        __WFI();
    }
}
