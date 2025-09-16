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

#ifndef ti_devices_msp_peripherals_hw_unicommuart__include
#define ti_devices_msp_peripherals_hw_unicommuart__include

/* Filename: hw_unicommuart.h */
/* Revised: 2024-07-23 05:21:00 */
/* Revision: 7c260ce5cfafbadcd25e0fb6ff8594c604ea1d77 */

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
* UNICOMMUART Registers
******************************************************************************/
#define UNICOMMUART_SPGDMA_OFS                   ((uint32_t)0x000010B0U)
#define UNICOMMUART_DMA_TRIG_TX_OFS              ((uint32_t)0x00001080U)
#define UNICOMMUART_DMA_TRIG_RX_OFS              ((uint32_t)0x00001050U)
#define UNICOMMUART_CPU_INT_OFS                  ((uint32_t)0x00001020U)


/** @addtogroup UNICOMMUART_SPGDMA
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
} UNICOMMUART_SPGDMA_Regs;

/*@}*/ /* end of group UNICOMMUART_SPGDMA */

/** @addtogroup UNICOMMUART_DMA_TRIG_TX
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
} UNICOMMUART_DMA_TRIG_TX_Regs;

/*@}*/ /* end of group UNICOMMUART_DMA_TRIG_TX */

/** @addtogroup UNICOMMUART_DMA_TRIG_RX
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
} UNICOMMUART_DMA_TRIG_RX_Regs;

/*@}*/ /* end of group UNICOMMUART_DMA_TRIG_RX */

/** @addtogroup UNICOMMUART_CPU_INT
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
} UNICOMMUART_CPU_INT_Regs;

/*@}*/ /* end of group UNICOMMUART_CPU_INT */

/** @addtogroup UNICOMMUART
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
  UNICOMMUART_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) */
       uint32_t RESERVED4;
  UNICOMMUART_DMA_TRIG_RX_Regs  DMA_TRIG_RX;                       /* !< (@ 0x00001050) */
       uint32_t RESERVED5[3];
  UNICOMMUART_DMA_TRIG_TX_Regs  DMA_TRIG_TX;                       /* !< (@ 0x00001080) */
       uint32_t RESERVED6[3];
  UNICOMMUART_SPGDMA_Regs  SPGDMA;                            /* !< (@ 0x000010B0) */
       uint32_t RESERVED7[5];
  __O  uint32_t INTCTL;                            /* !< (@ 0x000010E4) Interrupt control register */
       uint32_t RESERVED8[6];
  __IO uint32_t CTL0;                              /* !< (@ 0x00001100) UART Control Register 0 */
  __IO uint32_t LCRH;                              /* !< (@ 0x00001104) UART Line Control Register */
  __I  uint32_t STAT;                              /* !< (@ 0x00001108) UART Status Register */
  __IO uint32_t IFLS;                              /* !< (@ 0x0000110C) UART Interrupt FIFO Level Select Register */
  __IO uint32_t IBRD;                              /* !< (@ 0x00001110) UART Integer Baud-Rate Divisor Register */
  __IO uint32_t FBRD;                              /* !< (@ 0x00001114) UART Fractional Baud-Rate Divisor Register */
  __IO uint32_t GFCTL;                             /* !< (@ 0x00001118) Glitch Filter Control */
       uint32_t RESERVED9;
  __O  uint32_t TXDATA;                            /* !< (@ 0x00001120) UART Transmit Data Register */
  __I  uint32_t RXDATA;                            /* !< (@ 0x00001124) UART Receive Data Register */
       uint32_t RESERVED10[2];
  __IO uint32_t LINCNT;                            /* !< (@ 0x00001130) UART LIN Mode Counter Register */
  __IO uint32_t LINCTL;                            /* !< (@ 0x00001134) UART LIN Mode Control Register */
  __IO uint32_t LINC0;                             /* !< (@ 0x00001138) UART LIN Mode Capture 0 Register */
  __IO uint32_t LINC1;                             /* !< (@ 0x0000113C) UART LIN Mode Capture 1 Register */
  __IO uint32_t IRCTL;                             /* !< (@ 0x00001140) eUSCI_Ax IrDA Control Word Register */
       uint32_t RESERVED11;
  __IO uint32_t AMASK;                             /* !< (@ 0x00001148) Self Address Mask Register */
  __IO uint32_t ADDR;                              /* !< (@ 0x0000114C) Self Address Register */
} UNICOMMUART_Regs;

/*@}*/ /* end of group UNICOMMUART */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* UNICOMMUART Register Control Bits
******************************************************************************/

/* UNICOMMUART_SPGDMARXCTL Bits */
/* UNICOMMUART_SPGDMARXCTL[DMAEN] Bits */
#define UNICOMMUART_SPGDMARXCTL_DMAEN_OFS        (1)                             /* !< DMAEN Offset */
#define UNICOMMUART_SPGDMARXCTL_DMAEN_MASK       ((uint32_t)0x00000002U)         /* !< DMA enable */
#define UNICOMMUART_SPGDMARXCTL_DMAEN_DISABLE    ((uint32_t)0x00000000U)         /* !< DMA channel disabled */
#define UNICOMMUART_SPGDMARXCTL_DMAEN_ENABLE     ((uint32_t)0x00000002U)         /* !< DMA channel enabled */
/* UNICOMMUART_SPGDMARXCTL[DMAMEMINCR] Bits */
#define UNICOMMUART_SPGDMARXCTL_DMAMEMINCR_OFS   (20)                            /* !< DMAMEMINCR Offset */
#define UNICOMMUART_SPGDMARXCTL_DMAMEMINCR_MASK  ((uint32_t)0x00100000U)         /* !< DMA destination increment. This bit
                                                                                    selects automatic incrementing or
                                                                                    decrementing of the destination
                                                                                    address DMADA for each transfer. The
                                                                                    amount of change to the DMADA is
                                                                                    based on the definition in the
                                                                                    DMADSTWDTH. For example, an increment
                                                                                    of 1 (+1) on a WORD transfer will
                                                                                    increment the DMADA by 4. */
#define UNICOMMUART_SPGDMARXCTL_DMAMEMINCR_INCREMENT ((uint32_t)0x00000000U)         /* !< Incremented by 1 (+1 * DMADSTWDTH) */
#define UNICOMMUART_SPGDMARXCTL_DMAMEMINCR_DECREMENT ((uint32_t)0x00100000U)         /* !< Decremented by 1 (-1 * DMADSTWDTH) */
/* UNICOMMUART_SPGDMARXCTL[DMAPREIRQ] Bits */
#define UNICOMMUART_SPGDMARXCTL_DMAPREIRQ_OFS    (24)                            /* !< DMAPREIRQ Offset */
#define UNICOMMUART_SPGDMARXCTL_DMAPREIRQ_MASK   ((uint32_t)0x01000000U)         /* !< Enable an early IRQ event. This can
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
#define UNICOMMUART_SPGDMARXCTL_DMAPREIRQ_PREIRQ_DISABLE ((uint32_t)0x00000000U)         /* !< Pre-IRQ event disabled. */
#define UNICOMMUART_SPGDMARXCTL_DMAPREIRQ_PREIRQ_HALF ((uint32_t)0x01000000U)         /* !< Issue Pre-IRQ event when DMASZ
                                                                                    reached the half size point of the
                                                                                    original transfer size */
/* UNICOMMUART_SPGDMARXCTL[DMATM] Bits */
#define UNICOMMUART_SPGDMARXCTL_DMATM_OFS        (27)                            /* !< DMATM Offset */
#define UNICOMMUART_SPGDMARXCTL_DMATM_MASK       ((uint32_t)0x08000000U)         /* !< DMA transfer mode register */
#define UNICOMMUART_SPGDMARXCTL_DMATM_SINGLE     ((uint32_t)0x00000000U)         /* !< Single transfer. Each transfer
                                                                                    requires a new trigger. When the
                                                                                    DMASZ counts down to zero an event
                                                                                    can be generated and the DMAEN is
                                                                                    cleared. */
#define UNICOMMUART_SPGDMARXCTL_DMATM_RPTSNGL    ((uint32_t)0x08000000U)         /* !< Repeated single transfer. Each
                                                                                    transfer requires a new trigger. When
                                                                                    the DMASZ counts down to zero an
                                                                                    event can be generated. After the
                                                                                    last transfer the DMASA, DMADA, DAMSZ
                                                                                    registers are restored to its initial
                                                                                    value and the DMAEN stays enabled. */
/* UNICOMMUART_SPGDMARXCTL[DMADSTWDTH] Bits */
#define UNICOMMUART_SPGDMARXCTL_DMADSTWDTH_OFS   (28)                            /* !< DMADSTWDTH Offset */
#define UNICOMMUART_SPGDMARXCTL_DMADSTWDTH_MASK  ((uint32_t)0x30000000U)         /* !< DMA destination width. This bit
                                                                                    selects the destination as a byte,
                                                                                    half word, word or long word. */
#define UNICOMMUART_SPGDMARXCTL_DMADSTWDTH_BYTE  ((uint32_t)0x00000000U)         /* !< Destination data width is BYTE
                                                                                    (8-bit) */
#define UNICOMMUART_SPGDMARXCTL_DMADSTWDTH_HALF  ((uint32_t)0x10000000U)         /* !< Destination data width is HALF-WORD
                                                                                    (16-bit) */
#define UNICOMMUART_SPGDMARXCTL_DMADSTWDTH_WORD  ((uint32_t)0x20000000U)         /* !< Destination data width is WORD
                                                                                    (32-bit) */

/* UNICOMMUART_SPGDMARXDA Bits */
/* UNICOMMUART_SPGDMARXDA[ADDR] Bits */
#define UNICOMMUART_SPGDMARXDA_ADDR_OFS          (0)                             /* !< ADDR Offset */
#define UNICOMMUART_SPGDMARXDA_ADDR_MASK         ((uint32_t)0xFFFFFFFFU)         /* !< DMA Channel Destination Address */
#define UNICOMMUART_SPGDMARXDA_ADDR_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_SPGDMARXDA_ADDR_MAXIMUM      ((uint32_t)0xFFFFFFFFU)         /* !< Highest possible value */

/* UNICOMMUART_SPGDMARXSZ Bits */
/* UNICOMMUART_SPGDMARXSZ[SIZE] Bits */
#define UNICOMMUART_SPGDMARXSZ_SIZE_OFS          (0)                             /* !< SIZE Offset */
#define UNICOMMUART_SPGDMARXSZ_SIZE_MASK         ((uint32_t)0x0000FFFFU)         /* !< DMA Channel Size in number of
                                                                                    transfers */
#define UNICOMMUART_SPGDMARXSZ_SIZE_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_SPGDMARXSZ_SIZE_MAXIMUM      ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMUART_SPGDMATXCTL Bits */
/* UNICOMMUART_SPGDMATXCTL[DMAEN] Bits */
#define UNICOMMUART_SPGDMATXCTL_DMAEN_OFS        (1)                             /* !< DMAEN Offset */
#define UNICOMMUART_SPGDMATXCTL_DMAEN_MASK       ((uint32_t)0x00000002U)         /* !< DMA enable */
#define UNICOMMUART_SPGDMATXCTL_DMAEN_DISABLE    ((uint32_t)0x00000000U)         /* !< DMA channel disabled */
#define UNICOMMUART_SPGDMATXCTL_DMAEN_ENABLE     ((uint32_t)0x00000002U)         /* !< DMA channel enabled */
/* UNICOMMUART_SPGDMATXCTL[DMAMEMINCR] Bits */
#define UNICOMMUART_SPGDMATXCTL_DMAMEMINCR_OFS   (20)                            /* !< DMAMEMINCR Offset */
#define UNICOMMUART_SPGDMATXCTL_DMAMEMINCR_MASK  ((uint32_t)0x00100000U)         /* !< DMA destination increment. This bit
                                                                                    selects automatic incrementing or
                                                                                    decrementing of the destination
                                                                                    address DMADA for each transfer. The
                                                                                    amount of change to the DMADA is
                                                                                    based on the definition in the
                                                                                    DMADSTWDTH. For example, an increment
                                                                                    of 1 (+1) on a WORD transfer will
                                                                                    increment the DMADA by 4. */
#define UNICOMMUART_SPGDMATXCTL_DMAMEMINCR_INCREMENT ((uint32_t)0x00000000U)         /* !< Incremented by 1 (+1 * DMASRCWDTH) */
#define UNICOMMUART_SPGDMATXCTL_DMAMEMINCR_DECREMENT ((uint32_t)0x00100000U)         /* !< Decremented by 1 (-1 * DMASRCWDTH) */
/* UNICOMMUART_SPGDMATXCTL[DMAPREIRQ] Bits */
#define UNICOMMUART_SPGDMATXCTL_DMAPREIRQ_OFS    (24)                            /* !< DMAPREIRQ Offset */
#define UNICOMMUART_SPGDMATXCTL_DMAPREIRQ_MASK   ((uint32_t)0x01000000U)         /* !< Enable an early IRQ event. This can
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
#define UNICOMMUART_SPGDMATXCTL_DMAPREIRQ_PREIRQ_DISABLE ((uint32_t)0x00000000U)         /* !< Pre-IRQ event disabled. */
#define UNICOMMUART_SPGDMATXCTL_DMAPREIRQ_PREIRQ_HALF ((uint32_t)0x01000000U)         /* !< Issue Pre-IRQ event when DMASZ
                                                                                    reached the half size point of the
                                                                                    original transfer size */
/* UNICOMMUART_SPGDMATXCTL[DMATM] Bits */
#define UNICOMMUART_SPGDMATXCTL_DMATM_OFS        (27)                            /* !< DMATM Offset */
#define UNICOMMUART_SPGDMATXCTL_DMATM_MASK       ((uint32_t)0x08000000U)         /* !< DMA transfer mode register  Note:
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
#define UNICOMMUART_SPGDMATXCTL_DMATM_SINGLE     ((uint32_t)0x00000000U)         /* !< Single transfer. Each transfer
                                                                                    requires a new trigger. When the
                                                                                    DMASZ counts down to zero an event
                                                                                    can be generated and the DMAEN is
                                                                                    cleared. */
#define UNICOMMUART_SPGDMATXCTL_DMATM_RPTSNGL    ((uint32_t)0x08000000U)         /* !< Repeated single transfer. Each
                                                                                    transfer requires a new trigger. When
                                                                                    the DMASZ counts down to zero an
                                                                                    event can be generated. After the
                                                                                    last transfer the DMASA, DMADA, DAMSZ
                                                                                    registers are restored to its initial
                                                                                    value and the DMAEN stays enabled. */
/* UNICOMMUART_SPGDMATXCTL[DMASRCWDTH] Bits */
#define UNICOMMUART_SPGDMATXCTL_DMASRCWDTH_OFS   (28)                            /* !< DMASRCWDTH Offset */
#define UNICOMMUART_SPGDMATXCTL_DMASRCWDTH_MASK  ((uint32_t)0x30000000U)         /* !< DMA source width. This bit selects
                                                                                    the source data width as a byte, half
                                                                                    word, word or long word. */
#define UNICOMMUART_SPGDMATXCTL_DMASRCWDTH_BYTE  ((uint32_t)0x00000000U)         /* !< Source data width is BYTE (8-bit) */
#define UNICOMMUART_SPGDMATXCTL_DMASRCWDTH_HALF  ((uint32_t)0x10000000U)         /* !< Source data width is HALF-WORD
                                                                                    (16-bit) */
#define UNICOMMUART_SPGDMATXCTL_DMASRCWDTH_WORD  ((uint32_t)0x20000000U)         /* !< Source data width is WORD (32-bit) */

/* UNICOMMUART_SPGDMATXSA Bits */
/* UNICOMMUART_SPGDMATXSA[ADDR] Bits */
#define UNICOMMUART_SPGDMATXSA_ADDR_OFS          (0)                             /* !< ADDR Offset */
#define UNICOMMUART_SPGDMATXSA_ADDR_MASK         ((uint32_t)0xFFFFFFFFU)         /* !< DMA Channel Destination Address */
#define UNICOMMUART_SPGDMATXSA_ADDR_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_SPGDMATXSA_ADDR_MAXIMUM      ((uint32_t)0xFFFFFFFFU)         /* !< Highest possible value */

/* UNICOMMUART_SPGDMATXSZ Bits */
/* UNICOMMUART_SPGDMATXSZ[SIZE] Bits */
#define UNICOMMUART_SPGDMATXSZ_SIZE_OFS          (0)                             /* !< SIZE Offset */
#define UNICOMMUART_SPGDMATXSZ_SIZE_MASK         ((uint32_t)0x0000FFFFU)         /* !< DMA Channel Size in number of
                                                                                    transfers */
