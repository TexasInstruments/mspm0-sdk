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
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L1117
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0L1117
#define CONFIG_MSPM0L1117

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     32000000




/* Defines for I2C */
#define I2C_INST                                                            I2C0
#define I2C_INST_IRQHandler                                      I2C0_IRQHandler
#define I2C_INST_INT_IRQN                                          I2C0_INT_IRQn
#define I2C_TARGET_OWN_ADDR                                                 0x48
#define GPIO_I2C_SDA_PORT                                                  GPIOA
#define GPIO_I2C_SDA_PIN                                           DL_GPIO_PIN_0
#define GPIO_I2C_IOMUX_SDA                                        (IOMUX_PINCM1)
#define GPIO_I2C_IOMUX_SDA_FUNC                         IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_SCL_PORT                                                  GPIOA
#define GPIO_I2C_SCL_PIN                                           DL_GPIO_PIN_1
#define GPIO_I2C_IOMUX_SCL                                        (IOMUX_PINCM2)
#define GPIO_I2C_IOMUX_SCL_FUNC                         IOMUX_PINCM2_PF_I2C0_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           16000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_16_MHZ_9600_BAUD                                       (104)
#define UART_0_FBRD_16_MHZ_9600_BAUD                                        (11)




/* Defines for SPI_0 */
#define SPI_0_INST                                                         SPI0
#define SPI_0_INST_IRQHandler                                   SPI0_IRQHandler
#define SPI_0_INST_INT_IRQN                                       SPI0_INT_IRQn
#define GPIO_SPI_0_PICO_PORT                                              GPIOB
#define GPIO_SPI_0_PICO_PIN                                       DL_GPIO_PIN_2
#define GPIO_SPI_0_IOMUX_PICO                                   (IOMUX_PINCM15)
#define GPIO_SPI_0_IOMUX_PICO_FUNC                   IOMUX_PINCM15_PF_SPI0_PICO
#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                      DL_GPIO_PIN_13
#define GPIO_SPI_0_IOMUX_POCI                                   (IOMUX_PINCM35)
#define GPIO_SPI_0_IOMUX_POCI_FUNC                   IOMUX_PINCM35_PF_SPI0_POCI
/* GPIO configuration for SPI_0 */
#define GPIO_SPI_0_SCLK_PORT                                              GPIOA
#define GPIO_SPI_0_SCLK_PIN                                      DL_GPIO_PIN_12
#define GPIO_SPI_0_IOMUX_SCLK                                   (IOMUX_PINCM34)
#define GPIO_SPI_0_IOMUX_SCLK_FUNC                   IOMUX_PINCM34_PF_SPI0_SCLK



/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.8 with pinCMx 19 on package pin 16 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_8)
#define GPIO_GRP_0_PIN_0_IOMUX                                   (IOMUX_PINCM19)
/* Defines for PIN_1: GPIOA.9 with pinCMx 20 on package pin 17 */
#define GPIO_GRP_0_PIN_1_PIN                                     (DL_GPIO_PIN_9)
#define GPIO_GRP_0_PIN_1_IOMUX                                   (IOMUX_PINCM20)
/* Defines for PIN_2: GPIOA.25 with pinCMx 55 on package pin 45 */
#define GPIO_GRP_0_PIN_2_PIN                                    (DL_GPIO_PIN_25)
#define GPIO_GRP_0_PIN_2_IOMUX                                   (IOMUX_PINCM55)
/* Defines for PIN_3: GPIOA.26 with pinCMx 59 on package pin 46 */
#define GPIO_GRP_0_PIN_3_PIN                                    (DL_GPIO_PIN_26)
#define GPIO_GRP_0_PIN_3_IOMUX                                   (IOMUX_PINCM59)
/* Defines for PIN_4: GPIOA.14 with pinCMx 36 on package pin 29 */
#define GPIO_GRP_0_PIN_4_PIN                                    (DL_GPIO_PIN_14)
#define GPIO_GRP_0_PIN_4_IOMUX                                   (IOMUX_PINCM36)
/* Defines for PIN_5: GPIOA.18 with pinCMx 40 on package pin 33 */
#define GPIO_GRP_0_PIN_5_PIN                                    (DL_GPIO_PIN_18)
#define GPIO_GRP_0_PIN_5_IOMUX                                   (IOMUX_PINCM40)
/* Defines for PIN_6: GPIOA.24 with pinCMx 54 on package pin 44 */
#define GPIO_GRP_0_PIN_6_PIN                                    (DL_GPIO_PIN_24)
#define GPIO_GRP_0_PIN_6_IOMUX                                   (IOMUX_PINCM54)
/* Defines for PIN_7: GPIOA.17 with pinCMx 39 on package pin 32 */
#define GPIO_GRP_0_PIN_7_PIN                                    (DL_GPIO_PIN_17)
#define GPIO_GRP_0_PIN_7_IOMUX                                   (IOMUX_PINCM39)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_I2C_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_SPI_0_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
