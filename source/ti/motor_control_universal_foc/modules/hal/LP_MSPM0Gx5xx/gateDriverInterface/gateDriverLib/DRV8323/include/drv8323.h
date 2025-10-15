/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       drv8323.h
 *  @brief      DRV8323 gate driver Module
 *
 * 
 *  @anchor drv8323_h
 *  # Overview
 *
 *  defines APIs for drv8323 gate driver
 *
 *  <hr>
 ******************************************************************************/

#ifndef DRV8323_H
#define DRV8323_H

#include "stdint.h"
#include "main.h"
#include "appInputCtrlInterface.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef DRV8323

/*! @brief Defines the address mask */
#define DRV8323_ADDR_MASK                   (0x7800)

/*! @brief Defines the data mask */
#define DRV8323_DATA_MASK                   (0x07FF)

/*! @brief Defines the R/W mask */
#define DRV8323_RW_MASK                     (0x8000)

/*! @brief DRV8323 REG 0 */
#define DRV8323_REG_0                0x00
/*! @brief DRV8323 REG 1 */
#define DRV8323_REG_1                0x01
/*! @brief DRV8323 REG 2 */
#define DRV8323_REG_2                0x02
/*! @brief DRV8323 REG 3 */
#define DRV8323_REG_3                0x03
/*! @brief DRV8323 REG 4 */
#define DRV8323_REG_4                0x04
/*! @brief DRV8323 REG 5 */
#define DRV8323_REG_5                0x05
/*! @brief DRV8323 REG 6 */
#define DRV8323_REG_6                0x06

/*! @brief Defines the Fault type mask */
#define DRV8323_FAULT_TYPE_MASK             (0x07FF)

/*! @brief Defines the location of VDS LC bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_LC_BITS        (1 << 0)
/*! @brief Defines the location of VDS HC bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_HC_BITS        (1 << 1)
/*! @brief Defines the location of VDS LB bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_LB_BITS        (1 << 2)
/*! @brief Defines the location of VDS HB bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_HB_BITS        (1 << 3)
/*! @brief Defines the location of VDS LA bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_LA_BITS        (1 << 4)
/*! @brief Defines the location of VDS HA bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_HA_BITS        (1 << 5)
/*! @brief Defines the location of OTSD (Over temperature shutdown) bit in the Status 1 register  */
#define DRV8323_STATUS00_OTSD_BITS          (1 << 6)
/*! @brief Defines the location of UVLO bit in the Status 1 register  */
#define DRV8323_STATUS00_UVLO_BITS          (1 << 7)
/*! @brief Defines the location of GDF bit in the Status 1 register  */
#define DRV8323_STATUS00_GDF_BITS           (1 << 8)
/*! @brief Defines the location of VDS OCP bit in the Status 1 register  */
#define DRV8323_STATUS00_VDS_OCP_BITS       (1 << 9)
/*! @brief Defines the location of FAULT bit in the Status 1 register */
#define DRV8323_STATUS00_FAULT_BITS         (1 << 10)


/*! @brief Defines the location of the VGS_LC bits in the Status 2 register */
#define DRV8323_STATUS01_VGS_LC_BITS     (1 << 0)

/*! @brief Defines the location of the VGS_HC bits in the Status 2 register */
#define DRV8323_STATUS01_VGS_HC_BITS     (1 << 1)

/*! @brief Defines the location of the VGS_LB bits in the Status 2 register */
#define DRV8323_STATUS01_VGS_LB_BITS     (1 << 2)

/*! @brief Defines the location of the VGS_HB bits in the Status 2 register */
#define DRV8323_STATUS01_VGS_HB_BITS     (1 << 3)

/*! @brief Defines the location of the VGS_LA bits in the Status 2 register */
#define DRV8323_STATUS01_VGS_LA_BITS     (1 << 4)

/*! @brief Defines the location of the VGS_HA bits in the Status 2 register */
#define DRV8323_STATUS01_VGS_HA_BITS     (1 << 5)

/*! @brief Defines the location of the CPUV (charge pump undervoltage) bits in the Status 2 register */
#define DRV8323_STATUS01_CPUV_BITS     (1 << 6)

/*! @brief Defines the location of the OTW bits in the Status 2 register */
#define DRV8323_STATUS01_OTW_BITS     (1 << 7)

/*! @brief Defines the location of the SC_OC bits in the Status 2 register */
#define DRV8323_STATUS01_SC_OC_BITS     (1 << 8)

/*! @brief Defines the location of the SB_OC bits in the Status 2 register */
#define DRV8323_STATUS01_SB_OC_BITS     (1 << 9)

/*! @brief Defines the location of the SA_OC bits in the Status 2 register */
#define DRV8323_STATUS01_SA_OC_BITS     (1 << 10)



/*! @brief Defines the location of the CLR_FLT bits in the Control 2 register */
#define DRV8323_CTRL02_CLR_FLT_BITS       (1 << 0)

/*! @brief Defines the location of the BRAKE bits in the Control 2 register */
#define DRV8323_CTRL02_BRAKE_BITS         (1 << 1)

/*! @brief Defines the location of the COAST bits in the Control 2 register */
#define DRV8323_CTRL02_COAST_BITS         (1 << 2)

