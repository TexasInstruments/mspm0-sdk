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
/*!****************************************************************************
 *  @file       hal.h
 *  @brief      Sensorless FOC Motor Control Library HAL Module
 *  @defgroup   FOC__MSPM0G_HAL MSPM0G3507 - Hardware Abstraction Layer (HAL)
 *
 *
 *  @anchor foc_lp_mpsm0g3507_hal_Overview
 *  # Overview
 *
 *  The HAL module provides micro-controller agnostic set of application
 *  programming interfaces (APIs) to be used by other module's in the library
 *  as well as in the user's application code.
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup FOC__MSPM0G_HAL
 * @{
 */
#ifndef HAL_H
#define HAL_H

/* Including all the sysconfig generated defines */
#include "ti_msp_dl_config.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief MCU clock frequency
 */
#define HAL_MCU_CLK_SPEED_HZ                        (80000000)

/*!
 * @brief Cycles for 1 ms delay
 */
#define HAL_CYCLES_TO_1_MS_DELAY                    (HAL_MCU_CLK_SPEED_HZ/1000)

/*!
 * @brief Maximum value of data from the ADC
 */
#define HAL_ADC_MAX_VALUE                           (4095)

/*!
 * @brief Value of default ADC reference voltage in volts
 */
#define HAL_ADC_DEFAULT_REF_VOLTAGE                 (3.3f)

/*!
 * @brief Value of the source clock for the timer peripherals
 */
#define HAL_TIM_PERIPHERAL_CLOCK_SOURCE_MHZ         (80)

/*! @brief Define SPI instance */
typedef struct {
    /*! SPI inst address of the channel */
    SPI_Regs *inst;
} HAL_SPI;

/*! @brief Define ADC instance */
typedef struct {
    /*! ADC inst address of the channel */
    ADC12_Regs *inst;
    /*! Reference voltage of the channel */
    _iq15 refVoltage;
    /*! ADC converted data */
    uint16_t value;
    /*! IRQ number for the ADC channel */
    IRQn_Type irqn;
    /*! Memory index for the ADC channel */
    DL_ADC12_MEM_IDX memIDX;
    /*! End of Sequence for the ADC inst */
    DL_ADC12_IIDX endOfSeq;
} HAL_ADC;

/*! @brief Define PWM instance */
typedef struct {
    /*! PWM timer inst address of the channel */
    GPTIMER_Regs *inst;
    /*! IRQ number for the PWM channel */
    IRQn_Type irqn;
    /*! Capture compare index for the PWM channel */
    DL_TIMER_CC_INDEX CCIndex;
} HAL_PWM;

/*! @brief Define TIM instance */
typedef struct {
    /*! Timer inst address of the channel */
    GPTIMER_Regs *inst;
    /*! IRQ number for the Timer channel */
    IRQn_Type irqn;
} HAL_TIM;

/*! @brief Define GPIO instance */
typedef struct {
    /*! Pincm iomux */
    IOMUX_PINCM iomux;
    /*! Pin port */
    GPIO_Regs *port;
    /*! Pin */
    uint32_t pin;
} HAL_GPIO;

/*! @enum HAL_PWM_CHANNEL */
typedef enum {
    /*! Index associated to PWM channel 0 */
    HAL_PWM_CHANNEL_0 = 0,
    /*! Index associated to PWM channel 1 */
    HAL_PWM_CHANNEL_1,
    /*! Index associated to PWM channel 2 */
    HAL_PWM_CHANNEL_2,
    /*! Total number of PWM channels */
    HAL_PWM_CHANNEL_MAX,
} HAL_PWM_CHANNEL;

/*! @enum HAL_PWM_FAULT */
typedef enum {
    /*! Index associated to PWM fault input 0 */
    HAL_PWM_FAULT_0 = 0,
    /*! Total number of PWM Fault Inputs */
    HAL_PWM_FAULT_MAX,
} HAL_PWM_FAULT;

/*! @enum HAL_ADC_CHANNEL */
typedef enum {
    /*! Index associated to ADC channel 0 */
    HAL_ADC_CHANNEL_0 = 0,
    /*! Index associated to ADC channel 1 */
    HAL_ADC_CHANNEL_1,
    /*! Index associated to ADC channel 2 */
    HAL_ADC_CHANNEL_2,
    /*! Total number of ADC channels */
    HAL_ADC_CHANNEL_MAX
} HAL_ADC_CHANNEL;

