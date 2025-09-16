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
#define CONFIG_MSPM0L1306

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



/* Defines for THERMISTOR_TIMER_ADC */
#define THERMISTOR_TIMER_ADC_INST                                        (TIMG0)
#define THERMISTOR_TIMER_ADC_INST_IRQHandler                        TIMG0_IRQHandler
#define THERMISTOR_TIMER_ADC_INST_INT_IRQN                        (TIMG0_INT_IRQn)
#define THERMISTOR_TIMER_ADC_INST_LOAD_VALUE                                 (8191U)
#define THERMISTOR_TIMER_ADC_INST_PUB_0_CH                                     (1)




/* Defines for ADC */
#define ADC_INST                                                            ADC0
#define ADC_INST_IRQHandler                                      ADC0_IRQHandler
#define ADC_INST_INT_IRQN                                        (ADC0_INT_IRQn)
#define ADC_ADCMEM_0                                          DL_ADC12_MEM_IDX_0
#define ADC_ADCMEM_0_REF                         DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC_ADCMEM_0_REF_VOLTAGE_V                                           3.3
#define ADC_INST_SUB_CH                                                      (1)
#define GPIO_ADC_C9_PORT                                                   GPIOA
#define GPIO_ADC_C9_PIN                                           DL_GPIO_PIN_15
#define GPIO_ADC_IOMUX_C9                                        (IOMUX_PINCM16)
#define GPIO_ADC_IOMUX_C9_FUNC                    (IOMUX_PINCM16_PF_UNCONNECTED)


/* Defines for THERMISTOR_OPA */
#define THERMISTOR_OPA_INST                                                 OPA0
#define GPIO_THERMISTOR_OPA_IN0POS_PORT                                    GPIOA
#define GPIO_THERMISTOR_OPA_IN0POS_PIN                            DL_GPIO_PIN_25
#define GPIO_THERMISTOR_OPA_IOMUX_IN0POS                         (IOMUX_PINCM26)
#define GPIO_THERMISTOR_OPA_IOMUX_IN0POS_FUNC       IOMUX_PINCM26_PF_UNCONNECTED
#define GPIO_THERMISTOR_OPA_IN0NEG_PORT                                    GPIOA
#define GPIO_THERMISTOR_OPA_IN0NEG_PIN                            DL_GPIO_PIN_24
#define GPIO_THERMISTOR_OPA_IOMUX_IN0NEG                         (IOMUX_PINCM25)
#define GPIO_THERMISTOR_OPA_IOMUX_IN0NEG_FUNC       IOMUX_PINCM25_PF_UNCONNECTED
#define GPIO_THERMISTOR_OPA_OUT_PORT                                       GPIOA
#define GPIO_THERMISTOR_OPA_OUT_PIN                               DL_GPIO_PIN_22
#define GPIO_THERMISTOR_OPA_IOMUX_OUT                            (IOMUX_PINCM23)
#define GPIO_THERMISTOR_OPA_IOMUX_OUT_FUNC          IOMUX_PINCM23_PF_UNCONNECTED



/* Port definition for Pin Group RGB */
#define RGB_PORT                                                         (GPIOA)

/* Defines for RED: GPIOA.26 with pinCMx 27 on package pin 30 */
#define RGB_RED_PIN                                             (DL_GPIO_PIN_26)
#define RGB_RED_IOMUX                                            (IOMUX_PINCM27)
/* Defines for BLUE: GPIOA.27 with pinCMx 28 on package pin 31 */
#define RGB_BLUE_PIN                                            (DL_GPIO_PIN_27)
#define RGB_BLUE_IOMUX                                           (IOMUX_PINCM28)
/* Defines for GREEN: GPIOA.13 with pinCMx 14 on package pin 17 */
#define RGB_GREEN_PIN                                           (DL_GPIO_PIN_13)
#define RGB_GREEN_IOMUX                                          (IOMUX_PINCM14)


/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_THERMISTOR_TIMER_ADC_init(void);
void SYSCFG_DL_ADC_init(void);
void SYSCFG_DL_THERMISTOR_OPA_init(void);



#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
