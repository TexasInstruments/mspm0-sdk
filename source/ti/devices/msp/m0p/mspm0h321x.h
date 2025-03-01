/*****************************************************************************

  Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/ 

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

   Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the   
   distribution.

   Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef ti_devices_msp_m0p_mspm0h321x__include
#define ti_devices_msp_m0p_mspm0h321x__include

/* Filename: mspm0h321x.h */
/* Revised: 2024-10-13 23:08:26*/

/* Use standard integer types with explicit width */
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup MSPM0H321X_Definitions MSPM0H321X Definitions
  This file defines all structures and symbols for MSPM0H321X
  @{
*/

/** @addtogroup MSPM0H321X_CMSIS Device CMSIS Definitions
  Configuration of the Processor and Core Peripherals
  @{
*/
/******************************************************************************
* Interrupt Definitions                                                       *
******************************************************************************/
typedef enum IRQn
{
  NonMaskableInt_IRQn         = -14,    /* 2  Non Maskable Interrupt */
  HardFault_IRQn              = -13,    /* 3  Hard Fault Interrupt */
  SVCall_IRQn                 = -5,     /* 11 SV Call Interrupt */
  PendSV_IRQn                 = -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn                = -1,     /* 15 System Tick Interrupt */
  SYSCTL_INT_IRQn             = 0,      /* 16 SYSCTL_INT Interrupt */
  DEBUGSS_INT_IRQn            = 1,      /* 17 DEBUGSS_INT Interrupt */
  TIMG8_INT_IRQn              = 2,      /* 18 TIMG8_INT Interrupt */
  UART1_INT_IRQn              = 3,      /* 19 UART1_INT Interrupt */
  ADC0_INT_IRQn               = 4,      /* 20 ADC0_INT Interrupt */
  UART2_INT_IRQn              = 8,      /* 24 UART2_INT Interrupt */
  SPI0_INT_IRQn               = 9,      /* 25 SPI0_INT Interrupt */
  GENSUB0_INT_IRQn            = 11,     /* 27 GENSUB0_INT Interrupt */
  GENSUB1_INT_IRQn            = 12,     /* 28 GENSUB1_INT Interrupt */
  UART0_INT_IRQn              = 15,     /* 31 UART0_INT Interrupt */
  TIMG14_INT_IRQn             = 16,     /* 32 TIMG14_INT Interrupt */
  TIMG2_INT_IRQn              = 17,     /* 33 TIMG2_INT Interrupt */
  TIMA0_INT_IRQn              = 18,     /* 34 TIMA0_INT Interrupt */
  TIMG1_INT_IRQn              = 19,     /* 35 TIMG1_INT Interrupt */
  GPIOA_INT_IRQn              = 22,     /* 38 GPIOA_INT Interrupt */
  GPIOB_INT_IRQn              = 23,     /* 39 GPIOB_INT Interrupt */
  I2C0_INT_IRQn               = 24,     /* 40 I2C0_INT Interrupt */
  I2C1_INT_IRQn               = 25,     /* 41 I2C1_INT Interrupt */
  FLASHCTL_INT_IRQn           = 27,     /* 43 FLASHCTL_INT Interrupt */
  WWDT0_INT_IRQn              = 29,     /* 45 WWDT0_INT Interrupt */
  LFSS_INT_IRQn               = 30,     /* 46 LFSS_INT Interrupt */
  RTC_B_INT_IRQn              = 30,     /* 46 RTC_B_INT Interrupt */
  DMA_INT_IRQn                = 31,     /* 47 DMA_INT Interrupt */
} IRQn_Type;

#define NonMaskableInt_VECn     2     /* Non Maskable Interrupt */
#define HardFault_VECn          3     /* Hard Fault Interrupt */
#define SVCall_VECn             11    /* SV Call Interrupt */
#define PendSV_VECn             14    /* Pend SV Interrupt */
#define SysTick_VECn            15    /* System Tick Interrupt */
#define SYSCTL_INT_VECn         16    /* SYSCTL_INT Interrupt */
#define DEBUGSS_INT_VECn        17    /* DEBUGSS_INT Interrupt */
#define TIMG8_INT_VECn          18    /* TIMG8_INT Interrupt */
#define UART1_INT_VECn          19    /* UART1_INT Interrupt */
#define ADC0_INT_VECn           20    /* ADC0_INT Interrupt */
#define UART2_INT_VECn          24    /* UART2_INT Interrupt */
#define SPI0_INT_VECn           25    /* SPI0_INT Interrupt */
#define GENSUB0_INT_VECn        27    /* GENSUB0_INT Interrupt */
#define GENSUB1_INT_VECn        28    /* GENSUB1_INT Interrupt */
#define UART0_INT_VECn          31    /* UART0_INT Interrupt */
#define TIMG14_INT_VECn         32    /* TIMG14_INT Interrupt */
#define TIMG2_INT_VECn          33    /* TIMG2_INT Interrupt */
#define TIMA0_INT_VECn          34    /* TIMA0_INT Interrupt */
#define TIMG1_INT_VECn          35    /* TIMG1_INT Interrupt */
#define GPIOA_INT_VECn          38    /* GPIOA_INT Interrupt */
#define GPIOB_INT_VECn          39    /* GPIOB_INT Interrupt */
#define I2C0_INT_VECn           40    /* I2C0_INT Interrupt */
#define I2C1_INT_VECn           41    /* I2C1_INT Interrupt */
#define FLASHCTL_INT_VECn       43    /* FLASHCTL_INT Interrupt */
#define WWDT0_INT_VECn          45    /* WWDT0_INT Interrupt */
#define LFSS_INT_VECn           46    /* LFSS_INT Interrupt */
#define RTC_B_INT_VECn          46    /* RTC_B_INT Interrupt */
#define DMA_INT_VECn            47    /* DMA_INT Interrupt */


/******************************************************************************
* Ignore unused variables                                                     *
******************************************************************************/

#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/******************************************************************************
* Processor and Core Peripheral Section                                       *
******************************************************************************/

#define __CM0PLUS_REV           0x0001U    /* Core revision r0p1 */
#define __MPU_PRESENT           0x0001U    /* MPU present */
#define __VTOR_PRESENT          0x0001U    /* VTOR present */
#define __NVIC_PRIO_BITS        0x0002U    /* Number of bits used for Priority Levels */
#define __Vendor_SysTickConfig  0x0000U    /* Set to 1 if different SysTick Config is used */

#include "core_cm0plus.h"                  /* Processor and core peripherals */

/******************************************************************************
* Peripheral headers                                                          *
******************************************************************************/
/** @addtogroup MSPM0H321X_Peripherals MSPM0H321X Peripherals
  MSPM0H321X Peripheral registers structures
  @{
*/

#include <ti/devices/msp/peripherals/hw_adc12.h>
#include <ti/devices/msp/peripherals/hw_crc.h>
#include <ti/devices/msp/peripherals/hw_dma.h>
#include <ti/devices/msp/peripherals/hw_flashctl.h>
#include <ti/devices/msp/peripherals/hw_gpio.h>
#include <ti/devices/msp/peripherals/hw_gptimer.h>
#include <ti/devices/msp/peripherals/hw_i2c.h>
#include <ti/devices/msp/peripherals/hw_iomux.h>
#include <ti/devices/msp/peripherals/hw_lfss.h>
#include <ti/devices/msp/peripherals/hw_rtc.h>
#include <ti/devices/msp/peripherals/hw_spi.h>
#include <ti/devices/msp/peripherals/hw_uart.h>
#include <ti/devices/msp/peripherals/hw_vref.h>
#include <ti/devices/msp/peripherals/hw_wuc.h>
#include <ti/devices/msp/peripherals/hw_wwdt.h>
#include <ti/devices/msp/peripherals/m0p/hw_factoryregion.h>
#include <ti/devices/msp/peripherals/m0p/hw_cpuss.h>
#include <ti/devices/msp/peripherals/m0p/hw_debugss.h>
#include <ti/devices/msp/peripherals/m0p/hw_sysctl.h>

#define __MSPM0_HAS_ADC12__
#define __MSPM0_HAS_CRC__
#define __MSPM0_HAS_GPIO__
#define __MSPM0_HAS_TIMER_A__
#define __MSPM0_HAS_TIMER_G__
#define __MSPM0_HAS_I2C__
#define __MSPM0_HAS_LFSS__
#define __MSPM0_HAS_SPI__
#define __MSPM0_HAS_UART_EXTD__
#define __MSPM0_HAS_UART_MAIN__
#define __MSPM0_HAS_VREF__
#define __MSPM0_HAS_WWDT__
#define __MSPM0_HAS_RTC_B__
#define __MSPM0_HAS_IWDT__
#define __MSPM0_HAS_ADC12_SH_CAP_DISCH__

/*@}*/ /* end of group MSPM0H321X_Peripherals */

/******************************************************************************
* Device and peripheral memory map                                            *
******************************************************************************/
/** @addtogroup MSPM0H321X_MemoryMap MSPM0H321X Memory Mapping
  @{
*/

#define TIMG14_BASE                    (0x40084000U)     /*!< Base address of module TIMG14 */
#define VREF_BASE                      (0x40030000U)     /*!< Base address of module VREF */
#define UART0_BASE                     (0x40108000U)     /*!< Base address of module UART0 */
#define GPIOA_BASE                     (0x400A0000U)     /*!< Base address of module GPIOA */
#define SYSCTL_BASE                    (0x400AF000U)     /*!< Base address of module SYSCTL */
#define CRC_BASE                       (0x40440000U)     /*!< Base address of module CRC */
#define IOMUX_BASE                     (0x40428000U)     /*!< Base address of module IOMUX */
#define TIMG8_BASE                     (0x40090000U)     /*!< Base address of module TIMG8 */
#define DMA_BASE                       (0x4042A000U)     /*!< Base address of module DMA */
#define I2C0_BASE                      (0x400F0000U)     /*!< Base address of module I2C0 */
#define DEBUGSS_BASE                   (0x400C7000U)     /*!< Base address of module DEBUGSS */
#define TIMA0_BASE                     (0x40860000U)     /*!< Base address of module TIMA0 */
#define CPUSS_BASE                     (0x40400000U)     /*!< Base address of module CPUSS */
#define ADC0_BASE                      (0x40004000U)     /*!< Base address of module ADC0 */
#define ADC0_PERIPHERALREGIONSVT_BASE  (0x4055A000U)     /*!< Base address of module ADC0_PERIPHERALREGIONSVT */
#define WUC_BASE                       (0x40424000U)     /*!< Base address of module WUC */
#define FACTORYREGION_BASE             (0x41C40000U)     /*!< Base address of module FACTORYREGION */
#define WWDT0_BASE                     (0x40080000U)     /*!< Base address of module WWDT0 */
#define SPI0_BASE                      (0x40468000U)     /*!< Base address of module SPI0 */
#define I2C1_BASE                      (0x400F2000U)     /*!< Base address of module I2C1 */
#define TIMG1_BASE                     (0x40086000U)     /*!< Base address of module TIMG1 */
#define TIMG2_BASE                     (0x40088000U)     /*!< Base address of module TIMG2 */
#define GPIOB_BASE                     (0x400A2000U)     /*!< Base address of module GPIOB */
#define UART1_BASE                     (0x40100000U)     /*!< Base address of module UART1 */
#define UART2_BASE                     (0x40102000U)     /*!< Base address of module UART2 */
#define FLASHCTL_BASE                  (0x400CD000U)     /*!< Base address of module FLASHCTL */
#define LFSS_BASE                      (0x40094000U)     /*!< Base address of module LFSS */
#define RTC_B_BASE                     (0x40094000U)     /*!< Base address of module RTC_B */


/*@}*/ /* end of group MSPM0H321X_MemoryMap */

/******************************************************************************
* Peripheral declarations                                                     *
******************************************************************************/
/** @addtogroup MSPM0H321X_PeripheralDecl MSPM0H321X Peripheral Declaration
  @{
*/

static GPTIMER_Regs                             * const TIMG14                          = ((GPTIMER_Regs *) TIMG14_BASE);
static VREF_Regs                                * const VREF                           = ((VREF_Regs *) VREF_BASE);
static UART_Regs                                * const UART0                          = ((UART_Regs *) UART0_BASE);
static GPIO_Regs                                * const GPIOA                          = ((GPIO_Regs *) GPIOA_BASE);
static SYSCTL_Regs                              * const SYSCTL                         = ((SYSCTL_Regs *) SYSCTL_BASE);
static CRC_Regs                                 * const CRC                            = ((CRC_Regs *) CRC_BASE);
static IOMUX_Regs                               * const IOMUX                          = ((IOMUX_Regs *) IOMUX_BASE);
static GPTIMER_Regs                             * const TIMG8                          = ((GPTIMER_Regs *) TIMG8_BASE);
static DMA_Regs                                 * const DMA                            = ((DMA_Regs *) DMA_BASE);
static I2C_Regs                                 * const I2C0                           = ((I2C_Regs *) I2C0_BASE);
static DEBUGSS_Regs                             * const DEBUGSS                        = ((DEBUGSS_Regs *) DEBUGSS_BASE);
static GPTIMER_Regs                             * const TIMA0                          = ((GPTIMER_Regs *) TIMA0_BASE);
static CPUSS_Regs                               * const CPUSS                          = ((CPUSS_Regs *) CPUSS_BASE);
static ADC12_Regs                               * const ADC0                           = ((ADC12_Regs *) ADC0_BASE);
static ADC12_PERIPHERALREGIONSVT_Regs           * const ADC0_PERIPHERALREGIONSVT       = ((ADC12_PERIPHERALREGIONSVT_Regs *) ADC0_PERIPHERALREGIONSVT_BASE);
static WUC_Regs                                 * const WUC                            = ((WUC_Regs *) WUC_BASE);
static FACTORYREGION_OPEN_Regs                  * const FACTORYREGION                  = ((FACTORYREGION_OPEN_Regs *) FACTORYREGION_BASE);
static WWDT_Regs                                * const WWDT0                          = ((WWDT_Regs *) WWDT0_BASE);
static SPI_Regs                                 * const SPI0                           = ((SPI_Regs *) SPI0_BASE);
static I2C_Regs                                 * const I2C1                           = ((I2C_Regs *) I2C1_BASE);
static GPTIMER_Regs                             * const TIMG1                          = ((GPTIMER_Regs *) TIMG1_BASE);
static GPTIMER_Regs                             * const TIMG2                          = ((GPTIMER_Regs *) TIMG2_BASE);
static GPIO_Regs                                * const GPIOB                          = ((GPIO_Regs *) GPIOB_BASE);
static UART_Regs                                * const UART1                          = ((UART_Regs *) UART1_BASE);
static UART_Regs                                * const UART2                          = ((UART_Regs *) UART2_BASE);
static FLASHCTL_Regs                            * const FLASHCTL                       = ((FLASHCTL_Regs *) FLASHCTL_BASE);
static LFSS_Regs                                * const LFSS                           = ((LFSS_Regs *) LFSS_BASE);
static RTC_Regs                                 * const RTC_B                          = ((RTC_Regs *) RTC_B_BASE);


/******************************************************************************
* SYS parameters                                                              *
******************************************************************************/

#define DMA_SYS_N_DMA_CHANNEL                         (3)       /* !< Number of DMA channels implemented in DMA. */
#define DMA_SYS_N_DMA_FULL_CHANNEL                    (2)       /* !< Number of FULL-DMA channels implemented in DMA. */
#define DMA_SYS_MMR_AUTO                              (1)       /* !< Boolean for if auto enable channels implemented in DMA. */
#define DMA_SYS_MMR_EM                                (1)       /* !< Boolean for if extended mode channels implemented in DMA. */
#define DMA_SYS_MMR_LLONG                             (1)       /* !< Boolean for if channels implemented in DMA with 128-bit access. */
#define DMA_SYS_MMR_STRIDE                            (1)       /* !< Boolean for if channels implemented in DMA with stride mode. */


#define CRC_SYS_CRC32_ENABLE                          (0)       /* !< Parameter to exclude or include 32-bit CRC. */
#define FLASHCTL_SYS_DATAWIDTH                        (128)     /* !< Data bit width of a single flash word. */
#define ADC_SYS_NUM_ANALOG_CHAN                       (32)      /* !< Number of analog channels. */
#define I2C_SYS_FENTRIES                              (4)       /* !< Number of FIFO entries */
#define FLASHCTL_SYS_WEPROTAWIDTH                     (0)       /* !< Bit width of WEPROTA register */
#define FLASHCTL_SYS_WEPROTBWIDTH                     (8)       /* !< Bit width of WEPROTB register */
#define FLASHCTL_SYS_WEPROTCWIDTH                     (0)       /* !< Bit width of WEPROTC register */

/******************************************************************************
* DMA Triggers                                                                *
******************************************************************************/

/* External DMA Triggers */
#define DMA_SOFTWARE_TRIG                             (0)
#define DMA_GENERIC_SUB0_TRIG                         (1)
#define DMA_GENERIC_SUB1_TRIG                         (2)
#define DMA_I2C0_TX_TRIG                              (3)
#define DMA_I2C0_RX_TRIG                              (4)
#define DMA_I2C1_TX_TRIG                              (5)
#define DMA_I2C1_RX_TRIG                              (6)
#define DMA_SPI0_RX_TRIG                              (7)
#define DMA_SPI0_TX_TRIG                              (8)
#define DMA_UART0_RX_TRIG                             (9)
#define DMA_UART0_TX_TRIG                             (10)
#define DMA_UART1_RX_TRIG                             (11)
#define DMA_UART1_TX_TRIG                             (12)
#define DMA_UART2_RX_TRIG                             (13)
#define DMA_UART2_TX_TRIG                             (14)
#define DMA_ADC0_EVT_GEN_BD_TRIG                      (15)

/* Internal DMA Triggers */
#define DMA_CH_0_TRIG                                 (0)
#define DMA_CH_1_TRIG                                 (1)
#define DMA_CH_2_TRIG                                 (2)


/******************************************************************************
* IOMUX Pin Definitions                                                       *
******************************************************************************/

typedef enum IOMUX_PINCM
{
  IOMUX_PINCM1         = (0),
  IOMUX_PINCM2         = (1),
  IOMUX_PINCM3         = (2),
  IOMUX_PINCM4         = (3),
  IOMUX_PINCM5         = (4),
  IOMUX_PINCM6         = (5),
  IOMUX_PINCM7         = (6),
  IOMUX_PINCM8         = (7),
  IOMUX_PINCM9         = (8),
  IOMUX_PINCM10        = (9),
  IOMUX_PINCM11        = (10),
  IOMUX_PINCM12        = (11),
  IOMUX_PINCM13        = (12),
  IOMUX_PINCM14        = (13),
  IOMUX_PINCM15        = (14),
  IOMUX_PINCM16        = (15),
  IOMUX_PINCM17        = (16),
  IOMUX_PINCM18        = (17),
  IOMUX_PINCM19        = (18),
  IOMUX_PINCM20        = (19),
  IOMUX_PINCM21        = (20),
  IOMUX_PINCM22        = (21),
  IOMUX_PINCM23        = (22),
  IOMUX_PINCM24        = (23),
  IOMUX_PINCM25        = (24),
  IOMUX_PINCM26        = (25),
  IOMUX_PINCM27        = (26),
  IOMUX_PINCM28        = (27),
  IOMUX_PINCM29        = (28),
  IOMUX_PINCM30        = (29),
  IOMUX_PINCM31        = (30),
  IOMUX_PINCM32        = (31),
  IOMUX_PINCM33        = (32),
  IOMUX_PINCM34        = (33),
  IOMUX_PINCM35        = (34),
  IOMUX_PINCM36        = (35),
  IOMUX_PINCM37        = (36),
  IOMUX_PINCM38        = (37),
  IOMUX_PINCM39        = (38),
  IOMUX_PINCM40        = (39),
  IOMUX_PINCM41        = (40),
  IOMUX_PINCM42        = (41),
  IOMUX_PINCM43        = (42),
  IOMUX_PINCM44        = (43),
  IOMUX_PINCM45        = (44),
} IOMUX_PINCM;


/* IOMUX_PINCM1[PF] Bits */
#define IOMUX_PINCM1_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM1_PF_GPIOA_DIO00                   ((uint32_t)0X00000001)
#define IOMUX_PINCM1_PF_UART0_TX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM1_PF_I2C0_SDA                      ((uint32_t)0X00000003)
#define IOMUX_PINCM1_PF_TIMA0_CCP0                    ((uint32_t)0X00000004)
#define IOMUX_PINCM1_PF_TIMA_FAULT1                   ((uint32_t)0X00000005)
#define IOMUX_PINCM1_PF_SYSCTL_FCC_IN                 ((uint32_t)0X00000006)
#define IOMUX_PINCM1_PF_TIMG8_CCP1                    ((uint32_t)0X00000007)
#define IOMUX_PINCM1_PF_SYSCTL_BEEPER                 ((uint32_t)0X00000008)
#define IOMUX_PINCM1_PF_TIMG14_CCP0                   ((uint32_t)0X00000009)
#define IOMUX_PINCM1_PF_SPI0_CS1_POCI1                ((uint32_t)0X0000000A)
#define IOMUX_PINCM1_PF_LFSS_RTC_OUT                  ((uint32_t)0X0000000C)

/* IOMUX_PINCM2[PF] Bits */
#define IOMUX_PINCM2_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM2_PF_GPIOA_DIO01                   ((uint32_t)0X00000001)
#define IOMUX_PINCM2_PF_UART0_RX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM2_PF_I2C0_SCL                      ((uint32_t)0X00000003)
#define IOMUX_PINCM2_PF_TIMA0_CCP1                    ((uint32_t)0X00000004)
#define IOMUX_PINCM2_PF_TIMA_FAULT2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM2_PF_TIMG8_IDX                     ((uint32_t)0X00000006)
#define IOMUX_PINCM2_PF_TIMG8_CCP0                    ((uint32_t)0X00000007)
#define IOMUX_PINCM2_PF_TIMG14_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM2_PF_SPI0_CS3_CD_POCI3             ((uint32_t)0X0000000A)
#define IOMUX_PINCM2_PF_SYSCTL_HFCLKIN                ((uint32_t)0X0000000B)
#define IOMUX_PINCM2_PF_UART0_TX                      ((uint32_t)0X0000000C)
#define IOMUX_PINCM2_PF_UART1_RTS                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM2_PF_I2C0_SDA                      ((uint32_t)0X0000000E)

/* IOMUX_PINCM3[PF] Bits */
#define IOMUX_PINCM3_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM3_PF_GPIOA_DIO28                   ((uint32_t)0X00000001)
#define IOMUX_PINCM3_PF_UART0_TX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM3_PF_I2C0_SDA                      ((uint32_t)0X00000003)
#define IOMUX_PINCM3_PF_TIMA0_CCP3                    ((uint32_t)0X00000004)
#define IOMUX_PINCM3_PF_TIMA_FAULT0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM3_PF_TIMG2_CCP0                    ((uint32_t)0X00000006)
#define IOMUX_PINCM3_PF_TIMA0_CCP1                    ((uint32_t)0X00000007)

/* IOMUX_PINCM4[PF] Bits */
#define IOMUX_PINCM4_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM4_PF_GPIOA_DIO31                   ((uint32_t)0X00000001)
#define IOMUX_PINCM4_PF_UART0_RX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM4_PF_I2C0_SCL                      ((uint32_t)0X00000003)
#define IOMUX_PINCM4_PF_TIMA0_CCP3_CMPL               ((uint32_t)0X00000004)
#define IOMUX_PINCM4_PF_SYSCTL_CLK_OUT                ((uint32_t)0X00000006)

/* IOMUX_PINCM5[PF] Bits */
#define IOMUX_PINCM5_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM5_PF_GPIOA_DIO02                   ((uint32_t)0X00000001)
#define IOMUX_PINCM5_PF_TIMG8_CCP1                    ((uint32_t)0X00000002)
#define IOMUX_PINCM5_PF_SPI0_CS0                      ((uint32_t)0X00000003)
#define IOMUX_PINCM5_PF_TIMG2_CCP1                    ((uint32_t)0X00000004)
#define IOMUX_PINCM5_PF_TIMG8_IDX                     ((uint32_t)0X00000005)
#define IOMUX_PINCM5_PF_TIMA0_CCP3_CMPL               ((uint32_t)0X00000006)
#define IOMUX_PINCM5_PF_TIMA0_CCP2_CMPL               ((uint32_t)0X00000007)
#define IOMUX_PINCM5_PF_TIMA_FAULT0                   ((uint32_t)0X00000008)
#define IOMUX_PINCM5_PF_TIMA_FAULT1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM5_PF_TIMA0_CCP0                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM5_PF_I2C0_SCL                      ((uint32_t)0X0000000C)

/* IOMUX_PINCM6[PF] Bits */
#define IOMUX_PINCM6_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM6_PF_GPIOA_DIO03                   ((uint32_t)0X00000001)
#define IOMUX_PINCM6_PF_TIMG8_CCP0                    ((uint32_t)0X00000002)
#define IOMUX_PINCM6_PF_SPI0_CS1_POCI1                ((uint32_t)0X00000003)
#define IOMUX_PINCM6_PF_I2C1_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM6_PF_TIMA0_CCP1                    ((uint32_t)0X00000005)
#define IOMUX_PINCM6_PF_TIMG2_CCP0                    ((uint32_t)0X00000007)
#define IOMUX_PINCM6_PF_TIMA0_CCP2                    ((uint32_t)0X00000008)
#define IOMUX_PINCM6_PF_UART2_CTS                     ((uint32_t)0X00000009)
#define IOMUX_PINCM6_PF_UART1_TX                      ((uint32_t)0X0000000A)
#define IOMUX_PINCM6_PF_SPI0_CS3_CD_POCI3             ((uint32_t)0X0000000B)
#define IOMUX_PINCM6_PF_I2C0_SDA                      ((uint32_t)0X0000000C)

/* IOMUX_PINCM7[PF] Bits */
#define IOMUX_PINCM7_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM7_PF_GPIOA_DIO04                   ((uint32_t)0X00000001)
#define IOMUX_PINCM7_PF_TIMG8_CCP1                    ((uint32_t)0X00000002)
#define IOMUX_PINCM7_PF_SPI0_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM7_PF_I2C1_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM7_PF_TIMA0_CCP1_CMPL               ((uint32_t)0X00000005)
#define IOMUX_PINCM7_PF_TIMG2_CCP1                    ((uint32_t)0X00000007)
#define IOMUX_PINCM7_PF_TIMA0_CCP3                    ((uint32_t)0X00000008)
#define IOMUX_PINCM7_PF_UART2_RTS                     ((uint32_t)0X00000009)
#define IOMUX_PINCM7_PF_UART1_RX                      ((uint32_t)0X0000000A)
#define IOMUX_PINCM7_PF_SPI0_CS0                      ((uint32_t)0X0000000B)
#define IOMUX_PINCM7_PF_TIMA0_CCP0_CMPL               ((uint32_t)0X0000000C)
#define IOMUX_PINCM7_PF_SYSCTL_HFCLKIN                ((uint32_t)0X0000000D)

/* IOMUX_PINCM8[PF] Bits */
#define IOMUX_PINCM8_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM8_PF_GPIOA_DIO05                   ((uint32_t)0X00000001)
#define IOMUX_PINCM8_PF_TIMG8_CCP0                    ((uint32_t)0X00000002)
#define IOMUX_PINCM8_PF_SPI0_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM8_PF_I2C1_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM8_PF_TIMG14_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM8_PF_SYSCTL_FCC_IN                 ((uint32_t)0X00000006)
#define IOMUX_PINCM8_PF_TIMG1_CCP0                    ((uint32_t)0X00000007)
#define IOMUX_PINCM8_PF_TIMA_FAULT1                   ((uint32_t)0X00000008)
#define IOMUX_PINCM8_PF_UART0_CTS                     ((uint32_t)0X00000009)
#define IOMUX_PINCM8_PF_UART1_TX                      ((uint32_t)0X0000000B)
#define IOMUX_PINCM8_PF_TIMA0_CCP1                    ((uint32_t)0X0000000C)

/* IOMUX_PINCM9[PF] Bits */
#define IOMUX_PINCM9_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM9_PF_GPIOA_DIO06                   ((uint32_t)0X00000001)
#define IOMUX_PINCM9_PF_TIMG8_CCP1                    ((uint32_t)0X00000002)
#define IOMUX_PINCM9_PF_SPI0_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM9_PF_I2C1_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM9_PF_TIMG14_CCP1                   ((uint32_t)0X00000005)
#define IOMUX_PINCM9_PF_SYSCTL_HFCLKIN                ((uint32_t)0X00000006)
#define IOMUX_PINCM9_PF_TIMG1_CCP1                    ((uint32_t)0X00000007)
#define IOMUX_PINCM9_PF_TIMA_FAULT0                   ((uint32_t)0X00000008)
#define IOMUX_PINCM9_PF_UART0_RTS                     ((uint32_t)0X00000009)
#define IOMUX_PINCM9_PF_TIMA0_CCP2_CMPL               ((uint32_t)0X0000000A)
#define IOMUX_PINCM9_PF_UART1_RX                      ((uint32_t)0X0000000B)
#define IOMUX_PINCM9_PF_TIMA0_CCP2                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM9_PF_I2C0_SDA                      ((uint32_t)0X0000000D)
#define IOMUX_PINCM9_PF_SYSCTL_BEEPER                 ((uint32_t)0X0000000E)

/* IOMUX_PINCM10[PF] Bits */
#define IOMUX_PINCM10_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM10_PF_GPIOA_DIO07                  ((uint32_t)0X00000001)
#define IOMUX_PINCM10_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000002)
#define IOMUX_PINCM10_PF_TIMG8_CCP0                   ((uint32_t)0X00000003)
#define IOMUX_PINCM10_PF_TIMA0_CCP2                   ((uint32_t)0X00000004)
#define IOMUX_PINCM10_PF_TIMG8_IDX                    ((uint32_t)0X00000005)
#define IOMUX_PINCM10_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM10_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM10_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000008)
#define IOMUX_PINCM10_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000009)
#define IOMUX_PINCM10_PF_SPI0_POCI                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM10_PF_SPI0_PICO                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM10_PF_UART1_TX                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM10_PF_TIMG1_CCP0                   ((uint32_t)0X0000000D)

/* IOMUX_PINCM11[PF] Bits */
#define IOMUX_PINCM11_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM11_PF_GPIOB_DIO02                  ((uint32_t)0X00000001)
#define IOMUX_PINCM11_PF_UART2_CTS                    ((uint32_t)0X00000003)
#define IOMUX_PINCM11_PF_I2C1_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM11_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM11_PF_UART1_CTS                    ((uint32_t)0X00000006)
#define IOMUX_PINCM11_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM11_PF_UART2_TX                     ((uint32_t)0X00000008)
#define IOMUX_PINCM11_PF_SYSCTL_HFCLKIN               ((uint32_t)0X0000000A)
#define IOMUX_PINCM11_PF_SPI0_PICO                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM11_PF_UART1_RX                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM11_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X0000000D)

/* IOMUX_PINCM12[PF] Bits */
#define IOMUX_PINCM12_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM12_PF_GPIOB_DIO03                  ((uint32_t)0X00000001)
#define IOMUX_PINCM12_PF_TIMA_FAULT0                  ((uint32_t)0X00000002)
#define IOMUX_PINCM12_PF_UART2_RTS                    ((uint32_t)0X00000003)
#define IOMUX_PINCM12_PF_I2C1_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM12_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM12_PF_UART1_RTS                    ((uint32_t)0X00000006)
#define IOMUX_PINCM12_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM12_PF_UART2_RX                     ((uint32_t)0X00000008)
#define IOMUX_PINCM12_PF_TIMG2_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM12_PF_TIMA0_CCP0                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM12_PF_SPI0_SCLK                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM12_PF_SPI0_CS0                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM12_PF_UART1_TX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM12_PF_LFSS_RTC_OUT                 ((uint32_t)0X0000000E)

/* IOMUX_PINCM13[PF] Bits */
#define IOMUX_PINCM13_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM13_PF_GPIOA_DIO08                  ((uint32_t)0X00000001)
#define IOMUX_PINCM13_PF_UART1_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM13_PF_SPI0_CS0                     ((uint32_t)0X00000003)
#define IOMUX_PINCM13_PF_I2C0_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM13_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM13_PF_TIMA_FAULT2                  ((uint32_t)0X00000006)
#define IOMUX_PINCM13_PF_TIMA_FAULT0                  ((uint32_t)0X00000007)
#define IOMUX_PINCM13_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000008)
#define IOMUX_PINCM13_PF_TIMG2_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM13_PF_SYSCTL_HFCLKIN               ((uint32_t)0X0000000A)
#define IOMUX_PINCM13_PF_UART0_RTS                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM13_PF_SPI0_SCLK                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM13_PF_UART1_RX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM13_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X0000000E)

/* IOMUX_PINCM14[PF] Bits */
#define IOMUX_PINCM14_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM14_PF_GPIOA_DIO09                  ((uint32_t)0X00000001)
#define IOMUX_PINCM14_PF_UART1_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM14_PF_SPI0_PICO                    ((uint32_t)0X00000003)
#define IOMUX_PINCM14_PF_I2C0_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM14_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM14_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM14_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM14_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000008)
#define IOMUX_PINCM14_PF_TIMG2_CCP0                   ((uint32_t)0X00000009)
#define IOMUX_PINCM14_PF_SPI0_POCI                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM14_PF_UART0_CTS                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM14_PF_TIMA_FAULT1                  ((uint32_t)0X0000000C)
#define IOMUX_PINCM14_PF_TIMG1_CCP1                   ((uint32_t)0X0000000D)

/* IOMUX_PINCM15[PF] Bits */
#define IOMUX_PINCM15_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM15_PF_GPIOA_DIO10                  ((uint32_t)0X00000001)
#define IOMUX_PINCM15_PF_UART0_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM15_PF_SPI0_POCI                    ((uint32_t)0X00000003)
#define IOMUX_PINCM15_PF_I2C0_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM15_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM15_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM15_PF_TIMG14_CCP0                  ((uint32_t)0X00000007)
#define IOMUX_PINCM15_PF_I2C1_SDA                     ((uint32_t)0X00000008)
#define IOMUX_PINCM15_PF_TIMA_FAULT1                  ((uint32_t)0X0000000A)
#define IOMUX_PINCM15_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X0000000C)
#define IOMUX_PINCM15_PF_TIMG8_CCP1                   ((uint32_t)0X0000000D)
#define IOMUX_PINCM15_PF_SPI0_PICO                    ((uint32_t)0X0000000E)

/* IOMUX_PINCM16[PF] Bits */
#define IOMUX_PINCM16_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM16_PF_GPIOA_DIO11                  ((uint32_t)0X00000001)
#define IOMUX_PINCM16_PF_UART0_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM16_PF_SPI0_SCLK                    ((uint32_t)0X00000003)
#define IOMUX_PINCM16_PF_I2C0_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM16_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM16_PF_UART1_RX                     ((uint32_t)0X00000006)
#define IOMUX_PINCM16_PF_TIMG14_CCP1                  ((uint32_t)0X00000007)
#define IOMUX_PINCM16_PF_I2C1_SCL                     ((uint32_t)0X00000008)
#define IOMUX_PINCM16_PF_TIMA_FAULT0                  ((uint32_t)0X0000000A)
#define IOMUX_PINCM16_PF_SPI0_CS0                     ((uint32_t)0X0000000C)

/* IOMUX_PINCM17[PF] Bits */
#define IOMUX_PINCM17_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM17_PF_GPIOB_DIO06                  ((uint32_t)0X00000001)
#define IOMUX_PINCM17_PF_UART1_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM17_PF_TIMG8_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM17_PF_UART2_CTS                    ((uint32_t)0X00000006)
#define IOMUX_PINCM17_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM17_PF_TIMA_FAULT2                  ((uint32_t)0X00000008)
#define IOMUX_PINCM17_PF_SPI0_CS1_POCI1               ((uint32_t)0X00000009)
#define IOMUX_PINCM17_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X0000000B)
#define IOMUX_PINCM17_PF_TIMG8_CCP1                   ((uint32_t)0X0000000C)
#define IOMUX_PINCM17_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X0000000D)
#define IOMUX_PINCM17_PF_UART0_TX                     ((uint32_t)0X0000000E)

/* IOMUX_PINCM18[PF] Bits */
#define IOMUX_PINCM18_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM18_PF_GPIOB_DIO07                  ((uint32_t)0X00000001)
#define IOMUX_PINCM18_PF_UART1_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM18_PF_TIMG8_CCP1                   ((uint32_t)0X00000005)
#define IOMUX_PINCM18_PF_UART2_RTS                    ((uint32_t)0X00000006)
#define IOMUX_PINCM18_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM18_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000009)
#define IOMUX_PINCM18_PF_SYSCTL_BEEPER                ((uint32_t)0X0000000C)
#define IOMUX_PINCM18_PF_SPI0_SCLK                    ((uint32_t)0X0000000D)
#define IOMUX_PINCM18_PF_UART0_RX                     ((uint32_t)0X0000000E)

/* IOMUX_PINCM19[PF] Bits */
#define IOMUX_PINCM19_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM19_PF_GPIOB_DIO08                  ((uint32_t)0X00000001)
#define IOMUX_PINCM19_PF_UART1_CTS                    ((uint32_t)0X00000002)
#define IOMUX_PINCM19_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM19_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM19_PF_SPI0_SCLK                    ((uint32_t)0X00000009)
#define IOMUX_PINCM19_PF_SYSCTL_BEEPER                ((uint32_t)0X0000000A)
#define IOMUX_PINCM19_PF_TIMG8_CCP0                   ((uint32_t)0X0000000B)
#define IOMUX_PINCM19_PF_UART0_RX                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM19_PF_SPI0_POCI                    ((uint32_t)0X0000000D)
#define IOMUX_PINCM19_PF_I2C0_SCL                     ((uint32_t)0X0000000E)

/* IOMUX_PINCM20[PF] Bits */
#define IOMUX_PINCM20_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM20_PF_GPIOB_DIO09                  ((uint32_t)0X00000001)
#define IOMUX_PINCM20_PF_UART1_RTS                    ((uint32_t)0X00000002)
#define IOMUX_PINCM20_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM20_PF_TIMA0_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM20_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM20_PF_TIMG2_CCP0                   ((uint32_t)0X00000008)
#define IOMUX_PINCM20_PF_SPI0_POCI                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM20_PF_UART0_RX                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM20_PF_I2C0_SCL                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM20_PF_UART0_TX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM20_PF_I2C0_SDA                     ((uint32_t)0X0000000E)

/* IOMUX_PINCM21[PF] Bits */
#define IOMUX_PINCM21_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM21_PF_GPIOB_DIO14                  ((uint32_t)0X00000001)
#define IOMUX_PINCM21_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM21_PF_TIMG8_IDX                    ((uint32_t)0X00000006)
#define IOMUX_PINCM21_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000007)
#define IOMUX_PINCM21_PF_TIMG2_CCP1                   ((uint32_t)0X00000008)
#define IOMUX_PINCM21_PF_I2C0_SDA                     ((uint32_t)0X00000009)
#define IOMUX_PINCM21_PF_SPI0_PICO                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM21_PF_UART0_TX                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM21_PF_TIMA_FAULT2                  ((uint32_t)0X0000000C)
#define IOMUX_PINCM21_PF_TIMA_FAULT0                  ((uint32_t)0X0000000D)
#define IOMUX_PINCM21_PF_TIMG14_CCP2                  ((uint32_t)0X0000000E)

/* IOMUX_PINCM22[PF] Bits */
#define IOMUX_PINCM22_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM22_PF_GPIOB_DIO15                  ((uint32_t)0X00000001)
#define IOMUX_PINCM22_PF_UART2_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM22_PF_TIMG8_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM22_PF_TIMG2_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM22_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X0000000C)
#define IOMUX_PINCM22_PF_UART1_TX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM22_PF_TIMG2_CCP1                   ((uint32_t)0X0000000E)

/* IOMUX_PINCM23[PF] Bits */
#define IOMUX_PINCM23_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM23_PF_GPIOB_DIO16                  ((uint32_t)0X00000001)
#define IOMUX_PINCM23_PF_UART2_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM23_PF_TIMG8_CCP1                   ((uint32_t)0X00000005)
#define IOMUX_PINCM23_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM23_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X0000000C)
#define IOMUX_PINCM23_PF_UART1_RX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM23_PF_I2C1_SDA                     ((uint32_t)0X0000000E)

/* IOMUX_PINCM24[PF] Bits */
#define IOMUX_PINCM24_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM24_PF_GPIOA_DIO12                  ((uint32_t)0X00000001)
#define IOMUX_PINCM24_PF_SPI0_SCLK                    ((uint32_t)0X00000002)
#define IOMUX_PINCM24_PF_TIMA0_CCP3                   ((uint32_t)0X00000004)
#define IOMUX_PINCM24_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000005)
#define IOMUX_PINCM24_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)
#define IOMUX_PINCM24_PF_SPI0_CS1_POCI1               ((uint32_t)0X00000008)
#define IOMUX_PINCM24_PF_UART2_CTS                    ((uint32_t)0X00000009)
#define IOMUX_PINCM24_PF_UART1_CTS                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM24_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X0000000B)
#define IOMUX_PINCM24_PF_I2C1_SCL                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM24_PF_TIMG2_CCP1                   ((uint32_t)0X0000000D)

/* IOMUX_PINCM25[PF] Bits */
#define IOMUX_PINCM25_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM25_PF_GPIOA_DIO13                  ((uint32_t)0X00000001)
#define IOMUX_PINCM25_PF_SPI0_POCI                    ((uint32_t)0X00000003)
#define IOMUX_PINCM25_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000004)
#define IOMUX_PINCM25_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM25_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000006)
#define IOMUX_PINCM25_PF_TIMG14_CCP1                  ((uint32_t)0X00000007)
#define IOMUX_PINCM25_PF_TIMG14_CCP3                  ((uint32_t)0X00000008)
#define IOMUX_PINCM25_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000009)
#define IOMUX_PINCM25_PF_UART2_TX                     ((uint32_t)0X0000000A)
#define IOMUX_PINCM25_PF_UART1_RTS                    ((uint32_t)0X0000000B)
#define IOMUX_PINCM25_PF_SPI0_CS0                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM25_PF_TIMG8_CCP1                   ((uint32_t)0X0000000D)
#define IOMUX_PINCM25_PF_TIMA0_CCP1                   ((uint32_t)0X0000000E)

/* IOMUX_PINCM26[PF] Bits */
#define IOMUX_PINCM26_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM26_PF_GPIOA_DIO14                  ((uint32_t)0X00000001)
#define IOMUX_PINCM26_PF_UART0_CTS                    ((uint32_t)0X00000002)
#define IOMUX_PINCM26_PF_SPI0_PICO                    ((uint32_t)0X00000003)
#define IOMUX_PINCM26_PF_TIMG1_CCP0                   ((uint32_t)0X00000004)
#define IOMUX_PINCM26_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM26_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000009)
#define IOMUX_PINCM26_PF_UART2_RX                     ((uint32_t)0X0000000A)
#define IOMUX_PINCM26_PF_I2C0_SCL                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM26_PF_UART0_TX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM26_PF_TIMA0_CCP2                   ((uint32_t)0X0000000E)

/* IOMUX_PINCM27[PF] Bits */
#define IOMUX_PINCM27_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM27_PF_GPIOA_DIO15                  ((uint32_t)0X00000001)
#define IOMUX_PINCM27_PF_UART0_RTS                    ((uint32_t)0X00000002)
#define IOMUX_PINCM27_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000003)
#define IOMUX_PINCM27_PF_I2C1_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM27_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM27_PF_TIMG8_IDX                    ((uint32_t)0X00000007)
#define IOMUX_PINCM27_PF_UART2_RTS                    ((uint32_t)0X0000000A)

/* IOMUX_PINCM28[PF] Bits */
#define IOMUX_PINCM28_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM28_PF_GPIOA_DIO16                  ((uint32_t)0X00000001)
#define IOMUX_PINCM28_PF_SPI0_POCI                    ((uint32_t)0X00000003)
#define IOMUX_PINCM28_PF_I2C1_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM28_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM28_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000007)
#define IOMUX_PINCM28_PF_UART2_CTS                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM28_PF_TIMG14_CCP2                  ((uint32_t)0X0000000C)

/* IOMUX_PINCM29[PF] Bits */
#define IOMUX_PINCM29_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM29_PF_GPIOA_DIO17                  ((uint32_t)0X00000001)
#define IOMUX_PINCM29_PF_UART1_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM29_PF_TIMA0_CCP2                   ((uint32_t)0X00000003)
#define IOMUX_PINCM29_PF_I2C1_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM29_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM29_PF_TIMG2_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM29_PF_TIMG8_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM29_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000008)
#define IOMUX_PINCM29_PF_SPI0_CS1_POCI1               ((uint32_t)0X00000009)
#define IOMUX_PINCM29_PF_SPI0_SCLK                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM29_PF_UART0_RX                     ((uint32_t)0X0000000C)

/* IOMUX_PINCM30[PF] Bits */
#define IOMUX_PINCM30_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM30_PF_GPIOA_DIO18                  ((uint32_t)0X00000001)
#define IOMUX_PINCM30_PF_UART1_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM30_PF_UART1_RTS                    ((uint32_t)0X00000003)
#define IOMUX_PINCM30_PF_I2C1_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM30_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM30_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM30_PF_TIMG8_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM30_PF_SPI0_PICO                    ((uint32_t)0X00000008)
#define IOMUX_PINCM30_PF_SPI0_CS0                     ((uint32_t)0X00000009)
#define IOMUX_PINCM30_PF_UART0_CTS                    ((uint32_t)0X0000000A)
#define IOMUX_PINCM30_PF_TIMA0_CCP0                   ((uint32_t)0X0000000B)
#define IOMUX_PINCM30_PF_SPI0_POCI                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM30_PF_TIMA_FAULT2                  ((uint32_t)0X0000000D)
#define IOMUX_PINCM30_PF_SYSCTL_CLK_OUT               ((uint32_t)0X0000000E)

/* IOMUX_PINCM31[PF] Bits */
#define IOMUX_PINCM31_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM31_PF_GPIOA_DIO19                  ((uint32_t)0X00000001)
#define IOMUX_PINCM31_PF_DEBUGSS_SWDIO                ((uint32_t)0X00000002)
#define IOMUX_PINCM31_PF_SPI0_SCLK                    ((uint32_t)0X00000003)
#define IOMUX_PINCM31_PF_I2C1_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM31_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM31_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)
#define IOMUX_PINCM31_PF_SPI0_POCI                    ((uint32_t)0X00000007)
#define IOMUX_PINCM31_PF_UART0_CTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM31_PF_UART0_RTS                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM31_PF_SPI0_PICO                    ((uint32_t)0X0000000D)

/* IOMUX_PINCM32[PF] Bits */
#define IOMUX_PINCM32_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM32_PF_GPIOA_DIO20                  ((uint32_t)0X00000001)
#define IOMUX_PINCM32_PF_DEBUGSS_SWCLK                ((uint32_t)0X00000002)
#define IOMUX_PINCM32_PF_TIMA_FAULT1                  ((uint32_t)0X00000003)
#define IOMUX_PINCM32_PF_I2C1_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM32_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM32_PF_TIMG14_CCP1                  ((uint32_t)0X00000006)
#define IOMUX_PINCM32_PF_SPI0_PICO                    ((uint32_t)0X00000007)
#define IOMUX_PINCM32_PF_TIMA0_CCP0                   ((uint32_t)0X00000008)
#define IOMUX_PINCM32_PF_UART0_RTS                    ((uint32_t)0X00000009)
#define IOMUX_PINCM32_PF_UART1_RX                     ((uint32_t)0X0000000D)

/* IOMUX_PINCM33[PF] Bits */
#define IOMUX_PINCM33_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM33_PF_GPIOB_DIO17                  ((uint32_t)0X00000001)
#define IOMUX_PINCM33_PF_UART2_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM33_PF_SPI0_PICO                    ((uint32_t)0X00000003)
#define IOMUX_PINCM33_PF_I2C0_SCL                     ((uint32_t)0X00000004)
#define IOMUX_PINCM33_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM33_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)
#define IOMUX_PINCM33_PF_TIMG1_CCP0                   ((uint32_t)0X00000009)
#define IOMUX_PINCM33_PF_SPI0_CS0                     ((uint32_t)0X0000000A)
#define IOMUX_PINCM33_PF_UART1_RX                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM33_PF_UART1_TX                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM33_PF_UART0_RTS                    ((uint32_t)0X0000000E)

/* IOMUX_PINCM34[PF] Bits */
#define IOMUX_PINCM34_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM34_PF_GPIOB_DIO18                  ((uint32_t)0X00000001)
#define IOMUX_PINCM34_PF_UART2_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM34_PF_SPI0_SCLK                    ((uint32_t)0X00000003)
#define IOMUX_PINCM34_PF_I2C0_SDA                     ((uint32_t)0X00000004)
#define IOMUX_PINCM34_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM34_PF_TIMG14_CCP1                  ((uint32_t)0X00000006)
#define IOMUX_PINCM34_PF_SPI0_CS0                     ((uint32_t)0X00000007)
#define IOMUX_PINCM34_PF_TIMG1_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM34_PF_TIMA0_CCP1                   ((uint32_t)0X0000000C)
#define IOMUX_PINCM34_PF_UART0_RTS                    ((uint32_t)0X0000000D)

/* IOMUX_PINCM35[PF] Bits */
#define IOMUX_PINCM35_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM35_PF_GPIOB_DIO19                  ((uint32_t)0X00000001)
#define IOMUX_PINCM35_PF_SPI0_POCI                    ((uint32_t)0X00000003)
#define IOMUX_PINCM35_PF_TIMG8_CCP1                   ((uint32_t)0X00000004)
#define IOMUX_PINCM35_PF_UART0_CTS                    ((uint32_t)0X00000005)
#define IOMUX_PINCM35_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM35_PF_TIMG8_IDX                    ((uint32_t)0X00000007)
#define IOMUX_PINCM35_PF_UART2_CTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM35_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X0000000C)
#define IOMUX_PINCM35_PF_UART2_RX                     ((uint32_t)0X0000000D)

/* IOMUX_PINCM36[PF] Bits */
#define IOMUX_PINCM36_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM36_PF_GPIOA_DIO21                  ((uint32_t)0X00000001)
#define IOMUX_PINCM36_PF_UART2_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM36_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000003)
#define IOMUX_PINCM36_PF_UART1_CTS                    ((uint32_t)0X00000004)
#define IOMUX_PINCM36_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM36_PF_TIMG1_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM36_PF_UART2_CTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM36_PF_TIMG8_CCP0                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM36_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X0000000C)
#define IOMUX_PINCM36_PF_UART2_RX                     ((uint32_t)0X0000000D)

/* IOMUX_PINCM37[PF] Bits */
#define IOMUX_PINCM37_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM37_PF_GPIOA_DIO22                  ((uint32_t)0X00000001)
#define IOMUX_PINCM37_PF_UART2_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM37_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000003)
#define IOMUX_PINCM37_PF_UART1_RTS                    ((uint32_t)0X00000004)
#define IOMUX_PINCM37_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM37_PF_TIMG1_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM37_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM37_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000008)
#define IOMUX_PINCM37_PF_I2C0_SCL                     ((uint32_t)0X00000009)
#define IOMUX_PINCM37_PF_TIMG8_CCP1                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM37_PF_UART1_RX                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM37_PF_SPI0_POCI                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM37_PF_UART2_TX                     ((uint32_t)0X0000000D)

/* IOMUX_PINCM38[PF] Bits */
#define IOMUX_PINCM38_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM38_PF_GPIOB_DIO20                  ((uint32_t)0X00000001)
#define IOMUX_PINCM38_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000002)
#define IOMUX_PINCM38_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM38_PF_TIMA_FAULT1                  ((uint32_t)0X00000006)
#define IOMUX_PINCM38_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM38_PF_UART2_RTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM38_PF_I2C0_SDA                     ((uint32_t)0X00000009)
#define IOMUX_PINCM38_PF_UART1_CTS                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM38_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X0000000D)
#define IOMUX_PINCM38_PF_TIMG8_CCP1                   ((uint32_t)0X0000000E)

/* IOMUX_PINCM39[PF] Bits */
#define IOMUX_PINCM39_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM39_PF_GPIOB_DIO24                  ((uint32_t)0X00000001)
#define IOMUX_PINCM39_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000002)
#define IOMUX_PINCM39_PF_SPI0_CS1_POCI1               ((uint32_t)0X00000003)
#define IOMUX_PINCM39_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM39_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X00000006)
#define IOMUX_PINCM39_PF_UART2_RTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM39_PF_SPI0_SCLK                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM39_PF_TIMG14_CCP2                  ((uint32_t)0X0000000D)
#define IOMUX_PINCM39_PF_UART0_RTS                    ((uint32_t)0X0000000E)

/* IOMUX_PINCM40[PF] Bits */
#define IOMUX_PINCM40_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM40_PF_GPIOA_DIO23                  ((uint32_t)0X00000001)
#define IOMUX_PINCM40_PF_UART2_TX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM40_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000003)
#define IOMUX_PINCM40_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM40_PF_TIMG8_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM40_PF_TIMG2_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM40_PF_UART0_TX                     ((uint32_t)0X00000008)
#define IOMUX_PINCM40_PF_TIMG14_CCP0                  ((uint32_t)0X00000009)
#define IOMUX_PINCM40_PF_SPI0_POCI                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM40_PF_UART0_CTS                    ((uint32_t)0X0000000D)

/* IOMUX_PINCM41[PF] Bits */
#define IOMUX_PINCM41_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM41_PF_GPIOA_DIO24                  ((uint32_t)0X00000001)
#define IOMUX_PINCM41_PF_UART2_RX                     ((uint32_t)0X00000002)
#define IOMUX_PINCM41_PF_SPI0_CS2_POCI2               ((uint32_t)0X00000003)
#define IOMUX_PINCM41_PF_UART0_RTS                    ((uint32_t)0X00000004)
#define IOMUX_PINCM41_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM41_PF_TIMG8_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM41_PF_TIMG2_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM41_PF_UART1_RX                     ((uint32_t)0X00000008)
#define IOMUX_PINCM41_PF_TIMG14_CCP1                  ((uint32_t)0X00000009)
#define IOMUX_PINCM41_PF_SPI0_PICO                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM41_PF_I2C0_SDA                     ((uint32_t)0X0000000D)

/* IOMUX_PINCM42[PF] Bits */
#define IOMUX_PINCM42_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM42_PF_GPIOA_DIO25                  ((uint32_t)0X00000001)
#define IOMUX_PINCM42_PF_SPI0_PICO                    ((uint32_t)0X00000002)
#define IOMUX_PINCM42_PF_SPI0_POCI                    ((uint32_t)0X00000003)
#define IOMUX_PINCM42_PF_SPI0_SCLK                    ((uint32_t)0X00000004)
#define IOMUX_PINCM42_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM42_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X00000006)
#define IOMUX_PINCM42_PF_TIMA0_CCP2                   ((uint32_t)0X00000007)
#define IOMUX_PINCM42_PF_UART2_CTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM42_PF_TIMG14_CCP0                  ((uint32_t)0X00000009)
#define IOMUX_PINCM42_PF_TIMG1_CCP0                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM42_PF_I2C0_SDA                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM42_PF_UART0_TX                     ((uint32_t)0X0000000C)
#define IOMUX_PINCM42_PF_UART0_RTS                    ((uint32_t)0X0000000D)
#define IOMUX_PINCM42_PF_I2C0_SCL                     ((uint32_t)0X0000000E)

/* IOMUX_PINCM43[PF] Bits */
#define IOMUX_PINCM43_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM43_PF_GPIOA_DIO26                  ((uint32_t)0X00000001)
#define IOMUX_PINCM43_PF_SYSCTL_BEEPER                ((uint32_t)0X00000002)
#define IOMUX_PINCM43_PF_SPI0_POCI                    ((uint32_t)0X00000003)
#define IOMUX_PINCM43_PF_TIMG8_CCP0                   ((uint32_t)0X00000004)
#define IOMUX_PINCM43_PF_TIMA_FAULT0                  ((uint32_t)0X00000005)
#define IOMUX_PINCM43_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000006)
#define IOMUX_PINCM43_PF_TIMG2_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM43_PF_UART2_RTS                    ((uint32_t)0X00000008)
#define IOMUX_PINCM43_PF_I2C0_SCL                     ((uint32_t)0X00000009)
#define IOMUX_PINCM43_PF_TIMG1_CCP1                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM43_PF_UART0_RX                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM43_PF_TIMA0_CCP0                   ((uint32_t)0X0000000C)
#define IOMUX_PINCM43_PF_I2C0_SDA                     ((uint32_t)0X0000000D)
#define IOMUX_PINCM43_PF_UART1_CTS                    ((uint32_t)0X0000000E)

/* IOMUX_PINCM44[PF] Bits */
#define IOMUX_PINCM44_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM44_PF_GPIOA_DIO27                  ((uint32_t)0X00000001)
#define IOMUX_PINCM44_PF_SPI0_CS3_CD_POCI3            ((uint32_t)0X00000002)
#define IOMUX_PINCM44_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000003)
#define IOMUX_PINCM44_PF_TIMG8_CCP1                   ((uint32_t)0X00000004)
#define IOMUX_PINCM44_PF_TIMA_FAULT2                  ((uint32_t)0X00000005)
#define IOMUX_PINCM44_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM44_PF_TIMG2_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM44_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000008)
#define IOMUX_PINCM44_PF_UART1_CTS                    ((uint32_t)0X00000009)
#define IOMUX_PINCM44_PF_I2C0_SCL                     ((uint32_t)0X0000000A)
#define IOMUX_PINCM44_PF_UART0_TX                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM44_PF_SPI0_POCI                    ((uint32_t)0X0000000C)
#define IOMUX_PINCM44_PF_SYSCTL_LFCLKIN               ((uint32_t)0X0000000E)

/* IOMUX_PINCM45[PF] Bits */
#define IOMUX_PINCM45_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM45_PF_GPIOA_DIO30                  ((uint32_t)0X00000001)
#define IOMUX_PINCM45_PF_UART0_RX                     ((uint32_t)0X00000004)
#define IOMUX_PINCM45_PF_TIMG8_IDX                    ((uint32_t)0X00000005)
#define IOMUX_PINCM45_PF_TIMA0_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM45_PF_UART1_RTS                    ((uint32_t)0X00000009)
#define IOMUX_PINCM45_PF_TIMG2_CCP1                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM45_PF_TIMG14_CCP2                  ((uint32_t)0X0000000B)
#define IOMUX_PINCM45_PF_I2C0_SDA                     ((uint32_t)0X0000000C)


#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/*@}*/ /* end of group MSPM0H321X_PeripheralDecl */

/*@}*/ /* end of group MSPM0H321X_Definitions */

#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_m0p_mspm0h321x__include */
