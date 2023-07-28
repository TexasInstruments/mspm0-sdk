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
#include "hal.h"

/**
 * @brief Array for storing the SPI instances
 */
HAL_SPI halSpi[HAL_SPI_CHANNEL_MAX];

/**
 * @brief Array for storing chip selects for spi peripheral
 */
DL_SPI_CHIP_SELECT  halSpiCS[HAL_SPI_CS_MAX];

/**
 * @brief Array for storing the ADC instances
 */
HAL_ADC halAdc[HAL_ADC_CHANNEL_MAX];

/**
 * @brief Array for storing the PWM instances
 */
HAL_PWM halPwm[HAL_PWM_CHANNEL_MAX];

/**
 * @brief Array for storing the PWM instances
 */
HAL_PWM halPwmFault[HAL_PWM_FAULT_MAX];

/**
 * @brief Array for storing the input GPIO pin instances
 */
HAL_GPIO halGpioIN[HAL_GPIO_IN_PIN_MAX];

/**
 * @brief Array for storing the ouput GPIO pin instances
 */
HAL_GPIO halGpioOUT[HAL_GPIO_OUT_PIN_MAX];

/**
 * @brief Scale factor for converting ADC data to voltage
 */
static _iq15 adcSf;

void HAL_init(void)
{
    /* Set the SPI array */
    halSpi[HAL_SPI_CHANNEL_0].inst             = (SPI_Regs *)SPI1_BASE;
    
    halSpiCS[HAL_SPI_CS_0] = DL_SPI_CHIP_SELECT_0;
    halSpiCS[HAL_SPI_CS_1] = DL_SPI_CHIP_SELECT_1;
    halSpiCS[HAL_SPI_CS_2] = DL_SPI_CHIP_SELECT_2;
    halSpiCS[HAL_SPI_CS_3] = DL_SPI_CHIP_SELECT_3;

    /* Set the ADC array */
    for(int x = 0;x < HAL_ADC_CHANNEL_MAX ;x++)
    {
        halAdc[x].refVoltage = 0;
        halAdc[x].value = 0;
    }
#ifdef GENERIC_ADC_0_INST
#ifdef GENERIC_ADC_0_ADCMEM_0
    halAdc[HAL_ADC_CHANNEL_0].inst          = (ADC12_Regs *)GENERIC_ADC_0_INST;
    halAdc[HAL_ADC_CHANNEL_0].irqn          = GENERIC_ADC_0_INST_INT_IRQN;
    halAdc[HAL_ADC_CHANNEL_0].memIDX        = GENERIC_ADC_0_ADCMEM_0;
    halAdc[HAL_ADC_CHANNEL_0].endOfSeq      = DL_ADC12_IIDX_MEM1_RESULT_LOADED;
    HAL_setADCRefVoltage(HAL_ADC_CHANNEL_0, _IQ15(HAL_ADC_DEFAULT_REF_VOLTAGE));
#endif
#ifdef GENERIC_ADC_0_ADCMEM_1
    halAdc[HAL_ADC_CHANNEL_1].inst          = (ADC12_Regs *)GENERIC_ADC_0_INST;
    halAdc[HAL_ADC_CHANNEL_1].irqn          = GENERIC_ADC_0_INST_INT_IRQN;
    halAdc[HAL_ADC_CHANNEL_1].memIDX        = GENERIC_ADC_0_ADCMEM_1;
    halAdc[HAL_ADC_CHANNEL_1].endOfSeq      = DL_ADC12_IIDX_MEM1_RESULT_LOADED;
    HAL_setADCRefVoltage(HAL_ADC_CHANNEL_1, _IQ15(HAL_ADC_DEFAULT_REF_VOLTAGE));
#endif
#endif

#ifdef GENERIC_ADC_1_INST
#ifdef GENERIC_ADC_1_ADCMEM_0
    halAdc[HAL_ADC_CHANNEL_2].inst          = (ADC12_Regs *)GENERIC_ADC_1_INST;
    halAdc[HAL_ADC_CHANNEL_2].irqn          = GENERIC_ADC_1_INST_INT_IRQN;
    halAdc[HAL_ADC_CHANNEL_2].memIDX        = GENERIC_ADC_1_ADCMEM_0;
    halAdc[HAL_ADC_CHANNEL_2].endOfSeq      = DL_ADC12_IIDX_MEM0_RESULT_LOADED;
    HAL_setADCRefVoltage(HAL_ADC_CHANNEL_2, _IQ15(HAL_ADC_DEFAULT_REF_VOLTAGE));
#endif
    /* Set the ADC scaling factor*/
    HAL_setADCSf();
#endif

    /* Set the PWM array */
#ifdef GENERIC_PWM_0_INST
    halPwm[HAL_PWM_CHANNEL_0].inst         = GENERIC_PWM_0_INST;
    halPwm[HAL_PWM_CHANNEL_0].irqn         = GENERIC_PWM_0_INST_INT_IRQN;
    halPwm[HAL_PWM_CHANNEL_0].CCIndex      = GPIO_GENERIC_PWM_0_C1_IDX;
#endif

#ifdef GENERIC_PWM_0_INST
    halPwm[HAL_PWM_CHANNEL_1].inst         = GENERIC_PWM_0_INST;
    halPwm[HAL_PWM_CHANNEL_1].irqn         = GENERIC_PWM_0_INST_INT_IRQN;
    halPwm[HAL_PWM_CHANNEL_1].CCIndex      = GPIO_GENERIC_PWM_0_C2_IDX;
#endif

#ifdef GENERIC_PWM_0_INST
    halPwm[HAL_PWM_CHANNEL_2].inst         = GENERIC_PWM_0_INST;
    halPwm[HAL_PWM_CHANNEL_2].irqn         = GENERIC_PWM_0_INST_INT_IRQN;
    halPwm[HAL_PWM_CHANNEL_2].CCIndex      = GPIO_GENERIC_PWM_0_C3_IDX;
#endif

    /* Set the PWM fault array */
#ifdef GENERIC_PWM_0_INST
    halPwmFault[HAL_PWM_FAULT_0].inst         = GENERIC_PWM_0_INST;
#endif

    /* Set the GPIO input array */
#ifdef GENERIC_GPIO_IN_PINI_0_PIN
    halGpioIN[HAL_GPIO_IN_PIN_0].iomux     = GENERIC_GPIO_IN_PINI_0_IOMUX;
    halGpioIN[HAL_GPIO_IN_PIN_0].port      = GENERIC_GPIO_IN_PORT;
    halGpioIN[HAL_GPIO_IN_PIN_0].pin       = GENERIC_GPIO_IN_PINI_0_PIN;
#endif

    /* Set the GPIO output array */

#ifdef GENERIC_GPIO_OUT_PINO_0_PIN
    halGpioOUT[HAL_GPIO_OUT_PIN_0].iomux   = GENERIC_GPIO_OUT_PINO_0_IOMUX;
    halGpioOUT[HAL_GPIO_OUT_PIN_0].port    = GENERIC_GPIO_OUT_PORT;
    halGpioOUT[HAL_GPIO_OUT_PIN_0].pin     = GENERIC_GPIO_OUT_PINO_0_PIN;
#endif
}

