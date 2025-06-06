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

#include "TPS92543_API.h"

uint32_t CTMPin[DEVICE_CNT] = {MPIOx_MPIO0_PIN, MPIOx_MPIO1_PIN};
uint8_t SgDEVID[DEVICE_CNT];
uint8_t MultiDEVID;

uint8_t InitRegValue[90] = {
    0x00, 0x00, 0x07, 0x00, 0x48, 0x00, 0x74, 0x00, 0x00,
    0x0F,  //0x00 ~ 0x09 register
    0x57, 0x63, 0x50, 0x00, 0xA0, 0x00, 0x0C, 0x00, 0xC4,
    0xDE,  //0x0A ~ 0x13 register
    0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00,  //0x14 ~ 0x1D register
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,  //0x1E ~ 0x27 register
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,  //0x28 ~ 0x31 register
    0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x3F, 0x3F, 0xFF,
    0xFF,  //0x32 ~ 0x3B register
    0xFF, 0xFF, 0xFF, 0xFF, 0x1C, 0x08, 0x00, 0x2A, 0x55,
    0x80,  //0x3C ~ 0x45 register
    0xAA, 0xD5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00,  //0x46 ~ 0x4F register
    0x77, 0x77, 0x77, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00
    //  0x50    0x51        0x52        0x80        0x81        0x82        0xE9        0xEA        0xFC        0xFD    register
};
//0x10: 0x0C, buck current is 100mA

uint8_t getDevID(uint8_t DevAdd, uint8_t P0InvEn, uint8_t P1InvEn)
{
    uint8_t DEV[5];
    uint8_t P0, P1, VOL = 0;
    uint8_t DevID;

    DEV[0] = DevAdd & 0x01;
    DEV[1] = (DevAdd >> 1) & 0x01;
    DEV[2] = (DevAdd >> 2) & 0x01;
    DEV[3] = (DevAdd >> 3) & 0x01;
    DEV[4] = (DevAdd >> 4) & 0x01;
    VOL    = 1;
    P0     = (DEV[0] ^ DEV[1] ^ DEV[2] ^ DEV[4]);
    P1     = !(VOL ^ DEV[1] ^ DEV[3] ^ DEV[4]);
    if (P0InvEn == P0_INV_EN) {
        P0 = !P0;
    }
    if (P1InvEn == P1_INV_EN) {
        P1 = !P1;
    }
    DevID = (DevAdd & 0x1F) | VOL << 5 | P0 << 6 | P1 << 7;
    return DevID;
}

uint8_t CheckACKEN(uint8_t DevID)
{
    uint8_t ReadData;
    ReadReg(READ1, DevID, UARTCFG, &ReadData);
    return (ReadData & ACKEN_EN);
}

