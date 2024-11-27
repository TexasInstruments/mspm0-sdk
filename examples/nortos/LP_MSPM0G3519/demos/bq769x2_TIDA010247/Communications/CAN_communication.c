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

#include "CAN_communication.h"
#include "BQ769x2_Configs/BQ769x2_protocol.h"
/******************************************************************
 * Define:
 */
#define ID_MODE_STANDARD (0x0U)
#define ID_MODE_EXTENDED (0x1U)
#define CAN_DEBUG 1

/******************************************************************
 * Variables:
 */
volatile uint32_t gCANIntLine1Status = 0;
volatile bool gCANServiceInt         = false;
volatile bool CAN_Status             = true;

DL_MCAN_TxBufElement gCANTxMsg;
DL_MCAN_RxBufElement gCANRxMsg;
DL_MCAN_RxFIFOStatus gCANRxFS;

uint32_t gCANNodeId = CAN_ID_DEFAULT;

/******************************************************************
 * Functions:
 */
void CANTxMsgSendParamInitDefault(void)
{
    gCANTxMsg.rtr = 0U;
    gCANTxMsg.xtd = 0U;
    gCANTxMsg.esi = 0U;
    gCANTxMsg.brs = 1U;
    gCANTxMsg.fdf = 1U;
    gCANTxMsg.efc = 1U;
    gCANTxMsg.mm  = 0U;
}

const uint32_t CANDLC_Coding[16][2] = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 12}, {10, 16}, {11, 20}, {12, 24},
    {13, 32}, {14, 48}, {15, 64}};
//Using CANFD format, DLC_CODE = 1
#define DLC_CODE 1
void CANTxMsgSendPoll(uint32_t id, uint32_t dlc, uint32_t mm, uint8_t *data)
{
#if CAN_DEBUG
    //Enable CAN Power
    DL_MCAN_reset(MCAN0_INST);  //reset FIFO
    DL_MCAN_enablePower(MCAN0_INST);
    delay_cycles(1600);  //800 can't, 1600 can!
    //CAN Init
    SYSCFG_DL_MCAN0_init();

    gCANServiceInt     = false;
    gCANIntLine1Status = 0;

    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);  //Enable CAN interrupt
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;
#endif
    /* Initialize message to transmit. */
    gCANTxMsg.id  = id;
    gCANTxMsg.rtr = 0U;
    gCANTxMsg.xtd = 0U;
    gCANTxMsg.esi = 0U;
    gCANTxMsg.dlc = dlc;
    gCANTxMsg.brs = 1U;
    gCANTxMsg.fdf = 1U;
    gCANTxMsg.efc = 1U;
    gCANTxMsg.mm  = mm;
    for (uint32_t i = 0; i < CANDLC_Coding[dlc][DLC_CODE]; i++) {
        gCANTxMsg.data[i] = data[i];
    }

    /* Write Tx Message to the Message RAM. */
    DL_MCAN_writeMsgRam(MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0U, &gCANTxMsg);
    /* Add request for transmission. */
    DL_MCAN_TXBufAddReq(MCAN0_INST, 0U);

    //Waiting for CAN Tx done
    while (false == gCANServiceInt) {
        __WFE();
    }
}

//length: the length of data array(16bit).
//CRC: BQid + cmd + data (length*2 + 2)
void CAN_Write(
    uint32_t CANid, uint8_t BQid, uint8_t cmd, uint32_t length, uint16_t *data)
{
    DL_GPIO_setPins(WAKE_OUT_MCU_PORT, WAKE_OUT_MCU_PIN);

    uint8_t can_data[DL_MCAN_MAX_PAYLOAD_BYTES] = {0};
    uint32_t dlc                                = 0;

    if ((length * 2 + 3) > DL_MCAN_MAX_PAYLOAD_BYTES) {
        //max length is large than 64
        CAN_Status = false;
        __BKPT(0);  //Add error flag here
    }

    //Set CAN dlc according to length
    for (dlc = 0; dlc < 16; dlc++) {
        if ((length * 2 + 3) <= CANDLC_Coding[dlc][DLC_CODE]) break;
    }

    can_data[0] = BQid;
    can_data[1] = cmd;
    for (int i = 0; i < length; i++) {
        can_data[(i + 1) * 2]     = (data[i] & 0x00FF);
        can_data[(i + 1) * 2 + 1] = (data[i] & 0xFF00) >> 8;
    }
    can_data[(length + 1) * 2] = CRC8(can_data, length * 2 + 2);

    CANTxMsgSendPoll(CANid, dlc, 0xAA, can_data);

    DL_GPIO_clearPins(WAKE_OUT_MCU_PORT, WAKE_OUT_MCU_PIN);
}

