/* --COPYRIGHT--,BSD
 * Copyright (c) 2019, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
#include <Src/DualRaySmokeAFE_HAL/DualRaySmokeAFE_HAL_Config_Private.h>
#include <Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/tps880x_mspm0_i2c/tps880x_mspm0_i2c.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include "ti_msp_dl_config.h"

/**** Local Variables *********************************************************/
// ADC State/Mode
static enum tADC {
    ADC_Disabled = 0,
    ADC_AFE_Measurement,
    ADC_AFE_MeasurementComplete,
    ADC_AFE_Temperature,
} eADC;

static uint8_t u8PGA_Gain[DUALRAYSMOKEAFE_LED_TOTAL];
static uint16_t u16LEDHoldOn[DUALRAYSMOKEAFE_LED_TOTAL];
static uint16_t u16LEDHoldOff[DUALRAYSMOKEAFE_LED_TOTAL];
static uint8_t u8TIAMask;
static uint16_t u16ADCCounter, u16ADCMeasurements;
#if (DUALRAYSMOKEAFE_HAL_AFE_LED_SAMPLE_BUFFER_SIZE <= 16)
static uint16_t u16ADCAccumulator;
#else
#error "More than 16 ADC samples can overflow uint16_t."
#endif
#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
static bool bCOEnabled;
#endif

/**** Local Function Prototypes ***********************************************/
static int16_t ADCTriggerAccumulatedSamples(
    uint16_t num_samples, uint16_t *result);
static uint16_t ADCCompensateResult(uint16_t ADCData, uint16_t reference);
static void enable_TPS880x_LED_reflection(
    tDualRaySmokeAFE_LED LED, bool LEDON, uint8_t gain);
static void IntRefEnable(uint16_t settingmV);
static void IntRefDisable(void);

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1)
static int8_t DualRaySmokeAFE_HAL_AFE_TemperatureMeasurementExt(
    int *measurement);
static int ADCTemperatureCalcExt(uint16_t ADCData, uint16_t reference);
#endif
#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1)
static int8_t DualRaySmokeAFE_HAL_AFE_TemperatureMeasurementInt(
    int *measurement);
static int ADCTemperatureCalcInt(uint16_t ADCData, uint16_t reference);
#endif

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
static uint16_t ADCLEDCurrentCalc(
    uint16_t ADCData, uint16_t reference, uint16_t resistance);
#endif

/**** Functions ***************************************************************/
int8_t DualRaySmokeAFE_HAL_AFE_Init(tDualRaySmokeAFE_HAL_AFE_Config *Config)
{
    uint16_t i;
#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
    uint8_t CO_Val;
#endif
    uint8_t TempCoeff_Val[DUALRAYSMOKEAFE_LED_TOTAL];

    // Measurement overhead is fixed and defined by HAL
    Config->u16LEDMeasOverheadus =
        (uint16_t)(DUALRAYSMOKEAFE_HAL_AFE_ADC_LEADMEAS_TOTALTIME_US);

    // Enable or disable the Photodiode TIA and PGA
    if (Config->bPDTIAEn == true) {
        // Set PAMP_EN and PGAIN_EN bits
        u8TIAMask = PAMP_EN | PGAIN_EN;
    } else {
        // clear PAMP_EN and PGAIN_EN bits
        u8TIAMask = 0;
    }

    for (i = 0; i < DUALRAYSMOKEAFE_LED_TOTAL; i++) {
        // Set PGA Gain register value based on desired setting
        switch (Config->u16PGA_Gain[i]) {
            case 5:
                u8PGA_Gain[i] = (PGAIN__5);
                break;
            case 11:
                u8PGA_Gain[i] = (PGAIN__11);
                break;
            case 20:
                u8PGA_Gain[i] = (PGAIN__20);
                break;
            case 35:
                u8PGA_Gain[i] = (PGAIN__35);
                break;
            default:
                return -1;  // unsupported Gain
        }

        switch (Config->u16LED_TempCoeff[i]) {
            // Set register value based on desired coefficient
            // Note that the register contains the TEMPCOA value which is bit 4-3
            // TempCoeff_Val for LED A is OK, but LED B must be shifted by 2
            case 347:
                TempCoeff_Val[i] = TEMPCOA__347MV_C;
                break;
            case 416:
                TempCoeff_Val[i] = TEMPCOA__416MV_C;
                break;
            case 693:
                TempCoeff_Val[i] = TEMPCOA__693MV_C;
                break;
            case 1040:
                TempCoeff_Val[i] = TEMPCOA__1040MV_C;
                break;
            default:
                return -1;  // unsupported Coefficient
        }

        // Set LED hold on/off time
        u16LEDHoldOn[i]  = Config->u16LEDHoldOnTime[i];
        u16LEDHoldOff[i] = Config->u16LEDHoldOffTime[i];
    }

    // Set DAC value value based on desired setting
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_LED_DAC_A,
        Config->u16LED_Strength[DUALRAYSMOKEAFE_LED_RED]);
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_LED_DAC_B,
        Config->u16LED_Strength[DUALRAYSMOKEAFE_LED_BLUE]);

    // Set Temperature coefficient setting
    // Coefficient for blue is shifted by 2 to fill bits 6-5
    tps880x_msp430_i2c_UpdateBitsReg(TPS880X_REG_PH_CTRL,
        TEMPCOB_0 | TEMPCOB_1 | TEMPCOA_0 | TEMPCOA_1,
        TempCoeff_Val[DUALRAYSMOKEAFE_LED_RED] |
            (TempCoeff_Val[DUALRAYSMOKEAFE_LED_BLUE] << 2));

    //    // Disable LED by default LEDEN
    DL_GPIO_clearPins(TPS_PORT, TPS_LEDEN_PIN);

    eADC               = ADC_Disabled;  // ADC disabled by default
    u16ADCCounter      = 0;
    u16ADCMeasurements = 0;
    u16ADCAccumulator  = 0;

    DL_ADC12_disableConversions(ADC0_INST);
