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

#ifndef ti_devices_msp_peripherals_hw_i2s__include
#define ti_devices_msp_peripherals_hw_i2s__include

/* Filename: hw_i2s.h */
/* Revised: 2025-07-22 15:38:34 */
/* Revision: 5269ff1910f5f8ca76039289749ec80c6ead1fda */

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
* I2S Registers
******************************************************************************/
#define I2S_DMA_TRIG_TX_OFS                      ((uint32_t)0x00001080U)
#define I2S_DMA_TRIG_RX_OFS                      ((uint32_t)0x00001050U)
#define I2S_CPU_INT_OFS                          ((uint32_t)0x00001020U)
#define I2S_GPRCM_OFS                            ((uint32_t)0x00000800U)


/** @addtogroup I2S_DMA_TRIG_TX
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
} I2S_DMA_TRIG_TX_Regs;

/*@}*/ /* end of group I2S_DMA_TRIG_TX */

/** @addtogroup I2S_DMA_TRIG_RX
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
} I2S_DMA_TRIG_RX_Regs;

/*@}*/ /* end of group I2S_DMA_TRIG_RX */

/** @addtogroup I2S_CPU_INT
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) Interrupt index */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) Interrupt Mask Register  Selects mask states of
                                                      the flags in [IRQFLAGS.*] that contribute to the I2S_IRQ event. */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) This registers gives the raw interrupt status */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) This registers gives the raw interrupt status */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) Interrupt Set Register. This register can be used
                                                      by software for diagnostics and safety checking purposes. */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) Interrupt clear register. This register allows
                                                      software to clear interrupts. */
} I2S_CPU_INT_Regs;

/*@}*/ /* end of group I2S_CPU_INT */

/** @addtogroup I2S_GPRCM
  @{
*/

typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) Power enable */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) Reset Control */
  __IO uint32_t CLKCFG;                            /* !< (@ 0x00000808) Peripheral Clock Configuration Register */
       uint32_t RESERVED0[2];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) Status Register */
} I2S_GPRCM_Regs;

/*@}*/ /* end of group I2S_GPRCM */

/** @addtogroup I2S
  @{
*/

typedef struct {
       uint32_t RESERVED0[512];
  I2S_GPRCM_Regs  GPRCM;                             /* !< (@ 0x00000800) */
       uint32_t RESERVED1[508];
  __IO uint32_t PDBGCTL;                           /* !< (@ 0x00001008) Peripheral Debug Control */
       uint32_t RESERVED2[5];
  I2S_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) */
       uint32_t RESERVED3;
  I2S_DMA_TRIG_RX_Regs  DMA_TRIG_RX;                       /* !< (@ 0x00001050) */
       uint32_t RESERVED4[3];
  I2S_DMA_TRIG_TX_Regs  DMA_TRIG_TX;                       /* !< (@ 0x00001080) */
       uint32_t RESERVED5[16];
  __O  uint32_t INTCTL;                            /* !< (@ 0x000010E4) Interrupt control register */
       uint32_t RESERVED6[6];
  __IO uint32_t FMTCFG;                            /* !< (@ 0x00001100) This register configures the serial interface
                                                      format */
  __IO uint32_t CLKCTL;                            /* !< (@ 0x00001104) This register controls internal audio clock */
  __I  uint32_t STAT;                              /* !< (@ 0x00001108) Status Register */
  __IO uint32_t IFLS;                              /* !< (@ 0x0000110C) Interrupt FIFO Level Select Register */
  __IO uint32_t WCLKSRC;                           /* !< (@ 0x00001110) This register configures the WCLK Source */
       uint32_t RESERVED7;
  __IO uint32_t DIRCFG;                            /* !< (@ 0x00001118) This register configures the direction of data
                                                      pins(AD0/AD1) */
       uint32_t RESERVED8;
  __O  uint32_t TXDATA;                            /* !< (@ 0x00001120) Transmit Data Register */
  __I  uint32_t RXDATA;                            /* !< (@ 0x00001124) Receive Data Register */
       uint32_t RESERVED9[8];
  __IO uint32_t WMASK0;                            /* !< (@ 0x00001148) This register configures the word selection dit
                                                      mask for data pin 0(AD0) */
  __IO uint32_t WMASK1;                            /* !< (@ 0x0000114C) This register configures the word selection dit
                                                      mask for data pin 1(AD1) */
       uint32_t RESERVED10[4];
  __IO uint32_t MCLKDIV;                           /* !< (@ 0x00001160) This field configures MCLK division ratio */
  __IO uint32_t WCLKDIV;                           /* !< (@ 0x00001164) Configures WCLK division ratio */
  __IO uint32_t BCLKDIV;                           /* !< (@ 0x00001168) This field configures BCLK division ratio */
} I2S_Regs;

/*@}*/ /* end of group I2S */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* I2S Register Control Bits
******************************************************************************/

/* I2S_DMA_TRIG_TX_IMASK Bits */
/* I2S_DMA_TRIG_TX_IMASK[TXINT] Bits */
#define I2S_DMA_TRIG_TX_IMASK_TXINT_OFS          (2)                             /* !< TXINT Offset */
#define I2S_DMA_TRIG_TX_IMASK_TXINT_MASK         ((uint32_t)0x00000004U)         /* !< Enable  Transmit Interrupt. */
#define I2S_DMA_TRIG_TX_IMASK_TXINT_CLR          ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define I2S_DMA_TRIG_TX_IMASK_TXINT_SET          ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */

/* I2S_DMA_TRIG_TX_RIS Bits */
/* I2S_DMA_TRIG_TX_RIS[TXINT] Bits */
#define I2S_DMA_TRIG_TX_RIS_TXINT_OFS            (2)                             /* !< TXINT Offset */
#define I2S_DMA_TRIG_TX_RIS_TXINT_MASK           ((uint32_t)0x00000004U)         /* !< Transmit Interrupt. */
#define I2S_DMA_TRIG_TX_RIS_TXINT_CLR            ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_DMA_TRIG_TX_RIS_TXINT_SET            ((uint32_t)0x00000004U)         /* !< Interrupt occured */

/* I2S_DMA_TRIG_TX_MIS Bits */
/* I2S_DMA_TRIG_TX_MIS[TXINT] Bits */
#define I2S_DMA_TRIG_TX_MIS_TXINT_OFS            (2)                             /* !< TXINT Offset */
#define I2S_DMA_TRIG_TX_MIS_TXINT_MASK           ((uint32_t)0x00000004U)         /* !< Masked  Transmit Interrupt. */
#define I2S_DMA_TRIG_TX_MIS_TXINT_CLR            ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_DMA_TRIG_TX_MIS_TXINT_SET            ((uint32_t)0x00000004U)         /* !< Interrupt occured */

/* I2S_DMA_TRIG_TX_ISET Bits */
/* I2S_DMA_TRIG_TX_ISET[TXINT] Bits */
#define I2S_DMA_TRIG_TX_ISET_TXINT_OFS           (2)                             /* !< TXINT Offset */
#define I2S_DMA_TRIG_TX_ISET_TXINT_MASK          ((uint32_t)0x00000004U)         /* !< Set  Transmit Interrupt. */
#define I2S_DMA_TRIG_TX_ISET_TXINT_NO_EFFECT     ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_DMA_TRIG_TX_ISET_TXINT_SET           ((uint32_t)0x00000004U)         /* !< Set Interrupt */

/* I2S_DMA_TRIG_RX_IMASK Bits */
/* I2S_DMA_TRIG_RX_IMASK[RXINT] Bits */
#define I2S_DMA_TRIG_RX_IMASK_RXINT_OFS          (1)                             /* !< RXINT Offset */
#define I2S_DMA_TRIG_RX_IMASK_RXINT_MASK         ((uint32_t)0x00000002U)         /* !< Enable Receive Interrupt. */
#define I2S_DMA_TRIG_RX_IMASK_RXINT_CLR          ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define I2S_DMA_TRIG_RX_IMASK_RXINT_SET          ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */

/* I2S_DMA_TRIG_RX_RIS Bits */
/* I2S_DMA_TRIG_RX_RIS[RXINT] Bits */
#define I2S_DMA_TRIG_RX_RIS_RXINT_OFS            (1)                             /* !< RXINT Offset */
#define I2S_DMA_TRIG_RX_RIS_RXINT_MASK           ((uint32_t)0x00000002U)         /* !< Receive Interrupt. */
#define I2S_DMA_TRIG_RX_RIS_RXINT_CLR            ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_DMA_TRIG_RX_RIS_RXINT_SET            ((uint32_t)0x00000002U)         /* !< Interrupt occured */

