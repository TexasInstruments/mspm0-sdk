///*
// * Copyright (c) 2021, Texas Instruments Incorporated
// * All rights reserved.
// *
// * Redistribution and use in source and binary forms, with or without
// * modification, are permitted provided that the following conditions
// * are met:
// *
// * *  Redistributions of source code must retain the above copyright
// *    notice, this list of conditions and the following disclaimer.
// *
// * *  Redistributions in binary form must reproduce the above copyright
// *    notice, this list of conditions and the following disclaimer in the
// *    documentation and/or other materials provided with the distribution.
// *
// * *  Neither the name of Texas Instruments Incorporated nor the names of
// *    its contributors may be used to endorse or promote products derived
// *    from this software without specific prior written permission.
// *
// * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// */
//
#include "ti_msp_dl_config.h"

#define CRCP_NUM_OF_WORDS (3)
#define CRCP_EXPECTED_RESULT (0x2C81)

uint16_t gData16[CRCP_NUM_OF_WORDS] = {0x1234, 0x5678, 0x9012};
const uint16_t gCrcpSeed            = CRCP_SEED;

int main(void)
{
    volatile uint16_t crcpChecksum;
    volatile bool crcpChecksumMatch __attribute__((unused));

    SYSCFG_DL_init();

    crcpChecksum = DL_CRCP_calculateBlock16(
        CRCP0, gCrcpSeed, &gData16[0], CRCP_NUM_OF_WORDS);

    if (CRCP_EXPECTED_RESULT == crcpChecksum) {
        crcpChecksumMatch = true;
    } else {
        crcpChecksumMatch = false;
    }

    /* Stop the debugger to verify crcpChecksumMatch == true */
    __BKPT(0);

    while (1) {
        __WFI();
    }
}
