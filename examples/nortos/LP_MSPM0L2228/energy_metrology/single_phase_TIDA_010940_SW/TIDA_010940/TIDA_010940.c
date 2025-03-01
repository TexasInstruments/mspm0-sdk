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
#include "TIDA_010940.h"

/*! @brief Defines ADS instance*/
volatile ADS_Instance    gADSHandle;
/*! @brief Defines metrology working data*/
metrologyData   gMetrologyworkingData;
/*! @brief Defines DLT645 instance*/
DLT645Buf       gDLT645;
/*! @brief Defines TIDA handle instance*/
TIDA_instance   gTidaHandle;
/*! @brief Defines LCD handle instance*/
LCD_instance    gLCDHandle;
/*! @brief Stores phase log status  */
volatile uint8_t phaseLog = 0;
/*! @brief Stores parameter to display on LCD   */
DISPLAY_PARAMS displayParams;

/*!
 * @brief TIDA initialization
 * @param[in] tidaHandle   The TIDA Instance
 * @param[in] workingData  The Metrology Data
 * @param[in] adsHandle    The ADS handle
 * @param[in] dlt645       The dlt645 instance
 * @param[in] lcdHandle    The LCD instance
 */
void TIDA_init(TIDA_instance *tidaHandle, metrologyData *workingData, volatile ADS_Instance *adsHandle, DLT645Buf *dlt645, LCD_instance *lcdHandle)
{
    adsHandle->ready            = HAL_GPIO_IN_00;
    adsHandle->sync             = HAL_GPIO_OUT_00;
    adsHandle->spiChan          = HAL_SPI_CHAN_0;
    adsHandle->spiCs            = HAL_SPI_CS_1;
    adsHandle->crcEnable        = ADS_CRC_ENABLE;
    adsHandle->crcType          = ADS_CRC_TYPE_CCITT;

    dlt645->uartChan            = HAL_UART_CHAN_0;

    workingData->activePulse    = HAL_GPIO_OUT_01;
    workingData->reactivePulse  = HAL_GPIO_OUT_02;

    tidaHandle->intr0Status    = 0x0;
    tidaHandle->intr1Status    = 0x0;
    tidaHandle->sync_reset     = HAL_GPIO_OUT_00;
    tidaHandle->adcStatus      = ADC_INIT;

    lcdHandle->lcdPin1 = DL_LCD_SEGMENT_LINE_7;
    lcdHandle->lcdPin2 = DL_LCD_SEGMENT_LINE_36;
    lcdHandle->lcdPin3 = DL_LCD_SEGMENT_LINE_38;
    lcdHandle->lcdPin4 = DL_LCD_SEGMENT_LINE_9;
    lcdHandle->lcdPin5 = DL_LCD_SEGMENT_LINE_11;
    lcdHandle->lcdPin6 = DL_LCD_SEGMENT_LINE_13;
    lcdHandle->lcdPin7 = DL_LCD_SEGMENT_LINE_50;
    lcdHandle->lcdPin8 = DL_LCD_SEGMENT_LINE_52;
    lcdHandle->lcdPin9 = DL_LCD_SEGMENT_LINE_54;
    lcdHandle->lcdPin10 = DL_LCD_SEGMENT_LINE_40;
    lcdHandle->lcdPin11 = DL_LCD_SEGMENT_LINE_22;
    lcdHandle->lcdPin12 = DL_LCD_SEGMENT_LINE_25;
    lcdHandle->lcdPin13 = DL_LCD_SEGMENT_LINE_27;
    lcdHandle->lcdPin14 = DL_LCD_SEGMENT_LINE_44;
    lcdHandle->lcdPin15 = DL_LCD_SEGMENT_LINE_30;
    lcdHandle->lcdPin16 = DL_LCD_SEGMENT_LINE_24;
    lcdHandle->lcdPin17 = DL_LCD_SEGMENT_LINE_26;
    lcdHandle->lcdPin18 = DL_LCD_SEGMENT_LINE_42;
    lcdHandle->lcdPin19 = DL_LCD_SEGMENT_LINE_39;
    lcdHandle->lcdPin20 = DL_LCD_SEGMENT_LINE_23;
    lcdHandle->lcdPin21 = DL_LCD_SEGMENT_LINE_3;
    lcdHandle->lcdPin22 = DL_LCD_SEGMENT_LINE_2;
    lcdHandle->lcdPin23 = DL_LCD_SEGMENT_LINE_58;
    lcdHandle->lcdPin24 = DL_LCD_SEGMENT_LINE_57;
    lcdHandle->lcdPin25 = DL_LCD_SEGMENT_LINE_56;
    lcdHandle->lcdPin26 = DL_LCD_SEGMENT_LINE_55;
    lcdHandle->lcdPin27 = DL_LCD_SEGMENT_LINE_21;
    lcdHandle->lcdPin28 = DL_LCD_SEGMENT_LINE_41;
    lcdHandle->lcdPin29 = DL_LCD_SEGMENT_LINE_53;
    lcdHandle->lcdPin30 = DL_LCD_SEGMENT_LINE_51;
    lcdHandle->lcdPin31 = DL_LCD_SEGMENT_LINE_49;
    lcdHandle->lcdPin32 = DL_LCD_SEGMENT_LINE_12;
    lcdHandle->lcdPin33 = DL_LCD_SEGMENT_LINE_10;
    lcdHandle->lcdPin34 = DL_LCD_SEGMENT_LINE_8;
    lcdHandle->lcdPin35 = DL_LCD_SEGMENT_LINE_37;
    lcdHandle->lcdPin36 = DL_LCD_SEGMENT_LINE_35;

}

