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

#include "TPS92543_RWPACK.h"

// When an error in CRC of the received data is observed, set this to TRUE
bool rcvCrcError = false;

void WriteReg(uint8_t INIT, uint8_t DevID, uint8_t REGADDR, uint8_t *data,
    bool AckEnabled)
{
    uint8_t CommandFrame[MAX_BURST_CNT + 3 + 3] = {0};
    uint8_t numDataBytes, FrameTxLength = 0;
    uint32_t CRCCalculate;
    uint8_t i          = 0;
    uint8_t ReadOffset = 0;

    //Check if the write command is for signal device or Multicast
    if (isMulticastID(DevID)) {
        AckEnabled = false;
    }

    CommandFrame[0] = INIT;
    CommandFrame[1] = DevID;
    CommandFrame[2] = REGADDR;

    switch (INIT) {
        case WRITE1:
            numDataBytes = 1;
            break;
        case WRITE2:
            numDataBytes = 2;
            break;
        case WRITE3:
            numDataBytes = 3;
            break;
        case WRITE5:
            numDataBytes = 5;
            break;
        case WRITE4:
            numDataBytes = 4;
            break;
        case WRITE8:
            numDataBytes = 8;
            break;
        case WRITE12:
            numDataBytes = 12;
            break;
        case WRITE16:
            numDataBytes = 16;
            break;
        case WRITE20:
            numDataBytes = 20;
            break;
        case WRITE32:
            numDataBytes = 32;
            break;
        case WRITE64:
            numDataBytes = 64;
            break;
        default:
            numDataBytes = 0;
            break;
    }

    for (i = 0; i < numDataBytes; i++) {
        CommandFrame[3 + i] = data[i];
    }

    //CRC calculation
    if (numDataBytes == 64) {
        CRCCalculate = CRC_24_IBM(CommandFrame, 3 + numDataBytes);
        CommandFrame[3 + numDataBytes] =
            (uint8_t)(CRCCalculate & 0x000000FF);  //CRCL
        CommandFrame[4 + numDataBytes] =
            (uint8_t)((CRCCalculate >> 8) & 0x000000FF);  //CRCM
        CommandFrame[5 + numDataBytes] =
            (uint8_t)((CRCCalculate >> 16) & 0x000000FF);  //CRCH
        FrameTxLength = 3 + numDataBytes + 3;
    } else {
        CRCCalculate = CRC_16_IBM(CommandFrame, 3 + numDataBytes);
        CommandFrame[3 + numDataBytes] =
            (uint8_t)(CRCCalculate & 0x000000FF);  //CRCL
        CommandFrame[4 + numDataBytes] =
            (uint8_t)((CRCCalculate >> 8) & 0x000000FF);  //CRCH

        FrameTxLength = 3 + numDataBytes + 2;
    }

#if (COMM_TYPE == SEPTR_CAN)
    ReadOffset = FrameTxLength;
#endif

    //Transmit the data to Txbuffer
    for (i = 0; i < FrameTxLength; i++) {
        TmtBuffer[i] = CommandFrame[i];
    }

    //Start Communication
    if (AckEnabled) {
        //Receive 1byte ACK
        DMA_UARTTxRx(FrameTxLength, 1 + ReadOffset);

        //Check ACK
        if (RecBuffer[0 + ReadOffset] != TPSACK) {
            LED_ERR_ON;
            __BKPT(0);
        }
    } else {
        DMA_UARTTxRx(FrameTxLength, ReadOffset);
    }
}