#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
    CO_Val     = 0;
    bCOEnabled = Config->bCOMeasEn;
    // Enable or disable the CO Amplifier
    if (Config->bCOAmpEn == true) {
        // Enable the CO Amplifier
        CO_Val |= COAMP_EN;
    }
    // Get CO gain register value based on requested setting
    switch (Config->u16COGain) {
        case 1100:
            CO_Val |= COGAIN___1100K;
            break;
        case 300:
            CO_Val |= COGAIN___300K;
            break;
        case 500:
            CO_Val |= COGAIN___500K;
            break;
        case 800:
            CO_Val |= COGAIN___800K;
            break;
        default:
            return -1;  // unsupported Gain
    }

    // Update the value of the register
    tps880x_msp430_i2c_UpdateBitsReg(
        TPS880X_REG_CO, COAMP_EN | COGAIN_0 | COGAIN_1, CO_Val);

    //    // Disable CO_TEST_EN by default
    DL_GPIO_clearPins(CO_PORT, CO_TEST_ENABLE_PIN);

#endif
    return 0;
}

int8_t DualRaySmokeAFE_HAL_AFE_Measurement(
    tDualRaySmokeAFE_HAL_AFE_SmokeMeasurement *measurement)
{
    tDualRaySmokeAFE_LED LED;
    uint16_t adc_accum_result;

    // Set VBST=4.7V
    tps880x_msp430_i2c_WriteReg(
        TPS880X_REG_VBOOST, BST_CLIM__500MA | VBST__9V);
    // Enable boost and LEDLDO
    tps880x_msp430_i2c_SetBitsReg(TPS880X_REG_ENABLE1, BST_EN | LEDLDO_EN);

    // Wait for Boost and LED LDO
    DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingLPDelayms(
        DUALRAYSMOKEAFE_HAL_AFE_DELAY_VBOOST_ON_MS);

    // LEDLDO_EN=0. PAMP_EN=PGAIN_EN=1 depending on TIA enable bit
    tps880x_msp430_i2c_UpdateBitsReg(
        TPS880X_REG_ENABLE1, LEDLDO_EN | PAMP_EN | PGAIN_EN, u8TIAMask);

    // AMUX_SEL=AOUT_PH
    tps880x_msp430_i2c_UpdateBitsReg(
        TPS880X_REG_GPIO_AMUX, AMUX_SEL_1 | AMUX_SEL_0, AMUX_SEL__AOUT_PH);

    for (LED = DUALRAYSMOKEAFE_LED_RED; LED < DUALRAYSMOKEAFE_LED_TOTAL;
         LED++) {
        // Configure ADC to repeat-single-channel mode
        //  With Rs = 4.7K, Cext = 330pF, Ci = 5.5pF, Ri = 2K
        //      tsample >= (Ri + Rs) x ln(2^(12+2)) x (Ci + Cext)
        //      tsample >= 20.25us
        //      SHT_5 = 96 cycles SMCLK/2(4MHz) = 24us
#if (DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SNH == 96)

        DL_ADC12_setSampleTime0(
            ADC0_INST, 25);  // 1MHz ADC clock.  25 = 25 us sample time

#else
#error "ADC S&H configuration not supported. Modify AFE."
#endif

        // Input = AMUX to A0_8, use External VeREF
        DL_ADC12_configConversionMem(ADC0_INST, DL_ADC12_MEM_IDX_0,
            DL_ADC12_INPUT_CHAN_8, DL_ADC12_REFERENCE_VOLTAGE_EXTREF,
            DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0,
            DL_ADC12_AVERAGING_MODE_DISABLED,
            DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT,
            DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

        // Turn on IR LED light with strength = 0
        enable_TPS880x_LED_reflection(LED, false, u8PGA_Gain[LED]);

        // Wait for Ref, AFE settling
        DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(
            u16LEDHoldOff[LED]);

        // Trigger ADC to take n samples for dark measurement
        ADCTriggerAccumulatedSamples(DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES,
                                         &adc_accum_result);

        // Store accumulated result in array temporarily
        // Finish calculations after turning OFF the AFE.
        measurement->u16Dark[LED] = adc_accum_result;

        // Turn on LED with corresponding light strength
        enable_TPS880x_LED_reflection(LED, true, u8PGA_Gain[LED]);

        // Delay waiting for AFE
        DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(
            u16LEDHoldOn[LED]);

        // Trigger ADC to take n samples for light measurement
        ADCTriggerAccumulatedSamples(
            DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES, &adc_accum_result);
        // Store accumulated result in array temporarily
        // Finish calculations after turning OFF the AFE.
        measurement->u16Light[LED] = adc_accum_result;

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
        // Configure ADC to repeat-single-channel mode
        //      SHT_0 = 4 cycles SMCLK/2(4MHz) = 1us
#if (DUALRAYSMOKEAFE_HAL_AFE_ADC_LEDCURR_SNH == 4)

        //update

//            DL_ADC12_setSampleTime0(ADC0_INST,1);  // 1MHz ADC clock.  1 = 1 us sample time

#else
#error "ADC S&H configuration not supported. Modify AFE."
#endif

        // update
        // Measure the LED current
        if (LED == DUALRAYSMOKEAFE_LED_RED) {
                // IR LED: CSA on A0_7, use external Vref+
                DL_ADC12_configConversionMem(ADC0_INST, DL_ADC12_MEM_IDX_0,
                        DL_ADC12_INPUT_CHAN_7, DL_ADC12_REFERENCE_VOLTAGE_EXTREF, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
                        DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);
                // Trigger ADC to take n samples for LED current
                ADCTriggerAccumulatedSamples(
                DUALRAYSMOKEAFE_HAL_AFE_LEDCURRENT_NUM_SAMPLES, &adc_accum_result);


                // Store accumulated result in array temporarily
                // Finish calculations after turning OFF the AFE.
                measurement->u16LEDCurrent[LED] = adc_accum_result;


        } else {
                // Blue LED: CSB on PA17 and routed through OPA1_IN1- and using DAC as reference, use external Vref+
                DL_ADC12_configConversionMem(ADC0_INST, DL_ADC12_MEM_IDX_0,
                            DL_ADC12_INPUT_CHAN_13, DL_ADC12_REFERENCE_VOLTAGE_EXTREF, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
                            DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);


                // Trigger ADC to take n samples for LED current
                ADCTriggerAccumulatedSamples(
                DUALRAYSMOKEAFE_HAL_AFE_LEDCURRENT_NUM_SAMPLES, &adc_accum_result);


                // Store accumulated result in array temporarily
                // Finish calculations after turning OFF the AFE.

                //WORKAROUND: due to limited GPIO, this signal is being routed through the OPA1_IN1- pin.  This inverts the signal.
                //The DAC reference voltage is making the signal start at VREF+ and move down in relation to the signal.  IE: real signal = 4096 - measured signal
                measurement->u16LEDCurrent[LED] = ((4096 * DUALRAYSMOKEAFE_HAL_AFE_LEDCURRENT_NUM_SAMPLES) - adc_accum_result);
        }
        // Make sure LED is OFF
        DL_GPIO_clearPins(TPS_PORT, TPS_LEDEN_PIN);

        // Store accumulated result in array temporarily
        // Finish calculations after turning OFF the AFE.
//            measurement->u16LEDCurrent[LED] = adc_accum_result;

#else
        // Make sure LED is OFF
        DL_GPIO_clearPins(TPS_PORT, TPS_LEDEN_PIN);
#endif  // (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
    }

#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)

    if (bCOEnabled == true) {
        // Configure ADC to repeat-single-channel mode
        //  With Rs = 4.7K, Cext = 330pF, Ci = 5.5pF, Ri = 2K
        //      tsample >= (Ri + Rs) x ln(2^(12+2)) x (Ci + Cext)
        //      tsample >= 20.25us
        //      SHT_5 = 96 cycles SMCLK/2(4MHz) = 24us
#if (DUALRAYSMOKEAFE_HAL_AFE_ADC_PD_SNH == 96)

        DL_ADC12_setSampleTime0(
            ADC0_INST, 25);  // 1MHz ADC clock.  25 = 25 us sample time

#else
#error "ADC S&H configuration not supported. Modify AFE."
#endif
        // Input = AMUX to A0_8, use External VeREF
        // AMUX_SEL=CO
        DL_ADC12_configConversionMem(ADC0_INST, DL_ADC12_MEM_IDX_0,
                DL_ADC12_INPUT_CHAN_8, DL_ADC12_REFERENCE_VOLTAGE_EXTREF, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
                DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);


        tps880x_msp430_i2c_UpdateBitsReg(
            TPS880X_REG_GPIO_AMUX, AMUX_SEL_1 | AMUX_SEL_0, AMUX_SEL__COO);
        // Delay waiting for AFE
        DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(
            DUALRAYSMOKEAFE_HAL_AFE_DELAY_MEASUREMENT3);

        // Trigger ADC to take n samples for CO measurement
        ADCTriggerAccumulatedSamples(
            DUALRAYSMOKEAFE_HAL_AFE_CO_NUM_SAMPLES, &adc_accum_result);
        // Store accumulated result, perform calculations after AFE is OFF
        measurement->u16CO = adc_accum_result;
    }

#endif

    // Wait before turning OFF Boost
    DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingLPDelayms(
        DUALRAYSMOKEAFE_HAL_AFE_DELAY_VBOOSTOFF_MS);
    // Disable boost
    tps880x_msp430_i2c_ClearBitsReg(TPS880X_REG_ENABLE1, BST_EN);

    // Perform calculations to get average measurements and calibrate results
    for (LED = DUALRAYSMOKEAFE_LED_RED; LED < DUALRAYSMOKEAFE_LED_TOTAL;
         LED++) {
        // Get the average for dark measurement
        measurement->u16Dark[LED] = measurement->u16Dark[LED] /
                                    DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES;
        // Get calibrated result for dark measurement
        measurement->u16Dark[LED] =
            ADCCompensateResult(measurement->u16Dark[LED], 0);
        // Get the average of light measurement
        measurement->u16Light[LED] = measurement->u16Light[LED] /
                                     DUALRAYSMOKEAFE_HAL_AFE_LED_NUM_SAMPLES;
        // Get calibrated result
        measurement->u16Light[LED] =
            ADCCompensateResult(measurement->u16Light[LED], 0);
        // Get the difference between dark and light
        measurement->i16Diff[LED] =
            measurement->u16Light[LED] - measurement->u16Dark[LED];
#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
        // Get the average for LED current
        measurement->u16LEDCurrent[LED] =
            measurement->u16LEDCurrent[LED] /
            DUALRAYSMOKEAFE_HAL_AFE_LEDCURRENT_NUM_SAMPLES;
        // Get calibrated result for LED current
        measurement->u16LEDCurrent[LED] =
            ADCCompensateResult(measurement->u16LEDCurrent[LED], 0);
#endif
    }

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
    // Get the actual current
    measurement->u16LEDCurrent[DUALRAYSMOKEAFE_LED_RED] =
        ADCLEDCurrentCalc(measurement->u16LEDCurrent[DUALRAYSMOKEAFE_LED_RED],
            DUALRAYSMOKEAFE_HAL_AFE_LED_VREF_MV,
            DUALRAYSMOKEAFE_HAL_AFE_LED_CSA_RESISTANCE);
    measurement->u16LEDCurrent[DUALRAYSMOKEAFE_LED_BLUE] =
        ADCLEDCurrentCalc(measurement->u16LEDCurrent[DUALRAYSMOKEAFE_LED_BLUE],
            DUALRAYSMOKEAFE_HAL_AFE_LED_VREF_MV,
            DUALRAYSMOKEAFE_HAL_AFE_LED_CSB_RESISTANCE);
#endif

#if (DUALRAYSMOKEAFELIB_ENABLE_CO_MEAS == 1)
    if (bCOEnabled == true) {
        // Take average of CO samples
        measurement->u16CO =
            measurement->u16CO / DUALRAYSMOKEAFE_HAL_AFE_CO_NUM_SAMPLES;
        // Get calibrated result
        measurement->u16CO = ADCCompensateResult(measurement->u16CO, 0);
    } else {
        measurement->u16CO = 0;
    }

#endif

    return 0;  // OK
}

#if ((DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1) || \
     (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1))
