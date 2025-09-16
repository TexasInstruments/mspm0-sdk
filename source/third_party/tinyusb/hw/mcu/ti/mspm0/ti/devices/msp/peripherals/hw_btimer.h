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

#ifndef ti_devices_msp_peripherals_hw_btimer__include
#define ti_devices_msp_peripherals_hw_btimer__include

/* Filename: hw_btimer.h */
/* Revised: 2024-06-05 04:16:28 */
/* Revision: 952104399038670370f108bbde01f8fcea266891 */

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
* BTIMER Registers
******************************************************************************/
#define BTIMER_CTRREGS_OFS                       ((uint32_t)0x00001100U)
#define BTIMER_GEN_EVENT_OFS                     ((uint32_t)0x00001050U)
#define BTIMER_CPU_INT_OFS                       ((uint32_t)0x00001020U)
#define BTIMER_GPRCM_OFS                         ((uint32_t)0x00000800U)


/** @addtogroup BTIMER_CTRREGS
  @{
*/

typedef struct {
  __IO uint32_t CTL0;                              /* !< (@ 0x00001100) CTL0 register */
  __IO uint32_t LD;                                /* !< (@ 0x00001104) Counter period register. */
  __IO uint32_t CNT;                               /* !< (@ 0x00001108) Counter register. */
       uint32_t RESERVED0[61];
} BTIMER_CTRREGS_Regs;

/*@}*/ /* end of group BTIMER_CTRREGS */

/** @addtogroup BTIMER_GEN_EVENT
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001050) Interrupt index */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001058) Interrupt Mask. If a bit is set, then
                                                      corresponding interrupt is un-masked. Un-masking the interrupt
                                                      causes the raw interrupt to be visible in IIDX, as well as MIS. */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001060) Raw interrupt status. Reflects all pending
                                                      interrupts, regardless of masking. The RIS register allows the user
                                                      to implement a poll scheme. A flag set in this register can be
                                                      cleared by writing 1 to the ICLR register bit even if the
                                                      corresponding IMASK bit is not enabled. */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001068) Masked interrupt status. This is an AND of the
                                                      IMASK and RIS registers. */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001070) Interrupt set. Allows interrupts to be set by
                                                      software (useful in diagnostics and safety checks). Writing a 1 to
                                                      a bit in ISET will set the event and therefore the related RIS bit
                                                      also gets set. If the interrupt is enabled through the mask, then
                                                      the corresponding MIS bit is also set. */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001078) Interrupt clear. Write a 1 to clear corresponding
                                                      Interrupt. */
} BTIMER_GEN_EVENT_Regs;

/*@}*/ /* end of group BTIMER_GEN_EVENT */

/** @addtogroup BTIMER_CPU_INT
  @{
*/

typedef struct {
  __I  uint32_t IIDX;                              /* !< (@ 0x00001020) Interrupt index */
       uint32_t RESERVED0;
  __IO uint32_t IMASK;                             /* !< (@ 0x00001028) Interrupt Mask. If a bit is set, then
                                                      corresponding interrupt is un-masked. Un-masking the interrupt
                                                      causes the raw interrupt to be visible in IIDX, as well as MIS. */
       uint32_t RESERVED1;
  __I  uint32_t RIS;                               /* !< (@ 0x00001030) Raw interrupt status. Reflects all pending
                                                      interrupts, regardless of masking. The RIS register allows the user
                                                      to implement a poll scheme. A flag set in this register can be
                                                      cleared by writing 1 to the ICLR register bit even if the
                                                      corresponding IMASK bit is not enabled. */
       uint32_t RESERVED2;
  __I  uint32_t MIS;                               /* !< (@ 0x00001038) Masked interrupt status. This is an AND of the
                                                      IMASK and RIS registers. */
       uint32_t RESERVED3;
  __O  uint32_t ISET;                              /* !< (@ 0x00001040) Interrupt set. Allows interrupts to be set by
                                                      software (useful in diagnostics and safety checks). Writing a 1 to
                                                      a bit in ISET will set the event and therefore the related RIS bit
                                                      also gets set. If the interrupt is enabled through the mask, then
                                                      the corresponding MIS bit is also set. */
       uint32_t RESERVED4;
  __O  uint32_t ICLR;                              /* !< (@ 0x00001048) Interrupt clear. Write a 1 to clear corresponding
                                                      Interrupt. */
} BTIMER_CPU_INT_Regs;

/*@}*/ /* end of group BTIMER_CPU_INT */

/** @addtogroup BTIMER_GPRCM
  @{
*/

typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) Power enable */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) Reset Control */
       uint32_t RESERVED0[3];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) Status Register */
} BTIMER_GPRCM_Regs;

/*@}*/ /* end of group BTIMER_GPRCM */

/** @addtogroup BTIMER
  @{
*/

typedef struct {
       uint32_t RESERVED0[256];
  __IO uint32_t FSUB_0;                            /* !< (@ 0x00000400) Subscriber Configuration Register. */
       uint32_t RESERVED1[16];
  __IO uint32_t FPUB_0;                            /* !< (@ 0x00000444) Publisher Port 0 */
       uint32_t RESERVED2[238];
  BTIMER_GPRCM_Regs  GPRCM;                             /* !< (@ 0x00000800) */
       uint32_t RESERVED3[512];
  __IO uint32_t PDBGCTL;                           /* !< (@ 0x00001018) Peripheral Debug Control */
       uint32_t RESERVED4;
  BTIMER_CPU_INT_Regs  CPU_INT;                           /* !< (@ 0x00001020) */
       uint32_t RESERVED5;
  BTIMER_GEN_EVENT_Regs  GEN_EVENT;                         /* !< (@ 0x00001050) */
       uint32_t RESERVED6[25];
  __I  uint32_t EVT_MODE;                          /* !< (@ 0x000010E0) Event Mode */
       uint32_t RESERVED7[7];
  BTIMER_CTRREGS_Regs  CTRREGS[2];                        /* !< (@ 0x00001100) */
} BTIMER_Regs;

/*@}*/ /* end of group BTIMER */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* BTIMER Register Control Bits
******************************************************************************/

/* BTIMER_CTL0 Bits */
/* BTIMER_CTL0[EN] Bits */
#define BTIMER_CTL0_EN_OFS                       (0)                             /* !< EN Offset */
#define BTIMER_CTL0_EN_MASK                      ((uint32_t)0x00000001U)         /* !< Counter Enable */
/* BTIMER_CTL0[STARTSEL] Bits */
#define BTIMER_CTL0_STARTSEL_OFS                 (4)                             /* !< STARTSEL Offset */
#define BTIMER_CTL0_STARTSEL_MASK                ((uint32_t)0x000000F0U)         /* !< Selects the source used to start
                                                                                    the counter. */
/* BTIMER_CTL0[STOPSEL] Bits */
#define BTIMER_CTL0_STOPSEL_OFS                  (8)                             /* !< STOPSEL Offset */
#define BTIMER_CTL0_STOPSEL_MASK                 ((uint32_t)0x00000F00U)         /* !< Selects the source used to stop the
                                                                                    counter. */
/* BTIMER_CTL0[RESETSEL] Bits */
#define BTIMER_CTL0_RESETSEL_OFS                 (12)                            /* !< RESETSEL Offset */
#define BTIMER_CTL0_RESETSEL_MASK                ((uint32_t)0x0000F000U)         /* !< Selects the source used to reset
                                                                                    the counter. */
/* BTIMER_CTL0[CLKSEL] Bits */
#define BTIMER_CTL0_CLKSEL_OFS                   (16)                            /* !< CLKSEL Offset */
#define BTIMER_CTL0_CLKSEL_MASK                  ((uint32_t)0x000F0000U)         /* !< Clock sources */

/* BTIMER_LD Bits */
/* BTIMER_LD[VAL] Bits */
#define BTIMER_LD_VAL_OFS                        (0)                             /* !< VAL Offset */
#define BTIMER_LD_VAL_MASK                       ((uint32_t)0x0000FFFFU)         /* !< Period value */
#define BTIMER_LD_VAL_MINIMUM                    ((uint32_t)0x00000001U)         /* !< Minimum counter value */
#define BTIMER_LD_VAL_MAXIMUM                    ((uint32_t)0x0000FFFFU)         /* !< Maximum counter value */

/* BTIMER_CNT Bits */
/* BTIMER_CNT[VALUE] Bits */
#define BTIMER_CNT_VALUE_OFS                     (0)                             /* !< VALUE Offset */
#define BTIMER_CNT_VALUE_MASK                    ((uint32_t)0x0000FFFFU)         /* !< Counter Value */

/* BTIMER_GEN_EVENT_IIDX Bits */
/* BTIMER_GEN_EVENT_IIDX[STAT] Bits */
#define BTIMER_GEN_EVENT_IIDX_STAT_OFS           (0)                             /* !< STAT Offset */
#define BTIMER_GEN_EVENT_IIDX_STAT_MASK          ((uint32_t)0x000003FFU)         /* !< Interrupt index status */
#define BTIMER_GEN_EVENT_IIDX_STAT_NO_INTR       ((uint32_t)0x00000000U)         /* !< No bit is set means there is no
                                                                                    pending interrupt request */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT0OVF       ((uint32_t)0x00000001U)         /* !< Counter0 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT0STRT      ((uint32_t)0x00000002U)         /* !< Counter 0 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT0STOP      ((uint32_t)0x00000003U)         /* !< Counter0 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT1OVF       ((uint32_t)0x00000004U)         /* !< Counter1 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT1STRT      ((uint32_t)0x00000005U)         /* !< Counter 1 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT1STOP      ((uint32_t)0x00000006U)         /* !< Counter1 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT2OVF       ((uint32_t)0x00000007U)         /* !< Counter2 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT2STRT      ((uint32_t)0x00000008U)         /* !< Counter 2 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT2STOP      ((uint32_t)0x00000009U)         /* !< Counter2 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT3OVF       ((uint32_t)0x0000000AU)         /* !< Counter3 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT3STRT      ((uint32_t)0x0000000BU)         /* !< Counter 3 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT3STOP      ((uint32_t)0x0000000CU)         /* !< Counter3 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT4OVF       ((uint32_t)0x0000000DU)         /* !< Counter4 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT4STRT      ((uint32_t)0x0000000EU)         /* !< Counter 4 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT4STOP      ((uint32_t)0x0000000FU)         /* !< Counter4 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT5OVF       ((uint32_t)0x00000010U)         /* !< Counter5 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT5STRT      ((uint32_t)0x00000011U)         /* !< Counter 5 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT5STOP      ((uint32_t)0x00000012U)         /* !< Counter5 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT6OVF       ((uint32_t)0x00000013U)         /* !< Counter6 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT6STRT      ((uint32_t)0x00000014U)         /* !< Counter 6 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT6STOP      ((uint32_t)0x00000015U)         /* !< Counter6 stop */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT7OVF       ((uint32_t)0x00000016U)         /* !< Counter7 period match  interrupt */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT7STRT      ((uint32_t)0x00000017U)         /* !< Counter 7 start */
