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
 *  @file       TIDA_010243.h
 *  @brief      TIDA_010243 module
 *
 *  @anchor TIDA_010243_h
 *  # Overview
 *
 *  APIs related to TIDA_010243 module
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup TIDA_010243
 * @{
 */

#ifndef TIDA_010243_H_
#define TIDA_010243_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "metrology.h"
#include "ADS131M0x.h"
#include "DLT645.h"
#include "ADS131M08_Defaults.h"

/*! @brief Defines Number of cycles to skip initially   */
#define INITIAL_ZERO_CROSSINGS_TO_SKIP     10
/*! @brief Defines phase log complete status in three phases    */
#define THREE_PHASE_LOG_DONE    0x07
/*! @brief Defines phase log complete status    */
#define PHASE_LOG_DONE          0x01
/*! @brief Defines ADC are in initialization state  */
#define ADC_INIT                0x00
/*! @brief Defines ADC are ready for data conversion  */
#define ADC_READY               0x01

/*! @brief Defines TIDA instance    */
typedef struct
{
    /*! @brief interrupt 0 Enable */
    uint32_t intr0Enable;
    /*! @brief interrupt 1 Enable */
    uint32_t intr1Enable;
    /*! @brief interrupt 0 status */
    uint32_t intr0Status;
    /*! @brief interrupt 1 status */
    uint32_t intr1Status;
    /*! @brief sync pin to all ADCs */
    HAL_GPIO_OUT sync_reset;
    /*! @brief Stores ADC status bit    */
    uint32_t adcStatus;
}TIDA_instance;

/*! @brief Defines ADS instance*/
extern volatile ADS_Instance     gADSHandle;
/*! @brief Defines metrology working data*/
extern metrologyData    gMetrologyWorkingData;
/*! @brief Defines DLT645 instance*/
extern DLT645Buf        gDLT645;
/*! @brief Defines TIDA handle instance*/
extern TIDA_instance    gTidaHandle;

/*! @enum EVENTS0  */
typedef enum
{
    /*! @brief Defines Phase 1 active power is negative */
    PHASE_ONE_ACTIVE_POWER_NEGATIVE     = 0x00000001,
    /*! @brief Defines Phase 2 active power is negative */
    PHASE_TWO_ACTIVE_POWER_NEGATIVE     = 0x00000002,
    /*! @brief Defines Phase 3 active power is negative */
    PHASE_THREE_ACTIVE_POWER_NEGATIVE   = 0x00000004,
    /*! @brief Defines total active power is negative */
    TOTAL_ACTIVE_POWER_NEGATIVE         = 0x00000008,
    /*! @brief Defines Phase 1 reactive power is negative */
    PHASE_ONE_REACTIVE_POWER_NEGATIVE   = 0x00000010,
    /*! @brief Defines Phase 2 reactive power is negative */
    PHASE_TWO_REACTIVE_POWER_NEGATIVE   = 0x00000020,
    /*! @brief Defines Phase 3 reactive power is negative */
    PHASE_THREE_REACTIVE_POWER_NEGATIVE = 0x00000040,
    /*! @brief Defines total reactive power is negative */
    TOTAL_REACTIVE_POWER_NEGATIVE       = 0x00000080,
    /*! @brief Defines phase 1 cycle count  is done */
    PHASE_ONE_CYCLE_COUNT_DONE          = 0x00000100,
    /*! @brief Defines phase 2 cycle count  is done */
    PHASE_TWO_CYCLE_COUNT_DONE          = 0x00000200,
    /*! @brief Defines phase 3 cycle count  is done */
    PHASE_THERE_CYCLE_COUNT_DONE        = 0x00000400,
}EVENTS0;

/*! @enum EVENTS1  */
typedef enum
{
    /*! @brief Defines zero crossing is missed in phase 1   */
    ZERO_CROSSING_PHASE_ONE    = 0x00000001,
    /*! @brief Defines zero crossing is missed in phase 2   */
    ZERO_CROSSING_PHASE_TWO    = 0x00000002,
    /*! @brief Defines zero crossing is missed in phase 3   */
    ZERO_CROSSING_PHASE_THREE  = 0x00000004,
    /*! @brief Defines sag event occured in phase 1   */
    SAG_EVENT_PHASE_ONE        = 0x00000008,
    /*! @brief Defines sag event occured in phase 2   */
    SAG_EVENT_PHASE_TWO        = 0x00000010,
    /*! @brief Defines sag event occured in phase 3   */
    SAG_EVENT_PHASE_THREE      = 0x00000020,
}EVENTS1;

/*!
 * @brief TIDA calculate metrology parameters
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] adsHandle    The ADS handle
 * @param[in] dlt645       The DLT645 buffer
 */
void TIDA_calculateMetrologyParameters(TIDA_instance *tidaHandle, metrologyData *workingData, volatile ADS_Instance *adsHandle, DLT645Buf *dlt645);

/*!
 * @brief TIDA initialization
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] adsHandle    The ADS handle
 * @param[in] dlt645       The dlt645 buffer
 */
void TIDA_init(TIDA_instance *tidaHandle, metrologyData *workingData, volatile ADS_Instance *adsHandle, DLT645Buf *dlt645);

/*!
 * @brief TIDA start data collection
 * @param[in] tidaHandle   The TIDA Instance
 */
void TIDA_startDataCollection(TIDA_instance *tidaHandle);

/*!
 * @brief TIDA reset all ADCs
 * @param[in] tidaHandle   The TIDA Instance
 */
void TIDA_resetADC(TIDA_instance * tidaHandle);

/*!
 * @brief Debug init
 *        initializes the lookup table
 */
void debugInit(void);

#ifdef __cplusplus
}
#endif
#endif /* TIDA_010243_H_ */
/** @}*/
