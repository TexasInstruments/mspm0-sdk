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
#include "bq78706_B0_reg.h"
#include "bq78706_app.h"

/**
 * BQ Device Delay Task fucntion group
 */
uint8_t BQDev_TaskStatus              = BQTaskStatus_IDLE;
volatile uint8_t BQDev_TaskDelayReady = 0;

/**
 * @brief Basic function group
 *
 */

/**
 * @brief Initialize the BQ register value to default
 *          When creat a new bq78706_dev_t variable,
 *          need to call this function to init this bq78706_dev_t
 *
 * @param pBQDev BQ register structure
 */
void BQParamInit(bq78706_dev_t *pBQDev, bq78706_syssta_t *pBQSta)
{
    //All register default value is 0x00.

    // NVM BACKED REGISTERS
    // Need to read from BQ when NVM is programmed.
    pBQDev->DevConf1.reg     = 0x2C;
    pBQDev->AdcCtrl2.reg     = 0x00;
    pBQDev->DevConf2.reg     = 0x00;
    pBQDev->DevConf5.reg     = 0x00;
    pBQDev->BbvcPosn2.reg    = 0x00;
    pBQDev->BbvcPosn3.reg    = 0x00;
    pBQDev->AdcConf.reg      = 0x00;
    pBQDev->DiagAdcCtrl1.reg = 0x00;
    pBQDev->GpioConf1.reg    = 0x00;
    pBQDev->GpioConf2.reg    = 0x00;
    pBQDev->GpioConf3.reg    = 0x00;
    pBQDev->GpioConf4.reg    = 0x00;
    pBQDev->GpioConf5.reg    = 0x00;
    pBQDev->GpioConf6.reg    = 0x00;

    // Dua to ID can be 0, set default value to -1
    pBQSta->BrokenNodeID         = -1;
    pBQSta->FaultNode            = -1;
    pBQSta->BrokenNodeID_Reverse = -1;
    pBQSta->FaultNode_Reverse    = -1;
    pBQSta->CANIDSet             = -1;
}

/**
 * @brief Wakeup all stack devices
 *
 * @param None
 *
 * @return None
 *
 * @note WAKE ping duration is ~1.6ms/device
 *      + 10ms/device for each device to wake up from shutdown
 *      = 11.6ms/BQ78706 device
 *      (rounded to 12ms since variable used is an integer)
 *
 * @note When a BQ78706 is used as a base,
 *      it does not forward any tones (WAKE/ S2A/ SD_HW_RESET)
 *      upon receiving pings (WAKE/ S2A/ SD_HW_RESET).
 *      To command the base device to send tones to the stack,
 *      the host must write to register CONTROL1 to do so.
 */
void WakeStackDevice(void)
{
    Wake78706();                         //Wake stack1
    delay_cycles(CLKperus * 1000 * 12);  //Delay 12ms
#if IS_79600_BRIDGE == TRUE
    delayms(12 * TOTALBOARDS);  //Delay 12ms/device
    Wake78706();
    delay_cycles(CLKperus * 1000 * 12);  //Delay 12ms
    delayms(12 * TOTALBOARDS);           //Delay 12ms/device
#endif
    WriteRegUART(0, 0x309, 0x20, 1, FRMWRT_SGL_W);  //Wake stack1+
    delayms((TOTALBOARDS - 1) * 12);
}

/**
 * @brief Auto address and Recovery function group
 *
 */
void AutoAddress(bq78706_syssta_t *pBQSta, uint8_t *ReadBuf)
{
    uint32_t currentBoard = 0;

    // DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL
    // (IF A DEVICE RESET OCCURED PRIOR TO THIS)
    WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);

    // Enable auto address mode
    WriteReg(0, CONTROL1, 0X01, 1, FRMWRT_ALL_W);

    // SET ADDRESSES FOR EVERY BOARD
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        WriteReg(0, DIR0_ADDR, currentBoard, 1, FRMWRT_ALL_W);
    }

    // Clear top of stack for all BQ device
    WriteReg(0, COMM_CTRL, 0x00, 1, FRMWRT_ALL_W);  //yan

    // Read back all device addresses to verify
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        if (ReadReg(currentBoard, DIR0_ADDR, ReadBuf, 1, 0, FRMWRT_SGL_R) !=
            (1 + 6)) {
            /**
             * Stricter judgment logic, Optional
             * enter recoveryMode if addressing fails twice
             */
            // if(pBQSta->BrokenNodeID != -1) {
            // Common judgment logic
            if (pBQSta->BrokenNodeID == currentBoard) {
                // Auto addressing failed, Set highest device as top stack
                WriteReg(currentBoard - 1, COMM_CTRL, 0x01, 1, FRMWRT_SGL_W);
                pBQSta->StackTopNodeID = currentBoard - 1;

                pBQSta->RecoveryMode = 1;
                return;
            }
            /**
             * Try to Reset Device and Set address again.
             */
            if (currentBoard == 0) {
                // Send SD_HW_RST to first device.
                SD78706();
                // Send WAKE to first device.
                WakeStackDevice();

                // No top stack
                pBQSta->FaultNode    = -1;
                pBQSta->BrokenNodeID = currentBoard;
            } else {
                /**
                 * Sends SD_HW_RST tone to the next device up the stack
                 * which will cause shutdown as well as hardware reset
                 * to the receiving device.
                 */
                WriteReg(currentBoard - 1, CONTROL1, 0x40, 1, FRMWRT_SGL_W);
                /**
                 * BQ78706 Datasheet: Timing Requirements - Power state transition
                 * tHWRST max = 45ms
                 * Wait at least 50 ms for device reset
                 */
                delay_cycles(CLKperus * 1000 * 50);

                /**
                 * Send WAKE tone to next device up the stack.
                 */
                WriteReg(currentBoard - 1, CONTROL1, 0x20, 1, FRMWRT_SGL_W);
                /**
                 * BQ78706 Datasheet: Timing Requirements - Power state transition
                 * tSU(WAKE_SHUT) = 6.5 ~ 9ms
                 * Wait at least 10 ms for 1 device wakeup
                 */
                delay_cycles(CLKperus * 1000 * 10);

                pBQSta->FaultNode    = currentBoard - 1;
                pBQSta->BrokenNodeID = currentBoard;
            }
            return;
        }
    }

    // Set highest device as top stack
    WriteReg(currentBoard - 1, COMM_CTRL, 0x01, 1, FRMWRT_SGL_W);
    pBQSta->StackTopNodeID = currentBoard - 1;

    // SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
    ReadReg(0, OTP_ECC_DATAIN1, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN2, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN3, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN4, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN5, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN6, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN7, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN8, ReadBuf, 1, 0, FRMWRT_STK_R);

    if (ReadBuf[4] != 0x14) {
        // return error here.
    }

    return;
}

/**
 * Auto addressing in backward or south direction.
 * Auto addressing reverse mode.
 */
