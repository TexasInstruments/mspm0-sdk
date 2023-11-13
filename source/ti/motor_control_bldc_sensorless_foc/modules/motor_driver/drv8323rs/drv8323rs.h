/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 *  @file       drv8323rs.h
 *  @brief      Sensorless FOC Motor Control Library DRV8323RS Motor Driver 
 * Module
 *
 *
 *  @anchor drv8323rs_h
 *  # Overview
 *
 *  APIs for DRV8323RS
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup MD_MODULE Motor_Driver/DRV8323RS
 * @{
 */
#ifndef DRV8323RS_H
#define DRV8323RS_H

#include "hal.h"
#include <stdlib.h>

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Delay to wait for the SPI to be ready when the DRV is powered on
 */
#define DRV8323RS_SPI_READY_DELAY_MS                    (1)

/*!
 * @brief Delay to wait to enter sleep mode after enable is low
 */
#define DRV8323RS_TURNOFF_DELAY_MS                      (1)

/*!
 * @brief Dummmy data that is written to read the spi register 
 */
#define DRV8323RS_SPI_DUMMY_DATA                        (0x00)

/*!
 * @brief Mask for the 11 bit spi data
 */
#define DRV8323RS_SPI_DATA_MASK                        (0x7FF)

/*! @brief Defines the offset of the VDS_LC bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_LC_OFS             (0)

/*! @brief Defines the offset of the VDS_HC bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_HC_OFS             (1)

/*! @brief Defines the offset of the VDS_LB bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_LB_OFS             (2)

/*! @brief Defines the offset of the VDS_HB bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_HB_OFS             (3)

/*! @brief Defines the offset of the VDS_LA bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_LA_OFS             (4)

/*! @brief Defines the offset of the VDA_HA bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_HA_OFS             (5)

/*! @brief Defines the offset of the OTSD bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_OTSD_OFS               (6)

/*! @brief Defines the offset of the UVLO bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_UVLO_OFS               (7)

/*! @brief Defines the offset of the GDF bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_GDF_OFS                (8)

/*! @brief Defines the offset of the VDS_OCP bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_OCP_OFS            (9)

/*! @brief Defines the offset of the FAULT bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_FAULT_OFS              (10)

/*! @brief Defines the mask of the VDS_LC bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_LC_MASK      \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_LC_OFS)

/*! @brief Defines the mask of the VDS_HC bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_HC_MASK      \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_HC_OFS)

/*! @brief Defines the mask of the VDS_LB bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_LB_MASK      \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_LB_OFS)

/*! @brief Defines the mask of the VDS_HB bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_HB_MASK      \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_HB_OFS)

/*! @brief Defines the mask of the VDS_LA bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_LA_MASK      \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_LA_OFS)

/*! @brief Defines the mask of the VDA_HA bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_HA_MASK      \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_HA_OFS)

/*! @brief Defines the mask of the OTSD bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_OTSD_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_1_OTSD_OFS)

/*! @brief Defines the mask of the UVLO bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_UVLO_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_1_UVLO_OFS)

/*! @brief Defines the mask of the GDF bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_GDF_MASK         \
                                    (1 << DRV8323RS_FAULT_STATUS_1_GDF_OFS)

/*! @brief Defines the mask of the VDS_OCP bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_VDS_OCP_MASK     \
                                    (1 << DRV8323RS_FAULT_STATUS_1_VDS_OCP_OFS)

/*! @brief Defines the mask of the FAULT bit in the Status 1 register */
#define DRV8323RS_FAULT_STATUS_1_FAULT_MASK       \
                                    (1 << DRV8323RS_FAULT_STATUS_1_FAULT_OFS)               

/*! @brief Defines the offset of the VGS_LC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_LC_OFS               (0)

/*! @brief Defines the offset of the VGS_HC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_HC_OFS               (1)

/*! @brief Defines the offset of the VGS_LB bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_LB_OFS               (2)

/*! @brief Defines the offset of the VGS_HB bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_HB_OFS               (3)

/*! @brief Defines the offset of the VGS_LA bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_LA_OFS               (4)

/*! @brief Defines the offset of the VGS_HA bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_HA_OFS               (5)

/**
 * @brief Defines the offset of the CPUV (charge pump undervoltage) bit in
 * the Status 2 register
 */
#define DRV8323RS_FAULT_STATUS_2_CPUV_OFS                 (6)

/*! @brief Defines the offset of the OTW bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_OTW_OFS                  (7)

/*! @brief Defines the offset of the SC_OC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_SC_OC_OFS                (8)

/*! @brief Defines the offset of the SB_OC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_SB_OC_OFS                (9)

/*! @brief Defines the offset of the SA_OC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_SA_OC_OFS                (10)

/*! @brief Defines the mask of the VGS_LC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_LC_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_2_VGS_LC_OFS)

/*! @brief Defines the mask of the VGS_HC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_HC_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_2_VGS_HC_OFS)

/*! @brief Defines the mask of the VGS_LB bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_LB_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_2_VGS_LB_OFS)

/*! @brief Defines the mask of the VGS_HB bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_HB_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_2_VGS_HB_OFS)

/*! @brief Defines the mask of the VGS_LA bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_LA_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_2_VGS_LA_OFS)

/*! @brief Defines the mask of the VGS_HA bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_VGS_HA_MASK        \
                                    (1 << DRV8323RS_FAULT_STATUS_2_VGS_HA_OFS)

/**
 * @brief Defines the mask of the CPUV (charge pump undervoltage) bit in
 * the Status 2 register
 */
#define DRV8323RS_FAULT_STATUS_2_CPUV_MASK          \
                                    (1 << DRV8323RS_FAULT_STATUS_2_CPUV_OFS)

/*! @brief Defines the mask of the OTW bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_OTW_MASK           \
                                    (1 << DRV8323RS_FAULT_STATUS_2_OTW_OFS)

/*! @brief Defines the mask of the SC_OC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_SC_OC_MASK         \
                                    (1 << DRV8323RS_FAULT_STATUS_2_SC_OC_OFS)

/*! @brief Defines the mask of the SB_OC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_SB_OC_MASK         \
                                    (1 << DRV8323RS_FAULT_STATUS_2_SB_OC_OFS)

/*! @brief Defines the mask of the SA_OC bit in the Status 2 register */
#define DRV8323RS_FAULT_STATUS_2_SA_OC_MASK         \
                                    (1 << DRV8323RS_FAULT_STATUS_2_SA_OC_OFS)

/*! @brief Defines the offset of the CLR_FLT bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_CLR_FLT_OFS            (0)

/*! @brief Defines the offset of the BRAKE bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_BRAKE_OFS              (1)

/*! @brief Defines the offset of the COAST bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_COAST_OFS              (2)

/*! @brief Defines the offset of the 1PWM_DIR bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_PWM1_DIR_OFS           (3)

/*! @brief Defines the offset of the 1PWM_COM bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_PWM1_COM_OFS           (4)

/*! @brief Defines the offset of the PWM_MODE bits in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_PWM_MODE_OFS           (5)

/*! @brief Defines the offset of the OTW_REP bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_OTW_REP_OFS            (7)

/*! @brief Defines the offset of the DIS_GDF bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_DIS_GDF_OFS            (8)

/*! @brief Defines the offset of the DIS_CPUV bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_DIS_CPUV_OFS           (9)

/*! @brief Defines the offset of the RESERVED1 bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_RESERVED1_OFS          (10)

/*! @brief Defines the mask of the CLR_FLT bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_CLR_FLT_MASK     \
                                  (1 << DRV8323RS_DRIVER_CONTROL_CLR_FLT_OFS)

/*! @brief Defines the mask of the BRAKE bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_BRAKE_MASK       \
                                  (1 << DRV8323RS_DRIVER_CONTROL_BRAKE_OFS)

/*! @brief Defines the mask of the COAST bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_COAST_MASK       \
                                  (1 << DRV8323RS_DRIVER_CONTROL_COAST_OFS)

/*! @brief Defines the mask of the 1PWM_DIR bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_PWM1_DIR_MASK    \
                                  (1 << DRV8323RS_DRIVER_CONTROL_PWM1_DIR_OFS)

/*! @brief Defines the mask of the 1PWM_COM bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_PWM1_COM_MASK    \
                                  (1 << DRV8323RS_DRIVER_CONTROL_PWM1_COM_OFS)

/*! @brief Defines the mask of the PWM_MODE bits in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_PWM_MODE_MASK    \
                                  (3 << DRV8323RS_DRIVER_CONTROL_PWM_MODE_OFS)

/*! @brief Defines the mask of the OTW_REP bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_OTW_REP_MASK     \
                                  (1 << DRV8323RS_DRIVER_CONTROL_OTW_REP_OFS)

/*! @brief Defines the mask of the DIS_GDF bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_DIS_GDF_MASK     \
                                  (1 << DRV8323RS_DRIVER_CONTROL_DIS_GDF_OFS)

/*! @brief Defines the mask of the DIS_CPUV bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_DIS_CPUV_MASK    \
                                  (1 << DRV8323RS_DRIVER_CONTROL_DIS_CPUV_OFS)

/*! @brief Defines the mask of the RESERVED1 bit in the Control 2 register */
#define DRV8323RS_DRIVER_CONTROL_RESERVED1_MASK   \
                                  (1 << DRV8323RS_DRIVER_CONTROL_RESERVED1_OFS)

/*! @brief Defines the offset of the IDRIVEN_HS bits in the Control 3 register
 */
#define DRV8323RS_GATE_DRIVE_HS_IDRIVEN_HS_OFS          (0)

/*! @brief Defines the offset of the IDRIVEP_HS bits in the Control 3 register
 */
#define DRV8323RS_GATE_DRIVE_HS_IDRIVEP_HS_OFS          (4)

/*! @brief Defines the offset of the LOCK bits in the Control 3 register */
#define DRV8323RS_GATE_DRIVE_HS_LOCK_OFS                (8)

/*! @brief Defines the mask of the IDRIVEN_HS bits in the Control 3 register */
#define DRV8323RS_GATE_DRIVE_HS_IDRIVEN_HS_MASK   \
                                  (15 << DRV8323RS_GATE_DRIVE_HS_IDRIVEN_HS_OFS)

/*! @brief Defines the mask of the IDRIVEP_HS bits in the Control 3 register */
#define DRV8323RS_GATE_DRIVE_HS_IDRIVEP_HS_MASK   \
                                  (15 << DRV8323RS_GATE_DRIVE_HS_IDRIVEP_HS_OFS)

/*! @brief Defines the mask of the LOCK bits in the Control 3 register */
#define DRV8323RS_GATE_DRIVE_HS_LOCK_MASK         \
                                  (7 << DRV8323RS_GATE_DRIVE_HS_LOCK_OFS)

/*! @brief Defines the offset of the IDRIVEN_LS bits in the Control 4 register
 */
#define DRV8323RS_GATE_DRIVE_LS_IDRIVEN_LS_OFS          (0)

/*! @brief Defines the offset of the IDRIVEP_LS bits in the Control 4 register
 */
#define DRV8323RS_GATE_DRIVE_LS_IDRIVEP_LS_OFS          (4)

/*! @brief Defines the offset of the TDRIVE bits in the Control 4 register */
#define DRV8323RS_GATE_DRIVE_LS_TDRIVE_OFS              (8)

/*! @brief Defines the offset of the CBC bit in the Control 4 register */
#define DRV8323RS_GATE_DRIVE_LS_CBC_OFS                 (10)

/*! @brief Defines the mask of the IDRIVEN_LS bits in the Control 4 register */
#define DRV8323RS_GATE_DRIVE_LS_IDRIVEN_LS_MASK   \
                                  (15 << DRV8323RS_GATE_DRIVE_LS_IDRIVEN_LS_OFS)

/*! @brief Defines the mask of the IDRIVEP_LS bits in the Control 4 register */
#define DRV8323RS_GATE_DRIVE_LS_IDRIVEP_LS_MASK   \
                                  (15 << DRV8323RS_GATE_DRIVE_LS_IDRIVEP_LS_OFS)

/*! @brief Defines the mask of the TDRIVE bits in the Control 4 register */
#define DRV8323RS_GATE_DRIVE_LS_TDRIVE_MASK       \
                                  (3 << DRV8323RS_GATE_DRIVE_LS_TDRIVE_OFS)

/*! @brief Defines the mask of the CBC bit in the Control 4 register */
#define DRV8323RS_GATE_DRIVE_LS_CBC_MASK          \
                                  (1 << DRV8323RS_GATE_DRIVE_LS_CBC_OFS)

/*! @brief Defines the offset of the VDS_LVL bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_VDS_LVL_OFS               (0)

/*! @brief Defines the offset of the OCP_DEG bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_OCP_DEG_OFS               (4)

/*! @brief Defines the offset of the OCP_MODE bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_OCP_MODE_OFS              (6)

/*! @brief Defines the offset of the DEAD_TIME bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_DEAD_TIME_OFS             (8)

/*! @brief Defines the offset of the TRETRY bit in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_TRETRY_OFS                (10)

/*! @brief Defines the mask of the VDS_LVL bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_VDS_LVL_MASK        \
                                      (15 << DRV8323RS_OCP_CONTROL_VDS_LVL_OFS)

/*! @brief Defines the mask of the OCP_DEG bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_OCP_DEG_MASK        \
                                      (3 << DRV8323RS_OCP_CONTROL_OCP_DEG_OFS)

/*! @brief Defines the mask of the OCP_MODE bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_OCP_MODE_MASK       \
                                      (3 << DRV8323RS_OCP_CONTROL_OCP_MODE_OFS)

/*! @brief Defines the mask of the DEAD_TIME bits in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_DEAD_TIME_MASK      \
                                      (3 << DRV8323RS_OCP_CONTROL_DEAD_TIME_OFS)

/*! @brief Defines the mask of the TRETRY bit in the Control 5 register */
#define DRV8323RS_OCP_CONTROL_TRETRY_MASK         \
                                      (1 << DRV8323RS_OCP_CONTROL_TRETRY_OFS)

/*! @brief Defines the offset of the SEN_LVL bits in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_SEN_LVL_OFS               (0)

/*! @brief Defines the offset of the CSA_CAL_C bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_CAL_C_OFS             (2)

/*! @brief Defines the offset of the CSA_CAL_B bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_CAL_B_OFS             (3)

/*! @brief Defines the offset of the CSA_CAL_A bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_CAL_A_OFS             (4)

/*! @brief Defines the offset of the DIS_SEN bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_DIS_SEN_OFS               (5)

/*! @brief Defines the offset of the CSA_GAIN bits in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_GAIN_OFS              (6)

/*! @brief Defines the offset of the LS_REF bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_LS_REF_OFS                (8)

/*! @brief Defines the offset of the VREF_DIV bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_VREF_DIV_OFS              (9)

/*! @brief Defines the offset of the CSA_FET bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_FET_OFS               (10)

/*! @brief Defines the mask of the SEN_LVL bits in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_SEN_LVL_MASK        \
                                    (3 << DRV8323RS_CSA_CONTROL_SEN_LVL_OFS)

/*! @brief Defines the mask of the CSA_CAL_C bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_CAL_C_MASK      \
                                    (1 << DRV8323RS_CSA_CONTROL_CSA_CAL_C_OFS)

/*! @brief Defines the mask of the CSA_CAL_B bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_CAL_B_MASK      \
                                    (1 << DRV8323RS_CSA_CONTROL_CSA_CAL_B_OFS)

/*! @brief Defines the mask of the CSA_CAL_A bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_CAL_A_MASK      \
                                    (1 << DRV8323RS_CSA_CONTROL_CSA_CAL_A_OFS)

/*! @brief Defines the mask of the DIS_SEN bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_DIS_SEN_MASK        \
                                    (1 << DRV8323RS_CSA_CONTROL_DIS_SEN_OFS)

/*! @brief Defines the mask of the CSA_GAIN bits in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_GAIN_MASK       \
                                    (3 << DRV8323RS_CSA_CONTROL_CSA_GAIN_OFS)

/*! @brief Defines the mask of the LS_REF bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_LS_REF_MASK         \
                                    (1 << DRV8323RS_CSA_CONTROL_LS_REF_OFS)

/*! @brief Defines the mask of the VREF_DIV bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_VREF_DIV_MASK       \
                                    (1 << DRV8323RS_CSA_CONTROL_VREF_DIV_OFS)

/*! @brief Defines the mask of the CSA_FET bit in the Control 6 register */
#define DRV8323RS_CSA_CONTROL_CSA_FET_MASK        \
                                    (1 << DRV8323RS_CSA_CONTROL_CSA_FET_OFS)

/*! @brief Defines the offset for SPI command bit */
#define DRV8323RS_SPI_CMD_OFS             (15)

/*! @brief Defines the offset for SPI register address */
#define DRV8323RS_SPI_ADDR_OFS            (11)

/*! @brief Defines the mask for SPI register address */
#define DRV8323RS_SPI_ADDR_MASK           (0xF << DRV8323RS_SPI_ADDR_OFS)

/** @brief ADC voltage to dc bus voltage scale factor 
 * This is the ratio ((R1 + R2) / R2), where R1 and R2 forms the potential
 *  divider circuit scaling the dc bus voltage to the range of input for the adc
 * Refer to the DRV8323RS schematics for more understanding
*/
#define DRV8323RS_DEFAULT_VOLT_R_DIVIDER  ((82+4.99)/4.99)

/*! @brief Voltage scale factor for vm, in iq15 value */
#define DRV8323RS_VOLTAGE_SF_IQ15        _IQ15(DRV8323RS_DEFAULT_VOLT_R_DIVIDER)

/*! @brief Maximum number of gains configurable for the drv */
#define DRV8323RS_CSA_GAIN_MAX_VALUES      (4)

/** @brief Default shunt resistor in the drv */
#define DRV8323RS_DEFAULT_R_SHUNT         (0.007f)

/** @brief Default Deadband for the drv */
#define DRV8323RS_DEFAULT_PWM_DEADBAND_NS (100)

/*!
 * @enum DRV8323RS_SPI_CMD
 * @brief SPI command modes
 */
typedef enum {
    /*! SPI write command */
    DRV8323RS_SPI_CMD_WRITE = 0,
    /*! SPI read command */
    DRV8323RS_SPI_CMD_READ,
}DRV8323RS_SPI_CMD;

/*!
 * @enum DRV8323RS_GATE_DRIVE_HS_LOCK
 * @brief Enumeration for locking and unlocking the SPI registers
 */
typedef enum {
    /*! Unlock value */
    DRV8323RS_GATE_DRIVE_HS_LOCK_UNLOCK = 3,
    /*! Lock value */
    DRV8323RS_GATE_DRIVE_HS_LOCK_LOCK = 6
}DRV8323RS_GATE_DRIVE_HS_LOCK;

/*!
 * @enum DRV8323RS_DRIVER_CONTROL_PWM_MODE
 * @brief Enumeration for the driver PWM mode
 */
typedef enum {
  /*! PWM_MODE value is 6 inputs */
  DRV8323RS_DRIVER_CONTROL_PWM_MODE_6 = 0, 
  /*! PWM_MODE value is 3 inputs */
  DRV8323RS_DRIVER_CONTROL_PWM_MODE_3, 
  /*! PWM_MODE value is 1 input  */
  DRV8323RS_DRIVER_CONTROL_PWM_MODE_1 
} DRV8323RS_DRIVER_CONTROL_PWM_MODE;

/*!
 * @enum DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME
 * @brief Enumeration for the peak current gate drive time
 */
typedef enum {
  /*! TDRIVEN value is  500ns */
  DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME_500_NS =0,  
  /*! TDRIVEN value is 1000ns */
  DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME_1000_NS, 
  /*! TDRIVEN value is 2000ns */
  DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME_2000_NS, 
  /*! TDRIVEN value is 4000ns */
  DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME_4000_NS  
} DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME;

/*! 
 * @enum DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR
 * @brief Enumeration for the low side gate drive peak source current
 */
typedef enum {
  /*! IDRIVEP_LS value is 0.010A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P010_A =0,  
  /*! IDRIVEP_LS value is 0.030A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P030_A,  
  /*! IDRIVEP_LS value is 0.060A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P060_A,  
  /*! IDRIVEP_LS value is 0.080A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P080_A,  
  /*! IDRIVEP_LS value is 0.120A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P120_A,  
  /*! IDRIVEP_LS value is 0.140A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P140_A,  
  /*! IDRIVEP_LS value is 0.170A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P170_A,  
  /*! IDRIVEP_LS value is 0.190A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P190_A,  
  /*! IDRIVEP_LS value is 0.260A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P260_A,  
  /*! IDRIVEP_LS value is 0.330A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P330_A,  
  /*! IDRIVEP_LS value is 0.370A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P370_A, 
  /*! IDRIVEP_LS value is 0.440A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P440_A, 
  /*! IDRIVEP_LS value is 0.570A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P570_A, 
  /*! IDRIVEP_LS value is 0.680A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P680_A, 
  /*! IDRIVEP_LS value is 0.820A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_0P820_A, 
  /*! IDRIVEP_LS value is 1.000A */
  DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR_1P000_A, 
} DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR;

/*!
 * @enum DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR
 * @brief Enumeration for the low side gate drive peak sink current
 */
typedef enum {
  /*! IDRIVEN_LS value is 0.020A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P020_A = 0,  
  /*! IDRIVEN_LS value is 0.060A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P060_A,  
  /*! IDRIVEN_LS value is 0.120A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P120_A,  
  /*! IDRIVEN_LS value is 0.160A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P160_A,  
  /*! IDRIVEN_LS value is 0.240A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P240_A,  
  /*! IDRIVEN_LS value is 0.280A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P280_A,  
  /*! IDRIVEN_LS value is 0.340A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P340_A,  
  /*! IDRIVEN_LS value is 0.380A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P380_A,  
  /*! IDRIVEN_LS value is 0.520A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P520_A,  
  /*! IDRIVEN_LS value is 0.660A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P660_A,  
  /*! IDRIVEN_LS value is 0.740A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P740_A, 
  /*! IDRIVEN_LS value is 0.880A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_0P880_A, 
  /*! IDRIVEN_LS value is 1.140A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_1P140_A, 
  /*! IDRIVEN_LS value is 1.360A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_1P360_A, 
  /*! IDRIVEN_LS value is 1.640A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_1P640_A, 
  /*! IDRIVEN_LS value is 2.000A */
  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR_2P000_A, 
} DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR;

/*!
 * @enum DRV8323RS_OCP_CONTROL_VDS_LVL
 * @brief Enumeration for the VDS comparator threshold
 */
typedef enum {
  /*! VDS_LEVEL value is 0.060V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P060_V = 0,  
  /*! VDS_LEVEL value is 0.130V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P130_V,  
  /*! VDS_LEVEL value is 0.200V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P200_V,  
  /*! VDS_LEVEL value is 0.260V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P260_V,  
  /*! VDS_LEVEL value is 0.310V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P310_V,  
  /*! VDS_LEVEL value is 0.450V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P450_V,  
  /*! VDS_LEVEL value is 0.530V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P530_V,  
  /*! VDS_LEVEL value is 0.600V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P600_V,  
  /*! VDS_LEVEL value is 0.680V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P680_V,  
  /*! VDS_LEVEL value is 0.750V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P750_V,  
  /*! VDS_LEVEL value is 0.940V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_0P940_V, 
  /*! VDS_LEVEL value is 1.130V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_1P130_V, 
  /*! VDS_LEVEL value is 1.300V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_1P300_V, 
  /*! VDS_LEVEL value is 1.500V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_1P500_V, 
  /*! VDS_LEVEL value is 1.700V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_1P700_V, 
  /*! VDS_LEVEL value is 1.880V */
  DRV8323RS_OCP_CONTROL_VDS_LVL_1P880_V  
} DRV8323RS_OCP_CONTROL_VDS_LVL;

/*!
 * @enum DRV8323RS_OCP_CONTROL_OCP_DEG
 * @brief Enumeration for the OCP/VDS sense deglitch time
 */
typedef enum {
  /*! TVDS value is 2us */
  DRV8323RS_OCP_CONTROL_OCP_DEG_2_US = 0, 
  /*! TVDS value is 4us */
  DRV8323RS_OCP_CONTROL_OCP_DEG_4_US, 
  /*! TVDS value is 6us */
  DRV8323RS_OCP_CONTROL_OCP_DEG_6_US, 
  /*! TVDS value is 8us */
  DRV8323RS_OCP_CONTROL_OCP_DEG_8_US  
} DRV8323RS_OCP_CONTROL_OCP_DEG;

/*!
 * @enum DRV8323RS_OCP_CONTROL_OCP_MODE
 * @brief Enumeration for the OCP report mode
 */
typedef enum {
  /*! OCP_MODE is Latched fault */
  DRV8323RS_OCP_CONTROL_OCP_MODE_LATCHED_SHUTDOWN = 0, 
  /*! OCP_MODE is Automatic Retry */
  DRV8323RS_OCP_CONTROL_OCP_MODE_AUTOMATIC_RETRY,  
  /*! OCP_MODE is Report only */
  DRV8323RS_OCP_CONTROL_OCP_MODE_REPORT_ONLY,      
  /*! OCP_MODE is Disabled */
  DRV8323RS_OCP_CONTROL_OCP_MODE_DISABLE_OCP       
} DRV8323RS_OCP_CONTROL_OCP_MODE;

/*!
 * @enum DRV8323RS_OCP_CONTROL_DEADTIME
 * @brief Enumeration for the deadtime
 */
typedef enum {
  /*! DEAD_TIME value is 50ns */
  DRV8323RS_OCP_CONTROL_DEADTIME_50_NS = 0,  
  /*! DEAD_TIME value is 100ns */
  DRV8323RS_OCP_CONTROL_DEADTIME_100_NS, 
  /*! DEAD_TIME value is 200ns */
  DRV8323RS_OCP_CONTROL_DEADTIME_200_NS, 
  /*! DEAD_TIME value is 400ns */
  DRV8323RS_OCP_CONTROL_DEADTIME_400_NS  
} DRV8323RS_OCP_CONTROL_DEADTIME;

/*!
 * @enum DRV8323RS_OCP_CONTROL_TRETRY
 * @brief Enumeration for the retry time
 */
typedef enum {
  /*! Retry time value is 4ms */
  DRV8323RS_OCP_CONTROL_TRETRY_4_MS = 0,
  /*! Retry time value is 50us */
  DRV8323RS_OCP_CONTROL_TRETRY_50_US,
} DRV8323RS_OCP_CONTROL_TRETRY;

/*!
 * @enum DRV8323RS_CSA_CONTROL_DIS_SEN
 * @brief Enumeration for sense of overcurrent */
typedef enum {
  /*! Overcurrent fault enabled */
  DRV8323RS_CSA_CONTROL_DIS_SEN_ENABLE = 0,
  /*! Overcurrent fault disabled */
  DRV8323RS_CSA_CONTROL_DIS_SEN_DISABLE,
} DRV8323RS_CSA_CONTROL_DIS_SEN;

/*!
 * @enum DRV8323RS_CSA_CONTROL_SEN_LVL
 * @brief Enumeration for the sense OCP level
 */
typedef enum {
  /*! SEN_LVL value is 0.25V */
  DRV8323RS_CSA_CONTROL_SEN_LVL_0P25_V = 0, 
  /*! SEN_LVL value is 0.50V */
  DRV8323RS_CSA_CONTROL_SEN_LVL_0P50_V, 
  /*! SEN_LVL value is 0.75V */
  DRV8323RS_CSA_CONTROL_SEN_LVL_0P75_V, 
  /*! SEN_LVL value is 1.00V */
  DRV8323RS_CSA_CONTROL_SEN_LVL_1P00_V  
} DRV8323RS_CSA_CONTROL_SEN_LVL;

/*!
 * @enum DRV8323RS_CSA_CONTROL_CSA_GAIN
 * @brief Enumeration for the gain of CSA
 */
typedef enum {
  /*! GAIN_CSA value is 5V/V */
  DRV8323RS_CSA_CONTROL_CSA_GAIN_5VPV = 0,
  /*! GAIN_CSA value is 10V/V */
  DRV8323RS_CSA_CONTROL_CSA_GAIN_10VPV,
  /*! GAIN_CSA value is 20V/V */
  DRV8323RS_CSA_CONTROL_CSA_GAIN_20VPV,
  /*! GAIN_CSA value is 40V/V */
  DRV8323RS_CSA_CONTROL_CSA_GAIN_40VPV,
} DRV8323RS_CSA_CONTROL_CSA_GAIN;

/*!
 * @enum DRV8323RS_CSA_CONTROL_CSA_FET
 * @brief Enumeration for the CSA positive input
 */
typedef enum {
  /*! Current sense amplifier positive input is SPx */
  DRV8323RS_CSA_CONTROL_CSA_FET_IN_SPX = 0,  
  /*! Current sense amplifier positive input is SHx */
  DRV8323RS_CSA_CONTROL_CSA_FET_IN_SHX,
} DRV8323RS_CSA_CONTROL_CSA_FET;

/*!
 * @enum DRV8323RS_CSA_CONTROL_LS_REF
 * @brief Enumeration for the low side reference
 */
typedef enum {
  /*! Low side mosfet overcurrent measured across SHx to SPx */
  DRV8323RS_CSA_CONTROL_LS_REF_SH_SP = 0,  
  /*! Low side mosfet overcurrent measured across SHx to SNx */
  DRV8323RS_CSA_CONTROL_LS_REF_SH_SN,
} DRV8323RS_CSA_CONTROL_LS_REF;

/*!
 * @enum DRV8323RS_CSA_CONTROL_VREF_DIV
 * @brief Enumeration for the CSA reference voltage
 */
typedef enum {
  /*! Current sense amplifier reference voltage is VREF */
  DRV8323RS_CSA_CONTROL_VREF_DIV_1 = 0,  
  /*! Current sense amplifier reference voltage is VREF/2 */
  DRV8323RS_CSA_CONTROL_VREF_DIV_2,
} DRV8323RS_CSA_CONTROL_VREF_DIV;

/*!
 * @enum DRV8323RS_REG_ADDR
 * @brief Enumeration for SPI register address
 */
typedef enum {
    /*! Address of fault status register */
    DRV8323RS_REG_ADDR_FAULT_STATUS_1 = 0,
    /*! Address of diag status 1 register */
    DRV8323RS_REG_ADDR_FAULT_STATUS_2,
    /*! Address of diag status 2 register */
    DRV8323RS_REG_ADDR_DRIVER_CONTROL,
    /*! Address of gate drive HS register */
    DRV8323RS_REG_ADDR_GATE_DRIVE_HS,
    /*! Address of gate driver LS register */
    DRV8323RS_REG_ADDR_GATE_DRIVE_LS,
    /*! Address of OCP control register */
    DRV8323RS_REG_ADDR_OCP_CONTROL,
    /*! Address of CSA control register */
    DRV8323RS_REG_ADDR_CSA_CONTROL,
}DRV8323RS_REG_ADDR;

/*! @brief Define DRV8323RS structure */
typedef struct {
    /*! Scale factor for currents read from ADC */
    _iq15 iSf;
    /*! GPIO pin set for enable. @ref HAL_GPIO_OUT_PIN */
    HAL_GPIO_OUT_PIN enable;
    /*! Timer fault input set to nfault pin. @ref HAL_PWM_FAULT */
    HAL_PWM_FAULT nfault;
    /*! SPI channel. @ref HAL_SPI_CHANNEL */
    HAL_SPI_CHANNEL spi;
    /*! SPI chip select. @ref HAL_SPI_CS */
    HAL_SPI_CS spiCS;
    /*! ADC channel set for vm. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL vsenvm;
    /*! ADC channel set for A phase. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL isena;
    /*! ADC channel set for B phase. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL isenb;
    /*! ADC channel set for C phase. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL isenc;
} DRV8323RS_Instance;

/* Extern drvGain */
extern int8_t drvGain[DRV8323RS_CSA_GAIN_MAX_VALUES];

/**
 * @brief     Enable the DRV8323RS 
 * @param[in] handle    A pointer to drv8323rs instance
 */
void DRV8323RS_enable(DRV8323RS_Instance *handle);

/**
 * @brief     Disable the DRV8323RS 
 * @param[in] handle    A pointer to drv8323rs instance
 */
void DRV8323RS_disable(DRV8323RS_Instance *handle);

/**
 * @brief     Initializes the DRV8323RS object
 * @param[in] handle    A pointer to drv8323rs instance
 */
void DRV8323RS_init(DRV8323RS_Instance *handle);

/**
 * @brief     Creates a data word to be send through SPI
 * @param[in] wrCmd    SPI read or write command. One of @ref DRV8323RS_SPI_CMD
 * @param[in] addr     A SPI register address. One of @ref DRV8323RS_REG_ADDR
 * @param[in] data     Data to be written to the SPI register.
 * @return    Return created SPI word
 */
uint16_t DRV8323RS_createSPIDataWord(
               DRV8323RS_SPI_CMD wrCmd, DRV8323RS_REG_ADDR addr, uint16_t data);

/**
 * @brief     Writes the data to the SPI register
 * @param[in] handle   A pointer to drv8323rs instance
 * @param[in] addr     A SPI register address. One of @ref DRV8323RS_REG_ADDR
 * @param[in] data     Data to be written to the SPI register.
 */
void DRV8323RS_spiWrite(
            DRV8323RS_Instance *handle, DRV8323RS_REG_ADDR addr, uint16_t data);

/**
 * @brief     Reads the data in the SPI register
 * @param[in] handle   A pointer to drv8323rs instance
 * @param[in] addr     A SPI register address. One of @ref DRV8323RS_REG_ADDR
 * @return    Returns the data in spi register
 */
uint16_t DRV8323RS_spiRead(DRV8323RS_Instance *handle, DRV8323RS_REG_ADDR addr);

/**
 * @brief     Update only some bits in the SPI register
 * @param[in] handle   A pointer to drv8323rs instance
 * @param[in] addr     A SPI register address. One of @ref DRV8323RS_REG_ADDR
 * @param[in] mask     Mask for the bits to change
 * @param[in] data     Data to be written to the SPI register
 */
void DRV8323RS_spiUpdateRegister(DRV8323RS_Instance *handle, 
                         DRV8323RS_REG_ADDR addr, uint32_t mask, uint16_t data);

/**
 * @brief     Unlock or lock spi registers of the DRV
 * @param[in] handle   A pointer to drv8323rs instance
 * @param[in] lock    Lock or Unlock value.
 * One of @ref DRV8323RS_GATE_DRIVE_HS_LOCK
 */
void DRV8323RS_setSPIRegisterLock(
                 DRV8323RS_Instance *handle, DRV8323RS_GATE_DRIVE_HS_LOCK lock);

/**
 * @brief     Set peak source gate current
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] hsPeakCurr High side peak source current.
 * One of @ref DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR
 * @param[in] lsPeakCurr Low side peak source current.
 * One of @ref DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR
 */
void DRV8323RS_setPeakSourceGateCurr(
                               DRV8323RS_Instance *handle,
                               DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR hsPeakCurr, 
                               DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR lsPeakCurr);

/**
 * @brief     Set peak sink gate current
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] hsPeakCurr High side peak sink current.
 * One of @ref DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR
 * @param[in] lsPeakCurr Low side peak sink current.
 * One of @ref DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR
 */
void DRV8323RS_setPeakSinkGateCurr(
                                 DRV8323RS_Instance *handle,
                                 DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR hsPeakCurr, 
                                 DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR lsPeakCurr);

/**
 * @brief     Set peak current gate drive time
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] tdrive Peak gate drive current time to set.
 * One of @ref DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME
 */
void DRV8323RS_setPeakGateCurrTime(DRV8323RS_Instance *handle,
                                     DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME tdrive);   

/**
 * @brief     Set pwm mode
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] pwmMode PWM mode to set.
 *  One of @ref DRV8323RS_DRIVER_CONTROL_PWM_MODE
 */
void DRV8323RS_setPwmMode(DRV8323RS_Instance *handle,
                                     DRV8323RS_DRIVER_CONTROL_PWM_MODE pwmMode);

/**
 * @brief     Set overcurrent trip voltage
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] vdsLvl  Overcurrent level to set.
 *  One of @ref DRV8323RS_OCP_CONTROL_VDS_LVL
 */
void DRV8323RS_setVdsLvl(DRV8323RS_Instance *handle,
                                          DRV8323RS_OCP_CONTROL_VDS_LVL vdsLvl);

/**
 * @brief     Set overcurrent deglitch time
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] ocpDeg  Overcurrent deglitch time to set.
 *  One of @ref DRV8323RS_OCP_CONTROL_OCP_DEG
 */
void DRV8323RS_setOcpDeg(DRV8323RS_Instance *handle,
                                          DRV8323RS_OCP_CONTROL_OCP_DEG ocpDeg);

/**
 * @brief     Set overcurrent report mode
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] ocpMode OCP report mode to set.
 *  One of @ref DRV8323RS_OCP_CONTROL_OCP_MODE
 */
void DRV8323RS_setOcpMode(DRV8323RS_Instance *handle,
                                        DRV8323RS_OCP_CONTROL_OCP_MODE ocpMode);

/**
 * @brief     Set deadtime
 * @param[in] handle   A pointer to drv8323rs instance
 * @param[in] deadtime Deadtime to set.
 * One of @ref DRV8323RS_OCP_CONTROL_DEADTIME
 */
void DRV8323RS_setDeadtime(DRV8323RS_Instance *handle,
                                       DRV8323RS_OCP_CONTROL_DEADTIME deadtime);

/**
 * @brief     Set overcurrent retry time
 * @param[in] handle A pointer to drv8323rs instance
 * @param[in] tretry Retry time to set. One of @ref DRV8323RS_OCP_CONTROL_TRETRY
 */
void DRV8323RS_setTretry(DRV8323RS_Instance *handle,
                                           DRV8323RS_OCP_CONTROL_TRETRY tretry);

/**
 * @brief     Set overcurrent sense level
 * @param[in] handle A pointer to drv8323rs instance
 * @param[in] senLvl Overcurrent sense level to set.
 * One of @ref DRV8323RS_CSA_CONTROL_SEN_LVL
 */
void DRV8323RS_setSenLvl(DRV8323RS_Instance *handle,
                                          DRV8323RS_CSA_CONTROL_SEN_LVL senLvl);

/**
 * @brief     Enable or disable overcurrent sense
 * @param[in] handle A pointer to drv8323rs instance
 * @param[in] disSen Overcurrent sense state to set.
 * One of @ref DRV8323RS_CSA_CONTROL_DIS_SEN
 */
void DRV8323RS_setDisSen(DRV8323RS_Instance *handle,
                                          DRV8323RS_CSA_CONTROL_DIS_SEN disSen);

/**
 * @brief     Set current sense amplifier gain
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] csaGain Gain to set. One of @ref DRV8323RS_CSA_CONTROL_CSA_GAIN
 */
void DRV8323RS_setCsaGain(DRV8323RS_Instance *handle,
                                        DRV8323RS_CSA_CONTROL_CSA_GAIN csaGain);

/**
 * @brief     Set reference point for low side ocp
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] lsRef   Reference point to set.
 * One of @ref DRV8323RS_CSA_CONTROL_LS_REF
 */
void DRV8323RS_setLsRef(DRV8323RS_Instance *handle,
                                            DRV8323RS_CSA_CONTROL_LS_REF lsRef);

/**
 * @brief     Set reference voltage for current sense amplifier
 * @param[in] handle   A pointer to drv8323rs instance
 * @param[in] vrefDiv  Reference voltage to set.
 * One of @ref DRV8323RS_CSA_CONTROL_VREF_DIV
 */
void DRV8323RS_setVrefDiv(DRV8323RS_Instance *handle,
                                        DRV8323RS_CSA_CONTROL_VREF_DIV vrefDiv);

/**
 * @brief     Set reference point for current sense amplifier positive input
 * @param[in] handle  A pointer to drv8323rs instance
 * @param[in] csaFet  Reference point to set.
 * One of @ref DRV8323RS_CSA_CONTROL_CSA_FET
 */
void DRV8323RS_setCsaFet(DRV8323RS_Instance *handle,
                                          DRV8323RS_CSA_CONTROL_CSA_FET csaFet);

/**
 * @brief     Gets the raw vm value from vsenvm adc
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the raw adc value read
 */
__STATIC_INLINE uint16_t DRV8323RS_getVMRaw(DRV8323RS_Instance *handle)
{
  return (HAL_getADCValue(handle->vsenvm));
}

/**
 * @brief     Gets the raw adc value from isena adc
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the raw adc value read
 */
__STATIC_INLINE uint16_t DRV8323RS_getIARaw(DRV8323RS_Instance *handle)
{
  return (HAL_getADCValue(handle->isena));
}

/**
 * @brief     Gets the raw adc value from isenb adc
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the raw adc value read
 */
__STATIC_INLINE uint16_t DRV8323RS_getIBRaw(DRV8323RS_Instance *handle)
{
  return (HAL_getADCValue(handle->isenb));
}

/**
 * @brief     Gets the raw adc value from ic adc
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the raw adc value read
 */
__STATIC_INLINE uint16_t DRV8323RS_getICRaw(DRV8323RS_Instance *handle)
{
  return (HAL_getADCValue(handle->isenc));
}

/**
 * @brief     Gets the converted IA current
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the converted IA current in IQ
 */
_iq DRV8323RS_getIA(DRV8323RS_Instance *handle);

/**
 * @brief     Gets the converted IB current
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the converted IB current in IQ
 */
_iq DRV8323RS_getIB(DRV8323RS_Instance *handle);

/**
 * @brief     Gets the converted IC current
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the converted IC current in IQ
 */
_iq DRV8323RS_getIC(DRV8323RS_Instance *handle);

/**
 * @brief     Gets the converted VM voltage
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns the converted VM voltage in IQ
 */
_iq DRV8323RS_getVM(DRV8323RS_Instance *handle);

/**
 * @brief     Sets Phase current scale factors
 * @param[in] handle A pointer to drv8323rs instance
 * @param[in] gain   Gain of current sense amplifier
 * @param[in] rshunt Rshunt value in iq15
 */
void DRV8323RS_setISf(DRV8323RS_Instance *handle,
                            DRV8323RS_CSA_CONTROL_CSA_GAIN  gain, _iq15 rshunt);

/**
 * @brief     Checks if fault occurred
 * @param[in] handle    A pointer to drv8323rs instance
 * @return    Returns state of fault status
 */
bool DRV8323RS_isFaultOccurred(DRV8323RS_Instance *handle);

/**
 * @brief     Clears the fault status in the timer and in SPI register
 * @param[in] handle    A pointer to drv8323rs instance
 */
void DRV8323RS_clearFaults(DRV8323RS_Instance *handle);

#ifdef __cplusplus
}
#endif
#endif /* DRV8323RS_H */
/** @}*/
