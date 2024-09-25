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

#ifndef DRIVER_MSPM0LXX_DRIVER_H_
#define DRIVER_MSPM0LXX_DRIVER_H_

#include <UserConfig/UserConfig.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/iqmath/include/IQmathLib.h>

/* Default value */
#define POWER_STARTUP_DELAY (16)
#define NTC_SHIFT (-40)
#define NTC_TABLE_LENGTH (126)

/* Definitions for ADC */
#define ADC12_INST (ADC0)
#define ADC12_INST_IRQHandler ADC0_IRQHandler
#define ADC12_INST_INT_IRQN (ADC0_INT_IRQn)
#define GPIO_ADC12_C2_PORT (GPIOA)
#define GPIO_ADC12_C2_PIN (DL_GPIO_PIN_25)
#define GPIO_IOMUX_ADC12_C2 (IOMUX_PINCM26)
#define GPIO_IOMUX_ADC12_C2_FUNCTION (IOMUX_PINCM26_PF_UNCONNECTED)

/* Definitions for UART */
#define UART_OVERSAMPLING_32_KHZ_9600_BAUD (DL_UART_OVERSAMPLING_RATE_3X)
#define UART_IBRD_32_MHZ_115200_BAUD (17)
#define UART_FBRD_32_MHZ_115200_BAUD (23)
#define GPIO_UART1_TX_PIN (DL_GPIO_PIN_0)
#define GPIO_UART1_IOMUX_TX (IOMUX_PINCM1)
#define GPIO_UART1_IOMUX_TX_FUNCTION (IOMUX_PINCM1_PF_UART1_TX)
#define GPIO_UART1_RX_PIN (DL_GPIO_PIN_1)
#define GPIO_UART1_IOMUX_RX (IOMUX_PINCM2)
#define GPIO_UART1_IOMUX_RX_FUNCTION (IOMUX_PINCM2_PF_UART1_RX)

/* Definitions for TIMERG */
#define TIMER_INST (TIMG0)
#define TIMER_INST_IRQHandler TIMG0_IRQHandler
#define TIMER_INST_INT_IRQN (TIMG0_INT_IRQn)

/* Definitions for ADC */
#define DMA_CH0_SEL (0)
#define ADC12_INST (ADC0)
#define ADC12_INST_IRQHandler ADC0_IRQHandler

/* clang-format on */
void SYSCFG_DL_initPower(void);
void Driver_TempMeassure(void);
void Driver_BattVoltMeassure(void);
void Driver_GpioInit(void);
void Driver_Uart1Init(void);
void Driver_DACBufferSet(void);
void Driver_DiffAmpSet(void);
void Driver_DetectionPowerEnable(void);
void Driver_DetectionPowerDisable(void);
void Driver_VrefInit(void);
_iq5 Driver_ADCInitAndSampling(
    uint32_t adcChannel, uint32_t vrefSource, tStatus highAvg);
void Driver_DACInit(uint32_t u32DacValue);
void Driver_OPA0Init(void);
void Driver_OPA1Init(uint32_t u32Gain, tOPACali opaCali);
void Driver_Uart1TxU16Data(uint16_t u16Data);
void Driver_TimerInit(void);
int16_t BSP_CalcuMcuTemp(void);
#endif /* DRIVER_MSPM0LXX_DRIVER_H_ */
