/*****************************************************************************

  Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/ 

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

#ifndef ti_devices_msp_peripherals_hw_NPU__include
#define ti_devices_msp_peripherals_hw_NPU__include

/* This preliminary header file does not have a version number */
/* MMR repo: https://bitbucket.itg.ti.com/projects/cmcu_msp65ip/repos/f65mspNPU */
/* MMR revision: 6c55eebd30e67cec8845b250f0f6bee6d28ea20b */
/* Generator revision: b130192d23745dee22048f2279d2827836bc52d4
   (MInT: ec7ec7482a60c6871be32db8b378ec27aa4771f6) */

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
* NPU Registers
******************************************************************************/
#define NPU_REGFILE_OFS                    ((uint32_t)0x00002800U)
#define NPU_INSTMEM_OFS                    ((uint32_t)0x00002000U)
#define NPU_ACCREG_OFS                     ((uint32_t)0x00001C00U)
#define NPU_ADDRREG_OFS                    ((uint32_t)0x00001154U)
#define NPU_LCREG_OFS                      ((uint32_t)0x00001138U)
#define NPU_INT_EVENT_OFS                  ((uint32_t)0x00001020U)
#define NPU_GPRCM_OFS                      ((uint32_t)0x00000800U)


/** @addtogroup NPU_REGFILE
  @{
*/

typedef struct {
  __IO uint32_t RFDATA[512];                       /* !< (@ 0x00002800) Register file data. */
} NPU_REGFILE_Regs;

/*@}*/ /* end of group NPU_REGFILE */

/** @addtogroup NPU_INSTMEM
  @{
*/

typedef struct {
  __IO uint32_t INSTMEM[256];                      /* !< (@ 0x00002000) Instruction memory word. */
} NPU_INSTMEM_Regs;

/*@}*/ /* end of group NPU_INSTMEM */

/** @addtogroup NPU_ACCREG
  @{
*/

typedef struct {
  __IO uint32_t ACCVAL[32];                        /* !< (@ 0x00001C00) Register file data. */
} NPU_ACCREG_Regs;

/*@}*/ /* end of group NPU_ACCREG */

/** @addtogroup NPU_ADDRREG
  @{
*/

typedef struct {
  __IO uint32_t AREG[20];                          /* !< (@ 0x00001154) Address registers. */
} NPU_ADDRREG_Regs;

/*@}*/ /* end of group NPU_ADDRREG */

/** @addtogroup NPU_LCREG
  @{
*/

typedef struct {
  __IO uint32_t LC[6];                             /* !< (@ 0x00001138) Loop count registers. */
} NPU_LCREG_Regs;

/*@}*/ /* end of group NPU_LCREG */

/** @addtogroup NPU_INT_EVENT
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
} NPU_INT_EVENT_Regs;

/*@}*/ /* end of group NPU_INT_EVENT */

/** @addtogroup NPU_GPRCM
  @{
*/

typedef struct {
  __IO uint32_t PWREN;                             /* !< (@ 0x00000800) Power enable */
  __O  uint32_t RSTCTL;                            /* !< (@ 0x00000804) Reset Control */
       uint32_t RESERVED0[3];
  __I  uint32_t STAT;                              /* !< (@ 0x00000814) Status Register */
} NPU_GPRCM_Regs;

/*@}*/ /* end of group NPU_GPRCM */

/** @addtogroup NPU
  @{
*/

