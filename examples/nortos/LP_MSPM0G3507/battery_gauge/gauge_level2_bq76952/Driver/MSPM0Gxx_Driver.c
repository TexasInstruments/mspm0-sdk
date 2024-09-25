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

#include "Driver/MSPM0Gxx_Driver.h"

//*****************************************************************************
// I2C control
//*****************************************************************************
void Driver_I2cInit(void)
{
    DL_I2C_reset(I2C_0_INST);
    DL_I2C_enablePower(I2C_0_INST);
    SYSCFG_DL_I2C_0_init();
}

void Driver_I2cReset(void)
{
    DL_I2C_reset(I2C_0_INST);
    DL_I2C_disablePower(I2C_0_INST);
}
//*****************************************************************************
// UART control
//*****************************************************************************

void Driver_UartInit(void)
{
    DL_UART_Main_reset(UART_SYS_INST);
    DL_UART_Main_enablePower(UART_SYS_INST);
    delay_cycles(POWER_STARTUP_DELAY);
    SYSCFG_DL_UART_SYS_init();
}

void Driver_UartReset(void)
{
    //Disable UART
    DL_UART_Main_reset(UART_SYS_INST);
    DL_UART_Main_disablePower(UART_SYS_INST);
}

void Driver_UartTxU16Data(uint16_t u16Data)
{
    while (DL_UART_isBusy(UART_SYS_INST))
        ;
    DL_UART_Main_transmitData(UART_SYS_INST, (u16Data & 0xFF00) >> 8);
    while (DL_UART_isBusy(UART_SYS_INST))
        ;
    DL_UART_Main_transmitData(UART_SYS_INST, u16Data & 0xFF);
    while (DL_UART_isBusy(UART_SYS_INST))
        ;
}

void Driver_UartTxU32Data(uint32_t u32Data)
{
    Driver_UartTxU16Data((u32Data & 0xFFFF0000) >> 16);
    Driver_UartTxU16Data(u32Data & 0xFFFF);
}

uint16_t Driver_UartRxU16Data(void)
{
    uint16_t u16RxData;

    u16RxData = 0;
    while (DL_UART_getRawInterruptStatus(
               UART_SYS_INST, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART_SYS_INST, DL_UART_INTERRUPT_RX);
    u16RxData += (DL_UART_Main_receiveData(UART_SYS_INST) << 8);

    while (DL_UART_getRawInterruptStatus(
               UART_SYS_INST, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART_SYS_INST, DL_UART_INTERRUPT_RX);
    u16RxData += DL_UART_Main_receiveData(UART_SYS_INST);

    return u16RxData;
}

uint32_t Driver_UartRxU32Data(void)
{
    uint32_t u32RxData;
    uint16_t u16RxData;

    u32RxData = (Driver_UartRxU16Data() << 16);
    u16RxData = Driver_UartRxU16Data();

    return u32RxData + u16RxData;
}
//*****************************************************************************
// Timer Init
//*****************************************************************************
//*****************************************************************************
// Timer Init
//*****************************************************************************
DL_TimerG_TimerConfig gTIMER_INSTTimerConfig = {
    /* Timer configuration to operate in periodic mode */
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP};

void Driver_TimerInit(void)
{
    SYSCFG_DL_TIMER_init();

    //When period = 4096, it is one second
    gTIMER_INSTTimerConfig.period =
        4096 * gaugeApp.sysTikFreq / gaugeApp.u8SysTikShift;
    DL_TimerG_initTimerMode(
        TIMER_INST, (DL_TimerG_TimerConfig *) &gTIMER_INSTTimerConfig);
    NVIC_EnableIRQ(TIMER_INST_INT_IRQN);
    DL_TimerG_startCounter(TIMER_INST);
}

void TIMG0_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_INST)) {
        case DL_TIMER_IIDX_ZERO:
            gaugeApp.sysTikFlag = 1;
            break;
        default:
            break;
    }
}
