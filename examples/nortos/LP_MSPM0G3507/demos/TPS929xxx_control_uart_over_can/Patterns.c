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
#include "mspm0_timer.h"
#include "ti_msp_dl_config.h"

#include "FlexWire.h"
#include "Patterns.h"
#include "TPS929xxx_APIs.h"
#include "system_info.h"

int ADC_value_MAXOUT;
int ADC_value_SUPPLY;
int CC;

void checkVoltage(void)
{
    unsigned int regValue[1];
    CC        = 100;
    bool flag = 0;

    //enableAllCh(BROADCAST);

    regValue[0] = IOUT__2;
    setIOUTAllCh(BROADCAST, &regValue[0], TRUE);
    // timeout flag  125=1ms
    watchDog(2500000);  //20s   &&(timeOutFlag == 0)

    while ((flag == 0) && (timeOutFlag == 0)) {
        ADC_value_MAXOUT = getADCvalue(DEVICE_ADDR__1, ADCCHSEL__MAXOUT, 1);
        ADC_value_SUPPLY = getADCvalue(DEVICE_ADDR__1, ADCCHSEL__SUPPLY, 1);

        if ((ADC_value_SUPPLY - ADC_value_MAXOUT) > 1000) {
            if ((CC <= 100) && (CC >= 3)) {
                CC -= 3;
            }
            DL_TimerG_setCaptureCompareValue(
                PWM_0_INST, CC, DL_TIMER_CC_0_INDEX);
            delay(1000);
        } else if ((ADC_value_SUPPLY - ADC_value_MAXOUT) < 800) {
            if ((CC <= 97) && (CC >= 0)) {
                CC += 3;
            }
            DL_TimerG_setCaptureCompareValue(
                PWM_0_INST, CC, DL_TIMER_CC_0_INDEX);
            delay(1000);
        } else if ((CC == 100) || (CC == 1)) {
            flag = 1;
            break;
        } else {
            flag = 1;
        }
    }
}

