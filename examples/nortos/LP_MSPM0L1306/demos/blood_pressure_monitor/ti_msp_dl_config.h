/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

/* Defines for UART */
#define UART_INST                                                          UART0
#define UART_INST_IRQHandler                                    UART0_IRQHandler
#define UART_INST_INT_IRQN                                        UART0_INT_IRQn
#define GPIO_UART_RX_PORT                                                  GPIOA
#define GPIO_UART_TX_PORT                                                  GPIOA
#define GPIO_UART_RX_PIN                                           DL_GPIO_PIN_9
#define GPIO_UART_TX_PIN                                           DL_GPIO_PIN_8
#define GPIO_UART_IOMUX_RX                                       (IOMUX_PINCM10)
#define GPIO_UART_IOMUX_TX                                        (IOMUX_PINCM9)
#define GPIO_UART_IOMUX_RX_FUNC                        IOMUX_PINCM10_PF_UART0_RX
#define GPIO_UART_IOMUX_TX_FUNC                         IOMUX_PINCM9_PF_UART0_TX
#define UART_BAUD_RATE                                                  (115200)
#define UART_IBRD_32_MHZ_115200_BAUD                                        (17)
#define UART_FBRD_32_MHZ_115200_BAUD                                        (23)

#define SAMPLES_PER_SECOND                                                 (40)

/* Defines for GPIO motor and valve */
/* Port definition for Pin Group GPIO_PINS */
#define GPIO_PINS_PORT                                                   (GPIOA)
/* Defines for VALVE: GPIOA.15 with pinCMx 16 on package pin 19 */
#define GPIO_PINS_VALVE_PIN                                     (DL_GPIO_PIN_15)
#define GPIO_PINS_VALVE_IOMUX                                    (IOMUX_PINCM16)
/* Defines for MOTOR: GPIOA.14 with pinCMx 15 on package pin 18 */
#define GPIO_PINS_MOTOR_PIN                                     (DL_GPIO_PIN_14)
#define GPIO_PINS_MOTOR_IOMUX                                    (IOMUX_PINCM15)

/* Defines for ADC */
#define TIMER_ADC_INST                                                  (TIMG2)
#define ADC_INST                                                         (ADC0)
#define ADC_INST_IRQHandler                                     ADC0_IRQHandler
#define ADC_INST_INT_IRQN                                       (ADC0_INT_IRQn)

/* Defines used to open valve */
#define CAPTURE_1_INST                                                   (TIMG1)
#define CAPTURE_1_INST_IRQHandler                               TIMG1_IRQHandler
#define CAPTURE_1_INST_INT_IRQN                                 (TIMG1_INT_IRQn)
#define CAPTURE_1_INST_LOAD_VALUE                                       (19499U)

/* Defines for OPA_0 */
#define OPA_0_INST                                                          OPA0
#define GPIO_OPA_0_IN0POS_PORT                                             GPIOA
#define GPIO_OPA_0_IN0POS_PIN                                     DL_GPIO_PIN_25
#define GPIO_OPA_0_IOMUX_IN0POS                                  (IOMUX_PINCM26)
#define GPIO_OPA_0_IOMUX_IN0POS_FUNC                IOMUX_PINCM26_PF_UNCONNECTED
#define GPIO_OPA_0_IN0NEG_PORT                                             GPIOA
#define GPIO_OPA_0_IN0NEG_PIN                                     DL_GPIO_PIN_24
#define GPIO_OPA_0_IOMUX_IN0NEG                                  (IOMUX_PINCM25)
#define GPIO_OPA_0_IOMUX_IN0NEG_FUNC                IOMUX_PINCM25_PF_UNCONNECTED
#define GPIO_OPA_0_OUT_PORT                                                GPIOA
#define GPIO_OPA_0_OUT_PIN                                        DL_GPIO_PIN_22
#define GPIO_OPA_0_IOMUX_OUT                                     (IOMUX_PINCM23)
#define GPIO_OPA_0_IOMUX_OUT_FUNC                   IOMUX_PINCM23_PF_UNCONNECTED

/* Defines for OPA_1 */
#define OPA_1_INST                                                          OPA1
#define GPIO_OPA_1_IN0POS_PORT                                             GPIOA
#define GPIO_OPA_1_IN0POS_PIN                                     DL_GPIO_PIN_18
#define GPIO_OPA_1_IOMUX_IN0POS                                  (IOMUX_PINCM19)
#define GPIO_OPA_1_IOMUX_IN0POS_FUNC                IOMUX_PINCM19_PF_UNCONNECTED
#define GPIO_OPA_1_IN1NEG_PORT                                             GPIOA
#define GPIO_OPA_1_IN1NEG_PIN                                     DL_GPIO_PIN_17
#define GPIO_OPA_1_IOMUX_IN1NEG                                  (IOMUX_PINCM18)
#define GPIO_OPA_1_IOMUX_IN1NEG_FUNC                IOMUX_PINCM18_PF_UNCONNECTED
#define GPIO_OPA_1_OUT_PORT                                                GPIOA
#define GPIO_OPA_1_OUT_PIN                                        DL_GPIO_PIN_16
#define GPIO_OPA_1_IOMUX_OUT                                     (IOMUX_PINCM17)
#define GPIO_OPA_1_IOMUX_OUT_FUNC                   IOMUX_PINCM17_PF_UNCONNECTED

/* clang-format on */
void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_UART_init(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_OPA_0_init(void);
void SYSCFG_DL_OPA_1_init(void);
void SYSCFG_DL_CAPTURE_1_init(void);
void startADC(void);
void ADC_initPower(void);
void TIMER_ADC_init(void);
void ADC_init(void);

#endif /* ti_msp_dl_config_h */
