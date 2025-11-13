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
/*!****************************************************************************
 *  @file       uart_comm_target.h
 *  @brief      UART communication Module
 *
 * 
 *  @anchor uart_comm_target
 *  # Overview
 *
 *  APIs for UART communication module
 *
 *  <hr>
 ******************************************************************************/

#ifndef UART_COMM_TARGET_H_
#define UART_COMM_TARGET_H_

#include "ti_msp_dl_config.h"
#include "device_agent_protocol/dap_interface.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FRAME STRUCTURE */
/* SOF */
/*! @brief Start index */
#define START_IDX                   (0)
/*! @brief Start byte size */
#define START_BYTE_SIZE             (1)
/*! @brief Frame start byte */
#define FRAME_START_BYTE            (0xED)
/*! @brief Preamble byte size */
#define PREAMBLE_BYTE_SIZE          (1)

/* Command Byte */
/*! @brief Command index */
#define CMD_IDX                     (START_IDX + START_BYTE_SIZE)
/*! @brief Command size */
#define CMD_SIZE                    (1)

/*! @brief Header size */
#define HEADER_SIZE                 (START_BYTE_SIZE + CMD_SIZE)

/* Length Byte */
/*! @brief Length Byte index */
#define LEN_IDX                     (CMD_IDX + CMD_SIZE)
/*! @brief Max Length Byte size */
#define MAX_LEN_IDX                 (3)

/* Payload */
/*! @brief Payload index */
#define PAYLOAD_IDX                 (LEN_IDX + MAX_LEN_IDX)


/* EOF */
/*! @brief End offset */
#define END_OFFSET                  (1)
/*! @brief Frame end byte */              
#define FRAME_END_BYTE              (0x9E)
/*! @brief End byte size */
#define END_BYTE_SIZE               (1)


/* TRAILER */
/*! @brief Trailer size */
#define TRAILER_SIZE                (END_BYTE_SIZE)

/*! @brief Packet Fixed part size */
#define PACKET_FIXED_PART_SIZE      (4)

/*! @brief DMA receive transfer size */
#define DMA_RX_TRANSFER_SIZE        (64 + PACKET_FIXED_PART_SIZE)

/*! @brief Maximum buffer size */
#define MAX_BUFFER_SIZE             (600)

/*! @brief CRC Initial Value as per device agent protocol spec */
#define CRC_INIT_VALUE              (0x66)

/*! @brief Buffer info structure */
typedef struct{
    /*! Buffer array */    
    _Alignas(uint32_t) uint8_t buffer[MAX_BUFFER_SIZE];
    /*! Pointer */
    uint32_t ptr;
    /*! Length */
    uint32_t len;
} BufferInfo;

/*! @brief UART status */
typedef enum 
{
    /*! UART IDLE state */
    UART_STATUS_IDLE = 0,
    /*! UART RX buffering state */
    UART_STATUS_RX_BUFFERING,
    /*! UART process frame state */
    UART_STATUS_PROCESS_FRAME,
    /*! UART TX transmitting state */
    UART_STATUS_TX_TRANSMITTING,
} UART_Status;

/*! @brief UART ERROR types 
 * @note Error numbers are not defined in DAP v1.0
 *       - Update the Enum accordingly once the error numbers are defined
 */
typedef enum
{
    /*! No error */
    ERROR_TYPE_NONE          = 0x00,
    /*! Mismatch Start byte error */
    ERROR_TYPE_MISMATCH_START_BYTE = 0xE1,
    /*! Mismatch end byte error */
    ERROR_TYPE_MISMATCH_END_BYTE = 0xE2,
    /*! Mismatch CRC error */
    ERROR_TYPE_MISMATCH_CRC = 0xE3,
} ErrorType;

/*! @brief UART instance */
typedef struct
{
    /*! Transmit message */
    BufferInfo txMsg;
    /*! Receive message */
    BufferInfo rxMsg;

    /*! Data length */
    uint8_t dataLen;
    /*! CRC enabled */
    _Bool isCrc;

    /*! UART Tx status */
    volatile UART_Status TxStatus;
    /*! UART Rx status */
    volatile UART_Status RxStatus;
    /*! UART error type */
    ErrorType error;
} UART_Instance;


/**
 * @brief     Initializes the UART module
 * @param[in] UART_handle    A pointer to UART_Instance
 */
void UART_init(UART_Instance *UART_handle);

/**
 * @brief     Checks the Rx Buffer for commands
 * @param[in] UART_handle    A pointer to UART_Instance
 */
void UART_checkForCommand(UART_Instance *UART_handle);

/**
 * @brief     Initializes DMA to store the command in RX Buffer
 * @param[in] UART_handle  Pointer to UART_Instance
 */
void DMA_RX_init(UART_Instance *UART_handle);

/**
 * @brief     Clears the Buffer
 * @param[in] frame  Pointer to BufferInfo
 */
void UART_clearBuffer(BufferInfo *frame);

/**
 * @brief     Initializes DMA to send the response stored in Tx Buffer
 * @param[in] frame  Pointer to BufferInfo (Tx)
 */
void UART_sendBuffer(BufferInfo *frame);


/**
 * @brief Computes 8-bit CRC on the input message array
 *
 * @param   ptr   Buffer which holds the input message (array of bytes)
 * @param   len   Length of the data in the Buffer (array)
 * @return  crc   8-bit CRC calculated on the input message, 
 * CCITT Polynomial x^8 + x^2 + x + 1 
 */

uint8_t CRC_calc8(uint8_t* ptr,uint16_t len);

/**
 * @brief     Resets pointer and length of the buffer to synchronize with controller
 * @param[in] UART_handle  Pointer to UART_Instance
*/
void UART_reSync(UART_Instance* UART_handle);

#ifdef __cplusplus
}
#endif
#endif /* UART_COMM_TARGET_H_ */