int8_t DualRaySmokeAFE_HAL_AFE_TemperatureMeasurement(
    tDualRaySmokeAFE_HAL_AFE_TemperatureMeasurement *measurement)
{
    int8_t ret = 0;

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1)
    ret = DualRaySmokeAFE_HAL_AFE_TemperatureMeasurementExt(
        &(measurement->iq15TemperatureExtC));
#endif
#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1)
    ret = DualRaySmokeAFE_HAL_AFE_TemperatureMeasurementInt(
        &(measurement->iq15TemperatureIntC));
#endif

    return ret;
}

#endif

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1)
static int8_t DualRaySmokeAFE_HAL_AFE_TemperatureMeasurementExt(
    int *measurement)
{
    uint16_t meas;

    // Turn ON the external sensor
    DL_GPIO_setPins(TMP_SENSOR_PORT, TMP_SENSOR_PWR_PIN);

    eADC = ADC_AFE_Temperature;  // Set ADC to perform Temp measurement

    // Input = TMP_OUT to A0_2, use External VeREF
    DL_ADC12_configConversionMem(ADC0_INST, ADC0_ADCMEM_EXT_TEMP_SENSOR,
            DL_ADC12_INPUT_CHAN_2, DL_ADC12_REFERENCE_VOLTAGE_EXTREF, DL_ADC12_SAMPLE_TIMER_SOURCE_SCOMP0, DL_ADC12_AVERAGING_MODE_DISABLED,
            DL_ADC12_BURN_OUT_SOURCE_DISABLED, DL_ADC12_TRIGGER_MODE_AUTO_NEXT, DL_ADC12_WINDOWS_COMP_MODE_DISABLED);

    // Wait for Temperature sensor to turn ON
    DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(
        DUALRAYSMOKEAFE_HAL_AFE_DELAY_TEMPERATURE_EXT_US);

    // Trigger ADC to take n samples for temperature sensor
    ADCTriggerAccumulatedSamples(DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_NUM_SAMPLES,
                                         &meas);

    // Turn OFF the external sensor
    DL_GPIO_clearPins(TMP_SENSOR_PORT, TMP_SENSOR_PWR_PIN);

    // Get the average
    meas = meas / DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_NUM_SAMPLES;
    // Get ADC calibrated result
    //Not done for MSP- M0
    meas = ADCCompensateResult(
        meas, DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_VREF_MV);

    // Calculate temperature using LMT84 calibration
    *measurement = ADCTemperatureCalcExt(
        meas, DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_VREF_MV);

    return 0;  // OK
}
#endif

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1)
static int8_t DualRaySmokeAFE_HAL_AFE_TemperatureMeasurementInt(
    int *measurement)
{
    //TODO Needs to be updated/ported but internal temp sensor not available on first customer MSP M0 samples so currently just commented out.

    //    uint16_t meas;
    //
    //    // enable internal Vref
    //    IntRefEnable(DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_VREF_MV);
    //    // Enable internal temp sensor
    //    PMMCTL2 |= TSENSOREN;
    //
    //    eADC = ADC_AFE_Temperature;         // Set ADC to perform Temp measurement
    //
    //    // Configure ADC for single conversion of Temperature sensor
    //    // ADCON, sampling period > 30us
    //    //      SHT_8 = 256 cycles SMCLK/2(4MHz) = 64us
    //    ADCCTL0 = ADCSHT_8  | ADCON;
    //#if ( (HSBUS_FREQ_MHZ == 8) && \
//      (DUALRAYSMOKEAFE_HAL_AFE_ADC_CLOCKFREQ_HZ == 4000000) )
    //    // Pulse Sample mode, repeat single channel, SW trigger, Use SMCLK/2
    //    ADCCTL1 = ADCSHP | ADCCONSEQ_2 | ADCSHS_0 | ADCSSEL_2 | ADCDIV_1;
    //#else
    //#error "ADC clock configuration not supported. Modify AFE."
    //#endif
    //    // 12-bit conversion results
    //    ADCCTL2 = ADCRES_2;
    //    // Input = temp sense, use Internal VREF
    //    ADCMCTL0 = ADCINCH_12 | ADCSREF_1;
    //
    //    // Wait for Int Ref
    //    DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(
    //                                    DUALRAYSMOKEAFE_HAL_AFE_DELAY_TEMPERATURE_INT_US);
    //
    //    // Trigger ADC to take n samples for temperature sensor
    //    ADCTriggerAccumulatedSamples(DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_NUM_SAMPLES,
    //                                             &meas);
    //
    //    ADCCTL0 &= ~ (ADCENC + ADCSC);      // Disable ADC
    //    ADCCTL0 &= ~ADCON;
    //
    //    PMMCTL2 &= ~TSENSOREN;  // Disable temp sensor
    //    IntRefDisable();                    // disable internal Vref
    //
    //    // Get the average
    //    meas = meas / DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_NUM_SAMPLES;
    //    // Get ADC calibrated result
    //    meas = ADCCompensateResult(meas,
    //                              DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_VREF_MV);
    //
    //    // Calculate temperature using 3 point calibration
    //    *measurement = ADCTemperatureCalcInt(meas,
    //                               DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_INT_VREF_MV);
    //
    return 0;  // OK
}
#endif

