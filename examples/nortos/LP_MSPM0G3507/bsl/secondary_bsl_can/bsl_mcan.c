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
//#include <bsl_can_config.h>

#include "bsl_mcan.h"
#include <stdlib.h>
#include "boot_config.h"

/* Global variables */

volatile uint8_t BSL_Buffer_Status;

volatile uint8_t* BSL_MCAN_Back_Buf;
volatile uint8_t* BSL_MCAN_Active_Buf;
volatile uint8_t* BSL_MCAN_Buf1;
volatile uint8_t* BSL_MCAN_Buf2;

volatile uint16_t BSL_RXBufferSize;
volatile uint16_t BSL_maxBufferSize = 1024;

volatile uint8_t protocol_mode;
volatile bool frame_send_success = false;

volatile const uint16_t canfd_dlc_lengths[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
volatile const uint16_t canfd_dlc_size = 16;

volatile BSL_RX_states BSL_RX_state;
volatile uint32_t BSL_PI_dataPointer;
volatile uint32_t BSL_PI_dataWritePointer;
volatile uint32_t BSL_PI_checksum;
volatile uint16_t BSL_PI_RxDataCnt;
volatile uint16_t BSL_activePluginType;

DL_MCAN_InitParams new_gMCAN0InitParams;
DL_MCAN_BitTimingParams new_gMCAN0BitTimes;

/*
 * Static Function declarations
 */

/*
 * @brief       Interrupt handler for UART interrupts
 */
void BSL_PI_MCAN_ISR(void);

/*
 * @brief       Sends one byte of data
 *
 * @param[in]   data    One byte of data to be transferred
 *
 * @return      none
 */
static void BSL_PI_sendByte(uint8_t data);

/*
 * @brief       Initializes data buffer
 *
 * Initializes the buffer variables and all the global variables
 * needed for Plugin operation.
 *
 * @param[in]   buffer      Pointer to the SRAM data buffer sent from the
 *                          BSL core.
 * @param[in]   bufferSize  1/2 of the SRAM memory size available to use as
 *                          data buffer.
 * @return      none
 */
void BSL_initBuffers(uint8_t* buffer, uint16_t bufferSize);

/*
 * @brief       Calculates the CRC32 for the given data
 *
 * This function calculates the CRC32 for the given data using the CRC module
 * present in the device. It doesn't need to configure CRC, since it would have
 * been already taken care by ROM BSL.
 *
 * ROM BSL configures CRC modules with following specifications:
 * 1. CRC32-ISO3309 polynomial
 * 2. Bit reversed configuration
 * 3. Initial seed - 0xFFFFFFFF
 *
 * @param[in]   data        Pointer to the data byte array for which CRC should
 *                          be calculated
 * @param[in]   dataSize    Size of the data transferred for calculation
 *
 * @return      32 bit CRC value
 */
uint32_t BSL_calculateCRC(uint8_t* data, uint16_t dataSize);

/*
 * @brief       Process the commands which needs handling in interface layer
 *
 * @param[in]   dataBuffer  Pointer to the data buffer which contains the packet
 *                          received
 *
 * @return      none
 */
void BSL_PI_interpretPICommand(const uint8_t* dataBuffer);

/*
 * Calculates the current TDCO for given DBTP values
 */
uint32_t get_CAN_TDCO(DL_MCAN_BitTimingParams* dbtp);

/*
 * Function definitions
 */
void SYSCFG_DL_MCAN_reconfig(void)
{
    /* Put MCAN in SW initialization mode. */

    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_SW_INIT);
    /* Wait till MCAN is not initialized. */
    while (DL_MCAN_OPERATION_MODE_SW_INIT != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    /* Initialize MCAN module. */
    DL_MCAN_init(MCAN0_INST, (DL_MCAN_InitParams*) &new_gMCAN0InitParams);

    /* Configure Bit timings. */
    DL_MCAN_setBitTime(
        MCAN0_INST, (DL_MCAN_BitTimingParams*) &new_gMCAN0BitTimes);

    /* Take MCAN out of the SW initialization mode */
    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_NORMAL);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    /* Enable MCAN mopdule Interrupts */
    DL_MCAN_enableIntr(MCAN0_INST, MCAN0_INST_MCAN_INTERRUPTS, 1U);

    DL_MCAN_selectIntrLine(
        MCAN0_INST, DL_MCAN_INTR_MASK_ALL, DL_MCAN_INTR_LINE_NUM_1);
    DL_MCAN_enableIntrLine(MCAN0_INST, DL_MCAN_INTR_LINE_NUM_1, 1U);

    /* Enable MSPM0 MCAN interrupt */
    DL_MCAN_clearInterruptStatus(MCAN0_INST, (DL_MCAN_MSP_INTERRUPT_LINE1));
    DL_MCAN_enableInterrupt(MCAN0_INST, (DL_MCAN_MSP_INTERRUPT_LINE1));
}

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
uint16_t decode_dlc(DL_MCAN_RxBufElement* rxMsg)
{
    return canfd_dlc_lengths[rxMsg->dlc];
}

