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

#ifndef ti_devices_msp_peripherals_hw_spgss__include
#define ti_devices_msp_peripherals_hw_spgss__include

/* Filename: hw_spgss.h */
/* Revised: 2024-07-09 09:23:19 */
/* Revision: 3060da33b0a30a38b851efb52ec3edd92cebbf75 */

#ifndef __CORTEX_M
  #ifdef __cplusplus
    #define __I  volatile        /*!< Defines 'read only' permissions */
  #else
    #define __I  volatile const  /*!< Defines 'read only' permissions */
  #endif
  #define __O  volatile          /*!< Defines 'write only' permissions */
  #define __IO  volatile         /*!< Defines 'read / write' permissions */

  /* following defines should be used for structure members */
  #define __IM  volatile const   /*! Defines 'read only' structure member permissions */
  #define __OM  volatile         /*! Defines 'write only' structure member permissions */
  #define __IOM  volatile        /*! Defines 'read / write' structure member permissions */
#endif

/* Use standard integer types with explicit width */
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#if defined ( __CC_ARM )
#pragma anon_unions
#endif

/******************************************************************************
* SPGSS Registers
******************************************************************************/
#define SPGSS_I2C_PAIR_OFS                       ((uint32_t)0x000011C0U)
#define SPGSS_UNICOMM_LPBK_OFS                   ((uint32_t)0x00001120U)
#define SPGSS_DMA_CHAIN_OFS                      ((uint32_t)0x00001100U)
#define SPGSS_DMA_CHAIN1_OFS                     ((uint32_t)0x00001080U)
#define SPGSS_DMA_CHAIN0_OFS                     ((uint32_t)0x00001050U)
#define SPGSS_CPU_INT_OFS                        ((uint32_t)0x00001020U)


/** @addtogroup SPGSS_I2C_PAIR
  @{
*/

typedef struct {
  __IO uint32_t PAIR0;                             /* !< (@ 0x000011C0) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR1;                             /* !< (@ 0x000011C4) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR2;                             /* !< (@ 0x000011C8) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR3;                             /* !< (@ 0x000011CC) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR4;                             /* !< (@ 0x000011D0) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR5;                             /* !< (@ 0x000011D4) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR6;                             /* !< (@ 0x000011D8) Pairing of I2C for MULTI-MASTER/SMBUS applications */
  __IO uint32_t PAIR7;                             /* !< (@ 0x000011DC) Pairing of I2C for MULTI-MASTER/SMBUS applications */
} SPGSS_I2C_PAIR_Regs;

/*@}*/ /* end of group SPGSS_I2C_PAIR */

/** @addtogroup SPGSS_UNICOMM_LPBK
  @{
*/

typedef struct {
  __IO uint32_t LPBK0;                             /* !< (@ 0x00001120) Loopback between SPG instances  User has to ensure
                                                      that the SPGs looped back have the same underlying IP instantiated;
                                                      i.e. both UART, both SPI, both I2C, etc., */
  __IO uint32_t LPBK1;                             /* !< (@ 0x00001124) Loopback between SPG instances  User has to ensure
                                                      that the SPGs looped back have the same underlying IP instantiated;
                                                      i.e. both UART, both SPI, both I2C, etc., */
} SPGSS_UNICOMM_LPBK_Regs;

/*@}*/ /* end of group SPGSS_UNICOMM_LPBK */

/** @addtogroup SPGSS_DMA_CHAIN
  @{
*/

typedef struct {
  __IO uint32_t DMACHAIN0;                         /* !< (@ 0x00001100) Select SPG FIFO for system DMA chaining */
  __IO uint32_t DMACHAIN1;                         /* !< (@ 0x00001104) Select SPG FIFO for system DMA chaining */
} SPGSS_DMA_CHAIN_Regs;

/*@}*/ /* end of group SPGSS_DMA_CHAIN */

/** @addtogroup SPGSS_DMA_CHAIN1
  @{
*/