/* I2S_DMA_TRIG_RX_MIS Bits */
/* I2S_DMA_TRIG_RX_MIS[RXINT] Bits */
#define I2S_DMA_TRIG_RX_MIS_RXINT_OFS            (1)                             /* !< RXINT Offset */
#define I2S_DMA_TRIG_RX_MIS_RXINT_MASK           ((uint32_t)0x00000002U)         /* !< Masked Receive Interrupt. */
#define I2S_DMA_TRIG_RX_MIS_RXINT_CLR            ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_DMA_TRIG_RX_MIS_RXINT_SET            ((uint32_t)0x00000002U)         /* !< Interrupt occured */

/* I2S_DMA_TRIG_RX_ISET Bits */
/* I2S_DMA_TRIG_RX_ISET[RXINT] Bits */
#define I2S_DMA_TRIG_RX_ISET_RXINT_OFS           (1)                             /* !< RXINT Offset */
#define I2S_DMA_TRIG_RX_ISET_RXINT_MASK          ((uint32_t)0x00000002U)         /* !< Set Receive Interrupt. */
#define I2S_DMA_TRIG_RX_ISET_RXINT_NO_EFFECT     ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_DMA_TRIG_RX_ISET_RXINT_SET           ((uint32_t)0x00000002U)         /* !< Set Interrupt */

/* I2S_IIDX Bits */
/* I2S_IIDX[STAT] Bits */
#define I2S_IIDX_STAT_OFS                        (0)                             /* !< STAT Offset */
#define I2S_IIDX_STAT_MASK                       ((uint32_t)0x000000FFU)         /* !< Module Interrupt Vector Value.
                                                                                    This register provides the highes
                                                                                    priority interrupt index. A read
                                                                                    clears the corresponding interrupt
                                                                                    flag in RIS and MIS registers.
                                                                                    15h-1Fh = Reserved */
#define I2S_IIDX_STAT_NO_INTR                    ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define I2S_IIDX_STAT_WCLKERR                    ((uint32_t)0x00000001U)         /* !< WCLK Error */
#define I2S_IIDX_STAT_RXIFG                      ((uint32_t)0x00000002U)         /* !< Receive interrupt; Interrupt Flag:
                                                                                    RX */
#define I2S_IIDX_STAT_TXIFG                      ((uint32_t)0x00000003U)         /* !< Transmit interrupt; Interrupt Flag:
                                                                                    TX */
#define I2S_IIDX_STAT_RXFIFO_OVF_EVT             ((uint32_t)0x00000004U)         /* !< RX FIFO Overflow Event/interrupt
                                                                                    pending */
#define I2S_IIDX_STAT_TXFIFO_UNF_EVT             ((uint32_t)0x00000005U)         /* !< TX FIFO underflow interrupt */
#define I2S_IIDX_STAT_DMA_DONE_RX                ((uint32_t)0x00000010U)         /* !< DMA DONE on RX */
#define I2S_IIDX_STAT_DMA_DONE_TX                ((uint32_t)0x00000011U)         /* !< DMA DONE on TX */
#define I2S_IIDX_STAT_DMA_PREIRQ_RX              ((uint32_t)0x00000013U)         /* !< DMA PRE IRQ INTERRUPT */
#define I2S_IIDX_STAT_DMA_PREIRQ_TX              ((uint32_t)0x00000014U)         /* !< DMA PRE IRQ INTERRUPT */

/* I2S_CPU_INT_IMASK Bits */
/* I2S_CPU_INT_IMASK[WCLKERR] Bits */
#define I2S_CPU_INT_IMASK_WCLKERR_OFS            (0)                             /* !< WCLKERR Offset */
#define I2S_CPU_INT_IMASK_WCLKERR_MASK           ((uint32_t)0x00000001U)         /* !< WCLKERR interrupt mask */
#define I2S_CPU_INT_IMASK_WCLKERR_DIS            ((uint32_t)0x00000000U)         /* !< Disable the interrupt mask */
#define I2S_CPU_INT_IMASK_WCLKERR_EN             ((uint32_t)0x00000001U)         /* !< Enable the interrupt mask */
/* I2S_CPU_INT_IMASK[RXINT] Bits */
#define I2S_CPU_INT_IMASK_RXINT_OFS              (1)                             /* !< RXINT Offset */
#define I2S_CPU_INT_IMASK_RXINT_MASK             ((uint32_t)0x00000002U)         /* !< Enable Receive Interrupt. */
#define I2S_CPU_INT_IMASK_RXINT_CLR              ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define I2S_CPU_INT_IMASK_RXINT_SET              ((uint32_t)0x00000002U)         /* !< Set Interrupt Mask */
/* I2S_CPU_INT_IMASK[TXINT] Bits */
#define I2S_CPU_INT_IMASK_TXINT_OFS              (2)                             /* !< TXINT Offset */
#define I2S_CPU_INT_IMASK_TXINT_MASK             ((uint32_t)0x00000004U)         /* !< Enable Transmit Interrupt. */
#define I2S_CPU_INT_IMASK_TXINT_CLR              ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define I2S_CPU_INT_IMASK_TXINT_SET              ((uint32_t)0x00000004U)         /* !< Set Interrupt Mask */
/* I2S_CPU_INT_IMASK[DMA_DONE_RX] Bits */
#define I2S_CPU_INT_IMASK_DMA_DONE_RX_OFS        (15)                            /* !< DMA_DONE_RX Offset */
#define I2S_CPU_INT_IMASK_DMA_DONE_RX_MASK       ((uint32_t)0x00008000U)         /* !< Enable DMA Done on RX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_IMASK_DMA_DONE_RX_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_CPU_INT_IMASK_DMA_DONE_RX_SET        ((uint32_t)0x00008000U)         /* !< Set Interrupt Mask */
/* I2S_CPU_INT_IMASK[DMA_DONE_TX] Bits */
#define I2S_CPU_INT_IMASK_DMA_DONE_TX_OFS        (16)                            /* !< DMA_DONE_TX Offset */
#define I2S_CPU_INT_IMASK_DMA_DONE_TX_MASK       ((uint32_t)0x00010000U)         /* !< Enable DMA Done on TX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_IMASK_DMA_DONE_TX_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_CPU_INT_IMASK_DMA_DONE_TX_SET        ((uint32_t)0x00010000U)         /* !< Set Interrupt Mask */
/* I2S_CPU_INT_IMASK[RXFIFO_OVF] Bits */
#define I2S_CPU_INT_IMASK_RXFIFO_OVF_OFS         (3)                             /* !< RXFIFO_OVF Offset */
#define I2S_CPU_INT_IMASK_RXFIFO_OVF_MASK        ((uint32_t)0x00000008U)         /* !< RXFIFO overflow event mask. */
#define I2S_CPU_INT_IMASK_RXFIFO_OVF_CLR         ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define I2S_CPU_INT_IMASK_RXFIFO_OVF_SET         ((uint32_t)0x00000008U)         /* !< Set Interrupt Mask */
/* I2S_CPU_INT_IMASK[TXFIFO_UNF] Bits */
#define I2S_CPU_INT_IMASK_TXFIFO_UNF_OFS         (4)                             /* !< TXFIFO_UNF Offset */
#define I2S_CPU_INT_IMASK_TXFIFO_UNF_MASK        ((uint32_t)0x00000010U)         /* !< TX FIFO underflow interrupt mask */
#define I2S_CPU_INT_IMASK_TXFIFO_UNF_CLR         ((uint32_t)0x00000000U)         /* !< Clear Interrupt Mask */
#define I2S_CPU_INT_IMASK_TXFIFO_UNF_SET         ((uint32_t)0x00000010U)         /* !< Set Interrupt Mask */

/* I2S_CPU_INT_RIS Bits */
/* I2S_CPU_INT_RIS[WCLKERR] Bits */
#define I2S_CPU_INT_RIS_WCLKERR_OFS              (0)                             /* !< WCLKERR Offset */
#define I2S_CPU_INT_RIS_WCLKERR_MASK             ((uint32_t)0x00000001U)         /* !< This interrupt is set when:  - An
                                                                                    unexpected WCLK edge occurs during
                                                                                    the data delay period of a phase.
                                                                                    Note unexpected WCLK edges during the
                                                                                    word and idle periods of the phase
                                                                                    are not detected. -  In dual-phase
                                                                                    mode, when two WCLK edges are less
                                                                                    than 4 BCLK cycles apart. -  In
                                                                                    single-phase mode, when a WCLK pulse
                                                                                    occurs before the last channel. This
                                                                                    error requires a complete restart
                                                                                    since word synchronization has been
                                                                                    lost. */
