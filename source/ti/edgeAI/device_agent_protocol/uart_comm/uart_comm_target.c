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
#include "uart_comm_target.h"

/*! @brief Macro for left shift by 8 bits */
#define LEFT_SHIFT_8(x)     ((x) << 8)
/*! @brief Macro for right shift by 8 bits */
#define RIGHT_SHIFT_8(x)    ((x) >> 8)


/**
 * @brief     Resets pointer and length of the buffer
 * @param[in] frame  Pointer to BufferInfo (Rx,Tx)
 */
void UART_clearBuffer(BufferInfo *frame)
{
    frame->ptr = 0;
    frame->len = 0;
}

void UART_init(UART_Instance *UART_handle)
{
    UART_clearBuffer(&UART_handle->rxMsg);
    UART_clearBuffer(&UART_handle->txMsg);

    UART_handle->dataLen = 0;
    UART_handle->isCrc = false;
    UART_handle->TxStatus = UART_STATUS_IDLE;
    UART_handle->RxStatus = UART_STATUS_IDLE;
    UART_handle->error = ERROR_TYPE_NONE;

    DMA_RX_init(UART_handle);
}

void UART_checkForCommand(UART_Instance *UART_handle)
{

    if(UART_handle->RxStatus == UART_STATUS_RX_BUFFERING)
    {
        /* Check if all the bytes of command frame are stored */
        if(UART_handle->rxMsg.len <= (DMA_RX_TRANSFER_SIZE 
                                - DL_DMA_getTransferSize(DMA,DMA_CH0_CHAN_ID)))
        {
            UART_handle->RxStatus = UART_STATUS_PROCESS_FRAME;
        }
    }
}

void UART_sendBuffer(BufferInfo *frame)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &frame->buffer[0]);

#if defined(__MCU_HAS_UNICOMMUART__)
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &UART_DAP_INST->uart->TXDATA);
#endif

#if defined(__MSPM0_HAS_UART__)
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &UART_DAP_INST->TXDATA);
#endif

    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, frame->len);
    DL_DMA_setTransferMode(DMA,DMA_CH1_CHAN_ID, DL_DMA_SINGLE_TRANSFER_MODE);
    DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL1);
    DL_DMA_enableInterrupt(DMA, DL_DMA_INTERRUPT_CHANNEL1);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
}

void DMA_RX_init(UART_Instance *UART_handle)
{
    DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);

#if defined(__MCU_HAS_UNICOMMUART__)
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &UART_DAP_INST->uart->RXDATA);
#endif

#if defined(__MSPM0_HAS_UART__)
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &UART_DAP_INST->RXDATA);
#endif

    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID,
                                     (uint32_t) &UART_handle->rxMsg.buffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, DMA_RX_TRANSFER_SIZE);

    DL_DMA_Full_Ch_setEarlyInterruptThreshold(DMA, DMA_CH0_CHAN_ID,
                                         DL_DMA_EARLY_INTERRUPT_THRESHOLD_64);
    DL_DMA_enableInterrupt(DMA, DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL0);

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

uint8_t CRC_calc8(uint8_t* ptr,uint16_t len)
{
    uint8_t CRC_Init = CRC_INIT_VALUE;
    uint8_t crc = 0x0;
    uint8_t i = 0, j = 0, carry = 0;

    /* Default initial value for CRC */
    crc = CRC_Init;

    for(i = 0; i < len; i++)
    {
        crc ^= *(ptr + i);
        /* Loop to shift the message bits one by one */
        for (j = 0; j < 8; j++)
        {
            carry = crc & 0x80;
            crc <<= 1;
            if (carry)
            {
                /* CCITT Polynomial x^8 + x^2 + x + 1 */
                crc ^= 0x07;
            }
        }
    }

    return (crc & 0xFF);
}


void UART_reSync(UART_Instance* UART_handle)
{
    if(DL_DMA_getTransferSize(DMA,DMA_CH0_CHAN_ID) 
    != DMA_RX_TRANSFER_SIZE)
    {
        /* Check if last byte stored is end byte */
        while(UART_handle->rxMsg.buffer[(DMA_RX_TRANSFER_SIZE 
        - DL_DMA_getTransferSize(DMA,DMA_CH0_CHAN_ID)) - 1] 
        != FRAME_END_BYTE)
        {};
    }

    return;
}
