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

#ifndef BQ78706_H_
#define BQ78706_H_

#include "bq78706_B0_reg.h"
#include "bq78706_data_types.h"
#include "ti_msp_dl_config.h"

/****************************************************************
 *  User configuration area
 */
/* Set SPI_COMM  to 'TRUE' if the communication type used is SPI, for UART keep it 'FALSE' */
#define SPI_COMM FALSE

#define READ_MAIN_ADC FALSE

/* NO NEED TO CHANGE 'UART_COMM', setting the value of 'SPI_COMM' is enough */
#if SPI_COMM == FALSE
#define UART_COMM TRUE
#else
#define UART_COMM FALSE
#endif

#define IS_79600_BRIDGE FALSH
#define FORWARD 0
#define REVERSE 1

#define AUTO_ADDR FORWARD

#define GPIO_READ_EN TRUE
#define MAXBYTES \
    (16 *        \
        2)  //maximum number of bytes to be read from the devices (for array creation)
#define BAUDRATE 1000000  //device + uC baudrate

#if IS_79600_BRIDGE == TRUE
#define TOTALBOARDS 4  //boards in stack + base board
#else
#define TOTALBOARDS 4  //boards in stack
#endif

#define ACTIVECHANNELS 13
#define GADC_ACTIVECHANNEL 11
#define BRIDGEDEVICE 1

/* Device IO definition */
#define BQ78706_UART_INST UART_0_INST
#define BQ78706_NFAULT_PORT IOBQ78706_PORT
#define BQ78706_NFAULT_PIN IOBQ78706_Fault2AFE_PIN
#define BQ78706_TX_PORT GPIO_UART_0_TX_PORT
#define BQ78706_TX_PIN GPIO_UART_0_TX_PIN
#define BQ78706_TX_IOMUX GPIO_UART_0_IOMUX_TX
#define BQ78706_TX_IOMUX_FUNC GPIO_UART_0_IOMUX_TX_FUNC

/* define for Busclk per us */
// #define CLKperus (CPUCLK_FREQ / 1000000)
#define CLKperus 80

/****************************************************************
 *  Code definition area
 */
#define FRMWRT_SGL_R 0x00      //single device READ
#define FRMWRT_SGL_W 0x10      //single device WRITE
#define FRMWRT_STK_R 0x20      //stack READ
#define FRMWRT_STK_W 0x30      //stack WRITE
#define FRMWRT_ALL_R 0x40      //broadcast READ
#define FRMWRT_ALL_W 0x50      //broadcast WRITE
#define FRMWRT_REV_ALL_W 0x60  //broadcast WRITE reverse direction

/**
 * Parameter
 * Resolution for VC/CB/GPIO/D1/D2/REF4V/TSREF
 */
#define VLSB_STANDARD 100       // uV/LSB
#define VLSB_STANDARD_F 0.0001  // V/LSB
#define VLSB_GPIO_RATIO 0.005   //%/LSB
/**
 * @brief   FORMAT WRITE DATA, SEND TO BE COMBINED WITH REST OF FRAME
 *
 * @param   bID         device address
 * @param   wAddr       register start address
 * @param   dwData      data bytes
 * @param   bLen        data length
 * @param   bWriteType  write type (single, broadcast, stack)
 * @return  Number of bytes sent.
 */
int WriteRegUART(uint8_t bID, uint16_t wAddr, uint64_t dwData, uint8_t bLen,
    uint8_t bWriteType);
int ReadRegUART(uint8_t bID, uint16_t wAddr, uint8_t *pData, uint8_t bLen,
    uint32_t dwTimeOut, uint8_t bWriteType);

#if SPI_COMM == TRUE
#define ReadReg(bID, wAddr, pData, bLen, dwTimeOut, bWriteType) \
    SpiReadReg(bID, wAddr, pData, bLen, dwTimeOut, bWriteType);
#define WriteReg(bID, wAddr, dwData, bLen, bWriteType) \
    SpiWriteReg(bID, wAddr, dwData, bLen, bWriteType);
#endif
#if UART_COMM == TRUE
#define ReadReg(bID, wAddr, pData, bLen, dwTimeOut, bWriteType) \
    ReadRegUART(bID, wAddr, (uint8_t *) pData, bLen, dwTimeOut, bWriteType)
#define WriteReg(bID, wAddr, dwData, bLen, bWriteType) \
    WriteRegUART(bID, wAddr, dwData, bLen, bWriteType)
#endif

// Function Prototypes
void Wake78706(void);
void SD78706(void);
void StA78706(void);
// void HWRST78706(void);
// void AutoAddress(void);
BOOL GetFaultStat();

uint16_t CRC16(uint8_t *pBuf, int nLen);

//int  WriteReg(uint8_t bID, uint16_t wAddr, uint64_t dwData, uint8_t bLen, uint8_t bWriteType);
//int  ReadReg(uint8_t bID, uint16_t wAddr, uint16_t * pData, uint8_t bLen, uint32_t dwTimeOut, uint8_t bWriteType);

int WriteFrame(uint8_t bID, uint16_t wAddr, uint8_t *pData, uint8_t bLen,
    uint8_t bWriteType);
int ReadFrameReq(
    uint8_t bID, uint16_t wAddr, uint8_t bByteToReturn, uint8_t bWriteType);
int WaitRespFrame(uint8_t *pFrame, uint32_t bLen, uint32_t dwTimeOut);

void delayms(uint32_t ms);
void delayus(uint32_t us);

void RunCB();
void ReverseAddressing();

#if UART_COMM == TRUE
int ReadDeviceStat2(uint8_t *response_frame);
#elif SPI_COMM == TRUE
int ReadDeviceStat2(uint16_t *response_frame);
#endif
#if UART_COMM == TRUE
void ReadCoulumbCount(uint8_t *response_frame, uint8_t rshunt_u8);
#elif SPI_COMM == TRUE
void ReadCoulumbCount(uint16_t *response_frame, uint8_t rshunt_u8);
#endif
void ConfigureOverCurrent();
void set_SW_CTRL(uint8_t pinNum, uint8_t value);
void set_GPIO_Out_Val(uint8_t gpioNum, BOOL val);
void set_GPIO_As_PWM(uint8_t gpioNum, BOOL val);

#endif /* BQ78706_H_ */
//EOF
