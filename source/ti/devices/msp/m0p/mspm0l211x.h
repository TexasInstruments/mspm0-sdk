/*****************************************************************************

  Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/ 

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

#ifndef ti_devices_msp_m0p_mspm0l211x__include
#define ti_devices_msp_m0p_mspm0l211x__include

/* Filename: mspm0l211x.h */
/* Revised: 2025-05-22 14:35:05*/

/* Use standard integer types with explicit width */
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup MSPM0L211X_Definitions MSPM0L211X Definitions
  This file defines all structures and symbols for MSPM0L211X
  @{
*/

/** @addtogroup MSPM0L211X_CMSIS Device CMSIS Definitions
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
  UC6_INT_IRQn                = 3,      /* 19 UC6_INT Interrupt */
  ADC0_INT_IRQn               = 4,      /* 20 ADC0_INT Interrupt */
  TIMB0_INT_IRQn              = 5,      /* 21 TIMB0_INT Interrupt */
  TIMB1_INT_IRQn              = 6,      /* 22 TIMB1_INT Interrupt */
  COMP0_INT_IRQn              = 7,      /* 23 COMP0_INT Interrupt */
  UC7_INT_IRQn                = 8,      /* 24 UC7_INT Interrupt */
  UC8_INT_IRQn                = 9,      /* 25 UC8_INT Interrupt */
  TIMB2_INT_IRQn              = 13,     /* 29 TIMB2_INT Interrupt */
  TIMB3_INT_IRQn              = 14,     /* 30 TIMB3_INT Interrupt */
  UC4_INT_IRQn                = 15,     /* 31 UC4_INT Interrupt */
  TIMG14_INT_IRQn             = 16,     /* 32 TIMG14_INT Interrupt */
  TIMG2_INT_IRQn              = 17,     /* 33 TIMG2_INT Interrupt */
  TIMA0_INT_IRQn              = 18,     /* 34 TIMA0_INT Interrupt */
  TIMG1_INT_IRQn              = 19,     /* 35 TIMG1_INT Interrupt */
  GPIOA_INT_IRQn              = 22,     /* 38 GPIOA_INT Interrupt */
  GPIOB_INT_IRQn              = 23,     /* 39 GPIOB_INT Interrupt */
  UC11_INT_IRQn               = 24,     /* 40 UC11_INT Interrupt */
  AESADV_INT_IRQn             = 26,     /* 42 AESADV_INT Interrupt */
  FLASHCTL_INT_IRQn           = 27,     /* 43 FLASHCTL_INT Interrupt */
  LCD_INT_IRQn                = 28,     /* 44 LCD_INT Interrupt */
  WWDT0_INT_IRQn              = 29,     /* 45 WWDT0_INT Interrupt */
  LFSS_INT_IRQn               = 30,     /* 46 LFSS_INT Interrupt */
  RTC_B_INT_IRQn              = 30,     /* 46 RTC_B_INT Interrupt */
  IWDT_INT_IRQn               = 30,     /* 46 IWDT_INT Interrupt */
  DMA_INT_IRQn                = 31,     /* 47 DMA_INT Interrupt */
} IRQn_Type;

#define NonMaskableInt_VECn     2     /* Non Maskable Interrupt */
#define HardFault_VECn          3     /* Hard Fault Interrupt */
#define SVCall_VECn             11    /* SV Call Interrupt */
#define PendSV_VECn             14    /* Pend SV Interrupt */
#define SysTick_VECn            15    /* System Tick Interrupt */
#define SYSCTL_INT_VECn         16    /* SYSCTL_INT Interrupt */
#define DEBUGSS_INT_VECn        17    /* DEBUGSS_INT Interrupt */
#define UC6_INT_VECn            19    /* UC6_INT Interrupt */
#define ADC0_INT_VECn           20    /* ADC0_INT Interrupt */
#define TIMB0_INT_VECn          21    /* TIMB0_INT Interrupt */
#define TIMB1_INT_VECn          22    /* TIMB1_INT Interrupt */
#define COMP0_INT_VECn          23    /* COMP0_INT Interrupt */
#define UC7_INT_VECn            24    /* UC7_INT Interrupt */
#define UC8_INT_VECn            25    /* UC8_INT Interrupt */
#define TIMB2_INT_VECn          29    /* TIMB2_INT Interrupt */
#define TIMB3_INT_VECn          30    /* TIMB3_INT Interrupt */
#define UC4_INT_VECn            31    /* UC4_INT Interrupt */
#define TIMG14_INT_VECn         32    /* TIMG14_INT Interrupt */
#define TIMG2_INT_VECn          33    /* TIMG2_INT Interrupt */
#define TIMA0_INT_VECn          34    /* TIMA0_INT Interrupt */
#define TIMG1_INT_VECn          35    /* TIMG1_INT Interrupt */
#define GPIOA_INT_VECn          38    /* GPIOA_INT Interrupt */
#define GPIOB_INT_VECn          39    /* GPIOB_INT Interrupt */
#define UC11_INT_VECn           40    /* UC11_INT Interrupt */
#define AESADV_INT_VECn         42    /* AESADV_INT Interrupt */
#define FLASHCTL_INT_VECn       43    /* FLASHCTL_INT Interrupt */
#define LCD_INT_VECn            44    /* LCD_INT Interrupt */
#define WWDT0_INT_VECn          45    /* WWDT0_INT Interrupt */
#define LFSS_INT_VECn           46    /* LFSS_INT Interrupt */
#define RTC_B_INT_VECn          46    /* RTC_B_INT Interrupt */
#define IWDT_INT_VECn           46    /* IWDT_INT Interrupt */
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

#define __CM0PLUS_REV           0x0001U       /* Core revision */
#define __MPU_PRESENT           0x0001U       /* MPU present */
#define __VTOR_PRESENT          0x0001U       /* VTOR present */
#define __NVIC_PRIO_BITS        0x0002U       /* Number of bits used for Priority Levels */
#define __Vendor_SysTickConfig  0x0000U       /* Set to 1 if different SysTick Config is used */

#include "core_cm0plus.h"                  /* Processor and core peripherals */

/******************************************************************************
* Peripheral headers                                                          *
******************************************************************************/
/** @addtogroup MSPM0L211X_Peripherals MSPM0L211X Peripherals
  MSPM0L211X Peripheral registers structures
  @{
*/

#include <ti/devices/msp/peripherals/hw_adc12.h>
#include <ti/devices/msp/peripherals/hw_aesadv.h>
#include <ti/devices/msp/peripherals/hw_btimer.h>
#include <ti/devices/msp/peripherals/hw_comp.h>
#include <ti/devices/msp/peripherals/hw_crc.h>
#include <ti/devices/msp/peripherals/hw_dma.h>
#include <ti/devices/msp/peripherals/hw_flashctl.h>
#include <ti/devices/msp/peripherals/hw_gpio.h>
#include <ti/devices/msp/peripherals/hw_gptimer.h>
#include <ti/devices/msp/peripherals/hw_iomux.h>
#include <ti/devices/msp/peripherals/hw_keystorectl.h>
#include <ti/devices/msp/peripherals/hw_rtc.h>
#include <ti/devices/msp/peripherals/hw_lcd.h>
#include <ti/devices/msp/peripherals/hw_lfss.h>
#include <ti/devices/msp/peripherals/hw_spgss.h>
#include <ti/devices/msp/peripherals/hw_unicomm.h>
#include <ti/devices/msp/peripherals/hw_unicommi2cc.h>
#include <ti/devices/msp/peripherals/hw_unicommi2ct.h>
#include <ti/devices/msp/peripherals/hw_unicommspi.h>
#include <ti/devices/msp/peripherals/hw_unicommuart.h>
#include <ti/devices/msp/peripherals/hw_vref.h>
#include <ti/devices/msp/peripherals/hw_wuc.h>
#include <ti/devices/msp/peripherals/hw_wwdt.h>
#include <ti/devices/msp/peripherals/m0p/hw_factoryregion.h>
#include <ti/devices/msp/peripherals/m0p/hw_cpuss.h>
#include <ti/devices/msp/peripherals/m0p/hw_debugss.h>
#include <ti/devices/msp/peripherals/m0p/hw_sysctl.h>

