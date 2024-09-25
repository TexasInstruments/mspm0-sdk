/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_LP_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_gpio.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/adc/ADCMSPM0.h>
#include <ti/drivers/dma/DMAMSPM0.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== ADC ========
 */
#define ADC_0_INST ADC0
#define ADC_1_INST ADC1
/* PA22_ADC0_7 */
extern const uint_least8_t CONFIG_ADC_0_CONST;
#define CONFIG_ADC_0 0
/* PA17_ADC1_2 */
extern const uint_least8_t CONFIG_ADC_1_CONST;
#define CONFIG_ADC_1 1
#define CONFIG_TI_DRIVERS_ADC_COUNT 2

/*
 *  ======== GPIO ========
 */
/* Owned by CONFIG_ADC_0 as  */
extern const uint_least8_t CONFIG_GPIO_ADC_0_AIN_CONST;
/* This is actual port pin */
#define CONFIG_GPIO_ADC_0_AIN (22)
#define CONFIG_GPIO_ADC_0_AIN_PINCM (IOMUX_PINCM47)
#define CONFIG_GPIO_ADC_0_AIN_PINMUX (IOMUX_PINCM47_PF_GPIOA_DIO22)
/* Owned by CONFIG_ADC_1 as  */
extern const uint_least8_t CONFIG_GPIO_ADC_1_AIN_CONST;
/* This is actual port pin */
#define CONFIG_GPIO_ADC_1_AIN (17)
#define CONFIG_GPIO_ADC_1_AIN_PINCM (IOMUX_PINCM39)
#define CONFIG_GPIO_ADC_1_AIN_PINMUX (IOMUX_PINCM39_PF_GPIOA_DIO17)

/* Owned by CONFIG_UART2_0 as  */
extern const uint_least8_t CONFIG_GPIO_UART2_0_TX_CONST;
#define CONFIG_GPIO_UART_0_TX (10)

/* Owned by CONFIG_UART2_0 as  */
extern const uint_least8_t CONFIG_GPIO_UART2_0_RX_CONST;
#define CONFIG_GPIO_UART_0_RX (11)

extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0 0
#define CONFIG_GPIO_LED_0_IOMUX (IOMUX_PINCM1)

/* The range of pins available on this device */
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* LEDs are active low */
#define CONFIG_GPIO_LED_ON (0)
#define CONFIG_GPIO_LED_OFF (1)

#define CONFIG_LED_ON (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)

/*
 *  ======== UART ========
 */

extern const uint_least8_t CONFIG_UART_0_CONST;

/*********************************************************/
/* Defines for CONFIG_UART_0 */
#define CONFIG_UART_0_INST UART0

#define CONFIG_UART_COUNT 1
#define CONFIG_UART_BUFFER_LENGTH 1

#define CONFIG_DMA_COUNT 1
#define CONFIG_DMA_CH_COUNT 1
#define DEFAULT_DMA_PRIORITY 31

extern const uint_least8_t CONFIG_UART_0;
extern const uint_least8_t UART_count;

uint8_t rxBuffer[CONFIG_UART_BUFFER_LENGTH];
uint8_t txBuffer[CONFIG_UART_BUFFER_LENGTH];

void UART0_IRQHandler(void);

/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif
#endif /* ti_drivers_config_h */