/*
 * Calculates the size of each frame generated
 * by fragmenting without any padding inside CAN Frame
 */
void frame_fragmentation(uint16_t len, uint16_t* frame_fragments)
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

uint16_t BSL_PI_MCAN_init(uint8_t* buffer, uint16_t bufferSize)
{
    protocol_mode = CAN_MODE;
    BSL_initBuffers(buffer, bufferSize);

    SYSCFG_DL_init();
    /* Initialize interrupts in NVIC */
    NVIC_ClearPendingIRQ(CANFD0_INT_IRQn);
    NVIC_EnableIRQ(CANFD0_INT_IRQn);

    /* Register ISR for MCAN */
    DL_Interrupt_registerInterrupt(
        ((uint32_t) CANFD0_INT_IRQn), BSL_PI_MCAN_ISR);

    /* Return the Plugin version info to the ROM BSL core */
    return FLASH_PLUGIN_VERSION_MCAN;
}

/*Handles Primary processing of BSL packet before sending to Core*/
void process_bsl_packet()
{
    BSL_PI_sendByte(BSL_ACK);
    BSL_PI_interpretPICommand((const uint8_t*) BSL_MCAN_Back_Buf);

    /* Ping pong the buffers */
    if (BSL_Buffer_Status == BSL_BUF1_ACTIVE) {
        BSL_MCAN_Back_Buf   = BSL_MCAN_Buf1;
        BSL_MCAN_Active_Buf = BSL_MCAN_Buf2;
        BSL_Buffer_Status   = BSL_BUF2_ACTIVE;
    } else {
        BSL_MCAN_Back_Buf   = BSL_MCAN_Buf2;
        BSL_MCAN_Active_Buf = BSL_MCAN_Buf1;
        BSL_Buffer_Status   = BSL_BUF1_ACTIVE;
    }
}