/**** Local Functions *********************************************************/
//! \brief Triggers ADC to perform multiple samples which will be accumulated
//!         ADC must be preconfigured and just waiting for ADCSC trigger.
//!         Function will block until all measurements are taken.
//!         ADC interrupts will be enabled in function and disabled on exit.
//!
//! \param[in] num_samples is the number of samples
//! \param[out] result is a pointer where result will be stored
//!
//! \return 0 if OK, -1 if error
static int16_t ADCTriggerAccumulatedSamples(
    uint16_t num_samples, uint16_t *result)
{
    // Set ADC to take light measurements
    eADC = ADC_AFE_Measurement;  // Set ADC to perform AFE measurement
    u16ADCMeasurements = num_samples;
    u16ADCCounter      = 0;
    u16ADCAccumulator  = 0;

    NVIC_EnableIRQ(ADC0_INST_INT_IRQN);

    /* Start a new conversion  */

    DL_ADC12_clearInterruptStatus(ADC0_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
    DL_ADC12_enableInterrupt(ADC0_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
    DL_ADC12_enableConversions(ADC0_INST);
    DL_ADC12_startConversion(ADC0_INST);

    while (eADC != ADC_AFE_MeasurementComplete) {
        __WFE();
        __NOP();
    }

    __NOP(); /* Set breakpoint here to check result */
    *result = u16ADCAccumulator;

    return 0;
}


//! \brief Adjusts the ADC result based on device calibration.
//!
//! \param[in] ADCData is the raw ADC
//! \param[in] reference is the reference in mV (set to 0 if not using int VREF)
//!
//! \return uint16_t with the calibrated ADC result
static uint16_t ADCCompensateResult(uint16_t ADCData, uint16_t reference)
{
    //No ADC Compensation required on MSP M0.  Legacy MSP430 function.

    return (ADCData);
}

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS == 1)
//! \brief Calculates ADC measurement for a specific voltage using the
//!         internal reference
//!
#define     LMT84_V(x)  (x*4096.0/DUALRAYSMOKEAFE_HAL_AFE_TEMPERATURE_EXT_VREF_MV)

//! \brief Lookup table for LMT84. Datasheet specifies voltage outputs with
//!         1C accuracy. This implementation uses 25C accuracy and a 3 point
//!         interpolation.
//!
//!
static int lmt84_lookup_table[] = {
    (LMT84_V(1299)),  // -50C     ~3547.136
    (LMT84_V(1168)),  // -25C     ~3189.418667
    (LMT84_V(1034)),  //  0C      ~2823.509333
    (LMT84_V(898)),   //  25C     ~2452.138667
    (LMT84_V(760)),   //  50C     ~2075.306667
    (LMT84_V(619)),   //  75C     ~1690.282667
    (LMT84_V(476)),   //  100C    ~1299.797333
    (LMT84_V(332)),   //  125C    ~906.5813333
    (LMT84_V(183)),   //  150C    ~499.712
};

//! \brief Calculates temperature in C, given the ADC result
//!
//! \param[in] ADCData is the raw ADC
//! \param[in] reference is the reference in mV
//!
//! \return IQ15 with temperature in Celsius. 17 integer, 15 fractional.
static int ADCTemperatureCalcExt(uint16_t ADCData, uint16_t reference)
{
    int tempHigh, tempLow;
    int tempC;
    uint16_t i;

    tempC = (ADCData);

    // Check for low limit
    if (tempC >= lmt84_lookup_table[0]) {
        return (-50.0);
    }

    for (i = 0;
         i < (sizeof(lmt84_lookup_table) / sizeof(lmt84_lookup_table[0])) - 1;
         i++) {
        // Find the 2 points where the temperature falls in between
        if ((tempC < lmt84_lookup_table[i]) &&
            (tempC > lmt84_lookup_table[i + 1])) {
            tempLow  = (int) (25 * (i) -50);  // Low temperature
            tempHigh = tempLow + (25);        // High temperature
            //          (ADC - ADC_Low) * (tempHigh-tempLow))
            //  tempC = ----------------------------- + tempLow
            //              (ADC_High - ADC_Low)
            tempC = (tempC - lmt84_lookup_table[i]) * (tempHigh - tempLow);
            tempC =
                (tempC) / (lmt84_lookup_table[i + 1] - lmt84_lookup_table[i]);
            tempC += tempLow;
            return tempC;
        }
    }

    return (150.0);  // Maximum limit
}
#endif  // DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_EXT_MEAS

#if (DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS == 1)
//! \brief Calculates temperature in C, given the ADC result
//!
//! \param[in] ADCData is the raw ADC
//! \param[in] reference is the reference in mV
//!
//! \return IQ15 with temperature in Celsius. 17 integer, 15 fractional.
static int ADCTemperatureCalcInt(uint16_t ADCData, uint16_t reference)
{
    //TODO ONLY USED FOR INTERNAL TEMP MEASUREMENT.  DID NOT PORT FROM MSP430 YET

    int calib_30C, calib_105C;
    int tempC;

    //    // These calculations are done in IQMath to reduce overhead
    //    // IQ15 has a range of -65,536 to 65,535.999
    //    // It has a resolution of .000 030 518
    //    if (reference == 2500)
    //    {
    //        calib_30C = int(CAL_ADC_TEMPERATURE_2_5V_30C);
    //        calib_105C = int(CAL_ADC_TEMPERATURE_2_5V_105C);
    //    }
    //    else if (reference == 1500)
    //    {
    //        calib_30C = int(CAL_ADC_TEMPERATURE_1_5V_30C);
    //        calib_105C = int(CAL_ADC_TEMPERATURE_1_5V_105C);
    //    }
    //    else
    //    {
    //        calib_30C = int(CAL_ADC_TEMPERATURE_2_0V_30C);
    //        calib_105C = int(CAL_ADC_TEMPERATURE_2_0V_105C);
    //    }
    //    // Use calibration data to calculate using linear approximation
    //    //          (ADC - calib30C) * (105-30))
    //    //  tempC = ----------------------------- + 30
    //    //              (calib105C - calib30C)
    //    tempC = intmpy((int(ADCData) - calib_30C) , int(105-30));
    //    tempC = intdiv( tempC , (calib_105C - calib_30C) );
    //    tempC += int(30);

    return (tempC);
}
#endif  //DUALRAYSMOKEAFELIB_ENABLE_TEMPERATURE_INT_MEAS

#if (DUALRAYSMOKEAFELIB_ENABLE_LEDCURRENT_MEAS == 1)
//! \brief Calculates the LED current
//!
//! \param[in] ADCData is the raw ADC
//! \param[in] reference is the reference in mV
//! \param[in] resistance is the shunt resistance in ohms
//!
//! \return uint16_t with the mA measured (integer only)
static uint16_t ADCLEDCurrentCalc(
    uint16_t ADCData, uint16_t reference, uint16_t resistance)
{
    uint32_t temp;

    // Voltage = ADCData x Reference / 4096
    // Current = Voltage / resistance
    temp = ((uint32_t) ADCData * (uint32_t) reference) / 4096;
    return (temp / resistance);
}
#endif

//! \brief Enables TPS880x to measure LED reflection.
//!
//! \param[in] LED is the LED used for measurement.
//! \param[in] LEDON is true if LED is ON, false if LED off.
//! \param[in] gain is the PGA gain
//!
//! \return none
static void enable_TPS880x_LED_reflection(
    tDualRaySmokeAFE_LED LED, bool LEDON, uint8_t gain)
{
    tps880x_msp430_i2c_UpdateBitsReg(
        TPS880X_REG_PH_CTRL, PGAIN_0 | PGAIN_1, gain);
    if (LED == DUALRAYSMOKEAFE_LED_RED) {
        // Set the photo gain
        tps880x_msp430_i2c_UpdateBitsReg(
            TPS880X_REG_ENABLE2, LEDSEL | SLP_EN, 0);
    } else if (LED == DUALRAYSMOKEAFE_LED_BLUE) {
        tps880x_msp430_i2c_UpdateBitsReg(
            TPS880X_REG_ENABLE2, LEDSEL | SLP_EN, LEDSEL);
    }

    if (LEDON == true) {
        //        MAP_GPIO_setOutputHighOnPin(DUALRAYSMOKEAFE_HAL_AFE_LEDEN_PORT,
        //                                   DUALRAYSMOKEAFE_HAL_AFE_LEDEN_PIN);
        DL_GPIO_setPins(TPS_PORT, TPS_LEDEN_PIN);
    } else {
        //        MAP_GPIO_setOutputLowOnPin(DUALRAYSMOKEAFE_HAL_AFE_LEDEN_PORT,
        //                               DUALRAYSMOKEAFE_HAL_AFE_LEDEN_PIN);
        DL_GPIO_clearPins(TPS_PORT, TPS_LEDEN_PIN);
    }
}

//! \brief Enables the Internal Shared reference
//!
//! \param[in] settingmV sets reference voltage.
//!             1500=1.5V, 2000=2.0V, any other value = 2.5V
//!
//! \return none
static void IntRefEnable(uint16_t settingmV)
{
    //TODO REF CONFIGURED IN CONFIG.C
    //No low power optimizations done yet
}

//! \brief Disables the Internal Shared reference
//!
//! \return none
static void IntRefDisable(void)
{
    //TODO REF CONFIGURED IN CONFIG.C
}

//ADC IRQ for EXT TEMP and measurements...  And CSA/CSB.
//TODO update this
void ADC0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:

            __NOP(); /* Set breakpoint here to check result */
            if ((eADC == ADC_AFE_Measurement) ||
                (eADC == ADC_AFE_Temperature)) {
                if (u16ADCCounter < u16ADCMeasurements) {
                    // Accumulate result to obtain average
                    u16ADCAccumulator +=
                        DL_ADC12_getMemResult(ADC0_INST, DL_ADC12_MEM_IDX_0);
                    u16ADCCounter++;
                }

                if (u16ADCCounter >= u16ADCMeasurements) {
                    // Disable ADC

                    eADC = ADC_AFE_MeasurementComplete;
                    DL_ADC12_stopConversion(ADC0_INST);
                    DL_ADC12_disableConversions(ADC0_INST);
                    DL_ADC12_disableInterrupt(ADC0_INST,(DL_ADC12_INTERRUPT_MEM0_RESULT_LOADED));
                    DL_SYSCTL_disableSleepOnExit();
                }
            } else {
                //Error Case:
            }

            break;
        default:
            __NOP(); /* Set breakpoint here to check result */
            break;
    }
}