typedef struct {
       uint32_t RESERVED0[512];
  NPU_GPRCM_Regs  GPRCM;                             /* !< (@ 0x00000800) */
       uint32_t RESERVED1[514];
  NPU_INT_EVENT_Regs  INT_EVENT;                         /* !< (@ 0x00001020) */
       uint32_t RESERVED2[37];
  __I  uint32_t EVT_MODE;                          /* !< (@ 0x000010E0) 'Event Mode' */
       uint32_t RESERVED3[6];
  __I  uint32_t DESC;                              /* !< (@ 0x000010FC) Module Description Register */
       uint32_t RESERVED4[8];
  __IO uint32_t CTL5;                              /* !< (@ 0x00001120) CTL5 register */
  __IO uint32_t CTL4;                              /* !< (@ 0x00001124) */
  __IO uint32_t CTL3;                              /* !< (@ 0x00001128) */
  __IO uint32_t CTL2;                              /* !< (@ 0x0000112C) CTL2 register */
  __IO uint32_t CTL1;                              /* !< (@ 0x00001130) CTL1 register */
  __IO uint32_t CTL0;                              /* !< (@ 0x00001134) CTL0 register */
  NPU_LCREG_Regs  LCREG;                             /* !< (@ 0x00001138) */
  __IO uint32_t PC;                                /* !< (@ 0x00001150) Program count register */
  NPU_ADDRREG_Regs  ADDRREG;                           /* !< (@ 0x00001154) */
       uint32_t RESERVED5[347];
  __IO uint32_t DATAIN[2];                         /* !< (@ 0x00001710) Input Data registers, used to load data from
                                                      memory. */
       uint32_t RESERVED6[2];
  __IO uint32_t DATAOUT;                           /* !< (@ 0x00001720) Output Data registers, used to store data to
                                                      memory. */
       uint32_t RESERVED7[3];
  __IO uint32_t SS0LOW;                            /* !< (@ 0x00001730) Scale and shift register 0, lower 32 bits. */
  __IO uint32_t SS0HIGH;                           /* !< (@ 0x00001734) Scale and shift register 0, upper 32 bits. */
  __IO uint32_t SS1LOW;                            /* !< (@ 0x00001738) Scale and shift register 1, lower 32 bits. */
  __IO uint32_t SS1HIGH;                           /* !< (@ 0x0000173C) Scale and shift register 1, upper 32 bits. */
       uint32_t RESERVED8[4];
  __IO uint32_t WGHT0LOW;                          /* !< (@ 0x00001750) Weight register 0, lower 32 bits. */
  __IO uint32_t WGHT0HIGH;                         /* !< (@ 0x00001754) Weight register 0, upper 32 bits. */
  __IO uint32_t WGHT1LOW;                          /* !< (@ 0x00001758) Weight register 1, lower 32 bits. */
  __IO uint32_t WGHT1HIGH;                         /* !< (@ 0x0000175C) Weight register 1, upper 32 bits. */
       uint32_t RESERVED9[296];
  NPU_ACCREG_Regs  ACCREG;                            /* !< (@ 0x00001C00) */
       uint32_t RESERVED10[224];
  NPU_INSTMEM_Regs  INSTMEM;                           /* !< (@ 0x00002000) */
       uint32_t RESERVED11[256];
  NPU_REGFILE_Regs  REGFILE;                           /* !< (@ 0x00002800) */
} NPU_Regs;

/*@}*/ /* end of group NPU */



#if defined ( __CC_ARM )
#pragma no_anon_unions
#endif

/******************************************************************************
* NPU Register Control Bits
******************************************************************************/

/* NPU_RFDATA Bits */
/* NPU_RFDATA[DATA] Bits */
#define NPU_RFDATA_DATA_OFS                (0)                             /* !< DATA Offset */
#define NPU_RFDATA_DATA_MASK               ((uint32_t)0xFFFFFFFFU)         /* !< Data word. */

/* NPU_INSTMEM_INSTMEM Bits */
/* NPU_INSTMEM_INSTMEM[MEM] Bits */
#define NPU_INSTMEM_INSTMEM_MEM_OFS        (0)                             /* !< MEM Offset */
#define NPU_INSTMEM_INSTMEM_MEM_MASK       ((uint32_t)0xFFFFFFFFU)         /* !< Instruction memory word. */

/* NPU_ACCVAL Bits */
/* NPU_ACCVAL[DATA] Bits */
#define NPU_ACCVAL_DATA_OFS                (0)                             /* !< DATA Offset */
#define NPU_ACCVAL_DATA_MASK               ((uint32_t)0x0003FFFFU)         /* !< Data word. */

