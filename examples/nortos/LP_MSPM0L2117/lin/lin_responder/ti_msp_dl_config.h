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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L2117
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0L2117
#define CONFIG_MSPM0L2117

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


#define GPIO_CLKOUT_PORT                                                   GPIOA
#define GPIO_CLKOUT_PIN                                           DL_GPIO_PIN_14
#define GPIO_CLKOUT_IOMUX                                        (IOMUX_PINCM36)
#define GPIO_CLKOUT_IOMUX_FUNC                   IOMUX_PINCM36_PF_SYSCTL_CLK_OUT
#define CPUCLK_FREQ                                                     32000000



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                             (0U)



/* Defines for LIN_0 */
#define LIN_0_INST                                                           UC4
#define LIN_0_INST_FREQUENCY                                            32000000
#define LIN_0_INST_IRQHandler                                     UC4_IRQHandler
#define LIN_0_INST_INT_IRQN                                         UC4_INT_IRQn
#define GPIO_LIN_0_RX_PORT                                                 GPIOA
#define GPIO_LIN_0_TX_PORT                                                 GPIOA
#define GPIO_LIN_0_RX_PIN                                         DL_GPIO_PIN_11
#define GPIO_LIN_0_TX_PIN                                         DL_GPIO_PIN_10
#define GPIO_LIN_0_IOMUX_RX                                      (IOMUX_PINCM22)
#define GPIO_LIN_0_IOMUX_TX                                      (IOMUX_PINCM21)
#define GPIO_LIN_0_IOMUX_RX_FUNC                         IOMUX_PINCM22_PF_UC4_RX
#define GPIO_LIN_0_IOMUX_TX_FUNC                         IOMUX_PINCM21_PF_UC4_TX
#define LIN_0_BAUD_RATE                                                  (19200)
#define LIN_0_IBRD_32_MHZ_19200_BAUD                                       (104)
#define LIN_0_FBRD_32_MHZ_19200_BAUD                                        (11)
#define LIN_0_TBIT_WIDTH                                                  (1666)
#define LIN_0_TBIT_COUNTER_COEFFICIENT                                     (9.5)
#define LIN_0_COUNTER_COMPARE_VALUE                                      (15833)





/* Port definition for Pin Group GPIO_LIN_ENABLE */
#define GPIO_LIN_ENABLE_PORT                                             (GPIOB)

/* Defines for USER_LIN_ENABLE: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN                     (DL_GPIO_PIN_15)
#define GPIO_LIN_ENABLE_USER_LIN_ENABLE_IOMUX                    (IOMUX_PINCM32)
/* Defines for USER_LED_1: GPIOA.23 with pinCMx 53 on package pin 24 */
#define GPIO_LEDS_USER_LED_1_PORT                                        (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_23)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM53)
/* Defines for USER_LED_2: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_LEDS_USER_LED_2_PORT                                        (GPIOB)
#define GPIO_LEDS_USER_LED_2_PIN                                 (DL_GPIO_PIN_0)
#define GPIO_LEDS_USER_LED_2_IOMUX                               (IOMUX_PINCM12)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_LIN_0_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
