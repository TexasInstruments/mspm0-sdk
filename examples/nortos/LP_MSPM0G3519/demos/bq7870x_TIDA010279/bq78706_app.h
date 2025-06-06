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

#ifndef BQ78706_APP_H_
#define BQ78706_APP_H_

#include "bq78706.h"
#include "bq78706_data_types.h"

void WakeStackDevice(void);
void BQParamInit(bq78706_dev_t *pBQDev, bq78706_syssta_t *pBQSta);

/**
 * @brief Address and Recovery function group
 *
 */
void AutoAddress(bq78706_syssta_t *pBQSta, uint8_t *ReadBuf);
void AutoAddressReverse(bq78706_syssta_t *pBQSta, uint8_t *ReadBuf);
void BQDeviceRecovery(
    bq78706_dev_t *pBQDev, bq78706_syssta_t *pBQSta, uint8_t *ReadBuf);

/**
 * @brief Cell balance function group
 *
 */
uint32_t SetCbTime(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t CellNum,
    uint32_t CBTime, uint8_t Writetype);
uint8_t SetBalCtrlPWM(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Duty, uint8_t Writetype);
uint8_t StartBalCtrl(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t OTCBInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t OTCBThrVal,
    uint8_t Writetype);
uint16_t UVCBInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint16_t VCBDThrVal,
    uint8_t Writetype);
uint8_t PauseBalCtrl(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t ResumeBalCtrl(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t GetBalSts(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint16_t GetBalRemainTime(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);

/**
 * @brief ADC, GPIO function group
 *
 */
uint8_t AdcCtrlInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t GpioConfInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t AdcConversion(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t GetADCStatus(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t GetADCDiagStatus(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t ADCReadyCheck(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf,
    uint8_t Readtype);  //yan
uint8_t GetVCell(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t GetVGpio(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t DLPF_freeze(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t DLPF_Defreeze(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t TMUX_GPIO(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype);
/**
 * @brief Diagnostic function group
 *
 */
uint8_t GetAllFaults(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t ResetAllFaults(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype);
uint8_t MaskAllFaults(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype);
uint8_t GpioOpnWrUpdate(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t CellOpnWrUpdate(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t AcompDiag(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t OTP(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype);
uint8_t CbfetDiag(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Writetype);
/**
 * @brief Added function group
 *
 */
uint8_t OTUTInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t DisableTimeout(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t VcOpnWrUpdate(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf,
    float MaxVol, float MinVol, uint8_t Readtype);
uint8_t BusbarInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
uint8_t GetVBusbar(
    uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t *ReadBuf, uint8_t Readtype);
uint8_t BusbarVCInit(uint8_t BQID, bq78706_dev_t *pBQDev, uint8_t Writetype);
void TMP61(bq78706_dev_t *pBQDev);

/**
 * @brief BQ Task Flag
 */
#define BQTaskStatus_IDLE 0x00
#define BQTaskStatus_AcompDiag_WaitDelay 0x01
#define BQTaskStatus_VcOpn_WaitDelay 0x02
#define BQTaskStatus_GpioOpn_WaitDelay 0x03
#define BQTaskStatus_CbfetDiag_WaitDelay 0x04
#define BQTaskStatus_OTP_WaitDelay 0x05

/**
 * @brief Delay time = 20us * DelayTime
 */
#define BQTask_GpioOpn_DelayTime 50      // 50*20us = 1ms
#define BQTask_AcompDiag_DelayTime0 424  // 424*20us = 8.48ms
#define BQTask_AcompDiag_DelayTime1 730  // 730*20us = 14.6ms
#define BQTask_OTP_DelayTime 10000       // 10000*20us = 200ms
#define BQTask_CbfetDiag_DelayTime 1000  // 1000*20us = 20ms
#define BQTask_1MS_DelayTime 50          // 50*20us = 1ms
#define BQTask_VcOpn_DelayTime 5000      // 5000*20us = 100ms

extern uint8_t BQDev_TaskStatus;
extern volatile uint8_t BQDev_TaskDelayReady;

#endif
