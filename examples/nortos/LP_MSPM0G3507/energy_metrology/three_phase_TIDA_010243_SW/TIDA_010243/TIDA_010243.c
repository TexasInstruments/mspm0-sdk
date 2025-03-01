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

//*****************************************************************************
// the includes
//*****************************************************************************
#include "TIDA_010243.h"

/*! @brief Defines ADS instance*/
volatile ADS_Instance    gADSHandle;
/*! @brief Defines metrology working data*/
metrologyData   gMetrologyWorkingData;
/*! @brief Defines DLT645 instance*/
DLT645Buf       gDLT645;
/*! @brief Defines TIDA handle instance*/
TIDA_instance   gTidaHandle;
/*! @brief Stores phase log status  */
volatile uint8_t phaseLog = 0;

/*!
 * @brief Update interrupt 0 register
 * @param[in] tidaHandle  The TIDA instance
 * @param[in] eventData   Event Data
 * @param[in] event       Event Type
 */
void TIDA_updateINT0(TIDA_instance *tidaHandle, int32_t eventData, EVENTS0 event);

/*!
 * @brief Update interrupt 1 register
 * @param[in] tidaHandle  The TIDA instance
 * @param[in] eventData   Event Data
 * @param[in] event       Event Type
 */
void TIDA_updateINT1(TIDA_instance *tidaHandle, uint16_t eventData, EVENTS1 event);

/*!
 * @brief Update interrupt 0 register
 * @param[in] tidaHandle  The TIDA instance
 * @param[in] eventData   Event Data
 * @param[in] event       Event Type
 */
void TIDA_updateINT0(TIDA_instance *tidaHandle, int32_t eventData, EVENTS0 event)
{
    if(eventData < 0)
    {
        if(!(tidaHandle->intr0Status & event))
        {
            tidaHandle->intr0Status |= (tidaHandle->intr0Enable & event);
        }
    }
    else
    {
        if(tidaHandle->intr0Status & event)
        {
            tidaHandle->intr0Status &= ~event;
        }
    }
}

/*!
 * @brief Update interrupt 1 register
 * @param[in] tidaHandle  The TIDA instance
 * @param[in] eventData   Event Data
 * @param[in] event       Event Type
 */
void TIDA_updateINT1(TIDA_instance *tidaHandle, uint16_t eventData, EVENTS1 event)
{
    if(eventData == PHASE_STATUS_ZERO_CROSSING_MISSED || (int8_t)eventData == SAG_SWELL_VOLTAGE_SAG_CONTINUING)
    {
        if(!(tidaHandle->intr1Status & event))
        {
            tidaHandle->intr1Status |= (tidaHandle->intr1Enable & event);
        }
    }
    else
    {
        if(tidaHandle->intr1Status & event)
        {
            tidaHandle->intr1Status &= ~event;
        }
    }
}

/*!
 * @brief TIDA initialization
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] adsHandle    The ADS handle
 * @param[in] dlt645       The dlt645 buffer
 */
void TIDA_init(TIDA_instance *tidaHandle, metrologyData *workingData, volatile ADS_Instance *adsHandle, DLT645Buf *dlt645)
{
    adsHandle->ready            = HAL_GPIO_IN_00;
    adsHandle->sync             = HAL_GPIO_OUT_00;
    adsHandle->spiChan          = HAL_SPI_CHAN_0;
    adsHandle->spiCs            = HAL_SPI_CS_0;
    adsHandle->crcEnable        = ADS_CRC_ENABLE;
    adsHandle->crcType          = ADS_CRC_TYPE_CCITT;

    gDLT645.uartChan            = HAL_UART_CHAN_0;

    workingData->activePulse    = HAL_GPIO_OUT_02;
    workingData->reactivePulse  = HAL_GPIO_OUT_03;

    tidaHandle->intr0Enable    = PHASE_ONE_ACTIVE_POWER_NEGATIVE |
                                 PHASE_TWO_ACTIVE_POWER_NEGATIVE |
                                 PHASE_THREE_ACTIVE_POWER_NEGATIVE |
                                 TOTAL_ACTIVE_POWER_NEGATIVE      |
                                 PHASE_ONE_REACTIVE_POWER_NEGATIVE|
                                 PHASE_TWO_REACTIVE_POWER_NEGATIVE|
                                 PHASE_THREE_REACTIVE_POWER_NEGATIVE|
                                 TOTAL_REACTIVE_POWER_NEGATIVE|
                                 PHASE_ONE_CYCLE_COUNT_DONE|
                                 PHASE_TWO_CYCLE_COUNT_DONE|
                                 PHASE_THERE_CYCLE_COUNT_DONE;

    tidaHandle->intr1Enable    = ZERO_CROSSING_PHASE_ONE  |
                                 ZERO_CROSSING_PHASE_TWO  |
                                 ZERO_CROSSING_PHASE_THREE|
                                 SAG_EVENT_PHASE_ONE      |
                                 SAG_EVENT_PHASE_TWO      |
                                 SAG_EVENT_PHASE_THREE;

    tidaHandle->intr0Status    = 0x0;
    tidaHandle->intr1Status    = 0x0;
    tidaHandle->sync_reset     = HAL_GPIO_OUT_00;
    tidaHandle->adcStatus      = ADC_INIT;

    DLT645_UARTRxdmaInit(dlt645);
}

