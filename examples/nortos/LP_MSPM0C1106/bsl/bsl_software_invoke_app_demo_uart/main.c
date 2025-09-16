/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#define LED_DELAY 8000000U

volatile bool BSL_trigger_flag;

volatile uint8_t gData = 0;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    BSL_trigger_flag = false;

    while (BSL_trigger_flag == false) {
        /* Toggle LED while waiting for button press */
        delay_cycles(LED_DELAY);
        DL_GPIO_togglePins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
    }

    /*
     * When GPIO_SWITCHES1_USER_SWITCH_1_PIN is pressed, set
     * GPIO_LEDS_USER_LED_1_PIN for one second before resetting the device
     * with BOOTLOADERENTRY reset level. This will run the bootcode, which
     * will check if there is a request from the application, and then invoke
     * the bootloader.
     */
    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_setPins(GPIO_LEDS_USER_LED_2_PORT, GPIO_LEDS_USER_LED_2_PIN);
    delay_cycles(LED_DELAY);

    /* Force Reset calling BSL entry*/
    DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_BOOTLOADER_ENTRY);

    while (1) {
        __WFI();
    }
}

void GPIOA_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
        case GPIO_SWITCHES_USER_SWITCH_1_IIDX:
            /* Indicate that the button has been pressed */
            BSL_trigger_flag = true;
            break;
        default:
            break;
    }
}


void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            gData = DL_UART_Main_receiveData(UART_0_INST);
            if (gData == 0x22) {
                /* Indicate that the BSL software trigger needed*/
                BSL_trigger_flag = true;
            }

            break;
        default:
            break;
    }
}
