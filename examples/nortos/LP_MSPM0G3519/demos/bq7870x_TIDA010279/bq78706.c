/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include <stdio.h>
#include <string.h>

#include "bq78706.h"
#include "mspm0_uart.h"

#define RESPONSE_DATA_START_INDEX 4U
#define ACCUMULATION_DATA_SIZE 4U
#define SW_OUTPUT_OFF 0U
#define SW_OUTPUT_LOW 1U
#define SW_OUTPUT_HIGH 2U
#define VLSB_CS 0.1
#define TCS_REFRESH 64U

//GLOBAL VARIABLES (use these to avoid stack overflows by creating too many function variables)
//avoid creating variables/arrays in functions, or you will run out of stack space quickly
#if UART_COMM == TRUE
uint8_t
    response_frame2[(MAXBYTES + 6) *
                    TOTALBOARDS];  //response frame to be used by every read
uint8_t
    fault_frame[39 * TOTALBOARDS];  //hold fault frame if faults are printed
#elif SPI_COMM == TRUE
uint16_t
    response_frame2[(MAXBYTES + 6) *
                    TOTALBOARDS];  //response frame to be used by every read
uint16_t
    fault_frame[39 * TOTALBOARDS];  //hold fault frame if faults are printed
#endif

int currentCell  = 0;
int currentBoard = 0;
uint8_t bReturn  = 0;
int bRes         = 0;
uint8_t bBuf[8];
uint8_t pFrame[64];
uint16_t wCRC   = 0;
uint16_t wCRC16 = 0;
int crc_i       = 0;

char resultNames[24][10] = {"VF2", "VF1", "CP", "RSVD", "CURRENT1", "CURRENT2",
    "OC1", "OC2", "RSVD", "GPIO1", "GPIO2", "GPIO3", "GPIO4", "GPIO5", "GPIO6",
    "GPIO7", "GPIO8", "GPIO9", "GPIO10", "GPIO11", "GPIO12", "GPIO13",
    "GPIO14", "GPIO15"};

/********************************************************************************
 * Send negative pulse via UART Tx pin
 * BQ78706 Datasheet: Timing Requirements - PING
 * tHLD_WAKE        :       3 ~ 5 ms, using   4 ms
 * tHLD_SD_HW_RST   :     40 ~ 50 ms, using  45 ms
 * tUART(StA)       :   250 ~ 300 us, using 275 us
 */
void Wake78706(void)
{
    /* Disable UART function */
    DL_UART_disable(BQ78706_UART_INST);
    /* Redirect Pin to GPIO */
    DL_GPIO_initDigitalOutput(BQ78706_TX_IOMUX);
    /* Set output to low */
    DL_GPIO_clearPins(BQ78706_TX_PORT, BQ78706_TX_PIN);
    /* Enable GPIO output */
    DL_GPIO_enableOutput(BQ78706_TX_PORT, BQ78706_TX_PIN);

    /* WAKE ping = 3ms to 5ms */
    delay_cycles(CLKperus * 1000 * 4);

    DL_GPIO_initPeripheralOutputFunction(
        BQ78706_TX_IOMUX, BQ78706_TX_IOMUX_FUNC);
    /* Enable UART function */
    DL_UART_enable(BQ78706_UART_INST);
}

// SD_HW_RST ping
void SD78706(void)
{
    /* Disable UART function */
    DL_UART_disable(BQ78706_UART_INST);
    /* Redirect Pin to GPIO */
    DL_GPIO_initDigitalOutput(BQ78706_TX_IOMUX);
    /* Set output to low */
    DL_GPIO_clearPins(BQ78706_TX_PORT, BQ78706_TX_PIN);
    /* Enable GPIO output */
    DL_GPIO_enableOutput(BQ78706_TX_PORT, BQ78706_TX_PIN);

    /* SD ping = 40ms to 50ms */
    delay_cycles(CLKperus * 1000 * 45);

    DL_GPIO_initPeripheralOutputFunction(
        BQ78706_TX_IOMUX, BQ78706_TX_IOMUX_FUNC);
    /* Enable UART function */
    DL_UART_enable(BQ78706_UART_INST);
}