void AutoAddressReverse(bq78706_syssta_t *pBQSta, uint8_t *ReadBuf)
{
    uint32_t currentBoard = 0;

    // Enable reverse auto address mode to bridge device
    WriteReg(0, CONTROL1, 0x80, 1, FRMWRT_ALL_W);
    delayms(2);

    // Reverse broadcast all device to reverse address mode
    WriteReg(0, CONTROL1, 0x80, 1, FRMWRT_REV_ALL_W);
    delayms(10);

    // Clear top of stack
    WriteReg(0, COMM_CTRL, 0x00, 1, FRMWRT_ALL_W);

    // DUMMY WRITE TO SNCHRONIZE ALL DAISY CHAIN DEVICES DLL
    // (IF A DEVICE RESET OCCURED PRIOR TO THIS)
    WriteReg(0, OTP_ECC_DATAIN1, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN2, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN3, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN4, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN5, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN6, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN7, 0X00, 1, FRMWRT_STK_W);
    WriteReg(0, OTP_ECC_DATAIN8, 0X00, 1, FRMWRT_STK_W);

    // Enable reverse auto address mode
    WriteReg(0, CONTROL1, 0x81, 1, FRMWRT_ALL_W);

    // SET ADDRESSES FOR EVERY BOARD
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        WriteReg(0, DIR1_ADDR, currentBoard, 1, FRMWRT_ALL_W);
    }

    // Read back all device addresses to verify
    for (currentBoard = 0; currentBoard < TOTALBOARDS; currentBoard++) {
        if (ReadReg(currentBoard, DIR1_ADDR, ReadBuf, 1, 0, FRMWRT_SGL_R) !=
            (1 + 6)) {
            /**
             * Stricter judgment logic, Optional
             * enter recoveryMode if addressing fails twice
             */
            // if(pBQSta->BrokenNodeID != -1) {
            // Common judgment logic
            if (pBQSta->BrokenNodeID_Reverse == currentBoard) {
                // Auto addressing failed, Set highest device as top stack
                WriteReg(currentBoard - 1, COMM_CTRL, 0x01, 1, FRMWRT_SGL_W);
                pBQSta->StackTopNodeID_Reverse = currentBoard - 1;

                pBQSta->RecoveryMode_Reverse = 1;
                return;
            }
            /**
             * Try to Reset Device and Set address again.
             */
            if (currentBoard == 0) {
                // Send SD_HW_RST to first device.
                SD78706();
                // Send WAKE to first device.
                WakeStackDevice();

                // No top stack
                pBQSta->FaultNode_Reverse    = -1;
                pBQSta->BrokenNodeID_Reverse = currentBoard;
            } else {
                /**
                 * Sends SD_HW_RST tone to the next device up the stack
                 * which will cause shutdown as well as hardware reset
                 * to the receiving device.
                 */
                WriteReg(currentBoard - 1, CONTROL1, 0x40, 1, FRMWRT_SGL_W);
                /**
                 * BQ78706 Datasheet: Timing Requirements - Power state transition
                 * tHWRST max = 45ms
                 * Wait at least 50 ms for device reset
                 */
                delay_cycles(CLKperus * 1000 * 50);

                /**
                 * Send WAKE tone to next device up the stack.
                 */
                WriteReg(currentBoard - 1, CONTROL1, 0x20, 1, FRMWRT_SGL_W);
                /**
                 * BQ78706 Datasheet: Timing Requirements - Power state transition
                 * tSU(WAKE_SHUT) = 6.5 ~ 9ms
                 * Wait at least 10 ms for 1 device wakeup
                 */
                delay_cycles(CLKperus * 1000 * 10);

                pBQSta->FaultNode_Reverse    = currentBoard - 1;
                pBQSta->BrokenNodeID_Reverse = currentBoard;
            }
            return;
        }
    }

    // Set highest device as top stack
    WriteReg(currentBoard - 1, COMM_CTRL, 0x01, 1, FRMWRT_SGL_W);
    pBQSta->StackTopNodeID_Reverse = currentBoard - 1;

    // SYNCRHONIZE THE DLL WITH A THROW-AWAY READ
    ReadReg(0, OTP_ECC_DATAIN1, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN2, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN3, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN4, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN5, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN6, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN7, ReadBuf, 1, 0, FRMWRT_STK_R);
    ReadReg(0, OTP_ECC_DATAIN8, ReadBuf, 1, 0, FRMWRT_STK_R);

    if (ReadBuf[4] != 0x14) {
        // Return error here.
    }

    return;
}

void BQDeviceRecovery(
    bq78706_dev_t *pBQDev, bq78706_syssta_t *pBQSta, uint8_t *ReadBuf)
{
    if (pBQSta->BrokenNodeID == 0) {
        // Send SD_HW_RST to first device.
        SD78706();
        // Send WAKE to first device.
        WakeStackDevice();
    } else {
        /**
         * Sends SD_HW_RST tone to the next device up the stack
         * which will cause shutdown as well as hardware reset
         * to the receiving device.
         */
        WriteReg(pBQSta->FaultNode, CONTROL1, 0x40, 1, FRMWRT_SGL_W);
        /**
         * BQ78706 Datasheet: Timing Requirements - Power state transition
         * tHWRST max = 45ms
         * Wait at least 50 ms for device reset
         */
        delay_cycles(CLKperus * 1000 * 50);

        /**
         * Send WAKE tone to next device up the stack.
         */
        WriteReg(pBQSta->FaultNode, CONTROL1, 0x20, 1, FRMWRT_SGL_W);
        /**
         * BQ78706 Datasheet: Timing Requirements - Power state transition
         * tSU(WAKE_SHUT) = 6.5 ~ 9ms
         * Wait at least 10 ms for 1 device wakeup
         */
        delay_cycles(CLKperus * 1000 * 10);
    }

    AutoAddress(pBQSta, ReadBuf);
    AutoAddressReverse(pBQSta, ReadBuf);

    if (pBQSta->StackTopNodeID_Reverse + pBQSta->StackTopNodeID + 1 <
        TOTALBOARDS) {
        // Customer task
    } else {
        // Customer task
    }
}

/**
 * @brief Cell balance function group
 *
 */

/**
 * @brief Set the Cell balance Time
 *
 * @param BQID BQ device ID
 * @param pBQDev BQ device
 * @param CellNum
 *          0 ~ 14: Cell number to set
 *          0: is set all cell with same time
 * @param CBTime Cell balance time in second, please refer to datasheet
 *                0 <= uCbTime <= 36000
 * @param Writetype Register write type
 * @return uint32_t the actual CB time set to bq device.
 *          -1: CBTime invalid
 *          -2: CellNum invalid
 */
typedef struct {
    uint16_t CBT;
    uint8_t CBTReg;
} BQ_CBTime_Table_t;
#define CellBalanceTimeTable_LEN 32
const BQ_CBTime_Table_t CellBalanceTimeTable[CellBalanceTimeTable_LEN] = {
    {0, 0x00}, {30, 0x01}, {60, 0x02}, {300, 0x03}, {600, 0x04}, {1200, 0x05},
    {1800, 0x06}, {2400, 0x07}, {3000, 0x08}, {3600, 0x09}, {4200, 0x0A},
    {4800, 0x0B}, {5400, 0x0C}, {6000, 0x0D}, {6600, 0x0E}, {7200, 0x0F},
    {9000, 0x10}, {10800, 0x11}, {12600, 0x12}, {14400, 0x13}, {16200, 0x14},
    {18000, 0x15}, {19800, 0x16}, {21600, 0x17}, {23400, 0x18}, {25200, 0x19},
    {27000, 0x1A}, {28800, 0x1B}, {30600, 0x1C}, {32400, 0x1D}, {34200, 0x1E},
    {36000, 0x1F}};
uint32_t SetCbTime(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t CellNum,
    uint32_t CBTime, uint8_t Writetype)
{
    uint8_t bID_temp        = 0;
    uint32_t uCbTime_actual = 0;

    uint32_t i = 0;

    for (i = 0; i < CellBalanceTimeTable_LEN; i++) {
        if (CBTime <= CellBalanceTimeTable[i].CBT) {
            break;
        }
    }
    if (i == CellBalanceTimeTable_LEN) {
        return -1;
    }
    uCbTime_actual              = CellBalanceTimeTable[i].CBT;
    pBQDev->CbCellxCtrl.fs.time = CellBalanceTimeTable[i].CBTReg;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    if (CellNum == 0) {
        for (i = CB_CELL14_CTRL; i < CB_CELL1_CTRL; i++) {
            WriteReg(bID_temp, i, pBQDev->CbCellxCtrl.reg, 1, Writetype);
        }
    } else if ((CellNum >= 1) && (CellNum <= 14)) {
        WriteReg(bID_temp, 0x324 + (14 - CellNum), pBQDev->CbCellxCtrl.reg, 1,
            Writetype);
    } else {
        return -2;
    }

    return uCbTime_actual;
}

/**
 * @brief Set the BAL_CTRL1 PWM
 *
 * @param BQID BQ device ID
 * @param pBQDev BQ device
 * @param Duty from 1 to 100 integer
 * @param Writetype Register write type
 * @return uint8_t Register value
 */
uint8_t SetBalCtrlPWM(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Duty, uint8_t Writetype)
{
    uint8_t bID_temp  = 0;
    uint8_t Duty_temp = 0;

    Duty_temp = (Duty * 63) / 100;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    pBQDev->BalCtrl1.fs.pwm = Duty_temp;

    WriteReg(bID_temp, BAL_CTRL1, pBQDev->BalCtrl1.reg, 1, Writetype);

    return Duty_temp;
}

/**
 * @brief Set cell balancing configuration.
 *          Including balancing type, etc.
 *          Then start balance control.
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *                  BAL_CTRL2
 *                  DEV_CONF1
 * @param Writetype Register write type
 * @return uint8_t
 */
uint8_t StartBalCtrl(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp              = 0;
    uint8_t rtn                   = 0;
    pBQDev->BalCtrl2.fs.auto_bal  = 0;
    pBQDev->BalCtrl2.fs.bal_go    = 1;
    pBQDev->DevConf1.fs.no_adj_cb = 1;
    //initiate the cell CBTWARN
    pBQDev->DevConf1.fs.cbtwarn_dis = 1;
    pBQDev->DevConf1.fs.twarn_thr   = 0;
    //enable OTCB_EN
    pBQDev->BalCtrl2.fs.otcb_en = 0;
    //enable unmask fault stop
    pBQDev->BalCtrl2.fs.fltstop_en = 0;  // not used
    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    rtn = WriteReg(bID_temp, DEV_CONF1, pBQDev->DevConf1.reg, 1, Writetype);
    rtn = WriteReg(bID_temp, BAL_CTRL2, pBQDev->BalCtrl2.reg, 1, Writetype);

    return rtn;
}

