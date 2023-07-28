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
//*****************************************************************************
//        Dual Ray Smoke Sensor Application HAL
//
// Hardware Abstraction Layer for Dual Ray Smoke Detector Application

#ifndef __DUALRAYSMOKEAFE_HAL_H__
#define __DUALRAYSMOKEAFE_HAL_H__

#include <Src/include/DualRaySmokeAFELib_Config.h>
#include <Src/include/DualRaySmokeAFE_Config_Common.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "ti_msp_dl_config.h"

//! \brief Configuration of HAL for AFE
typedef struct {
    //!< LED Strength.
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16LED_Strength[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< PGA Gain for each LED
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16PGA_Gain[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Enables or Disables the Photodiode TIA and PGA
    //!     Set by application dynamically if needed, or as a default.
    bool bPDTIAEn;
    //!< Time with LED ON before ADC measurement.
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16LEDHoldOnTime[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Time with LED OFF before ADC measurement.
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16LEDHoldOffTime[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Measurement overhead in us, not including delay
    //!     Set by HAL during initialization.
    uint16_t u16LEDMeasOverheadus;
#ifdef __AFE_TPS880X__
    //!< Temperature coefficient for LEDs
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16LED_TempCoeff[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Enables or Disables the CO Measurements
    //!     Set by application dynamically if needed, or as a default.
    bool bCOMeasEn;
    //!< CO Amplifier
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16COGain;
    //!< Enables or Disables the CO Amplifier
    //!     Set by application dynamically if needed, or as a default.
    bool bCOAmpEn;
#endif

#ifdef __AFE_SACL3__
    //!< TIA Bias Voltage for each LED.
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16TIABiasmV[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< PGA Bias Voltage for each LED.
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16PGABiasmV[DUALRAYSMOKEAFE_LED_TOTAL];
#endif
} tDualRaySmokeAFE_HAL_AFE_Config;

//! \brief Contains measurement information from AFE HAL
typedef struct {
    //!< Dark measurement for each LED
    uint32_t u16Dark[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Light measurement
    uint32_t u16Light[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Difference between Light and Dark
    int32_t i16Diff[DUALRAYSMOKEAFE_LED_TOTAL];
    //!< Current for LED in mA
    uint32_t u16LEDCurrent[DUALRAYSMOKEAFE_LED_TOTAL];
#ifdef __AFE_TPS880X__
    //!< CO measurement
    int32_t u16CO;
#endif
} tDualRaySmokeAFE_HAL_AFE_SmokeMeasurement;

//! \brief Contains temperature measurement from AFE HAL
typedef struct {
    //!< Internal Temperature measurement in IQ15 format
    //!   IQ15 = 17 integer, 15 fractional
    int32_t iq15TemperatureIntC;
    //!< External Temperature measurement in IQ15 format
    //!   IQ15 = 17 integer, 15 fractional
    int32_t iq15TemperatureExtC;
} tDualRaySmokeAFE_HAL_AFE_TemperatureMeasurement;

//! \brief Configuration of HAL for Sounder
typedef struct {
    //!< Sounder frequency in Hz
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16Freq;
#ifdef __AFE_TPS880X__
    //!< Sounder Volume
    //!     Set by application dynamically if needed, or as a default.
    uint16_t u16Volume;
#endif
} tDualRaySmokeAFE_HAL_Sounder_Config;

//! \brief Configuration of HAL
typedef struct {
    tDualRaySmokeAFE_HAL_AFE_Config *AFEConfig;
    tDualRaySmokeAFE_HAL_Sounder_Config *sounderConfig;
} tDualRaySmokeAFE_HAL_Config;

//! \brief Ultra Low Power Timer ISR Callback function pointer
//!
//! \return true if device wakes-up after ISR, false if not.
typedef bool (*tULPTimerCallback)(void);

//! \brief GUI Comm RX Callback function pointer
//!
//! \param[in] char with Received byte
//!
//! \return true if device wakes-up after ISR, false if not.
typedef bool (*tGUICommRXCharCallback)(char);

//! \brief Test Button ISR Callback function pointer
//!
//! \param[in] bool true if a long press is detected, false if short press.
//!
//! \return true if device wakes-up after ISR, false if not.
typedef bool (*tTestButtonCallback)(bool);

//! \brief Callback for GPIO Interrupt
//!
//! \param[in] uint8_t with the ID of the GPIO interrupt calling the function
//!
//! \return true if device wakes-up after ISR, false if not.
typedef bool (*tGPIOIntCallback)(uint8_t);

//! \brief Low Power Periodic Timer ISR Callback function pointer.
//!
//! \return true if device wakes-up after ISR, false if not.
typedef bool (*tLPPerTimerCallback)(void);

// ********************* System ************************************************
//! \brief Initializes I/Os used by the system.
//!           Initialization is done to achieve lowest power consumption.
//!
//!
//! \return 0 if OK, -1 if error
extern int8_t DualRaySmokeAFE_HAL_System_Init(void);

// ********************* I/O ***************************************************
//! \brief Initializes I/Os used by the system.
//!           Initialization is done to achieve lowest power consumption.
//!
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_Init(void);

//! \brief Sets a callback function for the test button.
//!           This function will be called if the button is pressed.
//!
//! \param[in] callback is the pointer to a callback function for the
//!             test button. Set to NULL if disabled.
//!
//! \return none
extern int8_t DualRaySmokeAFE_HAL_IO_RegisterTestButtonCallback(
    tTestButtonCallback callback);

//! \brief Sets a callback function for a GPIO Interrupt.
//!           This function will be called if the button is pressed.
//!
//! \param[in] id is the ID of the callback function.
//!             Max number of callbacks is DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM.
//! \param[in] callback is the pointer to a callback function for the
//!             GPIO Interrupt button. Set to NULL if disabled.
//!
extern int8_t DualRaySmokeAFE_HAL_IO_RegisterGPIOIntCallback(
    uint8_t id, tGPIOIntCallback callback);

//! \brief Disables a GPIO Interrupt
//!
//! \param[in] id is the ID of the interrupt
//!
extern int8_t DualRaySmokeAFE_HAL_IO_DisableGPIOInt(uint8_t id);

//! \brief Enables a GPIO Interrupt
//!
//! \param[in] id is the ID of the interrupt
//!
extern int8_t DualRaySmokeAFE_HAL_IO_EnableGPIOInt(uint8_t id);

//! \brief Sets the Indicator LED
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_SetLEDIndicator(void);

//! \brief Clears the Indicator LED
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_ClearLEDIndicator(void);

//! \brief Toggles the Indicator LED
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_ToggleLEDIndicator(void);

//! \brief Sets the Warning LED
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_SetLEDWarning(void);

//! \brief Clears the Warning LED
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_ClearLEDWarning(void);

//! \brief Toggles the Warning LED
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_ToggleLEDWarning(void);

//! \brief Sets the Alarm Output pin High
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_SetAlarmOutputHigh(void);

//! \brief Sets the Alarm Output pin Low
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_SetAlarmOutputLow(void);

//! \brief Toggles the Indicator LED multiple times with a delay using timer
//!
//! \param[in] num_toggles is the number of LED toggles.
//! \param[in] delay_ms is the delay between toggles in ms.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(
    uint16_t num_toggles, uint16_t delay_ms);

//! \brief Return the status of the Low-Power Enable pin
//!
//! \return true if pin is high, false if it's low
extern bool DualRaySmokeAFE_HAL_IO_GetLowPowerEnableStatus(void);

// ********************* Sounder************************************************
//! \brief Initializes Buzzer used by the system.
//!
//! \param[in] config is the pointer to the configuration structure for sounder.
//!
//! \return 0 if OK, -1 if error
extern int16_t DualRaySmokeAFE_HAL_Sounder_Init(
    tDualRaySmokeAFE_HAL_Sounder_Config *Config);

//! \brief Turns on the Sounder Hardware
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Sounder_SetOn(void);

//! \brief Turns on the Sounder Hardware
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Sounder_SetOff(void);

// ********************* AFE **************************************************
//! \brief Initializes the AFE HAL
//!
//! \param[in] Config is the pointer to the library configuration structure
//!
//! \return 0 if initialization was successful, -1 if not
extern int8_t DualRaySmokeAFE_HAL_AFE_Init(
    tDualRaySmokeAFE_HAL_AFE_Config *Config);

//! \brief Takes a measurement using one of the LEDs
//!
//! \param[out] measurement is a pointer to the structure where the measurement
//!             will be stored.
//!
//! \return 0 if initialization was successful, -1 if not
extern int8_t DualRaySmokeAFE_HAL_AFE_Measurement(
    tDualRaySmokeAFE_HAL_AFE_SmokeMeasurement *measurement);

//! \brief Takes a measurement of the internal temperature sensor.
//!
//! \param[out] measurement is a pointer to the structure where the measurement
//!             will be stored.
//!
//! \return 0 if initialization was successful, -1 if not
extern int8_t DualRaySmokeAFE_HAL_AFE_TemperatureMeasurement(
    tDualRaySmokeAFE_HAL_AFE_TemperatureMeasurement *measurement);

// ********************* Timers ************************************************
//! \brief Initializes timers used by the system.
//!           Initializes RTC and general purpose timers.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_Init(void);

//! \brief Initializes the Ultra Low-Power timer used by application.
//!
//! \param[in] interval_ms is the periodic Interval in ms. Set to 0 to disable.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(
    uint16_t interval_ms);

//! \brief Sets a callback function which is called on Ultra Low-Power Periodic
//!         Timer event.
//!
//! \param[in] Callback is the pointer to a callback function for the
//!             Ultra Low power periodic timer. Set to NULL if disabled.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_ULPTimer_registerCallback(
    tULPTimerCallback Callback);

//! \brief Calibrate the Low Power periodic timer used by application.
//!         This function uses REFO clock and an internal timer to
//!         count period time of RTC clock in LPM3 mode.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_ULPTimer_calibrate(void);

//! \brief Sets ULP timer to sleep for predefined period.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_ULPTimer_Sleep(void);

//! \brief Starts a Low Power blocking delay with an interval in ms.
//!         The device will use a general purpose timer using REFO and go to
//!         LPM3 waiting for the delay.
//!
//! \param[in] delay_ms is the delay in ms.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingLPDelayms(
    uint16_t delay_ms);

//! \brief Starts a High Precision blocking delay with an interval in us.
//!         The device will use a general purpose timer using SMCLK and go to
//!         LPM0 waiting for the delay.
//!
//! \param[in] delay_us is the delay in us.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(
    uint16_t delay_us);

//! \brief Initializes the Low-Power periodic timer used by application.
//!
//! \param[in] interval_ms is the periodic Interval in ms. Set to 0 to disable.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_Timing_LPPerTimer_setIntervalms(
    uint16_t interval_ms);

//! \brief Gets the status of the Low Power Periodic Timer.
//!
//! \return true if timer is enabled, false if disabled.
extern bool DualRaySmokeAFE_HAL_Timing_LPPerTimer_isActive(void);

//! \brief Registers a callback function which is called on Low-Power Periodic
//!         Timer event.
//!
//! \param[in] LPPerTimer_Callback is the pointer to a callback function for the
//!             Low power periodic timer. Set to NULL if disabled.
//!
//! \return id if function was registered, -1 if an error occured.
extern int16_t DualRaySmokeAFE_HAL_Timing_LPPerTimer_registerCallback(
    tLPPerTimerCallback Callback);

//! \brief Enables a callback function which is called on Low-Power Periodic
//!         Timer event.
//!        The function must be registered first using
//!             DualRaySmokeAFE_HAL_Timing_LPPerTimer_setCallback.
//!
//! \param[in] id is the ID/number of the function. It must not exceed
//!             DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX.
//!
//! \return id if function was enabled correctly, -1 if an error occured.
extern int16_t DualRaySmokeAFE_HAL_Timing_LPPerTimer_enableCallback(
    int16_t id);

//! \brief Disables a callback function which is called on Low-Power Periodic
//!         Timer event.
//!        The function must be registered first using
//!             DualRaySmokeAFE_HAL_Timing_LPPerTimer_setCallback.
//!
//! \param[in] id is the ID/number of the function. It must not exceed
//!             DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX.
//!
//! \return id if function was disabled correctly, -1 if an error occured.
extern int16_t DualRaySmokeAFE_HAL_Timing_LPPerTimer_disableCallback(
    int16_t id);

// ********************* GUI Communication *************************************
//! \brief Initializes the communication interface for GUI.
//!
//! \param[in] RxByteCallback is the function called when a byte is received.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_GUI_Init(
    tGUICommRXCharCallback RxByteCallback);

//! \brief Transmits a character to the GUI in blocking mode.
//!
//! \param[in] character is the character being transmitted to GUI.
//!
//! \return none
extern void DualRaySmokeAFE_HAL_GUI_TransmitCharBlocking(char character);

#endif  //__DUALRAYSMOKEAFE_HAL_H__
