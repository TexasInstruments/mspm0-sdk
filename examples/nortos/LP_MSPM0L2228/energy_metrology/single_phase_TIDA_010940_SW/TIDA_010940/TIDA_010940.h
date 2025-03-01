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
 *  @file       TIDA_010940.h
 *  @brief      TIDA_010940 module
 *
 *  @anchor TIDA_010940_h
 *  # Overview
 *
 *  APIs related to TIDA_010940 module
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup TIDA_010940
 * @{
 */

#ifndef TIDA_010940_H_
#define TIDA_010940_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "metrology.h"
#include "ADS131M0x.h"
#include "DLT645.h"
#include "lcd.h"
#include "ADS131M03_Defaults.h"

/*! @brief Skip initial cycles to get stable reading    */
#define INITIAL_ZERO_CROSSINGS_TO_SKIP     (10)
/*! @brief Defines phase log complete status in three phases    */
#define SINGLE_PHASE_LOG_DONE   (0x01)
/*! @brief Defines phase log complete status    */
#define PHASE_LOG_DONE          (0x01)
/*! @brief Defines ADC are in initialization state  */
#define ADC_INIT                (0x00)
/*! @brief Defines ADC are ready for data conversion  */
#define ADC_READY               (0x01)

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
extern metrologyData    gMetrologyworkingData;
/*! @brief Defines DLT645 instance*/
extern DLT645Buf        gDLT645;
/*! @brief Defines TIDA handle instance*/
extern TIDA_instance    gTidaHandle;
/*! @brief Defines LCD handle instance*/
extern LCD_instance     gLCDHandle;

/*! @enum DISPLAY_PARAMS    */
typedef enum
{
    /*! @brief Displays voltage on LCD  */
    DISPLAY_VOLTAGE = 0,
    /*! @brief Displays current on LCD  */
    DISPLAY_CURRENT,
    /*! @brief Displays active power on LCD  */
    DISPLAY_ACTIVE_POWER,
    /*! @brief Displays reactive power on LCD  */
    DISPLAY_REACTIVE_POWER,
    /*! @brief Displays apparent power on LCD  */
    DISPLAY_APPARENT_POWER,
    /*! @brief Displays active energy on LCD  */
    DISPLAY_ACTIVE_ENERGY,
    /*! @brief Displays reactive energy on LCD  */
    DISPLAY_REACTIVE_ENERGY,
    /*! @brief Displays apparent energy on LCD  */
    DISPLAY_APPARENT_ENERGY,
    /*! @brief Displays frequency on LCD  */
    DISPLAY_FREQUENCY
}DISPLAY_PARAMS;

/*!
 * @brief TIDA initialization
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] dlt645       The DLT645 buffer
 */
void TIDA_calculateMetrologyParameters(TIDA_instance *tidaHandle, metrologyData *workingData, DLT645Buf *dlt645);

/*!
 * @brief TIDA initialization
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] adsHandle    The ADS handle
 * @param[in] dlt645       The dlt645 instance
 * @param[in] lcdHandle    The LCD instance
 */
void TIDA_init(TIDA_instance *tidaHandle, metrologyData *workingData, volatile ADS_Instance *adsHandle, DLT645Buf *dlt645, LCD_instance *lcdHandle);

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
 * @brief TIDA initialization
 * @param[in] lcdHandle   The LCD Instance
 */
void TIDA_displayOnLCD(LCD_instance *lcdHandle);

/*!
 * @brief Debug init
 *        initializes the lookup table
 */
void debugInit(void);

#ifdef __cplusplus
}
#endif
#endif /* TIDA_010940_H_ */
/** @}*/
