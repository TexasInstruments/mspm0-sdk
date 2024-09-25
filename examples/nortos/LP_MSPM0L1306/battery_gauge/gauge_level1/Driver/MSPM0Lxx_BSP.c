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

#include "Driver/MSPM0Lxx_BSP.h"
#include "BatteryRunFile_ChgDhg.h"

#define ADC_FULL_RANGE 4095

//NTC temp table shifted to 40C
const uint32_t tempNtcTable[NTC_TABLE_LENGTH] = {202269, 191064, 180555,
    170694, 161439, 152747, 144581, 136905, 129688, 122898, 116509, 110493,
    104827, 99488, 94456, 89710, 85233, 81008, 77019, 73252, 69693, 66329,
    63148, 60140, 57294, 54600, 52049, 49633, 47344, 45174, 43117, 41166,
    39315, 37559, 35891, 34307, 32803, 31373, 30014, 28722, 27494, 26324,
    25212, 24153, 23144, 22184, 21268, 20396, 19564, 18771, 18015, 17294,
    16605, 15948, 15320, 14720, 14148, 13600, 13077, 12577, 12099, 11641,
    11204, 10785, 10384, 10000, 9632, 9280, 8943, 8620, 8310, 8012, 7728, 7454,
    7192, 6940, 6699, 6467, 6244, 6030, 5825, 5628, 5438, 5256, 5080, 4912,
    4750, 4594, 4444, 4300, 4160, 4027, 3898, 3774, 3654, 3539, 3428, 3322,
    3218, 3119, 3023, 2931, 2842, 2756, 2673, 2593, 2516, 2441, 2369, 2299,
    2232, 2167, 2105, 2044, 1985, 1929, 1874, 1821, 1770, 1720, 1672, 1626,
    1581, 1538, 1496, 1455};

//*****************************************************************************
// BSP_SystemInit
//*****************************************************************************
void BSP_PeripheralsInit(void)
{
    Driver_GpioInit();
    Driver_TimerInit();
}

//*****************************************************************************
// Transmit ADC value to voltage using IQ15 type
//*****************************************************************************
_iq15 BSP_IQ15AdcCodeToVoltage(_iq15 iq15AdcCode)
{
    _iq18 iq18Result;

    //Avoid to exceed the iq15 range
    iq18Result =
        _IQ18mpy(_IQ18div(_IQ15toIQ18(iq15AdcCode), _IQ18(ADC_FULL_RANGE)),
            _IQ18(INTERNAL_REF));
    return _IQ18toIQ15(iq18Result);
}

//*****************************************************************************
// Measure the battery voltage, current and temperature
// And output the average voltage, current and temperature
//*****************************************************************************
#if (DETECTION_MODE == FLASH_DATA_INPUT)
uint32_t u32Count;
#endif

void BSP_BattInfoMeasure(tBattInfoInput* pBattInfoInput)
{
    _iq5 iq5AdcInCode;
    _iq5 iq5TcellCode;
    _iq5 iq5VcellCode;
    _iq5 iq5Vo2ToVo1;
    _iq15 iq15Vshunt_mV;
    _iq15 iq15ResFactor;
    int16_t i16Counter;
    uint32_t u32Rntc_mR;

#if (DETECTION_MODE == FLASH_DATA_INPUT)
    pBattInfoInput->u16Vcell_mV = i16BattRunFile[2 * u32Count + 1];
    pBattInfoInput->i8Tcell_C   = 25;
    u32Count++;
    if (u32Count > (DATA_RUN_FILE - 2)) {
        __BKPT(0);
        u32Count = 0;
    }
#endif

#if (DETECTION_MODE == DETECTION_DATA_INPUT)
    Driver_DetectionPowerEnable();
    Driver_VrefInit();

    iq5TcellCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_5, DL_ADC12_REFERENCE_VOLTAGE_VDDA, 0);
    u32Rntc_mR = _IQ5int(iq5TcellCode) * NTC_PAIR_RESISTOR_VALUE /
                 (ADC_FULL_RANGE - _IQ5int(iq5TcellCode));
    for (i16Counter = 0; i16Counter < NTC_TABLE_LENGTH;
         i16Counter++)  //rounding the Tcell
    {
        if (u32Rntc_mR > tempNtcTable[i16Counter]) {
            if ((tempNtcTable[i16Counter - 1] - u32Rntc_mR) <
                (u32Rntc_mR - tempNtcTable[i16Counter])) {
                pBattInfoInput->i8Tcell_C = i16Counter - 1 + NTC_SHIFT;
                break;
            } else {
                pBattInfoInput->i8Tcell_C = i16Counter + NTC_SHIFT;
                break;
            }
        }
    }

    iq5VcellCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_1, DL_ADC12_REFERENCE_VOLTAGE_INTREF, 0);
    pBattInfoInput->u16Vcell_mV =
        _IQ15int(_IQ15mpy(BSP_IQ15AdcCodeToVoltage(_IQ5toIQ15(iq5VcellCode)),
            _IQ15(VCELL_FACTOR)));

    Driver_DetectionPowerDisable();
#endif
}