/*! @brief Defines the location of the 1PWM_DIR bits in the Control 2 register */
#define DRV8323_CTRL02_PWM1_DIR_BITS       (1 << 3)

/*! @brief Defines the location of the 1PWM_COM bits in the Control 2 register */
#define DRV8323_CTRL02_PWM1_COM_BITS       (1 << 4)

/*! @brief Defines the location of the PWM_MODE bits in the Control 2 register */
#define DRV8323_CTRL02_PWM_MODE_BITS       (3 << 5)

/*! @brief Defines the location of the OTW_REP bits in the Control 2 register */
#define DRV8323_CTRL02_OTW_REP_BITS        (1 << 7)

/*! @brief Defines the location of the DIS_GDF bits in the Control 2 register */
#define DRV8323_CTRL02_DIS_GDF_BITS        (1 << 8)

/*! @brief Defines the location of the DIS_CPUV bits in the Control 2 register */
#define DRV8323_CTRL02_DIS_CPUV_BITS       (1 << 9)

/*! @brief Defines the location of the RESERVED1 bits in the Control 2 register */
#define DRV8323_CTRL02_RESERVED1_BITS       (1 << 10)


/*! @brief Defines the location of the IDRIVEN_HS bits in the Control 3 register */
#define DRV8323_CTRL03_IDRIVEN_HS_BITS      (15 << 0)

/*! @brief Defines the location of the IDRIVEP_HS bits in the Control 3 register */
#define DRV8323_CTRL03_IDRIVEP_HS_BITS      (15 << 4)

/*! @brief Defines the location of the LOCK bits in the Control 3 register */
#define DRV8323_CTRL03_LOCK_BITS            (7 << 8)

/*! @brief Defines the location of the IDRIVEN_LS bits in the Control 4 register */
#define DRV8323_CTRL04_IDRIVEN_LS_BITS      (15 << 0)

/*! @brief Defines the location of the IDRIVEP_LS bits in the Control 4 register */
#define DRV8323_CTRL04_IDRIVEP_LS_BITS      (15 << 4)

/*! @brief Defines the location of the TDRIVE bits in the Control 4 register */
#define DRV8323_CTRL04_TDRIVE_BITS          (3 << 8)

/*! @brief Defines the location of the CBC bits in the Control 4 register */
#define DRV8323_CTRL04_CBC_BITS             (1 << 10)


/*! @brief Defines the location of the VDS_LVL bits in the Control 5 register */
#define DRV8323_CTRL05_VDS_LVL_BITS         (15 << 0)

/*! @brief Defines the location of the OCP_DEG bits in the Control 5 register */
#define DRV8323_CTRL05_OCP_DEG_BITS         (3 << 4)

/*! @brief Defines the location of the OCP_MODE bits in the Control 5 register */
#define DRV8323_CTRL05_OCP_MODE_BITS        (3 << 6)

/*! @brief Defines the location of the DEAD_TIME bits in the Control 5 register */
#define DRV8323_CTRL05_DEAD_TIME_BITS      (3 << 8)

/*! @brief Defines the location of the TRETRY bits in the Control 5 register */
#define DRV8323_CTRL05_TRETRY_BITS         (1 << 10)


/*! @brief Defines the location of the SEN_LVL bits in the Control 6 register */
#define DRV8323_CTRL06_SEN_LVL_BITS         (3 << 0)

/*! @brief Defines the location of the CSA_CAL_C bits in the Control 6 register */
#define DRV8323_CTRL06_CSA_CAL_C_BITS       (1 << 2)

/*! @brief Defines the location of the CSA_CAL_B bits in the Control 6 register */
#define DRV8323_CTRL06_CSA_CAL_B_BITS       (1 << 3)

/*! @brief Defines the location of the CSA_CAL_A bits in the Control 6 register */
#define DRV8323_CTRL06_CSA_CAL_A_BITS       (1 << 4)

/*! @brief Defines the location of the DIS_SEN bits in the Control 6 register */
#define DRV8323_CTRL06_DIS_SEN_BITS         (1 << 5)

/*! @brief Defines the location of the CSA_GAIN bits in the Control 6 register */
#define DRV8323_CTRL06_CSA_GAIN_BITS        (3 << 6)

/*! @brief Defines the location of the LS_REF bits in the Control 6 register */
#define DRV8323_CTRL06_LS_REF_BITS          (1 << 8)

/*! @brief Defines the location of the VREF_DIV bits in the Control 6 register */
#define DRV8323_CTRL06_VREF_DIV_BITS        (1 << 9)

/*! @brief Defines the location of the CSA_FET bits in the Control 6 register */
#define DRV8323_CTRL06_CSA_FET_BITS         (1 << 10)

/*! shift values for HVdie parameters */

/*! @brief DRV8323 OTW REP POS */
#define    DRV8323_OTW_REP_POS      7
/*! @brief DRV8323 DIS GDF POS */
#define    DRV8323_DIS_GDF_POS      8
/*! @brief DRV8323 DIS CPUV POS */
#define    DRV8323_DIS_CPUV_POS     9

