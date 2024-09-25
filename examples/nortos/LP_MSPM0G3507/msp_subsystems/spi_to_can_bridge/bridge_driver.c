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

#include "ti_msp_dl_config.h"
#include "bridge_config.h"

static uint16_t num = 1;

uint16_t gSpiRXdummyGroup[SPI_RX_SIZE];
uint16_t gRead_rxFIFO0_Data[SPI_RX_SIZE];
uint16_t gRead_rxBuf_Data[SPI_RX_SIZE];
uint16_t gRead_register_Data[REGISTER_DATASIZE];
uint16_t gSpiReceiveGroup[SPI_RX_SIZE];
uint16_t gSpiTransmitGroup[SPI_TX_SIZE];
uint32_t gCan_Base_Address = (uint32_t)CAN_INST;
bool gtimeout_status;
bool gDMA_operation;
volatile uint16_t gRxFIFO0GetIndex;
volatile uint16_t gRxBufnum;
volatile uint32_t gRxBufstatus;
volatile uint16_t gTxFIFOPutIndex;
volatile uint16_t gTX_Buf_dataBytes_size;
volatile uint16_t gTX_Fifo_dataBytes_size;
volatile uint16_t gRX_Buf_dataBytes_size;
volatile uint16_t gRX_Fifo_dataBytes_size;
volatile uint16_t gRead_Register_dataBytes_size;
volatile uint16_t gRX_Status_dataBytes_size;
volatile uint32_t gRegister_Read_Data;
volatile uint16_t gRxFIFO0_FillLevel;
volatile bool gRxFIFO0_Full;
volatile uint16_t gRxBUF_NDAT_Status;
volatile uint16_t gDLEC_Bits;
volatile uint16_t gLEC_Bits;
volatile uint16_t gRX_Status[2];
volatile uint32_t gAddress;
volatile uint16_t gAddress_Offset;
volatile uint32_t gTxBufAddr;
volatile uint32_t gTxFifoAddr;
volatile uint32_t gRxFifoAddr;
volatile uint32_t gRxBufAddr;
volatile uint16_t gRegister_data_L;
volatile uint16_t gRegister_data_M;
volatile bool gError_signal;
volatile bool gAddress_flag;
/* Offset addresses of MCAN Registers */
const uint16_t offset_address_array[] = {0x0000, 0x0004, 0x0008, 0x000C, 0x0010, 0x0014, 0x0018, 0x001C,
                                   0x6004, 0x6008, 0x6204, 0x6208, 0x6480, 0x6800, 0x6804, 0x6814, 0x7000, 0x7004, 0x700C, 0x7010,
                                   0x7014, 0x7018, 0x701C, 0x7020, 0x7024, 0x7028, 0x702C, 0x7040, 0x7044, 0x7048, 0x7050, 0x7054,
                                   0x7058, 0x705C, 0x7080, 0x7084, 0x7088, 0x7090, 0x7094, 0x7098, 0x709C, 0x70A0, 0x70A4, 0x70A8,
                                   0x70AC, 0x70B0, 0x70B4, 0x70B8, 0x70BC, 0x70C0, 0x70C4, 0x70C8, 0x70CC, 0x70D0, 0x70D4, 0x70D8,
                                   0x70DC, 0x70E0, 0x70E4, 0x70F0, 0x70F4, 0x70F8, 0x7200, 0x7204, 0x7208, 0x720C, 0x7210, 0x7214,
                                   0x7218, 0x721C, 0x7220, 0x7224, 0x7228, 0x7400, 0x7408, 0x740C, 0x7410, 0x7414, 0x7418, 0x741C,
                                   0x7420, 0x7424, 0x7428, 0x743C, 0x7440, 0x7480, 0x74C0, 0x753C, 0x7540, 0x7580, 0x75C0, 0x7600,
                                   0x7604, 0x7608, 0x760C, 0x7820, 0x7828, 0x7830, 0x7838, 0x7840, 0x7848, 0x78E0, 0x78FC, 0x7900,
                                   0x7904, 0x7908, 0x790C};


