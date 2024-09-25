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
    /* Store ID to the element of C2U_FIFO */
#ifdef CAN_ID_EXTEND
    /* 29-bit extended identifier. */
    CAN_RX_Element->id = rxMsg->id;
#else
    /* Assuming package is using 11-bit standard ID.
    * When package uses standard id, ID is stored in ID[28:18] */
    CAN_RX_Element->id = ((rxMsg->id & (uint32_t) 0x1FFC0000) >> (uint32_t) 18);
#endif
    /* Store dlc to the element of C2U_FIFO */
    if(rxMsg->dlc<=8)
    {
        CAN_RX_Element->dlc = rxMsg->dlc;
    }
    else if(rxMsg->dlc <= 12)
    {
        CAN_RX_Element->dlc =(rxMsg->dlc)*4-24;
    }
    else if(rxMsg->dlc == 13)
    {
        CAN_RX_Element->dlc = 32;
    }
    else if(rxMsg->dlc == 14)
    {
        CAN_RX_Element->dlc = 48;
    }
    else if(rxMsg->dlc == 15)
    {
        CAN_RX_Element->dlc = 64;
    }
    /* Store data to the element of C2U_FIFO */
    for(uint16_t num = 0; num < (CAN_RX_Element->dlc); num++)
    {
        CAN_RX_Element->data[num] = rxMsg->data[num];
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
    uint32_t dlcTemp;

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

    /* Fill in ID area*/
#ifdef CAN_ID_EXTEND
    /* 29-bit extended identifier. */
    txMsg0->id = CAN_TX_Element->id;
#else
    /* 11-bit standard identifier. */
    txMsg0->id = (CAN_TX_Element->id) << 18U;
#endif

    /* Fill in dlc area*/
    if(CAN_TX_Element->dlc <= 8)
    {
        txMsg0->dlc = CAN_TX_Element->dlc;
        dlcTemp = 0;
    }
    else if(CAN_TX_Element->dlc <= 12)
    {
        txMsg0->dlc = 9;
        dlcTemp = 12-CAN_TX_Element->dlc;
    }
    else if(CAN_TX_Element->dlc <= 16)
    {
        txMsg0->dlc = 10;
        dlcTemp = 16-CAN_TX_Element->dlc;
    }
    else if(CAN_TX_Element->dlc <= 20)
    {
        txMsg0->dlc = 11;
        dlcTemp = 20-CAN_TX_Element->dlc;
    }
    else if(CAN_TX_Element->dlc <= 24)
    {
        txMsg0->dlc = 12;
        dlcTemp = 24-CAN_TX_Element->dlc;
    }
    else if(CAN_TX_Element->dlc <= 32)
    {
        txMsg0->dlc = 13;
        dlcTemp = 32-CAN_TX_Element->dlc;
    }
    else if(CAN_TX_Element->dlc <= 48)
    {
        txMsg0->dlc = 14;
        dlcTemp = 48-CAN_TX_Element->dlc;
    }
    else if(CAN_TX_Element->dlc <= 64)
    {
        txMsg0->dlc = 15;
        dlcTemp = 64-CAN_TX_Element->dlc;
    }
    /* Fill in data area */
    for(uint16_t num = 0; num < CAN_TX_Element->dlc; num++)
    {
        txMsg0->data[num] = CAN_TX_Element->data[num];
    }
    for(uint16_t num = 0; num < dlcTemp; num++)
    {
        txMsg0->data[num+(CAN_TX_Element->dlc)] = 0;
    }
    return true;
}

