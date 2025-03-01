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

//*****************************************************************************
// the includes
//*****************************************************************************
#include "TIDA_010244.h"

/*! Active Power threshold value*/
#define ACTIVE_POWER_THRESHOLD  2
/*! Reactive Power threshold value*/
#define REACTIVE_POWER_THRESHOLD  2

volatile AMC_Instance gAMCHandle1;
volatile AMC_Instance gAMCHandle2;
volatile AMC_Instance gAMCHandle3;
volatile AMC_Instance gAMCHandle4;
metrologyData gMetrologyworkingData;
DLT645Buf gDLT645;
TIDA_instance gTidaHandle;

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

#if defined(REACTIVE_OFFSET_CALIBRATION)
/*!
 * @brief Update the reactive power value
 * @param[in] workingData  The Metrology Data
 * @param[in] ph           phase
 */
void TIDA_updateReactivePower(metrologyData *workingData, PHASES ph)
{
    if(_IQ13abs(workingData->phases[ph].readings.reactivePower) < _IQ13(REACTIVE_POWER_THRESHOLD))
    {
        workingData->phases[ph].readings.reactivePower = 0;
    }
}

/*!
 * @brief Update the active power value
 * @param[in] workingData  The Metrology Data
 * @param[in] ph           phase
 */
void TIDA_updateActivePower(metrologyData *workingData, PHASES ph)
{
    if(_IQ13abs(workingData->phases[ph].readings.activePower) < _IQ13(ACTIVE_POWER_THRESHOLD))
    {
        workingData->phases[ph].readings.activePower = 0;
    }
}

/*!
 * @brief Update the reactive power offset 
 * @param[in] workingData  The Metrology Data
 * @param[in] ph           phase
 */
void TIDA_updateReactivePowerOffset(metrologyData *workingData, PHASES ph)
{
    phaseMetrology *phase = &workingData->phases[ph];
    phaseCalibrationData *phaseCal = &calInfo->phases[ph];

    /* Meter calibrated at 120V */
    _iq19 voltageFactor = _IQ19div(phase->readings.RMSVoltage, _IQ19(120));

    /* Implementing polynomial equation 2x^2 - 3x + 2 */
    int64_t poly = ((((int64_t)voltageFactor * voltageFactor) * 2) - (((int64_t)voltageFactor * 3) << 19) + ((int64_t)2 << 38));

    phaseCal->current.reactivePowerOffset = (poly * calibratedReactiveOffset[ph]) >> 38;
}
#endif

/*!
 * @brief TIDA initialization
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] amcHandle1   The AMC handle1
 * @param[in] amcHandle2   The AMC handle2
 * @param[in] amcHandle3   The AMC handle3
 * @param[in] amcHandle4   The AMC handle4
 * @param[in] dlt645       The dlt645 buffer
 */
void TIDA_init(TIDA_instance *tidaHandle, metrologyData *workingData,volatile AMC_Instance *amcHandle1, volatile AMC_Instance *amcHandle2, volatile AMC_Instance *amcHandle3, volatile AMC_Instance *amcHandle4, DLT645Buf *dlt645)
{
    amcHandle1->ready            = HAL_GPIO_IN_00;
    amcHandle1->sync             = HAL_GPIO_OUT_00;
    amcHandle1->spiChan          = HAL_SPI_CHAN_0;
    amcHandle1->spiCs            = HAL_SPI_CS_3;
    amcHandle1->crcEnable        = AMC_CRC_ENABLE;
    amcHandle1->crcType          = AMC_CRC_TYPE_CCITT;

    amcHandle2->ready            = HAL_GPIO_IN_01;
    amcHandle2->sync             = HAL_GPIO_OUT_00;
    amcHandle2->spiChan          = HAL_SPI_CHAN_0;
    amcHandle2->spiCs            = HAL_SPI_CS_1;
    amcHandle2->crcEnable        = AMC_CRC_ENABLE;
    amcHandle2->crcType          = AMC_CRC_TYPE_CCITT;

    amcHandle3->ready            = HAL_GPIO_IN_02;
    amcHandle3->sync             = HAL_GPIO_OUT_00;
    amcHandle3->spiChan          = HAL_SPI_CHAN_0;
    amcHandle3->spiCs            = HAL_SPI_CS_2;
    amcHandle3->crcEnable        = AMC_CRC_ENABLE;
    amcHandle3->crcType          = AMC_CRC_TYPE_CCITT;

    amcHandle4->ready            = HAL_GPIO_IN_03;
    amcHandle4->sync             = HAL_GPIO_OUT_00;
    amcHandle4->spiChan          = HAL_SPI_CHAN_0;
    amcHandle4->spiCs            = HAL_SPI_CS_0;
    amcHandle4->crcEnable        = AMC_CRC_ENABLE;
    amcHandle4->crcType          = AMC_CRC_TYPE_CCITT;

    dlt645->uartChan            = HAL_UART_CHAN_0;

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
}

/*!
 * @brief TIDA calculate metrology parameters
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] dlt645       The DLT645 buffer
 */
void TIDA_calculateMetrologyParameters(TIDA_instance *tidaHandle, metrologyData *workingData, DLT645Buf *dlt645)
{
    DLT645_service(workingData, dlt645);

    for(PHASES ph = PHASE_ONE; ph < MAX_PHASES; ph++)
    {
        phaseMetrology *phase = &workingData->phases[ph];

        if (phase->status & PHASE_STATUS_NEW_LOG)
        {
            phase->status &= ~PHASE_STATUS_NEW_LOG;
            Metrology_calculatePhaseReadings(workingData, ph);
            TIDA_updateINT0(tidaHandle, workingData->phases[ph].readings.activePower, PHASE_ONE_ACTIVE_POWER_NEGATIVE << ph);
            TIDA_updateINT0(tidaHandle, workingData->phases[ph].readings.reactivePower, PHASE_ONE_REACTIVE_POWER_NEGATIVE << ph);
            tidaHandle->intr0Status |= ((PHASE_ONE_CYCLE_COUNT_DONE << ph) & tidaHandle->intr0Enable);

            #if defined(REACTIVE_OFFSET_CALIBRATION)
            TIDA_updateActivePower(workingData, ph);
            TIDA_updateReactivePower(workingData, ph);
            TIDA_updateReactivePowerOffset(workingData, ph);
            #endif
            
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
    HAL_delayMicroSeconds(AMC_SYNC_US);
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
    HAL_delayMicroSeconds(AMC_RESET_US);
    HAL_writeGPIOPin(tidaHandle->sync_reset, HAL_GPIO_PIN_HIGH);
    HAL_delayMicroSeconds(AMC_WAKEUP_US);
}


