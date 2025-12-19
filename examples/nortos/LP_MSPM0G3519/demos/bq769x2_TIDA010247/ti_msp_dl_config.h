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
 *  DO NOT EDIT - This file is generated for the LP_MSPM0G3519
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0G3519
#define CONFIG_MSPM0G3519

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
#define CPUCLK_FREQ                                                     80000000
/* Defines for SYSPLL_ERR_01 Workaround */
/* Represent 1.000 as 1000 */
#define FLOAT_TO_INT_SCALE                                               (1000U)
#define FCC_EXPECTED_RATIO                                                  2000
#define FCC_UPPER_BOUND                       (FCC_EXPECTED_RATIO * (1 + 0.003))
#define FCC_LOWER_BOUND                       (FCC_EXPECTED_RATIO * (1 - 0.003))

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);


/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA1
#define PWM_0_INST_IRQHandler                                   TIMA1_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA1_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                               625000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_28
#define GPIO_PWM_0_C0_IOMUX                                       (IOMUX_PINCM3)
#define GPIO_PWM_0_C0_IOMUX_FUNC                      IOMUX_PINCM3_PF_TIMA1_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_24
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM54)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM54_PF_TIMA1_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA0)
#define TIMER_0_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (3276U)




/* Defines for I2C_0 */
#define I2C_0_INST                                                          I2C0
#define I2C_0_INST_IRQHandler                                    I2C0_IRQHandler
#define I2C_0_INST_INT_IRQN                                        I2C0_INT_IRQn
#define I2C_0_BUS_SPEED_HZ                                                400000
#define GPIO_I2C_0_SDA_PORT                                                GPIOA
#define GPIO_I2C_0_SDA_PIN                                        DL_GPIO_PIN_10
#define GPIO_I2C_0_IOMUX_SDA                                     (IOMUX_PINCM21)
#define GPIO_I2C_0_IOMUX_SDA_FUNC                      IOMUX_PINCM21_PF_I2C0_SDA
#define GPIO_I2C_0_SCL_PORT                                                GPIOA
#define GPIO_I2C_0_SCL_PIN                                        DL_GPIO_PIN_11
#define GPIO_I2C_0_IOMUX_SCL                                     (IOMUX_PINCM22)
#define GPIO_I2C_0_IOMUX_SCL_FUNC                      IOMUX_PINCM22_PF_I2C0_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM1)
#define GPIO_UART_0_IOMUX_RX_FUNC                       IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                       IOMUX_PINCM1_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (256000)
#define UART_0_IBRD_40_MHZ_256000_BAUD                                       (9)
#define UART_0_FBRD_40_MHZ_256000_BAUD                                      (49)





/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC1
#define ADC12_0_INST_IRQHandler                                  ADC1_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC1_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                DL_ADC12_REFERENCE_VOLTAGE_VDDA_VSSA
#define GPIO_ADC12_0_C0_PORT                                               GPIOA
#define GPIO_ADC12_0_C0_PIN                                       DL_GPIO_PIN_15
#define GPIO_ADC12_0_IOMUX_C0                                    (IOMUX_PINCM37)
#define GPIO_ADC12_0_IOMUX_C0_FUNC                (IOMUX_PINCM37_PF_UNCONNECTED)



/* Port definition for Pin Group Isolator */
#define Isolator_PORT                                                    (GPIOB)

/* Defines for EN: GPIOB.19 with pinCMx 45 on package pin 75 */
#define Isolator_EN_PIN                                         (DL_GPIO_PIN_19)
#define Isolator_EN_IOMUX                                        (IOMUX_PINCM45)
/* Port definition for Pin Group PTC_MCU */
#define PTC_MCU_PORT                                                     (GPIOB)

/* Defines for En: GPIOB.17 with pinCMx 43 on package pin 73 */
#define PTC_MCU_En_PIN                                          (DL_GPIO_PIN_17)
#define PTC_MCU_En_IOMUX                                         (IOMUX_PINCM43)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for LED1_Blue: GPIOB.1 with pinCMx 13 on package pin 21 */
#define LED_LED1_Blue_PIN                                        (DL_GPIO_PIN_1)
#define LED_LED1_Blue_IOMUX                                      (IOMUX_PINCM13)
/* Defines for LED2_Red: GPIOB.2 with pinCMx 15 on package pin 23 */
#define LED_LED2_Red_PIN                                         (DL_GPIO_PIN_2)
#define LED_LED2_Red_IOMUX                                       (IOMUX_PINCM15)
/* Defines for LED3_Green: GPIOB.3 with pinCMx 16 on package pin 24 */
#define LED_LED3_Green_PIN                                       (DL_GPIO_PIN_3)
#define LED_LED3_Green_IOMUX                                     (IOMUX_PINCM16)
/* Port definition for Pin Group RS485 */
#define RS485_PORT                                                       (GPIOB)

