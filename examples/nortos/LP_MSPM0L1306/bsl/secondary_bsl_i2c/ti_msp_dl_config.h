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
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/* GPIO configuration - UART */
#define BSL_GPIO_UART_TX_PIN (23U)
#define BSL_GPIO_UART_RX_PIN (22U)
#define BSL_GPIO_UART_TX (2U)
#define BSL_GPIO_UART_RX (2U)

/* Definitions for UART */
#define BSL_UART (UART0)
#define BSL_UART_INTERRUPT_NUM (UART0_INT_IRQn)
#define BSL_UART_CLOCK_SOURCE (DL_UART_MAIN_CLOCK_BUSCLK)
#define BSL_UART_DEFAULT_BAUD ((uint32_t) 9600U)
#define BSL_UART_CLOCK ((uint32_t) 32000000U)

/* GPIO configuration - I2C */
#define BSL_GPIO_I2C_SDA_PIN (0U)
#define BSL_GPIO_I2C_SCL_PIN (1U)
#define BSL_GPIO_I2C_SDA (3U)
#define BSL_GPIO_I2C_SCL (3U)

/* Definitions for I2C */
#define BSL_I2C (I2C0)
#define BSL_I2C_INTERRUPT_NUM (I2C0_INT_IRQn)
#define BSL_I2C_CLOCK_SOURCE (DL_I2C_CLOCK_BUSCLK)
#define BSL_I2C_TARGET_ADDRESS ((uint32_t) 0x00000048)

#define BSL_SPI_GPIO_IOMUX_SCLK (6U)
#define BSL_SPI_GPIO_IOMUX_COPI (5U)
#define BSL_SPI_GPIO_IOMUX_CIPO (4U)
#define BSL_SPI_GPIO_IOMUX_CS (8U)

#define BSL_SPI (SPI0)
#define BSL_SPI_INTERRUPT_NUM (SPI0_INT_IRQn)

#define BSL_SPI_GPIO_FUNCTION (3U)

#define BSL_SPI_CLOCK_SOURCE (DL_SPI_CLOCK_BUSCLK)

/* Definitions for CRC */
#define BSL_CRC (CRC)
#define BSL_CRC_SEED ((uint32_t) 0xFFFFFFFFU)

/* Definitions for TIMER */
#define BSL_TIMER TIMG0
#define BSL_GPTIMER_INT_VEC TIMG0_INT_IRQn

/* Equivalent time for 1 count = 1/((32KHz/60)) second.
 * where,
 * 32KHz clock frequency of LFCLK
 * 60 is the Pre-scale value loaded.
 * Equivalent count value for 1 second = (32KHz/60)
 */
#define BSL_ONE_SECOND_TIMEOUT ((uint16_t) 533U)
#define BSL_TWO_SECOND_TIMEOUT ((uint16_t) 2U * (BSL_ONE_SECOND_TIMEOUT))
#define BSL_TEN_SECOND_TIMEOUT ((uint16_t) 10U * (BSL_ONE_SECOND_TIMEOUT))
#define BSL_TIMER_PRE_SCALE ((uint8_t) 60U)

/* Definitions for FLASH */
#define MAIN_SECTOR_SIZE ((uint32_t) 1024)
#define INFO_SECTOR_SIZE (1024)

/* Definitions for Memory range */

#define BSL_SRAM_BUF_START_ADDR ((uint32_t) 0x20000180)
#define BSL_STACK_SIZE ((uint32_t) 120)

#define MSPM0_MAIN_FLASH_START_ADDRESS ((uint32_t) 0x00000000)

#define MSPM0_INFO_FLASH_START_ADDRESS ((uint32_t) 0x41C00000)
#define MSPM0_INFO_FLASH_END_ADDRESS ((uint32_t) 0x41C00400)

#define MSPM0_RAM_START_ADDRESS ((uint32_t) 0x20000000)

/*
 * Function declaration
 */
/*
 * @brief       Gets the RAM end address from factory configuration
 *
 * @return      Returns the end address of the SRAM available in the device
 */
uint32_t BSL_CI_getRAMEndAddress(void);

/*
 * @brief       Gets the Main Flash end address from factory configuration
 *
 * @return      Returns the end address of the Flash available in the device
 */
uint32_t BSL_CI_getFlashEndAddress(void);

#endif /* ti_msp_dl_config_h */