/* NPU_AREG Bits */
/* NPU_AREG[DATA] Bits */
#define NPU_AREG_DATA_OFS                  (0)                             /* !< DATA Offset */
#define NPU_AREG_DATA_MASK                 ((uint32_t)0x0000FFFFU)         /* !< Data word. */

/* NPU_LC Bits */
/* NPU_LC[CNT] Bits */
#define NPU_LC_CNT_OFS                     (0)                             /* !< CNT Offset */
#define NPU_LC_CNT_MASK                    ((uint32_t)0x0000FFFFU)         /* !< Loop count. */

/* NPU_IIDX Bits */
/* NPU_IIDX[STAT] Bits */
#define NPU_IIDX_STAT_OFS                  (0)                             /* !< STAT Offset */
#define NPU_IIDX_STAT_MASK                 ((uint32_t)0x000000FFU)         /* !< Interrupt index status */
#define NPU_IIDX_STAT_NO_INTR              ((uint32_t)0x00000000U)         /* !< No interrupt pending */
#define NPU_IIDX_STAT_DONE                 ((uint32_t)0x00000001U)         /* !< Done indication interrupt */

/* NPU_IMASK Bits */
/* NPU_IMASK[DONE] Bits */
#define NPU_IMASK_DONE_OFS                 (0)                             /* !< DONE Offset */
#define NPU_IMASK_DONE_MASK                ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_IMASK_DONE_CLR                 ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_IMASK_DONE_SET                 ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_RIS Bits */
/* NPU_RIS[DONE] Bits */
#define NPU_RIS_DONE_OFS                   (0)                             /* !< DONE Offset */
#define NPU_RIS_DONE_MASK                  ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_RIS_DONE_CLR                   ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_RIS_DONE_SET                   ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_MIS Bits */
/* NPU_MIS[DONE] Bits */
#define NPU_MIS_DONE_OFS                   (0)                             /* !< DONE Offset */
#define NPU_MIS_DONE_MASK                  ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_MIS_DONE_CLR                   ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_MIS_DONE_SET                   ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_ISET Bits */
/* NPU_ISET[DONE] Bits */
#define NPU_ISET_DONE_OFS                  (0)                             /* !< DONE Offset */
#define NPU_ISET_DONE_MASK                 ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_ISET_DONE_CLR                  ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_ISET_DONE_SET                  ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_ICLR Bits */
/* NPU_ICLR[DONE] Bits */
#define NPU_ICLR_DONE_OFS                  (0)                             /* !< DONE Offset */
#define NPU_ICLR_DONE_MASK                 ((uint32_t)0x00000001U)         /* !< DONE interrupt */
#define NPU_ICLR_DONE_CLR                  ((uint32_t)0x00000000U)         /* !< Disable Event */
#define NPU_ICLR_DONE_SET                  ((uint32_t)0x00000001U)         /* !< Enable Event */

/* NPU_PWREN Bits */
/* NPU_PWREN[ENABLE] Bits */
#define NPU_PWREN_ENABLE_OFS               (0)                             /* !< ENABLE Offset */
#define NPU_PWREN_ENABLE_MASK              ((uint32_t)0x00000001U)         /* !< Enable the power */
#define NPU_PWREN_ENABLE_DISABLE           ((uint32_t)0x00000000U)         /* !< Disable Power */
#define NPU_PWREN_ENABLE_ENABLE            ((uint32_t)0x00000001U)         /* !< Enable Power */
/* NPU_PWREN[KEY] Bits */
#define NPU_PWREN_KEY_OFS                  (24)                            /* !< KEY Offset */
#define NPU_PWREN_KEY_MASK                 ((uint32_t)0xFF000000U)         /* !< KEY to allow Power State Change */
#define NPU_PWREN_KEY_UNLOCK_W             ((uint32_t)0x26000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* NPU_RSTCTL Bits */
/* NPU_RSTCTL[RESETSTKYCLR] Bits */
#define NPU_RSTCTL_RESETSTKYCLR_OFS        (1)                             /* !< RESETSTKYCLR Offset */
#define NPU_RSTCTL_RESETSTKYCLR_MASK       ((uint32_t)0x00000002U)         /* !< Clear the RESETSTKY bit in the STAT
                                                                                    register */
