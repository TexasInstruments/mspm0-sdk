/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include "mspm0_mcan.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"

/******************************************************************
 * MSPM0 MCAN Function
 */

/**
 * @brief MCAN Auto address
 *
 * @param mcan MCAN INST
 * @return uint32_t MCAN ID of this CAN node
 */
uint32_t mcan_autoAddress(MCAN_Regs *mcan)
{
    DL_MCAN_StdMsgIDFilterElement gMCAN0StdFltWr = {0};
    DL_MCAN_RxBufElement rxMsg                   = {0};
    DL_MCAN_TxBufElement txMsg                   = {0};

    // Enable BMU WAKE IN pin (MCU_WAKEIN)
    DL_GPIO_setPins(GPIOB, BMU_WAKEIN_PIN);

    // Set default ID
    gMCAN0StdFltWr.sfec  = 0x1;
    gMCAN0StdFltWr.sft   = 0x2;
    gMCAN0StdFltWr.sfid1 = CAN_ID_DEFAULT;
    gMCAN0StdFltWr.sfid2 = CAN_IDMASK_DEFAULT;
    /* Configure Standard ID filter element */
    DL_MCAN_addStdMsgIDFilter(mcan, 0, &gMCAN0StdFltWr);

    // Enable MCAN receive interrupt and length is 0xFF
    MCAN_ReceiveInterruptStart(mcan, &rxMsg, 0xFF);
    // Waiting for New CAN message and check the ID
    while (CANRxDone == 0)
        ;
    CANRxDone = 0;

    // Set MCAN ID
    if (((rxMsg.id >> 18) ==
            CAN_ID_DEFAULT) &&  //1= rxMsg.id == CAN_ID_DEFAULT//(rxMsg.id>>18) == CAN_ID_DEFAULT
        (rxMsg.data[0] == CAN_CMD_SETID)) {
        gMCAN0StdFltWr.sfid1 =
            ((rxMsg.data[1] & 0xFF) << 0) + ((rxMsg.data[2] & 0x07) << 8);
        if (gMCAN0StdFltWr.sfid1 > CAN_ID_NODE16) {
            // Error, CAN Node is too long
            DL_GPIO_setPins(GPIOB, LED_Red_PIN);
            return -1;
        } else {
            // Set MCAN Standard ID
            DL_MCAN_addStdMsgIDFilter(mcan, 0, &gMCAN0StdFltWr);
        }
    } else {
        // CAN message invalid ID and set ID CMD error
        DL_GPIO_setPins(GPIOB, LED_Red_PIN);
        return -1;
    }

    // Enable next node power enable pin
    DL_GPIO_setPins(GPIOB, BMU_WAKEOUT_PIN);
    delay_cycles(8000000);  //100ms, cycle. 1/80Mhz

    // Send the set ID command message to next CAN node
    MCAN_TxBufElementInit(&txMsg, 1);
    txMsg.id      = CAN_ID_DEFAULT << 18U;
    txMsg.dlc     = CAN_DLC_SETID;
    txMsg.data[0] = CAN_CMD_SETID;
    txMsg.data[1] = (uint8_t)((gMCAN0StdFltWr.sfid1 + 1) & 0xFF);
    txMsg.data[2] = (uint8_t)(((gMCAN0StdFltWr.sfid1 + 1) >> 8) & 0x07);
    MCAN_SendPoll(mcan, &txMsg, 1);

    return gMCAN0StdFltWr.sfid1;
}

/******************************************************************
 * MSPM0 MCAN Test Example Code
 */

