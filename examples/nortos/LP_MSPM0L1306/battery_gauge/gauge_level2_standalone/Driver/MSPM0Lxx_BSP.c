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

#include "Driver/MSPM0Lxx_BSP.h"

#define ADC_FULL_RANGE 0xFFF  //ADC full range
#define VOLT_SWING 0xB0       //60mV under 1.4V voltage reference
#define VOLT_OFFSET \
    0xBC  //2mv(25°C max) * GAIN32 = 64mV under 1.4V voltage reference \
          //For GAIN32 and GAIN16, it will use the same voltage output offset
#define MAX_OPA_OUTPUT_CODE (ADC_FULL_RANGE - VOLT_SWING - VOLT_OFFSET)
#define MIN_OPA_OUTPUT_CODE \
    (VOLT_SWING +           \
        VOLT_OFFSET)  //Reduce the influence of VLOT_SWING and ADC offset error
#define LOW_TO_HIGH_RANGE_THD 0x555  // 1/3 ADC range

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
    SYSCFG_DL_init();
    Driver_UartInit();
    Driver_TimerInit();
    delay_cycles(POWER_STARTUP_DELAY);
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
// Detect NTC temperature
//*****************************************************************************
int8_t BSP_TempSensorDetect(void)
{
    uint32_t u32Rntc_mR;
    _iq5 iq5TcellCode;
    int16_t i16Counter;

    //    DL_GPIO_togglePins(GPIOA,DL_GPIO_PIN_9);
    iq5TcellCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_5, DL_ADC12_REFERENCE_VOLTAGE_VDDA, eFalse);
    //    DL_GPIO_togglePins(GPIOA,DL_GPIO_PIN_9);
    u32Rntc_mR = _IQ5int(iq5TcellCode) * NTC_PAIR_RESISTOR_VALUE /
                 (ADC_FULL_RANGE - _IQ5int(iq5TcellCode));
    for (i16Counter = 0; i16Counter < NTC_TABLE_LENGTH;
         i16Counter++)  //rounding the Tcell
    {
        if (u32Rntc_mR > tempNtcTable[i16Counter]) {
            if ((tempNtcTable[i16Counter - 1] - u32Rntc_mR) <
                (u32Rntc_mR - tempNtcTable[i16Counter])) {
                return (i16Counter - 1 + NTC_SHIFT);
                break;
            } else {
                return (i16Counter + NTC_SHIFT);
                break;
            }
        }
    }
    return 0;
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
uint32_t u32Count;
void BSP_BattInfoMeasure(
    tBattInfoInput *pBattInfoInput, tCurtCalcFactors *pCurtCalcFactors)
{
    _iq5 iq5AdcInCode;
    _iq5 iq5VcellCode;
    _iq5 iq5Vo2ToVo1;
    _iq15 iq15Vshunt_mV;
    _iq15 iq15ResFactor;

#if (DETECTION_MODE == DETECTION_DATA_INPUT)
    Driver_DetectionPowerEnable();
    Driver_VrefInit();
    Driver_OPA0Init();
    //Set the current detection circuit according to last range judgement
    if (pCurtCalcFactors->opaGainSet == eGAIN32) {
        Driver_DACInit(pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain32);
        Driver_OPA1Init(DL_OPA_GAIN_N31_P32, eNormalMode);
    } else {
        Driver_DACInit(pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain16);
        Driver_OPA1Init(DL_OPA_GAIN_N15_P16, eNormalMode);
    }

    //Tcell detection
    pBattInfoInput->i8Tcell_C = BSP_TempSensorDetect();

    //Vcell detection
    iq5VcellCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_1, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eFalse);
    pBattInfoInput->iq15Vcell_mV =
        _IQ15mpy(BSP_IQ15AdcCodeToVoltage(_IQ5toIQ15(iq5VcellCode)),
            _IQ15(VCELL_FACTOR));

    //Icell detection: i16Icell_mA = (Vo2-Vo1)/Rs
    iq5AdcInCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_13, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eTrue);

    //Switch the current detection mode according to the current range
    if (pCurtCalcFactors->opaGainSet == eGAIN32) {
        iq5Vo2ToVo1 =
            iq5AdcInCode - pCurtCalcFactors->iq5VadcinOffsetCode_Gain32;
        iq15ResFactor = pCurtCalcFactors->iq15ResFactor_Gain32;
        iq15Vshunt_mV = BSP_IQ15AdcCodeToVoltage(_IQ5toIQ15(iq5Vo2ToVo1));
        pBattInfoInput->i16Icell_mA =
            0 - _IQ15int(_IQ15mpy(iq15ResFactor, iq15Vshunt_mV));

        //When the detection result exceed the OPA range, change it to higher gain
        if (iq5AdcInCode > MAX_OPA_OUTPUT_CODE ||
            iq5AdcInCode < MIN_OPA_OUTPUT_CODE) {
            pCurtCalcFactors->opaGainSet = eGAIN16;
            BSP_BattInfoMeasure(pBattInfoInput,
                pCurtCalcFactors);  //Redo the battery information measurement when the current suddenly drop or increase
        }
    } else {
        iq5Vo2ToVo1 =
            iq5AdcInCode - pCurtCalcFactors->iq5VadcinOffsetCode_Gain16;
        iq15ResFactor = pCurtCalcFactors->iq15ResFactor_Gain16;
        iq15Vshunt_mV = BSP_IQ15AdcCodeToVoltage(_IQ5toIQ15(iq5Vo2ToVo1));
        pBattInfoInput->i16Icell_mA =
            0 - _IQ15int(_IQ15mpy(iq15ResFactor, iq15Vshunt_mV));

        //When the voltage goes to a small range
        if (iq5AdcInCode < (ADC_FULL_RANGE - LOW_TO_HIGH_RANGE_THD) &&
            iq5AdcInCode > LOW_TO_HIGH_RANGE_THD) {
            pCurtCalcFactors->opaGainSet = eGAIN32;
            BSP_BattInfoMeasure(pBattInfoInput,
                pCurtCalcFactors);  //Redo the battery information measurement when the current suddenly drop or increase
        }
    }
    Driver_DetectionPowerDisable();
