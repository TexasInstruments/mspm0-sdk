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

volatile bool gCheckTSEVT = false;
volatile uint32_t counter = 0;
volatile uint8_t gBlink;

int main(void)
{
    /* Initialization */
    SYSCFG_DL_init();

    /* Enable the RTC interrupt at NVIC */
    NVIC_EnableIRQ(RTC_A_INT_IRQn);

    /* Start RTC clock */
    DL_RTC_A_enableClockControl(RTC_A);

    while (1) {
        __WFI();

        /* Wait in a while() loop until the time stamp interrupt triggers - Trigger a tamper event externally */
        while (gCheckTSEVT == false)
            ;

        /* Blink LED a number of times equal to amount of time stamp events detected */
        if (DL_RTC_A_getTimeStampEventCause(
                RTC_A, DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_0) ==
            DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_0) {
            for (gBlink = 0; gBlink < (2 * counter); gBlink++) {
                DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
                delay_cycles(16000000);
            }
        }
    }
}

void LFSS_IRQHandler()
{
    switch (DL_RTC_A_getPendingInterrupt(RTC_A)) {
        case DL_RTC_A_IIDX_TSEVT:
            counter++;
            gCheckTSEVT = true;
        default:
            break;
    }
}
