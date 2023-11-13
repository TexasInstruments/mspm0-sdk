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

#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

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


/*Defines for HFXT*/
#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)


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
#define MCAN0_INST_MCAN_STD_ID_FILTER_NUM          (2)
#define MCAN0_INST_MCAN_EXT_ID_FILT_START_ADDR     (0)
#define MCAN0_INST_MCAN_EXT_ID_FILTER_NUM          (0)
#define MCAN0_INST_MCAN_TX_BUFF_START_ADDR         (148)
#define MCAN0_INST_MCAN_TX_BUFF_SIZE               (10)
#define MCAN0_INST_MCAN_FIFO_1_START_ADDR          (368)
#define MCAN0_INST_MCAN_FIFO_1_NUM                 (5)
#define MCAN0_INST_MCAN_TX_EVENT_START_ADDR        (255)
#define MCAN0_INST_MCAN_TX_EVENT_SIZE              (10)
#define MCAN0_INST_MCAN_EXT_ID_AND_MASK            (0x1FFFFFFFU)
#define MCAN0_INST_MCAN_RX_BUFF_START_ADDR         (728)
#define MCAN0_INST_MCAN_FIFO_0_START_ADDR          (8)
#define MCAN0_INST_MCAN_FIFO_0_NUM                 (5)

#define MCAN0_INST_MCAN_INTERRUPTS (DL_MCAN_INTERRUPT_ARA | \
                        DL_MCAN_INTERRUPT_BEU | \
                        DL_MCAN_INTERRUPT_BO | \
                        DL_MCAN_INTERRUPT_DRX | \
                        DL_MCAN_INTERRUPT_ELO | \
                        DL_MCAN_INTERRUPT_EP | \
                        DL_MCAN_INTERRUPT_EW | \
                        DL_MCAN_INTERRUPT_MRAF | \
                        DL_MCAN_INTERRUPT_PEA | \
                        DL_MCAN_INTERRUPT_PED | \
                        DL_MCAN_INTERRUPT_RF0N | \
                        DL_MCAN_INTERRUPT_RF1N | \
                        DL_MCAN_INTERRUPT_TC | \
                        DL_MCAN_INTERRUPT_TEFN | \
                        DL_MCAN_INTERRUPT_TOO | \
                        DL_MCAN_INTERRUPT_TSW | \
                        DL_MCAN_INTERRUPT_WDI)




/* Port definition for Pin Group GPIO_Button */
#define GPIO_Button_PORT                                                 (GPIOB)

/* Defines for PIN_0: GPIOB.21 with pinCMx 49 on package pin 20 */
#define GPIO_Button_PIN_0_PIN                                   (DL_GPIO_PIN_21)
#define GPIO_Button_PIN_0_IOMUX                                  (IOMUX_PINCM49)
/* Port definition for Pin Group GPIO_BSL */
#define GPIO_BSL_PORT                                                    (GPIOB)

/* Defines for NRST: GPIOB.0 with pinCMx 12 on package pin 47 */
#define GPIO_BSL_NRST_PIN                                        (DL_GPIO_PIN_0)
#define GPIO_BSL_NRST_IOMUX                                      (IOMUX_PINCM12)
/* Defines for Invoke: GPIOB.16 with pinCMx 33 on package pin 4 */
#define GPIO_BSL_Invoke_PIN                                     (DL_GPIO_PIN_16)
#define GPIO_BSL_Invoke_IOMUX                                    (IOMUX_PINCM33)
/* Defines for PA0: GPIOA.0 with pinCMx 1 on package pin 33 */
#define GPIO_LED_PA0_PORT                                                (GPIOA)
#define GPIO_LED_PA0_PIN                                         (DL_GPIO_PIN_0)
#define GPIO_LED_PA0_IOMUX                                        (IOMUX_PINCM1)
/* Defines for Error: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_LED_Error_PORT                                              (GPIOB)
#define GPIO_LED_Error_PIN                                      (DL_GPIO_PIN_27)
#define GPIO_LED_Error_IOMUX                                     (IOMUX_PINCM58)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_SYSCTL_CLK_init(void);
void SYSCFG_DL_MCAN0_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
