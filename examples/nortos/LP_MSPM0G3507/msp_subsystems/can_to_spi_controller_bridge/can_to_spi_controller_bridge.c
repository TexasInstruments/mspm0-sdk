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

/* Variables for C2S_FIFO
 * C2S_FIFO is used to temporarily store message from CAN to SPI */
Custom_Element gC2S_FIFO[C2S_FIFO_SIZE];
Custom_FIFO gCan2Spi_FIFO = {0, 0, 0, gC2S_FIFO};

/* Variables for S2C_FIFO
 * S2C_FIFO is used to temporarily store message from SPI to CAN */
Custom_Element gS2C_FIFO[S2C_FIFO_SIZE];
Custom_FIFO gSpi2Can_FIFO = {0, 0, 0, gS2C_FIFO};

int main(void)
{
    SYSCFG_DL_init();

    gInterruptLine1Status = 0;

    /* Enable Interrupts */
    NVIC_ClearPendingIRQ(CANFD0_INT_IRQn);
    NVIC_ClearPendingIRQ(SPI_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(GPIO_SWITCHES_INT_IRQN);
    NVIC_EnableIRQ(CANFD0_INT_IRQn);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CANFD0))
        ;

    while (1) {
        /* Send received CAN message to SPI */
        if(gCan2Spi_FIFO.fifo_count>0 && gSpiTxflag == false)
        {
            /* Get gSPI_TX_Element from C2S_FIFO
             * Update indicator variable of C2S_FIFO */
            gSPI_TX_Element = gCan2Spi_FIFO.fifo_pointer[gCan2Spi_FIFO.fifo_out];
            gCan2Spi_FIFO.fifo_count--;
            gCan2Spi_FIFO.fifo_out++;
            if(gCan2Spi_FIFO.fifo_out >= C2S_FIFO_SIZE)gCan2Spi_FIFO.fifo_out = 0;
#ifdef SPI_TRANSPARENT
            /* Convert message format */
            processSpiTxMsg_transparent(gSpiTransmitGroup, &gSPI_TX_Element);
            /* Send messages via SPI */
            /* Put one bytes from gSpiTransmitGroup to SPI FIFO */
            gSpiTxflag = true;
            gSpiDirection = false;
            sendSpiTxMsg(SPI_0_INST, gSpiTransmitGroup, (gSPI_TX_Element.dlc));
#else
            /* Convert message format */
            processSpiTxMsg(gSpiTransmitGroup, &gSPI_TX_Element);
            /* Send messages via SPI */
            /* Put one bytes from gSpiTransmitGroup to SPI FIFO */
            gSpiTxflag = true;
            gSpiDirection = false;
            sendSpiTxMsg(SPI_0_INST, gSpiTransmitGroup, (3 + SPI_ID_LENGTH + gSPI_TX_Element.dlc));
#endif
        }

        /* Send received SPI message to CAN */
        if(gSpi2Can_FIFO.fifo_count>0)
        {
            /* Get gCAN_TX_Element from S2C_FIFO
             * Update indicator variable of S2C_FIFO */
            gCAN_TX_Element = gSpi2Can_FIFO.fifo_pointer[gSpi2Can_FIFO.fifo_out];
            gSpi2Can_FIFO.fifo_count--;
            gSpi2Can_FIFO.fifo_out++;
            if(gSpi2Can_FIFO.fifo_out >= S2C_FIFO_SIZE)gSpi2Can_FIFO.fifo_out = 0;
            /* Convert message format */
            processCANTxMsg(&txMsg0, &gCAN_TX_Element);
            /* Send messages via CAN */
            sendCANTxMsg(&txMsg0);
        }
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            /* If SW is low, send the read command */
            if (!DL_GPIO_readPins(GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN))
            {
#ifdef SPI_TRANSPARENT
            /* Send messages via SPI */
            /* Put one bytes from gSpiTransmitGroup to SPI FIFO */
            gSpiRxflag = true;
            gSpiDirection = true;
            readSpiRxMsg(SPI_0_INST, (SPI_TRANSPARENT_LENGTH));
#else
            gSpiRxflag = true;
            gSpiDirection = true;
            gSpiReceiveGroup[SPI_ID_LENGTH + 2] = 1;
            readSpiRxMsg(SPI_0_INST, (3 + SPI_ID_LENGTH + gSpiReceiveGroup[SPI_ID_LENGTH + 2]));
#endif
            }
            break;
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
            if( gCan2Spi_FIFO.fifo_count >= C2S_FIFO_SIZE)
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
                /* Store gCAN_RX_Element into C2S_FIFO
                 * Update indicator variable of C2S_FIFO */
                gCan2Spi_FIFO.fifo_pointer[gCan2Spi_FIFO.fifo_in] = gCAN_RX_Element;
                gCan2Spi_FIFO.fifo_count++;
                gCan2Spi_FIFO.fifo_in++;
                if(gCan2Spi_FIFO.fifo_in >= C2S_FIFO_SIZE)gCan2Spi_FIFO.fifo_in = 0;
            }
            break;
        default:
            break;
    }
}