#define I2S_CPU_INT_RIS_WCLKERR_CLR              ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_RIS_WCLKERR_SET              ((uint32_t)0x00000001U)         /* !< Interrupt occured */
/* I2S_CPU_INT_RIS[RXINT] Bits */
#define I2S_CPU_INT_RIS_RXINT_OFS                (1)                             /* !< RXINT Offset */
#define I2S_CPU_INT_RIS_RXINT_MASK               ((uint32_t)0x00000002U)         /* !< Receive Interrupt. */
#define I2S_CPU_INT_RIS_RXINT_CLR                ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_RIS_RXINT_SET                ((uint32_t)0x00000002U)         /* !< Interrupt occured */
/* I2S_CPU_INT_RIS[TXINT] Bits */
#define I2S_CPU_INT_RIS_TXINT_OFS                (2)                             /* !< TXINT Offset */
#define I2S_CPU_INT_RIS_TXINT_MASK               ((uint32_t)0x00000004U)         /* !< Transmit Interrupt. */
#define I2S_CPU_INT_RIS_TXINT_CLR                ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_RIS_TXINT_SET                ((uint32_t)0x00000004U)         /* !< Interrupt occured */
/* I2S_CPU_INT_RIS[DMA_DONE_RX] Bits */
#define I2S_CPU_INT_RIS_DMA_DONE_RX_OFS          (15)                            /* !< DMA_DONE_RX Offset */
#define I2S_CPU_INT_RIS_DMA_DONE_RX_MASK         ((uint32_t)0x00008000U)         /* !< DMA Done on RX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_RIS_DMA_DONE_RX_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_CPU_INT_RIS_DMA_DONE_RX_SET          ((uint32_t)0x00008000U)         /* !< Interrupt occured */
/* I2S_CPU_INT_RIS[DMA_DONE_TX] Bits */
#define I2S_CPU_INT_RIS_DMA_DONE_TX_OFS          (16)                            /* !< DMA_DONE_TX Offset */
#define I2S_CPU_INT_RIS_DMA_DONE_TX_MASK         ((uint32_t)0x00010000U)         /* !< DMA Done on TX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_RIS_DMA_DONE_TX_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_CPU_INT_RIS_DMA_DONE_TX_SET          ((uint32_t)0x00010000U)         /* !< Interrupt occured */
/* I2S_CPU_INT_RIS[RXFIFO_OVF] Bits */
#define I2S_CPU_INT_RIS_RXFIFO_OVF_OFS           (3)                             /* !< RXFIFO_OVF Offset */
#define I2S_CPU_INT_RIS_RXFIFO_OVF_MASK          ((uint32_t)0x00000008U)         /* !< RXFIFO overflow event. This
                                                                                    interrupt is set if an RX FIFO
                                                                                    overflow has been detected. */
#define I2S_CPU_INT_RIS_RXFIFO_OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_RIS_RXFIFO_OVF_SET           ((uint32_t)0x00000008U)         /* !< Interrupt occurred */
/* I2S_CPU_INT_RIS[TXFIFO_UNF] Bits */
#define I2S_CPU_INT_RIS_TXFIFO_UNF_OFS           (4)                             /* !< TXFIFO_UNF Offset */
#define I2S_CPU_INT_RIS_TXFIFO_UNF_MASK          ((uint32_t)0x00000010U)         /* !< TX FIFO Underflow Interrupt */
#define I2S_CPU_INT_RIS_TXFIFO_UNF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_RIS_TXFIFO_UNF_SET           ((uint32_t)0x00000010U)         /* !< Interrupt occurred */

/* I2S_CPU_INT_MIS Bits */
/* I2S_CPU_INT_MIS[WCLKERR] Bits */
#define I2S_CPU_INT_MIS_WCLKERR_OFS              (0)                             /* !< WCLKERR Offset */
#define I2S_CPU_INT_MIS_WCLKERR_MASK             ((uint32_t)0x00000001U)         /* !< This interrupt is set when:  - An
                                                                                    unexpected WCLK edge occurs during
                                                                                    the data delay period of a phase.
                                                                                    Note unexpected WCLK edges during the
                                                                                    word and idle periods of the phase
                                                                                    are not detected. -  In dual-phase
                                                                                    mode, when two WCLK edges are less
                                                                                    than 4 BCLK cycles apart. -  In
                                                                                    single-phase mode, when a WCLK pulse
                                                                                    occurs before the last channel. This
                                                                                    error requires a complete restart
                                                                                    since word synchronization has been
                                                                                    lost. */
#define I2S_CPU_INT_MIS_WCLKERR_CLR              ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_WCLKERR_SET              ((uint32_t)0x00000001U)         /* !< Interrupt occured */
/* I2S_CPU_INT_MIS[RXINT] Bits */
#define I2S_CPU_INT_MIS_RXINT_OFS                (1)                             /* !< RXINT Offset */
#define I2S_CPU_INT_MIS_RXINT_MASK               ((uint32_t)0x00000002U)         /* !< Masked Receive Interrupt. */
#define I2S_CPU_INT_MIS_RXINT_CLR                ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_RXINT_SET                ((uint32_t)0x00000002U)         /* !< Interrupt occured */
/* I2S_CPU_INT_MIS[TXINT] Bits */
#define I2S_CPU_INT_MIS_TXINT_OFS                (2)                             /* !< TXINT Offset */
#define I2S_CPU_INT_MIS_TXINT_MASK               ((uint32_t)0x00000004U)         /* !< Masked Transmit Interrupt. */
#define I2S_CPU_INT_MIS_TXINT_CLR                ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_TXINT_SET                ((uint32_t)0x00000004U)         /* !< Interrupt occured */
/* I2S_CPU_INT_MIS[DMA_DONE_RX] Bits */
#define I2S_CPU_INT_MIS_DMA_DONE_RX_OFS          (15)                            /* !< DMA_DONE_RX Offset */
#define I2S_CPU_INT_MIS_DMA_DONE_RX_MASK         ((uint32_t)0x00008000U)         /* !< Masked DMA Done on RX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_MIS_DMA_DONE_RX_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_DMA_DONE_RX_SET          ((uint32_t)0x00008000U)         /* !< Interrupt occured */
/* I2S_CPU_INT_MIS[DMA_DONE_TX] Bits */
#define I2S_CPU_INT_MIS_DMA_DONE_TX_OFS          (16)                            /* !< DMA_DONE_TX Offset */
#define I2S_CPU_INT_MIS_DMA_DONE_TX_MASK         ((uint32_t)0x00010000U)         /* !< Masked DMA Done on TX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_MIS_DMA_DONE_TX_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_DMA_DONE_TX_SET          ((uint32_t)0x00010000U)         /* !< Interrupt occured */
/* I2S_CPU_INT_MIS[RXFIFO_OVF] Bits */
#define I2S_CPU_INT_MIS_RXFIFO_OVF_OFS           (3)                             /* !< RXFIFO_OVF Offset */
#define I2S_CPU_INT_MIS_RXFIFO_OVF_MASK          ((uint32_t)0x00000008U)         /* !< Masked RXFIFO overflow event. This
                                                                                    interrupt is set if an RX FIFO
                                                                                    overflow has been detected. */
#define I2S_CPU_INT_MIS_RXFIFO_OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_RXFIFO_OVF_SET           ((uint32_t)0x00000008U)         /* !< Interrupt occurred */
/* I2S_CPU_INT_MIS[TXFIFO_UNF] Bits */
#define I2S_CPU_INT_MIS_TXFIFO_UNF_OFS           (4)                             /* !< TXFIFO_UNF Offset */
#define I2S_CPU_INT_MIS_TXFIFO_UNF_MASK          ((uint32_t)0x00000010U)         /* !< TX FIFO underflow interrupt */
#define I2S_CPU_INT_MIS_TXFIFO_UNF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt did not occur */
#define I2S_CPU_INT_MIS_TXFIFO_UNF_SET           ((uint32_t)0x00000010U)         /* !< Interrupt occurred */

