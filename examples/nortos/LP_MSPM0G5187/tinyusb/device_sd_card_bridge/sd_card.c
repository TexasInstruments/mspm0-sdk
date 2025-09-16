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

#include "sd_card.h"
#include <string.h>
#include "bsp/board_api.h"
#include "ti_msp_dl_config.h"

/* Utilized to reverse endianness of data before throwing it inside mspm0SdCard struct */
void reverseByteOrder(uint8_t* dest, const uint8_t* src, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        dest[i] = src[length - 1 - i];
    }
}

uint8_t sendSdCardCommand(uint8_t cmd, uint32_t arg, uint8_t crc,
    uint8_t token, uint8_t* responseBuffer, uint32_t responseLength)
{
    /* Before a command is sent CS should be low */
    DL_GPIO_clearPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);

    /**
     * SD Card Command Frame in SPI Mode:
     *
     * Byte 0: 0b01XXXXXX (0x40+cmd) - Command byte (0=start bit, 1=transmission bit, X=command index)
     * Byte 1: Argument[31:24] - MSB of argument
     * Byte 2: Argument[23:16]
     * Byte 3: Argument[15:8]
     * Byte 4: Argument[7:0] - LSB of argument
     * Byte 5: CRC | 0x01 - 7-bit CRC with stop bit (bit 0 always 1)
     */
    uint8_t commandFrame[6];

    commandFrame[0] = (0x40 | cmd);
    commandFrame[1] = (arg >> 24) & 0xFF;
    commandFrame[2] = (arg >> 16) & 0xFF;
    commandFrame[3] = (arg >> 8) & 0xFF;
    commandFrame[4] = (arg) &0xFF;
    commandFrame[5] = crc;

    /* Transmit the entire command frame */
    for (uint8_t i = 0; i < 6; i++)
        DL_SPI_transmitDataBlocking8(SPI_0_INST, commandFrame[i]);

    volatile uint8_t response = 0xFF;

    /* Check for token response */
    for (uint16_t i = 0; i < CHECK_LIMIT; i++) {
        DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
        response = DL_SPI_receiveDataBlocking8(SPI_0_INST);
        if (response == token) break;
    }

    /* If response buffer is provided, fill it with the response */
    if ((responseBuffer != NULL) && (responseLength > 0)) {
        /* Get additional response bytes if needed (R3, R7 responses) */
        for (uint32_t i = 0; i < responseLength; i++) {
            DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
            responseBuffer[i] = DL_SPI_receiveDataBlocking8(SPI_0_INST);
        }
    }

    return response;
}

