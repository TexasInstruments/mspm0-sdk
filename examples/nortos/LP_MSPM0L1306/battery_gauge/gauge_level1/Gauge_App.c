/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include "Gauge_App.h"

//*****************************************************************************
// Init algorithm used in Gauge
//*****************************************************************************
void App_AlgorithmInit(void)
{
    warningFlags.VcellHighWarnFlg         = 0;
    warningFlags.VcellLowWarnFlg          = 0;
    warningFlags.TcellHighWarnFlg         = 0;
    warningFlags.TcellLowWarnFlg          = 0;
    warningFlags.CircuitTableOverRangeFlg = 0;

#if (DETECTION_MODE == COMMUNICATION_DATA_INPUT)
    App_UartFrameInput();
#endif
    BSP_BattInfoMeasure(
        &battInfoInput);  //Measure the battery for the first time

    //Update average data for gauge algorithm
    battInfoInput.u16AvgVcell_mV = battInfoInput.u16Vcell_mV;
    battInfoInput.i8AvgTcell_C   = battInfoInput.i8Tcell_C;

    //Init Gauge algorithm
    VGauge_Init(&battGlobalParams);  //Step2: Voltage gauge init
    App_UartFrameOutput();
}

//*****************************************************************************
// Algorithm Process
//*****************************************************************************
void App_AlgorithmProcess(void)
{
    VGauge_Handler(&battGlobalParams);
}

//*****************************************************************************
// Update gauge input data
//*****************************************************************************
void App_BattInfoParamsUpdate(void)
{
    BSP_BattInfoMeasure(&battInfoInput);

    //Update Avg used for rest and full judgement

    battInfoInput.i16SumTcell += battInfoInput.i8Tcell_C;
    battInfoInput.u32SumVcell += battInfoInput.u16Vcell_mV;

    battInfoInput.u8SumCount++;

    if (battInfoInput.u8SumCount >= battParamsCfg.u8AvgBattParamsUpdateCount) {
        battInfoInput.u16AvgVcell_mV =
            battInfoInput.u32SumVcell /
            battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i8AvgTcell_C = battInfoInput.i16SumTcell /
                                     battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i16SumTcell            = 0;
        battInfoInput.u32SumVcell            = 0;
        battInfoInput.u8SumCount             = 0;
        battInfoInput.avgBattParamsUpdateFlg = 1;
    } else {
        battInfoInput.avgBattParamsUpdateFlg = 0;
    }

    //Temperature range test
    if (battInfoInput.i8Tcell_C > battParamsCfg.i8MaxTempThd_C) {
        warningFlags.TcellHighWarnFlg = eTrue;
    } else if (battInfoInput.i8Tcell_C < battParamsCfg.i8MinTempThd_C) {
        warningFlags.TcellLowWarnFlg = eTrue;
    }

    //voltage range test
    if (battInfoInput.u16Vcell_mV > battParamsCfg.u16MaxBattVoltThd_mV) {
        warningFlags.VcellHighWarnFlg = eTrue;
    } else if (battInfoInput.u16Vcell_mV <
               battParamsCfg.u16MinBattVoltThd_mV) {
        warningFlags.VcellLowWarnFlg = eTrue;
    }
}

//*****************************************************************************
// System init
//*****************************************************************************
void App_SystemInit(void)
{
    BSP_PeripheralsInit();  //Hardware init
    App_AlgorithmInit();    //Algorithm init
}

//*****************************************************************************
// Communication functions
//*****************************************************************************
void App_UartFrameInput(void)
{
    uint8_t u8RxDataCount = 0;
    uint16_t u16RxData    = 0;

    Driver_Uart1Init();
    //Will receive:
    //0x5a5a (header) + Vcell(unit16_t) + Icell(int16_t) + Tcell(int8_t)
    while (DL_UART_getRawInterruptStatus(UART1, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART1, DL_UART_INTERRUPT_RX);
    u16RxData += DL_UART_Main_receiveData(UART1);
    while (DL_UART_getRawInterruptStatus(UART1, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART1, DL_UART_INTERRUPT_RX);
    u16RxData += (DL_UART_Main_receiveData(UART1) << 8);

    if (u16RxData == 0x5a5a) {
        while (u8RxDataCount < 5) {
            while (DL_UART_getRawInterruptStatus(
                       UART1, DL_UART_INTERRUPT_RX) == 0)
                ;
            DL_UART_clearInterruptStatus(UART1, DL_UART_INTERRUPT_RX);
            inputRarams.u8Table[u8RxDataCount] =
                DL_UART_Main_receiveData(UART1);
            u8RxDataCount++;
        }
        battInfoInput.u16Vcell_mV = inputRarams.sInputParams.u16Vcell_mV;
        battInfoInput.i8Tcell_C   = inputRarams.sInputParams.i8Tcell_C;
    }

    //Disable UART
    DL_UART_Main_reset(UART1);
    DL_UART_Main_disablePower(UART1);
}

void App_UartFrameOutput(void)
{
    int16_t i16NomSoc;
    int16_t temp;
    static uint16_t counter = 0;

    //Init UART IO function

    Driver_Uart1Init();

    outputParams.u16Dr_Vcell_mV    = battInfoInput.u16Vcell_mV;
    outputParams.i16Dr_Icell_mA    = 0;
    outputParams.i8Dr_Tcell_C      = battInfoInput.i8Tcell_C;
    outputParams.u16Dr_AvgVcell_mV = battInfoInput.u16AvgVcell_mV;
    outputParams.i16Dr_AvgIcell_mA = 0;
    outputParams.i8Dr_AvgTcell_C   = battInfoInput.i8AvgTcell_C;

    // VGauge parameters
    outputParams.iq15VF_CurCalcOcv_mV = vGaugeParams.iq15CalculatedOcv[0];
    ;
    outputParams.iq15VFAbsSoc_DEC = vGaugeParams.iq15VFAbsSoc_DEC;

    // App
    outputParams.App_WarningFlag = warningFlags;

#if OUTPUT_MODE == UART_OUTPUT

    //Used to show the system is working
    counter++;
    Driver_Uart1TxU16Data(counter);

    Driver_Uart1TxU16Data(outputParams.u16Dr_Vcell_mV);
    Driver_Uart1TxU16Data(outputParams.i16Dr_Icell_mA);
    Driver_Uart1TxU16Data(outputParams.u16Dr_AvgVcell_mV);
    Driver_Uart1TxU16Data(outputParams.i16Dr_AvgIcell_mA);
    Driver_Uart1TxU16Data(outputParams.i8Dr_AvgTcell_C);

    //Amply 1000 times
    i16NomSoc = _IQ15int(_IQ15mpy(outputParams.iq15VFAbsSoc_DEC, _IQ15(1000)));
    Driver_Uart1TxU16Data(i16NomSoc);
    //Frame end
    Driver_Uart1TxU16Data(0x5a5a);

#if DETECTION_MODE == FLASH_DATA_INPUT
    delay_cycles(1600000);  //Avoid data transmition too fast
#endif
#endif

    // Disable UART function
    DL_UART_Main_reset(UART1);
    DL_UART_Main_disablePower(UART1);
}
