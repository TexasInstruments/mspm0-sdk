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

#include <ti/eeprom/emulation_type_b/eeprom_emulation_type_b.h>
#include "ti_msp_dl_config.h"

/* Address in main memory to write to */
#define MAIN_BASE_ADDRESS (0x00001000)

/* Data and identifier used for test */
uint16_t var1_id       = 0x0011;
uint32_t var1          = 0xaaaa1234;
uint16_t var2_id       = 0x0022;
uint32_t var2          = 0xbbbb4321;
uint16_t var3_id       = 0x0033;
uint32_t var3          = 0xcccc0000;
uint16_t var4_id       = 0x0044;
uint32_t var4          = 0xddddffff;
uint16_t var_id        = 0;
uint32_t var_data      = 0;
uint32_t var_data_read = 0;

/* 64bytes data for unexpected case*/
uint32_t DataArray[16] = {0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
    0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678,
    0x00FEDCBA, 0x87654321, 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321};

int main(void)
{
    uint32_t EEPROMEmulationState;
    uint16_t count, num;

    SYSCFG_DL_init();

    /*
     *
     * 1. Test EEPROM_TypeB_init under abnormal conditions
     *
     */
    DL_FlashCTL_programMemoryBlocking64WithECCGenerated(FLASHCTL,
        MAIN_BASE_ADDRESS, &DataArray[0], 16, DL_FLASHCTL_REGION_SELECT_MAIN);
    __BKPT(0);
    /* In memory browser, search the address 0x00001000
     * Before EEPROM_TypeB_init, Flash is populated with the following 64-byte irrelevant data:
     * 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
     * 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
     * 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321,
     * 0xABCDEF00, 0x12345678, 0x00FEDCBA, 0x87654321
     *
     * EEPROM_TypeB_init will erase all the irrelevant data
     * */

    EEPROMEmulationState = EEPROM_TypeB_init();
    if (EEPROMEmulationState == EEPROM_EMULATION_INIT_ERROR ||
        EEPROMEmulationState == EEPROM_EMULATION_TRANSFER_ERROR) {
        __BKPT(0);
    }
    __BKPT(0);
    /*
     * After EEPROM_TypeB_init, flash is all erased
     * global variables are as below:
     *      gActiveGroupNum = 1
     *      gActiveDataItemNum = 0
     *      gEEPROMTypeBSearchFlag = 0
     *      gEEPROMTypeBEraseFlag = 0
     * */

    /*
     *
     * 2. Test normal initialization
     *
     */
    EEPROMEmulationState = EEPROM_TypeB_write(var1_id, var1);
    if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
        __BKPT(0);
    }
    EEPROMEmulationState = EEPROM_TypeB_write(var2_id, var2);
    if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
        __BKPT(0);
    }
    EEPROMEmulationState = EEPROM_TypeB_write(var3_id, var3);
    if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
        __BKPT(0);
    }
    EEPROMEmulationState = EEPROM_TypeB_write(var3_id, var4);
    if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
        __BKPT(0);
    }
    __BKPT(0);
    /* After 4 times EEPROM_TypeB_write, 4 new data items are created
     * There are three different identifier:
     *      0x0011, 0x0022, 0x0033
     *
     * Flash is as below:
     *      group1(start address 0x00001000):
     *          0x00000000, 0xFFFFFFFF, 0x00000011, 0xAAAA1234,
     *          0x00000022, 0xBBBB4321, 0x00000033, 0xCCCC0000,
     *          0x00000033, 0xDDDDFFFF
     *      group2(start address 0x00001800):
     *          all erased
     *      group3(start address 0x00002000):
     *          all erased
     *
     * global variables are as below:
     *      gActiveGroupNum = 1
     *      gActiveDataItemNum = 4
     *      gEEPROMTypeBSearchFlag = 0
     *      gEEPROMTypeBEraseFlag = 0
     * */

    EEPROMEmulationState = EEPROM_TypeB_init();
    if (EEPROMEmulationState == EEPROM_EMULATION_INIT_ERROR &&
        EEPROMEmulationState == EEPROM_EMULATION_TRANSFER_ERROR) {
        __BKPT(0);
    }
    __BKPT(0);
    /* After EEPROM_TypeB_init, data items are transferred to the next group
     * There are three different identifier:
     *      0x0011, 0x0022, 0x0033
     *
     * Flash is as below:
     *      group1(start address 0x00001000):
     *          all erased
     *      group2(start address 0x00001800):
     *          0x00000000, 0xFFFFFFFF, 0x00000033, 0xDDDDFFFF,
     *          0x00000022, 0xBBBB4321, 0x00000011, 0xAAAA1234
     *      group3(start address 0x00002000):
     *          all erased
     *
     * global variables are as below:
     *      gActiveGroupNum = 2
     *      gActiveDataItemNum = 3
     *      gEEPROMTypeBSearchFlag = 0
     *      gEEPROMTypeBEraseFlag = 0
     * */

    /*
     *
     * 3. Test EEPROM_TypeB_write and erase operation
     *
     */
    for (count = 0; count < 63; count++) {
        for (num = 0; num < 4; num++) {
            var_id               = num;
            EEPROMEmulationState = EEPROM_TypeB_write(var_id, var_data);
            if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
                __BKPT(0);
            }
        }
        var_data++;
    }
    __BKPT(0);
    /* After 252 times EEPROM_TypeB_write, group2 is full and
     * latest data items are transferred to group3.
     *
     * There are seven different identifier:
     *      0x0011, 0x0022, 0x0033, 0x0000, 0x0001, 0x0002, 0x0003
     *
     * Flash is as below:
     *      group1(start address 0x00001000):
     *          all erased
     *      group2(start address 0x00001800):
     *          0x00000000, 0x00000000, 0x00000033, 0xDDDDFFFF,
     *          0x00000022, 0xBBBB4321, 0x00000011, 0xAAAA1234,
     *          0x00000000, 0x00000000, 0x00000001, 0x00000000,
     *          0x00000002, 0x00000000, 0x00000003, 0x00000000,
     *          ...
     *          0x00000000, 0x0000003E, 0x00000001, 0x0000003E,
     *          0x00000002, 0x0000003E, 0x00000003, 0x0000003E,
     *      group3(start address 0x00002000):
     *          0x00000000, 0xFFFFFFFF, 0x00000003, 0x0000003E,
     *          0x00000002, 0x0000003E, 0x00000001, 0x0000003E,
     *          0x00000000, 0x0000003E, 0x00000011, 0xAAAA1234,
     *          0x00000022, 0xBBBB4321, 0x00000033, 0xDDDDFFFF
     *
     * global variables are as below:
     *      gActiveGroupNum = 3
     *      gActiveDataItemNum = 7
     *      gEEPROMTypeBSearchFlag = 0
     *      gEEPROMTypeBEraseFlag = 1
     * */

    if (gEEPROMTypeBEraseFlag == 1) {
        EEPROM_TypeB_eraseGroup();
        gEEPROMTypeBEraseFlag = 0;
        __BKPT(0);
        /* While gEEPROMTypeBEraseFlag = 1, EEPROM_TypeB_eraseGroup
         * will be performed and group2 will be erased. After erase,
         * gEEPROMTypeBEraseFlag will be clear.
         *
         * There are seven different identifier:
         *      0x0011, 0x0022, 0x0033, 0x0000, 0x0001, 0x0002, 0x0003
         *
         * Flash is as below:
         *      group1(start address 0x00001000):
         *          all erased
         *      group2(start address 0x00001800):
         *          all erased
         *      group3(start address 0x00002000):
         *          0x00000000, 0xFFFFFFFF, 0x00000003, 0x0000003E,
         *          0x00000002, 0x0000003E, 0x00000001, 0x0000003E,
         *          0x00000000, 0x0000003E, 0x00000011, 0xAAAA1234,
         *          0x00000022, 0xBBBB4321, 0x00000033, 0xDDDDFFFF
         *
         * global variables are as below:
         *      gActiveGroupNum = 3
         *      gActiveDataItemNum = 7
         *      gEEPROMTypeBSearchFlag = 0
         *      gEEPROMTypeBEraseFlag = 0
         * */
    }

    /*
     *
     * 4. Test EEPROM_TypeB_readDataItem
     *
     */
    var_data_read = EEPROM_TypeB_readDataItem(var3_id);
    if (gEEPROMTypeBSearchFlag == 0) {
        __BKPT(0);
    }
    __BKPT(0);
    /* Because var3_id exists, When using EEPROM_TypeB_readDataItem to read var3_id, it will be:
     *      var_data_read = 0xDDDDFFFF
     *      gEEPROMTypeBSearchFlag = 1
     * */

    var_data_read = EEPROM_TypeB_readDataItem(var4_id);
    if (gEEPROMTypeBSearchFlag == 0) {
        __BKPT(0);
    }
    __BKPT(0);
    /* Because var4_id does not exist, When using EEPROM_TypeB_readDataItem to read var4_id, it will be:
     *      var_data_read = 0
     *      gEEPROMTypeBSearchFlag = 0
     * */

    /*
     *
     * 5. Test a general usage scenario
     *
     */
    while (1) {
        for (count = 0; count < 31; count++) {
            for (num = 0; num < 4; num++) {
                var_id               = num;
                EEPROMEmulationState = EEPROM_TypeB_write(var_id, var_data);
                if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
                    __BKPT(0);
                }
            }
            var_data++;
        }
        __BKPT(0);
        /* Before enter the loop, there are 7 data items. Each loop will take 124 times
         * EEPROM_TypeB_write. After 2 cycles the group will be full and then
         * transfer is performed. After transfer, 7 items with different identifier exist
         * in next active group. Therefore, every 2 cycles will fill the group and then
         * transfer is performed.
         *
         * */

        if (gEEPROMTypeBEraseFlag == 1) {
            /* In this demo, when the group is full, it will be erased immediately*/
            EEPROM_TypeB_eraseGroup();
            gEEPROMTypeBEraseFlag = 0;
            __BKPT(0);
        }
    }
}
