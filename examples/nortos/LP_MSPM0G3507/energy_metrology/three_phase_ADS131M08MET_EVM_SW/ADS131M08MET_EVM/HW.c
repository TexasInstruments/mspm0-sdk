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
#include "HW.h"

/*! @brief GPIO Pins that control Current gain  */
HAL_GPIO_IN CurrentGainIOs [] = {HAL_GPIO_IN_01, HAL_GPIO_IN_02, HAL_GPIO_IN_03};
/*! @brief GPIO Pins that control Voltage gain  */
HAL_GPIO_IN VoltageGainsIOs [] = {HAL_GPIO_IN_04, HAL_GPIO_IN_05, HAL_GPIO_IN_06};

/*!
 * @brief Set ADS Gain
 * @param[in] adsHandle  The ADS instance
 */
void HW_setADCGain(volatile ADS_Instance *adsHandle)
{
    uint16_t currentGain = HW_readGainPins(CurrentGainIOs, 3);
    uint16_t voltageGain = HW_readGainPins(VoltageGainsIOs, 3);

    /* Check the ADC datasheet for gain registers and map the gains, also found in ADS131M0x.h    */
    adsHandle->gain1 = (currentGain << 12) | (voltageGain << 8) | (voltageGain << 4) | (voltageGain << 0);
    adsHandle->gain2 = (0 << 12) | (currentGain << 8) | (currentGain << 4) | (currentGain << 0);
}

/*!
 * @brief Read GPIO pins for gain setting
 * @param[in] gainPins  group of pins to be read
 * @param[in] numPins   number of pins to be read
 */
uint8_t HW_readGainPins(HAL_GPIO_IN gainPins[], uint8_t numPins)
{
    uint8_t gain = 0;

    /* Totals the gain, each GPIO is a new bit, so the left shift will set the value on the correct placement   */
    for(uint8_t i = 0; i < numPins; i++)
    {
        gain += HAL_readGPIOPin(gainPins[i]) * (1 << i);
    }

    return gain;  
}