uint32_t BSL_PI_MCAN_receive(void)
{
    volatile uint32_t receivedPacketAddr = (uint32_t) 0;
    volatile uint32_t command;
    /*
     * Since same data buffers are used between the ROM and flash plugins,
     * BSL_activePluginType helps to check if the data received is through
     * Flash plugin or not.
     */
    if (BSL_activePluginType == FLASH_PLUGIN_VERSION_MCAN) {
        /*

         * If the last received command packet is processed, and BSL core is ready
         * to accept a new data packet, the state will move from BLOCKED to IDLE.
         */
        if (BSL_RX_state == RX_blocked) {
            BSL_RX_state = RX_idle;
        }
        /*
         * Once the packet is started to arrive, the RX state will be changed to
         * RECEIVING and the same state is retained until complete packet is
         * received.
         */
        while (BSL_RX_state == RX_receiving) {
            if (BSL_PI_dataPointer == (uint32_t) BSL_PACK_HEADER_IDX) {
                if (BSL_PI_RxDataCnt > (uint16_t) BSL_PACK_HEADER_IDX) {
                    /* Check if first byte in the packet matches the Header byte */
                    if (BSL_MCAN_Back_Buf[BSL_PACK_HEADER_IDX] !=
                        BSL_PACKET_HEADER_BYTE) {
                        BSL_PI_sendByte(BSL_ERROR_HEADER_INCORRECT);
                        BSL_RX_state = RX_idle;
                    } else {
                        BSL_PI_dataPointer++;
                    }
                }
            }
            /* Next two bytes are buffer length */
            else if (BSL_PI_dataPointer == (uint32_t) BSL_PACK_LEN_IDX_1) {
                if (BSL_PI_RxDataCnt > (uint16_t) BSL_PACK_LEN_IDX_1) {
                    BSL_RXBufferSize =
                        (uint16_t) BSL_MCAN_Back_Buf[BSL_PACK_LEN_IDX_1];
                    BSL_PI_dataPointer++;
                }
            } else if (BSL_PI_dataPointer == (uint32_t) BSL_PACK_LEN_IDX_2) {
                if (BSL_PI_RxDataCnt > (uint16_t) BSL_PACK_LEN_IDX_2) {
                    uint16_t temp =
                        (uint16_t) BSL_MCAN_Back_Buf[BSL_PACK_LEN_IDX_2]
                        << (uint16_t) 8;
                    BSL_RXBufferSize |= temp;

                    /*
                     * Check if the packet size received is valid
                     * else return the error
                     */
                    if (BSL_RXBufferSize == (uint16_t) 0) {
                        BSL_PI_sendByte(BSL_ERROR_PACKET_SIZE_ZERO);
                        BSL_RX_state = RX_idle;
                    }
                    if ((BSL_RXBufferSize + BSL_PI_WRAPPER_SIZE) >
                        BSL_maxBufferSize) {
                        BSL_PI_sendByte(BSL_ERROR_PACKET_SIZE_TOO_BIG);
                        BSL_RX_state = RX_idle;
                    }
                    BSL_PI_dataPointer = (uint32_t) BSL_RXBufferSize +
                                         (uint32_t) BSL_HEAD_LEN_BYTES;
                }
            }
            /* Skip over the buffer data and go to the 32-bit checksum at the end */
            /* Start with the low checksum byte */
            else if (BSL_PI_dataPointer == ((uint32_t) BSL_RXBufferSize +
                                               (uint32_t) BSL_BUF_CRC_IDX_1)) {
                if (BSL_PI_RxDataCnt >
                    (BSL_RXBufferSize + (uint16_t) BSL_BUF_CRC_IDX_1)) {
                    BSL_PI_checksum = (uint32_t)
                        BSL_MCAN_Back_Buf[BSL_RXBufferSize +
                                          (uint16_t) BSL_BUF_CRC_IDX_1];

                    BSL_PI_dataPointer++;
                }
            }

            else if (BSL_PI_dataPointer == ((uint32_t) BSL_RXBufferSize +
                                               (uint32_t) BSL_BUF_CRC_IDX_2)) {
                if (BSL_PI_RxDataCnt >
                    (BSL_RXBufferSize + (uint16_t) BSL_BUF_CRC_IDX_2)) {
                    BSL_PI_checksum =
                        BSL_PI_checksum |
                        (BSL_MCAN_Back_Buf[BSL_RXBufferSize +
                                           (uint16_t) BSL_BUF_CRC_IDX_2]
                            << 8);
                    BSL_PI_dataPointer++;
                }
            } else if (BSL_PI_dataPointer ==
                       ((uint32_t) BSL_RXBufferSize +
                           (uint32_t) BSL_BUF_CRC_IDX_3)) {
                if (BSL_PI_RxDataCnt >
                    (BSL_RXBufferSize + (uint16_t) BSL_BUF_CRC_IDX_3)) {
                    BSL_PI_checksum =
                        BSL_PI_checksum |
                        (BSL_MCAN_Back_Buf[BSL_RXBufferSize +
                                           (uint16_t) BSL_BUF_CRC_IDX_3]
                            << 16);
                    BSL_PI_dataPointer++;
                }
            } else if (BSL_PI_dataPointer ==
                       ((uint32_t) BSL_RXBufferSize +
                           (uint32_t) BSL_BUF_CRC_IDX_4)) {
                if (BSL_PI_RxDataCnt >
                    (BSL_RXBufferSize + (uint16_t) BSL_BUF_CRC_IDX_4)) {
                    BSL_PI_checksum =
                        BSL_PI_checksum |
                        (BSL_MCAN_Back_Buf[BSL_RXBufferSize +
                                           (uint16_t) BSL_BUF_CRC_IDX_4]
                            << 24);
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;

                    BSL_RX_state = RX_blocked;

                    uint8_t* BSL_core_data_start =
                        (uint8_t*) &BSL_MCAN_Back_Buf[BSL_CORE_DATA_INDEX];

                    /* Verify checksum */
                    if (BSL_calculateCRC(BSL_core_data_start,
                            BSL_RXBufferSize) == BSL_PI_checksum) {
                        command = BSL_MCAN_Back_Buf[BSL_CORE_DATA_INDEX];
                        process_bsl_packet();

                        /*
                         * Return the packet address to the BSL core for processing
                         */
                        if (command == CMD_CHANGE_BAUDRATE)
                            receivedPacketAddr = 0;
                        else
                            receivedPacketAddr =
                                (uint32_t) BSL_MCAN_Active_Buf;
                        BSL_RX_state = RX_idle;
                    }

                    else /* Checksum failed */
                    {
                        BSL_PI_sendByte(BSL_ERROR_CHECKSUM_INCORRECT);
                        BSL_RX_state = RX_idle;
                    }
                }
            } else {
                /* Do nothing */
            }
        }
    }
    return receivedPacketAddr;
}

