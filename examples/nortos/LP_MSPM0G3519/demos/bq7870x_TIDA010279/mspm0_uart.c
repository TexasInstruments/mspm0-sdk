/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include "mspm0_uart.h"

volatile bool Uartrec = 0;

/********************************************************************************
 * UART Tx/Rx function
 */
void UART_Send(UART_Regs *uart, uint32_t length, uint8_t *data)
{
    while (length > 0U) {
        __NOP();
        DL_UART_transmitDataBlocking(uart, *data);
        data++;
        length--;
    }

    /* Wait for UART transmission Done */
    while (DL_UART_Main_isBusy(uart))
        ;
}

uint32_t UART_Receive(UART_Regs *uart, uint32_t length, uint8_t *data)
{
    uint32_t receive_count = 0;
    while (length > 0U) {
        *data = DL_UART_receiveDataBlocking(uart);
        data++;
        length--;
        receive_count++;
    }
    return receive_count;
}

uint32_t UART_Receive_TO(
    UART_Regs *uart, uint32_t length, uint8_t *data, uint32_t timeout)
{
    uint32_t receive_count = 0;

    while (timeout > 0) {
        if (Uartrec == true) {
            DL_UART_disableInterrupt(uart, DL_UART_MAIN_INTERRUPT_RX);
            while (length > 0U) {
                *data = DL_UART_receiveDataBlocking(uart);
                data++;
                length--;
                receive_count++;
            }
            Uartrec = false;
            break;
        }
        timeout--;
    }

    DL_UART_clearInterruptStatus(uart, DL_UART_MAIN_INTERRUPT_RX);
    DL_UART_enableInterrupt(uart, DL_UART_MAIN_INTERRUPT_RX);

    return receive_count;
}

void UART_clearRxFIFO(UART_Regs *uart)
{
    while (!DL_UART_isRXFIFOEmpty(uart)) {
        DL_UART_receiveData(uart);
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            Uartrec = true;
            break;
        default:
            break;
    }
}