#define BTIMER_GEN_EVENT_IIDX_STAT_CNT7STOP      ((uint32_t)0x00000018U)         /* !< Counter7 stop */

/* BTIMER_GEN_EVENT_IMASK Bits */
/* BTIMER_GEN_EVENT_IMASK[CNT0OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT0OVF_OFS       (0)                             /* !< CNT0OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT0OVF_MASK      ((uint32_t)0x00000001U)         /* !< CNT0OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT0OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT0OVF_SET       ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT0STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT0STRT_OFS      (1)                             /* !< CNT0STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT0STRT_MASK     ((uint32_t)0x00000002U)         /* !< CNT0STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT0STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT0STRT_SET      ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT0STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT0STOP_OFS      (2)                             /* !< CNT0STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT0STOP_MASK     ((uint32_t)0x00000004U)         /* !< CNT0STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT0STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT0STOP_SET      ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT1OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT1OVF_OFS       (3)                             /* !< CNT1OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT1OVF_MASK      ((uint32_t)0x00000008U)         /* !< CNT1OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT1OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT1OVF_SET       ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT1STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT1STRT_OFS      (4)                             /* !< CNT1STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT1STRT_MASK     ((uint32_t)0x00000010U)         /* !< CNT1STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT1STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT1STRT_SET      ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT1STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT1STOP_OFS      (5)                             /* !< CNT1STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT1STOP_MASK     ((uint32_t)0x00000020U)         /* !< CNT1STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT1STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT1STOP_SET      ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT2OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT2OVF_OFS       (6)                             /* !< CNT2OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT2OVF_MASK      ((uint32_t)0x00000040U)         /* !< CNT2OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT2OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT2OVF_SET       ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT2STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT2STRT_OFS      (7)                             /* !< CNT2STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT2STRT_MASK     ((uint32_t)0x00000080U)         /* !< CNT2STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT2STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT2STRT_SET      ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT2STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT2STOP_OFS      (8)                             /* !< CNT2STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT2STOP_MASK     ((uint32_t)0x00000100U)         /* !< CNT2STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT2STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT2STOP_SET      ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT3OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT3OVF_OFS       (9)                             /* !< CNT3OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT3OVF_MASK      ((uint32_t)0x00000200U)         /* !< CNT3OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT3OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT3OVF_SET       ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT3STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT3STRT_OFS      (10)                            /* !< CNT3STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT3STRT_MASK     ((uint32_t)0x00000400U)         /* !< CNT3STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT3STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT3STRT_SET      ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT3STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT3STOP_OFS      (11)                            /* !< CNT3STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT3STOP_MASK     ((uint32_t)0x00000800U)         /* !< CNT3STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT3STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT3STOP_SET      ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT4OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT4OVF_OFS       (12)                            /* !< CNT4OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT4OVF_MASK      ((uint32_t)0x00001000U)         /* !< CNT4OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT4OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT4OVF_SET       ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT4STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT4STRT_OFS      (13)                            /* !< CNT4STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT4STRT_MASK     ((uint32_t)0x00002000U)         /* !< CNT4STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT4STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT4STRT_SET      ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT4STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT4STOP_OFS      (14)                            /* !< CNT4STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT4STOP_MASK     ((uint32_t)0x00004000U)         /* !< CNT4STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT4STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT4STOP_SET      ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT5OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT5OVF_OFS       (15)                            /* !< CNT5OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT5OVF_MASK      ((uint32_t)0x00008000U)         /* !< CNT5OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT5OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT5OVF_SET       ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT5STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT5STRT_OFS      (16)                            /* !< CNT5STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT5STRT_MASK     ((uint32_t)0x00010000U)         /* !< CNT5STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT5STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT5STRT_SET      ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT5STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT5STOP_OFS      (17)                            /* !< CNT5STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT5STOP_MASK     ((uint32_t)0x00020000U)         /* !< CNT5STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT5STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT5STOP_SET      ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT6OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT6OVF_OFS       (18)                            /* !< CNT6OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT6OVF_MASK      ((uint32_t)0x00040000U)         /* !< CNT6OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT6OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT6OVF_SET       ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT6STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT6STRT_OFS      (19)                            /* !< CNT6STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT6STRT_MASK     ((uint32_t)0x00080000U)         /* !< CNT6STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT6STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT6STRT_SET      ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT6STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT6STOP_OFS      (20)                            /* !< CNT6STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT6STOP_MASK     ((uint32_t)0x00100000U)         /* !< CNT6STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT6STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT6STOP_SET      ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT7OVF] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT7OVF_OFS       (21)                            /* !< CNT7OVF Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT7OVF_MASK      ((uint32_t)0x00200000U)         /* !< CNT7OVF Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT7OVF_CLR       ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT7OVF_SET       ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT7STRT] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT7STRT_OFS      (22)                            /* !< CNT7STRT Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT7STRT_MASK     ((uint32_t)0x00400000U)         /* !< CNT7STRT Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT7STRT_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT7STRT_SET      ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_IMASK[CNT7STOP] Bits */
#define BTIMER_GEN_EVENT_IMASK_CNT7STOP_OFS      (23)                            /* !< CNT7STOP Offset */
#define BTIMER_GEN_EVENT_IMASK_CNT7STOP_MASK     ((uint32_t)0x00800000U)         /* !< CNT7STOP Interrupt enable bit */
#define BTIMER_GEN_EVENT_IMASK_CNT7STOP_CLR      ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_IMASK_CNT7STOP_SET      ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_GEN_EVENT_RIS Bits */
/* BTIMER_GEN_EVENT_RIS[CNT0OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT0OVF_OFS         (0)                             /* !< CNT0OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT0OVF_MASK        ((uint32_t)0x00000001U)         /* !< Counter 0 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT0OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT0OVF_SET         ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT0STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT0STRT_OFS        (1)                             /* !< CNT0STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT0STRT_MASK       ((uint32_t)0x00000002U)         /* !< Counter 0 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT0STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT0STRT_SET        ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT0STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT0STOP_OFS        (2)                             /* !< CNT0STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT0STOP_MASK       ((uint32_t)0x00000004U)         /* !< Counter 0 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT0STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT0STOP_SET        ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT1OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT1OVF_OFS         (3)                             /* !< CNT1OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT1OVF_MASK        ((uint32_t)0x00000008U)         /* !< Counter 1 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT1OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT1OVF_SET         ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT1STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT1STRT_OFS        (4)                             /* !< CNT1STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT1STRT_MASK       ((uint32_t)0x00000010U)         /* !< Counter 1 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT1STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT1STRT_SET        ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT1STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT1STOP_OFS        (5)                             /* !< CNT1STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT1STOP_MASK       ((uint32_t)0x00000020U)         /* !< Counter 1 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT1STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT1STOP_SET        ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT2OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT2OVF_OFS         (6)                             /* !< CNT2OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT2OVF_MASK        ((uint32_t)0x00000040U)         /* !< Counter 2 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT2OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT2OVF_SET         ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT2STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT2STRT_OFS        (7)                             /* !< CNT2STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT2STRT_MASK       ((uint32_t)0x00000080U)         /* !< Counter 2 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT2STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT2STRT_SET        ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT2STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT2STOP_OFS        (8)                             /* !< CNT2STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT2STOP_MASK       ((uint32_t)0x00000100U)         /* !< Counter 2 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT2STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT2STOP_SET        ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT3OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT3OVF_OFS         (9)                             /* !< CNT3OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT3OVF_MASK        ((uint32_t)0x00000200U)         /* !< Counter 3 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT3OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT3OVF_SET         ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT3STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT3STRT_OFS        (10)                            /* !< CNT3STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT3STRT_MASK       ((uint32_t)0x00000400U)         /* !< Counter 3 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT3STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT3STRT_SET        ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT3STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT3STOP_OFS        (11)                            /* !< CNT3STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT3STOP_MASK       ((uint32_t)0x00000800U)         /* !< Counter 3 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT3STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT3STOP_SET        ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT4OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT4OVF_OFS         (12)                            /* !< CNT4OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT4OVF_MASK        ((uint32_t)0x00001000U)         /* !< Counter 4 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT4OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT4OVF_SET         ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT4STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT4STRT_OFS        (13)                            /* !< CNT4STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT4STRT_MASK       ((uint32_t)0x00002000U)         /* !< Counter 4 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT4STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT4STRT_SET        ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT4STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT4STOP_OFS        (14)                            /* !< CNT4STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT4STOP_MASK       ((uint32_t)0x00004000U)         /* !< Counter 4 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT4STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT4STOP_SET        ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT5OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT5OVF_OFS         (15)                            /* !< CNT5OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT5OVF_MASK        ((uint32_t)0x00008000U)         /* !< Counter 5 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT5OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT5OVF_SET         ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT5STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT5STRT_OFS        (16)                            /* !< CNT5STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT5STRT_MASK       ((uint32_t)0x00010000U)         /* !< Counter 5 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT5STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT5STRT_SET        ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT5STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT5STOP_OFS        (17)                            /* !< CNT5STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT5STOP_MASK       ((uint32_t)0x00020000U)         /* !< Counter 5 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT5STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT5STOP_SET        ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT6OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT6OVF_OFS         (18)                            /* !< CNT6OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT6OVF_MASK        ((uint32_t)0x00040000U)         /* !< Counter 6 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT6OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT6OVF_SET         ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT6STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT6STRT_OFS        (19)                            /* !< CNT6STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT6STRT_MASK       ((uint32_t)0x00080000U)         /* !< Counter 6 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT6STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT6STRT_SET        ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT6STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT6STOP_OFS        (20)                            /* !< CNT6STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT6STOP_MASK       ((uint32_t)0x00100000U)         /* !< Counter 6 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT6STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT6STOP_SET        ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT7OVF] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT7OVF_OFS         (21)                            /* !< CNT7OVF Offset */
#define BTIMER_GEN_EVENT_RIS_CNT7OVF_MASK        ((uint32_t)0x00200000U)         /* !< Counter 7 Period match flag */
#define BTIMER_GEN_EVENT_RIS_CNT7OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT7OVF_SET         ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT7STRT] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT7STRT_OFS        (22)                            /* !< CNT7STRT Offset */
#define BTIMER_GEN_EVENT_RIS_CNT7STRT_MASK       ((uint32_t)0x00400000U)         /* !< Counter 7 start event flag */
#define BTIMER_GEN_EVENT_RIS_CNT7STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT7STRT_SET        ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_RIS[CNT7STOP] Bits */
#define BTIMER_GEN_EVENT_RIS_CNT7STOP_OFS        (23)                            /* !< CNT7STOP Offset */
#define BTIMER_GEN_EVENT_RIS_CNT7STOP_MASK       ((uint32_t)0x00800000U)         /* !< Counter 7 stop event flag */
#define BTIMER_GEN_EVENT_RIS_CNT7STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_RIS_CNT7STOP_SET        ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_GEN_EVENT_MIS Bits */
/* BTIMER_GEN_EVENT_MIS[CNT0OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT0OVF_OFS         (0)                             /* !< CNT0OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT0OVF_MASK        ((uint32_t)0x00000001U)         /* !< Counter 0 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT0OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT0OVF_SET         ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT0STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT0STRT_OFS        (1)                             /* !< CNT0STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT0STRT_MASK       ((uint32_t)0x00000002U)         /* !< Counter 0 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT0STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT0STRT_SET        ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT0STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT0STOP_OFS        (2)                             /* !< CNT0STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT0STOP_MASK       ((uint32_t)0x00000004U)         /* !< Counter 0 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT0STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT0STOP_SET        ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT1OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT1OVF_OFS         (3)                             /* !< CNT1OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT1OVF_MASK        ((uint32_t)0x00000008U)         /* !< Counter 1 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT1OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT1OVF_SET         ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT1STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT1STRT_OFS        (4)                             /* !< CNT1STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT1STRT_MASK       ((uint32_t)0x00000010U)         /* !< Counter 1 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT1STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT1STRT_SET        ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT1STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT1STOP_OFS        (5)                             /* !< CNT1STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT1STOP_MASK       ((uint32_t)0x00000020U)         /* !< Counter 1 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT1STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT1STOP_SET        ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT2OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT2OVF_OFS         (6)                             /* !< CNT2OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT2OVF_MASK        ((uint32_t)0x00000040U)         /* !< Counter 2 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT2OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT2OVF_SET         ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT2STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT2STRT_OFS        (7)                             /* !< CNT2STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT2STRT_MASK       ((uint32_t)0x00000080U)         /* !< Counter 2 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT2STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT2STRT_SET        ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT2STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT2STOP_OFS        (8)                             /* !< CNT2STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT2STOP_MASK       ((uint32_t)0x00000100U)         /* !< Counter 2 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT2STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT2STOP_SET        ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT3OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT3OVF_OFS         (9)                             /* !< CNT3OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT3OVF_MASK        ((uint32_t)0x00000200U)         /* !< Counter 3 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT3OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT3OVF_SET         ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT3STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT3STRT_OFS        (10)                            /* !< CNT3STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT3STRT_MASK       ((uint32_t)0x00000400U)         /* !< Counter 3 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT3STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT3STRT_SET        ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT3STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT3STOP_OFS        (11)                            /* !< CNT3STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT3STOP_MASK       ((uint32_t)0x00000800U)         /* !< Counter 3 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT3STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT3STOP_SET        ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT4OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT4OVF_OFS         (12)                            /* !< CNT4OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT4OVF_MASK        ((uint32_t)0x00001000U)         /* !< Counter 4 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT4OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT4OVF_SET         ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT4STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT4STRT_OFS        (13)                            /* !< CNT4STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT4STRT_MASK       ((uint32_t)0x00002000U)         /* !< Counter 4 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT4STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT4STRT_SET        ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT4STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT4STOP_OFS        (14)                            /* !< CNT4STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT4STOP_MASK       ((uint32_t)0x00004000U)         /* !< Counter 4 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT4STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT4STOP_SET        ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT5OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT5OVF_OFS         (15)                            /* !< CNT5OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT5OVF_MASK        ((uint32_t)0x00008000U)         /* !< Counter 5 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT5OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT5OVF_SET         ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT5STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT5STRT_OFS        (16)                            /* !< CNT5STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT5STRT_MASK       ((uint32_t)0x00010000U)         /* !< Counter 5 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT5STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT5STRT_SET        ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT5STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT5STOP_OFS        (17)                            /* !< CNT5STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT5STOP_MASK       ((uint32_t)0x00020000U)         /* !< Counter 5 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT5STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT5STOP_SET        ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT6OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT6OVF_OFS         (18)                            /* !< CNT6OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT6OVF_MASK        ((uint32_t)0x00040000U)         /* !< Counter 6 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT6OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT6OVF_SET         ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT6STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT6STRT_OFS        (19)                            /* !< CNT6STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT6STRT_MASK       ((uint32_t)0x00080000U)         /* !< Counter 6 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT6STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT6STRT_SET        ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT6STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT6STOP_OFS        (20)                            /* !< CNT6STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT6STOP_MASK       ((uint32_t)0x00100000U)         /* !< Counter 6 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT6STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT6STOP_SET        ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT7OVF] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT7OVF_OFS         (21)                            /* !< CNT7OVF Offset */
#define BTIMER_GEN_EVENT_MIS_CNT7OVF_MASK        ((uint32_t)0x00200000U)         /* !< Counter 7 period match flag. */
#define BTIMER_GEN_EVENT_MIS_CNT7OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT7OVF_SET         ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT7STRT] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT7STRT_OFS        (22)                            /* !< CNT7STRT Offset */
#define BTIMER_GEN_EVENT_MIS_CNT7STRT_MASK       ((uint32_t)0x00400000U)         /* !< Counter 7 start event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT7STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT7STRT_SET        ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_MIS[CNT7STOP] Bits */
#define BTIMER_GEN_EVENT_MIS_CNT7STOP_OFS        (23)                            /* !< CNT7STOP Offset */
#define BTIMER_GEN_EVENT_MIS_CNT7STOP_MASK       ((uint32_t)0x00800000U)         /* !< Counter 7 stop event flag. */
#define BTIMER_GEN_EVENT_MIS_CNT7STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_MIS_CNT7STOP_SET        ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_GEN_EVENT_ISET Bits */
/* BTIMER_GEN_EVENT_ISET[CNT0OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT0OVF_OFS        (0)                             /* !< CNT0OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT0OVF_MASK       ((uint32_t)0x00000001U)         /* !< Set Counter 0 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT0OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT0OVF_SET        ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT0STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT0STRT_OFS       (1)                             /* !< CNT0STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT0STRT_MASK      ((uint32_t)0x00000002U)         /* !< Set Counter 0 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT0STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT0STRT_SET       ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT0STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT0STOP_OFS       (2)                             /* !< CNT0STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT0STOP_MASK      ((uint32_t)0x00000004U)         /* !< Set Counter 0 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT0STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT0STOP_SET       ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT1OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT1OVF_OFS        (3)                             /* !< CNT1OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT1OVF_MASK       ((uint32_t)0x00000008U)         /* !< Set Counter 1 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT1OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT1OVF_SET        ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT1STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT1STRT_OFS       (4)                             /* !< CNT1STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT1STRT_MASK      ((uint32_t)0x00000010U)         /* !< Set Counter 1 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT1STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT1STRT_SET       ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT1STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT1STOP_OFS       (5)                             /* !< CNT1STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT1STOP_MASK      ((uint32_t)0x00000020U)         /* !< Set Counter 1 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT1STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT1STOP_SET       ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT2OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT2OVF_OFS        (6)                             /* !< CNT2OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT2OVF_MASK       ((uint32_t)0x00000040U)         /* !< Set Counter 2 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT2OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT2OVF_SET        ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT2STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT2STRT_OFS       (7)                             /* !< CNT2STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT2STRT_MASK      ((uint32_t)0x00000080U)         /* !< Set Counter 2 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT2STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT2STRT_SET       ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT2STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT2STOP_OFS       (8)                             /* !< CNT2STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT2STOP_MASK      ((uint32_t)0x00000100U)         /* !< Set Counter 2 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT2STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT2STOP_SET       ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT3OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT3OVF_OFS        (9)                             /* !< CNT3OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT3OVF_MASK       ((uint32_t)0x00000200U)         /* !< Set Counter 3 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT3OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT3OVF_SET        ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT3STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT3STRT_OFS       (10)                            /* !< CNT3STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT3STRT_MASK      ((uint32_t)0x00000400U)         /* !< Set Counter 3 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT3STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT3STRT_SET       ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT3STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT3STOP_OFS       (11)                            /* !< CNT3STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT3STOP_MASK      ((uint32_t)0x00000800U)         /* !< Set Counter 3 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT3STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT3STOP_SET       ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT4OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT4OVF_OFS        (12)                            /* !< CNT4OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT4OVF_MASK       ((uint32_t)0x00001000U)         /* !< Set Counter 4 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT4OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT4OVF_SET        ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT4STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT4STRT_OFS       (13)                            /* !< CNT4STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT4STRT_MASK      ((uint32_t)0x00002000U)         /* !< Set Counter 4 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT4STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT4STRT_SET       ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT4STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT4STOP_OFS       (14)                            /* !< CNT4STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT4STOP_MASK      ((uint32_t)0x00004000U)         /* !< Set Counter 4 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT4STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT4STOP_SET       ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT5OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT5OVF_OFS        (15)                            /* !< CNT5OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT5OVF_MASK       ((uint32_t)0x00008000U)         /* !< Set Counter 5 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT5OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT5OVF_SET        ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT5STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT5STRT_OFS       (16)                            /* !< CNT5STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT5STRT_MASK      ((uint32_t)0x00010000U)         /* !< Set Counter 5 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT5STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT5STRT_SET       ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT5STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT5STOP_OFS       (17)                            /* !< CNT5STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT5STOP_MASK      ((uint32_t)0x00020000U)         /* !< Set Counter 5 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT5STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT5STOP_SET       ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT6OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT6OVF_OFS        (18)                            /* !< CNT6OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT6OVF_MASK       ((uint32_t)0x00040000U)         /* !< Set Counter 6 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT6OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT6OVF_SET        ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT6STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT6STRT_OFS       (19)                            /* !< CNT6STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT6STRT_MASK      ((uint32_t)0x00080000U)         /* !< Set Counter 6 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT6STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT6STRT_SET       ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT6STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT6STOP_OFS       (20)                            /* !< CNT6STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT6STOP_MASK      ((uint32_t)0x00100000U)         /* !< Set Counter 6 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT6STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT6STOP_SET       ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT7OVF] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT7OVF_OFS        (21)                            /* !< CNT7OVF Offset */
#define BTIMER_GEN_EVENT_ISET_CNT7OVF_MASK       ((uint32_t)0x00200000U)         /* !< Set Counter 7 period match flag */
#define BTIMER_GEN_EVENT_ISET_CNT7OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT7OVF_SET        ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT7STRT] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT7STRT_OFS       (22)                            /* !< CNT7STRT Offset */
#define BTIMER_GEN_EVENT_ISET_CNT7STRT_MASK      ((uint32_t)0x00400000U)         /* !< Set Counter 7 start event flag */
#define BTIMER_GEN_EVENT_ISET_CNT7STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT7STRT_SET       ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ISET[CNT7STOP] Bits */
#define BTIMER_GEN_EVENT_ISET_CNT7STOP_OFS       (23)                            /* !< CNT7STOP Offset */
#define BTIMER_GEN_EVENT_ISET_CNT7STOP_MASK      ((uint32_t)0x00800000U)         /* !< Set Counter 7 stop event flag */
#define BTIMER_GEN_EVENT_ISET_CNT7STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ISET_CNT7STOP_SET       ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_GEN_EVENT_ICLR Bits */
/* BTIMER_GEN_EVENT_ICLR[CNT0OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT0OVF_OFS        (0)                             /* !< CNT0OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT0OVF_MASK       ((uint32_t)0x00000001U)         /* !< Clear Counter 0 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT0OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT0OVF_CLR        ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT0STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT0STRT_OFS       (1)                             /* !< CNT0STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT0STRT_MASK      ((uint32_t)0x00000002U)         /* !< Clear Counter 0 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT0STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT0STRT_CLR       ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT0STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT0STOP_OFS       (2)                             /* !< CNT0STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT0STOP_MASK      ((uint32_t)0x00000004U)         /* !< Clear Counter 0 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT0STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT0STOP_CLR       ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT1OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT1OVF_OFS        (3)                             /* !< CNT1OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT1OVF_MASK       ((uint32_t)0x00000008U)         /* !< Clear Counter 1 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT1OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT1OVF_CLR        ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT1STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT1STRT_OFS       (4)                             /* !< CNT1STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT1STRT_MASK      ((uint32_t)0x00000010U)         /* !< Clear Counter 1 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT1STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT1STRT_CLR       ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT1STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT1STOP_OFS       (5)                             /* !< CNT1STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT1STOP_MASK      ((uint32_t)0x00000020U)         /* !< Clear Counter 1 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT1STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT1STOP_CLR       ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT2OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT2OVF_OFS        (6)                             /* !< CNT2OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT2OVF_MASK       ((uint32_t)0x00000040U)         /* !< Clear Counter 2 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT2OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT2OVF_CLR        ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT2STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT2STRT_OFS       (7)                             /* !< CNT2STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT2STRT_MASK      ((uint32_t)0x00000080U)         /* !< Clear Counter 2 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT2STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT2STRT_CLR       ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT2STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT2STOP_OFS       (8)                             /* !< CNT2STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT2STOP_MASK      ((uint32_t)0x00000100U)         /* !< Clear Counter 2 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT2STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT2STOP_CLR       ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT3OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT3OVF_OFS        (9)                             /* !< CNT3OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT3OVF_MASK       ((uint32_t)0x00000200U)         /* !< Clear Counter 3 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT3OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT3OVF_CLR        ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT3STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT3STRT_OFS       (10)                            /* !< CNT3STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT3STRT_MASK      ((uint32_t)0x00000400U)         /* !< Clear Counter 3 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT3STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT3STRT_CLR       ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT3STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT3STOP_OFS       (11)                            /* !< CNT3STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT3STOP_MASK      ((uint32_t)0x00000800U)         /* !< Clear Counter 3 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT3STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT3STOP_CLR       ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT4OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT4OVF_OFS        (12)                            /* !< CNT4OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT4OVF_MASK       ((uint32_t)0x00001000U)         /* !< Clear Counter 4 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT4OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT4OVF_CLR        ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT4STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT4STRT_OFS       (13)                            /* !< CNT4STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT4STRT_MASK      ((uint32_t)0x00002000U)         /* !< Clear Counter 4 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT4STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT4STRT_CLR       ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT4STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT4STOP_OFS       (14)                            /* !< CNT4STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT4STOP_MASK      ((uint32_t)0x00004000U)         /* !< Clear Counter 4 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT4STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT4STOP_CLR       ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT5OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT5OVF_OFS        (15)                            /* !< CNT5OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT5OVF_MASK       ((uint32_t)0x00008000U)         /* !< Clear Counter 5 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT5OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT5OVF_CLR        ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT5STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT5STRT_OFS       (16)                            /* !< CNT5STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT5STRT_MASK      ((uint32_t)0x00010000U)         /* !< Clear Counter 5 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT5STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT5STRT_CLR       ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT5STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT5STOP_OFS       (17)                            /* !< CNT5STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT5STOP_MASK      ((uint32_t)0x00020000U)         /* !< Clear Counter 5 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT5STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT5STOP_CLR       ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT6OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT6OVF_OFS        (18)                            /* !< CNT6OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT6OVF_MASK       ((uint32_t)0x00040000U)         /* !< Clear Counter 6 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT6OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT6OVF_CLR        ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT6STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT6STRT_OFS       (19)                            /* !< CNT6STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT6STRT_MASK      ((uint32_t)0x00080000U)         /* !< Clear Counter 6 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT6STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT6STRT_CLR       ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT6STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT6STOP_OFS       (20)                            /* !< CNT6STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT6STOP_MASK      ((uint32_t)0x00100000U)         /* !< Clear Counter 6 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT6STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT6STOP_CLR       ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT7OVF] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT7OVF_OFS        (21)                            /* !< CNT7OVF Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT7OVF_MASK       ((uint32_t)0x00200000U)         /* !< Clear Counter 7 period match flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT7OVF_NO_EFFECT  ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT7OVF_CLR        ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT7STRT] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT7STRT_OFS       (22)                            /* !< CNT7STRT Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT7STRT_MASK      ((uint32_t)0x00400000U)         /* !< Clear Counter 7 start event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT7STRT_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT7STRT_CLR       ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_GEN_EVENT_ICLR[CNT7STOP] Bits */
#define BTIMER_GEN_EVENT_ICLR_CNT7STOP_OFS       (23)                            /* !< CNT7STOP Offset */
#define BTIMER_GEN_EVENT_ICLR_CNT7STOP_MASK      ((uint32_t)0x00800000U)         /* !< Clear Counter 7 stop event flag. */
#define BTIMER_GEN_EVENT_ICLR_CNT7STOP_NO_EFFECT ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_GEN_EVENT_ICLR_CNT7STOP_CLR       ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_CPU_INT_IIDX Bits */
/* BTIMER_CPU_INT_IIDX[STAT] Bits */
#define BTIMER_CPU_INT_IIDX_STAT_OFS             (0)                             /* !< STAT Offset */
#define BTIMER_CPU_INT_IIDX_STAT_MASK            ((uint32_t)0x000003FFU)         /* !< Interrupt index status */
#define BTIMER_CPU_INT_IIDX_STAT_NO_INTR         ((uint32_t)0x00000000U)         /* !< No bit is set means there is no
                                                                                    pending interrupt request */