#define __MSPM0_HAS_ADC12__
#define __MSPM0_HAS_AESADV__
#define __MSPM0_HAS_TIMER_B__
#define __MSPM0_HAS_COMP__ 
#define __MSPM0_HAS_CRC__
#define __MSPM0_HAS_GPIO__
#define __MSPM0_HAS_TIMER_A__
#define __MSPM0_HAS_TIMER_G__
#define __MSPM0_HAS_KEYSTORE_CTL__
#define __MSPM0_HAS_LCD__
#define __MSPM0_HAS_LFSS__
#define __MSPM0_HAS_RTC_B__
#define __MSPM0_HAS_IWDT__
#define __MCU_HAS_SPGSS__
#define __MCU_HAS_UNICOMMI2CC__
#define __MCU_HAS_UNICOMMI2CT__
#define __MCU_HAS_UNICOMMSPI__
#define __MCU_HAS_UNICOMMUART__
#define __MCU_HAS_UNICOMM__
#define __MSPM0_HAS_VREF__
#define __MSPM0_HAS_WWDT__
#define __MSPM0_HAS_ECC__

/*@}*/ /* end of group MSPM0L211X_Peripherals */

/******************************************************************************
* Device and peripheral memory map                                            *
******************************************************************************/
/** @addtogroup MSPM0L211X_MemoryMap MSPM0L211X Memory Mapping
  @{
*/

#define ADC0_BASE                      (0x40004000U)     /*!< Base address of module ADC0 */
#define COMP0_BASE                     (0x40008000U)     /*!< Base address of module COMP0 */
#define VREF_BASE                      (0x40030000U)     /*!< Base address of module VREF */
#define LCD_BASE                       (0x40070000U)     /*!< Base address of module LCD */
#define WWDT0_BASE                     (0x40080000U)     /*!< Base address of module WWDT0 */
#define TIMG14_BASE                    (0x40084000U)     /*!< Base address of module TIMG14 */
#define TIMG1_BASE                     (0x40086000U)     /*!< Base address of module TIMG1 */
#define TIMG2_BASE                     (0x40088000U)     /*!< Base address of module TIMG2 */
#define LFSS_BASE                      (0x40094000U)     /*!< Base address of module LFSS */
#define RTC_B_BASE                     (0x40094000U)     /*!< Base address of module RTC_B */
#define IWDT_BASE                      (0x40094000U)     /*!< Base address of module IWDT */
#define GPIOA_BASE                     (0x400A0000U)     /*!< Base address of module GPIOA */
#define GPIOB_BASE                     (0x400A2000U)     /*!< Base address of module GPIOB */
#define KEYSTORECTL_BASE               (0x400AC000U)     /*!< Base address of module KEYSTORECTL */
#define SYSCTL_BASE                    (0x400AF000U)     /*!< Base address of module SYSCTL */
#define TIMB0_BASE                     (0x400B8000U)     /*!< Base address of module TIMB0 */
#define TIMB1_BASE                     (0x400BA000U)     /*!< Base address of module TIMB1 */
#define TIMB2_BASE                     (0x400BC000U)     /*!< Base address of module TIMB2 */
#define TIMB3_BASE                     (0x400BE000U)     /*!< Base address of module TIMB3 */
#define DEBUGSS_BASE                   (0x400C7000U)     /*!< Base address of module DEBUGSS */
#define EVENTLP_BASE                   (0x400C9000U)     /*!< Base address of module EVENTLP */
#define FLASHCTL_BASE                  (0x400CD000U)     /*!< Base address of module FLASHCTL */
#define CPUSS_BASE                     (0x40400000U)     /*!< Base address of module CPUSS */
#define WUC_BASE                       (0x40424000U)     /*!< Base address of module WUC */
#define IOMUX_BASE                     (0x40428000U)     /*!< Base address of module IOMUX */
#define DMA_BASE                       (0x4042A000U)     /*!< Base address of module DMA */
#define CRC0_BASE                      (0x40440000U)     /*!< Base address of module CRC0 */
#define AESADV_BASE                    (0x40442000U)     /*!< Base address of module AESADV */
#define ADC0_SVT_BASE                  (0x4055A000U)     /*!< Base address of module ADC0_SVT */
#define TIMA0_BASE                     (0x40860000U)     /*!< Base address of module TIMA0 */
#define UC4_UART_BASE                  (0x40A00000U)     /*!< Base address of module UC4_UART */
#define UC6_I2CC_BASE                  (0x40A22000U)     /*!< Base address of module UC6_I2CC */
#define UC7_I2CT_BASE                  (0x40A44000U)     /*!< Base address of module UC7_I2CT */
#define UC4_SPI_BASE                   (0x40A60000U)     /*!< Base address of module UC4_SPI */
#define UC4_BASE                       (0x40A80000U)     /*!< Base address of module UC4 */
#define UC6_BASE                       (0x40A82000U)     /*!< Base address of module UC6 */
#define UC7_BASE                       (0x40A84000U)     /*!< Base address of module UC7 */
#define SPG0_BASE                      (0x40A9F000U)     /*!< Base address of module SPG0 */
#define UC8_UART_BASE                  (0x40B00000U)     /*!< Base address of module UC8_UART */
#define UC11_UART_BASE                 (0x40B02000U)     /*!< Base address of module UC11_UART */
#define UC8_SPI_BASE                   (0x40B60000U)     /*!< Base address of module UC8_SPI */
#define UC8_BASE                       (0x40B80000U)     /*!< Base address of module UC8 */
#define UC11_BASE                      (0x40B82000U)     /*!< Base address of module UC11 */
#define FACTORYREGION_BASE             (0x41C40000U)     /*!< Base address of module FACTORYREGION */


typedef struct {
    UNICOMM_Regs         * const inst;
    UNICOMMUART_Regs     * const uart;
    UNICOMMI2CC_Regs     * const i2cc;
    UNICOMMI2CT_Regs     * const i2ct;
    UNICOMMSPI_Regs      * const spi;
    bool                   const fixedMode;
} UNICOMM_Inst_Regs;

static const UNICOMM_Inst_Regs UC4_Inst = {
    .inst      = (UNICOMM_Regs *) UC4_BASE,
    .uart      = (UNICOMMUART_Regs *) UC4_UART_BASE,
    .spi       = (UNICOMMSPI_Regs *) UC4_SPI_BASE,
    .fixedMode = false
};
static const UNICOMM_Inst_Regs UC6_Inst = {
    .inst      = (UNICOMM_Regs *) UC6_BASE,
    .i2cc      = (UNICOMMI2CC_Regs *) UC6_I2CC_BASE,
    .fixedMode = true
};
static const UNICOMM_Inst_Regs UC7_Inst = {
    .inst      = (UNICOMM_Regs *) UC7_BASE,
    .i2ct      = (UNICOMMI2CT_Regs *) UC7_I2CT_BASE,
    .fixedMode = true
};
static const UNICOMM_Inst_Regs UC8_Inst = {
    .inst      = (UNICOMM_Regs *) UC8_BASE,
    .uart      = (UNICOMMUART_Regs *) UC8_UART_BASE,
    .spi       = (UNICOMMSPI_Regs *) UC8_SPI_BASE,
    .fixedMode = false
};
static const UNICOMM_Inst_Regs UC11_Inst = {
    .inst      = (UNICOMM_Regs *) UC11_BASE,
    .uart      = (UNICOMMUART_Regs *) UC11_UART_BASE,
    .fixedMode = true
};


/*@}*/ /* end of group MSPM0L211X_MemoryMap */

/******************************************************************************
* Peripheral declarations                                                     *
******************************************************************************/
/** @addtogroup MSPM0L211X_PeripheralDecl MSPM0L211X Peripheral Declaration
  @{
*/

