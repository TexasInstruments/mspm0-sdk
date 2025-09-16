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

#ifndef ti_devices_msp_peripherals_hw_unicommi2ct__include
#define ti_devices_msp_peripherals_hw_unicommi2ct__include

/* Filename: hw_unicommi2ct.h */
/* Revised: 2024-07-26 01:47:37 */
/* Revision: 7272f6b6a2f4592ada400278814de80dfb9c70f1 */

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
* UNICOMMI2CT Registers
******************************************************************************/
#define UNICOMMI2CT_SPGDMA_OFS                   ((uint32_t)0x000010B0U)
#define UNICOMMI2CT_DMA_TRIG0_OFS                ((uint32_t)0x00001080U)
#define UNICOMMI2CT_DMA_TRIG1_OFS                ((uint32_t)0x00001050U)
#define UNICOMMI2CT_CPU_INT_OFS                  ((uint32_t)0x00001020U)


/** @addtogroup UNICOMMI2CT_SPGDMA
  @{
*/

typedef struct {
       uint32_t RESERVED0;
  __IO uint32_t SPGDMARXCTL;                       /* !< (@ 0x000010B4) DMA Channel Control */
  __IO uint32_t SPGDMARXDA;                        /* !< (@ 0x000010B8) DMA Channel Destination Address */
  __IO uint32_t SPGDMARXSZ;                        /* !< (@ 0x000010BC) DMA Channel Size */
       uint32_t RESERVED1;
  __IO uint32_t SPGDMATXCTL;                       /* !< (@ 0x000010C4) DMA Channel Control */
  __IO uint32_t SPGDMATXSA;                        /* !< (@ 0x000010C8) DMA Channel Destination Address */
  __IO uint32_t SPGDMATXSZ;                        /* !< (@ 0x000010CC) DMA Channel Size */
} UNICOMMI2CT_SPGDMA_Regs;

/*@}*/ /* end of group UNICOMMI2CT_SPGDMA */

/** @addtogroup UNICOMMI2CT_DMA_TRIG0
  @{
*/

typedef struct {
       uint32_t RESERVED0[2];
  __IO uint32_t IMASK;                             /* !< (@ 0x00001088) Interrupt mask */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001090) Raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001098) Masked interrupt status */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x000010A0) Interrupt set */
} UNICOMMI2CT_DMA_TRIG0_Regs;

/*@}*/ /* end of group UNICOMMI2CT_DMA_TRIG0 */

/** @addtogroup UNICOMMI2CT_DMA_TRIG1
  @{
*/

typedef struct {
       uint32_t RESERVED0[2];
  __IO uint32_t IMASK;                             /* !< (@ 0x00001058) Interrupt mask */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001060) Raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001068) Masked interrupt status */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001070) Interrupt set */
} UNICOMMI2CT_DMA_TRIG1_Regs;

/*@}*/ /* end of group UNICOMMI2CT_DMA_TRIG1 */

/** @addtogroup UNICOMMI2CT_CPU_INT
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
} UNICOMMI2CT_CPU_INT_Regs;

/*@}*/ /* end of group UNICOMMI2CT_CPU_INT */

/** @addtogroup UNICOMMI2CT
  @{
*/

typedef struct {
       uint32_t RESERVED0[1024];
  __IO uint32_t CLKDIV;                            /* !< (@ 0x00001000) Clock Divider */
       uint32_t RESERVED1;
  __IO uint32_t CLKSEL;                            /* !< (@ 0x00001008) Clock Select for Ultra Low Power peripherals */
       uint32_t RESERVED2[3];
  __IO uint32_t PDBGCTL;                           /* !< (@ 0x00001018) Peripheral Debug Control */
       uint32_t RESERVED3;
  UNICOMMI2CT_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) */
       uint32_t RESERVED4;
  UNICOMMI2CT_DMA_TRIG1_Regs  DMA_TRIG1;                         /* !< (@ 0x00001050) */
       uint32_t RESERVED5[3];
  UNICOMMI2CT_DMA_TRIG0_Regs  DMA_TRIG0;                         /* !< (@ 0x00001080) */
       uint32_t RESERVED6[3];
  UNICOMMI2CT_SPGDMA_Regs  SPGDMA;                            /* !< (@ 0x000010B0) */
       uint32_t RESERVED7[5];
  __O  uint32_t INTCTL;                            /* !< (@ 0x000010E4) Interrupt control register */
       uint32_t RESERVED8[6];
  __IO uint32_t CTR;                               /* !< (@ 0x00001100) I2C Target Control Register */
  __IO uint32_t ACKCTL;                            /* !< (@ 0x00001104) I2C Target ACK Control */
  __I  uint32_t SR;                                /* !< (@ 0x00001108) Status Register */
  __IO uint32_t IFLS;                              /* !< (@ 0x0000110C) Interrupt FIFO Level Select Register */
       uint32_t RESERVED9[2];
  __IO uint32_t GFCTL;                             /* !< (@ 0x00001118) I2C Glitch Filter Control */
       uint32_t RESERVED10;
  __O  uint32_t TTXDATA;                           /* !< (@ 0x00001120) I2C TXData */
  __I  uint32_t RXDATA;                            /* !< (@ 0x00001124) I2C RXData */
  __I  uint32_t PECSR;                             /* !< (@ 0x00001128) PEC status register */
       uint32_t RESERVED11[7];
  __IO uint32_t OAR2;                              /* !< (@ 0x00001148) Own Address 2 */
  __IO uint32_t OAR;                               /* !< (@ 0x0000114C) I2C Own Address */
  __I  uint32_t TIMEOUT_CNT;                       /* !< (@ 0x00001150) I2C Timeout Count Register */
  __IO uint32_t TIMEOUT_CTL;                       /* !< (@ 0x00001154) I2C Timeout Count Control Register */
  __IO uint32_t PECCTL;                            /* !< (@ 0x00001158) I2C PEC control register */
} UNICOMMI2CT_Regs;

/*@}*/ /* end of group UNICOMMI2CT */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* UNICOMMI2CT Register Control Bits
******************************************************************************/

/* UNICOMMI2CT_SPGDMARXCTL Bits */
/* UNICOMMI2CT_SPGDMARXCTL[DMAEN] Bits */
#define UNICOMMI2CT_SPGDMARXCTL_DMAEN_OFS        (1)                             /* !< DMAEN Offset */
#define UNICOMMI2CT_SPGDMARXCTL_DMAEN_MASK       ((uint32_t)0x00000002U)         /* !< DMA enable */
#define UNICOMMI2CT_SPGDMARXCTL_DMAEN_DISABLE    ((uint32_t)0x00000000U)         /* !< DMA channel disabled */
#define UNICOMMI2CT_SPGDMARXCTL_DMAEN_ENABLE     ((uint32_t)0x00000002U)         /* !< DMA channel enabled */
/* UNICOMMI2CT_SPGDMARXCTL[DMAMEMINCR] Bits */
#define UNICOMMI2CT_SPGDMARXCTL_DMAMEMINCR_OFS   (20)                            /* !< DMAMEMINCR Offset */
#define UNICOMMI2CT_SPGDMARXCTL_DMAMEMINCR_MASK  ((uint32_t)0x00100000U)         /* !< DMA destination increment. This bit
                                                                                    selects automatic incrementing or
                                                                                    decrementing of the destination
                                                                                    address DMADA for each transfer. The
                                                                                    amount of change to the DMADA is
                                                                                    based on the definition in the
                                                                                    DMADSTWDTH. For example, an increment
                                                                                    of 1 (+1) on a WORD transfer will
                                                                                    increment the DMADA by 4. */
#define UNICOMMI2CT_SPGDMARXCTL_DMAMEMINCR_INCREMENT ((uint32_t)0x00000000U)         /* !< Incremented by 1 (+1 * DMADSTWDTH) */
#define UNICOMMI2CT_SPGDMARXCTL_DMAMEMINCR_DECREMENT ((uint32_t)0x00100000U)         /* !< Decremented by 1 (-1 * DMADSTWDTH) */
/* UNICOMMI2CT_SPGDMARXCTL[DMAPREIRQ] Bits */
#define UNICOMMI2CT_SPGDMARXCTL_DMAPREIRQ_OFS    (24)                            /* !< DMAPREIRQ Offset */
#define UNICOMMI2CT_SPGDMARXCTL_DMAPREIRQ_MASK   ((uint32_t)0x01000000U)         /* !< Enable an early IRQ event. This can
                                                                                    help software to react quicker to and
                                                                                    DMA done event or allows some
                                                                                    additional configuration before the
                                                                                    channel is complete.   Note: This
                                                                                    register is only available in a
                                                                                    FULL-channel configuration. Please
                                                                                    consult the datasheet of the specific
                                                                                    device to map which channel number
                                                                                    has FULL or BASIC capability. In a
                                                                                    BASIC configuration this register is
                                                                                    a read only value and always reads as
                                                                                    0x0. */
#define UNICOMMI2CT_SPGDMARXCTL_DMAPREIRQ_PREIRQ_DISABLE ((uint32_t)0x00000000U)         /* !< Pre-IRQ event disabled. */
#define UNICOMMI2CT_SPGDMARXCTL_DMAPREIRQ_PREIRQ_HALF ((uint32_t)0x01000000U)         /* !< Issue Pre-IRQ event when DMASZ
                                                                                    reached the half size point of the
                                                                                    original transfer size */
/* UNICOMMI2CT_SPGDMARXCTL[DMATM] Bits */
#define UNICOMMI2CT_SPGDMARXCTL_DMATM_OFS        (27)                            /* !< DMATM Offset */
#define UNICOMMI2CT_SPGDMARXCTL_DMATM_MASK       ((uint32_t)0x08000000U)         /* !< DMA transfer mode register */
#define UNICOMMI2CT_SPGDMARXCTL_DMATM_SINGLE     ((uint32_t)0x00000000U)         /* !< Single transfer. Each transfer
                                                                                    requires a new trigger. When the
                                                                                    DMASZ counts down to zero an event
                                                                                    can be generated and the DMAEN is
                                                                                    cleared. */
#define UNICOMMI2CT_SPGDMARXCTL_DMATM_RPTSNGL    ((uint32_t)0x08000000U)         /* !< Repeated single transfer. Each
                                                                                    transfer requires a new trigger. When
                                                                                    the DMASZ counts down to zero an
                                                                                    event can be generated. After the
                                                                                    last transfer the DMASA, DMADA, DAMSZ
                                                                                    registers are restored to its initial
                                                                                    value and the DMAEN stays enabled. */
/* UNICOMMI2CT_SPGDMARXCTL[DMADSTWDTH] Bits */
#define UNICOMMI2CT_SPGDMARXCTL_DMADSTWDTH_OFS   (28)                            /* !< DMADSTWDTH Offset */
#define UNICOMMI2CT_SPGDMARXCTL_DMADSTWDTH_MASK  ((uint32_t)0x30000000U)         /* !< DMA destination width. This bit
                                                                                    selects the destination as a byte,
                                                                                    half word, word or long word. */
#define UNICOMMI2CT_SPGDMARXCTL_DMADSTWDTH_BYTE  ((uint32_t)0x00000000U)         /* !< Destination data width is BYTE
                                                                                    (8-bit) */
#define UNICOMMI2CT_SPGDMARXCTL_DMADSTWDTH_HALF  ((uint32_t)0x10000000U)         /* !< Destination data width is HALF-WORD
                                                                                    (16-bit) */
#define UNICOMMI2CT_SPGDMARXCTL_DMADSTWDTH_WORD  ((uint32_t)0x20000000U)         /* !< Destination data width is WORD
                                                                                    (32-bit) */

/* UNICOMMI2CT_SPGDMARXDA Bits */
/* UNICOMMI2CT_SPGDMARXDA[ADDR] Bits */
#define UNICOMMI2CT_SPGDMARXDA_ADDR_OFS          (0)                             /* !< ADDR Offset */
#define UNICOMMI2CT_SPGDMARXDA_ADDR_MASK         ((uint32_t)0xFFFFFFFFU)         /* !< DMA Channel Destination Address */
#define UNICOMMI2CT_SPGDMARXDA_ADDR_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_SPGDMARXDA_ADDR_MAXIMUM      ((uint32_t)0xFFFFFFFFU)         /* !< Highest possible value */

/* UNICOMMI2CT_SPGDMARXSZ Bits */
/* UNICOMMI2CT_SPGDMARXSZ[SIZE] Bits */
#define UNICOMMI2CT_SPGDMARXSZ_SIZE_OFS          (0)                             /* !< SIZE Offset */
#define UNICOMMI2CT_SPGDMARXSZ_SIZE_MASK         ((uint32_t)0x0000FFFFU)         /* !< DMA Channel Size in number of
                                                                                    transfers */
#define UNICOMMI2CT_SPGDMARXSZ_SIZE_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_SPGDMARXSZ_SIZE_MAXIMUM      ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMI2CT_SPGDMATXCTL Bits */
/* UNICOMMI2CT_SPGDMATXCTL[DMAEN] Bits */
#define UNICOMMI2CT_SPGDMATXCTL_DMAEN_OFS        (1)                             /* !< DMAEN Offset */
#define UNICOMMI2CT_SPGDMATXCTL_DMAEN_MASK       ((uint32_t)0x00000002U)         /* !< DMA enable */
#define UNICOMMI2CT_SPGDMATXCTL_DMAEN_DISABLE    ((uint32_t)0x00000000U)         /* !< DMA channel disabled */
#define UNICOMMI2CT_SPGDMATXCTL_DMAEN_ENABLE     ((uint32_t)0x00000002U)         /* !< DMA channel enabled */
/* UNICOMMI2CT_SPGDMATXCTL[DMAMEMINCR] Bits */
#define UNICOMMI2CT_SPGDMATXCTL_DMAMEMINCR_OFS   (20)                            /* !< DMAMEMINCR Offset */
#define UNICOMMI2CT_SPGDMATXCTL_DMAMEMINCR_MASK  ((uint32_t)0x00100000U)         /* !< DMA destination increment. This bit
                                                                                    selects automatic incrementing or
                                                                                    decrementing of the destination
                                                                                    address DMADA for each transfer. The
                                                                                    amount of change to the DMADA is
                                                                                    based on the definition in the
                                                                                    DMADSTWDTH. For example, an increment
                                                                                    of 1 (+1) on a WORD transfer will
                                                                                    increment the DMADA by 4. */
#define UNICOMMI2CT_SPGDMATXCTL_DMAMEMINCR_INCREMENT ((uint32_t)0x00000000U)         /* !< Incremented by 1 (+1 * DMASRCWDTH) */
#define UNICOMMI2CT_SPGDMATXCTL_DMAMEMINCR_DECREMENT ((uint32_t)0x00100000U)         /* !< Decremented by 1 (-1 * DMASRCWDTH) */
/* UNICOMMI2CT_SPGDMATXCTL[DMAPREIRQ] Bits */
#define UNICOMMI2CT_SPGDMATXCTL_DMAPREIRQ_OFS    (24)                            /* !< DMAPREIRQ Offset */
#define UNICOMMI2CT_SPGDMATXCTL_DMAPREIRQ_MASK   ((uint32_t)0x01000000U)         /* !< Enable an early IRQ event. This can
                                                                                    help software to react quicker to and
                                                                                    DMA done event or allows some
                                                                                    additional configuration before the
                                                                                    channel is complete.   Note: This
                                                                                    register is only available in a
                                                                                    FULL-channel configuration. Please
                                                                                    consult the datasheet of the specific
                                                                                    device to map which channel number
                                                                                    has FULL or BASIC capability. In a
                                                                                    BASIC configuration this register is
                                                                                    a read only value and always reads as
                                                                                    0x0. */
