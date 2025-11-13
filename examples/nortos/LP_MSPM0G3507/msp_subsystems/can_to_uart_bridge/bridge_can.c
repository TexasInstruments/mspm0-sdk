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
#include "ti_msp_dl_config.h"
#include "user_define.h"

volatile uint32_t gInterruptLine1Status;
DL_MCAN_RxBufElement rxMsg;
DL_MCAN_TxBufElement txMsg0;
Custom_Element gCAN_RX_Element;
Custom_Element gCAN_TX_Element;
bool getCANRxMsg(DL_MCAN_RxBufElement *rxMsg)
{
    static DL_MCAN_RxFIFOStatus rxFS;
    rxFS.fillLvl = 0;
    if ((gInterruptLine1Status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK) {
        rxFS.num = DL_MCAN_RX_FIFO_NUM_0;
        while ((rxFS.fillLvl) == 0) {
            DL_MCAN_getRxFIFOStatus(CANFD0, &rxFS);
        }
        DL_MCAN_readMsgRam(
            CANFD0, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, rxMsg);
        DL_MCAN_writeRxFIFOAck(CANFD0, rxFS.num, rxFS.getIdx);
        gInterruptLine1Status &= ~(MCAN_IR_RF0N_MASK);
        return true;
    }
    else
    {
        return false;
    }
}

bool processCANRxMsg(DL_MCAN_RxBufElement *rxMsg, Custom_Element *CAN_RX_Element)
{
    /* The format of CAN data packet is
     * if CAN_ID_LENGTH = 4, format is ID1 ID2 ID3 ID4 Data1 Data2 ...
     * if CAN_ID_LENGTH = 1, format is ID Data1 Data2 ...
     * if CAN_ID_LENGTH = 0, format is Data1 Data2 ...
     * */
    uint16_t count;
    uint32_t id;
    uint32_t dlc;

    /* Get origin id, indicating the origin of the message */
#ifdef CAN_ID_EXTEND
    /* 29-bit extended identifier. */
    CAN_RX_Element->origin_id = rxMsg->id;
#else
    /* Assuming package is using 11-bit standard ID.
    * When package uses standard id, ID is stored in ID[28:18] */
    CAN_RX_Element->origin_id = ((rxMsg->id & CAN_STANDARD_ID_MASK) >> (uint32_t) 18);
#endif
    /* Get Destination id, indicating the destination of the message */
    /* if CAN_ID_LENGTH = 4, id = data[0]<<24 + data[1]<<16 +
     * data[2]<<8 + data[3]
     * if CAN_ID_LENGTH = 1, id = data[0]
     * if CAN_ID_LENGTH = 0, id = default value;
     * */
    if(CAN_ID_LENGTH)
    {
        id = 0;
        for(count=0;count<CAN_ID_LENGTH;count++)
        {
            id += rxMsg->data[count]<<((CAN_ID_LENGTH-1-count)<<3);
        }
    }
    else
    {
        id = DEFAULT_CAN_DESTINATION_ID;
    }
    CAN_RX_Element->destination_id = id;

    /* Get data length */
    if(rxMsg->dlc<=8)
    {
        dlc = rxMsg->dlc;
    }
    else if(rxMsg->dlc <= 12)
    {
        dlc =(rxMsg->dlc)*4-24;
    }
    else if(rxMsg->dlc == 13)
    {
        dlc = 32;
    }
    else if(rxMsg->dlc == 14)
    {
        dlc = 48;
    }
    else if(rxMsg->dlc == 15)
    {
        dlc = 64;
    }

    CAN_RX_Element->dlc = dlc - CAN_ID_LENGTH;

    /* Get data */
    /* if CAN_ID_LENGTH = 4, data start from data[4]
     * if CAN_ID_LENGTH = 1, data start from data[1]
     * if CAN_ID_LENGTH = 0, data start from data[0]
     * */
    for(count = 0; count < (CAN_RX_Element->dlc); count++)
    {
        CAN_RX_Element->data[count] = rxMsg->data[count + CAN_ID_LENGTH];
    }
    return true;
}

bool sendCANTxMsg(DL_MCAN_TxBufElement *txMsg0)
{
    /* Write Tx Message to the Message RAM. */
    DL_MCAN_writeMsgRam(CANFD0, DL_MCAN_MEM_TYPE_BUF, 0, txMsg0);
    /* Add request for CAN transmission. */
    DL_MCAN_TXBufAddReq(CANFD0, 0);
    return true;
}
bool processCANTxMsg(DL_MCAN_TxBufElement *txMsg0, Custom_Element *CAN_TX_Element)
{
    /* The format of CAN data packet:
     * if CAN_ID_LENGTH = 4, format is ID1 ID2 ID3 ID4 Data1 Data2 ...
     * if CAN_ID_LENGTH = 1, format is ID Data1 Data2 ...
     * if CAN_ID_LENGTH = 0, format is Data1 Data2 ...
     * */
    uint32_t dlcTemp;
    uint16_t count;
    uint32_t dlc;

    /* Initialize txMsg0 to transmit. */
    /* Transmit data frame. */
    txMsg0->rtr = 0U;
#ifdef CAN_ID_EXTEND
    /* 29-bit extended identifier. */
    txMsg0->xtd = 1U;
#else
    /* 11-bit standard identifier. */
    txMsg0->xtd = 0U;
#endif
    /* ESI bit in CAN FD format depends only on error passive flag. */
    txMsg0->esi = 0U;
    /* CAN FD frames transmitted with bit rate switching. */
    txMsg0->brs = 1U;
    /* Frame transmitted in CAN FD format. */
    txMsg0->fdf = 1U;
    /* Store Tx events. */
    txMsg0->efc = 1U;
    /* Message Marker. */
    txMsg0->mm = 0xAAU;

    /* Fill in dlc area */
    dlc = CAN_TX_Element->dlc + CAN_ID_LENGTH;
    if(dlc <= 8)
    {
        txMsg0->dlc = dlc;
        dlcTemp = 0;
    }
    else if(dlc <= 12)
    {
        txMsg0->dlc = 9;
        dlcTemp = 12-dlc;
    }
    else if(dlc <= 16)
    {
        txMsg0->dlc = 10;
        dlcTemp = 16-dlc;
    }
    else if(dlc <= 20)
    {
        txMsg0->dlc = 11;
        dlcTemp = 20-dlc;
    }
    else if(dlc <= 24)
    {
        txMsg0->dlc = 12;
        dlcTemp = 24-dlc;
    }
    else if(dlc <= 32)
    {
        txMsg0->dlc = 13;
        dlcTemp = 32-dlc;
    }
    else if(dlc <= 48)
    {
        txMsg0->dlc = 14;
        dlcTemp = 48-dlc;
    }
    else if(dlc <= 64)
    {
        txMsg0->dlc = 15;
        dlcTemp = 64-dlc;
    }

    /* Set CAN id with Destination id*/
#ifdef CAN_ID_EXTEND
    /* 29-bit extended identifier. */
    txMsg0->id = CAN_TX_Element->destination_id;
#else
    /* 11-bit standard identifier. */
    txMsg0->id = (CAN_TX_Element->destination_id) << 18U;
#endif
    /* Fill in ID area */
    /* In this demo, ID area is filled with origin id to indicate the origin of message
     * Users can modify the ID area as needed */
    /* if CAN_ID_LENGTH = 4, id = data[0]<<24 + data[1]<<16 +
     * data[2]<<8 + data[3]
     * if CAN_ID_LENGTH = 1, id = data[0]
     * if CAN_ID_LENGTH = 0, id = 0;
     * */
    if(CAN_ID_LENGTH)
    {
        for(count=0;count<CAN_ID_LENGTH;count++)
        {
            txMsg0->data[count] = (uint8_t)((CAN_TX_Element->origin_id >> ((CAN_ID_LENGTH-1-count)<<3)) &0x000000ff);
        }
    }

    /* Fill in data area */
    /* if CAN_ID_LENGTH = 4, data start from data[4]
     * if CAN_ID_LENGTH = 1, data start from data[1]
     * if CAN_ID_LENGTH = 0, data start from data[0]
     * */
    for(count = 0; count < (CAN_TX_Element->dlc); count++)
    {
        txMsg0->data[CAN_ID_LENGTH + count] = CAN_TX_Element->data[count];
    }
    for(count = 0; count < (dlcTemp); count++)
    {
        txMsg0->data[CAN_ID_LENGTH + count + (CAN_TX_Element->dlc)] = 0;
    }
    return true;
}