/* I2S_CPU_INT_ISET Bits */
/* I2S_CPU_INT_ISET[WCLKERR] Bits */
#define I2S_CPU_INT_ISET_WCLKERR_OFS             (0)                             /* !< WCLKERR Offset */
#define I2S_CPU_INT_ISET_WCLKERR_MASK            ((uint32_t)0x00000001U)         /* !< This field sets the interrupt
                                                                                    WCLKERR */
#define I2S_CPU_INT_ISET_WCLKERR_NOEFF           ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_CPU_INT_ISET_WCLKERR_SET             ((uint32_t)0x00000001U)         /* !< Set interrupt */
/* I2S_CPU_INT_ISET[RXINT] Bits */
#define I2S_CPU_INT_ISET_RXINT_OFS               (1)                             /* !< RXINT Offset */
#define I2S_CPU_INT_ISET_RXINT_MASK              ((uint32_t)0x00000002U)         /* !< Set Receive Interrupt. */
#define I2S_CPU_INT_ISET_RXINT_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_CPU_INT_ISET_RXINT_SET               ((uint32_t)0x00000002U)         /* !< Set Interrupt */
/* I2S_CPU_INT_ISET[TXINT] Bits */
#define I2S_CPU_INT_ISET_TXINT_OFS               (2)                             /* !< TXINT Offset */
#define I2S_CPU_INT_ISET_TXINT_MASK              ((uint32_t)0x00000004U)         /* !< Set Transmit Interrupt. */
#define I2S_CPU_INT_ISET_TXINT_NO_EFFECT         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_CPU_INT_ISET_TXINT_SET               ((uint32_t)0x00000004U)         /* !< Set Interrupt */
/* I2S_CPU_INT_ISET[DMA_DONE_RX] Bits */
#define I2S_CPU_INT_ISET_DMA_DONE_RX_OFS         (15)                            /* !< DMA_DONE_RX Offset */
#define I2S_CPU_INT_ISET_DMA_DONE_RX_MASK        ((uint32_t)0x00008000U)         /* !< Set DMA Done on RX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_ISET_DMA_DONE_RX_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_CPU_INT_ISET_DMA_DONE_RX_SET         ((uint32_t)0x00008000U)         /* !< Set Interrupt */
/* I2S_CPU_INT_ISET[DMA_DONE_TX] Bits */
#define I2S_CPU_INT_ISET_DMA_DONE_TX_OFS         (16)                            /* !< DMA_DONE_TX Offset */
#define I2S_CPU_INT_ISET_DMA_DONE_TX_MASK        ((uint32_t)0x00010000U)         /* !< Set DMA Done on TX Event Channel
                                                                                    Interrupt */
#define I2S_CPU_INT_ISET_DMA_DONE_TX_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_CPU_INT_ISET_DMA_DONE_TX_SET         ((uint32_t)0x00010000U)         /* !< Set Interrupt */
/* I2S_CPU_INT_ISET[RXFIFO_OVF] Bits */
#define I2S_CPU_INT_ISET_RXFIFO_OVF_OFS          (3)                             /* !< RXFIFO_OVF Offset */
#define I2S_CPU_INT_ISET_RXFIFO_OVF_MASK         ((uint32_t)0x00000008U)         /* !< Set RXFIFO overflow event. */
#define I2S_CPU_INT_ISET_RXFIFO_OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_CPU_INT_ISET_RXFIFO_OVF_SET          ((uint32_t)0x00000008U)         /* !< Set Interrupt */
/* I2S_CPU_INT_ISET[TXFIFO_UNF] Bits */
#define I2S_CPU_INT_ISET_TXFIFO_UNF_OFS          (4)                             /* !< TXFIFO_UNF Offset */
#define I2S_CPU_INT_ISET_TXFIFO_UNF_MASK         ((uint32_t)0x00000010U)         /* !< Set TX FIFO Underflow Event */
#define I2S_CPU_INT_ISET_TXFIFO_UNF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Writing has no effect */
#define I2S_CPU_INT_ISET_TXFIFO_UNF_SET          ((uint32_t)0x00000010U)         /* !< Set interrupt */

/* I2S_ICLR Bits */
/* I2S_ICLR[WCLKERR] Bits */
#define I2S_ICLR_WCLKERR_OFS                     (0)                             /* !< WCLKERR Offset */
#define I2S_ICLR_WCLKERR_MASK                    ((uint32_t)0x00000001U)         /* !< This field clears the interrupt
                                                                                    WCLKERR */
#define I2S_ICLR_WCLKERR_NOEFF                   ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_ICLR_WCLKERR_CLR                     ((uint32_t)0x00000001U)         /* !< Clear interrupt */
/* I2S_ICLR[RXINT] Bits */
#define I2S_ICLR_RXINT_OFS                       (1)                             /* !< RXINT Offset */
#define I2S_ICLR_RXINT_MASK                      ((uint32_t)0x00000002U)         /* !< Clear Receive Interrupt. */
#define I2S_ICLR_RXINT_NO_EFFECT                 ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_ICLR_RXINT_CLR                       ((uint32_t)0x00000002U)         /* !< Clear Interrupt */
/* I2S_ICLR[TXINT] Bits */
#define I2S_ICLR_TXINT_OFS                       (2)                             /* !< TXINT Offset */
#define I2S_ICLR_TXINT_MASK                      ((uint32_t)0x00000004U)         /* !< Clear Transmit Interrupt. */
#define I2S_ICLR_TXINT_NO_EFFECT                 ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_ICLR_TXINT_CLR                       ((uint32_t)0x00000004U)         /* !< Clear Interrupt */
/* I2S_ICLR[DMA_DONE_RX] Bits */
#define I2S_ICLR_DMA_DONE_RX_OFS                 (15)                            /* !< DMA_DONE_RX Offset */
#define I2S_ICLR_DMA_DONE_RX_MASK                ((uint32_t)0x00008000U)         /* !< Clear DMA Done on RX Event Channel
                                                                                    Interrupt */
#define I2S_ICLR_DMA_DONE_RX_NO_EFFECT           ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_ICLR_DMA_DONE_RX_CLR                 ((uint32_t)0x00008000U)         /* !< Clear Interrupt */
/* I2S_ICLR[DMA_DONE_TX] Bits */
#define I2S_ICLR_DMA_DONE_TX_OFS                 (16)                            /* !< DMA_DONE_TX Offset */
#define I2S_ICLR_DMA_DONE_TX_MASK                ((uint32_t)0x00010000U)         /* !< Clear DMA Done on TX Event Channel
                                                                                    Interrupt */
#define I2S_ICLR_DMA_DONE_TX_NO_EFFECT           ((uint32_t)0x00000000U)         /* !< Interrupt disabled */
#define I2S_ICLR_DMA_DONE_TX_CLR                 ((uint32_t)0x00010000U)         /* !< Clear Interrupt */
/* I2S_ICLR[RXFIFO_OVF] Bits */
#define I2S_ICLR_RXFIFO_OVF_OFS                  (3)                             /* !< RXFIFO_OVF Offset */
#define I2S_ICLR_RXFIFO_OVF_MASK                 ((uint32_t)0x00000008U)         /* !< Clear RXFIFO overflow event. */
#define I2S_ICLR_RXFIFO_OVF_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_ICLR_RXFIFO_OVF_CLR                  ((uint32_t)0x00000008U)         /* !< Clear Interrupt */
/* I2S_ICLR[TXFIFO_UNF] Bits */
#define I2S_ICLR_TXFIFO_UNF_OFS                  (4)                             /* !< TXFIFO_UNF Offset */
#define I2S_ICLR_TXFIFO_UNF_MASK                 ((uint32_t)0x00000010U)         /* !< Clear TXFIFO underflow event */
#define I2S_ICLR_TXFIFO_UNF_NO_EFFECT            ((uint32_t)0x00000000U)         /* !< Writing has no effect */
#define I2S_ICLR_TXFIFO_UNF_CLR                  ((uint32_t)0x00000010U)         /* !< Clear interrupt */

