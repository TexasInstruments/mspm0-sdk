/*
 * Copyright (c) 2021, Texas Instruments Incorporated - http://www.ti.com
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
 *  ============ ti_drivers_config.h =============
 *  Configured MSPM0 Driver module declarations
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L222X
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_LP_MSPM0L2228

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <stdio.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSPM0.h>

#define CONFIG_UART_COUNT 1
#define CONFIG_UART_BUFFER_LENGTH 1

extern const uint_least8_t CONFIG_UART_0;
extern const uint_least8_t UART_count;

uint8_t rxBuffer[CONFIG_UART_BUFFER_LENGTH];
uint8_t txBuffer[CONFIG_UART_BUFFER_LENGTH];

/* clang-format on */
void UART0_IRQHandler(void);
/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */

/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


/* Defines for CONFIG_UART_0 */
#define CONFIG_UART_0_INST                                                 UART0
#define CONFIG_UART_BAUD_RATE                                           (115200)

/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_GRP_0_PIN_0_IOMUX                                    (IOMUX_PINCM1)
/* clang-format on */

#endif /* ti_drivers_config_h */