static ADC12_Regs                               * const ADC0                           = ((ADC12_Regs *) ADC0_BASE);
static COMP_Regs                                * const COMP0                          = ((COMP_Regs *) COMP0_BASE);
static VREF_Regs                                * const VREF                           = ((VREF_Regs *) VREF_BASE);
static LCD_Regs                                 * const LCD                            = ((LCD_Regs *) LCD_BASE);
static WWDT_Regs                                * const WWDT0                          = ((WWDT_Regs *) WWDT0_BASE);
static GPTIMER_Regs                             * const TIMG14                         = ((GPTIMER_Regs *) TIMG14_BASE);
static GPTIMER_Regs                             * const TIMG1                          = ((GPTIMER_Regs *) TIMG1_BASE);
static GPTIMER_Regs                             * const TIMG2                          = ((GPTIMER_Regs *) TIMG2_BASE);
static LFSS_Regs                                * const LFSS                           = ((LFSS_Regs *) LFSS_BASE);
static RTC_Regs                                 * const RTC_B                          = ((RTC_Regs *) RTC_B_BASE);
static GPIO_Regs                                * const GPIOA                          = ((GPIO_Regs *) GPIOA_BASE);
static GPIO_Regs                                * const GPIOB                          = ((GPIO_Regs *) GPIOB_BASE);
static KEYSTORECTL_Regs                         * const KEYSTORECTL                    = ((KEYSTORECTL_Regs *) KEYSTORECTL_BASE);
static SYSCTL_Regs                              * const SYSCTL                         = ((SYSCTL_Regs *) SYSCTL_BASE);
static BTIMER_Regs                              * const TIMB0                          = ((BTIMER_Regs *) TIMB0_BASE);
static BTIMER_Regs                              * const TIMB1                          = ((BTIMER_Regs *) TIMB1_BASE);
static BTIMER_Regs                              * const TIMB2                          = ((BTIMER_Regs *) TIMB2_BASE);
static BTIMER_Regs                              * const TIMB3                          = ((BTIMER_Regs *) TIMB3_BASE);
static DEBUGSS_Regs                             * const DEBUGSS                        = ((DEBUGSS_Regs *) DEBUGSS_BASE);
static FLASHCTL_Regs                            * const FLASHCTL                       = ((FLASHCTL_Regs *) FLASHCTL_BASE);
static CPUSS_Regs                               * const CPUSS                          = ((CPUSS_Regs *) CPUSS_BASE);
static WUC_Regs                                 * const WUC                            = ((WUC_Regs *) WUC_BASE);
static IOMUX_Regs                               * const IOMUX                          = ((IOMUX_Regs *) IOMUX_BASE);
static DMA_Regs                                 * const DMA                            = ((DMA_Regs *) DMA_BASE);
static CRC_Regs                                 * const CRC0                           = ((CRC_Regs *) CRC0_BASE);
static CRC_Regs                                 * const CRC                           = ((CRC_Regs *) CRC0_BASE);
static AESADV_Regs                              * const AESADV                         = ((AESADV_Regs *) AESADV_BASE);
static ADC12_PERIPHERALREGIONSVT_Regs           * const ADC0_SVT                       = ((ADC12_PERIPHERALREGIONSVT_Regs *) ADC0_SVT_BASE);
static GPTIMER_Regs                             * const TIMA0                          = ((GPTIMER_Regs *) TIMA0_BASE);
static UNICOMMUART_Regs                         * const UC4_UART                       = ((UNICOMMUART_Regs *) UC4_UART_BASE);
static UNICOMMI2CC_Regs                         * const UC6_I2CC                       = ((UNICOMMI2CC_Regs *) UC6_I2CC_BASE);
static UNICOMMI2CT_Regs                         * const UC7_I2CT                       = ((UNICOMMI2CT_Regs *) UC7_I2CT_BASE);
static UNICOMMSPI_Regs                          * const UC4_SPI                        = ((UNICOMMSPI_Regs *) UC4_SPI_BASE);
static UNICOMM_Inst_Regs                        * const UC4                            = ((UNICOMM_Inst_Regs *) &UC4_Inst);
static UNICOMM_Inst_Regs                        * const UC6                            = ((UNICOMM_Inst_Regs *) &UC6_Inst);
static UNICOMM_Inst_Regs                        * const UC7                            = ((UNICOMM_Inst_Regs *) &UC7_Inst);
static SPGSS_Regs                               * const SPG0                           = ((SPGSS_Regs *) SPG0_BASE);
static UNICOMMUART_Regs                         * const UC8_UART                       = ((UNICOMMUART_Regs *) UC8_UART_BASE);
static UNICOMMUART_Regs                         * const UC11_UART                      = ((UNICOMMUART_Regs *) UC11_UART_BASE);
static UNICOMMSPI_Regs                          * const UC8_SPI                        = ((UNICOMMSPI_Regs *) UC8_SPI_BASE);
static UNICOMM_Inst_Regs                        * const UC8                            = ((UNICOMM_Inst_Regs *) &UC8_Inst);
static UNICOMM_Inst_Regs                        * const UC11                           = ((UNICOMM_Inst_Regs *) &UC11_Inst);
static FACTORYREGION_OPEN_Regs                  * const FACTORYREGION                  = ((FACTORYREGION_OPEN_Regs *) FACTORYREGION_BASE);


/******************************************************************************
* SYS parameters                                                              *
******************************************************************************/

#define ADC_SYS_NUM_ANALOG_CHAN                       (32)      /* !< Number of analog channels. */
#define CRC0_SYS_CRC32_ENABLE                         (0)       /* !< Parameter to exclude or include 32-bit CRC. */
#define DMA_SYS_N_DMA_CHANNEL                         (3)       /* !< Number of DMA channels implemented in DMA. */
#define DMA_SYS_N_DMA_FULL_CHANNEL                    (2)       /* !< Number of FULL-DMA channels implemented in DMA. If
                                                      SYS_N_DMA_CHANNEL is larger than SYS_N_DMA_FULL_CHANNEL, the
                                                      remaining channel will be BASIC channel with limited functionality. */
#define DMA_SYS_MMR_AUTO                              (1)       /* !< Boolean for if auto enable channels implemented in DMA. */
#define DMA_SYS_MMR_EM                                (1)       /* !< Boolean for if extended mode channels implemented in DMA. */
#define DMA_SYS_MMR_LLONG                             (1)       /* !< Boolean for if channels implemented in DMA with 128-bit access. */
#define DMA_SYS_MMR_STRIDE                            (1)       /* !< Boolean for if channels implemented in DMA with stride mode. */
#define FLASHCTL_SYS_DATAWIDTH                        (64)      /* !< Data bit width of a single flash word:    Minimum: 64    Maximum:
                                                      128 */
#define FLASHCTL_SYS_WEPROTBWIDTH                     (16)      /* !< Bit width of WEPROTB register. */
#define FLASHCTL_SYS_WEPROTAWIDTH                     (0)       /* !< Bit width of WEPROTA register. */
#define FLASHCTL_SYS_WEPROTCWIDTH                     (0)       /* !< Bit width of WEPROTC register. */
#define COMP_SYS_DACOUT_EN                            (1)       /* !< Boolean for if 8-bit DAC output to pin is available */


/******************************************************************************
* DMA Triggers                                                                *
******************************************************************************/

/* External DMA Triggers */
#define DMA_SOFTWARE_TRIG                             (0)
#define DMA_GENERIC_SUB0_TRIG                         (1)
#define DMA_GENERIC_SUB1_TRIG                         (2)
#define DMA_AESADV_AES_0_TRIG                         (3)
#define DMA_AESADV_AES_1_TRIG                         (4)
#define DMA_UC4_RX_BD_TRIG                            (5)
#define DMA_UC4_TX_BD_TRIG                            (6)
#define DMA_UC6_RX_BD_TRIG                            (7)
#define DMA_UC6_TX_BD_TRIG                            (8)
#define DMA_UC7_RX_BD_TRIG                            (9)
#define DMA_UC7_TX_BD_TRIG                            (10)
#define DMA_UC8_RX_BD_TRIG                            (11)
#define DMA_UC8_TX_BD_TRIG                            (12)
#define DMA_ADC0_EVT_GEN_BD_TRIG                      (13)

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
  IOMUX_PINCM46        = (45),
  IOMUX_PINCM47        = (46),
  IOMUX_PINCM48        = (47),
  IOMUX_PINCM49        = (48),
  IOMUX_PINCM50        = (49),
  IOMUX_PINCM51        = (50),
  IOMUX_PINCM52        = (51),
  IOMUX_PINCM53        = (52),
  IOMUX_PINCM54        = (53),
  IOMUX_PINCM55        = (54),
  IOMUX_PINCM56        = (55),
  IOMUX_PINCM57        = (56),
  IOMUX_PINCM58        = (57),
  IOMUX_PINCM59        = (58),
  IOMUX_PINCM60        = (59),
} IOMUX_PINCM;