/**
 * @brief Set the OTCB threshold for the OTUT comparator.
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *              Read pBQDev->BalCtrl2.fs.otcb_en
 *              Set pBQDev->OtcbThresh depending on OTCBThrVal
 * @param OTCBThrVal OTCB threshold value, from 5% to 24% or 56% to 63%
 *              When pBQDev->BalCtrl2.fs.otcb_en == 0
 *                  OTCBThrVal range from 5 to 24
 *              When pBQDev->BalCtrl2.fs.otcb_en == 1
 *                  OTCBThrVal range from 56 to 63
 * @param Writetype Register write type
 * @return uint8_t OTCBThresh value written to the BQ register
 */
typedef struct {
    uint16_t OTCBThr;
    uint8_t OTCBThrReg;
} BQ_OTCBThr_Table_t;
#define OTCBThreshTable0_LEN 13
#define OTCBThreshTable1_LEN 8
const BQ_OTCBThr_Table_t OTCBThreshTable0[OTCBThreshTable0_LEN] = {{5, 0x00},
    {6, 0x01}, {7, 0x02}, {8, 0x03}, {9, 0x04}, {10, 0x05}, {12, 0x06},
    {14, 0x07}, {16, 0x08}, {18, 0x09}, {20, 0x0A}, {22, 0x0B}, {24, 0x0C}};
const BQ_OTCBThr_Table_t OTCBThreshTable1[OTCBThreshTable1_LEN] = {{56, 0x00},
    {57, 0x01}, {58, 0x02}, {59, 0x03}, {60, 0x04}, {61, 0x05}, {62, 0x06},
    {63, 0x07}};
uint8_t OTCBInit(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t OTCBThrVal, uint8_t Writetype)
{
    uint8_t bID_temp        = 0;
    uint16_t OTCBThrVal_rtn = 0;
    ;

    uint32_t i = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    if (pBQDev->BalCtrl2.fs.otcb_en == 0) {
        for (i = 0; i < OTCBThreshTable0_LEN; i++) {
            if (OTCBThrVal <= OTCBThreshTable0[i].OTCBThr) {
                break;
            }
        }
        if (i == OTCBThreshTable0_LEN) {
            return -1;
        }
        pBQDev->OtcbThresh.fs.otcb_thr = OTCBThreshTable0[i].OTCBThrReg;
        OTCBThrVal_rtn                 = OTCBThreshTable0[i].OTCBThr;
    } else {
        for (i = 0; i < OTCBThreshTable1_LEN; i++) {
            if (OTCBThrVal <= OTCBThreshTable1[i].OTCBThr) {
                break;
            }
        }
        if (i == OTCBThreshTable1_LEN) {
            return -1;
        }
        pBQDev->OtcbThresh.fs.otcb_thr = OTCBThreshTable1[i].OTCBThrReg;
        OTCBThrVal_rtn                 = OTCBThreshTable1[i].OTCBThr;
    }

    WriteReg(bID_temp, OTCB_THRESH, pBQDev->OtcbThresh.reg, 1, Writetype);

    return OTCBThrVal_rtn;
}

/**
 * @brief Set the UVCB threshold.
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *          VCBDONE_THRESH
 *          OVUV_CTRL2
 * @param VCBDThrVal VCBDONE_THRESH
 *          If a cell’s voltage < this threshold, the cell balancing on that cell stops.
 *          Please refer to Datasheet 6.5.3.2.30 VCBDONE_THRESH for details.
 *          VCBDThrVal = 2000:25:5000, or can be any integer from 2000 to 5000
 * @param Writetype Register write type
 * @return uint8_t Actual voltage set to VCBDONE_THRESH
 *          -1: VCBDThrVal invalid.
 */
uint16_t UVCBInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint16_t VCBDThrVal,
    uint8_t Writetype)
{
    pBQDev->OvuvCtrl2.fs.ovuv_go   = 0b1;
    pBQDev->OvuvCtrl2.fs.ovuv_mode = 0b01;
    uint8_t bID_temp               = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    if (VCBDThrVal == 0) {
        pBQDev->VcbdoneThresh.fs.cb_thr = 0;
    } else if ((VCBDThrVal >= 2000) && (VCBDThrVal <= 5000)) {
        pBQDev->VcbdoneThresh.fs.cb_thr = (VCBDThrVal - 1975) / 25;
    } else {
        return -1;
    }

    WriteReg(
        bID_temp, VCBDONE_THRESH, pBQDev->VcbdoneThresh.reg, 1, Writetype);
    WriteReg(bID_temp, OVUV_CTRL2, pBQDev->OvuvCtrl2.reg, 1, Writetype);

    return ((pBQDev->VcbdoneThresh.fs.cb_thr - 1) * 25) + 2000;
}

/**
 * @brief Pause all cell balancing
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *              BAL_CTRL2
 * @param Writetype Register write type
 * @return uint8_t Data length write to BQ
 */
uint8_t PauseBalCtrl(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    pBQDev->BalCtrl2.fs.cb_pause = 1;
    pBQDev->BalCtrl2.fs.bal_go   = 0;
    return WriteReg(bID_temp, BAL_CTRL2, pBQDev->BalCtrl2.reg, 1, Writetype);
}

/**
 * @brief Normal cell balancing operation
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *              BAL_CTRL2
 * @param Writetype Register write type
 * @return uint8_t Data length write to BQ
 */
uint8_t ResumeBalCtrl(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    pBQDev->BalCtrl2.fs.cb_pause = 0;
    pBQDev->BalCtrl2.fs.bal_go   = 0;
    return WriteReg(bID_temp, BAL_CTRL2, pBQDev->BalCtrl2.reg, 1, Writetype);
}

/**
 * @brief Get the Cell Balance Status from BQ device
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *              BAL_STAT
 * @param Readtype Register read type
 * @return uint8_t The length of read frame
 */
uint8_t GetBalSts(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    rtn = ReadReg(bID_temp, BAL_STAT, ReadBuf, 1, 0, Readtype);

    // Please take care of these read back data
    // if Readtype is Stack or All devices read.
    pBQDev->BalStat.reg = ReadBuf[4] & 0x3F;

    return rtn;
}

/**
 * @brief Get the Balance Remain Time
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *              BAL_CTRL3
 *              BAL_TIME
 * @param Readtype
 * @return uint16_t Cell balance remain time in seconds
 *                  -1: Readtype invalid
 */
uint16_t GetBalRemainTime(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp     = 0;
    uint16_t BalTime_rtn = 0;
    uint8_t Writetype    = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp  = BQID;
        Writetype = FRMWRT_SGL_W;
    } else if (Readtype == FRMWRT_STK_R) {
        Writetype = FRMWRT_STK_W;
    } else if (Readtype == FRMWRT_ALL_R) {
        Writetype = FRMWRT_ALL_W;
    } else {
        return -1;
    }
    pBQDev->BalCtrl3.fs.bal_time_sel = 0;  // channel 1
    pBQDev->BalCtrl3.fs.bal_time_go  = 1;
    WriteReg(bID_temp, BAL_CTRL3, pBQDev->BalCtrl3.reg, 1, Writetype);

    ReadReg(bID_temp, BAL_TIME, ReadBuf, 1, 0, Readtype);

    // Please take care of these read back data
    // if Readtype is Stack or All devices read.
    pBQDev->BalTime.reg = ReadBuf[4] & 0x3F;

    if (pBQDev->BalTime.fs.time_unit == 0) {
        BalTime_rtn = pBQDev->BalTime.fs.time * 300;
    } else {
        BalTime_rtn = pBQDev->BalTime.fs.time * 5;
    }

    return BalTime_rtn;
}

/**
 * @brief ADC, GPIO function group
 *
 */

uint8_t AdcCtrlInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;
    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    pBQDev->AdcCtrl2.fs.lpf_vcell    = 0b010;  // LPF=39Hz
    pBQDev->AdcCtrl2.fs.lpf_vcell_en = 0b1;    // enable
    pBQDev->AdcCtrl2.fs.adc_mode     = 0b10;   // continous mode
    pBQDev->DevConf2.fs.num_cell     = 0x07;   // 13cell, if 18 cell set 0x0B;
#ifdef NOBUSBAR
    pBQDev->DevConf2.fs.bb_pin_en = 0b0;  // busbar not used
    pBQDev->DevConf2.fs.ptc_en    = 0b1;  // PTC_EN used
#else
    pBQDev->DevConf2.fs.bb_pin_en = 0b1;
    pBQDev->DevConf2.fs.bb_pin_loc =
        0b10;  // 9s is busbar; customer configure by their project
    pBQDev->BbvcPosn2.fs.cell9 = 0b1;