/*!
 * @brief TIDA calculate metrology parameters
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] adsHandle    The ADS handle
 * @param[in] dlt645       The DLT645 buffer
 */
void TIDA_calculateMetrologyParameters(TIDA_instance *tidaHandle, metrologyData *workingData, volatile ADS_Instance *adsHandle, DLT645Buf *dlt645)
{
    DLT645_service(workingData, dlt645);

    for(int ph = PHASE_ONE; ph < MAX_PHASES; ph++)
    {
        phaseMetrology *phase = &workingData->phases[ph];

        if (phase->status & PHASE_STATUS_NEW_LOG)
        {
            phase->status &= ~PHASE_STATUS_NEW_LOG;
            Metrology_calculatePhaseReadings(workingData, ph);
            TIDA_updateINT0(tidaHandle, workingData->phases[ph].readings.activePower, PHASE_ONE_ACTIVE_POWER_NEGATIVE << ph);
            TIDA_updateINT0(tidaHandle, workingData->phases[ph].readings.reactivePower, PHASE_ONE_REACTIVE_POWER_NEGATIVE << ph);
            tidaHandle->intr0Status |= ((PHASE_ONE_CYCLE_COUNT_DONE << ph) & tidaHandle->intr0Enable);
            phaseLog |= PHASE_LOG_DONE << ph;
        }
        else
        {
            tidaHandle->intr0Status &= ~(PHASE_ONE_CYCLE_COUNT_DONE << ph);
        }

        if (phase->cycleStatus & PHASE_STATUS_NEW_LOG)
        {
            phase->cycleStatus &= ~PHASE_STATUS_NEW_LOG;
            if(phase->params.cycleSkipCount < (INITIAL_ZERO_CROSSINGS_TO_SKIP-1))
            {
                phase->params.cycleSkipCount++;
            }
            else
            {
                Metrology_sagSwellDetection(workingData, ph);
                TIDA_updateINT1(tidaHandle, (uint16_t)phase->params.sagSwellState, (SAG_EVENT_PHASE_ONE << ph));
            }
        }
        else
        {
            TIDA_updateINT1(tidaHandle, (phase->cycleStatus & PHASE_STATUS_ZERO_CROSSING_MISSED), (ZERO_CROSSING_PHASE_ONE << ph));
        }
    }

    if(phaseLog == THREE_PHASE_LOG_DONE)
    {
       Metrology_calculateThreePhaseParameters(workingData);
       Metrology_calculateTotalParameters(workingData);
       TIDA_updateINT0(tidaHandle, workingData->totals.readings.activePower, TOTAL_ACTIVE_POWER_NEGATIVE);
       TIDA_updateINT0(tidaHandle, workingData->totals.readings.reactivePower, TOTAL_REACTIVE_POWER_NEGATIVE);
       phaseLog = 0;
    }

    if(workingData->neutral.status & PHASE_STATUS_NEW_LOG)
    {
       workingData->neutral.status &= ~PHASE_STATUS_NEW_LOG;
       Metrology_calculateNeutralReadings(workingData);
    }
}

/*!
 * @brief TIDA start data collection
 * @param[in] tidaHandle   The TIDA Instance
 */
void TIDA_startDataCollection(TIDA_instance *tidaHandle)
{
    HAL_writeGPIOPin(tidaHandle->sync_reset, HAL_GPIO_PIN_LOW);
    HAL_delayMicroSeconds(ADS_SYNC_US);
    HAL_writeGPIOPin(tidaHandle->sync_reset, HAL_GPIO_PIN_HIGH);

    tidaHandle->adcStatus = ADC_READY;
}

/*!
 * @brief TIDA reset all ADCs
 * @param[in] tidaHandle   The TIDA Instance
 */
void TIDA_resetADC(TIDA_instance * tidaHandle)
{
    HAL_writeGPIOPin(tidaHandle->sync_reset, HAL_GPIO_PIN_LOW);
    HAL_delayMicroSeconds(ADS_RESET_US);
    HAL_writeGPIOPin(tidaHandle->sync_reset, HAL_GPIO_PIN_HIGH);
    HAL_delayMicroSeconds(ADS_WAKEUP_US);
}