void mspm0_mcan_test_example(void)
{
/**
    * When DL_MCAN_StdMsgIDFilterElement's sft = 2 or
    *      DL_MCAN_ExtMsgIDFilterElement's eft = 2.
    * We are using SFID1/EFID1 as filter and SFID2/EFID2 as mask.
    * All mask bit are 1: only ID == filter can be received and put into RxFIFO0.
    * All mask bit are 0: All message IDs match.
    *
    * Up to 128 STD ID filter and 64 EXT ID filter is supported.
    */
#define CANSTDFLTNUM (MCAN0_INST_MCAN_STD_ID_FILTER_NUM)
#define CANEXTFLTNUM (MCAN0_INST_MCAN_EXT_ID_FILTER_NUM)

    DL_MCAN_StdMsgIDFilterElement gMCAN0StdFltWr[CANSTDFLTNUM] = {0};
    DL_MCAN_StdMsgIDFilterElement gMCAN0StdFltRd[CANSTDFLTNUM] = {0};
    DL_MCAN_ExtMsgIDFilterElement gMCAN0ExtFltWr[CANEXTFLTNUM] = {0};
    DL_MCAN_ExtMsgIDFilterElement gMCAN0ExtFltRd[CANEXTFLTNUM] = {0};

    /**
    * CAN Tx Buffer Operation Variables
    */
    DL_MCAN_TxBufElement txMsg0[MCAN0_INST_MCAN_TX_BUFF_SIZE * 2];

    /**
    * CAN Rx FIFO/Buffer Operation Variables
    */
    DL_MCAN_RxBufElement rxMsg0[MCAN0_INST_MCAN_FIFO_0_NUM * 2];

    /* Initialize txMsg0 to transmit. */
    for (int j = 0; j < MCAN0_INST_MCAN_TX_BUFF_SIZE * 2; j++) {
        /* Identifier Value, need to <<18U to fit into register. */
        txMsg0[j].id = 0x12 << 18U;
        /* Transmit data frame. */
        txMsg0[j].rtr = 0U;
        /* 11-bit standard identifier. */
        txMsg0[j].xtd = 0U;
        /* ESI bit in CAN FD format depends only on error passive flag. */
        txMsg0[j].esi = 0U;
        /* Transmitting 8 bytes. */
        txMsg0[j].dlc = 8U;
        /* CAN FD frames transmitted with bit rate switching. */
        txMsg0[j].brs = 0U;
        /* Frame transmitted in CAN FD format. */
        txMsg0[j].fdf = 0U;
        /* Store Tx events. */
        txMsg0[j].efc = 0U;
        /* Message Marker. */
        txMsg0[j].mm = 0xAAU;
        /* Data bytes. */
        txMsg0[j].data[0] = j;
        for (int i = 1; i < txMsg0[j].dlc; i++) {
            txMsg0[j].data[i] = i;
        }
    }

    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    /******************************************************************
     * CAN Filter Write Test
     */
    /* Filter initialization and write */
    for (int i = 0; i < CANSTDFLTNUM; i++) {
        gMCAN0StdFltWr[i].sfec  = 0x1;
        gMCAN0StdFltWr[i].sft   = 0x2;
        gMCAN0StdFltWr[i].sfid1 = i + 1;
        gMCAN0StdFltWr[i].sfid2 = 0x7FF;
        /* Configure Standard ID filter element */
        DL_MCAN_addStdMsgIDFilter(MCAN0_INST, i,
            (DL_MCAN_StdMsgIDFilterElement *) &gMCAN0StdFltWr[i]);
    }
    for (int i = 0; i < CANEXTFLTNUM; i++) {
        gMCAN0ExtFltWr[i].efec  = 0x1;
        gMCAN0ExtFltWr[i].eft   = 0x2;
        gMCAN0ExtFltWr[i].efid1 = i + 1;
        gMCAN0ExtFltWr[i].efid2 = 0x1FFFFFFF;
        /* Configure Extended ID filter element*/
        DL_MCAN_addExtMsgIDFilter(MCAN0_INST, i,
            (DL_MCAN_ExtMsgIDFilterElement *) &gMCAN0ExtFltWr[i]);
    }

    /******************************************************************
     * CAN Filter Read Test
     */
    for (int i = 0; i < CANSTDFLTNUM; i++) {
        DL_MCAN_readStdMsgIDFilter(MCAN0_INST, i,
            (DL_MCAN_StdMsgIDFilterElement *) &gMCAN0StdFltRd[i]);
    }
    for (int i = 0; i < CANEXTFLTNUM; i++) {
        DL_MCAN_readExtMsgIDFilter(MCAN0_INST, i,
            (DL_MCAN_ExtMsgIDFilterElement *) &gMCAN0ExtFltRd[i]);
    }

    /******************************************************************
     * CAN Poll Send Test
     */
    MCAN_SendPoll(MCAN0_INST, &txMsg0[0], MCAN0_INST_MCAN_TX_BUFF_SIZE * 2);

    /******************************************************************
     * CAN Poll Receive Test
     */
    MCAN_ReceivePoll(MCAN0_INST, &rxMsg0[0], MCAN0_INST_MCAN_FIFO_0_NUM * 2);

    /******************************************************************
     * CAN Interrupt Send Test
     *
     * Send one package first and the send other package in interrupt
     */
    MCAN_SendInterruptStart(
        MCAN0_INST, &txMsg0[0], MCAN0_INST_MCAN_TX_BUFF_SIZE * 2);

    /* Waiting for CAN Tx message interrupt send finish. */
    while (!CANTxDone) {
        __asm("NOP");
    }

    /******************************************************************
     * CAN Interrupt Receive Test
     */
    MCAN_ReceiveInterruptStart(
        MCAN0_INST, &rxMsg0[0], MCAN0_INST_MCAN_FIFO_0_NUM * 2);

    /* Waiting for CAN Rx message interrupt receive finish. */
    while (!CANRxDone) {
        __asm("NOP");
    }
}