/*IOMUX_PINCM1[PF] Bits */
#define IOMUX_PINCM1_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM1_PF_GPIOA_DIO00                   ((uint32_t)0X00000001)
#define IOMUX_PINCM1_PF_UC4_PICO                      ((uint32_t)0X00000002)
#define IOMUX_PINCM1_PF_UC4_TX                        ((uint32_t)0X00000002)
#define IOMUX_PINCM1_PF_UC6_SDA                       ((uint32_t)0X00000003)
#define IOMUX_PINCM1_PF_TIMA0_CCP0                    ((uint32_t)0X00000004)
#define IOMUX_PINCM1_PF_TIMA_FAULT1                   ((uint32_t)0X00000005)
#define IOMUX_PINCM1_PF_SYSCTL_FCC_IN                 ((uint32_t)0X00000006)
#define IOMUX_PINCM1_PF_SYSCTL_BEEPER                 ((uint32_t)0X00000008)
#define IOMUX_PINCM1_PF_TIMG14_CCP0                   ((uint32_t)0X00000009)
#define IOMUX_PINCM1_PF_UC7_SDA                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM2[PF] Bits */
#define IOMUX_PINCM2_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM2_PF_GPIOA_DIO01                   ((uint32_t)0X00000001)
#define IOMUX_PINCM2_PF_UC4_SCLK                      ((uint32_t)0X00000002)
#define IOMUX_PINCM2_PF_UC4_RX                        ((uint32_t)0X00000002)
#define IOMUX_PINCM2_PF_UC6_SCL                       ((uint32_t)0X00000003)
#define IOMUX_PINCM2_PF_TIMA0_CCP1                    ((uint32_t)0X00000004)
#define IOMUX_PINCM2_PF_TIMA_FAULT2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM2_PF_TIMG14_CCP3                   ((uint32_t)0X00000006)
#define IOMUX_PINCM2_PF_TIMG14_CCP2                   ((uint32_t)0X00000007)
#define IOMUX_PINCM2_PF_TIMG14_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM2_PF_UC7_SCL                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM3[PF] Bits */
#define IOMUX_PINCM3_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM3_PF_GPIOA_DIO28                   ((uint32_t)0X00000001)
#define IOMUX_PINCM3_PF_UC4_PICO                      ((uint32_t)0X00000002)
#define IOMUX_PINCM3_PF_UC4_TX                        ((uint32_t)0X00000002)
#define IOMUX_PINCM3_PF_UC6_SDA                       ((uint32_t)0X00000003)
#define IOMUX_PINCM3_PF_TIMA0_CCP3                    ((uint32_t)0X00000004)
#define IOMUX_PINCM3_PF_TIMA_FAULT0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM3_PF_TIMG2_CCP0                    ((uint32_t)0X00000006)
#define IOMUX_PINCM3_PF_TIMA0_CCP1                    ((uint32_t)0X00000007)

/*IOMUX_PINCM4[PF] Bits */
#define IOMUX_PINCM4_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM4_PF_GPIOA_DIO29                   ((uint32_t)0X00000001)
#define IOMUX_PINCM4_PF_UC7_SCL                       ((uint32_t)0X00000002)
#define IOMUX_PINCM4_PF_TIMG14_CCP2                   ((uint32_t)0X00000004)
#define IOMUX_PINCM4_PF_TIMG1_CCP0                    ((uint32_t)0X00000005)
#define IOMUX_PINCM4_PF_UC4_CS0                       ((uint32_t)0X00000007)
#define IOMUX_PINCM4_PF_UC4_CTS                       ((uint32_t)0X00000007)

/*IOMUX_PINCM5[PF] Bits */
#define IOMUX_PINCM5_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM5_PF_GPIOA_DIO30                   ((uint32_t)0X00000001)
#define IOMUX_PINCM5_PF_UC7_SDA                       ((uint32_t)0X00000002)
#define IOMUX_PINCM5_PF_TIMG14_CCP3                   ((uint32_t)0X00000004)
#define IOMUX_PINCM5_PF_TIMG1_CCP1                    ((uint32_t)0X00000005)
#define IOMUX_PINCM5_PF_UC4_POCI                      ((uint32_t)0X00000007)
#define IOMUX_PINCM5_PF_UC4_RTS                       ((uint32_t)0X00000007)

/*IOMUX_PINCM6[PF] Bits */
#define IOMUX_PINCM6_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM6_PF_GPIOA_DIO31                   ((uint32_t)0X00000001)
#define IOMUX_PINCM6_PF_UC4_SCLK                      ((uint32_t)0X00000002)
#define IOMUX_PINCM6_PF_UC4_RX                        ((uint32_t)0X00000002)
#define IOMUX_PINCM6_PF_UC6_SCL                       ((uint32_t)0X00000003)
#define IOMUX_PINCM6_PF_TIMA0_CCP3_CMPL               ((uint32_t)0X00000004)
#define IOMUX_PINCM6_PF_SYSCTL_CLK_OUT                ((uint32_t)0X00000006)
#define IOMUX_PINCM6_PF_TIMG2_CCP1                    ((uint32_t)0X00000007)

/*IOMUX_PINCM7[PF] Bits */
#define IOMUX_PINCM7_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM7_PF_GPIOA_DIO02                   ((uint32_t)0X00000001)
#define IOMUX_PINCM7_PF_TIMG14_CCP3                   ((uint32_t)0X00000002)
#define IOMUX_PINCM7_PF_UC4_CS0                       ((uint32_t)0X00000003)
#define IOMUX_PINCM7_PF_UC4_CTS                       ((uint32_t)0X00000003)
#define IOMUX_PINCM7_PF_TIMG2_CCP1                    ((uint32_t)0X00000004)
#define IOMUX_PINCM7_PF_UC8_CS0                       ((uint32_t)0X00000005)
#define IOMUX_PINCM7_PF_TIMA0_CCP3_CMPL               ((uint32_t)0X00000006)
#define IOMUX_PINCM7_PF_TIMA0_CCP2_CMPL               ((uint32_t)0X00000007)
#define IOMUX_PINCM7_PF_TIMA_FAULT0                   ((uint32_t)0X00000008)
#define IOMUX_PINCM7_PF_TIMA_FAULT1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM7_PF_TIMA0_CCP0                    ((uint32_t)0X0000000B)

/*IOMUX_PINCM8[PF] Bits */
#define IOMUX_PINCM8_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM8_PF_GPIOA_DIO03                   ((uint32_t)0X00000001)
#define IOMUX_PINCM8_PF_TIMG14_CCP2                   ((uint32_t)0X00000002)
#define IOMUX_PINCM8_PF_UC7_SDA                       ((uint32_t)0X00000004)
#define IOMUX_PINCM8_PF_TIMA0_CCP1                    ((uint32_t)0X00000005)
#define IOMUX_PINCM8_PF_COMP0_OUT                     ((uint32_t)0X00000006)
#define IOMUX_PINCM8_PF_TIMG2_CCP0                    ((uint32_t)0X00000007)
#define IOMUX_PINCM8_PF_TIMA0_CCP2                    ((uint32_t)0X00000008)
#define IOMUX_PINCM8_PF_UC8_PICO                      ((uint32_t)0X0000000A)
#define IOMUX_PINCM8_PF_UC8_TX                        ((uint32_t)0X0000000A)

