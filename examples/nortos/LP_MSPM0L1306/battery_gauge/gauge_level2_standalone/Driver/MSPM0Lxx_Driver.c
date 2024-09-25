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

#include "Driver/MSPM0Lxx_Driver.h"

//*****************************************************************************
// Detection module power init
//*****************************************************************************
void Driver_DetectionPowerEnable(void)
{
    DL_ADC12_reset(ADC12_INST);
    DL_VREF_reset(VREF);
    DL_COMP_reset(COMP0);
    DL_OPA_reset(OPA0);
    DL_OPA_reset(OPA1);
    DL_ADC12_enablePower(ADC12_INST);
    DL_VREF_enablePower(VREF);
    DL_COMP_enablePower(COMP0);
    DL_OPA_enablePower(OPA0);
    DL_OPA_enablePower(OPA1);
    delay_cycles(POWER_STARTUP_DELAY);
}

//*****************************************************************************
// Detection module power disable
//*****************************************************************************
void Driver_DetectionPowerDisable(void)
{
    DL_ADC12_disablePower(ADC12_INST);
    DL_VREF_disablePower(VREF);
    DL_COMP_disablePower(COMP0);
    DL_OPA_disablePower(OPA0);
    DL_OPA_disablePower(OPA1);
    DL_ADC12_reset(ADC12_INST);
    DL_VREF_reset(VREF);
    DL_COMP_reset(COMP0);
    DL_OPA_reset(OPA0);
    DL_OPA_reset(OPA1);
}

//*****************************************************************************
// Vref Init
//*****************************************************************************
static const DL_VREF_Config gVREFConfig = {
    .bufConfig      = DL_VREF_BUFCONFIG_OUTPUT_1_4V,
    .holdCycleCount = DL_VREF_HOLD_MIN,
    .shCycleCount   = DL_VREF_SH_MIN,
    .shModeEnable   = DL_VREF_SHMODE_DISABLE,
    .vrefEnable     = DL_VREF_ENABLE_ENABLE,
};
static const DL_VREF_ClockConfig gVREFClockConfig = {
    .clockSel    = DL_VREF_CLOCK_BUSCLK,
    .divideRatio = DL_VREF_CLOCK_DIVIDE_1,
};

void Driver_VrefInit(void)
{
    DL_VREF_setClockConfig(VREF, (DL_VREF_ClockConfig *) &gVREFClockConfig);
    DL_VREF_configReference(VREF, (DL_VREF_Config *) &gVREFConfig);
    while (DL_VREF_CTL1_READY_NOTRDY == DL_VREF_getStatus(VREF))
        ;
}

//*****************************************************************************
// ADC Init and sampling
// Use software averaging to increase resolution
// ADC sampling total time:ADC 1 sampling time * ADC_AVERAGE_TIMES
//*****************************************************************************
volatile bool gCheckADC;
uint16_t gADCSamples[ADC_AVERAGE_TIMES];
static const DL_DMA_Config gDMA_CH0Config = {
    .transferMode  = DL_DMA_SINGLE_TRANSFER_MODE,
    .extendedMode  = DL_DMA_NORMAL_MODE,
    .destIncrement = DL_DMA_ADDR_INCREMENT,
    .srcIncrement  = DL_DMA_ADDR_UNCHANGED,
    .destWidth     = DL_DMA_WIDTH_HALF_WORD,
    .srcWidth      = DL_DMA_WIDTH_HALF_WORD,
    .trigger       = DMA_ADC0_EVT_GEN_BD_TRIG,
    .triggerType   = DMA_DMATCTL_DMATINT_EXTERNAL,
};