/*! @brief DRV8323 IDRIVEN HS POS */
#define    DRV8323_IDRIVEN_HS_POS   0
/*! @brief DRV8323 IDRIVEP HS POS */
#define    DRV8323_IDRIVEP_HS_POS   4

/*! @brief DRV8323 IDRIVEN LS POS */
#define    DRV8323_IDRIVEN_LS_POS   0
/*! @brief DRV8323 IDRIVEP LS POS */
#define    DRV8323_IDRIVEP_LS_POS   4
/*! @brief DRV8323 TDRIVE POS */
#define    DRV8323_TDRIVE_POS       8
/*! @brief DRV8323 CBC POS */
#define    DRV8323_CBC_POS          10

/*! @brief DRV8323 VDS LVL POS */
#define    DRV8323_VDS_LVL_POS      0
/*! @brief DRV8323 OCP DEG POS */
#define    DRV8323_OCP_DEG_POS      4
/*! @brief DRV8323 OCP MODE POS */
#define    DRV8323_OCP_MODE_POS     6
/*! @brief DRV8323 DEAD TIME POS */
#define    DRV8323_DEAD_TIME_POS    8
/*! @brief DRV8323 TRETRY POS */
#define    DRV8323_TRETRY_POS       10

/*! @brief DRV8323 SEN LVL POS */
#define    DRV8323_SEN_LVL_POS      0
/*! @brief DRV8323 DIS SEN POS */
#define    DRV8323_DIS_SEN_POS      5
/*! @brief DRV8323 CSA GAIN POS */
#define    DRV8323_CSA_GAIN_POS     6
/*! @brief DRV8323 LS REF POS */
#define    DRV8323_LS_REF_POS       8
/*! @brief DRV8323 VREF DIV POS */
#define    DRV8323_VREF_DIV_POS     9
/*! @brief DRV8323 CSA FET POS */
#define    DRV8323_CSA_FET_POS      10
/*************************************************************************/

/*! @brief Enumeration for the R/W modes */
typedef enum
{
  /*! Read Mode */
  CtrlMode_Read = 1 << 15,
  /*! Write Mode */
  CtrlMode_Write = 0 << 15
} DRV8323_CtrlMode_e;

/*! @brief Enumeration for the Status 0 register, faults */
typedef enum
{
  /*! VDS overcurrent fault on C low-side MOSFET */
  VDS_LC      = (1 << 0),    
  /*! VDS overcurrent fault on C high-side MOSFET */
  VDS_HC      = (1 << 1),    
  /*! VDS overcurrent fault on B low-side MOSFET */
  VDS_LB      = (1 << 2),    
  /*! VDS overcurrent fault on B high-side MOSFET */
  VDS_HB      = (1 << 3),    
  /*! VDS overcurrent fault on A low-side MOSFET */
  VDS_LA      = (1 << 4),    
  /*! VDS overcurrent fault on A high-side MOSFET */
  VDS_HA      = (1 << 5),    
  /*! Overtemperature shutdown */
  OTSD        = (1 << 6),    
  /*! Undervoltage lockout fault condition */
  UVLO        = (1 << 7),    
  /*! Gate driver fault condition */
  GDF         = (1 << 8),    
  /*! VDS monitor overcurrent fault condition */
  VDS_OCP     = (1 << 9),    
  /*! FAULT type, 0-Warning, 1-Latched */
  FAULT1       = (1 << 10)    
} DRV8323_STATUS00_WarningWatchdog_e;


/*! @brief Enumeration for the Status 1 register, OV/VDS faults */
typedef enum
{
  /*! VGS gate drive fault on C low-side MOSFET */
  VGS_LC      = (1 << 0),    
  /*! VGS gate drive fault on C high-side MOSFET */
  VGS_HC      = (1 << 1),    
  /*! VGS gate drive fault on B low-side MOSFET */
  VGS_LB      = (1 << 2),    
  /*! VGS gate drive fault on B high-side MOSFET */
  VGS_HB      = (1 << 3),    
  /*! VGS gate drive fault on A low-side MOSFET */
  VGS_LA      = (1 << 4),    
  /*! VGS gate drive fault on A high-side MOSFET */
  VGS_HA      = (1 << 5),    
  /*! charge pump undervoltage fault */
  CPUV        = (1 << 6),    
  /*! overtemperature warning */
  OTW         = (1 << 7),    
  /*! overcurrent on phase C */
  SC_OC       = (1 << 8),    
  /*! overcurrent on phase B */
  SB_OC       = (1 << 9),    
  /*! overcurrent on phase A */
  SA_OC       = (1 << 10)    
} DRV8323_STATUS01_OvVdsFaults_e;


/*! @brief Enumeration for the driver PWM mode */
typedef enum 
{
  /*! PWM_MODE = 6 inputs */
  PwmMode_6 = (0 << 5),     
  /*! PWM_MODE = 3 inputs */
  PwmMode_3 = (1 << 5),     
  /*! PWM_MODE = 1 input */
  PwmMode_1 = (2 << 5)      
} DRV8323_CTRL02_PwmMode_e;