typedef struct {
       uint32_t RESERVED0[2];
  __IO uint32_t IMASK;                             /* !< (@ 0x00001088) Interrupt mask */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001090) Raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001098) Masked interrupt status */
       uint32_t RESERVED3[3];
  __O  uint32_t ICLR;                              /* !< (@ 0x000010A8) Interrupt clear */
} SPGSS_DMA_CHAIN1_Regs;

/*@}*/ /* end of group SPGSS_DMA_CHAIN1 */

/** @addtogroup SPGSS_DMA_CHAIN0
  @{
*/

typedef struct {
       uint32_t RESERVED0[2];
  __IO uint32_t IMASK;                             /* !< (@ 0x00001058) Interrupt mask */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001060) Raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001068) Masked interrupt status */
       uint32_t RESERVED3[3];
  __O  uint32_t ICLR;                              /* !< (@ 0x00001078) Interrupt clear */
} SPGSS_DMA_CHAIN0_Regs;

/*@}*/ /* end of group SPGSS_DMA_CHAIN0 */

/** @addtogroup SPGSS_CPU_INT
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) Interrupt index */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) Interrupt mask */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) Raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) Masked interrupt status */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) Interrupt set */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) Interrupt clear */
} SPGSS_CPU_INT_Regs;

/*@}*/ /* end of group SPGSS_CPU_INT */

/** @addtogroup SPGSS
  @{
*/

typedef struct {
       uint32_t RESERVED0[1032];
  SPGSS_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) */
       uint32_t RESERVED1;
  SPGSS_DMA_CHAIN0_Regs  DMA_CHAIN0;                        /* !< (@ 0x00001050) */
       uint32_t RESERVED2;
  SPGSS_DMA_CHAIN1_Regs  DMA_CHAIN1;                        /* !< (@ 0x00001080) */
       uint32_t RESERVED3[21];
  SPGSS_DMA_CHAIN_Regs  DMA_CHAIN;                         /* !< (@ 0x00001100) */
       uint32_t RESERVED4[6];
  SPGSS_UNICOMM_LPBK_Regs  UNICOMM_LPBK;                      /* !< (@ 0x00001120) */
       uint32_t RESERVED5[38];
  SPGSS_I2C_PAIR_Regs  I2C_PAIR;                          /* !< (@ 0x000011C0) */
} SPGSS_Regs;

/*@}*/ /* end of group SPGSS */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* SPGSS Register Control Bits
******************************************************************************/

/* SPGSS_PAIR0 Bits */
/* SPGSS_PAIR0[CTL] Bits */
#define SPGSS_PAIR0_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR0_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR0_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR0_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR0[TARGET] Bits */
#define SPGSS_PAIR0_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR0_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR0_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR0_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR0[EN] Bits */
#define SPGSS_PAIR0_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR0_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR0_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR0_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR1 Bits */
/* SPGSS_PAIR1[CTL] Bits */
#define SPGSS_PAIR1_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR1_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR1_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR1_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR1[TARGET] Bits */
#define SPGSS_PAIR1_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR1_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR1_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR1_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR1[EN] Bits */
#define SPGSS_PAIR1_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR1_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR1_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR1_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR2 Bits */
/* SPGSS_PAIR2[CTL] Bits */
#define SPGSS_PAIR2_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR2_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR2_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR2_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR2[TARGET] Bits */
#define SPGSS_PAIR2_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR2_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR2_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR2_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR2[EN] Bits */
#define SPGSS_PAIR2_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR2_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR2_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR2_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR3 Bits */
/* SPGSS_PAIR3[CTL] Bits */
#define SPGSS_PAIR3_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR3_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR3_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR3_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR3[TARGET] Bits */
#define SPGSS_PAIR3_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR3_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR3_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR3_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR3[EN] Bits */
#define SPGSS_PAIR3_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR3_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR3_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR3_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR4 Bits */
/* SPGSS_PAIR4[CTL] Bits */
#define SPGSS_PAIR4_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR4_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR4_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR4_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR4[TARGET] Bits */
#define SPGSS_PAIR4_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR4_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR4_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR4_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR4[EN] Bits */
#define SPGSS_PAIR4_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR4_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR4_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR4_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR5 Bits */
/* SPGSS_PAIR5[CTL] Bits */
#define SPGSS_PAIR5_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR5_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR5_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR5_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR5[TARGET] Bits */
#define SPGSS_PAIR5_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR5_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR5_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR5_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR5[EN] Bits */
#define SPGSS_PAIR5_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR5_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR5_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR5_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR6 Bits */
/* SPGSS_PAIR6[CTL] Bits */
#define SPGSS_PAIR6_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR6_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR6_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR6_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR6[TARGET] Bits */
#define SPGSS_PAIR6_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR6_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR6_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR6_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR6[EN] Bits */
#define SPGSS_PAIR6_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR6_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR6_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR6_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_PAIR7 Bits */
/* SPGSS_PAIR7[CTL] Bits */
#define SPGSS_PAIR7_CTL_OFS                      (8)                             /* !< CTL Offset */
#define SPGSS_PAIR7_CTL_MASK                     ((uint32_t)0x00000F00U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    controller. */
#define SPGSS_PAIR7_CTL_SPG1                     ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG2                     ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG3                     ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG4                     ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG5                     ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG6                     ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG7                     ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG8                     ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG9                     ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG10                    ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG11                    ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG12                    ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG13                    ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG14                    ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG15                    ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_PAIR7_CTL_SPG16                    ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_PAIR7[TARGET] Bits */
#define SPGSS_PAIR7_TARGET_OFS                   (12)                            /* !< TARGET Offset */
#define SPGSS_PAIR7_TARGET_MASK                  ((uint32_t)0x0000F000U)         /* !< UNICOMM instance for loopback. This
                                                                                    instance will be treated as the
                                                                                    target */