uint8_t MCAN_send_frame(uint32_t id, uint8_t* data, uint16_t len)
{
    frame_send_success = false;
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
    if (txMsg.id == MCAN_HOST_ID) {
        while (frame_send_success == false) {
            ;
        }
    }
    return 0;
}

uint8_t BSL_PI_MCAN_send(uint8_t* data, uint16_t len)
{
    uint16_t total_bytes;
    uint16_t i;
    uint16_t packet_length;
    uint16_t frame_length;
    uint16_t bytes_to_send;
    uint16_t total_fragments;
    uint16_t dataSize;
    uint16_t frame_fragments[3] = {0, 0, 0};
    uint8_t* fragment_start;
    uint8_t status    = BSL_PI_SUCCESS;
    uint32_t checksum = 0;

    uint8_t* BSL_core_data_start;
    DL_MCAN_TxBufElement txMsg;

    if (len <= 0)
        return 0;
    else if (len == 1) {
        MCAN_send_frame(MCAN_HOST_ID, data, len);
    } else {
        BSL_core_data_start = &data[3];
        dataSize            = (uint16_t) data[1];
        dataSize            = dataSize | (data[2] << 8U);

        //    * Calculate CRC for BSL Core Command and data *
        checksum = BSL_calculateCRC(BSL_core_data_start, dataSize);

        //    * Append the 32-bit CRC result to the response packet *
        data[len]                = (uint8_t) checksum & (uint8_t) 0xFF;
        uint8_t temp             = checksum >> 8U;
        data[len + (uint16_t) 1] = temp & (uint8_t) 0xFF;
        temp                     = checksum >> 16U;
        data[len + (uint16_t) 2] = temp & (uint8_t) 0xFF;
        data[len + (uint16_t) 3] = (checksum >> 24U);

        /*
     *     * Transmit the packet *
     * - Based on the total packet length calculate number of frames required;
     * - Copy data of each fragment into CAN Frame (txMsg element)
     * - send the frame
     *
    */
        packet_length = len + 4;
        bytes_to_send = packet_length;
        if (protocol_mode == CAN_MODE)
            frame_length = 8;
        else
            frame_length = 64;

        fragment_start = &data[0];
        while (bytes_to_send >= frame_length) {
            MCAN_send_frame(MCAN_HOST_ID, fragment_start, frame_length);
            fragment_start += frame_length;
            bytes_to_send -= frame_length;
        }
        frame_fragmentation(bytes_to_send, &frame_fragments[0]);

        for (int i = 0; i < 3 && (frame_fragments[i] != 0); i++) {
            MCAN_send_frame(MCAN_HOST_ID, fragment_start, frame_fragments[i]);
            fragment_start += frame_fragments[i];
        }
    }
    return status;
}

