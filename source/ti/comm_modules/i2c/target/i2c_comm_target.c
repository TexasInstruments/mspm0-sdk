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

#include "i2c_comm_target.h"

#define LEFT_SHIFT_8(x)     ((x) << 8)
#define RIGHT_SHIFT_8(x)    ((x) >> 8)

/* Function Declarations */
/**
 * @brief    Services the command stored in Rx Buffer.
 * @param[in] I2C_handle  Pointer to I2C_Instance
 */
static void I2C_serviceCommand(I2C_Instance *I2C_handle);

/**
 * @brief     Process the stored data in buffer.
 * @param[in] I2C_handle  Pointer to I2C_Instance
 */
static void I2C_processRxBuffer(I2C_Instance *I2C_handle);

/**
 * @brief     Verifies the buffer and sets the type of error if any.
 * @param[in] I2C_handle  Pointer to I2C_Instance
 */
static void I2C_verifyBuffer(I2C_Instance *I2C_handle);

/**
 * @brief    Process the read command and stores the data in Tx Buffer.
 * @param[in] frame  Pointer to BufferInfo (Tx)
 * @param[in] len    length of the data to be read
 * @param[in] addr   starting address of the data
 */
static void I2C_readCommand(BufferInfo *frame,uint8_t len,uint32_t addr);

/**
 * @brief    Process the write command and stores ACK in Tx Buffer.
 * @param[in] frame  Pointer to BufferInfo (Tx)
 * @param[in] len    length of the data to be written
 * @param[in] addr   starting address of the data
 * @param[in] data   Pointer to the data to be written
 */
static void I2C_writeCommand(BufferInfo *frame,uint8_t len,uint32_t addr,uint8_t *data);

/**
 * @brief    Stores the corresponding error byte in Tx Buffer.
 * @param[in] I2C_handle  Pointer to I2C_Instance
 */
static void I2C_sendErrorType(I2C_Instance *I2C_handle);

/**
 * @brief     Calculates CRC 16 for an array of 8-bit data
 * @param[in] ptr   Pointer to the data to compute CRC
 * @param[in] size  size of the array of data in bytes
 * @return    Returns the CRC value
 */
static uint16_t CRC_calc16(uint8_t* ptr,uint8_t size);


/**
 * @brief     16 bit to 8 bit
 * @param[in] b  pointer to unsigned 8 bit output
 * @param[in] u16  unsigned 16 bit input
 */
static void u8From16(uint8_t *b, uint16_t u16){
    b[0] = (uint8_t)u16;
    b[1] = (uint8_t)(u16 = RIGHT_SHIFT_8(u16));
}

/**
 * @brief     8 bit to 32 bit
 * @param[in] b  pointer to unsigned 8 bit output
 * @return    32 bit result
 */
uint32_t u32(uint8_t *b){
  uint32_t u;
  u = b[3];
  u = LEFT_SHIFT_8(u) + b[2];
  u = LEFT_SHIFT_8(u) + b[1];
  u = LEFT_SHIFT_8(u) + b[0];
  return u;
}

/**
 * @brief     8 bit to 16 bit
 * @param[in] b  pointer to unsigned 8 bit output
 * @return    16 bit result
 */
static uint16_t u16(uint8_t *b){
  uint16_t u;
  u = b[1];
  u = LEFT_SHIFT_8(u) + b[0];
  return u;
}

/**
 * @brief     Resets pointer and length of the buffer
 * @param[in] frame  Pointer to BufferInfo (Rx,Tx)
 */
__STATIC_INLINE void I2C_clearBuffer(BufferInfo *frame)
{
    frame->ptr = 0;
    frame->len = 0;
}

#ifdef CONFIG_MSPM0G351X
static uint16_t CRC_calc16(uint8_t* ptr,uint8_t size)
{
    uint8_t remainder = (size & 1);
    uint16_t size16 = size>>1;

    uint16_t checkSum = DL_CRCP_calculateBlock16(CRCP0,CRCP_SEED,
                                                    (uint16_t*)ptr,size16);

    if(remainder)
    {
        DL_CRCP_feedData8(CRCP0,ptr[size - 1]);
        checkSum = ((uint16_t) DL_CRCP_getResult16(CRCP0));
    }

    return checkSum;
}

