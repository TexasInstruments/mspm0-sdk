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

#include <App.h>

//*****************************************************************************
// Init algorithm used in Gauge
//*****************************************************************************
void App_AlgorithmInit(void)
{
    BSP_CurtCalcFactorsCali(
        &curtCalcFactors);  //calibrate current detection module
    BSP_BattInfoMeasure(&battInfoInput,
        &curtCalcFactors);  //Measure the battery for the first time

    //Update average data for gauge algorithm
    battInfoInput.u16AvgVcell_mV = battInfoInput.u16Vcell_mV;
    battInfoInput.i8AvgTcell_C   = battInfoInput.i8Tcell_C;
    battInfoInput.i16AvgIcell_mA = battInfoInput.i16Icell_mA;
    battInfoInput.passiveAvgFlg  = eFalse;

    App_UartFrameOutput();
}

//*****************************************************************************
// Update input data
//*****************************************************************************
void App_BattInfoParamsUpdate(void)
{
    BSP_BattInfoMeasure(&battInfoInput, &curtCalcFactors);

    //Update Avg used for rest and full judgement
    battInfoInput.i16SumTcell += battInfoInput.i8Tcell_C;
    battInfoInput.i32SumIcell += battInfoInput.i16Icell_mA;
    battInfoInput.u32SumVcell += battInfoInput.u16Vcell_mV;

    battInfoInput.u8SumCount++;

    if (battInfoInput.u8SumCount >= battParamsCfg.u8AvgBattParamsUpdateCount) {
        battInfoInput.u16AvgVcell_mV =
            battInfoInput.u32SumVcell /
            battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i8AvgTcell_C = battInfoInput.i16SumTcell /
                                     battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i16AvgIcell_mA =
            battInfoInput.i32SumIcell /
            battParamsCfg.u8AvgBattParamsUpdateCount;
        battInfoInput.i16SumTcell            = 0;
        battInfoInput.i32SumIcell            = 0;
        battInfoInput.u32SumVcell            = 0;
        battInfoInput.u8SumCount             = 0;
        battInfoInput.avgBattParamsUpdateFlg = 1;
    } else {
        battInfoInput.avgBattParamsUpdateFlg = 0;
    }

    //Update Capacity learn when the average data is prepared
    //Update frequency = sysTikFreq * u8AvgBattParamsUpdateCount
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
//*****************************************************************************}
void App_UartFrameOutput(void)
{
    int16_t i16SmoothSoc, i16AbsSoc, i16NomSoc, i16SOH, i16DeltaSoc,
        i16DeltaCap, i16Efcy, i16Data;
    int16_t temp;
    uint16_t u16Data;
    uint16_t* p_u16Flags;
    static uint16_t counter = 0;

    Driver_Uart1Init();

    outputParams.u16Dr_Vcell_mV    = battInfoInput.u16Vcell_mV;
    outputParams.i16Dr_Icell_mA    = battInfoInput.i16Icell_mA;
    outputParams.i8Dr_Tcell_C      = battInfoInput.i8Tcell_C;
    outputParams.u16Dr_AvgVcell_mV = battInfoInput.u16AvgVcell_mV;
    outputParams.i16Dr_AvgIcell_mA = battInfoInput.i16AvgIcell_mA;
    outputParams.i8Dr_AvgTcell_C   = battInfoInput.i8AvgTcell_C;

#if OUTPUT_MODE == UART_OUTPUT

    //Used to show the system is working
    counter++;
    Driver_Uart1TxU16Data(counter);

    Driver_Uart1TxU16Data(outputParams.u16Dr_Vcell_mV);
    Driver_Uart1TxU16Data(outputParams.i16Dr_Icell_mA);
    Driver_Uart1TxU16Data(outputParams.u16Dr_AvgVcell_mV);
    Driver_Uart1TxU16Data(outputParams.i16Dr_AvgIcell_mA);
    Driver_Uart1TxU16Data(outputParams.i8Dr_AvgTcell_C);

    //Frame end
    Driver_Uart1TxU16Data(0x5a5a);

#endif
    //Disable UART
    DL_UART_Main_reset(UART1);
    DL_UART_Main_disablePower(UART1);
}
