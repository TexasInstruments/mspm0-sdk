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

#include "ti_msp_dl_config.h"

/*
 * DAC8 Reference Voltage in mV
 *  Adjust this value according to VDDA
 */
#define COMP_INST_REF_VOLTAGE_mV (3300)
/*
 * DAC8 static output voltage in mV.
 *  Adjust output as needed.
 */
#define COMP_INST_DAC8_OUTPUT_VOLTAGE_mV (1000)

/* Maximum number of captures to record */
#define MAX_CAPTURES (10)

volatile uint8_t gCaptureCounter;
volatile uint16_t gCaptureArray[MAX_CAPTURES];
volatile bool gTimeout;

int main(void)
{
    uint32_t dacValue;

    SYSCFG_DL_init();

    /*
     * Set output voltage:
     *  DAC value (8-bits) = DesiredOutputVoltage x 255
     *                          -----------------------
     *                              ReferenceVoltage
     */
    dacValue =
        (COMP_INST_DAC8_OUTPUT_VOLTAGE_mV * 255) / COMP_INST_REF_VOLTAGE_mV;

    DL_COMP_setDACCode0(COMP_INST, dacValue);
    DL_COMP_enable(COMP_INST);

    NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);

    /* Start counting */
    gCaptureCounter = 0;
    gTimeout        = false;
    DL_TimerG_startCounter(CAPTURE_0_INST);

    while (false == gTimeout) {
        __WFE();
    }

    /*
     * This breakpoint is reached after the capture window, which is set by
     *   default to 1s.
     * gCaptureCounter will contain the number of captures during this window,
     *  not exceeding a maximum of MAX_CAPTURES(10).
     * gCaptureArray will contain the captures. Note that the timer counts down.
     *
     */
    __BKPT(0);
    while (1) {
        __WFI();
    }
}

void CAPTURE_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_0_INST)) {
        case DL_TIMERG_IIDX_CC0_DN:
            if (gCaptureCounter < MAX_CAPTURES) {
                gCaptureArray[gCaptureCounter++] =
                    DL_Timer_getCaptureCompareValue(
                        CAPTURE_0_INST, DL_TIMER_CC_0_INDEX);
            }
            break;
        case DL_TIMERG_IIDX_ZERO:
            gTimeout = true;
            break;
        default:
            break;
    }
}
