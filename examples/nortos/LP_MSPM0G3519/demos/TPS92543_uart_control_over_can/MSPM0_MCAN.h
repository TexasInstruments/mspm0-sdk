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

#ifndef MSPM0_MCAN_H_
#define MSPM0_MCAN_H_

#include "ti_msp_dl_config.h"

/**
 * MCAN Function
 */
void MCAN0_Restart(void);
void MCAN_LowPowerMode(void);

void DL_MCAN_readStdMsgIDFilter(
    MCAN_Regs *mcan, uint32_t filtNum, DL_MCAN_StdMsgIDFilterElement *elem);
void DL_MCAN_readExtMsgIDFilter(
    MCAN_Regs *mcan, uint32_t filtNum, DL_MCAN_ExtMsgIDFilterElement *elem);

uint8_t MCAN_SendPoll(
    MCAN_Regs *mcan, DL_MCAN_TxBufElement *txMsg, uint8_t length);
uint8_t MCAN_ReceivePoll(
    MCAN_Regs *mcan, DL_MCAN_RxBufElement *rxMsg, uint8_t length);

void MCAN_InterruptTransmissionControl(MCAN_Regs *mcan, bool enable);
void MCAN_SendInterruptStart(
    MCAN_Regs *mcan, DL_MCAN_TxBufElement *txMsg, uint8_t length);
uint8_t MCAN_SendInterruptService(MCAN_Regs *mcan);
uint8_t MCAN_ReceiveInterruptStart(
    MCAN_Regs *mcan, DL_MCAN_RxBufElement *rxMsg, uint8_t length);
uint8_t MCAN_ReceiveInterruptService(MCAN_Regs *mcan);

extern volatile uint8_t CANBusOff;
extern volatile uint8_t CANRxDone;
extern volatile uint8_t CANTxDone;

#endif /* MSPM0_MCAN_H_ */