#define SPGSS_PAIR7_TARGET_SPG1                  ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG2                  ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG3                  ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG4                  ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG5                  ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG6                  ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG7                  ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG8                  ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG9                  ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG10                 ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG11                 ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG12                 ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG13                 ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG14                 ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG15                 ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_PAIR7_TARGET_SPG16                 ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */
/* SPGSS_PAIR7[EN] Bits */
#define SPGSS_PAIR7_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_PAIR7_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Enable */
#define SPGSS_PAIR7_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_PAIR7_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_LPBK0 Bits */
/* SPGSS_LPBK0[CONTROLLER] Bits */
#define SPGSS_LPBK0_CONTROLLER_OFS               (8)                             /* !< CONTROLLER Offset */
#define SPGSS_LPBK0_CONTROLLER_MASK              ((uint32_t)0x00000F00U)         /* !< Select SPG controller for loopback.
                                                                                    Signals invovled in loopback based
                                                                                    on interface chosen 1. SPI : CS,
                                                                                    SCLK, SDI, SDO  2. I2C : SCL, SDA 3.
                                                                                    UART : TX, RX */
#define SPGSS_LPBK0_CONTROLLER_SPG1              ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG2              ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG3              ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG4              ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG5              ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG6              ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG7              ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG8              ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG9              ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG10             ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG11             ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG12             ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG13             ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG14             ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG15             ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK0_CONTROLLER_SPG16             ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_LPBK0[EN] Bits */
#define SPGSS_LPBK0_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_LPBK0_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Loopback  enable */
#define SPGSS_LPBK0_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_LPBK0_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */
/* SPGSS_LPBK0[PERIPHERAL] Bits */
#define SPGSS_LPBK0_PERIPHERAL_OFS               (12)                            /* !< PERIPHERAL Offset */
#define SPGSS_LPBK0_PERIPHERAL_MASK              ((uint32_t)0x0000F000U)         /* !< Select SPG peripheral for loopback.
                                                                                    Signals invovled in loopback based
                                                                                    on interface chosen 1. SPI : CS,
                                                                                    SCLK, SDI, SDO  2. I2C : SCL, SDA 3.
                                                                                    UART : TX, RX */
#define SPGSS_LPBK0_PERIPHERAL_SPG1              ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG2              ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG3              ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG4              ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG5              ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG6              ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG7              ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG8              ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG9              ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG10             ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG11             ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG12             ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG13             ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG14             ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG15             ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK0_PERIPHERAL_SPG16             ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */

