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
#include <ti_msp_dl_config.h>
#include "can.h"
#include "stdio.h"
#include "string.h"
extern DL_MCAN_InitParams new_gMCAN0InitParams;
extern DL_MCAN_BitTimingParams new_gMCAN0BitTimes;
volatile uint8_t BSL_TX_buffer[MAX_PACKET_SIZE + 2];
volatile uint8_t BSL_RX_buffer[MAX_PACKET_SIZE + 2];
//*****************************************************************************
//
// ! BSL Entry Sequence
// ! Forces target to enter BSL mode
//
//*****************************************************************************
void Host_BSL_entry_sequence()
{
    /* BSL reset sequence
     *
     *                   H                        +-------------+
     * PB0 NRST          L -----------------------+
     *
     *                   H        +------------------------------+
     * PB16 Invoke GPIO  L--------+                              +----
     *
     */
    /* NRST low, TEST low */

    DL_GPIO_clearPins(GPIO_BSL_PORT, GPIO_BSL_NRST_PIN);
    DL_GPIO_clearPins(GPIO_BSL_PORT, GPIO_BSL_Invoke_PIN);
    delay_cycles(BSL_INVOKE_DELAY);

    /* Invoke GPIO high*/
    DL_GPIO_setPins(GPIO_BSL_PORT, GPIO_BSL_Invoke_PIN);
    delay_cycles(BSL_INVOKE_DELAY);
    delay_cycles(33000000);
    /* NRST high*/
    DL_GPIO_setPins(GPIO_BSL_PORT, GPIO_BSL_NRST_PIN);
    delay_cycles(BSL_INVOKE_DELAY);
    delay_cycles(BSL_INVOKE_DELAY);
    delay_cycles(BSL_INVOKE_DELAY);
    DL_GPIO_clearPins(GPIO_BSL_PORT, GPIO_BSL_Invoke_PIN);
}

void Host_BSL_software_trigger(void)
{
    uint8_t Software_trigger[1] = {0x22};
    MCAN_send_frame(MCAN_HOST_ID, Software_trigger, 1);
}

/*
 * Turn on the error LED
 */
void TurnOnErrorLED(void)
{
    DL_GPIO_setPins(GPIO_LED_Error_PORT, GPIO_LED_Error_PIN);
}
//*****************************************************************************
//
// ! Host_BSL_Connection
// ! Need to send first to build connection with target
//
//*****************************************************************************

uint8_t Host_BSL_Connection(void)
{
    active_BSL_command   = CMD_CONNECTION;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) BSL_PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(BSL_CMD_SIZE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_CONNECTION;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC((uint8_t *) &BSL_TX_buffer[3], BSL_CMD_SIZE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE] = ui32CRC;

    MCAN_send_BSL_packet(BSL_TX_buffer, BSL_HED_LEN_CMD_SIZE + BSL_CRC_SIZE);

    bsl_response = receive_bsl_response();
    if (bsl_response != BSL_OPERATION_SUCCESFUL) {
        TurnOnErrorLED();
    }
    return (bsl_response);
}