#define NPU_RSTCTL_RESETSTKYCLR_NOP        ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define NPU_RSTCTL_RESETSTKYCLR_CLR        ((uint32_t)0x00000002U)         /* !< Clear reset sticky bit */
/* NPU_RSTCTL[RESETASSERT] Bits */
#define NPU_RSTCTL_RESETASSERT_OFS         (0)                             /* !< RESETASSERT Offset */
#define NPU_RSTCTL_RESETASSERT_MASK        ((uint32_t)0x00000001U)         /* !< Assert reset to the peripheral */
#define NPU_RSTCTL_RESETASSERT_NOP         ((uint32_t)0x00000000U)         /* !< Writing 0 has no effect */
#define NPU_RSTCTL_RESETASSERT_ASSERT      ((uint32_t)0x00000001U)         /* !< Assert reset */
/* NPU_RSTCTL[KEY] Bits */
#define NPU_RSTCTL_KEY_OFS                 (24)                            /* !< KEY Offset */
#define NPU_RSTCTL_KEY_MASK                ((uint32_t)0xFF000000U)         /* !< Unlock key */
#define NPU_RSTCTL_KEY_UNLOCK_W            ((uint32_t)0xB1000000U)         /* !< KEY to allow write access to this
                                                                                    register */

/* NPU_STAT Bits */
/* NPU_STAT[RESETSTKY] Bits */
#define NPU_STAT_RESETSTKY_OFS             (16)                            /* !< RESETSTKY Offset */
#define NPU_STAT_RESETSTKY_MASK            ((uint32_t)0x00010000U)         /* !< This bit indicates, if the
                                                                                    peripheral was reset, since this bit
                                                                                    was cleared by RESETSTKYCLR in the
                                                                                    RSTCTL register */
#define NPU_STAT_RESETSTKY_NORES           ((uint32_t)0x00000000U)         /* !< The peripheral has not been reset
                                                                                    since this bit was last cleared by
                                                                                    RESETSTKYCLR in the RSTCTL register */
#define NPU_STAT_RESETSTKY_RESET           ((uint32_t)0x00010000U)         /* !< The peripheral was reset since the
                                                                                    last bit clear */

/* NPU_EVT_MODE Bits */
/* NPU_EVT_MODE[INT0_CFG] Bits */
#define NPU_EVT_MODE_INT0_CFG_OFS          (0)                             /* !< INT0_CFG Offset */
#define NPU_EVT_MODE_INT0_CFG_MASK         ((uint32_t)0x00000003U)         /* !< 'Event line mode select for event
                                                                                    corresponding to
                                                                                    [IPSTANDARD.INT_EVENT][0]' */
#define NPU_EVT_MODE_INT0_CFG_DISABLE      ((uint32_t)0x00000000U)         /* !< &apos;The interrupt or event line
                                                                                    is disabled.&apos; */
#define NPU_EVT_MODE_INT0_CFG_SOFTWARE     ((uint32_t)0x00000001U)         /* !< &apos;The interrupt or event line
                                                                                    is in software mode. Software must
                                                                                    clear the RIS.&apos; */
#define NPU_EVT_MODE_INT0_CFG_HARDWARE     ((uint32_t)0x00000002U)         /* !< &apos;The interrupt or event line
                                                                                    is in hardware mode. The hardware
                                                                                    (another module) clears automatically
                                                                                    the associated RIS flag.&apos; */