/* SPGSS_LPBK1 Bits */
/* SPGSS_LPBK1[CONTROLLER] Bits */
#define SPGSS_LPBK1_CONTROLLER_OFS               (8)                             /* !< CONTROLLER Offset */
#define SPGSS_LPBK1_CONTROLLER_MASK              ((uint32_t)0x00000F00U)         /* !< Select SPG controller for loopback.
                                                                                    Signals invovled in loopback based
                                                                                    on interface chosen 1. SPI : CS,
                                                                                    SCLK, SDI, SDO  2. I2C : SCL, SDA 3.
                                                                                    UART : TX, RX */
#define SPGSS_LPBK1_CONTROLLER_SPG1              ((uint32_t)0x00000000U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG2              ((uint32_t)0x00000100U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG3              ((uint32_t)0x00000200U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG4              ((uint32_t)0x00000300U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG5              ((uint32_t)0x00000400U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG6              ((uint32_t)0x00000500U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG7              ((uint32_t)0x00000600U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG8              ((uint32_t)0x00000700U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG9              ((uint32_t)0x00000800U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG10             ((uint32_t)0x00000900U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG11             ((uint32_t)0x00000A00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG12             ((uint32_t)0x00000B00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG13             ((uint32_t)0x00000C00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG14             ((uint32_t)0x00000D00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG15             ((uint32_t)0x00000E00U)         /* !< SPG commander select for loopback. */
#define SPGSS_LPBK1_CONTROLLER_SPG16             ((uint32_t)0x00000F00U)         /* !< SPG commander select for loopback. */
/* SPGSS_LPBK1[EN] Bits */
#define SPGSS_LPBK1_EN_OFS                       (0)                             /* !< EN Offset */
#define SPGSS_LPBK1_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Loopback  enable */
#define SPGSS_LPBK1_EN_DISABLE                   ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_LPBK1_EN_ENABLE                    ((uint32_t)0x00000001U)         /* !< Select this clock as a source */
/* SPGSS_LPBK1[PERIPHERAL] Bits */
#define SPGSS_LPBK1_PERIPHERAL_OFS               (12)                            /* !< PERIPHERAL Offset */
#define SPGSS_LPBK1_PERIPHERAL_MASK              ((uint32_t)0x0000F000U)         /* !< Select SPG peripheral for loopback.
                                                                                    Signals invovled in loopback based
                                                                                    on interface chosen 1. SPI : CS,
                                                                                    SCLK, SDI, SDO  2. I2C : SCL, SDA 3.
                                                                                    UART : TX, RX */
#define SPGSS_LPBK1_PERIPHERAL_SPG1              ((uint32_t)0x00000000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG2              ((uint32_t)0x00001000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG3              ((uint32_t)0x00002000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG4              ((uint32_t)0x00003000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG5              ((uint32_t)0x00004000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG6              ((uint32_t)0x00005000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG7              ((uint32_t)0x00006000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG8              ((uint32_t)0x00007000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG9              ((uint32_t)0x00008000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG10             ((uint32_t)0x00009000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG11             ((uint32_t)0x0000A000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG12             ((uint32_t)0x0000B000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG13             ((uint32_t)0x0000C000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG14             ((uint32_t)0x0000D000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG15             ((uint32_t)0x0000E000U)         /* !< SPG responder select for loopback. */
#define SPGSS_LPBK1_PERIPHERAL_SPG16             ((uint32_t)0x0000F000U)         /* !< SPG responder select for loopback. */

