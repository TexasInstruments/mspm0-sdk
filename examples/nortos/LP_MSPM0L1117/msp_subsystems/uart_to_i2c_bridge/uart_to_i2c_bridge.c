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

#include "ti/driverlib/dl_uart_main.h"
#include "ti_msp_dl_config.h"


/* Delay for 5ms to ensure UART TX is idle before starting transmission */
#define UART_TX_DELAY (160000)

/* Define UART Header and Start Byte*/
#define UART_HEADER_LENGTH 0x03
#define UART_START_BYTE 0xF8
#define UART_READ_I2C_BYTE 0xFA
#define UART_WRITE_I2C_BYTE 0xFB
#define ADDRESS_INDEX   0x00
#define RW_INDEX        0x01
#define LENGTH_INDEX    0x02

/*Define max packet sizes*/
#define I2C_MAX_PACKET_SIZE 16
#define UART_MAX_PACKET_SIZE (I2C_MAX_PACKET_SIZE + UART_HEADER_LENGTH)

/*global variables for keeping track of bridge process*/
uint8_t gUartTxData[UART_MAX_PACKET_SIZE] ; 
uint8_t gUartRxData[UART_MAX_PACKET_SIZE] ;
volatile uint8_t gUartRxDataCount = 0;
uint8_t gI2C_Data[I2C_MAX_PACKET_SIZE];
volatile uint8_t gI2C_Address = 0;
volatile uint8_t gI2C_Length = 0;
volatile uint8_t gI2C_RW = 0;
volatile uint8_t gI2C_Count = 0;
volatile bool gDataReceived;
volatile bool gUartStartDetected = false;
volatile bool gUartFull = false;

/* Indicates status of Bridge */
enum UARTBridgeStatus {
    UART_STATUS_WAITING = 0,
    UART_STATUS_START,
    UART_STATUS_WRITE_I2C,
    UART_STATUS_READ_I2C,
    UART_STATUS_TX_UART,
    UART_STATUS_ERROR
} gUARTBridgeStatus;

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

/* Error Flags*/
enum ErrorFlags {
    ERROR_NONE = 0,
    ERROR_UARTFULL = 31,
    ERROR_UART_OVERRUN = 32,
    ERROR_IMPROPER_MESSAGE_LENGTH = 33,
    ERROR_I2C_NACK = 41,
    ERROR_I2C_OVERUN = 42,
    ERROR_I2C_NO_RW = 43,
} gError;

void Uart_Bridge(void);

int main(void)
{
    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(UART_BRIDGE_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_BRIDGE_INST_INT_IRQN);
    NVIC_EnableIRQ(I2C_BRIDGE_INST_INT_IRQN);

    /* Optional delay to ensure UART TX is idle before starting transmission */
    delay_cycles(UART_TX_DELAY);
    /*Initial states*/
    gError = ERROR_NONE;
    gUARTBridgeStatus = UART_STATUS_WAITING;
    gI2cControllerStatus = I2C_STATUS_IDLE;
    gDataReceived = false;

    while (gDataReceived == false) {
        __WFE();
    }
    /* Data received, start bridge */
    while (1) {
    /*
    * Optionally check for errors and do something
    * Some examples give below
    */
        if (gError) {
            // TX error code upstream
            DL_UART_Main_transmitDataBlocking(UART_BRIDGE_INST, gError);
            
            if(gError == ERROR_UARTFULL){
                DL_UART_Main_enableInterrupt(UART_BRIDGE_INST, 
                                            DL_UART_MAIN_INTERRUPT_RX );
            }
            gError = ERROR_NONE;
            gUartStartDetected = false;
        }

            Uart_Bridge();
    __WFE();
    }
}