void SPI_0_INST_IRQHandler(void)
{
    bool processFlag = false;
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
#ifdef SPI_TRANSPARENT
            getSpiRxMsg_transparent(SPI_0_INST, gSpiReceiveGroup);
            /* Set processFlag when the message is fully received */
            if(gSpiDirection == true)
            {
                /* SPI master receiving */
                if(gGetSpiRxMsg_Count >= SPI_TRANSPARENT_LENGTH)
                    processFlag = true;
            }
            else
            {
                /* SPI master transmitting */
                if(gGetSpiRxMsg_Count >= (gSPI_TX_Element.dlc))
                    processFlag = true;
            }
            if(gSpi2Can_FIFO.fifo_count >= S2C_FIFO_SIZE)
            {
                /* When the SPI is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                //__BKPT(0);
            }
            if(processFlag == true)
            {
                /* Convert the received message to gSPI_RX_Element */
                processSpiRxMsg_transparent(gSpiReceiveGroup, &gSPI_RX_Element);
                /* Store gSPI_RX_Element into S2C_FIFO
                 * Update indicator variable of S2C_FIFO */
                gSpi2Can_FIFO.fifo_pointer[gSpi2Can_FIFO.fifo_in] = gSPI_RX_Element;
                gSpi2Can_FIFO.fifo_count++;
                gSpi2Can_FIFO.fifo_in++;
                if(gSpi2Can_FIFO.fifo_in >= S2C_FIFO_SIZE)gSpi2Can_FIFO.fifo_in = 0;
                processFlag = false;
                gGetSpiRxMsg_Count = 0;
            }
            /* Put the transmit message into SPI FIFO */
            if(gSpiTxflag == true)
            {
                sendSpiTxMsg(SPI_0_INST, gSpiTransmitGroup, (gSPI_TX_Element.dlc));
            }
            else if(gSpiRxflag == true)
            {
                readSpiRxMsg(SPI_0_INST, (SPI_TRANSPARENT_LENGTH));
            }
#else
            /* Returns true when the SPI group is fully received */
            processFlag = getSpiRxMsg(SPI_0_INST, gSpiReceiveGroup);
            if(gSpi2Can_FIFO.fifo_count >= S2C_FIFO_SIZE)
            {
                /* When the SPI is overloaded, stop receiving messages */
                processFlag = false;
                /* Overload handle */
                __BKPT(0);
            }
            if(processFlag == true)
            {
                /* Convert the received message to gSPI_RX_Element */
                processSpiRxMsg(gSpiReceiveGroup, &gSPI_RX_Element);
                /* Store gSPI_RX_Element into S2C_FIFO
                 * Update indicator variable of S2C_FIFO */
                gSpi2Can_FIFO.fifo_pointer[gSpi2Can_FIFO.fifo_in] = gSPI_RX_Element;
                gSpi2Can_FIFO.fifo_count++;
                gSpi2Can_FIFO.fifo_in++;
                if(gSpi2Can_FIFO.fifo_in >= S2C_FIFO_SIZE)gSpi2Can_FIFO.fifo_in = 0;
                processFlag = false;
            }
            /* Put the transmit message into SPI FIFO */
            if(gSpiTxflag == true)
            {
                sendSpiTxMsg(SPI_0_INST, gSpiTransmitGroup, (3 + SPI_ID_LENGTH + gSPI_TX_Element.dlc));
            }
            else if(gSpiRxflag == true)
            {
                readSpiRxMsg(SPI_0_INST, (3 + SPI_ID_LENGTH + gSpiReceiveGroup[SPI_ID_LENGTH + 2]));
            }
#endif
            break;
        default:
            break;
    }
}
