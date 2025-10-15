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

/* I2C Definitions */
#define I2C_TX_PACKET_SIZE (2)
#define I2C_RX_PACKET_SIZE (1)
#define I2C_IO_EXPANDER_ADDRESS (0x48)

/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;
/* Data sent to the IO Expander */
uint8_t gTxPacket[I2C_TX_PACKET_SIZE];
/* Data received from IO Expander */
uint8_t gRxPacket[I2C_RX_PACKET_SIZE] = {0x00}; /* initialized to 0x00 so LED is OFF */
/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount;
/* Indicates status of I2C */
enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE,
    I2C_STATUS_ERROR,
} gI2cControllerStatus;

/* Definitions for IO Expander Switch press logic */
#define IO_EXPANDER_SWITCH_NOT_PRESSED (gRxPacket[0] == 0x00 || gRxPacket[0] == 0xBF)
#define IO_EXPANDER_SWITCH_PRESSED (gRxPacket[0] == 0x40 || gRxPacket[0] == 0xFF)

void updateIoExpanderPinState(uint8_t pinStates);
void setIoExpanderPinsToOutput(void);
void updateControllerLED(void);

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GROUP0_INT_IRQN);
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);

    setIoExpanderPinsToOutput();

    while (1) {
        /* Set all IO Expander output pins HIGH */
        updateIoExpanderPinState(0xFF);
        /* Uses RX Packet data to set or clear LED */
        updateControllerLED();
        /* Delay for 10,000,000 cycles for visual toggle */
        delay_cycles(10000000);

        /* Set all IO Expander output pins LOW */
        updateIoExpanderPinState(0x00);
        /* Uses RX Packet data to set or clear LED */
        updateControllerLED();
        /* Delay for 10,000,000 cycles for visual toggle */
        delay_cycles(10000000);
    }
}

void GROUP1_IRQHandler(void)
{
    /* Set LED HIGH */
    DL_GPIO_setPins(GROUP0_PORT, GROUP0_LED_PIN);

    /* Request a read from the IO Expander */
    gRxLen = I2C_RX_PACKET_SIZE;
    gRxCount = 0;
    
    /* Wait until the I2C bus is idle */
    while (!(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    
    /* Start a read operation */
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    DL_I2C_startControllerTransfer(I2C_INST, I2C_IO_EXPANDER_ADDRESS, 
                                    DL_I2C_CONTROLLER_DIRECTION_RX, gRxLen);
    
    /* Enable RX FIFO trigger interrupt */
    DL_I2C_enableInterrupt(I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER);
}

void I2C_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from IO Expander */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] =
                        DL_I2C_receiveControllerData(I2C_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_INST);
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_INST, &gTxPacket[gTxCount], gTxLen - gTxCount);
            }
            break;
            /* Not used for this example */
        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
                (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
                /* NACK interrupt if I2C IO Expander is disconnected */
                gI2cControllerStatus = I2C_STATUS_ERROR;
            }
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_FULL:
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_EMPTY:
        case DL_I2C_IIDX_CONTROLLER_START:
        case DL_I2C_IIDX_CONTROLLER_STOP:
        case DL_I2C_IIDX_CONTROLLER_EVENT1_DMA_DONE:
        case DL_I2C_IIDX_CONTROLLER_EVENT2_DMA_DONE:
        default:
            break;
    }
}

/* Function that sends the I2C command that configures IO Expander GPIO as outputs */
void setIoExpanderPinsToOutput(void)
{
    gI2cControllerStatus = I2C_STATUS_IDLE;

    gTxLen = I2C_TX_PACKET_SIZE;

    gTxPacket[0] = 0x1; /* IO control command */
    gTxPacket[1] = 0xBF; /* Set all pins to Output except for PA16 (S2) */

    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    /* Enable TXFIFO trigger interrupt if there are more bytes to send */
    if (gTxCount < gTxLen) {
        DL_I2C_enableInterrupt(
            I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(
            I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }

    /*
     * Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
    DL_I2C_startControllerTransfer(
        I2C_INST, I2C_IO_EXPANDER_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);

    /* Wait until the Controller sends all bytes */
    while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
           (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFE();
    }

    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* LED will remain high if there is an error */
        __BKPT(0);
    }

    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
}

/* Function to toggle LEDs on IO Expander */
void updateIoExpanderPinState(uint8_t pinStates)
{
    /* Wait for any previous transaction to complete */
    while (gI2cControllerStatus != I2C_STATUS_IDLE && 
           gI2cControllerStatus != I2C_STATUS_TX_COMPLETE &&
           gI2cControllerStatus != I2C_STATUS_RX_COMPLETE &&
           gI2cControllerStatus != I2C_STATUS_ERROR) {
        __WFE();
    }
    
    /* Prepare command packet */
    gTxLen = I2C_TX_PACKET_SIZE;
    gTxPacket[0] = 0x2;  /* IO output control command */
    gTxPacket[1] = pinStates;  /* Set pin states */
    
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    
    /* Enable TXFIFO trigger interrupt if there are more bytes to send */
    if (gTxCount < gTxLen) {
        DL_I2C_enableInterrupt(I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }
    
    /* Start the transfer */
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    
    while (!(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    
    DL_I2C_startControllerTransfer(I2C_INST, I2C_IO_EXPANDER_ADDRESS, 
                                  DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);
    
    /* Wait until completion */
    while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
           (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFE();
    }

    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
}

void updateControllerLED(void)
{
    if (IO_EXPANDER_SWITCH_NOT_PRESSED) {
        /* Turn LED OFF */
        DL_GPIO_clearPins(GROUP0_PORT, GROUP0_LED_PIN);
    } else if (IO_EXPANDER_SWITCH_PRESSED) {
        /* Turn LED ON */
        DL_GPIO_setPins(GROUP0_PORT, GROUP0_LED_PIN);
    }
}