/*
 * Copyright (c) 2022, Texas Instruments Incorporated
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
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0L222X

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

/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)

#define CPUCLK_FREQ                                                     32000000

/* GPIO configuration for S1 on the LP */
#define GPIO_SWITCHES1_PORT                                               (GPIOA)
#define GPIO_SWITCHES1_INT_IRQN                                  (GPIOA_INT_IRQn)
#define GPIO_SWITCHES1_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_SWITCHES1_USER_SWITCH_1_IIDX                    (DL_GPIO_IIDX_DIO18)
#define GPIO_SWITCHES1_USER_SWITCH_1_PIN                         (DL_GPIO_PIN_18)
#define GPIO_SWITCHES1_USER_SWITCH_1_IOMUX                        (IOMUX_PINCM50)

/* GPIO configuration for S3 on the LP */
#define GPIO_SWITCHES_PORT                                             (GPIOB)
#define GPIO_SWITCHES_USER_SWITCH_3_PIN                        (DL_GPIO_PIN_8)
#define GPIO_SWITCHES_INT_IIDX                (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_SWITCHES_USER_SWITCH_3_IIDX                   (DL_GPIO_IIDX_DIO8)
#define GPIO_SWITCHES_USER_SWITCH_3_IOMUX                      (IOMUX_PINCM29)

/* GPIO configuration for UART_INST */
#define GPIO_UART_TX_PORT                                               (GPIOA)
#define GPIO_UART_TX_PIN                                       (DL_GPIO_PIN_10)
#define GPIO_UART_IOMUX_TX                                      (IOMUX_PINCM25)
#define GPIO_UART_IOMUX_TX_FUNCTION                 (IOMUX_PINCM25_PF_UART0_TX)

#define GPIO_UART_RX_PORT                                               (GPIOA)
#define GPIO_UART_RX_PIN                                       (DL_GPIO_PIN_11)
#define GPIO_UART_IOMUX_RX                                      (IOMUX_PINCM26)
#define GPIO_UART_IOMUX_RX_FUNCTION                 (IOMUX_PINCM26_PF_UART0_RX)

/* GPIO defines for LEDS */
#define GPIO_LEDS_PORT                                                  (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_0)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM1)

/* GPIO configuration for RGB LED on Launchpad */
#define GPIO_RGB_PORT                                                   (GPIOB)
#define GPIO_RGB_RED_PIN                                       (DL_GPIO_PIN_10)
#define GPIO_IOMUX_RGB_RED                                      (IOMUX_PINCM31)

#define GPIO_RGB_GREEN_PIN                                     (DL_GPIO_PIN_9)
#define GPIO_IOMUX_RGB_GREEN                                    (IOMUX_PINCM30)

#define GPIO_RGB_BLUE_PIN                                      (DL_GPIO_PIN_23)
#define GPIO_IOMUX_RGB_BLUE                                     (IOMUX_PINCM67)

/* Definitions for SWITCHES1_USER_SWITCH_1 */
#define SWITCHES1_USER_SWITCH_1_IIDX                       (DL_GPIO_IIDX_DIO21)
#define SWITCHES1_USER_SWITCH_1_INT_IRQN                       (GPIOB_INT_IRQn)

/* Definitions for UART_INST */
#define UART_INST                                                       (UART0)
#define UART_INST_IRQHandler                                 (UART0_IRQHandler)
#define UART_INST_NVIC_IRQn                                    (UART0_INT_IRQn)

#define UART_BAUD                                                      (115200)

/* Definitions for timer controlling blinking LED */
#define BLINK_LED_TIMER_INST                                            (TIMA0)
#define BLINK_LED_TIMER_INST_IRQHandler                        TIMA0_IRQHandler
#define BLINK_LED_TIMER_INST_INT_IRQN                          (TIMA0_INT_IRQn)

/* Definitions for peripherals used in temperature sensor example */

#define TEMPERATURE_TIMER_ADC_INST                                      (TIMG0)

/* Definitions for ADC */
#define ADC12_0_INST                                                     (ADC0)
#define ADC12_0_INST_IRQHandler                                 ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                   (ADC0_INT_IRQn)

#define TEMPERATURE_TIMER_ADC_INST                                      (TIMG0)


/* Defines for RTC_A */
#define RTC_A_INST_IRQHandler                                    LFSS_IRQHandler
#define RTC_A_INST_INT_IRQN                                      (LFSS_INT_IRQn)

/* Defines for TIO */
#define TAMPERIO                                                          (LFSS)
#define TAMPERIO_INST_IRQHandler                                 LFSS_IRQHandler
#define TAMPERIO_INST_INT_IRQN                                   (LFSS_INT_IRQn)


/* Defines for LCD */
#define LCD_IRQHandler                                           LCD_IRQHandler
#define LCD_INT_IRQN                                               LCD_INT_IRQn

/* clang-format on */
void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_UART_init(void);
void SYSCFG_DL_GPIO_init(void);

void blink_led_init(void);
void blink_led_initPower(void);
void blink_led_SYSCTL_init(void);
void blink_led_GPIO_init(void);
void blink_led_TIMER_init(void);

void temperature_init(void);
void temperature_initPower(void);
void temperature_SYSCTL_init(void);
void temperature_GPIO_init(void);

void temperature_TIMER_ADC_init(void);
void temperature_ADC_init(void);

void tamper_init(void);
void tamper_initPower(void);
void tamper_RTC_A_init(void);
void tamper_TAMPERIO_init(void);

void LCD_init(void);
void VREF_init(void);
#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
