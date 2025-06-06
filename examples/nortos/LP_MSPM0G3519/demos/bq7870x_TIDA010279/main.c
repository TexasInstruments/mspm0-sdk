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

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bq78706.h"
#include "bq78706_app.h"
#include "mspm0_mcan.h"
#include "ti_msp_dl_config.h"

bq78706_dev_t gBQDev[TOTALBOARDS] = {0};
bq78706_syssta_t gBQSta           = {0};

uint8_t gBQRxBuffer[(MAXBYTES + 6) * TOTALBOARDS + 64] = {0};

volatile uint32_t gSysTickCnt  = 0;
volatile bool gTaskReadVoltage = false;
volatile bool gTaskDignostic   = false;
volatile bool gTaskCellBalance = false;

void SysTask_CellBalance(void);
void SysTask_ReadVoltage(void);
uint8_t SysTask_Dignostic(void);
void SysTask_Dignostic_VcOpnWrUpdate(void);

/**
 * CAN Buffer to AFE Operation Variables
 */
uint8_t txMsgbuffer[TOTALBOARDS * ACTIVECHANNELS * 2 +
                    TOTALBOARDS * GADC_ACTIVECHANNEL * 2];
/**
 * CAN Tx Buffer Operation Variables
 */
DL_MCAN_TxBufElement txMsg0[ACTIVECHANNELS + GADC_ACTIVECHANNEL];

uint32_t idArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24};