#endif
    pBQDev->AdcConf.fs.adc_dly         = 0x00;  // 0us
    pBQDev->DiagAdcCtrl1.fs.vcell_filt = 0b0;   // 8 samples
    pBQDev->DiagAdcCtrl1.fs.vcell_thr =
        0b000110;  // The VC vs. CB check is considered to pass if the measured delta is < this threshold 6mV

    WriteReg(bID_temp, ADC_CTRL2, pBQDev->AdcCtrl2.reg, 1, Writetype);
    WriteReg(bID_temp, DEV_CONF2, pBQDev->DevConf2.reg, 1, Writetype);
    WriteReg(bID_temp, DEV_CONF5, pBQDev->DevConf5.reg, 1, Writetype);
    WriteReg(bID_temp, BBVC_POSN2, pBQDev->BbvcPosn2.reg, 1, Writetype);
    WriteReg(bID_temp, BBVC_POSN3, pBQDev->BbvcPosn3.reg, 1, Writetype);
    WriteReg(bID_temp, ADC_CONF, pBQDev->AdcConf.reg, 1, Writetype);
    WriteReg(bID_temp, DIAG_ADC_CTRL1, pBQDev->DiagAdcCtrl1.reg, 1, Writetype);

    return 0;
}

uint8_t GpioConfInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    pBQDev->GpioConf2.fs.gpio3  = ADC_RATIO;
    pBQDev->GpioConf2.fs.gpio4  = ADC_RATIO;
    pBQDev->GpioConf3.fs.gpio5  = ADC_RATIO;
    pBQDev->GpioConf3.fs.gpio6  = ADC_RATIO;
    pBQDev->GpioConf4.fs.gpio7  = ADC_RATIO;
    pBQDev->GpioConf4.fs.gpio8  = ADC_RATIO;
    pBQDev->GpioConf5.fs.gpio9  = OUTPUT_H;
    pBQDev->GpioConf5.fs.gpio10 = OUTPUT_H;
    pBQDev->GpioConf6.fs.gpio11 = OUTPUT_L;

    uint8_t bID_temp = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    WriteReg(bID_temp, GPIO_CONF1, pBQDev->GpioConf1.reg, 1, Writetype);
    WriteReg(bID_temp, GPIO_CONF2, pBQDev->GpioConf2.reg, 1, Writetype);
    WriteReg(bID_temp, GPIO_CONF3, pBQDev->GpioConf3.reg, 1, Writetype);
    WriteReg(bID_temp, GPIO_CONF4, pBQDev->GpioConf4.reg, 1, Writetype);
    WriteReg(bID_temp, GPIO_CONF5, pBQDev->GpioConf5.reg, 1, Writetype);
    WriteReg(bID_temp, GPIO_CONF6, pBQDev->GpioConf6.reg, 1, Writetype);
    WriteReg(bID_temp, ADC_CTRL1, pBQDev->AdcCtrl1.reg, 1, Writetype);

    return 0;
}

uint8_t AdcConversion(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp           = 0;
    pBQDev->AdcCtrl2.fs.adc_go = 1;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    WriteReg(bID_temp, ADC_CTRL2, pBQDev->AdcCtrl2.reg, 1, Writetype);

    return 0;
}

uint8_t GetADCStatus(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    ReadReg(bID_temp, ADC_DATA_RDY, ReadBuf, 1, 0, Readtype);
    pBQDev->AdcDataRdy.reg = ReadBuf[4] & 0xFF;  // Debug ADC status
    ReadReg(bID_temp, DEV_STAT1, ReadBuf, 1, 0, Readtype);
    pBQDev->DevStat1.reg = ReadBuf[4] & 0xFF;  // Debug ADC status

    return 0;
}
uint8_t ADCReadyCheck(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }
    do {
        rtn = GetADCStatus(bID_temp, pBQDev, ReadBuf, Readtype);
    } while ((pBQDev->AdcDataRdy.fs.drdy_vcelladc == 0) ||
             (pBQDev->AdcDataRdy.fs.drdy_gpadc == 0) ||
             (pBQDev->DevStat1.fs.vcelladc_run == 0) ||
             (pBQDev->DevStat1.fs.gpadc_run == 0));
    return rtn;
}

uint8_t GetADCDiagStatus(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    rtn = ReadReg(bID_temp, DIAG_STAT2, ReadBuf, 1, 0, Readtype);
    pBQDev->DiagStat1.reg = ReadBuf[4] & 0xFF;  // Debug ADC status
    return rtn;
}

uint8_t GetVCell(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    // Read from BQ VCELL14_HI to VCELL1_LO, 28 bytes in total
    ReadReg(bID_temp, VCELL13_HI, ReadBuf, ACTIVECHANNELS * 2, 0, Readtype);

    if (Readtype == FRMWRT_SGL_R) {
        for (int i = 0; i < ACTIVECHANNELS; i++) {
            pBQDev[bID_temp].VcellSinglex[ACTIVECHANNELS * 2 - i * 2 - 2] =
                ReadBuf[4 + i * 2];
            pBQDev[bID_temp].VcellSinglex[ACTIVECHANNELS * 2 - i * 2 - 1] =
                ReadBuf[5 + i * 2];
            pBQDev[bID_temp].Vcellx[ACTIVECHANNELS - i - 1] =
                ((ReadBuf[4 + i * 2] << 8) + (ReadBuf[5 + i * 2])) *
                VLSB_STANDARD_F;
        }
    } else if (Readtype == FRMWRT_ALL_R) {
        for (int j = 0; j < TOTALBOARDS; j++) {
            for (int i = 0; i < ACTIVECHANNELS; i++) {
                pBQDev[TOTALBOARDS - 1 - j]
                    .VcellSinglex[ACTIVECHANNELS * 2 - i * 2 - 2] =
                    ReadBuf[j * (ACTIVECHANNELS * 2 + 6) + 4 + i * 2];
                pBQDev[TOTALBOARDS - 1 - j]
                    .VcellSinglex[ACTIVECHANNELS * 2 - i * 2 - 1] =
                    ReadBuf[j * (ACTIVECHANNELS * 2 + 6) + 5 + i * 2];
                pBQDev[TOTALBOARDS - 1 - j].Vcellx[ACTIVECHANNELS - i - 1] =
                    (
                        // Need to confirm whether each board will have 4 extra bytes to ignore
                        // (j+1) is each board have 4 extra bytes
                        (ReadBuf[j * (ACTIVECHANNELS * 2 + 6) + 4 + i * 2]
                            << 8) +
                        (ReadBuf[j * (ACTIVECHANNELS * 2 + 6) + 5 + i * 2])) *
                    VLSB_STANDARD_F;
            }
        }
    }

    return rtn;
}

// Hardware related
// 3 4 5 6 is single channel
// 7 8 connected mux
// 9 10 11 mux control
// 3 4 5 6 is single channel
// 7 8 connected mux
// 9 10 11 mux control
uint8_t VGpio_conf[2] = {OUTPUT_L, OUTPUT_H};
uint8_t TMUX_GPIO(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype)
{
    uint8_t bID_temp     = 0;
    static uint8_t rdSeq = 0;
    uint8_t Readtype     = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
        Readtype = FRMWRT_SGL_R;
    } else if (Writetype == FRMWRT_STK_W) {
        Readtype = FRMWRT_STK_R;
    } else if (Writetype == FRMWRT_ALL_W) {
        Readtype = FRMWRT_ALL_R;
    } else {
        return -1;
    }
    ReadReg(bID_temp, GPIO_STAT1, ReadBuf, 1, 0, Readtype);
    pBQDev->GpioStat1.reg = ReadBuf[4];

    pBQDev->GpioConf5.fs.gpio9  = VGpio_conf[rdSeq & 0x01];
    pBQDev->GpioConf5.fs.gpio10 = VGpio_conf[(rdSeq & 0x02) >> 1];
    pBQDev->GpioConf6.fs.gpio11 = VGpio_conf[(rdSeq & 0x04) >> 2];

    // Standard steps
    // Set GPIO output H/L
    // Delay
    // Read GPIO voltage
    rdSeq++;
    if (rdSeq == 8) {
        rdSeq = 0;
    }
    WriteReg(bID_temp, GPIO_CONF5, pBQDev->GpioConf5.reg, 1, Writetype);
    WriteReg(bID_temp, GPIO_CONF6, pBQDev->GpioConf6.reg, 1, Writetype);
    WriteReg(bID_temp, ADC_CTRL2, pBQDev->AdcCtrl2.reg, 1, Writetype);
    return 0;
}
uint8_t GetVGpio(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;
    float VLSB_GPIO  = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    if (pBQDev->GpioConf1.fs.gpio1 == ADC_RATIO ||
        pBQDev->GpioConf1.fs.gpio2 == ADC_RATIO ||
        pBQDev->GpioConf2.fs.gpio3 == ADC_RATIO ||
        pBQDev->GpioConf2.fs.gpio4 == ADC_RATIO ||
        pBQDev->GpioConf3.fs.gpio5 == ADC_RATIO ||
        pBQDev->GpioConf3.fs.gpio6 == ADC_RATIO ||
        pBQDev->GpioConf4.fs.gpio7 == ADC_RATIO ||
        pBQDev->GpioConf4.fs.gpio8 == ADC_RATIO ||
        pBQDev->GpioConf5.fs.gpio9 == ADC_RATIO ||
        pBQDev->GpioConf5.fs.gpio10 == ADC_RATIO ||
        pBQDev->GpioConf6.fs.gpio11 == ADC_RATIO) {
        VLSB_GPIO = VLSB_GPIO_RATIO;
    } else {
        VLSB_GPIO = VLSB_STANDARD_F;
    }

    rtn = ReadReg(
        bID_temp, GPIO1_HI, ReadBuf, GADC_ACTIVECHANNEL * 2, 0, Readtype);
    if (Readtype == FRMWRT_SGL_R) {
        for (int i = 0; i < GADC_ACTIVECHANNEL; i++) {
            pBQDev[bID_temp].VGPIOSinglex[GADC_ACTIVECHANNEL * 2 - i * 2 - 2] =
                ReadBuf[4 + i * 2];
            pBQDev[bID_temp].VGPIOSinglex[GADC_ACTIVECHANNEL * 2 - i * 2 - 1] =
                ReadBuf[5 + i * 2];
            pBQDev[bID_temp].GPIOxVoltage[GADC_ACTIVECHANNEL - i - 1] =
                ((ReadBuf[4 + i * 2] << 8) + (ReadBuf[5 + i * 2])) * VLSB_GPIO;
        }
    } else if (Readtype == FRMWRT_ALL_R) {
        for (int j = 0; j < TOTALBOARDS; j++) {
            for (int i = 0; i < GADC_ACTIVECHANNEL; i++) {
                pBQDev[j].VGPIOSinglex[GADC_ACTIVECHANNEL * 2 - i * 2 - 2] =
                    ReadBuf[j * (GADC_ACTIVECHANNEL * 2 + 6) + 4 + i * 2];
                pBQDev[j].VGPIOSinglex[GADC_ACTIVECHANNEL * 2 - i * 2 - 1] =
                    ReadBuf[j * (GADC_ACTIVECHANNEL * 2 + 6) + 5 + i * 2];
                pBQDev[j].GPIOxVoltage[GADC_ACTIVECHANNEL - i - 1] =
                    (
                        // Need to confirm whether each board will have 4 extra bytes to ignore
                        // (j+1) is each board have 4 extra bytes
                        (ReadBuf[j * (GADC_ACTIVECHANNEL * 2 + 6) + 4 + i * 2]
                            << 8) +
                        (ReadBuf[j * (GADC_ACTIVECHANNEL * 2 + 6) + 5 +
                                 i * 2])) *
                    VLSB_GPIO;
            }
        }
    }

    return rtn;
}

