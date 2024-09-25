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

#include "ti_msp_dl_config.h"
#include <string.h>

/* Header indice definitions */
#define ADDRESS_INDEX               0X00
#define RW_INDEX                    0X01
#define BUS_INDEX                   0x02
#define DATA_LENGTH_INDEX           0x03

/* Maximum size of TX packet */
#define I2C_TX_MAX_PACKET_SIZE (17) /* additional bit for gStatus */

/* Maximum size of RX packet (4 bytes of header) */
#define I2C_RX_MAX_PACKET_SIZE (20)

/* Data sent to Controller in response to Read transfer */
uint8_t gTxPacket[I2C_TX_MAX_PACKET_SIZE] = {0x00};

/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;

/* Data received from Controller during a Write transfer */
uint8_t gRxPacket[I2C_RX_MAX_PACKET_SIZE];
/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount;

/* 
 * Set default I2C controller to I2C1
 * I2C1 address: 0x400F0000U
 * I2C2 address: 0x400F4000U
 */
uint32_t gController = I2C1_BASE;
uint8_t gData[16]; /* max of 16 bytes of data */

/*
 * gReceived and gSequence handle the process of the expander
 * gReceived: flag for when host writes to the expander
 * gSequence: 0:idle. 1:received from host. 2:sent back to host.
 */
bool gReceived = false;
uint8_t gSequence = 0;

/* Initialize variables for parsing the header sent by the host */
uint8_t gRxData[I2C_RX_MAX_PACKET_SIZE];
uint8_t address;
uint8_t rwBit;
uint8_t bus;
uint8_t dataLength;

/*
 * Status:
 * 0x00 = "ACK"
 * 0x01 = "NAK"
 */
uint8_t gStatus = 0;

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

void I2C_Expander(void);

int main(void)
{
    SYSCFG_DL_init();

    gTxCount = 0;
    gTxLen   = I2C_TX_MAX_PACKET_SIZE;
    DL_I2C_enableInterrupt(I2C_0_INST, DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER);

    gRxCount = 0;
    gRxLen   = I2C_RX_MAX_PACKET_SIZE;

    NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
    NVIC_EnableIRQ(I2C_1_INST_INT_IRQN);
    NVIC_EnableIRQ(I2C_2_INST_INT_IRQN);

    while (1) {
        __WFI();
        
        I2C_Expander();
    }
}