#else
static uint16_t CRC_calc16(uint8_t* ptr,uint8_t size)
{
    uint8_t remainder = (size & 1);
    uint16_t size16 = size>>1;

    uint16_t checkSum = DL_CRC_calculateBlock16(CRC,CRC_SEED,
                                                    (uint16_t*)ptr,size16);

    if(remainder)
    {
        DL_CRC_feedData8(CRC,ptr[size - 1]);
        checkSum = ((uint16_t) DL_CRC_getResult16(CRC));
    }

    return checkSum;
}

#endif

void I2C_init(I2C_Instance* I2C_handle)
{
    I2C_clearBuffer(&I2C_handle->rxMsg);
    I2C_clearBuffer(&I2C_handle->txMsg);
 
    I2C_handle->dataLen = 0;
    I2C_handle->isCrc = false;
    I2C_handle->status = I2C_STATUS_IDLE;
    I2C_handle->error = ERROR_TYPE_NONE;

    DMA_RX_init(I2C_handle);
}

void I2C_checkForCommand(I2C_Instance *I2C_handle)
{
    if(I2C_handle->status == I2C_STATUS_RX_BUFFERING)
    {
        I2C_handle->status = I2C_STATUS_PROCESS_FRAME;
        I2C_processRxBuffer(I2C_handle);
    }
}

static void I2C_verifyBuffer(I2C_Instance *I2C_handle)
{
    /* Get Data length from buffer */
    I2C_handle->dataLen = ( I2C_handle->rxMsg.buffer[CTRL_IDX] & LEN_MASK ) + 1;

    /* Check whether CRC is enabled */
    I2C_handle->isCrc = ( I2C_handle->rxMsg.buffer[CTRL_IDX] & CRC_MASK );

    uint8_t crcLen = I2C_handle->isCrc ? (CRC_SIZE) : 0;

    /* Expected length of Frame based on CTRL byte */
    I2C_handle->rxMsg.len = CTRL_SIZE + ADDR_SIZE + I2C_handle->dataLen + crcLen;

    /* Data is not sent in Read Command */
    if((I2C_handle->rxMsg.buffer[CTRL_IDX] & CMD_MASK) == READ_CMD)
    {
        I2C_handle->rxMsg.len = CTRL_SIZE + ADDR_SIZE + 0 + crcLen;
    }

    /* Verify Address is in the range */
    uint32_t addr = u32(&I2C_handle->rxMsg.buffer[ADDR_IDX]);
    if( addr < ADDR_RANGE_START || (addr + I2C_handle->dataLen > ADDR_RANGE_END) )
    {
        I2C_handle->error = ERROR_TYPE_ADDR_RANGE;
        return;
    }

    /* Verify CRC16 */
    if( I2C_handle->isCrc )
    {
        uint8_t size = CTRL_SIZE + ADDR_SIZE + I2C_handle->dataLen;
        if((I2C_handle->rxMsg.buffer[CTRL_IDX] & CMD_MASK) == READ_CMD)
        {
            size = CTRL_SIZE + ADDR_SIZE + 0;
        }

        uint16_t checkSum = CRC_calc16((&I2C_handle->rxMsg.buffer[CTRL_IDX]),size);
        uint16_t expectedCheckSum 
         = u16(&I2C_handle->rxMsg.buffer[I2C_handle->rxMsg.len - CRC_SIZE]);

        if( checkSum != expectedCheckSum )
        {
            I2C_handle->error = ERROR_TYPE_MISMATCH_CRC;
            return;
        }
    }

    I2C_handle->error = ERROR_TYPE_NONE;
    return;
}

static void I2C_processRxBuffer(I2C_Instance *I2C_handle)
{
    if( I2C_handle->status == I2C_STATUS_PROCESS_FRAME )
    {
        I2C_verifyBuffer(I2C_handle);

        /* Service Command if Command is proper */
        if( I2C_handle->error == ERROR_TYPE_NONE )
        {
            I2C_serviceCommand(I2C_handle);
        }

        else
        {
            /* Send appropriate byte to specify the type of Error */
            I2C_sendErrorType(I2C_handle);
        }

        I2C_handle->status = I2C_STATUS_TX_PREPARED;

        /* Enable DMA Rx Channel for receiving next command */
        DMA_RX_init(I2C_handle);
    }
}