void StA78706(void)
{
    /* Disable UART function */
    DL_UART_disable(BQ78706_UART_INST);
    /* Redirect Pin to GPIO */
    DL_GPIO_initDigitalOutput(BQ78706_TX_IOMUX);
    /* Set output to low */
    DL_GPIO_clearPins(BQ78706_TX_PORT, BQ78706_TX_PIN);
    /* Enable GPIO output */
    DL_GPIO_enableOutput(BQ78706_TX_PORT, BQ78706_TX_PIN);

    /* StA ping = 250us to 300us */
    delay_cycles(CLKperus * 275);

    DL_GPIO_initPeripheralOutputFunction(
        BQ78706_TX_IOMUX, BQ78706_TX_IOMUX_FUNC);
    /* Enable UART function */
    DL_UART_enable(BQ78706_UART_INST);
}

/********************************************************************************
 * CRC check
 * ITU_T polynomial: x^16 + x^15 + x^2 + 1
 * CRC-16/MODBUS
 *  Poly = 0x8005
 *  Init = 0xFFFF
 *  RefIn = RefOut = true
 *  XorOut = 0x0000
 */
const uint16_t crc16_table[256] = {0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301,
    0x03C0, 0x0280, 0xC241, 0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1,
    0xC481, 0x0440, 0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81,
    0x0E40, 0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00,
    0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1,
    0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 0xD201, 0x12C0, 0x1380,
    0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 0xF001, 0x30C0, 0x3180, 0xF141,
    0x3300, 0xF3C1, 0xF281, 0x3240, 0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501,
    0x35C0, 0x3480, 0xF441, 0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0,
    0x3E80, 0xFE41, 0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881,
    0x3840, 0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401,
    0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1,
    0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 0xA001, 0x60C0, 0x6180,
    0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740,
    0xA501, 0x65C0, 0x6480, 0xA441, 0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01,
    0x6FC0, 0x6E80, 0xAE41, 0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1,
    0xA881, 0x6840, 0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80,
    0xBA41, 0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200,
    0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1,
    0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 0x9601, 0x56C0, 0x5780,
    0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41,
    0x5F00, 0x9FC1, 0x9E81, 0x5E40, 0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901,
    0x59C0, 0x5880, 0x9841, 0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1,
    0x8A81, 0x4A40, 0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80,
    0x8C41, 0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

uint16_t CRC16(uint8_t *pBuf, int nLen)
{
    uint16_t wCRC = 0xFFFF;
    int i;

    for (i = 0; i < nLen; i++) {
        wCRC ^= (*pBuf++) & 0x00FF;
        wCRC = crc16_table[wCRC & 0x00FF] ^ (wCRC >> 8);
    }

    return wCRC;
}

/********************************************************************************
 * BQ78706 WRITE AND READ FUNCTIONS
 */
// COMMAND FRAME GENERATE
int WriteFrame(uint8_t bID, uint16_t wAddr, uint8_t *pData, uint8_t bLen,
    uint8_t bWriteType)
{
    int bPktLen   = 0;
    uint8_t *pBuf = pFrame;

    memset(pFrame, 0x7F, sizeof(pFrame));
    /* Only include blen if it is a write; Writes are 0x90, 0xB0, 0xD0 */
    *pBuf++ = 0x80 | (bWriteType) | ((bWriteType & 0x10) ? bLen - 0x01 : 0x00);
    if (bWriteType == FRMWRT_SGL_R || bWriteType == FRMWRT_SGL_W) {
        *pBuf++ = (bID & 0x00FF);
    }
    *pBuf++ = (wAddr & 0xFF00) >> 8;
    *pBuf++ = wAddr & 0x00FF;

    while (bLen--) *pBuf++ = *pData++;

    bPktLen = pBuf - pFrame;

    wCRC    = CRC16(pFrame, bPktLen);
    *pBuf++ = wCRC & 0x00FF;
    *pBuf++ = (wCRC & 0xFF00) >> 8;
    bPktLen += 2;

    UART_Send(BQ78706_UART_INST, bPktLen, pFrame);

    return bPktLen;
}

int ReadFrameReq(
    uint8_t bID, uint16_t wAddr, uint8_t bByteToReturn, uint8_t bWriteType)
{
    bReturn = bByteToReturn - 1;

    if (bReturn > 127) return 0;

    return WriteFrame(bID, wAddr, &bReturn, 1, bWriteType);
}

#if UART_COMM == TRUE
/**
 * @brief   FORMAT WRITE DATA, SEND TO BE COMBINED WITH REST OF FRAME
 *
 * @param   bID         device address
 * @param   wAddr       register start address
 * @param   dwData      data bytes
 * @param   bLen        data length in uint8_t / byte
 * @param   bWriteType  write type (single, broadcast, stack)
 *
 * @return  Number of bytes sent.
*/
int WriteRegUART(uint8_t bID, uint16_t wAddr, uint64_t dwData, uint8_t bLen,
    uint8_t bWriteType)
{
    bRes = 0;
    memset(bBuf, 0, sizeof(bBuf));

    /**
     * @brief Smaller code size
     * run time: 9.55us
     * Optinal, for code flash optimization purpose
     */
#if 0
    int i = bLen;
    while (i > 0) {
        bBuf[i - 1] =
            (dwData & (0x00000000000000FF << ((bLen - i) * 8))) >>
            ((bLen - i) * 8);
        i--;
    }
    bRes = WriteFrame(bID, wAddr, bBuf, bLen, bWriteType);
#endif
    /**
     * @brief Faster run time
     * run time: 6.66us
     */
    switch (bLen) {
        case 1:
            bBuf[0] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 1, bWriteType);
            break;
        case 2:
            bBuf[0] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[1] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 2, bWriteType);
            break;
        case 3:
            bBuf[0] = (dwData & 0x0000000000FF0000) >> 16;
            bBuf[1] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[2] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 3, bWriteType);
            break;
        case 4:
            bBuf[0] = (dwData & 0x00000000FF000000) >> 24;
            bBuf[1] = (dwData & 0x0000000000FF0000) >> 16;
            bBuf[2] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[3] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 4, bWriteType);
            break;
        case 5:
            bBuf[0] = (dwData & 0x000000FF00000000) >> 32;
            bBuf[1] = (dwData & 0x00000000FF000000) >> 24;
            bBuf[2] = (dwData & 0x0000000000FF0000) >> 16;
            bBuf[3] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[4] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 5, bWriteType);
            break;
        case 6:
            bBuf[0] = (dwData & 0x0000FF0000000000) >> 40;
            bBuf[1] = (dwData & 0x000000FF00000000) >> 32;
            bBuf[2] = (dwData & 0x00000000FF000000) >> 24;
            bBuf[3] = (dwData & 0x0000000000FF0000) >> 16;
            bBuf[4] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[5] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 6, bWriteType);
            break;
        case 7:
            bBuf[0] = (dwData & 0x00FF000000000000) >> 48;
            bBuf[1] = (dwData & 0x0000FF0000000000) >> 40;
            bBuf[2] = (dwData & 0x000000FF00000000) >> 32;
            bBuf[3] = (dwData & 0x00000000FF000000) >> 24;
            bBuf[4] = (dwData & 0x0000000000FF0000) >> 16;
            bBuf[5] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[6] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 7, bWriteType);
            break;
        case 8:
            bBuf[0] = (dwData & 0xFF00000000000000) >> 56;
            bBuf[1] = (dwData & 0x00FF000000000000) >> 48;
            bBuf[2] = (dwData & 0x0000FF0000000000) >> 40;
            bBuf[3] = (dwData & 0x000000FF00000000) >> 32;
            bBuf[4] = (dwData & 0x00000000FF000000) >> 24;
            bBuf[5] = (dwData & 0x0000000000FF0000) >> 16;
            bBuf[6] = (dwData & 0x000000000000FF00) >> 8;
            bBuf[7] = dwData & 0x00000000000000FF;
            bRes    = WriteFrame(bID, wAddr, bBuf, 8, bWriteType);
            break;
        default:
            break;
    }
    DL_GPIO_clearPins(DBG_PORT, DBG_PA11_PIN);

    return bRes;
}

