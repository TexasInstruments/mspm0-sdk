/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 * Number of bytes for SPI packet size
 * The packet will be transmitted by the SPI controller.
 * This example uses FIFOs with polling, and the maximum FIFO size is 4.
 * Refer to interrupt examples to handle larger packets.
 */
#define SPI_PACKET_SIZE (4)

/* Data for SPI to transmit */
uint8_t gTxPacket[SPI_PACKET_SIZE] = {'M', 'S', 'P', '!'};

/* Data received from SPI Peripheral */
volatile uint8_t gRxPacket[SPI_PACKET_SIZE];
int main(void)
{
    SYSCFG_DL_init();

    /* Fill TX FIFO with data and transmit to SPI Peripheral */
    DL_SPI_fillTXFIFO8(SPI_0_INST, &gTxPacket[0], SPI_PACKET_SIZE);

    /* Wait until all bytes have been transmitted and the TX FIFO is empty */
    while (DL_SPI_isBusy(SPI_0_INST))
        ;

    /*
     * Wait to receive the SPI data
     * This loop expects SPI_PACKET_SIZE bytes
     */
    for (uint8_t i = 0; i < SPI_PACKET_SIZE; i++) {
        gRxPacket[i] = DL_SPI_receiveDataBlocking8(SPI_0_INST);
    }
    /*
     * If this example is used with the spi_peripheral_multibyte_fifo_poll
     * example, the expected data to receive in gRxPacket is
     * {0x01, 0x02, 0x03, 0x04}.
     */

    /* If write and read were successful, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT,
            GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
        delay_cycles(16000000);
    }
}