/******************************************************************
 * MCAN Driver Layer Function
 */
/**
 * MCAN restart need to call SYSCFG_DL_MCAN0_init()
 * For MCAN1 restart function, please add manually
 */
void MCAN0_Restart(void)
{
    DL_MCAN_reset(CANFD0);
    delay_cycles(16);
    DL_MCAN_disablePower(CANFD0);
    delay_cycles(32);
    DL_MCAN_enablePower(CANFD0);
    // MCAN RAM need at least 50us to finish init
    // 1600 CPU cycles@CPU32MHz
    // 4000 CPU cycles@CPU80MHz
    delay_cycles(4000);
    SYSCFG_DL_MCAN0_init();
}

/**
 * Demo for enter and exit STANBY0 mode steps while using MCAN
 * In Standby0 mode, switch MCAN Rx to external interrupt GPIO
 */
void MCAN_LowPowerMode(void)
{
    DL_GPIO_initDigitalInputFeatures(GPIO_MCAN0_IOMUX_CAN_RX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_setLowerPinsPolarity(
        GPIO_MCAN0_CAN_RX_PORT, DL_GPIO_PIN_13_EDGE_FALL);
    DL_GPIO_clearInterruptStatus(
        GPIO_MCAN0_CAN_RX_PORT, GPIO_MCAN0_CAN_RX_PIN);
    DL_GPIO_enableInterrupt(GPIO_MCAN0_CAN_RX_PORT, GPIO_MCAN0_CAN_RX_PIN);

    //    NVIC_EnableIRQ(GPIOA_INT_IRQn);
    DL_SYSCTL_setPowerPolicySTANDBY0();
    __WFI();
    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

    DL_GPIO_initPeripheralInputFunction(
        GPIO_MCAN0_IOMUX_CAN_RX, GPIO_MCAN0_IOMUX_CAN_RX_FUNC);

    MCAN0_Restart();
}

void MCAN_TxBufElementInit(DL_MCAN_TxBufElement *txMsg, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        txMsg[i].rtr = 0U;
        txMsg[i].xtd = 0U;
        txMsg[i].esi = 0U;
        txMsg[i].dlc = 8U;
        txMsg[i].brs = 0U;
        txMsg[i].fdf = 0U;
        txMsg[i].efc = 0U;
        txMsg[i].mm  = 0xAAU;
    }
}
/**
 * When DL_MCAN_StdMsgIDFilterElement's sft = 2 or
 *      DL_MCAN_ExtMsgIDFilterElement's eft = 2.
 * We are using SFID1/EFID1 as filter and SFID2/EFID2 as mask.
 * All mask bit are 1: only ID == filter can be received and put into RxFIFO0.
 * All mask bit are 0: All message IDs match, ignore SFID1/EFID1.
 *
 * Up to 128 STD ID filter and 64 EXT ID filter is supported.
 */
void DL_MCAN_readStdMsgIDFilter(
    MCAN_Regs *mcan, uint32_t filtNum, DL_MCAN_StdMsgIDFilterElement *elem)
{
    uint32_t startAddr, elemAddr, regVal;

    //startAddr = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_SIDFC, MCAN_SIDFC_FLSSA);
    startAddr = *(volatile uint32_t *) (&mcan->MCANSS.MCAN.MCAN_SIDFC);
    startAddr = (startAddr & 0x0000FFFC) >> 2;
    startAddr = (uint32_t)(startAddr << 2U);
    elemAddr  = startAddr + (filtNum * 1U * 4U);
    elemAddr += 0x0U;

    //regVal = HW_RD_REG32(((uint32_t) mcan + (uint32_t) elemAddr));
    regVal = *(volatile uint32_t *) ((uint32_t) mcan + (uint32_t) elemAddr);
    elem->sfid2 = (regVal >> 0U) & 0x000007FF;
    elem->sfid1 = (regVal >> 16U) & 0x000007FF;
    elem->sfec  = (regVal >> 27U) & 0x00000007;
    elem->sft   = (regVal >> 30U) & 0x00000003;
}

