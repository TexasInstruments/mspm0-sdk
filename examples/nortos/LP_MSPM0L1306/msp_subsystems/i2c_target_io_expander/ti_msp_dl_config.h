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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L1306
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0L1306
#define CONFIG_MSPM0L1306

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
#define I2C_TARGET_INST                                                     I2C0
#define I2C_TARGET_INST_IRQHandler                               I2C0_IRQHandler
#define I2C_TARGET_INST_INT_IRQN                                   I2C0_INT_IRQn
#define I2C_TARGET_TARGET_OWN_ADDR                                          0x48
#define GPIO_I2C_TARGET_SDA_PORT                                           GPIOA
#define GPIO_I2C_TARGET_SDA_PIN                                    DL_GPIO_PIN_0
#define GPIO_I2C_TARGET_IOMUX_SDA                                 (IOMUX_PINCM1)
#define GPIO_I2C_TARGET_IOMUX_SDA_FUNC                  IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_TARGET_SCL_PORT                                           GPIOA
#define GPIO_I2C_TARGET_SCL_PIN                                    DL_GPIO_PIN_1
#define GPIO_I2C_TARGET_IOMUX_SCL                                 (IOMUX_PINCM2)
#define GPIO_I2C_TARGET_IOMUX_SCL_FUNC                  IOMUX_PINCM2_PF_I2C0_SCL



/* Port definition for Pin Group GPIO_A */
#define GPIO_A_PORT                                                      (GPIOA)

/* Defines for LED_RGB_GREEN: GPIOA.13 with pinCMx 14 on package pin 17 */
#define GPIO_A_LED_RGB_GREEN_PIN                                (DL_GPIO_PIN_13)
#define GPIO_A_LED_RGB_GREEN_IOMUX                               (IOMUX_PINCM14)
/* Defines for LED_RGB_RED: GPIOA.26 with pinCMx 27 on package pin 30 */
#define GPIO_A_LED_RGB_RED_PIN                                  (DL_GPIO_PIN_26)
#define GPIO_A_LED_RGB_RED_IOMUX                                 (IOMUX_PINCM27)
/* Defines for LED_RGB_BLUE: GPIOA.27 with pinCMx 28 on package pin 31 */
#define GPIO_A_LED_RGB_BLUE_PIN                                 (DL_GPIO_PIN_27)
#define GPIO_A_LED_RGB_BLUE_IOMUX                                (IOMUX_PINCM28)
/* Defines for IRQ_OUT: GPIOA.23 with pinCMx 24 on package pin 27 */
#define GPIO_A_IRQ_OUT_PIN                                      (DL_GPIO_PIN_23)
#define GPIO_A_IRQ_OUT_IOMUX                                     (IOMUX_PINCM24)
/* Defines for USER_OUT: GPIOA.12 with pinCMx 13 on package pin 16 */
#define GPIO_A_USER_OUT_PIN                                     (DL_GPIO_PIN_12)
#define GPIO_A_USER_OUT_IOMUX                                    (IOMUX_PINCM13)
/* Defines for USER_IN: GPIOA.7 with pinCMx 8 on package pin 11 */
#define GPIO_A_USER_IN_PIN                                       (DL_GPIO_PIN_7)
#define GPIO_A_USER_IN_IOMUX                                      (IOMUX_PINCM8)
/* Defines for SW_S1: GPIOA.18 with pinCMx 19 on package pin 22 */
// pins affected by this interrupt request:["SW_S1","SW_S2"]
#define GPIO_A_INT_IRQN                                         (GPIOA_INT_IRQn)
#define GPIO_A_INT_IIDX                         (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_A_SW_S1_IIDX                                   (DL_GPIO_IIDX_DIO18)
#define GPIO_A_SW_S1_PIN                                        (DL_GPIO_PIN_18)
#define GPIO_A_SW_S1_IOMUX                                       (IOMUX_PINCM19)
/* Defines for SW_S2: GPIOA.14 with pinCMx 15 on package pin 18 */
#define GPIO_A_SW_S2_IIDX                                   (DL_GPIO_IIDX_DIO14)
#define GPIO_A_SW_S2_PIN                                        (DL_GPIO_PIN_14)
#define GPIO_A_SW_S2_IOMUX                                       (IOMUX_PINCM15)
#define GPIOA_EVENT_SUBSCRIBER_1_CHANNEL                                     (1)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMG_DELAY_BLOCKING_init(void);
void SYSCFG_DL_I2C_TARGET_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
