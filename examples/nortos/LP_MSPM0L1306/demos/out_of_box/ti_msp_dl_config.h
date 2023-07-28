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

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)

/* GPIO configuration for S2 */
#define GPIO_SWITCHES_PORT                                              (GPIOA)
#define GPIO_SWITCHES_USER_SWITCH_1_PIN                        (DL_GPIO_PIN_14)
#define GPIO_SWITCHES_USER_SWITCH_1_IOMUX                       (IOMUX_PINCM15)

/* GPIO configuration for UART_INST */
#define GPIO_UART_TX_PORT                                               (GPIOA)
#define GPIO_UART_TX_PIN                                        (DL_GPIO_PIN_8)
#define GPIO_UART_IOMUX_TX                                       (IOMUX_PINCM9)
#define GPIO_UART_IOMUX_TX_FUNCTION                  (IOMUX_PINCM9_PF_UART0_TX)

#define GPIO_UART_RX_PORT                                               (GPIOA)
#define GPIO_UART_RX_PIN                                        (DL_GPIO_PIN_9)
#define GPIO_UART_IOMUX_RX                                      (IOMUX_PINCM10)
#define GPIO_UART_IOMUX_RX_FUNCTION                 (IOMUX_PINCM10_PF_UART0_RX)

/* GPIO defines for LED1 */
#define GPIO_LEDS_PORT                                                  (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_0)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM1)

/* GPIO configuration for RGB LED */
#define GPIO_RGB_PORT                                                   (GPIOA)
#define GPIO_RGB_RED_PIN                                       (DL_GPIO_PIN_26)
#define GPIO_IOMUX_RGB_RED                                      (IOMUX_PINCM27)
#define GPIO_IOMUX_FUNC_RED_PWM                   (IOMUX_PINCM27_PF_TIMG1_CCP0)

#define GPIO_RGB_GREEN_PIN                                     (DL_GPIO_PIN_13)
#define GPIO_IOMUX_RGB_GREEN                                    (IOMUX_PINCM14)
#define GPIO_IOMUX_FUNC_GREEN_PWM                 (IOMUX_PINCM14_PF_TIMG0_CCP1)

#define GPIO_RGB_BLUE_PIN                                      (DL_GPIO_PIN_27)
#define GPIO_IOMUX_RGB_BLUE                                     (IOMUX_PINCM28)
#define GPIO_IOMUX_FUNC_BLUE_PWM                  (IOMUX_PINCM28_PF_TIMG1_CCP1)

/* Definitions for SWITCHES_USER_SWITCH_1 */
#define SWITCHES_USER_SWITCH_1_IIDX                        (DL_GPIO_IIDX_DIO14)
#define SWITCHES_USER_SWITCH_1_INT_IRQN                        (GPIOA_INT_IRQn)

/* Definitions for UART_INST */
#define UART_INST                                                       (UART0)
#define UART_INST_IRQHandler                                 (UART0_IRQHandler)
#define UART_INST_NVIC_IRQn                                    (UART0_INT_IRQn)

#define UART_BAUD_RATE                                                  (115200)
#define UART_IBRD_32_MHZ_115200_BAUD                                        (17)
#define UART_FBRD_32_MHZ_115200_BAUD                                        (23)

/* Definitions for timer controlling blinking LED */
#define BLINK_LED_TIMER_INST                                            (TIMG4)
#define BLINK_LED_TIMER_INST_IRQHandler                        TIMG4_IRQHandler
#define BLINK_LED_TIMER_INST_INT_IRQN                          (TIMG4_INT_IRQn)

/* Definitions for peripherals used in light sensor */
#define RGB_LED_PERIOD                                                    (512)

#define LIGHTSENSOR_OPA_INST                                             (OPA0)

#define LIGHTSENSOR_TIMER_ADC_INST                                      (TIMG2)

#define LIGHTSENSOR_PWM_RED_BLUE_INST                                   (TIMG1)
#define LIGHTSENSOR_PWM_RED_BLUE_INST_IRQHandler               TIMG1_IRQHandler
#define LIGHTSENSOR_PWM_RED_BLUE_INST_INT_IRQN                 (TIMG1_INT_IRQn)

#define LIGHTSENSOR_PWM_GREEN_INST                                      (TIMG0)
#define LIGHTSENSOR_PWM_GREEN_INST_IRQHandler                  TIMG0_IRQHandler
#define LIGHTSENSOR_PWM_GREEN_INST_INT_IRQN                    (TIMG0_INT_IRQn)

/* Definitions for peripherals used in thermistor */
#define THERMISTOR_OPA_INST                                              (OPA0)
#define THERMISTOR_TIMER_ADC_INST                                       (TIMG0)

/* Definitions for peripherals used in thermistor and light sensor*/
#define ADC_INST                                                         (ADC0)
#define ADC_INST_IRQHandler                                     ADC0_IRQHandler
#define ADC_INST_INT_IRQN                                       (ADC0_INT_IRQn)

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

void lightsensor_init(void);
void lightsensor_initPower(void);
void lightsensor_SYSCTL_init(void);
void lightsensor_GPIO_init(void);
void lightsensor_PWM_RED_BLUE_init(void);
void lightsensor_PWN_GREEN_init(void);
void lightsensor_OPA_init(void);
void lightsensor_TIMER_ADC_init(void);
void lightsensor_ADC_init(void);

void thermistor_init(void);
void thermistor_initPower(void);
void thermistor_SYSCTL_init(void);
void thermistor_GPIO_init(void);
void thermistor_OPA_init(void);
void thermistor_TIMER_ADC_init(void);
void thermistor_ADC_init(void);

#endif /* ti_msp_dl_config_h */
