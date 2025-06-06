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
#include "mspm0_dma.h"
#include "mspm0_gpio.h"
#include "mspm0_timer.h"
#include "ti_msp_dl_config.h"

#include "CRC_LUT.h"
#include "FlexWire.h"
#include "Patterns.h"
#include "TPS929xxx_APIs.h"
#include "system_info.h"
#include "tps_eeprom.h"
#include "tps_eeprom_default.h"

int main(void)
{
    uint32_t dev_idx = 0;
    uint32_t patternMode;
    uint32_t regValue[1];
    uint32_t progResult = FALSE;
    uint32_t dev_addr_temp;
    uint32_t eepromData[EEPROM_DEFAULT_REGS];

    SYSCFG_DL_init();

    NVIC_EnableIRQ(1);
    NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);

    // Calculate and store the CRC of data from 0x00 to 0xFF
    crcInitial();
    // DMA initialize
    dmaInitial();

    delay(25000);  //delay 200ms
    delay(25000);  //delay 200ms
    delay(25000);  //delay 200ms
    delay(25000);  //delay 200ms
    delay(25000);  //delay 200ms

    // Initialize global variables
    delayFlag       = 0;
    patternMode     = 0;
    buttonS1Pressed = 0;
    buttonS2Pressed = 0;

    // When CAN is not used, the read offset is always 0. When CAN is used, it depends on the number of transmitted bytes.
    if (CAN_USED == FALSE) {
        read_offset = 0;
    }
    // Unlock device
    clearLockAll(BROADCAST);

    // Clear POR, FAULT for device
    setClr(BROADCAST, CLRFAULT | CLRPOR);

    // Check if ACKEN is set
    for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
#if TPS92912X
        ackEnabled[device_address[dev_idx]] = TRUE;
#else
        FlexRead(device_address[dev_idx], FLEXWIRE0, DATA_LENGTH__1, TRUE);
        if (ledRcvBuffer[read_offset] & ACKEN_1) {
            ackEnabled[device_address[dev_idx]] = TRUE;
        } else {
            ackEnabled[device_address[dev_idx]] = FALSE;
        }
#endif
    }
    if (PROG_EEPROM == FALSE) {
        // Disable all channels for device - this is reset value of register, but ensure for restart of program
        disableAllCh(BROADCAST);

        // Set PWMFREQ = 2000Hz
        // Can set PWM frequency as broadcast which will overwrite all other settings in same register
        setPWMfreq(BROADCAST, PWMFREQ__2K0);

        // Need to set REFRANGE per device to retain existing setting - PWM frequency and REFRANGE are for TPS92912x in the same register
        // Set REFRANGE = 512
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            setRefRange(device_address[dev_idx], REFRANGE__512);
        }

        // Set DC for all Channels - actually this is the reset value in EEPROM
        regValue[0] = IOUT__MAX;
        setIOUTAllCh(BROADCAST, (unsigned int *) &regValue[0], TRUE);

        // Set PWMOUT for all Channels - PWM high is reset value in EEPROM and PWM low is reset from register
        regValue[0] = PWM__1;
        setPWMAllCh(BROADCAST, (unsigned int *) &regValue[0], TRUE);

        // Set Single-LED-Short threshold and Low Supply Threshold - Settings are for EVMs
#if TPS929120
        setSLSth(BROADCAST, CONF_ADCSHORTTH__4V49, 0);

        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            // Set Low Supply Threshold per device to prevent overwriting of setting CONF_FLTIMEOUT
            setLowSupplyTh(BROADCAST, LOWSUPTH__7V);
            // Lock CLR register - only for TPS929120/TPS929121
            setLock(device_address[dev_idx], CLRLOCK);
        }
#elif TPS929121
        setSLSth(BROADCAST, CONF_ADCSHORTTH__4V40, 0);

        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            // Set Low Supply Threshold per device to prevent overwriting of setting CONF_FLTIMEOUT
            setLowSupplyTh(BROADCAST, LOWSUPTH__10V);
            // Lock CLR register - only for TPS929120/TPS929121
            setLock(device_address[dev_idx], CLRLOCK);
        }
#else
        setSLSth(BROADCAST, SLSTH0__2V50, 0);
        setSLSth(BROADCAST, SLSTH1__2V50, 1);

        // Need to set SLSEN and LowSupplyTh per device to retain existing setting - REFRANGE is for TPS929160/240 in the same register
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            setLowSupplyTh(device_address[dev_idx], LOWSUPTH__5V);
            enableSLS(device_address[dev_idx]);
        }