uint8_t DLPF_freeze(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp  = 0;
    uint8_t rtn       = 0;
    uint8_t Writetype = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp  = BQID;
        Writetype = FRMWRT_SGL_W;
    } else if (Readtype == FRMWRT_STK_R) {
        Writetype = FRMWRT_STK_W;
    } else if (Readtype == FRMWRT_ALL_R) {
        Writetype = FRMWRT_ALL_W;
    } else {
        return -1;
    }

    pBQDev->AdcCtrl3.fs.freeze_lpf_en = 1;
    WriteReg(bID_temp, ADC_CTRL3, pBQDev->AdcCtrl3.reg, 1, Writetype);
    do {
        // Read from BQ DIAG_STAT1
        rtn = ReadReg(bID_temp, DIAG_STAT1, ReadBuf, 1, 0, Readtype);
        // Need to confirm the data structure.
        pBQDev->DiagStat1.reg = ReadBuf[4];
    } while (pBQDev->DiagStat1.fs.freeze_lpf_active == 0);

    return rtn;
}

uint8_t DLPF_Defreeze(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp  = 0;
    uint8_t rtn       = 0;
    uint8_t Writetype = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp  = BQID;
        Writetype = FRMWRT_SGL_W;
    } else if (Readtype == FRMWRT_STK_R) {
        Writetype = FRMWRT_STK_W;
    } else if (Readtype == FRMWRT_ALL_R) {
        Writetype = FRMWRT_ALL_W;
    } else {
        return -1;
    }

    pBQDev->AdcCtrl3.fs.freeze_lpf_en = 0;
    WriteReg(bID_temp, ADC_CTRL3, pBQDev->AdcCtrl3.reg, 1, Writetype);

    // Read verify, waiting for BQ device exit freeze mode.
    do {
        // Read from BQ DIAG_STAT1
        rtn = ReadReg(bID_temp, DIAG_STAT1, ReadBuf, 1, 0, Readtype);
        // Need to confirm the data structure.
        pBQDev->DiagStat1.reg = ReadBuf[4];
    } while (pBQDev->DiagStat1.fs.freeze_lpf_active == 1);

    return rtn;
}

/**
 * @brief ADC, GPIO function group
 *
 */

uint8_t GetAllFaults(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    rtn = ReadReg(bID_temp, FAULT_SUMMARY, ReadBuf, 1, 0, Readtype);

    // Single device's fault process.
    pBQDev->FaultSummary.reg = ReadBuf[4];

// Read back Level 2 fault register
#if 1
    // Example for reading the pwr fault details
    if (pBQDev->FaultSummary.fs.fault_pwr == 1) {
        ReadReg(bID_temp, FAULT_PWR1, ReadBuf, 2, 0, Readtype);
        pBQDev->FaultPwr1.reg = ReadBuf[4];
        pBQDev->FaultPwr2.reg = ReadBuf[5];
    } else {
        pBQDev->FaultPwr1.reg = 0;
        pBQDev->FaultPwr2.reg = 0;
    }

    if (pBQDev->FaultSummary.fs.fault_comm == 1) {
        ReadReg(bID_temp, FAULT_COMM, ReadBuf, 1, 0, Readtype);
        pBQDev->FaultComm.reg = ReadBuf[4];
    }

    if (pBQDev->FaultSummary.fs.fault_otp == 1) {
        ReadReg(bID_temp, FAULT_OTP, ReadBuf, 1, 0, Readtype);
        pBQDev->FaultOtp.reg = ReadBuf[4];
    } else {
        pBQDev->FaultOtp.reg = 0;
    }
#endif
    return rtn;
}

uint8_t ResetAllFaults(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype)
{
    uint8_t bID_temp = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    WriteReg(bID_temp, FAULT_RST1, 0xFFFF, 2, Writetype);

    // Customer Task: clear other level 2 and level 3 fault.

    return 0;
}

uint8_t MaskAllFaults(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype)
{
    uint8_t bID_temp = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    return WriteReg(bID_temp, FAULT_MSK1, 0xFFFF, 2, Writetype);
}

