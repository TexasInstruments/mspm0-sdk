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

/* GPIO configuration for UART_INST */
#define GPIO_UART_TX_PORT                                               (GPIOB)
#define GPIO_UART_TX_PIN                                        (DL_GPIO_PIN_6)
#define GPIO_UART_IOMUX_TX                                      (IOMUX_PINCM17)
#define GPIO_UART_IOMUX_TX_FUNCTION                 (IOMUX_PINCM17_PF_UART0_TX)

#define GPIO_UART_RX_PORT                                               (GPIOB)
#define GPIO_UART_RX_PIN                                        (DL_GPIO_PIN_7)
#define GPIO_UART_IOMUX_RX                                      (IOMUX_PINCM18)
#define GPIO_UART_IOMUX_RX_FUNCTION                 (IOMUX_PINCM18_PF_UART0_RX)

/* GPIO defines for LED1 (PA0)*/
#define GPIO_LEDS_PORT                                                  (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_0)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM1)

/* GPIO defines for LED_GREEN (PA30)*/
#define GPIO_LEDS_USER_LED_GREEN_PIN                           (DL_GPIO_PIN_30)
#define GPIO_LEDS_USER_LED_GREEN_IOMUX                          (IOMUX_PINCM45)

/* Definitions for SWITCHES_USER_SWITCH_1 */
#define SWITCHES_USER_SWITCH_1_IIDX                        (DL_GPIO_IIDX_DIO14)
#define SWITCHES_USER_SWITCH_1_INT_IRQN                        (GPIOA_INT_IRQn)
#define SWITCHES_USER_SWITCH_1_PIN                             (DL_GPIO_PIN_14)
#define SWITCHES_USER_SWITCH_1_IOMUX                            (IOMUX_PINCM26)

/* Definitions for UART_INST */
#define UART_INST                                                       (UART0)
#define UART_INST_IRQHandler                                 (UART0_IRQHandler)
#define UART_INST_NVIC_IRQn                                    (UART0_INT_IRQn)

#define UART_BAUD_RATE                                                 (115200)
#define UART_IBRD_32_MHZ_115200_BAUD                                       (17)
#define UART_FBRD_32_MHZ_115200_BAUD                                       (23)

/* Definitions for timer controlling blinking LED */
#define BLINK_LED_TIMER_INST                                            (TIMG8)
#define BLINK_LED_TIMER_INST_IRQHandler                        TIMG8_IRQHandler
#define BLINK_LED_TIMER_INST_INT_IRQN                          (TIMG8_INT_IRQn)

/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
//#define ADC12_0_ADCMEM_0_REF_VOLTAGE_V                                       3.3

/* clang-format on */
void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_UART_init(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_ADC12_0_init(void);

void blink_led_init(void);
void blink_led_initPower(void);
void blink_led_SYSCTL_init(void);
void blink_led_GPIO_init(void);
void blink_led_TIMER_init(void);

void temp_sensor_init(void);
void temp_sensor_initPower(void);

#endif /* ti_msp_dl_config_h */