int main(void)
{
    SYSCFG_DL_init();

    // Enable BQ Communication UART interrupt
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    // Enable BQ task delay timer's interrupt.
    NVIC_EnableIRQ(TIMER_BQTASK_INST_INT_IRQN);
    // Enable CAN interrupt
    NVIC_EnableIRQ(MCAN0_INST_INT_IRQN);

    // BQ dev and BQ sta init
    BQParamInit(&gBQDev[0], &gBQSta);

    gBQSta.CANIDSet = mcan_autoAddress(MCAN0_INST);

    for (int j = 0; j < ACTIVECHANNELS + GADC_ACTIVECHANNEL; j++) {
        /* Identifier Value, need to <<18U to fit into register. */
        txMsg0[j].id = (gBQSta.CANIDSet * 100 + idArray[j]) << 18U;
        /* Transmit data frame. */
        txMsg0[j].rtr = 0U;
        /* 11-bit standard identifier. */
        txMsg0[j].xtd = 0U;
        /* ESI bit in CAN FD format depends only on error passive flag. */
        txMsg0[j].esi = 0U;
        /* Transmitting 8 bytes. */
        txMsg0[j].dlc = 8U;
        /* CAN FD frames transmitted with bit rate switching. */
        txMsg0[j].brs = 0U;
        /* Frame transmitted in CAN FD format. */
        txMsg0[j].fdf = 0U;
        /* Store Tx events. */
        txMsg0[j].efc = 0U;
        /* Message Marker. */
        txMsg0[j].mm = 0xAAU;
    }

    WakeStackDevice();

    AutoAddress(&gBQSta, gBQRxBuffer);
    // DisableTimeout(0, &gBQDev[0], FRMWRT_ALL_W); // Debug
    AdcCtrlInit(0, &gBQDev[0], FRMWRT_ALL_W);
    GpioConfInit(0, &gBQDev[0], FRMWRT_ALL_W);
    // BusbarInit(0, &gBQDev[0], FRMWRT_SGL_W); // Optional
    // BusbarVCInit(0, &gBQDev[0], FRMWRT_SGL_W); // Optional
    AdcConversion(0, &gBQDev[0], FRMWRT_ALL_W);
    ADCReadyCheck(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_R);
    ResetAllFaults(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_W);

    MaskAllFaults(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_W);

    // SysTask_CellBalance(); // CB test debug

    while (1) {
        __NOP();
        __WFI();

        /****************************************************************
        *  Read voltage task
        */
        if (gTaskReadVoltage == true) {
            SysTask_ReadVoltage();
            // Entering this 8 times for 4 BQ Dev
            if ((gSysTickCnt >= (200 * TOTALBOARDS)) &&
                (gSysTickCnt <= 5000)) {
                SysTask_Dignostic_VcOpnWrUpdate();
            }
            gTaskReadVoltage = false;
            continue;
        }

        /****************************************************************
        *  Dignostic task
        */
        if (gTaskDignostic == true) {
            if ((gSysTickCnt % 100) >= 80) {
                continue;
            }
            if (SysTask_Dignostic() == 0) {
                gTaskDignostic = false;
            }
            continue;
        }

        /****************************************************************
        *  Cell Balance task
        */
        if (gTaskCellBalance == true) {
            gTaskCellBalance = false;
            continue;
        }

        /****************************************************************
        *  BQ Device delay task manager
        */
        if (BQDev_TaskDelayReady == 1) {
            switch (BQDev_TaskStatus) {
                case BQTaskStatus_IDLE:
                    // Need to clear the Task status.
                    BQDev_TaskDelayReady = 0;
                    break;
                case BQTaskStatus_OTP_WaitDelay:
                    OTP(0, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
                    break;
                default:
                    break;
            }
        }
    }
}

void SysTask_ReadVoltage(void)
{
    uint8_t BQBalState[TOTALBOARDS] = {0};
    uint8_t loop_i                  = 0;

    // Update, save the Cell Balance status
    // Pause the active cell balance
    for (loop_i = 0; loop_i < TOTALBOARDS; loop_i++) {
        GetBalSts(loop_i, &gBQDev[loop_i], gBQRxBuffer, FRMWRT_SGL_R);
        if (gBQDev[loop_i].BalStat.fs.cb_run == 1) {
            BQBalState[loop_i] = 1;
            // Pause cell balance
            PauseBalCtrl(loop_i, &gBQDev[loop_i], FRMWRT_SGL_W);
        }
    }
    // Waiting for cell balance is paused.
    for (loop_i = 0; loop_i < TOTALBOARDS; loop_i++) {
        if (BQBalState[loop_i] == 1) {
            do {
                GetBalSts(loop_i, &gBQDev[loop_i], gBQRxBuffer, FRMWRT_SGL_R);
            } while (gBQDev[loop_i].BalStat.fs.cb_inpause == 0);
        }
    }

    // Ccb capacitor settle time
    // Delay time depends on Ccb*Rcb, TIDA-010279, 0.0034ms->200us test log
    delayus(200);
    // Settle time reached and defreeze DLPF for valid reading
    for (loop_i = 0; loop_i < TOTALBOARDS; loop_i++) {
        if (BQBalState[loop_i] == 1) {
            DLPF_Defreeze(loop_i, &gBQDev[loop_i], gBQRxBuffer, FRMWRT_SGL_R);
        }
    }
    delayus(128 * 2);  // t_Vcell_refresh

    // Start cell voltage update 1st, voltage data should remains stable in 1mV
    GetVCell(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_R);
    delayus(128);
    // Start cell voltage update 1st, voltage data should remains stable in 1mV
    GetVCell(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_R);
    for (int j = 0; j < TOTALBOARDS; j++) {
        for (int i = 0; i < 2 * ACTIVECHANNELS; i++) {
            txMsgbuffer[i + j * ACTIVECHANNELS * 2] =
                gBQDev[j].VcellSinglex[i];
        }
    }
    GetVGpio(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_R);
    TMUX_GPIO(0, &gBQDev[0], gBQRxBuffer, FRMWRT_ALL_W);

    for (int j = 0; j < TOTALBOARDS; j++) {
        for (int i = 0; i < 2 * GADC_ACTIVECHANNEL; i++) {
            txMsgbuffer[i + j * GADC_ACTIVECHANNEL * 2 +
                        ACTIVECHANNELS * 2 * TOTALBOARDS] =
                gBQDev[j].VGPIOSinglex[i];
        }
    }
    for (int j = 0; j < GADC_ACTIVECHANNEL + ACTIVECHANNELS; j++) {
        /* Data bytes. */
        for (int i = 0; i < txMsg0[j].dlc; i++) {
            txMsg0[j].data[i] = txMsgbuffer[i + j * 8];
        }
    }
    MCAN_SendPoll(MCAN0_INST, &txMsg0[0], GADC_ACTIVECHANNEL + ACTIVECHANNELS);

    // Finish cell voltage update, freeze DPLF before continue cell balancing
    for (loop_i = 0; loop_i < TOTALBOARDS; loop_i++) {
        if (BQBalState[loop_i] == 1) {
            DLPF_freeze(loop_i, &gBQDev[loop_i], gBQRxBuffer, FRMWRT_SGL_R);
        }
    }

    // Resume the cell balance
    for (int loop_i = 0; loop_i < TOTALBOARDS; loop_i++) {
        if (BQBalState[loop_i] == 1) {
            ResumeBalCtrl(loop_i, &gBQDev[loop_i], FRMWRT_SGL_W);
            do {
                GetBalSts(loop_i, &gBQDev[loop_i], gBQRxBuffer, FRMWRT_SGL_R);
            } while (gBQDev[loop_i].BalStat.fs.cb_inpause == 1);
            GetBalRemainTime(
                loop_i, &gBQDev[loop_i], gBQRxBuffer, FRMWRT_SGL_R);
        }
    }

    return;
}

void SysTask_Dignostic_VcOpnWrUpdate(void)
{
    static uint8_t subCount = 0;
    static uint8_t BQDevID  = 0;

    VcOpnWrUpdate(BQDevID, &gBQDev[BQDevID], gBQRxBuffer, 3, 4, FRMWRT_SGL_W);

    subCount++;
    if (subCount == 2) {
        subCount = 0;
        BQDevID++;
        if (BQDevID == TOTALBOARDS) {
            subCount = 0;
            BQDevID  = 0;
        }
    }
}

uint8_t SysTask_Dignostic(void)
{
    static uint8_t BQDevID = 0;
    uint8_t rtn            = 1;

    // First call this function
    if (BQDev_TaskStatus == BQTaskStatus_IDLE) {
        GetAllFaults(BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
        // Conflict with CB, will pause CB in this API
        AcompDiag(BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
    } else {
        switch (BQDev_TaskStatus) {
            case BQTaskStatus_IDLE:
                // Normally, won't run into here
                // Need to clear the Task status
                BQDev_TaskDelayReady = 0;
                break;
            case BQTaskStatus_AcompDiag_WaitDelay:
                // Conflict with CB, will pause CB in this API
                AcompDiag(BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
                GpioOpnWrUpdate(
                    BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
                break;
            case BQTaskStatus_GpioOpn_WaitDelay:
                GpioOpnWrUpdate(
                    BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
                CbfetDiag(BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_W);
                break;
            case BQTaskStatus_CbfetDiag_WaitDelay:
                // Conflict with ADC and CB, will pause CB in this API
                CbfetDiag(BQDevID, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_W);
                // Task finish
                BQDevID++;
                if (BQDevID == (TOTALBOARDS)) {
                    BQDevID = 0;
                    rtn     = 0;
                }
                break;
            default:
                break;
        }
    }
    return rtn;
}

void SysTask_CellBalance(void)
{
    // 1.Channel selection and set cell balancing time for each CB pin
    SetCbTime(
        0, &gBQDev[0], 1, 300, FRMWRT_SGL_W);  // Step1: channel 1 and 300s
    // 2.PWM setting ( By default PWM duty cycle is 1.5625% )
    SetBalCtrlPWM(
        0, &gBQDev[0], 100, FRMWRT_SGL_W);  //Setp2: set PWM =0x3F, 100%
    // 3a. Balancing thermal management
    // 3b. Steps to configure OTCB
    OTCBInit(0, &gBQDev[0], 63,
        FRMWRT_ALL_W);  //Setp3: set OBCT  as 07, 63%, PTC_EN(set in ADCinit)
    OTUTInit(0, &gBQDev[0], FRMWRT_SGL_W);  //Setp4: set OTUT
    // 4. Set cell voltage threshold to stop cell balancing. ( By default voltage threshold monitoring is disabled )
    UVCBInit(0, &gBQDev[0], 3000, FRMWRT_SGL_W);
    // Not use 5. Configuring the CB behavior during active fault
    // 6. Configure the balancing type ( Auto / Manual ) ( By default the system will be configured for manual cell balancing )
    StartBalCtrl(0, &gBQDev[0], FRMWRT_SGL_W);
    // 7. Configuring balancing action during sleep mode, procedure to start balancing, stop balancing, read balance stat
    GetBalSts(0, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
    // 8. Read the remaining balancing time of a CB channel.
    GetBalRemainTime(0, &gBQDev[0], gBQRxBuffer, FRMWRT_SGL_R);
}

// 10 ms Systick
void SysTick_Handler(void)
{
    gSysTickCnt += 10;

    if (gSysTickCnt == (5000)) {
        gSysTickCnt = 0;
    }

    if (gSysTickCnt % 10 != 0) {
        gSysTickCnt = 0;
    }

    if (gSysTickCnt % 100 == 0) {
        // Set task flag: Read voltage
        gTaskReadVoltage = true;
    }

    if (gSysTickCnt % 5000 == 0) {
        // Set task flag: Dignostic
        gTaskDignostic = true;
    }
}

// Debug
void NMI_Handler(void)
{
    __asm("NOP");
    __BKPT(0);
}
// Debug
void HardFault_Handler(void)
{
    __asm("NOP");
    __BKPT(0);
}