/**
 * @brief   GENERATE READ COMMAND FRAME AND THEN WAIT FOR RESPONSE DATA (INTERRUPT MODE FOR SCIRX)
 *
 * @param   bID         device address
 * @param   wAddr       register start address
 * @param   pData       byte frame pointer to store data
 * @param   bLen        data length
 * @param   dwTimeOut   timeout count (not in use)
 * @param   bWriteType  read type (single, broadcast, stack)
 *
 * @return  Number of bytes received.
*/
int ReadRegUART(uint8_t bID, uint16_t wAddr, uint8_t *pData, uint8_t bLen,
    uint32_t dwTimeOut, uint8_t bWriteType)
{
    uint32_t Timeout_count = dwTimeOut;
    uint32_t receivecount  = 0;

    bRes          = 0;
    Timeout_count = 5000;  // 5000 Timeout_count -> 2ms

    UART_clearRxFIFO(BQ78706_UART_INST);

    if (bWriteType == FRMWRT_SGL_R) {
        ReadFrameReq(bID, wAddr, bLen, bWriteType);
        memset(pData, 0, (uint8_t) sizeof(pData));

        receivecount =
            UART_Receive_TO(BQ78706_UART_INST, bLen + 6, pData, Timeout_count);

        bRes = bLen + 6;
    } else if (bWriteType == FRMWRT_STK_R) {
        ReadFrameReq(bID, wAddr, bLen, bWriteType);
        memset(pData, 0, (uint8_t) sizeof(pData));

        receivecount = UART_Receive_TO(BQ78706_UART_INST,
            (bLen + 6) * (TOTALBOARDS - 1), pData, Timeout_count);

        bRes = (bLen + 6) * (TOTALBOARDS - 1);
    } else if (bWriteType == FRMWRT_ALL_R) {
        ReadFrameReq(bID, wAddr, bLen, bWriteType);
        memset(pData, 0, (uint8_t) sizeof(pData));

        receivecount = UART_Receive_TO(
            BQ78706_UART_INST, (bLen + 6) * TOTALBOARDS, pData, Timeout_count);

        bRes = (bLen + 6) * TOTALBOARDS;
    } else {
        ReadFrameReq(bID, wAddr, bLen, bWriteType);
        memset(pData, 0, (uint8_t) sizeof(pData));

        receivecount = UART_Receive_TO(BQ78706_UART_INST,
            (bLen + 6) * (TOTALBOARDS - 1), pData, Timeout_count);

        bRes = (bLen + 6) * (TOTALBOARDS - 1);
    }

    //CHECK IF CRC IS CORRECT
    for (crc_i = 0; crc_i < bRes; crc_i += (bLen + 6)) {
        if (CRC16(&pData[crc_i], bLen + 6) != 0) {
            printf("BAD CRC\n");
        }
    }

    return receivecount;
}
#endif

