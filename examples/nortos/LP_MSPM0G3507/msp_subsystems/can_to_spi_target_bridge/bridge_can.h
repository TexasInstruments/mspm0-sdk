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
#ifndef BRIDGE_CAN_H_
#define BRIDGE_CAN_H_

#ifndef CAN_ID_LENGTH
/* Define for id length(0, 1, 4) */
/* The format of CAN:
     * if CAN_ID_LENGTH = 4, format is ID1 ID2 ID3 ID4 Data1 Data2 ...
     * if CAN_ID_LENGTH = 1, format is ID Data1 Data2 ...
     * if CAN_ID_LENGTH = 0, format is Data1 Data2 ...*/
//#define CAN_ID_LENGTH  (0)
#define CAN_ID_LENGTH  (1)
//#define CAN_ID_LENGTH  (4)
#endif

/* Variables for CAN receive */
extern DL_MCAN_RxBufElement rxMsg;
extern Custom_Element gCAN_RX_Element;
extern volatile uint32_t gInterruptLine1Status;
bool getCANRxMsg(DL_MCAN_RxBufElement *rxMsg);
bool processCANRxMsg(DL_MCAN_RxBufElement *rxMsg, Custom_Element *gCAN_RX_Element);

/* Variables for CAN transit */
extern DL_MCAN_TxBufElement txMsg0;
extern Custom_Element gCAN_TX_Element;
bool sendCANTxMsg(DL_MCAN_TxBufElement *txMsg0);
bool processCANTxMsg(DL_MCAN_TxBufElement *txMsg0, Custom_Element *gCAN_TX_Element);

/* Parameters */
#define     CAN_STANDARD_ID_MASK    ((uint32_t)0x1FFC0000)

#endif /* BRIDGE_CAN_H_ */
