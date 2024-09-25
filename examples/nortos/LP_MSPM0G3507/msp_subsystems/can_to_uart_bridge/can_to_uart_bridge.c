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

/* Variables for U2C_FIFO
 * U2C_FIFO is used to temporarily store message from UART to CAN */
#define U2C_FIFO_SIZE   (10)
uint16_t U2C_in = 0;
uint16_t U2C_out = 0;
uint16_t U2C_count = 0;
Custom_Element U2C_FIFO[U2C_FIFO_SIZE];

/* Variables for C2U_FIFO
 * C2U_FIFO is used to temporarily store message from CAN to UART */
#define C2U_FIFO_SIZE   (10)
uint16_t C2U_in = 0;
uint16_t C2U_out = 0;
uint16_t C2U_count = 0;
Custom_Element C2U_FIFO[C2U_FIFO_SIZE];

int main(void)
{
    SYSCFG_DL_init();

    gInterruptLine1Status = 0;

    /* Enable Interrupts */
    NVIC_ClearPendingIRQ(CANFD0_INT_IRQn);
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(CANFD0_INT_IRQn);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CANFD0))
        ;

    while (1) {
        /* Send received CAN message to UART */
        if(C2U_count>0 && gUartTxflag == false)
        {
            /* Get gUART_TX_Element from C2U_FIFO
             * Update indicator variable of C2U_FIFO */
            gUART_TX_Element = C2U_FIFO[C2U_out];
            C2U_count--;
            C2U_out++;
            if(C2U_out >= C2U_FIFO_SIZE)C2U_out = 0;
            /* Convert message format */
            processUartTxMsg(gUartTransmitGroup, &gUART_TX_Element);
            /* Send messages via UART */
            gUartTxflag = true;
            sendUartTxMsg(gUartTransmitGroup, (7 + gUartTransmitGroup[6]));
        }

        /* Send received UART message to CAN */
        if(U2C_count>0)
        {
            /* Get gCAN_RX_Element from C2U_FIFO
             * Update indicator variable of U2C_FIFO */
            gCAN_TX_Element = U2C_FIFO[U2C_out];
            U2C_count--;
            U2C_out++;
            if(U2C_out >= U2C_FIFO_SIZE)U2C_out = 0;
            /* Convert message format */
            processCANTxMsg(&txMsg0, &gCAN_TX_Element);
            /* Send messages via CAN */
            sendCANTxMsg(&txMsg0);
        }
    }
}

void CANFD0_IRQHandler(void)
{
    bool processFlag;
    switch (DL_MCAN_getPendingInterrupt(CANFD0)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during TX/RX of CAN package */
            gInterruptLine1Status |= DL_MCAN_getIntrStatus(CANFD0);
            DL_MCAN_clearIntrStatus(
                CANFD0, gInterruptLine1Status, DL_MCAN_INTR_SRC_MCAN_LINE_1);
            processFlag = getCANRxMsg(&rxMsg);
            if( C2U_count >= C2U_FIFO_SIZE)
            {
                /* When the CAN bus is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                __BKPT(0);
            }
            if(processFlag == true)
            {
                /* Convert the received message to gCAN_RX_Element */
                processCANRxMsg(&rxMsg, &gCAN_RX_Element);
                /* Store gCAN_RX_Element into C2U_FIFO
                 * Update indicator variable of C2U_FIFO */
                C2U_FIFO[C2U_in] = gCAN_RX_Element;
                C2U_count++;
                C2U_in++;
                if(C2U_in >= C2U_FIFO_SIZE)C2U_in = 0;
            }
            break;
        default:
            break;
    }
}

void UART_0_INST_IRQHandler(void)
{
    bool processFlag;
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            /* Returns true when the UART group is fully received */
            processFlag = getUartRxMsg(gUartReceiveGroup);
            if( U2C_count >= U2C_FIFO_SIZE)
            {
                /* When the UART is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                __BKPT(0);
            }
            if(processFlag == true)
            {
                /* Convert the received message to gUART_RX_Element */
                processUartRxMsg(gUartReceiveGroup, &gUART_RX_Element);
                /* Store gUART_RX_Element into U2C_FIFO
                 * Update indicator variable of U2C_FIFO */
                U2C_FIFO[U2C_in] = gUART_RX_Element;
                U2C_count++;
                U2C_in++;
                if(U2C_in >= U2C_FIFO_SIZE)U2C_in = 0;
            }
            break;
        case DL_UART_MAIN_IIDX_TX:
            /* Put the transmit message into UART FIFO */
            if(gUartTxflag == true)
            {
                sendUartTxMsg(gUartTransmitGroup, (7 + gUartTransmitGroup[6]));
            }
            break;
        default:
            break;
    }
}