/* NPU_DESC Bits */
/* NPU_DESC[MODULEID] Bits */
#define NPU_DESC_MODULEID_OFS              (16)                            /* !< MODULEID Offset */
#define NPU_DESC_MODULEID_MASK             ((uint32_t)0xFFFF0000U)         /* !< specifies module id */
/* NPU_DESC[FEATUREVER] Bits */
#define NPU_DESC_FEATUREVER_OFS            (12)                            /* !< FEATUREVER Offset */
#define NPU_DESC_FEATUREVER_MASK           ((uint32_t)0x0000F000U)         /* !< specifies feature version info */
/* NPU_DESC[INSTNUM] Bits */
#define NPU_DESC_INSTNUM_OFS               (8)                             /* !< INSTNUM Offset */
#define NPU_DESC_INSTNUM_MASK              ((uint32_t)0x00000F00U)         /* !< specifies module instance number */
/* NPU_DESC[MAJREV] Bits */
#define NPU_DESC_MAJREV_OFS                (4)                             /* !< MAJREV Offset */
#define NPU_DESC_MAJREV_MASK               ((uint32_t)0x000000F0U)         /* !< specifies major revision id */
/* NPU_DESC[MINREV] Bits */
#define NPU_DESC_MINREV_OFS                (0)                             /* !< MINREV Offset */
#define NPU_DESC_MINREV_MASK               ((uint32_t)0x0000000FU)         /* !< specifies minor revision id */

/* NPU_CTL5 Bits */
/* NPU_CTL5[RUN] Bits */
#define NPU_CTL5_RUN_OFS                   (0)                             /* !< RUN Offset */
#define NPU_CTL5_RUN_MASK                  ((uint32_t)0x00000001U)         /* !< Start the accelerator. Writing a 1
                                                                                    to this bit starts the accelerator.
                                                                                    When the STOP instruction is executed
                                                                                    this bit is cleared by hardware and
                                                                                    DONE interrupt flag is set. */
/* NPU_CTL5[ENIFMAPCIRC] Bits */
#define NPU_CTL5_ENIFMAPCIRC_OFS           (8)                             /* !< ENIFMAPCIRC Offset */
#define NPU_CTL5_ENIFMAPCIRC_MASK          ((uint32_t)0x00000100U)         /* !< Enable circular addressing on input
                                                                                    feature map read address. */
#define NPU_CTL5_ENIFMAPCIRC_DISABLE       ((uint32_t)0x00000000U)
#define NPU_CTL5_ENIFMAPCIRC_ENABLE        ((uint32_t)0x00000100U)         /* !< Enable circular buffering on the
                                                                                    input feature map */
/* NPU_CTL5[ENOFMAPCIRC] Bits */
#define NPU_CTL5_ENOFMAPCIRC_OFS           (9)                             /* !< ENOFMAPCIRC Offset */
#define NPU_CTL5_ENOFMAPCIRC_MASK          ((uint32_t)0x00000200U)         /* !< Enable circular addressing on
                                                                                    output feature map read address. */
#define NPU_CTL5_ENOFMAPCIRC_DISABLE       ((uint32_t)0x00000000U)         /* !< Disable circular buffering on the
                                                                                    output feature map */
#define NPU_CTL5_ENOFMAPCIRC_ENABLE        ((uint32_t)0x00000200U)         /* !< Enable circular buffering on the
                                                                                    output feature map */

/* NPU_CTL4 Bits */
/* NPU_CTL4[OFMAPCIRSTART] Bits */
#define NPU_CTL4_OFMAPCIRSTART_OFS         (0)                             /* !< OFMAPCIRSTART Offset */
#define NPU_CTL4_OFMAPCIRSTART_MASK        ((uint32_t)0x0000FFFFU)         /* !< Circular buffer output feature map
                                                                                    start address. */
/* NPU_CTL4[OFMAPCIREND] Bits */
#define NPU_CTL4_OFMAPCIREND_OFS           (16)                            /* !< OFMAPCIREND Offset */
#define NPU_CTL4_OFMAPCIREND_MASK          ((uint32_t)0xFFFF0000U)         /* !< Circular buffer output feature map
                                                                                    end address. */

/* NPU_CTL3 Bits */
/* NPU_CTL3[IFMAPCIRSTART] Bits */
#define NPU_CTL3_IFMAPCIRSTART_OFS         (0)                             /* !< IFMAPCIRSTART Offset */
#define NPU_CTL3_IFMAPCIRSTART_MASK        ((uint32_t)0x0000FFFFU)         /* !< Circular buffer input feature map
                                                                                    start address */
