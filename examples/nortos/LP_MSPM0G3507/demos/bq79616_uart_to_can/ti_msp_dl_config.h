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


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     32000000



/* Defines for UART_0 */
#define UART_0_INST                                                        UART1
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART1_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_9
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_8
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM20)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM19)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM20_PF_UART1_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM19_PF_UART1_TX
#define UART_0_BAUD_RATE                                               (1000000)
#define UART_0_IBRD_32_MHZ_1000000_BAUD                                      (2)
#define UART_0_FBRD_32_MHZ_1000000_BAUD                                      (0)





/* Port definition for Pin Group GPIO_SWITCHES */
#define GPIO_SWITCHES_PORT                                               (GPIOB)

/* Defines for USER_SWITCH_1: GPIOB.21 with pinCMx 49 on package pin 20 */
// pins affected by this interrupt request:["USER_SWITCH_1"]
#define GPIO_SWITCHES_INT_IRQN                                  (GPIOB_INT_IRQn)
#define GPIO_SWITCHES_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_SWITCHES_USER_SWITCH_1_IIDX                    (DL_GPIO_IIDX_DIO21)
#define GPIO_SWITCHES_USER_SWITCH_1_PIN                         (DL_GPIO_PIN_21)
#define GPIO_SWITCHES_USER_SWITCH_1_IOMUX                        (IOMUX_PINCM49)
/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOB)

/* Defines for PIN_0: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_GRP_0_PIN_0_IOMUX                                   (IOMUX_PINCM12)


/* Defines for MCAN0 */
#define MCAN0_INST                                                        CANFD0
#define GPIO_MCAN0_CAN_TX_PORT                                             GPIOA
#define GPIO_MCAN0_CAN_TX_PIN                                     DL_GPIO_PIN_12
#define GPIO_MCAN0_IOMUX_CAN_TX                                  (IOMUX_PINCM34)
#define GPIO_MCAN0_IOMUX_CAN_TX_FUNC               IOMUX_PINCM34_PF_CANFD0_CANTX
#define GPIO_MCAN0_CAN_RX_PORT                                             GPIOA
#define GPIO_MCAN0_CAN_RX_PIN                                     DL_GPIO_PIN_13
#define GPIO_MCAN0_IOMUX_CAN_RX                                  (IOMUX_PINCM35)
#define GPIO_MCAN0_IOMUX_CAN_RX_FUNC               IOMUX_PINCM35_PF_CANFD0_CANRX
#define MCAN0_INST_IRQHandler                                 CANFD0_IRQHandler
#define MCAN0_INST_INT_IRQN                                     CANFD0_INT_IRQn


/* Defines for MCAN0 MCAN RAM configuration */
#define MCAN0_INST_MCAN_STD_ID_FILT_START_ADDR     (0)
#define MCAN0_INST_MCAN_STD_ID_FILTER_NUM          (1)
#define MCAN0_INST_MCAN_EXT_ID_FILT_START_ADDR     (8)
#define MCAN0_INST_MCAN_EXT_ID_FILTER_NUM          (0)
#define MCAN0_INST_MCAN_TX_BUFF_START_ADDR         (12)
#define MCAN0_INST_MCAN_TX_BUFF_SIZE               (2)
#define MCAN0_INST_MCAN_FIFO_1_START_ADDR          (804)
#define MCAN0_INST_MCAN_FIFO_1_NUM                 (1)
#define MCAN0_INST_MCAN_TX_EVENT_START_ADDR        (452)
#define MCAN0_INST_MCAN_TX_EVENT_SIZE              (2)
#define MCAN0_INST_MCAN_EXT_ID_AND_MASK            (0x1FFFFFFFU)
#define MCAN0_INST_MCAN_RX_BUFF_START_ADDR         (880)
#define MCAN0_INST_MCAN_FIFO_0_START_ADDR          (512)
#define MCAN0_INST_MCAN_FIFO_0_NUM                 (4)

#define MCAN0_INST_MCAN_INTERRUPTS (DL_MCAN_INTERRUPT_TC)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_SYSCTL_CLK_init(void);
void SYSCFG_DL_UART_0_init(void);

void SYSCFG_DL_MCAN0_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
