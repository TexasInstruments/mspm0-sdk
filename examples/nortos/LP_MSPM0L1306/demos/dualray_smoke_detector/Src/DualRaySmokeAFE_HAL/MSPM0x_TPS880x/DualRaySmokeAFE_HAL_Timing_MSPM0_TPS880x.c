/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/
#include <Src/DualRaySmokeAFE_HAL/DualRaySmokeAFE_HAL_Config_Private.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"
#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 1)
#include <Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/tps880x_mspm0_i2c/tps880x_mspm0_i2c.h>
#endif

/**** Local Variables *********************************************************/
#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 0)
//// State of ULP Timer Calibration
//static enum tULPTimerCalibration {
//    ULPTimer_Calibration_Disabled = 0,
//    ULPTimer_Calibration_Start,
//    ULPTimer_Calibration_Cap1,
//    ULPTimer_Calibration_Done,
//    ULPTimer_Calibration_Error,
//}eULPTimerCalibration;
#endif

// State of ULP Timer Calibration
static enum tULPTimer {
    ULPTimer_Disabled = 0,
    ULPTimer_Idle,
    ULPTimer_Sleeping,
} eULPTimer;

// State of General Purpose Timer
static enum tGPTimer {
    GPTimer_Disabled = 0,
#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 0)
//   GPTimer_CaptureMode,
#endif
    GPTimer_CompareOnce,
} eGPTimer;

// State of LP Timer
static enum tLPTimer {
    LPTimer_Disabled = 0,
    LPTimer_Initialized,
    LPTimer_Active,
} eLPTimer;

#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 0)
//// Timer capture values for ULP Timer calibration
//static uint16_t ULPTimerCap1,ULPTimerCap2;
//// Actual calculated frequency of VLO
//static uint16_t ULPTimerActualSrcFreq;
#endif
// ULP Timer Interval in ms
static uint16_t ULPTimerIntervalms;
// ULP Timer ISR Callback
static tULPTimerCallback ULPTimer_ISRCallback;
// LP Periodic Timer Array of Callback functions
static tLPPerTimerCallback LPPerTimer_CallbackFunct
    [DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX] = {NULL};
// LP Periodic Timer Callback Enable flag
static bool LPPerTimer_CallbackEnable
    [DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX];
// Callback for GPIO Interrupt from TPS880x
static bool TPS880x_GPIO_Callback(uint8_t id);

/**** Functions **************************************************************/
void DualRaySmokeAFE_HAL_Timing_Init(void)
{
    uint16_t i;
#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 0)
//    // Default state of calibration state machine
//    eULPTimerCalibration = ULPTimer_Calibration_Disabled;
//    // Initialize expected RTC frequency
//    ULPTimerActualSrcFreq = DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_SOURCECLK_HZ;
#endif
    // Initialize RTC interval, disabled by default
    ULPTimerIntervalms = 0;
    // RTC ISR Callback is disabled by default
    ULPTimer_ISRCallback = NULL;
    // ULP timer is disabled by default
    eULPTimer = ULPTimer_Disabled;

    // GP Timer disabled by default
    eGPTimer = GPTimer_Disabled;

    // Initialize Low-Power Periodic Timer (not running by default)
    // Clear all callback functions
    for (i = 0; i < DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX; i++) {
        LPPerTimer_CallbackEnable[i] = false;
    }
    eLPTimer = LPTimer_Disabled;

    // TPS880x timer disabled by default
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_SLPTMR1, 0x00);
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_SLPTMR2, 0x00);

    // Unmask the Sleep interrupt
    tps880x_msp430_i2c_ClearBitsReg(TPS880X_REG_MASK, SLP_DONEM);
    // Enable GPIO Interrupt to detect wake-up
    DualRaySmokeAFE_HAL_IO_RegisterGPIOIntCallback(
        DUALRAYSMOKEAFE_HAL_IO_TPS880X_GPIO_INT_ID, TPS880x_GPIO_Callback);

    DualRaySmokeAFE_HAL_IO_EnableGPIOInt(
        DUALRAYSMOKEAFE_HAL_IO_TPS880X_GPIO_INT_ID);
}