#define BTIMER_CPU_INT_IIDX_STAT_CNT0OVF         ((uint32_t)0x00000001U)         /* !< Counter0 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT0STRT        ((uint32_t)0x00000002U)         /* !< Counter 0 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT0STOP        ((uint32_t)0x00000003U)         /* !< Counter0 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT1OVF         ((uint32_t)0x00000004U)         /* !< Counter1 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT1STRT        ((uint32_t)0x00000005U)         /* !< Counter 1 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT1STOP        ((uint32_t)0x00000006U)         /* !< Counter1 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT2OVF         ((uint32_t)0x00000007U)         /* !< Counter2 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT2STRT        ((uint32_t)0x00000008U)         /* !< Counter 2 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT2STOP        ((uint32_t)0x00000009U)         /* !< Counter2 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT3OVF         ((uint32_t)0x0000000AU)         /* !< Counter3 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT3STRT        ((uint32_t)0x0000000BU)         /* !< Counter 3 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT3STOP        ((uint32_t)0x0000000CU)         /* !< Counter3 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT4OVF         ((uint32_t)0x0000000DU)         /* !< Counter4 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT4STRT        ((uint32_t)0x0000000EU)         /* !< Counter 4 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT4STOP        ((uint32_t)0x0000000FU)         /* !< Counter4 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT5OVF         ((uint32_t)0x00000010U)         /* !< Counter5 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT5STRT        ((uint32_t)0x00000011U)         /* !< Counter 5 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT5STOP        ((uint32_t)0x00000012U)         /* !< Counter5 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT6OVF         ((uint32_t)0x00000013U)         /* !< Counter6 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT6STRT        ((uint32_t)0x00000014U)         /* !< Counter 6 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT6STOP        ((uint32_t)0x00000015U)         /* !< Counter6 stop */
#define BTIMER_CPU_INT_IIDX_STAT_CNT7OVF         ((uint32_t)0x00000016U)         /* !< Counter7 period match  interrupt */
#define BTIMER_CPU_INT_IIDX_STAT_CNT7STRT        ((uint32_t)0x00000017U)         /* !< Counter 7 start */
#define BTIMER_CPU_INT_IIDX_STAT_CNT7STOP        ((uint32_t)0x00000018U)         /* !< Counter7 stop */