void greeting_and_goodbye(void)
{
    unsigned int j, m;
    const int brightness_steps    = 5;
    unsigned int pwmStep[]        = {0x00, 0x14, 0x3C, 0xA0, 0xFF};
    const unsigned int maxChannel = MAX_CHANNEL_CNT * DEVICE_CNT;
    unsigned int pwmValue[MAX_CHANNEL_CNT * DEVICE_CNT];
    int ch_idx                       = 0;
    unsigned int dev_idx             = 0;
    const unsigned int middleChannel = (MAX_CHANNEL_CNT * DEVICE_CNT) >> 1;

    pwmValue[0] = PWM__0;
    setPWMAllCh(BROADCAST, &pwmValue[0], TRUE);

    for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
        // Enable LUT - Execute per device to retain other setting in the same register
        enableExpPWM(device_address[dev_idx]);

        // Set PWM frequency as 1000Hz - Execute per device to retain other setting in the same register
        setPWMfreq(device_address[dev_idx], PWMFREQ__1000);
    }

    //***Enable all channels for all devices
    enableAllCh(BROADCAST);

    // Initialize array
    for (ch_idx = 0; ch_idx < maxChannel; ch_idx++) {
        pwmValue[ch_idx] = pwmStep[0];
    }

    //***greeting***********************//
    for (j = 0; j < (brightness_steps - 1); j++) {
        // Set middle 2 channels to new value
        pwmValue[middleChannel - 1] = pwmStep[j + 1];
        pwmValue[middleChannel]     = pwmStep[j + 1];

        // Write PWM for this device
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            setPWMhighAllCh(device_address[dev_idx],
                &pwmValue[dev_idx * MAX_CHANNEL_CNT], FALSE);
        }

        delay(12500);  //delay 100ms

        // Start moving data
        for (m = 0; m < middleChannel; m++) {
            // Half move down
            for (ch_idx = 0; ch_idx < (middleChannel - 1); ch_idx++) {
                pwmValue[ch_idx] = pwmValue[ch_idx + 1];
            }

            // Set middle channel back
            pwmValue[middleChannel - 1] = pwmStep[j];

            // Half move up
            for (ch_idx = (maxChannel - 1); ch_idx > middleChannel; ch_idx--) {
                pwmValue[ch_idx] = pwmValue[ch_idx - 1];
            }

            // Set middle channel back
            pwmValue[middleChannel] = pwmStep[j];

            // Write PWM for this device
            for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
                setPWMhighAllCh(device_address[dev_idx],
                    &pwmValue[dev_idx * MAX_CHANNEL_CNT], FALSE);
            }
            delay(12500);  //delay 100ms
        }

        // Start making all channels bright
        for (m = 0; m < middleChannel; m++) {
            pwmValue[middleChannel - 1 - m] = pwmStep[j + 1];
            pwmValue[middleChannel + m]     = pwmStep[j + 1];

            // Write PWM for this device
            for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
                setPWMhighAllCh(device_address[dev_idx],
                    &pwmValue[dev_idx * MAX_CHANNEL_CNT], FALSE);
            }
            delay(12500);  //delay 100ms
        }
    }

    //***goodbye***********************//
    for (j = (brightness_steps - 1); j > 0; j--) {
        // Set 2 outer channels to new value
        pwmValue[0]              = pwmStep[j - 1];
        pwmValue[maxChannel - 1] = pwmStep[j - 1];

        // Write PWM for this device
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            setPWMhighAllCh(device_address[dev_idx],
                &pwmValue[dev_idx * MAX_CHANNEL_CNT], FALSE);
        }

        delay(12500);  //delay 100ms

        // Start moving data
        for (m = 0; m < middleChannel; m++) {
            // Lower half move up
            for (ch_idx = (middleChannel - 1); ch_idx > 0; ch_idx--) {
                pwmValue[ch_idx] = pwmValue[ch_idx - 1];
            }

            // Set lowest channel back
            pwmValue[0] = pwmStep[j];

            // Upper move down
            for (ch_idx = middleChannel; ch_idx < (maxChannel - 1); ch_idx++) {
                pwmValue[ch_idx] = pwmValue[ch_idx + 1];
            }

            // Set highest channel back
            pwmValue[maxChannel - 1] = pwmStep[j];

            // Write PWM for this device
            for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
                setPWMhighAllCh(device_address[dev_idx],
                    &pwmValue[dev_idx * MAX_CHANNEL_CNT], FALSE);
            }
            delay(12500);  //delay 100ms
        }

        // Start making all channels less bright
        for (m = 0; m < middleChannel; m++) {
            pwmValue[0 + m]              = pwmStep[j - 1];
            pwmValue[maxChannel - 1 - m] = pwmStep[j - 1];

            // Write PWM for this device
            for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
                setPWMhighAllCh(device_address[dev_idx],
                    &pwmValue[dev_idx * MAX_CHANNEL_CNT], FALSE);
            }
            delay(12500);  //delay 100ms
        }
    }

    //***Disable all channels for all devices
    disableAllCh(BROADCAST);

    // Restore original settings
    for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
        // Disable LUT - Execute per device to retain other setting in the same register
        disableExpPWM(device_address[dev_idx]);

        // Set PWM frequency as 2000Hz - Execute per device to retain other setting in the same register
        setPWMfreq(device_address[dev_idx], PWMFREQ__2K0);
    }

    //***Set PWMOUT for all Channels
    pwmValue[0] = PWM__1;
    setPWMAllCh(BROADCAST, &pwmValue[0], TRUE);
}

void sequential_turn(unsigned int mode)
{
    unsigned int data[MAX_BURST_CNT];
    int ch_idx  = 0x00;
    int dev_idx = 0x00;

    delay(13750);  //delay 110ms

    if (mode == 0) {
        // mode=0, right to left
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
                data[0] = ch_idx;
                setCh(device_address[dev_idx], data, 1);
                delay(2500);  //delay 20ms
            }
        }
    } else {
        // mode=1, left to right
        for (dev_idx = (DEVICE_CNT - 1); dev_idx >= 0; dev_idx--) {
            for (ch_idx = (MAX_CHANNEL_CNT - 1); ch_idx >= 0; ch_idx--) {
                data[0] = ch_idx;
                setCh(device_address[dev_idx], data, 1);
                delay(2500);  //delay 20ms
            }
        }
    }

    delay(13750);  //delay 110ms

    //***Disable all channels for all devices
    disableAllCh(BROADCAST);
}