#if (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 0)
//void DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(uint16_t interval_ms)
//{             TODO DIDN'T UPDATE ANY ULP TIMER FUNCTIONS
//
//
//    uint32_t rtc_count;
//
//    if (interval_ms == 0)
//    {
//        RTCCTL = RTCSS__DISABLED;
//    }
//    else
//    {
//        // Validate interval
//        if (interval_ms < DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MIN_INTERVAL_MS)
//        {
//            interval_ms = DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MIN_INTERVAL_MS;
//        }
//        else if (interval_ms > DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MAX_INTERVAL_MS)
//        {
//            interval_ms = DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MIN_INTERVAL_MS;
//        }
//
//        ULPTimerIntervalms = interval_ms;
//
//        // Calculate the number of RTC counts using the calibrated value of RTC
//        rtc_count = ((uint32_t) ULPTimerIntervalms * (uint32_t) ULPTimerActualSrcFreq) /
//                     (uint32_t) 1000;
//        // setup RTC Modulo register
//        RTCMOD = (uint16_t)(rtc_count) - 1;
//#if (HAL_TIMING_RTC_FREQ_HZ == (HAL_TIMING_RTC_SOURCECLK_HZ/10))
//        // source = VLO, divided by 10
//        RTCCTL = RTCSS__VLOCLK | RTCSR | RTCPS__10 | RTCIE;
//#else
//#error "RTC setting not currently supported. Modify DualRaySmokeAFE_HAL_Timing.c"
//#endif
//    }
//}
//
//void DualRaySmokeAFE_HAL_Timing_ULPTimer_registerCallback(
//                                       tULPTimerCallback Callback)
//{
//    ULPTimer_ISRCallback = Callback;
//}
//
//void DualRaySmokeAFE_HAL_Timing_ULPTimer_calibrate(void)
//{
//
//    uint16_t rtc_counter;
//
//    RTCCTL = RTCSS__DISABLED;
//
//    eGPTimer = GPTimer_CaptureMode;
//    DUALRAYSMOKEAFE_HAL_TIMING_GPTIMER_CAPTUREINIT();   // Initialize GP timer in capture mode
//    eULPTimerCalibration = ULPTimer_Calibration_Start;    // Start calibration
//
//    // Output RTC pulse every RTC_CAL VLO*Divider clock cycles
//    RTCMOD = DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_CAL_CYCLES -1;
//#if (HAL_TIMING_RTC_FREQ_HZ == HAL_TIMING_RTC_SOURCECLK_HZ/10)
//    // RTC clock divider=10. software reset => VLOCLK_freq/10 for RTC counter
//    RTCCTL = RTCSS__VLOCLK | RTCSR | RTCPS__10;
//#else
//#error "RTC setting not currently supported. Modify DualRaySmokeAFE_HAL_Timing.c"
//#endif
//
//    DUALRAYSMOKEAFE_HAL_TIMING_GPTIMER_CAPTURESTART(); // Start Timer to take captures
//
////    __disable_interrupt();
//    while((eULPTimerCalibration != ULPTimer_Calibration_Done) && (eULPTimerCalibration != ULPTimer_Calibration_Error))
//    {
//        //TODO update
////        __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3 with general interrupt enabled, calibrate VLO with REFO in LPM3 mode
////        __no_operation();                       // Wait for LPM3 wake-up by RTC overflow
////        __disable_interrupt();
//    }
////    __enable_interrupt();
//
//    DUALRAYSMOKEAFE_HAL_TIMING_GPTIMER_STOP();
//    eGPTimer = GPTimer_Disabled;
//    RTCCTL = RTCSS__DISABLED;               // Stop RTC
//
//    if (eULPTimerCalibration == ULPTimer_Calibration_Done)
//    {
//        // Get VLO calibration counter value,
//        // rtc_counter value stands for REFO clock cycles for period of RTC_CAL VLO*Divider clock cycles
//        // REFO clock is 32768Hz. Then VLO*Divider clock cycle = count / (RTC_CAL*32768) (s)
//        rtc_counter = ULPTimerCap2 - ULPTimerCap1;
//        // how many counts VLO/10 cycles for INTERVAL_TIME(s)
//        ULPTimerActualSrcFreq = ((uint32_t) DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_CAL_CYCLES *
//                         (uint32_t) LSBUS_FREQ_HZ) /
//                         (uint32_t) rtc_counter;
//    }
//    else
//    {
//        // Error during calibration, set default frequency
//        ULPTimerActualSrcFreq = DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_SOURCECLK_HZ;
//    }
//
//    // Update RTC configuration
//    DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(ULPTimerIntervalms);
//
//}
#elif (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 1)
void DualRaySmokeAFE_HAL_Timing_ULPTimer_setIntervalms(uint16_t interval_ms)
{
    ULPTimerIntervalms = interval_ms;
    if (ULPTimerIntervalms == 0) {
        // Disable the Sleep timer
        tps880x_msp430_i2c_WriteReg(TPS880X_REG_SLPTMR1, 0x00);
        tps880x_msp430_i2c_WriteReg(TPS880X_REG_SLPTMR2, 0x00);
        eULPTimer = ULPTimer_Disabled;
    } else {
        // Validate interval
        if (ULPTimerIntervalms <
            DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MIN_INTERVAL_MS) {
            ULPTimerIntervalms =
                DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MIN_INTERVAL_MS;
        } else if (interval_ms >
                   DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MAX_INTERVAL_MS) {
            ULPTimerIntervalms =
                DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_MAX_INTERVAL_MS;
        }

        // MSB
        tps880x_msp430_i2c_WriteReg(
            TPS880X_REG_SLPTMR1, ULPTimerIntervalms >> 8);
        // LSB
        tps880x_msp430_i2c_WriteReg(
            TPS880X_REG_SLPTMR2, ULPTimerIntervalms & 0xFF);
        eULPTimer = ULPTimer_Idle;
    }
}