/* BTIMER_CPU_INT_IMASK Bits */
/* BTIMER_CPU_INT_IMASK[CNT0OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT0OVF_OFS         (0)                             /* !< CNT0OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT0OVF_MASK        ((uint32_t)0x00000001U)         /* !< CNT0OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT0OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT0OVF_SET         ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT0STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT0STRT_OFS        (1)                             /* !< CNT0STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT0STRT_MASK       ((uint32_t)0x00000002U)         /* !< CNT0STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT0STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT0STRT_SET        ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT0STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT0STOP_OFS        (2)                             /* !< CNT0STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT0STOP_MASK       ((uint32_t)0x00000004U)         /* !< CNT0STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT0STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT0STOP_SET        ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT1OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT1OVF_OFS         (3)                             /* !< CNT1OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT1OVF_MASK        ((uint32_t)0x00000008U)         /* !< CNT1OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT1OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT1OVF_SET         ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT1STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT1STRT_OFS        (4)                             /* !< CNT1STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT1STRT_MASK       ((uint32_t)0x00000010U)         /* !< CNT1STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT1STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT1STRT_SET        ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT1STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT1STOP_OFS        (5)                             /* !< CNT1STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT1STOP_MASK       ((uint32_t)0x00000020U)         /* !< CNT1STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT1STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT1STOP_SET        ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT2OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT2OVF_OFS         (6)                             /* !< CNT2OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT2OVF_MASK        ((uint32_t)0x00000040U)         /* !< CNT2OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT2OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT2OVF_SET         ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT2STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT2STRT_OFS        (7)                             /* !< CNT2STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT2STRT_MASK       ((uint32_t)0x00000080U)         /* !< CNT2STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT2STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT2STRT_SET        ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT2STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT2STOP_OFS        (8)                             /* !< CNT2STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT2STOP_MASK       ((uint32_t)0x00000100U)         /* !< CNT2STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT2STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT2STOP_SET        ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT3OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT3OVF_OFS         (9)                             /* !< CNT3OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT3OVF_MASK        ((uint32_t)0x00000200U)         /* !< CNT3OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT3OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT3OVF_SET         ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT3STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT3STRT_OFS        (10)                            /* !< CNT3STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT3STRT_MASK       ((uint32_t)0x00000400U)         /* !< CNT3STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT3STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT3STRT_SET        ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT3STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT3STOP_OFS        (11)                            /* !< CNT3STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT3STOP_MASK       ((uint32_t)0x00000800U)         /* !< CNT3STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT3STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT3STOP_SET        ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT4OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT4OVF_OFS         (12)                            /* !< CNT4OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT4OVF_MASK        ((uint32_t)0x00001000U)         /* !< CNT4OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT4OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT4OVF_SET         ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT4STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT4STRT_OFS        (13)                            /* !< CNT4STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT4STRT_MASK       ((uint32_t)0x00002000U)         /* !< CNT4STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT4STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT4STRT_SET        ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT4STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT4STOP_OFS        (14)                            /* !< CNT4STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT4STOP_MASK       ((uint32_t)0x00004000U)         /* !< CNT4STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT4STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT4STOP_SET        ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT5OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT5OVF_OFS         (15)                            /* !< CNT5OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT5OVF_MASK        ((uint32_t)0x00008000U)         /* !< CNT5OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT5OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT5OVF_SET         ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT5STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT5STRT_OFS        (16)                            /* !< CNT5STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT5STRT_MASK       ((uint32_t)0x00010000U)         /* !< CNT5STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT5STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT5STRT_SET        ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT5STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT5STOP_OFS        (17)                            /* !< CNT5STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT5STOP_MASK       ((uint32_t)0x00020000U)         /* !< CNT5STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT5STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT5STOP_SET        ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT6OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT6OVF_OFS         (18)                            /* !< CNT6OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT6OVF_MASK        ((uint32_t)0x00040000U)         /* !< CNT6OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT6OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT6OVF_SET         ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT6STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT6STRT_OFS        (19)                            /* !< CNT6STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT6STRT_MASK       ((uint32_t)0x00080000U)         /* !< CNT6STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT6STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT6STRT_SET        ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT6STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT6STOP_OFS        (20)                            /* !< CNT6STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT6STOP_MASK       ((uint32_t)0x00100000U)         /* !< CNT6STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT6STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT6STOP_SET        ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT7OVF] Bits */
#define BTIMER_CPU_INT_IMASK_CNT7OVF_OFS         (21)                            /* !< CNT7OVF Offset */
#define BTIMER_CPU_INT_IMASK_CNT7OVF_MASK        ((uint32_t)0x00200000U)         /* !< CNT7OVF Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT7OVF_CLR         ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT7OVF_SET         ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT7STRT] Bits */
#define BTIMER_CPU_INT_IMASK_CNT7STRT_OFS        (22)                            /* !< CNT7STRT Offset */
#define BTIMER_CPU_INT_IMASK_CNT7STRT_MASK       ((uint32_t)0x00400000U)         /* !< CNT7STRT Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT7STRT_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT7STRT_SET        ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_IMASK[CNT7STOP] Bits */
#define BTIMER_CPU_INT_IMASK_CNT7STOP_OFS        (23)                            /* !< CNT7STOP Offset */
#define BTIMER_CPU_INT_IMASK_CNT7STOP_MASK       ((uint32_t)0x00800000U)         /* !< CNT7STOP Interrupt enable bit */
#define BTIMER_CPU_INT_IMASK_CNT7STOP_CLR        ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_IMASK_CNT7STOP_SET        ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_CPU_INT_RIS Bits */
/* BTIMER_CPU_INT_RIS[CNT0OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT0OVF_OFS           (0)                             /* !< CNT0OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT0OVF_MASK          ((uint32_t)0x00000001U)         /* !< Counter 0 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT0OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT0OVF_SET           ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT0STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT0STRT_OFS          (1)                             /* !< CNT0STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT0STRT_MASK         ((uint32_t)0x00000002U)         /* !< Counter 0 start event flag */
#define BTIMER_CPU_INT_RIS_CNT0STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT0STRT_SET          ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT0STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT0STOP_OFS          (2)                             /* !< CNT0STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT0STOP_MASK         ((uint32_t)0x00000004U)         /* !< Counter 0 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT0STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT0STOP_SET          ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT1OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT1OVF_OFS           (3)                             /* !< CNT1OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT1OVF_MASK          ((uint32_t)0x00000008U)         /* !< Counter 1 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT1OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT1OVF_SET           ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT1STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT1STRT_OFS          (4)                             /* !< CNT1STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT1STRT_MASK         ((uint32_t)0x00000010U)         /* !< Counter 1 start event flag */
#define BTIMER_CPU_INT_RIS_CNT1STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT1STRT_SET          ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT1STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT1STOP_OFS          (5)                             /* !< CNT1STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT1STOP_MASK         ((uint32_t)0x00000020U)         /* !< Counter 1 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT1STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT1STOP_SET          ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT2OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT2OVF_OFS           (6)                             /* !< CNT2OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT2OVF_MASK          ((uint32_t)0x00000040U)         /* !< Counter 2 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT2OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT2OVF_SET           ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT2STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT2STRT_OFS          (7)                             /* !< CNT2STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT2STRT_MASK         ((uint32_t)0x00000080U)         /* !< Counter 2 start event flag */
#define BTIMER_CPU_INT_RIS_CNT2STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT2STRT_SET          ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT2STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT2STOP_OFS          (8)                             /* !< CNT2STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT2STOP_MASK         ((uint32_t)0x00000100U)         /* !< Counter 2 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT2STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT2STOP_SET          ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT3OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT3OVF_OFS           (9)                             /* !< CNT3OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT3OVF_MASK          ((uint32_t)0x00000200U)         /* !< Counter 3 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT3OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT3OVF_SET           ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT3STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT3STRT_OFS          (10)                            /* !< CNT3STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT3STRT_MASK         ((uint32_t)0x00000400U)         /* !< Counter 3 start event flag */
#define BTIMER_CPU_INT_RIS_CNT3STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT3STRT_SET          ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT3STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT3STOP_OFS          (11)                            /* !< CNT3STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT3STOP_MASK         ((uint32_t)0x00000800U)         /* !< Counter 3 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT3STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT3STOP_SET          ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT4OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT4OVF_OFS           (12)                            /* !< CNT4OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT4OVF_MASK          ((uint32_t)0x00001000U)         /* !< Counter 4 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT4OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT4OVF_SET           ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT4STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT4STRT_OFS          (13)                            /* !< CNT4STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT4STRT_MASK         ((uint32_t)0x00002000U)         /* !< Counter 4 start event flag */
#define BTIMER_CPU_INT_RIS_CNT4STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT4STRT_SET          ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT4STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT4STOP_OFS          (14)                            /* !< CNT4STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT4STOP_MASK         ((uint32_t)0x00004000U)         /* !< Counter 4 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT4STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT4STOP_SET          ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT5OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT5OVF_OFS           (15)                            /* !< CNT5OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT5OVF_MASK          ((uint32_t)0x00008000U)         /* !< Counter 5 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT5OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT5OVF_SET           ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT5STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT5STRT_OFS          (16)                            /* !< CNT5STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT5STRT_MASK         ((uint32_t)0x00010000U)         /* !< Counter 5 start event flag */
#define BTIMER_CPU_INT_RIS_CNT5STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT5STRT_SET          ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT5STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT5STOP_OFS          (17)                            /* !< CNT5STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT5STOP_MASK         ((uint32_t)0x00020000U)         /* !< Counter 5 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT5STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT5STOP_SET          ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT6OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT6OVF_OFS           (18)                            /* !< CNT6OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT6OVF_MASK          ((uint32_t)0x00040000U)         /* !< Counter 6 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT6OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT6OVF_SET           ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT6STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT6STRT_OFS          (19)                            /* !< CNT6STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT6STRT_MASK         ((uint32_t)0x00080000U)         /* !< Counter 6 start event flag */
#define BTIMER_CPU_INT_RIS_CNT6STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT6STRT_SET          ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT6STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT6STOP_OFS          (20)                            /* !< CNT6STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT6STOP_MASK         ((uint32_t)0x00100000U)         /* !< Counter 6 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT6STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT6STOP_SET          ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT7OVF] Bits */
#define BTIMER_CPU_INT_RIS_CNT7OVF_OFS           (21)                            /* !< CNT7OVF Offset */
#define BTIMER_CPU_INT_RIS_CNT7OVF_MASK          ((uint32_t)0x00200000U)         /* !< Counter 7 Period match flag */
#define BTIMER_CPU_INT_RIS_CNT7OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT7OVF_SET           ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT7STRT] Bits */
#define BTIMER_CPU_INT_RIS_CNT7STRT_OFS          (22)                            /* !< CNT7STRT Offset */
#define BTIMER_CPU_INT_RIS_CNT7STRT_MASK         ((uint32_t)0x00400000U)         /* !< Counter 7 start event flag */
#define BTIMER_CPU_INT_RIS_CNT7STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT7STRT_SET          ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_RIS[CNT7STOP] Bits */
#define BTIMER_CPU_INT_RIS_CNT7STOP_OFS          (23)                            /* !< CNT7STOP Offset */
#define BTIMER_CPU_INT_RIS_CNT7STOP_MASK         ((uint32_t)0x00800000U)         /* !< Counter 7 stop event flag */
#define BTIMER_CPU_INT_RIS_CNT7STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_RIS_CNT7STOP_SET          ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_CPU_INT_MIS Bits */
/* BTIMER_CPU_INT_MIS[CNT0OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT0OVF_OFS           (0)                             /* !< CNT0OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT0OVF_MASK          ((uint32_t)0x00000001U)         /* !< Counter 0 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT0OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT0OVF_SET           ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT0STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT0STRT_OFS          (1)                             /* !< CNT0STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT0STRT_MASK         ((uint32_t)0x00000002U)         /* !< Counter 0 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT0STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT0STRT_SET          ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT0STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT0STOP_OFS          (2)                             /* !< CNT0STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT0STOP_MASK         ((uint32_t)0x00000004U)         /* !< Counter 0 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT0STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT0STOP_SET          ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT1OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT1OVF_OFS           (3)                             /* !< CNT1OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT1OVF_MASK          ((uint32_t)0x00000008U)         /* !< Counter 1 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT1OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT1OVF_SET           ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT1STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT1STRT_OFS          (4)                             /* !< CNT1STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT1STRT_MASK         ((uint32_t)0x00000010U)         /* !< Counter 1 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT1STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT1STRT_SET          ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT1STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT1STOP_OFS          (5)                             /* !< CNT1STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT1STOP_MASK         ((uint32_t)0x00000020U)         /* !< Counter 1 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT1STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT1STOP_SET          ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT2OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT2OVF_OFS           (6)                             /* !< CNT2OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT2OVF_MASK          ((uint32_t)0x00000040U)         /* !< Counter 2 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT2OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT2OVF_SET           ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT2STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT2STRT_OFS          (7)                             /* !< CNT2STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT2STRT_MASK         ((uint32_t)0x00000080U)         /* !< Counter 2 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT2STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT2STRT_SET          ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT2STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT2STOP_OFS          (8)                             /* !< CNT2STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT2STOP_MASK         ((uint32_t)0x00000100U)         /* !< Counter 2 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT2STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT2STOP_SET          ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT3OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT3OVF_OFS           (9)                             /* !< CNT3OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT3OVF_MASK          ((uint32_t)0x00000200U)         /* !< Counter 3 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT3OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT3OVF_SET           ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT3STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT3STRT_OFS          (10)                            /* !< CNT3STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT3STRT_MASK         ((uint32_t)0x00000400U)         /* !< Counter 3 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT3STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT3STRT_SET          ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT3STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT3STOP_OFS          (11)                            /* !< CNT3STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT3STOP_MASK         ((uint32_t)0x00000800U)         /* !< Counter 3 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT3STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT3STOP_SET          ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT4OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT4OVF_OFS           (12)                            /* !< CNT4OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT4OVF_MASK          ((uint32_t)0x00001000U)         /* !< Counter 4 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT4OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT4OVF_SET           ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT4STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT4STRT_OFS          (13)                            /* !< CNT4STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT4STRT_MASK         ((uint32_t)0x00002000U)         /* !< Counter 4 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT4STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT4STRT_SET          ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT4STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT4STOP_OFS          (14)                            /* !< CNT4STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT4STOP_MASK         ((uint32_t)0x00004000U)         /* !< Counter 4 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT4STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT4STOP_SET          ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT5OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT5OVF_OFS           (15)                            /* !< CNT5OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT5OVF_MASK          ((uint32_t)0x00008000U)         /* !< Counter 5 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT5OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT5OVF_SET           ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT5STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT5STRT_OFS          (16)                            /* !< CNT5STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT5STRT_MASK         ((uint32_t)0x00010000U)         /* !< Counter 5 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT5STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT5STRT_SET          ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT5STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT5STOP_OFS          (17)                            /* !< CNT5STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT5STOP_MASK         ((uint32_t)0x00020000U)         /* !< Counter 5 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT5STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT5STOP_SET          ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT6OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT6OVF_OFS           (18)                            /* !< CNT6OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT6OVF_MASK          ((uint32_t)0x00040000U)         /* !< Counter 6 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT6OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT6OVF_SET           ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT6STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT6STRT_OFS          (19)                            /* !< CNT6STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT6STRT_MASK         ((uint32_t)0x00080000U)         /* !< Counter 6 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT6STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT6STRT_SET          ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT6STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT6STOP_OFS          (20)                            /* !< CNT6STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT6STOP_MASK         ((uint32_t)0x00100000U)         /* !< Counter 6 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT6STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT6STOP_SET          ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT7OVF] Bits */
#define BTIMER_CPU_INT_MIS_CNT7OVF_OFS           (21)                            /* !< CNT7OVF Offset */
#define BTIMER_CPU_INT_MIS_CNT7OVF_MASK          ((uint32_t)0x00200000U)         /* !< Counter 7 period match flag. */
#define BTIMER_CPU_INT_MIS_CNT7OVF_CLR           ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT7OVF_SET           ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT7STRT] Bits */
#define BTIMER_CPU_INT_MIS_CNT7STRT_OFS          (22)                            /* !< CNT7STRT Offset */
#define BTIMER_CPU_INT_MIS_CNT7STRT_MASK         ((uint32_t)0x00400000U)         /* !< Counter 7 start event flag. */
#define BTIMER_CPU_INT_MIS_CNT7STRT_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT7STRT_SET          ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_MIS[CNT7STOP] Bits */
#define BTIMER_CPU_INT_MIS_CNT7STOP_OFS          (23)                            /* !< CNT7STOP Offset */
#define BTIMER_CPU_INT_MIS_CNT7STOP_MASK         ((uint32_t)0x00800000U)         /* !< Counter 7 stop event flag. */
#define BTIMER_CPU_INT_MIS_CNT7STOP_CLR          ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_MIS_CNT7STOP_SET          ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_CPU_INT_ISET Bits */
/* BTIMER_CPU_INT_ISET[CNT0OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT0OVF_OFS          (0)                             /* !< CNT0OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT0OVF_MASK         ((uint32_t)0x00000001U)         /* !< Set Counter 0 period match flag */
#define BTIMER_CPU_INT_ISET_CNT0OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT0OVF_SET          ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT0STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT0STRT_OFS         (1)                             /* !< CNT0STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT0STRT_MASK        ((uint32_t)0x00000002U)         /* !< Set Counter 0 start event flag */
#define BTIMER_CPU_INT_ISET_CNT0STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT0STRT_SET         ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT0STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT0STOP_OFS         (2)                             /* !< CNT0STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT0STOP_MASK        ((uint32_t)0x00000004U)         /* !< Set Counter 0 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT0STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT0STOP_SET         ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT1OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT1OVF_OFS          (3)                             /* !< CNT1OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT1OVF_MASK         ((uint32_t)0x00000008U)         /* !< Set Counter 1 period match flag */
#define BTIMER_CPU_INT_ISET_CNT1OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT1OVF_SET          ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT1STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT1STRT_OFS         (4)                             /* !< CNT1STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT1STRT_MASK        ((uint32_t)0x00000010U)         /* !< Set Counter 1 start event flag */
#define BTIMER_CPU_INT_ISET_CNT1STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT1STRT_SET         ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT1STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT1STOP_OFS         (5)                             /* !< CNT1STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT1STOP_MASK        ((uint32_t)0x00000020U)         /* !< Set Counter 1 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT1STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT1STOP_SET         ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT2OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT2OVF_OFS          (6)                             /* !< CNT2OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT2OVF_MASK         ((uint32_t)0x00000040U)         /* !< Set Counter 2 period match flag */
#define BTIMER_CPU_INT_ISET_CNT2OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT2OVF_SET          ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT2STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT2STRT_OFS         (7)                             /* !< CNT2STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT2STRT_MASK        ((uint32_t)0x00000080U)         /* !< Set Counter 2 start event flag */
#define BTIMER_CPU_INT_ISET_CNT2STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT2STRT_SET         ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT2STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT2STOP_OFS         (8)                             /* !< CNT2STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT2STOP_MASK        ((uint32_t)0x00000100U)         /* !< Set Counter 2 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT2STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT2STOP_SET         ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT3OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT3OVF_OFS          (9)                             /* !< CNT3OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT3OVF_MASK         ((uint32_t)0x00000200U)         /* !< Set Counter 3 period match flag */
#define BTIMER_CPU_INT_ISET_CNT3OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT3OVF_SET          ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT3STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT3STRT_OFS         (10)                            /* !< CNT3STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT3STRT_MASK        ((uint32_t)0x00000400U)         /* !< Set Counter 3 start event flag */
#define BTIMER_CPU_INT_ISET_CNT3STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT3STRT_SET         ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT3STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT3STOP_OFS         (11)                            /* !< CNT3STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT3STOP_MASK        ((uint32_t)0x00000800U)         /* !< Set Counter 3 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT3STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT3STOP_SET         ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT4OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT4OVF_OFS          (12)                            /* !< CNT4OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT4OVF_MASK         ((uint32_t)0x00001000U)         /* !< Set Counter 4 period match flag */
#define BTIMER_CPU_INT_ISET_CNT4OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT4OVF_SET          ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT4STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT4STRT_OFS         (13)                            /* !< CNT4STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT4STRT_MASK        ((uint32_t)0x00002000U)         /* !< Set Counter 4 start event flag */
#define BTIMER_CPU_INT_ISET_CNT4STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT4STRT_SET         ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT4STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT4STOP_OFS         (14)                            /* !< CNT4STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT4STOP_MASK        ((uint32_t)0x00004000U)         /* !< Set Counter 4 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT4STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT4STOP_SET         ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT5OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT5OVF_OFS          (15)                            /* !< CNT5OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT5OVF_MASK         ((uint32_t)0x00008000U)         /* !< Set Counter 5 period match flag */
#define BTIMER_CPU_INT_ISET_CNT5OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT5OVF_SET          ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT5STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT5STRT_OFS         (16)                            /* !< CNT5STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT5STRT_MASK        ((uint32_t)0x00010000U)         /* !< Set Counter 5 start event flag */
#define BTIMER_CPU_INT_ISET_CNT5STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT5STRT_SET         ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT5STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT5STOP_OFS         (17)                            /* !< CNT5STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT5STOP_MASK        ((uint32_t)0x00020000U)         /* !< Set Counter 5 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT5STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT5STOP_SET         ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT6OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT6OVF_OFS          (18)                            /* !< CNT6OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT6OVF_MASK         ((uint32_t)0x00040000U)         /* !< Set Counter 6 period match flag */
#define BTIMER_CPU_INT_ISET_CNT6OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT6OVF_SET          ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT6STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT6STRT_OFS         (19)                            /* !< CNT6STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT6STRT_MASK        ((uint32_t)0x00080000U)         /* !< Set Counter 6 start event flag */
#define BTIMER_CPU_INT_ISET_CNT6STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT6STRT_SET         ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT6STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT6STOP_OFS         (20)                            /* !< CNT6STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT6STOP_MASK        ((uint32_t)0x00100000U)         /* !< Set Counter 6 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT6STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT6STOP_SET         ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT7OVF] Bits */
#define BTIMER_CPU_INT_ISET_CNT7OVF_OFS          (21)                            /* !< CNT7OVF Offset */
#define BTIMER_CPU_INT_ISET_CNT7OVF_MASK         ((uint32_t)0x00200000U)         /* !< Set Counter 7 period match flag */
#define BTIMER_CPU_INT_ISET_CNT7OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT7OVF_SET          ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT7STRT] Bits */
#define BTIMER_CPU_INT_ISET_CNT7STRT_OFS         (22)                            /* !< CNT7STRT Offset */
#define BTIMER_CPU_INT_ISET_CNT7STRT_MASK        ((uint32_t)0x00400000U)         /* !< Set Counter 7 start event flag */
#define BTIMER_CPU_INT_ISET_CNT7STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT7STRT_SET         ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ISET[CNT7STOP] Bits */
#define BTIMER_CPU_INT_ISET_CNT7STOP_OFS         (23)                            /* !< CNT7STOP Offset */
#define BTIMER_CPU_INT_ISET_CNT7STOP_MASK        ((uint32_t)0x00800000U)         /* !< Set Counter 7 stop event flag */
#define BTIMER_CPU_INT_ISET_CNT7STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ISET_CNT7STOP_SET         ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_CPU_INT_ICLR Bits */
/* BTIMER_CPU_INT_ICLR[CNT0OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT0OVF_OFS          (0)                             /* !< CNT0OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT0OVF_MASK         ((uint32_t)0x00000001U)         /* !< Clear Counter 0 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT0OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT0OVF_CLR          ((uint32_t)0x00000001U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT0STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT0STRT_OFS         (1)                             /* !< CNT0STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT0STRT_MASK        ((uint32_t)0x00000002U)         /* !< Clear Counter 0 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT0STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT0STRT_CLR         ((uint32_t)0x00000002U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT0STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT0STOP_OFS         (2)                             /* !< CNT0STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT0STOP_MASK        ((uint32_t)0x00000004U)         /* !< Clear Counter 0 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT0STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT0STOP_CLR         ((uint32_t)0x00000004U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT1OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT1OVF_OFS          (3)                             /* !< CNT1OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT1OVF_MASK         ((uint32_t)0x00000008U)         /* !< Clear Counter 1 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT1OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT1OVF_CLR          ((uint32_t)0x00000008U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT1STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT1STRT_OFS         (4)                             /* !< CNT1STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT1STRT_MASK        ((uint32_t)0x00000010U)         /* !< Clear Counter 1 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT1STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT1STRT_CLR         ((uint32_t)0x00000010U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT1STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT1STOP_OFS         (5)                             /* !< CNT1STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT1STOP_MASK        ((uint32_t)0x00000020U)         /* !< Clear Counter 1 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT1STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT1STOP_CLR         ((uint32_t)0x00000020U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT2OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT2OVF_OFS          (6)                             /* !< CNT2OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT2OVF_MASK         ((uint32_t)0x00000040U)         /* !< Clear Counter 2 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT2OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT2OVF_CLR          ((uint32_t)0x00000040U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT2STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT2STRT_OFS         (7)                             /* !< CNT2STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT2STRT_MASK        ((uint32_t)0x00000080U)         /* !< Clear Counter 2 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT2STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT2STRT_CLR         ((uint32_t)0x00000080U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT2STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT2STOP_OFS         (8)                             /* !< CNT2STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT2STOP_MASK        ((uint32_t)0x00000100U)         /* !< Clear Counter 2 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT2STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT2STOP_CLR         ((uint32_t)0x00000100U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT3OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT3OVF_OFS          (9)                             /* !< CNT3OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT3OVF_MASK         ((uint32_t)0x00000200U)         /* !< Clear Counter 3 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT3OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT3OVF_CLR          ((uint32_t)0x00000200U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT3STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT3STRT_OFS         (10)                            /* !< CNT3STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT3STRT_MASK        ((uint32_t)0x00000400U)         /* !< Clear Counter 3 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT3STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT3STRT_CLR         ((uint32_t)0x00000400U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT3STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT3STOP_OFS         (11)                            /* !< CNT3STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT3STOP_MASK        ((uint32_t)0x00000800U)         /* !< Clear Counter 3 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT3STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT3STOP_CLR         ((uint32_t)0x00000800U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT4OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT4OVF_OFS          (12)                            /* !< CNT4OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT4OVF_MASK         ((uint32_t)0x00001000U)         /* !< Clear Counter 4 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT4OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT4OVF_CLR          ((uint32_t)0x00001000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT4STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT4STRT_OFS         (13)                            /* !< CNT4STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT4STRT_MASK        ((uint32_t)0x00002000U)         /* !< Clear Counter 4 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT4STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT4STRT_CLR         ((uint32_t)0x00002000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT4STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT4STOP_OFS         (14)                            /* !< CNT4STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT4STOP_MASK        ((uint32_t)0x00004000U)         /* !< Clear Counter 4 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT4STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT4STOP_CLR         ((uint32_t)0x00004000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT5OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT5OVF_OFS          (15)                            /* !< CNT5OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT5OVF_MASK         ((uint32_t)0x00008000U)         /* !< Clear Counter 5 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT5OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT5OVF_CLR          ((uint32_t)0x00008000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT5STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT5STRT_OFS         (16)                            /* !< CNT5STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT5STRT_MASK        ((uint32_t)0x00010000U)         /* !< Clear Counter 5 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT5STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT5STRT_CLR         ((uint32_t)0x00010000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT5STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT5STOP_OFS         (17)                            /* !< CNT5STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT5STOP_MASK        ((uint32_t)0x00020000U)         /* !< Clear Counter 5 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT5STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT5STOP_CLR         ((uint32_t)0x00020000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT6OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT6OVF_OFS          (18)                            /* !< CNT6OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT6OVF_MASK         ((uint32_t)0x00040000U)         /* !< Clear Counter 6 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT6OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT6OVF_CLR          ((uint32_t)0x00040000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT6STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT6STRT_OFS         (19)                            /* !< CNT6STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT6STRT_MASK        ((uint32_t)0x00080000U)         /* !< Clear Counter 6 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT6STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT6STRT_CLR         ((uint32_t)0x00080000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT6STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT6STOP_OFS         (20)                            /* !< CNT6STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT6STOP_MASK        ((uint32_t)0x00100000U)         /* !< Clear Counter 6 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT6STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT6STOP_CLR         ((uint32_t)0x00100000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT7OVF] Bits */
#define BTIMER_CPU_INT_ICLR_CNT7OVF_OFS          (21)                            /* !< CNT7OVF Offset */
#define BTIMER_CPU_INT_ICLR_CNT7OVF_MASK         ((uint32_t)0x00200000U)         /* !< Clear Counter 7 period match flag. */
#define BTIMER_CPU_INT_ICLR_CNT7OVF_NO_EFFECT    ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT7OVF_CLR          ((uint32_t)0x00200000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT7STRT] Bits */
#define BTIMER_CPU_INT_ICLR_CNT7STRT_OFS         (22)                            /* !< CNT7STRT Offset */
#define BTIMER_CPU_INT_ICLR_CNT7STRT_MASK        ((uint32_t)0x00400000U)         /* !< Clear Counter 7 start event flag. */
#define BTIMER_CPU_INT_ICLR_CNT7STRT_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT7STRT_CLR         ((uint32_t)0x00400000U)         /* !< Interrupt is pending. */
/* BTIMER_CPU_INT_ICLR[CNT7STOP] Bits */
#define BTIMER_CPU_INT_ICLR_CNT7STOP_OFS         (23)                            /* !< CNT7STOP Offset */
#define BTIMER_CPU_INT_ICLR_CNT7STOP_MASK        ((uint32_t)0x00800000U)         /* !< Clear Counter 7 stop event flag. */
#define BTIMER_CPU_INT_ICLR_CNT7STOP_NO_EFFECT   ((uint32_t)0x00000000U)         /* !< Interrupt is not pending. */
#define BTIMER_CPU_INT_ICLR_CNT7STOP_CLR         ((uint32_t)0x00800000U)         /* !< Interrupt is pending. */