/********************************************************************************
 * MISCELLANEOUS FUNCTIONS
 */
void delayms(uint32_t ms)
{
    if (ms == 0)
        return;
    else {
        delay_cycles(CLKperus * 1000 * ms);
    }
}

void delayus(uint32_t us)
{
    if (us == 0)
        return;
    else {
        delay_cycles(CLKperus * us);
    }
}

BOOL GetFaultStat()
{
    if (!DL_GPIO_readPins(BQ78706_NFAULT_PORT, BQ78706_NFAULT_PIN)) return 0;
    return 1;
}

void PrintAllFaults(uint8_t bID, uint8_t bWriteType)
{
    //PRINT 39 REGISTERS STARTING FROM FAULT_SUMMARY (INCLUDES RESERVED REGISTERS)
    printf("\n");
    currentBoard = 0;
    currentCell  = 0;
    memset(fault_frame, 0, sizeof(fault_frame));
    if (bWriteType == FRMWRT_ALL_R) {
        ReadReg(0, FAULT_SUMMARY, fault_frame, 39, 0, FRMWRT_ALL_R);
        for (currentBoard = 0; currentBoard < TOTALBOARDS - 1;
             currentBoard++) {
            printf("BOARD %d FAULTS (0x532 - 0x559):\t",
                TOTALBOARDS - currentBoard - 1);
            for (currentCell = 0; currentCell < 39; currentCell++) {
                printf("%02x ",
                    fault_frame[(currentBoard * (39 + 6)) + 4 + currentCell]);
            }
            printf("\n");
        }
    } else if (bWriteType == FRMWRT_SGL_R) {
        ReadReg(bID, FAULT_SUMMARY, fault_frame, 39, 0, FRMWRT_SGL_R);
        printf("BOARD %d FAULTS(0x532 - 0x559):\t", bID);
        for (currentCell = 0; currentCell < 39; currentCell++) {
            printf("%02x ", fault_frame[4 + currentCell]);
        }
        printf("\n");
    } else if (bWriteType == FRMWRT_STK_R) {
        ReadReg(0, FAULT_SUMMARY, fault_frame, 39, 0, FRMWRT_STK_R);
        for (currentBoard = 0; currentBoard < (TOTALBOARDS - 1);
             currentBoard++) {
            printf("BOARD %d FAULTS(0x532 - 0x559):\t",
                TOTALBOARDS - currentBoard - 1);
            for (currentCell = 0; currentCell < 39; currentCell++) {
                printf("%02x ",
                    fault_frame[(currentBoard * (39 + 6)) + 4 + currentCell]);
            }
            printf("\n");
        }
    } else {
        printf("ERROR: PrintAllFaults bWriteType incorrect\n");
    }
    printf("\n");
}