/*IOMUX_PINCM9[PF] Bits */
#define IOMUX_PINCM9_PF_UNCONNECTED                   ((uint32_t)0X00000000)
#define IOMUX_PINCM9_PF_GPIOA_DIO04                   ((uint32_t)0X00000001)
#define IOMUX_PINCM9_PF_TIMG14_CCP3                   ((uint32_t)0X00000002)
#define IOMUX_PINCM9_PF_UC4_POCI                      ((uint32_t)0X00000003)
#define IOMUX_PINCM9_PF_UC4_RTS                       ((uint32_t)0X00000003)
#define IOMUX_PINCM9_PF_UC7_SCL                       ((uint32_t)0X00000004)
#define IOMUX_PINCM9_PF_TIMA0_CCP1_CMPL               ((uint32_t)0X00000005)
#define IOMUX_PINCM9_PF_SYSCTL_LFCLKIN                ((uint32_t)0X00000006)
#define IOMUX_PINCM9_PF_TIMG2_CCP1                    ((uint32_t)0X00000007)
#define IOMUX_PINCM9_PF_TIMA0_CCP3                    ((uint32_t)0X00000008)
#define IOMUX_PINCM9_PF_UC8_SCLK                      ((uint32_t)0X0000000A)
#define IOMUX_PINCM9_PF_UC8_RX                        ((uint32_t)0X0000000A)
#define IOMUX_PINCM9_PF_UC4_CS0                       ((uint32_t)0X0000000B)
#define IOMUX_PINCM9_PF_UC4_CTS                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM10[PF] Bits */
#define IOMUX_PINCM10_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM10_PF_GPIOA_DIO05                  ((uint32_t)0X00000001)
#define IOMUX_PINCM10_PF_TIMG14_CCP2                  ((uint32_t)0X00000002)
#define IOMUX_PINCM10_PF_UC4_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM10_PF_UC4_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM10_PF_UC7_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM10_PF_TIMG14_CCP0                  ((uint32_t)0X00000005)
#define IOMUX_PINCM10_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000006)
#define IOMUX_PINCM10_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM10_PF_TIMA_FAULT1                  ((uint32_t)0X00000008)
#define IOMUX_PINCM10_PF_UC4_CS0                      ((uint32_t)0X00000009)
#define IOMUX_PINCM10_PF_UC4_CTS                      ((uint32_t)0X00000009)
#define IOMUX_PINCM10_PF_UC8_PICO                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM10_PF_UC8_TX                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM11[PF] Bits */
#define IOMUX_PINCM11_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM11_PF_GPIOA_DIO06                  ((uint32_t)0X00000001)
#define IOMUX_PINCM11_PF_TIMG14_CCP3                  ((uint32_t)0X00000002)
#define IOMUX_PINCM11_PF_UC4_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM11_PF_UC4_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM11_PF_UC7_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM11_PF_TIMG14_CCP1                  ((uint32_t)0X00000005)
#define IOMUX_PINCM11_PF_SYSCTL_HFCLKIN               ((uint32_t)0X00000006)
#define IOMUX_PINCM11_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM11_PF_TIMA_FAULT0                  ((uint32_t)0X00000008)
#define IOMUX_PINCM11_PF_UC4_POCI                     ((uint32_t)0X00000009)
#define IOMUX_PINCM11_PF_UC4_RTS                      ((uint32_t)0X00000009)
#define IOMUX_PINCM11_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X0000000A)
#define IOMUX_PINCM11_PF_UC8_SCLK                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM11_PF_UC8_RX                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM12[PF] Bits */
#define IOMUX_PINCM12_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM12_PF_GPIOB_DIO00                  ((uint32_t)0X00000001)
#define IOMUX_PINCM12_PF_UC4_PICO                     ((uint32_t)0X00000002)
#define IOMUX_PINCM12_PF_UC4_TX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM12_PF_UC6_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM12_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM12_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)

/*IOMUX_PINCM13[PF] Bits */
#define IOMUX_PINCM13_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM13_PF_GPIOB_DIO01                  ((uint32_t)0X00000001)
#define IOMUX_PINCM13_PF_UC4_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM13_PF_UC4_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM13_PF_UC6_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM13_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM13_PF_TIMG14_CCP1                  ((uint32_t)0X00000006)

/*IOMUX_PINCM14[PF] Bits */
#define IOMUX_PINCM14_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM14_PF_GPIOA_DIO07                  ((uint32_t)0X00000001)
#define IOMUX_PINCM14_PF_COMP0_OUT                    ((uint32_t)0X00000002)
#define IOMUX_PINCM14_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000003)
#define IOMUX_PINCM14_PF_TIMG14_CCP2                  ((uint32_t)0X00000004)
#define IOMUX_PINCM14_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM14_PF_TIMG14_CCP3                  ((uint32_t)0X00000006)
#define IOMUX_PINCM14_PF_TIMG2_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM14_PF_TIMA0_CCP1                   ((uint32_t)0X00000008)
#define IOMUX_PINCM14_PF_SYSCTL_FCC_IN                ((uint32_t)0X0000000A)
#define IOMUX_PINCM14_PF_UC4_POCI                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM14_PF_UC4_RTS                      ((uint32_t)0X0000000B)

/*IOMUX_PINCM15[PF] Bits */
#define IOMUX_PINCM15_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM15_PF_GPIOB_DIO02                  ((uint32_t)0X00000001)
#define IOMUX_PINCM15_PF_UC7_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM15_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM15_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM15_PF_UC11_TX                      ((uint32_t)0X00000008)
#define IOMUX_PINCM15_PF_SYSCTL_HFCLKIN               ((uint32_t)0X0000000A)
#define IOMUX_PINCM15_PF_UC4_PICO                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM15_PF_UC4_TX                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM16[PF] Bits */
#define IOMUX_PINCM16_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM16_PF_GPIOB_DIO03                  ((uint32_t)0X00000001)
#define IOMUX_PINCM16_PF_UC7_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM16_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM16_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM16_PF_UC11_RX                      ((uint32_t)0X00000008)
#define IOMUX_PINCM16_PF_TIMA0_CCP0                   ((uint32_t)0X0000000A)
#define IOMUX_PINCM16_PF_UC4_SCLK                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM16_PF_UC4_RX                       ((uint32_t)0X0000000B)

/*IOMUX_PINCM17[PF] Bits */
#define IOMUX_PINCM17_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM17_PF_GPIOB_DIO04                  ((uint32_t)0X00000001)
#define IOMUX_PINCM17_PF_UC8_PICO                     ((uint32_t)0X00000002)
#define IOMUX_PINCM17_PF_UC8_TX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM17_PF_TIMA0_CCP1                   ((uint32_t)0X00000004)
#define IOMUX_PINCM17_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM17_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)
#define IOMUX_PINCM17_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)

/*IOMUX_PINCM18[PF] Bits */
#define IOMUX_PINCM18_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM18_PF_GPIOB_DIO05                  ((uint32_t)0X00000001)
#define IOMUX_PINCM18_PF_UC8_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM18_PF_UC8_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM18_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X00000004)
#define IOMUX_PINCM18_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM18_PF_TIMG14_CCP1                  ((uint32_t)0X00000006)
#define IOMUX_PINCM18_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)

/*IOMUX_PINCM19[PF] Bits */
#define IOMUX_PINCM19_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM19_PF_GPIOA_DIO08                  ((uint32_t)0X00000001)
#define IOMUX_PINCM19_PF_UC8_PICO                     ((uint32_t)0X00000002)
#define IOMUX_PINCM19_PF_UC8_TX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM19_PF_UC4_CS0                      ((uint32_t)0X00000003)
#define IOMUX_PINCM19_PF_UC4_CTS                      ((uint32_t)0X00000003)
#define IOMUX_PINCM19_PF_UC6_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM19_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM19_PF_TIMA_FAULT2                  ((uint32_t)0X00000006)
#define IOMUX_PINCM19_PF_TIMA_FAULT0                  ((uint32_t)0X00000007)
#define IOMUX_PINCM19_PF_TIMG2_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM19_PF_SYSCTL_HFCLKIN               ((uint32_t)0X0000000A)
#define IOMUX_PINCM19_PF_UC4_POCI                     ((uint32_t)0X0000000B)
#define IOMUX_PINCM19_PF_UC4_RTS                      ((uint32_t)0X0000000B)

/*IOMUX_PINCM20[PF] Bits */
#define IOMUX_PINCM20_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM20_PF_GPIOA_DIO09                  ((uint32_t)0X00000001)
#define IOMUX_PINCM20_PF_UC8_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM20_PF_UC8_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM20_PF_UC4_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM20_PF_UC4_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM20_PF_UC6_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM20_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM20_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM20_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM20_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000008)
#define IOMUX_PINCM20_PF_TIMG2_CCP0                   ((uint32_t)0X00000009)
#define IOMUX_PINCM20_PF_UC4_CS0                      ((uint32_t)0X0000000B)
#define IOMUX_PINCM20_PF_UC4_CTS                      ((uint32_t)0X0000000B)

/*IOMUX_PINCM21[PF] Bits */
#define IOMUX_PINCM21_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM21_PF_GPIOA_DIO10                  ((uint32_t)0X00000001)
#define IOMUX_PINCM21_PF_UC4_PICO                     ((uint32_t)0X00000002)
#define IOMUX_PINCM21_PF_UC4_TX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM21_PF_UC4_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM21_PF_UC4_RTS                      ((uint32_t)0X00000003)
#define IOMUX_PINCM21_PF_UC6_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM21_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM21_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM21_PF_TIMG14_CCP0                  ((uint32_t)0X00000007)
#define IOMUX_PINCM21_PF_UC7_SDA                      ((uint32_t)0X00000008)
#define IOMUX_PINCM21_PF_TIMA_FAULT1                  ((uint32_t)0X0000000A)

