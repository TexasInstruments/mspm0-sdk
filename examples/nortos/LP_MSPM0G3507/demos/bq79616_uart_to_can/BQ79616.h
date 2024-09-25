/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#ifndef BQ79616_H_
#define BQ79616_H_

#include "B0_reg.h"
#include "ti_msp_dl_config.h"

/********************************************************************************
 * User Naming Definition
 */
/* Device IO definition */
#define BQ79616_UART_INST UART_0_INST
#define BQ79616_NFAULT_PORT GPIO_GRP_0_PORT
#define BQ79616_NFAULT_PIN GPIO_GRP_0_PIN_0_PIN
#define BQ79616_TX_PORT GPIO_UART_0_TX_PORT
#define BQ79616_TX_PIN GPIO_UART_0_TX_PIN
#define BQ79616_TX_IOMUX GPIO_UART_0_IOMUX_TX
#define BQ79616_TX_IOMUX_FUNC GPIO_UART_0_IOMUX_TX_FUNC

/* BQ79616 Device Definition */
/* boards in stack */
#define TOTALBOARDS 2
/* maximum number of bytes to be read from the devices (for array creation) */
#define MAXBYTES (16 * 2)
/* device + uC baudrate */
#define BAUDRATE 1000000
/* define for Busclk per us */
#define CLKper CPUCLK_FREQ / 1000000

#define CRC_CHECK_ENABLE false

/* Read/Write Operation Definition */
#define FRMWRT_SGL_R 0x00      //single device READ
#define FRMWRT_SGL_W 0x10      //single device WRITE
#define FRMWRT_STK_R 0x20      //stack READ
#define FRMWRT_STK_W 0x30      //stack WRITE
#define FRMWRT_ALL_R 0x40      //broadcast READ
#define FRMWRT_ALL_W 0x50      //broadcast WRITE
#define FRMWRT_REV_ALL_W 0x60  //broadcast WRITE reverse direction

#define BYTE uint8_t
/********************************************************************************
 * Function Prototypes
 */
int WriteReg(
    BYTE bID, uint16_t wAddr, uint64_t dwData, BYTE bLen, BYTE bWriteType);
int ReadReg(BYTE bID, uint16_t wAddr, BYTE *pData, BYTE bLen,
    uint32_t dwTimeOut, BYTE bWriteType);

void BQ79616_Wake(void);
void AutoAddress(void);
void ReverseAddressing(void);

void RunCB(void);

bool GetFaultStat(void);
void ResetAllFaults(BYTE bID, BYTE bWriteType);
void MaskAllFaults(BYTE bID, BYTE bWriteType);

#endif /* BQ79606_H_ */
