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

#ifndef BQ78706_DATA_TYPES_H_
#define BQ78706_DATA_TYPES_H_

#include "bq78706.h"
#include "bq78706_B0_reg.h"

/**
 * @brief Basic types definition
 */
typedef unsigned int BOOL;
typedef signed int HANDLE;

/**
 * @brief TRUE and FALSE definition
 */
#define true 1
#define false 0
#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif

/**
 * @brief BQ78706 Register table
 *
 */
typedef struct {
    cb_cellx_ctrl_t CbCellxCtrl;
    bal_ctrl1_t BalCtrl1;
    bal_ctrl2_t BalCtrl2;
    dev_conf1_t DevConf1;
    dev_conf2_t DevConf2;
    dev_conf5_t DevConf5;
    otcb_thresh_t OtcbThresh;
    vcbdone_thresh_t VcbdoneThresh;
    ovuv_ctrl2_t OvuvCtrl2;
    bal_stat_t BalStat;
    bal_ctrl3_t BalCtrl3;
    bal_time_t BalTime;

    adc_ctrl1_t AdcCtrl1;
    adc_ctrl2_t AdcCtrl2;
    adc_ctrl3_t AdcCtrl3;
    adc_ctrl4_t AdcCtrl4;

    bbvc_posn2_t BbvcPosn2;
    bbvc_posn3_t BbvcPosn3;

    adc_conf_t AdcConf;
    uv_disable2_t UvDisable2;
    diag_adc_ctrl1_t DiagAdcCtrl1;
    diag_adc_ctrl2_t DiagAdcCtrl2;
    diag_adc_ctrl3_t DiagAdcCtrl3;
    adc_data_rdy_t AdcDataRdy;
    diag_stat1_t DiagStat1;
    diag_stat2_t DiagStat2;
    gpio_stat1_t GpioStat1;
    gpio_conf1_t GpioConf1;
    gpio_conf2_t GpioConf2;
    gpio_conf3_t GpioConf3;
    gpio_conf4_t GpioConf4;
    gpio_conf5_t GpioConf5;
    gpio_conf6_t GpioConf6;
    //yan
    otut_ctrl2_t Otutctrl2;

    // Diagnostic
    fault_summary_t FaultSummary;
    // fault_rst1_t FaultRst1;
    // fault_rst2_t FaultRst2;
    fault_pwr1_t FaultPwr1;
    fault_pwr2_t FaultPwr2;
    fault_comm_t FaultComm;
    fault_otp_t FaultOtp;

    // GPIO1 ~ GPIO11's voltage
    float GPIOxVoltage[11];
    float GPIOxVoltageA[11];
    float GPIOxVoltageB[11];
    uint8_t VGPIOSinglex[11 * 2];
    float GPIOTemp[11];
    uint8_t VcellOffsetx[13];
    uint8_t VcellSinglex[13 * 2];
    float Vcellx[13];
    float VcellxA[13];
    float VcellxB[13];

    float Vbusbar;
    float Diag_rdnt_cbbb;
    float Diag_main_vc;

    dev_stat1_t DevStat1;
    dev_stat2_t DevStat2;
    fault_adc_misc_t FaultAdcMisc;
    fault_adc_gpio1_t FaultAdcGpio1;
    fault_adc_gpio2_t FaultAdcGpio2;
    fault_adc_vcell1_t FaultAdcVcell1;
    fault_adc_vcell2_t FaultAdcVcell2;
    fault_adc_vcell3_t FaultAdcVcell3;

    otp_prog_unlock1_t OtpProgUnlock1;
    otp_prog_unlock2_t OtpProgUnlock2;
    otp_stat_t OtpStat;
    control2_t Control2;
    control1_t Control1;

    diag_misc_ctrl2_t DiagMiscCtrl2;
    fault_cb_fetow1_t FaultCbFetow1;
    fault_cb_fetow2_t FaultCbFetow2;
    fault_cb_fetow3_t FaultCbFetow3;

} bq78706_dev_t;

/**
 * @brief BQ78706 Register table
 *
 */
typedef struct {
    uint8_t TaskStatus;
    volatile uint8_t TaskDelayReady;

    uint8_t RecoveryMode;
    uint8_t RecoveryMode_Reverse;

    uint8_t FaultNode;
    uint8_t BrokenNodeID;
    uint8_t FaultNode_Reverse;
    uint8_t BrokenNodeID_Reverse;

    uint8_t StackTopNodeID;
    uint8_t StackTopNodeID_Reverse;

    uint8_t CANIDSet;
} bq78706_syssta_t;

#endif /* BQ78706_DATA_TYPES_H_ */