void DL_MCAN_readExtMsgIDFilter(
    MCAN_Regs *mcan, uint32_t filtNum, DL_MCAN_ExtMsgIDFilterElement *elem)
{
    uint32_t startAddr, elemAddr, regVal;

    //startAddr = HW_RD_FIELD32(&mcan->MCANSS.MCAN.MCAN_XIDFC, MCAN_XIDFC_FLESA);
    startAddr = *(volatile uint32_t *) (&mcan->MCANSS.MCAN.MCAN_XIDFC);
    startAddr = (startAddr & 0x0000FFFC) >> 2;
    startAddr = (uint32_t)(startAddr << 2U);
    elemAddr  = startAddr + (filtNum * 2U * 4U);
    elemAddr += 0x0U;

    //regVal = HW_RD_REG32(((uint32_t) mcan + (uint32_t) elemAddr));
    regVal = *(volatile uint32_t *) ((uint32_t) mcan + (uint32_t) elemAddr);
    elem->efid1 = (regVal >> 0U) & 0x1FFFFFFF;
    elem->efec  = (regVal >> 29U) & 0x00000007;

    elemAddr += 4U;
    //regVal = HW_RD_REG32(((uint32_t) mcan + (uint32_t) elemAddr));
    regVal = *(volatile uint32_t *) ((uint32_t) mcan + (uint32_t) elemAddr);
    elem->efid2 = (regVal >> 0U) & 0x1FFFFFFF;
    elem->eft   = (regVal >> 30U) & 0x00000003;
}

uint8_t CAN0TxCount             = 0;
uint8_t CAN0TxAmount            = 0;
DL_MCAN_TxBufElement *CAN0TxMsg = NULL;

uint8_t CAN0RxCount             = 0;
uint8_t CAN0RxAmount            = 0;
DL_MCAN_RxBufElement *CAN0RxMsg = NULL;
DL_MCAN_RxFIFOStatus rxFS;

uint8_t MCAN_SendPoll(
    MCAN_Regs *mcan, DL_MCAN_TxBufElement *txMsg, uint8_t length)
{
    uint8_t CANTxBufferPutIndex        = 0;
    uint32_t CANTxBufferPendingRequest = 0;
    uint8_t loop_i                     = 0;

    for (loop_i = 0; loop_i < length; loop_i++) {
        /* Find the empty Tx buffer to start transfer. */
        CANTxBufferPutIndex       = 0;
        CANTxBufferPendingRequest = DL_MCAN_getTxBufReqPend(mcan);
        while (CANTxBufferPendingRequest & (1 << CANTxBufferPutIndex)) {
            CANTxBufferPutIndex++;
            if (CANTxBufferPutIndex >= MCAN0_INST_MCAN_TX_BUFF_SIZE) {
                delay_cycles(
                    32000);  //Delay 1ms waiting for Tx finish, CPU @ 32MHz
                CANTxBufferPutIndex       = 0;
                CANTxBufferPendingRequest = DL_MCAN_getTxBufReqPend(mcan);
            }
        }

        /* Write Tx Message to the Message RAM. */
        DL_MCAN_writeMsgRam(
            mcan, DL_MCAN_MEM_TYPE_BUF, CANTxBufferPutIndex, &txMsg[loop_i]);
        /* Add request for transmission. */
        DL_MCAN_TXBufAddReq(mcan, CANTxBufferPutIndex);
    }

    // Wait until all Transmission Occurred is Done
    while (DL_MCAN_getTxBufReqPend(mcan) != 0x00)
        ;

    return loop_i;
}

