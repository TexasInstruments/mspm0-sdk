/* Copyright (c) 2023, Texas Instruments Incorporated
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

/* Delay for 5ms to ensure SPI TX is idle before starting transmission */
#define SPI_TX_DELAY (160000)
/* Define max packet sizes */
#define MAX_PACKET_SIZE 4

/* SPI Buffers & Variables */
uint8_t gSPITxData[MAX_PACKET_SIZE];
uint8_t gSPIRxData[MAX_PACKET_SIZE];
/* Variable to track # of bytes SPI Received */
volatile uint8_t gSPIRxCount = 0;
/* I2C Controller Buffers &  Variable */
uint8_t gI2CTxData[MAX_PACKET_SIZE];
uint8_t gI2CRxData[MAX_PACKET_SIZE];
/* Target Address to communicate to */
volatile uint8_t gI2CAddress = 0x48;                    
/* Variable to track # of bytes I2C Transmitted */ 
volatile uint8_t gI2CTxCount = 0;                      
/* Variable to track # of bytes I2C Received */
volatile uint8_t gI2CRxCount = 0;                       
volatile uint8_t rxTemp = 0;
/* Indicates status of Bridge */
enum BridgeStates {
    SPI_RX_STATE = 0,
    I2C_TX_STATE,
    I2C_RX_STATE,
    SPI_TX_STATE
} gBridgeStates;
/* Indicates status of I2C Controller */
enum I2cControllerStatus {
    I2C_C_STATUS_IDLE = 0,
    I2C_C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE
} gI2cControllerStatus;

void bridge(void);

int main(void)
{
    SYSCFG_DL_init();
    /* Activate Interrupts */
    NVIC_ClearPendingIRQ(SPI_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_INST_INT_IRQN);
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);

    /* Optional delay to ensure SPI TX is idle before starting transmission */
    delay_cycles(SPI_TX_DELAY);

    /* Initial states */
    gBridgeStates = SPI_RX_STATE;
    gI2cControllerStatus = I2C_C_STATUS_IDLE;

    /* Start & Run bridge */
    while (1) {
        bridge();   
    }
}

void bridge(){
    /* Setting counter variables */
    uint8_t i=0; uint8_t j=0;uint8_t k=0;                                    
    switch (gBridgeStates) {
        case SPI_RX_STATE:
            if (gSPIRxCount >= MAX_PACKET_SIZE){
                /* Storing data from SPI Buffer to message that is addressed to Bridge A */
                for(k = 0; k < MAX_PACKET_SIZE; k++){
                    gI2CTxData[k] = gSPIRxData[k];
                    gSPIRxData[k] = 0;
                }
                /* Resetting gSPIRxCount variable */
                gSPIRxCount = 0;
                gBridgeStates = I2C_TX_STATE;
            }
            else {
                break;
            }
        case I2C_TX_STATE:
            /* Sending the I2C WRITE message to the 9724 */
            gI2CTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &gI2CTxData[0], MAX_PACKET_SIZE);

            /* Send the packet to the target. This function will send Start + Stop automatically. */
            while (!(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
            DL_I2C_startControllerTransfer(I2C_INST, gI2CAddress, DL_I2C_CONTROLLER_DIRECTION_TX, MAX_PACKET_SIZE);

            /* Wait until the Controller sends all bytes AKA the I2C_C_STATUS_TX_COMPLETE to be true */
            while (gI2cControllerStatus != I2C_C_STATUS_TX_COMPLETE) {
                __WFE();
            }
            while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

            gI2cControllerStatus = I2C_C_STATUS_IDLE;
            /* Go to I2C Controller Receive Bridge state */
            gBridgeStates = I2C_RX_STATE;                                       
            break;
        case I2C_RX_STATE:
            /* Add delay between transfers */
            delay_cycles(1000);

            /* Send a read request to Target */
            gI2cControllerStatus = I2C_STATUS_RX_STARTED;
            DL_I2C_startControllerTransfer(I2C_INST, gI2CAddress, DL_I2C_CONTROLLER_DIRECTION_RX, MAX_PACKET_SIZE);

            /* Wait for all bytes to be received in interrupt */
            while (gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) {
                __WFE();
            }
            while (DL_I2C_getControllerStatus(I2C_INST) &
                   DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

            /* Waiting for I2C Rx buffer interrupt to happen AKA When expected package size is received from I2C Target */
            if(gI2CRxCount >= MAX_PACKET_SIZE){
                /* Extract the received bytes from the I2C read and store them in SPI buffer #2 (Tx) */
                for(j = 0; j < MAX_PACKET_SIZE; j++){
                    gSPITxData[j] = gI2CRxData[j];
                    gI2CRxData[j] = 0;
                }
                /* Resetting gI2CRxCount variable */
                gI2CRxCount = 0;
                gI2cControllerStatus = I2C_C_STATUS_IDLE;
                /* Go to SPI Target Transmit Bridge state */
                gBridgeStates = SPI_TX_STATE;                                   
            }
            break;
        case SPI_TX_STATE:
            DL_SPI_fillTXFIFO8(SPI_INST, &gSPITxData[0], MAX_PACKET_SIZE);
            while(!DL_SPI_isTXFIFOEmpty(SPI_INST));

            DL_SPI_enableInterrupt(SPI_INST, DL_SPI_INTERRUPT_RX);
            gBridgeStates = SPI_RX_STATE;
            break;
    }
}

void SPI_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_INST)) {
        case DL_SPI_IIDX_RX:
            rxTemp = DL_SPI_receiveDataBlocking8(SPI_INST);
            gSPIRxData[gSPIRxCount] = rxTemp;
            if (gSPIRxCount >= MAX_PACKET_SIZE){
                DL_SPI_disableInterrupt(SPI_INST, DL_SPI_INTERRUPT_RX);
            }else {
                gSPIRxCount++;
            }
            break;
        default:
            break;
    }
}

void I2C_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            gI2cControllerStatus = I2C_C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true){
                if(gI2CRxCount < MAX_PACKET_SIZE) {
                    gI2CRxData[gI2CRxCount++] = DL_I2C_receiveControllerData(I2C_INST);
                }else{
                    DL_I2C_receiveControllerData(I2C_INST);
                }
            }
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        default:
            break;
    }
}
