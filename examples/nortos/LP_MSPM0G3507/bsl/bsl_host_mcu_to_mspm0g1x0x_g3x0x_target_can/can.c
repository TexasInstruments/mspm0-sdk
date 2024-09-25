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

#include <bsl_can.h>
#include <can.h>
#include <ti_msp_dl_config.h>

DL_MCAN_InitParams new_gMCAN0InitParams;
DL_MCAN_BitTimingParams new_gMCAN0BitTimes;

volatile RX_states RX_state;
volatile bool frame_send_success          = false;
static const uint16_t canfd_dlc_lengths[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
const uint16_t canfd_dlc_size = 16;

CANFD_baudrate_config br_cfg = {
    .fdMode             = 1,
    .brsEnable          = 1,
    .nomTimeSeg1        = 33,
    .nomTimeSeg2        = 4,
    .nomSynchJumpWidth  = 4,
    .nomRatePrescalar   = 0,
    .dataTimeSeg1       = 4,
    .dataTimeSeg2       = 1,
    .dataSynchJumpWidth = 0,
    .dataRatePrescalar  = 0,
};
volatile uint16_t BSL_maxBufferSize = (uint16_t) 64;

/*
 * Encodes the Integer value into binary format for DLC field
 */
uint16_t encode_dlc(uint16_t len)
{
    if (len <= 8)
        return len;
    else {
        for (int i = canfd_dlc_size - 1; i > 8; i--) {
            if (canfd_dlc_lengths[i] == len) {
                return i;
            }
        }
    }
    return 0xFFFF;
}

/*
 * Decodes the binary format into Integer value for DLC field
 */

uint16_t decode_dlc(DL_MCAN_RxBufElement *rxMsg)
{
    return canfd_dlc_lengths[rxMsg->dlc];
}

uint32_t get_CAN_TDCO(DL_MCAN_BitTimingParams *dbtp)
{
    uint32_t tdco = (dbtp->dataTimeSeg1 + dbtp->dataTimeSeg2 + 3);
    tdco *= (dbtp->dataRatePrescalar + 1);
    tdco /= 2;
    return tdco;
}

void MCAN_reconfig(void)
{
    /* Put MCAN in SW initialization mode. */
    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_SW_INIT);

    /* Wait till MCAN is not initialized. */
    while (DL_MCAN_OPERATION_MODE_SW_INIT != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    /* Initialize MCAN module. */
    DL_MCAN_init(MCAN0_INST, (DL_MCAN_InitParams *) &new_gMCAN0InitParams);

    /* Configure Bit timings. */
    DL_MCAN_setBitTime(
        MCAN0_INST, (DL_MCAN_BitTimingParams *) &new_gMCAN0BitTimes);

    /* Take MCAN out of the SW initialization mode */
    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_NORMAL);
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CANFD0))
        ;

    /* Enable MCAN module Interrupts */
    DL_MCAN_enableIntr(MCAN0_INST, MCAN0_INST_MCAN_INTERRUPTS, 1U);

    DL_MCAN_selectIntrLine(
        MCAN0_INST, DL_MCAN_INTR_MASK_ALL, DL_MCAN_INTR_LINE_NUM_1);
    DL_MCAN_enableIntrLine(MCAN0_INST, DL_MCAN_INTR_LINE_NUM_1, 1U);

    /* Enable MSPM0 MCAN interrupt */
    DL_MCAN_clearInterruptStatus(MCAN0_INST, (DL_MCAN_MSP_INTERRUPT_LINE1));
    DL_MCAN_enableInterrupt(MCAN0_INST, (DL_MCAN_MSP_INTERRUPT_LINE1));

    return;
}

