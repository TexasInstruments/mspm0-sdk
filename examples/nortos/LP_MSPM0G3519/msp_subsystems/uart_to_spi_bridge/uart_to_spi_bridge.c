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

/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)

/* Define UART Header and Start Byte */
#define UART_HEADER_LENGTH  (0x02)
#define UART_START_BYTE     (0xF8)
#define UART_READ_SPI_BYTE  (0xFA)
#define UART_WRITE_SPI_BYTE (0xFB)
#define RW_INDEX            (0x00)
#define LENGTH_INDEX        (0x01)

/* Define max packet sizes */
#define SPI_MAX_PACKET_SIZE     (16)
#define UART_MAX_PACKET_SIZE    (SPI_MAX_PACKET_SIZE + UART_HEADER_LENGTH)

/* global variables for keeping track of bridge process */
uint8_t gUartRxData[UART_MAX_PACKET_SIZE];
volatile uint8_t gUartRxDataCount = 0;
uint8_t gSPIData[SPI_MAX_PACKET_SIZE];
volatile uint8_t gSPICount = 0;
volatile uint8_t gMsgLength = 0;
volatile uint8_t gMsgRW = 0;
volatile bool gDataReceived;
volatile bool gUartStartDetected = false;

/* Indicates status of Bridge */
enum UARTBridgeStatus {
    UART_STATUS_WAITING = 0,
    UART_STATUS_START,
    UART_STATUS_WRITE_SPI,
    UART_STATUS_READ_SPI,
    UART_STATUS_TX_UART,
    UART_STATUS_ERROR
} gUARTBridgeStatus;

/* Indicates status of I2C */
enum SPIControllerStatus {
    SPI_STATUS_IDLE = 0,
    SPI_STATUS_TX_STARTED,
    SPI_STATUS_TX_INPROGRESS,
    SPI_STATUS_TX_COMPLETE,
    SPI_STATUS_RX_STARTED,
    SPI_STATUS_RX_INPROGRESS,
    SPI_STATUS_RX_COMPLETE,
    SPI_STATUS_ERROR,
} gSPIControllerStatus;

/* Error Flags */
enum ErrorFlags {
    ERROR_NONE = 0,
    ERROR_UARTFULL = 31,
    ERROR_UART_OVERRUN,
    ERROR_IMPROPER_MESSAGE_LENGTH,
    ERROR_SPI_NACK = 41,
    ERROR_SPI_OVERUN,
    ERROR_SPI_NO_RW,
    ERROR_SPI_WRITE_FAILED,
} gError;

void Uart_Bridge(void);

int main(void)
{
    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(UART_BRIDGE_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_BRIDGE_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);

    /* Delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);

    /* Initial states */
    gError = ERROR_NONE;
    gUARTBridgeStatus = UART_STATUS_WAITING;
    gSPIControllerStatus = SPI_STATUS_IDLE;
    gDataReceived = false;

    while (gDataReceived == false) {
        __WFE();
    }

    /* Data recieved, start bridge */
    while (1) {
    /*
    * Optionally check for errors and do something
    * Some examples give below
    */
    if (gError) {
        /* TX error code upstream */
        DL_UART_Main_transmitDataBlocking(UART_BRIDGE_INST, gError);
        gError = ERROR_NONE;
        gUartStartDetected = false;
    }

    Uart_Bridge();
    __WFE();
    }
}