bool BSL_PI_MCAN_deinit(void)
{
    /*  Reset the MCAN */
    DL_MCAN_reset(CANFD0);

    /*  Disable and Unregister the interrupt */
    DL_Interrupt_unregisterInterrupt((uint32_t) CANFD0_INT_IRQn);
    NVIC_DisableIRQ(CANFD0_INT_IRQn);

    return true;
}

static void BSL_PI_sendByte(uint8_t data)
{
    BSL_PI_MCAN_send(&data, 1);
}

void BSL_initBuffers(uint8_t* buffer, uint16_t bufferSize)
{
    /*
     * Two buffers are configured for BSL communication, in which one buffer
     * will be used for receiving the packets, while other buffer contains the
     * last received command packet shared with BSL core for processing.
     */
    BSL_MCAN_Buf1       = buffer;
    BSL_MCAN_Buf2       = &buffer[bufferSize];
    BSL_MCAN_Active_Buf = BSL_MCAN_Buf1;
    BSL_MCAN_Back_Buf   = BSL_MCAN_Buf2;

    BSL_Buffer_Status = BSL_BUF1_ACTIVE;
    BSL_maxBufferSize = bufferSize;

    /* Initialize buffer pointers */
    BSL_RXBufferSize   = (uint16_t) 0;
    BSL_PI_dataPointer = (uint32_t) 0;
    BSL_PI_checksum    = (uint32_t) 0;
    BSL_PI_RxDataCnt   = (uint16_t) 0;

    /* Reset the Receive state */
    BSL_RX_state = RX_idle;
}

void write_into_buffer(
    uint8_t* write_buffer, DL_MCAN_RxBufElement* rxMsg, uint16_t size)
{
    uint16_t i;
    for (i = 0; i < size; i++) {
        write_buffer[BSL_PI_dataWritePointer] = (uint8_t) rxMsg->data[i];
        BSL_PI_dataWritePointer++;
    }
    return;
}