void HAL_setADCRefVoltage(HAL_ADC_CHANNEL adcChan, _iq15 value)
{
    halAdc[adcChan].refVoltage = value;
}

_iq15 HAL_getADCRefVoltage(HAL_ADC_CHANNEL adcChan)
{
    return halAdc[adcChan].refVoltage;
}

void HAL_setADCSf(void)
{
    adcSf = _IQ15div(_IQ15(1), _IQ15(HAL_ADC_MAX_VALUE));
}

void HAL_checkAndEnableIRQ(IRQn_Type irqn)
{
    if(!NVIC_GetEnableIRQ(irqn))
    {
        NVIC_EnableIRQ(irqn);
    }
}

void HAL_enableADCInterrupt(HAL_ADC_CHANNEL adcChan)
{
    HAL_checkAndEnableIRQ(halAdc[adcChan].irqn);
}

void HAL_disableADCInterrupt(HAL_ADC_CHANNEL adcChan)
{
    NVIC_DisableIRQ(halAdc[adcChan].irqn);
}

void HAL_enablePWMInterrupt(HAL_PWM_CHANNEL pwmChan)
{
    HAL_checkAndEnableIRQ(halPwm[pwmChan].irqn);
}

void HAL_disablePWMInterrupts(HAL_PWM_CHANNEL pwmChan)
{
    NVIC_DisableIRQ(halPwm[pwmChan].irqn);
}

void HAL_setGPIOVal(HAL_GPIO_OUT_PIN gpioPin, HAL_GPIO_VALUE value)
{
   if(value == HAL_GPIO_VALUE_HIGH)
   {
       DL_GPIO_setPins(halGpioOUT[gpioPin].port, halGpioOUT[gpioPin].pin);
   }
   else
   {
       DL_GPIO_clearPins(halGpioOUT[gpioPin].port, halGpioOUT[gpioPin].pin);
   }
}

HAL_GPIO_VALUE HAL_readGPIOVal(HAL_GPIO_IN_PIN gpioPin)
{
    return DL_GPIO_readPins(halGpioIN[gpioPin].port,
                            halGpioIN[gpioPin].pin)?
                            HAL_GPIO_VALUE_HIGH : HAL_GPIO_VALUE_LOW;
}

uint32_t HAL_getPWMLoadValue(HAL_PWM_CHANNEL pwmChan)
{
    uint32_t loadVal = DL_TimerA_getLoadValue(halPwm[pwmChan].inst);
    return  loadVal;
}

