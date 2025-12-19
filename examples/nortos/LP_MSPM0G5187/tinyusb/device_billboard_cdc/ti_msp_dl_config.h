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



#define CPUCLK_FREQ                                                     80000000
/* Defines for SYSPLL_ERR_01 Workaround */
/* Represent 1.000 as 1000 */
#define FLOAT_TO_INT_SCALE                                               (1000U)
#define FCC_EXPECTED_RATIO                                                  2500
#define FCC_UPPER_BOUND                       (FCC_EXPECTED_RATIO * (1 + 0.003))
#define FCC_LOWER_BOUND                       (FCC_EXPECTED_RATIO * (1 - 0.003))

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);


/* Defines for UART_0 */
#define UART_0_INST                                                          UC0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                    UC0_IRQHandler
#define UART_0_INST_INT_IRQN                                        UC0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM1)
#define GPIO_UART_0_IOMUX_RX_FUNC                         IOMUX_PINCM2_PF_UC0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                         IOMUX_PINCM1_PF_UC0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)





/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.24 with pinCMx 54 on package pin 25 */
#define GPIO_GRP_0_PIN_0_PIN                                    (DL_GPIO_PIN_24)
#define GPIO_GRP_0_PIN_0_IOMUX                                   (IOMUX_PINCM54)
/* Port definition for Pin Group GPIO_GRP_1 */
#define GPIO_GRP_1_PORT                                                  (GPIOA)

/* Defines for PIN_1: GPIOA.7 with pinCMx 14 on package pin 49 */
#define GPIO_GRP_1_PIN_1_PIN                                     (DL_GPIO_PIN_7)
#define GPIO_GRP_1_PIN_1_IOMUX                                   (IOMUX_PINCM14)




/* Defines for USB_0 */
#define USB_0_INST                                                        USBFS0

/* TinyUSB GPIO LED Defines */
#define GPIO_LED_TUSB                                            GPIO_GRP_0_PORT
#define GPIO_LED_PIN_TUSB                                   GPIO_GRP_0_PIN_0_PIN
#define GPIO_LED_IOMUX_TUSB                               GPIO_GRP_0_PIN_0_IOMUX
/* TinyUSB GPIO Button Defines */
#define GPIO_BUTTON_TUSB                                         GPIO_GRP_1_PORT
#define GPIO_BUTTON_PIN_TUSB                                GPIO_GRP_1_PIN_1_PIN
#define GPIO_BUTTON_IOMUX_TUSB                            GPIO_GRP_1_PIN_1_IOMUX
/* TinyUSB UART Logging defines */
#define UNICOMM_TUSB                                           UART_0_INST->inst
#define UART_TUSB                                              UART_0_INST->uart
#define GPIO_UART_TX_IO_FUNC_TUSB                      GPIO_UART_0_IOMUX_TX_FUNC
#define GPIO_UART_TX_IOMUX_TUSB                             GPIO_UART_0_IOMUX_TX
#define GPIO_UART_RX_IO_FUNC_TUSB                      GPIO_UART_0_IOMUX_RX_FUNC
#define GPIO_UART_RX_IOMUX_TUSB                             GPIO_UART_0_IOMUX_RX
/* TinyUSB HFCLK Defines */
#define GPIO_HFXIN_IOMUX_TUSB                                                  0
#define GPIO_HFXOUT_IOMUX_TUSB                                                 0

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);
void SYSCFG_DL_UART_0_init(void);

void SYSCFG_DL_SYSTICK_init(void);
void SYSCFG_DL_USB_0_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