/*! @enum HAL_GPIO_IN_PIN */
typedef enum {
    /*! Index associated to input GPIO PIN 0 */
    HAL_GPIO_IN_PIN_0 = 0,
    /*! Total number of input GPIO pins */
    HAL_GPIO_IN_PIN_MAX,
} HAL_GPIO_IN_PIN;

/*! @enum HAL_GPIO_OUT_PIN */
typedef enum {
    /*! Index associated to output GPIO PIN 0 */
    HAL_GPIO_OUT_PIN_0 = 0,
    /*! Total number of output GPIO pins */
    HAL_GPIO_OUT_PIN_MAX,
} HAL_GPIO_OUT_PIN;

/*! @enum HAL_SPI_CHANNEL */
typedef enum {
    /*! Index associated to SPI channel 0 */
    HAL_SPI_CHANNEL_0 = 0,
    /*! Total number of SPI channels */
    HAL_SPI_CHANNEL_MAX,
} HAL_SPI_CHANNEL;

/*! @enum HAL_SPI_CS */
typedef enum {
    /*! Index associated to chip select 0 */
    HAL_SPI_CS_0 = 0,
    /*! Index associated to chip select 1 */
    HAL_SPI_CS_1,
    /*! Index associated to chip select 2 */
    HAL_SPI_CS_2,
    /*! Index associated to chip select 3 */
    HAL_SPI_CS_3,
    /*! Total number of chip select pins */
    HAL_SPI_CS_MAX
} HAL_SPI_CS;

/*! @enum HAL_GPIO_VALUE */
typedef enum {
    /*! GPIO set to low */
    HAL_GPIO_VALUE_LOW = 0,
    /*! GPIO set to high */
    HAL_GPIO_VALUE_HIGH,
} HAL_GPIO_VALUE;

/* Extern the peripheral arrays */
extern HAL_PWM halPwm[HAL_PWM_CHANNEL_MAX];
extern HAL_SPI halSpi[HAL_SPI_CHANNEL_MAX];
extern DL_SPI_CHIP_SELECT  halSpiCS[HAL_SPI_CS_MAX];
extern HAL_ADC halAdc[HAL_ADC_CHANNEL_MAX];
extern HAL_GPIO halGpioIN[HAL_GPIO_IN_PIN_MAX];
extern HAL_GPIO halGpioOUT[HAL_GPIO_OUT_PIN_MAX];

/**
 * @brief     Initializes the hal module
 */
void HAL_init(void);

/**
 * @brief     Sets the ADC reference voltage
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 * @param[in] value     Reference voltage to be set in IQ
 */
void HAL_setADCRefVoltage(HAL_ADC_CHANNEL adcChan, _iq15 value);

/**
 * @brief     Gets the ADC reference voltage
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 * @return    Returns the value of the adc reference voltage in iq15
 */
_iq15 HAL_getADCRefVoltage(HAL_ADC_CHANNEL adcChan);

/**
 * @brief     Sets the ADC scale factor
 */
void HAL_setADCSf(void);

/**
 * @brief     If the IRQ is disabled it is enabled
 * @param[in] irqn   An IRQn.
 */
void HAL_checkAndEnableIRQ(IRQn_Type irqn);

/**
 * @brief     Enable ADC interrupt
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 */
void HAL_enableADCInterrupt(HAL_ADC_CHANNEL adcChan);

/**
 * @brief     Disable ADC interrupt
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 */
void HAL_disableADCInterrupt(HAL_ADC_CHANNEL adcChan);

/**
 * @brief     Enable PWM interrupt
 * @param[in] pwmChan   A PWM channel. One of @ref HAL_PWM_CHANNEL
 */
void HAL_enablePWMInterrupt(HAL_PWM_CHANNEL pwmChan);

/**
 * @brief     Disable PWM interrupt
 * @param[in] pwmChan   A PWM channel. One of @ref HAL_PWM_CHANNEL
 */
void HAL_disablePWMInterrupts(HAL_PWM_CHANNEL pwmChan);

