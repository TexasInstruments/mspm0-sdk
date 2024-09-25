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

#include <ti_msp_dl_config.h>
#include "stdint.h"

#define BSL_PACK_HEADER_IDX 0x0
#define BSL_PACK_LEN_IDX_1 0x1
#define BSL_PACK_LEN_IDX_2 0x2

#define BSL_BUF_CRC_IDX_1 0x3
#define BSL_BUF_CRC_IDX_2 0x4
#define BSL_BUF_CRC_IDX_3 0x5
#define BSL_BUF_CRC_IDX_4 0x6

#define DELAY_CAN_FRAME_TX 2000
#define MCAN_HOST_ID ((uint32_t)(0x3)) << 18U
bool ack_received;
uint8_t protocol_mode;
uint8_t ack_error_status;
uint8_t msg_error_status;
uint16_t RxDataCnt;
uint16_t active_BSL_command;
uint32_t BSL_resp_checksum;
volatile uint16_t BSL_response_size;
volatile uint32_t dataPointer;
volatile uint32_t dataWritePointer;

/*
 * Enum that lists the various states involved in packet reception
 */
typedef enum {
    /*
     * IDLE state indicates that the Host is ready to accept
     * the new data packet from the BSL.
     */
    RX_idle = 0x0,
    /*
     * When the Interface receives first CAN frame containing acknowledgment the
     * receive state machine will be moved from IDLE to ACKNOWLEDGED.
     * It goes into idle state if the active command doesnt require a response message.
     */
    RX_acknowledged = 0x1,
    /*
    * When the Interface receives first frame of the response packet,
    * the receive state machine will be moved from IDLE/Acknowledged to RECEIVING.
    * At this state it reads the data from Host
    */
    RX_receiving = 0x2,
    /*
     * BLOCKED is a state when complete packet is received and is yet to be
     * processed. At this state any data from the Host will be ignored.
     */
    RX_blocked = 0x3,

} RX_states;

/*
 * Enum that lists the CAN Modes Supported
 */
typedef enum { CAN_MODE = 0, CAN_FD_MODE = 1 } CAN_MODES;

/**
 *  @brief  Structure for MCAN Bitrate and Mode Reconfiguration
 *          Contains CAN Mode and bitrate switching,
 *          DBTP and NBTP values.
 */
typedef struct {
    uint8_t fdMode;
    uint8_t brsEnable;
    uint8_t nomTimeSeg1;
    uint8_t nomTimeSeg2;
    uint8_t nomSynchJumpWidth;
    uint16_t nomRatePrescalar;
    uint8_t dataTimeSeg1;
    uint8_t dataTimeSeg2;
    uint8_t dataSynchJumpWidth;
    uint8_t dataRatePrescalar;
} CANFD_baudrate_config;

void CAN_initialize(void);
uint8_t MCAN_send_BSL_packet(volatile uint8_t *BSL_packet, uint16_t size);
uint8_t receive_bsl_response();
bool MCAN_send_frame(uint32_t id, uint8_t *data, uint16_t len);
void MCAN_reconfig(void);
uint8_t Host_BSL_Change_Bitrate(CANFD_baudrate_config *cfg);
