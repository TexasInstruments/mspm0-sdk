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




/* Port definition for Pin Group OUTPUT */
#define OUTPUT_PORT                                                      (GPIOA)

/* Defines for LED: GPIOA.0 with pinCMx 1 on package pin 1 */
#define OUTPUT_LED_PIN                                           (DL_GPIO_PIN_0)
#define OUTPUT_LED_IOMUX                                          (IOMUX_PINCM1)
/* Port definition for Pin Group INPUT */
#define INPUT_PORT                                                       (GPIOA)

/* Defines for CHANNEL_0: GPIOA.3 with pinCMx 4 on package pin 7 */
// groups represented: ["SELECT","INPUT"]
// pins affected: ["S0","S1","CHANNEL_0","CHANNEL_1","CHANNEL_2","CHANNEL_3"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define INPUT_CHANNEL_0_IIDX                                 (DL_GPIO_IIDX_DIO3)
#define INPUT_CHANNEL_0_PIN                                      (DL_GPIO_PIN_3)
#define INPUT_CHANNEL_0_IOMUX                                     (IOMUX_PINCM4)
/* Defines for CHANNEL_1: GPIOA.7 with pinCMx 8 on package pin 11 */
#define INPUT_CHANNEL_1_IIDX                                 (DL_GPIO_IIDX_DIO7)
#define INPUT_CHANNEL_1_PIN                                      (DL_GPIO_PIN_7)
#define INPUT_CHANNEL_1_IOMUX                                     (IOMUX_PINCM8)
/* Defines for CHANNEL_2: GPIOA.4 with pinCMx 5 on package pin 8 */
#define INPUT_CHANNEL_2_IIDX                                 (DL_GPIO_IIDX_DIO4)
#define INPUT_CHANNEL_2_PIN                                      (DL_GPIO_PIN_4)
#define INPUT_CHANNEL_2_IOMUX                                     (IOMUX_PINCM5)
/* Defines for CHANNEL_3: GPIOA.23 with pinCMx 24 on package pin 27 */
#define INPUT_CHANNEL_3_IIDX                                (DL_GPIO_IIDX_DIO23)
#define INPUT_CHANNEL_3_PIN                                     (DL_GPIO_PIN_23)
#define INPUT_CHANNEL_3_IOMUX                                    (IOMUX_PINCM24)
/* Port definition for Pin Group SELECT */
#define SELECT_PORT                                                      (GPIOA)

/* Defines for S0: GPIOA.26 with pinCMx 27 on package pin 30 */
#define SELECT_S0_IIDX                                      (DL_GPIO_IIDX_DIO26)
#define SELECT_S0_PIN                                           (DL_GPIO_PIN_26)
#define SELECT_S0_IOMUX                                          (IOMUX_PINCM27)
/* Defines for S1: GPIOA.27 with pinCMx 28 on package pin 31 */
#define SELECT_S1_IIDX                                      (DL_GPIO_IIDX_DIO27)
#define SELECT_S1_PIN                                           (DL_GPIO_PIN_27)
#define SELECT_S1_IOMUX                                          (IOMUX_PINCM28)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