/**
 * @brief     Set value to GPIO pin
 * @param[in] gpioPin   A GPIO pin. One of @ref HAL_GPIO_OUT_PIN
 * @param[in] value     Value to be set to pin. One of @ref HAL_GPIO_VALUE
 */
void HAL_setGPIOVal(HAL_GPIO_OUT_PIN gpioPin, HAL_GPIO_VALUE value);

/**
 * @brief     Read GPIO pin
 * @param[in] gpioPin   A GPIO pin. One of @ref HAL_GPIO_IN_PIN
 * @return    Returns the value of the GPIO pin. One of @ref HAL_GPIO_VALUE
 */
HAL_GPIO_VALUE HAL_readGPIOVal(HAL_GPIO_IN_PIN gpioPin);

/**
 * @brief     Gets the Load value of the timer associated with the PWM channel
 * @param[in] pwmChan   A PWM channel. One of @ref HAL_PWM_CHANNEL
 * @return    Returns the load register value of the timer instance of the 
 *            specified PWM channel
 */
uint32_t HAL_getPWMLoadValue(HAL_PWM_CHANNEL pwmChan);

/**
 * @brief     Set deadband to the PWM channel
 * @param[in] pwmChan  A PWM channel. One of @ref HAL_PWM_CHANNEL
 * @param[in] deadband Deadtime in nanoseconds
 * @return    Returns the deadtime in PWM cycles
 */
uint16_t HAL_setDeadband(HAL_PWM_CHANNEL pwmChan, uint16_t deadband);

/**
 * @brief     Set Compare to the PWM channel
 * @param[in] pwmChan  A PWM channel. One of @ref HAL_PWM_CHANNEL
 * @param[in] ccVal    Compare value to be set. One of @ref HAL_PWM_CHANNEL
 */
__STATIC_INLINE void HAL_setPWMCcValue(HAL_PWM_CHANNEL pwmChan, uint32_t ccVal)
{
  DL_TimerA_setCaptureCompareValue(
                          halPwm[pwmChan].inst, ccVal, halPwm[pwmChan].CCIndex);
}

/**
 * @brief     Set the frequency of the timer associated with the PWM channel
 * @param[in] pwmChan   A PWM channel. One of @ref HAL_PWM_CHANNEL
 * @param[in] freq      PWM frequency to be set.
 */
void HAL_setPWMFreq(HAL_PWM_CHANNEL pwmChan, uint32_t freq);

/**
 * @brief     Set the frequency for timer peripheral
 * @param[in] timerInst Pointer to a timer peripheral.
 * @param[in] freq      Frequency to be set to peripheral.
 */
void HAL_setFreq(GPTIMER_Regs * timerInst, uint32_t freq);

/**
 * @brief     Set the Load value of a timer peripheral
 * @param[in] timerInst Pointer to a timer peripheral.
 * @param[in] loadValue Load value to be set to the timer peripheral.
 */
void HAL_setLoadVal(GPTIMER_Regs * timerInst, uint32_t loadValue);

/**
 * @brief     Gets the ADC reading converted to voltage
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 * @return    Returns the ADC voltage reading in iq15
 */
_iq15 HAL_getADCVoltage(HAL_ADC_CHANNEL adcChan);

/**
 * @brief     Gets the ADC sampled value
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 * @return    Returns the ADC sampled value
 */
__STATIC_INLINE uint16_t HAL_getADCData(HAL_ADC_CHANNEL adcChan)
{
    return DL_ADC12_getMemResult(
                                  halAdc[adcChan].inst, halAdc[adcChan].memIDX);
}

/**
 * @brief     Gets the raw value read from adc
 * @param[in] adcChan   An ADC Channel. One of @ref HAL_ADC_CHANNEL
 * @return    Returns the raw value read from adc
 */
__STATIC_INLINE uint16_t HAL_getADCValue(HAL_ADC_CHANNEL adcChan)
{
    return halAdc[adcChan].value;
}

/**
 * @brief     Collects the ADC data if the adc conversion is over
 * @param[in] adcInst   Pointer to a ADC peripheral
 * @return DL_ADC12_IIDX returns interrupt information that caused the IRQ
 *         to fire.
 */
