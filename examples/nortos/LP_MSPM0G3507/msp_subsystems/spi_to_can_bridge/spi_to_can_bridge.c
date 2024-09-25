/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include <ti_msp_dl_config.h>
#include <bridge_config.h>

uint16_t gSpiRXdummyGroup[SPI_TX_SIZE];

int main(void)
{

    SYSCFG_DL_init();
    DL_CORE_configInstruction(DL_CORE_CACHE_ENABLED, DL_CORE_PREFETCH_ENABLED, DL_CORE_LITERAL_CACHE_ENABLED);

    /* Clear and  Enable Interrupts */
    NVIC_ClearPendingIRQ(SPI_1_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(CANFD0_INT_IRQn);
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_1_INST_INT_IRQN);
    NVIC_EnableIRQ(CANFD0_INT_IRQn);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

    while(1);

}

void SPI_1_INST_IRQHandler(void)
{
    bool instruction_data_received_flag = 0;

    switch (DL_SPI_getPendingInterrupt(SPI_1_INST)) {
    case DL_SPI_IIDX_RX:
        /* Returns true when the SPI group is fully received */
        instruction_data_received_flag = instruction_check(gSpiReceiveGroup);
        /* Clear the Spi RX pending interrupt */
        DL_SPI_clearInterruptStatus(SPI_1_INST, SPI_CPU_INT_IMASK_RX_SET);
        break;

    case DL_SPI_IIDX_DMA_DONE_RX:
        /* DMA is done transferring data from RXFIFO to gRxPacket*/
        /* Disable the DMA Rx event after receiving the Data */
        DL_SPI_disableDMAReceiveEvent(SPI_1_INST, DL_SPI_DMA_INTERRUPT_RX);
        /* Disable the DMA channel after receiving the Data */
        DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
        /* Clear the SPI RX pending interrupt */
        DL_SPI_clearInterruptStatus(SPI_1_INST, SPI_CPU_INT_IMASK_RX_SET);
        /* Enable the SPI RX Interrupt*/
        DL_SPI_enableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));
        DL_TimerA_stopCounter(TIMER_0_INST);
        /* Clear the busy status GPIO pin */
        DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
        break;

    case DL_SPI_IIDX_DMA_DONE_TX:
        /* DMA is done transferring data from RXFIFO to gRxPacket*/
        /* Disable the DMA Rx event after receiving the Data */
        DL_SPI_disableDMATransmitEvent(SPI_1_INST);
        /* Disable the DMA channel after receiving the Data */
        DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);
        while(false == DL_SPI_isTXFIFOEmpty(SPI_1_INST));
        /* Make sure SPI RXFIFO is empty */
        while(false == DL_SPI_isRXFIFOEmpty(SPI_1_INST)){
            gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
        }
        /* Clear the SPI RX pending interrupt */
        DL_SPI_clearInterruptStatus(SPI_1_INST, SPI_CPU_INT_IMASK_RX_SET);
        /* Enable the SPI RX Interrupt */
        DL_SPI_enableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));
        DL_TimerA_stopCounter(TIMER_0_INST);

        /* Clear the busy status GPIO pin */
        DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
        break;

    default:
        break;
    }
}

void CANFD0_IRQHandler(void)
{
    volatile uint32_t gInterruptLine1Status = 0;

    switch (DL_MCAN_getPendingInterrupt(CAN_INST)) {
        case DL_MCAN_IIDX_LINE1:
            /* Check MCAN interrupts fired during RX of CAN package */
            gInterruptLine1Status = DL_MCAN_getIntrStatus(CAN_INST);
            DL_MCAN_clearIntrStatus(CAN_INST, gInterruptLine1Status, DL_MCAN_INTR_SRC_MCAN_LINE_1);
            /* Toggle the GPIO if CAN message frame received in RXFIFO or RXBUFFER  */
            if (((gInterruptLine1Status & MCAN_IR_DRX_MASK) == MCAN_IR_DRX_MASK) | ((gInterruptLine1Status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK)){
                DL_GPIO_setPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_CAN_NEWMSG_INDICATION_PIN);
                delay_cycles(GPIO_TOGGLE_DELAY);
                DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_CAN_NEWMSG_INDICATION_PIN);
            }

            break;
        default:
            break;
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
           /* Disable the DMA Rx event after receiving the Data */
           DL_SPI_disableDMAReceiveEvent(SPI_1_INST, DL_SPI_DMA_INTERRUPT_RX);
           /* Disable the DMA channel after receiving the Data */
           DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);

           /* Disable the DMA Rx event after receiving the Data */
           DL_SPI_disableDMATransmitEvent(SPI_1_INST);
           /* Disable the DMA channel after receiving the Data */
           DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);

           while(false == DL_SPI_isTXFIFOEmpty(SPI_1_INST));
           /* Make sure SPI RXFIFO is empty */
           while(false == DL_SPI_isRXFIFOEmpty(SPI_1_INST)){
               gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
           }

           /* Clear the SPI RX pending interrupt */
           DL_SPI_clearInterruptStatus(SPI_1_INST, SPI_CPU_INT_IMASK_RX_SET);
           /* Enable the SPI RX Interrupt */
           DL_SPI_enableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));

           /* Update the timeout status bit */
           gtimeout_status = 1;
           /* Clearing the busy status GPIO pin here when Timeout happens while doing DMA operation */
           if(gDMA_operation == 1)
           {
               DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
           }
           break;
        default:
            break;
    }
}