void DeviceInit(void)
{
    uint8_t i;
    uint8_t WData;

    for (i = 0; i < DEVICE_CNT; i++) {
        //Get device ID
        SgDEVID[i] =
            getDevID(PreDev[i].DevAdd, PreDev[i].P0InvEn, PreDev[i].P1InvEn);

        //Check ACKEN is set
        PreDev[i].AckEn = CheckACKEN(SgDEVID[i]);

        //Enter CTM
        CommsRst();
        WData = EnterCTM;
        WriteReg(WRITE1, SgDEVID[i], COMMAND, &WData, (bool) PreDev[i].AckEn);

        DL_GPIO_setPins(MPIOx_PORT, CTMPin[i]);
        delay_cycles(80000 * 10);  //10ms
        DL_GPIO_clearPins(MPIOx_PORT, CTMPin[i]);
        delay_cycles(80000 * 10);  //10ms
        DL_GPIO_setPins(MPIOx_PORT, CTMPin[i]);
        delay_cycles(80000 * 20);  //20ms
        DL_GPIO_clearPins(MPIOx_PORT, CTMPin[i]);
        delay_cycles(80000 * 20);  //20ms
        DL_GPIO_setPins(MPIOx_PORT, CTMPin[i]);
        delay_cycles(80000 * 30);  //30ms
        DL_GPIO_clearPins(MPIOx_PORT, CTMPin[i]);

        // UART Config
        // Use new ACK, BaudRate, and type of comms afterwards
        WData = COMM_TYPE | CurDev[i].AckEn | CurDev[i].BaudRate;
        WriteReg(WRITE1, SgDEVID[i], UARTCFG, &WData, (bool) PreDev[i].AckEn);

        //MLTCSTx Config
        //Use new multicastID afterwards
        WData = CurDev[i].MltcstEN | CurDev[i].MltcstId;
        WriteReg(WRITE1, SgDEVID[i], MLTCST1, &WData, (bool) CurDev[i].AckEn);

        //DEVADDR config
        //Use new deviceID, P1, P0 afterwards
        WData = CurDev[i].P0InvEn | CurDev[i].P1InvEn | CurDev[i].DevAdd;
        WriteReg(WRITE1, SgDEVID[i], DEVADDR, &WData, (bool) CurDev[i].AckEn);
        //Calculate new device ID
        SgDEVID[i] = getDevID(
            CurDev[i].DevAdd, CurDev[i].P0InvEn, (bool) CurDev[i].P1InvEn);

#if (EEPROM_CHANGE)
        //Load into EEPROM
        WData = ProgEEPROM;
        WriteReg(WRITE1, SgDEVID[i], COMMAND, &WData, CurDev[i].AckEn);
        delay_cycles(80000 * 150);  //150ms
#endif
        //Leave CTM moode
        WData = LeaveCTM;
        WriteReg(WRITE1, SgDEVID[i], COMMAND, &WData, (bool) CurDev[i].AckEn);
        CommsRst();
    }
    MultiDEVID = getDevID(DEVICE_MULTIADD, P0_INV_EN, P1_INV_EN);
}

void RegInit(void)
{
    uint8_t WData;
    // Init DEVADDR, UARTCFG, MLTCSTx, register
    // Init device ID, Muliticase device ID, UART configuration
    DeviceInit();

    // Init normal register
    WriteReg(WRITE64, MultiDEVID, 0x00, InitRegValue, false);
    WriteReg(WRITE20, MultiDEVID, 0x40, &InitRegValue[64], false);
    WriteReg(WRITE3, MultiDEVID, 0x80, &InitRegValue[83], false);
    WriteReg(WRITE2, MultiDEVID, 0xE9, &InitRegValue[86], false);
    WriteReg(WRITE2, MultiDEVID, 0xFC, &InitRegValue[88], false);

    //Init some normal register specially for Follower
    WData = 0x54;
    WriteReg(WRITE1, SgDEVID[1], BSTCFG3, &WData, (bool) CurDev[1].AckEn);
    WData = 0xC0;
    WriteReg(WRITE1, SgDEVID[1], MPIOCFG1, &WData, (bool) CurDev[1].AckEn);
    delay_cycles(80000 * 10);
}

// Change 6 ch PWM dute cycle for all device

void WidthChange(uint16_t *DutyCycle)
{
    uint8_t WidthReg[8];  //WIDTH3_1 ~ WIDTH6H


    WidthReg[0] = (DutyCycle[0] & 0x0003) | ((DutyCycle[1] & 0x0003) << 2) |
                  ((DutyCycle[2] & 0x0003) << 4);  //WIDTH3_1
    WidthReg[1] = (DutyCycle[3] & 0x0003) | ((DutyCycle[4] & 0x0003) << 2) |
                  ((DutyCycle[5] & 0x0003) << 4);  //WIDTH6_4
    WidthReg[2] = (uint8_t)(DutyCycle[0] >> 2);    //WIDTH1H
    WidthReg[3] = (uint8_t)(DutyCycle[1] >> 2);    //WIDTH2H
    WidthReg[4] = (uint8_t)(DutyCycle[2] >> 2);    //WIDTH3H
    WidthReg[5] = (uint8_t)(DutyCycle[3] >> 2);    //WIDTH4H
    WidthReg[6] = (uint8_t)(DutyCycle[4] >> 2);    //WIDTH5H
    WidthReg[7] = (uint8_t)(DutyCycle[5] >> 2);    //WIDTH6H

    WriteReg(WRITE8, MultiDEVID, WIDTH3_1L, WidthReg, false);
}