/* I2S_PWREN Bits */
/* I2S_PWREN[ENABLE] Bits */
#define I2S_PWREN_ENABLE_OFS                     (0)                             /* !< ENABLE Offset */
#define I2S_PWREN_ENABLE_MASK                    ((uint32_t)0x00000001U)         /* !< Enable the power */
#define I2S_PWREN_ENABLE_DISABLE                 ((uint32_t)0x00000000U)         /* !< Disable Power */
#define I2S_PWREN_ENABLE_ENABLE                  ((uint32_t)0x00000001U)         /* !< Enable Power */
/* I2S_PWREN[KEY] Bits */
#define I2S_PWREN_KEY_OFS                        (24)                            /* !< KEY Offset */
#define I2S_PWREN_KEY_MASK                       ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define I2S_PWREN_KEY_UNLOCK_W                   ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* I2S_RSTCTL Bits */
/* I2S_RSTCTL[RESETSTKYCLR] Bits */
#define I2S_RSTCTL_RESETSTKYCLR_OFS              (1)                             /* !< RESETSTKYCLR Offset */
#define I2S_RSTCTL_RESETSTKYCLR_MASK             ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define I2S_RSTCTL_RESETSTKYCLR_NOP              ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_RSTCTL_RESETSTKYCLR_CLR              ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* I2S_RSTCTL[RESETASSERT] Bits */
#define I2S_RSTCTL_RESETASSERT_OFS               (0)                             /* !< RESETASSERT Offset */
#define I2S_RSTCTL_RESETASSERT_MASK              ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define I2S_RSTCTL_RESETASSERT_NOP               ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define I2S_RSTCTL_RESETASSERT_ASSERT            ((uint32_t)0x00000001U)         /* !< Assert reset */
/* I2S_RSTCTL[KEY] Bits */
#define I2S_RSTCTL_KEY_OFS                       (24)                            /* !< KEY Offset */
#define I2S_RSTCTL_KEY_MASK                      ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define I2S_RSTCTL_KEY_UNLOCK_W                  ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* I2S_CLKCFG Bits */
/* I2S_CLKCFG[KEY] Bits */
#define I2S_CLKCFG_KEY_OFS                       (24)                            /* !< KEY Offset */
#define I2S_CLKCFG_KEY_MASK                      ((uint32_t)0xFF000000U)         /* !< KEY to Allow State Change -- 0xA9 */
#define I2S_CLKCFG_KEY_UNLOCK                    ((uint32_t)0xA9000000U)         /* !< key value to allow change field of
                                                                                    GPRCM */
/* I2S_CLKCFG[DAICLK] Bits */
#define I2S_CLKCFG_DAICLK_OFS                    (0)                             /* !< DAICLK Offset */
#define I2S_CLKCFG_DAICLK_MASK                   ((uint32_t)0x00000003U)         /* !< Audio Clock */
#define I2S_CLKCFG_DAICLK_SYSOSC                 ((uint32_t)0x00000000U)         /* !< SYSOSC */
#define I2S_CLKCFG_DAICLK_HFXT                   ((uint32_t)0x00000001U)         /* !< HF crystal */
#define I2S_CLKCFG_DAICLK_PLL                    ((uint32_t)0x00000002U)         /* !< PLL */

/* I2S_GPRCM_STAT Bits */
/* I2S_GPRCM_STAT[RESETSTKY] Bits */
#define I2S_GPRCM_STAT_RESETSTKY_OFS             (16)                            /* !< RESETSTKY Offset */
#define I2S_GPRCM_STAT_RESETSTKY_MASK            ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define I2S_GPRCM_STAT_RESETSTKY_NORES           ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define I2S_GPRCM_STAT_RESETSTKY_RESET           ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */

/* I2S_PDBGCTL Bits */
/* I2S_PDBGCTL[FREE] Bits */
#define I2S_PDBGCTL_FREE_OFS                     (0)                             /* !< FREE Offset */
#define I2S_PDBGCTL_FREE_MASK                    ((uint32_t)0x00000001U)         /* !< Free run control */
#define I2S_PDBGCTL_FREE_STOP                    ((uint32_t)0x00000000U)         /* !< The peripheral freezes
                                                                                    functionality while the Core Halted
                                                                                    input is asserted and resumes when it
                                                                                    is deasserted. */
#define I2S_PDBGCTL_FREE_RUN                     ((uint32_t)0x00000001U)         /* !< The peripheral ignores the state of
                                                                                    the Core Halted input */
/* I2S_PDBGCTL[SOFT] Bits */
#define I2S_PDBGCTL_SOFT_OFS                     (1)                             /* !< SOFT Offset */
#define I2S_PDBGCTL_SOFT_MASK                    ((uint32_t)0x00000002U)         /* !< Soft halt boundary control. This
                                                                                    function is only available, if [FREE]
                                                                                    is set to 'STOP' */
#define I2S_PDBGCTL_SOFT_IMMEDIATE               ((uint32_t)0x00000000U)         /* !< The peripheral will halt
                                                                                    immediately, even if the resultant
                                                                                    state will result in corruption if
                                                                                    the system is restarted */
#define I2S_PDBGCTL_SOFT_DELAYED                 ((uint32_t)0x00000002U)         /* !< The peripheral blocks the debug
                                                                                    freeze until it has reached a
                                                                                    boundary where it can resume without
                                                                                    corruption */

/* I2S_INTCTL Bits */
/* I2S_INTCTL[INTEVAL] Bits */
#define I2S_INTCTL_INTEVAL_OFS                   (0)                             /* !< INTEVAL Offset */
#define I2S_INTCTL_INTEVAL_MASK                  ((uint32_t)0x00000001U)         /* !< Writing a 1 to this field
                                                                                    re-evaluates the interrupt sources. */
#define I2S_INTCTL_INTEVAL_DISABLE               ((uint32_t)0x00000000U)         /* !< The interrupt or event line is
                                                                                    disabled. */
#define I2S_INTCTL_INTEVAL_EVAL                  ((uint32_t)0x00000001U)         /* !< The interrupt or event line is in
                                                                                    software mode. Software must clear
                                                                                    the RIS. */

/* I2S_FMTCFG Bits */
/* I2S_FMTCFG[WORDLEN] Bits */
#define I2S_FMTCFG_WORDLEN_OFS                   (0)                             /* !< WORDLEN Offset */
#define I2S_FMTCFG_WORDLEN_MASK                  ((uint32_t)0x0000001FU)         /* !< Number of bits per word (8-32): In
                                                                                    single-phase format, this is the
                                                                                    exact number of bits per word.  In
                                                                                    dual-phase format, this is the
                                                                                    maximum number of bits per word.
                                                                                    Values below 8 and above 32 give
                                                                                    undefined behavior. Data written to
                                                                                    memory is always aligned to 16 or 32
                                                                                    bits as defined by [MEMLEN32]. Bit
                                                                                    widths that differ from this
                                                                                    alignment will either be truncated or
                                                                                    zero padded. */
/* I2S_FMTCFG[DUALPHASE] Bits */
#define I2S_FMTCFG_DUALPHASE_OFS                 (5)                             /* !< DUALPHASE Offset */
#define I2S_FMTCFG_DUALPHASE_MASK                ((uint32_t)0x00000020U)         /* !< This field selects between
                                                                                    dual-phase or single-phase format */
#define I2S_FMTCFG_DUALPHASE_SINGLEPHASE         ((uint32_t)0x00000000U)         /* !< Single-phase: DSP format */
#define I2S_FMTCFG_DUALPHASE_DUALPHASE           ((uint32_t)0x00000020U)         /* !< Dual-phase: I2S, LJF and RJF
                                                                                    formats */
/* I2S_FMTCFG[SMPLEDGE] Bits */
#define I2S_FMTCFG_SMPLEDGE_OFS                  (6)                             /* !< SMPLEDGE Offset */
#define I2S_FMTCFG_SMPLEDGE_MASK                 ((uint32_t)0x00000040U)         /* !< This field configures the sample
                                                                                    edge/ transfer edge of data (and
                                                                                    WCLK) on BCLK */
#define I2S_FMTCFG_SMPLEDGE_NEGEDGE              ((uint32_t)0x00000000U)         /* !< Data is sampled on the negative
                                                                                    edge and clocked out on the positive
                                                                                    edge. */
#define I2S_FMTCFG_SMPLEDGE_POSEDGE              ((uint32_t)0x00000040U)         /* !< Data is sampled on the positive
                                                                                    edge and clocked out on the negative
                                                                                    edge. */
/* I2S_FMTCFG[MEMLEN32] Bits */
#define I2S_FMTCFG_MEMLEN32_OFS                  (7)                             /* !< MEMLEN32 Offset */
#define I2S_FMTCFG_MEMLEN32_MASK                 ((uint32_t)0x00000080U)         /* !< This register configures the size
                                                                                    of each word stored to or loaded from
                                                                                    memory */
