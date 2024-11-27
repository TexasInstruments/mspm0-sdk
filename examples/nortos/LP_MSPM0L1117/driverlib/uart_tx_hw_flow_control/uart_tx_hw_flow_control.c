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

/* Number of bytes for UART packet size */
#define UART_PACKET_SIZE (26)

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)

/* Data packets to transmit over UART */
uint8_t gTxPacket1[UART_PACKET_SIZE] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
    'x', 'y', 'z'};
uint8_t gTxPacket2[UART_PACKET_SIZE] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z'};

/*
 * Helper function to transmit a packet of data, and ensure that the
 * transmitter is ready before sending data
 */
void transmitPacketBlocking(uint8_t *buffer, uint32_t size)
{
    uint32_t i;
    for (i = 0; i < size; i++) {
        /*
         * The transmitter will automatically check that the CTS signal is
         * asserted (low) before transmitting the next byte. If CTS is
         * de-asserted (high), the byte will not be transmitted out of the TX
         * FIFO. Therefore block if the TX FIFO is full before attempting
         * to fill it.
         */
        DL_UART_Main_transmitDataBlocking(UART_0_INST, buffer[i]);
    }
}

int main(void)
{
    SYSCFG_DL_init();

    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);
    transmitPacketBlocking(gTxPacket1, UART_PACKET_SIZE);
    /* Spin for some cycles to pause transmission between packets */
    delay_cycles(100000);

    transmitPacketBlocking(gTxPacket2, UART_PACKET_SIZE);

    /* Clear LED and USER_TEST pin */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
    /* Transmission of all data is complete */
    while (1) {
        __WFI();
    }
}