#endif
        // Clear FAULT for device because low supply threshold was just set
        setClr(BROADCAST, CLRFAULT);

        // Enable diagnostics for all channels - actually this is the reset value in EEPROM
        enableDiagAllCh(BROADCAST);
        DL_TimerG_startCounter(PWM_0_INST);
        // Start endless loop over all available patterns
        while (1) {
            switch (patternMode) {
                case (0):
                    enableAllCh(BROADCAST);
                    checkVoltage();
                    delay(2500);
                    break;
                case (1):
                    // CC = 99;
                    // DL_TimerG_setCaptureCompareValue(PWM_0_INST, CC, DL_TIMER_CC_0_INDEX);
                    sequential_turn(0);
                    break;
                case (2):
                    sequential_turn(1);
                    break;
                case (3):
                    sequential_turn_inv(0);
                    break;
                case (4):
                    sequential_turn_inv(1);
                    break;
                case (5):
                    meteor_shower();
                    break;
                case (6):
                    greeting_and_goodbye();
                    break;
                default:
                    sequential_turn(0);
            }
            // After pattern is finished, check if button was pressed
            if (buttonS2Pressed) {
                buttonS2Pressed = 0;
                patternMode++;
                // Reset pattern mode to first pattern
                if (patternMode > 6) {
                    patternMode = 0;
                }
            }

            // Run diagnostics
            for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
#if TPS92912X
                // Run On-Demand Off-State Single-LED Short-Circuit (SS) Diagnostics - Since channels are disabled here
                enableSLS(device_address[dev_idx]);
#endif
                LED_Update_Chip_Status(device_address[dev_idx]);

                // Clear all fault flags
                setClr(device_address[dev_idx], CLRFAULT);
            }
        }
    } else {
        // Loop over all devices on the FlexWire bus
        for (dev_idx = 0; dev_idx < DEVICE_CNT; dev_idx++) {
            dev_addr_temp = device_address[dev_idx];
            // Read EEPROM - Not required for programming
            // For TPS929160 and TPS929240 this is actually the current data in the registers and not the data in the EEPROM
            readEEPROM(dev_addr_temp, (unsigned int *) eepromData, FALSE);
#if TPS92912X
            // Read EEPROM shadow registers - They are expected to be random here - Not required for programming
            readEEPROM(dev_addr_temp, eepromData, TRUE);
#endif

            if (USE_REF_PIN_FOR_EEPROM_PROG == TRUE) {
                // Clear POR, FAULT, Fail-safe for device
                setClr(dev_addr_temp, CLRFS | CLRFAULT | CLRPOR);

                // Now pull-up REF pin of device to be programmed
                buttonS1Pressed = 0;
                // Set LED2 to show we are ready for programming
                LED_RUN_ON;

                // Wait until S1 is pressed
                while (!buttonS1Pressed) {
                    // Wait 100ms
                    delay(12500);
                }

#if PROG_DEFAULT_EEPROM
                progResult =
                    programDefaultEEPROM((unsigned int *) &dev_addr_temp,
                        USE_REF_PIN_FOR_EEPROM_PROG);
#else
                progResult = programEEPROM((unsigned int *) &dev_addr_temp,
                    USE_REF_PIN_FOR_EEPROM_PROG);
#endif
                if (progResult == FALSE) {
                    // Error handle
                }
                // Reset progResult value here
                progResult = FALSE;

                // Now release REF pin pull-up
                // Wait until S2 is pressed after releasing REF pin pull-up
                buttonS2Pressed = 0;

                // Wait until S2 is pressed
                while (!buttonS2Pressed) {
                    LED_RUN_OFF;   // Blink LED2
                    delay(62500);  // Wait 500ms
                    LED_RUN_ON;    // Blink LED2
                    delay(62500);  // Wait 500ms
                }

                // Unlock device
                clearLockAll(dev_addr_temp);

                // Clear POR
                setClr(dev_addr_temp, CLRFAULT | CLRPOR);
            } else {
                buttonS1Pressed = 0;
                // Set LED2 to show we are ready for programming
                LED_RUN_ON;

                // Wait until S1 is pressed
                while (!buttonS1Pressed) {
                    // Wait 100ms
                    delay(12500);
                }

                // Write EEPROM shadow registers and program EEPROM for all devices specified
#if PROG_DEFAULT_EEPROM
                progResult =
                    programDefaultEEPROM((unsigned int *) &dev_addr_temp,
                        USE_REF_PIN_FOR_EEPROM_PROG);
#else
                progResult = programEEPROM((unsigned int *) &dev_addr_temp,
                    USE_REF_PIN_FOR_EEPROM_PROG);
#endif
                if (progResult == FALSE) {
                    // Error handle
                }
                // Reset progResult value here
                progResult = FALSE;
            }

            // If address has been updated, now need to use the new address
#if TPS92912X
            // Read EEPROM shadow registers - Now should be the same as programmed data - Not required
            readEEPROM(dev_addr_temp, (unsigned int *) eepromData, TRUE);
#endif
            // Read EEPROM - Now should be the same as programmed data - Not required
            // For TPS929160 and TPS929240 this is actually the current data in the registers and not the data in the EEPROM
            readEEPROM(dev_addr_temp, (unsigned int *) eepromData, FALSE);

            // Turn off LED2 to show EEPROM programming and verification is done
            LED_RUN_OFF;
        }

        // Stay here forever
        while (1) {
        };
    }
}