/*! @brief Enumeration for the high side gate drive peak source current */
typedef enum 
{
  /*! IDRIVEP_HS = 0.010A */
  ISour_HS_0p010_A = (0 << 4),  
  /*! IDRIVEP_HS = 0.020A */
  ISour_HS_0p020_A = (1 << 4),  
  /*! IDRIVEP_HS = 0.030A */
  ISour_HS_0p030_A = (2 << 4),  
  /*! IDRIVEP_HS = 0.040A */
  ISour_HS_0p040_A = (3 << 4),  
  /*! IDRIVEP_HS = 0.050A */
  ISour_HS_0p050_A = (4 << 4),  
  /*! IDRIVEP_HS = 0.060A */
  ISour_HS_0p060_A = (5 << 4),  
  /*! IDRIVEP_HS = 0.070A */
  ISour_HS_0p070_A = (6 << 4),  
  /*! IDRIVEP_HS = 0.125A */
  ISour_HS_0p125_A = (7 << 4),  
  /*! IDRIVEP_HS = 0.250A */
  ISour_HS_0p250_A = (8 << 4),  
  /*! IDRIVEP_HS = 0.500A */
  ISour_HS_0p500_A = (9 << 4),  
  /*! IDRIVEP_HS = 0.750A */
  ISour_HS_0p750_A = (10 << 4), 
  /*! IDRIVEP_HS = 1.000A */
  ISour_HS_1p000_A = (11 << 4)  
} DRV8323_CTRL03_PeakSourCurHS_e;


/*! @brief Enumeration for the high side gate drive peak sink current */
typedef enum 
{
  /*! IDRIVEN_HS = 0.020A */
  ISink_HS_0p020_A = (0 << 0),  
  /*! IDRIVEN_HS = 0.030A */
  ISink_HS_0p030_A = (1 << 0),  
  /*! IDRIVEN_HS = 0.040A */
  ISink_HS_0p040_A = (2 << 0),  
  /*! IDRIVEN_HS = 0.050A */
  ISink_HS_0p050_A = (3 << 0),  
  /*! IDRIVEN_HS = 0.060A */
  ISink_HS_0p060_A = (4 << 0),  
  /*! IDRIVEN_HS = 0.070A */
  ISink_HS_0p070_A = (5 << 0),  
  /*! IDRIVEN_HS = 0.125A */
  ISink_HS_0p125_A = (6 << 0),  
  /*! IDRIVEN_HS = 0.250A */
  ISink_HS_0p250_A = (7 << 0),  
  /*! IDRIVEN_HS = 0.500A */
  ISink_HS_0p500_A = (8 << 0),  
  /*! IDRIVEN_HS = 0.750A */
  ISink_HS_0p750_A = (9 << 0),  
  /*! IDRIVEN_HS = 1.000A */
  ISink_HS_1p000_A = (10 << 0), 
  /*! IDRIVEN_HS = 1.250A */
  ISink_HS_1p250_A = (11 << 0)  
} DRV8323_CTRL03_PeakSinkCurHS_e;


/*! @brief Enumeration for the high side and low side gate drive peak source time */
typedef enum 
{
  /*! Lock settings */
  Lock_lock     = (6 << 8),     
  /*! Unlock settings */
  Lock_unlock   = (3 << 8)      
} DRV8323_CTRL03_Lock_e;


/*! @brief Enumeration for the high side and low side gate drive peak source time */
typedef enum 
{
   /*! TDRIVEN = 250ns */
  TSour_250_ns  = (0 << 8),    
   /*! TDRIVEN = 500ns */
  TSour_500_ns  = (1 << 8),    
   /*! TDRIVEN = 1000ns */
  TSour_1000_ns = (2 << 8),    
   /*! TDRIVEN = 2000ns */
  TSour_2000_ns = (3 << 8)     
} DRV8323_CTRL04_PeakTime_e;


/*! @brief Enumeration for the low side gate drive peak source current */
typedef enum 
{
  /*! IDRIVEP_LS = 0.010A */
  ISour_LS_0p010_A = (0 << 4),  
  /*! IDRIVEP_LS = 0.020A */
  ISour_LS_0p030_A = (1 << 4),  
  /*! IDRIVEP_LS = 0.030A */
  ISour_LS_0p060_A = (2 << 4),  
  /*! IDRIVEP_LS = 0.040A */
  ISour_LS_0p080_A = (3 << 4),  
  /*! IDRIVEP_LS = 0.050A */
  ISour_LS_0p120_A = (4 << 4),  
  /*! IDRIVEP_LS = 0.060A */
  ISour_LS_0p140_A = (5 << 4),  
  /*! IDRIVEP_LS = 0.070A */
  ISour_LS_0p170_A = (6 << 4),  
  /*! IDRIVEP_LS = 0.125A */
  ISour_LS_0p190_A = (7 << 4),  
  /*! IDRIVEP_LS = 0.250A */
  ISour_LS_0p250_A = (8 << 4),  
  /*! IDRIVEP_LS = 0.500A */
  ISour_LS_0p330_A = (9 << 4),  
  /*! IDRIVEP_LS = 0.750A */
  ISour_LS_0p370_A = (10 << 4), 
  /*! IDRIVEP_LS = 1.000A */
  ISour_LS_0p440_A = (11 << 4),  
  /*! IDRIVEP_LS = 1.000A */
  ISour_LS_0p570_A = (12 << 4),  
  /*! IDRIVEP_LS = 1.000A */
  ISour_LS_0p680_A = (13 << 4),  
  /*! IDRIVEP_LS = 1.000A */
  ISour_LS_0p820_A = (14 << 4), 
  /*! IDRIVEP_LS = 1.000A */ 
  ISour_LS_1p000_A = (15 << 4)
} DRV8323_CTRL04_PeakSourCurLS_e;