bool MCAN_send_frame(uint32_t id, uint8_t *data, uint16_t len)
{
    frame_send_success = false;
    uint32_t intr_status;
    DL_MCAN_TxBufElement txMsg;
    DL_MCAN_TxFIFOStatus txfifoStatus;

    /* Initialize message to transmit. */
    /* Identifier Value. */
    txMsg.id = id;
    /* Transmit data frame. */
    txMsg.rtr = 0U;
    /* 11-bit standard identifier. */
    txMsg.xtd = 0U;
    /* ESI bit in CAN FD format depends only on error passive flag. */
    txMsg.esi = 0U;
    /* Transmitting 4 bytes. */
    txMsg.dlc = encode_dlc(len);
    /* CAN FD frames transmitted with bit rate switching. */
    txMsg.brs = protocol_mode;
    /* Frame transmitted in CAN FD format. */
    txMsg.fdf = protocol_mode;  //protocol_mode;
    /* Store Tx events. */
    txMsg.efc = 1U;
    /* Message Marker. */
    txMsg.mm = 0xAAU;
    /* Data bytes. */
    for (int i = 0; i < len; i++) txMsg.data[i] = data[i];
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CANFD0))
        ;

    /* Write Tx Message to the Message RAM (FIFO). */
    DL_MCAN_writeMsgRam(CANFD0, DL_MCAN_MEM_TYPE_FIFO, 0, &txMsg);

    /* Get put index and other TxFIFO details in txfifoStatus*/
    DL_MCAN_getTxFIFOQueStatus(CANFD0, &txfifoStatus);

    /* Enable Transmission interrupt.*/
    DL_MCAN_TXBufTransIntrEnable(CANFD0, txfifoStatus.putIdx, 1U);

    /* Add request for transmission. */
    DL_MCAN_TXBufAddReq(CANFD0, txfifoStatus.putIdx);

    /* Wait till ACK bit is written by receiver on Bus*/
    if (txMsg.id == MCAN_HOST_ID) {
        while (frame_send_success == false) {
            ;
        }
    }
    delay_cycles(DELAY_CAN_FRAME_TX);

    return true;
}

void frame_fragmentation(uint16_t len, volatile uint8_t *frame_fragments)
{
    int f = 0;
    while (len > 0) {
        for (int i = canfd_dlc_size - 2; i > 0; i--) {
            if (len >= canfd_dlc_lengths[i]) {
                len -= canfd_dlc_lengths[i];
                frame_fragments[f++] = canfd_dlc_lengths[i];
                break;
            }
        }
    }
}

uint8_t MCAN_send_BSL_packet(volatile uint8_t *BSL_packet, uint16_t size)
{
    volatile uint16_t total_count = 0;
    uint16_t frame_length;
    volatile uint16_t bytes_to_send;
    uint8_t frame_fragments[3] = {0, 0, 0};
    uint8_t *fragment_start;
    bytes_to_send = size;
    if (protocol_mode == CAN_MODE)
        frame_length = 8;
    else
        frame_length = 64;

    fragment_start = (uint8_t *) &BSL_packet[0];

    while (bytes_to_send >= frame_length) {
        MCAN_send_frame(MCAN_HOST_ID, fragment_start, frame_length);
        total_count++;
        fragment_start += frame_length;
        bytes_to_send -= frame_length;
    }
    frame_fragmentation(bytes_to_send, &frame_fragments[0]);

    for (int i = 0; i < 3 && (frame_fragments[i] != 0); i++) {
        MCAN_send_frame(MCAN_HOST_ID, fragment_start, frame_fragments[i]);
        fragment_start += frame_fragments[i];
    }
    return 0;
}

uint8_t interpret_bsl_response()
{
    if (BSL_RX_buffer[CORE_INDEX] == BSL_RESP_MESSAGE) {
        msg_error_status = BSL_RX_buffer[4];
        if (msg_error_status != BSL_OPERATION_SUCCESFUL)
            return msg_error_status;
    } else if (BSL_RX_buffer[CORE_INDEX] == BSL_RESP_GET_IDENTITY) {
        BSL_maxBufferSize =
            *(uint16_t *) &BSL_RX_buffer[BSL_HED_LEN_CMD_SIZE +
                                         BLS_DEVICE_INFO_SIZE - 14];
    }
    return msg_error_status;
}

