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
#ifndef EXPANDER_H_
#define EXPANDER_H_

//*****************************************************************************
// Describe the packet length
//*****************************************************************************
#define TX_PACKET_LEN                 (1)
#define RX_PACKET_LEN                 (2)

//*****************************************************************************
// Describe the status
//*****************************************************************************
typedef enum
{
    eFalse = 0,
    eTrue = 1,
}tStatus;

//*****************************************************************************
// Describe the packet type
//*****************************************************************************
typedef enum {
    eCommd,
    eData=1,
}tPrtclType;

//*****************************************************************************
// Describe the status
//*****************************************************************************
typedef enum
{
    eNoPkt,
    eGpioDir,
    eGpioOtp,
}tPacketType;

//*****************************************************************************
// Describe the data type
//*****************************************************************************
typedef struct
{
    tPacketType pktTyp;
    tPrtclType prtclType;
    uint8_t rxDataPkt[RX_PACKET_LEN];
    uint8_t txDataPkt[TX_PACKET_LEN];
}tDataType;

//System init
void systemInit(void);

//GPIO control
void DL_GPIO_init(uint32_t pincmIndex);
uint8_t DL_GPIO_readPinStatus(uint32_t pins);
void getPinStatus(void);

// communication part
void receivePacket(uint8_t u8Data);

// command part
void gpioDirectionSet(void);
void gpioOutputCtl(void);
void gpioStateRead(void);

#endif /* EXPANDER_H_ */