uint16_t HAL_setDeadband(HAL_PWM_CHANNEL pwmChan, uint16_t deadband)
{   
    /* Convert deadband in nanoseconds to pwm timer clocks */
    deadband = deadband * HAL_TIM_PERIPHERAL_CLOCK_SOURCE_MHZ/1000;
    DL_TimerA_setDeadBand(
        halPwm[pwmChan].inst, deadband, deadband, DL_TIMER_DEAD_BAND_MODE_1);
    return deadband;
}

void HAL_setPWMFreq(HAL_PWM_CHANNEL pwmChan, uint32_t freq)
{
    HAL_setFreq(halPwm[pwmChan].inst, freq);
}

void HAL_setFreq(GPTIMER_Regs * timerInst, uint32_t freq)
{
    DL_Timer_ClockConfig clockConfig;
    DL_Timer_getClockConfig(timerInst, &clockConfig);
    uint32_t loadValue = HAL_TIM_PERIPHERAL_CLOCK_SOURCE_MHZ*1000000/
                                         ((uint8_t)clockConfig.divideRatio + 1);
    loadValue /= freq;
    /* adjusting for center aligned mode */
    loadValue >>= 1;
    uint32_t psc = loadValue/65535;
    if(psc>256)
    {
        psc = 255;
    }
    loadValue /= (psc+1);
    clockConfig.prescale = (uint8_t)psc;
    DL_Timer_setClockConfig(timerInst, &clockConfig);
    HAL_setLoadVal(timerInst, loadValue);
}

void HAL_setLoadVal(GPTIMER_Regs * timerInst, uint32_t loadValue)
{
    /*
    * The clock is disable before changing the load value for safety
    */
    DL_Timer_disableClock(timerInst);
    DL_Timer_setLoadValue(timerInst, loadValue);
    DL_Timer_enableClock(timerInst);
}

_iq15 HAL_getADCVoltage(HAL_ADC_CHANNEL adcChan)
{
    _iq15 ret = _IQ15mpy(_IQ15mpy(_IQ15(halAdc[adcChan].value),
                                            halAdc[adcChan].refVoltage), adcSf);
    return ret;
}

void HAL_delayMilliSeconds(uint32_t num_ms)
{
    if(num_ms == 0)
    {
        return;
    }
    DL_Common_delayCycles(num_ms*HAL_CYCLES_TO_1_MS_DELAY);
}

bool HAL_isSPIRxFifoEmpty(HAL_SPI_CHANNEL spiPeripheral)
{
    return DL_SPI_isRXFIFOEmpty(halSpi[spiPeripheral].inst);
}

uint16_t HAL_receiveSPIData16(HAL_SPI_CHANNEL spiPeripheral, HAL_SPI_CS cs)
{
    DL_SPI_setChipSelect(halSpi[spiPeripheral].inst, halSpiCS[cs]);
    return DL_SPI_receiveData16(halSpi[spiPeripheral].inst);
}

void HAL_transmitSPIData16(
                    HAL_SPI_CHANNEL spiPeripheral, HAL_SPI_CS cs, uint16_t data)
{
    DL_SPI_setChipSelect(halSpi[spiPeripheral].inst, halSpiCS[cs]);
    DL_SPI_transmitData16(halSpi[spiPeripheral].inst, data);
}

bool HAL_isSPIBusy(HAL_SPI_CHANNEL spiPeripheral)
{
    return DL_SPI_isBusy(halSpi[spiPeripheral].inst);
}

void HAL_PWMEnableChannel(HAL_PWM_CHANNEL pwmChan)
{
    DL_Timer_overrideCCPOut(halPwm[pwmChan].inst, DL_TIMER_FORCE_OUT_DISABLED,
                     DL_TIMER_FORCE_CMPL_OUT_DISABLED, halPwm[pwmChan].CCIndex);
}

void HAL_PWMDisableChannel(HAL_PWM_CHANNEL pwmChan)
{
    DL_Timer_overrideCCPOut(halPwm[pwmChan].inst, DL_TIMER_FORCE_OUT_LOW,
                          DL_TIMER_FORCE_CMPL_OUT_LOW, halPwm[pwmChan].CCIndex);
}

void HAL_clearPWMFault(HAL_PWM_FAULT pwmFault)
{
    GPTIMER_Regs *gptimer = halPwmFault[pwmFault].inst;
    DL_Timer_clearInterruptStatus(gptimer, GPTIMER_CPU_INT_IMASK_F_MASK);
}

bool HAL_getPWMFaultStatus(HAL_PWM_FAULT pwmFault)
{
    GPTIMER_Regs *gptimer = halPwmFault[pwmFault].inst;
    uint32_t status = DL_Timer_getRawInterruptStatus(gptimer,
                                                  GPTIMER_CPU_INT_IMASK_F_MASK);
    return ((bool)(status >> GPTIMER_CPU_INT_IMASK_F_OFS));
}
