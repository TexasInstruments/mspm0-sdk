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

#include "ti_msp_dl_config.h"

/* Configure scratchpad memory with size of 32 bytes */
#define SPMEM_SIZE (32)

/* Set this variable to true following disconnect and reconnect of VDD power */
volatile bool gVDDLost      = false;
volatile bool gErrorOccured = false;
int main(void)
{
    /* Initialization */
    SYSCFG_DL_init();

    const uint8_t randomData[SPMEM_SIZE] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE,
        0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C, 0x2B, 0x7E,
        0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF,
        0x4F, 0x3C};

    if (gVDDLost) {
        /* Verify scratchpad byte was retained */
        for (int i = DL_SCRATCHPAD_MEM_WORD_0; i <= DL_SCRATCHPAD_MEM_WORD_7;
             i++) {
            for (int j = DL_SCRATCHPAD_MEM_BYTE_0;
                 j <= DL_SCRATCHPAD_MEM_BYTE_3; j++) {
                if ((DL_ScratchPad_readDataByte(LFSS, i, j)) !=
                    randomData[((i * 4) + j)]) {
                    gErrorOccured = true;
                }
            }
        }

        if (!gErrorOccured) {
            DL_GPIO_clearPins(GPIO_LEDS_PORT,
                GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
        }

        /* gErrorOcurred can be inspected to verify scratchpad data was retained */
        __BKPT(0);
    }

    /* Write data into scratchpad byte */
    for (int i = DL_SCRATCHPAD_MEM_WORD_0; i <= DL_SCRATCHPAD_MEM_WORD_7;
         i++) {
        for (int j = DL_SCRATCHPAD_MEM_BYTE_0; j <= DL_SCRATCHPAD_MEM_BYTE_3;
             j++) {
            DL_ScratchPad_disableWriteProtectByte(LFSS, i, j);
            DL_ScratchPad_writeDataByte(LFSS, i, j, randomData[((i * 4) + j)]);
            DL_ScratchPad_enableWriteProtectByte(LFSS, i, j);
        }
    }

    while (1) {
        __WFI();
    }
}
