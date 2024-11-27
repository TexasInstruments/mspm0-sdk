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

#ifndef CAN_COMMUNICATION_H_
#define CAN_COMMUNICATION_H_

#include "ti_msp_dl_config.h"

/******************************************************************
 * Define:
 */
//Controller's ID
#define CAN_ID_CONTROLLER 0x0EC
//Nodes' ID
#define CAN_ID_DEFAULT 0x0FF
#define CAN_IDMASK_DEFAULT 0x7FF
#define CAN_ID_NODE01 0x001
#define CAN_ID_NODE02 0x002
#define CAN_ID_NODE03 0x003
#define CAN_ID_NODE04 0x004
#define CAN_ID_NODE05 0x005
#define CAN_ID_NODE06 0x006
#define CAN_ID_NODE07 0x007
#define CAN_ID_NODE08 0x008
#define CAN_ID_NODE09 0x009
#define CAN_ID_NODE10 0x00A
#define CAN_ID_NODE11 0x00B
#define CAN_ID_NODE12 0x00C
#define CAN_ID_NODE13 0x00D
#define CAN_ID_NODE14 0x00E
#define CAN_ID_NODE15 0x00F
#define CAN_ID_NODE16 0x010

//CAN command is 8 bit width in gCANRxMsg.data[0]
//CAN SET ID Command Format: 7D ID_L ID_H
#define CAN_CMD_SETID 0x7D
#define CAN_DLC_SETID 0x03
#define CAN_MM_SETID 0xAA  //TRM: 20.4.19.3, Ignore when not in use
//CAN SET ID Command Format: 7E MASK_L MASK_H
#define CAN_CMD_SETIDMASK 0x7E
#define CAN_DLC_SETIDMASK 0x03
/******************************************************************
 * Variables:
 */
extern volatile uint32_t gCANIntLine1Status;
extern volatile bool gCANServiceInt;
extern volatile bool gWAKEINMCU;

extern DL_MCAN_TxBufElement gCANTxMsg;
extern DL_MCAN_RxBufElement gCANRxMsg;
extern DL_MCAN_RxFIFOStatus gCANRxFS;

extern uint32_t gCANNodeId;

/******************************************************************
 * Functions:
 */
void CANprocessCANRxMsg(void);
void CANTxMsgSendParamInitDefault(void);
void CAN_Write(uint32_t CANid, uint8_t BQid, uint8_t cmd, uint32_t length,
    uint16_t *data);
void CAN1processCANRxMsg(void);
void CAN1TxMsgSendParamInitDefault(void);
void CAN1_Write(uint32_t CANid, uint8_t BQid, uint8_t cmd, uint32_t length,
    uint16_t *data);

#endif /* CAN_COMMUNICATION_H_ */