/*!
 * @brief TIDA initialization
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
            phaseLog |= PHASE_LOG_DONE << ph;
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
            }
        }
    }

    if(phaseLog == SINGLE_PHASE_LOG_DONE)
    {
        Metrology_calculateTotalParameters(workingData);
        phaseLog = 0;
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

/*!
 * @brief Display the value on LCD
 * @param[in] lcdHandle   The LCD Instance
 * @param[in] value       The data to be displayed
 */
void TIDA_displayValue(LCD_instance *lcdHandle, int32_t value)
{
    char ch;
    /*
     * Position 8 is not connected.
     * Position 7 is used to display units.
     */
    LCD_POSITION lcdPosition = LCD_POSITION_6;
    LCD_clearDisplay(lcdHandle);
    if (value < 0)
    {
        value = abs(value);
    }
    while(lcdPosition >= LCD_POSITION_NULL)
    {
        ch = (value % 10) + '0';
        LCD_showChar(lcdHandle, ch, lcdHandle->lcdPinPosition[lcdPosition]);
        value = value/10;
        lcdPosition--;
        if ((value == 0) && (lcdPosition < LCD_POSITION_3))
        {
            break;
        }
    }
}

/*!
 * @brief Display on LCD
 * @param[in] lcdHandle   The LCD Instance
 */
void TIDA_displayOnLCD(LCD_instance *lcdHandle)
{
    if (displayParams > DISPLAY_APPARENT_POWER)
    {
        displayParams = DISPLAY_VOLTAGE;
    }
    int32_t value;
    switch (displayParams)
    {
    case DISPLAY_VOLTAGE:
        value = Metrology_getRMSVoltage(&gMetrologyworkingData, PHASE_ONE);
        TIDA_displayValue(lcdHandle, value);
        LCD_showDP(lcdHandle, lcdHandle->lcdPinPosition[LCD_POSITION_3]);
        LCD_showChar(lcdHandle, 'V', lcdHandle->lcdPinPosition[LCD_POSITION_7]);
        displayParams++;
        break;
    case DISPLAY_CURRENT:
        value = Metrology_getRMSCurrent(&gMetrologyworkingData, PHASE_ONE);
        value = value/1000;
        TIDA_displayValue(lcdHandle, value);
        LCD_showDP(lcdHandle, lcdHandle->lcdPinPosition[LCD_POSITION_3]);
        LCD_showChar(lcdHandle, 'A', lcdHandle->lcdPinPosition[LCD_POSITION_7]);
        displayParams++;
        break;
    case DISPLAY_ACTIVE_POWER:
        value = Metrology_getTotalActivePower(&gMetrologyworkingData);
        value = value/100;
        TIDA_displayValue(lcdHandle, value);
        LCD_showDP(lcdHandle, lcdHandle->lcdPinPosition[LCD_POSITION_5]);
        LCD_showChar(lcdHandle, 'W', lcdHandle->lcdPinPosition[LCD_POSITION_7]);
        displayParams++;
        break;
    case DISPLAY_REACTIVE_POWER:
        value = Metrology_getTotalReactivePower(&gMetrologyworkingData);
        value = value/100;
        TIDA_displayValue(lcdHandle, value);
        LCD_showDP(lcdHandle, lcdHandle->lcdPinPosition[LCD_POSITION_5]);
        LCD_showChar(lcdHandle, 'R', lcdHandle->lcdPinPosition[LCD_POSITION_7]);
        displayParams++;
        break;
    case DISPLAY_APPARENT_POWER:
        value = Metrology_getTotalApparentPower(&gMetrologyworkingData);
        value = value/100;
        TIDA_displayValue(lcdHandle, value);
        LCD_showDP(lcdHandle, lcdHandle->lcdPinPosition[LCD_POSITION_5]);
        LCD_showChar(lcdHandle, 'P', lcdHandle->lcdPinPosition[LCD_POSITION_7]);
        displayParams++;
        break;
    default:
        /* Do nothing   */
        break;
    }
}


