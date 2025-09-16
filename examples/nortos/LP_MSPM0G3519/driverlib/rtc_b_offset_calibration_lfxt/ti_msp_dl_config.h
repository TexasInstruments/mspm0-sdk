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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3519
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0G3519
#define CONFIG_MSPM0G3519

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




/* Defines for USER_SWITCH_1: GPIOB.3 with pinCMx 16 on package pin 24 */
#define GPIO_SWITCHES_USER_SWITCH_1_PORT                                 (GPIOB)
// pins affected by this interrupt request:["USER_SWITCH_1"]
#define GPIO_SWITCHES_GPIOB_INT_IRQN                            (GPIOB_INT_IRQn)
#define GPIO_SWITCHES_GPIOB_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_SWITCHES_USER_SWITCH_1_IIDX                     (DL_GPIO_IIDX_DIO3)
#define GPIO_SWITCHES_USER_SWITCH_1_PIN                          (DL_GPIO_PIN_3)
#define GPIO_SWITCHES_USER_SWITCH_1_IOMUX                        (IOMUX_PINCM16)
/* Defines for USER_SWITCH_2: GPIOA.18 with pinCMx 40 on package pin 70 */
#define GPIO_SWITCHES_USER_SWITCH_2_PORT                                 (GPIOA)
// pins affected by this interrupt request:["USER_SWITCH_2"]
#define GPIO_SWITCHES_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_SWITCHES_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_SWITCHES_USER_SWITCH_2_IIDX                    (DL_GPIO_IIDX_DIO18)
#define GPIO_SWITCHES_USER_SWITCH_2_PIN                         (DL_GPIO_PIN_18)
#define GPIO_SWITCHES_USER_SWITCH_2_IOMUX                        (IOMUX_PINCM40)


/* Defines for RTC_B */
#define GPIO_RTC_B_RTCOUT_PORT                                           (GPIOA)
#define GPIO_RTC_B_RTCOUT_PIN                                   (DL_GPIO_PIN_13)
#define GPIO_RTC_B_IOMUX_RTCOUT                                  (IOMUX_PINCM35)
#define GPIO_RTC_B_IOMUX_RTCOUT_FUNC             (IOMUX_PINCM35_PF_LFSS_RTC_OUT)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

void SYSCFG_DL_RTC_B_init(void);


#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
