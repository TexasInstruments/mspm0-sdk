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


/*
 * Subsystem example shows basic WDT functionality.
 * It enables two input pins, a WDT_Start and a WDT_WDI, as well
 * as an WDT_WDO output pin to be connected to the reset of the external device.
 *
 *
 * PA27: Input - WDT_Start
 * PA24: Input - WDT_WDI
 * PA0: Output - WDT_WDO (open-drain)
 *
 */

#include "watchdog_hal.h"

int main(void)
{
    SYSCFG_DL_init();
    DL_GPIO_disableInterrupt(GPIOA, WDT_WDI_PIN);

    /* Enable start signal capture */
    DL_GPIO_enableInterrupt(GPIOA, WDT_Start_PIN);
    NVIC_EnableIRQ(WDT_INT_IRQN);

    while (1) {
        __WFI();
    }
}

void watchdog_timer_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(watchdog_timer_INST)) {
        case DL_TIMER_IIDX_ZERO:
            /* WDT Timed out */
            watchdog_timeout();
            break;
        default:
            break;
    }
}

void GPIOA_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(GPIOA)) {
        case WDT_Start_IIDX:
            /* WDT start pin detected  */
            watchdog_start();
            break;

        case WDT_WDI_IIDX:
            /* WDT feed pin detected  */
            watchdog_reload();
            break;
        
        default:
            break;
    }
}
