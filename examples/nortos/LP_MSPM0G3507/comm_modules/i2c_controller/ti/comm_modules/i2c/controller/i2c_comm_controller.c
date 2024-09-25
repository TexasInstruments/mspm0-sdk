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

#include "i2c_comm_controller.h"

#define LEFT_SHIFT_8(x)     ((x) << 8)
#define RIGHT_SHIFT_8(x)    ((x) >> 8)


/* Function Declarations */
/**
 * @brief     Calculates CRC 16 for an array of 8-bit data
 * @param[in] ptr   Pointer to the data to compute CRC
 * @param[in] size  size of the array of data in bytes
 * @return    Returns the CRC value
 */
static uint16_t CRC_calc16(uint8_t* ptr,uint8_t size);

/**
 * @brief     Prepares I2C Tx FIFO and issues I2C write command
 * @param[in] I2C_handle     Pointer to I2C_Instance
 * @param[in] targetAddr     I2C Target Address
 */
static void I2C_sendBuffer(I2C_Instance *I2C_handle,uint8_t targetAddr);


/* Helper Functions */
/**
 * @brief     32 bit to 8 bit
 * @param[in] b  pointer to unsigned 8 bit output
 * @param[in] u32  unsigned 32 bit input
 */
static void u8From32(uint8_t *b, uint32_t u32)
{
    b[0] = (uint8_t)u32;
    b[1] = (uint8_t)(u32 = RIGHT_SHIFT_8(u32));
    b[2] = (uint8_t)(u32 = RIGHT_SHIFT_8(u32));
    b[3] = (uint8_t)(u32 = RIGHT_SHIFT_8(u32));
}

/**
 * @brief     16 bit to 8 bit
 * @param[in] b  pointer to unsigned 8 bit output
 * @param[in] u16  unsigned 16 bit input
 */
static void u8From16(uint8_t *b, uint16_t u16)
{
    b[0] = (uint8_t)u16;
    b[1] = (uint8_t)(u16 = RIGHT_SHIFT_8(u16));
}

/**
 * @brief     8 bit to 32 bit
 * @param[in] b  pointer to unsigned 8 bit output
 * @return    32 bit result
 */
