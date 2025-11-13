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
 * MgmtLink.cpp
 *
 * Created on: Mar 30, 2021
 *     Author: aw@pionier-manufaktur.de
 *	   Modified: Texas Instruments Incorporated, 2025
*/

#include "MgmtLink.h"
#include "includes/EVConfig.h"

#include "pb/nanopb/pb_decode.h"
#include "pb/nanopb/pb_encode.h"

static size_t cobsEncode(const void *data, size_t length, uint8_t *buffer)
{
    uint8_t *encode = buffer;    // Encoded byte pointer
    uint8_t *codep  = encode++;  // Output code pointer
    uint8_t code    = 1;         // Code value

    for (const uint8_t *byte = (const uint8_t *) data; length--; ++byte) {
        if (*byte)  // Byte not zero, write it
            *encode++ = *byte, ++code;

        if (!*byte ||
            code == 0xff) {  // Input is zero or block completed, restart
            *codep = code, code = 1, codep = encode;
            if (!*byte || length) ++encode;
        }
    }
    *codep = code;  // Write final code value

    // add final 0
    *encode++ = 0x00;

    return encode - buffer;
}

// Custom CRC function to output 32b CRC from sequence of byte inputs
uint32_t DL_CRC_calculateBlock8(
    CRC_Regs *crc, uint32_t seed, const uint8_t *ptr, uint32_t size)
{
    uint32_t i;

    DL_CRC_setSeed32(crc, seed);

    for (i = 0; i < size; i++) {
        DL_CRC_feedData8(crc, ptr[i]);
    }

    return (DL_CRC_getResult32(crc));
}

static inline int32_t calculateCrc(uint8_t *buf, size_t len)
{
    return DL_CRC_calculateBlock8(CRC, CRC_SEED, buf, len);
}

MgmtLink::MgmtLink(UART_Regs *_uart) : uart(_uart)
{
    cobsDecodeReset();
    still_sending = false;
    rx_write_idx  = 0;
    rx_read_idx   = 0;
}

void MgmtLink::init()
{
    setRxDMA();
}

void MgmtLink::setRxDMA()
{
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&UART0->RXDATA));
    DL_DMA_setDestAddr(
        DMA, DMA_CH1_CHAN_ID, (uint32_t) &uart_rx_buf[uart_rx_idx]);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, 1);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
}

void MgmtLink::setTxDMA()
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &encoded_tx_buf[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&UART0->TXDATA));
    // adjust DMA transfer size to encoded packet length
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, tx_encode_len);

    // Encoded data ready, enable DMA to send to UART_TX_FIFO
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

void MgmtLink::TxDone()
{
    still_sending = false;
}

void MgmtLink::cobsDecodeReset()
{
    code   = 0xff;
    block  = 0;
    decode = rx_queue[rx_write_idx].rx_packet_buf;
}

// returns true if processed byte was the stop byte (0x00)
bool MgmtLink::processByte()
{
    bool packet_done = false;

    // if end of packet, reset DMA
    if (cobsDecode(uart_rx_buf[uart_rx_idx])) {
        packet_done = true;
        uart_rx_idx = 0;
        // else increment DMA dest to next addr in RX buffer
    } else {
        uart_rx_idx++;
        if ((uart_rx_idx + 1) >= COBS_BUFFER_SIZE) {
            uart_rx_idx = 0;
        }
    }
    setRxDMA();
    return packet_done;
}

// returns true if decoded byte was the stop byte (0x00)
bool MgmtLink::cobsDecode(uint8_t byte)
{
    bool done = false;

    // check max length
    if ((decode - rx_queue[rx_write_idx].rx_packet_buf == COBS_BUFFER_SIZE) &&
        byte != 0x00) {
        cobsDecodeReset();
    }

    if (block) {
        // we're currently decoding and should not get a 0
        if (byte == 0x00) {
            // probably found some garbage -> reset
            cobsDecodeReset();
            return done;
        }
        *decode++ = byte;

        // determine distance to next encoded 0 byte
    } else {
        if (code != 0xff) {
            *decode++ = 0;
        }
        block = code = byte;
        if (code == 0x00) {
            // we're finished, reset everything and commit
            if (decode == rx_queue[rx_write_idx].rx_packet_buf) {
                // we received nothing, just a 0x00
            } else {
                // set back decode with one, as it gets post-incremented
                size_t raw_len =
                    decode - 1 - rx_queue[rx_write_idx].rx_packet_buf;
                if (raw_len >= COBS_MIN_PAYLOAD_SIZE &&
                    raw_len <= TX_MAX_PAYLOAD_SIZE + 4) {
                    done                       = true;
                    rx_queue[rx_write_idx].len = raw_len;
                    rx_read_idx                = rx_write_idx;
                    rx_write_idx++;

                    if (rx_write_idx >= RX_QUEUE_SIZE) rx_write_idx = 0;
                }
            }
            cobsDecodeReset();
            return done;
        }
    }
    block--;
    return done;
}

bool MgmtLink::read(EverestToMcu *msg)
{
    uint8_t *rx_packet_buf = &(rx_queue[rx_read_idx].rx_packet_buf[0]);
    size_t rx_packet_len   = rx_queue[rx_read_idx].len;

    uint32_t expected_crc = calculateCrc(rx_packet_buf, rx_packet_len - 4);
    uint32_t received_crc =
        ((uint32_t) rx_packet_buf[rx_packet_len - 1] << 24) |
        ((uint32_t) rx_packet_buf[rx_packet_len - 2] << 16) |
        ((uint32_t) rx_packet_buf[rx_packet_len - 3] << 8) |
        ((uint32_t) rx_packet_buf[rx_packet_len - 4]);

    if (expected_crc != received_crc) {
        // crc mismatch, drop packet
        return false;
    } else {
        rx_packet_len = rx_packet_len - 4;
    }

    pb_istream_t istream =
        pb_istream_from_buffer(rx_packet_buf, rx_packet_len);

    bool status = pb_decode(&istream, EverestToMcu_fields, msg);

    return status;
}

bool MgmtLink::write(McuToEverest *msg)
{
    while (still_sending) {
        delay_cycles(100);
    }

    pb_ostream_t ostream =
        pb_ostream_from_buffer(tx_packet_buf, TX_MAX_PAYLOAD_SIZE);

    bool status = pb_encode(&ostream, McuToEverest_fields, msg);

    if (!status) {
        // couldn't encode
        return false;
    }

    size_t tx_payload_len = ostream.bytes_written;

    // generate crc32
    uint32_t crc = calculateCrc(&tx_packet_buf[0], (uint32_t) tx_payload_len);

    // add crc32 to end of tx buffer
    for (int byte_pos = 0; byte_pos < 4; ++byte_pos) {
        tx_packet_buf[tx_payload_len] = (uint8_t) crc & 0xFF;
        crc                           = crc >> 8;
        tx_payload_len++;
    }

    // COBS encode the tx buffer
    tx_encode_len = cobsEncode(tx_packet_buf, tx_payload_len, encoded_tx_buf);

    if (tx_encode_len > COBS_BUFFER_SIZE) {
        // buffer overflow
        return false;
    }

    setTxDMA();
    still_sending = true;

    return true;
}