void CANprocessCANRxMsg(void)
{
    uint32_t idMode;
    uint32_t id;

    if ((gCANIntLine1Status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK) {
        gCANRxFS.num = DL_MCAN_RX_FIFO_NUM_0;
        while ((gCANRxFS.fillLvl) == 0) {
            DL_MCAN_getRxFIFOStatus(MCAN0_INST, &gCANRxFS);
        }

        DL_MCAN_readMsgRam(
            MCAN0_INST, DL_MCAN_MEM_TYPE_FIFO, 0U, gCANRxFS.num, &gCANRxMsg);

        DL_MCAN_writeRxFIFOAck(MCAN0_INST, gCANRxFS.num, gCANRxFS.getIdx);

        idMode = gCANRxMsg.xtd;

        if (ID_MODE_EXTENDED == idMode) {
            id = gCANRxMsg.id;
        } else {
            /* Assuming package is using 11-bit standard ID.
             * When package uses standard id, ID is stored in ID[28:18]*/
            id = ((gCANRxMsg.id & (uint32_t) 0x1FFC0000) >> (uint32_t) 18);
        }

        switch (id) {
            case CAN_ID_DEFAULT:
                if (gCANRxMsg.data[0] == CAN_CMD_SETID) {
                    gCANNodeId = ((gCANRxMsg.data[1] & 0xFF) << 0) +
                                 ((gCANRxMsg.data[2] & 0x07) << 8);

                    if (gCANNodeId > CAN_ID_NODE16) {
                        //Error
                        __BKPT(0);
                    }

                    //Set Self-ID
                    DL_MCAN_StdMsgIDFilterElement gMCAN0StdFiltelem = {
                        .sfec  = 0x1,
                        .sft   = 0x2,
                        .sfid1 = gCANNodeId,
                        .sfid2 = CAN_IDMASK_DEFAULT,
                    };
                    /* Configure Standard ID filter element */
                    DL_MCAN_addStdMsgIDFilter(MCAN0_INST, 0U,
                        (DL_MCAN_StdMsgIDFilterElement *) &gMCAN0StdFiltelem);

                    //The last Node process
                    if (gCANNodeId == CAN_ID_NODE16) {
                        //The last CAN node process
                        break;
                    }
                }
                break;
            case CAN_ID_NODE01:

                break;
            default:
                /* Don't do anything */
                break;
        }

        gCANIntLine1Status &= ~(MCAN_IR_RF0N_MASK);
    }
}

//MCAN0 INTP, triggered by
void MCAN0_INST_IRQHandler(void)
{
    switch (DL_MCAN_getPendingInterrupt(MCAN0_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gCANIntLine1Status |= DL_MCAN_getIntrStatus(MCAN0_INST);
            DL_MCAN_clearIntrStatus(
                MCAN0_INST, gCANIntLine1Status, DL_MCAN_INTR_SRC_MCAN_LINE_1);

            gCANServiceInt = true;

            break;
        default:
            break;
    }
}

/******************************************************************
 * CAN1 Define:
 */

/******************************************************************
 * CAN1 Variables:
 */
volatile uint32_t gCAN1IntLine1Status = 0;
volatile bool gCAN1ServiceInt         = false;
volatile bool CAN1_Status             = true;

DL_MCAN_TxBufElement gCAN1TxMsg;
DL_MCAN_RxBufElement gCAN1RxMsg;
DL_MCAN_RxFIFOStatus gCAN1RxFS;

/******************************************************************
 * CAN1 Functions:
 */
void CAN1TxMsgSendParamInitDefault(void)
{
    gCAN1TxMsg.rtr = 0U;
    gCAN1TxMsg.xtd = 0U;
    gCAN1TxMsg.esi = 0U;
    gCAN1TxMsg.brs = 1U;
    gCAN1TxMsg.fdf = 1U;
    gCAN1TxMsg.efc = 1U;
    gCAN1TxMsg.mm  = 0U;
}

//Using CANFD format, DLC_CODE = 1
#define DLC_CODE 1
void CAN1TxMsgSendPoll(uint32_t id, uint32_t dlc, uint32_t mm, uint8_t *data)
{
#if CAN_DEBUG
    //Enable CAN Power
    DL_MCAN_reset(MCAN1_INST);  //reset FIFO
    DL_MCAN_enablePower(MCAN1_INST);
    delay_cycles(1600);  //800 can't, 1600 can!
    //CAN Init
    SYSCFG_DL_MCAN1_init();

    gCAN1ServiceInt     = false;
    gCAN1IntLine1Status = 0;

    NVIC_EnableIRQ(MCAN1_INST_INT_IRQN);  //Enable CAN interrupt
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN1_INST))
        ;
#endif
    /* Initialize message to transmit. */
    gCAN1TxMsg.id  = id;
    gCAN1TxMsg.rtr = 0U;
    gCAN1TxMsg.xtd = 0U;
    gCAN1TxMsg.esi = 0U;
    gCAN1TxMsg.dlc = dlc;
    gCAN1TxMsg.brs = 1U;
    gCAN1TxMsg.fdf = 1U;
    gCAN1TxMsg.efc = 1U;
    gCAN1TxMsg.mm  = mm;
    for (uint32_t i = 0; i < CANDLC_Coding[dlc][DLC_CODE]; i++) {
        gCAN1TxMsg.data[i] = data[i];
    }

    /* Write Tx Message to the Message RAM. */
    DL_MCAN_writeMsgRam(MCAN1_INST, DL_MCAN_MEM_TYPE_BUF, 0U, &gCAN1TxMsg);
    /* Add request for transmission. */
    DL_MCAN_TXBufAddReq(MCAN1_INST, 0U);

    //Waiting for CAN Tx done
    while (false == gCAN1ServiceInt) {
        __WFE();
    }
}

