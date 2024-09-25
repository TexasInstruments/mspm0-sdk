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
#include "I2C_communication.h"

/* Data sent to the Target */
uint8_t gTxPacket[I2C_TX_MAX_PACKET_SIZE] = {0x3E, 0x01, 0x00, 0x40, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;

/* Data received from Target */
uint8_t gRxPacket[I2C_RX_MAX_PACKET_SIZE];
/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount;
/* I2C status */
I2cControllerStatus_t gI2cControllerStatus;

//************Array copy **********************
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++) {
        dest[copyIndex] = source[copyIndex];
    }
}

//************I2C write register **********************
void I2C_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    unsigned char I2Ctxbuff[8] = {0x00};

    I2Ctxbuff[0] = reg_addr;
    unsigned char i, j = 1;

    for (i = 0; i < count; i++) {
        I2Ctxbuff[j] = reg_data[i];
        j++;
    }

    //    DL_I2C_flushControllerTXFIFO(I2C_0_INST);
    DL_I2C_fillControllerTXFIFO(I2C_0_INST, &I2Ctxbuff[0], count + 1);

    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_startControllerTransfer(I2C_0_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_TX, count + 1);

    while (DL_I2C_getControllerStatus(I2C_0_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    //Avoid BQ769x2 to stretch the SCLK too long and generate a timeout interrupt at 400kHz because of low power mode
    // if(DL_I2C_getRawInterruptStatus(I2C_0_INST,DL_I2C_INTERRUPT_CONTROLLER_CLOCK_TIMEOUT))
    // {
    //     DL_I2C_flushControllerTXFIFO(I2C_0_INST);
    //     DL_I2C_clearInterruptStatus(I2C_0_INST,DL_I2C_INTERRUPT_CONTROLLER_CLOCK_TIMEOUT);
    //     I2C_WriteReg(reg_addr, reg_data, count);
    // }
    DL_I2C_flushControllerTXFIFO(I2C_0_INST);
}

//************I2C read register **********************
void I2C_ReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    DL_I2C_fillControllerTXFIFO(I2C_0_INST, &reg_addr, count);

    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_startControllerTransfer(
        I2C_0_INST, I2C_TARGET_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 1);

    while (DL_I2C_getControllerStatus(I2C_0_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_flushControllerTXFIFO(I2C_0_INST);

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(
        I2C_0_INST, I2C_TARGET_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_RX, count);

    for (uint8_t i = 0; i < count; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_0_INST))
            ;
        reg_data[i] = DL_I2C_receiveControllerData(I2C_0_INST);
    }
}

void I2C_0_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_0_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_0_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_0_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] =
                        DL_I2C_receiveControllerData(I2C_0_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_0_INST);
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_0_INST, &gTxPacket[gTxCount], gTxLen - gTxCount);
            }
            break;
            /* Not used for this example */
        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_NACK:
            if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
                (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
                /* NACK interrupt if I2C Target is disconnected */
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