uint8_t receive_bsl_response()
{
    bool require_msg_response = true;
    while (1) {
        /*
         * If the last received response packet is processed,
         * the state will move from BLOCKED to IDLE.
         */
        if (RX_state == RX_blocked) {
            RX_state = RX_idle;
        }
        if ((active_BSL_command == CMD_CONNECTION) ||
            (active_BSL_command == CMD_START_APPLICATION) ||
            (active_BSL_command == CMD_CHANGE_BAUDRATE))
            require_msg_response = false;

        if (RX_state == RX_acknowledged && !require_msg_response) {
            RX_state = RX_idle;
            return ack_error_status;
        }

        if (dataPointer == (uint32_t) BSL_PACK_HEADER_IDX) {
            if (RxDataCnt > (uint16_t) BSL_PACK_HEADER_IDX) {
                /* Check if first byte in the packet matches the Header byte */
                if (BSL_RX_buffer[BSL_PACK_HEADER_IDX] != BSL_RESP_HEADER)
                    return BSL_ERROR_HEADER_INCORRECT;
                else
                    dataPointer += 1;
            }
        }
        /* Next two bytes are buffer length */
        else if (dataPointer == (uint32_t) BSL_PACK_LEN_IDX_1) {
            if (RxDataCnt > (uint16_t) BSL_PACK_LEN_IDX_1) {
                BSL_response_size =
                    (uint16_t) BSL_RX_buffer[BSL_PACK_LEN_IDX_1];
                dataPointer = dataPointer + (uint32_t) 1;
            }
        } else if (dataPointer == (uint32_t) BSL_PACK_LEN_IDX_2) {
            if (RxDataCnt > (uint16_t) BSL_PACK_LEN_IDX_2) {
                uint16_t temp = (uint16_t) BSL_RX_buffer[BSL_PACK_LEN_IDX_2]
                                << (uint16_t) 8;
                BSL_response_size |= temp;
                dataPointer = (uint32_t) BSL_response_size + (uint32_t) 3;
            }
        } else if (dataPointer == ((uint32_t) BSL_response_size +
                                      (uint32_t) BSL_BUF_CRC_IDX_1)) {
            if (RxDataCnt >
                (BSL_response_size + (uint16_t) BSL_BUF_CRC_IDX_1)) {
                BSL_resp_checksum =
                    (uint32_t) BSL_RX_buffer[BSL_response_size +
                                             (uint16_t) BSL_BUF_CRC_IDX_1];

                dataPointer += (uint32_t) 1;
            }
        } else if (dataPointer == ((uint32_t) BSL_response_size +
                                      (uint32_t) BSL_BUF_CRC_IDX_2)) {
            if (RxDataCnt >
                (BSL_response_size + (uint16_t) BSL_BUF_CRC_IDX_2)) {
                BSL_resp_checksum =
                    BSL_resp_checksum |
                    (BSL_RX_buffer[BSL_response_size +
                                   (uint16_t) BSL_BUF_CRC_IDX_2]
                        << 8);
                dataPointer = dataPointer + (uint32_t) 1;
            }
        } else if (dataPointer == ((uint32_t) BSL_response_size +
                                      (uint32_t) BSL_BUF_CRC_IDX_3)) {
            if (RxDataCnt >
                (BSL_response_size + (uint16_t) BSL_BUF_CRC_IDX_3)) {
                BSL_resp_checksum =
                    BSL_resp_checksum |
                    (BSL_RX_buffer[BSL_response_size +
                                   (uint16_t) BSL_BUF_CRC_IDX_3]
                        << 16);
                dataPointer = dataPointer + (uint32_t) 1;
            }
        } else if (dataPointer == ((uint32_t) BSL_response_size +
                                      (uint32_t) BSL_BUF_CRC_IDX_4)) {
            if (RxDataCnt >
                (BSL_response_size + (uint16_t) BSL_BUF_CRC_IDX_4)) {
                BSL_resp_checksum =
                    BSL_resp_checksum |
                    (BSL_RX_buffer[BSL_response_size +
                                   (uint16_t) BSL_BUF_CRC_IDX_4]
                        << 24);
                dataPointer = dataPointer + (uint32_t) 1;
                RX_state    = RX_blocked;

                uint32_t bsl_check_sum = softwareCRC(
                    (uint8_t *) &BSL_RX_buffer[3], BSL_response_size);
                /* Verify checksum */
                if (bsl_check_sum == BSL_resp_checksum) {
                    return interpret_bsl_response();
                } else {
                    msg_error_status = CHECKSUM_FAIL;
                    return msg_error_status;
                }
            }
        }
    }
}