uint8_t MCAN_ReceivePoll(
    MCAN_Regs *mcan, DL_MCAN_RxBufElement *rxMsg, uint8_t length)
{
    uint8_t loop_i   = 0;
    uint32_t timeout = CPUCLK_FREQ;

    DL_MCAN_RxFIFOStatus rxFS;

    for (loop_i = 0; loop_i < length; loop_i++) {
        rxFS.num     = DL_MCAN_RX_FIFO_NUM_0;
        rxFS.fillLvl = 0;

        /* Waiting for New CAN Message Stored into FIFO 0 */
        while (!(DL_MCAN_getIntrStatus(mcan) & DL_MCAN_INTERRUPT_RF0N)) {
            if ((--timeout) == 0) {
                return loop_i;
            }
        }

        /* Waiting for FIFO 0 Fill Level Not 0 */
        while ((rxFS.fillLvl) == 0) {
            DL_MCAN_getRxFIFOStatus(mcan, &rxFS);
        }
        DL_MCAN_readMsgRam(
            mcan, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, &rxMsg[loop_i]);
        DL_MCAN_writeRxFIFOAck(mcan, rxFS.num, rxFS.getIdx);
    }

    /* Clear Receive FIFO 0 New Message Interrupt Flag */
    DL_MCAN_clearIntrStatus(
        mcan, DL_MCAN_INTERRUPT_RF0N, DL_MCAN_INTR_SRC_MCAN_LINE_1);

    return loop_i;
}

void MCAN_InterruptTransmissionControl(MCAN_Regs *mcan, bool enable)
{
    /* Enable/Disable mcan receive FIFO 0 and transmit complete interrupt */
    DL_MCAN_enableIntr(mcan, DL_MCAN_INTERRUPT_RF0N, enable);
    DL_MCAN_enableIntr(mcan, DL_MCAN_INTERRUPT_TC, enable);
}

void MCAN_SendInterruptStart(
    MCAN_Regs *mcan, DL_MCAN_TxBufElement *txMsg, uint8_t length)
{
    uint8_t CANTxBufferPutIndex        = 0;
    uint32_t CANTxBufferPendingRequest = 0;

    /* Save information to internal global variables */
    CAN0TxCount  = 0;
    CAN0TxAmount = length;
    CAN0TxMsg    = txMsg;

    /* Enable MCAN Transmission Completed interrupt. */
    DL_MCAN_enableIntr(mcan, DL_MCAN_INTERRUPT_TC, 1U);

    /* Find the empty Tx buffer to start transfer. */
    CANTxBufferPutIndex       = 0;
    CANTxBufferPendingRequest = DL_MCAN_getTxBufReqPend(mcan);
    while (CANTxBufferPendingRequest & (1 << CANTxBufferPutIndex)) {
        CANTxBufferPutIndex++;
        if (CANTxBufferPutIndex >= MCAN0_INST_MCAN_TX_BUFF_SIZE) {
            delay_cycles(32);  //Delay 1ms waiting for Tx finish
            CANTxBufferPutIndex       = 0;
            CANTxBufferPendingRequest = DL_MCAN_getTxBufReqPend(mcan);
        }
    }

    /* Enable the corresponding Tx Buffer's Interrupt */
    DL_MCAN_TXBufTransIntrEnable(mcan, CANTxBufferPutIndex, 1U);

    /* Write Tx Message to the Message RAM. */
    DL_MCAN_writeMsgRam(
        mcan, DL_MCAN_MEM_TYPE_BUF, CANTxBufferPutIndex, &txMsg[0]);
    /* Add request for transmission. */
    DL_MCAN_TXBufAddReq(mcan, CANTxBufferPutIndex);

    CAN0TxCount++;
}

uint8_t MCAN_SendInterruptService(MCAN_Regs *mcan)
{
    static uint8_t CANTxBufferPutIndex          = 0;
    static uint8_t NoCANTxBufferInterruptEnable = 0;
    static uint32_t CANTxBufferIE               = 0;

    CANTxBufferPutIndex          = 0;
    CANTxBufferIE                = mcan->MCANSS.MCAN.MCAN_TXBTIE;
    NoCANTxBufferInterruptEnable = 0;
    while (!(CANTxBufferIE & (1 << CANTxBufferPutIndex))) {
        CANTxBufferPutIndex++;

        if (CANTxBufferPutIndex == 31) {
            NoCANTxBufferInterruptEnable = 1;
            break;
        }
    }

    if (NoCANTxBufferInterruptEnable == 0) {
        /* Write Tx Message to the Message RAM. */
        DL_MCAN_writeMsgRam(mcan, DL_MCAN_MEM_TYPE_BUF, CANTxBufferPutIndex,
            &CAN0TxMsg[CAN0TxCount]);
        /* Add request for transmission. */
        DL_MCAN_TXBufAddReq(mcan, CANTxBufferPutIndex);

        CAN0TxCount++;

        /* Stop sending when 0xFF messages have been sent. */
        if (CAN0TxCount == CAN0TxAmount) {
            /* Disable the corresponding Tx Buffer's Interrupt */
            DL_MCAN_TXBufTransIntrEnable(mcan, CANTxBufferPutIndex, 0U);
            /* Disable MCAN Transmission Completed interrupt. */
            DL_MCAN_enableIntr(mcan, DL_MCAN_INTERRUPT_TC, 0U);
            return 1;  // Transmission complete
        }
    } else {
        return -1;  // No buffer is in using
    }

    return 0;  // Continuous transmission
}

