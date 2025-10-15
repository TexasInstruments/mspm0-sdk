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

#include "ti_msp_dl_config.h"

/* Flash Plugin Interface Version */
#define FLASH_PLUGIN_VERSION_I2C ((uint16_t) 0x1002)

/* BSL Packet definitions */
#define BSL_PACKET_HEADER_BYTE ((uint8_t) 0x80)

/* Wrapper size includes 1 Header + 2 length + 4 crc bytes */
#define BSL_PI_WRAPPER_SIZE ((uint16_t) 0x7)

/* 4 CRC bytes for CRC32 */
#define BSL_PI_CRC_SIZE ((uint16_t) 0x4)

/* BSL Core commands to handle in Interface layer */
#define CMD_PROGRAM_DATA_FAST ((uint8_t) 0x24)

/* BSL Acknowledgment responses */
#define BSL_ERROR_HEADER_INCORRECT ((uint8_t) 0x51)
#define BSL_ERROR_CHECKSUM_INCORRECT ((uint8_t) 0x52)
#define BSL_ERROR_PACKET_SIZE_ZERO ((uint8_t) 0x53)
#define BSL_ERROR_PACKET_SIZE_TOO_BIG ((uint8_t) 0x54)
#define BSL_ERROR_UNKNOWN_ERROR ((uint8_t) 0x55)
#define BSL_ACK ((uint8_t) 0x0)

/* BSL PI Return codes */
#define BSL_PI_FAIL ((uint8_t) 0x0)
#define BSL_PI_SUCCESS ((uint8_t) 0x1)

/* BSL Buffer Status */
#define BSL_BUF1_ACTIVE ((uint8_t) 0x0)
#define BSL_BUF2_ACTIVE ((uint8_t) 0x1)

/*
 * Enum that lists the various states involved in packet reception
 */
typedef enum {
    /*
     * IDLE state indicates that the BSL is ready to accept
     * the new data packet from the Host.
     */
    RX_idle = 0x0,
    /*
     * When the Interface receives first byte of the packet, the receive state
     * machine will be moved from IDLE to REVEIVING. At this state it reads
     * the data from Host
     */
    RX_receiving = 0x1,
    /*
     * BLOCKED is a state when complete packet is received and is yet to be
     * processed. At this state any data from the Host will be ignored.
     */
    RX_blocked = 0x2

} BSL_RX_states;

/*
 * Flash Plugin APIs to be registered in Bootloader configuration
 */
/*
 * @brief Configures the Flash Plugin
 *
 * This function configures the I2C interface for BSL communication,
 * Initializes the buffer variables. Also initializes all the global variables
 * needed for Plugin operation.
 *
 * @param[in]   buffer      Pointer to the SRAM data buffer sent from the
 *                          BSL core.
 * @param[in]   bufferSize  1/2 of the SRAM memory size available to use as
 *                          data buffer.
 *
 * @return      16 bit Plugin version information
 */
uint16_t BSL_PI_I2C_init(uint8_t* buffer, uint16_t bufferSize);

/*
 * @brief   Receives the command packet
 *
 * It shares the command packet with BSL core only when the complete packet is
 * received and correctness of the data is checked (CRC verification of data).
 * If this function is called by BSL core, when there is no data packet received
 * or when receiving is in progress, it returns '0'.
 *
 * @return  32 bit start address of the Received data packet
 */
uint32_t BSL_PI_I2C_receive(void);
/*
 * @brief       Transfers the BSL core response packet
 *
 * It takes care of calculating and adding CRC for the packet. And then
 * transmits the packet.
 *
 * @param[in]   data    Pointer to the BSL core response packet to be sent to
 *                      the host.
 * @param[in]   len     Length of the BSL core response packet excluding the
 *                      length of the CRC (4 bytes).
 *
 * @return      Status of transmission
 *
 * @retval      Returns '1' if the transmission is successful
 * @retval      Returns '0' if the transmission failed.
 */
uint8_t BSL_PI_I2C_send(uint8_t* data, uint16_t len);
/*
 * @brief       Disables the I2C
 *
 * Resets the I2C peripheral and disables the Interrupts
 *
 * @return      Returns '1' if deinit is done
 */
bool BSL_PI_I2C_deinit(void);
