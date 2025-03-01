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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L2228
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0L2228
#define CONFIG_MSPM0L2228

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




/* Defines for I2C_2 */
#define I2C_2_INST                                                          I2C2
#define I2C_2_INST_IRQHandler                                    I2C2_IRQHandler
#define I2C_2_INST_INT_IRQN                                        I2C2_INT_IRQn
#define I2C_2_BUS_SPEED_HZ                                                400000
#define GPIO_I2C_2_SDA_PORT                                                GPIOA
#define GPIO_I2C_2_SDA_PIN                                        DL_GPIO_PIN_16
#define GPIO_I2C_2_IOMUX_SDA                                     (IOMUX_PINCM42)
#define GPIO_I2C_2_IOMUX_SDA_FUNC                      IOMUX_PINCM42_PF_I2C2_SDA
#define GPIO_I2C_2_SCL_PORT                                                GPIOA
#define GPIO_I2C_2_SCL_PIN                                        DL_GPIO_PIN_15
#define GPIO_I2C_2_IOMUX_SCL                                     (IOMUX_PINCM41)
#define GPIO_I2C_2_IOMUX_SCL_FUNC                      IOMUX_PINCM41_PF_I2C2_SCL

/* Defines for I2C_1 */
#define I2C_1_INST                                                          I2C1
#define I2C_1_INST_IRQHandler                                    I2C1_IRQHandler
#define I2C_1_INST_INT_IRQN                                        I2C1_INT_IRQn
#define I2C_1_BUS_SPEED_HZ                                                400000
#define GPIO_I2C_1_SDA_PORT                                                GPIOA
#define GPIO_I2C_1_SDA_PIN                                        DL_GPIO_PIN_10
#define GPIO_I2C_1_IOMUX_SDA                                     (IOMUX_PINCM25)
#define GPIO_I2C_1_IOMUX_SDA_FUNC                      IOMUX_PINCM25_PF_I2C1_SDA
#define GPIO_I2C_1_SCL_PORT                                                GPIOA
#define GPIO_I2C_1_SCL_PIN                                        DL_GPIO_PIN_11
#define GPIO_I2C_1_IOMUX_SCL                                     (IOMUX_PINCM26)
#define GPIO_I2C_1_IOMUX_SCL_FUNC                      IOMUX_PINCM26_PF_I2C1_SCL

/* Defines for I2C_0 */
#define I2C_0_INST                                                          I2C0
#define I2C_0_INST_IRQHandler                                    I2C0_IRQHandler
#define I2C_0_INST_INT_IRQN                                        I2C0_INT_IRQn
#define I2C_0_TARGET_OWN_ADDR                                               0x10
#define GPIO_I2C_0_SDA_PORT                                                GPIOA
#define GPIO_I2C_0_SDA_PIN                                         DL_GPIO_PIN_0
#define GPIO_I2C_0_IOMUX_SDA                                      (IOMUX_PINCM1)
#define GPIO_I2C_0_IOMUX_SDA_FUNC                       IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_0_SCL_PORT                                                GPIOA
#define GPIO_I2C_0_SCL_PIN                                         DL_GPIO_PIN_1
#define GPIO_I2C_0_IOMUX_SCL                                      (IOMUX_PINCM2)
#define GPIO_I2C_0_IOMUX_SCL_FUNC                       IOMUX_PINCM2_PF_I2C0_SCL



/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.9 with pinCMx 20 on package pin 23 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_9)
#define GPIO_GRP_0_PIN_0_IOMUX                                   (IOMUX_PINCM20)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_I2C_2_init(void);
void SYSCFG_DL_I2C_1_init(void);
void SYSCFG_DL_I2C_0_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
