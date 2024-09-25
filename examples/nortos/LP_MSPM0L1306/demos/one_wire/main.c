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

#include "one_wire_target.h"
#include "ti_msp_dl_config.h"

OneWireTarget gOW_A;

int main(void)
{
    SYSCFG_DL_init();

    /****************************************************************/
    /*                                                              */
    /* Interface A:                                                 */
    /* Initialize the registration number for 1-wire                */
    /* Devices must be in order or 1-wire search ROM priority       */
    /*                                                              */
    /****************************************************************/

    // Tell 1-wire configuration which HW to use
    gOW_A.config.timerInst   = CAPTURE_A_INST;
    gOW_A.config.outputPort  = OW_OUTPUT_A_PORT;
    gOW_A.config.outputPin   = OW_OUTPUT_A_OD_A_PIN;
    gOW_A.config.deviceCount = 4;

    // Enable interrupts
    NVIC_EnableIRQ(CAPTURE_A_INST_INT_IRQN);

    // First device
    gOW_A.config.regNums[0][0] = 0xa6;  // 0xa7; // Family number
    // 1st byte is the least significant byte
    gOW_A.config.regNums[0][1] = 0x11;  // 1st byte of ROM code
    gOW_A.config.regNums[0][2] = 0xb4;  // 2nd byte of ROM code
    gOW_A.config.regNums[0][3] = 0x60;  // 3rd byte of ROM code
    gOW_A.config.regNums[0][4] = 0x02;  // 4th byte of ROM code
    gOW_A.config.regNums[0][5] = 0x00;  // 5th byte of ROM code
    gOW_A.config.regNums[0][6] =
        0x50;  // 6th (most significant) byte of ROM code
    oneWireTarget_computeRegNumCRC(&gOW_A, 0);

    if (gOW_A.config.deviceCount > 1) {
        gOW_A.config.regNums[1][0] = 0xa3;  // 0xa7; // Family number
        // 1st byte is the least significant byte
        gOW_A.config.regNums[1][1] = 0x11;  // 0x11; // 1st byte of ROM code
        gOW_A.config.regNums[1][2] = 0xb4;  // 2nd byte of ROM code
        gOW_A.config.regNums[1][3] = 0x60;  // 3rd byte of ROM code
        gOW_A.config.regNums[1][4] = 0x02;  // 4th byte of ROM code
        gOW_A.config.regNums[1][5] = 0x00;  // 5th byte of ROM code
        gOW_A.config.regNums[1][6] =
            0x50;  // 6th (most significant) byte of ROM code
        oneWireTarget_computeRegNumCRC(&gOW_A, 1);

        gOW_A.config.conflicts[0][0] =
            0x01;  // Conflict between device 0 and device 1 at bit position 0
        gOW_A.config.conflicts[1][0] =
            0x01;  // Conflict between device 0 and device 1 at bit position 0
    }
    if (gOW_A.config.deviceCount > 2) {
        gOW_A.config.regNums[2][0] = 0xa7;  // 0xa7; // Family number
        // 1st byte is the least significant byte
        gOW_A.config.regNums[2][1] = 0x11;  // 0x11; // 1st byte of ROM code
        gOW_A.config.regNums[2][2] = 0xb4;  // 2nd byte of ROM code
        gOW_A.config.regNums[2][3] = 0x60;  // 3rd byte of ROM code
        gOW_A.config.regNums[2][4] = 0x02;  // 4th byte of ROM code
        gOW_A.config.regNums[2][5] = 0x00;  // 5th byte of ROM code
        gOW_A.config.regNums[2][6] =
            0x50;  // 6th (most significant) byte of ROM code
        oneWireTarget_computeRegNumCRC(&gOW_A, 2);

        gOW_A.config.conflicts[1][0] =
            0x05;  // Conflict between device 0 and device 1 at bit position 0
                   // Conflict between device 1 and device 2 at bit position 2
        gOW_A.config.conflicts[2][0] =
            0x04;  // Conflict between device 1 and device 2 at bit position 2
    }
    if (gOW_A.config.deviceCount > 3) {
        gOW_A.config.regNums[3][0] = 0xbf;  // 0xa7; // Family number
        // 1st byte is the least significant byte
        gOW_A.config.regNums[3][1] = 0xde;  // 0x11; // 1st byte of ROM code
        gOW_A.config.regNums[3][2] = 0xad;  // 2nd byte of ROM code
        gOW_A.config.regNums[3][3] = 0xbe;  // 3rd byte of ROM code
        gOW_A.config.regNums[3][4] = 0xef;  // 4th byte of ROM code
        gOW_A.config.regNums[3][5] = 0x33;  // 5th byte of ROM code
        gOW_A.config.regNums[3][6] =
            0x55;  // 6th (most significant) byte of ROM code
        oneWireTarget_computeRegNumCRC(&gOW_A, 3);

        gOW_A.config.conflicts[2][0] =
            0x0C;  // Conflict between device 1 and device 2 at bit position 2
                   // Conflict between device 2 and device 3 at bit position 3
        gOW_A.config.conflicts[3][0] =
            0x08;  // Conflict between device 2 and device 3 at bit position 3
    }
    oneWireTarget_init(&gOW_A);

    while (1) {
        oneWireTarget_checkState(&gOW_A);
        __WFE();
    }
}

/*
 * TimerGx handler for 1wire interface. Configured in sysconfig for capture combined mode
 */
void CAPTURE_A_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_A_INST)) {
        case DL_TIMERG_IIDX_CC0_DN:
            oneWireTarget_pulseFinished(&gOW_A);
            break;
        case DL_TIMERG_IIDX_LOAD:
            oneWireTarget_pulseStarted(&gOW_A);
            break;
        default:
            break;
    }
}