/* SPGSS_DMACHAIN0 Bits */
/* SPGSS_DMACHAIN0[SEL] Bits */
#define SPGSS_DMACHAIN0_SEL_OFS                  (1)                             /* !< SEL Offset */
#define SPGSS_DMACHAIN0_SEL_MASK                 ((uint32_t)0x0000003EU)         /* !< Select FIFO for chain */
#define SPGSS_DMACHAIN0_SEL_SPG1RXDONE           ((uint32_t)0x00000000U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG2RXDONE           ((uint32_t)0x00000002U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG3RXDONE           ((uint32_t)0x00000004U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG4RXDONE           ((uint32_t)0x00000006U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG5RXDONE           ((uint32_t)0x00000008U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG6RXDONE           ((uint32_t)0x0000000AU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG7RXDONE           ((uint32_t)0x0000000CU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG8RXDONE           ((uint32_t)0x0000000EU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG9RXDONE           ((uint32_t)0x00000010U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG10RXDONE          ((uint32_t)0x00000012U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG11RXDONE          ((uint32_t)0x00000014U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG12RXDONE          ((uint32_t)0x00000016U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG13RXDONE          ((uint32_t)0x00000018U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG14RXDONE          ((uint32_t)0x0000001AU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG15RXDONE          ((uint32_t)0x0000001CU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG16RXDONE          ((uint32_t)0x0000001EU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG1TXDONE           ((uint32_t)0x00000020U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG2TXDONE           ((uint32_t)0x00000022U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG3TXDONE           ((uint32_t)0x00000024U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG4TXDONE           ((uint32_t)0x00000026U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG5TXDONE           ((uint32_t)0x00000028U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG6TXDONE           ((uint32_t)0x0000002AU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG7TXDONE           ((uint32_t)0x0000002CU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG8TXDONE           ((uint32_t)0x0000002EU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG9TXDONE           ((uint32_t)0x00000030U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG10TXDONE          ((uint32_t)0x00000032U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG11TXDONE          ((uint32_t)0x00000034U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG12TXDONE          ((uint32_t)0x00000036U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG13TXDONE          ((uint32_t)0x00000038U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG14TXDONE          ((uint32_t)0x0000003AU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG15TXDONE          ((uint32_t)0x0000003CU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN0_SEL_SPG16TXDONE          ((uint32_t)0x0000003EU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
/* SPGSS_DMACHAIN0[EN] Bits */
#define SPGSS_DMACHAIN0_EN_OFS                   (0)                             /* !< EN Offset */
#define SPGSS_DMACHAIN0_EN_MASK                  ((uint32_t)0x00000001U)         /* !< Enable chain */
#define SPGSS_DMACHAIN0_EN_DISABLE               ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_DMACHAIN0_EN_ENABLE                ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_DMACHAIN1 Bits */
/* SPGSS_DMACHAIN1[SEL] Bits */
#define SPGSS_DMACHAIN1_SEL_OFS                  (1)                             /* !< SEL Offset */
#define SPGSS_DMACHAIN1_SEL_MASK                 ((uint32_t)0x0000003EU)         /* !< Select FIFO for chain */
#define SPGSS_DMACHAIN1_SEL_SPG1RXDONE           ((uint32_t)0x00000000U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG2RXDONE           ((uint32_t)0x00000002U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG3RXDONE           ((uint32_t)0x00000004U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG4RXDONE           ((uint32_t)0x00000006U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG5RXDONE           ((uint32_t)0x00000008U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG6RXDONE           ((uint32_t)0x0000000AU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG7RXDONE           ((uint32_t)0x0000000CU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG8RXDONE           ((uint32_t)0x0000000EU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG9RXDONE           ((uint32_t)0x00000010U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG10RXDONE          ((uint32_t)0x00000012U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG11RXDONE          ((uint32_t)0x00000014U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG12RXDONE          ((uint32_t)0x00000016U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG13RXDONE          ((uint32_t)0x00000018U)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG14RXDONE          ((uint32_t)0x0000001AU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG15RXDONE          ((uint32_t)0x0000001CU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG16RXDONE          ((uint32_t)0x0000001EU)         /* !< SPG DMA RX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from RX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG1TXDONE           ((uint32_t)0x00000020U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG2TXDONE           ((uint32_t)0x00000022U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG3TXDONE           ((uint32_t)0x00000024U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG4TXDONE           ((uint32_t)0x00000026U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG5TXDONE           ((uint32_t)0x00000028U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG6TXDONE           ((uint32_t)0x0000002AU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG7TXDONE           ((uint32_t)0x0000002CU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG8TXDONE           ((uint32_t)0x0000002EU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG9TXDONE           ((uint32_t)0x00000030U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG10TXDONE          ((uint32_t)0x00000032U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG11TXDONE          ((uint32_t)0x00000034U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG12TXDONE          ((uint32_t)0x00000036U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG13TXDONE          ((uint32_t)0x00000038U)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG14TXDONE          ((uint32_t)0x0000003AU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG15TXDONE          ((uint32_t)0x0000003CU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
#define SPGSS_DMACHAIN1_SEL_SPG16TXDONE          ((uint32_t)0x0000003EU)         /* !< SPG DMA TX Done Raw Interrupt
                                                                                    Status.  DMA_DONE from TX is selected
                                                                                    as a trigger for global DMA */