bool sdCardInit(SD_Card_t* sdCard)
{
    /* Before sending 10-16 cycles of 0xFF, ensure that CS pin is high */
    DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);

    /* Send 10-16 cycles of 0xFF */
    for (uint8_t i = 0; i < 10; i++) {
        DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
    }

    /* Send SET_IDLE command to SD card */
    volatile uint8_t response =
        sendSdCardCommand(GO_IDLE_STATE, 0, 0x95, 0x01, NULL, 0);

    if (response == 0x01)
        sdCard->cardIsInSpiMode = true;
    else
        return false;

    /* Send SEND_IF_COND to SD card */
    uint8_t r7Response[4];
    response = sendSdCardCommand(
        SEND_IF_COND, 0x000001AA, 0x87, 0x01, r7Response, sizeof(r7Response));

    /* Check if card is SD version 2.0 or later */
    if (response == 0x01) {
        /* Check voltage acceptance and echo pattern */
        if (r7Response[2] == 0x01 && r7Response[3] == 0xAA) {
            /* If response echos back properly, the SD Card is version 2.x */
            sdCard->cardIsV2_0 = true;
        } else {
            /* If first byte is proper but unable to echo back, then return */
            return false;
        }
    } else {
        /* SD Card is of version 1.x */
        sdCard->cardIsV2_0 = false;
    }

    /* Pre-configure command, argument, and crc before attempting to power up the card */
    uint32_t startTime = board_millis();
    const uint8_t cmd =
        mspm0SdCard.cardIsV2_0 ? SEND_OP_COND_CMD : SEND_OP_CMD;
    const uint32_t argument = mspm0SdCard.cardIsV2_0 ? 0x40000000 : 0;
    const uint8_t crc       = mspm0SdCard.cardIsV2_0 ? 0x77 : 0xFF;

    do {
        /* Send APP_CMD */
        sendSdCardCommand(APP_CMD, 0, 0x65, 0x01, NULL, 0);
        /* For V2.0 cards, use SEND_OP_COND_CMD with HCS bit set, otherwise for V1.x cards, use SEND_OP_CMD */
        response = sendSdCardCommand(cmd, argument, crc, 0x00, NULL, 0);
    } while (
        (response != 0x00) && ((board_millis() - startTime) <= TIMEOUT_5S));

    /* If response is not 0x00, then return */
    if (response != 0x00) return false;

    /* Send read OCR command */
    uint8_t ocrBuffer[4];
    response = sendSdCardCommand(
        READ_OCR, 0, 0xFD, 0x00, ocrBuffer, sizeof(sdCard->OCR));

    /* Check OCR register */
    if (response == 0x00) {
        reverseByteOrder(sdCard->OCR.raw, ocrBuffer, sizeof(sdCard->OCR));

        /* Check to see if SD card has been powered up */
        if (sdCard->OCR.ocrBits.cardPwrUpStat == true) {
            sdCard->cardIsPowered = true;
            /* If capacity is true, then the SD card is of high-capacity */
            if (sdCard->OCR.ocrBits.cardCapacityStat == true)
                sdCard->cardIsSDHC_SDXC = true;
        } else {
            return false;
        }
    } else {
        return false;
    }

    /* For standard capacity cards ensure block length is always 512 */
    if (sdCard->cardIsSDHC_SDXC == 0)
        response =
            sendSdCardCommand(SET_BLOCKLEN, 0x00000200, 0xFF, 0x00, NULL, 0);

    /* If response receive is invalid then return */
    if (response != 0x00) return false;

    /* Check CSD register */
    uint8_t csdBuffer[16];
    response = sendSdCardCommand(
        READ_CSD, 0, 0xFD, 0xFE, csdBuffer, sizeof(sdCard->CSD));

    /* Read results from CSD register */
    if (response == 0xFE) {
        reverseByteOrder(sdCard->CSD.raw, csdBuffer, sizeof(sdCard->CSD));
        /* Check capacity type of SD card*/
        if (sdCard->cardIsSDHC_SDXC == true) {
            sdCard->blockSize  = (1 << sdCard->CSD.csdHcBits.READ_BL_LEN);
            sdCard->blockCount = (((sdCard->CSD.csdHcBits.C_SIZEH << 16) |
                                      sdCard->CSD.csdHcBits.C_SIZEL) +
                                     1) *
                                 1024;
        } else {
            sdCard->blockSize = (1 << sdCard->CSD.csdSdBits.READ_BL_LEN);
            sdCard->blockCount =
                (((sdCard->CSD.csdSdBits.C_SIZEH << 2) |
                     sdCard->CSD.csdSdBits.C_SIZEL) +
                    1) *
                (1 << (sdCard->CSD.csdSdBits.C_SIZE_MULT + 2));
        }
    } else {
        return false;
    }

    /* Check CID register */
    uint8_t cidBuffer[16];
    response = sendSdCardCommand(
        READ_CID, 0, 0xFD, 0xFE, cidBuffer, sizeof(sdCard->CID));

    /* Check CID read response */
    if (response == 0xFE)
        reverseByteOrder(sdCard->CID.raw, cidBuffer, sizeof(sdCard->CID));
    else
        return false;

    /* Card is now initialized for read/write operations */
    sdCard->cardIsInitialized = true;

    /* After operation is complete, raise CS pin high */
    DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);

    return true;
}