/* NPU_CTL3[IFMAPCIREND] Bits */
#define NPU_CTL3_IFMAPCIREND_OFS           (16)                            /* !< IFMAPCIREND Offset */
#define NPU_CTL3_IFMAPCIREND_MASK          ((uint32_t)0xFFFF0000U)         /* !< Circular buffer input feature map
                                                                                    end address */

/* NPU_CTL2 Bits */
/* NPU_CTL2[CLLOW] Bits */
#define NPU_CTL2_CLLOW_OFS                 (0)                             /* !< CLLOW Offset */
#define NPU_CTL2_CLLOW_MASK                ((uint32_t)0x000001FFU)         /* !< Lower clamp value */
/* NPU_CTL2[CLHIGH] Bits */
#define NPU_CTL2_CLHIGH_OFS                (16)                            /* !< CLHIGH Offset */
#define NPU_CTL2_CLHIGH_MASK               ((uint32_t)0x01FF0000U)         /* !< Upper clamp value */

/* NPU_CTL1 Bits */
/* NPU_CTL1[IFMAPSTARTHI] Bits */
#define NPU_CTL1_IFMAPSTARTHI_OFS          (0)                             /* !< IFMAPSTARTHI Offset */
#define NPU_CTL1_IFMAPSTARTHI_MASK         ((uint32_t)0x0000FFFFU)         /* !< Start address (Higher 16 bits) of
                                                                                    input feature map data. */
/* NPU_CTL1[OFMAPSTARTHI] Bits */
#define NPU_CTL1_OFMAPSTARTHI_OFS          (16)                            /* !< OFMAPSTARTHI Offset */
#define NPU_CTL1_OFMAPSTARTHI_MASK         ((uint32_t)0xFFFF0000U)         /* !< Start address (Higher 16 bits) of
                                                                                    output feature map data. */