#define I2S_FMTCFG_MEMLEN32_16BIT                ((uint32_t)0x00000000U)         /* !< 16-bit (one 16 bit access per
                                                                                    sample) */
#define I2S_FMTCFG_MEMLEN32_32BIT                ((uint32_t)0x00000080U)         /* !< 32-bit(one 32-bit access per
                                                                                    sample) */
/* I2S_FMTCFG[DATADLY] Bits */
#define I2S_FMTCFG_DATADLY_OFS                   (8)                             /* !< DATADLY Offset */
#define I2S_FMTCFG_DATADLY_MASK                  ((uint32_t)0x0000FF00U)         /* !< This field configures the number of
                                                                                    BCLK periods between a WCLK edge and
                                                                                    MSB of the first word in a phase
                                                                                    Note: When 0, MSB of the next word
                                                                                    will be output in the idle period
                                                                                    between LSB of the previous word and
                                                                                    the start of the next word. Otherwise
                                                                                    logical 0 will be output until the
                                                                                    data delay has expired. */
#define I2S_FMTCFG_DATADLY_ZERO                  ((uint32_t)0x00000000U)         /* !< Zero BCLK periods - LJF and DSP
                                                                                    formats */
#define I2S_FMTCFG_DATADLY_ONE                   ((uint32_t)0x00000100U)         /* !< One BCLK periods - I2S and DSP
                                                                                    formats */
#define I2S_FMTCFG_DATADLY_TWO                   ((uint32_t)0x00000200U)         /* !< Two(Min) BCLK periods - RJF format */
#define I2S_FMTCFG_DATADLY_MAX                   ((uint32_t)0x0000FF00U)         /* !< Max(255) BCLK periods - RJF format */
/* I2S_FMTCFG[SUSPEND] Bits */
#define I2S_FMTCFG_SUSPEND_OFS                   (24)                            /* !< SUSPEND Offset */
#define I2S_FMTCFG_SUSPEND_MASK                  ((uint32_t)0x01000000U)         /* !< Suspend external communication */
#define I2S_FMTCFG_SUSPEND_DISABLE               ((uint32_t)0x00000000U)         /* !< Functional mode resumed */
#define I2S_FMTCFG_SUSPEND_ENABLE                ((uint32_t)0x01000000U)         /* !< External communication suspended */
/* I2S_FMTCFG[ENABLE] Bits */
#define I2S_FMTCFG_ENABLE_OFS                    (31)                            /* !< ENABLE Offset */
#define I2S_FMTCFG_ENABLE_MASK                   ((uint32_t)0x80000000U)         /* !< I2S Module Enable.       If the I2S
                                                                                    is disabled in the middle of
                                                                                    transmission or reception, it
                                                                                    completes the current character
                                                                                    before stopping.      If ENABLE bit
                                                                                    is not set, all registers can still
                                                                                    be accessed and updated. It is
                                                                                    recommended to setup and change I2S
                                                                                    operation mode whhile ENABLE bit is
                                                                                    cleared to avoid unpredictable
                                                                                    behavior during setup or update.
                                                                                    If disabled,  I2S module will not
                                                                                    send or receive any data. */
#define I2S_FMTCFG_ENABLE_DISABLE                ((uint32_t)0x00000000U)         /* !< Disable Module */
#define I2S_FMTCFG_ENABLE_ENABLE                 ((uint32_t)0x80000000U)         /* !< Enable module */
/* I2S_FMTCFG[EMPTYSLOTOUTPUT] Bits */
#define I2S_FMTCFG_EMPTYSLOTOUTPUT_OFS           (16)                            /* !< EMPTYSLOTOUTPUT Offset */
#define I2S_FMTCFG_EMPTYSLOTOUTPUT_MASK          ((uint32_t)0x00030000U)         /* !< The field configures the
                                                                                    EMPTYSLOTOUTPUT data pin direction */
#define I2S_FMTCFG_EMPTYSLOTOUTPUT_ZERO          ((uint32_t)0x00000000U)         /* !< Send out zeroes in empty slots */
#define I2S_FMTCFG_EMPTYSLOTOUTPUT_ONE           ((uint32_t)0x00010000U)         /* !< send out ones in empty slot */
#define I2S_FMTCFG_EMPTYSLOTOUTPUT_TRISTATE      ((uint32_t)0x00020000U)         /* !< Tristate line during empty slot */

/* I2S_CLKCTL Bits */
/* I2S_CLKCTL[WBEN] Bits */
#define I2S_CLKCTL_WBEN_OFS                      (0)                             /* !< WBEN Offset */
#define I2S_CLKCTL_WBEN_MASK                     ((uint32_t)0x00000001U)         /* !< This field configures WCLK/BCLK
                                                                                    generation */
#define I2S_CLKCTL_WBEN_DIS                      ((uint32_t)0x00000000U)         /* !< Disables the generation */
#define I2S_CLKCTL_WBEN_EN                       ((uint32_t)0x00000001U)         /* !< Enable the generation */
/* I2S_CLKCTL[WCLKPHASE] Bits */
#define I2S_CLKCTL_WCLKPHASE_OFS                 (1)                             /* !< WCLKPHASE Offset */
#define I2S_CLKCTL_WCLKPHASE_MASK                ((uint32_t)0x00000006U)         /* !< The field configures how the WCLK
                                                                                    division ratio is calculated and used
                                                                                    to generate different duty cycles
                                                                                    (See [WCLKDIV.WDIV]) */
/* I2S_CLKCTL[MEN] Bits */
#define I2S_CLKCTL_MEN_OFS                       (3)                             /* !< MEN Offset */
#define I2S_CLKCTL_MEN_MASK                      ((uint32_t)0x00000008U)         /* !< This field configures the MCLK
                                                                                    generation */
#define I2S_CLKCTL_MEN_DIS                       ((uint32_t)0x00000000U)         /* !< Disable the generation */
#define I2S_CLKCTL_MEN_EN                        ((uint32_t)0x00000008U)         /* !< Enable the generation */

/* I2S_STAT Bits */
/* I2S_STAT[TXFF] Bits */
#define I2S_STAT_TXFF_OFS                        (6)                             /* !< TXFF Offset */
#define I2S_STAT_TXFF_MASK                       ((uint32_t)0x00000040U)         /* !< Transmit FIFO Full  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define I2S_STAT_TXFF_CLEARED                    ((uint32_t)0x00000000U)         /* !< The transmitter is not full. */
#define I2S_STAT_TXFF_SET                        ((uint32_t)0x00000040U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the transmit holding register is
                                                                                    full. If the FIFO is enabled (FEN is
                                                                                    1), the transmit FIFO is full. */
/* I2S_STAT[RXFF] Bits */
#define I2S_STAT_RXFF_OFS                        (3)                             /* !< RXFF Offset */
#define I2S_STAT_RXFF_MASK                       ((uint32_t)0x00000008U)         /* !< Receive FIFO Full  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define I2S_STAT_RXFF_CLEARED                    ((uint32_t)0x00000000U)         /* !< The receiver can receive data. */
#define I2S_STAT_RXFF_SET                        ((uint32_t)0x00000008U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the receive holding register is full.
                                                                                    If the FIFO is enabled (FEN is 1),
                                                                                    the receive FIFO is full. */
/* I2S_STAT[TXFE] Bits */
#define I2S_STAT_TXFE_OFS                        (5)                             /* !< TXFE Offset */
#define I2S_STAT_TXFE_MASK                       ((uint32_t)0x00000020U)         /* !< Transmit FIFO Empty  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define I2S_STAT_TXFE_CLEARED                    ((uint32_t)0x00000000U)         /* !< The transmitter has data to
                                                                                    transmit. */
#define I2S_STAT_TXFE_SET                        ((uint32_t)0x00000020U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the transmit holding register is
                                                                                    empty. If the FIFO is enabled (FEN is
                                                                                    1), the transmit FIFO is empty. */
/* I2S_STAT[RXFE] Bits */
#define I2S_STAT_RXFE_OFS                        (2)                             /* !< RXFE Offset */
#define I2S_STAT_RXFE_MASK                       ((uint32_t)0x00000004U)         /* !< Receive FIFO Empty  The meaning of
                                                                                    this bit depends on the state of the
                                                                                    FEN bit in the CTL0 register. */
