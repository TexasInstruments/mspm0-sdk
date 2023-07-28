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
uint16_t var_id        = 0;
uint32_t var_data      = 0;
uint32_t var_data_read = 0;

int main(void)
{
    uint32_t EEPROMEmulationState;
    uint16_t num;

    SYSCFG_DL_init();

    /* Initialize */
    EEPROMEmulationState = EEPROM_TypeB_init();
    if (EEPROMEmulationState == EEPROM_EMULATION_INIT_ERROR ||
        EEPROMEmulationState == EEPROM_EMULATION_TRANSFER_ERROR) {
        /* If there is an error, it will stop here */
        __BKPT(0);
    }

    num = 1;
    while (1) {
        /* data and identifier */
        var_id = num;
        var_data++;
        num++;
        if (num > 5) {
            num = 1;
        }

        /* Write operation */
        EEPROMEmulationState = EEPROM_TypeB_write(var_id, var_data);
        if (EEPROMEmulationState != EEPROM_EMULATION_WRITE_OK) {
            /* If there is an error, it will stop here */
            __BKPT(0);
        }

        /* Erase operation */
        if (gEEPROMTypeBEraseFlag == 1) {
            /*
             * In this demo, gEEPROMTypeBEraseFlag is checked after write
             * operation. If the group is full, it will be erased immediately
             */
            EEPROM_TypeB_eraseGroup();
            gEEPROMTypeBEraseFlag = 0;
            __BKPT(0);
        }

        /* Read operation */
        var_data_read = EEPROM_TypeB_readDataItem(var_id);
        if (gEEPROMTypeBSearchFlag == 0) {
            /* If identifier is not found, it will stop here */
            __BKPT(0);
        }

        __BKPT(0);
    }
}