#define UNICOMMUART_SPGDMATXSZ_SIZE_MINIMUM      ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_SPGDMATXSZ_SIZE_MAXIMUM      ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMUART_DMA_TRIG_TX_IMASK Bits */
/* UNICOMMUART_DMA_TRIG_TX_IMASK[TXINT] Bits */
#define UNICOMMUART_DMA_TRIG_TX_IMASK_TXINT_OFS  (11)                            /* !< TXINT Offset */
#define UNICOMMUART_DMA_TRIG_TX_IMASK_TXINT_MASK ((uint32_t)0x00000800U)         /* !< Enable UART Transmit Interrupt. */
#define UNICOMMUART_DMA_TRIG_TX_IMASK_TXINT_CLR  ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_DMA_TRIG_TX_IMASK_TXINT_SET  ((uint32_t)0x00000800U)         /* !< Set Interrupt Mask */

/* UNICOMMUART_DMA_TRIG_TX_RIS Bits */
/* UNICOMMUART_DMA_TRIG_TX_RIS[TXINT] Bits */
#define UNICOMMUART_DMA_TRIG_TX_RIS_TXINT_OFS    (11)                            /* !< TXINT Offset */
#define UNICOMMUART_DMA_TRIG_TX_RIS_TXINT_MASK   ((uint32_t)0x00000800U)         /* !< UART Transmit Interrupt. */
#define UNICOMMUART_DMA_TRIG_TX_RIS_TXINT_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_DMA_TRIG_TX_RIS_TXINT_SET    ((uint32_t)0x00000800U)         /* !< Interrupt occurred */

/* UNICOMMUART_DMA_TRIG_TX_MIS Bits */
/* UNICOMMUART_DMA_TRIG_TX_MIS[TXINT] Bits */
#define UNICOMMUART_DMA_TRIG_TX_MIS_TXINT_OFS    (11)                            /* !< TXINT Offset */
#define UNICOMMUART_DMA_TRIG_TX_MIS_TXINT_MASK   ((uint32_t)0x00000800U)         /* !< Masked UART Transmit Interrupt. */
#define UNICOMMUART_DMA_TRIG_TX_MIS_TXINT_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_DMA_TRIG_TX_MIS_TXINT_SET    ((uint32_t)0x00000800U)         /* !< Interrupt occurred */

/* UNICOMMUART_DMA_TRIG_TX_ISET Bits */
/* UNICOMMUART_DMA_TRIG_TX_ISET[TXINT] Bits */
#define UNICOMMUART_DMA_TRIG_TX_ISET_TXINT_OFS   (11)                            /* !< TXINT Offset */
#define UNICOMMUART_DMA_TRIG_TX_ISET_TXINT_MASK  ((uint32_t)0x00000800U)         /* !< Set UART Transmit Interrupt. */
#define UNICOMMUART_DMA_TRIG_TX_ISET_TXINT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_DMA_TRIG_TX_ISET_TXINT_SET   ((uint32_t)0x00000800U)         /* !< Set Interrupt */

/* UNICOMMUART_DMA_TRIG_RX_IMASK Bits */
/* UNICOMMUART_DMA_TRIG_RX_IMASK[RTOUT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RTOUT_OFS  (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RTOUT_MASK ((uint32_t)0x00000001U)         /* !< Enable UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RTOUT_CLR  ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RTOUT_SET  ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_DMA_TRIG_RX_IMASK[RXINT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RXINT_OFS  (10)                            /* !< RXINT Offset */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RXINT_MASK ((uint32_t)0x00000400U)         /* !< Enable UART Receive Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RXINT_CLR  ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_DMA_TRIG_RX_IMASK_RXINT_SET  ((uint32_t)0x00000400U)         /* !< Set Interrupt Mask */

/* UNICOMMUART_DMA_TRIG_RX_RIS Bits */
/* UNICOMMUART_DMA_TRIG_RX_RIS[RTOUT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RTOUT_OFS    (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RTOUT_MASK   ((uint32_t)0x00000001U)         /* !< UARTOUT Receive Time-Out Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RTOUT_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RTOUT_SET    ((uint32_t)0x00000001U)         /* !< Interrupt occurred */
/* UNICOMMUART_DMA_TRIG_RX_RIS[RXINT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RXINT_OFS    (10)                            /* !< RXINT Offset */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RXINT_MASK   ((uint32_t)0x00000400U)         /* !< UART Receive Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RXINT_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_DMA_TRIG_RX_RIS_RXINT_SET    ((uint32_t)0x00000400U)         /* !< Interrupt occurred */

/* UNICOMMUART_DMA_TRIG_RX_MIS Bits */
/* UNICOMMUART_DMA_TRIG_RX_MIS[RTOUT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RTOUT_OFS    (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RTOUT_MASK   ((uint32_t)0x00000001U)         /* !< Masked UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RTOUT_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RTOUT_SET    ((uint32_t)0x00000001U)         /* !< Interrupt occurred */
/* UNICOMMUART_DMA_TRIG_RX_MIS[RXINT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RXINT_OFS    (10)                            /* !< RXINT Offset */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RXINT_MASK   ((uint32_t)0x00000400U)         /* !< Masked UART Receive Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RXINT_CLR    ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_DMA_TRIG_RX_MIS_RXINT_SET    ((uint32_t)0x00000400U)         /* !< Interrupt occurred */

/* UNICOMMUART_DMA_TRIG_RX_ISET Bits */
/* UNICOMMUART_DMA_TRIG_RX_ISET[RTOUT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RTOUT_OFS   (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RTOUT_MASK  ((uint32_t)0x00000001U)         /* !< Set UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RTOUT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RTOUT_SET   ((uint32_t)0x00000001U)         /* !< Set Interrupt */
/* UNICOMMUART_DMA_TRIG_RX_ISET[RXINT] Bits */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RXINT_OFS   (10)                            /* !< RXINT Offset */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RXINT_MASK  ((uint32_t)0x00000400U)         /* !< Set UART Receive Interrupt. */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RXINT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_DMA_TRIG_RX_ISET_RXINT_SET   ((uint32_t)0x00000400U)         /* !< Set Interrupt */

/* UNICOMMUART_IIDX Bits */
/* UNICOMMUART_IIDX[STAT] Bits */
#define UNICOMMUART_IIDX_STAT_OFS                (0)                             /* !< STAT Offset */
#define UNICOMMUART_IIDX_STAT_MASK               ((uint32_t)0x000000FFU)         /* !< UART Module Interrupt Vector Value.
                                                                                    This register provides the highes
                                                                                    priority interrupt index. A read
                                                                                    clears the corresponding interrupt
                                                                                    flag in RIS and MIS registers.
                                                                                    15h-1Fh = Reserved */
#define UNICOMMUART_IIDX_STAT_NO_INTR            ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define UNICOMMUART_IIDX_STAT_RTFG               ((uint32_t)0x00000001U)         /* !< UART receive time-out interrupt;
                                                                                    Interrupt Flag: RT; Interrupt
                                                                                    Priority: Highest */
#define UNICOMMUART_IIDX_STAT_FEFG               ((uint32_t)0x00000002U)         /* !< UART framing error interrupt;
                                                                                    Interrupt Flag: FE */
#define UNICOMMUART_IIDX_STAT_PEFG               ((uint32_t)0x00000003U)         /* !< UART parity error interrupt;
                                                                                    Interrupt Flag: PE */
#define UNICOMMUART_IIDX_STAT_BEFG               ((uint32_t)0x00000004U)         /* !< UART break error interrupt;
                                                                                    Interrupt Flag: BE */
#define UNICOMMUART_IIDX_STAT_OEFG               ((uint32_t)0x00000005U)         /* !< UART receive overrun error
                                                                                    interrupt; Interrupt Flag: OE */
#define UNICOMMUART_IIDX_STAT_RXNE               ((uint32_t)0x00000006U)         /* !< Negative edge on UARTxRXD
                                                                                    interrupt; Interrupt Flag: RXNE */
#define UNICOMMUART_IIDX_STAT_RXPE               ((uint32_t)0x00000007U)         /* !< Positive edge on UARTxRXD
                                                                                    interrupt; Interrupt Flag: RXPE */
#define UNICOMMUART_IIDX_STAT_LINC0              ((uint32_t)0x00000008U)         /* !< LIN capture 0 / match interrupt;
                                                                                    Interrupt Flag: LINC0 */
#define UNICOMMUART_IIDX_STAT_LINC1              ((uint32_t)0x00000009U)         /* !< LIN capture 1 interrupt; Interrupt
                                                                                    Flag: LINC1 */
#define UNICOMMUART_IIDX_STAT_LINOVF             ((uint32_t)0x0000000AU)         /* !< LIN hardware counter overflow
                                                                                    interrupt; Interrupt Flag: LINOVF */
#define UNICOMMUART_IIDX_STAT_RXIFG              ((uint32_t)0x0000000BU)         /* !< UART receive interrupt; Interrupt
                                                                                    Flag: RX */
#define UNICOMMUART_IIDX_STAT_TXIFG              ((uint32_t)0x0000000CU)         /* !< UART transmit interrupt; Interrupt
                                                                                    Flag: TX */
#define UNICOMMUART_IIDX_STAT_EOT                ((uint32_t)0x0000000DU)         /* !< UART end of transmission interrupt
                                                                                    (transmit serializer empty);
                                                                                    Interrupt Flag: EOT */
#define UNICOMMUART_IIDX_STAT_ADDR_MATCH         ((uint32_t)0x0000000EU)         /* !< 9-bit mode address match interrupt;
                                                                                    Interrupt Flag: ADDR_MATCH */
#define UNICOMMUART_IIDX_STAT_CTS                ((uint32_t)0x0000000FU)         /* !< UART Clear to Send Modem interrupt;
                                                                                    Interrupt Flag: CTS */
#define UNICOMMUART_IIDX_STAT_DMA_DONE_RX        ((uint32_t)0x00000010U)         /* !< DMA DONE on RX */
#define UNICOMMUART_IIDX_STAT_DMA_DONE_TX        ((uint32_t)0x00000011U)         /* !< DMA DONE on TX */
#define UNICOMMUART_IIDX_STAT_NERR_EVT           ((uint32_t)0x00000012U)         /* !< Noise Error Event */
#define UNICOMMUART_IIDX_STAT_DMA_PREIRQ_RX      ((uint32_t)0x00000013U)         /* !< DMA PRE IRQ INTERRUPT */
#define UNICOMMUART_IIDX_STAT_DMA_PREIRQ_TX      ((uint32_t)0x00000014U)         /* !< DMA PRE IRQ INTERRUPT */
#define UNICOMMUART_IIDX_STAT_LTFG               ((uint32_t)0x00000015U)         /* !< UART line time-out interrupt;
                                                                                    Interrupt Flag */

