/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#include <ti/iqmath/include/IQmathLib.h>
#include "ti_msp_dl_config.h"

/* clang-format off */

/*
 * The following trim parameter are provided in the device datasheet in chapter
 * "Temperature Sensor"
 */
#define TEMP_TS_TRIM_C                                            ((uint32_t)30)
/*
 * Constant below is (1/TSc). Where TSc is Temperature Sensor coefficient
 * available in the device datasheet
 */
#define TEMP_TS_COEF_mV_C                                             (-555.55f)

#define ADC_VREF_VOLTAGE                                                  (3.3f)
#define ADC_BIT_RESOLUTION                                   ((uint32_t)(1)<<12)

/* clang-format off */

volatile bool gCheckADC;
volatile float gTemperatureDegC;
volatile float gTemperatureDegF;

int main(void)
{
    uint32_t adcResult;

    _iq15 qVSample, qTsample, qTempDegF, qVTrim;


    SYSCFG_DL_init();


    /*
     * Convert TEMP_SENSE0 result to equivalent voltage:
     * Vtrim = (ADC_VREF_VOLTAGE*(TEMP_SENSE0 -0.5))/(2^12)
     */

    qVTrim = _IQ15div(_IQ15mpy((_IQ15(DL_SYSCTL_getTempCalibrationConstant()) -
            _IQ15(0.5)), _IQ15(ADC_VREF_VOLTAGE)), ((uint32_t)(1) << 27));


    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);


    gCheckADC = false;
    while (1) {

        DL_ADC12_startConversion(ADC12_0_INST);

        while (false == gCheckADC) {
            __WFE();
        }
        gCheckADC = false;

        DL_ADC12_stopConversion(ADC12_0_INST);

        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);


        /*
         * Convert ADC result to equivalent voltage:
         * Vsample = (VREF_VOLTAGE_MV*(adcResult -0.5))/(2^ADC_BIT_RESOLUTION)
         */

        qVSample = _IQ15div(_IQ15mpy((adcResult << 15) -
                _IQ15(0.5),_IQ15(ADC_VREF_VOLTAGE)), _IQ15(ADC_BIT_RESOLUTION));

        /*
         * Apply temperature sensor calibration data
         * TSAMPLE = (TEMP_TS_COEF_mV_C) * (qAdcResultV - vTrim) + TEMP_TS_TRIM_C
         */
        qTsample = _IQ15mpy(_IQ15(TEMP_TS_COEF_mV_C), (qVSample - qVTrim)) +
                            (TEMP_TS_TRIM_C << 15);


        qTempDegF = _IQ15mpy(qTsample , _IQ15(9.0/5.0)) + _IQ15(32.0);


        gTemperatureDegC = _IQ15toF(qTsample);
        gTemperatureDegF = _IQ15toF(qTempDegF);

        __BKPT(0);

    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC = true;
            break;
        default:
            break;
    }
}
