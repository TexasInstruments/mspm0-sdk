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
#include "UART_communication.h"
#include "BQ769x2_Configs/BQ769x2_protocol.h"

RS485_t gRS485;

/******************************************************************
 *  @brief      Send data to UART FIFO and wait for finish
 *
 *  @param[in]  BQid indicate Top or Bottom AFE
 *  @param[in]  cmd  indicate the type of data sent
 *  @param[in]  RS485_data  Pointer to the data array
 *  @param[in]  RS485_count Length of data to send
 *  CRC: BQid + cmd + data (length*2 + 2)
 */
void RS485_Send(uint8_t BQid, uint8_t cmd, uint16_t *data, uint32_t length)
{
    DL_GPIO_setPins(RS485_PORT, RS485_TX_EN_PIN);
    DL_GPIO_clearPins(RS485_PORT, RS485_RX_EN_PIN);

    uint8_t RS485_data[RS485_TX_MAX_PACKET_SIZE] = {0};
    uint32_t RS485_count;

    RS485_count = length * 2 + 2;

    if ((RS485_count > RS485_TX_MAX_PACKET_SIZE) ||
        (gRS485.status != RS485_STATUS_IDLE)) {
        gRS485.status = RS485_STATUS_ERROR;
        return;
    }

    RS485_data[0] = BQid;
    RS485_data[1] = cmd;

    for (int i = 0; i < length; i++) {
        RS485_data[(i + 1) * 2]     = (data[i] & 0x00FF);
        RS485_data[(i + 1) * 2 + 1] = (data[i] & 0xFF00) >> 8;
    }
    RS485_data[(length + 1) * 2] = CRC8(RS485_data, length * 2 + 2);

    gRS485.status  = RS485_STATUS_TX_STARTED;
    gRS485.txLen   = RS485_count;
    gRS485.txCount = 0;

    while (RS485_count--) {
        gRS485.txPacket[RS485_count] = RS485_data[RS485_count];
    }

    gRS485.txCount =
        DL_UART_Main_fillTXFIFO(RS485_INST, &gRS485.txPacket[0], gRS485.txLen);

    DL_UART_Main_clearInterruptStatus(RS485_INST, DL_UART_MAIN_INTERRUPT_TX);
    DL_UART_Main_enableInterrupt(RS485_INST, DL_UART_MAIN_INTERRUPT_TX);

    while (gRS485.status != RS485_STATUS_TX_COMPLETE)
        ;
    DL_UART_Main_disableInterrupt(RS485_INST, DL_UART_MAIN_INTERRUPT_TX);
    gRS485.status = RS485_STATUS_IDLE;

    DL_GPIO_clearPins(RS485_PORT, RS485_TX_EN_PIN);
    DL_GPIO_setPins(RS485_PORT, RS485_RX_EN_PIN);
}

/******************************************************************
 *  @brief      Receive data from UART and wait for finish
 *
 *  @param[in]  RS485_data  Pointer to the data array to receive
 *  @param[in]  RS485_count Length of data to receive
 */
void RS485_Receive(uint8_t *RS485_data, uint32_t RS485_count)
{
    DL_GPIO_clearPins(RS485_PORT, RS485_TX_EN_PIN);
    DL_GPIO_setPins(RS485_PORT, RS485_RX_EN_PIN);

    if ((RS485_count > RS485_RX_MAX_PACKET_SIZE) ||
        (gRS485.status != RS485_STATUS_IDLE)) {
        gRS485.status = RS485_STATUS_ERROR;
        return;
    }

    gRS485.status  = RS485_STATUS_RX_STARTED;
    gRS485.rxLen   = RS485_count;
    gRS485.rxCount = 0;

    DL_UART_Main_clearInterruptStatus(RS485_INST, DL_UART_MAIN_INTERRUPT_RX);
    DL_UART_Main_enableInterrupt(RS485_INST, DL_UART_MAIN_INTERRUPT_RX);

    while (gRS485.status != RS485_STATUS_RX_COMPLETE)
        ;
    DL_UART_Main_disableInterrupt(RS485_INST, DL_UART_MAIN_INTERRUPT_RX);
    gRS485.status = RS485_STATUS_IDLE;

    while (RS485_count--) {
        RS485_data[RS485_count] = gRS485.rxPacket[RS485_count];
    }

    DL_GPIO_setPins(RS485_PORT, RS485_TX_EN_PIN);
    DL_GPIO_clearPins(RS485_PORT, RS485_RX_EN_PIN);
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(RS485_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            switch (gRS485.status) {
                case RS485_STATUS_RX_STARTED:
                    gRS485.rxPacket[gRS485.rxCount++] =
                        DL_UART_receiveDataBlocking(RS485_INST);
                    gRS485.status = RS485_STATUS_RX_INPROGRESS;
                    break;
                case RS485_STATUS_RX_INPROGRESS:
                    if (gRS485.rxCount <= gRS485.rxLen) {
                        gRS485.rxPacket[gRS485.rxCount++] =
                            DL_UART_receiveDataBlocking(RS485_INST);
                    } else {
                        DL_UART_receiveDataBlocking(RS485_INST);
                    }

                    if (gRS485.rxCount == gRS485.rxLen) {
                        gRS485.status = RS485_STATUS_RX_COMPLETE;
                    }
                    break;
                case RS485_STATUS_RX_COMPLETE:
                default:
                    gRS485.status = RS485_STATUS_ERROR;
                    break;
            }
            break;
        case DL_UART_MAIN_IIDX_TX:
            switch (gRS485.status) {
                case RS485_STATUS_TX_STARTED:
                    if (gRS485.txCount < gRS485.txLen) {
                        gRS485.txCount += DL_UART_Main_fillTXFIFO(RS485_INST,
                            &gRS485.txPacket[gRS485.txCount],
                            gRS485.txLen - gRS485.txCount);
                    }

                    if (gRS485.txCount == gRS485.txLen) {
                        gRS485.status = RS485_STATUS_TX_INPROGRESS;
                    }
                    break;
                case RS485_STATUS_TX_INPROGRESS:
                    gRS485.status = RS485_STATUS_TX_COMPLETE;
                    break;
                case RS485_STATUS_TX_COMPLETE:
                default:
                    gRS485.status = RS485_STATUS_ERROR;
                    break;
            }
            break;
        default:
            break;
    }
}
