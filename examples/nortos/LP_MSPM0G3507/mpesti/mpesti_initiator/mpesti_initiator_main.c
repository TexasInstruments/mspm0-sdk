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
 * Configure the internal loopback mode.
 * Note that data received on the UART RXD IO pin will be ignored when
 * loopback is enabled.
 */
#define ENABLE_LOOPBACK_MODE true

/*
 * Number of bytes for UART packet size
 * The packet will be transmitted by the UART.
 * This example uses FIFOs with polling, and the maximum FIFO size is 4.
 * Refer to interrupt examples to handle larger packets.
 */
#define UART_PACKET_SIZE (1)

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)

/* Data for UART to transmit */
uint8_t txPacket[2] = {0x5A, 0x00};

/* Data received from UART */
uint8_t rxPacket[32];
volatile uint8_t isButtonPressed = 0;
volatile uint8_t toggleBtnPress  = 0;
volatile bool broadcastCmd       = false;

volatile bool gBeginTest = false;
volatile bool gUartSent  = false;

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(GPIO_Switches_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_Switches_GPIOB_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);

    /* Set LED to indicate start of transfer */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    while (1) {
        if (isButtonPressed == 1) {
            if (broadcastCmd == true) {
                /* Fills TX FIFO with data and transmits the data */
                DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], 2);
                broadcastCmd = false;
            } else {
                /* Fills TX FIFO with data and transmits the data */
                DL_UART_Main_fillTXFIFO(
                    UART_0_INST, &txPacket[0], UART_PACKET_SIZE);
            }
            isButtonPressed = 0;
        }
    }
}

void GROUP1_IRQHandler(void)
{
    uint32_t interrupts = DL_Interrupt_getStatusGroup(DL_INTERRUPT_GROUP_1,
        (DL_INTERRUPT_GROUP1_GPIOA | DL_INTERRUPT_GROUP1_GPIOB));
    if (interrupts & DL_INTERRUPT_GROUP1_GPIOA) {
        if (toggleBtnPress == 0) {
            toggleBtnPress = 1;
            txPacket[0]    = 0;
        } else {
            toggleBtnPress = 0;
            txPacket[0]    = 0xFF;
            txPacket[1]    = 0x00;
            broadcastCmd   = true;
        }
        isButtonPressed = 1;
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        DL_Interrupt_clearGroup(
            DL_INTERRUPT_GROUP_1, DL_INTERRUPT_GROUP1_GPIOA);
    } else if (interrupts & DL_INTERRUPT_GROUP1_GPIOB) {
        isButtonPressed = 1;
        txPacket[0]     = 1;
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        DL_Interrupt_clearGroup(
            DL_INTERRUPT_GROUP_1, DL_INTERRUPT_GROUP1_GPIOB);
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_IIDX_RX:
            DL_UART_drainRXFIFO(UART_0_INST, &rxPacket[0], 32);
            break;
        default:
            break;
    }
}
