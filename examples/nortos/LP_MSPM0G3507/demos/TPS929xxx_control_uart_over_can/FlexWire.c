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
#include "ti_msp_dl_config.h"

#include "CRC_LUT.h"
#include "FlexWire.h"
#include "TPS929xxx_APIs.h"
#include "system_info.h"

char ledXmtBuffer[MAX_BURST_CNT + 4];
char ledRcvBuffer[MAX_BURST_CNT + 6];

// When an error in CRC of the received data is observed, set this to TRUE
unsigned int rcvCrcError;

unsigned int ackEnabled[DEVICE_ADDR__15 + 1];

void FlexReadWriteInternal(unsigned int dev_addr, unsigned int reg_addr,
    unsigned int *dataBytes, unsigned int length, unsigned int checkResponse,
    unsigned int checkRcvCrc)
{
    // One longest command frame length is MAX_BURST_CNT + 4
    unsigned int commandFrame[MAX_BURST_CNT + 4];
    unsigned int i = 0;
    unsigned int dataLength, ledXmtFrameSize, ledRcvFrameSize, crcCheckSize;
    unsigned int response_needed;
    unsigned int wait_for_response;
    unsigned int check_receive_crc = (checkRcvCrc | ALWAYS_CHECK_CRC);

    response_needed = checkResponse;

    commandFrame[0] = SYNC;  //SYNC byte
    commandFrame[1] = dev_addr;
    commandFrame[2] = reg_addr;

    // Make function to be able to distinguish TPS929120 with TPS929160 and TPS929240
    dataLength = DATA_LENGTH(length);

    if (dev_addr & BROADCAST) {
        wait_for_response = FALSE;
    } else {
        if ((ackEnabled[dev_addr & DEVICE_ADDR__15] == FALSE) &&
            (dev_addr & WRITE_MODE)) {
            wait_for_response = FALSE;
        } else {
            wait_for_response = TRUE;
        }
    }

    // Distinguish between read and write mode
    if (dev_addr & WRITE_MODE) {
        // Write mode
        ledXmtFrameSize = 4 + dataLength;
        ledRcvFrameSize = 2;
        crcCheckSize    = 2;
        for (i = 0; i < dataLength; i++) {
            commandFrame[i + 3] = dataBytes[i];
        }
        if (check_receive_crc == TRUE) {
            if (!(dev_addr & BROADCAST)) {
                response_needed = TRUE;
            }
        }
    } else {
        // Read mode
        ledXmtFrameSize = 4;
        ledRcvFrameSize = 1 + dataLength;
        crcCheckSize    = ledRcvFrameSize;
    }

    // Calculate CRC of all the command frame bytes
    commandFrame[i + 3] = CRC_LUT((uint32_t *) commandFrame, i + 3);

    for (i = 0; i < ledXmtFrameSize; i++) {
        ledXmtBuffer[i] = (char) commandFrame[i];
    }

    // When CAN is used, the read_offset and the number of received bytes depend on the number of transmitted bytes
    if (CAN_USED == TRUE) {
        read_offset = ledXmtFrameSize;
        ledRcvFrameSize += ledXmtFrameSize;
    }

    // Data will be stored in ledXmtBuffer
    dmaTransfer(ledXmtFrameSize, ledRcvFrameSize, wait_for_response);

    rcvCrcError = FALSE;

    // If requested, check CRC
    if ((response_needed == TRUE) & (check_receive_crc == TRUE)) {
        for (i = 1; i < crcCheckSize; i++) {
            commandFrame[i] = ledRcvBuffer[i + read_offset - 1];
        }
        // Check if calculated CRC matched received CRC
        if ((CRC_LUT((uint32_t *) commandFrame, i) & 0x7F) !=
            (ledRcvBuffer[i + read_offset - 1] & 0x7F)) {
            // Error occurred
            rcvCrcError = TRUE;
        }
    }

    // If checkRepsonse is not set, a minimum wait time is needed before writing next command frame, to make sure next writing command is after response finished sending
    // the minimum wait time can be calculated by the formula below
    // minimum wait time = response length * 10 *1/baudrate
}

void FlexWriteBroadcast(
    unsigned int reg_addr, unsigned int *dataBytes, unsigned int length)
{
    unsigned int dev_addr = 0x00;

    dev_addr = (WRITE_MODE | BROADCAST | length);

    FlexReadWriteInternal(dev_addr, reg_addr, dataBytes, length, FALSE, FALSE);
}

void FlexWrite(unsigned int dev_addr_x, unsigned int reg_addr,
    unsigned int *dataBytes, unsigned int length, unsigned int checkResponse)
{
    unsigned int dev_addr = 0x00;

    dev_addr = (WRITE_MODE | length | dev_addr_x);

    FlexReadWriteInternal(
        dev_addr, reg_addr, dataBytes, length, checkResponse, FALSE);
}

void FlexRead(unsigned int dev_addr_x, unsigned int reg_addr,
    unsigned int length, unsigned int checkRcvCrc)
{
    unsigned int dev_addr = 0x00;
    unsigned int data[]   = {0x00};

    dev_addr = (READ_MODE | length | dev_addr_x);

    FlexReadWriteInternal(dev_addr, reg_addr, data, length, TRUE, checkRcvCrc);
}