uint8_t GpioOpnWrUpdate(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp  = 0;
    float ABS_A_B     = 0;
    uint8_t Writetype = 0;
    if (Readtype == FRMWRT_SGL_R) {
        bID_temp  = BQID;
        Writetype = FRMWRT_SGL_W;
    } else if (Readtype == FRMWRT_STK_R) {
        Writetype = FRMWRT_STK_W;
    } else if (Readtype == FRMWRT_ALL_R) {
        Writetype = FRMWRT_ALL_W;
    } else {
        return -1;
    }
    switch (BQDev_TaskStatus) {
        case BQTaskStatus_IDLE:
            GetVGpio(bID_temp, pBQDev, ReadBuf, Readtype);

            for (int i = 0; i < GADC_ACTIVECHANNEL; i++) {
                // Convert GPIOx's voltage depending on GPIOx_HI + GPIOx_LO
                pBQDev->GPIOxVoltageA[i] = (pBQDev->GPIOxVoltage[i]) / 100 * 4;

                if (pBQDev->GPIOxVoltageA[i] > 2) {
                    // Set GPIO to weak pull-down
                    switch (i) {
                        case 2: {
                            // Configure GPIO3 to ADC absolute power-down mode
                            pBQDev->GpioConf2.fs.gpio3 = ADC_ABS_PD;
                            break;
                        }
                        case 4: {
                            // Configure GPIO5 to ADC absolute power-down mode
                            pBQDev->GpioConf3.fs.gpio5 = ADC_ABS_PD;
                            break;
                        }
                        case 6: {
                            // Configure GPIO7 to ADC absolute power-down mode
                            pBQDev->GpioConf4.fs.gpio7 = ADC_ABS_PD;
                            break;
                        }
                        default: {
                            // Handle unexpected values of 'i' (e.g., ignore or log an error)
                            break;
                        }
                    }
                } else {
                    // Set GPIO to weak pull-up
                    // Todo: how to set GPIO to weak pull-down or pull-up?
                    switch (i) {
                        case 2: {
                            // Configure GPIO3 to ADC absolute power-down mode
                            pBQDev->GpioConf2.fs.gpio3 = ADC_ABS_PU;
                            break;
                        }
                        case 4: {
                            // Configure GPIO5 to ADC absolute power-down mode
                            pBQDev->GpioConf3.fs.gpio5 = ADC_ABS_PU;
                            break;
                        }
                        case 6: {
                            // Configure GPIO7 to ADC absolute power-down mode
                            pBQDev->GpioConf4.fs.gpio7 = ADC_ABS_PU;
                            break;
                        }
                        default: {
                            // Handle unexpected values of 'i' (e.g., ignore or log an error)
                            break;
                        }
                    }
                }
            }
            WriteReg(
                bID_temp, GPIO_CONF1, pBQDev->GpioConf1.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF2, pBQDev->GpioConf2.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF3, pBQDev->GpioConf3.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF4, pBQDev->GpioConf4.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF5, pBQDev->GpioConf5.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF6, pBQDev->GpioConf6.reg, 1, Writetype);
            AdcConversion(bID_temp, pBQDev, Writetype);
            ADCReadyCheck(bID_temp, pBQDev, ReadBuf, Readtype);
            BQDev_TaskStatus = BQTaskStatus_GpioOpn_WaitDelay;
            // Start delay task interrrupt
            DL_Timer_setLoadValue(TIMER_BQTASK_INST, BQTask_GpioOpn_DelayTime);
            DL_Timer_startCounter(TIMER_BQTASK_INST);
            break;

        case BQTaskStatus_GpioOpn_WaitDelay:
            GetVGpio(bID_temp, pBQDev, ReadBuf, Readtype);

            for (int i = 0; i < GADC_ACTIVECHANNEL; i++) {
                // Convert GPIOx's voltage depending on GPIOx_HI + GPIOx_LO
                pBQDev->GPIOxVoltageB[i] = pBQDev->GPIOxVoltage[i];

                if (pBQDev->GPIOxVoltageB[i] > pBQDev->GPIOxVoltageA[i]) {
                    ABS_A_B =
                        pBQDev->GPIOxVoltageB[i] - pBQDev->GPIOxVoltageA[i];
                } else {
                    ABS_A_B =
                        pBQDev->GPIOxVoltageA[i] - pBQDev->GPIOxVoltageB[i];
                }

                if (ABS_A_B > 1) {
                    // Open Wire Fail
                    // Customer task
                } else {
                    // Open Wire Pass
                    // Customer task
                }
            }
            pBQDev->GpioConf2.fs.gpio3 = ADC_RATIO;
            pBQDev->GpioConf3.fs.gpio5 = ADC_RATIO;
            pBQDev->GpioConf4.fs.gpio7 = ADC_RATIO;
            WriteReg(
                bID_temp, GPIO_CONF1, pBQDev->GpioConf1.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF2, pBQDev->GpioConf2.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF3, pBQDev->GpioConf3.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF4, pBQDev->GpioConf4.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF5, pBQDev->GpioConf5.reg, 1, Writetype);
            WriteReg(
                bID_temp, GPIO_CONF6, pBQDev->GpioConf6.reg, 1, Writetype);
            AdcConversion(bID_temp, pBQDev, Writetype);
            ADCReadyCheck(bID_temp, pBQDev, ReadBuf, Readtype);
            BQDev_TaskStatus = BQTaskStatus_IDLE;
            break;

        default:
            // BQDev_TaskStatus is not IDLE or valid value.
            return -1;
            break;
    }
    return 0;
}

uint8_t CellOpnWrUpdate(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp = BQID;
    }

    GetVCell(bID_temp, pBQDev, ReadBuf, Readtype);

    // Customer task

    return 0;
}

uint8_t AcompDiag(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp  = 0;
    uint8_t Writetype = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp  = BQID;
        Writetype = FRMWRT_SGL_W;
    } else if (Readtype == FRMWRT_STK_R) {
        Writetype = FRMWRT_STK_W;
    } else if (Readtype == FRMWRT_ALL_R) {
        Writetype = FRMWRT_ALL_W;
    } else {
        return -1;
    }

    switch (BQDev_TaskStatus) {
        case BQTaskStatus_IDLE:
            GetBalSts(bID_temp, pBQDev, ReadBuf, Readtype);
            if (pBQDev->BalStat.fs.cb_run == 1) {
                // Pause cell balance
                DL_GPIO_setPins(GPIOB, LED_GREEN_PIN);
                PauseBalCtrl(bID_temp, pBQDev, Writetype);
                // Waiting for cell balance is paused.
                do {
                    GetBalSts(bID_temp, pBQDev, ReadBuf, Readtype);
                } while (pBQDev->BalStat.fs.cb_inpause == 0);
            }
            // Need to set ADC CTRL1, CTRL2 by write pBQDev
            WriteReg(bID_temp, DIAG_ADC_CTRL1, pBQDev->DiagAdcCtrl1.reg, 1,
                Writetype);
            pBQDev->DiagAdcCtrl2.fs.gpio_thr = 6;
            WriteReg(bID_temp, DIAG_ADC_CTRL2, pBQDev->DiagAdcCtrl2.reg, 1,
                Writetype);
            // Start ACOMP and DCOMP
            pBQDev->DiagAdcCtrl3.fs.diag_dig_en   = 1;
            pBQDev->DiagAdcCtrl3.fs.diag_ana_mode = 0b11;
            // 0b11 = Both cell voltage measurement analog path diagnostic and GPIO measurement analog path diagnostic
            // 0b01 = Cell voltage measurement analog path diagnostic is performed.
            // 0b10 = GPIO measurement analog path diagnostic is performed (applies to GPIO configured as “ADC and OTUT inputs” or “ADC only input”).
            pBQDev->DiagAdcCtrl3.fs.diag_ana_go = 1;
            WriteReg(bID_temp, DIAG_ADC_CTRL3, pBQDev->DiagAdcCtrl3.reg, 1,
                Writetype);
            // Ensure diagnostic is running
            do {
                ReadReg(bID_temp, DEV_STAT1, ReadBuf, 2, 0, Readtype);
                pBQDev->DevStat1.reg = ReadBuf[4];
                pBQDev->DevStat2.reg = ReadBuf[5];
            } while ((pBQDev->DevStat1.fs.diag_ana_run == 0) ||
                     (pBQDev->DevStat2.fs.diag_dig_run == 0));

            BQDev_TaskStatus = BQTaskStatus_AcompDiag_WaitDelay;
            // Start delay task interrrupt
            if (pBQDev->DiagAdcCtrl1.fs.vcell_filt == 0) {
                DL_Timer_setLoadValue(
                    TIMER_BQTASK_INST, BQTask_AcompDiag_DelayTime0);
            } else {
                DL_Timer_setLoadValue(
                    TIMER_BQTASK_INST, BQTask_AcompDiag_DelayTime1);
            }
            DL_Timer_startCounter(TIMER_BQTASK_INST);
            break;

        case BQTaskStatus_AcompDiag_WaitDelay:
            ReadReg(bID_temp, FAULT_ADC_MISC, ReadBuf, 1, 0, Readtype);
            pBQDev->FaultAdcMisc.reg = ReadBuf[4];
            if (pBQDev->FaultAdcMisc.reg != 0) {
                // Diagnostic failed.
                return -2;
            }
            do {
                ReadReg(bID_temp, DIAG_STAT1, ReadBuf, 2, 0, Readtype);
                pBQDev->DiagStat1.reg = ReadBuf[4];
                pBQDev->DiagStat2.reg = ReadBuf[5];
            } while (
                (pBQDev->DiagStat2.fs.drdy_ana_gpio !=
                    1) ||  // 0 is (pBQDev->DiagStat2.fs.drdy_ana_gpio != 1)
                (pBQDev->DiagStat2.fs.drdy_ana_vcell != 1) ||
                (0));  // 0 is pBQDev->DiagStat2.fs.drdy_dig != 1

            ReadReg(bID_temp, DIAG_STAT1, ReadBuf, 2, 0, Readtype);
            pBQDev->DiagStat1.reg = ReadBuf[4];
            pBQDev->DiagStat2.reg = ReadBuf[5];
            if ((pBQDev->DiagStat2.fs.drdy_ana_gpio != 1) ||
                (pBQDev->DiagStat2.fs.drdy_ana_vcell != 1) ||
                0) {  //0 is (pBQDev->DiagStat2.fs.drdy_dig != 1)
                // DRDY failed.
                return -3;
            }

            // Read faults
            ReadReg(bID_temp, FAULT_ADC_GPIO1, ReadBuf, 2, 0, Readtype);
            pBQDev->FaultAdcGpio1.reg = ReadBuf[4];
            pBQDev->FaultAdcGpio2.reg = ReadBuf[5];
            ReadReg(bID_temp, FAULT_ADC_VCELL1, ReadBuf, 3, 0, Readtype);
            pBQDev->FaultAdcVcell1.reg = ReadBuf[4];
            pBQDev->FaultAdcVcell2.reg = ReadBuf[5];
            pBQDev->FaultAdcVcell3.reg = ReadBuf[6];

            //Reset ADC faults
            WriteReg(bID_temp, FAULT_RST2, 0x08, 1, Writetype);

            // Ensure all ADC faults are cleared.
            ReadReg(bID_temp, FAULT_ADC_GPIO1, ReadBuf, 2, 0, Readtype);
            if ((ReadBuf[4] != 0) || (ReadBuf[5] != 0)) {
                // return -4;
            }
            ReadReg(bID_temp, FAULT_ADC_VCELL1, ReadBuf, 3, 0, Readtype);
            if ((ReadBuf[4] != 0) || (ReadBuf[5] != 0) || (ReadBuf[6] != 0)) {
                // return -4;
            }

            ResumeBalCtrl(bID_temp, pBQDev, Writetype);
            DL_GPIO_clearPins(GPIOB, LED_GREEN_PIN);
            BQDev_TaskStatus = BQTaskStatus_IDLE;
            break;

        default:
            // BQDev_TaskStatus is not IDLE or valid value.
            return -1;
            break;
    }

    return 0;
}