void write_into_buffer(DL_MCAN_RxBufElement *rxMsg, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        BSL_RX_buffer[dataWritePointer] = (uint8_t) rxMsg->data[i];
        dataWritePointer++;
    }
    return;
}

void BSL_PI_MCAN_ISR(void)
{
    bool require_msg_response;
    static DL_MCAN_RxBufElement rxMsg;
    DL_MCAN_RxFIFOStatus rxFS;
    uint16_t mcan_frame_size;
    uint32_t intr_status;
    switch (DL_MCAN_getPendingInterrupt(CANFD0)) {
        case DL_MCAN_IIDX_LINE1:
            intr_status = DL_MCAN_getIntrStatus(CANFD0);
            DL_MCAN_clearIntrStatus(
                CANFD0, intr_status, DL_MCAN_INTR_SRC_MCAN_LINE_1);  // SS
            if ((intr_status & MCAN_IR_TC_MASK) == MCAN_IR_TC_MASK) {
                frame_send_success = true;
            }

            else if ((intr_status & MCAN_IR_RF1N_MASK) == MCAN_IR_RF1N_MASK) {
                rxFS.num = DL_MCAN_RX_FIFO_NUM_1;
                do {
                    DL_MCAN_getRxFIFOStatus(CANFD0, &rxFS);
                } while ((rxFS.fillLvl) == 0);

                DL_MCAN_readMsgRam(
                    CANFD0, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, &rxMsg);
                DL_MCAN_writeRxFIFOAck(CANFD0, rxFS.num, rxFS.getIdx);
                mcan_frame_size = decode_dlc(&rxMsg);

                /*
                * If a Packet is received when RX is in IDLE state, it is treated as
                * a new repsonse packet and buffer is filled with packet
                *
                */

                if ((RX_state == RX_idle) && (rxMsg.dlc == 1)) {
                    if (rxMsg.data[0] == BSL_ACK) {
                        if ((active_BSL_command == CMD_CONNECTION) ||
                            (active_BSL_command == CMD_START_APPLICATION) ||
                            (active_BSL_command == CMD_CHANGE_BAUDRATE))
                            require_msg_response = false;

                        ack_received = true;
                        RX_state     = RX_acknowledged;

                    } else {
                        ack_error_status = rxMsg.data[0];
                        return;
                    }
                } else if (RX_state == RX_acknowledged) {
                    //Frame payload size
                    uint8_t payload_size = decode_dlc(&rxMsg);

                    dataPointer      = (uint32_t) 0;
                    dataWritePointer = (uint32_t) 0;
                    write_into_buffer(&rxMsg, payload_size);
                    RxDataCnt = payload_size;
                    RX_state  = RX_receiving;
                }
                /*
                 * Whenever a new packet is received, after a frame reception is
                 * started, append it to the data buffer
                 */
                else if (RX_state == RX_receiving) {
                    if (RxDataCnt < (BSL_maxBufferSize + (uint16_t) 4)) {
                        write_into_buffer(&rxMsg, mcan_frame_size);
                        RxDataCnt += mcan_frame_size;
                    }
                }

                /* When RX state machine is in any other state (BLOCKED),
                * packet is dropped */
                else {
                }
            }
            break;

        default:
            break;
    }
}

void CAN_initialize()
{
    NVIC_ClearPendingIRQ(MCAN0_INST_INT_IRQN);
    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);
    DL_Interrupt_registerInterrupt(
        ((uint32_t) MCAN0_INST_INT_IRQN), BSL_PI_MCAN_ISR);
    return;
}