__STATIC_INLINE DL_ADC12_IIDX HAL_processADCIRQ(ADC12_Regs *adcInst)
{
    DL_ADC12_IIDX pendIrq = DL_ADC12_getPendingInterrupt(adcInst);
    for(int x = 0;x < HAL_ADC_CHANNEL_MAX;x++)
    {
        if(adcInst == halAdc[x].inst && pendIrq ==  halAdc[x].endOfSeq)
        {
            halAdc[x].value = (uint16_t)HAL_getADCData(x);
        }
    }
    return pendIrq;
}

/**
 * @brief     Blocks for the specified delay time
 * @param[in] num_ms   Delay in ms
 */
void HAL_delayMilliSeconds(uint32_t num_ms);

/**
 * @brief     Gets status of RX Fifo 
 * @param[in] spiPeripheral   A SPI peripheral. One of @ref HAL_SPI_CHANNEL
 * @return    Returns empty status of RX Fifo 
 * 
 *  @retval     true  The RX FIFO is empty
 *  @retval     false The RX FIFO is not empty
 */
bool HAL_isSPIRxFifoEmpty(HAL_SPI_CHANNEL spiPeripheral);

/**
 * @brief     Reads 16 bit data in SPI peripheral 
 * @param[in] spiPeripheral   A SPI peripheral. One of @ref HAL_SPI_CHANNEL
 * @param[in] cs     A SPI chip select. One of @ref HAL_SPI_CS
 * @return    Data received in SPI peripheral
 */
uint16_t HAL_receiveSPIData16(HAL_SPI_CHANNEL spiPeripheral, HAL_SPI_CS cs);

/**
 * @brief     Writes 16 bit data in SPI peripheral 
 * @param[in] spiPeripheral   A SPI peripheral. One of @ref HAL_SPI_CHANNEL
 * @param[in] cs     A SPI chip select. One of @ref HAL_SPI_CS
 * @param[in] data   Data to be written in to the SPI peripheral
 */
void HAL_transmitSPIData16(
                HAL_SPI_CHANNEL spiPeripheral, HAL_SPI_CS cs, uint16_t data);

/**
 * @brief     Gets busy status of SPI peripheral
 * @param[in] spiPeripheral   A SPI peripheral. One of @ref HAL_SPI_CHANNEL
 * @return    Returns busy status of RX Fifo 
 * 
 * @retval     true  The SPI peripheral is currently busy with transmitting data
 * @retval     false The SPI peripheral is in idle
 */
bool HAL_isSPIBusy(HAL_SPI_CHANNEL spiPeripheral);

/**
 * @brief     Clears the RX Fifo if the SPI peripheral 
 * @param[in] spiPeripheral   A SPI peripheral. One of @ref HAL_SPI_CHANNEL
 * @param[in] cs     A SPI chip select. One of @ref HAL_SPI_CS
 */
__STATIC_INLINE void HAL_SPI_clearRXFifo(
                                HAL_SPI_CHANNEL spiPeripheral, HAL_SPI_CS cs)
{
    while (!HAL_isSPIRxFifoEmpty(spiPeripheral))
    {
        HAL_receiveSPIData16(spiPeripheral, cs);
    }
}

/**
 * @brief     Enables the PWM channel.
 * @param[in] pwmChan   A PWM channel. One of @ref HAL_PWM_CHANNEL
 */
void HAL_PWMEnableChannel(HAL_PWM_CHANNEL pwmChan);

/**
 * @brief     Disables the PWM channel and overrides the channel
 * and complementary outputs to low
 * @param[in] pwmChan   A PWM channel. One of @ref HAL_PWM_CHANNEL
 */
void HAL_PWMDisableChannel(HAL_PWM_CHANNEL pwmChan);

/**
 * @brief Clears the PWM timer fault status
 * @param[in] pwmFault   A PWM fault input. One of @ref HAL_PWM_FAULT
 */
void HAL_clearPWMFault(HAL_PWM_FAULT pwmFault);

/**
 * @brief Gets the PWM timer fault status
 * @param[in] pwmFault   A PWM fault input. One of @ref HAL_PWM_FAULT
 * @return  Returns PWM timer fault status

 * @retval     true  Fault occurred
 * @retval     false No fault occurred
 */
bool HAL_getPWMFaultStatus(HAL_PWM_FAULT pwmFault);


#ifdef __cplusplus
}
#endif
#endif /* HAL_H */
/** @}*/