/* Defines for TX_EN: GPIOB.25 with pinCMx 56 on package pin 95 */
#define RS485_TX_EN_PIN                                         (DL_GPIO_PIN_25)
#define RS485_TX_EN_IOMUX                                        (IOMUX_PINCM56)
/* Defines for RX_EN: GPIOB.26 with pinCMx 57 on package pin 96 */
#define RS485_RX_EN_PIN                                         (DL_GPIO_PIN_26)
#define RS485_RX_EN_IOMUX                                        (IOMUX_PINCM57)
/* Defines for IN_MCU: GPIOB.23 with pinCMx 51 on package pin 85 */
#define WAKE_IN_MCU_PORT                                                 (GPIOB)
// pins affected by this interrupt request:["IN_MCU"]
#define WAKE_INT_IRQN                                           (GPIOB_INT_IRQn)
#define WAKE_INT_IIDX                           (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define WAKE_IN_MCU_IIDX                                    (DL_GPIO_IIDX_DIO23)
#define WAKE_IN_MCU_PIN                                         (DL_GPIO_PIN_23)
#define WAKE_IN_MCU_IOMUX                                        (IOMUX_PINCM51)
/* Defines for OUT_MCU: GPIOB.24 with pinCMx 52 on package pin 86 */
#define WAKE_OUT_MCU_PORT                                                (GPIOB)
#define WAKE_OUT_MCU_PIN                                        (DL_GPIO_PIN_24)
#define WAKE_OUT_MCU_IOMUX                                       (IOMUX_PINCM52)
/* Defines for BOT_AFE_TS2: GPIOB.11 with pinCMx 28 on package pin 45 */
#define WAKE_BOT_AFE_TS2_PORT                                            (GPIOB)
#define WAKE_BOT_AFE_TS2_PIN                                    (DL_GPIO_PIN_11)
#define WAKE_BOT_AFE_TS2_IOMUX                                   (IOMUX_PINCM28)
/* Defines for TOP_AFE_TS2: GPIOA.9 with pinCMx 20 on package pin 28 */
#define WAKE_TOP_AFE_TS2_PORT                                            (GPIOA)
#define WAKE_TOP_AFE_TS2_PIN                                     (DL_GPIO_PIN_9)
#define WAKE_TOP_AFE_TS2_IOMUX                                   (IOMUX_PINCM20)
/* Defines for Enable: GPIOA.31 with pinCMx 6 on package pin 7 */
#define Power_Enable_PORT                                                (GPIOA)
#define Power_Enable_PIN                                        (DL_GPIO_PIN_31)
#define Power_Enable_IOMUX                                        (IOMUX_PINCM6)
/* Defines for UCC_EN1: GPIOB.20 with pinCMx 48 on package pin 82 */
#define Power_UCC_EN1_PORT                                               (GPIOB)
#define Power_UCC_EN1_PIN                                       (DL_GPIO_PIN_20)
#define Power_UCC_EN1_IOMUX                                      (IOMUX_PINCM48)
/* Defines for UCC_EN2: GPIOB.14 with pinCMx 31 on package pin 48 */
#define Power_UCC_EN2_PORT                                               (GPIOB)
#define Power_UCC_EN2_PIN                                       (DL_GPIO_PIN_14)
#define Power_UCC_EN2_IOMUX                                      (IOMUX_PINCM31)
/* Defines for BOT_AFE: GPIOA.18 with pinCMx 40 on package pin 70 */
#define Reset_BOT_AFE_PORT                                               (GPIOA)
#define Reset_BOT_AFE_PIN                                       (DL_GPIO_PIN_18)
#define Reset_BOT_AFE_IOMUX                                      (IOMUX_PINCM40)
/* Defines for TOP_AFE: GPIOB.13 with pinCMx 30 on package pin 47 */
#define Reset_TOP_AFE_PORT                                               (GPIOB)
#define Reset_TOP_AFE_PIN                                       (DL_GPIO_PIN_13)
#define Reset_TOP_AFE_IOMUX                                      (IOMUX_PINCM30)
/* Port definition for Pin Group ALERT */
#define ALERT_PORT                                                       (GPIOA)