/**
 * length: 1 ~ 0xfe, set the number that want to receive.
 * length: 0xff, infinity receive number
 *         M0 will set CANRxDone every single CAN message.
*/
uint8_t MCAN_ReceiveInterruptStart(
    MCAN_Regs *mcan, DL_MCAN_RxBufElement *rxMsg, uint8_t length)
{
    CAN0RxCount  = 0;
    CAN0RxAmount = length;
    CAN0RxMsg    = rxMsg;

    rxFS.num     = DL_MCAN_RX_FIFO_NUM_0;
    rxFS.fillLvl = 0;

    /* Enable MCAN receive FIFO 0 new message interrupt. */
    DL_MCAN_enableIntr(MCAN0_INST, DL_MCAN_INTERRUPT_RF0N, 1U);

    return 0;
}

// Return 0: didn't receive MCAN Message
// Return 1: CAM message received
uint8_t MCAN_ReceiveInterruptService(MCAN_Regs *mcan)
{
    uint32_t timeoutCount = 100;
    do {
        DL_MCAN_getRxFIFOStatus(mcan, &rxFS);
        if ((timeoutCount--) == 0) {
            return 0;
        }
    } while ((rxFS.fillLvl) == 0);

    DL_MCAN_readMsgRam(
        mcan, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, &CAN0RxMsg[CAN0RxCount]);
    DL_MCAN_writeRxFIFOAck(mcan, rxFS.num, rxFS.getIdx);

    CAN0RxCount++;
    // -1 is infinity receive
    if (255 == CAN0RxAmount) {
        CAN0RxCount = 0;
        return 1;
    }

    if (CAN0RxCount == CAN0RxAmount) {
        /* Disable MCAN receive FIFO 0 new message interrupt. */
        DL_MCAN_enableIntr(MCAN0_INST, DL_MCAN_INTERRUPT_RF0N, 0U);

        return 1;
    }

    return 0;
}
/**
 * CAN protocol status
 *      Bus off
 */
DL_MCAN_ProtocolStatus gProtStatus;
volatile uint8_t CANBusOff = 0;

/**
 * CAN Interrupt
 */
volatile uint32_t gInterruptLine1Status;

volatile uint8_t CANRxDone = 0;
volatile uint8_t CANTxDone = 0;

void MCAN0_INST_IRQHandler(void)
{
    switch (DL_MCAN_getPendingInterrupt(MCAN0_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gInterruptLine1Status = DL_MCAN_getIntrStatus(MCAN0_INST);

            /* New message received by Rx FIFO 0 (Filter matched) */
            /* If RF0F is triggered, RF0N won't br triggered */
            if (gInterruptLine1Status & DL_MCAN_INTERRUPT_RF0N) {
                DL_GPIO_setPins(GPIOB, LED_GREEN_PIN);
                if (MCAN_ReceiveInterruptService(MCAN0_INST) == 1) {
                    CANRxDone = 1;
                }
                DL_GPIO_clearPins(GPIOB, LED_GREEN_PIN);
            }

            /* Transmission Complete from one interrupt enabled buffer */
            if (gInterruptLine1Status & DL_MCAN_INTERRUPT_TC) {
                if (MCAN_SendInterruptService(MCAN0_INST) == 1) {
                    CANTxDone = 1;
                }
            }

            /* MCAN bus off status changed */
            if (gInterruptLine1Status & DL_MCAN_INTERRUPT_BO) {
                DL_MCAN_getProtocolStatus(MCAN0_INST, &gProtStatus);
                if (gProtStatus.busOffStatus == 1) {
                    CANBusOff = 1;
                } else {
                    CANBusOff = 0;
                }
            }

            /* Clear all MCAN interrupt status */
            DL_MCAN_clearIntrStatus(MCAN0_INST, gInterruptLine1Status,
                DL_MCAN_INTR_SRC_MCAN_LINE_1);
            break;
        default:
            break;
    }
}
