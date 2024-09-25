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
 * Number of bytes to send from Target to Controller.
 *  The packet will be sent to the Controller during a Read request.
 *  This example uses FIFO with polling, and the maximum FIFO size is 4.
 *  Refer to interrupt examples to handle larger packets.
 */
#define I2C_TX_PACKET_SIZE (4)

/*
 * Number of bytes to receive from Controller.
 *  This example uses FIFO with polling, and the maximum FIFO size is 4.
 *  The example will wait until it receives all bytes.
 *  Refer to interrupt examples to handle larger packets
 */
#define I2C_RX_PACKET_SIZE (4)

/* Data sent to Controller in response to Read transfer */
uint8_t gTxPacket[I2C_TX_PACKET_SIZE] = {'M', 'S', 'P', 'M'};

/* Data received from Controller during a Write transfer */
uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

int main(void)
{
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    /*
     * Fill FIFO with data.
     * Note that transactions are initiated by the Controller, so this function
     * only fills the buffer and the Target device will send this data when
     * requested by the Controller.
     */
    DL_I2C_fillTargetTXFIFO(I2C_INST, &gTxPacket[0], I2C_TX_PACKET_SIZE);

    /*
     * Wait to receive data from the Controller.
     * This loop expects I2C_RX_PACKET_SIZE bytes.
     */
    for (uint8_t i = 0; i < I2C_RX_PACKET_SIZE; i++) {
        while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST))
            ;
        gRxPacket[i] = DL_I2C_receiveTargetData(I2C_INST);
    }

    /*
     * Wait until all bytes written to TX FIFO are sent after a successful
     * request.
     */
    while (!DL_I2C_isTargetTXFIFOEmpty(I2C_INST))
        ;

    /* If write and read were successful, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(12000000);
    }
}
