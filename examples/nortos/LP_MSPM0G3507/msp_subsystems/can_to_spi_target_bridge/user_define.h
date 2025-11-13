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
#ifndef USER_DEFINE_H_
#define USER_DEFINE_H_

#define SPI_TRANSPARENT
#ifdef SPI_TRANSPARENT
/* The format of SPI:
     * Transparent transmission - Data1 Data2 ...*/
/* data length for SPI master receiving or SPI slave transmitting*/
#define SPI_TRANSPARENT_LENGTH  (8)     //need be <= TRANSMIT_DATA_LENGTH
#define SPI_TIMEOUT     (0x4000)      //timeout 250ms
#else
/* The format of SPI:
     * if SPI_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...*/
#define SPI_ID_LENGTH  (4)
#endif

/* The format of CAN:
     * if CAN_ID_LENGTH = 4, format is ID1 ID2 ID3 ID4 Data1 Data2 ...
     * if CAN_ID_LENGTH = 1, format is ID Data1 Data2 ...
     * if CAN_ID_LENGTH = 0, format is Data1 Data2 ...*/
#define CAN_ID_LENGTH  (0)

/* User can test different config */
#define TRANSMIT_DATA_LENGTH (12)
#define C2S_FIFO_SIZE   (8)
#define S2C_FIFO_SIZE   (8)

/*user-defined information storage structure */
typedef struct {
    /*! Origin Identifier, indicating the origin of the message */
    uint32_t origin_id;
    /*! Destination Identifier, indicating the destination of the message */
    uint32_t destination_id;
    /*! Data Length Code */
    uint8_t dlc;
    /*! Data bytes */
    uint8_t data[TRANSMIT_DATA_LENGTH];
} Custom_Element;

typedef struct {
    uint16_t fifo_in;
    uint16_t fifo_out;
    uint16_t fifo_count;
    Custom_Element *fifo_pointer;
} Custom_FIFO;

#define DEFAULT_SPI_ORIGIN_ID (0x00)
#define DEFAULT_SPI_DESTINATION_ID (0x00)
#define DEFAULT_CAN_ORIGIN_ID (0x00)
#define DEFAULT_CAN_DESTINATION_ID (0x00)

#include "bridge_can.h"
#include "bridge_spi.h"

#endif /* USER_DEFINE_H_ */