/* BTIMER_PWREN Bits */
/* BTIMER_PWREN[ENABLE] Bits */
#define BTIMER_PWREN_ENABLE_OFS                  (0)                             /* !< ENABLE Offset */
#define BTIMER_PWREN_ENABLE_MASK                 ((uint32_t)0x00000001U)         /* !< Enable the power */
#define BTIMER_PWREN_ENABLE_DISABLE              ((uint32_t)0x00000000U)         /* !< Disable Power */
#define BTIMER_PWREN_ENABLE_ENABLE               ((uint32_t)0x00000001U)         /* !< Enable Power */
/* BTIMER_PWREN[KEY] Bits */
#define BTIMER_PWREN_KEY_OFS                     (24)                            /* !< KEY Offset */
#define BTIMER_PWREN_KEY_MASK                    ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define BTIMER_PWREN_KEY_UNLOCK_W                ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* BTIMER_RSTCTL Bits */
/* BTIMER_RSTCTL[RESETSTKYCLR] Bits */
#define BTIMER_RSTCTL_RESETSTKYCLR_OFS           (1)                             /* !< RESETSTKYCLR Offset */
#define BTIMER_RSTCTL_RESETSTKYCLR_MASK          ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define BTIMER_RSTCTL_RESETSTKYCLR_NOP           ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define BTIMER_RSTCTL_RESETSTKYCLR_CLR           ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* BTIMER_RSTCTL[RESETASSERT] Bits */
#define BTIMER_RSTCTL_RESETASSERT_OFS            (0)                             /* !< RESETASSERT Offset */
#define BTIMER_RSTCTL_RESETASSERT_MASK           ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define BTIMER_RSTCTL_RESETASSERT_NOP            ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define BTIMER_RSTCTL_RESETASSERT_ASSERT         ((uint32_t)0x00000001U)         /* !< Assert reset */
/* BTIMER_RSTCTL[KEY] Bits */
#define BTIMER_RSTCTL_KEY_OFS                    (24)                            /* !< KEY Offset */
#define BTIMER_RSTCTL_KEY_MASK                   ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define BTIMER_RSTCTL_KEY_UNLOCK_W               ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* BTIMER_STAT Bits */
/* BTIMER_STAT[RESETSTKY] Bits */
#define BTIMER_STAT_RESETSTKY_OFS                (16)                            /* !< RESETSTKY Offset */
#define BTIMER_STAT_RESETSTKY_MASK               ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define BTIMER_STAT_RESETSTKY_NORES              ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define BTIMER_STAT_RESETSTKY_RESET              ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */

/* BTIMER_FSUB_0 Bits */
/* BTIMER_FSUB_0[CHANID] Bits */
#define BTIMER_FSUB_0_CHANID_OFS                 (0)                             /* !< CHANID Offset */
#define BTIMER_FSUB_0_CHANID_MASK                ((uint32_t)0x000000FFU)         /* !< 0 = disconnected. 1-15 = connected
                                                                                    to channelID = CHANID. */
#define BTIMER_FSUB_0_CHANID_MNIMUM              ((uint32_t)0x00000000U)         /* !< 0 is an allowed value, signifying
                                                                                    that the event is unconnected */
#define BTIMER_FSUB_0_CHANID_UNCONNECTED         ((uint32_t)0x00000000U)         /* !< A value of 0 specifies that the
                                                                                    event is not connected */
#define BTIMER_FSUB_0_CHANID_MAXIMUM             ((uint32_t)0x0000000FU)         /* !< Consult your device datasheet as
                                                                                    the actual allowed maximum may be
                                                                                    less than 255. */

/* BTIMER_FPUB_0 Bits */
/* BTIMER_FPUB_0[CHANID] Bits */
#define BTIMER_FPUB_0_CHANID_OFS                 (0)                             /* !< CHANID Offset */
#define BTIMER_FPUB_0_CHANID_MASK                ((uint32_t)0x000000FFU)         /* !< 0 = disconnected. 1-15 = connected
                                                                                    to channelID = CHANID. */