/* ADC12_0 Initialization */
static const DL_ADC12_ClockConfig gADC12_0ClockConfig = {
    .clockSel = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio =
        DL_ADC12_CLOCK_DIVIDE_8,  //Select suitable clock divide ratio
    .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

_iq5 Driver_ADCInitAndSampling(
    uint32_t adcChannel, uint32_t vrefSource, tStatus highAvg)
{
    _iq5 iq5AdcConvResult;
    uint32_t u32AdcSum = 0;
    uint16_t u16Counter;
    uint16_t u16AdcResult;

    gCheckADC = false;
    DL_ADC12_reset(ADC12_INST);
    DL_ADC12_enablePower(ADC12_INST);
    DL_ADC12_setClockConfig(
        ADC12_INST, (DL_ADC12_ClockConfig *) &gADC12_0ClockConfig);
    DL_ADC12_initSingleSample(ADC12_INST, DL_ADC12_REPEAT_MODE_ENABLED,
        DL_ADC12_SAMPLING_SOURCE_AUTO, DL_ADC12_TRIG_SRC_SOFTWARE,
        DL_ADC12_SAMP_CONV_RES_12_BIT,
        DL_ADC12_SAMP_CONV_DATA_FORMAT_UNSIGNED);
    DL_ADC12_configConversionMem(ADC12_INST, DL_ADC12_MEM_IDX_0, adcChannel,
        vrefSource, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
        DL_ADC12_AVERAGING_MODE_ENABLED, DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_setPowerDownMode(ADC12_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);
    if (highAvg == eTrue) {
        DL_ADC12_configHwAverage(ADC12_INST, DL_ADC12_HW_AVG_NUM_ACC_128,
            DL_ADC12_HW_AVG_DEN_DIV_BY_128);
    } else {
        DL_ADC12_configHwAverage(ADC12_INST, DL_ADC12_HW_AVG_NUM_ACC_4,
            DL_ADC12_HW_AVG_DEN_DIV_BY_4);
    }

    DL_ADC12_setSampleTime0(ADC12_INST, ADC_SAMPLE_TIME);
    DL_ADC12_enableDMA(ADC12_INST);
    DL_ADC12_setDMASamplesCnt(ADC12_INST, 1);
    DL_ADC12_enableDMATrigger(ADC12_INST, DL_ADC12_DMA_MEM0_RESULT_LOADED);
    /* Enable ADC12 interrupt */
    DL_ADC12_clearInterruptStatus(ADC12_INST, (DL_ADC12_INTERRUPT_DMA_DONE));
    DL_ADC12_enableInterrupt(ADC12_INST, (DL_ADC12_INTERRUPT_DMA_DONE));

    DL_DMA_initChannel(
        DMA, DMA_CH0_CHAN_ID, (DL_DMA_Config *) &gDMA_CH0Config);
    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID,
        (uint32_t) DL_ADC12_getMemResultAddress(
            ADC12_INST, DL_ADC12_MEM_IDX_0));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gADCSamples[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_AVERAGE_TIMES);

    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    /* Setup interrupts on device */
    NVIC_EnableIRQ(ADC12_INST_INT_IRQN);

    DL_ADC12_enableConversions(ADC12_INST);
    DL_ADC12_startConversion(ADC12_INST);

    while (false == gCheckADC) {
        __WFE();
    }

    for (u16Counter = 0; u16Counter < ADC_AVERAGE_TIMES; u16Counter++) {
        u32AdcSum += gADCSamples[u16Counter];
    }
    iq5AdcConvResult = _IQ5div(_IQ5(u32AdcSum), _IQ5(ADC_AVERAGE_TIMES));

    return iq5AdcConvResult;
}

void ADC12_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_INST)) {
        case DL_ADC12_IIDX_DMA_DONE:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
//*****************************************************************************
// 8bit DAC Init
//*****************************************************************************
static const DL_COMP_Config gCOMP_INSTConfig = {
    .channelEnable = DL_COMP_ENABLE_CHANNEL_NONE,
    .mode          = DL_COMP_MODE_FAST,
    .negChannel    = DL_COMP_IMSEL_CHANNEL_0,
    .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
    .hysteresis    = DL_COMP_HYSTERESIS_20,
    .polarity      = DL_COMP_POLARITY_NON_INV};

static const DL_COMP_RefVoltageConfig gCOMP_INSTVRefConfig = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = COMP_CTL2_REFSRC_VDDA_DAC,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_NEG,
    .controlSelect  = DL_COMP_DAC_CONTROL_SW,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0};

