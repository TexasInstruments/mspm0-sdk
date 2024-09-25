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
#include <bsl_uart.h>

#include "stdio.h"
#include "string.h"
#include "ti_msp_dl_config.h"
#include "uart.h"

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
    delay_cycles(BSL_DELAY);

    /* Invoke GPIO high*/
    DL_GPIO_setPins(GPIO_BSL_PORT, GPIO_BSL_Invoke_PIN);
    delay_cycles(BSL_DELAY);
    delay_cycles(33000000);
    /* NRST high*/
    DL_GPIO_setPins(GPIO_BSL_PORT, GPIO_BSL_NRST_PIN);
    delay_cycles(BSL_DELAY);
    delay_cycles(BSL_DELAY);
    delay_cycles(BSL_DELAY);
    DL_GPIO_clearPins(GPIO_BSL_PORT, GPIO_BSL_Invoke_PIN);
}

void Host_BSL_software_trigger(void)
{
    /* Wait until all bytes have been transmitted and the TX FIFO is empty */
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;
    DL_UART_transmitDataBlocking(UART_0_INST, 0x22);
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

BSL_error_t Host_BSL_Connection(void)
{
    BSL_error_t bsl_err = eBSL_success;
    uart_error_t uart_ack;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_CONNECTION;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui32CRC;

    uart_ack = UART_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);

    if (uart_ack != uart_noError) {
        TurnOnErrorLED();
    }
    return (bsl_err);
}

//*****************************************************************************
// ! Host_BSL_GetID
// ! Need to send when build connection to get RAM BSL_RX_buffer size and other information
//
//*****************************************************************************
BSL_error_t Host_BSL_GetID(void)
{
    BSL_error_t bsl_err = eBSL_success;
    uart_error_t uart_ack;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_GET_ID;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui32CRC;

    // Write the packet to the target
    uart_ack = UART_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    if (uart_ack != uart_noError) {
        TurnOnErrorLED();
    }

    UART_readBuffer(BSL_RX_buffer, HDR_LEN_CMD_BYTES + ID_BACK + CRC_BYTES);
    BSL_MAX_BUFFER_SIZE = 0;
    BSL_MAX_BUFFER_SIZE =
        *(uint16_t *) &BSL_RX_buffer[HDR_LEN_CMD_BYTES + ID_BACK - 14];
    return (bsl_err);
}
//*****************************************************************************
// ! Unlock BSL for programming
// ! If first time, assume blank device.
// ! This will cause a mass erase and destroy previous password.
// ! When programming complete, issue BSL_readPassword to retrieve new one.
//
//*****************************************************************************
BSL_error_t Host_BSL_loadPassword(uint8_t *pPassword)
{
    BSL_error_t bsl_err = eBSL_success;
    uart_error_t uart_ack;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(PASSWORD_SIZE + CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_RX_PASSWORD;

    memcpy(&BSL_TX_buffer[4], pPassword, PASSWORD_SIZE);

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(&BSL_TX_buffer[3], PASSWORD_SIZE + CMD_BYTE);

    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + PASSWORD_SIZE] = ui32CRC;

    // Write the packet to the target
    uart_ack = UART_writeBuffer(
        BSL_TX_buffer, HDR_LEN_CMD_BYTES + PASSWORD_SIZE + CRC_BYTES);
    if (uart_ack != uart_noError) {
        TurnOnErrorLED();
    }

    bsl_err = Host_BSL_getResponse();

    return (bsl_err);
}

//*****************************************************************************
// ! Host_BSL_MassErase
// ! Need to do mess erase before write new image
//
//*****************************************************************************
BSL_error_t Host_BSL_MassErase(void)
{
    BSL_error_t bsl_err = eBSL_success;
    uart_error_t uart_ack;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_MASS_ERASE;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui32CRC;

    // Write the packet to the target
    uart_ack = UART_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    if (uart_ack != uart_noError) {
        TurnOnErrorLED();
    }

    bsl_err = Host_BSL_getResponse();
    return (bsl_err);
}

