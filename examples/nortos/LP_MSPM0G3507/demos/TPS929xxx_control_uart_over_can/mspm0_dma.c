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

#include "mspm0_dma.h"
#include "mspm0_gpio.h"
#include "mspm0_timer.h"

#include "FlexWire.h"
#include "system_info.h"

volatile bool gCheckUART, gDMATxDone, gDMARxDone;

void dmaInitial(void)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ledXmtBuffer[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART_1_INST->TXDATA));

    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&UART_1_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &ledRcvBuffer[0]);
}

void dmaTransfer(
    uint32_t dmaTxSize, uint32_t dmaRxSize, uint32_t responseEnabled)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ledXmtBuffer[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &ledRcvBuffer[0]);

    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, dmaTxSize);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, dmaRxSize);

    // Enable Tx DMA
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    if (responseEnabled == TRUE) {
        // Clear IFG to prevent previous received data to be transferred by the DMA
        DL_UART_Main_clearInterruptStatus(
            UART_1_INST, DL_UART_MAIN_INTERRUPT_RX);

        // Read all available data out of the RX FIFO using 8 bit access
        for (uint32_t i = 0; i < 4; i++) {
            if (!DL_UART_isRXFIFOEmpty(UART_1_INST)) {
                DL_UART_receiveData(UART_1_INST);
            } else {
                break;
            }
        }

        // Enable Rx DMA
        DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

        // In order to avoid program entering infinite waiting for receiving specified numbers of response,
        // we set a longest wait time, if the response wait time exceed the time, the program will also exit the wait
        // Launch timer and set wait time = 200ms
        watchDog(25000);

        // Received all response byte or the wait time exceeds the specified time
        while ((!gDMARxDone) && (timeOutFlag == 0))
            ;

        // Have not received all specified numbers of response in defined time
        if (timeOutFlag == 1) {
            // You can take some action once the response has not been received
            // Below is an example: turn on one red LED on the launch board
            // Set P1.0 high to turn on the red LED in the launch pad
            LED_ERR_ON;
        } else {
            // Successfully received response, clear the red LED
            LED_ERR_OFF;
        }

        delay_cycles(32);

        // Disable DMA0 DMA1
        DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);
        DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
        gDMARxDone = false;
        gDMATxDone = false;
    } else {
        // In order to avoid program entering infinite waiting for sending specified numbers of command bytes,
        // we set a longest wait time, if the response wait time exceed the time, the program will also exit the wait
        // Launch timer and set wait time = 200ms
        watchDog(25000);

        // Send all command bytes or the wait time exceeds the specified time
        while (!gDMATxDone && (timeOutFlag == 0))
            ;

        // Have not sent all specified numbers of command bytes in defined time
        if (timeOutFlag == 1) {
            // You can take some action once the command bytes have not been sent
            // Below is an example: turn on one red LED on the launch board
            // Set P1.0 high to turn on the red LED in the launch pad
            DL_GPIO_setPins(GPIO_GRP_0_LED1_PORT, GPIO_GRP_0_LED1_PIN);
        } else {
            // Successfully sent all command bytes, clear the red LED
            DL_GPIO_clearPins(GPIO_GRP_0_LED1_PORT, GPIO_GRP_0_LED1_PIN);
        }

        // Disable DMA0
        DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
        gDMATxDone = false;
    }
}

void UART_1_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_1_INST)) {
        case DL_UART_MAIN_IIDX_EOT_DONE:
            gCheckUART = true;
            break;
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