void Uart_Bridge(){
    uint8_t i=0;
    uint8_t k=0;
    switch (gUARTBridgeStatus) {
        case UART_STATUS_WAITING:
        /* Wait for header information */
            if (gUartRxDataCount >= UART_HEADER_LENGTH) {
                gUARTBridgeStatus = UART_STATUS_START;
                gI2C_Address = gUartRxData[ADDRESS_INDEX];
                gI2C_Length = gUartRxData[LENGTH_INDEX];
                gI2C_RW = gUartRxData[RW_INDEX];            
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

            /*Check for error cases first*/
            if (!gI2C_Length) {
                gError = ERROR_IMPROPER_MESSAGE_LENGTH;
                gUARTBridgeStatus = UART_STATUS_ERROR;
                break;
            }
            if (gI2C_Length > I2C_MAX_PACKET_SIZE) {
                gError = ERROR_IMPROPER_MESSAGE_LENGTH;
                gUARTBridgeStatus = UART_STATUS_ERROR;
                break;
            }
            if (gI2C_RW == UART_WRITE_I2C_BYTE) {
                
                while (gI2C_Length > (gUartRxDataCount - UART_HEADER_LENGTH) ) {
                    /* Wait until full message is received*/
                     __WFE();
                }
                /*Transfer UART Buffer over */
                for (i = 0; i <= gI2C_Length; i++) {
                    gI2C_Data[i] = gUartRxData[i + UART_HEADER_LENGTH];
                }
                gUARTBridgeStatus = UART_STATUS_WRITE_I2C;
            }
            else if (gI2C_RW == UART_READ_I2C_BYTE)  {
                gUARTBridgeStatus = UART_STATUS_READ_I2C;     
                /* Optionally clear gI2C_Data buffer here before read*/           
            }
            else {
                 /*Error: Wrong RW value */
                 gError = ERROR_I2C_NO_RW;
                 gUARTBridgeStatus = UART_STATUS_ERROR;
            }
            /*Full message received, advance and reset UART RX */
            gUartRxDataCount = 0;
            gUartRxData[ADDRESS_INDEX] = 0;
            gUartRxData[LENGTH_INDEX] = 0;
            gUartStartDetected = false;
            gI2C_RW = 0;
            break;
        case UART_STATUS_WRITE_I2C:
           /*
            * Perform I2C Write
            * Fill the FIFO
            *  The FIFO is 8-bytes deep, and this function will return number
            *  of bytes written to FIFO
             */
            gI2C_Count = DL_I2C_fillControllerTXFIFO(I2C_BRIDGE_INST, &gI2C_Data[0], gI2C_Length);
            
            /* Enable TXFIFO trigger interrupt if there are more bytes to send */
            if (gI2C_Count < gI2C_Length) {
                DL_I2C_enableInterrupt(
                    I2C_BRIDGE_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            } else {
                DL_I2C_disableInterrupt(
                    I2C_BRIDGE_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            }
                /*
                * Send the packet to the controller.
                * This function will send Start + Stop automatically.
                */
                gI2cControllerStatus = I2C_STATUS_TX_STARTED;
                while (!(
                    DL_I2C_getControllerStatus(I2C_BRIDGE_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
                    ;
                DL_I2C_startControllerTransfer(
                    I2C_BRIDGE_INST, gI2C_Address, DL_I2C_CONTROLLER_DIRECTION_TX, gI2C_Length);

                /* Wait until the Controller sends all bytes */
                while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
                    (gI2cControllerStatus != I2C_STATUS_ERROR)) {
                    __WFE();
                }
                while (DL_I2C_getControllerStatus(I2C_BRIDGE_INST) &
                    DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
                    ;
               
                /*Optionally add trap or UART TX for I2C error detection*/

                while (!(
                    DL_I2C_getControllerStatus(I2C_BRIDGE_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
                    ;
                gI2C_Count = 0;
                gI2C_Length = 0;
                gUARTBridgeStatus = UART_STATUS_WAITING;
            break;                
        case UART_STATUS_READ_I2C:
            /*
            * Perform I2C Read and ready UART TX upstream
            */

            /* Send a read request to Target */
            gI2C_Count             = 0;
            gI2cControllerStatus = I2C_STATUS_RX_STARTED;
            DL_I2C_startControllerTransfer(
                I2C_BRIDGE_INST, gI2C_Address, DL_I2C_CONTROLLER_DIRECTION_RX, gI2C_Length);

            /* Wait for all bytes to be received in interrupt */
            while (gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) {
                __WFE();
            }

            while (DL_I2C_getControllerStatus(I2C_BRIDGE_INST) &
                DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
                ;
            gUARTBridgeStatus = UART_STATUS_TX_UART;   
        case UART_STATUS_TX_UART:
            /*
            * Transmit I2C data received 
            * Optionally add check for errors 
            * Reset UART once complete
            */
                
                for (k = 0; k < gI2C_Length; k++) {
                    /*
                    * Block if the TX FIFO is full before attempting
                    * to fill it.
                    */
                    DL_UART_Main_transmitDataBlocking(UART_BRIDGE_INST, gI2C_Data[k]);
                }
            gI2C_Count = 0;
            gI2C_Length = 0;    
            gUARTBridgeStatus = UART_STATUS_WAITING;
            break;
        case UART_STATUS_ERROR:
            /*
            * Handle error cases' clean up here
            */
            if(!gError)
            {
                //Error has already been transmitted. Reset
                gUARTBridgeStatus = UART_STATUS_WAITING;
                gUartStartDetected = false;
            }
            gUartRxDataCount = 0;
            gUartRxData[ADDRESS_INDEX] = 0;
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
                    /*Over run of max UART buffer size*/
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

void I2C_BRIDGE_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_BRIDGE_INST)) {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_BRIDGE_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
            break;
        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
            /* Receive all bytes from target */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_BRIDGE_INST) != true) {
                if (gI2C_Count < gI2C_Length) {
                    gI2C_Data[gI2C_Count++] =
                        DL_I2C_receiveControllerData(I2C_BRIDGE_INST);
                } else {
                    /* 
                    * Ignore and remove from FIFO if the buffer is full
                    * Optionally add error flag update
                     */
                    DL_I2C_receiveControllerData(I2C_BRIDGE_INST);
                    gError = ERROR_I2C_OVERUN;
                }
            }
            break;
        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with next bytes to send */
            if (gI2C_Count < gI2C_Length) {
                gI2C_Count += DL_I2C_fillControllerTXFIFO(
                    I2C_BRIDGE_INST, &gI2C_Data[gI2C_Count], gI2C_Length - gI2C_Count);
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
            gError = ERROR_I2C_NACK;
            break;
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