/* UNICOMMUART_CPU_INT_IMASK Bits */
/* UNICOMMUART_CPU_INT_IMASK[FRMERR] Bits */
#define UNICOMMUART_CPU_INT_IMASK_FRMERR_OFS     (1)                             /* !< FRMERR Offset */
#define UNICOMMUART_CPU_INT_IMASK_FRMERR_MASK    ((uint32_t)0x00000002U)         /* !< Enable UART Framing Error
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_FRMERR_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_FRMERR_SET     ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[PARERR] Bits */
#define UNICOMMUART_CPU_INT_IMASK_PARERR_OFS     (2)                             /* !< PARERR Offset */
#define UNICOMMUART_CPU_INT_IMASK_PARERR_MASK    ((uint32_t)0x00000004U)         /* !< Enable UART Parity Error Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_PARERR_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_PARERR_SET     ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[BRKERR] Bits */
#define UNICOMMUART_CPU_INT_IMASK_BRKERR_OFS     (3)                             /* !< BRKERR Offset */
#define UNICOMMUART_CPU_INT_IMASK_BRKERR_MASK    ((uint32_t)0x00000008U)         /* !< Enable UART Break Error Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_BRKERR_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_BRKERR_SET     ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[OVRERR] Bits */
#define UNICOMMUART_CPU_INT_IMASK_OVRERR_OFS     (4)                             /* !< OVRERR Offset */
#define UNICOMMUART_CPU_INT_IMASK_OVRERR_MASK    ((uint32_t)0x00000010U)         /* !< Enable UART Receive Overrun Error
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_OVRERR_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_OVRERR_SET     ((uint32_t)0x00000010U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[RXNE] Bits */
#define UNICOMMUART_CPU_INT_IMASK_RXNE_OFS       (5)                             /* !< RXNE Offset */
#define UNICOMMUART_CPU_INT_IMASK_RXNE_MASK      ((uint32_t)0x00000020U)         /* !< Enable Negative Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_RXNE_CLR       ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_RXNE_SET       ((uint32_t)0x00000020U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[RXPE] Bits */
#define UNICOMMUART_CPU_INT_IMASK_RXPE_OFS       (6)                             /* !< RXPE Offset */
#define UNICOMMUART_CPU_INT_IMASK_RXPE_MASK      ((uint32_t)0x00000040U)         /* !< Enable Positive Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_RXPE_CLR       ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_RXPE_SET       ((uint32_t)0x00000040U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[LINOVF] Bits */
#define UNICOMMUART_CPU_INT_IMASK_LINOVF_OFS     (9)                             /* !< LINOVF Offset */
#define UNICOMMUART_CPU_INT_IMASK_LINOVF_MASK    ((uint32_t)0x00000200U)         /* !< Enable LIN Hardware Counter
                                                                                    Overflow Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_LINOVF_CLR     ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_LINOVF_SET     ((uint32_t)0x00000200U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[RXINT] Bits */
#define UNICOMMUART_CPU_INT_IMASK_RXINT_OFS      (10)                            /* !< RXINT Offset */
#define UNICOMMUART_CPU_INT_IMASK_RXINT_MASK     ((uint32_t)0x00000400U)         /* !< Enable UART Receive Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_RXINT_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_RXINT_SET      ((uint32_t)0x00000400U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[TXINT] Bits */
#define UNICOMMUART_CPU_INT_IMASK_TXINT_OFS      (11)                            /* !< TXINT Offset */
#define UNICOMMUART_CPU_INT_IMASK_TXINT_MASK     ((uint32_t)0x00000800U)         /* !< Enable UART Transmit Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_TXINT_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_TXINT_SET      ((uint32_t)0x00000800U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[EOT] Bits */
#define UNICOMMUART_CPU_INT_IMASK_EOT_OFS        (12)                            /* !< EOT Offset */
#define UNICOMMUART_CPU_INT_IMASK_EOT_MASK       ((uint32_t)0x00001000U)         /* !< Enable UART End of Transmission
                                                                                    Interrupt Indicates that the last bit
                                                                                    of all transmitted data and flags has
                                                                                    left the serializer and without any
                                                                                    further Data in the TX Fifo or
                                                                                    Buffer. */
#define UNICOMMUART_CPU_INT_IMASK_EOT_CLR        ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_EOT_SET        ((uint32_t)0x00001000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[ADDR_MATCH] Bits */
#define UNICOMMUART_CPU_INT_IMASK_ADDR_MATCH_OFS (13)                            /* !< ADDR_MATCH Offset */
#define UNICOMMUART_CPU_INT_IMASK_ADDR_MATCH_MASK ((uint32_t)0x00002000U)         /* !< Enable Address Match Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_ADDR_MATCH_CLR ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_ADDR_MATCH_SET ((uint32_t)0x00002000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[CTS] Bits */
#define UNICOMMUART_CPU_INT_IMASK_CTS_OFS        (14)                            /* !< CTS Offset */
#define UNICOMMUART_CPU_INT_IMASK_CTS_MASK       ((uint32_t)0x00004000U)         /* !< Enable UART Clear to Send Modem
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_CTS_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_IMASK_CTS_SET        ((uint32_t)0x00004000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[DMA_DONE_RX] Bits */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_RX_OFS (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< Enable DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_RX_SET ((uint32_t)0x00008000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[DMA_DONE_TX] Bits */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_TX_OFS (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< Enable DMA Done on TX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_IMASK_DMA_DONE_TX_SET ((uint32_t)0x00010000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[RTOUT] Bits */
#define UNICOMMUART_CPU_INT_IMASK_RTOUT_OFS      (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_CPU_INT_IMASK_RTOUT_MASK     ((uint32_t)0x00000001U)         /* !< Enable UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_RTOUT_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_RTOUT_SET      ((uint32_t)0x00000001U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[LINC0] Bits */
#define UNICOMMUART_CPU_INT_IMASK_LINC0_OFS      (7)                             /* !< LINC0 Offset */
#define UNICOMMUART_CPU_INT_IMASK_LINC0_MASK     ((uint32_t)0x00000080U)         /* !< Enable LIN Capture 0 / Match
                                                                                    Interrupt . */
#define UNICOMMUART_CPU_INT_IMASK_LINC0_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_LINC0_SET      ((uint32_t)0x00000080U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[LINC1] Bits */
#define UNICOMMUART_CPU_INT_IMASK_LINC1_OFS      (8)                             /* !< LINC1 Offset */
#define UNICOMMUART_CPU_INT_IMASK_LINC1_MASK     ((uint32_t)0x00000100U)         /* !< Enable LIN Capture 1 Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_LINC1_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_LINC1_SET      ((uint32_t)0x00000100U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[NERR] Bits */
#define UNICOMMUART_CPU_INT_IMASK_NERR_OFS       (17)                            /* !< NERR Offset */
#define UNICOMMUART_CPU_INT_IMASK_NERR_MASK      ((uint32_t)0x00020000U)         /* !< Noise Error on triple voting.
                                                                                    Asserted when the 3 samples of
                                                                                    majority voting are not equal */
#define UNICOMMUART_CPU_INT_IMASK_NERR_CLR       ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_NERR_SET       ((uint32_t)0x00020000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[DMA_PREIRQ_RX] Bits */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< Enable DMA Done PREIRQ RX interrupt */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[DMA_PREIRQ_TX] Bits */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< Enable DMA Done PREIRQ TX interrupt */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_IMASK_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Set Interrupt Mask */
/* UNICOMMUART_CPU_INT_IMASK[LTOUT] Bits */
#define UNICOMMUART_CPU_INT_IMASK_LTOUT_OFS      (20)                            /* !< LTOUT Offset */
#define UNICOMMUART_CPU_INT_IMASK_LTOUT_MASK     ((uint32_t)0x00100000U)         /* !< Enable UARTOUT Line Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_IMASK_LTOUT_CLR      ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define UNICOMMUART_CPU_INT_IMASK_LTOUT_SET      ((uint32_t)0x00100000U)         /* !< Set Interrupt Mask */

/* UNICOMMUART_CPU_INT_RIS Bits */
/* UNICOMMUART_CPU_INT_RIS[RTOUT] Bits */
#define UNICOMMUART_CPU_INT_RIS_RTOUT_OFS        (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_CPU_INT_RIS_RTOUT_MASK       ((uint32_t)0x00000001U)         /* !< UARTOUT Receive Time-Out Interrupt.
                                                                                    Interupt is raised when a time-out
                                                                                    has lapsed and there are still items
                                                                                    in the FIFO */
#define UNICOMMUART_CPU_INT_RIS_RTOUT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_RTOUT_SET        ((uint32_t)0x00000001U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[FRMERR] Bits */
#define UNICOMMUART_CPU_INT_RIS_FRMERR_OFS       (1)                             /* !< FRMERR Offset */
#define UNICOMMUART_CPU_INT_RIS_FRMERR_MASK      ((uint32_t)0x00000002U)         /* !< UART Framing Error Interrupt.
                                                                                    Framing error interrupt is raised
                                                                                    when a "low" stop bit is detected. */
#define UNICOMMUART_CPU_INT_RIS_FRMERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_FRMERR_SET       ((uint32_t)0x00000002U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[PARERR] Bits */
#define UNICOMMUART_CPU_INT_RIS_PARERR_OFS       (2)                             /* !< PARERR Offset */
#define UNICOMMUART_CPU_INT_RIS_PARERR_MASK      ((uint32_t)0x00000004U)         /* !< UART Parity Error Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_PARERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_PARERR_SET       ((uint32_t)0x00000004U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[BRKERR] Bits */
#define UNICOMMUART_CPU_INT_RIS_BRKERR_OFS       (3)                             /* !< BRKERR Offset */
#define UNICOMMUART_CPU_INT_RIS_BRKERR_MASK      ((uint32_t)0x00000008U)         /* !< UART Break Error Interrupt.   Break
                                                                                    Error Interrupt is raised when all
                                                                                    data bits, parity bit and stop bits
                                                                                    are '0' */
#define UNICOMMUART_CPU_INT_RIS_BRKERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_BRKERR_SET       ((uint32_t)0x00000008U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[OVRERR] Bits */
#define UNICOMMUART_CPU_INT_RIS_OVRERR_OFS       (4)                             /* !< OVRERR Offset */
#define UNICOMMUART_CPU_INT_RIS_OVRERR_MASK      ((uint32_t)0x00000010U)         /* !< UART Receive Overrun Error
                                                                                    Interrupt.   UART Receive Overrun
                                                                                    Error Interrupt is raised when the
                                                                                    receive FIFO is flow and a new frame
                                                                                    is received */
#define UNICOMMUART_CPU_INT_RIS_OVRERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_OVRERR_SET       ((uint32_t)0x00000010U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[RXNE] Bits */
#define UNICOMMUART_CPU_INT_RIS_RXNE_OFS         (5)                             /* !< RXNE Offset */
#define UNICOMMUART_CPU_INT_RIS_RXNE_MASK        ((uint32_t)0x00000020U)         /* !< Negative Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_RXNE_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_RXNE_SET         ((uint32_t)0x00000020U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[RXPE] Bits */
#define UNICOMMUART_CPU_INT_RIS_RXPE_OFS         (6)                             /* !< RXPE Offset */
#define UNICOMMUART_CPU_INT_RIS_RXPE_MASK        ((uint32_t)0x00000040U)         /* !< Positive Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_RXPE_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_RXPE_SET         ((uint32_t)0x00000040U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[LINC0] Bits */
#define UNICOMMUART_CPU_INT_RIS_LINC0_OFS        (7)                             /* !< LINC0 Offset */
#define UNICOMMUART_CPU_INT_RIS_LINC0_MASK       ((uint32_t)0x00000080U)         /* !< LIN Capture 0 / Match Interrupt . */
#define UNICOMMUART_CPU_INT_RIS_LINC0_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_LINC0_SET        ((uint32_t)0x00000080U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[LINC1] Bits */
#define UNICOMMUART_CPU_INT_RIS_LINC1_OFS        (8)                             /* !< LINC1 Offset */
#define UNICOMMUART_CPU_INT_RIS_LINC1_MASK       ((uint32_t)0x00000100U)         /* !< LIN Capture 1 Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_LINC1_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_LINC1_SET        ((uint32_t)0x00000100U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[LINOVF] Bits */
#define UNICOMMUART_CPU_INT_RIS_LINOVF_OFS       (9)                             /* !< LINOVF Offset */
#define UNICOMMUART_CPU_INT_RIS_LINOVF_MASK      ((uint32_t)0x00000200U)         /* !< LIN Hardware Counter Overflow
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_LINOVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_LINOVF_SET       ((uint32_t)0x00000200U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[RXINT] Bits */
#define UNICOMMUART_CPU_INT_RIS_RXINT_OFS        (10)                            /* !< RXINT Offset */
#define UNICOMMUART_CPU_INT_RIS_RXINT_MASK       ((uint32_t)0x00000400U)         /* !< UART Receive Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_RXINT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_RXINT_SET        ((uint32_t)0x00000400U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[TXINT] Bits */
#define UNICOMMUART_CPU_INT_RIS_TXINT_OFS        (11)                            /* !< TXINT Offset */
#define UNICOMMUART_CPU_INT_RIS_TXINT_MASK       ((uint32_t)0x00000800U)         /* !< UART Transmit Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_TXINT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_TXINT_SET        ((uint32_t)0x00000800U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[EOT] Bits */
#define UNICOMMUART_CPU_INT_RIS_EOT_OFS          (12)                            /* !< EOT Offset */
#define UNICOMMUART_CPU_INT_RIS_EOT_MASK         ((uint32_t)0x00001000U)         /* !< UART End of Transmission Interrupt
                                                                                    Indicates that the last bit of all
                                                                                    transmitted data and flags has left
                                                                                    the serializer and without any
                                                                                    further Data in the TX Fifo or
                                                                                    Buffer. */
#define UNICOMMUART_CPU_INT_RIS_EOT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_EOT_SET          ((uint32_t)0x00001000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[ADDR_MATCH] Bits */
#define UNICOMMUART_CPU_INT_RIS_ADDR_MATCH_OFS   (13)                            /* !< ADDR_MATCH Offset */
#define UNICOMMUART_CPU_INT_RIS_ADDR_MATCH_MASK  ((uint32_t)0x00002000U)         /* !< Address Match Interrupt. */
#define UNICOMMUART_CPU_INT_RIS_ADDR_MATCH_CLR   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_ADDR_MATCH_SET   ((uint32_t)0x00002000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[CTS] Bits */
#define UNICOMMUART_CPU_INT_RIS_CTS_OFS          (14)                            /* !< CTS Offset */
#define UNICOMMUART_CPU_INT_RIS_CTS_MASK         ((uint32_t)0x00004000U)         /* !< UART Clear to Send Modem Interrupt.
                                                                                    This interrupt is raised when CTS
                                                                                    toggles, either from '0' to '1' or
                                                                                    from '1' to '0'. */
#define UNICOMMUART_CPU_INT_RIS_CTS_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_RIS_CTS_SET          ((uint32_t)0x00004000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[DMA_DONE_RX] Bits */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_RX_OFS  (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_RX_CLR  ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_RX_SET  ((uint32_t)0x00008000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[DMA_DONE_TX] Bits */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_TX_OFS  (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< DMA Done on TX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_TX_CLR  ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_RIS_DMA_DONE_TX_SET  ((uint32_t)0x00010000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[NERR] Bits */
#define UNICOMMUART_CPU_INT_RIS_NERR_OFS         (17)                            /* !< NERR Offset */
#define UNICOMMUART_CPU_INT_RIS_NERR_MASK        ((uint32_t)0x00020000U)         /* !< Noise Error on triple voting.
                                                                                    Asserted when the 3 samples of
                                                                                    majority voting are not equal */
#define UNICOMMUART_CPU_INT_RIS_NERR_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_NERR_SET         ((uint32_t)0x00020000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[DMA_PREIRQ_RX] Bits */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< DMA Pre-emptive Interrupt Done on
                                                                                    RX Event Channel Interrupt */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[DMA_PREIRQ_TX] Bits */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< DMA Pre-Emptive Done on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_RIS_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_RIS[LTOUT] Bits */
#define UNICOMMUART_CPU_INT_RIS_LTOUT_OFS        (20)                            /* !< LTOUT Offset */
#define UNICOMMUART_CPU_INT_RIS_LTOUT_MASK       ((uint32_t)0x00100000U)         /* !< UARTOUT Line Time-Out Interrupt.
                                                                                    Set: no start edge has been detected
                                                                                    for an additional character period
                                                                                    after reception of last character.
                                                                                    This is irrespective of FIFO contents
                                                                                    Clear: timeout has not occured */
#define UNICOMMUART_CPU_INT_RIS_LTOUT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_RIS_LTOUT_SET        ((uint32_t)0x00100000U)         /* !< Interrupt occurred */

/* UNICOMMUART_CPU_INT_MIS Bits */
/* UNICOMMUART_CPU_INT_MIS[RTOUT] Bits */
#define UNICOMMUART_CPU_INT_MIS_RTOUT_OFS        (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_CPU_INT_MIS_RTOUT_MASK       ((uint32_t)0x00000001U)         /* !< Masked UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_RTOUT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_RTOUT_SET        ((uint32_t)0x00000001U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[FRMERR] Bits */
#define UNICOMMUART_CPU_INT_MIS_FRMERR_OFS       (1)                             /* !< FRMERR Offset */
#define UNICOMMUART_CPU_INT_MIS_FRMERR_MASK      ((uint32_t)0x00000002U)         /* !< Masked UART Framing Error
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_FRMERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_FRMERR_SET       ((uint32_t)0x00000002U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[PARERR] Bits */
#define UNICOMMUART_CPU_INT_MIS_PARERR_OFS       (2)                             /* !< PARERR Offset */
#define UNICOMMUART_CPU_INT_MIS_PARERR_MASK      ((uint32_t)0x00000004U)         /* !< Masked UART Parity Error Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_PARERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_PARERR_SET       ((uint32_t)0x00000004U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[BRKERR] Bits */
#define UNICOMMUART_CPU_INT_MIS_BRKERR_OFS       (3)                             /* !< BRKERR Offset */
#define UNICOMMUART_CPU_INT_MIS_BRKERR_MASK      ((uint32_t)0x00000008U)         /* !< Masked UART Break Error Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_BRKERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_BRKERR_SET       ((uint32_t)0x00000008U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[OVRERR] Bits */
#define UNICOMMUART_CPU_INT_MIS_OVRERR_OFS       (4)                             /* !< OVRERR Offset */
#define UNICOMMUART_CPU_INT_MIS_OVRERR_MASK      ((uint32_t)0x00000010U)         /* !< Masked UART Receive Overrun Error
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_OVRERR_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_OVRERR_SET       ((uint32_t)0x00000010U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[RXNE] Bits */
#define UNICOMMUART_CPU_INT_MIS_RXNE_OFS         (5)                             /* !< RXNE Offset */
#define UNICOMMUART_CPU_INT_MIS_RXNE_MASK        ((uint32_t)0x00000020U)         /* !< Masked Negative Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_RXNE_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_RXNE_SET         ((uint32_t)0x00000020U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[RXPE] Bits */
#define UNICOMMUART_CPU_INT_MIS_RXPE_OFS         (6)                             /* !< RXPE Offset */
#define UNICOMMUART_CPU_INT_MIS_RXPE_MASK        ((uint32_t)0x00000040U)         /* !< Masked Positive Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_RXPE_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_RXPE_SET         ((uint32_t)0x00000040U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[LINC0] Bits */
#define UNICOMMUART_CPU_INT_MIS_LINC0_OFS        (7)                             /* !< LINC0 Offset */
#define UNICOMMUART_CPU_INT_MIS_LINC0_MASK       ((uint32_t)0x00000080U)         /* !< Masked LIN Capture 0 / Match
                                                                                    Interrupt . */
#define UNICOMMUART_CPU_INT_MIS_LINC0_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_LINC0_SET        ((uint32_t)0x00000080U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[LINC1] Bits */
#define UNICOMMUART_CPU_INT_MIS_LINC1_OFS        (8)                             /* !< LINC1 Offset */
#define UNICOMMUART_CPU_INT_MIS_LINC1_MASK       ((uint32_t)0x00000100U)         /* !< Masked LIN Capture 1 Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_LINC1_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_LINC1_SET        ((uint32_t)0x00000100U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[LINOVF] Bits */
#define UNICOMMUART_CPU_INT_MIS_LINOVF_OFS       (9)                             /* !< LINOVF Offset */
#define UNICOMMUART_CPU_INT_MIS_LINOVF_MASK      ((uint32_t)0x00000200U)         /* !< Masked LIN Hardware Counter
                                                                                    Overflow Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_LINOVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_LINOVF_SET       ((uint32_t)0x00000200U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[RXINT] Bits */
#define UNICOMMUART_CPU_INT_MIS_RXINT_OFS        (10)                            /* !< RXINT Offset */
#define UNICOMMUART_CPU_INT_MIS_RXINT_MASK       ((uint32_t)0x00000400U)         /* !< Masked UART Receive Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_RXINT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_RXINT_SET        ((uint32_t)0x00000400U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[TXINT] Bits */
#define UNICOMMUART_CPU_INT_MIS_TXINT_OFS        (11)                            /* !< TXINT Offset */
#define UNICOMMUART_CPU_INT_MIS_TXINT_MASK       ((uint32_t)0x00000800U)         /* !< Masked UART Transmit Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_TXINT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_TXINT_SET        ((uint32_t)0x00000800U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[EOT] Bits */
#define UNICOMMUART_CPU_INT_MIS_EOT_OFS          (12)                            /* !< EOT Offset */
#define UNICOMMUART_CPU_INT_MIS_EOT_MASK         ((uint32_t)0x00001000U)         /* !< UART End of Transmission Interrupt
                                                                                    Indicates that the last bit of all
                                                                                    transmitted data and flags has left
                                                                                    the serializer and without any
                                                                                    further Data in the TX Fifo or
                                                                                    Buffer. */
#define UNICOMMUART_CPU_INT_MIS_EOT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_EOT_SET          ((uint32_t)0x00001000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[ADDR_MATCH] Bits */
#define UNICOMMUART_CPU_INT_MIS_ADDR_MATCH_OFS   (13)                            /* !< ADDR_MATCH Offset */
#define UNICOMMUART_CPU_INT_MIS_ADDR_MATCH_MASK  ((uint32_t)0x00002000U)         /* !< Masked Address Match Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_ADDR_MATCH_CLR   ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_ADDR_MATCH_SET   ((uint32_t)0x00002000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[CTS] Bits */
#define UNICOMMUART_CPU_INT_MIS_CTS_OFS          (14)                            /* !< CTS Offset */
#define UNICOMMUART_CPU_INT_MIS_CTS_MASK         ((uint32_t)0x00004000U)         /* !< Masked UART Clear to Send Modem
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_CTS_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_CTS_SET          ((uint32_t)0x00004000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[DMA_DONE_RX] Bits */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_RX_OFS  (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< Masked DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_RX_CLR  ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_RX_SET  ((uint32_t)0x00008000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[DMA_DONE_TX] Bits */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_TX_OFS  (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< Masked DMA Done on TX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_TX_CLR  ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_DMA_DONE_TX_SET  ((uint32_t)0x00010000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[NERR] Bits */
#define UNICOMMUART_CPU_INT_MIS_NERR_OFS         (17)                            /* !< NERR Offset */
#define UNICOMMUART_CPU_INT_MIS_NERR_MASK        ((uint32_t)0x00020000U)         /* !< Noise Error on triple voting.
                                                                                    Asserted when the 3 samples of
                                                                                    majority voting are not equal */
#define UNICOMMUART_CPU_INT_MIS_NERR_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_NERR_SET         ((uint32_t)0x00020000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[DMA_PREIRQ_RX] Bits */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< Masked DMA PREIRQ Done on RX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_RX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[DMA_PREIRQ_TX] Bits */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< Masked DMA Done PREIRQ on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_TX_CLR ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Interrupt occurred */
/* UNICOMMUART_CPU_INT_MIS[LTOUT] Bits */
#define UNICOMMUART_CPU_INT_MIS_LTOUT_OFS        (20)                            /* !< LTOUT Offset */
#define UNICOMMUART_CPU_INT_MIS_LTOUT_MASK       ((uint32_t)0x00100000U)         /* !< Masked UARTOUT Line Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_MIS_LTOUT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define UNICOMMUART_CPU_INT_MIS_LTOUT_SET        ((uint32_t)0x00100000U)         /* !< Interrupt occurred */

/* UNICOMMUART_CPU_INT_ISET Bits */
/* UNICOMMUART_CPU_INT_ISET[FRMERR] Bits */
#define UNICOMMUART_CPU_INT_ISET_FRMERR_OFS      (1)                             /* !< FRMERR Offset */
#define UNICOMMUART_CPU_INT_ISET_FRMERR_MASK     ((uint32_t)0x00000002U)         /* !< Set UART Framing Error Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_FRMERR_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_FRMERR_SET      ((uint32_t)0x00000002U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[PARERR] Bits */
#define UNICOMMUART_CPU_INT_ISET_PARERR_OFS      (2)                             /* !< PARERR Offset */
#define UNICOMMUART_CPU_INT_ISET_PARERR_MASK     ((uint32_t)0x00000004U)         /* !< Set UART Parity Error Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_PARERR_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_PARERR_SET      ((uint32_t)0x00000004U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[BRKERR] Bits */
#define UNICOMMUART_CPU_INT_ISET_BRKERR_OFS      (3)                             /* !< BRKERR Offset */
#define UNICOMMUART_CPU_INT_ISET_BRKERR_MASK     ((uint32_t)0x00000008U)         /* !< Set UART Break Error Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_BRKERR_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_BRKERR_SET      ((uint32_t)0x00000008U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[OVRERR] Bits */
#define UNICOMMUART_CPU_INT_ISET_OVRERR_OFS      (4)                             /* !< OVRERR Offset */
#define UNICOMMUART_CPU_INT_ISET_OVRERR_MASK     ((uint32_t)0x00000010U)         /* !< Set  UART Receive Overrun Error
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_OVRERR_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_OVRERR_SET      ((uint32_t)0x00000010U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[RXNE] Bits */
#define UNICOMMUART_CPU_INT_ISET_RXNE_OFS        (5)                             /* !< RXNE Offset */
#define UNICOMMUART_CPU_INT_ISET_RXNE_MASK       ((uint32_t)0x00000020U)         /* !< Set Negative Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_RXNE_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_RXNE_SET        ((uint32_t)0x00000020U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[RXPE] Bits */
#define UNICOMMUART_CPU_INT_ISET_RXPE_OFS        (6)                             /* !< RXPE Offset */
#define UNICOMMUART_CPU_INT_ISET_RXPE_MASK       ((uint32_t)0x00000040U)         /* !< Set Positive Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_RXPE_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_RXPE_SET        ((uint32_t)0x00000040U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[LINC0] Bits */
#define UNICOMMUART_CPU_INT_ISET_LINC0_OFS       (7)                             /* !< LINC0 Offset */
#define UNICOMMUART_CPU_INT_ISET_LINC0_MASK      ((uint32_t)0x00000080U)         /* !< Set LIN Capture 0 / Match Interrupt
                                                                                    . */
#define UNICOMMUART_CPU_INT_ISET_LINC0_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_LINC0_SET       ((uint32_t)0x00000080U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[LINC1] Bits */
#define UNICOMMUART_CPU_INT_ISET_LINC1_OFS       (8)                             /* !< LINC1 Offset */
#define UNICOMMUART_CPU_INT_ISET_LINC1_MASK      ((uint32_t)0x00000100U)         /* !< Set LIN Capture 1 Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_LINC1_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_LINC1_SET       ((uint32_t)0x00000100U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[LINOVF] Bits */
#define UNICOMMUART_CPU_INT_ISET_LINOVF_OFS      (9)                             /* !< LINOVF Offset */
#define UNICOMMUART_CPU_INT_ISET_LINOVF_MASK     ((uint32_t)0x00000200U)         /* !< Set LIN Hardware Counter Overflow
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_LINOVF_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_LINOVF_SET      ((uint32_t)0x00000200U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[RXINT] Bits */
#define UNICOMMUART_CPU_INT_ISET_RXINT_OFS       (10)                            /* !< RXINT Offset */
#define UNICOMMUART_CPU_INT_ISET_RXINT_MASK      ((uint32_t)0x00000400U)         /* !< Set UART Receive Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_RXINT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_RXINT_SET       ((uint32_t)0x00000400U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[TXINT] Bits */
#define UNICOMMUART_CPU_INT_ISET_TXINT_OFS       (11)                            /* !< TXINT Offset */
#define UNICOMMUART_CPU_INT_ISET_TXINT_MASK      ((uint32_t)0x00000800U)         /* !< Set UART Transmit Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_TXINT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_TXINT_SET       ((uint32_t)0x00000800U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[EOT] Bits */
#define UNICOMMUART_CPU_INT_ISET_EOT_OFS         (12)                            /* !< EOT Offset */
#define UNICOMMUART_CPU_INT_ISET_EOT_MASK        ((uint32_t)0x00001000U)         /* !< Set UART End of Transmission
                                                                                    Interrupt Indicates that the last bit
                                                                                    of all transmitted data and flags has
                                                                                    left the serializer and without any
                                                                                    further Data in the TX Fifo or
                                                                                    Buffer. */
#define UNICOMMUART_CPU_INT_ISET_EOT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_EOT_SET         ((uint32_t)0x00001000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[ADDR_MATCH] Bits */
#define UNICOMMUART_CPU_INT_ISET_ADDR_MATCH_OFS  (13)                            /* !< ADDR_MATCH Offset */
#define UNICOMMUART_CPU_INT_ISET_ADDR_MATCH_MASK ((uint32_t)0x00002000U)         /* !< Set Address Match Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_ADDR_MATCH_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_ADDR_MATCH_SET  ((uint32_t)0x00002000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[CTS] Bits */
#define UNICOMMUART_CPU_INT_ISET_CTS_OFS         (14)                            /* !< CTS Offset */
#define UNICOMMUART_CPU_INT_ISET_CTS_MASK        ((uint32_t)0x00004000U)         /* !< Set UART Clear to Send Modem
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_CTS_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_CTS_SET         ((uint32_t)0x00004000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[DMA_DONE_RX] Bits */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_RX_OFS (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_RX_MASK ((uint32_t)0x00008000U)         /* !< Set DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_RX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_RX_SET ((uint32_t)0x00008000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[DMA_DONE_TX] Bits */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_TX_OFS (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_TX_MASK ((uint32_t)0x00010000U)         /* !< Set DMA Done on TX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_TX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_ISET_DMA_DONE_TX_SET ((uint32_t)0x00010000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[RTOUT] Bits */
#define UNICOMMUART_CPU_INT_ISET_RTOUT_OFS       (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_CPU_INT_ISET_RTOUT_MASK      ((uint32_t)0x00000001U)         /* !< Set UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_RTOUT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_RTOUT_SET       ((uint32_t)0x00000001U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[NERR] Bits */
#define UNICOMMUART_CPU_INT_ISET_NERR_OFS        (17)                            /* !< NERR Offset */
#define UNICOMMUART_CPU_INT_ISET_NERR_MASK       ((uint32_t)0x00020000U)         /* !< Noise Error on triple voting.
                                                                                    Asserted when the 3 samples of
                                                                                    majority voting are not equal */
#define UNICOMMUART_CPU_INT_ISET_NERR_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Writing this has no effect */
#define UNICOMMUART_CPU_INT_ISET_NERR_SET        ((uint32_t)0x00020000U)         /* !< Set the interrupt */
/* UNICOMMUART_CPU_INT_ISET[DMA_PREIRQ_RX] Bits */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_RX_OFS (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_RX_MASK ((uint32_t)0x00040000U)         /* !< Set DMA PREIRQ Done on RX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_RX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_RX_SET ((uint32_t)0x00040000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[DMA_PREIRQ_TX] Bits */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_TX_OFS (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_TX_MASK ((uint32_t)0x00080000U)         /* !< Set DMA PREIRQ Done on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_TX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_CPU_INT_ISET_DMA_PREIRQ_TX_SET ((uint32_t)0x00080000U)         /* !< Set Interrupt */
/* UNICOMMUART_CPU_INT_ISET[LTOUT] Bits */
#define UNICOMMUART_CPU_INT_ISET_LTOUT_OFS       (20)                            /* !< LTOUT Offset */
#define UNICOMMUART_CPU_INT_ISET_LTOUT_MASK      ((uint32_t)0x00100000U)         /* !< Set UARTOUT Line Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_CPU_INT_ISET_LTOUT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_CPU_INT_ISET_LTOUT_SET       ((uint32_t)0x00100000U)         /* !< Set Interrupt */

/* UNICOMMUART_ICLR Bits */
/* UNICOMMUART_ICLR[FRMERR] Bits */
#define UNICOMMUART_ICLR_FRMERR_OFS              (1)                             /* !< FRMERR Offset */
#define UNICOMMUART_ICLR_FRMERR_MASK             ((uint32_t)0x00000002U)         /* !< Clear UART Framing Error Interrupt. */
#define UNICOMMUART_ICLR_FRMERR_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_FRMERR_CLR              ((uint32_t)0x00000002U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[PARERR] Bits */
#define UNICOMMUART_ICLR_PARERR_OFS              (2)                             /* !< PARERR Offset */
#define UNICOMMUART_ICLR_PARERR_MASK             ((uint32_t)0x00000004U)         /* !< Clear UART Parity Error Interrupt. */
#define UNICOMMUART_ICLR_PARERR_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_PARERR_CLR              ((uint32_t)0x00000004U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[BRKERR] Bits */
#define UNICOMMUART_ICLR_BRKERR_OFS              (3)                             /* !< BRKERR Offset */
#define UNICOMMUART_ICLR_BRKERR_MASK             ((uint32_t)0x00000008U)         /* !< Clear UART Break Error Interrupt. */
#define UNICOMMUART_ICLR_BRKERR_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_BRKERR_CLR              ((uint32_t)0x00000008U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[OVRERR] Bits */
#define UNICOMMUART_ICLR_OVRERR_OFS              (4)                             /* !< OVRERR Offset */
#define UNICOMMUART_ICLR_OVRERR_MASK             ((uint32_t)0x00000010U)         /* !< Clear UART Receive Overrun Error
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_OVRERR_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_OVRERR_CLR              ((uint32_t)0x00000010U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[RXNE] Bits */
#define UNICOMMUART_ICLR_RXNE_OFS                (5)                             /* !< RXNE Offset */
#define UNICOMMUART_ICLR_RXNE_MASK               ((uint32_t)0x00000020U)         /* !< Clear Negative Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_RXNE_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_RXNE_CLR                ((uint32_t)0x00000020U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[RXPE] Bits */
#define UNICOMMUART_ICLR_RXPE_OFS                (6)                             /* !< RXPE Offset */
#define UNICOMMUART_ICLR_RXPE_MASK               ((uint32_t)0x00000040U)         /* !< Clear Positive Edge on UARTxRXD
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_RXPE_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_RXPE_CLR                ((uint32_t)0x00000040U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[LINC0] Bits */
#define UNICOMMUART_ICLR_LINC0_OFS               (7)                             /* !< LINC0 Offset */
#define UNICOMMUART_ICLR_LINC0_MASK              ((uint32_t)0x00000080U)         /* !< Clear LIN Capture 0 / Match
                                                                                    Interrupt . */
#define UNICOMMUART_ICLR_LINC0_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_LINC0_CLR               ((uint32_t)0x00000080U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[LINC1] Bits */
#define UNICOMMUART_ICLR_LINC1_OFS               (8)                             /* !< LINC1 Offset */
#define UNICOMMUART_ICLR_LINC1_MASK              ((uint32_t)0x00000100U)         /* !< Clear LIN Capture 1 Interrupt. */
#define UNICOMMUART_ICLR_LINC1_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_LINC1_CLR               ((uint32_t)0x00000100U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[LINOVF] Bits */
#define UNICOMMUART_ICLR_LINOVF_OFS              (9)                             /* !< LINOVF Offset */
#define UNICOMMUART_ICLR_LINOVF_MASK             ((uint32_t)0x00000200U)         /* !< Clear LIN Hardware Counter Overflow
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_LINOVF_NO_EFFECT        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_LINOVF_CLR              ((uint32_t)0x00000200U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[RXINT] Bits */
#define UNICOMMUART_ICLR_RXINT_OFS               (10)                            /* !< RXINT Offset */
#define UNICOMMUART_ICLR_RXINT_MASK              ((uint32_t)0x00000400U)         /* !< Clear UART Receive Interrupt. */
#define UNICOMMUART_ICLR_RXINT_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_RXINT_CLR               ((uint32_t)0x00000400U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[TXINT] Bits */
#define UNICOMMUART_ICLR_TXINT_OFS               (11)                            /* !< TXINT Offset */
#define UNICOMMUART_ICLR_TXINT_MASK              ((uint32_t)0x00000800U)         /* !< Clear UART Transmit Interrupt. */
#define UNICOMMUART_ICLR_TXINT_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_TXINT_CLR               ((uint32_t)0x00000800U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[EOT] Bits */
#define UNICOMMUART_ICLR_EOT_OFS                 (12)                            /* !< EOT Offset */
#define UNICOMMUART_ICLR_EOT_MASK                ((uint32_t)0x00001000U)         /* !< Clear UART End of Transmission
                                                                                    Interrupt Indicates that the last bit
                                                                                    of all transmitted data and flags has
                                                                                    left the serializer and without any
                                                                                    further Data in the TX Fifo or
                                                                                    Buffer. */
#define UNICOMMUART_ICLR_EOT_NO_EFFECT           ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_EOT_CLR                 ((uint32_t)0x00001000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[ADDR_MATCH] Bits */
#define UNICOMMUART_ICLR_ADDR_MATCH_OFS          (13)                            /* !< ADDR_MATCH Offset */
#define UNICOMMUART_ICLR_ADDR_MATCH_MASK         ((uint32_t)0x00002000U)         /* !< Clear Address Match Interrupt. */
#define UNICOMMUART_ICLR_ADDR_MATCH_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_ADDR_MATCH_CLR          ((uint32_t)0x00002000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[CTS] Bits */
#define UNICOMMUART_ICLR_CTS_OFS                 (14)                            /* !< CTS Offset */
#define UNICOMMUART_ICLR_CTS_MASK                ((uint32_t)0x00004000U)         /* !< Clear UART Clear to Send Modem
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_CTS_NO_EFFECT           ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_CTS_CLR                 ((uint32_t)0x00004000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[DMA_DONE_RX] Bits */
#define UNICOMMUART_ICLR_DMA_DONE_RX_OFS         (15)                            /* !< DMA_DONE_RX Offset */
#define UNICOMMUART_ICLR_DMA_DONE_RX_MASK        ((uint32_t)0x00008000U)         /* !< Clear DMA Done on RX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_ICLR_DMA_DONE_RX_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_ICLR_DMA_DONE_RX_CLR         ((uint32_t)0x00008000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[DMA_DONE_TX] Bits */
#define UNICOMMUART_ICLR_DMA_DONE_TX_OFS         (16)                            /* !< DMA_DONE_TX Offset */
#define UNICOMMUART_ICLR_DMA_DONE_TX_MASK        ((uint32_t)0x00010000U)         /* !< Clear DMA Done on TX Event Channel
                                                                                    Interrupt */
#define UNICOMMUART_ICLR_DMA_DONE_TX_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_ICLR_DMA_DONE_TX_CLR         ((uint32_t)0x00010000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[RTOUT] Bits */
#define UNICOMMUART_ICLR_RTOUT_OFS               (0)                             /* !< RTOUT Offset */
#define UNICOMMUART_ICLR_RTOUT_MASK              ((uint32_t)0x00000001U)         /* !< Clear UARTOUT Receive Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_RTOUT_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_RTOUT_CLR               ((uint32_t)0x00000001U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[NERR] Bits */
#define UNICOMMUART_ICLR_NERR_OFS                (17)                            /* !< NERR Offset */
#define UNICOMMUART_ICLR_NERR_MASK               ((uint32_t)0x00020000U)         /* !< Noise Error on triple voting.
                                                                                    Asserted when the 3 samples of
                                                                                    majority voting are not equal */
#define UNICOMMUART_ICLR_NERR_NO_EFFECT          ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_NERR_CLR                ((uint32_t)0x00020000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[DMA_PREIRQ_RX] Bits */
#define UNICOMMUART_ICLR_DMA_PREIRQ_RX_OFS       (18)                            /* !< DMA_PREIRQ_RX Offset */
#define UNICOMMUART_ICLR_DMA_PREIRQ_RX_MASK      ((uint32_t)0x00040000U)         /* !< Clear DMA PREIRQ Done on RX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_ICLR_DMA_PREIRQ_RX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_ICLR_DMA_PREIRQ_RX_CLR       ((uint32_t)0x00040000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[DMA_PREIRQ_TX] Bits */
#define UNICOMMUART_ICLR_DMA_PREIRQ_TX_OFS       (19)                            /* !< DMA_PREIRQ_TX Offset */
#define UNICOMMUART_ICLR_DMA_PREIRQ_TX_MASK      ((uint32_t)0x00080000U)         /* !< Clear DMA PREIRQ Done on TX Event
                                                                                    Channel Interrupt */
#define UNICOMMUART_ICLR_DMA_PREIRQ_TX_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define UNICOMMUART_ICLR_DMA_PREIRQ_TX_CLR       ((uint32_t)0x00080000U)         /* !< Clear Interrupt */
/* UNICOMMUART_ICLR[LTOUT] Bits */
#define UNICOMMUART_ICLR_LTOUT_OFS               (20)                            /* !< LTOUT Offset */
#define UNICOMMUART_ICLR_LTOUT_MASK              ((uint32_t)0x00100000U)         /* !< Clear UARTOUT Line Time-Out
                                                                                    Interrupt. */
#define UNICOMMUART_ICLR_LTOUT_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define UNICOMMUART_ICLR_LTOUT_CLR               ((uint32_t)0x00100000U)         /* !< Clear Interrupt */

/* UNICOMMUART_CLKDIV Bits */
/* UNICOMMUART_CLKDIV[RATIO] Bits */
#define UNICOMMUART_CLKDIV_RATIO_OFS             (0)                             /* !< RATIO Offset */
#define UNICOMMUART_CLKDIV_RATIO_MASK            ((uint32_t)0x0000003FU)         /* !< Selects divide ratio of module
                                                                                    clock  Division factor  0 : DIV_BY_1
                                                                                    1 : DIV_BY_2 .... 63: DIV_BY_64 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_1        ((uint32_t)0x00000000U)         /* !< Do not divide clock source */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_2        ((uint32_t)0x00000001U)         /* !< Divide clock source by 2 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_3        ((uint32_t)0x00000002U)         /* !< Divide clock source by 3 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_4        ((uint32_t)0x00000003U)         /* !< Divide clock source by 4 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_5        ((uint32_t)0x00000004U)         /* !< Divide clock source by 5 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_6        ((uint32_t)0x00000005U)         /* !< Divide clock source by 6 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_7        ((uint32_t)0x00000006U)         /* !< Divide clock source by 7 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_8        ((uint32_t)0x00000007U)         /* !< Divide clock source by 8 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_61       ((uint32_t)0x0000003CU)         /* !< Divide clock source by 8 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_62       ((uint32_t)0x0000003DU)         /* !< Divide clock source by 8 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_63       ((uint32_t)0x0000003EU)         /* !< Divide clock source by 8 */
#define UNICOMMUART_CLKDIV_RATIO_DIV_BY_64       ((uint32_t)0x0000003FU)         /* !< Divide clock source by 8 */

/* UNICOMMUART_CLKSEL Bits */
/* UNICOMMUART_CLKSEL[MFCLK_SEL] Bits */
#define UNICOMMUART_CLKSEL_MFCLK_SEL_OFS         (2)                             /* !< MFCLK_SEL Offset */
#define UNICOMMUART_CLKSEL_MFCLK_SEL_MASK        ((uint32_t)0x00000004U)         /* !< Selects MFCLK as clock source if
                                                                                    enabled */
#define UNICOMMUART_CLKSEL_MFCLK_SEL_DISABLE     ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_MFCLK_SEL_ENABLE      ((uint32_t)0x00000004U)         /* !< Select this clock as a source */
/* UNICOMMUART_CLKSEL[BUSCLK_SEL] Bits */
#define UNICOMMUART_CLKSEL_BUSCLK_SEL_OFS        (3)                             /* !< BUSCLK_SEL Offset */
#define UNICOMMUART_CLKSEL_BUSCLK_SEL_MASK       ((uint32_t)0x00000008U)         /* !< Selects BUS CLK as clock source if
                                                                                    enabled */
#define UNICOMMUART_CLKSEL_BUSCLK_SEL_DISABLE    ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_BUSCLK_SEL_ENABLE     ((uint32_t)0x00000008U)         /* !< Select this clock as a source */
/* UNICOMMUART_CLKSEL[LFCLK_SEL] Bits */
#define UNICOMMUART_CLKSEL_LFCLK_SEL_OFS         (1)                             /* !< LFCLK_SEL Offset */
#define UNICOMMUART_CLKSEL_LFCLK_SEL_MASK        ((uint32_t)0x00000002U)         /* !< Selects LFCLK as clock source if
                                                                                    enabled */
#define UNICOMMUART_CLKSEL_LFCLK_SEL_DISABLE     ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_LFCLK_SEL_ENABLE      ((uint32_t)0x00000002U)         /* !< Select this clock as a source */
/* UNICOMMUART_CLKSEL[ASYNC_LFCLK_SEL] Bits */
#define UNICOMMUART_CLKSEL_ASYNC_LFCLK_SEL_OFS   (8)                             /* !< ASYNC_LFCLK_SEL Offset */
#define UNICOMMUART_CLKSEL_ASYNC_LFCLK_SEL_MASK  ((uint32_t)0x00000100U)         /* !< Asynchronous lfclk selected as
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_LFCLK_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_LFCLK_SEL_ENABLE ((uint32_t)0x00000100U)         /* !< Select this clock as a source */
/* UNICOMMUART_CLKSEL[ASYNC_SYSCLK_SEL] Bits */
#define UNICOMMUART_CLKSEL_ASYNC_SYSCLK_SEL_OFS  (9)                             /* !< ASYNC_SYSCLK_SEL Offset */
#define UNICOMMUART_CLKSEL_ASYNC_SYSCLK_SEL_MASK ((uint32_t)0x00000200U)         /* !< Asynchronous sysclk selected as
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_SYSCLK_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_SYSCLK_SEL_ENABLE ((uint32_t)0x00000200U)         /* !< Select this clock as a source */
/* UNICOMMUART_CLKSEL[ASYNC_HFCLK_SEL] Bits */
#define UNICOMMUART_CLKSEL_ASYNC_HFCLK_SEL_OFS   (10)                            /* !< ASYNC_HFCLK_SEL Offset */
#define UNICOMMUART_CLKSEL_ASYNC_HFCLK_SEL_MASK  ((uint32_t)0x00000400U)         /* !< Asynchronous HFCLK selected as
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_HFCLK_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_HFCLK_SEL_ENABLE ((uint32_t)0x00000400U)         /* !< Select this clock as a source */
/* UNICOMMUART_CLKSEL[ASYNC_PLL_SEL] Bits */
#define UNICOMMUART_CLKSEL_ASYNC_PLL_SEL_OFS     (11)                            /* !< ASYNC_PLL_SEL Offset */
#define UNICOMMUART_CLKSEL_ASYNC_PLL_SEL_MASK    ((uint32_t)0x00000800U)         /* !< Asynchronous PLL selected as source */
#define UNICOMMUART_CLKSEL_ASYNC_PLL_SEL_DISABLE ((uint32_t)0x00000000U)         /* !< Does not select this clock as a
                                                                                    source */
#define UNICOMMUART_CLKSEL_ASYNC_PLL_SEL_ENABLE  ((uint32_t)0x00000800U)         /* !< Select this clock as a source */

/* UNICOMMUART_PDBGCTL Bits */
/* UNICOMMUART_PDBGCTL[FREE] Bits */
#define UNICOMMUART_PDBGCTL_FREE_OFS             (0)                             /* !< FREE Offset */
#define UNICOMMUART_PDBGCTL_FREE_MASK            ((uint32_t)0x00000001U)         /* !< Free run control */
#define UNICOMMUART_PDBGCTL_FREE_STOP            ((uint32_t)0x00000000U)         /* !< The peripheral freezes
                                                                                    functionality while the Core Halted
                                                                                    input is asserted and resumes when it
                                                                                    is deasserted. */
#define UNICOMMUART_PDBGCTL_FREE_RUN             ((uint32_t)0x00000001U)         /* !< The peripheral ignores the state of
                                                                                    the Core Halted input */
/* UNICOMMUART_PDBGCTL[SOFT] Bits */
#define UNICOMMUART_PDBGCTL_SOFT_OFS             (1)                             /* !< SOFT Offset */
#define UNICOMMUART_PDBGCTL_SOFT_MASK            ((uint32_t)0x00000002U)         /* !< Soft halt boundary control. This
                                                                                    function is only available, if [FREE]
                                                                                    is set to 'STOP' */
#define UNICOMMUART_PDBGCTL_SOFT_DEPRECATED      ((uint32_t)0x00000000U)         /* !< Not supported */
#define UNICOMMUART_PDBGCTL_SOFT_DELAYED         ((uint32_t)0x00000002U)         /* !< The peripheral blocks the debug
                                                                                    freeze until it has reached a
                                                                                    boundary where it can resume without
                                                                                    corruption */

/* UNICOMMUART_INTCTL Bits */
/* UNICOMMUART_INTCTL[INTEVAL] Bits */
#define UNICOMMUART_INTCTL_INTEVAL_OFS           (0)                             /* !< INTEVAL Offset */
#define UNICOMMUART_INTCTL_INTEVAL_MASK          ((uint32_t)0x00000001U)         /* !< Writing a 1 to this field
                                                                                    re-evaluates the interrupt sources. */
#define UNICOMMUART_INTCTL_INTEVAL_DISABLE       ((uint32_t)0x00000000U)         /* !< The interrupt or event line is
                                                                                    disabled. */
#define UNICOMMUART_INTCTL_INTEVAL_EVAL          ((uint32_t)0x00000001U)         /* !< The interrupt or event line is in
                                                                                    software mode. Software must clear
                                                                                    the RIS. */

/* UNICOMMUART_CTL0 Bits */
/* UNICOMMUART_CTL0[ENABLE] Bits */
#define UNICOMMUART_CTL0_ENABLE_OFS              (0)                             /* !< ENABLE Offset */
#define UNICOMMUART_CTL0_ENABLE_MASK             ((uint32_t)0x00000001U)         /* !< UART Module Enable.  If the UART is
                                                                                    disabled in the middle of
                                                                                    transmission or reception, it
                                                                                    completes the current character
                                                                                    before stopping.  If the ENABLE bit
                                                                                    is not set, all registers can still
                                                                                    be accessed and updated. It is
                                                                                    recommended to setup and change the
                                                                                    UART operation mode with having the
                                                                                    ENABLE bit cleared to avoid
                                                                                    unpredictable behavior during the
                                                                                    setup or update. If disabled the UART
                                                                                    module will not send or receive any
                                                                                    data and the logic is held in reset
                                                                                    state. */
#define UNICOMMUART_CTL0_ENABLE_DISABLE          ((uint32_t)0x00000000U)         /* !< Disable Module */
#define UNICOMMUART_CTL0_ENABLE_ENABLE           ((uint32_t)0x00000001U)         /* !< Enable module */
/* UNICOMMUART_CTL0[HSE] Bits */
#define UNICOMMUART_CTL0_HSE_OFS                 (15)                            /* !< HSE Offset */
#define UNICOMMUART_CTL0_HSE_MASK                ((uint32_t)0x00018000U)         /* !< High-Speed Bit Oversampling Enable
                                                                                    #b#NOTE:#/b# The bit oversampling
                                                                                    influences the UART baud-rate
                                                                                    configuration.  The state of this bit
                                                                                    has no effect on clock generation in
                                                                                    ISO7816 smart card mode (the SMART
                                                                                    bit is set). */
#define UNICOMMUART_CTL0_HSE_OVS16               ((uint32_t)0x00000000U)         /* !< 16x oversampling. */
#define UNICOMMUART_CTL0_HSE_OVS8                ((uint32_t)0x00008000U)         /* !< 8x oversampling. */
#define UNICOMMUART_CTL0_HSE_OVS3                ((uint32_t)0x00010000U)         /* !< 3x oversampling. IrDA, Manchester
                                                                                    and DALI not supported when 3x
                                                                                    oversampling is enabled. */
/* UNICOMMUART_CTL0[LBE] Bits */
#define UNICOMMUART_CTL0_LBE_OFS                 (2)                             /* !< LBE Offset */
#define UNICOMMUART_CTL0_LBE_MASK                ((uint32_t)0x00000004U)         /* !< UART Loop Back Enable */
#define UNICOMMUART_CTL0_LBE_DISABLE             ((uint32_t)0x00000000U)         /* !< Normal operation. */
#define UNICOMMUART_CTL0_LBE_ENABLE              ((uint32_t)0x00000004U)         /* !< The UARTxTX path is fed through the
                                                                                    UARTxRX path internally. */
/* UNICOMMUART_CTL0[RXE] Bits */
#define UNICOMMUART_CTL0_RXE_OFS                 (3)                             /* !< RXE Offset */
#define UNICOMMUART_CTL0_RXE_MASK                ((uint32_t)0x00000008U)         /* !< UART Receive Enable  If the UART is
                                                                                    disabled in the middle of a receive,
                                                                                    it completes the current character
                                                                                    before stopping.   #b#NOTE:#/b# To
                                                                                    enable reception, the UARTEN bit must
                                                                                    be set. */
#define UNICOMMUART_CTL0_RXE_DISABLE             ((uint32_t)0x00000000U)         /* !< The receive section of the UART is
                                                                                    disabled. */
#define UNICOMMUART_CTL0_RXE_ENABLE              ((uint32_t)0x00000008U)         /* !< The receive section of the UART is
                                                                                    enabled. */
/* UNICOMMUART_CTL0[TXE] Bits */
#define UNICOMMUART_CTL0_TXE_OFS                 (4)                             /* !< TXE Offset */
#define UNICOMMUART_CTL0_TXE_MASK                ((uint32_t)0x00000010U)         /* !< UART Transmit Enable  If the UART
                                                                                    is disabled in the middle of a
                                                                                    transmission, it completes the
                                                                                    current character before stopping.
                                                                                    #b#NOTE:#/b# To enable transmission,
                                                                                    the UARTEN bit must be set. */
#define UNICOMMUART_CTL0_TXE_DISABLE             ((uint32_t)0x00000000U)         /* !< The transmit section of the UART is
                                                                                    disabled. The UARTxTXD pin of the
                                                                                    UART can be controlled by the TXD_CTL
                                                                                    bit when enabled. */
#define UNICOMMUART_CTL0_TXE_ENABLE              ((uint32_t)0x00000010U)         /* !< The transmit section of the UART is
                                                                                    enabled. */
/* UNICOMMUART_CTL0[RTS] Bits */
#define UNICOMMUART_CTL0_RTS_OFS                 (12)                            /* !< RTS Offset */
#define UNICOMMUART_CTL0_RTS_MASK                ((uint32_t)0x00001000U)         /* !< Request to Send    If RTSEN is set
                                                                                    the RTS output signals is controlled
                                                                                    by the hardware logic using the FIFO
                                                                                    fill level or buffer status.  If
                                                                                    RTSEN is cleared the RTS output is
                                                                                    controlled by the RTS bit. The bit is
                                                                                    the complement of the UART request to
                                                                                    send, RTS modem status output. */
#define UNICOMMUART_CTL0_RTS_CLR                 ((uint32_t)0x00000000U)         /* !< Signal not RTS */
#define UNICOMMUART_CTL0_RTS_SET                 ((uint32_t)0x00001000U)         /* !< Signal RTS */
/* UNICOMMUART_CTL0[RTSEN] Bits */
#define UNICOMMUART_CTL0_RTSEN_OFS               (13)                            /* !< RTSEN Offset */
#define UNICOMMUART_CTL0_RTSEN_MASK              ((uint32_t)0x00002000U)         /* !< Enable hardware controlled Request
                                                                                    to Send */
#define UNICOMMUART_CTL0_RTSEN_DISABLE           ((uint32_t)0x00000000U)         /* !< RTS hardware flow control is
                                                                                    disabled. */
#define UNICOMMUART_CTL0_RTSEN_ENABLE            ((uint32_t)0x00002000U)         /* !< RTS hardware flow control is
                                                                                    enabled. Data is only requested (by
                                                                                    asserting UARTxRTS) when the receive
                                                                                    FIFO/buffer has available entries. */
/* UNICOMMUART_CTL0[CTSEN] Bits */
#define UNICOMMUART_CTL0_CTSEN_OFS               (14)                            /* !< CTSEN Offset */
#define UNICOMMUART_CTL0_CTSEN_MASK              ((uint32_t)0x00004000U)         /* !< Enable Clear To Send */
#define UNICOMMUART_CTL0_CTSEN_DISABLE           ((uint32_t)0x00000000U)         /* !< CTS hardware flow control is
                                                                                    disabled. */
#define UNICOMMUART_CTL0_CTSEN_ENABLE            ((uint32_t)0x00004000U)         /* !< CTS hardware flow control is
                                                                                    enabled. Data is only transmitted
                                                                                    when the UARTxCTS signal is asserted. */
/* UNICOMMUART_CTL0[MENC] Bits */
#define UNICOMMUART_CTL0_MENC_OFS                (7)                             /* !< MENC Offset */
#define UNICOMMUART_CTL0_MENC_MASK               ((uint32_t)0x00000080U)         /* !< Manchester Encode enable */
#define UNICOMMUART_CTL0_MENC_DISABLE            ((uint32_t)0x00000000U)         /* !< Disable Manchester Encoding */
#define UNICOMMUART_CTL0_MENC_ENABLE             ((uint32_t)0x00000080U)         /* !< Enable Manchester Encoding */
/* UNICOMMUART_CTL0[MODE] Bits */
#define UNICOMMUART_CTL0_MODE_OFS                (8)                             /* !< MODE Offset */
#define UNICOMMUART_CTL0_MODE_MASK               ((uint32_t)0x00000700U)         /* !< Set the communication mode and
                                                                                    protocol used. (Not defined settings
                                                                                    uses the default setting: 0) */
#define UNICOMMUART_CTL0_MODE_UART               ((uint32_t)0x00000000U)         /* !< Normal operation */
#define UNICOMMUART_CTL0_MODE_RS485              ((uint32_t)0x00000100U)         /* !< RS485 mode: UART needs to be IDLE
                                                                                    with receiving data for the in
                                                                                    EXTDIR_HOLD set time. EXTDIR_SETUP
                                                                                    defines the time the RTS line is set
                                                                                    to high before sending. When the
                                                                                    buffer is empty the RTS line is set
                                                                                    low again. A transmit will be delayed
                                                                                    as long the UART is receiving data. */
#define UNICOMMUART_CTL0_MODE_IDLELINE           ((uint32_t)0x00000200U)         /* !< The UART operates in IDLE Line Mode */
#define UNICOMMUART_CTL0_MODE_ADDR9BIT           ((uint32_t)0x00000300U)         /* !< The UART operates in 9 Bit Address
                                                                                    mode */
#define UNICOMMUART_CTL0_MODE_SMART              ((uint32_t)0x00000400U)         /* !< ISO7816 Smart Card Support  The
                                                                                    application must ensure that it sets
                                                                                    8-bit word length (WLEN set to 3h)
                                                                                    and even parity (PEN set to 1, EPS
                                                                                    set to 1, SPS set to 0) in UARTLCRH
                                                                                    when using ISO7816 mode. The value of
                                                                                    the STP2 bit in UARTLCRH is ignored
                                                                                    and the number of stop bits is forced
                                                                                    to 2. */
#define UNICOMMUART_CTL0_MODE_DALI               ((uint32_t)0x00000500U)         /* !< DALI Mode: */
/* UNICOMMUART_CTL0[TXD_OUT] Bits */
#define UNICOMMUART_CTL0_TXD_OUT_OFS             (6)                             /* !< TXD_OUT Offset */
#define UNICOMMUART_CTL0_TXD_OUT_MASK            ((uint32_t)0x00000040U)         /* !< TXD Pin Control Controls the TXD
                                                                                    pin when TXD_OUT_EN = 1 and TXE = 0. */
#define UNICOMMUART_CTL0_TXD_OUT_LOW             ((uint32_t)0x00000000U)         /* !< TXD pin is low */
#define UNICOMMUART_CTL0_TXD_OUT_HIGH            ((uint32_t)0x00000040U)         /* !< TXD pin is high */
/* UNICOMMUART_CTL0[TXD_OUT_EN] Bits */
#define UNICOMMUART_CTL0_TXD_OUT_EN_OFS          (5)                             /* !< TXD_OUT_EN Offset */
#define UNICOMMUART_CTL0_TXD_OUT_EN_MASK         ((uint32_t)0x00000020U)         /* !< TXD Pin Control Enable. When the
                                                                                    transmit section of the UART is
                                                                                    disabled (TXE = 0), the TXD pin can
                                                                                    be controlled by the TXD_OUT bit. */
#define UNICOMMUART_CTL0_TXD_OUT_EN_DISABLE      ((uint32_t)0x00000000U)         /* !< TXD pin can not be controlled by
                                                                                    TXD_OUT */
#define UNICOMMUART_CTL0_TXD_OUT_EN_ENABLE       ((uint32_t)0x00000020U)         /* !< TXD pin can be controlled by
                                                                                    TXD_OUT */
/* UNICOMMUART_CTL0[MAJVOTE] Bits */
#define UNICOMMUART_CTL0_MAJVOTE_OFS             (18)                            /* !< MAJVOTE Offset */
#define UNICOMMUART_CTL0_MAJVOTE_MASK            ((uint32_t)0x00040000U)         /* !< Majority Vote Enable   When
                                                                                    Majority Voting is enabled, the three
                                                                                    center bits are used to determine
                                                                                    received sample value. In case of
                                                                                    error (i.e. all 3 bits are not the
                                                                                    same), noise error is detected and
                                                                                    bits RIS.NERR and register
                                                                                    RXDATA.NERR are set.
                                                                                    Oversampling of 16 : bits 7, 8, 9 are
                                                                                    used      Oversampling of 8 : bits 3,
                                                                                    4, 5 are used Disabled : Single
                                                                                    sample value (center value) used */
#define UNICOMMUART_CTL0_MAJVOTE_DISABLE         ((uint32_t)0x00000000U)         /* !< Majority voting is disabled */
#define UNICOMMUART_CTL0_MAJVOTE_ENABLE          ((uint32_t)0x00040000U)         /* !< Majority voting is enabled */
/* UNICOMMUART_CTL0[MSBFIRST] Bits */
#define UNICOMMUART_CTL0_MSBFIRST_OFS            (19)                            /* !< MSBFIRST Offset */
#define UNICOMMUART_CTL0_MSBFIRST_MASK           ((uint32_t)0x00080000U)         /* !< Most Significant Bit First This bit
                                                                                    has effect both on the way protocol
                                                                                    byte is transmitted and received.
                                                                                    Notes: User needs to match the
                                                                                    protocol to the correct value of this
                                                                                    bit to send MSb or LSb first. The
                                                                                    hardware engine will send the byte
                                                                                    entirely based on this bit. */
#define UNICOMMUART_CTL0_MSBFIRST_DISABLE        ((uint32_t)0x00000000U)         /* !< Least significant bit is sent first
                                                                                    in the protocol packet */
#define UNICOMMUART_CTL0_MSBFIRST_ENABLE         ((uint32_t)0x00080000U)         /* !< Most significant bit is sent first
                                                                                    in the protocol packet */

/* UNICOMMUART_LCRH Bits */
/* UNICOMMUART_LCRH[BRK] Bits */
#define UNICOMMUART_LCRH_BRK_OFS                 (0)                             /* !< BRK Offset */
#define UNICOMMUART_LCRH_BRK_MASK                ((uint32_t)0x00000001U)         /* !< UART Send Break (for LIN Protocol)
                                                                                    1. Break condition is sent on the
                                                                                    line for as long as this bit is set */
#define UNICOMMUART_LCRH_BRK_DISABLE             ((uint32_t)0x00000000U)         /* !< Normal operation */
#define UNICOMMUART_LCRH_BRK_ENABLE              ((uint32_t)0x00000001U)         /* !< A low level is continually output
                                                                                    on the UARTxTXD signal, after
                                                                                    completing transmission of the
                                                                                    current character. For the proper
                                                                                    execution of the break command,
                                                                                    software must set this bit for at
                                                                                    least two frames (character periods). */
/* UNICOMMUART_LCRH[PEN] Bits */
#define UNICOMMUART_LCRH_PEN_OFS                 (1)                             /* !< PEN Offset */
#define UNICOMMUART_LCRH_PEN_MASK                ((uint32_t)0x00000002U)         /* !< UART Parity Enable */
#define UNICOMMUART_LCRH_PEN_DISABLE             ((uint32_t)0x00000000U)         /* !< Parity is disabled and no parity
                                                                                    bit is added to the data frame. */
#define UNICOMMUART_LCRH_PEN_ENABLE              ((uint32_t)0x00000002U)         /* !< Parity checking and generation is
                                                                                    enabled. */
/* UNICOMMUART_LCRH[EPS] Bits */
#define UNICOMMUART_LCRH_EPS_OFS                 (2)                             /* !< EPS Offset */
#define UNICOMMUART_LCRH_EPS_MASK                ((uint32_t)0x00000004U)         /* !< UART Even Parity Select  This bit
                                                                                    has no effect when parity is disabled
                                                                                    by the PEN bit.  For 9-Bit UART Mode
                                                                                    transmissions, this bit controls the
                                                                                    address byte and data byte indication
                                                                                    (9th bit). 0 = The transferred byte
                                                                                    is a data byte 1 = The transferred
                                                                                    byte is an address byte */
#define UNICOMMUART_LCRH_EPS_ODD                 ((uint32_t)0x00000000U)         /* !< Odd parity is performed, which
                                                                                    checks for an odd number of 1s. */
#define UNICOMMUART_LCRH_EPS_EVEN                ((uint32_t)0x00000004U)         /* !< Even parity generation and checking
                                                                                    is performed during transmission and
                                                                                    reception, which checks for an even
                                                                                    number of 1s in data and parity bits. */
/* UNICOMMUART_LCRH[STP2] Bits */
#define UNICOMMUART_LCRH_STP2_OFS                (3)                             /* !< STP2 Offset */
#define UNICOMMUART_LCRH_STP2_MASK               ((uint32_t)0x00000008U)         /* !< UART Two Stop Bits Select     When
                                                                                    in 7816 smart card mode (the SMART
                                                                                    bit is set in the UARTCTL register),
                                                                                    the number of stop bits is forced to
                                                                                    2. */
#define UNICOMMUART_LCRH_STP2_DISABLE            ((uint32_t)0x00000000U)         /* !< One stop bit is transmitted at the
                                                                                    end of a frame. */
#define UNICOMMUART_LCRH_STP2_ENABLE             ((uint32_t)0x00000008U)         /* !< Two stop bits are transmitted at
                                                                                    the end of a frame. The receive logic
                                                                                    checks for two stop bits being
                                                                                    received and provide Frame Error if
                                                                                    either is invalid. */
/* UNICOMMUART_LCRH[WLEN] Bits */
#define UNICOMMUART_LCRH_WLEN_OFS                (4)                             /* !< WLEN Offset */
#define UNICOMMUART_LCRH_WLEN_MASK               ((uint32_t)0x00000030U)         /* !< UART Word Length  The bits indicate
                                                                                    the number of data bits transmitted
                                                                                    or received in a frame as follows: */
#define UNICOMMUART_LCRH_WLEN_DATABIT5           ((uint32_t)0x00000000U)         /* !< 5 bits (default) */
#define UNICOMMUART_LCRH_WLEN_DATABIT6           ((uint32_t)0x00000010U)         /* !< 6 bits */
#define UNICOMMUART_LCRH_WLEN_DATABIT7           ((uint32_t)0x00000020U)         /* !< 7 bits */
#define UNICOMMUART_LCRH_WLEN_DATABIT8           ((uint32_t)0x00000030U)         /* !< 8 bits */
/* UNICOMMUART_LCRH[SPS] Bits */
#define UNICOMMUART_LCRH_SPS_OFS                 (6)                             /* !< SPS Offset */
#define UNICOMMUART_LCRH_SPS_MASK                ((uint32_t)0x00000040U)         /* !< UART Stick Parity Select The Stick
                                                                                    Parity Select (SPS) bit is used to
                                                                                    set either a permanent '1' or a
                                                                                    permanent '0' as parity when
                                                                                    transmitting or receiving data. Its
                                                                                    purpose is to typically indicate the
                                                                                    first byte of a package or to mark an
                                                                                    address byte, for example in a
                                                                                    multi-drop RS-485 network.  When bits
                                                                                    PEN, EPS, and SPS of UARTLCRH are
                                                                                    set, the parity bit is transmitted
                                                                                    and checked as a 0.  When bits PEN
                                                                                    and SPS are set and EPS is cleared,
                                                                                    the parity bit is transmitted and
                                                                                    checked as a 1. */
#define UNICOMMUART_LCRH_SPS_DISABLE             ((uint32_t)0x00000000U)         /* !< Disable Stick Parity */
#define UNICOMMUART_LCRH_SPS_ENABLE              ((uint32_t)0x00000040U)         /* !< Enable Stick Parity */
/* UNICOMMUART_LCRH[SENDIDLE] Bits */
#define UNICOMMUART_LCRH_SENDIDLE_OFS            (7)                             /* !< SENDIDLE Offset */
#define UNICOMMUART_LCRH_SENDIDLE_MASK           ((uint32_t)0x00000080U)         /* !< UART send IDLE pattern. When this
                                                                                    bit is set,  SENDIDLE period of 11
                                                                                    bit times will be sent on the TX
                                                                                    line.   Read STAT.SENDIDLE bit to
                                                                                    readback current status of SENDIDLE */
#define UNICOMMUART_LCRH_SENDIDLE_DISABLE        ((uint32_t)0x00000000U)         /* !< Normal operation */
#define UNICOMMUART_LCRH_SENDIDLE_ENABLE         ((uint32_t)0x00000080U)         /* !< Send Idle Pattern */
/* UNICOMMUART_LCRH[EXTDIR_SETUP] Bits */
#define UNICOMMUART_LCRH_EXTDIR_SETUP_OFS        (16)                            /* !< EXTDIR_SETUP Offset */
#define UNICOMMUART_LCRH_EXTDIR_SETUP_MASK       ((uint32_t)0x001F0000U)         /* !< Defines the number of UARTclk ticks
                                                                                    the signal to control the external
                                                                                    driver for the RS485 will be set
                                                                                    before the START bit is send */
#define UNICOMMUART_LCRH_EXTDIR_SETUP_MINIMUM    ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_LCRH_EXTDIR_SETUP_MAXIMUM    ((uint32_t)0x001F0000U)         /* !< Highest possible value */
/* UNICOMMUART_LCRH[EXTDIR_HOLD] Bits */
#define UNICOMMUART_LCRH_EXTDIR_HOLD_OFS         (21)                            /* !< EXTDIR_HOLD Offset */
#define UNICOMMUART_LCRH_EXTDIR_HOLD_MASK        ((uint32_t)0x03E00000U)         /* !< Defines the number of UARTclk ticks
                                                                                    the signal to control the external
                                                                                    driver for the RS485 will be reset
                                                                                    after the beginning of the stop bit.
                                                                                    (If 2 STOP bits are enabled the
                                                                                    beginning of the 2nd STOP bit.) */
#define UNICOMMUART_LCRH_EXTDIR_HOLD_MINIMUM     ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_LCRH_EXTDIR_HOLD_MAXIMUM     ((uint32_t)0x03E00000U)         /* !< Highest possible value */
/* UNICOMMUART_LCRH[SUSPEND] Bits */
#define UNICOMMUART_LCRH_SUSPEND_OFS             (26)                            /* !< SUSPEND Offset */
#define UNICOMMUART_LCRH_SUSPEND_MASK            ((uint32_t)0x04000000U)         /* !< Suspend external communication */
#define UNICOMMUART_LCRH_SUSPEND_DISABLE         ((uint32_t)0x00000000U)         /* !< Functional mode resumed */
#define UNICOMMUART_LCRH_SUSPEND_ENABLE          ((uint32_t)0x04000000U)         /* !< External communication suspended */

/* UNICOMMUART_STAT Bits */
/* UNICOMMUART_STAT[BUSY] Bits */
#define UNICOMMUART_STAT_BUSY_OFS                (0)                             /* !< BUSY Offset */
#define UNICOMMUART_STAT_BUSY_MASK               ((uint32_t)0x00000001U)         /* !< UART Busy 			This bit is set as
                                                                                    soon as the transmit FIFO/buffer
                                                                                    becomes non-empty (regardless of
                                                                                    whether UART is enabled) or if a
                                                                                    receive data is currently ongoing
                                                                                    (after the start edge have been
                                                                                    detected until a complete byte,
                                                                                    including all stop bits, has been
                                                                                    received by the shift register). In
                                                                                    IDLE_Line mode the Busy signal also
                                                                                    stays set during the idle time
                                                                                    generation. */
#define UNICOMMUART_STAT_BUSY_CLEARED            ((uint32_t)0x00000000U)         /* !< The UART is not busy. */
#define UNICOMMUART_STAT_BUSY_SET                ((uint32_t)0x00000001U)         /* !< The UART is busy transmitting data.
                                                                                    This bit remains set until the
                                                                                    complete byte, including all stop
                                                                                    bits, has been sent/received
                                                                                    from/into the shift register. */
/* UNICOMMUART_STAT[TXFF] Bits */
#define UNICOMMUART_STAT_TXFF_OFS                (6)                             /* !< TXFF Offset */
#define UNICOMMUART_STAT_TXFF_MASK               ((uint32_t)0x00000040U)         /* !< UART Transmit FIFO Full  The
                                                                                    meaning of this bit depends on the
                                                                                    state of the FEN bit in the CTL0
                                                                                    register. */
#define UNICOMMUART_STAT_TXFF_CLEARED            ((uint32_t)0x00000000U)         /* !< The transmitter is not full. */
#define UNICOMMUART_STAT_TXFF_SET                ((uint32_t)0x00000040U)         /* !< transmit FIFO is full. */
/* UNICOMMUART_STAT[RXFF] Bits */
#define UNICOMMUART_STAT_RXFF_OFS                (3)                             /* !< RXFF Offset */
#define UNICOMMUART_STAT_RXFF_MASK               ((uint32_t)0x00000008U)         /* !< UART Receive FIFO Full */
#define UNICOMMUART_STAT_RXFF_CLEARED            ((uint32_t)0x00000000U)         /* !< The receiver can receive data. */
#define UNICOMMUART_STAT_RXFF_SET                ((uint32_t)0x00000008U)         /* !< receive FIFO is full. */
/* UNICOMMUART_STAT[TXFE] Bits */
#define UNICOMMUART_STAT_TXFE_OFS                (5)                             /* !< TXFE Offset */
#define UNICOMMUART_STAT_TXFE_MASK               ((uint32_t)0x00000020U)         /* !< UART Transmit FIFO Empty */
#define UNICOMMUART_STAT_TXFE_CLEARED            ((uint32_t)0x00000000U)         /* !< The transmitter has data to
                                                                                    transmit. */
#define UNICOMMUART_STAT_TXFE_SET                ((uint32_t)0x00000020U)         /* !< transmit FIFO is empty. */
/* UNICOMMUART_STAT[CTS] Bits */
#define UNICOMMUART_STAT_CTS_OFS                 (8)                             /* !< CTS Offset */
#define UNICOMMUART_STAT_CTS_MASK                ((uint32_t)0x00000100U)         /* !< Clear To Send */
#define UNICOMMUART_STAT_CTS_CLEARED             ((uint32_t)0x00000000U)         /* !< The CTS signal is not asserted
                                                                                    (high). */
#define UNICOMMUART_STAT_CTS_SET                 ((uint32_t)0x00000100U)         /* !< The CTS signal is asserted (low). */
/* UNICOMMUART_STAT[IDLE] Bits */
#define UNICOMMUART_STAT_IDLE_OFS                (9)                             /* !< IDLE Offset */
#define UNICOMMUART_STAT_IDLE_MASK               ((uint32_t)0x00000200U)         /* !< IDLE mode has been detected in
                                                                                    Idleline-Multiprocessor-Mode. The
                                                                                    IDLE bit is used as an address tag
                                                                                    for each block of characters. In
                                                                                    idle-line multiprocessor format, this
                                                                                    bit is set when a received character
                                                                                    is an address. */
#define UNICOMMUART_STAT_IDLE_CLEARED            ((uint32_t)0x00000000U)         /* !< IDLE has not been detected before
                                                                                    last received character. (In
                                                                                    idle-line multiprocessor mode). */
#define UNICOMMUART_STAT_IDLE_SET                ((uint32_t)0x00000200U)         /* !< IDLE has been detected before last
                                                                                    received character. (In idle-line
                                                                                    multiprocessor mode). */
/* UNICOMMUART_STAT[RXFE] Bits */
#define UNICOMMUART_STAT_RXFE_OFS                (2)                             /* !< RXFE Offset */
#define UNICOMMUART_STAT_RXFE_MASK               ((uint32_t)0x00000004U)         /* !< UART Receive FIFO Empty */
#define UNICOMMUART_STAT_RXFE_CLEARED            ((uint32_t)0x00000000U)         /* !< The receiver is not empty. */
#define UNICOMMUART_STAT_RXFE_SET                ((uint32_t)0x00000004U)         /* !< eceive FIFO is empty. */
/* UNICOMMUART_STAT[RXCLR] Bits */
#define UNICOMMUART_STAT_RXCLR_OFS               (4)                             /* !< RXCLR Offset */
#define UNICOMMUART_STAT_RXCLR_MASK              ((uint32_t)0x00000010U)         /* !< RX FIFO Clear Status */
#define UNICOMMUART_STAT_RXCLR_CLEARED           ((uint32_t)0x00000000U)         /* !< FIFO is not cleared */
#define UNICOMMUART_STAT_RXCLR_SET               ((uint32_t)0x00000010U)         /* !< FIFO clear is complete */
/* UNICOMMUART_STAT[TXCLR] Bits */
#define UNICOMMUART_STAT_TXCLR_OFS               (7)                             /* !< TXCLR Offset */
#define UNICOMMUART_STAT_TXCLR_MASK              ((uint32_t)0x00000080U)         /* !< TX FIFO Clear Status */
#define UNICOMMUART_STAT_TXCLR_CLEARED           ((uint32_t)0x00000000U)         /* !< FIFO is not cleared */
#define UNICOMMUART_STAT_TXCLR_SET               ((uint32_t)0x00000080U)         /* !< FIFO clear is complete */
/* UNICOMMUART_STAT[SENDIDLE] Bits */
#define UNICOMMUART_STAT_SENDIDLE_OFS            (10)                            /* !< SENDIDLE Offset */
#define UNICOMMUART_STAT_SENDIDLE_MASK           ((uint32_t)0x00000400U)         /* !< TX FIFO Clear Status */
#define UNICOMMUART_STAT_SENDIDLE_CLEARED        ((uint32_t)0x00000000U)         /* !< IDLE condition has not yet been
                                                                                    sent on the line */
#define UNICOMMUART_STAT_SENDIDLE_SET            ((uint32_t)0x00000400U)         /* !< IDLE condition has been sent on the
                                                                                    line */

/* UNICOMMUART_IFLS Bits */
/* UNICOMMUART_IFLS[TXIFLSEL] Bits */
#define UNICOMMUART_IFLS_TXIFLSEL_OFS            (0)                             /* !< TXIFLSEL Offset */
#define UNICOMMUART_IFLS_TXIFLSEL_MASK           ((uint32_t)0x00000007U)         /* !< FIFO Level Select for generating
                                                                                    events (interrupt/dma).  Note: for
                                                                                    undefined settings the default
                                                                                    configuration is used. */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_3_4        ((uint32_t)0x00000001U)         /* !< TX FIFO <= 3/4 empty */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_1_2        ((uint32_t)0x00000002U)         /* !< TX FIFO <= 1/2 empty (default) */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_1_4        ((uint32_t)0x00000003U)         /* !< TX FIFO <= 1/4 empty */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_NOT_FULL   ((uint32_t)0x00000004U)         /* !< Opposite of full */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_EMPTY      ((uint32_t)0x00000005U)         /* !< TX FIFO is empty */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY ((uint32_t)0x00000006U)         /* !< TX FIFO <= 1 */
#define UNICOMMUART_IFLS_TXIFLSEL_LVL_ALMOST_FULL ((uint32_t)0x00000007U)         /* !< TX_FIFO >= (MAX_FIFO_LEN -1) */
/* UNICOMMUART_IFLS[RXIFLSEL] Bits */
#define UNICOMMUART_IFLS_RXIFLSEL_OFS            (4)                             /* !< RXIFLSEL Offset */
#define UNICOMMUART_IFLS_RXIFLSEL_MASK           ((uint32_t)0x00000070U)         /* !< FIFO Level Select for generating
                                                                                    events (interrupt/dma).  Note: for
                                                                                    undefined settings the default
                                                                                    configuration is used. */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_1_4        ((uint32_t)0x00000010U)         /* !< RX FIFO >= 1/4 full */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_1_2        ((uint32_t)0x00000020U)         /* !< RX FIFO >= 1/2 full (default) */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_3_4        ((uint32_t)0x00000030U)         /* !< RX FIFO >= 3/4 full */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_NOT_EMPTY  ((uint32_t)0x00000040U)         /* !< Opposite of empty */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_FULL       ((uint32_t)0x00000050U)         /* !< RX FIFO is full */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_ALMOST_FULL ((uint32_t)0x00000060U)         /* !< RX_FIFO >= (MAX_FIFO_LEN -1) */
#define UNICOMMUART_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY ((uint32_t)0x00000070U)         /* !< RX_FIFO <= 1 */
/* UNICOMMUART_IFLS[RXTOSEL] Bits */
#define UNICOMMUART_IFLS_RXTOSEL_OFS             (8)                             /* !< RXTOSEL Offset */
#define UNICOMMUART_IFLS_RXTOSEL_MASK            ((uint32_t)0x00000F00U)         /* !< UART Receive Interrupt Timeout
                                                                                    Select. When receiving no start edge
                                                                                    for an additional character within
                                                                                    the set bittimes a RX interrupt is
                                                                                    set even if the FIFO level is not
                                                                                    reached. A value of 0 disables this
                                                                                    function. */
#define UNICOMMUART_IFLS_RXTOSEL_MINIMUM         ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_IFLS_RXTOSEL_MAXIMUM         ((uint32_t)0x00000F00U)         /* !< Highest possible value */
/* UNICOMMUART_IFLS[TXCLR] Bits */
#define UNICOMMUART_IFLS_TXCLR_OFS               (3)                             /* !< TXCLR Offset */
#define UNICOMMUART_IFLS_TXCLR_MASK              ((uint32_t)0x00000008U)         /* !< TX FIFO CLEAR. Setting this bit
                                                                                    will clear the TX FIFO contents. */
#define UNICOMMUART_IFLS_TXCLR_DISABLE           ((uint32_t)0x00000000U)         /* !< Disable FIFO clear */
#define UNICOMMUART_IFLS_TXCLR_ENABLE            ((uint32_t)0x00000008U)         /* !< Enable FIFO Clear */
/* UNICOMMUART_IFLS[RXCLR] Bits */
#define UNICOMMUART_IFLS_RXCLR_OFS               (7)                             /* !< RXCLR Offset */
#define UNICOMMUART_IFLS_RXCLR_MASK              ((uint32_t)0x00000080U)         /* !< RX FIFO CLEAR. Setting this bit
                                                                                    will clear the RX FIFO contents. */
#define UNICOMMUART_IFLS_RXCLR_DISABLE           ((uint32_t)0x00000000U)         /* !< Disable FIFO clear */
#define UNICOMMUART_IFLS_RXCLR_ENABLE            ((uint32_t)0x00000080U)         /* !< Enable FIFO Clear */

/* UNICOMMUART_IBRD Bits */
/* UNICOMMUART_IBRD[DIVINT] Bits */
#define UNICOMMUART_IBRD_DIVINT_OFS              (0)                             /* !< DIVINT Offset */
#define UNICOMMUART_IBRD_DIVINT_MASK             ((uint32_t)0x0000FFFFU)         /* !< Integer Baud-Rate Divisor */
#define UNICOMMUART_IBRD_DIVINT_MINIMUM          ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_IBRD_DIVINT_MAXIMUM          ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMUART_FBRD Bits */
/* UNICOMMUART_FBRD[DIVFRAC] Bits */
#define UNICOMMUART_FBRD_DIVFRAC_OFS             (0)                             /* !< DIVFRAC Offset */
#define UNICOMMUART_FBRD_DIVFRAC_MASK            ((uint32_t)0x0000003FU)         /* !< Fractional Baud-Rate Divisor */
#define UNICOMMUART_FBRD_DIVFRAC_MINIMUM         ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_FBRD_DIVFRAC_MAXIMUM         ((uint32_t)0x0000003FU)         /* !< Highest possible value */

/* UNICOMMUART_GFCTL Bits */
/* UNICOMMUART_GFCTL[DGFSEL] Bits */
#define UNICOMMUART_GFCTL_DGFSEL_OFS             (0)                             /* !< DGFSEL Offset */
#define UNICOMMUART_GFCTL_DGFSEL_MASK            ((uint32_t)0x0000003FU)         /* !< Glitch Suppression Pulse Width
                                                                                    This field controls the pulse width
                                                                                    select for glitch suppression on the
                                                                                    RX line.  The value programmed in
                                                                                    this field gives the number of cycles
                                                                                    of functional clock up to which the
                                                                                    glitch has to be suppressed on the RX
                                                                                    line.  In IRDA mode: The minimum
                                                                                    pulse length for receive is given by:
                                                                                    t(MIN) = (DGFSEL) / f(IRTXCLK) */
#define UNICOMMUART_GFCTL_DGFSEL_DISABLED        ((uint32_t)0x00000000U)         /* !< Bypass GF */
#define UNICOMMUART_GFCTL_DGFSEL_MAXIMUM         ((uint32_t)0x0000003FU)         /* !< Highest Possible Value */

/* UNICOMMUART_TXDATA Bits */
/* UNICOMMUART_TXDATA[DATA] Bits */
#define UNICOMMUART_TXDATA_DATA_OFS              (0)                             /* !< DATA Offset */
#define UNICOMMUART_TXDATA_DATA_MASK             ((uint32_t)0x000000FFU)         /* !< Data Transmitted or Received  Data
                                                                                    that is to be transmitted via the
                                                                                    UART is written to this field.  When
                                                                                    read, this field contains the data
                                                                                    that was received by the UART. */
#define UNICOMMUART_TXDATA_DATA_MINIMUM          ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_TXDATA_DATA_MAXIMUM          ((uint32_t)0x000000FFU)         /* !< Highest possible value */

/* UNICOMMUART_RXDATA Bits */
/* UNICOMMUART_RXDATA[DATA] Bits */
#define UNICOMMUART_RXDATA_DATA_OFS              (0)                             /* !< DATA Offset */
#define UNICOMMUART_RXDATA_DATA_MASK             ((uint32_t)0x000000FFU)         /* !< Received Data.  When read, this
                                                                                    field contains the data that was
                                                                                    received by the UART. */
#define UNICOMMUART_RXDATA_DATA_MINIMUM          ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_RXDATA_DATA_MAXIMUM          ((uint32_t)0x000000FFU)         /* !< Highest possible value */
/* UNICOMMUART_RXDATA[FRMERR] Bits */
#define UNICOMMUART_RXDATA_FRMERR_OFS            (8)                             /* !< FRMERR Offset */
#define UNICOMMUART_RXDATA_FRMERR_MASK           ((uint32_t)0x00000100U)         /* !< UART Framing Error  Writing to this
                                                                                    bit has no effect. The flag is
                                                                                    cleared by writing 1 to the FRMERR
                                                                                    bit in the UART EVENT ICLR register.
                                                                                    This error is associated with the
                                                                                    character at the top of the FIFO. */
#define UNICOMMUART_RXDATA_FRMERR_CLR            ((uint32_t)0x00000000U)         /* !< No framing error has occurred */
#define UNICOMMUART_RXDATA_FRMERR_SET            ((uint32_t)0x00000100U)         /* !< The received character does not
                                                                                    have a valid stop bit sequence, which
                                                                                    is one or two stop bits depending on
                                                                                    the UARTLCRH.STP2 setting (a valid
                                                                                    stop bit is 1). */
/* UNICOMMUART_RXDATA[PARERR] Bits */
#define UNICOMMUART_RXDATA_PARERR_OFS            (9)                             /* !< PARERR Offset */
#define UNICOMMUART_RXDATA_PARERR_MASK           ((uint32_t)0x00000200U)         /* !< UART Parity Error   Writing to this
                                                                                    bit has no effect. The flag is
                                                                                    cleared by writing 1 to the PARERR
                                                                                    bit in the UART EVENT ICLR register. */
#define UNICOMMUART_RXDATA_PARERR_CLR            ((uint32_t)0x00000000U)         /* !< No parity error has occurred */
#define UNICOMMUART_RXDATA_PARERR_SET            ((uint32_t)0x00000200U)         /* !< The parity of the received data
                                                                                    character does not match the parity
                                                                                    defined by bits 2 and 7 of the
                                                                                    UARTLCRH register. */
/* UNICOMMUART_RXDATA[BRKERR] Bits */
#define UNICOMMUART_RXDATA_BRKERR_OFS            (10)                            /* !< BRKERR Offset */
#define UNICOMMUART_RXDATA_BRKERR_MASK           ((uint32_t)0x00000400U)         /* !< UART Break Error  Writing to this
                                                                                    bit has no effect. The flag is
                                                                                    cleared by writing 1 to the BRKERR
                                                                                    bit in the UART EVENT ICLR register.
                                                                                    This error is associated with the
                                                                                    character at the top of the FIFO.
                                                                                    When a break occurs, only one 0
                                                                                    character is loaded into the FIFO.
                                                                                    The next character is only enabled
                                                                                    after the receive data input goes to
                                                                                    a 1 (marking state) and the next
                                                                                    valid start bit is received. */
#define UNICOMMUART_RXDATA_BRKERR_CLR            ((uint32_t)0x00000000U)         /* !< No break condition has occurred */
#define UNICOMMUART_RXDATA_BRKERR_SET            ((uint32_t)0x00000400U)         /* !< A break condition has been
                                                                                    detected, indicating that the receive
                                                                                    data input was held low for longer
                                                                                    than a full-word transmission time
                                                                                    (defined as start, data, parity, and
                                                                                    stop bits). */
/* UNICOMMUART_RXDATA[NERR] Bits */
#define UNICOMMUART_RXDATA_NERR_OFS              (11)                            /* !< NERR Offset */
#define UNICOMMUART_RXDATA_NERR_MASK             ((uint32_t)0x00000800U)         /* !< Noise Error. Writing to this bit
                                                                                    has no effect. The flag is cleared by
                                                                                    writing 1 to the NERR bit in the UART
                                                                                    EVENT ICLR register. */
#define UNICOMMUART_RXDATA_NERR_CLR              ((uint32_t)0x00000000U)         /* !< No noise error occurred */
#define UNICOMMUART_RXDATA_NERR_SET              ((uint32_t)0x00000800U)         /* !< Noise error occurred during
                                                                                    majority voting */

/* UNICOMMUART_LINCNT Bits */
/* UNICOMMUART_LINCNT[VALUE] Bits */
#define UNICOMMUART_LINCNT_VALUE_OFS             (0)                             /* !< VALUE Offset */
#define UNICOMMUART_LINCNT_VALUE_MASK            ((uint32_t)0x0000FFFFU)         /* !< 16 bit up counter clocked by the
                                                                                    functional clock of the UART. */
#define UNICOMMUART_LINCNT_VALUE_MINIMUM         ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_LINCNT_VALUE_MAXIMUM         ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMUART_LINCTL Bits */
/* UNICOMMUART_LINCTL[CTRENA] Bits */
#define UNICOMMUART_LINCTL_CTRENA_OFS            (0)                             /* !< CTRENA Offset */
#define UNICOMMUART_LINCTL_CTRENA_MASK           ((uint32_t)0x00000001U)         /* !< LIN Counter Enable. LIN counter
                                                                                    will only count when enabled. */
#define UNICOMMUART_LINCTL_CTRENA_DISABLE        ((uint32_t)0x00000000U)         /* !< Counter disabled */
#define UNICOMMUART_LINCTL_CTRENA_ENABLE         ((uint32_t)0x00000001U)         /* !< Counter enabled */
/* UNICOMMUART_LINCTL[ZERONE] Bits */
#define UNICOMMUART_LINCTL_ZERONE_OFS            (1)                             /* !< ZERONE Offset */
#define UNICOMMUART_LINCTL_ZERONE_MASK           ((uint32_t)0x00000002U)         /* !< Zero on negative Edge of RXD. When
                                                                                    enabled the counter is set to 0 and
                                                                                    starts counting on a negative edge of
                                                                                    RXD */
#define UNICOMMUART_LINCTL_ZERONE_DISABLE        ((uint32_t)0x00000000U)         /* !< Zero on negative edge disabled */
#define UNICOMMUART_LINCTL_ZERONE_ENABLE         ((uint32_t)0x00000002U)         /* !< Zero on negative edge enabled */
/* UNICOMMUART_LINCTL[CNTRXLOW] Bits */
#define UNICOMMUART_LINCTL_CNTRXLOW_OFS          (2)                             /* !< CNTRXLOW Offset */
#define UNICOMMUART_LINCTL_CNTRXLOW_MASK         ((uint32_t)0x00000004U)         /* !< Count while low Signal on RXD When
                                                                                    counter is enabled and the signal on
                                                                                    RXD is low, the counter increments. */
#define UNICOMMUART_LINCTL_CNTRXLOW_DISABLE      ((uint32_t)0x00000000U)         /* !< Count while low Signal on UARTxRXD
                                                                                    disabled */
#define UNICOMMUART_LINCTL_CNTRXLOW_ENABLE       ((uint32_t)0x00000004U)         /* !< Count while low Signal on UARTxRXD
                                                                                    enabled */
/* UNICOMMUART_LINCTL[LINC0CAP] Bits */
#define UNICOMMUART_LINCTL_LINC0CAP_OFS          (4)                             /* !< LINC0CAP Offset */
#define UNICOMMUART_LINCTL_LINC0CAP_MASK         ((uint32_t)0x00000010U)         /* !< Capture Counter on negative RXD
                                                                                    Edge. When enabled the counter value
                                                                                    is captured to LINC0 register on each
                                                                                    negative RXD edge. A LINC0 interrupt
                                                                                    is triggered when enabled. */
#define UNICOMMUART_LINCTL_LINC0CAP_DISABLE      ((uint32_t)0x00000000U)         /* !< Capture counter on negative
                                                                                    UARTxRXD edge disabled */
#define UNICOMMUART_LINCTL_LINC0CAP_ENABLE       ((uint32_t)0x00000010U)         /* !< Capture counter on negative
                                                                                    UARTxRXD edge enabled */
/* UNICOMMUART_LINCTL[LINC1CAP] Bits */
#define UNICOMMUART_LINCTL_LINC1CAP_OFS          (5)                             /* !< LINC1CAP Offset */
#define UNICOMMUART_LINCTL_LINC1CAP_MASK         ((uint32_t)0x00000020U)         /* !< Capture Counter on positive RXD
                                                                                    Edge. When enabled the counter value
                                                                                    is captured to LINC1 register on each
                                                                                    positive RXD edge. A LINC1 interrupt
                                                                                    is triggered when enabled. */
#define UNICOMMUART_LINCTL_LINC1CAP_DISABLE      ((uint32_t)0x00000000U)         /* !< Capture counter on positive
                                                                                    UARTxRXD edge disabled */
#define UNICOMMUART_LINCTL_LINC1CAP_ENABLE       ((uint32_t)0x00000020U)         /* !< Capture counter on positive
                                                                                    UARTxRXD edge enabled */
/* UNICOMMUART_LINCTL[LINC0_MATCH] Bits */
#define UNICOMMUART_LINCTL_LINC0_MATCH_OFS       (6)                             /* !< LINC0_MATCH Offset */
#define UNICOMMUART_LINCTL_LINC0_MATCH_MASK      ((uint32_t)0x00000040U)         /* !< Counter Compare Match Mode  When
                                                                                    this bit is set to 1 a counter
                                                                                    compare match with LINC0 register
                                                                                    triggers an LINC0 interrupt when
                                                                                    enabled. */
#define UNICOMMUART_LINCTL_LINC0_MATCH_DISABLE   ((uint32_t)0x00000000U)         /* !< Counter compare match mode disabled
                                                                                    (capture mode enabled) */
#define UNICOMMUART_LINCTL_LINC0_MATCH_ENABLE    ((uint32_t)0x00000040U)         /* !< Counter compare match enabled
                                                                                    (capture mode disabled) */

/* UNICOMMUART_LINC0 Bits */
/* UNICOMMUART_LINC0[DATA] Bits */
#define UNICOMMUART_LINC0_DATA_OFS               (0)                             /* !< DATA Offset */
#define UNICOMMUART_LINC0_DATA_MASK              ((uint32_t)0x0000FFFFU)         /* !< 16 Bit Capture / Compare Register
                                                                                    Captures current LINCTR value on RXD
                                                                                    falling edge and can generate a LINC0
                                                                                    interrupt when capture is enabled
                                                                                    (LINC0CAP = 1).  If compare mode is
                                                                                    enabled (LINC0_MATCH = 1), a counter
                                                                                    match can generate a LINC0 interrupt. */
#define UNICOMMUART_LINC0_DATA_MINIMUM           ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_LINC0_DATA_MAXIMUM           ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMUART_LINC1 Bits */
/* UNICOMMUART_LINC1[DATA] Bits */
#define UNICOMMUART_LINC1_DATA_OFS               (0)                             /* !< DATA Offset */
#define UNICOMMUART_LINC1_DATA_MASK              ((uint32_t)0x0000FFFFU)         /* !< 16 Bit Capture / Compare Register
                                                                                    Captures current LINCTR value on RXD
                                                                                    rising edge and can generate a LINC1
                                                                                    interrupt when capture is enabled
                                                                                    (LINC1CAP = 1) */
#define UNICOMMUART_LINC1_DATA_MINIMUM           ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_LINC1_DATA_MAXIMUM           ((uint32_t)0x0000FFFFU)         /* !< Highest possible value */

/* UNICOMMUART_IRCTL Bits */
/* UNICOMMUART_IRCTL[IRTXPL] Bits */
#define UNICOMMUART_IRCTL_IRTXPL_OFS             (2)                             /* !< IRTXPL Offset */
#define UNICOMMUART_IRCTL_IRTXPL_MASK            ((uint32_t)0x000000FCU)         /* !< Transmit pulse length. Pulse length
                                                                                    t(PULSE) = (IRTXPLx + 1) / [2 *
                                                                                    f(IRTXCLK)] (IRTXCLK = functional
                                                                                    clock of the UART) */
#define UNICOMMUART_IRCTL_IRTXPL_MINIMUM         ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_IRCTL_IRTXPL_MAXIMUM         ((uint32_t)0x000000FCU)         /* !< Highest possible value */
/* UNICOMMUART_IRCTL[IRRXPL] Bits */
#define UNICOMMUART_IRCTL_IRRXPL_OFS             (9)                             /* !< IRRXPL Offset */
#define UNICOMMUART_IRCTL_IRRXPL_MASK            ((uint32_t)0x00000200U)         /* !< IrDA receive input UCAxRXD polarity */
#define UNICOMMUART_IRCTL_IRRXPL_HIGH            ((uint32_t)0x00000000U)         /* !< IrDA transceiver delivers a high
                                                                                    pulse when a light pulse is seen */
#define UNICOMMUART_IRCTL_IRRXPL_LOW             ((uint32_t)0x00000200U)         /* !< IrDA transceiver delivers a low
                                                                                    pulse when a light pulse is seen */
/* UNICOMMUART_IRCTL[IREN] Bits */
#define UNICOMMUART_IRCTL_IREN_OFS               (0)                             /* !< IREN Offset */
#define UNICOMMUART_IRCTL_IREN_MASK              ((uint32_t)0x00000001U)         /* !< IrDA encoder/decoder enable */
#define UNICOMMUART_IRCTL_IREN_DISABLE           ((uint32_t)0x00000000U)         /* !< IrDA encoder/decoder disabled */
#define UNICOMMUART_IRCTL_IREN_ENABLE            ((uint32_t)0x00000001U)         /* !< IrDA encoder/decoder enabled */
/* UNICOMMUART_IRCTL[IRTXCLK] Bits */
#define UNICOMMUART_IRCTL_IRTXCLK_OFS            (1)                             /* !< IRTXCLK Offset */
#define UNICOMMUART_IRCTL_IRTXCLK_MASK           ((uint32_t)0x00000002U)         /* !< IrDA transmit pulse clock select */
#define UNICOMMUART_IRCTL_IRTXCLK_BITCLK         ((uint32_t)0x00000000U)         /* !< IrDA encode data is based on the
                                                                                    functional clock. */
#define UNICOMMUART_IRCTL_IRTXCLK_BRCLK          ((uint32_t)0x00000002U)         /* !< IrDA encode data is based on the
                                                                                    Baud Rate clock< when select 8x
                                                                                    oversampling, the IRTXPL cycle should
                                                                                    less 8;  when select 16x
                                                                                    oversampling, the IRTXPL cycle should
                                                                                    less 16. */

/* UNICOMMUART_AMASK Bits */
/* UNICOMMUART_AMASK[VALUE] Bits */
#define UNICOMMUART_AMASK_VALUE_OFS              (0)                             /* !< VALUE Offset */
#define UNICOMMUART_AMASK_VALUE_MASK             ((uint32_t)0x000000FFU)         /* !< Self Address Mask for 9-Bit Mode
                                                                                    This field contains the address mask
                                                                                    that creates a set of addresses that
                                                                                    should be matched.  A 0 bit in the
                                                                                    MSK bitfield configures, that the
                                                                                    corresponding bit in the ADDR
                                                                                    bitfield of the UARTxADDR register is
                                                                                    don't care. A 1 bit in the MSK
                                                                                    bitfield configures, that the
                                                                                    corresponding bit in the ADDR
                                                                                    bitfield of the UARTxADDR register
                                                                                    must match. */
#define UNICOMMUART_AMASK_VALUE_MINIMUM          ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_AMASK_VALUE_MAXIMUM          ((uint32_t)0x000000FFU)         /* !< Highest possible value */

/* UNICOMMUART_ADDR Bits */
/* UNICOMMUART_ADDR[VALUE] Bits */
#define UNICOMMUART_ADDR_VALUE_OFS               (0)                             /* !< VALUE Offset */
#define UNICOMMUART_ADDR_VALUE_MASK              ((uint32_t)0x000000FFU)         /* !< Self Address for 9-Bit Mode  This
                                                                                    field contains the address that
                                                                                    should be matched when UARTxAMASK is
                                                                                    FFh. */
#define UNICOMMUART_ADDR_VALUE_MINIMUM           ((uint32_t)0x00000000U)         /* !< Smallest value */
#define UNICOMMUART_ADDR_VALUE_MAXIMUM           ((uint32_t)0x000000FFU)         /* !< Highest possible value */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_unicommuart__include */