/*! @brief Enumeration for the low side gate drive peak sink current */
typedef enum 
{
  /*! IDRIVEN_LS = 0.020A */
  ISink_LS_0p020_A = (0 << 0),  
  /*! IDRIVEN_LS = 0.030A */
  ISink_LS_0p060_A = (1 << 0),  
  /*! IDRIVEN_LS = 0.040A */
  ISink_LS_0p120_A = (2 << 0),  
  /*! IDRIVEN_LS = 0.050A */
  ISink_LS_0p160_A = (3 << 0),  
  /*! IDRIVEN_LS = 0.060A */
  ISink_LS_0p240_A = (4 << 0),  
  /*! IDRIVEN_LS = 0.070A */
  ISink_LS_0p280_A = (5 << 0),  
  /*! IDRIVEN_LS = 0.125A */
  ISink_LS_0p340_A = (6 << 0),  
  /*! IDRIVEN_LS = 0.250A */
  ISink_LS_0p380_A = (7 << 0),  
  /*! IDRIVEN_LS = 0.500A */
  ISink_LS_0p520_A = (8 << 0),  
  /*! IDRIVEN_LS = 0.750A */
  ISink_LS_0p660_A = (9 << 0),  
  /*! IDRIVEN_LS = 1.000A */
  ISink_LS_0p740_A = (10 << 0), 
  /*! IDRIVEN_LS = 1.250A */
  ISink_LS_0p880_A = (11 << 0),  
  /*! IDRIVEN_LS = 1.250A */
  ISink_LS_1p140_A = (12 << 0),  
  /*! IDRIVEN_LS = 1.250A */
  ISink_LS_1p360_A = (13 << 0),  
  /*! IDRIVEN_LS = 1.250A */
  ISink_LS_1p640_A = (14 << 0), 
  /*! IDRIVEN_LS = 1.250A */ 
  ISink_LS_2p000_A = (15 << 0)  
} DRV8323_CTRL04_PeakSinkCurLS_e;


/*! @brief Enumeration for the VDS comparator threshold */
typedef enum 
{
  /*! VDS_LEVEL = 0.060V */
  VDS_Level_0p060_V = (0 << 0),    
  /*! VDS_LEVEL = 0.130V */
  VDS_Level_0p130_V = (1 << 0),    
  /*! VDS_LEVEL = 0.200V */
  VDS_Level_0p200_V = (2 << 0),    
  /*! VDS_LEVEL = 0.260V */
  VDS_Level_0p260_V = (3 << 0),    
  /*! VDS_LEVEL = 0.310V */
  VDS_Level_0p310_V = (4 << 0),    
  /*! VDS_LEVEL = 0.450V */
  VDS_Level_0p450_V = (5 << 0),    
  /*! VDS_LEVEL = 0.530V */
  VDS_Level_0p530_V = (6 << 0),    
  /*! VDS_LEVEL = 0.600V */
  VDS_Level_0p600_V = (7 << 0),    
  /*! VDS_LEVEL = 0.680V */
  VDS_Level_0p680_V = (8 << 0),    
  /*! VDS_LEVEL = 0.750V */
  VDS_Level_0p750_V = (9 << 0),    
  /*! VDS_LEVEL = 0.940V */
  VDS_Level_0p940_V = (10 << 0),   
  /*! VDS_LEVEL = 1.130V */
  VDS_Level_1p130_V = (11 << 0),   
  /*! VDS_LEVEL = 1.300V */
  VDS_Level_1p300_V = (12 << 0),   
  /*! VDS_LEVEL = 1.500V */
  VDS_Level_1p500_V = (13 << 0),   
  /*! VDS_LEVEL = 1.700V */
  VDS_Level_1p700_V = (14 << 0),   
  /*! VDS_LEVEL = 1.880V */
  VDS_Level_1p880_V = (15 << 0)    
} DRV8323_CTRL05_VDSLVL_e;


/*! @brief Enumeration for the OCP/VDS sense deglitch time */
typedef enum 
{
  /*! TVDS = 0us */
  VDSDeg_0_us = (0 << 4),       
  /*! TVDS = 2us */
  VDSDeg_2_us = (1 << 4),       
  /*! TVDS = 4us */
  VDSDeg_4_us = (2 << 4),       
  /*! TVDS = 8us */
  VDSDeg_8_us = (3 << 4)        
} DRV8323_CTRL05_OcpDeg_e;


