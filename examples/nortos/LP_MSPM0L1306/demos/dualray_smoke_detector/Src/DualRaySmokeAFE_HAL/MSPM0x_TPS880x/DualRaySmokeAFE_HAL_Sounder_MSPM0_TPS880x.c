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
//   HAL functions to control Sounder/Buzzer Hardware
//
// Texas Instruments, Inc.
// *****************************************************************************
#include <Src/DualRaySmokeAFE_HAL/DualRaySmokeAFE_HAL_Config_Private.h>
#include <Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/tps880x_mspm0_i2c/tps880x_mspm0_i2c.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>
#include <stdint.h>
#include <stdlib.h>
#include "ti_msp_dl_config.h"

uint8_t vBoostVal;

/**** Functions **************************************************************/
int16_t DualRaySmokeAFE_HAL_Sounder_Init(
    tDualRaySmokeAFE_HAL_Sounder_Config *Config)
{
    //M0 GPIO and Timer configured in config.c

    // Initialize HBEN as output low (disabled) by default
    DL_GPIO_clearPins(TPS_PORT, TPS_HBEN_PIN);

    switch (Config->u16Volume) {
        case 2700:
            vBoostVal = VBST__2_7V;
            break;
        case 3800:
            vBoostVal = VBST__3_8V;
            break;
        case 4700:
            vBoostVal = VBST__4_7V;
            break;
        case 6000:
            vBoostVal = VBST__6V;
            break;
        case 9000:
            vBoostVal = VBST__9V;
            break;
        case 10000:
            vBoostVal = VBST__10V;
            break;
        case 10500:
            vBoostVal = VBST__10_5V;
            break;
        case 11000:
            vBoostVal = VBST__11V;
            break;
        case 11500:
            vBoostVal = VBST__11_5V;
            break;
        case 15000:
            vBoostVal = VBST__15V;
            break;
        default:
            return -1;  // Return error
    }

    return 0;  // OK
}

void DualRaySmokeAFE_HAL_Sounder_SetOn(void)
{
    // Enable the Horn circuitry, turn off sleep
    tps880x_msp430_i2c_UpdateBitsReg(
        TPS880X_REG_ENABLE2, HORN_EN | SLP_EN, HORN_EN);
    // Set vBoost
    tps880x_msp430_i2c_UpdateBitsReg(
        TPS880X_REG_VBOOST, VBST_0 | VBST_1 | VBST_2, vBoostVal);
    // Turn on Boost
    tps880x_msp430_i2c_SetBitsReg(TPS880X_REG_ENABLE1, BST_EN);

    //TODO Re-enable sounder
    //    DL_Timer_startCounter(SOUNDER_PWM_INST);
    __NOP();

    // Enable HBEN
    DL_GPIO_setPins(TPS_PORT, TPS_HBEN_PIN);
}

void DualRaySmokeAFE_HAL_Sounder_SetOff(void)
{
    // Disable HBEN
    DL_GPIO_clearPins(TPS_PORT, TPS_HBEN_PIN);

    // Stop Timer
    DL_Timer_stopCounter(SOUNDER_PWM_INST);

    // Disable the Horn circuitry
    tps880x_msp430_i2c_ClearBitsReg(TPS880X_REG_ENABLE2, HORN_EN);
}
