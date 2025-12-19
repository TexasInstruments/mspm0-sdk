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



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                             (0U)



/* Defines for LIN_0 */
#define LIN_0_INST                                                           UC0
#define LIN_0_INST_FREQUENCY                                            32000000
#define LIN_0_INST_IRQHandler                                     UC0_IRQHandler
#define LIN_0_INST_INT_IRQN                                         UC0_INT_IRQn
#define GPIO_LIN_0_RX_PORT                                                 GPIOA
#define GPIO_LIN_0_TX_PORT                                                 GPIOA
#define GPIO_LIN_0_RX_PIN                                          DL_GPIO_PIN_9
#define GPIO_LIN_0_TX_PIN                                          DL_GPIO_PIN_8
#define GPIO_LIN_0_IOMUX_RX                                      (IOMUX_PINCM20)
#define GPIO_LIN_0_IOMUX_TX                                      (IOMUX_PINCM19)
#define GPIO_LIN_0_IOMUX_RX_FUNC                         IOMUX_PINCM20_PF_UC0_RX
#define GPIO_LIN_0_IOMUX_TX_FUNC                         IOMUX_PINCM19_PF_UC0_TX
#define LIN_0_BAUD_RATE                                                  (19200)
#define LIN_0_IBRD_32_MHZ_19200_BAUD                                       (104)
#define LIN_0_FBRD_32_MHZ_19200_BAUD                                        (11)
#define LIN_0_TBIT_WIDTH                                                  (1666)





/* Port definition for Pin Group GPIO_LIN_ENABLE */
#define GPIO_LIN_ENABLE_PORT                                             (GPIOB)

/* Defines for USER_LIN_ENABLE: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN                     (DL_GPIO_PIN_15)
#define GPIO_LIN_ENABLE_USER_LIN_ENABLE_IOMUX                    (IOMUX_PINCM32)
/* Port definition for Pin Group GPIO_SWITCHES1 */
#define GPIO_SWITCHES1_PORT                                              (GPIOA)

/* Defines for USER_SWITCH_1: GPIOA.7 with pinCMx 14 on package pin 49 */
// groups represented: ["GPIO_SWITCHES2","GPIO_SWITCHES1"]
// pins affected: ["USER_SWITCH_2","USER_SWITCH_1"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_SWITCHES1_USER_SWITCH_1_IIDX                    (DL_GPIO_IIDX_DIO7)
#define GPIO_SWITCHES1_USER_SWITCH_1_PIN                         (DL_GPIO_PIN_7)
#define GPIO_SWITCHES1_USER_SWITCH_1_IOMUX                       (IOMUX_PINCM14)
/* Port definition for Pin Group GPIO_SWITCHES2 */
#define GPIO_SWITCHES2_PORT                                              (GPIOA)

/* Defines for USER_SWITCH_2: GPIOA.18 with pinCMx 40 on package pin 11 */
#define GPIO_SWITCHES2_USER_SWITCH_2_IIDX                   (DL_GPIO_IIDX_DIO18)
#define GPIO_SWITCHES2_USER_SWITCH_2_PIN                        (DL_GPIO_PIN_18)
#define GPIO_SWITCHES2_USER_SWITCH_2_IOMUX                       (IOMUX_PINCM40)
/* Defines for USER_LED_1: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GPIO_LEDS_USER_LED_1_PORT                                        (GPIOB)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_13)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM30)
/* Defines for USER_LED_2: GPIOA.17 with pinCMx 39 on package pin 10 */
#define GPIO_LEDS_USER_LED_2_PORT                                        (GPIOA)
#define GPIO_LEDS_USER_LED_2_PIN                                (DL_GPIO_PIN_17)
#define GPIO_LEDS_USER_LED_2_IOMUX                               (IOMUX_PINCM39)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_LIN_0_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