#define I2S_STAT_RXFE_CLEARED                    ((uint32_t)0x00000000U)         /* !< The receiver is not empty. */
#define I2S_STAT_RXFE_SET                        ((uint32_t)0x00000004U)         /* !< If the FIFO is disabled (FEN is 0),
                                                                                    the receive holding register is
                                                                                    empty. If the FIFO is enabled (FEN is
                                                                                    1), the receive FIFO is empty. */
/* I2S_STAT[RXCLR] Bits */
#define I2S_STAT_RXCLR_OFS                       (4)                             /* !< RXCLR Offset */
#define I2S_STAT_RXCLR_MASK                      ((uint32_t)0x00000010U)         /* !< RX FIFO Clear Status */
#define I2S_STAT_RXCLR_CLEARED                   ((uint32_t)0x00000000U)         /* !< FIFO is not cleared */
#define I2S_STAT_RXCLR_SET                       ((uint32_t)0x00000010U)         /* !< FIFO clear is complete */
/* I2S_STAT[TXCLR] Bits */
#define I2S_STAT_TXCLR_OFS                       (7)                             /* !< TXCLR Offset */
#define I2S_STAT_TXCLR_MASK                      ((uint32_t)0x00000080U)         /* !< TX FIFO Clear Status */
#define I2S_STAT_TXCLR_CLEARED                   ((uint32_t)0x00000000U)         /* !< FIFO is not cleared */
#define I2S_STAT_TXCLR_SET                       ((uint32_t)0x00000080U)         /* !< FIFO clear is complete */

/* I2S_IFLS Bits */
/* I2S_IFLS[TXIFLSEL] Bits */
#define I2S_IFLS_TXIFLSEL_OFS                    (0)                             /* !< TXIFLSEL Offset */
#define I2S_IFLS_TXIFLSEL_MASK                   ((uint32_t)0x00000007U)         /* !< FIFO Level Select for generating
                                                                                    events (interrupt/dma).  Note: for
                                                                                    undefined settings the default
                                                                                    configuration is used. */
#define I2S_IFLS_TXIFLSEL_LVL_3_4                ((uint32_t)0x00000001U)         /* !< TX FIFO <= 3/4 empty */
#define I2S_IFLS_TXIFLSEL_LVL_1_2                ((uint32_t)0x00000002U)         /* !< TX FIFO <= 1/2 empty (default) */
#define I2S_IFLS_TXIFLSEL_LVL_1_4                ((uint32_t)0x00000003U)         /* !< TX FIFO <= 1/4 empty */
#define I2S_IFLS_TXIFLSEL_LVL_EMPTY              ((uint32_t)0x00000005U)         /* !< TX FIFO is empty */
#define I2S_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY       ((uint32_t)0x00000006U)         /* !< TX FIFO <= 1 */
#define I2S_IFLS_TXIFLSEL_LVL_ALMOST_FULL        ((uint32_t)0x00000007U)         /* !< TX_FIFO >= (MAX_FIFO_LEN -1) */
/* I2S_IFLS[RXIFLSEL] Bits */
#define I2S_IFLS_RXIFLSEL_OFS                    (4)                             /* !< RXIFLSEL Offset */
#define I2S_IFLS_RXIFLSEL_MASK                   ((uint32_t)0x00000070U)         /* !< Receive Interrupt FIFO Level Select
                                                                                    The trigger points for the receive
                                                                                    interrupt are as follows:   Note:
                                                                                    In ULP domain the trigger levels are
                                                                                    used for:   0: LVL_1_4   4: LVL_FULL
                                                                                    For undefined settings the default
                                                                                    configuration is used. */
#define I2S_IFLS_RXIFLSEL_LVL_1_4                ((uint32_t)0x00000010U)         /* !< RX FIFO >= 1/4 full */
#define I2S_IFLS_RXIFLSEL_LVL_1_2                ((uint32_t)0x00000020U)         /* !< RX FIFO >= 1/2 full (default) */
#define I2S_IFLS_RXIFLSEL_LVL_3_4                ((uint32_t)0x00000030U)         /* !< RX FIFO >= 3/4 full */
#define I2S_IFLS_RXIFLSEL_LVL_FULL               ((uint32_t)0x00000050U)         /* !< RX FIFO is full */
#define I2S_IFLS_RXIFLSEL_LVL_ALMOST_FULL        ((uint32_t)0x00000060U)         /* !< RX_FIFO >= (MAX_FIFO_LEN -1) */
#define I2S_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY       ((uint32_t)0x00000070U)         /* !< RX_FIFO <= 1 */
/* I2S_IFLS[TXCLR] Bits */
#define I2S_IFLS_TXCLR_OFS                       (3)                             /* !< TXCLR Offset */
#define I2S_IFLS_TXCLR_MASK                      ((uint32_t)0x00000008U)         /* !< TX FIFO CLEAR. Setting this bit
                                                                                    will clear the TX FIFO contents. */
#define I2S_IFLS_TXCLR_DISABLE                   ((uint32_t)0x00000000U)         /* !< Disable FIFO clear */
#define I2S_IFLS_TXCLR_ENABLE                    ((uint32_t)0x00000008U)         /* !< Enable FIFO Clear */
/* I2S_IFLS[RXCLR] Bits */
#define I2S_IFLS_RXCLR_OFS                       (7)                             /* !< RXCLR Offset */
#define I2S_IFLS_RXCLR_MASK                      ((uint32_t)0x00000080U)         /* !< RX FIFO CLEAR. Setting this bit
                                                                                    will clear the RX FIFO contents. */
#define I2S_IFLS_RXCLR_DISABLE                   ((uint32_t)0x00000000U)         /* !< Disable FIFO clear */
#define I2S_IFLS_RXCLR_ENABLE                    ((uint32_t)0x00000080U)         /* !< Enable FIFO Clear */

/* I2S_WCLKSRC Bits */
/* I2S_WCLKSRC[WBCLKSRC] Bits */
#define I2S_WCLKSRC_WBCLKSRC_OFS                 (0)                             /* !< WBCLKSRC Offset */
#define I2S_WCLKSRC_WBCLKSRC_MASK                ((uint32_t)0x00000003U)         /* !< This field selects WCLK/BCLK source
                                                                                    for I2S. */
#define I2S_WCLKSRC_WBCLKSRC_NONE                ((uint32_t)0x00000000U)         /* !< None ('0') */
#define I2S_WCLKSRC_WBCLKSRC_EXT                 ((uint32_t)0x00000001U)         /* !< External WCLK generator, from pad */
#define I2S_WCLKSRC_WBCLKSRC_INT                 ((uint32_t)0x00000002U)         /* !< Internal WCLK generator, from
                                                                                    module PRCM */
/* I2S_WCLKSRC[WCLKINV] Bits */
#define I2S_WCLKSRC_WCLKINV_OFS                  (2)                             /* !< WCLKINV Offset */
#define I2S_WCLKSRC_WCLKINV_MASK                 ((uint32_t)0x00000004U)         /* !< This field Inverts WCLK source (pad
                                                                                    or internal). */
#define I2S_WCLKSRC_WCLKINV_NOT_INVERTED         ((uint32_t)0x00000000U)         /* !< Source is not intverted */
#define I2S_WCLKSRC_WCLKINV_INVERTED             ((uint32_t)0x00000004U)         /* !< Source is inverted */

/* I2S_DIRCFG Bits */
/* I2S_DIRCFG[AD0] Bits */
#define I2S_DIRCFG_AD0_OFS                       (0)                             /* !< AD0 Offset */
#define I2S_DIRCFG_AD0_MASK                      ((uint32_t)0x00000003U)         /* !< The field configures the AD0 data
                                                                                    pin direction */
#define I2S_DIRCFG_AD0_DIS                       ((uint32_t)0x00000000U)         /* !< Not in use (disabled) */
#define I2S_DIRCFG_AD0_IN                        ((uint32_t)0x00000001U)         /* !< Input */
#define I2S_DIRCFG_AD0_OUT                       ((uint32_t)0x00000002U)         /* !< Output */
/* I2S_DIRCFG[AD1] Bits */
#define I2S_DIRCFG_AD1_OFS                       (4)                             /* !< AD1 Offset */
#define I2S_DIRCFG_AD1_MASK                      ((uint32_t)0x00000030U)         /* !< The field configures the AD1 data
                                                                                    pin direction */
