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



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (4999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM1)
#define GPIO_UART_0_IOMUX_RX_FUNC                       IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                       IOMUX_PINCM1_PF_UART0_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_0_FBRD_32_MHZ_9600_BAUD                                        (21)





/* Port definition for Pin Group GPIO_Signal_14 */
#define GPIO_Signal_14_PORT                                              (GPIOA)

/* Defines for USER_Signal_14: GPIOA.14 with pinCMx 36 on package pin 29 */
// groups represented: ["GPIO_Signal_13","GPIO_Signal_15","GPIO_Signal_16","GPIO_Signal_17","GPIO_Signal_18","GPIO_Signal_12","GPIO_Signal_11","GPIO_Signal_10","GPIO_Signal_14"]
// pins affected: ["PIN_0","PIN_1","PIN_2","PIN_3","PIN_4","PIN_5","PIN_6","PIN_7","USER_Signal_14"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_Signal_14_USER_Signal_14_IIDX                  (DL_GPIO_IIDX_DIO14)
#define GPIO_Signal_14_USER_Signal_14_PIN                       (DL_GPIO_PIN_14)
#define GPIO_Signal_14_USER_Signal_14_IOMUX                      (IOMUX_PINCM36)
/* Port definition for Pin Group GPIO_Signal_13 */
#define GPIO_Signal_13_PORT                                              (GPIOA)

/* Defines for PIN_0: GPIOA.13 with pinCMx 35 on package pin 28 */
#define GPIO_Signal_13_PIN_0_IIDX                           (DL_GPIO_IIDX_DIO13)
#define GPIO_Signal_13_PIN_0_PIN                                (DL_GPIO_PIN_13)
#define GPIO_Signal_13_PIN_0_IOMUX                               (IOMUX_PINCM35)
/* Port definition for Pin Group GPIO_Signal_15 */
#define GPIO_Signal_15_PORT                                              (GPIOA)

/* Defines for PIN_1: GPIOA.15 with pinCMx 37 on package pin 30 */
#define GPIO_Signal_15_PIN_1_IIDX                           (DL_GPIO_IIDX_DIO15)
#define GPIO_Signal_15_PIN_1_PIN                                (DL_GPIO_PIN_15)
#define GPIO_Signal_15_PIN_1_IOMUX                               (IOMUX_PINCM37)
/* Port definition for Pin Group GPIO_Signal_16 */
#define GPIO_Signal_16_PORT                                              (GPIOA)

/* Defines for PIN_2: GPIOA.16 with pinCMx 38 on package pin 31 */
#define GPIO_Signal_16_PIN_2_IIDX                           (DL_GPIO_IIDX_DIO16)
#define GPIO_Signal_16_PIN_2_PIN                                (DL_GPIO_PIN_16)
#define GPIO_Signal_16_PIN_2_IOMUX                               (IOMUX_PINCM38)
/* Port definition for Pin Group GPIO_Signal_17 */
#define GPIO_Signal_17_PORT                                              (GPIOA)

/* Defines for PIN_3: GPIOA.17 with pinCMx 39 on package pin 32 */
#define GPIO_Signal_17_PIN_3_IIDX                           (DL_GPIO_IIDX_DIO17)
#define GPIO_Signal_17_PIN_3_PIN                                (DL_GPIO_PIN_17)
#define GPIO_Signal_17_PIN_3_IOMUX                               (IOMUX_PINCM39)
/* Port definition for Pin Group GPIO_Signal_18 */
#define GPIO_Signal_18_PORT                                              (GPIOA)

/* Defines for PIN_4: GPIOA.18 with pinCMx 40 on package pin 33 */
#define GPIO_Signal_18_PIN_4_IIDX                           (DL_GPIO_IIDX_DIO18)
#define GPIO_Signal_18_PIN_4_PIN                                (DL_GPIO_PIN_18)
#define GPIO_Signal_18_PIN_4_IOMUX                               (IOMUX_PINCM40)
/* Port definition for Pin Group GPIO_Signal_12 */
#define GPIO_Signal_12_PORT                                              (GPIOA)

/* Defines for PIN_5: GPIOA.12 with pinCMx 34 on package pin 27 */
#define GPIO_Signal_12_PIN_5_IIDX                           (DL_GPIO_IIDX_DIO12)
#define GPIO_Signal_12_PIN_5_PIN                                (DL_GPIO_PIN_12)
#define GPIO_Signal_12_PIN_5_IOMUX                               (IOMUX_PINCM34)
/* Port definition for Pin Group GPIO_Signal_11 */
#define GPIO_Signal_11_PORT                                              (GPIOA)

/* Defines for PIN_6: GPIOA.11 with pinCMx 22 on package pin 19 */
#define GPIO_Signal_11_PIN_6_IIDX                           (DL_GPIO_IIDX_DIO11)
#define GPIO_Signal_11_PIN_6_PIN                                (DL_GPIO_PIN_11)
#define GPIO_Signal_11_PIN_6_IOMUX                               (IOMUX_PINCM22)
/* Port definition for Pin Group GPIO_Signal_10 */
#define GPIO_Signal_10_PORT                                              (GPIOA)

/* Defines for PIN_7: GPIOA.10 with pinCMx 21 on package pin 18 */
#define GPIO_Signal_10_PIN_7_IIDX                           (DL_GPIO_IIDX_DIO10)
#define GPIO_Signal_10_PIN_7_PIN                                (DL_GPIO_PIN_10)
#define GPIO_Signal_10_PIN_7_IOMUX                               (IOMUX_PINCM21)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART_0_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
