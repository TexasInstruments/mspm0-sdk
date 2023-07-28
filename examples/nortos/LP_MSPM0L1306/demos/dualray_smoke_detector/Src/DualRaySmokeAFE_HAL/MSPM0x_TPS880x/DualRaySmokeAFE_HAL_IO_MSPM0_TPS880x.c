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
//   HAL IO functions using SAC L3 demo HW
#include <stdint.h>
#include <stdlib.h>
#include "ti_msp_dl_config.h"
//#include <driverlib.h>
#include <Src/DualRaySmokeAFE_HAL/DualRaySmokeAFE_HAL_Config_Private.h>
#include <Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/tps880x_mspm0_i2c/tps880x_mspm0_i2c.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include "ti_msp_dl_config.h"

#define HAL_IO_VALIDATEGPIOINTID  // Enable to validate GPIO Interrupt ID \
                                  // Disable to save code space

static tGPIOIntCallback
    GPIOIntCallback[DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM];
static tTestButtonCallback testButtonCallback;
static uint16_t testButtonCounter;
static int16_t TestButtonLPTimerCallbackID;

static bool TestButtonLPTimerCallback(void);
static bool TestButtonGPIOIntCallback(uint8_t id);

/**** Functions **************************************************************/
void DualRaySmokeAFE_HAL_IO_Init(void)
{
    uint8_t i;

    DL_GPIO_clearPins(LDO_PORT, LDO_EN_PIN);

    //    DL_GPIO_clearPins(TPS_BOARD_LED_YLW_PORT, TPS_BOARD_LED_YLW_PIN);
    //    DL_GPIO_clearPins(TPS_BOARD_LED_RED_PORT, TPS_BOARD_LED_RED_PIN);
    //    DL_GPIO_clearPins(TPS_BOARD_LED_GRN_PORT, TPS_BOARD_LED_GRN_PIN);

    DL_GPIO_clearPins(BSCATTER_PORT, BSCATTER_ENABLE_PIN);
    DL_GPIO_clearPins(TMP_SENSOR_PORT, TMP_SENSOR_PWR_PIN);

    //    DL_GPIO_clearPins(VBAT_PORT, VBAT_TEST_EN_PIN);

    //Setting Pull up resistor on S3 Switch, for Buzzer testing
    DL_GPIO_initDigitalInputFeatures(LP_BOARD_BUZZER_TEST_SWITCH_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_clearPins(CO_PORT, CO_TEST_ENABLE_PIN);
    //    DL_GPIO_clearPins(LP_BOARD_PORT, LP_BOARD_BLUE_LED_PIN | LP_BOARD_RED_LED_PIN | LP_BOARD_GREEN_LED_PIN);

    // Green LED pin as Indicator, Off by default
    DualRaySmokeAFE_HAL_IO_ClearLEDIndicator();  // LED off

    // Alarm output (Red LED) , default output low
    DualRaySmokeAFE_HAL_IO_SetAlarmOutputLow();

    testButtonCallback = NULL;

    // Wait for pull-up before clearing flag for test button
    delay_cycles(100000);
    DualRaySmokeAFE_HAL_IO_EnableGPIOInt(
        DUALRAYSMOKEAFE_HAL_IO_TESTBUTTON_INT_ID);

    // Register Low-Power periodic timer Callback function to debounce
    //  and long press detection
    TestButtonLPTimerCallbackID =
        DualRaySmokeAFE_HAL_Timing_LPPerTimer_registerCallback(
            TestButtonLPTimerCallback);

    testButtonCounter = 0;

    // Check state of LP Enable
    //  If high enable pull-up, otherwise enable pull-down
    //  This will reduce the default power consumption of the pin

    //JD: Ignoring this pin for now.  Still unsure of function
    //    if (MAP_GPIO_getInputPinValue(DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PORT,
    //                                  DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PIN) == 0)
    //    {
    //        MAP_GPIO_setAsInputPinWithPullDownResistor(DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PORT,
    //                                                   DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PIN);
    //    }
    //    else
    //    {
    //        MAP_GPIO_setAsInputPinWithPullUpResistor(DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PORT,
    //                                                 DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PIN);
    //    }

    for (i = 0; i < DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM; i++) {
        GPIOIntCallback[i] = NULL;
    }

    // Register callback for Test button GPIO Interrupt
    DualRaySmokeAFE_HAL_IO_RegisterGPIOIntCallback(
        DUALRAYSMOKEAFE_HAL_IO_TESTBUTTON_INT_ID, TestButtonGPIOIntCallback);
}

int8_t DualRaySmokeAFE_HAL_IO_RegisterGPIOIntCallback(
    uint8_t id, tGPIOIntCallback callback)
{
#ifdef HAL_IO_VALIDATEGPIOINTID
    if (id >= DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM) {
        return -1;
    }
#endif

    GPIOIntCallback[id] = callback;
    return id;
}

int8_t DualRaySmokeAFE_HAL_IO_DisableGPIOInt(uint8_t id)
{
#ifdef HAL_IO_VALIDATEGPIOINTID
    if (id >= DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM) {
        return -1;
    }
#endif

    __NOP();

    if (id == 0) {
        DL_GPIO_clearInterruptStatus(
            LP_BOARD_PORT, LP_BOARD_BUZZER_TEST_SWITCH_PIN);
        DL_GPIO_disableInterrupt(
            LP_BOARD_PORT, LP_BOARD_BUZZER_TEST_SWITCH_PIN);
    } else if (id == 1) {
        DL_GPIO_clearInterruptStatus(TPS_PORT, TPS_INT_MCU_WAKE_PIN);
        DL_GPIO_disableInterrupt(TPS_PORT, TPS_INT_MCU_WAKE_PIN);
    }

    return 0;
}

int8_t DualRaySmokeAFE_HAL_IO_EnableGPIOInt(uint8_t id)
{
#ifdef HAL_IO_VALIDATEGPIOINTID
    if (id >= DUALRAYSMOKEAFE_HAL_IO_INT_CALLBACKS_NUM) {
        return -1;
    }
#endif
    if (id == DUALRAYSMOKEAFE_HAL_IO_TESTBUTTON_INT_ID) {
        DL_GPIO_clearInterruptStatus(
            LP_BOARD_PORT, LP_BOARD_BUZZER_TEST_SWITCH_PIN);
        DL_GPIO_enableInterrupt(
            LP_BOARD_PORT, LP_BOARD_BUZZER_TEST_SWITCH_PIN);
        NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);

    } else if (id == DUALRAYSMOKEAFE_HAL_IO_TPS880X_GPIO_INT_ID) {
        DL_GPIO_clearInterruptStatus(TPS_PORT, TPS_INT_MCU_WAKE_PIN);
        DL_GPIO_enableInterrupt(TPS_PORT, TPS_INT_MCU_WAKE_PIN);
        NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
    }

    return 0;
}

