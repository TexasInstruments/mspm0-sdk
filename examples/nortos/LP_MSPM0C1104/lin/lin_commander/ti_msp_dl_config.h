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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0C1104
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0C1104
#define CONFIG_MSPM0C1104

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


#define CPUCLK_FREQ                                                     24000000



/* Defines for LIN_0 */
#define LIN_0_INST                                                         UART0
#define LIN_0_INST_FREQUENCY                                            24000000
#define LIN_0_INST_IRQHandler                                   UART0_IRQHandler
#define LIN_0_INST_INT_IRQN                                       UART0_INT_IRQn
#define GPIO_LIN_0_RX_PORT                                                 GPIOA
#define GPIO_LIN_0_TX_PORT                                                 GPIOA
#define GPIO_LIN_0_RX_PIN                                         DL_GPIO_PIN_26
#define GPIO_LIN_0_TX_PIN                                         DL_GPIO_PIN_27
#define GPIO_LIN_0_IOMUX_RX                                      (IOMUX_PINCM27)
#define GPIO_LIN_0_IOMUX_TX                                      (IOMUX_PINCM28)
#define GPIO_LIN_0_IOMUX_RX_FUNC                       IOMUX_PINCM27_PF_UART0_RX
#define GPIO_LIN_0_IOMUX_TX_FUNC                       IOMUX_PINCM28_PF_UART0_TX
#define LIN_0_BAUD_RATE                                                  (19200)
#define LIN_0_IBRD_24_MHZ_19200_BAUD                                        (78)
#define LIN_0_FBRD_24_MHZ_19200_BAUD                                         (8)
#define LIN_0_TBIT_WIDTH                                                  (1250)





/* Port definition for Pin Group GPIO_LEDS */
#define GPIO_LEDS_PORT                                                   (GPIOA)

/* Defines for USER_LED_1: GPIOA.4 with pinCMx 5 on package pin 9 */
#define GPIO_LEDS_USER_LED_1_PIN                                 (DL_GPIO_PIN_4)
#define GPIO_LEDS_USER_LED_1_IOMUX                                (IOMUX_PINCM5)
/* Port definition for Pin Group GPIO_LIN_ENABLE */
#define GPIO_LIN_ENABLE_PORT                                             (GPIOA)

/* Defines for USER_LIN_ENABLE: GPIOA.17 with pinCMx 18 on package pin 13 */
#define GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN                     (DL_GPIO_PIN_17)
#define GPIO_LIN_ENABLE_USER_LIN_ENABLE_IOMUX                    (IOMUX_PINCM18)
/* Port definition for Pin Group GPIO_SWITCHES1 */
#define GPIO_SWITCHES1_PORT                                              (GPIOA)

/* Defines for USER_SWITCH_1: GPIOA.16 with pinCMx 17 on package pin 12 */
// pins affected by this interrupt request:["USER_SWITCH_1"]
#define GPIO_SWITCHES1_INT_IRQN                                 (GPIOA_INT_IRQn)
#define GPIO_SWITCHES1_INT_IIDX                 (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_SWITCHES1_USER_SWITCH_1_IIDX                   (DL_GPIO_IIDX_DIO16)
#define GPIO_SWITCHES1_USER_SWITCH_1_PIN                        (DL_GPIO_PIN_16)
#define GPIO_SWITCHES1_USER_SWITCH_1_IOMUX                       (IOMUX_PINCM17)
/* Port definition for Pin Group GPIO_INPUT */
#define GPIO_INPUT_PORT                                                  (GPIOA)

/* Defines for USER_INPUT_1: GPIOA.6 with pinCMx 7 on package pin 10 */
#define GPIO_INPUT_USER_INPUT_1_PIN                              (DL_GPIO_PIN_6)
#define GPIO_INPUT_USER_INPUT_1_IOMUX                             (IOMUX_PINCM7)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_LIN_0_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
