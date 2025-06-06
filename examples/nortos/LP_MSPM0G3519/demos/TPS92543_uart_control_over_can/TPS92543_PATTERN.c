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

#include "TPS92543_PATTERN.h"

bool gLEDOnFlag = false;

void LedOn(void)
{
    uint8_t WData;

    if (gLEDOnFlag == false) {
        //Enable BOOST;
        WData = BSTEN_EN;
        WriteReg(WRITE1, SgDEVID[0], SYSCFG1, &WData, CurDev[0].AckEn);
        WriteReg(WRITE1, SgDEVID[1], SYSCFG1, &WData, CurDev[1].AckEn);
        delay_cycles(80000);

        //Enable BUCK;
        WData = BSTEN_EN | BCK1EN_EN;
        WriteReg(WRITE1, SgDEVID[0], SYSCFG1, &WData, CurDev[0].AckEn);
        WriteReg(WRITE1, SgDEVID[1], SYSCFG1, &WData, CurDev[1].AckEn);
        delay_cycles(80000 * 10);
        gLEDOnFlag = true;
    }
}

void BreathingLED(void)
{
    uint8_t i;
    uint16_t DutyCycle;
    uint16_t DutyCycle6Ch[6];

    if (gLEDOnFlag == false) {
        LedOn();
    }

    for (DutyCycle = 5; DutyCycle < 900; DutyCycle = DutyCycle + 15) {
        for (i = 0; i < 6; i++) {
            DutyCycle6Ch[i] = DutyCycle;
        }
        WidthChange(DutyCycle6Ch);
        delay_cycles(80000 * 15);  //delay 15ms
    }
    for (DutyCycle = 900; DutyCycle > 5; DutyCycle = DutyCycle - 15) {
        for (i = 0; i < 6; i++) {
            DutyCycle6Ch[i] = DutyCycle;
        }
        WidthChange(DutyCycle6Ch);
        delay_cycles(80000 * 15);  //delay 15ms
    }
}

void SquentialLED(void)
{
    uint16_t DutyCycle6Ch[6] = {0, 0, 0, 0, 0, 0};
    uint8_t i;
    if (gLEDOnFlag == false) {
        LedOn();
    }

    for (i = 0; i < 6; i++) {
        DutyCycle6Ch[i] = 900;
        WidthChange(DutyCycle6Ch);
        delay_cycles(80000 * 100);  //delay 100ms
    }

    for (i = 0; i < 6; i++) {
        DutyCycle6Ch[5 - i] = 0;
        WidthChange(DutyCycle6Ch);
        delay_cycles(80000 * 100);  //delay 100ms
    }
}

void LedOff(void)
{
    uint8_t WData;
    if (gLEDOnFlag == true) {
        WData = BSTEN_EN | BCK1EN_DIS;
        WriteReg(WRITE1, MultiDEVID, SYSCFG1, &WData, false);  //close buck
        delay_cycles(80000);

        WData = BSTEN_DIS;
        WriteReg(WRITE1, MultiDEVID, SYSCFG1, &WData, false);  //close buck

        uint8_t WidthReg[8] = {0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF};  //WIDTH3_1 ~ WIDTH6H
        WriteReg(WRITE8, MultiDEVID, WIDTH3_1L, WidthReg,
            false);  //Reinitial duty cycle
        gLEDOnFlag = false;
    }
}