#define BTIMER_FPUB_0_CHANID_MNIMUM              ((uint32_t)0x00000000U)         /* !< 0 is an allowed value, signifying
                                                                                    that the event is unconnected */
#define BTIMER_FPUB_0_CHANID_UNCONNECTED         ((uint32_t)0x00000000U)         /* !< A value of 0 specifies that the
                                                                                    event is not connected */
#define BTIMER_FPUB_0_CHANID_MAXIMUM             ((uint32_t)0x0000000FU)         /* !< Consult your device datasheet as
                                                                                    the actual allowed maximum may be
                                                                                    less than 15. */

/* BTIMER_PDBGCTL Bits */
/* BTIMER_PDBGCTL[FREE] Bits */
#define BTIMER_PDBGCTL_FREE_OFS                  (0)                             /* !< FREE Offset */
#define BTIMER_PDBGCTL_FREE_MASK                 ((uint32_t)0x00000001U)         /* !< Free run control */
#define BTIMER_PDBGCTL_FREE_STOP                 ((uint32_t)0x00000000U)         /* !< The peripheral freezes
                                                                                    functionality while the Core Halted
                                                                                    input is asserted and resumes when it
                                                                                    is deasserted. */
#define BTIMER_PDBGCTL_FREE_RUN                  ((uint32_t)0x00000001U)         /* !< The peripheral ignores the state of
                                                                                    the Core Halted input */
