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

#include "Driver/MSPM0Lxx_Driver.h"

//*****************************************************************************
// Detection module power init
//*****************************************************************************
void Driver_DetectionPowerEnable(void)
{
    DL_ADC12_enablePower(ADC12_INST);
    DL_VREF_enablePower(VREF);
    delay_cycles(POWER_STARTUP_DELAY);
}
void Driver_DetectionPowerDisable(void)
{
    DL_ADC12_disablePower(ADC12_INST);
    DL_VREF_disablePower(VREF);
    DL_ADC12_reset(ADC12_INST);
    DL_VREF_reset(VREF);
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
    .destWidth     = DL_DMA_WIDTH_WORD,
    .srcWidth      = DL_DMA_WIDTH_WORD,
    .trigger       = DMA_ADC0_EVT_GEN_BD_TRIG};

/* ADC12_0 Initialization */
static const DL_ADC12_ClockConfig gADC12_0ClockConfig = {
    .clockSel = DL_ADC12_CLOCK_SYSOSC,
    .divideRatio =
        DL_ADC12_CLOCK_DIVIDE_8,  //Select suitable clock divide ratio
    .freqRange = DL_ADC12_CLOCK_FREQ_RANGE_24_TO_32,
};

_iq5 Driver_ADCInitAndSampling(
    uint32_t adcChannel, uint32_t vrefSource, _iq5 iq5AdcOffset)
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
        DL_ADC12_AVERAGING_MODE_DISABLED, DL_ADC12_BURN_OUT_SOURCE_DISABLED,
        DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
    DL_ADC12_enableFIFO(ADC12_INST);
    DL_ADC12_setPowerDownMode(ADC12_INST, DL_ADC12_POWER_DOWN_MODE_MANUAL);
    DL_ADC12_setSampleTime0(ADC12_INST, ADC_SAMPLE_TIME);
    DL_ADC12_enableDMA(ADC12_INST);
    DL_ADC12_setDMASamplesCnt(ADC12_INST, 1);
    DL_ADC12_enableDMATrigger(ADC12_INST,
        (DL_ADC12_DMA_MEM1_RESULT_LOADED | DL_ADC12_DMA_MEM3_RESULT_LOADED));
    /* Enable ADC12 interrupt */
    DL_ADC12_clearInterruptStatus(ADC12_INST, (DL_ADC12_INTERRUPT_DMA_DONE));
    DL_ADC12_enableInterrupt(ADC12_INST, (DL_ADC12_INTERRUPT_DMA_DONE));

    DL_DMA_initChannel(DMA, DMA_CH0_SEL, (DL_DMA_Config *) &gDMA_CH0Config);
    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(
        DMA, DMA_CH0_SEL, (uint32_t) DL_ADC12_getFIFOAddress(ADC12_INST));
    DL_DMA_setDestAddr(DMA, DMA_CH0_SEL, (uint32_t) &gADCSamples[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_SEL, ADC_AVERAGE_TIMES >> 1);

    DL_DMA_enableChannel(DMA, DMA_CH0_SEL);

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

    return (iq5AdcConvResult + iq5AdcOffset);
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
// GPIO INIT
//*****************************************************************************
void Driver_GpioInit(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_enablePower(GPIOA);
    delay_cycles(POWER_STARTUP_DELAY);

    DL_GPIO_initDigitalOutput(IOMUX_PINCM9);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM10);
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_8 | DL_GPIO_PIN_9);
    DL_GPIO_clearPins(GPIOA, DL_GPIO_PIN_8 | DL_GPIO_PIN_9);
}

//*****************************************************************************
// UART Initialization
//*****************************************************************************
static const DL_UART_Main_ClockConfig gUARTClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1};

static const DL_UART_Main_Config gUARTConfig = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE};

void Driver_Uart1Init(void)
{
    DL_UART_Main_reset(UART1);
    DL_UART_Main_enablePower(UART1);
    delay_cycles(POWER_STARTUP_DELAY);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART1_IOMUX_TX, GPIO_UART1_IOMUX_TX_FUNCTION);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART1_IOMUX_RX, GPIO_UART1_IOMUX_RX_FUNCTION);
    /* UART clock configuration to use LFCLK at 32kHz */
    DL_UART_Main_setClockConfig(
        UART1, (DL_UART_Main_ClockConfig *) &gUARTClockConfig);

    /* UART configuration to operate in normal mode */
    DL_UART_Main_init(UART1, (DL_UART_Main_Config *) &gUARTConfig);

    /*
       * Configure baud rate by setting oversampling and baud rate divisors.
       *  Target baud rate: 115200
       *  Actual baud rate: 115211.52
       */
    DL_UART_Main_setOversampling(UART1, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(
        UART1, UART_IBRD_32_MHZ_115200_BAUD, UART_FBRD_32_MHZ_115200_BAUD);

    DL_UART_Main_enable(UART1);
    NVIC_ClearPendingIRQ(UART1_INT_IRQn);
}

//*****************************************************************************
// UART Send data
//*****************************************************************************
void Driver_Uart1TxU16Data(uint16_t u16Data)
{
    while (DL_UART_isBusy(UART1))
        ;
    DL_UART_Main_transmitData(UART1, (u16Data & 0xFF00) >> 8);
    while (DL_UART_isBusy(UART1))
        ;
    DL_UART_Main_transmitData(UART1, u16Data & 0xFF);
    while (DL_UART_isBusy(UART1))
        ;
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
    DL_TimerG_reset(TIMER_INST);
    DL_TimerG_enablePower(TIMER_INST);
    delay_cycles(POWER_STARTUP_DELAY);

    //When period = 4096, it is one second
    gTIMER_INSTTimerConfig.period =
        4096 * battParamsCfg.sysTikFreq / SYSTEM_TIK_SHIFT;
    DL_TimerG_setClockConfig(
        TIMER_INST, (DL_TimerG_ClockConfig *) &gTIMER_INSTClockConfig);

    DL_TimerG_initTimerMode(
        TIMER_INST, (DL_TimerG_TimerConfig *) &gTIMER_INSTTimerConfig);
    DL_TimerG_enableInterrupt(TIMER_INST, DL_TIMER_INTERRUPT_ZERO_EVENT);
    NVIC_EnableIRQ(TIMER_INST_INT_IRQN);
    DL_TimerG_enableClock(TIMER_INST);
    DL_TimerG_startCounter(TIMER_INST);
}

void TIMG0_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMG0)) {
        case DL_TIMER_IIDX_ZERO:
            gaugeApp.sysTikFlag = 1;
            break;
        default:
            break;
    }
}
