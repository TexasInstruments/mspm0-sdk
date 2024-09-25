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

#include <stdint.h>
#ifndef EXPENDER_H_
#define EXPENDER_H_

#define UART
#define I2C
#define SPI

//*****************************************************************************
// Describe the packet length
//*****************************************************************************
#define TX_PACKET_LEN                 (4)
#define RX_PACKET_LEN                 (4)

//*****************************************************************************
// Describe the status
//*****************************************************************************
typedef enum
{
    eFalse = 0,
    eTrue = 1,
}tStatus;

//*****************************************************************************
// Describe the communication type
//*****************************************************************************
typedef enum {
    eNoSet,
    eUART=1,
    eSPI=2,
    eI2C=3,
}tCommType;

//*****************************************************************************
// Describe the packet type
//*****************************************************************************
typedef enum {
    eNone,
    eHeader=1,
    eCommd=2,
    eData=3,
    eCheck=4,
}tPrtclType;

//*****************************************************************************
// Describe the status
//*****************************************************************************
typedef enum
{
    eNoPkt,
    eGpioDir,
    eGpioOtp,
    eGpioInp,
}tPacketType;

//*****************************************************************************
// Describe the data type
//*****************************************************************************
typedef struct
{
    tPacketType pktTyp;
    tPrtclType prtclType;
    tCommType  commType;
    tStatus txPktStatus;
    uint8_t u8RTCheckSum;
    uint8_t rxDataPkt[RX_PACKET_LEN];
    uint8_t txDataPkt[TX_PACKET_LEN];
}tDataType;

//System init
void systemInit(void);

//GPIO control
void DL_GPIO_init(uint32_t pincmIndex);
uint8_t DL_GPIO_readPinStatus(uint32_t pins);

// communication part
void transmitPacket(uint8_t *buffer, uint32_t size,int mode);
void receivePacket(uint8_t u8Data);
uint8_t  checkSum(uint8_t* data, int len);
void uartTxData(uint8_t * u8Data,uint16_t Len);

// command part
void gpioDirectionSet(void);
void gpioOutputCtl(void);
void gpioStateRead(void);

#endif /* EXPENDER_H_ */
