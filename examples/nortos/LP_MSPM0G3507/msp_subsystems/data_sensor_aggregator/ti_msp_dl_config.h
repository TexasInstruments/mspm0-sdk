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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3507
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0G3507
#define CONFIG_MSPM0G3507

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




/* Defines for I2C */
#define I2C_INST                                                            I2C1
#define I2C_INST_IRQHandler                                      I2C1_IRQHandler
#define I2C_INST_INT_IRQN                                          I2C1_INT_IRQn
#define I2C_BUS_SPEED_HZ                                                  100000
#define GPIO_I2C_SDA_PORT                                                  GPIOB
#define GPIO_I2C_SDA_PIN                                           DL_GPIO_PIN_3
#define GPIO_I2C_IOMUX_SDA                                       (IOMUX_PINCM16)
#define GPIO_I2C_IOMUX_SDA_FUNC                        IOMUX_PINCM16_PF_I2C1_SDA
#define GPIO_I2C_SCL_PORT                                                  GPIOB
#define GPIO_I2C_SCL_PIN                                           DL_GPIO_PIN_2
#define GPIO_I2C_IOMUX_SCL                                       (IOMUX_PINCM15)
#define GPIO_I2C_IOMUX_SCL_FUNC                        IOMUX_PINCM15_PF_I2C1_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_0_FBRD_32_MHZ_9600_BAUD                                        (21)





/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE_V                                       3.3
#define GPIO_ADC12_0_C2_PORT                                               GPIOA
#define GPIO_ADC12_0_C2_PIN                                       DL_GPIO_PIN_25



/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define UART_0_INST_DMA_TRIGGER                              (DMA_UART0_TX_TRIG)


/* Defines for HDC_V: GPIOB.24 with pinCMx 52 on package pin 23 */
#define GPIO_SENSORS_POWER_HDC_V_PORT                                    (GPIOB)
#define GPIO_SENSORS_POWER_HDC_V_PIN                            (DL_GPIO_PIN_24)
#define GPIO_SENSORS_POWER_HDC_V_IOMUX                           (IOMUX_PINCM52)
/* Defines for DRV_V: GPIOA.22 with pinCMx 47 on package pin 18 */
#define GPIO_SENSORS_POWER_DRV_V_PORT                                    (GPIOA)
#define GPIO_SENSORS_POWER_DRV_V_PIN                            (DL_GPIO_PIN_22)
#define GPIO_SENSORS_POWER_DRV_V_IOMUX                           (IOMUX_PINCM47)
/* Defines for OPT_V: GPIOA.24 with pinCMx 54 on package pin 25 */
#define GPIO_SENSORS_POWER_OPT_V_PORT                                    (GPIOA)
#define GPIO_SENSORS_POWER_OPT_V_PIN                            (DL_GPIO_PIN_24)
#define GPIO_SENSORS_POWER_OPT_V_IOMUX                           (IOMUX_PINCM54)
/* Defines for INT1: GPIOA.26 with pinCMx 59 on package pin 30 */
#define GPIO_SENSORS_POWER_INT1_PORT                                     (GPIOA)
#define GPIO_SENSORS_POWER_INT1_PIN                             (DL_GPIO_PIN_26)
#define GPIO_SENSORS_POWER_INT1_IOMUX                            (IOMUX_PINCM59)
/* Defines for INT2: GPIOB.6 with pinCMx 23 on package pin 58 */
#define GPIO_SENSORS_POWER_INT2_PORT                                     (GPIOB)
#define GPIO_SENSORS_POWER_INT2_PIN                              (DL_GPIO_PIN_6)
#define GPIO_SENSORS_POWER_INT2_IOMUX                            (IOMUX_PINCM23)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_I2C_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_ADC12_0_init(void);
void SYSCFG_DL_DMA_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
