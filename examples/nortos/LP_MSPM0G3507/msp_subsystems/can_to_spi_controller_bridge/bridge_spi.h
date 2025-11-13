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
#ifndef BRIDGE_SPI_H_
#define BRIDGE_SPI_H_

#ifndef SPI_ID_LENGTH
/* Define for id length(0, 1, 4) */
/* The format of SPI:
     * if SPI_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...*/
//#define SPI_ID_LENGTH  (0)
#define SPI_ID_LENGTH  (1)
//#define SPI_ID_LENGTH  (4)
#endif

/* data length for SPI master receiving or SPI slave transmitting*/
#ifndef SPI_TRANSPARENT_LENGTH
#define SPI_TRANSPARENT_LENGTH  (8)
#endif

#ifndef SPI_TIMEOUT
#define SPI_TIMEOUT  (0x4000)      //timeout 250ms
#endif

/* Variables for SPI receive */
/* default char for SPI master receiving */
#define SPI_RX_DEFAULT_CHAR     (0xFF)

#ifdef SPI_TRANSPARENT
#define SPI_RX_SIZE   (TRANSMIT_DATA_LENGTH)
#else
#define SPI_RX_SIZE   (TRANSMIT_DATA_LENGTH + 3 + SPI_ID_LENGTH)
#endif
extern Custom_Element gSPI_RX_Element;
extern uint8_t gSpiReceiveGroup[SPI_RX_SIZE];
extern uint16_t gGetSpiRxMsg_Count;
extern bool gSpiRxflag, gSpiDirection;
bool getSpiRxMsg(SPI_Regs *spi, uint8_t *SpiReceiveGroup);
bool readSpiRxMsg(SPI_Regs *spi, uint16_t length);
bool processSpiRxMsg(uint8_t *SpiReceiveGroup, Custom_Element *SPI_RX_Element);
bool getSpiRxMsg_transparent(SPI_Regs *spi, uint8_t *SpiReceiveGroup);
bool processSpiRxMsg_transparent(uint8_t *SpiReceiveGroup, Custom_Element *SPI_RX_Element);

/* Variables for UART transmit */
#ifdef SPI_TRANSPARENT
#define SPI_TX_SIZE   (TRANSMIT_DATA_LENGTH)
#else
#define SPI_TX_SIZE   (TRANSMIT_DATA_LENGTH + 3 + SPI_ID_LENGTH)
#endif
extern Custom_Element gSPI_TX_Element;
extern uint8_t gSpiTransmitGroup[SPI_TX_SIZE];
extern bool gSpiTxflag;
bool sendSpiTxMsg(SPI_Regs *spi, uint8_t *SpiTransmitGroup, uint16_t length);
bool processSpiTxMsg(uint8_t *SpiTransmitGroup, Custom_Element *SPI_TX_Element);
bool processSpiTxMsg_transparent(uint8_t *SpiTransmitGroup, Custom_Element *SPI_TX_Element);

#endif /* BRIDGE_SPI_H_ */