//RUN BASIC REVERSE ADDRESSING SEQUENCE
void ReverseAddressing(void)
{
    //CHANGE BASE DEVICE DIRECTION
    WriteReg(0, CONTROL1, 0x80, 1, FRMWRT_SGL_W);

    //CHANGE REST OF STACK DIRECTION
    WriteReg(0, CONTROL1, 0x80, 1, FRMWRT_REV_ALL_W);

    delay_cycles(CLKperus * 50);

    WriteReg(0, COMM_CTRL, 0x00, 1, FRMWRT_ALL_W);

    //DO NORMAL AUTO ADDRESS SEQUENCE, BUT FOR DIR1_ADDR
    //DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL (IF A DEVICE RESET OCCURED PRIOR TO THIS)
    WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);

    //ENABLE AUTO ADDRESSING MODE, WHILE KEEPING REVERSE DIRECTION
    WriteReg(0, CONTROL1, 0X81, 1, FRMWRT_ALL_W);

    //SET ADDRESSES FOR EVERY BOARD (REVERSE DIRECTION)
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        WriteReg(0, DIR1_ADDR, currentBoard, 1, FRMWRT_ALL_W);
        delay_cycles(CLKperus * 50);
        //ReadReg(currentBoard, DIR1_ADDR, response_frame2, 1, 0, FRMWRT_SGL_R);
        //delay_cycles(CLKperus * 50);
    }

    WriteReg(0, COMM_CTRL, 0x02, 1,
        FRMWRT_ALL_W);  //set everything as a stack device first

    /* if there's only 1 board, it's the base AND top of stack, so change it to those */
    if (TOTALBOARDS == 1) {
        WriteReg(0, COMM_CTRL, 0x01, 1, FRMWRT_STK_W);
    }
    /* otherwise set the base and top of stack individually */
    else {
        WriteReg(TOTALBOARDS - 1, COMM_CTRL, 0x03, 1, FRMWRT_SGL_W);
    }

    //SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
    ReadReg(0, OTP_ECC_DATAIN1, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN2, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN3, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN4, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN5, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN6, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN7, response_frame2, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN8, response_frame2, 1, 0, FRMWRT_STK_R);

    //OPTIONAL: read back all device addresses
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        ReadReg(currentBoard, DIR1_ADDR, response_frame2, 1, 0, FRMWRT_SGL_R);
        //printf("board %d\n",response_frame2[4]);
    }

    //RESET ANY COMM FAULT CONDITIONS FROM STARTUP
    WriteReg(0, FAULT_RST2, 0x03, 1, FRMWRT_STK_W);
}