void sequential_turn_inv(unsigned int mode)
{
    unsigned int data[MAX_BURST_CNT];
    int ch_idx  = 0x00;
    int dev_idx = 0x00;

    delay(13750);  //delay 110ms

    //***Enable all channels for all devices
    enableAllCh(BROADCAST);

    delay(13750);  //delay 110ms

    if (mode == 0) {
        // mode=0, right to left
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
                data[0] = ch_idx;
                unsetCh(device_address[dev_idx], data, 1);
                delay(2500);  //delay 20ms
            }
        }
    } else {
        // mode=1, left to right
        for (dev_idx = (DEVICE_CNT - 1); dev_idx >= 0; dev_idx--) {
            for (ch_idx = (MAX_CHANNEL_CNT - 1); ch_idx >= 0; ch_idx--) {
                data[0] = ch_idx;
                unsetCh(device_address[dev_idx], data, 1);
                delay(2500);  //delay 20ms
            }
        }
    }

    //***Disable all channels for all devices
    disableAllCh(BROADCAST);
}

void meteor_shower(void)
{
    // Brightness steps, 100%, 80%, 60%, 40%, 20%, 10%, 5%
    unsigned int pwmSteps[] = {0xFF, 0xCC, 0x98, 0x65, 0x33, 0x18, 0x0C, 0x00};
    // Store the data wrote to device1-PWMOUT0 - 11 and device2-PWMOUT0 - 11
    unsigned int dataCmd[DEVICE_CNT][MAX_CHANNEL_CNT];
    unsigned int i                   = 0;
    int dev_idx                      = 0;
    int ch_idx                       = 0;
    const unsigned int pwm_steps_cnt = 8;
    unsigned int pwmValue[1];

    pwmValue[0] = PWM__0;
    setPWMAllCh(BROADCAST, &pwmValue[0], TRUE);

    // Enable LUT - Execute per device to retain other setting in the same register
    for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
        enableExpPWM(device_address[dev_idx]);
    }

    // Enable all channels
    enableAllCh(BROADCAST);

    // Initialize dataCmd with all zeros
    for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
        for (ch_idx = 0; ch_idx < MAX_CHANNEL_CNT; ch_idx++) {
            dataCmd[dev_idx][ch_idx] = 0x00;
        }
    }

    i = 0;
    do {
        // Shift data
        for (dev_idx = (DEVICE_CNT - 1); dev_idx >= 0; dev_idx--) {
            for (ch_idx = (MAX_CHANNEL_CNT - 2); ch_idx >= 0; ch_idx--) {
                dataCmd[dev_idx][ch_idx + 1] = dataCmd[dev_idx][ch_idx];
            }
            // For devices larger than zero get value from previous device
            if (dev_idx > 0) {
                dataCmd[dev_idx][0] =
                    dataCmd[dev_idx - 1][MAX_CHANNEL_CNT - 1];
            }
        }
        if (i < pwm_steps_cnt) {
            // Update data of first channel of first device
            dataCmd[0][0] = pwmSteps[i];
        }

        // Write PWM for this device
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            setPWMhighAllCh(
                device_address[dev_idx], &dataCmd[dev_idx][0], FALSE);
        }
        i++;

        delay(2500);  //delay 20ms
    } while (i < ((MAX_CHANNEL_CNT * DEVICE_CNT) + pwm_steps_cnt));

    //***Disable all channels for all devices
    disableAllCh(BROADCAST);

    // Disable LUT - Execute per device to retain other setting in the same register
    for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
        disableExpPWM(device_address[dev_idx]);
    }

    //***Set PWMOUT for all Channels
    pwmValue[0] = PWM__0;
    setPWMAllCh(BROADCAST, &pwmValue[0], TRUE);
}
