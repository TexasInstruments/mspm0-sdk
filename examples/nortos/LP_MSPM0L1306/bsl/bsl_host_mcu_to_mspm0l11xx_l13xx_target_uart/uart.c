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
#include "uart.h"
#include "stdint.h"
#include "ti_msp_dl_config.h"

uint8_t test_d;
uint8_t UART_writeBuffer(uint8_t *pData, uint8_t ui8Cnt)
{
    uint8_t res;

    while (ui8Cnt--) {
        DL_UART_transmitDataBlocking(UART_0_INST, *pData);
        pData++;
        // __delay_cycles(10000);
    }
    res = DL_UART_receiveDataBlocking(UART_0_INST);
    return res;
}

void UART_readBuffer(uint8_t *pData, uint8_t ui8Cnt)
{
    //   uint8_t res;
    while (ui8Cnt-- > 0) {
        *pData = DL_UART_receiveDataBlocking(UART_0_INST);
        pData++;
    }
}

//uint8_t test_d;

uint8_t Status_check(void)
{
    uint8_t res;
    DL_UART_transmitDataBlocking(UART_0_INST, 0xBB);
    /* Wait until all bytes have been transmitted and the TX FIFO is empty */
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;
    res = DL_UART_receiveDataBlocking(UART_0_INST);
    // res = DL_UART_receiveDataBlocking(UART_0_INST);
    return res;
}