/* NPU_CTL0 Bits */
/* NPU_CTL0[WTYPE] Bits */
#define NPU_CTL0_WTYPE_OFS                 (12)                            /* !< WTYPE Offset */
#define NPU_CTL0_WTYPE_MASK                ((uint32_t)0x0000F000U)         /* !< Weight Type */
#define NPU_CTL0_WTYPE__2T                 ((uint32_t)0x00000000U)         /* !< +1,-1,0 */
#define NPU_CTL0_WTYPE_BINARY              ((uint32_t)0x00001000U)         /* !< Binary data type, +1 or -1 */
#define NPU_CTL0_WTYPE_SIGNED_2BIT         ((uint32_t)0x00002000U)         /* !< Unsigned 8 bit data type */
#define NPU_CTL0_WTYPE_SIGNED_4BIT         ((uint32_t)0x00003000U)         /* !< Signed 4 bit data type */
#define NPU_CTL0_WTYPE_SIGNED_8BIT         ((uint32_t)0x00004000U)         /* !< Signed 8 bit data type */
/* NPU_CTL0[IFMAPTYPE] Bits */
#define NPU_CTL0_IFMAPTYPE_OFS             (0)                             /* !< IFMAPTYPE Offset */
#define NPU_CTL0_IFMAPTYPE_MASK            ((uint32_t)0x0000000FU)         /* !< Input feature map type, */
#define NPU_CTL0_IFMAPTYPE_UNSIGNED_16BIT  ((uint32_t)0x00000000U)
#define NPU_CTL0_IFMAPTYPE_UNSIGNED_8BIT   ((uint32_t)0x00000001U)         /* !< Unsigned 8 bit data type */
#define NPU_CTL0_IFMAPTYPE_UNSIGNED_4BIT   ((uint32_t)0x00000002U)         /* !< Unsigned 4 bit data type */
#define NPU_CTL0_IFMAPTYPE_BINARY          ((uint32_t)0x00000003U)         /* !< Binary data type */
#define NPU_CTL0_IFMAPTYPE_SIGNED_16BIT    ((uint32_t)0x00000008U)
#define NPU_CTL0_IFMAPTYPE_SIGNED_8BIT     ((uint32_t)0x00000009U)         /* !< Signed 8 bit data type */
#define NPU_CTL0_IFMAPTYPE_SIGNED_4BIT     ((uint32_t)0x0000000AU)         /* !< Signed 4 bit data type */
/* NPU_CTL0[OFMAPTYPE] Bits */
#define NPU_CTL0_OFMAPTYPE_OFS             (4)                             /* !< OFMAPTYPE Offset */
#define NPU_CTL0_OFMAPTYPE_MASK            ((uint32_t)0x000000F0U)         /* !< Output feature map type. */
#define NPU_CTL0_OFMAPTYPE_UNSIGNED_16BIT  ((uint32_t)0x00000000U)
#define NPU_CTL0_OFMAPTYPE_UNSIGNED_8BIT   ((uint32_t)0x00000010U)         /* !< Unsigned 8 bit data type */
#define NPU_CTL0_OFMAPTYPE_UNSIGNED_4BIT   ((uint32_t)0x00000020U)         /* !< Unsigned 4 bit data type */
#define NPU_CTL0_OFMAPTYPE_BINARY          ((uint32_t)0x00000030U)         /* !< Binary data type */
#define NPU_CTL0_OFMAPTYPE_SIGNED_16BIT    ((uint32_t)0x00000080U)
#define NPU_CTL0_OFMAPTYPE_SIGNED_8BIT     ((uint32_t)0x00000090U)         /* !< Signed 8 bit data type */
#define NPU_CTL0_OFMAPTYPE_SIGNED_4BIT     ((uint32_t)0x000000A0U)         /* !< Signed 4 bit data type */
/* NPU_CTL0[LAYERTYPE] Bits */
#define NPU_CTL0_LAYERTYPE_OFS             (20)                            /* !< LAYERTYPE Offset */
#define NPU_CTL0_LAYERTYPE_MASK            ((uint32_t)0x01F00000U)         /* !< Layer type */
#define NPU_CTL0_LAYERTYPE_FCONV           ((uint32_t)0x00000000U)
#define NPU_CTL0_LAYERTYPE_DW              ((uint32_t)0x00200000U)
#define NPU_CTL0_LAYERTYPE_PW              ((uint32_t)0x00300000U)
#define NPU_CTL0_LAYERTYPE_CONV            ((uint32_t)0x00500000U)
#define NPU_CTL0_LAYERTYPE_FC              ((uint32_t)0x00600000U)
#define NPU_CTL0_LAYERTYPE_AVGPOOL         ((uint32_t)0x00700000U)
#define NPU_CTL0_LAYERTYPE_MAXPOOL         ((uint32_t)0x00800000U)
#define NPU_CTL0_LAYERTYPE_FCONV_RESIDUAL  ((uint32_t)0x01000000U)
#define NPU_CTL0_LAYERTYPE_DW_RESIDUAL     ((uint32_t)0x01200000U)
#define NPU_CTL0_LAYERTYPE_PW_RESIDUAL     ((uint32_t)0x01300000U)
#define NPU_CTL0_LAYERTYPE_CONV_RESIDUAL   ((uint32_t)0x01500000U)
#define NPU_CTL0_LAYERTYPE_FC_RESIDUAL     ((uint32_t)0x01600000U)
#define NPU_CTL0_LAYERTYPE_AVGPOOL_RESIDUAL ((uint32_t)0x01700000U)
#define NPU_CTL0_LAYERTYPE_MAXPOOL_RESIDUAL ((uint32_t)0x01800000U)
/* NPU_CTL0[RESIFMAPTYPE] Bits */
#define NPU_CTL0_RESIFMAPTYPE_OFS          (26)                            /* !< RESIFMAPTYPE Offset */
#define NPU_CTL0_RESIFMAPTYPE_MASK         ((uint32_t)0x3C000000U)         /* !< Residual Input feature map type, */
#define NPU_CTL0_RESIFMAPTYPE_UNSIGNED_16BIT ((uint32_t)0x00000000U)
#define NPU_CTL0_RESIFMAPTYPE_UNSIGNED_8BIT ((uint32_t)0x04000000U)         /* !< Unsigned 8 bit data type */
#define NPU_CTL0_RESIFMAPTYPE_UNSIGNED_4BIT ((uint32_t)0x08000000U)         /* !< Unsigned 4 bit data type */
#define NPU_CTL0_RESIFMAPTYPE_SIGNED_16BIT ((uint32_t)0x20000000U)
#define NPU_CTL0_RESIFMAPTYPE_SIGNED_8BIT  ((uint32_t)0x24000000U)         /* !< Signed 8 bit data type */
#define NPU_CTL0_RESIFMAPTYPE_SIGNED_4BIT  ((uint32_t)0x28000000U)         /* !< Signed 4 bit data type */