/*! @brief Enumeration for the OCP report mode */
typedef enum 
{
  /*! OCP_MODE = Latched fault */
  Latched_Shutdown = (0 << 6),  
  /*! OCP_MODE = Automatic Retry */
  Automatic_Retry = (1 << 6),   
  /*! OCP_MODE = Report only */
  Report_Only  = (2 << 6),      
  /*! OCP_MODE = Disabled */
  Disable_OCP = (3 << 6)        
} DRV8323_CTRL05_OcpMode_e;


/*! @brief Enumeration for the driver dead time */
typedef enum 
{
  /*! DEAD_TIME = 50ns */
  DeadTime_50_ns = (0 << 8),    
  /*! DEAD_TIME = 100ns */
  DeadTime_100_ns = (1 << 8),   
  /*! DEAD_TIME = 200ns */
  DeadTime_200_ns = (2 << 8),   
  /*! DEAD_TIME = 400ns */
  DeadTime_400_ns = (3 << 8)    
} DRV8323_CTRL05_DeadTime_e;


/*! @brief Enumeration for the Sense OCP level */
typedef enum 
{
  /*! SEN_LVL = 0.25V */
  SEN_Lvl_Ocp_0p25 = (0 << 0),  
  /*! SEN_LVL = 0.50V */
  SEN_Lvl_Ocp_0p50 = (1 << 0),  
  /*! SEN_LVL = 0.75V */
  SEN_Lvl_Ocp_0p75 = (2 << 0),  
  /*! SEN_LVL = 1.00V */
  SEN_Lvl_Ocp_1p00 = (3 << 0)   
} DRV8323_CTRL06_SENLevel_e;


/*! @brief Enumeration for the gain of shunt amplifier */
typedef enum 
{
  /*! GAIN_CSA = 5V/V */
  Gain_5VpV =  (0 << 6),   
  /*! GAIN_CSA = 10V/V */
  Gain_10VpV = (1 << 6),   
  /*! GAIN_CSA = 20V/V */
  Gain_20VpV = (2 << 6),   
  /*! GAIN_CSA = 40V/V */
  Gain_40VpV = (3 << 6)    
} DRV8323_CTRL06_CSAGain_e;


/*! @brief Enumeration for the register addresses */
typedef enum 
{
  /*! Status Register 0 */
  Address_Status_0  = 0 << 11,   
  /*! Status Register 1 */
  Address_Status_1  = 1 << 11,   
  /*! Control Register 2 */
  Address_Control_2 = 2 << 11,   
  /*! Control Register 3 */
  Address_Control_3 = 3 << 11,   
  /*! Control Register 4 */
  Address_Control_4 = 4 << 11,   
  /*! Control Register 5 */
  Address_Control_5 = 5 << 11,   
  /*! Control Register 6 */
  Address_Control_6 = 6 << 11    
} DRV8323_Address_e;


/*! @brief Object for the DRV8323 STATUS00 register */
typedef struct _DRV_SPI_8323_Stat00_t_
{
  /*! Bits 0 */
  _Bool                  VDS_LC;         
  /*! Bits 1 */
  _Bool                  VDS_HC;         
  /*! Bits 2 */
  _Bool                  VDS_LB;         
  /*! Bits 3 */
  _Bool                  VDS_HB;         
  /*! Bits 4 */
  _Bool                  VDS_LA;         
  /*! Bits 5 */
  _Bool                  VDS_HA;         
  /*! Bits 6 */
  _Bool                  OTSD;           
  /*! Bits 7 */
  _Bool                  UVLO;           
  /*! Bits 8 */
  _Bool                  GDF;            
  /*! Bits 9 */
  _Bool                  VDS_OCP;        
  /*! Bits 10 */
  _Bool                  FAULT;          
}DRV_SPI_8323_Stat00_t_;


/*! @brief Object for the DRV8323 STATUS01 register */
typedef struct _DRV_SPI_8323_Stat01_t_
{
  /*! Bits 0 */
  _Bool                  VGS_LC;         
  /*! Bits 1 */
  _Bool                  VGS_HC;         
  /*! Bits 2 */
  _Bool                  VGS_LB;         
  /*! Bits 3 */
  _Bool                  VGS_HB;         
  /*! Bits 4 */
  _Bool                  VGS_LA;         
  /*! Bits 5 */
  _Bool                  VGS_HA;         
  /*! Bits 6 */
  _Bool                  CPUV;           
  /*! Bits 7 */
  _Bool                  OTW;            
  /*! Bits 8 */
  _Bool                  SC_OC;          
  /*! Bits 9 */
  _Bool                  SB_OC;          
  /*! Bits 10 */
  _Bool                  SA_OC;          
}DRV_SPI_8323_Stat01_t_;


/*! @brief Object for the DRV8323 CTRL02 register */
typedef struct _DRV_SPI_8323_Ctrl02_t_
{
  /*! Bits 0 */
  _Bool                          CLR_FLT;        
  /*! Bits 1 */
  _Bool                          BRAKE;          
  /*! Bits 2 */
  _Bool                          COAST;          
  /*! Bits 3 */
  _Bool                          PWM1_DIR;       
  /*! Bits 4 */
  _Bool                          PWM1_COM;       
  /*! Bits 6-5 */
  DRV8323_CTRL02_PwmMode_e       PWM_MODE;       
  /*! Bits 7 */
  _Bool                          OTW_REP;        
  /*! Bits 8 */
  _Bool                          DIS_GDF;        
  /*! Bits 9 */
  _Bool                          DIS_CPUV;       
  /*! Bits 10 */
  _Bool                          CTRL02_RSV1;    
}DRV_SPI_8323_Ctrl02_t_;


