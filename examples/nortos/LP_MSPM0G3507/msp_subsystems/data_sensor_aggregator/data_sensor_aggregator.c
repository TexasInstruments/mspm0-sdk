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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ti_msp_dl_config.h"

/* Initializing functions */

void DataCollection(void);
void TxFunction(void);
void RxFunction(void);
void Transmit(void);
void UART_Console_write(const uint8_t *data, uint16_t size);

/* Earth's gravity in m/s^2 */
#define GRAVITY_EARTH (9.80665f) 

/* Maximum size of TX packet */
#define I2C_TX_MAX_PACKET_SIZE (16)

/* Number of bytes to send to target device */
#define I2C_TX_PACKET_SIZE (3)

/* Maximum size of RX packet */
#define I2C_RX_MAX_PACKET_SIZE (16)

/* Number of bytes to received from target */
#define I2C_RX_PACKET_SIZE (16)

/*
 * Number of bytes for UART packet size
 * The packet will be transmitted by the UART.
 * This example uses FIFOs with polling, and the maximum FIFO size is 4.
 * Refer to interrupt examples to handle larger packets.
 */
#define UART_PACKET_SIZE (8)

uint8_t gSpace[] = "\r\n";
volatile bool gConsoleTxTransmitted;
volatile bool gConsoleTxDMATransmitted;
/* Data for UART to transmit */
uint8_t gTxData[UART_PACKET_SIZE];
/* Booleans for interrupts */
bool gCheckADC;
bool gDataReceived;
/* Variable to change the target address */
uint8_t gTargetAdd;
/* I2C variables for data collection */
float gHumidity, gTempHDC, gAmbient;
uint16_t gAmbientE, gAmbientR, gDRV;
uint16_t gMagX, gMagY, gMagZ, gGyrX, gGyrY, gGyrZ, gAccX, gAccY, gAccZ;
/* Data sent to the Target */
uint8_t gTxPacket[I2C_TX_MAX_PACKET_SIZE];
/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;
/* Data received from Target */
uint8_t gRxPacket[I2C_RX_MAX_PACKET_SIZE];
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

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    DL_SYSCTL_disableSleepOnExit();


    while(1) {
        DataCollection();
        Transmit();
	/* This delay is to the data is transmitted every few seconds */
        delay_cycles(100000000);
    }
}

void DataCollection(void)
{
/* Tempurature and Humidity Sensor */
    gTargetAdd = (0x40);
    gTxPacket[0] = (0x0F);
    gTxPacket[1] = (0x01);
    gI2cControllerStatus = I2C_STATUS_IDLE;
    gTxLen = 2;
    /*
     * Fill the FIFO
     *  The FIFO is 8-bytes deep, and this function will return number
     *  of bytes written to FIFO */
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    /*
     * Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    /* Call transfer function */
    TxFunction();

    /* Read registers pointer */
    gTxLen = 1;
    gTxPacket[0] = 0x00;
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    TxFunction();
    /* Delay to make sure all data has time to transfer */
    delay_cycles(160000);

    /* Send a read request to Target */
    gRxLen               = I2C_RX_PACKET_SIZE;
    gRxCount             = 0;
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    RxFunction();
    gTempHDC = ((uint16_t)gRxPacket[1] << 8 | gRxPacket[0]);
    gHumidity = ((uint16_t)gRxPacket[3] << 8 | gRxPacket[2]);


/* OPT Ambient Light Sensor */
    gI2cControllerStatus = I2C_STATUS_IDLE;
    gTargetAdd = (0x44);

    /* Configuration pointer */
    gTxLen = 3;
    gTxPacket[0] = 0x01;
    gTxPacket[1] = 0xCA;
    gTxPacket[2] = 0x10;
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    TxFunction();

    /* Read pointer */
    gTxLen = 1;
    gTxPacket[0] = 0x00;
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    TxFunction();
    /* Delay to make sure all data has time to transfer */
    delay_cycles(50000000);

    gRxLen               = I2C_RX_PACKET_SIZE;
    gRxCount             = 0;
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    RxFunction();
    gAmbientE = (gRxPacket[0] >> 4);
    gAmbientR = ((uint16_t)gRxPacket[0] << 8 | gRxPacket[1]) & 0x0FFF;


/* Inertial Measurement/Geomagnetic Sensor */
    gI2cControllerStatus = I2C_STATUS_IDLE;
    gTargetAdd = (0x69);

    /* Read pointer for 1 - 9 bytes */
    gTxLen = 1;
    gTxPacket[0] = 0x04;
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    TxFunction();
    /* Delay to make sure all data has time to transfer */
    delay_cycles(5000000);

    gRxLen               = I2C_RX_PACKET_SIZE;
    gRxCount             = 0;
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    RxFunction();
    gMagX = ((uint16_t)gRxPacket[1] << 8 | gRxPacket[0]);
    gMagY = ((uint16_t)gRxPacket[3] << 8 | gRxPacket[2]);
    gMagZ = ((uint16_t)gRxPacket[5] << 8 | gRxPacket[4]);
    gGyrX = ((uint16_t)gRxPacket[9] << 8 | gRxPacket[8]);
    gGyrY = ((uint16_t)gRxPacket[11] << 8 | gRxPacket[10]);
    gGyrZ = ((uint16_t)gRxPacket[13] << 8 | gRxPacket[12]);

    /* Read pointer for a different 6 bytes */
    gTxLen = 1;
    gTxPacket[0] = 0x12;
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gTxPacket[0], gTxLen);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    TxFunction();
    /* Delay to make sure all data has time to transfer */
    delay_cycles(5000000);

    gRxLen               = I2C_RX_PACKET_SIZE;
    gRxCount             = 0;
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    RxFunction();
    gAccX = ((uint16_t)gRxPacket[1] << 8 | gRxPacket[0]);
    gAccY = ((uint16_t)gRxPacket[3] << 8 | gRxPacket[2]);
    gAccZ = ((uint16_t)gRxPacket[5] << 8 | gRxPacket[4]);


