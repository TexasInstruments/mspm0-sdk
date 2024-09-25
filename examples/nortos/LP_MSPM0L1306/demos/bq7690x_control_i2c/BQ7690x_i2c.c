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
#include <stdio.h>
#include "BQ7690x.h"
#include "ti_msp_dl_config.h"

/* Configuration */

// Indicates status of I2C
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

// Counters for TX length and bytes sent
uint32_t gTxLen, gTxCount;

// Data sent to the Target
uint8_t gTxPacket[4];

// Data received from Target
uint8_t gRxPacket[32];
// Counters for TX length and bytes sent
uint32_t gRxLen, gRxCount;

/********* I2C Controller Driver Functions *********/

//I2C Interrupts IRQ
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

            /* Receive all bytes from target */
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

// I2C Write
void I2C_Write(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    gTxPacket[0] = reg_addr;
    for (int i = 0; i < count; i++) {
        gTxPacket[i + 1] = reg_data[i];
    }

    /*Prepare the FIFO for TX*/
    DL_I2C_flushControllerTXFIFO(I2C_INST);  // Empty I2C FIFO ( 8- bytes deep)
    DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0],
        count + 1);  // Fill FIFO with TX Data
    DL_I2C_enableInterrupt(I2C_INST,
        DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);  // Enable Interrupts for TX

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send Package to the Controller */
    DL_I2C_startControllerTransfer(
        I2C_INST, DEV_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, count + 1);

    /* Wait until controller send all bytes */

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
        // LED will remain high if there is an error
        __BKPT(0);
    }

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Add delay between transfers */
    delay_cycles(1000);

    /*Cleanup the FIFO */
    DL_I2C_flushControllerTXFIFO(I2C_INST);  // Empty I2C FIFO ( 8- bytes deep)
}

// I2C Read
void I2C_Read(uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    gTxPacket[0] = reg_addr;
    gRxLen       = count;  // Set expected data length
    gRxCount     = 0;      // Initialize RX counter to 0

    DL_I2C_enableInterrupt(I2C_INST,
        DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);  // Enable Interrupts for TX

    /*Prepare the FIFO for TX*/
    DL_I2C_flushControllerTXFIFO(I2C_INST);  // Empty I2C FIFO ( 8- bytes deep)
    DL_I2C_flushTargetRXFIFO(I2C_INST);
    DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0],
        1);  // Fill FIFO with TX Data

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send Package to the Controller */
    DL_I2C_startControllerTransfer(
        I2C_INST, DEV_ADDR, DL_I2C_CONTROLLER_DIRECTION_TX, 1);

    /* Wait until controller send all bytes */
    while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
           (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFE();
    }

    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Add delay between transfers */
    delay_cycles(1000);

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(
        I2C_INST, DEV_ADDR, DL_I2C_CONTROLLER_DIRECTION_RX, count);

    for (uint8_t i = 0; i < count; i++) {
        while (  // Wait for RX to read a byte
            (DL_I2C_getControllerStatus(I2C_INST) & I2C_STATUS_RX_INPROGRESS))
            ;
        reg_data[i] = gRxPacket[i];  // Record the byte
    }

    /*Cleanup the FIFOs */
    DL_I2C_flushControllerTXFIFO(I2C_INST);  // Empty I2C FIFO ( 8- bytes deep)
    DL_I2C_flushControllerRXFIFO(I2C_INST);
}

/********* Common Functions *********/

// Calculates CRC8
unsigned char CRC8(unsigned char *ptr, unsigned char len)
{
    unsigned char i;
    unsigned char crc = 0;
    while (len-- != 0) {
        for (i = 0x80; i != 0; i /= 2) {
            if ((crc & 0x80) != 0) {
                crc *= 2;
                crc ^= 0x107;
            } else
                crc *= 2;

            if ((*ptr & i) != 0) crc ^= 0x107;
        }
        ptr++;
    }
    return (crc);
}

// Calculate checksum for RAM writes
unsigned char Checksum(unsigned char *ptr, unsigned char len)
{
    unsigned char i;
    unsigned char checksum = 0;

    for (i = 0; i < len; i++) checksum += ptr[i];

    checksum = 0xff & ~checksum;

    return (checksum);
}
