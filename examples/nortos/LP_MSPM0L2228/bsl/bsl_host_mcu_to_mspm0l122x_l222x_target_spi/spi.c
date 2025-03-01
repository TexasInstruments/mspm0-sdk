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
#include "spi.h"
#include <bsl_spi.h>
#include <stddef.h>
#include <stdint.h>
#include "ti/driverlib/driverlib.h"
#include "ti_msp_dl_config.h"

typedef enum {
    Rx_idle     = 0x0,
    Rx_waiting  = 0x1,
    Rx_response = 0x2
} SPI_RX_states;

#define READ_REQUEST 0x00

volatile SPI_RX_states SPI_RX_state;
volatile uint16_t gLength;
volatile uint16_t gSize;
volatile uint8_t *gRxBuffer;
volatile uint8_t gRxData;
volatile uint8_t gByteRx;

void SPI_IRQHandler(void);

void SPI_Initialize(void)
{
    NVIC_ClearPendingIRQ(SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
    DL_Interrupt_registerInterrupt(
        (uint32_t) SPI_0_INST_INT_IRQN, SPI_IRQHandler);
    DL_SPI_enableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_RX);
}

/*-------------------------------------------------*/

void SPI_SendByte(uint8_t data)
{
    while (DL_SPI_isTXFIFOFull(SPI_0_INST))
        ;
    DL_SPI_transmitData8(SPI_0_INST, data);
    while (DL_SPI_isBusy(SPI_0_INST))
        ;
}

/*-------------------------------------------------*/

void SPI_SendByteArray(uint8_t *data, uint8_t dataLength)
{
    uint8_t i = 0;
    for (i = 0; i < dataLength; i++) {
        while (DL_SPI_isTXFIFOFull(SPI_0_INST))
            ;
        DL_SPI_transmitData8(SPI_0_INST, data[i]);
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
    }
}
/*-------------------------------------------------*/

uint8_t SPI_ReadByte()
{
    while (1) {
        if (DL_SPI_isRXFIFOEmpty(SPI_0_INST) == false) {
            gRxData = DL_SPI_receiveData8(SPI_0_INST);
            return gRxData;
        }
    }
}

/*-------------------------------------------------*/

void SPI_ReadByteArray(uint8_t *data, uint16_t dataLength)
{
    gSize        = 0;
    gLength      = dataLength;
    gRxBuffer    = data;
    SPI_RX_state = Rx_waiting;
    while (SPI_RX_state != Rx_idle) {
        SPI_SendByte(READ_REQUEST);
    }
    while (DL_SPI_getRawInterruptStatus(SPI_0_INST, DL_SPI_INTERRUPT_IDLE) !=
           DL_SPI_INTERRUPT_IDLE)
        ;
}

/*-------------------------------------------------*/
void SPI_Close(void)
{
    DL_SPI_disable(SPI_0_INST);
    DL_SPI_disableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_RX);
    DL_Interrupt_unregisterInterrupt((uint32_t) SPI_0_INST_INT_IRQN);
    NVIC_DisableIRQ(SPI_0_INST_INT_IRQN);
    DL_SPI_reset(SPI_0_INST);
}

/*-------------------------------------------------*/

void SPI_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
            gByteRx = SPI_ReadByte();
            if (SPI_RX_state == Rx_waiting) {
                if (gByteRx != 0x00) SPI_RX_state = Rx_response;
            }
            if (SPI_RX_state == Rx_response) {
                gRxBuffer[gSize] = gByteRx;
                gSize++;
                if (gSize == gLength) SPI_RX_state = Rx_idle;
            }
            break;
        default:
            break;
    }
}

/*-------------------------------------------------*/

uint8_t Status_check(void)
{
    uint8_t buf[1] = {0xBB};
    SPI_SendByteArray(buf, 1);
    SPI_ReadByteArray(BSL_RX_buffer, 1);
    return BSL_RX_buffer[0];
}