bool sdCardReadBlock(SD_Card_t* sdCard, uint32_t blockAddr)
{
    /* If SD card has not been intialized then return */
    if (sdCard->cardIsInitialized == false) return false;

    /*
     * Addressing will differ between standard and high capacity cards.
     * For high capacity cards they will directly utilize the block address
     * while standard capacity cards will utilize byte addressing.
     */
    const uint32_t address =
        sdCard->cardIsSDHC_SDXC ? blockAddr : blockAddr * 512;

    /* Send READ_SINGLE_BLOCK command */
    volatile uint8_t response = sendSdCardCommand(READ_SINGLE_BLOCK, address,
        0x00, 0xFE, sdCard->buffer, sizeof(sdCard->buffer));

    /* If an invalid response is received then set CS pin high and return */
    if (response != 0xFE) {
        DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
        return false;
    }

    /* Read and discard CRC (2 bytes) */
    DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
    DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);

    /* Poll the SD card until response of 0xFF is received or 5 s has passed */
    uint32_t startTime = board_millis();
    response           = 0;

    do {
        DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
        response = DL_SPI_receiveDataBlocking8(SPI_0_INST);
    } while (
        (response != 0xFF) && ((board_millis() - startTime) <= TIMEOUT_5S));

    /* If an invalid response is received then set CS pin high and return */
    if (response != 0xFF) {
        DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
        return false;
    }

    /* Make sure SPI is no longer busy for clean data */
    while (DL_SPI_isBusy(SPI_0_INST))
        ;

    /* After operation is complete, raise CS pin high */
    DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);

    return true;
}

bool sdCardWriteBlock(SD_Card_t* sdCard, uint32_t blockAddr, uint8_t* buffer)
{
    /* If SD card has not been intialized then return */
    if (sdCard->cardIsInitialized == false) return false;

    /*
     * Addressing will differ between standard and high capacity cards.
     * For high capacity cards they will directly utilize the block address
     * while standard capacity cards will utilize byte addressing.
     */
    const uint32_t address =
        sdCard->cardIsSDHC_SDXC ? blockAddr : blockAddr * 512;

    /* Send WRITE_BLOCK command*/
    volatile uint8_t response =
        sendSdCardCommand(WRITE_BLOCK, address, 0x00, 0x00, NULL, 0);

    /* If proper response is not received then return */
    if (response != 0x00) {
        DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
        return false;
    }

    /* Send start token */
    DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFE);

    for (uint16_t i = 0; i < 512; i++)
        DL_SPI_transmitDataBlocking8(SPI_0_INST, buffer[i]);

    /*
     * Data response token: xxx0sss1
     * where xxx = don't care, sss = status (010 = Data accepted)
     */
    uint32_t startTime = board_millis();
    response           = 0;

    /* Wait for data response token */
    do {
        DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
        response = DL_SPI_receiveDataBlocking8(SPI_0_INST);
    } while (((response & 0x1F) != 0x05) &&
             ((board_millis() - startTime) <= TIMEOUT_5S));

    if ((response & 0x1F) != 0x05) {
        /* After operation has failed, raise CS pin high */
        DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
        return false;
    }

    /* Wait for the card to complete the write operation */
    startTime = board_millis();
    response  = 0;

    do {
        DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);
        response = DL_SPI_receiveDataBlocking8(SPI_0_INST);
    } while (
        (response != 0xFF) && ((board_millis() - startTime) <= TIMEOUT_5S));

    if (response != 0xFF) {
        /* After operation has failed, raise CS pin high */
        DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
        return false;
    }

    /* Additional dummy byte to provide clock cycles */
    DL_SPI_transmitDataBlocking8(SPI_0_INST, 0xFF);

    while (DL_SPI_isBusy(SPI_0_INST))
        ;

    /* After operation is complete, raise CS pin high */
    DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);

    return true;
}