/* BTIMER_PDBGCTL[SOFT] Bits */
#define BTIMER_PDBGCTL_SOFT_OFS                  (1)                             /* !< SOFT Offset */
#define BTIMER_PDBGCTL_SOFT_MASK                 ((uint32_t)0x00000002U)         /* !< Soft halt boundary control. This
                                                                                    function is only available, if [FREE]
                                                                                    is set to 'STOP' */
#define BTIMER_PDBGCTL_SOFT_IMMEDIATE            ((uint32_t)0x00000000U)         /* !< The peripheral will halt
                                                                                    immediately, even if the resultant
                                                                                    state will result in corruption if
                                                                                    the system is restarted */
#define BTIMER_PDBGCTL_SOFT_DELAYED              ((uint32_t)0x00000002U)         /* !< The peripheral blocks the debug
                                                                                    freeze until it has reached a
                                                                                    boundary where it can resume without
                                                                                    corruption */

/* BTIMER_EVT_MODE Bits */
/* BTIMER_EVT_MODE[CPU_INT_CFG] Bits */
#define BTIMER_EVT_MODE_CPU_INT_CFG_OFS          (0)                             /* !< CPU_INT_CFG Offset */
#define BTIMER_EVT_MODE_CPU_INT_CFG_MASK         ((uint32_t)0x00000003U)         /* !< Event line mode select for event
                                                                                    corresponding to CPU_INT */
#define BTIMER_EVT_MODE_CPU_INT_CFG_DISABLE      ((uint32_t)0x00000000U)         /* !< The interrupt or event line is
                                                                                    disabled. */
#define BTIMER_EVT_MODE_CPU_INT_CFG_SOFTWARE     ((uint32_t)0x00000001U)         /* !< The interrupt or event line is in
                                                                                    software mode. Software must clear
                                                                                    the RIS. */
#define BTIMER_EVT_MODE_CPU_INT_CFG_HARDWARE     ((uint32_t)0x00000002U)         /* !< The interrupt or event line is in
                                                                                    hardware mode. The hardware (another
                                                                                    module) clears automatically the
                                                                                    associated RIS flag. */
/* BTIMER_EVT_MODE[GEN_EVENT_CFG] Bits */
#define BTIMER_EVT_MODE_GEN_EVENT_CFG_OFS        (2)                             /* !< GEN_EVENT_CFG Offset */
#define BTIMER_EVT_MODE_GEN_EVENT_CFG_MASK       ((uint32_t)0x0000000CU)         /* !< Event line mode select for event
                                                                                    corresponding to CPU_INT */
#define BTIMER_EVT_MODE_GEN_EVENT_CFG_DISABLE    ((uint32_t)0x00000000U)         /* !< The interrupt or event line is
                                                                                    disabled. */
#define BTIMER_EVT_MODE_GEN_EVENT_CFG_SOFTWARE   ((uint32_t)0x00000004U)         /* !< The interrupt or event line is in
                                                                                    software mode. Software must clear
                                                                                    the RIS. */
#define BTIMER_EVT_MODE_GEN_EVENT_CFG_HARDWARE   ((uint32_t)0x00000008U)         /* !< The interrupt or event line is in
                                                                                    hardware mode. The hardware (another
                                                                                    module) clears automatically the
                                                                                    associated RIS flag. */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_btimer__include */

