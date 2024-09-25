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

#include "Driver/MSPM0Gxx_BSP.h"
#include "BQ769x2_protocol.h"
char enableReadFlag = 0;

uint32_t debug[20] = {0};

//*****************************************************************************
// BSP_SystemInit
//*****************************************************************************
void BSP_PeripheralsInit(void)
{
    SYSCFG_DL_init();
    Driver_UartInit();
    Driver_I2cInit();
    Driver_TimerInit();
    delayUS(10000);
}

//*****************************************************************************
// Calibrate systick
//*****************************************************************************
void BSP_SysTikCali(void)
{
    uint16_t u16Counter;
    uint32_t u32TimerCounter;

    DL_SYSCTL_configFCC(DL_SYSCTL_FCC_TRIG_TYPE_RISE_RISE,
        DL_SYSCTL_FCC_TRIG_SOURCE_LFCLK, DL_SYSCTL_FCC_CLOCK_SOURCE_SYSOSC);
    DL_SYSCTL_setFCCPeriods(DL_SYSCTL_FCC_TRIG_CNT_02);
    DL_SYSCTL_startFCC();
    while (!(DL_SYSCTL_getClockStatus() & SYSCTL_CLKSTATUS_FCCDONE_DONE))
        ;
    u16Counter = DL_SYSCTL_readFCC();
    //u16Counter*(32M*2/32768)*4096*sysTikFreq/u8SysTikShift
    u32TimerCounter = 32768 / gaugeApp.u8SysTikShift * u16Counter *
                      gaugeApp.sysTikFreq / 15625;
    DL_Timer_setLoadValue(TIMER_INST, u32TimerCounter);
}

//*****************************************************************************
// Measure the battery voltage, current and temperature
// And output the average voltage, current and temperature
//*****************************************************************************
extern uint16_t AlarmBits;
extern uint8_t FET_Status;
void BSP_BattInfoMeasure(tGaugeApplication *pGaugeApp)
{
#if (DETECTION_MODE == DETECTION_DATA_INPUT)
    Driver_I2cInit();
    BQ769x2_ReadAllVoltages();  // write 0x14, read 2 bytes. From 0x14 - 0x38
    BQ769x2_ReadCurrent();      // write 0x3A, read 2 bytes.
    BQ769x2_ReadAllTemperatures();  // write 0x70, read 2 bytes. write 0x72, read 2 bytes. write 0x74, read 2 bytes.
    BQ769x2_ReadAlarmStatus();
    BQ769x2_ReadFETStatus();

    if (AlarmBits & (~0x82) || (FET_Status != 0x45)) {
        BQ769x2_ReadSafetyStatus();
        BQ769x2_ReadFETStatus();
        BQ769x2_ReadPFStatus();
        BQ769x2_Init(pGaugeApp);
    }

    Driver_I2cReset();
#endif
}