void DualRaySmokeAFE_HAL_Timing_ULPTimer_registerCallback(
    tULPTimerCallback Callback)
{
    ULPTimer_ISRCallback = Callback;
}

void DualRaySmokeAFE_HAL_Timing_ULPTimer_calibrate(void)
{
    // No calibration of TPS88xx timer
}

void DualRaySmokeAFE_HAL_Timing_ULPTimer_Sleep(void)
{
    if (eULPTimer == ULPTimer_Idle) {
        tps880x_msp430_i2c_SetBitsReg(TPS880X_REG_ENABLE2, SLP_EN);
        eULPTimer = ULPTimer_Sleeping;
    }
}

#endif

void DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingLPDelayms(uint16_t delay_ms)
{
    //TODO update to timer for lower power.  DIDN'T IMPLEMENT ANY LPM TIMER FUNCTION

    uint32_t delay_cycles = ((uint32_t) delay_ms * 32000);  // MCLK is 32MHz

    delay_cycles(delay_cycles);
}

void DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingHPDelayus(uint16_t delay_us)
{
    //TODO update to timer for lower power.  DIDN'T IMPLEMENT ANY LPM TIMER FUNCTION

    uint32_t delay_cycles = ((uint32_t) delay_us * 64);  // MCLK is 32MHz

    delay_cycles(delay_cycles);
}

void DualRaySmokeAFE_HAL_Timing_LPPerTimer_setIntervalms(uint16_t interval_ms)
{
    uint32_t temp;

    // calculate period using ACLK
    temp = ((uint32_t) interval_ms) *
           ((uint32_t) 50);  // Timer clock is running at 50kHz

    // Initialize Low Power Periodic Timer
    DL_Timer_setLoadValue(LP_PERIODIC_INST, temp);

    eLPTimer = LPTimer_Initialized;
}