bool instruction_check(uint16_t *SpiReceiveGroup)
{
    DL_GPIO_setPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
    DL_TimerA_startCounter(TIMER_0_INST);
    uint8_t ui8loopVar;
    uint32_t register_new_data;
    uint32_t register_old_data;
    uint32_t mask_bytes;
    uint8_t buf_num;
    /* Making num as 1 to have as an index for next bytes of instruction received */
    num = 1;
    gDMA_operation = 0;
    /* First Byte is Instruction Byte */
    gSpiReceiveGroup[0] = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

    switch(gSpiReceiveGroup[0] & AVV_INSTRUCTION_MASK){

        case AVV_BIT_MODIFY_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <2 offset address bytes> <4 bytes of mask data> <4 bytes of new data>
             */
            gError_signal = 0;
            gtimeout_status = 0;
            /* Receives 2 bytes offset address, 4 bytes mask data and 4 bytes of new data to be written to the register */
            /* Get 16 bit register Offset address */
            gAddress_Offset = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            /* Get 32 bit CANFD0 Register address */
            gAddress = gCan_Base_Address + gAddress_Offset;
            /* Get the existing data in the register */
            register_old_data = *(uint32_t*) gAddress;
            while(num<=4){
                SpiReceiveGroup[num] = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
                num++;
            }

            gAddress_flag = address_check(gAddress_Offset);
            if (gtimeout_status == 0){
                if(gAddress_flag){
                    /* Store the bit mask bytes */
                    mask_bytes = (gSpiReceiveGroup[2] << 16) | (gSpiReceiveGroup[1]);
                    /* Store the new register data to be written */
                    register_new_data = (gSpiReceiveGroup[4] << 16) | (gSpiReceiveGroup[3]);
                    /* Sore the new data into the register with the mask */
                    *(uint32_t*)gAddress = (mask_bytes & register_new_data)|((~mask_bytes) & register_old_data);
                }
                else{
                    gError_signal = 1;
                }
                DL_TimerA_stopCounter(TIMER_0_INST);
            }
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            /* Returns true when the group is fully received */
            return true;
            break;

        case AVV_RESET_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Reset_Instruction bytes>
             */
            /* Write the data into the register to reset CANFD0 */
            DL_MCAN_reset(CAN_INST);
            DL_TimerA_stopCounter(TIMER_0_INST);
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_POWER_ENABLE_AND_CAN_CONFIG_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Powerenable_Instruction bytes>
             */
            /* CAN Message RAM Configurations */
            SYSCFG_DL_user_init();
            DL_TimerA_stopCounter(TIMER_0_INST);
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_LOAD_TX_BUFFER_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes> <sending data bytes size> <Data to be stored in TX Buffer>
             * Last 2 bits in the instruction indicates the CANFD buffer number to store the data
             */
            gDMA_operation = 1;
            ui8loopVar = 0;
            gtimeout_status = 0;
            /* Get the TX Buffer number */
            buf_num = gSpiReceiveGroup[0] & CAN_TX_BUF_NUM_MASK;
            /* Get the TX Buffer address */
            gTxBufAddr = AVV_MCAN_TXBUF_START_SOC_ADDR + buf_num * AVV_MCAN_ELEM_SIZE;
            /* DMA Configurations to load the received data into the TXBUFFER */
            DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) (&SPI_1_INST->RXDATA));
            DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(gTxBufAddr));
            /* Receives data byte size to decide number of data bytes to be stored in TX buffer */
            gTX_Buf_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            if (gtimeout_status == 0){
                DL_SPI_disableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));
                DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, (gTX_Buf_dataBytes_size/2));
                /* Enable the DMA channel */
                DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
                /* Enable the SPI interrupt for triggering the DMA receive event */
                DL_SPI_enableDMAReceiveEvent(SPI_1_INST, DL_SPI_DMA_INTERRUPT_RX);
                gError_signal = 0;
            }
            /* Returns true when the group is fully received */
            return true;
            break;

        case AVV_LOAD_TX_FIFO_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes> <sending data bytes size> <Data to be stored in TX Buffer>
             */
            gDMA_operation = 1;
            gtimeout_status = 0;
            /* Get the Put index of CANFD0 TXFIFO  */
            gTxFIFOPutIndex = (CAN_INST->MCANSS.MCAN.MCAN_TXFQS & AVV_MCAN_TXFOS_FOPI_Msk) >> AVV_MCAN_TXFOS_FOPI_Pos;
            /* Get the TXFIFO address */
            gTxFifoAddr = AVV_MCAN_TXBUF_START_SOC_ADDR + gTxFIFOPutIndex * AVV_MCAN_ELEM_SIZE;
            /* DMA Configurations to load the received data into the TXFIFO */
            DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) (&SPI_1_INST->RXDATA));
            DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(gTxFifoAddr));
            /* Receives data byte size to decide number of data bytes to be stored in TX FIFO */
            gTX_Fifo_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            if (gtimeout_status == 0){
                /* Receives all the data bytes to be stored in the TX FIFO */
                DL_SPI_disableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));
                DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, (gTX_Fifo_dataBytes_size/2));
                /* Enable the DMA channel */
                DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
                /* Enable the SPI interrupt for triggering the DMA receive event */
                DL_SPI_enableDMAReceiveEvent(SPI_1_INST, DL_SPI_DMA_INTERRUPT_RX);
                gError_signal = 0;
            }
            /* Returns true when the group is fully received */
            return true;
            break;

        case AVV_REQUEST_TO_SEND_TXBUFFER_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes>
             * Last 2 bits in the instruction indicates the CANFD buffer number
             */
            /* Get the TX Buffer number */
            buf_num = gSpiReceiveGroup[0] & CAN_TX_BUF_NUM_MASK;
            /* Enable Transmission interrupt.*/
            DL_MCAN_TXBufTransIntrEnable(CAN_INST, buf_num, 1U);
            /* set Buffer Add Request for transmission */
            DL_MCAN_TXBufAddReq(CAN_INST, buf_num);
            gError_signal = 0;
            DL_TimerA_stopCounter(TIMER_0_INST);
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_REQUEST_TO_SEND_TXFIFO_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes>
             */
            /* store Put index of CANFD0 TXFIFO  */
            gTxFIFOPutIndex = (CAN_INST->MCANSS.MCAN.MCAN_TXFQS & AVV_MCAN_TXFOS_FOPI_Msk) >> AVV_MCAN_TXFOS_FOPI_Pos;
            /* Enable Transmission interrupt.*/
            DL_MCAN_TXBufTransIntrEnable(CAN_INST, gTxFIFOPutIndex, 1U);
            /* set Buffer Add Request for transmission */
            DL_MCAN_TXBufAddReq(CAN_INST, gTxFIFOPutIndex);
            gError_signal = 0;
            DL_TimerA_stopCounter(TIMER_0_INST);
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_WRITE_REGISTER_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <2 offset address bytes> <4 bytes of register data to be written>
             */
            gError_signal = 0;
            gtimeout_status = 0;
            /* Receives 2 bytes offset address and 4 bytes register data to be written */
            gAddress_Offset = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            /* Get 32 bit CANFD0 Register address */
            gAddress = gCan_Base_Address + gAddress_Offset;
            /* LSB 16bits of the Register Data */
            gRegister_data_L = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            /* MSB 16bits of the Register Data */
            gRegister_data_M = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            if (gtimeout_status == 0){
                gAddress_flag = address_check(gAddress_Offset);
                if(gAddress_flag){
                    /* Writing the 32Bit Data into the Register */
                    *(uint32_t*)gAddress = (gRegister_data_M << 16) | gRegister_data_L;
                }
                else{
                    gError_signal = 1;
                }
                DL_TimerA_stopCounter(TIMER_0_INST);
            }
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            /* Returns true when the group is fully received */
            return true;
            break;

        case AVV_READ_REGISTER_DATA_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <2 offset address bytes>
             */
            gError_signal = 0;
            gtimeout_status = 0;
            /* Get 16 bit register Offset address */
            gAddress_Offset = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            /* Get 32 bit CANFD0 Register address */
            gAddress = gCan_Base_Address + gAddress_Offset;
            gAddress_flag = address_check(gAddress_Offset);
            if (gtimeout_status == 0){
                if(gAddress_flag){
                    /* Reading the Data from the register */
                    gRegister_Read_Data = *(uint32_t*) gAddress;
                    /* Storing MSB 16bits of the Register Data into the variable */
                    gRead_register_Data[1] = ((gRegister_Read_Data) >> 16) & 0xFFFF;
                    /* Storing LSB 16bits of the Register Data into the variable */
                    gRead_register_Data[0] = (gRegister_Read_Data) & 0xFFFF;
                }
                else{
                    gError_signal = 1;
                }
                DL_TimerA_stopCounter(TIMER_0_INST);
            }
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            /* Returns true when the group is fully received */
            return true;
            break;

        case AVV_FETCH_REGISTER_DATA_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <Number of data bytes to fetch (2 or 4)> <Dummy data bytes from controller to receive the register data from bridge>
             */
            gtimeout_status = 0;
            /* Receives data byte size to decide number of register data bytes to be sent back */
            gRead_Register_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            if (gtimeout_status == 0){
                while(ui8loopVar < (gRead_Register_dataBytes_size/2)){
                    DL_SPI_transmitDataBlocking16(SPI_1_INST, gRead_register_Data[ui8loopVar]);
                    gSpiRXdummyGroup[0] = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
                    if(gtimeout_status == 1){
                              break;
                    }
                    ui8loopVar+=1;
                }
                /* Make sure SPI RXFIFO is empty */
                while(false == DL_SPI_isRXFIFOEmpty(SPI_1_INST)){
                    gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
                }
                gError_signal = 0;
                DL_TimerA_stopCounter(TIMER_0_INST);
            }
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_READ_RX_BUF_DATA_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes> <data bytes size to read data from RX Buffer>
             * Last 2 bits in the instruction indicates the CANFD buffer number to read the data
             */
            gtimeout_status = 0;
            /* Get the RX Buffer number to read the Data  */
            gRxBufnum = gSpiReceiveGroup[0] & CAN_RX_BUF_NUM_MASK;
            /* Get the RX Buffer address to read the Data  */
            gRxBufAddr = AVV_MCAN_RXBUF_START_SOC_ADDR + gRxBufnum * AVV_MCAN_ELEM_SIZE;
            /* DMA Configurations to load the CAN RX BUFFER data into the variable */
            DL_DMA_disableChannel(DMA, DMA_CH2_CHAN_ID);
            DL_DMA_setSrcAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t) (gRxBufAddr));
            DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t) &gRead_rxBuf_Data[0]);
            /* Receiving data byte size to read from RX Buffer address */
            gRX_Buf_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            if (gtimeout_status == 0){
                DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, (gRX_Buf_dataBytes_size/2));
                DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);
                DL_DMA_startTransfer(DMA, DMA_CH2_CHAN_ID);
                /* Getting the New data status from the NDAT register */
                gRxBufstatus = (0b1 << gRxBufnum);
                /* Clear the RX Buffer New Data status */
                CAN_INST->MCANSS.MCAN.MCAN_NDAT1 = gRxBufstatus;

                /* DMA Configurations to load the received data into the TXFIFO */
                DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) (&gRead_rxBuf_Data[0]));
                DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&SPI_1_INST->TXDATA));
                DL_SPI_enableDMATransmitEvent(SPI_1_INST);
                gError_signal = 0;
                DL_TimerA_stopCounter(TIMER_0_INST);
            }
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_FETCH_RX_BUF_DATA_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <Number of data bytes to fetch> <Dummy data bytes from controller to receive the RX Buffer data from bridge>
             */
            gDMA_operation = 1;
            gtimeout_status = 0;
            /* Disable the SPI RX interrupt */
            DL_SPI_disableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));
            /* Receives data byte size to decide number of register data bytes to be sent back */
            gRX_Buf_dataBytes_size = DL_SPI_receiveData16(SPI_1_INST);
            /* Configure the DMA Transfer size */
            DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, (gRX_Buf_dataBytes_size/2));
            /* Enable the DMA channel */
            DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

            /* Make sure SPI RXFIFO is empty */
            while(false == DL_SPI_isRXFIFOEmpty(SPI_1_INST)){
                gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            }
            gError_signal = 0;
            return true;
            break;

        case AVV_READ_RX_FIFO_DATA_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes> <data bytes size to read data from RX Buffer>
             */
            gtimeout_status = 0;
            /* store get index of CANFD0 RXFIFO */
            gRxFIFO0GetIndex = (CAN_INST->MCANSS.MCAN.MCAN_RXF0S & AVV_MCAN_RXFOS_FOGI_Msk) >> AVV_MCAN_RXFOS_FOGI_Pos;
            /* Get the address of CANFD0 RXFIFO element */
            gRxFifoAddr = AVV_MCAN_RXFIFO_START_SOC_ADDR + gRxFIFO0GetIndex * AVV_MCAN_ELEM_SIZE;
            /* DMA Configurations to load the CAN RX FIFO data into the variable */
            DL_DMA_disableChannel(DMA, DMA_CH2_CHAN_ID);
            DL_DMA_setSrcAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t) (gRxFifoAddr));
            DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t) &gRead_rxFIFO0_Data[0]);
            /* Receiving data byte size to read from RX FIFO address */
            gRX_Fifo_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            if (gtimeout_status == 0){
                DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, (gRX_Fifo_dataBytes_size/2));
                DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);
                DL_DMA_startTransfer(DMA, DMA_CH2_CHAN_ID);
                /* store get index of CANFD0 RXFIFO */
                gRxFIFO0GetIndex = (CAN_INST->MCANSS.MCAN.MCAN_RXF0S & AVV_MCAN_RXFOS_FOGI_Msk) >> AVV_MCAN_RXFOS_FOGI_Pos;
                /* Acknowledge the CANFD0 RXFIFO */
                DL_MCAN_writeRxFIFOAck(CAN_INST, DL_MCAN_RX_FIFO_NUM_0, gRxFIFO0GetIndex);

                /* DMA Configurations to load the received data into the TXFIFO */
                DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) (&gRead_rxFIFO0_Data[0]));
                DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&SPI_1_INST->TXDATA));
                /* Enable the SPI interrupt for triggering the DMA Transfer event */
                DL_SPI_enableDMATransmitEvent(SPI_1_INST);
                gError_signal = 0;
                DL_TimerA_stopCounter(TIMER_0_INST);
            }
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);
            return true;
            break;

        case AVV_FETCH_RX_FIFO_DATA_INSTRUCTION:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <Number of data bytes to fetch> <Dummy data bytes from controller to receive the RX FIFO data from bridge>
             */
            gDMA_operation = 1;
            gtimeout_status = 0;
            /* Disable the SPI RX interrupt */
            DL_SPI_disableInterrupt(SPI_1_INST,  (DL_SPI_INTERRUPT_RX));
            /* Receives data byte size to decide number of register data bytes to be sent back */
            gRX_Fifo_dataBytes_size = DL_SPI_receiveData16(SPI_1_INST);
            /* Configure the DMA Transfer size */
            DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, (gRX_Fifo_dataBytes_size/2));
            /* Enable the DMA channel */
            DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

            /* Make sure SPI RXFIFO is empty */
            while(false == DL_SPI_isRXFIFOEmpty(SPI_1_INST)){
                gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            }

            gError_signal = 0;
            return true;
            break;


        case AVV_READ_RX_STATUS:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_bytes> <data bytes size to read status (1 or 2)>
             */
            gRX_Status_dataBytes_size = 0;
            gRX_Status_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            /* Get the RXFIFO0 Fill level */
            gRxFIFO0_FillLevel = (CAN_INST->MCANSS.MCAN.MCAN_RXF0S & AVV_MCAN_RXF0S_FOFL_Msk);
            /* Get the RXFIFO0 Full flag*/
            gRxFIFO0_Full = ((CAN_INST->MCANSS.MCAN.MCAN_RXF0S & AVV_MCAN_RXF0S_FOF_Msk) >> AVV_MCAN_RXF0S_FOF_Pos) & 0b1;
            /* Get the RXBUFFERS New Data status*/
            gRxBUF_NDAT_Status = (CAN_INST->MCANSS.MCAN.MCAN_NDAT1 & AVV_MCAN_NDAT1_Msk);
            gDLEC_Bits = (CAN_INST->MCANSS.MCAN.MCAN_PSR & AVV_MCAN_DLEC_Msk) >> 8;
            gLEC_Bits = (CAN_INST->MCANSS.MCAN.MCAN_PSR & AVV_MCAN_LEC_Msk);

            /*  Two Bytes RX status indication
             * Bits       --->    |    15     |      14      | 13   12   11 | 10   9   8 | 7   6   5   4 |      3      |   2     1     0   |
             * Indication --->    |Timeout bit| error signal |   DLEC bits  |  LEC bits  |  NDAT status  | RXFIFO Full | RXFIFO Fill level |
             */
            if(gRX_Status_dataBytes_size == 2){
                gRX_Status[0] = (gtimeout_status << AVV_MCAN_TIMEOUT_SIGNAL_OFS) | (gError_signal << AVV_MCAN_ERROR_SIGNAL_OFS) | (gDLEC_Bits << AVV_MCAN_DLEC_OFS) | (gLEC_Bits << AVV_MCAN_LEC_OFS) | (gRxBUF_NDAT_Status << AVV_MCAN_NDAT_STATUS_OFS) | (gRxFIFO0_Full << AVV_MCAN_RxFIFO0_Full_OFS) | gRxFIFO0_FillLevel;
            }
            /*  One Byte RX status Bits indication
             * Bits       --->    | 7   6   5   4 |      3      |   2     1     0   |
             * Indication --->    |  NDAT status  | RXFIFO Full | RXFIFO Fill level |
             */
            if(gRX_Status_dataBytes_size == 1){
                gRX_Status[0] = (gRxBUF_NDAT_Status << AVV_MCAN_NDAT_STATUS_OFS) | (gRxFIFO0_Full << AVV_MCAN_RxFIFO0_Full_OFS) | gRxFIFO0_FillLevel;
            }
            gError_signal = 0;
            gtimeout_status = 0;
            DL_TimerA_stopCounter(TIMER_0_INST);
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);

            return true;
            break;

        case AVV_FETCH_RX_STATUS:
            /*
             * INSTRUCTION FORMAT :
             * <Instruction_type> <Number of data bytes to fetch(1 or 2)> <Dummy data bytes from controller to receive the status data from bridge>
             */
            ui8loopVar = 0;
            gRX_Status_dataBytes_size = 0;
            /* Receiving data byte size to send back RX Status */
            gRX_Status_dataBytes_size = DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);

            /* Send back RX Status */
            DL_SPI_transmitData16(SPI_1_INST, gRX_Status[0]);
            gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            /* Make sure SPI RXFIFO is empty */
            while(false == DL_SPI_isRXFIFOEmpty(SPI_1_INST)){
                gSpiRXdummyGroup[0] =  DL_SPI_receiveDataBlocking16_tilltimeout(SPI_1_INST);
            }
            gError_signal = 0;
            DL_TimerA_stopCounter(TIMER_0_INST);
            DL_GPIO_clearPins(GPIO_TOGGLE_PORT, GPIO_TOGGLE_BUSY_STATUS_PIN);

            return true;
            break;

        default:
            num = 1;
            gError_signal = 1;
            break;
    }

    return false;
}


bool address_check(uint16_t offset_address)
{
    for (int i=0; i<AVV_OFFSET_ADDRESS_ARRAY_SIZE; i++)
    {
        if (offset_address_array[i] == offset_address)
            return true;
    }
    return false;
}

uint16_t DL_SPI_receiveDataBlocking16_tilltimeout(SPI_Regs *spi)
{
    while (DL_SPI_isRXFIFOEmpty(spi)) {
        if(gtimeout_status == 1){
          break;
        }
    };
    return DL_SPI_receiveData16(spi);
}
