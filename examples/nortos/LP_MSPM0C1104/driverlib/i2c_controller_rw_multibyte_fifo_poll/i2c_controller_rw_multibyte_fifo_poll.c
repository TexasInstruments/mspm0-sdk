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
 * Number of bytes to send from Controller to target.
 *  This example uses FIFO with polling, and the maximum FIFO size is 4.
 *  Refer to interrupt examples to handle larger packets
 */
#define I2C_TX_PACKET_SIZE (4)

/*
 * Number of bytes to received from target.
 *  This example uses FIFO with polling, and the maximum FIFO size is 4.
 *  Refer to interrupt examples to handle larger packets
 */
#define I2C_RX_PACKET_SIZE (4)

/* Data sent to the Target */
uint8_t gTxPacket[I2C_TX_PACKET_SIZE] = {0x01, 0x02, 0x03, 0x04};

/* Data received from Target */
volatile uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

/* I2C Target address */
#define I2C_TARGET_ADDRESS (0x48)

int main(void)
{
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    /*
     * Fill FIFO with data. This example will send a MAX of 4 bytes since it
     * doesn't handle the case where FIFO is full
     */
    DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], I2C_TX_PACKET_SIZE);

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    DL_I2C_startControllerTransfer(I2C_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_TX, I2C_TX_PACKET_SIZE);

    /* Poll until the Controller writes all bytes */
    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* LED will remain high if there is an error */
        __BKPT(0);
    }

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Add delay between transfers */
    delay_cycles(1000);

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(I2C_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_RX, I2C_RX_PACKET_SIZE);

    /*
     * Receive all bytes from target. LED will remain high if not all bytes
     * are received
     */
    for (uint8_t i = 0; i < I2C_RX_PACKET_SIZE; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST))
            ;
        gRxPacket[i] = DL_I2C_receiveControllerData(I2C_INST);
    }

    /* If write and read were successful, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT,
            GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
        delay_cycles(12000000);
    }
}
