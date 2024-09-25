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
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_LP_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_gpio.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/dma/DMAMSPM0.h>
#include <ti/drivers/uart/UARTMSPM0.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif

#define I2C_CLOCK_MHZ 32

/*
 *  ======== GPIO ========
 */

extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0 0
#define CONFIG_GPIO_LED_0_IOMUX (IOMUX_PINCM1)

/* Enable to the below code to configure power control of TMP117 */
#ifdef CONFIG_GPIO_TMP_EN
extern const uint_least8_t CONFIG_GPIO_TMP_EN_CONST;
#define CONFIG_GPIO_TMP_EN 27
#define CONFIG_GPIO_TMP_EN_IOMUX (IOMUX_PINCM60)
#endif

/* The range of pins available on this device */
extern const uint_least8_t GPIO_pinLowerBound;
extern const uint_least8_t GPIO_pinUpperBound;

/* LEDs are active low */
#define CONFIG_GPIO_LED_ON (0)
#define CONFIG_GPIO_LED_OFF (1)

#define CONFIG_LED_ON (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)

/*
 *  ======== I2C ========
 */

/*
 *  SCL: P01
 *  SDA: P02
 *  LaunchPad I2C
 */
extern const uint_least8_t CONFIG_I2C_TMP_CONST;
#define CONFIG_I2C_TMP 0
#define CONFIG_TI_DRIVERS_I2C_COUNT 1

/* ======== I2C Addresses and Speeds ======== */
#include <ti/drivers/I2C.h>

/* ---- CONFIG_I2C_TMP I2C bus components ---- */

/* BMA222E address and max speed */
#define CONFIG_I2C_TMP_BMA222E_ADDR (0x18)
#define CONFIG_I2C_TMP_BMA222E_MAXSPEED (400U) /* kbps */

/* TMP006 address and max speed */
#define CONFIG_I2C_TMP_TMP006_ADDR (0x41)
#define CONFIG_I2C_TMP_TMP006_MAXSPEED (3400U) /* kbps */

/* BP-BASSENSORSMKII/TMP117 address and max speed */
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_TMP117_ADDR (0x48)
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_TMP117_MAXSPEED (400U) /* kbps */

/* BP-BASSENSORSMKII/OPT3001 address and max speed */
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_OPT3001_ADDR (0x44)
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_OPT3001_MAXSPEED (2600U) /* kbps */

/* BP-BASSENSORSMKII/HDC2080 address and max speed */
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_HDC2080_ADDR (0x40)
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_HDC2080_MAXSPEED (400U) /* kbps */

/* BP-BASSENSORSMKII/BMI160_BMM150 address and max speed */
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_BMI160_BMM150_ADDR (0x69)
#define CONFIG_I2C_TMP_BP_BASSENSORSMKII_BMI160_BMM150_MAXSPEED \
    (1000U) /* kbps */

/* CONFIG_I2C_TMP max speed (supported by all components) */
#define CONFIG_I2C_TMP_MAXSPEED (400U) /* kbps */
#define CONFIG_I2C_TMP_MAXBITRATE ((I2C_BitRate) I2C_400kHz)

/* Defines for I2C */
#define I2C_INST I2C1
#define I2C_INST_IRQHandler I2C1_IRQHandler
#define I2C_INST_INT_IRQN I2C1_INT_IRQn
#define I2C_BUS_SPEED_HZ 400000

#define GPIO_I2C_SDA_PIN (35)
#define GPIO_I2C_IOMUX_SDA (IOMUX_PINCM16)
#define GPIO_I2C_IOMUX_SDA_FUNC IOMUX_PINCM16_PF_I2C1_SDA

#define GPIO_I2C_SCL_PIN (34)
#define GPIO_I2C_IOMUX_SCL (IOMUX_PINCM15)
#define GPIO_I2C_IOMUX_SCL_FUNC IOMUX_PINCM15_PF_I2C1_SCL

/*
 *  ======== UART ========
 */

/*
 *  TX: P55
 *  RX: P57
 *  XDS110 UART
 */
extern const uint_least8_t CONFIG_UART_0_CONST;
//#define CONFIG_UART_0                      0
#define CONFIG_TI_DRIVERS_UART_COUNT 1
/*********************************************************/
#define CONFIG_UART_COUNT 1
#define CONFIG_UART_BUFFER_LENGTH 1

#define CONFIG_DMA_COUNT 1
#define CONFIG_DMA_CH_COUNT 1
#define DEFAULT_DMA_PRIORITY 31

extern const uint_least8_t CONFIG_UART_0;
extern const uint_least8_t UART_count;

uint8_t rxBuffer[CONFIG_UART_BUFFER_LENGTH];
uint8_t txBuffer[CONFIG_UART_BUFFER_LENGTH];

/* clang-format on */
void UART0_IRQHandler(void);
/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */

/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


/* Defines for CONFIG_UART_0 */
#define CONFIG_UART_0_INST                                                 UART0
#define CONFIG_UART_BAUD_RATE                                           (115200)

/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_GRP_0_PIN_0_IOMUX                                    (IOMUX_PINCM1)
/* clang-format on */

/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif
#endif /* ti_drivers_config_h */
