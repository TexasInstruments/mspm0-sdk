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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G5187
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0G5187
#define CONFIG_MSPM0G5187

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
#define I2C_INST                                                             UC0
#define I2C_INST_IRQHandler                                       UC0_IRQHandler
#define I2C_INST_INT_IRQN                                           UC0_INT_IRQn
#define I2C_TARGET_OWN_ADDR                                                 0x48
#define GPIO_I2C_SDA_PORT                                                  GPIOA
#define GPIO_I2C_SDA_PIN                                           DL_GPIO_PIN_0
#define GPIO_I2C_IOMUX_SDA                                        (IOMUX_PINCM1)
#define GPIO_I2C_IOMUX_SDA_FUNC                          IOMUX_PINCM1_PF_UC0_SDA
#define GPIO_I2C_SCL_PORT                                                  GPIOA
#define GPIO_I2C_SCL_PIN                                           DL_GPIO_PIN_1
#define GPIO_I2C_IOMUX_SCL                                        (IOMUX_PINCM2)
#define GPIO_I2C_IOMUX_SCL_FUNC                          IOMUX_PINCM2_PF_UC0_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                          UC1
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                    UC1_IRQHandler
#define UART_0_INST_INT_IRQN                                        UC1_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_31
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_28
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM6)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM3)
#define GPIO_UART_0_IOMUX_RX_FUNC                         IOMUX_PINCM6_PF_UC1_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                         IOMUX_PINCM3_PF_UC1_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_0_FBRD_32_MHZ_9600_BAUD                                        (21)




/* Defines for SPI_0 */
#define SPI_0_INST                                                          UC2
#define SPI_0_INST_IRQHandler                                    UC2_IRQHandler
#define SPI_0_INST_INT_IRQN                                        UC2_INT_IRQn
#define GPIO_SPI_0_PICO_PORT                                              GPIOA
#define GPIO_SPI_0_PICO_PIN                                       DL_GPIO_PIN_9
#define GPIO_SPI_0_IOMUX_PICO                                   (IOMUX_PINCM20)
#define GPIO_SPI_0_IOMUX_PICO_FUNC                    IOMUX_PINCM20_PF_UC2_PICO
#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                      DL_GPIO_PIN_10
#define GPIO_SPI_0_IOMUX_POCI                                   (IOMUX_PINCM21)
#define GPIO_SPI_0_IOMUX_POCI_FUNC                    IOMUX_PINCM21_PF_UC2_POCI
/* GPIO configuration for SPI_0 */
#define GPIO_SPI_0_SCLK_PORT                                              GPIOA
#define GPIO_SPI_0_SCLK_PIN                                      DL_GPIO_PIN_11
#define GPIO_SPI_0_IOMUX_SCLK                                   (IOMUX_PINCM22)
#define GPIO_SPI_0_IOMUX_SCLK_FUNC                    IOMUX_PINCM22_PF_UC2_SCLK



/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.2 with pinCMx 7 on package pin 42 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_2)
#define GPIO_GRP_0_PIN_0_IOMUX                                    (IOMUX_PINCM7)
/* Defines for PIN_1: GPIOA.25 with pinCMx 55 on package pin 26 */
#define GPIO_GRP_0_PIN_1_PIN                                    (DL_GPIO_PIN_25)
#define GPIO_GRP_0_PIN_1_IOMUX                                   (IOMUX_PINCM55)
/* Defines for PIN_2: GPIOA.8 with pinCMx 19 on package pin 54 */
#define GPIO_GRP_0_PIN_2_PIN                                     (DL_GPIO_PIN_8)
#define GPIO_GRP_0_PIN_2_IOMUX                                   (IOMUX_PINCM19)
/* Defines for PIN_3: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_GRP_0_PIN_3_PIN                                    (DL_GPIO_PIN_13)
#define GPIO_GRP_0_PIN_3_IOMUX                                   (IOMUX_PINCM35)
/* Defines for PIN_4: GPIOA.29 with pinCMx 4 on package pin 36 */
#define GPIO_GRP_0_PIN_4_PIN                                    (DL_GPIO_PIN_29)
#define GPIO_GRP_0_PIN_4_IOMUX                                    (IOMUX_PINCM4)
/* Defines for PIN_5: GPIOA.14 with pinCMx 36 on package pin 7 */
#define GPIO_GRP_0_PIN_5_PIN                                    (DL_GPIO_PIN_14)
#define GPIO_GRP_0_PIN_5_IOMUX                                   (IOMUX_PINCM36)
/* Defines for PIN_6: GPIOA.16 with pinCMx 38 on package pin 9 */
#define GPIO_GRP_0_PIN_6_PIN                                    (DL_GPIO_PIN_16)
#define GPIO_GRP_0_PIN_6_IOMUX                                   (IOMUX_PINCM38)
/* Defines for PIN_7: GPIOA.15 with pinCMx 37 on package pin 8 */
#define GPIO_GRP_0_PIN_7_PIN                                    (DL_GPIO_PIN_15)
#define GPIO_GRP_0_PIN_7_IOMUX                                   (IOMUX_PINCM37)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_I2C_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_SPI_0_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