/** @fn int ReadDeviceStat2(uint8_t *)
*   @brief Read the device stat register
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The function reads the Dev_stat2 register and returns if the Coulomb count
*   feature is enabled.
*/
#if UART_COMM == TRUE
int ReadDeviceStat2(uint8_t *response_frame)
#elif SPI_COMM == TRUE
int ReadDeviceStat2(uint16_t *response_frame)
#endif
{
    int retval = 0U;

    ReadReg(0, DEV_STAT2, response_frame, 1, 0, FRMWRT_STK_R);
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        printf("Dev_Stat2 val for stack dev id %d is 0x%x\n", currentBoard,
            response_frame[RESPONSE_DATA_START_INDEX + currentBoard]);
        /* Check if the Coulomb count function is running */
        if (0x1U !=
            ((response_frame[RESPONSE_DATA_START_INDEX + currentBoard] >>
                 0x5U) &
                0x1U)) {
            retval = -1;
            break;
        }
    }

    return retval;
}

/** @fn void ReadCoulumbCount(uint8_t *)
*   @brief Read the coulomb count accumulation data value
*   @note This function is empty by default.
*
*   This function is called periodically.
*   The function reads the accumulation data register and prints its value
*/

#if UART_COMM == TRUE
void ReadCoulumbCount(uint8_t *response_frame, uint8_t rshunt_u8)
#elif SPI_COMM == TRUE
void ReadCoulumbCount(uint16_t *response_frame, uint8_t rshunt_u8)
#endif
{
#if 0
    int i = 0U;
    uint32_t cc_acc_val_u32;
    uint8_t cc_cnt_u8;

    /* Do CC_CLR */
    WriteReg(0, CC_CTRL, 0x07, 1, WriteType);


    /* Read the CC Accumulation data ( 4 bytes for each stack device ) */
    ReadReg(0U, CC_ACC_HI, response_frame, ACCUMULATION_DATA_SIZE, 0U, ReadType);

    for ( i = RESPONSE_DATA_START_INDEX; i < (RESPONSE_DATA_START_INDEX + (TOTALBOARDS * ACCUMULATION_DATA_SIZE));
            i += ACCUMULATION_DATA_SIZE )
    {
       cc_acc_val_u32 = response_frame[i] << 24;
       cc_acc_val_u32 = response_frame[i + 1] << 16;
       cc_acc_val_u32 = response_frame[i + 2] << 8;
       cc_acc_val_u32 = response_frame[i + 3];
    }

    ReadReg(0U, CC_CNT, response_frame, 1U, 0U, ReadType);
    cc_cnt_u8 = response_frame[RESPONSE_DATA_START_INDEX];

    if ( cc_cnt_u8 < 0xFFU )
    {
        printf("value of Iaverage %f\n", (cc_acc_val_u32 * VLSB_CS ) / ( rshunt_u8 * cc_cnt_u8));
        printf("value of coulomb count %f\n", (cc_acc_val_u32 * VLSB_CS * TCS_REFRESH * cc_cnt_u8) / (rshunt_u8) );
    }
#endif
}

/** @fn void ConfigureOverCurrent(void)
*   @brief configures over current register on BQ79731 stack device
*   @note This function is empty by default.
*
*   This function is called at startup.
*   The function write 'DIAG_OC_CTRL1' and 'DIAG_OC_CTRL2' registers
*/
void ConfigureOverCurrent()
{
#if 0
    /* Set the over current diagnostic injection level */
    WriteReg(0, DIAG_OC_CTRL1, 0xFF, 1, WriteType);
    delay_cycles(CLKperus * 1000 * 10);
    /* Enable Over current diagnostics by setting 'DIAG_OC_MODE' and 'DIAG_OC_GO' */
    WriteReg(0, DIAG_OC_CTRL2, 0x03, 1, WriteType);
#endif
}
//EOF