/*IOMUX_PINCM22[PF] Bits */
#define IOMUX_PINCM22_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM22_PF_GPIOA_DIO11                  ((uint32_t)0X00000001)
#define IOMUX_PINCM22_PF_UC4_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM22_PF_UC4_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM22_PF_UC6_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM22_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM22_PF_COMP0_OUT                    ((uint32_t)0X00000006)
#define IOMUX_PINCM22_PF_TIMG14_CCP1                  ((uint32_t)0X00000007)
#define IOMUX_PINCM22_PF_UC7_SCL                      ((uint32_t)0X00000008)
#define IOMUX_PINCM22_PF_TIMA_FAULT0                  ((uint32_t)0X0000000A)

/*IOMUX_PINCM23[PF] Bits */
#define IOMUX_PINCM23_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM23_PF_GPIOB_DIO06                  ((uint32_t)0X00000001)
#define IOMUX_PINCM23_PF_UC8_PICO                     ((uint32_t)0X00000002)
#define IOMUX_PINCM23_PF_UC8_TX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM23_PF_UC8_CS0                      ((uint32_t)0X00000003)
#define IOMUX_PINCM23_PF_TIMG14_CCP2                  ((uint32_t)0X00000005)
#define IOMUX_PINCM23_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM23_PF_TIMA_FAULT2                  ((uint32_t)0X00000008)

/*IOMUX_PINCM24[PF] Bits */
#define IOMUX_PINCM24_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM24_PF_GPIOB_DIO07                  ((uint32_t)0X00000001)
#define IOMUX_PINCM24_PF_UC8_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM24_PF_UC8_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM24_PF_UC8_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM24_PF_TIMG14_CCP3                  ((uint32_t)0X00000005)
#define IOMUX_PINCM24_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM24_PF_LCD_LCDLFCLK                 ((uint32_t)0X00000008)

/*IOMUX_PINCM25[PF] Bits */
#define IOMUX_PINCM25_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM25_PF_GPIOB_DIO08                  ((uint32_t)0X00000001)
#define IOMUX_PINCM25_PF_UC8_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM25_PF_UC8_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM25_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM25_PF_COMP0_OUT                    ((uint32_t)0X00000006)
#define IOMUX_PINCM25_PF_TIMG1_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM25_PF_LCD_LCDSON                   ((uint32_t)0X00000008)

/*IOMUX_PINCM26[PF] Bits */
#define IOMUX_PINCM26_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM26_PF_GPIOB_DIO09                  ((uint32_t)0X00000001)
#define IOMUX_PINCM26_PF_UC8_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM26_PF_UC8_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM26_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM26_PF_TIMA0_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM26_PF_TIMG1_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM26_PF_LCD_LCDEN                    ((uint32_t)0X00000008)

/*IOMUX_PINCM27[PF] Bits */
#define IOMUX_PINCM27_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM27_PF_GPIOB_DIO10                  ((uint32_t)0X00000001)
#define IOMUX_PINCM27_PF_TIMG14_CCP0                  ((uint32_t)0X00000002)
#define IOMUX_PINCM27_PF_TIMG14_CCP2                  ((uint32_t)0X00000003)
#define IOMUX_PINCM27_PF_COMP0_OUT                    ((uint32_t)0X00000004)
#define IOMUX_PINCM27_PF_TIMG1_CCP0                   ((uint32_t)0X00000005)

/*IOMUX_PINCM28[PF] Bits */
#define IOMUX_PINCM28_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM28_PF_GPIOB_DIO11                  ((uint32_t)0X00000001)
#define IOMUX_PINCM28_PF_TIMG14_CCP1                  ((uint32_t)0X00000002)
#define IOMUX_PINCM28_PF_TIMG14_CCP3                  ((uint32_t)0X00000003)
#define IOMUX_PINCM28_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000004)
#define IOMUX_PINCM28_PF_TIMG1_CCP1                   ((uint32_t)0X00000005)

/*IOMUX_PINCM29[PF] Bits */
#define IOMUX_PINCM29_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM29_PF_GPIOB_DIO12                  ((uint32_t)0X00000001)
#define IOMUX_PINCM29_PF_TIMA0_CCP2                   ((uint32_t)0X00000003)
#define IOMUX_PINCM29_PF_TIMA_FAULT1                  ((uint32_t)0X00000004)
#define IOMUX_PINCM29_PF_TIMA0_CCP1                   ((uint32_t)0X00000005)

/*IOMUX_PINCM30[PF] Bits */
#define IOMUX_PINCM30_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM30_PF_GPIOB_DIO13                  ((uint32_t)0X00000001)
#define IOMUX_PINCM30_PF_TIMA0_CCP3                   ((uint32_t)0X00000003)
#define IOMUX_PINCM30_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM30_PF_UC8_CS0                      ((uint32_t)0X00000007)

/*IOMUX_PINCM31[PF] Bits */
#define IOMUX_PINCM31_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM31_PF_GPIOB_DIO14                  ((uint32_t)0X00000001)
#define IOMUX_PINCM31_PF_UC8_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM31_PF_UC8_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM31_PF_UC8_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM31_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000004)
#define IOMUX_PINCM31_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM31_PF_TIMG14_CCP2                  ((uint32_t)0X00000006)
#define IOMUX_PINCM31_PF_TIMG14_CCP0                  ((uint32_t)0X00000007)
#define IOMUX_PINCM31_PF_COMP0_OUT                    ((uint32_t)0X0000000A)

/*IOMUX_PINCM32[PF] Bits */
#define IOMUX_PINCM32_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM32_PF_GPIOB_DIO15                  ((uint32_t)0X00000001)
#define IOMUX_PINCM32_PF_UC11_TX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM32_PF_UC8_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM32_PF_UC8_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM32_PF_UC4_POCI                     ((uint32_t)0X00000004)
#define IOMUX_PINCM32_PF_UC4_RTS                      ((uint32_t)0X00000004)
#define IOMUX_PINCM32_PF_TIMG14_CCP2                  ((uint32_t)0X00000005)
#define IOMUX_PINCM32_PF_TIMG2_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM32_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000007)
#define IOMUX_PINCM32_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000008)
#define IOMUX_PINCM32_PF_TIMG14_CCP1                  ((uint32_t)0X00000009)
#define IOMUX_PINCM32_PF_UC8_CS0                      ((uint32_t)0X0000000A)

/*IOMUX_PINCM33[PF] Bits */
#define IOMUX_PINCM33_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM33_PF_GPIOB_DIO16                  ((uint32_t)0X00000001)
#define IOMUX_PINCM33_PF_UC11_RX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM33_PF_UC8_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM33_PF_UC8_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM33_PF_UC4_CS0                      ((uint32_t)0X00000004)
#define IOMUX_PINCM33_PF_UC4_CTS                      ((uint32_t)0X00000004)
#define IOMUX_PINCM33_PF_TIMG14_CCP3                  ((uint32_t)0X00000005)
#define IOMUX_PINCM33_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM33_PF_UC4_PICO                     ((uint32_t)0X00000007)
#define IOMUX_PINCM33_PF_UC4_TX                       ((uint32_t)0X00000007)
#define IOMUX_PINCM33_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000008)

/*IOMUX_PINCM34[PF] Bits */
#define IOMUX_PINCM34_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM34_PF_GPIOA_DIO12                  ((uint32_t)0X00000001)
#define IOMUX_PINCM34_PF_UC4_POCI                     ((uint32_t)0X00000002)
#define IOMUX_PINCM34_PF_UC4_RTS                      ((uint32_t)0X00000002)
#define IOMUX_PINCM34_PF_UC4_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM34_PF_UC4_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM34_PF_COMP0_OUT                    ((uint32_t)0X00000004)
#define IOMUX_PINCM34_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM34_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000006)
#define IOMUX_PINCM34_PF_TIMG14_CCP0                  ((uint32_t)0X00000007)
#define IOMUX_PINCM34_PF_UC7_SCL                      ((uint32_t)0X00000008)
#define IOMUX_PINCM34_PF_TIMA0_CCP2                   ((uint32_t)0X00000009)