#endif
}

//*****************************************************************************
// Set the DAC value according to the voltage
//*****************************************************************************
void BSP_VccDetection(tCurtCalcFactors *pCurtCalcFactors)
{
    _iq5 iq5AdcInCode;
    _iq5 iq5Vcc;

    iq5AdcInCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_15, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eFalse);

    if (iq5AdcInCode < _IQ5(VCC_2V_ADC_VALUE)) {
        pCurtCalcFactors->dacSetValue.u8R12toR2FactorCaliValue =
            196;  //1.4V/VCC(1.8)*255-2= 196 (Minus 2 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain32 =
            47;  //(1.4V)/32*8/1.8*255 = 47 (Minus 2 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain16 =
            97;  //(1.4V)/16*8/1.8*255 = 97 (Minus 2 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain32 =
            23;  //0.7V/VCC(1.8)/32*8*255-1 = 24 (Minus 1 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain16 =
            48;  //0.7V/VCC(1.8)/16*8*255-1 = 49 (Minus 1 to avoid it get full)
        iq5Vcc = _IQ5(1800);

    } else if (iq5AdcInCode > _IQ5(VCC_3V_ADC_VALUE)) {
        pCurtCalcFactors->dacSetValue.u8R12toR2FactorCaliValue =
            106;  //1.4V/VCC(3.3)*255-2= 106 (Minus 2 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain32 =
            25;  //(1.4V)/32*8/3.3*255 = 25 (Minus 2 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain16 =
            52;  //(1.4V)/16*8/3.3*255 = 52 (Minus 2 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain32 =
            13;  //0.7V/VCC(3.3)/32*8*255-1 = 13 (Minus 1 to avoid it get full)
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain16 =
            26;  //0.7V/VCC(3.3)/16*8*255-1 = 26 (Minus 1 to avoid it get full)
        iq5Vcc = _IQ5(3300);
    } else {
        pCurtCalcFactors->dacSetValue.u8R12toR2FactorCaliValue        = 0;
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain32 = 0;
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain16 = 0;
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain32           = 0;
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain16           = 0;
        iq5Vcc = _IQ5(0);
    }
}

//*****************************************************************************
// Detect MCU temperature
//*****************************************************************************
int16_t BSP_CalcuMcuTemp(void)
{
    _iq15 iq15VSample;
    _iq15 iq15VTrim;
    _iq15 iq15AdcTemp;
    int16_t i16Temp;
    uint32_t *pU32TempTrim;

    DL_ADC12_reset(ADC12_INST);
    DL_VREF_reset(VREF);
    DL_ADC12_enablePower(ADC12_INST);
    DL_VREF_enablePower(VREF);
    delay_cycles(POWER_STARTUP_DELAY);

    return BSP_TempSensorDetect();
}