/* ADC Conversion for DRV */
    gCheckADC = false;
    DL_ADC12_startConversion(ADC12_0_INST);
    while (false == gCheckADC){
        __WFE();
    }
    gCheckADC = false;
    gDRV = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
    DL_ADC12_enableConversions(ADC12_0_INST);


/* Value calculations */

    /* Tempurature in Celsius */
    gTempHDC = ( gTempHDC / pow(2, 16) ) * 165 - 40;
    /* Humidity %RH */
    gHumidity = ( gHumidity / pow(2, 16) ) * 100;
    /* Ambient in Lux */
    gAmbient = 0.01 * pow(2, gAmbientE) * gAmbientR;

    /* Gyroscope(degree/s) in X plane */
    gGyrX = gGyrX/16.4;
    /* Gyroscope(degree/s) in Y plane */
    gGyrY = gGyrY/16.4;
    /* Gyroscope(degree/s) in Z plane */
    gGyrZ = gGyrZ/16.4;

    /* Accleration(mg) in X plane */
    gAccX = GRAVITY_EARTH * gAccX * 4096 / ((1 << 16) / 2.0f);
    /* Accleration(mg) in Y plane */
    gAccY = gAccY/4096;
    /* Accleration(mg) in Z plane */
    gAccZ = GRAVITY_EARTH * gAccZ * 4096 / ((1 << 16) / 2.0f);

}

/* This is the transfer function for I2C 
 * Checks transfer status, starts tranfer at designated address */
void TxFunction(void)
{
    /* Wait until the Controller finishes write and target ACKs */
    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    DL_I2C_startControllerTransfer(
        I2C_INST, gTargetAdd, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);
    /* Wait until the Controller sends all bytes */
    while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
           (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFE();
    }
    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
}

void RxFunction(void)
{
    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    DL_I2C_startControllerTransfer(
        I2C_INST, gTargetAdd, DL_I2C_CONTROLLER_DIRECTION_RX, gRxLen);
    /* Wait for all bytes to be received in interrupt */
    while (gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) {
        __WFE();
    }
    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
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

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_EOT_DONE:
            gConsoleTxTransmitted = true;
            break;
        case DL_UART_MAIN_IIDX_DMA_DONE_TX:
            gConsoleTxDMATransmitted = true;
            break;
        default:
            break;
    }
}

/* This function formats and transmits all of the collected data over UART */
void Transmit(void)
{
    int count = 1;
    char buffer[20];
    while (count < 14)
    {
	/* Formatting the name and converting int to string for transfer */
        switch(count){
            case 1:
                gTxData[0] = 84; 
                gTxData[1] = 67;
                gTxData[2] = 58;
                gTxData[3] = 32; 
                sprintf(buffer, "%f", gTempHDC);
                break;
            case 2:
                gTxData[0] = 72; 
                gTxData[1] = 37;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%f", gHumidity);
                break;
            case 3:
                gTxData[0] = 65;
                gTxData[1] = 109; 
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%f", gAmbient);
                break;
            case 4:
                gTxData[0] = 77; 
                gTxData[1] = 120;
                gTxData[2] = 58;
                gTxData[3] = 32; 
                sprintf(buffer, "%i", gMagX);
                break;
            case 5:
                gTxData[0] = 77; 
                gTxData[1] = 121;
                gTxData[2] = 58;
                gTxData[3] = 32; 
                sprintf(buffer, "%i", gMagY);
                break;
            case 6:
                gTxData[0] = 77; 
                gTxData[1] = 122;
                gTxData[2] = 58;
                gTxData[3] = 32; 
                sprintf(buffer, "%i", gMagZ);
                break;
            case 7:
                gTxData[0] = 71; 
                gTxData[1] = 120;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gGyrX);
                break;
            case 8:
                gTxData[0] = 71; 
                gTxData[1] = 121;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gGyrY);
                break;
            case 9:
                gTxData[0] = 71; 
                gTxData[1] = 122;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gGyrZ);
                break;
            case 10:
                gTxData[0] = 65; 
                gTxData[1] = 120;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gAccX);
                break;
            case 11:
                gTxData[0] = 65; 
                gTxData[1] = 121;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gAccY);
                break;
            case 12:
                gTxData[0] = 65; 
                gTxData[1] = 122;
                gTxData[2] = 58;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gAccZ);
                break;
            case 13:
                gTxData[0] = 68; 
                gTxData[1] = 82;
                gTxData[2] = 86;
                gTxData[3] = 32;
                sprintf(buffer, "%i", gDRV);
                break;
        }
        count++;
	/* Filling the UART transfer variable */
        gTxData[4] = buffer[0]; 
        gTxData[5] = buffer[1];
        gTxData[6] = buffer[2]; 
        gTxData[7] = buffer[3];

        /* Optional delay to ensure UART TX is idle before starting transmission */
        delay_cycles(160000);

        UART_Console_write(&gTxData[0], 8);
        UART_Console_write(&gSpace[0], sizeof(gSpace)); 
    }
    UART_Console_write(&gSpace[0], sizeof(gSpace)); 
}

void UART_Console_write(const uint8_t *data, uint16_t size)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(data));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->TXDATA));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, size);

    DL_SYSCTL_disableSleepOnExit();

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    while (false == gConsoleTxDMATransmitted) {
        __WFE();
    }

    while (false == gConsoleTxTransmitted) {
        __WFE();
    }

    gConsoleTxTransmitted    = false;
    gConsoleTxDMATransmitted = false;
}