void I2C_serviceCommand(I2C_Instance *I2C_handle)
{
    if((I2C_handle->rxMsg.buffer[CTRL_IDX] & CMD_MASK) == READ_CMD)
    {
        /* Clear TX Buffer */
        I2C_clearBuffer(&I2C_handle->txMsg);

        I2C_readCommand(&I2C_handle->txMsg,I2C_handle->dataLen,
                        u32(&I2C_handle->rxMsg.buffer[ADDR_IDX]));
    }
    else if((I2C_handle->rxMsg.buffer[CTRL_IDX] & CMD_MASK) == WRITE_CMD)
    {
        /* Clear TX Buffer */
        I2C_clearBuffer(&I2C_handle->txMsg);

        I2C_writeCommand(&I2C_handle->txMsg,I2C_handle->dataLen,
                        u32(&I2C_handle->rxMsg.buffer[ADDR_IDX]),&I2C_handle->rxMsg.buffer[DATA_IDX]);
    }

    if(I2C_handle->isCrc)
    {
        /* Calculate CRC16 */
        /* From Start byte to end of data byte */
        uint8_t size = I2C_handle->txMsg.ptr;
        /* Send address of Control Byte */
        uint16_t checkSum = CRC_calc16(&I2C_handle->txMsg.buffer[CTRL_IDX],size);

        /* Add Checksum */
        u8From16(&I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr], checkSum);
        I2C_handle->txMsg.ptr += 2;
    }

    I2C_handle->txMsg.len = I2C_handle->txMsg.ptr;
    I2C_handle->txMsg.ptr = 0;
}

void I2C_sendErrorType(I2C_Instance *I2C_handle)
{
        /* Clear TX Buffer */
        I2C_clearBuffer(&I2C_handle->txMsg);

        /* Add Error Bit and length */
        I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr++] = (1 << ERROR_OFFSET) | (0x0);

        /* Add Error Type */
        I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr++] = I2C_handle->error;

        I2C_handle->txMsg.len = I2C_handle->txMsg.ptr;

}


static void I2C_readCommand(BufferInfo *frame,uint8_t len,uint32_t addr)
{
    /* Add Len in Tx Buffer*/
    frame->buffer[frame->ptr++] = (len - 1);

    while(len > 0)
    {
        uint8_t *ptr;
        ptr = (uint8_t*) addr;
        uint8_t data = *ptr;
        frame->buffer[frame->ptr] = data;
        frame->ptr += 1;
        len -= 1;
        addr += 1;
    }

    return;
}

static void I2C_writeCommand(BufferInfo *frame,uint8_t len,uint32_t addr,uint8_t *data)
{
    uint8_t i = 0;
    /* Add Len in Tx Buffer*/
    frame->buffer[frame->ptr++] = 0;

    while(len > 0)
    {
        uint8_t *ptr;
        ptr = (uint8_t*) addr;
        *ptr = data[i];
        i += 1;
        len -= 1;
        addr += 1;
    }

    frame->buffer[frame->ptr++] = ACK_WRITE;
    return;
}

void DMA_RX_init(I2C_Instance *I2C_handle)
{
    DL_DMA_disableChannel(DMA, DMA_CH_RX_CHAN_ID);

    /* Rx */
    DL_DMA_setSrcAddr(DMA, DMA_CH_RX_CHAN_ID, 
                                    (uint32_t) &I2C_0_INST->SLAVE.SRXDATA);
    DL_DMA_setDestAddr(DMA, DMA_CH_RX_CHAN_ID,(uint32_t) &I2C_handle->rxMsg.buffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH_RX_CHAN_ID, MAX_BUFFER_SIZE);
    DL_DMA_enableChannel(DMA, DMA_CH_RX_CHAN_ID);
}

void DMA_TX_init(I2C_Instance *I2C_handle,uint32_t dataLen)
{
    DL_DMA_disableChannel(DMA, DMA_CH_TX_CHAN_ID);

    /* Tx */
    DL_DMA_setSrcAddr(DMA, DMA_CH_TX_CHAN_ID,(uint32_t)  &I2C_handle->txMsg.buffer[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH_TX_CHAN_ID,
                                         (uint32_t) &I2C_0_INST->SLAVE.STXDATA);
    DL_DMA_setTransferSize(DMA, DMA_CH_TX_CHAN_ID, dataLen);
    DL_DMA_enableChannel(DMA, DMA_CH_TX_CHAN_ID);
}