void BSP_CurtCalcFactorsCali(tCurtCalcFactors *pCurtCalcFactors)
{
    _iq5 iq5VrefCode;
    _iq5 iq5Vopa1pCode;
    _iq5 iq5VrsCode0, iq5VrsCode1;

    uint8_t temp2, i;
    _iq5 iq5AdcInCode;
    _iq15 abc, iq15Sum;
    Driver_DetectionPowerEnable();  //Ensure init OPAs before using
    Driver_VrefInit();

    //Choose the DAC setting according to the voltage supply
    BSP_VccDetection(pCurtCalcFactors);

    //Calculate R1+R2 to R2 factor and R1 to R1+R2 factor
    Driver_OPA0Init();
    Driver_DACInit(pCurtCalcFactors->dacSetValue.u8R12toR2FactorCaliValue);
    delay_cycles(32000);  //Wait Voltage to setup

    iq5Vopa1pCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_7, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eTrue);
    iq5VrefCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_12, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eTrue);
    pCurtCalcFactors->iq15R12toR2Factor = _IQ15div(
        _IQ5toIQ15(iq5VrefCode), _IQ5toIQ15(iq5VrefCode - iq5Vopa1pCode));
    pCurtCalcFactors->iq15R12toR1Factor =
        _IQ15div(_IQ5toIQ15(iq5VrefCode), _IQ5toIQ15(iq5Vopa1pCode));
#ifdef DEBUG
    Driver_Uart1TxU16Data(_IQ5int(iq5VrefCode));
    Driver_Uart1TxU16Data(
        _IQ15int(_IQ15mpy(pCurtCalcFactors->iq15R12toR1Factor, _IQ15(100))));
#endif

    //Get R3 to R4+R3 factor,resistor factor and Vo1 for GAIN32
    BSP_DiffGainFactorCali(DL_OPA_GAIN_N31_P32,
        &pCurtCalcFactors->iq15R34toR3Factor_Gain32,
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain32,
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain32, pCurtCalcFactors);

    //Get R3 to R4+R3 factor,resistor factor and Vo1 for GAIN16
    BSP_DiffGainFactorCali(DL_OPA_GAIN_N15_P16,
        &pCurtCalcFactors->iq15R34toR3Factor_Gain16,
        pCurtCalcFactors->dacSetValue.u8R34toR3FactorCaliValue_Gain16,
        pCurtCalcFactors->dacSetValue.u8DefaultValue_Gain16, pCurtCalcFactors);

    //Get current MCU temperature
    pCurtCalcFactors->i16McuTemp = BSP_CalcuMcuTemp();

    Driver_DetectionPowerDisable();

    //Set the default gain to be GAIN32
    pCurtCalcFactors->opaGainSet = eGAIN32;
}