//*****************************************************************************
//
// ! Host_BSL_writeMemory
// ! Writes memory section to target
//
//*****************************************************************************
BSL_error_t Host_BSL_writeMemory(
    uint32_t addr, const uint8_t *data, uint32_t len)
{
    BSL_error_t bsl_err = eBSL_success;
    uart_error_t uart_ack;
    uint16_t ui16DataLength;
    uint16_t ui16PayloadSize;
    uint16_t ui16PacketSize;
    uint32_t ui32CRC;
    uint16_t ui16BytesToWrite = len;
    uint32_t TargetAddress    = addr;

    //  pSection->checksum = softwareCRC(pSection->pMemory, pSection->mem_size);

    while (ui16BytesToWrite > 0) {
        delay_cycles(2000000);  //allow target deal with the packet send before

        if (ui16BytesToWrite >= MAX_PAYLOAD_DATA_SIZE)
            ui16DataLength = MAX_PAYLOAD_DATA_SIZE;
        else
            ui16DataLength = ui16BytesToWrite;

        ui16BytesToWrite = ui16BytesToWrite - ui16DataLength;

        // Add (1byte) command + (4 bytes)ADDRS = 5 bytes to the payload
        ui16PayloadSize = (CMD_BYTE + ADDRS_BYTES + ui16DataLength);

        BSL_TX_buffer[0] = PACKET_HEADER;
        BSL_TX_buffer[1] =
            LSB(ui16PayloadSize);  // typically 4 + MAX_PAYLOAD SIZE
        BSL_TX_buffer[2] = MSB(ui16PayloadSize);
        BSL_TX_buffer[3] = (uint8_t) CMD_PROGRAMDATA;
        *(uint32_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = TargetAddress;

        // Bump up the target address by 2x the number of bytes sent for the next packet
        TargetAddress += ui16DataLength;

        // Copy the data into the BSL_RX_buffer
        memcpy(&BSL_TX_buffer[HDR_LEN_CMD_BYTES + ADDRS_BYTES], data,
            ui16DataLength);

        data += ui16DataLength;

        // Calculate CRC on the PAYLOAD
        ui32CRC = softwareCRC(&BSL_TX_buffer[3], ui16PayloadSize);

        // Calculate the packet length
        ui16PacketSize = HDR_LEN_CMD_BYTES + ADDRS_BYTES + ui16DataLength;

        // Insert the CRC into the packet at the end
        *(uint32_t *) &BSL_TX_buffer[ui16PacketSize] = ui32CRC;

        // Write the packet to the target
        uart_ack = UART_writeBuffer(BSL_TX_buffer, ui16PacketSize + CRC_BYTES);
        if (uart_ack != uart_noError) {
            TurnOnErrorLED();
        }

        // Check operation was complete
        bsl_err = Host_BSL_getResponse();
        if (bsl_err != eBSL_success) break;

    }  // end while

    return (bsl_err);
}

//*****************************************************************************
// ! Host_BSL_StartApp
// ! Start the new application
//
//*****************************************************************************
BSL_error_t Host_BSL_StartApp(void)
{
    BSL_error_t bsl_err = eBSL_success;
    uart_error_t uart_ack;
    uint32_t ui32CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_START_APP;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui32CRC = softwareCRC(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint32_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui32CRC;

    // Write the packet to the target
    uart_ack = UART_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    if (uart_ack != uart_noError) {
        TurnOnErrorLED();
    }
    return (bsl_err);
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

//*****************************************************************************
//
// ! Host_BSL_getResponse
// ! For those function calls that don't return specific data.
// ! Returns errors.
//
//*****************************************************************************
BSL_error_t Host_BSL_getResponse(void)
{
    BSL_error_t bsl_err = eBSL_success;

    UART_readBuffer(BSL_RX_buffer, (HDR_LEN_CMD_BYTES + ACK_BYTE + CRC_BYTES));
    //   Get ACK value
    bsl_err = BSL_RX_buffer[HDR_LEN_CMD_BYTES + ACK_BYTE - 1];
    //   Return ACK value
    return (bsl_err);
}