void Driver_DACInit(uint32_t u32DacValue)
{
    DL_COMP_init(COMP0, (DL_COMP_Config *) &gCOMP_INSTConfig);
    DL_COMP_refVoltageInit(
        COMP0, (DL_COMP_RefVoltageConfig *) &gCOMP_INSTVRefConfig);
    DL_COMP_setDACCode0(COMP0, u32DacValue);
    DL_COMP_enable(COMP0);
}

//*****************************************************************************
// OPA0 (used as a buffer) Init
//*****************************************************************************
static const DL_OPA_Config gOPA0Config0 = {
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .pselChannel    = DL_OPA_PSEL_DAC8_OUT,
    .nselChannel    = DL_OPA_NSEL_RTOP,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
};
void Driver_OPA0Init(void)
{
    DL_OPA_reset(OPA0);
    DL_OPA_enablePower(OPA0);

    DL_SYSCTL_enableMFCLK();

    DL_OPA_enableRailToRailInput(OPA0);  //In order to input DAC= VCC
    DL_OPA_setGainBandwidth(OPA0, DL_OPA_GBW_LOW);
    DL_OPA_init(OPA0, (DL_OPA_Config *) &gOPA0Config0);
    DL_OPA_enable(OPA0);
}

//*****************************************************************************
// OPA1 (used as a buffer) Init
//*****************************************************************************
DL_OPA_Config gOPA1Config0 = {
    //offset software calibration can remove the influence of DC offset. But it still can't
    //Remove the influence of AC offset. The chopper mode is suggested to enable.
    .choppingMode   = DL_OPA_CHOPPING_MODE_ADC_AVERAGING,
    .outputPinState = OA_CFG_OUTPIN_DISABLED,
    .nselChannel    = DL_OPA_NSEL_RTAP,
};
void Driver_OPA1Init(uint32_t u32Gain, tOPACali opaCali)
{
    //Need to reset the OPA1, in order to make the setting work
    DL_OPA_reset(OPA1);
    DL_OPA_enablePower(OPA1);

    DL_SYSCTL_enableMFCLK();

    gOPA1Config0.gain = u32Gain;

    if (opaCali == eOffsetCali) {
        gOPA1Config0.mselChannel = DL_OPA_MSEL_RTOP;
        gOPA1Config0.pselChannel = DL_OPA_PSEL_RTOP;

    } else if (opaCali == eNormalMode) {
        gOPA1Config0.mselChannel = DL_OPA_MSEL_IN1_NEG;
        gOPA1Config0.pselChannel = DL_OPA_PSEL_IN0_POS;
    }

    DL_OPA_enableRailToRailInput(OPA1);

    //To further reduce the power, GBW can be changed to low
    //From one chip test, the change of R34toR3 is smaller with GBW high
    DL_OPA_setGainBandwidth(OPA1, DL_OPA_GBW_HIGH);
    DL_OPA_init(OPA1, (DL_OPA_Config *) &gOPA1Config0);
    DL_OPA_enable(OPA1);
}

//*****************************************************************************
// UART control
//*****************************************************************************
void Driver_UartInit(void)
{
    DL_UART_Main_reset(UART_INST);
    DL_UART_Main_enablePower(UART_INST);
    delay_cycles(POWER_STARTUP_DELAY);
    SYSCFG_DL_UART_init();
}
void Driver_UartTxU16Data(uint16_t u16Data)
{
    while (DL_UART_isBusy(UART_INST))
        ;
    DL_UART_Main_transmitData(UART_INST, (u16Data & 0xFF00) >> 8);
    while (DL_UART_isBusy(UART_INST))
        ;
    DL_UART_Main_transmitData(UART_INST, u16Data & 0xFF);
    while (DL_UART_isBusy(UART_INST))
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
    while (DL_UART_getRawInterruptStatus(UART_INST, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART_INST, DL_UART_INTERRUPT_RX);
    u16RxData += (DL_UART_Main_receiveData(UART_INST) << 8);

    while (DL_UART_getRawInterruptStatus(UART_INST, DL_UART_INTERRUPT_RX) == 0)
        ;
    DL_UART_clearInterruptStatus(UART_INST, DL_UART_INTERRUPT_RX);
    u16RxData += DL_UART_Main_receiveData(UART_INST);

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
static const DL_TimerG_ClockConfig gTIMER_INSTClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_LFCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 0U};

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