#define I2S_DIRCFG_AD1_DIS                       ((uint32_t)0x00000000U)         /* !< Not in use (disabled) */
#define I2S_DIRCFG_AD1_IN                        ((uint32_t)0x00000010U)         /* !< Input */
#define I2S_DIRCFG_AD1_OUT                       ((uint32_t)0x00000020U)         /* !< Output */

/* I2S_TXDATA Bits */
/* I2S_TXDATA[DATA] Bits */
#define I2S_TXDATA_DATA_OFS                      (0)                             /* !< DATA Offset */
#define I2S_TXDATA_DATA_MASK                     ((uint32_t)0xFFFFFFFFU)         /* !< Data Transmitted or Received  Data
                                                                                    that is to be transmitted via the  is
                                                                                    written to this field.  When read,
                                                                                    this field contains the data that was
                                                                                    received by the . */
#define I2S_TXDATA_DATA_MINIMUM                  ((uint32_t)0x00000000U)         /* !< Smallest value */
#define I2S_TXDATA_DATA_MAXIMUM                  ((uint32_t)0xFFFFFFFFU)         /* !< Highest possible value */

/* I2S_RXDATA Bits */
/* I2S_RXDATA[DATA] Bits */
#define I2S_RXDATA_DATA_OFS                      (0)                             /* !< DATA Offset */
#define I2S_RXDATA_DATA_MASK                     ((uint32_t)0xFFFFFFFFU)         /* !< Received Data.  When read, this
                                                                                    field contains the data that was
                                                                                    received by the . */
#define I2S_RXDATA_DATA_MINIMUM                  ((uint32_t)0x00000000U)         /* !< Smallest value */
#define I2S_RXDATA_DATA_MAXIMUM                  ((uint32_t)0xFFFFFFFFU)         /* !< Highest possible value */

/* I2S_WMASK0 Bits */
/* I2S_WMASK0[MASK] Bits */
#define I2S_WMASK0_MASK_OFS                      (0)                             /* !< MASK Offset */
#define I2S_WMASK0_MASK_MASK                     ((uint32_t)0x000000FFU)         /* !< Bit-mask indicating valid channels
                                                                                    in a frame on AD0.  In single-phase
                                                                                    mode, each bit represents one
                                                                                    channel, starting with LSB for the
                                                                                    first word in the frame. A frame can
                                                                                    contain up to 8/16 channels based on
                                                                                    SoC configuration.  Channels that are
                                                                                    not included in the mask will not be
                                                                                    sampled nor stored in memory; data
                                                                                    clocked out in these slots is as per
                                                                                    configured EMPTYSLOTOUTPUT.  In
                                                                                    dual-phase mode, only the two LSBs
                                                                                    are considered. For a stereo
                                                                                    configuration, set both bits. For a
                                                                                    mono configuration, set bit 0 only.
                                                                                    In mono mode, only channel 0 will be
                                                                                    sampled and stored to memory, and
                                                                                    channel 0 will be repeated when
                                                                                    clocked out.  In mono mode, only
                                                                                    channel 0 will be sampled and stored
                                                                                    to memory, and channel 0 will be
                                                                                    repeated in the second phase when
                                                                                    clocked out.  If all bits are zero,
                                                                                    no input words will be stored to
                                                                                    memory, and the output data lines
                                                                                    will be constant '0'. This can be
                                                                                    utilized when PWM debug output is
                                                                                    desired without any actively used
                                                                                    output pins. */

/* I2S_WMASK1 Bits */
/* I2S_WMASK1[MASK] Bits */
#define I2S_WMASK1_MASK_OFS                      (0)                             /* !< MASK Offset */
#define I2S_WMASK1_MASK_MASK                     ((uint32_t)0x000000FFU)         /* !< Bit-mask indicating valid channels
                                                                                    in a frame on AD1.  In single-phase
                                                                                    mode, each bit represents one
                                                                                    channel, starting with LSB for the
                                                                                    first word in the frame. A frame can
                                                                                    contain up to 8/16 channels based on
                                                                                    SoC configuration.  Channels that are
                                                                                    not included in the mask will not be
                                                                                    sampled nor stored in memory; data
                                                                                    clocked out in these slots is as per
                                                                                    configured EMPTYSLOTOUTPUT.  In
                                                                                    dual-phase mode, only the two LSBs
                                                                                    are considered. For a stereo
                                                                                    configuration, set both bits. For a
                                                                                    mono configuration, set bit 0 only.
                                                                                    In mono mode, only channel 0 will be
                                                                                    sampled and stored to memory, and
                                                                                    channel 0 will be repeated when
                                                                                    clocked out.  In mono mode, only
                                                                                    channel 0 will be sampled and stored
                                                                                    to memory, and channel 0 will be
                                                                                    repeated in the second phase when
                                                                                    clocked out.  If all bits are zero,
                                                                                    no input words will be stored to
                                                                                    memory, and the output data lines
                                                                                    will be constant '0'. This can be
                                                                                    utilized when PWM debug output is
                                                                                    desired without any actively used
                                                                                    output pins. */

/* I2S_MCLKDIV Bits */
/* I2S_MCLKDIV[MDIV] Bits */
#define I2S_MCLKDIV_MDIV_OFS                     (0)                             /* !< MDIV Offset */
#define I2S_MCLKDIV_MDIV_MASK                    ((uint32_t)0x000003FFU)         /* !< An unsigned factor of the division
                                                                                    ratio used to generate MCLK [2-1024]:
                                                                                    
                                                                                    MCLK = MCUCLK/MDIV[Hz]
                                                                                    
                                                                                    A value of 0 is interpreted as 1024.
                                                                                    
                                                                                    A value of 1 is invalid.
                                                                                    
                                                                                    If MDIV is odd the low phase of the
                                                                                    clock is one MCUCLK period longer
                                                                                    than the high phase. */

/* I2S_WCLKDIV Bits */
/* I2S_WCLKDIV[WDIV] Bits */
#define I2S_WCLKDIV_WDIV_OFS                     (0)                             /* !< WDIV Offset */
#define I2S_WCLKDIV_WDIV_MASK                    ((uint32_t)0x0000FFFFU)         /* !< If [CLKCTL.WCLKPHASE] = 0, Single
                                                                                    phase. WCLK is high one BCLK period
                                                                                    and low WDIV[9:0] (unsigned,
                                                                                    [1-1023]) BCLK periods.
                                                                                    WCLK =
                                                                                    MCUCLK / BDIV*(WDIV[9:0] + 1) [Hz]
                                                                                    
                                                                                    If [CLKCTL.WCLKPHASE.*] = 1, Dual
                                                                                    phase. Each phase on WCLK (50% duty
                                                                                    cycle) is WDIV[9:0] (unsigned,
                                                                                    [1-1023]) BCLK periods.
                                                                                    WCLK =
                                                                                    MCUCLK / BDIV*(2*WDIV[9:0]) [Hz]
                                                                                    
                                                                                    If [CLKCTL.WCLKPHASE.*] = 2, User
                                                                                    defined. WCLK is high WDIV[7:0]
                                                                                    (unsigned, [1-255]) BCLK periods and
                                                                                    low WDIV[15:8] (unsigned, [1-255])
                                                                                    BCLK periods.
                                                                                    WCLK = MCUCLK /
                                                                                    (BDIV*(WDIV[7:0] + WDIV[15:8]) [Hz] */

/* I2S_BCLKDIV Bits */
/* I2S_BCLKDIV[BDIV] Bits */
#define I2S_BCLKDIV_BDIV_OFS                     (0)                             /* !< BDIV Offset */
#define I2S_BCLKDIV_BDIV_MASK                    ((uint32_t)0x000003FFU)         /* !< An unsigned factor of the division
                                                                                    ratio used to generate BCLK [2-1024]:
                                                                                    
                                                                                    BCLK = MCUCLK/BDIV[Hz]
                                                                                    A
                                                                                    value of 0 is interpreted as 1024.
                                                                                    
                                                                                    A value of 1 is invalid.
                                                                                    If
                                                                                    BDIV is odd and [FMTCFG.SMPLEDGE] =
                                                                                    0, the low phase of the clock is one
                                                                                    MCUCLK period longer than the high
                                                                                    phase.
                                                                                    If BDIV is odd and
                                                                                    [FMTCFG.SMPLEDGE] = 1 , the high
                                                                                    phase of the clock is one MCUCLK
                                                                                    period longer than the low phase. */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_i2s__include */