/* NPU_PC Bits */
/* NPU_PC[VALUE] Bits */
#define NPU_PC_VALUE_OFS                   (0)                             /* !< VALUE Offset */
#define NPU_PC_VALUE_MASK                  ((uint32_t)0x0000FFFFU)         /* !< Program count value. */

/* NPU_DATAIN Bits */
/* NPU_DATAIN[VALUE] Bits */
#define NPU_DATAIN_VALUE_OFS               (0)                             /* !< VALUE Offset */
#define NPU_DATAIN_VALUE_MASK              ((uint32_t)0xFFFFFFFFU)         /* !< Data value */

/* NPU_DATAOUT Bits */
/* NPU_DATAOUT[VALUE] Bits */
#define NPU_DATAOUT_VALUE_OFS              (0)                             /* !< VALUE Offset */
#define NPU_DATAOUT_VALUE_MASK             ((uint32_t)0xFFFFFFFFU)         /* !< Data out value. */

/* NPU_SS0LOW Bits */
/* NPU_SS0LOW[VALUE] Bits */
#define NPU_SS0LOW_VALUE_OFS               (0)                             /* !< VALUE Offset */
#define NPU_SS0LOW_VALUE_MASK              ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_SS0HIGH Bits */
/* NPU_SS0HIGH[VALUE] Bits */
#define NPU_SS0HIGH_VALUE_OFS              (0)                             /* !< VALUE Offset */
#define NPU_SS0HIGH_VALUE_MASK             ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_SS1LOW Bits */
/* NPU_SS1LOW[VALUE] Bits */
#define NPU_SS1LOW_VALUE_OFS               (0)                             /* !< VALUE Offset */
#define NPU_SS1LOW_VALUE_MASK              ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_SS1HIGH Bits */
/* NPU_SS1HIGH[VALUE] Bits */
#define NPU_SS1HIGH_VALUE_OFS              (0)                             /* !< VALUE Offset */
#define NPU_SS1HIGH_VALUE_MASK             ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_WGHT0LOW Bits */
/* NPU_WGHT0LOW[VALUE] Bits */
#define NPU_WGHT0LOW_VALUE_OFS             (0)                             /* !< VALUE Offset */
#define NPU_WGHT0LOW_VALUE_MASK            ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_WGHT0HIGH Bits */
/* NPU_WGHT0HIGH[VALUE] Bits */
#define NPU_WGHT0HIGH_VALUE_OFS            (0)                             /* !< VALUE Offset */
#define NPU_WGHT0HIGH_VALUE_MASK           ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_WGHT1LOW Bits */
/* NPU_WGHT1LOW[VALUE] Bits */
#define NPU_WGHT1LOW_VALUE_OFS             (0)                             /* !< VALUE Offset */
#define NPU_WGHT1LOW_VALUE_MASK            ((uint32_t)0xFFFFFFFFU)         /* !< Value */

/* NPU_WGHT1HIGH Bits */
/* NPU_WGHT1HIGH[VALUE] Bits */
#define NPU_WGHT1HIGH_VALUE_OFS            (0)                             /* !< VALUE Offset */
#define NPU_WGHT1HIGH_VALUE_MASK           ((uint32_t)0xFFFFFFFFU)         /* !< Value */


#ifdef __cplusplus
}
#endif

#endif /* ti_devices_msp_peripherals_hw_NPU__include */