/*! @brief Object for the DRV8323 CTRL03 register */
typedef struct _DRV_SPI_8323_Ctrl03_t_
{
  /*! Bits 3-0 */
  DRV8323_CTRL03_PeakSinkCurHS_e    IDRIVEN_HS;     
  /*! Bits 7-4 */
  DRV8323_CTRL03_PeakSourCurHS_e    IDRIVEP_HS;     
  /*! Bits 10-8 */
  DRV8323_CTRL03_Lock_e             LOCK;           
}DRV_SPI_8323_Ctrl03_t_;


/*! @brief Object for the DRV8323 CTRL04 register */
typedef struct _DRV_SPI_8323_Ctrl04_t_
{
  /*! Bits 3-0 */
  DRV8323_CTRL04_PeakSinkCurLS_e    IDRIVEN_LS;     
  /*! Bits 7-4 */
  DRV8323_CTRL04_PeakSourCurLS_e    IDRIVEP_LS;     
  /*! Bits 9-8 */
  DRV8323_CTRL04_PeakTime_e         TDRIVE;         
  /*! Bits 10 */
  _Bool                             CBC;            
}DRV_SPI_8323_Ctrl04_t_;


/*! @brief Object for the DRV8323 CTRL05 register */
typedef struct _DRV_SPI_8323_Ctrl05_t_
{
  /*! Bits 3-0 */
  DRV8323_CTRL05_VDSLVL_e           VDS_LVL;        
  /*! Bits 5-4 */
  DRV8323_CTRL05_OcpDeg_e           OCP_DEG;        
  /*! Bits 7-6 */
  DRV8323_CTRL05_OcpMode_e          OCP_MODE;       
  /*! Bits 9-8 */
  DRV8323_CTRL05_DeadTime_e         DEAD_TIME;      
  /*! Bits 10 */
  _Bool                             TRETRY;         
}DRV_SPI_8323_Ctrl05_t_;


/*! @brief Object for the DRV8323 CTRL06 register */
typedef struct _DRV_SPI_8323_Ctrl06_t_
{
  /*! Bits 1-0 */
  DRV8323_CTRL06_SENLevel_e     SEN_LVL;        
  /*! Bits 2 */
  _Bool 						CSA_CAL_C;   	
  /*! Bits 3 */
  _Bool 						CSA_CAL_B;   	
  /*! Bits 4 */
  _Bool 						CSA_CAL_A;   	
  /*! Bits 5 */
  _Bool 						DIS_SEN; 	  	
  /*! Bits 7-6 */
  DRV8323_CTRL06_CSAGain_e      CSA_GAIN;       
  /*! Bits 8 */
  _Bool                         LS_REF;	        
  /*! Bits 9 */
  _Bool                         VREF_DIV;       
  /*! Bits 10 */
  _Bool                         CSA_FET;        
}DRV_SPI_8323_Ctrl06_t_;

/*! @brief Object for the DRV8323 registers and commands */
typedef struct _DRV_SPI_8323_Vars_t_
{
  /*! Status reg 1 */
  DRV_SPI_8323_Stat00_t_    Stat_Reg_00;
  /*! Status reg 2 */
  DRV_SPI_8323_Stat01_t_    Stat_Reg_01;
  /*! Ctrl reg 1 */
  DRV_SPI_8323_Ctrl02_t_    Ctrl_Reg_02;
  /*! Ctrl reg 2 */
  DRV_SPI_8323_Ctrl03_t_    Ctrl_Reg_03;
  /*! Ctrl reg 3 */
  DRV_SPI_8323_Ctrl04_t_    Ctrl_Reg_04;
  /*! Ctrl reg 4 */
  DRV_SPI_8323_Ctrl05_t_    Ctrl_Reg_05;
  /*! Ctrl reg 5 */
  DRV_SPI_8323_Ctrl06_t_    Ctrl_Reg_06;
} DRV_SPI_8323_Vars_t;

//! \brief Defines the DRV8323 Word type
//!
typedef  uint16_t    DRV8323_Word_t;

/*! @brief DRV823_CFG1 register */
typedef union
{
    struct DRV8323Cfg1
    {
        uint32_t
        /*! LS Idrive N setting */
        iDriveNLS:          4,
        /*! VDS Level setting */
        vdsLevel:           4,
        /*! OCP Deglitch setting */
        ocpDeg:             2,
        /*! OCP Mode setting */
        ocpMode:            2,
        /*! Dead Time setting */
        deadTime:           2,
        /*! retry Time setting */
        tRetry:             1,
        /*! Sense Level setting */
        senLvl:             2,
        /*! CSA CAL - Ph C Config */
        csaCalC:            1,
        /*! CSA CAL - Ph B Config */
        csaCalB:            1,
        /*! CSA CAL - Ph A Config */
        csaCalA:            1,
        /*! Disable Current Sense Config */
        disSen:             1,
        /*! CSA Gain Config */
        csaGain:            2,
        /*! VDS LS Reference Configuration*/
        lsRef:              1,
        /*! Vref Level Config */
        vrefDiv:            1,
        /*! CSA Pos Ref Config */
        csaFET:             1,
        /*! Reserved */
        reserved1:          5,
        parity:             1;

   }b;
    uint32_t w;
}GATE_DRIVE_8323_CFG1_T;