void Uart_Bridge(){
    switch (gUARTBridgeStatus) {
        case UART_STATUS_WAITING:
            /* Wait for header information */
            if (gUartRxDataCount >= UART_HEADER_LENGTH) {
                gUARTBridgeStatus = UART_STATUS_START;
                gMsgRW = gUartRxData[RW_INDEX];
                gMsgLength = gUartRxData[LENGTH_INDEX];
            }
            else {
                break;
            }
        case UART_STATUS_START:
            /*
            * This case waits until full UART message is received, then parses header
            * Optionally, error cases and I2C address ranges (not shown) can be checked
            * Or a timeout feature added in case of improper UART message lengths
            */

            /* Check for error cases first */
            if (!gMsgLength) {
                gError = ERROR_IMPROPER_MESSAGE_LENGTH;
                gUARTBridgeStatus = UART_STATUS_ERROR;
                break;
            }
            if (gMsgLength > SPI_MAX_PACKET_SIZE) {
                gError = ERROR_IMPROPER_MESSAGE_LENGTH;
                gUARTBridgeStatus = UART_STATUS_ERROR;
                break;
            }
            if (gMsgRW == UART_WRITE_SPI_BYTE) {
                
                while (gMsgLength > (gUartRxDataCount - UART_HEADER_LENGTH) ) {
                    /* Wait until full message is recieved */
                     __WFE();
                }
                /* Transfer UART Buffer over */
                for(int i = 0; i <= gMsgLength; i++) {
                    gSPIData[i] = gUartRxData[i + UART_HEADER_LENGTH];
                }
                gUARTBridgeStatus = UART_STATUS_WRITE_SPI;
            }
            else if (gMsgRW == UART_READ_SPI_BYTE)  {
                gUARTBridgeStatus = UART_STATUS_READ_SPI;
                /* Optionally clear gSPIData buffer here before read */
            }
            else {
                 /*Error: Wrong RW value */
                 gError = ERROR_SPI_NO_RW;
                 gUARTBridgeStatus = UART_STATUS_ERROR;
            }
            /* Full message received, advance and reset UART RX */
            gUartRxDataCount = 0;
            gUartRxData[LENGTH_INDEX] = 0;
            gUartStartDetected = false;
            gMsgRW = 0;
            break;
        case UART_STATUS_WRITE_SPI:
           /*
            * Perform SPI Write
            */
            gSPIControllerStatus = SPI_STATUS_TX_STARTED;
            for(int i = 0; i < gMsgLength; i++){
                if(!DL_SPI_transmitDataCheck8(SPI_0_INST, gSPIData[i])){
                    gError = ERROR_SPI_WRITE_FAILED;
                }
            }

            gSPICount = 0;
            gMsgLength = 0;
            gUARTBridgeStatus = UART_STATUS_WAITING;
            break;                
        case UART_STATUS_READ_SPI:
            /*
            * Perform SPI Read and ready UART TX upstream
            */

            /* Send dummy data to retrieve read data */
            gSPICount             = 0;
            gSPIControllerStatus = SPI_STATUS_RX_STARTED;
            for(int i = 0; i < gMsgLength; i++){
                if(!DL_SPI_transmitDataCheck8(SPI_0_INST, 0x00)){
                    gError = ERROR_SPI_WRITE_FAILED;
                }
            }
            gSPIControllerStatus = SPI_STATUS_RX_COMPLETE;
            gUARTBridgeStatus = UART_STATUS_TX_UART;   
        case UART_STATUS_TX_UART:
            /*
            * Transmit SPI data received
            * Optionally add check for errors 
            * Reset UART once complete
            */
                
            for (int i = 0; i < gMsgLength; i++) {
                /*
                 * Block if the TX FIFO is full before attempting
                 * to fill it.
                 */
                DL_UART_Main_transmitDataBlocking(UART_BRIDGE_INST, gSPIData[i]);
            }
            gSPICount = 0;
            gMsgLength = 0;
            gUARTBridgeStatus = UART_STATUS_WAITING;
            break;
        case UART_STATUS_ERROR:
            /*
            * Handle error cases' clean up here
            */
            if(!gError)
            {
                /* Error has already been transmitted. Reset */
                gUARTBridgeStatus = UART_STATUS_WAITING;
                gUartStartDetected = false;
            }
            gUartRxDataCount = 0;
            gUartRxData[RW_INDEX] = 0;
            gUartRxData[LENGTH_INDEX] = 0;
            break;
        default:
            break;
    }
}

void UART_BRIDGE_INST_IRQHandler(void)
{
    uint8_t rxTemp = 0;
    switch (DL_UART_Main_getPendingInterrupt(UART_BRIDGE_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            rxTemp = DL_UART_Main_receiveDataBlocking(UART_BRIDGE_INST);

            if ((!gUartStartDetected) && (rxTemp == UART_START_BYTE)) {
                gUartStartDetected =true;
                gUartRxDataCount = 0;
            }
            else if(gUartStartDetected){
                gUartRxData[gUartRxDataCount] = rxTemp;
                if (gUartRxDataCount >= UART_MAX_PACKET_SIZE){
                    /* Over run of max UART buffer size */
                    gError = ERROR_UARTFULL;
                    DL_UART_Main_disableInterrupt(UART_BRIDGE_INST, 
                                                   DL_UART_MAIN_INTERRUPT_RX);
                }
                else {
                gUartRxDataCount++;
                }
            }
            gDataReceived = true;
            break;
        default:
            break;
    }
}

void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
            /* Read RX FIFO */
            gSPIData[gSPICount++] = DL_SPI_receiveData8(SPI_0_INST);
            break;
        case DL_SPI_IIDX_TX:
            gSPIControllerStatus = SPI_STATUS_TX_COMPLETE;
            break;
        case DL_SPI_IIDX_TX_EMPTY:
            /* not used in this example */
            break;
        case DL_SPI_IIDX_RX_TIMEOUT:
            /* not used in this example */
            break;
        case DL_SPI_IIDX_RX_FULL:
            /* not used in this example */
            break;
        case DL_SPI_IIDX_TX_UNDERFLOW:
            /* not used in this example */
            break;
        case DL_SPI_IIDX_PARITY_ERROR:
            /* not used in this example */
            break;
        case DL_SPI_IIDX_RX_OVERFLOW:
            /* not used in this example */
            break;
        default:
            break;
    }
}
