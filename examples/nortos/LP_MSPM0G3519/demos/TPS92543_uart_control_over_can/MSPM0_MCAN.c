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

#include "MSPM0_MCAN.h"

/**
 * MCAN Application Layer function
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

    //    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);
    DL_SYSCTL_setPowerPolicySTANDBY0();
    __WFI();
    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

    DL_GPIO_initPeripheralInputFunction(
        GPIO_MCAN0_IOMUX_CAN_RX, GPIO_MCAN0_IOMUX_CAN_RX_FUNC);

    MCAN0_Restart();
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
        while (!(DL_MCAN_getIntrStatus(mcan) &
                 (DL_MCAN_INTERRUPT_RF0N | DL_MCAN_INTERRUPT_RF0F))) {
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
    DL_MCAN_clearIntrStatus(mcan,
        DL_MCAN_INTERRUPT_RF0N | DL_MCAN_INTERRUPT_RF0F,
        DL_MCAN_INTR_SRC_MCAN_LINE_1);

    return loop_i;
}

void MCAN_InterruptTransmissionControl(MCAN_Regs *mcan, bool enable)
{
    /* Enable/Disable mcan receive FIFO 0 and transmit complete interrupt */
    DL_MCAN_enableIntr(mcan, DL_MCAN_INTERRUPT_RF0N, enable);
    DL_MCAN_enableIntr(mcan, DL_MCAN_INTERRUPT_RF0F, enable);
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

        /* Stop sending when CAN0TxAmount messages have been sent. */
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
    DL_MCAN_enableIntr(MCAN0_INST, DL_MCAN_INTERRUPT_RF0F, 1U);

    return 0;
}

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
        DL_MCAN_enableIntr(MCAN0_INST, DL_MCAN_INTERRUPT_RF0F, 0U);

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
        case DL_MCAN_IIDX_LINE0:
            break;
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gInterruptLine1Status = DL_MCAN_getIntrStatus(MCAN0_INST);

            /* New message received by Rx FIFO 0 (Filter matched) */
            if ((gInterruptLine1Status & DL_MCAN_INTERRUPT_RF0N) ||
                (gInterruptLine1Status & DL_MCAN_INTERRUPT_RF0F)) {
                if (MCAN_ReceiveInterruptService(MCAN0_INST) == 1) {
                    CANRxDone = 1;
                }
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