bool DualRaySmokeAFE_HAL_Timing_LPPerTimer_isActive(void)
{
    if (eLPTimer == LPTimer_Active) {
        return true;
    } else {
        return false;
    }
}

int16_t DualRaySmokeAFE_HAL_Timing_LPPerTimer_registerCallback(
    tLPPerTimerCallback Callback)
{
    int16_t i;
    int16_t id;

    for (i = 0; i < DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX; i++) {
        if (LPPerTimer_CallbackFunct[i] == NULL) {
            // Empty slot found, return OK
            id                          = i;
            LPPerTimer_CallbackFunct[i] = Callback;

            return id;
        }
    }

    // No empty slot found, return error
    return -1;
}

int16_t DualRaySmokeAFE_HAL_Timing_LPPerTimer_enableCallback(int16_t id)
{
    int ret = -1;  // Return error by default

    if ((id < DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX) &&
        (LPPerTimer_CallbackFunct[id] != NULL)) {
        // Check if the timer should start for the first time
        if (eLPTimer == LPTimer_Disabled) {
            // Error, the ULP timer hasn't been initialized
            return -1;
        }
        if (eLPTimer == LPTimer_Initialized) {
            //Start interval timer
            DL_Timer_startCounter(LP_PERIODIC_INST);
            // enable interrupt for LP Periodic Timer
            NVIC_EnableIRQ(LP_PERIODIC_INST_INT_IRQN);

            eLPTimer = LPTimer_Active;
        }
        // enable the callback function
        LPPerTimer_CallbackEnable[id] = true;
        ret                           = id;  // return OK
    }

    return ret;
}

int16_t DualRaySmokeAFE_HAL_Timing_LPPerTimer_disableCallback(int16_t id)
{
    int ret = -1;  // Return error by default

    if ((id < DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX) &&
        (LPPerTimer_CallbackFunct[id] != NULL)) {
        LPPerTimer_CallbackEnable[id] = false;
        ret                           = id;  // return OK
    }

    return ret;
}

//#elif (DUALRAYSMOKEAFE_HAL_TIMING_ULPTIMER_USETPS880x == 1)
//! \brief Callback for GPIO interrupt from TPS880x indicating wake-up
//!
//! \return none
static bool TPS880x_GPIO_Callback(uint8_t id)
{
    uint8_t reg_val;

    tps880x_msp430_i2c_ReadReg(TPS880X_REG_STATUS1, &reg_val);
    if (reg_val & SLP_DONE) {
        eULPTimer = ULPTimer_Idle;
        if (ULPTimer_ISRCallback != NULL) {
            if (ULPTimer_ISRCallback() == true) {
                return true;
            }
        }
    }
    return false;
}

void LP_PERIODIC_INST_IRQHandler(void)
{
    bool enabled = false;
    bool wake_up = false;
    uint16_t i;

    __NOP();

    switch (DL_TimerG_getPendingInterrupt(LP_PERIODIC_INST)) {
        case DL_TIMER_IIDX_ZERO:

            if (eLPTimer == LPTimer_Active) {
                for (i = 0;
                     i < DUALRAYSMOKEAFE_HAL_TIMING_LPPERTIMER_CALLBACK_MAX;
                     i++) {
                    if ((LPPerTimer_CallbackFunct[i] != NULL) &&
                        (LPPerTimer_CallbackEnable[i] == true)) {
                        enabled |=
                            true;  // set flag to indicate that a function is enabled
                        // Call the callback function.
                        // The function will return true if it should wake-up MCU.
                        wake_up |= LPPerTimer_CallbackFunct[i]();
                    }
                }
                // If no function is enabled, disable the timer
                if (enabled == 0) {
                    eLPTimer = LPTimer_Initialized;

                    DL_Timer_stopCounter(LP_PERIODIC_INST);
                }
                // Wake-up MCU if any of the functions requested it
                if (wake_up == true) {
                    DL_SYSCTL_disableSleepOnExit();
                }
            }

            break;
        default:
            break;
    }
}