//*****************************************************************************
// Calibrate the current parameter setting for different gain
//*****************************************************************************
void BSP_DiffGainFactorCali(DL_OPA_GAIN opaGain, _iq15 *pIq15R34toR3FactorTemp,
    uint8_t u8R34toR3CaliDacValue, uint8_t u8DefaultDacValue,
    tCurtCalcFactors *pCurtCalcFactors)
{
    _iq5 iq5VrefCode;
    _iq5 iq5AdcInCode;
    _iq15 iq15Vopa1pCode;
    _iq15 iq5Vopa1npCode;
    _iq5 *pIq5VadcinOffsetCode;  // ADC in code with current equals to 0mA
    _iq15 *pIq15R34toR3Factor;   // 1/GAIN
    _iq5 *pIq5OpaOffset;
    _iq5 iq5evaluatedResult;
    _iq15 *pIq15ResFactor;
    int16_t temp;

    pIq15R34toR3Factor   = pIq15R34toR3FactorTemp;
    pIq5VadcinOffsetCode = (_iq5 *) pIq15R34toR3FactorTemp + 1;
    pIq5OpaOffset        = (_iq5 *) pIq15R34toR3FactorTemp + 2;
    pIq15ResFactor       = pIq15R34toR3FactorTemp + 3;

    //Calibrate OPA offset
    Driver_OPA1Init(opaGain, eOffsetCali);
    Driver_OPA0Init();
    Driver_DACInit(46);   // Give a bias voltage for offset calibration
    delay_cycles(32000);  //Wait Voltage to setup

    iq5AdcInCode = Driver_ADCInitAndSampling(DL_ADC12_INPUT_CHAN_13,
        DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        eTrue);  //Used to judge the sampling delay time
#ifdef DEBUG
    //Send Offset calibration OPA output
    Driver_Uart1TxU16Data(_IQ5int(iq5AdcInCode));
#endif
    iq5Vopa1npCode = Driver_ADCInitAndSampling(DL_ADC12_INPUT_CHAN_12,
        DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        eTrue);  //Used to judge the sampling delay time
    *pIq5OpaOffset = iq5AdcInCode - iq5Vopa1npCode;

    //Calibrate R3+R4 to R3 factor
    Driver_OPA1Init(opaGain, eNormalMode);
    Driver_OPA0Init();
    temp         = 0;
    iq5AdcInCode = _IQ5(0xFFF);
    //As the max of voltage output swing from rail range is 60mv (0x88 with 1.8V supply, 12 bit ADC)
    //opa max output code should be 0xFFF-0x88= 0xF70
    while (
        iq5AdcInCode >
        _IQ5(
            MAX_OPA_OUTPUT_CODE))  //Avoid OPA out over range, because of the offset change across different temperature
    {
        Driver_DACInit(u8R34toR3CaliDacValue - temp);
        delay_cycles(32000);  //Wait Voltage to setup
        iq5AdcInCode = Driver_ADCInitAndSampling(DL_ADC12_INPUT_CHAN_13,
            DL_ADC12_REFERENCE_VOLTAGE_INTREF,
            eTrue);  //Used to judge the sampling delay time
        temp++;
    }
    iq5VrefCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_12, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eTrue);
    iq15Vopa1pCode =
        _IQ15div(_IQ5toIQ15(iq5VrefCode), pCurtCalcFactors->iq15R12toR1Factor);
    *pIq15R34toR3Factor =
        _IQ15div(_IQ5toIQ15(iq5AdcInCode - *pIq5OpaOffset), iq15Vopa1pCode);
#ifdef DEBUG
    //Send R34toR3 calibration output
    Driver_Uart1TxU16Data(_IQ5int(iq5AdcInCode));
    //Send R34toR3Factor + SendOPA offset
    Driver_Uart1TxU16Data(_IQ15int(_IQ15mpy(*pIq15R34toR3Factor, _IQ15(100))));
    temp = _IQ5int(*pIq5OpaOffset);
    if (temp < 0)
        Driver_Uart1TxU16Data(0x8000 - temp);
    else
        Driver_Uart1TxU16Data(temp);
#endif
    //Calibrate resistor factor
    *pIq15ResFactor = _IQ15div(_IQ15div(pCurtCalcFactors->iq15R12toR2Factor,
                                   _IQ15(SHUNT_RESISTOR_VALUE_R)),
        *pIq15R34toR3Factor);

    //Calibrate Vo1
    Driver_DACInit(u8DefaultDacValue);
    delay_cycles(32000);  //Wait Voltage to setup
    iq5VrefCode = Driver_ADCInitAndSampling(
        DL_ADC12_INPUT_CHAN_12, DL_ADC12_REFERENCE_VOLTAGE_INTREF, eTrue);
    *pIq5VadcinOffsetCode = Driver_ADCInitAndSampling(DL_ADC12_INPUT_CHAN_13,
        DL_ADC12_REFERENCE_VOLTAGE_INTREF,
        eTrue);  //Used to judge the sampling delay time
#ifdef DEBUG
    iq5AdcInCode =
        _IQ15toIQ5(_IQ15mpy(_IQ15div(_IQ5toIQ15(iq5VrefCode),
                                pCurtCalcFactors->iq15R12toR1Factor),
            *pIq15R34toR3Factor)) +
        *pIq5OpaOffset;
    //Send tested OPA bias voltage
    Driver_Uart1TxU16Data(_IQ5int(*pIq5VadcinOffsetCode));
    //Send calculated OPA bias voltage
    Driver_Uart1TxU16Data(_IQ5int(iq5AdcInCode));
#endif
}