int8_t DualRaySmokeAFE_HAL_IO_RegisterTestButtonCallback(
    tTestButtonCallback callback)
{
    testButtonCallback = callback;
    return 0;  // No error
}

void DualRaySmokeAFE_HAL_IO_ClearLEDIndicator(void)
{
    //No LEDs on M0L Version

    //    DL_GPIO_clearPins(TPS_BOARD_LED_GRN_PORT, TPS_BOARD_LED_GRN_PIN);
    //    DL_GPIO_clearPins(LP_BOARD_PORT, LP_BOARD_GREEN_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_SetLEDIndicator(void)
{
    //No LEDs on M0L Version
    //
    //    DL_GPIO_setPins(TPS_BOARD_LED_GRN_PORT, TPS_BOARD_LED_GRN_PIN);
    //    DL_GPIO_setPins(LP_BOARD_PORT, LP_BOARD_GREEN_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_ToggleLEDIndicator(void)
{
    //No LEDs on M0L Version
    //
    //    DL_GPIO_togglePins(TPS_BOARD_LED_GRN_PORT, TPS_BOARD_LED_GRN_PIN);
    //    DL_GPIO_togglePins(LP_BOARD_PORT, LP_BOARD_GREEN_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_ClearLEDWarning(void)
{
    //No LEDs on M0L Version
    //
    //    DL_GPIO_clearPins(TPS_BOARD_LED_YLW_PORT, TPS_BOARD_LED_YLW_PIN);
    //    DL_GPIO_clearPins(LP_BOARD_PORT, LP_BOARD_RED_LED_PIN | LP_BOARD_GREEN_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_SetLEDWarning(void)
{
    //No LEDs on M0L Version
    //
    //    DL_GPIO_setPins(TPS_BOARD_LED_YLW_PORT, TPS_BOARD_LED_YLW_PIN);
    //    DL_GPIO_setPins(LP_BOARD_PORT, LP_BOARD_RED_LED_PIN | LP_BOARD_GREEN_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_ToggleLEDWarning(void)
{
    //No LEDs on M0L Version
    //
    //    DL_GPIO_togglePins(TPS_BOARD_LED_YLW_PORT, TPS_BOARD_LED_YLW_PIN);
    //    DL_GPIO_togglePins(LP_BOARD_PORT, LP_BOARD_RED_LED_PIN | LP_BOARD_GREEN_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_SetAlarmOutputHigh(void)
{
    //No LEDs on M0L Version

    //    DL_GPIO_setPins(TPS_BOARD_LED_RED_PORT, TPS_BOARD_LED_RED_PIN);
    //    DL_GPIO_setPins(LP_BOARD_PORT, LP_BOARD_RED_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_SetAlarmOutputLow(void)
{
    //No LEDs on M0L Version
    //
    //    DL_GPIO_clearPins(TPS_BOARD_LED_RED_PORT, TPS_BOARD_LED_RED_PIN);
    //    DL_GPIO_clearPins(LP_BOARD_PORT, LP_BOARD_RED_LED_PIN);
}

void DualRaySmokeAFE_HAL_IO_ToggleLEDIndicatorTimer(
    uint16_t num_toggles, uint16_t delay_ms)
{
    uint16_t i;

    for (i = 0; i < num_toggles; i++) {
        DualRaySmokeAFE_HAL_IO_ToggleLEDIndicator();
        DualRaySmokeAFE_HAL_Timing_GPTimer_BlockingLPDelayms(delay_ms);
    }
}

bool DualRaySmokeAFE_HAL_IO_GetLowPowerEnableStatus(void)
{
    //TODO leave for now as not connected on M0 LP
    // For now, just going to always return false.

    //    if (MAP_GPIO_getInputPinValue(DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PORT,
    //                                  DUALRAYSMOKEAFE_HAL_IO_LOWPOWEREN_PIN) == 0 )
    //    {
    return false;
    //    }
    //    else
    //    {
    //        return true;
    //    }
}

static bool TestButtonGPIOIntCallback(uint8_t id)
{
    // Enable TB1 to debounce and detect long press
    testButtonCounter = 0;
    // Enable the callback function to debounce and detect long press
    if (DualRaySmokeAFE_HAL_Timing_LPPerTimer_enableCallback(
            TestButtonLPTimerCallbackID) >= 0) {
        // Disable button Interrupt while timer is enabled
        DualRaySmokeAFE_HAL_IO_DisableGPIOInt(id);
    }
    return false;  // Don't wake-up
}

static bool TestButtonLPTimerCallback(void)
{
    bool ret                 = false;  // don't wake-up MCU by default
    static bool long_press   = false;
    static uint16_t debounce = 0;

    __NOP();

    if (debounce > 0) {
        debounce--;
        if (debounce == 0) {
            // Stop timer callback function
            DualRaySmokeAFE_HAL_Timing_LPPerTimer_disableCallback(
                TestButtonLPTimerCallbackID);
            // Re-enable button interrupt
            DualRaySmokeAFE_HAL_IO_EnableGPIOInt(
                DUALRAYSMOKEAFE_HAL_IO_TESTBUTTON_INT_ID);
        }
    } else {
        if ((DL_GPIO_readPins(
                LP_BOARD_PORT, LP_BOARD_BUZZER_TEST_SWITCH_PIN)) != 0) {
            if (long_press == false) {
                // If there was no long-press, indicate the short press
                if (testButtonCallback != NULL) {
                    // Callback to indicate a short press
                    ret = testButtonCallback(false);
                }
            } else {
                // If there was a long press, just clear flag.
                // The callback for long press was called while button was pressed
                //  after DUALRAYSMOKEAFE_HAL_IO_TESTBUTTONTIMER_LONGPRESSCYCLES.
                long_press = false;
            }
            // Set debounce before re-enabling input interrupt
            debounce = DUALRAYSMOKEAFE_HAL_IO_TESTBUTTONTIMER_DEBOUNCECYCLES;
        } else {
            if ((long_press == false) &&
                (++testButtonCounter >=
                    DUALRAYSMOKEAFE_HAL_IO_TESTBUTTONTIMER_LONGPRESSCYCLES)) {
                // if button is still pressed after timeout, inform long press
                if (testButtonCallback != NULL) {
                    // Callback to indicate a long press
                    ret = testButtonCallback(true);
                }
                long_press = true;
            }
        }  // if (long_press == false)
    }      // if (debounce > 0)
    return ret;
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_MULTIPLE_GPIOA_INT_IIDX:

            __NOP();

            switch (DL_GPIO_getPendingInterrupt(LP_BOARD_PORT)) {
                case LP_BOARD_BUZZER_TEST_SWITCH_IIDX:

                    if (GPIOIntCallback[0] != NULL) {
                        if (GPIOIntCallback[0](0) != false) {
                            DL_SYSCTL_disableSleepOnExit();
                            __NOP();
                        }
                    }

                    break;

                case TPS_INT_MCU_WAKE_IIDX:

                    __NOP();
                    if (GPIOIntCallback[1] != NULL) {
                        if (GPIOIntCallback[1](1) != false) {
                            DL_SYSCTL_disableSleepOnExit();
                            __NOP();
                        }
                    }

                    break;
                default:
                    break;
            }
    }
    __NOP();
}
