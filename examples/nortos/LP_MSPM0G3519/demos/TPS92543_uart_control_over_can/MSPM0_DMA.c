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

#include "MSPM0_DMA.h"

volatile bool gDMATxDone, gDMARxDone = false;
// Transmit/Receive buffer in UART communication
uint8_t TmtBuffer[MAX_BURST_CNT + 3 +
                  3];  //+3:INIT, DEVID, REGADDR   +3:CRCL CRCM CRCH
uint8_t RecBuffer[MAX_BURST_CNT + 3];  //+3:CRCL CRCM CRCH

void DMA_init(void)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_0_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &RecBuffer[0]);
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &TmtBuffer[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &UART_0_INST->TXDATA);
}

void DMA_UARTTxRx(uint8_t TxLength, uint8_t RxLength)
{
    //Config RX DMA, whether enable depends on the RxLength
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &RecBuffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, RxLength);

    //Whether TX or RX, both of them all need MCU sending command firstly
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &TmtBuffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, TxLength);

    //Start TX
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    if (RxLength == 0) {  //No receive
        // In order to avoid program entering infinite waiting for sending specified numbers of command bytes,
        // we set a longest wait time, if the response wait time exceed the time, the program will also exit the wait
        // Launch timer and set wait time = 200ms
        watchDog(Time1ms * 200);
        while ((gDMATxDone == false) && (timeOutFlag == false)) {
            __NOP();  //Bug for TICLANG
        }
        watchDogStop();
        if (timeOutFlag) {
            // You can take some action once the command bytes have not been sent
            // Below is an example: turn on one red LED on the launch board
            // Set P1.0 high to turn on the red LED in the launch pad
            LED_ERR_ON;
            __BKPT(0);
        } else {
            LED_ERR_OFF;
        }
        delay_cycles(800);  // wait last byte finishes transmitting
    } else {                //Receive data
        //Start RX
        DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
        watchDog(Time1ms * 200);

        while ((gDMARxDone == false) && (timeOutFlag == false)) {
            __NOP();  //Bug for TICLANG
        }
        watchDogStop();
        if (timeOutFlag) {
            // You can take some action once the command bytes have not been sent
            // Below is an example: turn on one red LED on the launch board
            // Set P1.0 high to turn on the red LED in the launch pad
            LED_ERR_ON;
            __BKPT(0);
        } else {
            LED_ERR_OFF;
        }
        delay_cycles(800);
        gDMARxDone = false;
        DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
    }

    gDMATxDone = false;
    NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
    DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_DMA_DONE_TX:
            gDMATxDone = true;
            break;
        case DL_UART_MAIN_IIDX_DMA_DONE_RX:
            gDMARxDone = true;
            break;
        default:
            break;
    }
}