uint8_t OTP(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype)
{
    uint8_t bID_temp = 0;
    uint8_t Readtype = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
        Readtype = FRMWRT_SGL_R;
    } else if (Writetype == FRMWRT_STK_W) {
        Readtype = FRMWRT_STK_R;
    } else if (Writetype == FRMWRT_ALL_W) {
        Readtype = FRMWRT_ALL_R;
    } else {
        return -1;
    }

    switch (BQDev_TaskStatus) {
        case BQTaskStatus_IDLE:
            ReadReg(bID_temp, CUST_MISC1, ReadBuf, 1, 0,
                Readtype);  // just to verify if CUST_MISC1 has written.
            pBQDev->OtpProgUnlock1.reg = 0xBC;
            pBQDev->OtpProgUnlock2.reg = 0x6F;
            WriteReg(bID_temp, OTP_PROG_UNLOCK1, pBQDev->OtpProgUnlock1.reg, 1,
                Writetype);
            WriteReg(bID_temp, OTP_PROG_UNLOCK2, pBQDev->OtpProgUnlock2.reg, 1,
                Writetype);

            do {
                ReadReg(bID_temp, OTP_STAT, ReadBuf, 1, 0, Readtype);
                pBQDev->OtpStat.reg = ReadBuf[4];
            } while (((pBQDev->OtpStat.fs.unlock != 1) ||
                      (pBQDev->OtpStat.fs.loader != 0) ||
                      (pBQDev->OtpStat.fs.uv_ovok != 0) ||
                      (pBQDev->OtpStat.fs._try != 0)));

            // Start OTP programming
            pBQDev->Control2.fs.prog_go = 1;
            WriteReg(bID_temp, CONTROL2, pBQDev->Control2.reg, 1, Writetype);

            BQDev_TaskStatus = BQTaskStatus_OTP_WaitDelay;
            // Start task delay timer
            DL_Timer_setLoadValue(TIMER_BQTASK_INST, BQTask_OTP_DelayTime);
            DL_Timer_startCounter(TIMER_BQTASK_INST);
            break;

        case BQTaskStatus_OTP_WaitDelay:
            ReadReg(bID_temp, OTP_STAT, ReadBuf, 1, 0, Readtype);
            pBQDev->OtpStat.reg = ReadBuf[4];
            if (pBQDev->OtpStat.fs.done == 0) {
                // OTP failed
                return -1;
            }

            // Issue a software reset
            pBQDev->Control1.fs.soft_reset = 1;
            WriteReg(bID_temp, CONTROL1, pBQDev->Control1.reg, 1, Writetype);

            // Check OTP_Stat register
            if (pBQDev->OtpStat.reg != 0xE0) {
                // Program failed.
                return -2;
            }

            BQDev_TaskStatus = BQTaskStatus_IDLE;
            break;

        default:
            // BQDev_TaskStatus is not IDLE or valid value.
            return -1;
            break;
    }

    return 0;
}

uint8_t HWReset(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype)
{
    return 0;
}

uint8_t CbfetDiag(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype)
{
    // 6.3.9.3 Balancing FET Diagnostic
    uint8_t bID_temp = 0;
    uint8_t Readtype = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
        Readtype = FRMWRT_SGL_R;
    } else if (Writetype == FRMWRT_STK_W) {
        Readtype = FRMWRT_STK_R;
    } else if (Writetype == FRMWRT_ALL_W) {
        Readtype = FRMWRT_ALL_R;
    } else {
        return -1;
    }

    switch (BQDev_TaskStatus) {
        case BQTaskStatus_IDLE:
            GetBalSts(bID_temp, pBQDev, ReadBuf, Readtype);
            if (pBQDev->BalStat.fs.cb_run == 1) {
                // Pause cell balance
                DL_GPIO_setPins(GPIOB, LED_GREEN_PIN);
                PauseBalCtrl(bID_temp, pBQDev, Writetype);
                // Waiting for cell balance is paused.
                do {
                    GetBalSts(bID_temp, pBQDev, ReadBuf, Readtype);
                } while (pBQDev->BalStat.fs.cb_inpause == 0);
            }

            pBQDev->DiagMiscCtrl2.fs.diag_cbfetow_go = 1;
            WriteReg(bID_temp, DIAG_MISC_CTRL2, pBQDev->DiagMiscCtrl2.reg, 1,
                Writetype);

            BQDev_TaskStatus = BQTaskStatus_CbfetDiag_WaitDelay;
            // Start task delay timer

            DL_Timer_setLoadValue(
                TIMER_BQTASK_INST, BQTask_CbfetDiag_DelayTime);
            DL_Timer_startCounter(TIMER_BQTASK_INST);
            break;

        case BQTaskStatus_CbfetDiag_WaitDelay:
            delayms(16);
            ReadReg(bID_temp, DIAG_STAT2, ReadBuf, 1, 0, Readtype);
            pBQDev->DiagStat2.reg = ReadBuf[4];
            if (pBQDev->DiagStat2.fs.drdy_cbfetow == 0) {
                DL_Timer_setLoadValue(TIMER_BQTASK_INST, BQTask_1MS_DelayTime);
                DL_Timer_startCounter(TIMER_BQTASK_INST);
                break;
            }

            ReadReg(bID_temp, FAULT_CB_FETOW1, ReadBuf, 3, 0, Readtype);
            pBQDev->FaultCbFetow1.reg = ReadBuf[4];
            pBQDev->FaultCbFetow2.reg = ReadBuf[5];
            pBQDev->FaultCbFetow3.reg = ReadBuf[6];

            ResumeBalCtrl(bID_temp, pBQDev, Writetype);
            DL_GPIO_clearPins(GPIOB, LED_GREEN_PIN);
            BQDev_TaskStatus = BQTaskStatus_IDLE;
            break;

        default:
            // BQDev_TaskStatus is not IDLE or valid value.
            return -1;
            break;
    }

    return 0;
}

/**
 * @brief
 *
 * @param BQID BQ device ID
 * @param pBQDev Cell balance configuration
 *                  DEV_CONF1
 * @param Writetype Register write type
 * @return uint8_t
 */
uint8_t OTUTInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    pBQDev->Otutctrl2.fs.otut_mode = 0b01;
    pBQDev->Otutctrl2.fs.otut_go   = 0b1;

    rtn = WriteReg(bID_temp, OTUT_CTRL2, pBQDev->Otutctrl2.reg, 1, Writetype);

    return rtn;
}

uint8_t DisableTimeout(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }
    rtn = WriteReg(bID_temp, DEBUG_CTRL_UNLOCK, 0xA5, 1, Writetype);
    rtn = WriteReg(bID_temp, DEBUG_COMM_CTRL1, 0x22, 1, Writetype);

    return rtn;
}

uint8_t VcOpnWrUpdate(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf,
    float MaxVol, float MinVol, uint8_t Readtype)
{
    float ABS_A_B = 0;

    switch (BQDev_TaskStatus) {
        case BQTaskStatus_IDLE:
            for (int i = 0; i < ACTIVECHANNELS; i++) {
                // Convert GPIOx's voltage depending on GPIOx_HI + GPIOx_LO
                pBQDev->VcellxA[i] = pBQDev->Vcellx[i];
                if (pBQDev->VcellxA[i] == 0xFFFF ||
                    pBQDev->VcellxA[i] > MaxVol ||
                    pBQDev->VcellxA[i] < MinVol) {
                    // Open Wire Fail
                    // Customer task
                    // printf("Vcell %d Value_exceed:%f\n",i+1, pBQDev->VcellxA[i]);
                }
            }
            BQDev_TaskStatus = BQTaskStatus_VcOpn_WaitDelay;

            // Use Readvoltage delay to replace this timer delay.
            // Start delay task interrrupt
            // DL_Timer_setLoadValue(TIMER_BQTASK_INST, BQTask_VcOpn_DelayTime);
            // DL_Timer_startCounter(TIMER_BQTASK_INST);
            break;

        case BQTaskStatus_VcOpn_WaitDelay:
            // GetVCell(bID_temp, pBQDev, ReadBuf, Readtype);
            for (int i = 0; i < ACTIVECHANNELS; i++) {
                // Convert GPIOx's voltage depending on GPIOx_HI + GPIOx_LO
                pBQDev->VcellxB[i] = pBQDev->Vcellx[i];
                if (pBQDev->VcellxB[i] == 0xFFFF ||
                    pBQDev->VcellxB[i] > MaxVol ||
                    pBQDev->VcellxB[i] < MinVol) {
                    // Open Wire Fail
                    // Customer task
                    // printf("GPIO %d ABS_A_B:%f\n",i+1, ABS_A_B);
                }
                if (pBQDev->VcellxB[i] > pBQDev->VcellxA[i]) {
                    ABS_A_B = pBQDev->VcellxB[i] - pBQDev->VcellxA[i];
                } else {
                    ABS_A_B = pBQDev->VcellxA[i] - pBQDev->VcellxB[i];
                }

                if (ABS_A_B > 0.5 || ABS_A_B == 0) {
                    // Open Wire Fail
                    // Customer task
                    // printf("Vcell_AB %d ABS_A_B:%f\n",i+1, ABS_A_B);
                } else {
                    // Open Wire Pass5
                    // Customer task
                    // printf("Vcell_AB %d ABS_A_B:%f\n",i+1, ABS_A_B);
                }
            }

            BQDev_TaskStatus = BQTaskStatus_IDLE;
            break;

        default:
            // BQDev_TaskStatus is not IDLE or valid value.
            return -1;
            break;
    }
    return 0;
}