//length: the length of data array(16bit).
//CRC: BQid + cmd + data (length*2 + 2)
void CAN1_Write(
    uint32_t CANid, uint8_t BQid, uint8_t cmd, uint32_t length, uint16_t *data)
{
    uint8_t can_data[DL_MCAN_MAX_PAYLOAD_BYTES] = {0};
    uint32_t dlc                                = 0;

    if ((length * 2 + 3) > DL_MCAN_MAX_PAYLOAD_BYTES) {
        //max length is large than 64
        CAN1_Status = false;
        __BKPT(0);  //Add error flag here
    }

    //Set CAN dlc according to length
    for (dlc = 0; dlc < 16; dlc++) {
        if ((length * 2 + 3) <= CANDLC_Coding[dlc][DLC_CODE]) break;
    }

    can_data[0] = BQid;
    can_data[1] = cmd;
    for (int i = 0; i < length; i++) {
        can_data[(i + 1) * 2]     = (data[i] & 0x00FF);
        can_data[(i + 1) * 2 + 1] = (data[i] & 0xFF00) >> 8;
    }
    can_data[(length + 1) * 2] = CRC8(can_data, length * 2 + 2);

    CAN1TxMsgSendPoll(CANid, dlc, 0xAA, can_data);
}

void CAN1processCANRxMsg(void)
{
    uint32_t idMode;
    uint32_t id;

    if ((gCAN1IntLine1Status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK) {
        gCAN1RxFS.num = DL_MCAN_RX_FIFO_NUM_0;
        while ((gCAN1RxFS.fillLvl) == 0) {
            DL_MCAN_getRxFIFOStatus(MCAN1_INST, &gCAN1RxFS);
        }

        DL_MCAN_readMsgRam(
            MCAN1_INST, DL_MCAN_MEM_TYPE_FIFO, 0U, gCAN1RxFS.num, &gCAN1RxMsg);

        DL_MCAN_writeRxFIFOAck(MCAN1_INST, gCAN1RxFS.num, gCAN1RxFS.getIdx);

        idMode = gCAN1RxMsg.xtd;

        if (ID_MODE_EXTENDED == idMode) {
            id = gCAN1RxMsg.id;
        } else {
            /* Assuming package is using 11-bit standard ID.
             * When package uses standard id, ID is stored in ID[28:18]*/
            id = ((gCAN1RxMsg.id & (uint32_t) 0x1FFC0000) >> (uint32_t) 18);
        }

        switch (id) {
            case CAN_ID_DEFAULT:

                break;
            case CAN_ID_NODE01:

                break;
            default:
                /* Don't do anything */
                break;
        }

        gCAN1IntLine1Status &= ~(MCAN_IR_RF0N_MASK);
    }
}

//MCAN1 INTP, triggered by
void MCAN1_INST_IRQHandler(void)
{
    switch (DL_MCAN_getPendingInterrupt(MCAN1_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gCAN1IntLine1Status |= DL_MCAN_getIntrStatus(MCAN1_INST);

            gCAN1ServiceInt = true;

            /* New message received by Rx FIFO 0 (Filter matched) */
            if (gCAN1IntLine1Status & DL_MCAN_INTERRUPT_RF0N) {
                //You can set CAN Rx new message flag here
            }

            if (gCAN1IntLine1Status & DL_MCAN_INTERRUPT_TC) {
                //Tx done interrupt
                //Need to enable buffer interrupt when start send
                //                /* Enable the corresponding Tx Buffer's Interrupt */
                //                DL_MCAN_TXBufTransIntrEnable(MCAN1_INST, 0U, 1U);
            }

            DL_MCAN_clearIntrStatus(
                MCAN1_INST, gCAN1IntLine1Status, DL_MCAN_INTR_SRC_MCAN_LINE_1);
            break;
        default:
            break;
    }
}