/* Defines for BOT_ALERT: GPIOA.17 with pinCMx 39 on package pin 69 */
// pins affected by this interrupt request:["BOT_ALERT"]
#define ALERT_INT_IRQN                                          (GPIOA_INT_IRQn)
#define ALERT_INT_IIDX                          (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define ALERT_BOT_ALERT_IIDX                                (DL_GPIO_IIDX_DIO17)
#define ALERT_BOT_ALERT_PIN                                     (DL_GPIO_PIN_17)
#define ALERT_BOT_ALERT_IOMUX                                    (IOMUX_PINCM39)
/* Defines for TOP_ALERT: GPIOA.8 with pinCMx 19 on package pin 27 */
#define ALERT_TOP_ALERT_PIN                                      (DL_GPIO_PIN_8)
#define ALERT_TOP_ALERT_IOMUX                                    (IOMUX_PINCM19)
/* Port definition for Pin Group Temp_Inhibit_IN */
#define Temp_Inhibit_IN_PORT                                             (GPIOB)

/* Defines for TOP: GPIOB.12 with pinCMx 29 on package pin 46 */
#define Temp_Inhibit_IN_TOP_PIN                                 (DL_GPIO_PIN_12)
#define Temp_Inhibit_IN_TOP_IOMUX                                (IOMUX_PINCM29)
/* Defines for BOT: GPIOB.18 with pinCMx 44 on package pin 74 */
#define Temp_Inhibit_IN_BOT_PIN                                 (DL_GPIO_PIN_18)
#define Temp_Inhibit_IN_BOT_IOMUX                                (IOMUX_PINCM44)
/* Defines for DSG_Driver: GPIOB.4 with pinCMx 17 on package pin 25 */
#define FET_Control_DSG_Driver_PORT                                      (GPIOB)
#define FET_Control_DSG_Driver_PIN                               (DL_GPIO_PIN_4)
#define FET_Control_DSG_Driver_IOMUX                             (IOMUX_PINCM17)
/* Defines for BOTHOFF: GPIOA.30 with pinCMx 5 on package pin 5 */
#define FET_Control_BOTHOFF_PORT                                         (GPIOA)
#define FET_Control_BOTHOFF_PIN                                 (DL_GPIO_PIN_30)
#define FET_Control_BOTHOFF_IOMUX                                 (IOMUX_PINCM5)
/* Port definition for Pin Group Test_Port */
#define Test_Port_PORT                                                   (GPIOB)

/* Defines for T1: GPIOB.15 with pinCMx 32 on package pin 49 */
#define Test_Port_T1_PIN                                        (DL_GPIO_PIN_15)
#define Test_Port_T1_IOMUX                                       (IOMUX_PINCM32)
/* Defines for T2: GPIOB.16 with pinCMx 33 on package pin 50 */
#define Test_Port_T2_PIN                                        (DL_GPIO_PIN_16)
#define Test_Port_T2_IOMUX                                       (IOMUX_PINCM33)




/* Defines for MCAN0 */
#define MCAN0_INST                                                        CANFD0
#define GPIO_MCAN0_CAN_TX_PORT                                             GPIOA
#define GPIO_MCAN0_CAN_TX_PIN                                     DL_GPIO_PIN_26
#define GPIO_MCAN0_IOMUX_CAN_TX                                  (IOMUX_PINCM59)
#define GPIO_MCAN0_IOMUX_CAN_TX_FUNC               IOMUX_PINCM59_PF_CANFD0_CANTX
#define GPIO_MCAN0_CAN_RX_PORT                                             GPIOA
#define GPIO_MCAN0_CAN_RX_PIN                                     DL_GPIO_PIN_27
#define GPIO_MCAN0_IOMUX_CAN_RX                                  (IOMUX_PINCM60)
#define GPIO_MCAN0_IOMUX_CAN_RX_FUNC               IOMUX_PINCM60_PF_CANFD0_CANRX
#define MCAN0_INST_IRQHandler                                 CANFD0_IRQHandler
#define MCAN0_INST_INT_IRQN                                     CANFD0_INT_IRQn


/* Defines for MCAN0 MCAN RAM configuration */
#define MCAN0_INST_MCAN_STD_ID_FILT_START_ADDR     (0)
#define MCAN0_INST_MCAN_STD_ID_FILTER_NUM          (1)
#define MCAN0_INST_MCAN_EXT_ID_FILT_START_ADDR     (8)
#define MCAN0_INST_MCAN_EXT_ID_FILTER_NUM          (1)
#define MCAN0_INST_MCAN_TX_BUFF_START_ADDR         (48)
#define MCAN0_INST_MCAN_TX_BUFF_SIZE               (2)
#define MCAN0_INST_MCAN_FIFO_1_START_ADDR          (632)
#define MCAN0_INST_MCAN_FIFO_1_NUM                 (2)
#define MCAN0_INST_MCAN_TX_EVENT_START_ADDR        (332)
#define MCAN0_INST_MCAN_TX_EVENT_SIZE              (2)
#define MCAN0_INST_MCAN_EXT_ID_AND_MASK            (0x1FFFFFFFU)
#define MCAN0_INST_MCAN_RX_BUFF_START_ADDR         (780)
#define MCAN0_INST_MCAN_FIFO_0_START_ADDR          (340)
#define MCAN0_INST_MCAN_FIFO_0_NUM                 (4)