void I2C_Expander() {
    if (gReceived && gSequence == 1) {
        /* set the I2C controller status */
        gI2cControllerStatus = I2C_STATUS_IDLE;
        gStatus = 0;

        /* parse through received data */
        address = gRxData[ADDRESS_INDEX];
        rwBit = gRxData[RW_INDEX];
        bus = gRxData[BUS_INDEX];
        dataLength = gRxData[DATA_LENGTH_INDEX];

        /* determine which I2C interface to communicate on */
        if (bus == 1) {
            gController = I2C1_BASE;
        }
        else if (bus == 2) {
            gController = I2C2_BASE;
        }

        /* If a write is requested */
        if (rwBit == 0) {
            /* fill gData with data from gRxData */
            for (int i = 0; i < dataLength; i++) {
                gData[i] = gRxData[i + 1 + DATA_LENGTH_INDEX];
            }
            
            /*
             * Fill the FIFO
             *  The FIFO is 8-bytes deep, and this function will return number
             *  of bytes written to FIFO 
             */
            gTxCount = DL_I2C_fillControllerTXFIFO((I2C_Regs *)gController, &gData[0], gTxLen);
            /* Enable TXFIFO trigger interrupt if there are more bytes to send */
            if (gTxCount < gTxLen) {
                DL_I2C_enableInterrupt(
                    (I2C_Regs *)gController, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            } else {
                DL_I2C_disableInterrupt(
                    (I2C_Regs *)gController, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            }

            /*
             * Send the packet to the controller.
             * This function will send Start + Stop automatically.
             */
            gI2cControllerStatus = I2C_STATUS_TX_STARTED;
            while (!(
                DL_I2C_getControllerStatus((I2C_Regs *)gController) & DL_I2C_CONTROLLER_STATUS_IDLE))
                ;
            DL_I2C_startControllerTransfer(
                (I2C_Regs *)gController, address, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);

            /* Wait until the Controller sends all bytes */
            while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
                (gI2cControllerStatus != I2C_STATUS_ERROR)) {
                __WFE();
            }

            while (DL_I2C_getControllerStatus((I2C_Regs *)gController) &
                DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
                ;
        }

        /* If a read is requested */
        else if (rwBit == 1) {
            /* Send a read request to Target */
            gRxLen               = dataLength;
            gRxCount             = 0;
            gI2cControllerStatus = I2C_STATUS_RX_STARTED;
            DL_I2C_startControllerTransfer(
                (I2C_Regs *)gController, address, DL_I2C_CONTROLLER_DIRECTION_RX, gRxLen);

            /* Wait for all bytes to be received in interrupt */
            while ((gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) &&
                    (gI2cControllerStatus != I2C_STATUS_ERROR)) {
                __WFE();
            }

            while (DL_I2C_getControllerStatus((I2C_Regs *)gController) &
                DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
                ;

            memset((uint8_t *) &gTxPacket, 0x00, I2C_TX_MAX_PACKET_SIZE);

            for (uint16_t i = 0;
                (i < gRxCount) && (i < I2C_TX_MAX_PACKET_SIZE); i++) {
                gTxPacket[i+1] = gRxPacket[i];
                DL_I2C_flushTargetTXFIFO((I2C_Regs *)gController);
            }
        }

        DL_I2C_flushTargetRXFIFO((I2C_Regs *)gController);

        /* Set the status byte in the returned data */
        gTxPacket[0] = gStatus;

        gReceived = false;
        gSequence++;

        /* 
         * Trigger an interrupt to request a read
         * NOTE: interrupt needs to be set up on host side
         */
        DL_GPIO_togglePins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
        delay_cycles(100);
        DL_GPIO_togglePins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }
}

void I2C_0_INST_IRQHandler(void)
{
    static bool dataRx = false;

    switch (DL_I2C_getPendingInterrupt(I2C_0_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            /* Initialize RX or TX after Start condition is received */
            gTxCount = 0;
            gRxCount = 0;
            dataRx   = false;
            /* Flush TX FIFO to refill it */
            DL_I2C_flushTargetTXFIFO(I2C_0_INST);
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            dataRx = true;
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_0_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] = DL_I2C_receiveTargetData(I2C_0_INST);
                } else {
                    /* Prevent overflow and just ignore data */
                    DL_I2C_receiveTargetData(I2C_0_INST);
                }
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:
            /* Fill TX FIFO if there are more bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillTargetTXFIFO(
                    I2C_0_INST, &gTxPacket[gTxCount], (gTxLen - gTxCount));
            } else {
                /*
                 * Fill FIFO with 0x00 if more data is requested than
                 * expected gTxLen
                 */
                while (DL_I2C_transmitTargetDataCheck(I2C_0_INST, 0x00) != false)
                    ;
            }
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            /* If data was received, echo to TX buffer */
            if (dataRx == true) {
                for (uint16_t i = 0;
                     (i < gRxCount) && (i < I2C_TX_MAX_PACKET_SIZE); i++) {
                    gRxData[i] = gRxPacket[i];
                    DL_I2C_flushTargetTXFIFO(I2C_0_INST);
                }
                dataRx = false;
            }
            /* Toggle GPIO to trigger an interrup to request a read */
            gReceived = true;
            gSequence++;
            if (gSequence >= 2) {
                gSequence = 0;
                gRxLen = I2C_RX_MAX_PACKET_SIZE;
            }
            break;
        case DL_I2C_IIDX_TARGET_RX_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_TX_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_RXFIFO_FULL:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_GENERAL_CALL:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE:
            /* Not used for this example */
        default:
            break;
    }
}

void I2C_1_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_1_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_1_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
             while (DL_I2C_isControllerRXFIFOEmpty(I2C_1_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] =
                        DL_I2C_receiveControllerData(I2C_1_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_1_INST);
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_1_INST, &gTxPacket[gTxCount], gTxLen - gTxCount);
            }
            break;
            /* Not used for this example */
        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
                (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
                /* NACK interrupt if I2C Target is disconnected */
                gI2cControllerStatus = I2C_STATUS_ERROR;
                gStatus = 1;
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

void I2C_2_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_2_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_2_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_2_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] =
                        DL_I2C_receiveControllerData(I2C_2_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_2_INST);
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_2_INST, &gTxPacket[gTxCount], gTxLen - gTxCount);
            }
            break;
            /* Not used for this example */
        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
                (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
                /* NACK interrupt if I2C Target is disconnected */
                gI2cControllerStatus = I2C_STATUS_ERROR;
                gStatus = 1;
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
