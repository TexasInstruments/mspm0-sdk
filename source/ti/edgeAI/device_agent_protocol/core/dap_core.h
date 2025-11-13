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
#ifndef DAP_CORE_H_
#define DAP_CORE_H_

#include "device_agent_protocol/dap_interface.h"
#include "uart_comm_target.h"

/*! @brief Macro to check if a value is in a range */
#define IN_RANGE(value, a, b) ((value) >= (a) && (value) <= (b))

/* Defines */
/*! Min length value for payload field of 1 byte */
#define PAYLOAD_FIELD_1B_MIN_LEN   (0U)
/*! Max length value for payload field of 1 byte */
#define PAYLOAD_FIELD_1B_MAX_LEN   (127U)
/*! Min length value for payload field of 2 bytes */
#define PAYLOAD_FIELD_2B_MIN_LEN   (128U)
/*! Max length value for payload field of 2 bytes */
#define PAYLOAD_FIELD_2B_MAX_LEN   (16383U)
/*! Min length value for payload field of 3 bytes */
#define PAYLOAD_FIELD_3B_MIN_LEN   (16384U)
/*! Max length value for payload field of 3 bytes */
#define PAYLOAD_FIELD_3B_MAX_LEN   (4194303U)
/*! Offset of payload field of 1 byte */
#define PAYLOAD_FIELD_1B_OFFSET    (0U)
/*! Offset of payload field of 2 bytes */
#define PAYLOAD_FIELD_2B_OFFSET    (0x8000)
/*! Offset of payload field of 3 bytes */
#define PAYLOAD_FIELD_3B_OFFSET    (0xC00000)


/* Function Declarations */

/**
 * @brief Decodes and executes the received UART data
 * @param[in] UART_handle    A pointer to UART_Instance
 */
void DAP_processBuffer(UART_Instance *UART_handle);

/**
 * @brief Sends response to Send Data command
 * @param[in] UART_handle    A pointer to UART_Instance
 * @param[in] channel        Channel of the response data
 * @param[in] dataPtr        A pointer to the response data
 * @param[in] dataLen        Length of the response data
 */
void DAP_receivedDataResponse(UART_Instance *UART_handle,Resp_CMD_Channel channel,uint32_t dataLen,uint8_t* dataPtr);

/**
 * @brief Decodes frame length from received UART data
 * @param[in] UART_handle    A pointer to UART_Instance
 */
void DAP_getFrameLength(UART_Instance *UART_handle);

/**
 * @brief Gets the data length in bytes
 * @param[in] dataType       Data type
 * @return    len            Length in bytes
 */
uint8_t getDataLen(dataFormat dataType);

#endif /* DAP_CORE_H_ */
