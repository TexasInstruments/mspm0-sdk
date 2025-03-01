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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0L2228
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0L2228
#define CONFIG_MSPM0L2228

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



/* Defines for PWM */
#define PWM_INST                                                           TIMG4
#define PWM_INST_IRQHandler                                     TIMG4_IRQHandler
#define PWM_INST_INT_IRQN                                       (TIMG4_INT_IRQn)
#define PWM_INST_CLK_FREQ                                                1000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_C0_PORT                                                   GPIOA
#define GPIO_PWM_C0_PIN                                           DL_GPIO_PIN_21
#define GPIO_PWM_C0_IOMUX                                        (IOMUX_PINCM56)
#define GPIO_PWM_C0_IOMUX_FUNC                       IOMUX_PINCM56_PF_TIMG4_CCP0
#define GPIO_PWM_C0_IDX                                      DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_C1_PORT                                                   GPIOA
#define GPIO_PWM_C1_PIN                                           DL_GPIO_PIN_22
#define GPIO_PWM_C1_IOMUX                                        (IOMUX_PINCM57)
#define GPIO_PWM_C1_IOMUX_FUNC                       IOMUX_PINCM57_PF_TIMG4_CCP1
#define GPIO_PWM_C1_IDX                                      DL_TIMER_CC_1_INDEX



/* Defines for TIMER */
#define TIMER_INST                                                       (TIMA0)
#define TIMER_INST_IRQHandler                                   TIMA0_IRQHandler
#define TIMER_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define TIMER_INST_LOAD_VALUE                                            (4999U)




/* Defines for USER_LED_1: GPIOA.23 with pinCMx 67 on package pin 72 */
#define GPIO_LEDS_USER_LED_1_PORT                                        (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_23)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM67)
/* Defines for USER_LED_2: GPIOB.9 with pinCMx 30 on package pin 33 */
#define GPIO_LEDS_USER_LED_2_PORT                                        (GPIOB)
#define GPIO_LEDS_USER_LED_2_PIN                                 (DL_GPIO_PIN_9)
#define GPIO_LEDS_USER_LED_2_IOMUX                               (IOMUX_PINCM30)
/* Defines for USER_LED_3: GPIOB.10 with pinCMx 31 on package pin 34 */
#define GPIO_LEDS_USER_LED_3_PORT                                        (GPIOB)
#define GPIO_LEDS_USER_LED_3_PIN                                (DL_GPIO_PIN_10)
#define GPIO_LEDS_USER_LED_3_IOMUX                               (IOMUX_PINCM31)
/* Defines for USER_SWITCH_1: GPIOA.18 with pinCMx 50 on package pin 55 */
#define GPIO_SWITCHES_USER_SWITCH_1_PORT                                 (GPIOA)
// pins affected by this interrupt request:["USER_SWITCH_1"]
#define GPIO_SWITCHES_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_SWITCHES_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_SWITCHES_USER_SWITCH_1_IIDX                    (DL_GPIO_IIDX_DIO18)
#define GPIO_SWITCHES_USER_SWITCH_1_PIN                         (DL_GPIO_PIN_18)
#define GPIO_SWITCHES_USER_SWITCH_1_IOMUX                        (IOMUX_PINCM50)
/* Defines for USER_SWITCH_2: GPIOB.8 with pinCMx 29 on package pin 32 */
#define GPIO_SWITCHES_USER_SWITCH_2_PORT                                 (GPIOB)
// pins affected by this interrupt request:["USER_SWITCH_2"]
#define GPIO_SWITCHES_GPIOB_INT_IRQN                            (GPIOB_INT_IRQn)
#define GPIO_SWITCHES_GPIOB_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_SWITCHES_USER_SWITCH_2_IIDX                     (DL_GPIO_IIDX_DIO8)
#define GPIO_SWITCHES_USER_SWITCH_2_PIN                          (DL_GPIO_PIN_8)
#define GPIO_SWITCHES_USER_SWITCH_2_IOMUX                        (IOMUX_PINCM29)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_init(void);
void SYSCFG_DL_TIMER_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
