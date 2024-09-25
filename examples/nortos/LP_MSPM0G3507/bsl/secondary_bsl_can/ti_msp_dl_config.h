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

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

/* GPIO configuration - UART */
#define BSL_GPIO_UART_TX_PIN (20U)
#define BSL_GPIO_UART_RX_PIN (21U)
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

/* GPIO configuration for SPI */

#define BSL_SPI_GPIO_IOMUX_SCLK (25U)
#define BSL_SPI_GPIO_IOMUX_COPI (24U)
#define BSL_SPI_GPIO_IOMUX_CIPO (23U)
#define BSL_SPI_GPIO_IOMUX_CS (22U)

#define BSL_SPI (SPI1)
#define BSL_SPI_INTERRUPT_NUM (SPI1_INT_IRQn)

#define BSL_SPI_GPIO_FUNCTION (3U)

#define BSL_SPI_CLOCK_SOURCE (DL_SPI_CLOCK_BUSCLK)

/* Defines for MCAN0 */
#define MCAN0_INST CANFD0
#define GPIO_MCAN0_CAN_TX_PORT GPIOA
#define GPIO_MCAN0_CAN_TX_PIN DL_GPIO_PIN_12
#define GPIO_MCAN0_IOMUX_CAN_TX (IOMUX_PINCM34)
#define GPIO_MCAN0_IOMUX_CAN_TX_FUNC IOMUX_PINCM34_PF_CANFD0_CANTX
#define GPIO_MCAN0_CAN_RX_PORT GPIOA
#define GPIO_MCAN0_CAN_RX_PIN DL_GPIO_PIN_13
#define GPIO_MCAN0_IOMUX_CAN_RX (IOMUX_PINCM35)
#define GPIO_MCAN0_IOMUX_CAN_RX_FUNC IOMUX_PINCM35_PF_CANFD0_CANRX
#define MCAN0_INST_IRQHandler CANFD0_IRQHandler
#define MCAN0_INST_INT_IRQN CANFD0_INT_IRQn

/* Defines for MCAN0 MCAN RAM configuration */
#define MCAN0_INST_MCAN_STD_ID_FILT_START_ADDR (0)
#define MCAN0_INST_MCAN_STD_ID_FILTER_NUM (1)
#define MCAN0_INST_MCAN_EXT_ID_FILT_START_ADDR (32)
#define MCAN0_INST_MCAN_EXT_ID_FILTER_NUM (1)
#define MCAN0_INST_MCAN_TX_BUFF_START_ADDR (64)
#define MCAN0_INST_MCAN_TX_BUFF_SIZE (2)
#define MCAN0_INST_MCAN_FIFO_1_START_ADDR (288)
#define MCAN0_INST_MCAN_FIFO_1_NUM (2)
#define MCAN0_INST_MCAN_TX_EVENT_START_ADDR (192)
#define MCAN0_INST_MCAN_TX_EVENT_SIZE (2)
#define MCAN0_INST_MCAN_EXT_ID_AND_MASK (0x1FFFFFFFU)
#define MCAN0_INST_MCAN_RX_BUFF_START_ADDR (320)
#define MCAN0_INST_MCAN_FIFO_0_START_ADDR (256)
#define MCAN0_INST_MCAN_FIFO_0_NUM (10)

#define MCAN0_INST_MCAN_INTERRUPTS (DL_MCAN_INTR_MASK_ALL)

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower_(void);
void SYSCFG_DL_GPIO_init_(void);
void SYSCFG_DL_SYSCTL_init_(void);
void SYSCFG_DL_SYSCTL_CLK_init_(void);
void SYSCFG_DL_MCAN0_init(void);

#define GPIO_HFXT_PORT GPIOA
#define GPIO_HFXIN_PIN DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX (IOMUX_PINCM11)
#define CPUCLK_FREQ 80000000

#define POWER_STARTUP_DELAY (16)
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

#define BSL_STACK_SIZE ((uint32_t) 120)

#define MSPM0_MAIN_FLASH_START_ADDRESS ((uint32_t) 0x00000000)

#define MSPM0_INFO_FLASH_START_ADDRESS ((uint32_t) 0x41C00000)
#define MSPM0_INFO_FLASH_END_ADDRESS ((uint32_t) 0x41C00400)

#define MSPM0_RAM_START_ADDRESS ((uint32_t) 0x20200000)

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
