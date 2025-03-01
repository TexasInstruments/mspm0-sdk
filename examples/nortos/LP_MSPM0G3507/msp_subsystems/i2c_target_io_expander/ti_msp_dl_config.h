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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0G3507
#define CONFIG_MSPM0G3507

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



/* Defines for TIMG_DELAY_BLOCKING */
#define TIMG_DELAY_BLOCKING_INST                                         (TIMG0)
#define TIMG_DELAY_BLOCKING_INST_IRQHandler                        TIMG0_IRQHandler
#define TIMG_DELAY_BLOCKING_INST_INT_IRQN                        (TIMG0_INT_IRQn)
#define TIMG_DELAY_BLOCKING_INST_LOAD_VALUE                                   (32U)




/* Defines for I2C_TARGET */
#define I2C_TARGET_INST                                                     I2C1
#define I2C_TARGET_INST_IRQHandler                               I2C1_IRQHandler
#define I2C_TARGET_INST_INT_IRQN                                   I2C1_INT_IRQn
#define I2C_TARGET_TARGET_OWN_ADDR                                          0x48
#define GPIO_I2C_TARGET_SDA_PORT                                           GPIOB
#define GPIO_I2C_TARGET_SDA_PIN                                    DL_GPIO_PIN_3
#define GPIO_I2C_TARGET_IOMUX_SDA                                (IOMUX_PINCM16)
#define GPIO_I2C_TARGET_IOMUX_SDA_FUNC                 IOMUX_PINCM16_PF_I2C1_SDA
#define GPIO_I2C_TARGET_SCL_PORT                                           GPIOB
#define GPIO_I2C_TARGET_SCL_PIN                                    DL_GPIO_PIN_2
#define GPIO_I2C_TARGET_IOMUX_SCL                                (IOMUX_PINCM15)
#define GPIO_I2C_TARGET_IOMUX_SCL_FUNC                 IOMUX_PINCM15_PF_I2C1_SCL



/* Port definition for Pin Group GPIO_B */
#define GPIO_B_PORT                                                      (GPIOB)

/* Defines for LED_RED: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_B_LED_RED_PIN                                      (DL_GPIO_PIN_26)
#define GPIO_B_LED_RED_IOMUX                                     (IOMUX_PINCM57)
/* Defines for LED_GREEN: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_B_LED_GREEN_PIN                                    (DL_GPIO_PIN_27)
#define GPIO_B_LED_GREEN_IOMUX                                   (IOMUX_PINCM58)
/* Defines for LED_BLUE: GPIOB.22 with pinCMx 50 on package pin 21 */
#define GPIO_B_LED_BLUE_PIN                                     (DL_GPIO_PIN_22)
#define GPIO_B_LED_BLUE_IOMUX                                    (IOMUX_PINCM50)
/* Defines for SW_S3: GPIOB.21 with pinCMx 49 on package pin 20 */
// pins affected by this interrupt request:["SW_S3"]
#define GPIO_B_INT_IRQN                                         (GPIOB_INT_IRQn)
#define GPIO_B_INT_IIDX                         (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_B_SW_S3_IIDX                                   (DL_GPIO_IIDX_DIO21)
#define GPIO_B_SW_S3_PIN                                        (DL_GPIO_PIN_21)
#define GPIO_B_SW_S3_IOMUX                                       (IOMUX_PINCM49)
/* Defines for IRQ_OUT: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_B_IRQ_OUT_PIN                                       (DL_GPIO_PIN_0)
#define GPIO_B_IRQ_OUT_IOMUX                                     (IOMUX_PINCM12)
/* Defines for USER_OUT: GPIOB.4 with pinCMx 17 on package pin 52 */
#define GPIO_B_USER_OUT_PIN                                      (DL_GPIO_PIN_4)
#define GPIO_B_USER_OUT_IOMUX                                    (IOMUX_PINCM17)
/* Port definition for Pin Group GPIO_A */
#define GPIO_A_PORT                                                      (GPIOA)

/* Defines for USER_IN: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_A_USER_IN_PIN                                      (DL_GPIO_PIN_13)
#define GPIO_A_USER_IN_IOMUX                                     (IOMUX_PINCM35)
/* Defines for SW_S2: GPIOA.18 with pinCMx 40 on package pin 11 */
// pins affected by this interrupt request:["SW_S2"]
#define GPIO_A_INT_IRQN                                         (GPIOA_INT_IRQn)
#define GPIO_A_INT_IIDX                         (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_A_SW_S2_IIDX                                   (DL_GPIO_IIDX_DIO18)
#define GPIO_A_SW_S2_PIN                                        (DL_GPIO_PIN_18)
#define GPIO_A_SW_S2_IOMUX                                       (IOMUX_PINCM40)
/* Defines for PA0_DEBUG: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_A_PA0_DEBUG_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_A_PA0_DEBUG_IOMUX                                    (IOMUX_PINCM1)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMG_DELAY_BLOCKING_init(void);
void SYSCFG_DL_I2C_TARGET_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