//*****************************************************************************
// ! Host_BSL_Change_Bitrate
// ! Need to send when new Bitrate has to be configured
//
//*****************************************************************************
uint8_t Host_BSL_Change_Bitrate(CANFD_baudrate_config *cfg)
{
    active_BSL_command   = CMD_CHANGE_BAUDRATE;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint32_t ui32CRC;
    uint64_t CBR = 0;
    DL_MCAN_ProtocolStatus psr;

    CBR |= (uint64_t) cfg->fdMode;
    CBR |= ((uint64_t) cfg->brsEnable) << 1;
    CBR |= ((uint64_t) cfg->nomTimeSeg1) << 2;
    CBR |= ((uint64_t) cfg->nomTimeSeg2) << 10;
    CBR |= ((uint64_t) cfg->nomSynchJumpWidth) << 17;
    CBR |= ((uint64_t) cfg->nomRatePrescalar) << 24;
    CBR |= ((uint64_t) cfg->dataTimeSeg1) << 33;
    CBR |= ((uint64_t) cfg->dataTimeSeg2) << 38;
    CBR |= ((uint64_t) cfg->dataSynchJumpWidth) << 42;
    CBR |= ((uint64_t) cfg->dataRatePrescalar) << 46;

    BSL_TX_buffer[0] = (uint8_t) BSL_PACKET_HEADER;
    BSL_TX_buffer[1] = LSB((BSL_CMD_SIZE + BSL_BITRATE_CONFIG_SIZE));
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_CHANGE_BAUDRATE;

    for (int i = 7; i > 0; i--) {
        BSL_TX_buffer[i + CORE_INDEX] = CBR & 0xFF;
        CBR >>= 8;
    }

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(
        (uint8_t *) &BSL_TX_buffer[3], BSL_CMD_SIZE + BSL_BITRATE_CONFIG_SIZE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE +
                                 BSL_BITRATE_CONFIG_SIZE] = ui32CRC;

    // Write the packet to the target
    MCAN_send_BSL_packet(BSL_TX_buffer,
        BSL_HED_LEN_CMD_SIZE + BSL_BITRATE_CONFIG_SIZE + BSL_CRC_SIZE);
    bsl_response = receive_bsl_response();
    if (bsl_response != BSL_OPERATION_SUCCESFUL) {
        TurnOnErrorLED();
    } else {
        protocol_mode                        = cfg->fdMode;
        new_gMCAN0InitParams.fdMode          = cfg->fdMode;
        new_gMCAN0InitParams.brsEnable       = cfg->brsEnable;
        new_gMCAN0InitParams.darEnable       = false;
        new_gMCAN0InitParams.wkupReqEnable   = true;
        new_gMCAN0InitParams.autoWkupEnable  = true;
        new_gMCAN0InitParams.emulationEnable = true;

        new_gMCAN0BitTimes.nomTimeSeg1        = cfg->nomTimeSeg1;
        new_gMCAN0BitTimes.nomTimeSeg2        = cfg->nomTimeSeg2;
        new_gMCAN0BitTimes.nomSynchJumpWidth  = cfg->nomSynchJumpWidth;
        new_gMCAN0BitTimes.nomRatePrescalar   = cfg->nomRatePrescalar;
        new_gMCAN0BitTimes.dataTimeSeg1       = cfg->dataTimeSeg1;
        new_gMCAN0BitTimes.dataTimeSeg2       = cfg->dataTimeSeg2;
        new_gMCAN0BitTimes.dataSynchJumpWidth = cfg->dataSynchJumpWidth;
        new_gMCAN0BitTimes.dataRatePrescalar  = cfg->dataRatePrescalar;

        new_gMCAN0InitParams.tdcEnable = cfg->fdMode;
        new_gMCAN0InitParams.tdcConfig.tdco =
            get_CAN_TDCO(&new_gMCAN0BitTimes);
        new_gMCAN0InitParams.tdcConfig.tdcf =
            new_gMCAN0InitParams.tdcConfig.tdco + 1;
        new_gMCAN0InitParams.wdcPreload = 255;

        MCAN_reconfig();

        if (protocol_mode == CAN_FD_MODE) {
            int x[1] = {1};
            MCAN_send_frame(((0x6) << 18), &x[0], 1);
            DL_MCAN_getProtocolStatus(CANFD0, &psr);
            new_gMCAN0InitParams.darEnable      = true;
            new_gMCAN0InitParams.tdcConfig.tdcf = (psr.tdcv) - 1;
            delay_cycles(300);
            MCAN_reconfig();
        }
    }

    return (bsl_response);
}

//*****************************************************************************
// ! Host_BSL_GetID
// ! Need to send when build connection to get RAM BSL_RX_buffer size and other information
//
//*****************************************************************************
uint8_t Host_BSL_GetID(void)
{
    active_BSL_command   = CMD_GET_DEVICE_INFO;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) BSL_PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(BSL_CMD_SIZE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_GET_DEVICE_INFO;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC((uint8_t *) &BSL_TX_buffer[3], BSL_CMD_SIZE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE] = ui32CRC;

    // Write the packet to the target
    MCAN_send_BSL_packet(BSL_TX_buffer, BSL_HED_LEN_CMD_SIZE + BSL_CRC_SIZE);
    bsl_response = receive_bsl_response();
    if (bsl_response != BSL_OPERATION_SUCCESFUL) {
        TurnOnErrorLED();
    }
    return (bsl_response);
}
//*****************************************************************************
// ! Unlock BSL for programming
// ! If first time, assume blank device.
// ! This will cause a mass erase and destroy previous password.
// ! When programming complete, issue BSL_readPassword to retrieve new one.
//
//*****************************************************************************
uint8_t Host_BSL_loadPassword(uint8_t *pPassword)
{
    active_BSL_command   = CMD_UNLOCK_BOOTLOADER;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) BSL_PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(BSL_PASSWORD_SIZE + BSL_CMD_SIZE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_UNLOCK_BOOTLOADER;

    memcpy((uint8_t *) &BSL_TX_buffer[4], pPassword, BSL_PASSWORD_SIZE);

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(
        (uint8_t *) &BSL_TX_buffer[3], BSL_PASSWORD_SIZE + BSL_CMD_SIZE);

    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE + BSL_PASSWORD_SIZE] =
        ui32CRC;

    // Write the packet to the target
    MCAN_send_BSL_packet(BSL_TX_buffer,
        BSL_HED_LEN_CMD_SIZE + BSL_PASSWORD_SIZE + BSL_CRC_SIZE);
    bsl_response = receive_bsl_response();
    if (bsl_response != BSL_OPERATION_SUCCESFUL) {
        TurnOnErrorLED();
    }
    return (bsl_response);
}