void BSL_PI_MCAN_ISR(void)
{
    uint8_t receivedData;
    DL_MCAN_RxBufElement rxMsg;
    DL_MCAN_RxFIFOStatus rxFS;
    uint16_t mcan_frame_size;
    uint32_t intr_status;

    switch (DL_MCAN_getPendingInterrupt(CANFD0)) {
        case DL_MCAN_IIDX_LINE1:

            intr_status = DL_MCAN_getIntrStatus(CANFD0);

            //clear MCAN IR reg
            DL_MCAN_clearIntrStatus(
                CANFD0, intr_status, DL_MCAN_INTR_SRC_MCAN_LINE_1);

            if ((intr_status & MCAN_IR_TC_MASK) == MCAN_IR_TC_MASK)
                frame_send_success = true;
            else if ((intr_status & MCAN_IR_RF0N_MASK) == MCAN_IR_RF0N_MASK) {
                rxFS.num = DL_MCAN_RX_FIFO_NUM_0;
                do {
                    DL_MCAN_getRxFIFOStatus(CANFD0, &rxFS);
                } while ((rxFS.fillLvl) == 0);

                DL_MCAN_readMsgRam(
                    CANFD0, DL_MCAN_MEM_TYPE_FIFO, 0U, rxFS.num, &rxMsg);
                DL_MCAN_writeRxFIFOAck(CANFD0, rxFS.num, rxFS.getIdx);

                mcan_frame_size = decode_dlc(&rxMsg);

                /*
                 * If a Packet is received when RX is in IDLE state, it is treated as
                 * a new command packet and buffer is filled with packet
                 *
                 * To have robust interface auto detection, only if the Header byte is present
                 * as the first byte of the first packet, the data transfer is considered as valid and
                 * the interface is chosen as active interface
                 */
                if (BSL_RX_state == RX_idle) {
                    BSL_activePluginType    = FLASH_PLUGIN_VERSION_MCAN;
                    BSL_PI_dataPointer      = (uint32_t) 0;
                    BSL_PI_dataWritePointer = (uint32_t) 0;

                    // call function to copy rxmsg.data content into back Buffer
                    write_into_buffer(
                        (uint8_t*) BSL_MCAN_Back_Buf, &rxMsg, mcan_frame_size);

                    BSL_PI_RxDataCnt = decode_dlc(&rxMsg);

                    if (BSL_PI_RxDataCnt >= 2) {
                        BSL_RXBufferSize = (uint16_t) BSL_MCAN_Back_Buf[1];
                        uint16_t temp    = (uint16_t) BSL_MCAN_Back_Buf[2]
                                        << (uint16_t) 8;
                        BSL_RXBufferSize |= temp;
                        BSL_RXBufferSize += BSL_PI_WRAPPER_SIZE;

                        BSL_RX_state = RX_receiving;
                    }
                }
                /*
                 * Whenever a new packet is received, after a frame reception is
                 * started, append it to the data buffer
                 */
                else if (BSL_RX_state == RX_receiving) {
                    if (BSL_PI_RxDataCnt <
                        (BSL_maxBufferSize + (uint16_t) 4)) {
                        write_into_buffer((uint8_t*) BSL_MCAN_Back_Buf, &rxMsg,
                            mcan_frame_size);
                        BSL_PI_RxDataCnt += mcan_frame_size;
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

uint32_t BSL_calculateCRC(uint8_t* data, uint16_t dataSize)
{
    uint16_t bufferIndex;
    /*
     * @note: CRC module would have been configured by the ROM BSL with
     * required specification.
     */
    DL_CRC_enablePower(CRC);
    DL_CRC_init(CRC, DL_CRC_32_POLYNOMIAL, DL_CRC_BIT_REVERSED,
        DL_CRC_INPUT_ENDIANESS_LITTLE_ENDIAN, DL_CRC_OUTPUT_BYTESWAP_DISABLED);
    /* Set the Seed value to reset the calculation */
    DL_CRC_setSeed32(BSL_CRC, BSL_CRC_SEED);

    /* Feed the data to CRC module */
    for (bufferIndex = (uint16_t) 0; bufferIndex < dataSize; bufferIndex++) {
        DL_CRC_feedData8(BSL_CRC, data[bufferIndex]);
    }

    /* Return the 32 bit result */  //
    return DL_CRC_getResult32(BSL_CRC);
}

uint32_t get_CAN_TDCO(DL_MCAN_BitTimingParams* dbtp)
{
    uint32_t tdco = (dbtp->dataTimeSeg1 + dbtp->dataTimeSeg2 + 3);
    tdco *= (dbtp->dataRatePrescalar + 1);
    tdco /= 2;
    return tdco;
}

void BSL_PI_interpretPICommand(const uint8_t* dataBuffer)
{
    DL_MCAN_ProtocolStatus psr;
    uint8_t command = dataBuffer[BSL_CORE_DATA_INDEX];
    /* Check if command is non-blocking */
    if (command == CMD_PROGRAM_DATA_FAST) {
        /*
         * When a non-blocking command is received, set the RX state to IDLE
         * So that the state machine continuous to receive the next command
         * packet without waiting for the current command to be processed by the
         * BSL core
         * */
        BSL_RX_state = RX_idle;
    } else if (command == CMD_CHANGE_BAUDRATE) {
        uint64_t temp = 0;
        for (int i = 0; i < 7; i++) {
            temp |= BSL_MCAN_Back_Buf[BSL_CORE_DATA_INDEX + 1 + i];
            if (i != 6) temp <<= 8;
        }

        /* Initialization of New MCAN Init parameters.    */
        new_gMCAN0InitParams.fdMode = temp & BITRATE_CONF_FD_MASK;
        temp >>= BITRATE_CONF_FD_LEN;

        if (new_gMCAN0InitParams.fdMode) protocol_mode = CAN_FD_MODE;

        new_gMCAN0InitParams.brsEnable = temp & BITRATE_CONF_BRS_MASK;
        temp >>= BITRATE_CONF_BRS_LEN;

        new_gMCAN0InitParams.txpEnable   = false;
        new_gMCAN0InitParams.efbi        = false;
        new_gMCAN0InitParams.pxhddisable = false;

        new_gMCAN0InitParams.darEnable       = false;
        new_gMCAN0InitParams.wkupReqEnable   = true;
        new_gMCAN0InitParams.autoWkupEnable  = true;
        new_gMCAN0InitParams.emulationEnable = true;

        new_gMCAN0BitTimes.nomTimeSeg1 = temp & BITRATE_CONF_NTSG1_MASK;
        temp >>= BITRATE_CONF_NTSG1_LEN;

        new_gMCAN0BitTimes.nomTimeSeg2 = temp & BITRATE_CONF_NTSG2_MASK;
        temp >>= BITRATE_CONF_NTSG2_LEN;

        new_gMCAN0BitTimes.nomSynchJumpWidth = temp & BITRATE_CONF_NSJW_MASK;
        temp >>= BITRATE_CONF_NSJW_LEN;

        new_gMCAN0BitTimes.nomRatePrescalar = temp & BITRATE_CONF_NPS_MASK;
        temp >>= BITRATE_CONF_NPS_LEN;

        new_gMCAN0BitTimes.dataTimeSeg1 = temp & BITRATE_CONF_DTSG1_MASK;
        temp >>= BITRATE_CONF_DTSG1_LEN;

        new_gMCAN0BitTimes.dataTimeSeg2 = temp & BITRATE_CONF_DTSG2_MASK;
        temp >>= BITRATE_CONF_DTSG2_LEN;

        new_gMCAN0BitTimes.dataSynchJumpWidth = temp & BITRATE_CONF_DSJW_MASK;
        temp >>= BITRATE_CONF_DSJW_LEN;

        new_gMCAN0BitTimes.dataRatePrescalar = temp & BITRATE_CONF_DPS_MASK;

        /* Transmitter Delay Compensation parameters. */
        new_gMCAN0InitParams.tdcEnable = protocol_mode;
        new_gMCAN0InitParams.tdcConfig.tdco =
            get_CAN_TDCO(&new_gMCAN0BitTimes);
        new_gMCAN0InitParams.tdcConfig.tdcf =
            new_gMCAN0InitParams.tdcConfig.tdco + 1;
        new_gMCAN0InitParams.wdcPreload = 255;

        SYSCFG_DL_MCAN_reconfig();

        if (protocol_mode == CAN_FD_MODE) {
            const int x[1] = {1};
            MCAN_send_frame(((0x5) << 18), (uint8_t*) &x[0], 1);
            DL_MCAN_getProtocolStatus(CANFD0, &psr);
            new_gMCAN0InitParams.darEnable      = true;
            new_gMCAN0InitParams.tdcConfig.tdcf = (psr.tdcv) - 1;
            SYSCFG_DL_MCAN_reconfig();
        }
        DL_Common_delayCycles(BITRATE_CHANGE_DELAY);
    } else {
        /* Command is blocking */
        BSL_RX_state = RX_blocked;
    }
}
