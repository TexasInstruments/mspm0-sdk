/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "ti_msp_dl_config.h"

/* clang-format off */
#define RTC_MAX_DRIFT_PPM                                                  (240)
/* clang-format on */

volatile int16_t gCalValue = 0;
volatile bool gUpdateCal   = false;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_SWITCHES1_INT_IRQN);
    NVIC_EnableIRQ(GPIO_SWITCHES2_INT_IRQN);

    /* Start RTC clock */
    DL_RTC_enableClockControl(RTC);

    while (1) {
        while (false == gUpdateCal) {
            __WFE();
        }
        /* Adjust calibration */
        gUpdateCal = false;
        /* Attempt to update compensation value */
        do {
            /* Wait until RTC is ready for compensation */
            while (DL_RTC_isReadyToCalibrate(RTC) != true)
                ;

            if (gCalValue >= 0) {
                DL_RTC_setOffsetCalibrationAdjValue(
                    RTC, DL_RTC_OFFSET_CALIBRATION_SIGN_UP, gCalValue);
            } else {
                DL_RTC_setOffsetCalibrationAdjValue(
                    RTC, DL_RTC_OFFSET_CALIBRATION_SIGN_DOWN, -gCalValue);
            }
            /* Check if write was successful */
        } while (DL_RTC_isCalibrationWriteResultOK(RTC) != true);
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case DL_INTERRUPT_GROUP1_IIDX_GPIOB:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES1_PORT)) {
                case GPIO_SWITCHES1_USER_SWITCH_1_IIDX:
                    /* Increment offset if GPIO_SWITCHES1_USER_SWITCH_1_PIN is pressed */
                    if (gCalValue < RTC_MAX_DRIFT_PPM) {
                        gCalValue++;
                        gUpdateCal = true;
                    }
                    break;
                default:
                    break;
            }
            break;
        case DL_INTERRUPT_GROUP1_IIDX_GPIOA:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES2_PORT)) {
                case GPIO_SWITCHES2_USER_SWITCH_2_IIDX:
                    /* Decrement offset if GPIO_SWITCHES2_USER_SWITCH_1_PIN is pressed */
                    if (gCalValue > -RTC_MAX_DRIFT_PPM) {
                        gCalValue--;
                        gUpdateCal = true;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