uint8_t BusbarInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    // 1. [BB_PIN_EN] = 0b1, [BB_PIN_LOC] = 0b00 (configure BB position).
    pBQDev->DevConf2.fs.num_cell  = 0b0111;
    pBQDev->DevConf2.fs.bb_pin_en = 0b1;
    pBQDev->DevConf2.fs.bb_pin_loc =
        0b00;  // 7s is busbar; customer configure by their project
    WriteReg(bID_temp, DEV_CONF2, pBQDev->DevConf2.reg, 1, Writetype);
    // pBQDev->BbvcPosn2.fs.cell9=0b1;
    // 2. [ADC_MODE] = 0b10, [ADC_GO] = 0b1(set ADC in continuous run mode).
    pBQDev->AdcCtrl2.fs.adc_mode = 0b10;
    pBQDev->AdcCtrl2.fs.adc_go   = 0b1;
    WriteReg(bID_temp, ADC_CTRL2, pBQDev->AdcCtrl2.reg, 1, Writetype);
    // 3. [DIAG_ANA_SEL] = CHANNEL 7; [DIAG_MEAS_GO]=0b1(at this point, DIAG_RDNT_HI/MI/LO is still CB cell 7).
    pBQDev->AdcCtrl3.fs.diag_ana_sel = 0b00111;
    pBQDev->AdcCtrl4.fs.diag_meas_go = 0b1;
    WriteReg(bID_temp, ADC_CTRL3, pBQDev->AdcCtrl3.reg, 1, Writetype);
    WriteReg(bID_temp, ADC_CTRL4, pBQDev->AdcCtrl4.reg, 1, Writetype);
    // rtn = WriteReg(bID_temp, BBVC_POSN2, pBQDev->BbvcPosn2.reg, 1, Writetype);

    return rtn;
}

uint8_t GetVBusbar(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype)
{
    uint8_t bID_temp  = 0;
    uint8_t Writetype = 0;

    if (Readtype == FRMWRT_SGL_R) {
        bID_temp  = BQID;
        Writetype = FRMWRT_SGL_W;
    } else if (Readtype == FRMWRT_STK_R) {
        Writetype = FRMWRT_STK_W;
    } else if (Readtype == FRMWRT_ALL_R) {
        Writetype = FRMWRT_ALL_W;
    } else {
        return -1;
    }

    // 4. The user can enable the VC/CB comparison by [DIAG_ANA_MODE] and [DIAG_ANA_GO].
    // 5. By the time the user would like to extract busbar IR drop: [BB_MEAS_MODE]=0b1, [DIAG_ANA_MODE] =0b00, [DIAG_ANA_GO]=0b1 (disable the VC/CB comparison, [BB_MEAS_MODE] takes effect).
    pBQDev->DiagAdcCtrl3.fs.bb_meas_mode  = 0b1;
    pBQDev->DiagAdcCtrl3.fs.diag_ana_mode = 0b00;
    pBQDev->DiagAdcCtrl3.fs.diag_ana_go   = 0b1;
    WriteReg(bID_temp, DIAG_ADC_CTRL3, pBQDev->DiagAdcCtrl3.reg, 1, Writetype);
    // 6. [DIAG_ANA_SEL] = CHANNEL 7; [DIAG_MEAS_GO]=0b1.
    pBQDev->AdcCtrl3.fs.diag_ana_sel = 0b00111;
    pBQDev->AdcCtrl4.fs.diag_meas_go = 0b1;
    WriteReg(bID_temp, ADC_CTRL3, pBQDev->AdcCtrl3.reg, 1, Writetype);
    WriteReg(bID_temp, ADC_CTRL4, pBQDev->AdcCtrl4.reg, 1, Writetype);
    // 7a. Wait until [DRDY_DIAG] =1, read out DIAG_RDNT_HI/MI/LO and DIAG_MAIN_HI/MI/LO
    do {
        // Read from BQ DIAG_STAT1
        ReadReg(bID_temp, ADC_DATA_RDY, ReadBuf, 1, 0, Readtype);
        // Need to confirm the data structure.
        pBQDev->AdcDataRdy.reg = ReadBuf[4];
    } while (pBQDev->AdcDataRdy.fs.drdy_diag == 0);
    // 7b. Subtract DIAG_MAIN_HI/MI/LO from DIAG_RDNT_HI/MI/LO to get the busbar IR drop.
    ReadReg(bID_temp, DIAG_MAIN_HI, ReadBuf, 6, 0, Readtype);
    pBQDev->Diag_rdnt_cbbb =
        ((ReadBuf[8] << 8) + ReadBuf[9]) * VLSB_STANDARD_F;
    pBQDev->Diag_main_vc = ((ReadBuf[5] << 8) + ReadBuf[6]) * VLSB_STANDARD_F;
    pBQDev->Vbusbar      = pBQDev->Diag_rdnt_cbbb - pBQDev->Diag_main_vc;
    return 0;
}

uint8_t BusbarVCInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype)
{
    uint8_t bID_temp = 0;
    uint8_t rtn      = 0;

    if (Writetype == FRMWRT_SGL_W) {
        bID_temp = BQID;
    }

    // 1. [CELL4] = 0b1, 4s is busbar; customer configure by their project
    pBQDev->BbvcPosn3.fs.cell4 = 0b1;
    WriteReg(bID_temp, BBVC_POSN3, pBQDev->BbvcPosn3.reg, 1, Writetype);
    // pBQDev->BbvcPosn2.fs.cell9=0b1;
    // 2. [CELL4] = 0b01, 4s is busbar; customer configure by their project
    pBQDev->UvDisable2.fs.cell4 = 0b1;
    WriteReg(bID_temp, UV_DISABLE2, pBQDev->UvDisable2.reg, 1, Writetype);

    return rtn;
}

// send the ADC bit value to the calculation function
void TMP61(bq78706_dev_t *pBQDev)
{
    // THRM calculations - 4th order polynomial regression
    float THRM_A0  = -2.691712E+02;
    float THRM_A1  = 5.062889E-02;
    float THRM_A2  = -3.099051E-06;
    float THRM_A3  = 1.153395E-10;
    float THRM_A4  = -1.746912E-15;
    float THRM_RES = 0;
    float RBias    = 10000;
    // float VBias = 4.00;
    // calculate the resistance of the thermistor
    if (pBQDev->GpioConf1.fs.gpio1 == ADC_RATIO ||
        pBQDev->GpioConf1.fs.gpio2 == ADC_RATIO ||
        pBQDev->GpioConf2.fs.gpio3 == ADC_RATIO ||
        pBQDev->GpioConf2.fs.gpio4 == ADC_RATIO ||
        pBQDev->GpioConf3.fs.gpio5 == ADC_RATIO ||
        pBQDev->GpioConf3.fs.gpio6 == ADC_RATIO ||
        pBQDev->GpioConf4.fs.gpio7 == ADC_RATIO ||
        pBQDev->GpioConf4.fs.gpio8 == ADC_RATIO ||
        pBQDev->GpioConf5.fs.gpio9 == ADC_RATIO ||
        pBQDev->GpioConf5.fs.gpio10 == ADC_RATIO ||
        pBQDev->GpioConf6.fs.gpio11 == ADC_RATIO) {
        for (int i = 0; i < GADC_ACTIVECHANNEL; i++) {
            THRM_RES = pBQDev->GPIOxVoltage[i] * 0.01 * RBias /
                       (1 - pBQDev->GPIOxVoltage[i] * 0.01);
            pBQDev->GPIOTemp[i] =
                (THRM_A4 * powf(THRM_RES, 4)) + (THRM_A3 * powf(THRM_RES, 3)) +
                (THRM_A2 * powf(THRM_RES, 2)) + (THRM_A1 * THRM_RES) + THRM_A0;
        }
    }
}

void TIMER_BQTASK_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_BQTASK_INST)) {
        case DL_TIMER_IIDX_ZERO:
            BQDev_TaskDelayReady = 1;
            break;
        default:
            break;
    }
}