/* SPGSS_DMACHAIN1[EN] Bits */
#define SPGSS_DMACHAIN1_EN_OFS                   (0)                             /* !< EN Offset */
#define SPGSS_DMACHAIN1_EN_MASK                  ((uint32_t)0x00000001U)         /* !< Enable chain */
#define SPGSS_DMACHAIN1_EN_DISABLE               ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define SPGSS_DMACHAIN1_EN_ENABLE                ((uint32_t)0x00000001U)         /* !< Select this clock as a source */

/* SPGSS_DMA_CHAIN1_IMASK Bits */
/* SPGSS_DMA_CHAIN1_IMASK[DMA_DONE_CHAIN0] Bits */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN0_OFS (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN0_MASK ((uint32_t)0x00000001U)         /* !< DMA Done Chain0 */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN0_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN0_SET ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* SPGSS_DMA_CHAIN1_IMASK[DMA_DONE_CHAIN1] Bits */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN1_OFS (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN1_MASK ((uint32_t)0x00000002U)         /* !< DMA Done Chain1 */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN1_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define SPGSS_DMA_CHAIN1_IMASK_DMA_DONE_CHAIN1_SET ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */

/* SPGSS_DMA_CHAIN1_RIS Bits */
/* SPGSS_DMA_CHAIN1_RIS[DMA_DONE_CHAIN1] Bits */
#define SPGSS_DMA_CHAIN1_RIS_DMA_DONE_CHAIN1_OFS (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_DMA_CHAIN1_RIS_DMA_DONE_CHAIN1_MASK ((uint32_t)0x00000002U)         /* !< DMA Done Chain 1 */
#define SPGSS_DMA_CHAIN1_RIS_DMA_DONE_CHAIN1_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_DMA_CHAIN1_RIS_DMA_DONE_CHAIN1_SET ((uint32_t)0x00000002U)         /* !< Interrupt occurred */

/* SPGSS_DMA_CHAIN1_MIS Bits */
/* SPGSS_DMA_CHAIN1_MIS[DMA_DONE_CHAIN1] Bits */
#define SPGSS_DMA_CHAIN1_MIS_DMA_DONE_CHAIN1_OFS (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_DMA_CHAIN1_MIS_DMA_DONE_CHAIN1_MASK ((uint32_t)0x00000002U)         /* !< Masked DMA_DONE_CHAIN1 */
#define SPGSS_DMA_CHAIN1_MIS_DMA_DONE_CHAIN1_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_DMA_CHAIN1_MIS_DMA_DONE_CHAIN1_SET ((uint32_t)0x00000002U)         /* !< Interrupt occurred */

/* SPGSS_DMA_CHAIN1_ICLR Bits */
/* SPGSS_DMA_CHAIN1_ICLR[DMA_DONE_CHAIN1] Bits */
#define SPGSS_DMA_CHAIN1_ICLR_DMA_DONE_CHAIN1_OFS (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_DMA_CHAIN1_ICLR_DMA_DONE_CHAIN1_MASK ((uint32_t)0x00000002U)         /* !< DMA DONE CHAIN1 */
#define SPGSS_DMA_CHAIN1_ICLR_DMA_DONE_CHAIN1_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define SPGSS_DMA_CHAIN1_ICLR_DMA_DONE_CHAIN1_CLR ((uint32_t)0x00000002U)         /* !< Clear Interrupt */

/* SPGSS_DMA_CHAIN0_IMASK Bits */
/* SPGSS_DMA_CHAIN0_IMASK[DMA_DONE_CHAIN0] Bits */
#define SPGSS_DMA_CHAIN0_IMASK_DMA_DONE_CHAIN0_OFS (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_DMA_CHAIN0_IMASK_DMA_DONE_CHAIN0_MASK ((uint32_t)0x00000001U)         /* !< DMA Done Chain0 */
#define SPGSS_DMA_CHAIN0_IMASK_DMA_DONE_CHAIN0_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define SPGSS_DMA_CHAIN0_IMASK_DMA_DONE_CHAIN0_SET ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */

/* SPGSS_DMA_CHAIN0_RIS Bits */
/* SPGSS_DMA_CHAIN0_RIS[DMA_DONE_CHAIN0] Bits */
#define SPGSS_DMA_CHAIN0_RIS_DMA_DONE_CHAIN0_OFS (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_DMA_CHAIN0_RIS_DMA_DONE_CHAIN0_MASK ((uint32_t)0x00000001U)         /* !< DMA Done Chain 0 */
#define SPGSS_DMA_CHAIN0_RIS_DMA_DONE_CHAIN0_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_DMA_CHAIN0_RIS_DMA_DONE_CHAIN0_SET ((uint32_t)0x00000001U)         /* !< Interrupt occurred */

/* SPGSS_DMA_CHAIN0_MIS Bits */
/* SPGSS_DMA_CHAIN0_MIS[DMA_DONE_CHAIN0] Bits */
#define SPGSS_DMA_CHAIN0_MIS_DMA_DONE_CHAIN0_OFS (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_DMA_CHAIN0_MIS_DMA_DONE_CHAIN0_MASK ((uint32_t)0x00000001U)         /* !< Masked DMA_DONE_CHAIN0 */
#define SPGSS_DMA_CHAIN0_MIS_DMA_DONE_CHAIN0_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_DMA_CHAIN0_MIS_DMA_DONE_CHAIN0_SET ((uint32_t)0x00000001U)         /* !< Interrupt occurred */

/* SPGSS_DMA_CHAIN0_ICLR Bits */
/* SPGSS_DMA_CHAIN0_ICLR[DMA_DONE_CHAIN0] Bits */
#define SPGSS_DMA_CHAIN0_ICLR_DMA_DONE_CHAIN0_OFS (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_DMA_CHAIN0_ICLR_DMA_DONE_CHAIN0_MASK ((uint32_t)0x00000001U)         /* !< DMA DONE CHAIN0 */
#define SPGSS_DMA_CHAIN0_ICLR_DMA_DONE_CHAIN0_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define SPGSS_DMA_CHAIN0_ICLR_DMA_DONE_CHAIN0_CLR ((uint32_t)0x00000001U)         /* !< Clear Interrupt */

/* SPGSS_IIDX Bits */
/* SPGSS_IIDX[STAT] Bits */
#define SPGSS_IIDX_STAT_OFS                      (0)                             /* !< STAT Offset */
#define SPGSS_IIDX_STAT_MASK                     ((uint32_t)0x000000FFU)         /* !< UART Module Interrupt Vector Value.
                                                                                    This register provides the highes
                                                                                    priority interrupt index. A read
                                                                                    clears the corresponding interrupt
                                                                                    flag in RIS and MIS registers.
                                                                                    15h-1Fh = Reserved */
#define SPGSS_IIDX_STAT_NO_INTR                  ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define SPGSS_IIDX_STAT_DMA_DONE_CHAIN0          ((uint32_t)0x00000001U)         /* !< DMA_DONE_CHAIN0 */
#define SPGSS_IIDX_STAT_DMA_DONE_CHAIN1          ((uint32_t)0x00000002U)         /* !< DMA DONE CHAIN1 */

/* SPGSS_CPU_INT_IMASK Bits */
/* SPGSS_CPU_INT_IMASK[DMA_DONE_CHAIN0] Bits */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN0_OFS  (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN0_MASK ((uint32_t)0x00000001U)         /* !< DMA Done Chain0 */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN0_CLR  ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN0_SET  ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* SPGSS_CPU_INT_IMASK[DMA_DONE_CHAIN1] Bits */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN1_OFS  (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN1_MASK ((uint32_t)0x00000002U)         /* !< DMA Done Chain1 */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN1_CLR  ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define SPGSS_CPU_INT_IMASK_DMA_DONE_CHAIN1_SET  ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */

/* SPGSS_CPU_INT_RIS Bits */
/* SPGSS_CPU_INT_RIS[DMA_DONE_CHAIN0] Bits */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN0_OFS    (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN0_MASK   ((uint32_t)0x00000001U)         /* !< DMA Done Chain 0 */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN0_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN0_SET    ((uint32_t)0x00000001U)         /* !< Interrupt occurred */
/* SPGSS_CPU_INT_RIS[DMA_DONE_CHAIN1] Bits */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN1_OFS    (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN1_MASK   ((uint32_t)0x00000002U)         /* !< DMA Done Chain 1 */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN1_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_CPU_INT_RIS_DMA_DONE_CHAIN1_SET    ((uint32_t)0x00000002U)         /* !< Interrupt occurred */

/* SPGSS_CPU_INT_MIS Bits */
/* SPGSS_CPU_INT_MIS[DMA_DONE_CHAIN0] Bits */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN0_OFS    (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN0_MASK   ((uint32_t)0x00000001U)         /* !< Masked DMA_DONE_CHAIN0 */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN0_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN0_SET    ((uint32_t)0x00000001U)         /* !< Interrupt occurred */
/* SPGSS_CPU_INT_MIS[DMA_DONE_CHAIN1] Bits */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN1_OFS    (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN1_MASK   ((uint32_t)0x00000002U)         /* !< Masked DMA_DONE_CHAIN1 */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN1_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define SPGSS_CPU_INT_MIS_DMA_DONE_CHAIN1_SET    ((uint32_t)0x00000002U)         /* !< Interrupt occurred */

/* SPGSS_ISET Bits */
/* SPGSS_ISET[DMA_DONE_CHAIN0] Bits */
#define SPGSS_ISET_DMA_DONE_CHAIN0_OFS           (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_ISET_DMA_DONE_CHAIN0_MASK          ((uint32_t)0x00000001U)         /* !< DMA_DONE_CHAIN0 */
#define SPGSS_ISET_DMA_DONE_CHAIN0_NO_EFFECT     ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define SPGSS_ISET_DMA_DONE_CHAIN0_SET           ((uint32_t)0x00000001U)         /* !< Set Interrupt */
/* SPGSS_ISET[DMA_DONE_CHAIN1] Bits */
#define SPGSS_ISET_DMA_DONE_CHAIN1_OFS           (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_ISET_DMA_DONE_CHAIN1_MASK          ((uint32_t)0x00000002U)         /* !< DMA_DONE_CHAIN1 */
#define SPGSS_ISET_DMA_DONE_CHAIN1_NO_EFFECT     ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define SPGSS_ISET_DMA_DONE_CHAIN1_SET           ((uint32_t)0x00000002U)         /* !< Set Interrupt */

/* SPGSS_CPU_INT_ICLR Bits */
/* SPGSS_CPU_INT_ICLR[DMA_DONE_CHAIN0] Bits */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN0_OFS   (0)                             /* !< DMA_DONE_CHAIN0 Offset */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN0_MASK  ((uint32_t)0x00000001U)         /* !< DMA DONE CHAIN0 */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN0_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN0_CLR   ((uint32_t)0x00000001U)         /* !< Clear Interrupt */
/* SPGSS_CPU_INT_ICLR[DMA_DONE_CHAIN1] Bits */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN1_OFS   (1)                             /* !< DMA_DONE_CHAIN1 Offset */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN1_MASK  ((uint32_t)0x00000002U)         /* !< DMA DONE CHAIN1 */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN1_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define SPGSS_CPU_INT_ICLR_DMA_DONE_CHAIN1_CLR   ((uint32_t)0x00000002U)         /* !< Clear Interrupt */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_spgss__include */

