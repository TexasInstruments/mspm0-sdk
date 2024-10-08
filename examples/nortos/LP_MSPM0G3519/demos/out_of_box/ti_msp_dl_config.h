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
#define CPUCLK_FREQ                                                     32000000

/* GPIO configuration for S1 on the LP */
#define GPIO_SWITCHES1_PORT                                               (GPIOA)
#define GPIO_SWITCHES1_INT_IRQN                                  (GPIOA_INT_IRQn)
#define GPIO_SWITCHES1_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_SWITCHES1_USER_SWITCH_1_IIDX                    (DL_GPIO_IIDX_DIO18)
#define GPIO_SWITCHES1_USER_SWITCH_1_PIN                         (DL_GPIO_PIN_18)
#define GPIO_SWITCHES1_USER_SWITCH_1_IOMUX                        (IOMUX_PINCM40)

/* GPIO configuration for S2 on the LP */
#define GPIO_SWITCHES_PORT                                             (GPIOB)
#define GPIO_SWITCHES_USER_SWITCH_2_PIN                       (DL_GPIO_PIN_3)
#define GPIO_SWITCHES_USER_SWITCH_2_IOMUX                      (IOMUX_PINCM16)
#define GPIO_SWITCHES_USER_SWITCH_2_IIDX                       (DL_GPIO_IIDX_DIO3)
#define GPIO_SWITCHES_USER_SWITCH_2_INT_IRQN                       (GPIOB_INT_IRQn)
#define GPIO_SWITCHES_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
/* GPIO configuration for UART_INST */
#define GPIO_UART_TX_PORT                                               (GPIOA)
#define GPIO_UART_TX_PIN                                       (DL_GPIO_PIN_10)
#define GPIO_UART_IOMUX_TX                                      (IOMUX_PINCM21)
#define GPIO_UART_IOMUX_TX_FUNCTION                 (IOMUX_PINCM21_PF_UART0_TX)

#define GPIO_UART_RX_PORT                                               (GPIOA)
#define GPIO_UART_RX_PIN                                       (DL_GPIO_PIN_11)
#define GPIO_UART_IOMUX_RX                                      (IOMUX_PINCM22)
#define GPIO_UART_IOMUX_RX_FUNCTION                 (IOMUX_PINCM22_PF_UART0_RX)

/* GPIO defines for LEDS */
#define GPIO_LEDS_PORT                                                  (GPIOA)
#define GPIO_LEDS_USER_LED_1_PIN                                (DL_GPIO_PIN_0)
#define GPIO_LEDS_USER_LED_1_IOMUX                               (IOMUX_PINCM1)

/* GPIO configuration for RGB LED on Launchpad */
#define GPIO_RGB_PORT                                                   (GPIOB)
#define GPIO_RGB_RED_PIN                                       (DL_GPIO_PIN_26)
#define GPIO_IOMUX_RGB_RED                                      (IOMUX_PINCM57)
#define GPIO_IOMUX_FUNC_RED_PWM                   (IOMUX_PINCM57_PF_TIMA1_CCP0)

#define GPIO_RGB_GREEN_PIN                                     (DL_GPIO_PIN_27)
#define GPIO_IOMUX_RGB_GREEN                                    (IOMUX_PINCM58)
#define GPIO_IOMUX_FUNC_GREEN_PWM                 (IOMUX_PINCM58_PF_TIMA1_CCP1)

#define GPIO_RGB_BLUE_PIN                                      (DL_GPIO_PIN_22)
#define GPIO_IOMUX_RGB_BLUE                                     (IOMUX_PINCM50)
#define GPIO_IOMUX_FUNC_BLUE_PWM                  (IOMUX_PINCM50_PF_TIMG8_CCP1)



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


 /*start function generator defines*/
#define FNGEN_DAC_INST                                                    (DAC0)
#define FNGEN_DAC_INST_IRQHandler                                DAC0_IRQHandler
#define FNGEN_DAC_INST_INT_IRQN                                  (DAC0_INT_IRQn)
#define FNGEN_DAC_INST_DMA_TRIGGER                      (DMA_DAC0_EVT_BD_1_TRIG)
#define FNGEN_GPIO_DAC_OUT_PORT                                          (GPIOA)
#define FNGEN_GPIO_DAC_OUT_PIN                                  (DL_GPIO_PIN_15)
#define FNGEN_GPIO_DAC_IOMUX_OUT                                 (IOMUX_PINCM37)
#define FNGEN_GPIO_DAC_IOMUX_OUT_FUNC             (IOMUX_PINCM37_PF_UNCONNECTED)

#define FNGEN_DMA_CH0_SEL                                                   (0)
#define FNGEN_DMA_CH1_SEL                                                   (1)


/* GPIO configuration for VREF */
#define FNGEN_GPIO_VREF_VREFPOS_PORT                                     (GPIOA)
#define FNGEN_GPIO_VREF_VREFPOS_PIN                             (DL_GPIO_PIN_23)
#define FNGEN_GPIO_VREF_IOMUX_VREFPOS                            (IOMUX_PINCM53)
#define FNGEN_GPIO_VREF_IOMUX_VREFPOS_FUNC        (IOMUX_PINCM53_PF_UNCONNECTED)
#define FNGEN_GPIO_VREF_VREFNEG_PORT                                     (GPIOA)
#define FNGEN_GPIO_VREF_VREFNEG_PIN                             (DL_GPIO_PIN_21)
#define FNGEN_GPIO_VREF_IOMUX_VREFNEG                            (IOMUX_PINCM46)
#define FNGEN_GPIO_VREF_IOMUX_VREFNEG_FUNC        (IOMUX_PINCM46_PF_UNCONNECTED)
/* End VREF Config */

/* Definitions for ADC */
#define FNGEN_ADC_INST                                                   (ADC1)
#define FNGEN_ADC_INST_IRQHandler                               ADC1_IRQHandler
#define FNGEN_ADC_INST_INT_IRQN                                 (ADC1_INT_IRQn)

#define FNGEN_TIMER_ADC_INST                                            (TIMG0)
#define FNGEN_TIMER_DAC_INST                                            (TIMA0)

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
void VREF_init(void);
void functiongenerator_init(void);
void functiongenerator_initPower(void);
void functiongenerator_GPIO_init(void);
void functiongenerator_DMA_init(void);
void functiongenerator_DAC_init(void);
void functiongenerator_SYSCTL_init(void);
void functiongenerator_VREF_init(void);
void functiongenerator_OPA_initINV(void);
void functiongenerator_TIMER_ADC_init(void);
void functiongenerator_TIMER_DAC_init();
void functiongenerator_ADC_init(void);
void functiongenerator_setPeriod(int frequency);
#endif /* ti_msp_dl_config_h */