/*IOMUX_PINCM35[PF] Bits */
#define IOMUX_PINCM35_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM35_PF_GPIOA_DIO13                  ((uint32_t)0X00000001)
#define IOMUX_PINCM35_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000002)
#define IOMUX_PINCM35_PF_UC4_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM35_PF_UC4_RTS                      ((uint32_t)0X00000003)
#define IOMUX_PINCM35_PF_UC7_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM35_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM35_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000006)
#define IOMUX_PINCM35_PF_TIMG14_CCP1                  ((uint32_t)0X00000007)
#define IOMUX_PINCM35_PF_UC8_CS0                      ((uint32_t)0X00000008)
#define IOMUX_PINCM35_PF_UC8_POCI                     ((uint32_t)0X00000009)
#define IOMUX_PINCM35_PF_UC11_TX                      ((uint32_t)0X0000000A)
#define IOMUX_PINCM35_PF_COMP0_OUT                    ((uint32_t)0X0000000B)

/*IOMUX_PINCM36[PF] Bits */
#define IOMUX_PINCM36_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM36_PF_GPIOA_DIO14                  ((uint32_t)0X00000001)
#define IOMUX_PINCM36_PF_UC4_CS0                      ((uint32_t)0X00000002)
#define IOMUX_PINCM36_PF_UC4_CTS                      ((uint32_t)0X00000002)
#define IOMUX_PINCM36_PF_UC4_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM36_PF_UC4_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM36_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM36_PF_UC11_RX                      ((uint32_t)0X0000000A)

/*IOMUX_PINCM37[PF] Bits */
#define IOMUX_PINCM37_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM37_PF_GPIOA_DIO15                  ((uint32_t)0X00000001)
#define IOMUX_PINCM37_PF_UC4_POCI                     ((uint32_t)0X00000002)
#define IOMUX_PINCM37_PF_UC4_RTS                      ((uint32_t)0X00000002)
#define IOMUX_PINCM37_PF_UC7_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM37_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM37_PF_TIMG14_CCP2                  ((uint32_t)0X00000007)
#define IOMUX_PINCM37_PF_LCD_LCDEN                    ((uint32_t)0X00000009)

/*IOMUX_PINCM38[PF] Bits */
#define IOMUX_PINCM38_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM38_PF_GPIOA_DIO16                  ((uint32_t)0X00000001)
#define IOMUX_PINCM38_PF_COMP0_OUT                    ((uint32_t)0X00000002)
#define IOMUX_PINCM38_PF_UC8_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM38_PF_UC7_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM38_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM38_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000007)
#define IOMUX_PINCM38_PF_LCD_LCDSON                   ((uint32_t)0X00000009)

/*IOMUX_PINCM39[PF] Bits */
#define IOMUX_PINCM39_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM39_PF_GPIOA_DIO17                  ((uint32_t)0X00000001)
#define IOMUX_PINCM39_PF_UC8_PICO                     ((uint32_t)0X00000002)
#define IOMUX_PINCM39_PF_UC8_TX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM39_PF_UC8_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM39_PF_UC8_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM39_PF_UC7_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM39_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM39_PF_TIMG2_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM39_PF_TIMG14_CCP2                  ((uint32_t)0X00000007)
#define IOMUX_PINCM39_PF_LCD_LCDLFCLK                 ((uint32_t)0X0000000A)

/*IOMUX_PINCM40[PF] Bits */
#define IOMUX_PINCM40_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM40_PF_GPIOA_DIO18                  ((uint32_t)0X00000001)
#define IOMUX_PINCM40_PF_UC8_SCLK                     ((uint32_t)0X00000002)
#define IOMUX_PINCM40_PF_UC8_RX                       ((uint32_t)0X00000002)
#define IOMUX_PINCM40_PF_UC8_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM40_PF_UC8_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM40_PF_UC7_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM40_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM40_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM40_PF_TIMG14_CCP3                  ((uint32_t)0X00000007)
#define IOMUX_PINCM40_PF_UC4_CS0                      ((uint32_t)0X00000009)
#define IOMUX_PINCM40_PF_UC4_CTS                      ((uint32_t)0X00000009)
#define IOMUX_PINCM40_PF_LCD_LCDEN                    ((uint32_t)0X0000000A)

/*IOMUX_PINCM41[PF] Bits */
#define IOMUX_PINCM41_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM41_PF_GPIOA_DIO19                  ((uint32_t)0X00000001)
#define IOMUX_PINCM41_PF_DEBUGSS_SWDIO                ((uint32_t)0X00000002)
#define IOMUX_PINCM41_PF_UC8_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM41_PF_UC7_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM41_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM41_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)

/*IOMUX_PINCM42[PF] Bits */
#define IOMUX_PINCM42_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM42_PF_GPIOA_DIO20                  ((uint32_t)0X00000001)
#define IOMUX_PINCM42_PF_DEBUGSS_SWCLK                ((uint32_t)0X00000002)
#define IOMUX_PINCM42_PF_UC8_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM42_PF_UC8_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM42_PF_UC7_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM42_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM42_PF_TIMG14_CCP1                  ((uint32_t)0X00000006)

/*IOMUX_PINCM43[PF] Bits */
#define IOMUX_PINCM43_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM43_PF_GPIOB_DIO17                  ((uint32_t)0X00000001)
#define IOMUX_PINCM43_PF_UC11_TX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM43_PF_UC4_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM43_PF_UC4_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM43_PF_UC6_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM43_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM43_PF_TIMG14_CCP0                  ((uint32_t)0X00000006)
#define IOMUX_PINCM43_PF_TIMG1_CCP0                   ((uint32_t)0X00000009)
#define IOMUX_PINCM43_PF_LCD_LCDSON                   ((uint32_t)0X0000000A)

/*IOMUX_PINCM44[PF] Bits */
#define IOMUX_PINCM44_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM44_PF_GPIOB_DIO18                  ((uint32_t)0X00000001)
#define IOMUX_PINCM44_PF_UC11_RX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM44_PF_UC4_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM44_PF_UC4_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM44_PF_UC6_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM44_PF_TIMA0_CCP2_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM44_PF_TIMG14_CCP1                  ((uint32_t)0X00000006)
#define IOMUX_PINCM44_PF_TIMG1_CCP1                   ((uint32_t)0X00000009)
#define IOMUX_PINCM44_PF_LCD_LCDLFCLK                 ((uint32_t)0X0000000A)

/*IOMUX_PINCM45[PF] Bits */
#define IOMUX_PINCM45_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM45_PF_GPIOB_DIO19                  ((uint32_t)0X00000001)
#define IOMUX_PINCM45_PF_COMP0_OUT                    ((uint32_t)0X00000002)
#define IOMUX_PINCM45_PF_UC4_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM45_PF_UC4_RTS                      ((uint32_t)0X00000003)
#define IOMUX_PINCM45_PF_UC4_CS0                      ((uint32_t)0X00000005)
#define IOMUX_PINCM45_PF_UC4_CTS                      ((uint32_t)0X00000005)
#define IOMUX_PINCM45_PF_TIMG2_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM45_PF_TIMG14_CCP2                  ((uint32_t)0X00000007)

/*IOMUX_PINCM46[PF] Bits */
#define IOMUX_PINCM46_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM46_PF_GPIOA_DIO21                  ((uint32_t)0X00000001)
#define IOMUX_PINCM46_PF_UC11_TX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM46_PF_TIMA0_CCP0                   ((uint32_t)0X00000005)
#define IOMUX_PINCM46_PF_TIMG1_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM46_PF_TIMG14_CCP2                  ((uint32_t)0X0000000A)

/*IOMUX_PINCM47[PF] Bits */
#define IOMUX_PINCM47_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM47_PF_GPIOA_DIO22                  ((uint32_t)0X00000001)
#define IOMUX_PINCM47_PF_UC11_RX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM47_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM47_PF_TIMG1_CCP1                   ((uint32_t)0X00000006)
#define IOMUX_PINCM47_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM47_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000008)
#define IOMUX_PINCM47_PF_UC6_SCL                      ((uint32_t)0X00000009)
#define IOMUX_PINCM47_PF_TIMG14_CCP3                  ((uint32_t)0X0000000A)

/*IOMUX_PINCM48[PF] Bits */
#define IOMUX_PINCM48_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM48_PF_GPIOB_DIO20                  ((uint32_t)0X00000001)
#define IOMUX_PINCM48_PF_UC8_CS0                      ((uint32_t)0X00000003)
#define IOMUX_PINCM48_PF_TIMA0_CCP2                   ((uint32_t)0X00000005)
#define IOMUX_PINCM48_PF_TIMA_FAULT1                  ((uint32_t)0X00000006)
#define IOMUX_PINCM48_PF_TIMA0_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM48_PF_UC6_SDA                      ((uint32_t)0X00000009)

