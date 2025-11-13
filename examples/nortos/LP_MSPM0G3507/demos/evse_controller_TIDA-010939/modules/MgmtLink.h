/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * MgmtLink.h
 *
 *  Created on: Mar 30, 2021
 *      Author: aw@pionier-manufaktur.de
 *		Modified: Texas Instruments Incorporated, 2025
*/

#ifndef SRC_EVDRIVERS_MGMTLINK_H_
#define SRC_EVDRIVERS_MGMTLINK_H_

//#include "cmsis_os.h"

#include <queue>
#include "pb/protobuf/tida010939.pb.h"
#include "ti_msp_dl_config.h"

#define COBS_BUFFER_SIZE 256 /* this will give 254 bytes of payload */
#define COBS_PAYLOAD_SIZE (COBS_BUFFER_SIZE - 2)
#define COBS_MIN_PAYLOAD_SIZE (4 + 1) /* 4 bytes for crc32 + min one byte */
#define TX_MAX_PAYLOAD_SIZE (COBS_PAYLOAD_SIZE - 4) /* 4 bytes for crc32 */
#define RX_QUEUE_SIZE 3

typedef struct _MgmtLinkMessage {
    uint8_t rx_packet_buf[COBS_BUFFER_SIZE];
    size_t len;
} MgmtLinkMessage;

class MgmtLink
{
   public:
    MgmtLink(UART_Regs *_uart);
    void init();

    // Fills up msg and returns status of decoding
    // NOTE: if decoding fails, msg might be still altered!
    bool read(EverestToMcu *msg);

    // Encodes a McuToEverest message and sends it out
    // If the message cannot be encoded, it will return false
    // NOTE: if sending fails due to a previous sending
    //       is still on-going, it also will return false
    // FIXME: right now, we would need to poll in order to
    //        see, if we can send and we can not distinguish
    //        between a encoding failure ..
    bool write(McuToEverest *msg);

    void setRxDMA();
    void setTxDMA();
    void TxDone();

    bool processByte();

   private:
    UART_Regs *uart;

    // rx related
    volatile uint8_t
        uart_rx_buf[COBS_BUFFER_SIZE];  // UART RX DMA destination register
    uint8_t uart_rx_idx = 0;
    MgmtLinkMessage rx_queue
        [RX_QUEUE_SIZE];  // queue to hold RX_QUEUE_SIZE complete messages
    uint8_t rx_write_idx;
    uint8_t rx_read_idx;
    size_t tx_encode_len;

    // tx related
    uint8_t tx_packet_buf[COBS_BUFFER_SIZE];
    uint8_t encoded_tx_buf[COBS_BUFFER_SIZE];
    volatile bool still_sending;

    // cobs decoding related
    uint8_t code;
    uint8_t block;
    uint8_t *decode;

    // reset cobs decode state
    void cobsDecodeReset();

    // decode message
    bool cobsDecode(uint8_t byte);
};

#endif  // SRC_EVDRIVERS_MGMTLINK_H_
