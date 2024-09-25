/*
 * Copyright (c) 2021, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_drivers_config.h =============
 *  Configured MSPM0 Driver module declarations
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
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

#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/dma/DMAMSPM0.h>
#include <ti/drivers/uart/UARTMSPM0.h>

#define CONFIG_UART_COUNT 1
#define CONFIG_UART_BUFFER_LENGTH 1

#define CONFIG_DMA_COUNT 1
#define CONFIG_DMA_CH_COUNT 2
#define DEFAULT_DMA_PRIORITY 31

extern const uint_least8_t CONFIG_UART_0;
extern const uint_least8_t UART_count;

uint8_t rxBuffer[CONFIG_UART_BUFFER_LENGTH];
uint8_t txBuffer[CONFIG_UART_BUFFER_LENGTH];

/* clang-format on */
void UART0_IRQHandler(void);
/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */

/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


/* Defines for CONFIG_UART_0 */
#define CONFIG_UART_0_INST                                                 UART0
#define CONFIG_UART_BAUD_RATE                                           (115200)

/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_0)
//#define GPIO_GRP_0_PIN_0_IOMUX                                    (IOMUX_PINCM1)

extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0 0
#define CONFIG_GPIO_LED_0_IOMUX         (IOMUX_PINCM1)

/* The range of pins available on this device */
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* LEDs are active low */
#define CONFIG_GPIO_LED_ON  (0)
#define CONFIG_GPIO_LED_OFF (1)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)

extern const uint_least8_t CONFIG_SPI_CONTROLLER_READY_CONST;
#define CONFIG_SPI_CONTROLLER_READY 47      //PB15
#define CONFIG_SPI_CONTROLLER_READY_IOMUX         (IOMUX_PINCM32)

extern const uint_least8_t CONFIG_SPI_PERIPHERAL_READY_CONST;
#define CONFIG_SPI_PERIPHERAL_READY 44      //PB12
#define CONFIG_SPI_PERIPHERAL_READY_IOMUX         (IOMUX_PINCM29)

/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define SPI_0_INST_DMA_TRIGGER_0                              (DMA_SPI1_TX_TRIG)

/* Defines for DMA_CH1 */
#define DMA_CH1_CHAN_ID                                                      (1)
#define SPI_0_INST_DMA_TRIGGER_1                              (DMA_SPI1_RX_TRIG)

/* clang-format on */

/*
 *  ======== SPI ========
 */

/*
 *  PICO: DIO13
 *  POCI: DIO12
 *  SCLK: DIO18
 *  CSN: DIO11
 *  LaunchPad SPI Bus with Chip Select
 */
#define SPI_1_INST SPI1
#define SPI_1_INST_IRQHandler SPI1_IRQHandler
#define SPI_1_INST_INT_IRQN SPI1_INT_IRQn

#define GPIO_SPI_1_PICO_PORT GPIOB
#define GPIO_SPI_1_PICO_PIN (40) /* PB8 */
#define GPIO_SPI_1_IOMUX_PICO (IOMUX_PINCM25)
#define GPIO_SPI_1_IOMUX_PICO_FUNC IOMUX_PINCM25_PF_SPI1_PICO

#define GPIO_SPI_1_POCI_PORT GPIOB
#define GPIO_SPI_1_POCI_PIN (39) /* PB7 */
#define GPIO_SPI_1_IOMUX_POCI (IOMUX_PINCM24)
#define GPIO_SPI_1_IOMUX_POCI_FUNC IOMUX_PINCM24_PF_SPI1_POCI
/* GPIO configuration for SPI_1 */
#define GPIO_SPI_1_SCLK_PORT GPIOB
#define GPIO_SPI_1_SCLK_PIN (41) /* PB9 */
#define GPIO_SPI_1_IOMUX_SCLK (IOMUX_PINCM26)
#define GPIO_SPI_1_IOMUX_SCLK_FUNC IOMUX_PINCM26_PF_SPI1_SCLK

#define GPIO_SPI_1_CS0_PORT GPIOB
#define GPIO_SPI_1_CS0_PIN (38) /* PB6 */
#define GPIO_SPI_1_IOMUX_CS0 (IOMUX_PINCM23)
#define GPIO_SPI_1_IOMUX_CS0_FUNC IOMUX_PINCM23_PF_SPI1_CS0

extern const uint_least8_t CONFIG_SPI_PERIPHERAL_CONST;
#define CONFIG_SPI_PERIPHERAL 0
#define CONFIG_TI_DRIVERS_SPI_COUNT 1

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
