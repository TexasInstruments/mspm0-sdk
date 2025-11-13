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

/* Variables for C2U_FIFO
 * C2U_FIFO is used to temporarily store message from CAN to UART */
Custom_Element gC2U_FIFO[C2U_FIFO_SIZE];
Custom_FIFO gCan2Uart_FIFO = {0, 0, 0, gC2U_FIFO};

/* Variables for U2C_FIFO
 * U2C_FIFO is used to temporarily store message from UART to CAN */
Custom_Element gU2C_FIFO[U2C_FIFO_SIZE];
Custom_FIFO gUart2Can_FIFO = {0, 0, 0, gU2C_FIFO};

int main(void)
{
    SYSCFG_DL_init();

    gInterruptLine1Status = 0;

    /* Enable Interrupts */
    NVIC_ClearPendingIRQ(CANFD0_INT_IRQn);
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(TIMER_UART_INST_INT_IRQN);
    NVIC_EnableIRQ(CANFD0_INT_IRQn);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_UART_INST_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CANFD0))
        ;

    while (1) {
        /* Send received CAN message to UART */
        if(gCan2Uart_FIFO.fifo_count>0 && gUartTxflag == false)
        {
            /* Get gUART_TX_Element from C2U_FIFO
             * Update indicator variable of C2U_FIFO */
            gUART_TX_Element = gCan2Uart_FIFO.fifo_pointer[gCan2Uart_FIFO.fifo_out];
            gCan2Uart_FIFO.fifo_count--;
            gCan2Uart_FIFO.fifo_out++;
            if(gCan2Uart_FIFO.fifo_out >= C2U_FIFO_SIZE)gCan2Uart_FIFO.fifo_out = 0;
#ifdef UART_TRANSPARENT     /* UART Transparent transmission */
            /* Convert message format */
            processUartTxMsg_transparent(gUartTransmitGroup, &gUART_TX_Element);
            /* Send messages via UART */
            gUartTxflag = true;
            sendUartTxMsg(UART_0_INST, gUartTransmitGroup, (gUART_TX_Element.dlc));
#else       /* UART Protocol transmission */
            /* Convert message format */
            processUartTxMsg(gUartTransmitGroup, &gUART_TX_Element);
            /* Send messages via UART */
            gUartTxflag = true;
            sendUartTxMsg(UART_0_INST, gUartTransmitGroup, (3 + UART_ID_LENGTH + gUART_TX_Element.dlc));
#endif
        }

        /* Send received UART message to CAN */
        if(gUart2Can_FIFO.fifo_count>0)
        {
            /* Get gCAN_TX_Element from U2C_FIFO
             * Update indicator variable of U2C_FIFO */
            gCAN_TX_Element = gUart2Can_FIFO.fifo_pointer[gUart2Can_FIFO.fifo_out];
            gUart2Can_FIFO.fifo_count--;
            gUart2Can_FIFO.fifo_out++;
            if(gUart2Can_FIFO.fifo_out >= U2C_FIFO_SIZE)gUart2Can_FIFO.fifo_out = 0;
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
            if( gCan2Uart_FIFO.fifo_count >= C2U_FIFO_SIZE)
            {
                /* When the CAN bus is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                //__BKPT(0);
            }
            if(processFlag == true)
            {
                /* Convert the received message to gCAN_RX_Element */
                processCANRxMsg(&rxMsg, &gCAN_RX_Element);
                /* Store gCAN_RX_Element into C2U_FIFO
                 * Update indicator variable of C2U_FIFO */
                gCan2Uart_FIFO.fifo_pointer[gCan2Uart_FIFO.fifo_in] = gCAN_RX_Element;
                gCan2Uart_FIFO.fifo_count++;
                gCan2Uart_FIFO.fifo_in++;
                if(gCan2Uart_FIFO.fifo_in >= C2U_FIFO_SIZE)gCan2Uart_FIFO.fifo_in = 0;
            }
            break;
        default:
            break;
    }
}


void UART_0_INST_IRQHandler(void)
{
    bool processFlag = false;
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
#ifdef UART_TRANSPARENT     /* UART Transparent transmission */
            getUartRxMsg_transparent(UART_0_INST, gUartReceiveGroup);
            /*Enable Timer to detect timeout issue */
            DL_Timer_setLoadValue(TIMER_UART_INST, UART_TIMEOUT);
            DL_Timer_startCounter(TIMER_UART_INST);
#else       /* UART Protocol transmission */
            /* Returns true when the UART group is fully received */
            processFlag = getUartRxMsg(UART_0_INST, gUartReceiveGroup);
            if( gUart2Can_FIFO.fifo_count >= U2C_FIFO_SIZE)
            {
                /* When the UART is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                //__BKPT(0);
            }
            if(processFlag == true)
            {
                /* Convert the received message to gUART_RX_Element */
                processUartRxMsg(gUartReceiveGroup, &gUART_RX_Element);
                /* Store gUART_RX_Element into U2C_FIFO
                 * Update indicator variable of U2C_FIFO */
                gUart2Can_FIFO.fifo_pointer[gUart2Can_FIFO.fifo_in] = gUART_RX_Element;
                gUart2Can_FIFO.fifo_count++;
                gUart2Can_FIFO.fifo_in++;
                if(gUart2Can_FIFO.fifo_in >= U2C_FIFO_SIZE)gUart2Can_FIFO.fifo_in = 0;
                processFlag = false;
            }
#endif
            break;
        case DL_UART_MAIN_IIDX_TX:
            /* Put the transmit message into UART FIFO */
            if(gUartTxflag == true)
            {
#ifdef UART_TRANSPARENT     /* UART Transparent transmission */
                sendUartTxMsg(UART_0_INST, gUartTransmitGroup, (gUART_TX_Element.dlc));
#else           /* UART Protocol transmission */
                sendUartTxMsg(UART_0_INST, gUartTransmitGroup, (3 + UART_ID_LENGTH + gUART_TX_Element.dlc));
#endif
            }
            break;
        default:
            break;
    }
}

void TIMER_UART_INST_IRQHandler(void)
{
    bool processFlag;
    switch (DL_Timer_getPendingInterrupt(TIMER_UART_INST)) {
        case DL_TIMER_IIDX_ZERO:
#ifdef UART_TRANSPARENT     /* UART Transparent transmission */
            /*Timeout to trigger receiving one UART packet*/
            processFlag = true;
            if( gUart2Can_FIFO.fifo_count >= U2C_FIFO_SIZE)
            {
                /* When the UART is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                //__BKPT(0);
            }
            if(processFlag == true) //Timeout to trigger receiving one UART packet
            {
                /* Convert the received message to gUART_RX_Element */
                processUartRxMsg_transparent(gUartReceiveGroup, &gUART_RX_Element);
                /* Store gUART_RX_Element into U2C_FIFO
                 * Update indicator variable of U2C_FIFO */
                gUart2Can_FIFO.fifo_pointer[gUart2Can_FIFO.fifo_in] = gUART_RX_Element;
                gUart2Can_FIFO.fifo_count++;
                gUart2Can_FIFO.fifo_in++;
                if(gUart2Can_FIFO.fifo_in >= U2C_FIFO_SIZE)gUart2Can_FIFO.fifo_in = 0;
                processFlag = false;
                gGetUartRxMsg_Count = 0;
            }
#endif
            break;
        default:
            break;
    }
}