#define MCAN0_INST_MCAN_INTERRUPTS (DL_MCAN_INTERRUPT_ARA | \
						DL_MCAN_INTERRUPT_BEU | \
						DL_MCAN_INTERRUPT_BO | \
						DL_MCAN_INTERRUPT_ELO | \
						DL_MCAN_INTERRUPT_EP | \
						DL_MCAN_INTERRUPT_EW | \
						DL_MCAN_INTERRUPT_MRAF | \
						DL_MCAN_INTERRUPT_PEA | \
						DL_MCAN_INTERRUPT_PED | \
						DL_MCAN_INTERRUPT_RF0N | \
						DL_MCAN_INTERRUPT_TC | \
						DL_MCAN_INTERRUPT_TEFN | \
						DL_MCAN_INTERRUPT_TOO | \
						DL_MCAN_INTERRUPT_TSW | \
						DL_MCAN_INTERRUPT_WDI)
/* Defines for MCAN1 */
#define MCAN1_INST                                                        CANFD1
#define GPIO_MCAN1_CAN_TX_PORT                                             GPIOB
#define GPIO_MCAN1_CAN_TX_PIN                                     DL_GPIO_PIN_21
#define GPIO_MCAN1_IOMUX_CAN_TX                                  (IOMUX_PINCM49)
#define GPIO_MCAN1_IOMUX_CAN_TX_FUNC               IOMUX_PINCM49_PF_CANFD1_CANTX
#define GPIO_MCAN1_CAN_RX_PORT                                             GPIOB
#define GPIO_MCAN1_CAN_RX_PIN                                     DL_GPIO_PIN_22
#define GPIO_MCAN1_IOMUX_CAN_RX                                  (IOMUX_PINCM50)
#define GPIO_MCAN1_IOMUX_CAN_RX_FUNC               IOMUX_PINCM50_PF_CANFD1_CANRX
#define MCAN1_INST_IRQHandler                                 CANFD1_IRQHandler
#define MCAN1_INST_INT_IRQN                                     CANFD1_INT_IRQn


/* Defines for MCAN1 MCAN RAM configuration */
#define MCAN1_INST_MCAN_STD_ID_FILT_START_ADDR     (0)
#define MCAN1_INST_MCAN_STD_ID_FILTER_NUM          (1)
#define MCAN1_INST_MCAN_EXT_ID_FILT_START_ADDR     (8)
#define MCAN1_INST_MCAN_EXT_ID_FILTER_NUM          (1)
#define MCAN1_INST_MCAN_TX_BUFF_START_ADDR         (48)
#define MCAN1_INST_MCAN_TX_BUFF_SIZE               (2)
#define MCAN1_INST_MCAN_FIFO_1_START_ADDR          (632)
#define MCAN1_INST_MCAN_FIFO_1_NUM                 (2)
#define MCAN1_INST_MCAN_TX_EVENT_START_ADDR        (332)
#define MCAN1_INST_MCAN_TX_EVENT_SIZE              (2)
#define MCAN1_INST_MCAN_EXT_ID_AND_MASK            (0x1FFFFFFFU)
#define MCAN1_INST_MCAN_RX_BUFF_START_ADDR         (780)
#define MCAN1_INST_MCAN_FIFO_0_START_ADDR          (340)
#define MCAN1_INST_MCAN_FIFO_0_NUM                 (4)

#define MCAN1_INST_MCAN_INTERRUPTS (DL_MCAN_INTERRUPT_ARA | \
						DL_MCAN_INTERRUPT_BEU | \
						DL_MCAN_INTERRUPT_BO | \
						DL_MCAN_INTERRUPT_ELO | \
						DL_MCAN_INTERRUPT_EP | \
						DL_MCAN_INTERRUPT_EW | \
						DL_MCAN_INTERRUPT_MRAF | \
						DL_MCAN_INTERRUPT_PEA | \
						DL_MCAN_INTERRUPT_PED | \
						DL_MCAN_INTERRUPT_RF0N | \
						DL_MCAN_INTERRUPT_TC | \
						DL_MCAN_INTERRUPT_TEFN | \
						DL_MCAN_INTERRUPT_TOO | \
						DL_MCAN_INTERRUPT_TSW | \
						DL_MCAN_INTERRUPT_WDI)




/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_I2C_0_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_ADC12_0_init(void);

void SYSCFG_DL_SYSTICK_init(void);
void SYSCFG_DL_MCAN0_init(void);
void SYSCFG_DL_MCAN1_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