#define UNICOMMI2CT_SPGDMATXCTL_DMAPREIRQ_PREIRQ_DISABLE ((uint32_t)0x00000000U)         /* !< Pre-IRQ event disabled. */
#define UNICOMMI2CT_SPGDMATXCTL_DMAPREIRQ_PREIRQ_HALF ((uint32_t)0x01000000U)         /* !< Issue Pre-IRQ event when DMASZ
                                                                                    reached the half size point of the
                                                                                    original transfer size */
/* UNICOMMI2CT_SPGDMATXCTL[DMATM] Bits */
#define UNICOMMI2CT_SPGDMATXCTL_DMATM_OFS        (27)                            /* !< DMATM Offset */
#define UNICOMMI2CT_SPGDMATXCTL_DMATM_MASK       ((uint32_t)0x08000000U)         /* !< DMA transfer mode register  Note:
                                                                                    The repeat-single (2h) and
                                                                                    repeat-block (3h) transfer are only
                                                                                    available in a FULL-channel
                                                                                    configuration. Please consult the
                                                                                    datasheet of the specific device to
                                                                                    map which channel number has FULL or
                                                                                    BASIC capability. In a BASIC channel
                                                                                    configuration only the values for
                                                                                    single (0h) and block (1h) transfer
                                                                                    can be set. */
#define UNICOMMI2CT_SPGDMATXCTL_DMATM_SINGLE     ((uint32_t)0x00000000U)         /* !< Single transfer. Each transfer
                                                                                    requires a new trigger. When the
                                                                                    DMASZ counts down to zero an event
                                                                                    can be generated and the DMAEN is
                                                                                    cleared. */
#define UNICOMMI2CT_SPGDMATXCTL_DMATM_RPTSNGL    ((uint32_t)0x08000000U)         /* !< Repeated single transfer. Each
                                                                                    transfer requires a new trigger. When
                                                                                    the DMASZ counts down to zero an
                                                                                    event can be generated. After the
                                                                                    last transfer the DMASA, DMADA, DAMSZ
                                                                                    registers are restored to its initial
                                                                                    value and the DMAEN stays enabled. */
/* UNICOMMI2CT_SPGDMATXCTL[DMASRCWDTH] Bits */
#define UNICOMMI2CT_SPGDMATXCTL_DMASRCWDTH_OFS   (28)                            /* !< DMASRCWDTH Offset */
#define UNICOMMI2CT_SPGDMATXCTL_DMASRCWDTH_MASK  ((uint32_t)0x30000000U)         /* !< DMA source width. This bit selects
                                                                                    the source data width as a byte, half
                                                                                    word, word or long word. */
#define UNICOMMI2CT_SPGDMATXCTL_DMASRCWDTH_BYTE  ((uint32_t)0x00000000U)         /* !< Source data width is BYTE (8-bit) */
#define UNICOMMI2CT_SPGDMATXCTL_DMASRCWDTH_HALF  ((uint32_t)0x10000000U)         /* !< Source data width is HALF-WORD
                                                                                    (16-bit) */
#define UNICOMMI2CT_SPGDMATXCTL_DMASRCWDTH_WORD  ((uint32_t)0x20000000U)         /* !< Source data width is WORD (32-bit) */

/* UNICOMMI2CT_SPGDMATXSA Bits */
/* UNICOMMI2CT_SPGDMATXSA[ADDR] Bits */
#define UNICOMMI2CT_SPGDMATXSA_ADDR_OFS          (0)                             /* !< ADDR Offset */
#define UNICOMMI2CT_SPGDMATXSA_ADDR_MASK         ((uint32_t)0xFFFFFFFFU)         /* !< DMA Channel Destination Address */
#define UNICOMMI2CT_SPGDMATXSA_ADDR_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_SPGDMATXSA_ADDR_MAXIMUM      ((uint32_t)0xFFFFFFFFU)         /* !< Highest possible value */

/* UNICOMMI2CT_SPGDMATXSZ Bits */
/* UNICOMMI2CT_SPGDMATXSZ[SIZE] Bits */
#define UNICOMMI2CT_SPGDMATXSZ_SIZE_OFS          (0)                             /* !< SIZE Offset */
#define UNICOMMI2CT_SPGDMATXSZ_SIZE_MASK         ((uint32_t)0x0000FFFFU)         /* !< DMA Channel Size in number of
                                                                                    transfers */
#define UNICOMMI2CT_SPGDMATXSZ_SIZE_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_SPGDMATXSZ_SIZE_MAXIMUM      ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMI2CT_DMA_TRIG0_IMASK Bits */
/* UNICOMMI2CT_DMA_TRIG0_IMASK[TXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG0_IMASK_TXTRG_OFS    (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG0_IMASK_TXTRG_MASK   ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_DMA_TRIG0_IMASK_TXTRG_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG0_IMASK_TXTRG_SET    ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG0_RIS Bits */
/* UNICOMMI2CT_DMA_TRIG0_RIS[TXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG0_RIS_TXTRG_OFS      (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG0_RIS_TXTRG_MASK     ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_DMA_TRIG0_RIS_TXTRG_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG0_RIS_TXTRG_SET      ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG0_MIS Bits */
/* UNICOMMI2CT_DMA_TRIG0_MIS[TXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG0_MIS_TXTRG_OFS      (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG0_MIS_TXTRG_MASK     ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_DMA_TRIG0_MIS_TXTRG_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG0_MIS_TXTRG_SET      ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG0_ISET Bits */
/* UNICOMMI2CT_DMA_TRIG0_ISET[TXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG0_ISET_TXTRG_OFS     (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG0_ISET_TXTRG_MASK    ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_DMA_TRIG0_ISET_TXTRG_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG0_ISET_TXTRG_SET     ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG1_IMASK Bits */
/* UNICOMMI2CT_DMA_TRIG1_IMASK[RXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG1_IMASK_RXTRG_OFS    (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG1_IMASK_RXTRG_MASK   ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_DMA_TRIG1_IMASK_RXTRG_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG1_IMASK_RXTRG_SET    ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG1_RIS Bits */
/* UNICOMMI2CT_DMA_TRIG1_RIS[RXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG1_RIS_RXTRG_OFS      (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG1_RIS_RXTRG_MASK     ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_DMA_TRIG1_RIS_RXTRG_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG1_RIS_RXTRG_SET      ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG1_MIS Bits */
/* UNICOMMI2CT_DMA_TRIG1_MIS[RXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG1_MIS_RXTRG_OFS      (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG1_MIS_RXTRG_MASK     ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_DMA_TRIG1_MIS_RXTRG_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG1_MIS_RXTRG_SET      ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_DMA_TRIG1_ISET Bits */
/* UNICOMMI2CT_DMA_TRIG1_ISET[RXTRG] Bits */
#define UNICOMMI2CT_DMA_TRIG1_ISET_RXTRG_OFS     (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_DMA_TRIG1_ISET_RXTRG_MASK    ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_DMA_TRIG1_ISET_RXTRG_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_DMA_TRIG1_ISET_RXTRG_SET     ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_IIDX Bits */
/* UNICOMMI2CT_IIDX[STAT] Bits */
#define UNICOMMI2CT_IIDX_STAT_OFS                (0)                             /* !< STAT Offset */
#define UNICOMMI2CT_IIDX_STAT_MASK               ((uint32_t)0x000000FFU)         /* !< I2C Module Interrupt Vector Value.
                                                                                    This register provides the highes
                                                                                    priority interrupt index. A read
                                                                                    clears the corresponding interrupt
                                                                                    flag in RIS and MISC. 15h-1Fh =
                                                                                    Reserved */
#define UNICOMMI2CT_IIDX_STAT_NO_INTR            ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define UNICOMMI2CT_IIDX_STAT_RXDONEFG           ((uint32_t)0x00000001U)         /* !< Receive Done Flag */
#define UNICOMMI2CT_IIDX_STAT_TXDONEFG           ((uint32_t)0x00000002U)         /* !< Transmit Done Flag */
#define UNICOMMI2CT_IIDX_STAT_RXTRG              ((uint32_t)0x00000003U)         /* !< receive FIFO Trigger Level */
#define UNICOMMI2CT_IIDX_STAT_TXTRG              ((uint32_t)0x00000004U)         /* !< transmit FIFO Trigger level */
#define UNICOMMI2CT_IIDX_STAT_RXFULL             ((uint32_t)0x00000005U)         /* !< RX FIFO FULL Event/interrupt
                                                                                    pending */
#define UNICOMMI2CT_IIDX_STAT_TXEMPTY            ((uint32_t)0x00000006U)         /* !< Transmit FIFO/Buffer Empty
                                                                                    Event/interrupt pending */
#define UNICOMMI2CT_IIDX_STAT_TX_UNFL            ((uint32_t)0x00000007U)         /* !< Target TX FIFO underflow */
#define UNICOMMI2CT_IIDX_STAT_RX_OVFL            ((uint32_t)0x00000008U)         /* !< Target RX FIFO overflow event */
#define UNICOMMI2CT_IIDX_STAT_GENCALL            ((uint32_t)0x00000009U)         /* !< General Call Event */
#define UNICOMMI2CT_IIDX_STAT_STARTFG            ((uint32_t)0x0000000AU)         /* !< Start Event */
#define UNICOMMI2CT_IIDX_STAT_STOPFG             ((uint32_t)0x0000000BU)         /* !< Stop Event */
#define UNICOMMI2CT_IIDX_STAT_PEC_RX_ERR         ((uint32_t)0x0000000CU)         /* !< PEC receive error event */
#define UNICOMMI2CT_IIDX_STAT_TIMEOUTA           ((uint32_t)0x0000000DU)         /* !< Timeout A Event */
#define UNICOMMI2CT_IIDX_STAT_TIMEOUTB           ((uint32_t)0x0000000EU)         /* !< Timeout B Event */
#define UNICOMMI2CT_IIDX_STAT_DMA_DONE_RX        ((uint32_t)0x00000010U)         /* !< DMA DONE on Channel RX */
#define UNICOMMI2CT_IIDX_STAT_DMA_DONE_TX        ((uint32_t)0x00000011U)         /* !< DMA DONE on Channel TX */
#define UNICOMMI2CT_IIDX_STAT_ARBLOSTFG          ((uint32_t)0x00000012U)         /* !< Arbitration Lost */
#define UNICOMMI2CT_IIDX_STAT_DMA_PREIRQ_RX      ((uint32_t)0x00000013U)         /* !< DMA PRE IRQ INTERRUPT */
#define UNICOMMI2CT_IIDX_STAT_DMA_PREIRQ_TX      ((uint32_t)0x00000014U)         /* !< DMA PRE IRQ INTERRUPT */

/* UNICOMMI2CT_CPU_INT_IMASK Bits */
/* UNICOMMI2CT_CPU_INT_IMASK[RXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_RXDONE_OFS     (0)                             /* !< RXDONE Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_RXDONE_MASK    ((uint32_t)0x00000001U)         /* !< Receive Data Interrupt Signals that
                                                                                    a byte has been received */
