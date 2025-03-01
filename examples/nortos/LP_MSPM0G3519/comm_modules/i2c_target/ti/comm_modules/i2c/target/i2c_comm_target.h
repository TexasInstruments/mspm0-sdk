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
/*!****************************************************************************
 *  @file       i2c_comm_target.h
 *  @brief      I2C communication Module
 *
 * 
 *  @anchor i2c_comm_target
 *  # Overview
 *
 *  APIs for I2C communication module
 *
 *  <hr>
 ******************************************************************************/

#ifndef I2C_COMM_TARGET_H_
#define I2C_COMM_TARGET_H_

#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

/* FRAME STRUCTURE */

/*  Control Byte */
/*! @brief Control index */
#define CTRL_IDX                    (0)
/*! @brief Control size */
#define CTRL_SIZE                   (1)
/*! @brief Control mask */
#define CMD_MASK                    (0x80)
/*! @brief CRC mask */
#define CRC_MASK                    (0x40)
/*! @brief Length mask */
#define LEN_MASK                    (0x3F)
/*! @brief Write command */
#define WRITE_CMD                   (0x80)
/*! @brief Read command */
#define READ_CMD                    (0x00)

/* Address  */
/*! @brief Address index */
#define ADDR_IDX                    (CTRL_IDX + CTRL_SIZE)
/*! @brief Address size */
#define ADDR_SIZE                   (4)

/*! @brief Address range start */
#define ADDR_RANGE_START            (0x20100000)
/*! @brief Address range end */
#define ADDR_RANGE_END              (0x20307FFF)

/* Data */
/*! @brief Data index */
#define DATA_IDX                    (ADDR_IDX + ADDR_SIZE)
/*! @brief maximum data size */
#define MAX_DATA_SIZE               (64)

/*! @brief Data in Acknowledge frame */
#define ACK_WRITE                   (0xAC)

/* OFFSET is from the end of frame */
/* CRC */
/*! @brief CRC offset */
#define CRC_OFFSET                  (0)
/*! @brief CRC size */
#define CRC_SIZE                    (2)

/*! @brief Maximum buffer size */
#define MAX_BUFFER_SIZE             (CTRL_SIZE + ADDR_SIZE + MAX_DATA_SIZE + CRC_SIZE)
/*! @brief Maximum response size */
#define MAX_RESP_SIZE               (CTRL_SIZE + MAX_DATA_SIZE + CRC_SIZE)
/*! @brief Error offset */
#define ERROR_OFFSET                (7)

/*! @brief Buffer info structure */
typedef struct{
    /*! Buffer array */
    _Alignas(uint32_t) uint8_t buffer[MAX_BUFFER_SIZE];
    /*! Pointer */
    uint8_t ptr;
    /*! Length */
    uint8_t len;
} BufferInfo;

/*! @brief I2C status */
typedef enum
{
    /*! I2C IDLE state */
    I2C_STATUS_IDLE = 0,
    /*! I2C Rx Buffering state */
    I2C_STATUS_RX_BUFFERING,
    /*! I2C process frame state */
    I2C_STATUS_PROCESS_FRAME,
    /*! I2C TX prepared state */
    I2C_STATUS_TX_PREPARED
} I2C_Status;

/*! @brief I2C ERROR types */
typedef enum
{
    /*! No error */
    ERROR_TYPE_NONE          = 0x00,
    /*! Mismatch CRC error */
    ERROR_TYPE_MISMATCH_CRC  = 0xE1,
    /*! Error in Address range */
    ERROR_TYPE_ADDR_RANGE    = 0xE2,
    /*! Error */
} ErrorType;

/*! @brief I2C instance */
typedef struct{
    /*! Receive message */
    BufferInfo rxMsg;
    /*! Transmit message */
    BufferInfo txMsg;

    /*! Data length */
    uint8_t dataLen;
    /*! CRC enabled */
    _Bool isCrc;

    /*! I2C status */
    I2C_Status  status;
    /*! I2C error type */
    ErrorType   error;
}I2C_Instance;

/**
 * @brief     Initializes the I2C module
 * @param[in] I2C_handle    A pointer to I2C_Instance
 */
void I2C_init(I2C_Instance *I2C_handle);

/**
 * @brief     Checks the Rx Buffer for commands
 * @param[in] I2C_handle    A pointer to I2C_Instance
 */
void I2C_checkForCommand(I2C_Instance *I2C_handle);

/**
 * @brief     Initializes DMA to store the command in RX Buffer
 * @param[in] I2C_handle    A pointer to I2C_Instance
 */
void DMA_RX_init(I2C_Instance *I2C_handle);

/**
 * @brief     Initializes DMA to send the response stored in TX Buffer
 * @param[in] I2C_handle    A pointer to I2C_Instance
 * @param[in] dataLen       Length of the data to be sent
 */
void DMA_TX_init(I2C_Instance *I2C_handle,uint32_t dataLen);

#ifdef __cplusplus
}
#endif

#endif /* I2C_COMM_TARGET_H_ */