/*! @brief DRV823_CFG2 register */
typedef union
{
    struct DRV8323Cfg2
    {
        uint32_t
        /*! Gate Driver Clear Fault Bit */
        clrFLT:             1,
        /*! Built In Brake Setting - Masked by Algo */
        brake:              1,
        /*! Built In Coast Setting - Masked by Algo */
        coast:              1,
        /*! Built In PWM Dir Setting - Masked by Algo */
        pwmDir:             1,
        /*! Built In PWM Comm Setting - Masked by Algo */
        pwmComm:            1,
        /*! Built In PWM Mode Setting - Masked by Algo */
        pwmMode:            2,
        /*! Over Temperature warning report */
        repOTW:             1,
        /*! Disable Gate Driver */
        disGDF:             1,
        /*! Disable Charge Pump Under Voltage Fault*/
        disCPUV:            1,
        /*! Reserved */
        reg2Rsvd:           1,
        /*! iDrive N HS Setting */
        iDriveNHS:          4,
        /*! iDrive P HS Setting */
        iDrivePHS:          4,
        /*! Built In Lock Setting - Masked by Algo */
        lock:               3,
        /*! iDrive P LS Setting */
        iDrivePLS:          4,
        /*! TDrive Setting */
        tDrive:             2,
        /*! CBC Fault Setting */
        cbc:                1,
        /*! Reserved */
        reserved:           2,
        parity:             1;
   }b;
    uint32_t w;
}GATE_DRIVE_8323_CFG2_T;

/*/*! @brief Interface Registers to configure Gate Driver DRV8323 */
typedef struct hvDie8323EEPROMInterface
{
    /*! gate driver config 1 */
    GATE_DRIVE_8323_CFG1_T    gateDrvCfg1;
    /*! gate driver config 2 */
    GATE_DRIVE_8323_CFG2_T    gateDrvCfg2;

} HV_DIE_EEPROM_INTERFACE_T;

extern HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig;

/**
 * @brief     Read fault status from DRV8323
 * @return    Returns fault status
 */
uint32_t drv8323StatusRead(void);

/******************************************************************************/
/*! @brief GD1 REG6 mask */
#define GD1_REG6_MASK         0x07FF8000

/*! @brief GD1 REG6 POS in gate driver register */
#define GD1_REG6_POS       15

/*! @brief REG6 write protect mask */
#define DRV_REG6_WRITE_PROTECT_MASK 0x61C

/******************************************************************************/
/*! @brief GD1 REG5 mask */
#define GD1_REG5_MASK         0x00007FF0

/*! @brief GD1 REG5 POS in gate driver register */
#define GD1_REG5_POS          4

/*! @brief REG5 write protect mask */
#define DRV_REG5_WRITE_PROTECT_MASK 0x00000000

/******************************************************************************/
/*! @brief GD2 REG4 mask */
#define GD2_REG4_MASK               0x1FC00000
/*! @brief GD1 REG4 mask */
#define GD1_REG4_MASK               0x0000000F

/*! @brief GD2 REG4 POS in gate driver register */
#define GD2_REG4_POS             22

/*! @brief GD2 REG4 POS in DRV register */
#define GD2_REG4_DRV_POS         4

/*! @brief GD1 REG4 POS in gate driver register */
#define GD1_REG4_POS             0

/*! @brief REG4 write protect mask */
#define DRV_REG4_WRITE_PROTECT_MASK 0x00000400

/******************************************************************************/
/*! @brief GD2 REG3 mask */
#define GD2_REG3_MASK               0x003FF800

/*! @brief GD2 REG3 POS in gate driver register */
#define GD2_REG3_POS             11

/*! @brief REG3 write protect mask */
#define DRV_REG3_WRITE_PROTECT_MASK 0x00000700

/******************************************************************************/
/*! @brief GD2 REG2 mask */
#define GD2_REG2_MASK         0x000007FF

/*! @brief GD2 REG2 POS in gate driver register */
#define GD2_REG2_POS       0

/*! @brief REG2 write protect mask */
#define DRV_REG2_WRITE_PROTECT_MASK 0x47F

/******************************************************************************/

/******************************************************************************/
/*! @brief Gate driver UVLO fault MASK */
#define GD_UVLO_FAULT_MASK      0x10000000
/*! @brief Gate driver UVLO fault POS */
#define GD_UVLO_FAULT_POS       28U

/*! @brief Gate driver OTSD fault MASK */
#define GD_OTSD_FAULT_MASK      0x08000000
/*! @brief Gate driver OTSD fault POS */
#define GD_OTSD_FAULT_POS       27U

/******************************************************************************/

#endif
#ifdef __cplusplus
}
#endif
#endif /*! DRV8323_H */
