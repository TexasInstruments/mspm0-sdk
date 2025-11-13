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
#ifndef BRIDGE_I2C_H_
#define BRIDGE_I2C_H_

#ifndef I2C_ID_LENGTH
/* Define for id length(0, 1, 4) */
/* The format of I2C:
     * if I2C_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...*/
#define I2C_ID_LENGTH  (4)
#endif

/* data length for I2C master receiving or I2C slave transmitting*/
#ifndef I2C_TRANSPARENT_LENGTH
#define I2C_TRANSPARENT_LENGTH  (8)
#endif

/* Variables for I2C receive */
/* default address for I2C master receiving */
#ifndef I2C_TARGET_ADDRESS
#define I2C_TARGET_ADDRESS (0x48)
#endif

#ifdef I2C_TRANSPARENT
#define I2C_RX_SIZE   (TRANSMIT_DATA_LENGTH)
#else
#define I2C_RX_SIZE   (TRANSMIT_DATA_LENGTH + 3 + I2C_ID_LENGTH)
#endif
extern Custom_Element gI2C_RX_Element;
extern uint8_t gI2CReceiveGroup[I2C_RX_SIZE];
extern uint16_t gGetI2cRxMsg_Count;
bool readI2CRxMsg_controller(uint16_t length, uint8_t address);
bool getI2CRxMsg_controller(uint8_t *I2CReceiveGroup);
bool getI2CRxMsg_target(uint8_t *I2CReceiveGroup);
bool processI2CRxMsg_controller(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element, uint8_t address);
bool processI2CRxMsg_target(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element);
bool getI2CRxMsg_controller_transparent(uint8_t *I2CReceiveGroup);
bool getI2CRxMsg_target_transparent(uint8_t *I2CReceiveGroup);
bool processI2CRxMsg_controller_transparent(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element, uint8_t address);
bool processI2CRxMsg_target_transparent(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element);

/* Variables for I2C Transmit */
#ifdef I2C_TRANSPARENT
#define I2C_TX_SIZE   (TRANSMIT_DATA_LENGTH)
#else
#define I2C_TX_SIZE   (TRANSMIT_DATA_LENGTH + 3 + I2C_ID_LENGTH)
#endif
extern Custom_Element gI2C_TX_Element;
extern uint8_t gI2CTransmitGroup[I2C_TX_SIZE];
extern bool gI2CTxflag;
extern uint32_t gTxLen, gTxCount;
bool sendI2CTxMsg_controller(uint8_t *I2CTransmitGroup, uint8_t address);
bool sendI2CTxMsg_target(uint8_t *I2CTransmitGroup);
uint8_t processI2CTxMsg_controller(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element);
bool processI2CTxMsg_target(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element);
uint8_t processI2CTxMsg_controller_transparent(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element);
bool processI2CTxMsg_target_transparent(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element);

#endif /* BRIDGE_I2C_H_ */
