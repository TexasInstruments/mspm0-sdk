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
#include <bsl_i2c.h>
#include <i2c.h>

#include "stdio.h"
#include "string.h"
#include "ti_msp_dl_config.h"

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
     * PA3 NRST          L -----------------------+
     *
     *                   H        +------------------------------+
     * PA7 Invoke GPIO   L--------+                              +----
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
    i2c_error_t i2c_ack;
    uint8_t buf[1] = {0x22};
    i2c_ack        = I2C_writeBuffer(buf, 1);
}

/*
 * Turn on the error LED
 */
void TurnOnErrorLED(void)
{
    DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_Error_PIN);
}
//*****************************************************************************
//
// ! Host_BSL_Connection
// ! Need to send first to build connection with target
//
//*****************************************************************************
uint8_t test_G;
BSL_error_t Host_BSL_Connection(void)
{
    BSL_error_t bsl_err = eBSL_success;
    i2c_error_t i2c_ack;
    uint16_t ui16CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_CONNECTION;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui16CRC = crc16_software(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui16CRC;
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + 2] = 0x0000;

    i2c_ack = I2C_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    I2C_readBuffer(BSL_RX_buffer, 1);
    if (i2c_ack != uart_noError) {
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
    i2c_error_t i2c_ack;
    uint16_t ui16CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_GET_ID;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui16CRC = crc16_software(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui16CRC;
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + 2] = 0x0000;
    // Write the packet to the target
    i2c_ack = I2C_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    if (i2c_ack != uart_noError) {
        TurnOnErrorLED();
    }

    I2C_readBuffer(BSL_RX_buffer, HDR_LEN_CMD_BYTES + ID_BACK + CRC_BYTES + 1);
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
    i2c_error_t i2c_ack;
    uint16_t ui16CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(PASSWORD_SIZE + CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_RX_PASSWORD;

    memcpy(&BSL_TX_buffer[4], pPassword, PASSWORD_SIZE);

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui16CRC = crc16_software(&BSL_TX_buffer[3], PASSWORD_SIZE + CMD_BYTE);

    // Insert the CRC into the packet
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + PASSWORD_SIZE] = ui16CRC;
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + PASSWORD_SIZE + 2] = 0x0000;

    // Write the packet to the target
    i2c_ack = I2C_writeBuffer(
        BSL_TX_buffer, HDR_LEN_CMD_BYTES + PASSWORD_SIZE + CRC_BYTES);
    if (i2c_ack != uart_noError) {
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
    i2c_error_t i2c_ack;
    uint16_t ui16CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_MASS_ERASE;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui16CRC = crc16_software(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui16CRC;
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + 2] = 0x0000;

    // Write the packet to the target
    i2c_ack = I2C_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    if (i2c_ack != uart_noError) {
        TurnOnErrorLED();
    }

    I2C_readBuffer(BSL_RX_buffer, 1);
    delay_cycles(2000000);
    I2C_readBuffer(BSL_RX_buffer, HDR_LEN_CMD_BYTES + MSG + CRC_BYTES);
    bsl_err = BSL_RX_buffer[HDR_LEN_CMD_BYTES + MSG - 1];
    //  bsl_err = Host_BSL_getResponse();
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
    i2c_error_t i2c_ack;
    uint16_t ui16DataLength;
    uint16_t ui16PayloadSize;
    uint16_t ui16PacketSize;
    uint16_t ui16CRC;
    uint32_t ui16BytesToWrite = len;
    uint32_t TargetAddress    = addr;

    //  pSection->checksum = softwareCRC(pSection->pMemory, pSection->mem_size);

    while (ui16BytesToWrite > 0) {
        //        delay_cycles(1000000); //allow target deal with the packet send before

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
        ui16CRC = crc16_software(&BSL_TX_buffer[3], ui16PayloadSize);


        // Calculate the packet length
        ui16PacketSize = HDR_LEN_CMD_BYTES + ADDRS_BYTES + ui16DataLength;

        // Insert the CRC into the packet at the end
        *(uint16_t *) &BSL_TX_buffer[ui16PacketSize] = ui16CRC;
        *(uint16_t *) &BSL_TX_buffer[ui16PacketSize + 2] = 0x00;

        // Write the packet to the target
        i2c_ack = I2C_writeBuffer(BSL_TX_buffer, ui16PacketSize + CRC_BYTES);
        if (i2c_ack != uart_noError) {
            TurnOnErrorLED();
        }

        // Check operation was complete
        //       bsl_err = Host_BSL_getResponse();
        I2C_readBuffer(BSL_RX_buffer, 1);
        delay_cycles(100000);
        I2C_readBuffer(BSL_RX_buffer, (HDR_LEN_CMD_BYTES + MSG + CRC_BYTES));
        bsl_err = BSL_RX_buffer[HDR_LEN_CMD_BYTES + MSG - 1];
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
    i2c_error_t i2c_ack;
    uint16_t ui16CRC;

    BSL_TX_buffer[0] = (uint8_t) PACKET_HEADER;
    BSL_TX_buffer[1] = LSB(CMD_BYTE);
    BSL_TX_buffer[2] = 0x00;
    BSL_TX_buffer[3] = CMD_START_APP;

    // Calculate CRC on the PAYLOAD (CMD + Data)
    ui16CRC = crc16_software(&BSL_TX_buffer[3], CMD_BYTE);
    // Insert the CRC into the packet
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES] = ui16CRC;
    *(uint16_t *) &BSL_TX_buffer[HDR_LEN_CMD_BYTES + 2] = 0x0000;

    // Write the packet to the target
    i2c_ack = I2C_writeBuffer(BSL_TX_buffer, HDR_LEN_CMD_BYTES + CRC_BYTES);
    I2C_readBuffer(BSL_RX_buffer, 1);
    if (i2c_ack != uart_noError) {
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


uint16_t crc16_software(const uint8_t *data, uint8_t length)
{
    uint16_t wCRCin = 0xFFFF;
    uint16_t wCPoly  = 0x1021;
    uint8_t  wChar = 0;

    while (length--)
    {
        wChar = *(data++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= wChar << 8;
        for(int i = 0;i < 8;i++)
        {
            if(wCRCin & 0x8000)
                wCRCin = (wCRCin << 1) ^ wCPoly;
            else
                wCRCin = wCRCin << 1;
        }
    }
    InvertUint16(&wCRCin,&wCRCin);

    return wCRCin;
}


void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
{
    int i;
    unsigned char tmp[4];
    tmp[0] = 0;
    for (i = 0; i< 8; i++)
    {
        if (srcBuf[0] & (1 << i))
            tmp[0] |= 1 << (7 - i);
    }
    dBuf[0] = tmp[0];

}

void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf)
{
    int i;
    unsigned short tmp[4];
    tmp[0] = 0;
    for (i = 0; i< 16; i++)
    {
        if (srcBuf[0] & (1 << i))
            tmp[0] |= 1 << (15 - i);
    }
    dBuf[0] = tmp[0];

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

    I2C_readBuffer(
        BSL_RX_buffer, (ACK_BYTE + HDR_LEN_CMD_BYTES + MSG + CRC_BYTES));
    //   Get ACK value
    bsl_err = BSL_RX_buffer[ACK_BYTE + HDR_LEN_CMD_BYTES + MSG - 1];
    //   Return ACK value
    return (bsl_err);
}