/*IOMUX_PINCM49[PF] Bits */
#define IOMUX_PINCM49_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM49_PF_GPIOB_DIO21                  ((uint32_t)0X00000001)
#define IOMUX_PINCM49_PF_UC8_POCI                     ((uint32_t)0X00000003)
#define IOMUX_PINCM49_PF_UC6_SCL                      ((uint32_t)0X00000004)
#define IOMUX_PINCM49_PF_TIMG14_CCP2                  ((uint32_t)0X00000005)
#define IOMUX_PINCM49_PF_UC8_PICO                     ((uint32_t)0X00000006)
#define IOMUX_PINCM49_PF_UC8_TX                       ((uint32_t)0X00000006)

/*IOMUX_PINCM50[PF] Bits */
#define IOMUX_PINCM50_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM50_PF_GPIOB_DIO22                  ((uint32_t)0X00000001)
#define IOMUX_PINCM50_PF_UC8_PICO                     ((uint32_t)0X00000003)
#define IOMUX_PINCM50_PF_UC8_TX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM50_PF_UC6_SDA                      ((uint32_t)0X00000004)
#define IOMUX_PINCM50_PF_TIMG14_CCP3                  ((uint32_t)0X00000005)
#define IOMUX_PINCM50_PF_UC8_SCLK                     ((uint32_t)0X00000006)
#define IOMUX_PINCM50_PF_UC8_RX                       ((uint32_t)0X00000006)

/*IOMUX_PINCM51[PF] Bits */
#define IOMUX_PINCM51_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM51_PF_GPIOB_DIO23                  ((uint32_t)0X00000001)
#define IOMUX_PINCM51_PF_UC8_SCLK                     ((uint32_t)0X00000003)
#define IOMUX_PINCM51_PF_UC8_RX                       ((uint32_t)0X00000003)
#define IOMUX_PINCM51_PF_TIMA_FAULT0                  ((uint32_t)0X00000004)
#define IOMUX_PINCM51_PF_COMP0_OUT                    ((uint32_t)0X00000005)

/*IOMUX_PINCM52[PF] Bits */
#define IOMUX_PINCM52_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM52_PF_GPIOB_DIO24                  ((uint32_t)0X00000001)
#define IOMUX_PINCM52_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM52_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X00000006)

/*IOMUX_PINCM53[PF] Bits */
#define IOMUX_PINCM53_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM53_PF_GPIOA_DIO23                  ((uint32_t)0X00000001)
#define IOMUX_PINCM53_PF_UC11_TX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM53_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM53_PF_TIMG14_CCP2                  ((uint32_t)0X00000006)
#define IOMUX_PINCM53_PF_TIMG2_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM53_PF_TIMG14_CCP0                  ((uint32_t)0X00000009)

/*IOMUX_PINCM54[PF] Bits */
#define IOMUX_PINCM54_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM54_PF_GPIOA_DIO24                  ((uint32_t)0X00000001)
#define IOMUX_PINCM54_PF_UC11_RX                      ((uint32_t)0X00000002)
#define IOMUX_PINCM54_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM54_PF_TIMG14_CCP3                  ((uint32_t)0X00000006)
#define IOMUX_PINCM54_PF_TIMG2_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM54_PF_TIMG14_CCP1                  ((uint32_t)0X00000009)

/*IOMUX_PINCM55[PF] Bits */
#define IOMUX_PINCM55_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM55_PF_GPIOA_DIO25                  ((uint32_t)0X00000001)
#define IOMUX_PINCM55_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM55_PF_TIMA0_CCP1_CMPL              ((uint32_t)0X00000006)
#define IOMUX_PINCM55_PF_COMP0_OUT                    ((uint32_t)0X00000007)
#define IOMUX_PINCM55_PF_TIMG1_CCP0                   ((uint32_t)0X0000000A)

/*IOMUX_PINCM56[PF] Bits */
#define IOMUX_PINCM56_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM56_PF_GPIOB_DIO25                  ((uint32_t)0X00000001)
#define IOMUX_PINCM56_PF_UC4_CS0                      ((uint32_t)0X00000002)
#define IOMUX_PINCM56_PF_UC4_CTS                      ((uint32_t)0X00000002)
#define IOMUX_PINCM56_PF_TIMA_FAULT0                  ((uint32_t)0X00000004)
#define IOMUX_PINCM56_PF_TIMA_FAULT1                  ((uint32_t)0X00000005)
#define IOMUX_PINCM56_PF_TIMA_FAULT2                  ((uint32_t)0X00000006)
#define IOMUX_PINCM56_PF_COMP0_OUT                    ((uint32_t)0X00000007)
#define IOMUX_PINCM56_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000008)

/*IOMUX_PINCM57[PF] Bits */
#define IOMUX_PINCM57_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM57_PF_GPIOB_DIO26                  ((uint32_t)0X00000001)
#define IOMUX_PINCM57_PF_UC4_POCI                     ((uint32_t)0X00000002)
#define IOMUX_PINCM57_PF_UC4_RTS                      ((uint32_t)0X00000002)
#define IOMUX_PINCM57_PF_TIMA0_CCP0                   ((uint32_t)0X00000004)
#define IOMUX_PINCM57_PF_TIMA0_CCP3                   ((uint32_t)0X00000005)
#define IOMUX_PINCM57_PF_TIMG1_CCP0                   ((uint32_t)0X00000006)
#define IOMUX_PINCM57_PF_COMP0_OUT                    ((uint32_t)0X00000007)
#define IOMUX_PINCM57_PF_SYSCTL_FCC_IN                ((uint32_t)0X00000008)

/*IOMUX_PINCM58[PF] Bits */
#define IOMUX_PINCM58_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM58_PF_GPIOB_DIO27                  ((uint32_t)0X00000001)
#define IOMUX_PINCM58_PF_COMP0_OUT                    ((uint32_t)0X00000002)
#define IOMUX_PINCM58_PF_TIMA0_CCP0_CMPL              ((uint32_t)0X00000004)
#define IOMUX_PINCM58_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000005)
#define IOMUX_PINCM58_PF_TIMG1_CCP1                   ((uint32_t)0X00000006)

/*IOMUX_PINCM59[PF] Bits */
#define IOMUX_PINCM59_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM59_PF_GPIOA_DIO26                  ((uint32_t)0X00000001)
#define IOMUX_PINCM59_PF_UC8_CS0                      ((uint32_t)0X00000003)
#define IOMUX_PINCM59_PF_TIMG14_CCP2                  ((uint32_t)0X00000004)
#define IOMUX_PINCM59_PF_TIMA_FAULT0                  ((uint32_t)0X00000005)
#define IOMUX_PINCM59_PF_TIMA0_CCP3_CMPL              ((uint32_t)0X00000006)
#define IOMUX_PINCM59_PF_TIMG2_CCP0                   ((uint32_t)0X00000007)
#define IOMUX_PINCM59_PF_TIMG1_CCP1                   ((uint32_t)0X0000000A)

/*IOMUX_PINCM60[PF] Bits */
#define IOMUX_PINCM60_PF_UNCONNECTED                  ((uint32_t)0X00000000)
#define IOMUX_PINCM60_PF_GPIOA_DIO27                  ((uint32_t)0X00000001)
#define IOMUX_PINCM60_PF_TIMG14_CCP3                  ((uint32_t)0X00000004)
#define IOMUX_PINCM60_PF_TIMA_FAULT2                  ((uint32_t)0X00000005)
#define IOMUX_PINCM60_PF_SYSCTL_CLK_OUT               ((uint32_t)0X00000006)
#define IOMUX_PINCM60_PF_TIMG2_CCP1                   ((uint32_t)0X00000007)
#define IOMUX_PINCM60_PF_LFSS_RTC_OUT                 ((uint32_t)0X00000008)
#define IOMUX_PINCM60_PF_COMP0_OUT                    ((uint32_t)0X00000009)


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

/*@}*/ /* end of group MSPM0L211X_PeripheralDecl */

/*@}*/ /* end of group MSPM0L211X_Definitions */

#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_m0p_mspm0l211x__include */