void ReadReg(uint8_t INIT, uint8_t DevID, uint8_t REGADDR, uint8_t *data)
{
    uint8_t CommandFrame[5] = {0};
    uint8_t numDataBytes, FrameRxLength = 0;
    uint32_t CRCCalculate, CRCReceive;
    uint8_t i          = 0;
    uint8_t ReadOffset = 0;

    CommandFrame[0] = INIT;
    CommandFrame[1] = DevID;
    CommandFrame[2] = REGADDR;
    CRCCalculate    = CRC_16_IBM(CommandFrame, 3);
    CommandFrame[3] = (uint8_t)(CRCCalculate & 0x000000FF);         //CRCL
    CommandFrame[4] = (uint8_t)((CRCCalculate >> 8) & 0x000000FF);  //CRCH

    //Transmit the data to Txbuffer
    for (i = 0; i < 5; i++) {
        TmtBuffer[i] = CommandFrame[i];
    }

    switch (INIT) {
        case READ1:
            numDataBytes = 1;
            break;
        case READ2:
            numDataBytes = 2;
            break;
        case READ3:
            numDataBytes = 3;
            break;
        case READ5:
            numDataBytes = 5;
            break;
        case READ4:
            numDataBytes = 4;
            break;
        case READ8:
            numDataBytes = 8;
            break;
        case READ12:
            numDataBytes = 12;
            break;
        case READ16:
            numDataBytes = 16;
            break;
        case READ20:
            numDataBytes = 20;
            break;
        case READ32:
            numDataBytes = 32;
            break;
        case READ64:
            numDataBytes = 64;
            break;
        default:
            numDataBytes = 0;
            break;
    }

#if (COMM_TYPE == SEPTR_CAN)
    ReadOffset = 5;
#endif

    if (numDataBytes == 64) {
        FrameRxLength = numDataBytes + 3 + ReadOffset;

        //Start Communication
        DMA_UARTTxRx(5, FrameRxLength);

        // CRC calculate
        CRCReceive = RecBuffer[FrameRxLength - 3] |
                     (RecBuffer[FrameRxLength - 2] << 8) |
                     (RecBuffer[FrameRxLength - 1] << 16);
        CRCCalculate = CRC_24_IBM(&RecBuffer[ReadOffset], numDataBytes);

    } else {
        FrameRxLength = numDataBytes + 2 + ReadOffset;

        //Start Communication
        DMA_UARTTxRx(5, FrameRxLength);

        // CRC calculate
        CRCReceive =
            RecBuffer[FrameRxLength - 2] | (RecBuffer[FrameRxLength - 1] << 8);
        CRCCalculate = CRC_16_IBM(&RecBuffer[ReadOffset], numDataBytes);
    }

    // CRC check
    if (CRCCalculate != CRCReceive) {
        rcvCrcError = true;
        return;
    } else {
        rcvCrcError = false;

        //Transmit the receive data to customer buffer
        for (i = 0; i < numDataBytes; i++) {
            data[i] = RecBuffer[i + ReadOffset];
        }
    }
}

void CommsRst(void)
{
    uint16_t lowLevelTime = 12 * 80000000 / UART_0_BAUD_RATE;
    DL_UART_enableLINSendBreak(UART_0_INST);
    delay_cycles(lowLevelTime - 15);
    DL_UART_disableLINSendBreak(UART_0_INST);
}

bool isMulticastID(uint8_t DevID)
{
    uint8_t DEV[5];
    uint8_t P1Rec, P0Rec, P1Cal, P0Cal, VOL = 0;

    DEV[0] = DevID & 0x01;
    DEV[1] = (DevID >> 1) & 0x01;
    DEV[2] = (DevID >> 2) & 0x01;
    DEV[3] = (DevID >> 3) & 0x01;
    DEV[4] = (DevID >> 4) & 0x01;
    VOL    = (DevID >> 5) & 0x01;
    P0Rec  = (~DevID >> 6) & 0x01;
    P1Rec  = (~DevID >> 7) & 0x01;

    P0Cal = (DEV[0] ^ DEV[1] ^ DEV[2] ^ DEV[4]);
    P1Cal = !(VOL ^ DEV[1] ^ DEV[3] ^ DEV[4]);

    if ((P0Cal == P0Rec) && (P1Cal == P1Rec)) {
        return true;
    } else {
        return false;
    }
}
