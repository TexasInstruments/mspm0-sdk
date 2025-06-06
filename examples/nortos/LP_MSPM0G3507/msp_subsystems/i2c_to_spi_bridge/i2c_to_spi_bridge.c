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

/* Maximum size of packet  */
#define MAX_PACKET_SIZE      (4)

/* Data sent to Controller in response to Read transfer  */
uint8_t gSPITxPacket[MAX_PACKET_SIZE] = {0x00};
/* Data received from Controller during a Write transfer  */
uint8_t gI2CRxPacket[MAX_PACKET_SIZE];                               
uint8_t gSPIRxData[MAX_PACKET_SIZE];

/* Counters for I2C RX & TX length and bytes sent  */
uint32_t gI2CTxCount;
uint32_t gI2CRxCount;

/* Counters for SPI RX & TX length and bytes sent */
uint32_t gSPIRxCount;

/* Variable used to skip false I2C Trigger when first sending a message */
uint32_t countSkip = 0;

enum error_codes{
    NO_ERROR = 0,
    DATA_BUFFER_OVERFLOW,
    I2C_TARGET_TXFIFO_UNDERFLOW,
    I2C_TARGET_RXFIFO_OVERFLOW,
    I2C_TARGET_ARBITRATION_LOST,
    I2C_INTERRUPT_OVERFLOW
};

/* Indicates status of Bridge */
enum BridgeStates {
    I2C_RX_STATE = 0,
    SPI_TX_STATE,
    SPI_RX_STATE,
    I2C_TX_STATE
} gBridgeStates;

uint8_t gErrorStatus = NO_ERROR;

/* Flag to start SPI transfer */
bool gSpiTxReady = false;
/* Flag to indicate SPI transfer Ongoing */                                                  
bool gSpiTxOngoing = false; 
/* Flag to indicate SPI data has been received */                                                
bool gSpiRxDone = false;
/* Flag to indicate SPI data is being receive */                                                    
bool gSpiRxOngoing = false;    
/* Flag to start SPI transfer */                                             
bool gI2cTxDone = false;                                                    

void bridge();
int main(void)
{
    SYSCFG_DL_init();

    /* Initialize variables to send data inside TX ISR and to receive data inside RX ISR */
    gI2CTxCount = 0;
    gI2CRxCount = 0;
    gSPIRxCount = 0;

    /* Setting flags to default values */
    gSpiTxReady = false;
    gSpiRxDone = false;

    /* Enabling Interrupts on I2C & SPI Modules */
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_INST_INT_IRQN);
    
    /* Start & Run bridge */
    while (1) {
        bridge();
    }
}

void bridge(){
    switch (gBridgeStates) {
        case I2C_RX_STATE:
            /* Wait for gI2C_Length(4) bytes to be received by SPI Peripheral */
            if (gSpiTxReady){
                gBridgeStates = SPI_TX_STATE;
            }
            else {
                break;
            }
        case SPI_TX_STATE:
            gSpiTxReady = false;
            for(int i = 0; i < gI2CRxCount; i++){
                /* Transmit data out via SPI and wait until transfer is complete */
                DL_SPI_transmitDataBlocking8(SPI_INST, gSPITxPacket[i]);
            }
            gSpiTxOngoing = false;
            gBridgeStates = SPI_RX_STATE;
            break;
        case SPI_RX_STATE:
            if(gSpiRxDone){
                gSPIRxCount = 0;
                gBridgeStates = I2C_TX_STATE;
            }
            break;
        case I2C_TX_STATE:
            if(gI2cTxDone){
                gI2cTxDone = false;
                gBridgeStates = I2C_RX_STATE;
            }
            break;
        default:
            break;
    }
}

void I2C_INST_IRQHandler(void)
{
    /* Flag that indicates I2C data received */
    static bool gI2CRxDone = false;
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            /* Initialize (resets) RX or TX after Start condition is received */
            gI2CTxCount = 0;
            gI2CRxCount = 0;
            gI2CRxDone   = false;
            /* Flush TX FIFO to refill it */
            DL_I2C_flushTargetTXFIFO(I2C_INST);
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            gI2CRxDone = true;
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true) {
                if(gI2CRxCount < MAX_PACKET_SIZE){
                    gI2CRxPacket[gI2CRxCount++] = DL_I2C_receiveTargetData(I2C_INST);
                }else{
                    /* Prevent overflow and just ignore data */
                    DL_I2C_receiveTargetData(I2C_INST);
                    gErrorStatus = DATA_BUFFER_OVERFLOW;
                }
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:            
                /* Fill TX FIFO if there are more bytes to send */
                if (gI2CTxCount < MAX_PACKET_SIZE) {
                    countSkip += 1;
                    gI2CTxCount += DL_I2C_fillTargetTXFIFO(I2C_INST, &gSPIRxData[gI2CTxCount], (MAX_PACKET_SIZE - gI2CTxCount));
                    if(gI2CTxCount >= MAX_PACKET_SIZE && countSkip > 1){
                        gI2cTxDone = true;
                    }
                }
                if(gErrorStatus != NO_ERROR)
                {
                    /* Fill FIFO with error status after sending latest received byte */
                    while (DL_I2C_transmitTargetDataCheck(I2C_INST, gErrorStatus) != false);
                }
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            /* If data was received, store it in SPI TX buffer */
            if (gI2CRxDone == true) {
                for (uint16_t i = 0; (i < gI2CRxCount) && (i < MAX_PACKET_SIZE); i++) {
                    gSPITxPacket[i] = gI2CRxPacket[i];
                    DL_I2C_flushTargetTXFIFO(I2C_INST);
                }
                gI2CRxDone = false;
                /* Set flag to indicate data ready for SPI TX */
                gSpiTxReady = true;
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_UNDERFLOW:
            gErrorStatus = I2C_TARGET_TXFIFO_UNDERFLOW;
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_OVERFLOW:
            gErrorStatus = I2C_TARGET_RXFIFO_OVERFLOW;
            break;
        case DL_I2C_IIDX_TARGET_ARBITRATION_LOST:
            gErrorStatus = I2C_TARGET_ARBITRATION_LOST;
            break;
        case DL_I2C_IIDX_INTERRUPT_OVERFLOW:
            gErrorStatus = I2C_INTERRUPT_OVERFLOW;
            break;
        default:
            break;
    }
}

void SPI_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_INST)) {
        case DL_SPI_IIDX_RX:
            /* Read RX FIFO */
            if(gSPIRxCount < MAX_PACKET_SIZE){
                gSpiRxOngoing = true;
                gSPIRxData[gSPIRxCount++] = DL_SPI_receiveData8(SPI_INST);
                if(gSPIRxCount >= MAX_PACKET_SIZE){
                    gSpiRxDone = true;
                    gSpiRxOngoing = false;
                }
            }
            break;
        case DL_SPI_IIDX_TX:
            gSpiTxOngoing = true;
            break;
        default:
            break;
    }
}