static uint32_t u32(uint8_t *b)
{
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
static uint16_t u16(uint8_t *b)
{
  uint16_t u;
  u = b[1];
  u = LEFT_SHIFT_8(u) + b[0];
  return u;
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

/**
 * @brief     Resets pointer and length of the buffer
 * @param[in] frame  Pointer to BufferInfo (Rx,Tx)
 */
__STATIC_INLINE void I2C_clearBuffer(BufferInfo *frame)
{
    frame->ptr = 0;
    frame->len = 0;
}

void I2C_init(I2C_Instance *I2C_handle)
{
    I2C_clearBuffer(&I2C_handle->rxMsg);
    I2C_clearBuffer(&I2C_handle->txMsg);
 
    I2C_handle->dataLen = 0;
    I2C_handle->isCrc = false;
    I2C_handle->status = I2C_STATUS_IDLE;
    I2C_handle->error = ERROR_TYPE_NONE;
}

void I2C_sendCommand(I2C_Instance *I2C_handle,I2C_CommandInfo *command)
{
    /* Prepare TX Buffer */
    I2C_handle->txMsg.ptr = 0;

    /* Control Byte */
    I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr++] = (command->commandType) | (command->crcEnable ? CRC_MASK : (0x00)) | ((command->dataSize - 1) & LEN_MASK) ;

    u8From32(&I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr],command->addr);
    I2C_handle->txMsg.ptr += 4;

    /* For Read Command */
    I2C_handle->dataLen = command->dataSize;
    
    /* For Write Command */
    if(command->commandType == WRITE_COMMAND)
    {
        I2C_handle->dataLen = 1;
        
        for(int i = 0 ; i < command->dataSize ; i++)
        {
            I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr++] = command->dataArray[i];
        }
    }

    /* CRC */
    I2C_handle->isCrc = false;
    if(command->crcEnable)
    {
        I2C_handle->isCrc = true;
        uint16_t Crc = CRC_calc16((&I2C_handle->txMsg.buffer[0]),I2C_handle->txMsg.ptr);
        u8From16(&I2C_handle->txMsg.buffer[I2C_handle->txMsg.ptr],Crc);
        I2C_handle->txMsg.ptr += 2;
    }


    I2C_handle->txMsg.len = I2C_handle->txMsg.ptr;

    I2C_sendBuffer(I2C_handle, command->targetAddr);

    /* Wait until I2C Controller is idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
}

static void I2C_sendBuffer(I2C_Instance *I2C_handle,uint8_t targetAddr)
{
    I2C_handle->txMsg.ptr = 0;

    I2C_handle->txMsg.ptr = DL_I2C_fillControllerTXFIFO(I2C_INST, &I2C_handle->txMsg.buffer[0], I2C_handle->txMsg.len);

    /* Enable TXFIFO trigger interrupt if there are more bytes to send */
    if (I2C_handle->txMsg.ptr < I2C_handle->txMsg.len) 
    {
        DL_I2C_enableInterrupt(
            I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else
    {
        DL_I2C_disableInterrupt(
            I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }


    I2C_handle->status = I2C_STATUS_TX_STARTED;

    /* Wait until I2C Controller is idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_startControllerTransfer(
        I2C_INST, targetAddr, DL_I2C_CONTROLLER_DIRECTION_TX, I2C_handle->txMsg.len);

    /* Wait until the Controller sends all bytes */
    while ((I2C_handle->status != I2C_STATUS_TX_COMPLETE) &&
            (I2C_handle->status != I2C_STATUS_ERROR)) {
        __WFE();
    }

    /* Wait until I2C bus is Idle */
    while (DL_I2C_getControllerStatus(I2C_INST) &
            DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        __BKPT(0);
    }
}


void I2C_getResponse(I2C_Instance* I2C_handle,uint32_t targetAddr)
{
    uint32_t expectedLen = CTRL_SIZE + I2C_handle->dataLen + (I2C_handle->isCrc ? CRC_SIZE : 0);

    DL_I2C_startControllerTransfer(
        I2C_INST, targetAddr, DL_I2C_CONTROLLER_DIRECTION_RX, expectedLen);

    /* Wait for all bytes to be received in interrupt */
    while (I2C_handle->status != I2C_STATUS_RX_COMPLETE) 
    {
        /* Resend the Read Command if it is N'ACKed */
        if(I2C_handle->status == I2C_STATUS_ERROR)
        {
            I2C_handle->status = I2C_STATUS_RX_STARTED;
            delay_cycles(10000);
            DL_I2C_startControllerTransfer(
                I2C_INST, targetAddr, DL_I2C_CONTROLLER_DIRECTION_RX, expectedLen);
        }
    }

    /* Wait until I2C bus is Idle */
    while (DL_I2C_getControllerStatus(I2C_INST) &
            DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
}

void I2C_decodeResponse(I2C_Instance *I2C_handle,I2C_ResponseInfo *response)
{
    response->received = true;

    /* Length stored in frame is 1 less than actual length */
    response->dataSize = ( I2C_handle->rxMsg.buffer[CTRL_IDX] & LEN_MASK ) + 1;

    response->status = ERROR_TYPE_NONE;

    if(I2C_handle->rxMsg.buffer[CTRL_IDX] & ERROR_MASK)
    {
        /* Store error code sent by target */
        response->status = (ErrorType) I2C_handle->rxMsg.buffer[RESP_DATA_IDX];
    }

    response->frame.ctrl = I2C_handle->rxMsg.buffer[CTRL_IDX];

    for(int i = 0; i < MAX_DATA_SIZE; i++)
    {
        if(i < response->dataSize)
        {
            response->frame.data[i] = I2C_handle->rxMsg.buffer[RESP_DATA_IDX + i];
        }
        /* Resetting extra space to zero*/
        else
        {
            response->frame.data[i] = 0;
        }
    }

    if(I2C_handle->isCrc)
    {
        response->frame.crc = u16(&I2C_handle->rxMsg.buffer[I2C_handle->rxMsg.len - CRC_SIZE]);
    }
}