//*****************************************************************************
// ! Host_BSL_MassErase
// ! Need to do mess erase before write new image
//
//*****************************************************************************
uint8_t Host_BSL_MassErase(void)
{
    active_BSL_command   = CMD_MASS_ERASE;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) BSL_PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(BSL_CMD_SIZE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_MASS_ERASE;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC((uint8_t *) &BSL_TX_buffer[3], BSL_CMD_SIZE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE] = ui32CRC;

    // Write the packet to the target
    MCAN_send_BSL_packet(BSL_TX_buffer, BSL_HED_LEN_CMD_SIZE + BSL_CRC_SIZE);

    bsl_response = receive_bsl_response();
    if (bsl_response != BSL_OPERATION_SUCCESFUL) {
        TurnOnErrorLED();
    }
    return (bsl_response);
}

//*****************************************************************************
//
// ! Host_BSL_writeMemory
// ! Writes memory section to target
//
//*****************************************************************************
uint8_t Host_BSL_writeMemory(uint32_t addr, const uint8_t *data, uint32_t len)
{
    active_BSL_command   = CMD_PROGRAM_DATA;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint16_t ui16DataLength;
    uint16_t ui16PayloadSize;
    uint16_t ui16PacketSize;
    uint32_t ui32CRC;
    uint16_t ui16BytesToWrite = len;
    uint32_t TargetAddress    = addr;

    while (ui16BytesToWrite > 0) {
        delay_cycles(2000000);  //allow target deal with the packet send before

        if (ui16BytesToWrite >= MAX_PAYLOAD_DATA_SIZE)
            ui16DataLength = MAX_PAYLOAD_DATA_SIZE;
        else
            ui16DataLength = ui16BytesToWrite;

        ui16BytesToWrite = ui16BytesToWrite - ui16DataLength;

        // Add (1byte) command + (4 bytes)ADDRS = 5 bytes to the payload
        ui16PayloadSize = (BSL_CMD_SIZE + BSL_ADDRS_SIZE + ui16DataLength);

        BSL_TX_buffer[0] = BSL_PACKET_HEADER;
        BSL_TX_buffer[1] =
            LSB(ui16PayloadSize);  // typically 4 + MAX_PAYLOAD SIZE
        BSL_TX_buffer[2] = MSB(ui16PayloadSize);
        BSL_TX_buffer[3] = (uint8_t) CMD_PROGRAM_DATA;
        *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE] = TargetAddress;

        // Bump up the target address by 2x the number of bytes sent for the next packet
        TargetAddress += ui16DataLength;

        // Copy the data into the BSL_TX_buffer
        memcpy(
            (uint8_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE + BSL_ADDRS_SIZE],
            data, ui16DataLength);

        data += ui16DataLength;

        // Calculate CRC on the PAYLOAD
        ui32CRC = softwareCRC((uint8_t *) &BSL_TX_buffer[3], ui16PayloadSize);

        // Calculate the packet length
        ui16PacketSize =
            BSL_HED_LEN_CMD_SIZE + BSL_ADDRS_SIZE + ui16DataLength;

        // Insert the CRC into the packet at the end
        *(uint32_t *) &BSL_TX_buffer[ui16PacketSize] = ui32CRC;

        // Write the packet to the target
        MCAN_send_BSL_packet(BSL_TX_buffer, ui16PacketSize + BSL_CRC_SIZE);

        bsl_response = receive_bsl_response();
        if (bsl_response != BSL_OPERATION_SUCCESFUL) {
            TurnOnErrorLED();
        }

        if (bsl_response != BSL_OPERATION_SUCCESFUL) break;

    }  // end while

    return (bsl_response);
}

//*****************************************************************************
// ! Host_BSL_StartApp
// ! Start the new application
//
//*****************************************************************************
uint8_t Host_BSL_StartApp(void)
{
    active_BSL_command   = CMD_START_APPLICATION;
    uint8_t bsl_response = BSL_OPERATION_SUCCESFUL;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) BSL_PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(BSL_CMD_SIZE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_START_APPLICATION;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC((uint8_t *) &BSL_TX_buffer[3], BSL_CMD_SIZE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[BSL_HED_LEN_CMD_SIZE] = ui32CRC;

    MCAN_send_BSL_packet(BSL_TX_buffer, BSL_HED_LEN_CMD_SIZE + BSL_CRC_SIZE);

    // Write the packet to the target
    bsl_response = receive_bsl_response();
    if (bsl_response != BSL_OPERATION_SUCCESFUL) {
        TurnOnErrorLED();
    }
    return (bsl_response);
}

//*****************************************************************************
//
// ! softwareCRC
// ! Can be used on MSP430 and non-MSP platforms
// ! This functions computes the 16-bit CRC (same as BSL on MSP target)
//
//*****************************************************************************
#define CRC32_POLY 0xEDB88320
uint32_t softwareCRC(const uint8_t *data, uint8_t length)
{
    uint32_t ii, jj, byte, crc, mask;
    ;

    crc = 0xFFFFFFFF;

    for (ii = 0; ii < length; ii++) {
        byte = data[ii];
        crc  = crc ^ byte;

        for (jj = 0; jj < 8; jj++) {
            mask = -(crc & 1);
            crc  = (crc >> 1) ^ (CRC32_POLY & mask);
        }
    }

    return crc;
}
