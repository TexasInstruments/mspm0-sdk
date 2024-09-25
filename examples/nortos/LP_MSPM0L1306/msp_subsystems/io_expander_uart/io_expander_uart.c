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

#include "ti_msp_dl_config.h"

#define UART_PACKET_SIZE (11)

/* Data for UART to transmit */
uint8_t gTxPacket[UART_PACKET_SIZE] = {0x5A, 0xA5, 0x02, 0x11, 0x01, 0x80, 0x00, 0x00, 0x10, 0x5B, 0xFF};

/*
 * Helper function to transmit a packet of data, and ensure that the
 * transmitter is ready before sending data
 */
void transmitPacketBlocking(uint8_t *buffer, uint32_t size)
{
    uint32_t i;
    for (i = 0; i < size; i++) {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, buffer[i]);
    }
}

uint16_t checkSum1ByteIn2ByteOut(uint8_t* data, int len)
{
    uint16_t checksum = 0;
    for (int i = 0; i < len; i++) {
        checksum += data[i];
    }
    checksum = ~checksum;
    return checksum;
}


int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerG_startCounter(TIMER_0_INST);
    while (1) {
        __WFI();
    }
}

void GROUP1_IRQHandler(void)
{
    uint32_t dataStatus = 0;
    uint16_t dataTemp = 0;
    uint16_t signalChecksum = 0;
    if (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {

        dataStatus = (GPIOA->DIN31_0);
        dataTemp = (dataStatus >> 10);
        gTxPacket[7] = dataTemp >> 8;
        gTxPacket[8] = dataTemp & 0xFF;

        signalChecksum = checkSum1ByteIn2ByteOut((gTxPacket + 2), 7);

        gTxPacket[10] = signalChecksum >> 8;
        gTxPacket[9] = signalChecksum & 0xFF;

        DL_TimerG_stopCounter(TIMER_0_INST);
        DL_TimerG_setTimerCount(TIMER_0_INST,TIMER_0_INST_LOAD_VALUE);
        DL_TimerG_startCounter(TIMER_0_INST);

        transmitPacketBlocking(gTxPacket,UART_PACKET_SIZE);
    }
}

void TIMG0_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            transmitPacketBlocking(gTxPacket,UART_PACKET_SIZE);
            break;
        default:
            break;
    }
}