#define UNICOMMI2CT_CPU_INT_IMASK_RXDONE_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_RXDONE_SET     ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[TXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_TXDONE_OFS     (1)                             /* !< TXDONE Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_TXDONE_MASK    ((uint32_t)0x00000002U)         /* !< Transmit Transaction completed
                                                                                    Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_TXDONE_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_TXDONE_SET     ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[GENCALL] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_GENCALL_OFS    (8)                             /* !< GENCALL Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_GENCALL_MASK   ((uint32_t)0x00000100U)         /* !< General Call Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_GENCALL_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_GENCALL_SET    ((uint32_t)0x00000100U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[RXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_RXTRG_OFS      (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_RXTRG_MASK     ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_CPU_INT_IMASK_RXTRG_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_RXTRG_SET      ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[TXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_TXTRG_OFS      (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_TXTRG_MASK     ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_CPU_INT_IMASK_TXTRG_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_TXTRG_SET      ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[START] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_START_OFS      (9)                             /* !< START Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_START_MASK     ((uint32_t)0x00000200U)         /* !< Start Condition Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_START_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_START_SET      ((uint32_t)0x00000200U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[STOP] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_STOP_OFS       (10)                            /* !< STOP Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_STOP_MASK      ((uint32_t)0x00000400U)         /* !< Stop Condition Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_STOP_CLR       ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_STOP_SET       ((uint32_t)0x00000400U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[DMA_DONE_TX] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_TX_OFS (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< DMA Done on Event Channel TX */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_TX_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_TX_SET ((uint32_t)0x00010000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[DMA_DONE_RX] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_RX_OFS (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< DMA Done on Event Channel RX */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_RX_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_RX_SET ((uint32_t)0x00008000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[PEC_RX_ERR] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_PEC_RX_ERR_OFS (11)                            /* !< PEC_RX_ERR Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_PEC_RX_ERR_MASK ((uint32_t)0x00000800U)         /* !< RX Pec Error Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_PEC_RX_ERR_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_PEC_RX_ERR_SET ((uint32_t)0x00000800U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[TIMEOUTA] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTA_OFS   (12)                            /* !< TIMEOUTA Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTA_MASK  ((uint32_t)0x00001000U)         /* !< Timeout A Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTA_CLR   ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTA_SET   ((uint32_t)0x00001000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[TIMEOUTB] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTB_OFS   (13)                            /* !< TIMEOUTB Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTB_MASK  ((uint32_t)0x00002000U)         /* !< Timeout B Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTB_CLR   ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTB_SET   ((uint32_t)0x00002000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[DMA_PREIRQ_RX] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< Enable DMA Done PREIRQ RX interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[DMA_PREIRQ_TX] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< Enable DMA Done PREIRQ TX interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_CPU_INT_IMASK_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[RXFULL] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_RXFULL_OFS     (4)                             /* !< RXFULL Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_RXFULL_MASK    ((uint32_t)0x00000010U)         /* !< RXFIFO full event. This interrupt
                                                                                    is set if an Target RX FIFO is full. */
#define UNICOMMI2CT_CPU_INT_IMASK_RXFULL_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_RXFULL_SET     ((uint32_t)0x00000010U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[TXEMPTY] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_TXEMPTY_OFS    (5)                             /* !< TXEMPTY Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_TXEMPTY_MASK   ((uint32_t)0x00000020U)         /* !< Target Transmit FIFO Empty
                                                                                    interrupt mask. This interrupt is set
                                                                                    if all data in the Transmit FIFO have
                                                                                    been shifted out and the transmit
                                                                                    goes into idle mode. */
#define UNICOMMI2CT_CPU_INT_IMASK_TXEMPTY_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_TXEMPTY_SET    ((uint32_t)0x00000020U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[TX_UNFL] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_TX_UNFL_OFS    (6)                             /* !< TX_UNFL Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_TX_UNFL_MASK   ((uint32_t)0x00000040U)         /* !< Target TX FIFO underflow */
#define UNICOMMI2CT_CPU_INT_IMASK_TX_UNFL_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_TX_UNFL_SET    ((uint32_t)0x00000040U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[RX_OVFL] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_RX_OVFL_OFS    (7)                             /* !< RX_OVFL Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_RX_OVFL_MASK   ((uint32_t)0x00000080U)         /* !< Target RX FIFO overflow */
#define UNICOMMI2CT_CPU_INT_IMASK_RX_OVFL_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_RX_OVFL_SET    ((uint32_t)0x00000080U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_IMASK[ARBLOST] Bits */
#define UNICOMMI2CT_CPU_INT_IMASK_ARBLOST_OFS    (17)                            /* !< ARBLOST Offset */
#define UNICOMMI2CT_CPU_INT_IMASK_ARBLOST_MASK   ((uint32_t)0x00020000U)         /* !< Arbitration Lost Interrupt */
#define UNICOMMI2CT_CPU_INT_IMASK_ARBLOST_CLR    ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_IMASK_ARBLOST_SET    ((uint32_t)0x00020000U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_CPU_INT_RIS Bits */
/* UNICOMMI2CT_CPU_INT_RIS[RXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_RXDONE_OFS       (0)                             /* !< RXDONE Offset */
#define UNICOMMI2CT_CPU_INT_RIS_RXDONE_MASK      ((uint32_t)0x00000001U)         /* !< Receive Done Interrupt: Set after a
                                                                                    byte is received */
#define UNICOMMI2CT_CPU_INT_RIS_RXDONE_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_RXDONE_SET       ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_RIS[TXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_TXDONE_OFS       (1)                             /* !< TXDONE Offset */
#define UNICOMMI2CT_CPU_INT_RIS_TXDONE_MASK      ((uint32_t)0x00000002U)         /* !< Transmission Done Interrupt: Set
                                                                                    after a byte is transmitted */
#define UNICOMMI2CT_CPU_INT_RIS_TXDONE_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_TXDONE_SET       ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_RIS[GENCALL] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_GENCALL_OFS      (8)                             /* !< GENCALL Offset */
#define UNICOMMI2CT_CPU_INT_RIS_GENCALL_MASK     ((uint32_t)0x00000100U)         /* !< General Call Interrupt: set when a
                                                                                    general call is received */
#define UNICOMMI2CT_CPU_INT_RIS_GENCALL_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_RIS_GENCALL_SET      ((uint32_t)0x00000100U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_RIS[RXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_RXTRG_OFS        (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_CPU_INT_RIS_RXTRG_MASK       ((uint32_t)0x00000004U)         /* !< Receive Trigger 				    When FIFO
                                                                                    is present, triggered as per IFLS
                                                                                    settings 				    when FIFO is not
                                                                                    present, this indicates when buffer
                                                                                    is full */
#define UNICOMMI2CT_CPU_INT_RIS_RXTRG_CLR        ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_RIS_RXTRG_SET        ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_RIS[TXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_TXTRG_OFS        (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_CPU_INT_RIS_TXTRG_MASK       ((uint32_t)0x00000008U)         /* !< Transmit Trigger     When FIFO is
                                                                                    present, triggered as per IFLS
                                                                                    settings
                                                                                    When FIFO is not present, trigger
                                                                                    when transmit buffer is empty */
#define UNICOMMI2CT_CPU_INT_RIS_TXTRG_CLR        ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_RIS_TXTRG_SET        ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_RIS[START] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_START_OFS        (9)                             /* !< START Offset */
#define UNICOMMI2CT_CPU_INT_RIS_START_MASK       ((uint32_t)0x00000200U)         /* !< Start Condition Interrupt: is set
                                                                                    after a START condition is received
                                                                                    and this target is addressed */
#define UNICOMMI2CT_CPU_INT_RIS_START_CLR        ((uint32_t)0x00000000U)         /* !< Clear interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_START_SET        ((uint32_t)0x00000200U)         /* !< Set when a START condition is
                                                                                    receivied and address matches
                                                                                    target's address */
/* UNICOMMI2CT_CPU_INT_RIS[STOP] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_STOP_OFS         (10)                            /* !< STOP Offset */
#define UNICOMMI2CT_CPU_INT_RIS_STOP_MASK        ((uint32_t)0x00000400U)         /* !< Stop Condition Interrupt: set on a
                                                                                    STOP condition if this target was
                                                                                    being addressed */
#define UNICOMMI2CT_CPU_INT_RIS_STOP_CLR         ((uint32_t)0x00000000U)         /* !< Clear Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_STOP_SET         ((uint32_t)0x00000400U)         /* !< Interrupt is set when target is
                                                                                    addressed and STOP condition is
                                                                                    received */
/* UNICOMMI2CT_CPU_INT_RIS[DMA_DONE_TX] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_TX_OFS  (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< DMA Done on Event Channel TX */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_TX_CLR  ((uint32_t)0x00000000U)         /* !< Clear interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_TX_SET  ((uint32_t)0x00010000U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_RIS[DMA_DONE_RX] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_RX_OFS  (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< DMA Done on Event Channel RX */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_RX_CLR  ((uint32_t)0x00000000U)         /* !< Clear interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_DONE_RX_SET  ((uint32_t)0x00008000U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_RIS[PEC_RX_ERR] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_PEC_RX_ERR_OFS   (11)                            /* !< PEC_RX_ERR Offset */
#define UNICOMMI2CT_CPU_INT_RIS_PEC_RX_ERR_MASK  ((uint32_t)0x00000800U)         /* !< RX Pec Error Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_PEC_RX_ERR_CLR   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_PEC_RX_ERR_SET   ((uint32_t)0x00000800U)         /* !< Interrupt ocuured */
/* UNICOMMI2CT_CPU_INT_RIS[TIMEOUTA] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTA_OFS     (12)                            /* !< TIMEOUTA Offset */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTA_MASK    ((uint32_t)0x00001000U)         /* !< Timeout A Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTA_CLR     ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTA_SET     ((uint32_t)0x00001000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_RIS[TIMEOUTB] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTB_OFS     (13)                            /* !< TIMEOUTB Offset */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTB_MASK    ((uint32_t)0x00002000U)         /* !< Timeout B Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTB_CLR     ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_TIMEOUTB_SET     ((uint32_t)0x00002000U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_RIS[DMA_PREIRQ_RX] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_RIS[DMA_PREIRQ_TX] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_CPU_INT_RIS_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_RIS[RXFULL] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_RXFULL_OFS       (4)                             /* !< RXFULL Offset */
#define UNICOMMI2CT_CPU_INT_RIS_RXFULL_MASK      ((uint32_t)0x00000010U)         /* !< RXFIFO full event. This interrupt
                                                                                    is set if an RX FIFO is full. */
#define UNICOMMI2CT_CPU_INT_RIS_RXFULL_CLR       ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_RIS_RXFULL_SET       ((uint32_t)0x00000010U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_RIS[TXEMPTY] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_TXEMPTY_OFS      (5)                             /* !< TXEMPTY Offset */
#define UNICOMMI2CT_CPU_INT_RIS_TXEMPTY_MASK     ((uint32_t)0x00000020U)         /* !< Transmit FIFO Empty Interrupt.
                                                                                    This interrupt is set if all data in
                                                                                    the Transmit FIFO have been shifted
                                                                                    out and FSM goes into idle mode. */
#define UNICOMMI2CT_CPU_INT_RIS_TXEMPTY_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_TXEMPTY_SET      ((uint32_t)0x00000020U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_RIS[TX_UNFL] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_TX_UNFL_OFS      (6)                             /* !< TX_UNFL Offset */
#define UNICOMMI2CT_CPU_INT_RIS_TX_UNFL_MASK     ((uint32_t)0x00000040U)         /* !< Transmit FIFO underflow */
#define UNICOMMI2CT_CPU_INT_RIS_TX_UNFL_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_TX_UNFL_SET      ((uint32_t)0x00000040U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_RIS[RX_OVFL] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_RX_OVFL_OFS      (7)                             /* !< RX_OVFL Offset */
#define UNICOMMI2CT_CPU_INT_RIS_RX_OVFL_MASK     ((uint32_t)0x00000080U)         /* !< Receive FIFO overflow */
#define UNICOMMI2CT_CPU_INT_RIS_RX_OVFL_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_RX_OVFL_SET      ((uint32_t)0x00000080U)         /* !< Interrupt Occured */
/* UNICOMMI2CT_CPU_INT_RIS[ARBLOST] Bits */
#define UNICOMMI2CT_CPU_INT_RIS_ARBLOST_OFS      (17)                            /* !< ARBLOST Offset */
#define UNICOMMI2CT_CPU_INT_RIS_ARBLOST_MASK     ((uint32_t)0x00020000U)         /* !< Arbitration Lost Interrupt */
#define UNICOMMI2CT_CPU_INT_RIS_ARBLOST_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_RIS_ARBLOST_SET      ((uint32_t)0x00020000U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_CPU_INT_MIS Bits */
/* UNICOMMI2CT_CPU_INT_MIS[RXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_RXDONE_OFS       (0)                             /* !< RXDONE Offset */
#define UNICOMMI2CT_CPU_INT_MIS_RXDONE_MASK      ((uint32_t)0x00000001U)         /* !< Receive Data Interrupt Signals that
                                                                                    a byte has been received */
#define UNICOMMI2CT_CPU_INT_MIS_RXDONE_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_RXDONE_SET       ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_MIS[TXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_TXDONE_OFS       (1)                             /* !< TXDONE Offset */
#define UNICOMMI2CT_CPU_INT_MIS_TXDONE_MASK      ((uint32_t)0x00000002U)         /* !< Transmit Transaction completed
                                                                                    Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_TXDONE_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_TXDONE_SET       ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_MIS[GENCALL] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_GENCALL_OFS      (8)                             /* !< GENCALL Offset */
#define UNICOMMI2CT_CPU_INT_MIS_GENCALL_MASK     ((uint32_t)0x00000100U)         /* !< General Call Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_GENCALL_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_GENCALL_SET      ((uint32_t)0x00000100U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_MIS[RXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_RXTRG_OFS        (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_CPU_INT_MIS_RXTRG_MASK       ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_CPU_INT_MIS_RXTRG_CLR        ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_MIS_RXTRG_SET        ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_MIS[TXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_TXTRG_OFS        (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_CPU_INT_MIS_TXTRG_MASK       ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_CPU_INT_MIS_TXTRG_CLR        ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_MIS_TXTRG_SET        ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_MIS[START] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_START_OFS        (9)                             /* !< START Offset */
#define UNICOMMI2CT_CPU_INT_MIS_START_MASK       ((uint32_t)0x00000200U)         /* !< START Detection Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_START_CLR        ((uint32_t)0x00000000U)         /* !< Clear MIS */
#define UNICOMMI2CT_CPU_INT_MIS_START_SET        ((uint32_t)0x00000200U)         /* !< Set MIS */
/* UNICOMMI2CT_CPU_INT_MIS[STOP] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_STOP_OFS         (10)                            /* !< STOP Offset */
#define UNICOMMI2CT_CPU_INT_MIS_STOP_MASK        ((uint32_t)0x00000400U)         /* !< STOP Detection Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_STOP_CLR         ((uint32_t)0x00000000U)         /* !< Clear MIS */
#define UNICOMMI2CT_CPU_INT_MIS_STOP_SET         ((uint32_t)0x00000400U)         /* !< Set MIS */
/* UNICOMMI2CT_CPU_INT_MIS[DMA_DONE_TX] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_TX_OFS  (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< DMA Done on Event Channel TX */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_TX_CLR  ((uint32_t)0x00000000U)         /* !< Clear MIS */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_TX_SET  ((uint32_t)0x00010000U)         /* !< Set MIS */
/* UNICOMMI2CT_CPU_INT_MIS[DMA_DONE_RX] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_RX_OFS  (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< DMA Done on Event Channel RX */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_RX_CLR  ((uint32_t)0x00000000U)         /* !< Clear MIS */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_DONE_RX_SET  ((uint32_t)0x00008000U)         /* !< Set MIS */
/* UNICOMMI2CT_CPU_INT_MIS[PEC_RX_ERR] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_PEC_RX_ERR_OFS   (11)                            /* !< PEC_RX_ERR Offset */
#define UNICOMMI2CT_CPU_INT_MIS_PEC_RX_ERR_MASK  ((uint32_t)0x00000800U)         /* !< Target RX Pec Error Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_PEC_RX_ERR_CLR   ((uint32_t)0x00000000U)         /* !< Clear interrupt mask */
#define UNICOMMI2CT_CPU_INT_MIS_PEC_RX_ERR_SET   ((uint32_t)0x00000800U)         /* !< Set interrupt mask */
/* UNICOMMI2CT_CPU_INT_MIS[TIMEOUTA] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTA_OFS     (12)                            /* !< TIMEOUTA Offset */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTA_MASK    ((uint32_t)0x00001000U)         /* !< Timeout A Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTA_CLR     ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTA_SET     ((uint32_t)0x00001000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_MIS[TIMEOUTB] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTB_OFS     (13)                            /* !< TIMEOUTB Offset */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTB_MASK    ((uint32_t)0x00002000U)         /* !< Timeout B Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTB_CLR     ((uint32_t)0x00000000U)         /* !< Clear interrupt mask */
#define UNICOMMI2CT_CPU_INT_MIS_TIMEOUTB_SET     ((uint32_t)0x00002000U)         /* !< Set interrupt mask */
/* UNICOMMI2CT_CPU_INT_MIS[DMA_PREIRQ_RX] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< Masked DMA PREIRQ Done on RX Event
                                                                                    Channel Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_MIS[DMA_PREIRQ_TX] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< Masked DMA Done PREIRQ on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_MIS[RXFULL] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_RXFULL_OFS       (4)                             /* !< RXFULL Offset */
#define UNICOMMI2CT_CPU_INT_MIS_RXFULL_MASK      ((uint32_t)0x00000010U)         /* !< RXFIFO full event. This interrupt
                                                                                    is set if an RX FIFO is full. */
#define UNICOMMI2CT_CPU_INT_MIS_RXFULL_CLR       ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_MIS_RXFULL_SET       ((uint32_t)0x00000010U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_MIS[TXEMPTY] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_TXEMPTY_OFS      (5)                             /* !< TXEMPTY Offset */
#define UNICOMMI2CT_CPU_INT_MIS_TXEMPTY_MASK     ((uint32_t)0x00000020U)         /* !< Transmit FIFO Empty interrupt mask.
                                                                                    This interrupt is set if all data in
                                                                                    the Transmit FIFO have been shifted
                                                                                    out and the transmit goes into idle
                                                                                    mode. */
#define UNICOMMI2CT_CPU_INT_MIS_TXEMPTY_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_TXEMPTY_SET      ((uint32_t)0x00000020U)         /* !< Interrupt occured */
/* UNICOMMI2CT_CPU_INT_MIS[TX_UNFL] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_TX_UNFL_OFS      (6)                             /* !< TX_UNFL Offset */
#define UNICOMMI2CT_CPU_INT_MIS_TX_UNFL_MASK     ((uint32_t)0x00000040U)         /* !< Target TX FIFO underflow */
#define UNICOMMI2CT_CPU_INT_MIS_TX_UNFL_CLR      ((uint32_t)0x00000000U)         /* !< Clear interrupt mask */
#define UNICOMMI2CT_CPU_INT_MIS_TX_UNFL_SET      ((uint32_t)0x00000040U)         /* !< Set interrupt mask */
/* UNICOMMI2CT_CPU_INT_MIS[RX_OVFL] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_RX_OVFL_OFS      (7)                             /* !< RX_OVFL Offset */
#define UNICOMMI2CT_CPU_INT_MIS_RX_OVFL_MASK     ((uint32_t)0x00000080U)         /* !< Target RX FIFO overflow */
#define UNICOMMI2CT_CPU_INT_MIS_RX_OVFL_CLR      ((uint32_t)0x00000000U)         /* !< Clear interrupt mask */
#define UNICOMMI2CT_CPU_INT_MIS_RX_OVFL_SET      ((uint32_t)0x00000080U)         /* !< Set interrupt mask */
/* UNICOMMI2CT_CPU_INT_MIS[ARBLOST] Bits */
#define UNICOMMI2CT_CPU_INT_MIS_ARBLOST_OFS      (17)                            /* !< ARBLOST Offset */
#define UNICOMMI2CT_CPU_INT_MIS_ARBLOST_MASK     ((uint32_t)0x00020000U)         /* !< Arbitration Lost Interrupt */
#define UNICOMMI2CT_CPU_INT_MIS_ARBLOST_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMI2CT_CPU_INT_MIS_ARBLOST_SET      ((uint32_t)0x00020000U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_CPU_INT_ISET Bits */
/* UNICOMMI2CT_CPU_INT_ISET[RXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_RXDONE_OFS      (0)                             /* !< RXDONE Offset */
#define UNICOMMI2CT_CPU_INT_ISET_RXDONE_MASK     ((uint32_t)0x00000001U)         /* !< Target Receive Data Interrupt
                                                                                    Signals that a byte has been received */
#define UNICOMMI2CT_CPU_INT_ISET_RXDONE_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_RXDONE_SET      ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_ISET[TXDONE] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_TXDONE_OFS      (1)                             /* !< TXDONE Offset */
#define UNICOMMI2CT_CPU_INT_ISET_TXDONE_MASK     ((uint32_t)0x00000002U)         /* !< Target Transmit Transaction
                                                                                    completed Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_TXDONE_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_TXDONE_SET      ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_ISET[GENCALL] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_GENCALL_OFS     (8)                             /* !< GENCALL Offset */
#define UNICOMMI2CT_CPU_INT_ISET_GENCALL_MASK    ((uint32_t)0x00000100U)         /* !< General Call Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_GENCALL_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_GENCALL_SET     ((uint32_t)0x00000100U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_ISET[RXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_RXTRG_OFS       (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_CPU_INT_ISET_RXTRG_MASK      ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_CPU_INT_ISET_RXTRG_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_ISET_RXTRG_SET       ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_ISET[TXTRG] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_TXTRG_OFS       (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_CPU_INT_ISET_TXTRG_MASK      ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_CPU_INT_ISET_TXTRG_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_ISET_TXTRG_SET       ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_ISET[START] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_START_OFS       (9)                             /* !< START Offset */
#define UNICOMMI2CT_CPU_INT_ISET_START_MASK      ((uint32_t)0x00000200U)         /* !< Start Condition Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_START_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_START_SET       ((uint32_t)0x00000200U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[STOP] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_STOP_OFS        (10)                            /* !< STOP Offset */
#define UNICOMMI2CT_CPU_INT_ISET_STOP_MASK       ((uint32_t)0x00000400U)         /* !< Stop Condition Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_STOP_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_STOP_SET        ((uint32_t)0x00000400U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[DMA_DONE_TX] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_TX_OFS (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< DMA Done on Event Channel TX */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_TX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_TX_SET ((uint32_t)0x00010000U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[DMA_DONE_RX] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_RX_OFS (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< DMA Done on Event Channel RX */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_RX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_DONE_RX_SET ((uint32_t)0x00008000U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[PEC_RX_ERR] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_PEC_RX_ERR_OFS  (11)                            /* !< PEC_RX_ERR Offset */
#define UNICOMMI2CT_CPU_INT_ISET_PEC_RX_ERR_MASK ((uint32_t)0x00000800U)         /* !< Target RX Pec Error Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_PEC_RX_ERR_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_PEC_RX_ERR_SET  ((uint32_t)0x00000800U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[TIMEOUTA] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTA_OFS    (12)                            /* !< TIMEOUTA Offset */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTA_MASK   ((uint32_t)0x00001000U)         /* !< Timeout A interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTA_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTA_SET    ((uint32_t)0x00001000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_CPU_INT_ISET[TIMEOUTB] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTB_OFS    (13)                            /* !< TIMEOUTB Offset */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTB_MASK   ((uint32_t)0x00002000U)         /* !< Timeout B Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTB_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_TIMEOUTB_SET    ((uint32_t)0x00002000U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[DMA_PREIRQ_RX] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< Set DMA PREIRQ Done on RX Event
                                                                                    Channel Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_RX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Set Interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[DMA_PREIRQ_TX] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< Set DMA PREIRQ Done on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_TX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_CPU_INT_ISET_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Set Interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[RXFULL] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_RXFULL_OFS      (4)                             /* !< RXFULL Offset */
#define UNICOMMI2CT_CPU_INT_ISET_RXFULL_MASK     ((uint32_t)0x00000010U)         /* !< RXFIFO full event. This interrupt
                                                                                    is set if an RX FIFO is full. */
#define UNICOMMI2CT_CPU_INT_ISET_RXFULL_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_CPU_INT_ISET_RXFULL_SET      ((uint32_t)0x00000010U)         /* !< Set Interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[TXEMPTY] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_TXEMPTY_OFS     (5)                             /* !< TXEMPTY Offset */
#define UNICOMMI2CT_CPU_INT_ISET_TXEMPTY_MASK    ((uint32_t)0x00000020U)         /* !< Transmit FIFO Empty interrupt mask.
                                                                                    This interrupt is set if all data in
                                                                                    the Transmit FIFO have been shifted
                                                                                    out and the transmit goes into idle
                                                                                    mode. */
#define UNICOMMI2CT_CPU_INT_ISET_TXEMPTY_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_TXEMPTY_SET     ((uint32_t)0x00000020U)         /* !< Set Interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[TX_UNFL] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_TX_UNFL_OFS     (6)                             /* !< TX_UNFL Offset */
#define UNICOMMI2CT_CPU_INT_ISET_TX_UNFL_MASK    ((uint32_t)0x00000040U)         /* !< Target TX FIFO underflow */
#define UNICOMMI2CT_CPU_INT_ISET_TX_UNFL_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_TX_UNFL_SET     ((uint32_t)0x00000040U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[RX_OVFL] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_RX_OVFL_OFS     (7)                             /* !< RX_OVFL Offset */
#define UNICOMMI2CT_CPU_INT_ISET_RX_OVFL_MASK    ((uint32_t)0x00000080U)         /* !< Target RX FIFO overflow */
#define UNICOMMI2CT_CPU_INT_ISET_RX_OVFL_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_RX_OVFL_SET     ((uint32_t)0x00000080U)         /* !< Set interrupt */
/* UNICOMMI2CT_CPU_INT_ISET[ARBLOST] Bits */
#define UNICOMMI2CT_CPU_INT_ISET_ARBLOST_OFS     (17)                            /* !< ARBLOST Offset */
#define UNICOMMI2CT_CPU_INT_ISET_ARBLOST_MASK    ((uint32_t)0x00020000U)         /* !< Arbitration Lost Interrupt */
#define UNICOMMI2CT_CPU_INT_ISET_ARBLOST_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_CPU_INT_ISET_ARBLOST_SET     ((uint32_t)0x00020000U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_ICLR Bits */
/* UNICOMMI2CT_ICLR[RXDONE] Bits */
#define UNICOMMI2CT_ICLR_RXDONE_OFS              (0)                             /* !< RXDONE Offset */
#define UNICOMMI2CT_ICLR_RXDONE_MASK             ((uint32_t)0x00000001U)         /* !< Target Receive Data Interrupt
                                                                                    Signals that a byte has been received */
#define UNICOMMI2CT_ICLR_RXDONE_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_RXDONE_CLR              ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_ICLR[TXDONE] Bits */
#define UNICOMMI2CT_ICLR_TXDONE_OFS              (1)                             /* !< TXDONE Offset */
#define UNICOMMI2CT_ICLR_TXDONE_MASK             ((uint32_t)0x00000002U)         /* !< Target Transmit Transaction
                                                                                    completed Interrupt */
#define UNICOMMI2CT_ICLR_TXDONE_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_TXDONE_CLR              ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_ICLR[GENCALL] Bits */
#define UNICOMMI2CT_ICLR_GENCALL_OFS             (8)                             /* !< GENCALL Offset */
#define UNICOMMI2CT_ICLR_GENCALL_MASK            ((uint32_t)0x00000100U)         /* !< General Call Interrupt */
#define UNICOMMI2CT_ICLR_GENCALL_NO_EFFECT       ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_GENCALL_CLR             ((uint32_t)0x00000100U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_ICLR[RXTRG] Bits */
#define UNICOMMI2CT_ICLR_RXTRG_OFS               (2)                             /* !< RXTRG Offset */
#define UNICOMMI2CT_ICLR_RXTRG_MASK              ((uint32_t)0x00000004U)         /* !< Receive Trigger */
#define UNICOMMI2CT_ICLR_RXTRG_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_ICLR_RXTRG_CLR               ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_ICLR[TXTRG] Bits */
#define UNICOMMI2CT_ICLR_TXTRG_OFS               (3)                             /* !< TXTRG Offset */
#define UNICOMMI2CT_ICLR_TXTRG_MASK              ((uint32_t)0x00000008U)         /* !< Transmit Trigger */
#define UNICOMMI2CT_ICLR_TXTRG_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_ICLR_TXTRG_CLR               ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_ICLR[START] Bits */
#define UNICOMMI2CT_ICLR_START_OFS               (9)                             /* !< START Offset */
#define UNICOMMI2CT_ICLR_START_MASK              ((uint32_t)0x00000200U)         /* !< Target START Detection Interrupt */
#define UNICOMMI2CT_ICLR_START_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_START_CLR               ((uint32_t)0x00000200U)         /* !< Clear interrupt */
/* UNICOMMI2CT_ICLR[STOP] Bits */
#define UNICOMMI2CT_ICLR_STOP_OFS                (10)                            /* !< STOP Offset */
#define UNICOMMI2CT_ICLR_STOP_MASK               ((uint32_t)0x00000400U)         /* !< Target STOP Detection Interrupt */
#define UNICOMMI2CT_ICLR_STOP_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_STOP_CLR                ((uint32_t)0x00000400U)         /* !< Clear interrupt */
/* UNICOMMI2CT_ICLR[DMA_DONE_TX] Bits */
#define UNICOMMI2CT_ICLR_DMA_DONE_TX_OFS         (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMI2CT_ICLR_DMA_DONE_TX_MASK        ((uint32_t)0x00010000U)         /* !< DMA Done on Event Channel TX */
#define UNICOMMI2CT_ICLR_DMA_DONE_TX_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_DMA_DONE_TX_CLR         ((uint32_t)0x00010000U)         /* !< Clear interrupt */
/* UNICOMMI2CT_ICLR[DMA_DONE_RX] Bits */
#define UNICOMMI2CT_ICLR_DMA_DONE_RX_OFS         (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMI2CT_ICLR_DMA_DONE_RX_MASK        ((uint32_t)0x00008000U)         /* !< DMA Done on Event Channel RX */
#define UNICOMMI2CT_ICLR_DMA_DONE_RX_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_DMA_DONE_RX_CLR         ((uint32_t)0x00008000U)         /* !< Clear interrupt */
/* UNICOMMI2CT_ICLR[PEC_RX_ERR] Bits */
#define UNICOMMI2CT_ICLR_PEC_RX_ERR_OFS          (11)                            /* !< PEC_RX_ERR Offset */
#define UNICOMMI2CT_ICLR_PEC_RX_ERR_MASK         ((uint32_t)0x00000800U)         /* !< Target RX Pec Error Interrupt */
#define UNICOMMI2CT_ICLR_PEC_RX_ERR_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_PEC_RX_ERR_CLR          ((uint32_t)0x00000800U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[TIMEOUTA] Bits */
#define UNICOMMI2CT_ICLR_TIMEOUTA_OFS            (12)                            /* !< TIMEOUTA Offset */
#define UNICOMMI2CT_ICLR_TIMEOUTA_MASK           ((uint32_t)0x00001000U)         /* !< Timeout A interrupt */
#define UNICOMMI2CT_ICLR_TIMEOUTA_NO_EFFECT      ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_TIMEOUTA_CLR            ((uint32_t)0x00001000U)         /* !< Set Interrupt Mask */
/* UNICOMMI2CT_ICLR[TIMEOUTB] Bits */
#define UNICOMMI2CT_ICLR_TIMEOUTB_OFS            (13)                            /* !< TIMEOUTB Offset */
#define UNICOMMI2CT_ICLR_TIMEOUTB_MASK           ((uint32_t)0x00002000U)         /* !< Timeout B Interrupt */
#define UNICOMMI2CT_ICLR_TIMEOUTB_NO_EFFECT      ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_TIMEOUTB_CLR            ((uint32_t)0x00002000U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[DMA_PREIRQ_RX] Bits */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_RX_OFS       (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_RX_MASK      ((uint32_t)0x00040000U)         /* !< Clear DMA PREIRQ Done on RX Event
                                                                                    Channel Interrupt */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_RX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_RX_CLR       ((uint32_t)0x00040000U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[DMA_PREIRQ_TX] Bits */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_TX_OFS       (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_TX_MASK      ((uint32_t)0x00080000U)         /* !< Clear DMA PREIRQ Done on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_TX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMI2CT_ICLR_DMA_PREIRQ_TX_CLR       ((uint32_t)0x00080000U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[RXFULL] Bits */
#define UNICOMMI2CT_ICLR_RXFULL_OFS              (4)                             /* !< RXFULL Offset */
#define UNICOMMI2CT_ICLR_RXFULL_MASK             ((uint32_t)0x00000010U)         /* !< RXFIFO full event. This interrupt
                                                                                    is set if an RX FIFO is full. */
#define UNICOMMI2CT_ICLR_RXFULL_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMI2CT_ICLR_RXFULL_CLR              ((uint32_t)0x00000010U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[TXEMPTY] Bits */
#define UNICOMMI2CT_ICLR_TXEMPTY_OFS             (5)                             /* !< TXEMPTY Offset */
#define UNICOMMI2CT_ICLR_TXEMPTY_MASK            ((uint32_t)0x00000020U)         /* !< Transmit FIFO Empty interrupt mask.
                                                                                    This interrupt is set if all data in
                                                                                    the Transmit FIFO have been shifted
                                                                                    out and the transmit goes into idle
                                                                                    mode. */
#define UNICOMMI2CT_ICLR_TXEMPTY_NO_EFFECT       ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_TXEMPTY_CLR             ((uint32_t)0x00000020U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[TX_UNFL] Bits */
#define UNICOMMI2CT_ICLR_TX_UNFL_OFS             (6)                             /* !< TX_UNFL Offset */
#define UNICOMMI2CT_ICLR_TX_UNFL_MASK            ((uint32_t)0x00000040U)         /* !< Target TX FIFO underflow */
#define UNICOMMI2CT_ICLR_TX_UNFL_NO_EFFECT       ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_TX_UNFL_CLR             ((uint32_t)0x00000040U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[RX_OVFL] Bits */
#define UNICOMMI2CT_ICLR_RX_OVFL_OFS             (7)                             /* !< RX_OVFL Offset */
#define UNICOMMI2CT_ICLR_RX_OVFL_MASK            ((uint32_t)0x00000080U)         /* !< Target RX FIFO overflow */
#define UNICOMMI2CT_ICLR_RX_OVFL_NO_EFFECT       ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_RX_OVFL_CLR             ((uint32_t)0x00000080U)         /* !< Clear Interrupt */
/* UNICOMMI2CT_ICLR[ARBLOST] Bits */
#define UNICOMMI2CT_ICLR_ARBLOST_OFS             (17)                            /* !< ARBLOST Offset */
#define UNICOMMI2CT_ICLR_ARBLOST_MASK            ((uint32_t)0x00020000U)         /* !< Arbitration Lost Interrupt */
#define UNICOMMI2CT_ICLR_ARBLOST_NO_EFFECT       ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMI2CT_ICLR_ARBLOST_CLR             ((uint32_t)0x00020000U)         /* !< Set Interrupt Mask */

/* UNICOMMI2CT_CLKDIV Bits */
/* UNICOMMI2CT_CLKDIV[RATIO] Bits */
#define UNICOMMI2CT_CLKDIV_RATIO_OFS             (0)                             /* !< RATIO Offset */
#define UNICOMMI2CT_CLKDIV_RATIO_MASK            ((uint32_t)0x0000003FU)         /* !< Selects divide ratio of module
                                                                                    clock  Division factor  0 : DIV_BY_1
                                                                                    1 : DIV_BY_2 .... 63: DIV_BY_64 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_1        ((uint32_t)0x00000000U)         /* !< Do not divide clock source */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_2        ((uint32_t)0x00000001U)         /* !< Divide clock source by 2 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_3        ((uint32_t)0x00000002U)         /* !< Divide clock source by 3 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_4        ((uint32_t)0x00000003U)         /* !< Divide clock source by 4 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_5        ((uint32_t)0x00000004U)         /* !< Divide clock source by 5 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_6        ((uint32_t)0x00000005U)         /* !< Divide clock source by 6 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_7        ((uint32_t)0x00000006U)         /* !< Divide clock source by 7 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_8        ((uint32_t)0x00000007U)         /* !< Divide clock source by 8 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_61       ((uint32_t)0x0000003CU)         /* !< Divide clock source by 8 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_62       ((uint32_t)0x0000003DU)         /* !< Divide clock source by 8 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_63       ((uint32_t)0x0000003EU)         /* !< Divide clock source by 8 */
#define UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_64       ((uint32_t)0x0000003FU)         /* !< Divide clock source by 8 */

/* UNICOMMI2CT_CLKSEL Bits */
/* UNICOMMI2CT_CLKSEL[MFCLK_SEL] Bits */
#define UNICOMMI2CT_CLKSEL_MFCLK_SEL_OFS         (2)                             /* !< MFCLK_SEL Offset */
#define UNICOMMI2CT_CLKSEL_MFCLK_SEL_MASK        ((uint32_t)0x00000004U)         /* !< Selects MFCLK as clock source if
                                                                                    enabled */
#define UNICOMMI2CT_CLKSEL_MFCLK_SEL_DISABLE     ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_MFCLK_SEL_ENABLE      ((uint32_t)0x00000004U)         /* !< Select this clock as a source */
/* UNICOMMI2CT_CLKSEL[BUSCLK_SEL] Bits */
#define UNICOMMI2CT_CLKSEL_BUSCLK_SEL_OFS        (3)                             /* !< BUSCLK_SEL Offset */
#define UNICOMMI2CT_CLKSEL_BUSCLK_SEL_MASK       ((uint32_t)0x00000008U)         /* !< Selects BUS CLK as clock source if
                                                                                    enabled */
#define UNICOMMI2CT_CLKSEL_BUSCLK_SEL_DISABLE    ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_BUSCLK_SEL_ENABLE     ((uint32_t)0x00000008U)         /* !< Select this clock as a source */
/* UNICOMMI2CT_CLKSEL[ASYNC_SYSCLK_SEL] Bits */
#define UNICOMMI2CT_CLKSEL_ASYNC_SYSCLK_SEL_OFS  (9)                             /* !< ASYNC_SYSCLK_SEL Offset */
#define UNICOMMI2CT_CLKSEL_ASYNC_SYSCLK_SEL_MASK ((uint32_t)0x00000200U)         /* !< Asynchronous sysclk selected as
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_ASYNC_SYSCLK_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_ASYNC_SYSCLK_SEL_ENABLE ((uint32_t)0x00000200U)         /* !< Select this clock as a source */
/* UNICOMMI2CT_CLKSEL[ASYNC_HFCLK_SEL] Bits */
#define UNICOMMI2CT_CLKSEL_ASYNC_HFCLK_SEL_OFS   (10)                            /* !< ASYNC_HFCLK_SEL Offset */
#define UNICOMMI2CT_CLKSEL_ASYNC_HFCLK_SEL_MASK  ((uint32_t)0x00000400U)         /* !< Asynchronous HFCLK selected as
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_ASYNC_HFCLK_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_ASYNC_HFCLK_SEL_ENABLE ((uint32_t)0x00000400U)         /* !< Select this clock as a source */
/* UNICOMMI2CT_CLKSEL[ASYNC_PLL_SEL] Bits */
#define UNICOMMI2CT_CLKSEL_ASYNC_PLL_SEL_OFS     (11)                            /* !< ASYNC_PLL_SEL Offset */
#define UNICOMMI2CT_CLKSEL_ASYNC_PLL_SEL_MASK    ((uint32_t)0x00000800U)         /* !< Asynchronous PLL selected as source */
#define UNICOMMI2CT_CLKSEL_ASYNC_PLL_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMI2CT_CLKSEL_ASYNC_PLL_SEL_ENABLE  ((uint32_t)0x00000800U)         /* !< Select this clock as a source */

/* UNICOMMI2CT_PDBGCTL Bits */
/* UNICOMMI2CT_PDBGCTL[FREE] Bits */
#define UNICOMMI2CT_PDBGCTL_FREE_OFS             (0)                             /* !< FREE Offset */
#define UNICOMMI2CT_PDBGCTL_FREE_MASK            ((uint32_t)0x00000001U)         /* !< Free run control */
#define UNICOMMI2CT_PDBGCTL_FREE_STOP            ((uint32_t)0x00000000U)         /* !< The peripheral freezes
                                                                                    functionality while the Core Halted
                                                                                    input is asserted and resumes when it
                                                                                    is deasserted. */
#define UNICOMMI2CT_PDBGCTL_FREE_RUN             ((uint32_t)0x00000001U)         /* !< The peripheral ignores the state of
                                                                                    the Core Halted input */
/* UNICOMMI2CT_PDBGCTL[SOFT] Bits */
#define UNICOMMI2CT_PDBGCTL_SOFT_OFS             (1)                             /* !< SOFT Offset */
#define UNICOMMI2CT_PDBGCTL_SOFT_MASK            ((uint32_t)0x00000002U)         /* !< Soft halt boundary control. This
                                                                                    function is only available, if [FREE]
                                                                                    is set to 'STOP' */
#define UNICOMMI2CT_PDBGCTL_SOFT_DEPRECATED      ((uint32_t)0x00000000U)         /* !< Not supported */
#define UNICOMMI2CT_PDBGCTL_SOFT_DELAYED         ((uint32_t)0x00000002U)         /* !< The peripheral blocks the debug
                                                                                    freeze until it has reached a
                                                                                    boundary where it can resume without
                                                                                    corruption */

/* UNICOMMI2CT_INTCTL Bits */
/* UNICOMMI2CT_INTCTL[INTEVAL] Bits */
#define UNICOMMI2CT_INTCTL_INTEVAL_OFS           (0)                             /* !< INTEVAL Offset */
#define UNICOMMI2CT_INTCTL_INTEVAL_MASK          ((uint32_t)0x00000001U)         /* !< Writing a 1 to this field
                                                                                    re-evaluates the interrupt sources. */
#define UNICOMMI2CT_INTCTL_INTEVAL_DISABLE       ((uint32_t)0x00000000U)         /* !< The interrupt or event line is
                                                                                    disabled. */
#define UNICOMMI2CT_INTCTL_INTEVAL_EVAL          ((uint32_t)0x00000001U)         /* !< The interrupt or event line is in
                                                                                    software mode. Software must clear
                                                                                    the RIS. */

/* UNICOMMI2CT_CTR Bits */
/* UNICOMMI2CT_CTR[ENABLE] Bits */
#define UNICOMMI2CT_CTR_ENABLE_OFS               (0)                             /* !< ENABLE Offset */
#define UNICOMMI2CT_CTR_ENABLE_MASK              ((uint32_t)0x00000001U)         /* !< Setting this bit enables the
                                                                                    module. */
#define UNICOMMI2CT_CTR_ENABLE_DISABLE           ((uint32_t)0x00000000U)         /* !< Disables module operation. */
#define UNICOMMI2CT_CTR_ENABLE_ENABLE            ((uint32_t)0x00000001U)         /* !< Enables module operation. */
/* UNICOMMI2CT_CTR[GENCALL] Bits */
#define UNICOMMI2CT_CTR_GENCALL_OFS              (1)                             /* !< GENCALL Offset */
#define UNICOMMI2CT_CTR_GENCALL_MASK             ((uint32_t)0x00000002U)         /* !< General call response enable Modify
                                                                                    only when UCSWRST = 1. 0b = Do not
                                                                                    respond to a general call 1b =
                                                                                    Respond to a general call */
#define UNICOMMI2CT_CTR_GENCALL_DISABLE          ((uint32_t)0x00000000U)         /* !< Do not respond to a general call */
#define UNICOMMI2CT_CTR_GENCALL_ENABLE           ((uint32_t)0x00000002U)         /* !< Respond to a general call */
/* UNICOMMI2CT_CTR[CLKSTRETCH] Bits */
#define UNICOMMI2CT_CTR_CLKSTRETCH_OFS           (20)                            /* !< CLKSTRETCH Offset */
#define UNICOMMI2CT_CTR_CLKSTRETCH_MASK          ((uint32_t)0x00100000U)         /* !< Clock Stretch Enable */
#define UNICOMMI2CT_CTR_CLKSTRETCH_DISABLE       ((uint32_t)0x00000000U)         /* !< clock stretching is disabled */
#define UNICOMMI2CT_CTR_CLKSTRETCH_ENABLE        ((uint32_t)0x00100000U)         /* !< clock stretching is enabled */
/* UNICOMMI2CT_CTR[TXEMPTY_ON_TREQ] Bits */
#define UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_OFS      (3)                             /* !< TXEMPTY_ON_TREQ Offset */
#define UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_MASK     ((uint32_t)0x00000008U)         /* !< Tx Empty Interrupt on TREQ */
#define UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_DISABLE  ((uint32_t)0x00000000U)         /* !< When 0, RIS:TXEMPTY will be set
                                                                                    when only the Target TX FIFO is
                                                                                    empty. This allows the TXEMPTY
                                                                                    interrupt to be used to indicate that
                                                                                    the I2C bus is being clock stretched
                                                                                    and that Target TX data is required. */
#define UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_ENABLE   ((uint32_t)0x00000008U)         /* !< When 1, RIS:TXEMPTY will be set
                                                                                    when the Target State Machine is in
                                                                                    the TX_WAIT state which occurs when
                                                                                    the TX FIFO is empty AND the I2C
                                                                                    transaction is clock stretched
                                                                                    waiting for the FIFO to receive data. */
/* UNICOMMI2CT_CTR[TXTRIG_TXMODE] Bits */
#define UNICOMMI2CT_CTR_TXTRIG_TXMODE_OFS        (4)                             /* !< TXTRIG_TXMODE Offset */
#define UNICOMMI2CT_CTR_TXTRIG_TXMODE_MASK       ((uint32_t)0x00000010U)         /* !< Tx Trigger when Target FSM is in Tx
                                                                                    Mode */
#define UNICOMMI2CT_CTR_TXTRIG_TXMODE_DISABLE    ((uint32_t)0x00000000U)         /* !< No special behavior */
#define UNICOMMI2CT_CTR_TXTRIG_TXMODE_ENABLE     ((uint32_t)0x00000010U)         /* !< When 1, RIS:TXTRG will be set when
                                                                                    the Target TX FIFO has reached the
                                                                                    trigger level AND the Target State
                                                                                    Machine is in the TXMODE as defined
                                                                                    in the SSR register. When cleared
                                                                                    RIS:TXTRG will be set when the Target
                                                                                    TX FIFO is at or above the trigger
                                                                                    level. This setting can be used to
                                                                                    hold off the TX DMA until a
                                                                                    transaction starts. This allows the
                                                                                    DMA to be configured when the I2C is
                                                                                    idle but have it wait till the
                                                                                    transaction starts to load the Target
                                                                                    TX FIFO, so it can load from a memory
                                                                                    buffer that might be changing over
                                                                                    time. */
/* UNICOMMI2CT_CTR[TXWAIT_STALE_TXFIFO] Bits */
#define UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_OFS  (5)                             /* !< TXWAIT_STALE_TXFIFO Offset */
#define UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_MASK ((uint32_t)0x00000020U)         /* !< Tx transfer waits when stale data
                                                                                    in Tx FIFO. This prevents stale bytes
                                                                                    left in the TX FIFO from
                                                                                    automatically being sent on the next
                                                                                    I2C packet. Note: this should be used
                                                                                    with TXEMPTY_ON_TREQ set to prevent
                                                                                    the Target State Machine from waiting
                                                                                    for TX FIFO data without an interrupt
                                                                                    notification when the FIFO data is
                                                                                    stale. */
#define UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_DISABLE ((uint32_t)0x00000000U)         /* !< When 0, the TX FIFO empty signal to
                                                                                    the Target State Machine indicates
                                                                                    that the TX FIFO is empty. */
#define UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_ENABLE ((uint32_t)0x00000020U)         /* !< When 1, the TX FIFO empty signal to
                                                                                    the Target State Machine will
                                                                                    indicate that the TX FIFO is empty or
                                                                                    that the TX FIFO data is stale. The
                                                                                    TX FIFO data is determined to be
                                                                                    stale when there is data in the TX
                                                                                    FIFO when the Target State Machine
                                                                                    leaves the TXMODE as defined in the
                                                                                    SSR register. This can occur is a
                                                                                    Stop or timeout occur when there are
                                                                                    bytes left in the TX FIFO. */
/* UNICOMMI2CT_CTR[RXFULL_ON_RREQ] Bits */
#define UNICOMMI2CT_CTR_RXFULL_ON_RREQ_OFS       (6)                             /* !< RXFULL_ON_RREQ Offset */
#define UNICOMMI2CT_CTR_RXFULL_ON_RREQ_MASK      ((uint32_t)0x00000040U)         /* !< Rx full interrupt generated on RREQ
                                                                                    condition as indicated in SSR */
#define UNICOMMI2CT_CTR_RXFULL_ON_RREQ_DISABLE   ((uint32_t)0x00000000U)         /* !< When 0, RIS:SRXFULL will be set
                                                                                    when only the Target RX FIFO is full.
                                                                                    This allows the SRXFULL interrupt to
                                                                                    be used to indicate that the I2C bus
                                                                                    is being clock stretched and that the
                                                                                    FW must either read the RX FIFO or
                                                                                    ACK/NACK the current Rx byte. */
#define UNICOMMI2CT_CTR_RXFULL_ON_RREQ_ENABLE    ((uint32_t)0x00000040U)         /* !< When 1, RIS:SRXFULL will be set
                                                                                    when the Target State Machine is in
                                                                                    the RX_WAIT or RX_ACK_WAIT states
                                                                                    which occurs when the I2C transaction
                                                                                    is clock stretched because the RX
                                                                                    FIFO is full or the ACKOEN has been
                                                                                    set and the state machine is waiting
                                                                                    for FW to ACK/NACK the current byte. */
/* UNICOMMI2CT_CTR[EN_DEFHOSTADR] Bits */
#define UNICOMMI2CT_CTR_EN_DEFHOSTADR_OFS        (7)                             /* !< EN_DEFHOSTADR Offset */
#define UNICOMMI2CT_CTR_EN_DEFHOSTADR_MASK       ((uint32_t)0x00000080U)         /* !< Enable Default Host Address */
#define UNICOMMI2CT_CTR_EN_DEFHOSTADR_DISABLE    ((uint32_t)0x00000000U)         /* !< When this bit is 0, the default
                                                                                    host address is not matched NOTE: it
                                                                                    may still be matched if programmed
                                                                                    inside SOAR/SOAR2 */
#define UNICOMMI2CT_CTR_EN_DEFHOSTADR_ENABLE     ((uint32_t)0x00000080U)         /* !< When this bit is 1, default host
                                                                                    address of 7h000_1000 is always
                                                                                    matched by the Target address match
                                                                                    logic. */
/* UNICOMMI2CT_CTR[EN_ALRESPADR] Bits */
#define UNICOMMI2CT_CTR_EN_ALRESPADR_OFS         (8)                             /* !< EN_ALRESPADR Offset */
#define UNICOMMI2CT_CTR_EN_ALRESPADR_MASK        ((uint32_t)0x00000100U)         /* !< Enable Alert Response Address */
#define UNICOMMI2CT_CTR_EN_ALRESPADR_DISABLE     ((uint32_t)0x00000000U)         /* !< When this bit is 0, the alert
                                                                                    response address is not matched.
                                                                                    NOTE: it may still be matched if
                                                                                    programmed inside SOAR/SOAR2 */
#define UNICOMMI2CT_CTR_EN_ALRESPADR_ENABLE      ((uint32_t)0x00000100U)         /* !< When this bit is 1, alert response
                                                                                    address of 7h000_1100 is always
                                                                                    matched by the Target address match
                                                                                    logic. */
/* UNICOMMI2CT_CTR[EN_DEFDEVADR] Bits */
#define UNICOMMI2CT_CTR_EN_DEFDEVADR_OFS         (9)                             /* !< EN_DEFDEVADR Offset */
#define UNICOMMI2CT_CTR_EN_DEFDEVADR_MASK        ((uint32_t)0x00000200U)         /* !< Enable Default device address */
#define UNICOMMI2CT_CTR_EN_DEFDEVADR_DISABLE     ((uint32_t)0x00000000U)         /* !< When this bit is 0, the default
                                                                                    device address is not matched. NOTE:
                                                                                    it may still be matched if programmed
                                                                                    inside SOAR/SOAR2. */
#define UNICOMMI2CT_CTR_EN_DEFDEVADR_ENABLE      ((uint32_t)0x00000200U)         /* !< When this bit is 1, default device
                                                                                    address of 7h110_0001 is always
                                                                                    matched by the Target address match
                                                                                    logic. */
/* UNICOMMI2CT_CTR[WUEN] Bits */
#define UNICOMMI2CT_CTR_WUEN_OFS                 (21)                            /* !< WUEN Offset */
#define UNICOMMI2CT_CTR_WUEN_MASK                ((uint32_t)0x00200000U)         /* !< Target Wakeup Enable */
#define UNICOMMI2CT_CTR_WUEN_DISABLE             ((uint32_t)0x00000000U)         /* !< When 0, the Target is not allowed
                                                                                    to clock stretch on START detection */
#define UNICOMMI2CT_CTR_WUEN_ENABLE              ((uint32_t)0x00200000U)         /* !< When 1, the Target is allowed to
                                                                                    clock stretch on START detection and
                                                                                    wait for faster clock to be
                                                                                    abvailable. This allows clean wake up
                                                                                    support for I2C in low power mode use
                                                                                    cases */
/* UNICOMMI2CT_CTR[SUSPEND] Bits */
#define UNICOMMI2CT_CTR_SUSPEND_OFS              (11)                            /* !< SUSPEND Offset */
#define UNICOMMI2CT_CTR_SUSPEND_MASK             ((uint32_t)0x00000800U)         /* !< Suspend external communication */
#define UNICOMMI2CT_CTR_SUSPEND_DISABLE          ((uint32_t)0x00000000U)         /* !< Functional mode resumed */
#define UNICOMMI2CT_CTR_SUSPEND_ENABLE           ((uint32_t)0x00000800U)         /* !< External communication suspended */

/* UNICOMMI2CT_ACKCTL Bits */
/* UNICOMMI2CT_ACKCTL[ACKOEN] Bits */
#define UNICOMMI2CT_ACKCTL_ACKOEN_OFS            (0)                             /* !< ACKOEN Offset */
#define UNICOMMI2CT_ACKCTL_ACKOEN_MASK           ((uint32_t)0x00000001U)         /* !< I2C Target ACK Override Enable
                                                                                    Read SR.ACKOEN to check current
                                                                                    status of this bit */
#define UNICOMMI2CT_ACKCTL_ACKOEN_DISABLE        ((uint32_t)0x00000000U)         /* !< A response in not provided. */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ENABLE         ((uint32_t)0x00000001U)         /* !< An ACK or NACK is sent according to
                                                                                    the value written to the ACKOVAL bit. */
/* UNICOMMI2CT_ACKCTL[ACKOVAL] Bits */
#define UNICOMMI2CT_ACKCTL_ACKOVAL_OFS           (1)                             /* !< ACKOVAL Offset */
#define UNICOMMI2CT_ACKCTL_ACKOVAL_MASK          ((uint32_t)0x00000002U)         /* !< ACK Override Value Note: for
                                                                                    General Call this bit will be ignored
                                                                                    if set to NACK and Target continues
                                                                                    to receive data. */
#define UNICOMMI2CT_ACKCTL_ACKOVAL_DISABLE       ((uint32_t)0x00000000U)         /* !< An ACK is sent indicating valid
                                                                                    data or command. */
#define UNICOMMI2CT_ACKCTL_ACKOVAL_ENABLE        ((uint32_t)0x00000002U)         /* !< A NACK is sent indicating invalid
                                                                                    data or command. */
/* UNICOMMI2CT_ACKCTL[ACKOEN_ON_START] Bits */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_OFS   (2)                             /* !< ACKOEN_ON_START Offset */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_MASK  ((uint32_t)0x00000004U)         /* !< When set this bit will
                                                                                    automatically enable target override
                                                                                    acknowlede following a Start
                                                                                    Condition. Status bit ACKOEN will
                                                                                    reflect a value of '1' when automatic
                                                                                    override enable is used. */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_DISABLE ((uint32_t)0x00000000U)         /* !< No special behavior */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_ENABLE ((uint32_t)0x00000004U)         /* !< When set this bit will
                                                                                    automatically turn on the Target
                                                                                    ACKOEN field following a Start
                                                                                    Condition. */
/* UNICOMMI2CT_ACKCTL[ACKOEN_ON_PECNEXT] Bits */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_OFS (3)                             /* !< ACKOEN_ON_PECNEXT Offset */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_MASK ((uint32_t)0x00000008U)         /* !< When set this bit will
                                                                                    automatically turn on the Target
                                                                                    acknowledge override following a
                                                                                    ACK/NACK of the byte received just
                                                                                    prior to the PEC byte.  However,
                                                                                    setting ACKCTL.ACKOEN bit will not
                                                                                    automatically be ACKed/NACKed by the
                                                                                    State Machine and firmware must
                                                                                    perform this function by writing
                                                                                    Target.ACKCTL register.  Status bit
                                                                                    ACKOEN will reflect a value of '1'
                                                                                    when automatic override enable is
                                                                                    used. */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_DISABLE ((uint32_t)0x00000000U)         /* !< No special behavior */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_ENABLE ((uint32_t)0x00000008U)         /* !< When set this bit will
                                                                                    automatically turn on the Target
                                                                                    ACKOEN field following the ACK/NACK
                                                                                    of the byte received just prior to
                                                                                    the PEC byte. Note that when ACKOEN
                                                                                    is set the PEC byte will not
                                                                                    automatically be ACKed/NACKed by the
                                                                                    State Machine and FW must perform
                                                                                    this function by writing
                                                                                    Target_SACKCTL. */
/* UNICOMMI2CT_ACKCTL[ACKOEN_ON_PECDONE] Bits */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_OFS (4)                             /* !< ACKOEN_ON_PECDONE Offset */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_MASK ((uint32_t)0x00000010U)         /* !< When set, this bit will
                                                                                    automatically turn on target
                                                                                    acknowledge enable field following
                                                                                    the ACK/NACK of the received PEC
                                                                                    byte. Status bit ACKOEN will reflect
                                                                                    a value of '1' when automatic
                                                                                    override enable is used. */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_DISABLE ((uint32_t)0x00000000U)         /* !< No special behavior */
#define UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_ENABLE ((uint32_t)0x00000010U)         /* !< When set this bit will
                                                                                    automatically turn on the Target
                                                                                    ACKOEN field following the ACK/NACK
                                                                                    of the received PEC byte. */

/* UNICOMMI2CT_SR Bits */
/* UNICOMMI2CT_SR[RREQ] Bits */
#define UNICOMMI2CT_SR_RREQ_OFS                  (0)                             /* !< RREQ Offset */
#define UNICOMMI2CT_SR_RREQ_MASK                 ((uint32_t)0x00000001U)         /* !< Receive Request */
#define UNICOMMI2CT_SR_RREQ_CLEARED              ((uint32_t)0x00000000U)         /* !< No outstanding receive data. */
#define UNICOMMI2CT_SR_RREQ_SET                  ((uint32_t)0x00000001U)         /* !< Module has outstanding receive data
                                                                                    and is using clock stretching to
                                                                                    delay the Controller until the data
                                                                                    has been read from the RXDATA FIFO
                                                                                    (RX FIFO is full). */
/* UNICOMMI2CT_SR[TREQ] Bits */
#define UNICOMMI2CT_SR_TREQ_OFS                  (1)                             /* !< TREQ Offset */
#define UNICOMMI2CT_SR_TREQ_MASK                 ((uint32_t)0x00000002U)         /* !< Transmit Request */
#define UNICOMMI2CT_SR_TREQ_CLEARED              ((uint32_t)0x00000000U)         /* !< No outstanding transmit request. */
#define UNICOMMI2CT_SR_TREQ_SET                  ((uint32_t)0x00000002U)         /* !< I2C Target is addressed as a
                                                                                    transmitter and is using clock
                                                                                    stretching to delay the Controller
                                                                                    until data has been written to the
                                                                                    TXDATA FIFO (TX FIFO is empty). */
/* UNICOMMI2CT_SR[OAR2SEL] Bits */
#define UNICOMMI2CT_SR_OAR2SEL_OFS               (3)                             /* !< OAR2SEL Offset */
#define UNICOMMI2CT_SR_OAR2SEL_MASK              ((uint32_t)0x00000008U)         /* !< OAR2 Address Matched This bit gets
                                                                                    reevaluated after every address
                                                                                    comparison. */
#define UNICOMMI2CT_SR_OAR2SEL_CLEARED           ((uint32_t)0x00000000U)         /* !< Either the OAR2 address is not
                                                                                    matched or the match is in legacy
                                                                                    mode. */
#define UNICOMMI2CT_SR_OAR2SEL_SET               ((uint32_t)0x00000008U)         /* !< OAR2 address matched and ACKed by
                                                                                    the Target. */
/* UNICOMMI2CT_SR[QCMDST] Bits */
#define UNICOMMI2CT_SR_QCMDST_OFS                (4)                             /* !< QCMDST Offset */
#define UNICOMMI2CT_SR_QCMDST_MASK               ((uint32_t)0x00000010U)         /* !< Quick Command Status Value
                                                                                    Description: 0: The last transaction
                                                                                    was a normal transaction or a
                                                                                    transaction has not occurred. 1: The
                                                                                    last transaction was a Quick Command
                                                                                    transaction */
#define UNICOMMI2CT_SR_QCMDST_CLEARED            ((uint32_t)0x00000000U)         /* !< The last transaction was a normal
                                                                                    transaction or a transaction has not
                                                                                    occurred. */
#define UNICOMMI2CT_SR_QCMDST_SET                ((uint32_t)0x00000010U)         /* !< The last transaction was a Quick
                                                                                    Command transaction. */
/* UNICOMMI2CT_SR[QCMDRW] Bits */
#define UNICOMMI2CT_SR_QCMDRW_OFS                (5)                             /* !< QCMDRW Offset */
#define UNICOMMI2CT_SR_QCMDRW_MASK               ((uint32_t)0x00000020U)         /* !< Quick Command Read / Write  This
                                                                                    bit only has meaning when the QCMDST
                                                                                    bit is set.  Value Description: 0:
                                                                                    Quick command was a write 1: Quick
                                                                                    command was a read */
#define UNICOMMI2CT_SR_QCMDRW_CLEARED            ((uint32_t)0x00000000U)         /* !< Quick command was a write */
#define UNICOMMI2CT_SR_QCMDRW_SET                ((uint32_t)0x00000020U)         /* !< Quick command was a read */
/* UNICOMMI2CT_SR[RXMODE] Bits */
#define UNICOMMI2CT_SR_RXMODE_OFS                (2)                             /* !< RXMODE Offset */
#define UNICOMMI2CT_SR_RXMODE_MASK               ((uint32_t)0x00000004U)         /* !< Target FSM is in Rx MODE */
#define UNICOMMI2CT_SR_RXMODE_CLEARED            ((uint32_t)0x00000000U)         /* !< The Target State Machine is not in
                                                                                    the RX_DATA, RX_ACK, RX_WAIT,
                                                                                    RX_ACK_WAIT or ADDR_ACK state with
                                                                                    the bus direction set to write. */
#define UNICOMMI2CT_SR_RXMODE_SET                ((uint32_t)0x00000004U)         /* !< The Target State Machine is in the
                                                                                    RX_DATA, RX_ACK, RX_WAIT, RX_ACK_WAIT
                                                                                    or ADDR_ACK state with the bus
                                                                                    direction set to write. */
/* UNICOMMI2CT_SR[BUSBSY] Bits */
#define UNICOMMI2CT_SR_BUSBSY_OFS                (6)                             /* !< BUSBSY Offset */
#define UNICOMMI2CT_SR_BUSBSY_MASK               ((uint32_t)0x00000040U)         /* !< I2C bus is busy */
#define UNICOMMI2CT_SR_BUSBSY_CLEARED            ((uint32_t)0x00000000U)         /* !< The I2C Bus is not busy */
#define UNICOMMI2CT_SR_BUSBSY_SET                ((uint32_t)0x00000040U)         /* !< The I2C Bus is busy. This is
                                                                                    cleared on a timeout. */
/* UNICOMMI2CT_SR[TXMODE] Bits */
#define UNICOMMI2CT_SR_TXMODE_OFS                (7)                             /* !< TXMODE Offset */
#define UNICOMMI2CT_SR_TXMODE_MASK               ((uint32_t)0x00000080U)         /* !< FSM is in TX MODE */
#define UNICOMMI2CT_SR_TXMODE_CLEARED            ((uint32_t)0x00000000U)         /* !< State Machine is not in TX_DATA,
                                                                                    TX_WAIT, TX_ACK  or ADDR_ACK state
                                                                                    with the bus direction set to read. */
#define UNICOMMI2CT_SR_TXMODE_SET                ((uint32_t)0x00000080U)         /* !< State Machine is in TX_DATA,
                                                                                    TX_WAIT, TX_ACK  or ADDR_ACK state
                                                                                    with the bus direction set to read. */
/* UNICOMMI2CT_SR[STALE_TXFIFO] Bits */
#define UNICOMMI2CT_SR_STALE_TXFIFO_OFS          (8)                             /* !< STALE_TXFIFO Offset */
#define UNICOMMI2CT_SR_STALE_TXFIFO_MASK         ((uint32_t)0x00000100U)         /* !< Stale Tx FIFO */
#define UNICOMMI2CT_SR_STALE_TXFIFO_CLEARED      ((uint32_t)0x00000000U)         /* !< Tx FIFO is not stale */
#define UNICOMMI2CT_SR_STALE_TXFIFO_SET          ((uint32_t)0x00000100U)         /* !< The TX FIFO is stale. This occurs
                                                                                    when the TX FIFO was not emptied
                                                                                    during the previous I2C transaction. */
/* UNICOMMI2CT_SR[ADDRMATCH] Bits */
#define UNICOMMI2CT_SR_ADDRMATCH_OFS             (16)                            /* !< ADDRMATCH Offset */
#define UNICOMMI2CT_SR_ADDRMATCH_MASK            ((uint32_t)0x03FF0000U)         /* !< Indicates the address for which
                                                                                    Target address match happened */
#define UNICOMMI2CT_SR_ADDRMATCH_MINIMUM         ((uint32_t)0x00000000U)         /* !< Minimum Value */
#define UNICOMMI2CT_SR_ADDRMATCH_MAXIMUM         ((uint32_t)0x03FF0000U)         /* !< Maximum Value */
/* UNICOMMI2CT_SR[RXCLR] Bits */
#define UNICOMMI2CT_SR_RXCLR_OFS                 (9)                             /* !< RXCLR Offset */
#define UNICOMMI2CT_SR_RXCLR_MASK                ((uint32_t)0x00000200U)         /* !< RX FIFO Clear Status */
#define UNICOMMI2CT_SR_RXCLR_CLEARED             ((uint32_t)0x00000000U)         /* !< FIFO is not cleared */
#define UNICOMMI2CT_SR_RXCLR_SET                 ((uint32_t)0x00000200U)         /* !< FIFO clear is complete */
/* UNICOMMI2CT_SR[TXCLR] Bits */
#define UNICOMMI2CT_SR_TXCLR_OFS                 (10)                            /* !< TXCLR Offset */
#define UNICOMMI2CT_SR_TXCLR_MASK                ((uint32_t)0x00000400U)         /* !< TX FIFO Clear Status */
#define UNICOMMI2CT_SR_TXCLR_CLEARED             ((uint32_t)0x00000000U)         /* !< FIFO is not cleared */
#define UNICOMMI2CT_SR_TXCLR_SET                 ((uint32_t)0x00000400U)         /* !< FIFO clear is complete */
/* UNICOMMI2CT_SR[RXFE] Bits */
#define UNICOMMI2CT_SR_RXFE_OFS                  (11)                            /* !< RXFE Offset */
#define UNICOMMI2CT_SR_RXFE_MASK                 ((uint32_t)0x00000800U)         /* !< Receive FIFO Empty  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define UNICOMMI2CT_SR_RXFE_CLEARED              ((uint32_t)0x00000000U)         /* !< The receiver is not empty. */
#define UNICOMMI2CT_SR_RXFE_SET                  ((uint32_t)0x00000800U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the receive holding register is
                                                                                    empty. If the FIFO is enabled (FEN is
                                                                                    1), the receive FIFO is empty. */
/* UNICOMMI2CT_SR[RXFF] Bits */
#define UNICOMMI2CT_SR_RXFF_OFS                  (12)                            /* !< RXFF Offset */
#define UNICOMMI2CT_SR_RXFF_MASK                 ((uint32_t)0x00001000U)         /* !< Receive FIFO Full  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define UNICOMMI2CT_SR_RXFF_CLEARED              ((uint32_t)0x00000000U)         /* !< The receiver can receive data. */
#define UNICOMMI2CT_SR_RXFF_SET                  ((uint32_t)0x00001000U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the receive holding register is full.
                                                                                    If the FIFO is enabled (FEN is 1),
                                                                                    the receive FIFO is full. */
/* UNICOMMI2CT_SR[TXFE] Bits */
#define UNICOMMI2CT_SR_TXFE_OFS                  (13)                            /* !< TXFE Offset */
#define UNICOMMI2CT_SR_TXFE_MASK                 ((uint32_t)0x00002000U)         /* !< Transmit FIFO Empty  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define UNICOMMI2CT_SR_TXFE_CLEARED              ((uint32_t)0x00000000U)         /* !< The transmitter has data to
                                                                                    transmit. */
#define UNICOMMI2CT_SR_TXFE_SET                  ((uint32_t)0x00002000U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the transmit holding register is
                                                                                    empty. If the FIFO is enabled (FEN is
                                                                                    1), the transmit FIFO is empty. */
/* UNICOMMI2CT_SR[TXFF] Bits */
#define UNICOMMI2CT_SR_TXFF_OFS                  (14)                            /* !< TXFF Offset */
#define UNICOMMI2CT_SR_TXFF_MASK                 ((uint32_t)0x00004000U)         /* !< Transmit FIFO Full  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define UNICOMMI2CT_SR_TXFF_CLEARED              ((uint32_t)0x00000000U)         /* !< The transmitter is not full. */
#define UNICOMMI2CT_SR_TXFF_SET                  ((uint32_t)0x00004000U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the transmit holding register is
                                                                                    full. If the FIFO is enabled (FEN is
                                                                                    1), the transmit FIFO is full. */
/* UNICOMMI2CT_SR[ACKOEN] Bits */
#define UNICOMMI2CT_SR_ACKOEN_OFS                (15)                            /* !< ACKOEN Offset */
#define UNICOMMI2CT_SR_ACKOEN_MASK               ((uint32_t)0x00008000U)         /* !< Status of ACK Override Enable */
#define UNICOMMI2CT_SR_ACKOEN_DISABLE            ((uint32_t)0x00000000U)         /* !< ACK override is disabled */
#define UNICOMMI2CT_SR_ACKOEN_ENABLE             ((uint32_t)0x00008000U)         /* !< ACK override was enabled in design */

/* UNICOMMI2CT_IFLS Bits */
/* UNICOMMI2CT_IFLS[TXIFLSEL] Bits */
#define UNICOMMI2CT_IFLS_TXIFLSEL_OFS            (0)                             /* !< TXIFLSEL Offset */
#define UNICOMMI2CT_IFLS_TXIFLSEL_MASK           ((uint32_t)0x00000007U)         /* !< FIFO Level Select for generating
                                                                                    events (interrupt/dma).  Note: for
                                                                                    undefined settings the default
                                                                                    configuration is used. */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_3_4        ((uint32_t)0x00000001U)         /* !< TX FIFO <= 3/4 empty */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_1_2        ((uint32_t)0x00000002U)         /* !< TX FIFO <= 1/2 empty (default) */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_1_4        ((uint32_t)0x00000003U)         /* !< TX FIFO <= 1/4 empty */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_NOT_FULL   ((uint32_t)0x00000004U)         /* !< Opposite of full */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_EMPTY      ((uint32_t)0x00000005U)         /* !< TX FIFO is empty */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY ((uint32_t)0x00000006U)         /* !< TX FIFO <= 1 */
#define UNICOMMI2CT_IFLS_TXIFLSEL_LVL_ALMOST_FULL ((uint32_t)0x00000007U)         /* !< TX_FIFO >= (MAX_FIFO_LEN -1) */
/* UNICOMMI2CT_IFLS[RXIFLSEL] Bits */
#define UNICOMMI2CT_IFLS_RXIFLSEL_OFS            (4)                             /* !< RXIFLSEL Offset */
#define UNICOMMI2CT_IFLS_RXIFLSEL_MASK           ((uint32_t)0x00000070U)         /* !< FIFO Level Select for generating
                                                                                    events (interrupt/dma).  Note: for
                                                                                    undefined settings the default
                                                                                    configuration is used. */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_1_4        ((uint32_t)0x00000010U)         /* !< RX FIFO >= 1/4 full */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_1_2        ((uint32_t)0x00000020U)         /* !< RX FIFO >= 1/2 full (default) */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_3_4        ((uint32_t)0x00000030U)         /* !< RX FIFO >= 3/4 full */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_NOT_EMPTY  ((uint32_t)0x00000040U)         /* !< Opposite of empty */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_FULL       ((uint32_t)0x00000050U)         /* !< RX FIFO is full */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_ALMOST_FULL ((uint32_t)0x00000060U)         /* !< RX_FIFO >= (MAX_FIFO_LEN -1) */
#define UNICOMMI2CT_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY ((uint32_t)0x00000070U)         /* !< RX_FIFO <= 1 */
/* UNICOMMI2CT_IFLS[TXCLR] Bits */
#define UNICOMMI2CT_IFLS_TXCLR_OFS               (3)                             /* !< TXCLR Offset */
#define UNICOMMI2CT_IFLS_TXCLR_MASK              ((uint32_t)0x00000008U)         /* !< TX FIFO CLEAR. Setting this bit
                                                                                    will clear the TX FIFO contents. */
#define UNICOMMI2CT_IFLS_TXCLR_DISABLE           ((uint32_t)0x00000000U)         /* !< Disable FIFO clear */
#define UNICOMMI2CT_IFLS_TXCLR_ENABLE            ((uint32_t)0x00000008U)         /* !< Enable FIFO Clear */
/* UNICOMMI2CT_IFLS[RXCLR] Bits */
#define UNICOMMI2CT_IFLS_RXCLR_OFS               (7)                             /* !< RXCLR Offset */
#define UNICOMMI2CT_IFLS_RXCLR_MASK              ((uint32_t)0x00000080U)         /* !< RX FIFO CLEAR. Setting this bit
                                                                                    will clear the RX FIFO contents. */
#define UNICOMMI2CT_IFLS_RXCLR_DISABLE           ((uint32_t)0x00000000U)         /* !< Disable FIFO clear */
#define UNICOMMI2CT_IFLS_RXCLR_ENABLE            ((uint32_t)0x00000080U)         /* !< Enable FIFO Clear */

/* UNICOMMI2CT_GFCTL Bits */
/* UNICOMMI2CT_GFCTL[DGFSEL] Bits */
#define UNICOMMI2CT_GFCTL_DGFSEL_OFS             (0)                             /* !< DGFSEL Offset */
#define UNICOMMI2CT_GFCTL_DGFSEL_MASK            ((uint32_t)0x00000007U)         /* !< Glitch Suppression Pulse Width This
                                                                                    field controls the pulse width select
                                                                                    for glitch suppression on the SCL and
                                                                                    SDA lines. The following values are
                                                                                    the glitch suppression values in
                                                                                    terms of functional clocks. (Core
                                                                                    Domain only) */
#define UNICOMMI2CT_GFCTL_DGFSEL_DISABLED        ((uint32_t)0x00000000U)         /* !< Bypass */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_1           ((uint32_t)0x00000001U)         /* !< 1 clock */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_2           ((uint32_t)0x00000002U)         /* !< 2 clocks */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_3           ((uint32_t)0x00000003U)         /* !< 3 clocks */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_4           ((uint32_t)0x00000004U)         /* !< 4 clocks */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_8           ((uint32_t)0x00000005U)         /* !< 8 clocks */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_16          ((uint32_t)0x00000006U)         /* !< 16 clocks */
#define UNICOMMI2CT_GFCTL_DGFSEL_CLK_31          ((uint32_t)0x00000007U)         /* !< 31 clocks */
/* UNICOMMI2CT_GFCTL[AGFEN] Bits */
#define UNICOMMI2CT_GFCTL_AGFEN_OFS              (8)                             /* !< AGFEN Offset */
#define UNICOMMI2CT_GFCTL_AGFEN_MASK             ((uint32_t)0x00000100U)         /* !< Analog Glitch Suppression Enable */
#define UNICOMMI2CT_GFCTL_AGFEN_DISABLE          ((uint32_t)0x00000000U)         /* !< Analog Glitch Filter disable */
#define UNICOMMI2CT_GFCTL_AGFEN_ENABLE           ((uint32_t)0x00000100U)         /* !< Analog Glitch Filter enable */

/* UNICOMMI2CT_TTXDATA Bits */
/* UNICOMMI2CT_TTXDATA[DATA] Bits */
#define UNICOMMI2CT_TTXDATA_DATA_OFS             (0)                             /* !< DATA Offset */
#define UNICOMMI2CT_TTXDATA_DATA_MASK            ((uint32_t)0x000000FFU)         /* !< Transmit Data This byte contains
                                                                                    the data to be transferred during the
                                                                                    next transaction. */
#define UNICOMMI2CT_TTXDATA_DATA_MINIMUM         ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_TTXDATA_DATA_MAXIMUM         ((uint32_t)0x000000FFU)         /* !< Highest possible value */

/* UNICOMMI2CT_RXDATA Bits */
/* UNICOMMI2CT_RXDATA[DATA] Bits */
#define UNICOMMI2CT_RXDATA_DATA_OFS              (0)                             /* !< DATA Offset */
#define UNICOMMI2CT_RXDATA_DATA_MASK             ((uint32_t)0x000000FFU)         /* !< Received Data.  This field contains
                                                                                    the last received data. */
#define UNICOMMI2CT_RXDATA_DATA_MINIMUM          ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_RXDATA_DATA_MAXIMUM          ((uint32_t)0x000000FFU)         /* !< Highest possible value */

/* UNICOMMI2CT_PECSR Bits */
/* UNICOMMI2CT_PECSR[PECBYTECNT] Bits */
#define UNICOMMI2CT_PECSR_PECBYTECNT_OFS         (0)                             /* !< PECBYTECNT Offset */
#define UNICOMMI2CT_PECSR_PECBYTECNT_MASK        ((uint32_t)0x000001FFU)         /* !< This is the current PEC Byte Count
                                                                                    of the State Machine. */
#define UNICOMMI2CT_PECSR_PECBYTECNT_MINIMUM     ((uint32_t)0x00000000U)         /* !< Minimum Value */
#define UNICOMMI2CT_PECSR_PECBYTECNT_MAXIMUM     ((uint32_t)0x000001FFU)         /* !< Maximum Value */
/* UNICOMMI2CT_PECSR[PECSTS_CHECK] Bits */
#define UNICOMMI2CT_PECSR_PECSTS_CHECK_OFS       (16)                            /* !< PECSTS_CHECK Offset */
#define UNICOMMI2CT_PECSR_PECSTS_CHECK_MASK      ((uint32_t)0x00010000U)         /* !< This status bit indicates if the
                                                                                    PEC was checked in the transaction
                                                                                    that occurred before the last Stop.
                                                                                    Latched on Stop. */
#define UNICOMMI2CT_PECSR_PECSTS_CHECK_CLEARED   ((uint32_t)0x00000000U)         /* !< Indicates PEC was not checked in
                                                                                    the transaction that occurred before
                                                                                    the last Stop */
#define UNICOMMI2CT_PECSR_PECSTS_CHECK_SET       ((uint32_t)0x00010000U)         /* !< Indicates PEC was checked in the
                                                                                    transaction that occurred before the
                                                                                    last Stop */
/* UNICOMMI2CT_PECSR[PECSTS_ERROR] Bits */
#define UNICOMMI2CT_PECSR_PECSTS_ERROR_OFS       (17)                            /* !< PECSTS_ERROR Offset */
#define UNICOMMI2CT_PECSR_PECSTS_ERROR_MASK      ((uint32_t)0x00020000U)         /* !< This status bit indicates if a PEC
                                                                                    check error occurred in the
                                                                                    transaction that occurred before the
                                                                                    last Stop. Latched on Stop. */
#define UNICOMMI2CT_PECSR_PECSTS_ERROR_CLEARED   ((uint32_t)0x00000000U)         /* !< Indicates PEC check error did not
                                                                                    occurr in the transaction that
                                                                                    occurred before the last Stop */
#define UNICOMMI2CT_PECSR_PECSTS_ERROR_SET       ((uint32_t)0x00020000U)         /* !< Indicates PEC check error occurred
                                                                                    in the transaction that occurred
                                                                                    before the last Stop */

/* UNICOMMI2CT_OAR2 Bits */
/* UNICOMMI2CT_OAR2[OAR2] Bits */
#define UNICOMMI2CT_OAR2_OAR2_OFS                (0)                             /* !< OAR2 Offset */
#define UNICOMMI2CT_OAR2_OAR2_MASK               ((uint32_t)0x0000007FU)         /* !< Own Address 2This field specifies
                                                                                    the alternate OAR2 address. */
#define UNICOMMI2CT_OAR2_OAR2_MINIMUM            ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_OAR2_OAR2_MAXIMUM            ((uint32_t)0x0000007FU)         /* !< Highest possible value */
/* UNICOMMI2CT_OAR2[OAR2EN] Bits */
#define UNICOMMI2CT_OAR2_OAR2EN_OFS              (7)                             /* !< OAR2EN Offset */
#define UNICOMMI2CT_OAR2_OAR2EN_MASK             ((uint32_t)0x00000080U)         /* !< Own Address 2 Enable */
#define UNICOMMI2CT_OAR2_OAR2EN_DISABLE          ((uint32_t)0x00000000U)         /* !< The alternate address is disabled. */
#define UNICOMMI2CT_OAR2_OAR2EN_ENABLE           ((uint32_t)0x00000080U)         /* !< Enables the use of the alternate
                                                                                    address in the OAR2 field. */
/* UNICOMMI2CT_OAR2[OAR2_MASK] Bits */
#define UNICOMMI2CT_OAR2_OAR2_MASK_OFS           (16)                            /* !< OAR2_MASK Offset */
#define UNICOMMI2CT_OAR2_OAR2_MASK_MASK          ((uint32_t)0x007F0000U)         /* !< Own Address 2 Mask: This field
                                                                                    specifies bits A6 through A0 of the
                                                                                    Target address. The bits with value 1
                                                                                    in SOAR2.OAR2_MASK field will make
                                                                                    the corresponding incoming address
                                                                                    bits to match by default regardless
                                                                                    of the value inside SOAR2.OAR2 i.e.
                                                                                    corresponding SOAR2.OAR2 bit is a
                                                                                    dont care. */
#define UNICOMMI2CT_OAR2_OAR2_MASK_MINIMUM       ((uint32_t)0x00000000U)         /* !< Minimum Value */
#define UNICOMMI2CT_OAR2_OAR2_MASK_MAXIMUM       ((uint32_t)0x007F0000U)         /* !< Maximum Value */

/* UNICOMMI2CT_OAR Bits */
/* UNICOMMI2CT_OAR[OAR] Bits */
#define UNICOMMI2CT_OAR_OAR_OFS                  (0)                             /* !< OAR Offset */
#define UNICOMMI2CT_OAR_OAR_MASK                 ((uint32_t)0x000003FFU)         /* !< Own Address: This field specifies
                                                                                    bits A9 through A0 of the Target
                                                                                    address. In 7-bit addressing mode as
                                                                                    selected by I2CSOAR.MODE bit, the top
                                                                                    3 bits are don't care */
#define UNICOMMI2CT_OAR_OAR_MINIMUM              ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMI2CT_OAR_OAR_MAXIMUM              ((uint32_t)0x000003FFU)         /* !< Highest possible value */
/* UNICOMMI2CT_OAR[OAREN] Bits */
#define UNICOMMI2CT_OAR_OAREN_OFS                (14)                            /* !< OAREN Offset */
#define UNICOMMI2CT_OAR_OAREN_MASK               ((uint32_t)0x00004000U)         /* !< Own Address Enable */
#define UNICOMMI2CT_OAR_OAREN_DISABLE            ((uint32_t)0x00000000U)         /* !< Disable OAR address */
#define UNICOMMI2CT_OAR_OAREN_ENABLE             ((uint32_t)0x00004000U)         /* !< Enable OAR address */
/* UNICOMMI2CT_OAR[MODE] Bits */
#define UNICOMMI2CT_OAR_MODE_OFS                 (15)                            /* !< MODE Offset */
#define UNICOMMI2CT_OAR_MODE_MASK                ((uint32_t)0x00008000U)         /* !< This bit selects the adressing mode
                                                                                    to be used. When 0, 7-bit addressing
                                                                                    is used. When 1, 10-bit addressing is
                                                                                    used. */
#define UNICOMMI2CT_OAR_MODE_MODE7               ((uint32_t)0x00000000U)         /* !< Enable 7-bit addressing */
#define UNICOMMI2CT_OAR_MODE_MODE10              ((uint32_t)0x00008000U)         /* !< Enable 10-bit addressing */

/* UNICOMMI2CT_TIMEOUT_CNT Bits */
/* UNICOMMI2CT_TIMEOUT_CNT[TCNTA] Bits */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTA_OFS        (0)                             /* !< TCNTA Offset */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTA_MASK       ((uint32_t)0x000000FFU)         /* !< Timeout Count A Current Count: This
                                                                                    field contains the upper 8 bits of a
                                                                                    12-bit current counter for timeout
                                                                                    counter A */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTA_MINIMUM    ((uint32_t)0x00000000U)         /* !< Smallest Value */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTA_MAXIMUM    ((uint32_t)0x000000FFU)         /* !< Highest possible value */
/* UNICOMMI2CT_TIMEOUT_CNT[TCNTB] Bits */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTB_OFS        (16)                            /* !< TCNTB Offset */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTB_MASK       ((uint32_t)0x00FF0000U)         /* !< Timeout Count B Current Count: This
                                                                                    field contains the upper 8 bits of a
                                                                                    12-bit current counter for timeout
                                                                                    counter B */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTB_MINIMUM    ((uint32_t)0x00000000U)         /* !< Smallest Value */
#define UNICOMMI2CT_TIMEOUT_CNT_TCNTB_MAXIMUM    ((uint32_t)0x00FF0000U)         /* !< Highest possible value */

/* UNICOMMI2CT_TIMEOUT_CTL Bits */
/* UNICOMMI2CT_TIMEOUT_CTL[TCNTLA] Bits */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLA_OFS       (0)                             /* !< TCNTLA Offset */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLA_MASK      ((uint32_t)0x000000FFU)         /* !< Timeout counter A load value
                                                                                    Counter A is used for SCL low
                                                                                    detection. This field contains the
                                                                                    upper 8 bits of a 12-bit pre-load
                                                                                    value for the Timeout A count. NOTE:
                                                                                    The value of CNTLA must be greater
                                                                                    than 1h.   Each count is equal to 520
                                                                                    times the timeout period of
                                                                                    functional clock. For example, with
                                                                                    8MHz functional clock and a 100KHz
                                                                                    operating I2C clock, one timeout
                                                                                    period will be equal to (1 / 8MHz) *
                                                                                    520 or 65 us. */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLA_MINIMUM   ((uint32_t)0x00000000U)         /* !< Smallest Value */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLA_MAXIMUM   ((uint32_t)0x000000FFU)         /* !< Highest possible value */
/* UNICOMMI2CT_TIMEOUT_CTL[TCNTAEN] Bits */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_OFS      (15)                            /* !< TCNTAEN Offset */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_MASK     ((uint32_t)0x00008000U)         /* !< Timeout Counter A Enable */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_DISABLE  ((uint32_t)0x00000000U)         /* !< Disable Timeout Counter B */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_ENABLE   ((uint32_t)0x00008000U)         /* !< Enable Timeout Counter B */
/* UNICOMMI2CT_TIMEOUT_CTL[TCNTLB] Bits */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_OFS       (16)                            /* !< TCNTLB Offset */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_MASK      ((uint32_t)0x00FF0000U)         /* !< Timeout Count B Load: Counter B is
                                                                                    used for SCL High Detection. This
                                                                                    field contains the upper 8 bits of a
                                                                                    12-bit pre-load value for the Timeout
                                                                                    B count. NOTE: The value of CNTLB
                                                                                    must be greater than 1h.   Each count
                                                                                    is equal to 1* clock period. For
                                                                                    example, with 10MHz functional clock
                                                                                    one timeout period will be equal
                                                                                    to1*100ns. */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_MINIMUM   ((uint32_t)0x00000000U)         /* !< Smallest possible value */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_MAXIMUM   ((uint32_t)0x00FF0000U)         /* !< Highest possible value */
/* UNICOMMI2CT_TIMEOUT_CTL[TCNTBEN] Bits */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_OFS      (31)                            /* !< TCNTBEN Offset */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_MASK     ((uint32_t)0x80000000U)         /* !< Timeout Counter B Enable */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_DISABLE  ((uint32_t)0x00000000U)         /* !< Disable Timeout Counter B */
#define UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_ENABLE   ((uint32_t)0x80000000U)         /* !< Enable Timeout Counter B */

/* UNICOMMI2CT_PECCTL Bits */
/* UNICOMMI2CT_PECCTL[PECCNT] Bits */
#define UNICOMMI2CT_PECCTL_PECCNT_OFS            (0)                             /* !< PECCNT Offset */
#define UNICOMMI2CT_PECCTL_PECCNT_MASK           ((uint32_t)0x000001FFU)         /* !< When this field is non zero, the
                                                                                    number of I2C data bytes are counted.
                                                                                    When the byte count = PECCNT and the
                                                                                    state machine is transmitting, the
                                                                                    contents of the LSFR is loaded into
                                                                                    the shift register instead of the
                                                                                    byte received from the Tx FIFO. When
                                                                                    the state machine is receiving, after
                                                                                    the last bit of this byte is received
                                                                                    the LSFR is checked and if it is
                                                                                    non-zero, a PEC RX Error interrupt is
                                                                                    generated. The I2C packet must be
                                                                                    padded to include the PEC byte for
                                                                                    both transmit and receive. In
                                                                                    transmit mode the FIFO must be loaded
                                                                                    with a dummy PEC byte. In receive
                                                                                    mode the PEC byte will be passed to
                                                                                    the Rx FIFO.  In the normal use case,
                                                                                    FW would set PECEN=1 and PECCNT=0 and
                                                                                    use the ACKOEN until the remaining
                                                                                    SMB packet length is known. FW would
                                                                                    then set the PECCNT to the remaining
                                                                                    packet length (Including PEC bye). FW
                                                                                    would then configure DMA to allow the
                                                                                    packet to complete unassisted and
                                                                                    exit NoAck mode.  Note that when the
                                                                                    byte count = PEC CNT, the byte count
                                                                                    is reset to 0 and multiple PEC
                                                                                    calculation can automatically occur
                                                                                    within a single I2C transaction */
#define UNICOMMI2CT_PECCTL_PECCNT_MINIMUM        ((uint32_t)0x00000000U)         /* !< Minimum Value */
#define UNICOMMI2CT_PECCTL_PECCNT_MAXIMUM        ((uint32_t)0x000001FFU)         /* !< Maximum Value */
/* UNICOMMI2CT_PECCTL[PECEN] Bits */
#define UNICOMMI2CT_PECCTL_PECEN_OFS             (12)                            /* !< PECEN Offset */
#define UNICOMMI2CT_PECCTL_PECEN_MASK            ((uint32_t)0x00001000U)         /* !< PEC Enable This bit enables the SMB
                                                                                    Packet Error Checking (PEC). When
                                                                                    enabled the PEC is calculated on all
                                                                                    bits except the Start, Stop, Ack and
                                                                                    Nack. The PEC LSFR and the Byte
                                                                                    Counter is set to 0 when the State
                                                                                    Machine is in the IDLE state, which
                                                                                    occur following a Stop or when a
                                                                                    timeout occurs. The Counter is also
                                                                                    set to 0 after the PEC byte is sent
                                                                                    or received. Note that the NACK is
                                                                                    automatically send following a PEC
                                                                                    byte that results in a PEC error. The
                                                                                    PEC Polynomial is x^8 + x^2 + x^1 +
                                                                                    1. */
#define UNICOMMI2CT_PECCTL_PECEN_DISABLE         ((uint32_t)0x00000000U)         /* !< PEC transmission and check is
                                                                                    disabled */
#define UNICOMMI2CT_PECCTL_PECEN_ENABLE          ((uint32_t)0x00001000U)         /* !< PEC transmission and check is
                                                                                    enabled */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_unicommi2ct__include */
