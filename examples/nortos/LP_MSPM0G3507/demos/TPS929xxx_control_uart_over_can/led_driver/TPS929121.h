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

#ifndef TPS929121_H_
#define TPS929121_H_

#include "FW_protocol_def.h"
#include "FW_protocol_def_1st_gen.h"

#define TPS92912X 1
#define TPS929121 1

enum CHANNEL_ID {
    OUT0 = 0x00,
    OUT1,
    OUT2,
    OUT3,
    OUT4,
    OUT5,
    OUT6,
    OUT7,
    OUT8,
    OUT9,
    OUT10,
    OUT11
};

#define MAX_CHANNEL_CNT 12
#define MAX_CHANNELS (MAX_CHANNEL_CNT)

/* Register addresses */
#define IOUT0 (0x00)  /* Output Current Setting for CH0 */
#define IOUT1 (0x01)  /* Output Current Setting for CH1 */
#define IOUT2 (0x02)  /* Output Current Setting for CH2 */
#define IOUT3 (0x03)  /* Output Current Setting for CH3 */
#define IOUT4 (0x04)  /* Output Current Setting for CH4 */
#define IOUT5 (0x05)  /* Output Current Setting for CH5 */
#define IOUT6 (0x06)  /* Output Current Setting for CH6 */
#define IOUT7 (0x07)  /* Output Current Setting for CH7 */
#define IOUT8 (0x08)  /* Output Current Setting for CH8 */
#define IOUT9 (0x09)  /* Output Current Setting for CH9 */
#define IOUT10 (0x0A) /* Output Current Setting for CH10 */
#define IOUT11 (0x0B) /* Output Current Setting for CH11 */

#define PWM0 (0x20)  /* Output PWM Duty-cycle Setting for CH0 */
#define PWM1 (0x21)  /* Output PWM Duty-cycle Setting for CH1 */
#define PWM2 (0x22)  /* Output PWM Duty-cycle Setting for CH2 */
#define PWM3 (0x23)  /* Output PWM Duty-cycle Setting for CH3 */
#define PWM4 (0x24)  /* Output PWM Duty-cycle Setting for CH4 */
#define PWM5 (0x25)  /* Output PWM Duty-cycle Setting for CH5 */
#define PWM6 (0x26)  /* Output PWM Duty-cycle Setting for CH6 */
#define PWM7 (0x27)  /* Output PWM Duty-cycle Setting for CH7 */
#define PWM8 (0x28)  /* Output PWM Duty-cycle Setting for CH8 */
#define PWM9 (0x29)  /* Output PWM Duty-cycle Setting for CH9 */
#define PWM10 (0x2A) /* Output PWM Duty-cycle Setting for CH10 */
#define PWM11 (0x2B) /* Output PWM Duty-cycle Setting for CH11 */

#define PWML0 (0x40)  /* Output PWM Duty-cycle Setting Lower bits for CH0 */
#define PWML1 (0x41)  /* Output PWM Duty-cycle Setting Lower bits for CH1 */
#define PWML2 (0x42)  /* Output PWM Duty-cycle Setting Lower bits for CH2 */
#define PWML3 (0x43)  /* Output PWM Duty-cycle Setting Lower bits for CH3 */
#define PWML4 (0x44)  /* Output PWM Duty-cycle Setting Lower bits for CH4 */
#define PWML5 (0x45)  /* Output PWM Duty-cycle Setting Lower bits for CH5 */
#define PWML6 (0x46)  /* Output PWM Duty-cycle Setting Lower bits for CH6 */
#define PWML7 (0x47)  /* Output PWM Duty-cycle Setting Lower bits for CH7 */
#define PWML8 (0x48)  /* Output PWM Duty-cycle Setting Lower bits for CH8 */
#define PWML9 (0x49)  /* Output PWM Duty-cycle Setting Lower bits for CH9 */
#define PWML10 (0x4A) /* Output PWM Duty-cycle Setting Lower bits for CH10 */
#define PWML11 (0x4B) /* Output PWM Duty-cycle Setting Lower bits for CH11 */

#define CONF_EN0 (0x50)     /* Channel Enable Register 0 */
#define CONF_EN1 (0x51)     /* Channel Enable Register 1 */
#define CONF_DIAGEN0 (0x54) /* Diagnostics Enable Register 0 */
#define CONF_DIAGEN1 (0x55) /* Diagnostics Enable Register 1 */
#define CONF_MISC0 (0x56)   /* Miscellanous Register 0 */
#define CONF_MISC1 (0x57)   /* Miscellanous Register 1 */
#define CONF_MISC2 (0x58)   /* Miscellanous Register 2 */
#define CONF_MISC3 (0x59)   /* Miscellanous Register 3 */
#define CONF_MISC4 (0x5A)   /* Miscellanous Register 4 */
#define CONF_MISC5 (0x5B)   /* Miscellanous Register 5 */

#define CLR (0x60)        /* Configuration Register for Clear */
#define CONF_LOCK (0x61)  /* Configuration Register for LOCK */
#define CONF_MISC6 (0x62) /* Miscellanous Register 6 */
#define CONF_MISC7 (0x63) /* Miscellanous Register 7 */
#define CONF_MISC8 (0x64) /* Miscellanous Register 8 */
#define CONF_MISC9 (0x65) /* Miscellanous Register 9 */

#define FLAG0 (0x70)  /* Device status flag register 0 */
#define FLAG1 (0x71)  /* Device status flag register 1 */
#define FLAG2 (0x72)  /* Device status flag register 2 */
#define FLAG3 (0x73)  /* Device status flag register 3 */
#define FLAG4 (0x74)  /* Device status flag register 4 */
#define FLAG5 (0x75)  /* Device status flag register 5 */
#define FLAG7 (0x77)  /* Device status flag register 7 */
#define FLAG8 (0x78)  /* Device status flag register 8 */
#define FLAG11 (0x7B) /* Device status flag register 11 */
#define FLAG12 (0x7C) /* Device status flag register 12 */
#define FLAG13 (0x7D) /* Device status flag register 13 */
#define FLAG14 (0x7E) /* Device status flag register 14 */

#define EEPI0 (0x80)  /* EEPROM Output Current Setting for CH0 */
#define EEPI1 (0x81)  /* EEPROM Output Current Setting for CH1 */
#define EEPI2 (0x82)  /* EEPROM Output Current Setting for CH2 */
#define EEPI3 (0x83)  /* EEPROM Output Current Setting for CH3 */
#define EEPI4 (0x84)  /* EEPROM Output Current Setting for CH4 */
#define EEPI5 (0x85)  /* EEPROM Output Current Setting for CH5 */
#define EEPI6 (0x86)  /* EEPROM Output Current Setting for CH6 */
#define EEPI7 (0x87)  /* EEPROM Output Current Setting for CH7 */
#define EEPI8 (0x88)  /* EEPROM Output Current Setting for CH8 */
#define EEPI9 (0x89)  /* EEPROM Output Current Setting for CH9 */
#define EEPI10 (0x8A) /* EEPROM Output Current Setting for CH10 */
#define EEPI11 (0x8B) /* EEPROM Output Current Setting for CH11 */

#define EEPP0 (0xA0)  /* EEPROM Output PWM Duty-cycle Setting for CH0 */
#define EEPP1 (0xA1)  /* EEPROM Output PWM Duty-cycle Setting for CH1 */
#define EEPP2 (0xA2)  /* EEPROM Output PWM Duty-cycle Setting for CH2 */
#define EEPP3 (0xA3)  /* EEPROM Output PWM Duty-cycle Setting for CH3 */
#define EEPP4 (0xA4)  /* EEPROM Output PWM Duty-cycle Setting for CH4 */
#define EEPP5 (0xA5)  /* EEPROM Output PWM Duty-cycle Setting for CH5 */
#define EEPP6 (0xA6)  /* EEPROM Output PWM Duty-cycle Setting for CH6 */
#define EEPP7 (0xA7)  /* EEPROM Output PWM Duty-cycle Setting for CH7 */
#define EEPP8 (0xA8)  /* EEPROM Output PWM Duty-cycle Setting for CH8 */
#define EEPP9 (0xA9)  /* EEPROM Output PWM Duty-cycle Setting for CH9 */
#define EEPP10 (0xAA) /* EEPROM Output PWM Duty-cycle Setting for CH10 */
#define EEPP11 (0xAB) /* EEPROM Output PWM Duty-cycle Setting for CH11 */

#define EEPM0 (0xC0)  /* EEPROM Miscellaneous registers 0 */
#define EEPM1 (0xC1)  /* EEPROM Miscellaneous registers 1 */
#define EEPM2 (0xC2)  /* EEPROM Miscellaneous registers 2 */
#define EEPM3 (0xC3)  /* EEPROM Miscellaneous registers 3 */
#define EEPM4 (0xC4)  /* EEPROM Miscellaneous registers 4 */
#define EEPM5 (0xC5)  /* EEPROM Miscellaneous registers 5 */
#define EEPM6 (0xC6)  /* EEPROM Miscellaneous registers 6 */
#define EEPM7 (0xC7)  /* EEPROM Miscellaneous registers 7 */
#define EEPM8 (0xC8)  /* EEPROM Miscellaneous registers 8 */
#define EEPM9 (0xC9)  /* EEPROM Miscellaneous registers 9 */
#define EEPM10 (0xCA) /* EEPROM Miscellaneous registers 10 */
#define EEPM11 (0xCB) /* EEPROM Miscellaneous registers 11 */
#define EEPM12 (0xCC) /* EEPROM Miscellaneous registers 12 */
#define EEPM13 (0xCD) /* EEPROM Miscellaneous registers 13 */
#define EEPM14 (0xCE) /* EEPROM Miscellaneous registers 14 */
#define EEPM15 (0xCF) /* EEPROM CRC Check Value Registers */

/* IOUTx registers */
#define CONF_IOUT__64 \
    (0x3F) /* Output current setting IOUT = 64/64 * I(FULL_RANGE) */
#define CONF_IOUT__63 \
    (0x3E) /* Output current setting IOUT = 63/64 * I(FULL_RANGE) */
#define CONF_IOUT__62 \
    (0x3D) /* Output current setting IOUT = 62/64 * I(FULL_RANGE) */
#define CONF_IOUT__61 \
    (0x3C) /* Output current setting IOUT = 61/64 * I(FULL_RANGE) */
#define CONF_IOUT__60 \
    (0x3B) /* Output current setting IOUT = 60/64 * I(FULL_RANGE) */
#define CONF_IOUT__59 \
    (0x3A) /* Output current setting IOUT = 59/64 * I(FULL_RANGE) */
#define CONF_IOUT__58 \
    (0x39) /* Output current setting IOUT = 58/64 * I(FULL_RANGE) */
#define CONF_IOUT__57 \
    (0x38) /* Output current setting IOUT = 57/64 * I(FULL_RANGE) */
#define CONF_IOUT__56 \
    (0x37) /* Output current setting IOUT = 56/64 * I(FULL_RANGE) */
#define CONF_IOUT__55 \
    (0x36) /* Output current setting IOUT = 55/64 * I(FULL_RANGE) */
#define CONF_IOUT__54 \
    (0x35) /* Output current setting IOUT = 54/64 * I(FULL_RANGE) */
#define CONF_IOUT__53 \
    (0x34) /* Output current setting IOUT = 53/64 * I(FULL_RANGE) */
#define CONF_IOUT__52 \
    (0x33) /* Output current setting IOUT = 52/64 * I(FULL_RANGE) */
#define CONF_IOUT__51 \
    (0x32) /* Output current setting IOUT = 51/64 * I(FULL_RANGE) */
#define CONF_IOUT__50 \
    (0x31) /* Output current setting IOUT = 50/64 * I(FULL_RANGE) */
#define CONF_IOUT__49 \
    (0x30) /* Output current setting IOUT = 49/64 * I(FULL_RANGE) */
#define CONF_IOUT__48 \
    (0x2F) /* Output current setting IOUT = 48/64 * I(FULL_RANGE) */
#define CONF_IOUT__47 \
    (0x2E) /* Output current setting IOUT = 47/64 * I(FULL_RANGE) */
#define CONF_IOUT__46 \
    (0x2D) /* Output current setting IOUT = 46/64 * I(FULL_RANGE) */
#define CONF_IOUT__45 \
    (0x2C) /* Output current setting IOUT = 45/64 * I(FULL_RANGE) */
#define CONF_IOUT__44 \
    (0x2B) /* Output current setting IOUT = 44/64 * I(FULL_RANGE) */
#define CONF_IOUT__43 \
    (0x2A) /* Output current setting IOUT = 43/64 * I(FULL_RANGE) */
#define CONF_IOUT__42 \
    (0x29) /* Output current setting IOUT = 42/64 * I(FULL_RANGE) */
#define CONF_IOUT__41 \
    (0x28) /* Output current setting IOUT = 41/64 * I(FULL_RANGE) */
#define CONF_IOUT__40 \
    (0x27) /* Output current setting IOUT = 40/64 * I(FULL_RANGE) */
#define CONF_IOUT__39 \
    (0x26) /* Output current setting IOUT = 39/64 * I(FULL_RANGE) */
#define CONF_IOUT__38 \
    (0x25) /* Output current setting IOUT = 38/64 * I(FULL_RANGE) */
#define CONF_IOUT__37 \
    (0x24) /* Output current setting IOUT = 37/64 * I(FULL_RANGE) */
#define CONF_IOUT__36 \
    (0x23) /* Output current setting IOUT = 36/64 * I(FULL_RANGE) */
#define CONF_IOUT__35 \
    (0x22) /* Output current setting IOUT = 35/64 * I(FULL_RANGE) */
#define CONF_IOUT__34 \
    (0x21) /* Output current setting IOUT = 34/64 * I(FULL_RANGE) */
#define CONF_IOUT__33 \
    (0x20) /* Output current setting IOUT = 33/64 * I(FULL_RANGE) */
#define CONF_IOUT__32 \
    (0x1F) /* Output current setting IOUT = 32/64 * I(FULL_RANGE) */
#define CONF_IOUT__31 \
    (0x1E) /* Output current setting IOUT = 31/64 * I(FULL_RANGE) */
#define CONF_IOUT__30 \
    (0x1D) /* Output current setting IOUT = 30/64 * I(FULL_RANGE) */
#define CONF_IOUT__29 \
    (0x1C) /* Output current setting IOUT = 29/64 * I(FULL_RANGE) */
#define CONF_IOUT__28 \
    (0x1B) /* Output current setting IOUT = 28/64 * I(FULL_RANGE) */
#define CONF_IOUT__27 \
    (0x1A) /* Output current setting IOUT = 27/64 * I(FULL_RANGE) */
#define CONF_IOUT__26 \
    (0x19) /* Output current setting IOUT = 26/64 * I(FULL_RANGE) */
#define CONF_IOUT__25 \
    (0x18) /* Output current setting IOUT = 25/64 * I(FULL_RANGE) */
#define CONF_IOUT__24 \
    (0x17) /* Output current setting IOUT = 24/64 * I(FULL_RANGE) */
#define CONF_IOUT__23 \
    (0x16) /* Output current setting IOUT = 23/64 * I(FULL_RANGE) */
#define CONF_IOUT__22 \
    (0x15) /* Output current setting IOUT = 22/64 * I(FULL_RANGE) */
#define CONF_IOUT__21 \
    (0x14) /* Output current setting IOUT = 21/64 * I(FULL_RANGE) */
#define CONF_IOUT__20 \
    (0x13) /* Output current setting IOUT = 20/64 * I(FULL_RANGE) */
#define CONF_IOUT__19 \
    (0x12) /* Output current setting IOUT = 19/64 * I(FULL_RANGE) */
#define CONF_IOUT__18 \
    (0x11) /* Output current setting IOUT = 18/64 * I(FULL_RANGE) */
#define CONF_IOUT__17 \
    (0x10) /* Output current setting IOUT = 17/64 * I(FULL_RANGE) */
#define CONF_IOUT__16 \
    (0x0F) /* Output current setting IOUT = 16/64 * I(FULL_RANGE) */
#define CONF_IOUT__15 \
    (0x0E) /* Output current setting IOUT = 15/64 * I(FULL_RANGE) */
#define CONF_IOUT__14 \
    (0x0D) /* Output current setting IOUT = 14/64 * I(FULL_RANGE) */
#define CONF_IOUT__13 \
    (0x0C) /* Output current setting IOUT = 13/64 * I(FULL_RANGE) */
#define CONF_IOUT__12 \
    (0x0B) /* Output current setting IOUT = 12/64 * I(FULL_RANGE) */
#define CONF_IOUT__11 \
    (0x0A) /* Output current setting IOUT = 11/64 * I(FULL_RANGE) */
#define CONF_IOUT__10 \
    (0x09) /* Output current setting IOUT = 10/64 * I(FULL_RANGE) */
#define CONF_IOUT__9 \
    (0x08) /* Output current setting IOUT = 9/64 * I(FULL_RANGE) */
#define CONF_IOUT__8 \
    (0x07) /* Output current setting IOUT = 8/64 * I(FULL_RANGE) */
#define CONF_IOUT__7 \
    (0x06) /* Output current setting IOUT = 7/64 * I(FULL_RANGE) */
#define CONF_IOUT__6 \
    (0x05) /* Output current setting IOUT = 6/64 * I(FULL_RANGE) */
#define CONF_IOUT__5 \
    (0x04) /* Output current setting IOUT = 5/64 * I(FULL_RANGE) */
#define CONF_IOUT__4 \
    (0x03) /* Output current setting IOUT = 4/64 * I(FULL_RANGE) */
#define CONF_IOUT__3 \
    (0x02) /* Output current setting IOUT = 3/64 * I(FULL_RANGE) */
#define CONF_IOUT__2 \
    (0x01) /* Output current setting IOUT = 2/64 * I(FULL_RANGE) */
#define CONF_IOUT__1 \
    (0x00) /* Output current setting IOUT = 1/64 * I(FULL_RANGE) */

#define IOUT__MAX (0x3F)

/* IOUT0 register */
#define CONF_IOUT0 (0x3F) /* Output current setting for OUT0 */

/* IOUT1 register */
#define CONF_IOUT1 (0x3F) /* Output current setting for OUT1 */

/* IOUT2 register */
#define CONF_IOUT2 (0x3F) /* Output current setting for OUT2 */

/* IOUT3 register */
#define CONF_IOUT3 (0x3F) /* Output current setting for OUT3 */

/* IOUT4 register */
#define CONF_IOUT4 (0x3F) /* Output current setting for OUT4 */

/* IOUT5 register */
#define CONF_IOUT5 (0x3F) /* Output current setting for OUT5 */

/* IOUT6 register */
#define CONF_IOUT6 (0x3F) /* Output current setting for OUT6 */

/* IOUT7 register */
#define CONF_IOUT7 (0x3F) /* Output current setting for OUT7 */

/* IOUT8 register */
#define CONF_IOUT8 (0x3F) /* Output current setting for OUT8 */

/* IOUT9 register */
#define CONF_IOUT9 (0x3F) /* Output current setting for OUT9 */

/* IOUT10 register */
#define CONF_IOUT10 (0x3F) /* Output current setting for OUT10 */

/* IOUT11 register */
#define CONF_IOUT11 (0x3F) /* Output current setting for OUT11 */

#define CONF_PWM__1 \
    (0xFFF) /* Set all bits of CONF_PWMOUT and CONF_PWMLOWOUT to 1 */
#define CONF_PWM__0 \
    (0x000) /* Set all bits of CONF_PWMOUT and CONF_PWMLOWOUT to 0 */
#define PWM__1 \
    (0xFFF) /* Set all bits of CONF_PWMOUT and CONF_PWMLOWOUT to 1 */
#define PWM__0 \
    (0x000) /* Set all bits of CONF_PWMOUT and CONF_PWMLOWOUT to 0 */

#define CONF_PWMOUT__1 (0xFF) /* Set all bits of CONF_PWMOUT to 1 */
#define CONF_PWMOUT__0 (0x00) /* Set all bits of CONF_PWMOUT to 0 */

/* PWM0 register */
#define CONF_PWMOUT0 (0xFF) /* Output PWM Duty-cycle Setting for CH0 */

/* PWM1 register */
#define CONF_PWMOUT1 (0xFF) /* Output PWM Duty-cycle Setting for CH1 */

/* PWM2 register */
#define CONF_PWMOUT2 (0xFF) /* Output PWM Duty-cycle Setting for CH2 */

/* PWM3 register */
#define CONF_PWMOUT3 (0xFF) /* Output PWM Duty-cycle Setting for CH3 */

/* PWM4 register */
#define CONF_PWMOUT4 (0xFF) /* Output PWM Duty-cycle Setting for CH4 */

/* PWM5 register */
#define CONF_PWMOUT5 (0xFF) /* Output PWM Duty-cycle Setting for CH5 */

/* PWM6 register */
#define CONF_PWMOUT6 (0xFF) /* Output PWM Duty-cycle Setting for CH6 */

/* PWM7 register */
#define CONF_PWMOUT7 (0xFF) /* Output PWM Duty-cycle Setting for CH7 */

/* PWM8 register */
#define CONF_PWMOUT8 (0xFF) /* Output PWM Duty-cycle Setting for CH8 */

/* PWM9 register */
#define CONF_PWMOUT9 (0xFF) /* Output PWM Duty-cycle Setting for CH9 */

/* PWM10 register */
#define CONF_PWMOUT10 (0xFF) /* Output PWM Duty-cycle Setting for CH10 */

/* PWM11 register */
#define CONF_PWMOUT11 (0xFF) /* Output PWM Duty-cycle Setting for CH11 */

#define CONF_PWMLOWOUT__1 (0x0F) /* Set all bits of CONF_PWMLOWOUT to 1 */
#define CONF_PWMLOWOUT__0 (0x00) /* Set all bits of CONF_PWMLOWOUT to 0 */

/* PWML0 register */
#define CONF_PWMLOWOUT0 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH0 */

/* PWML1 register */
#define CONF_PWMLOWOUT1 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH1 */

/* PWML2 register */
#define CONF_PWMLOWOUT2 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH2 */

/* PWML3 register */
#define CONF_PWMLOWOUT3 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH3 */

/* PWML4 register */
#define CONF_PWMLOWOUT4 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH4 */

/* PWML5 register */
#define CONF_PWMLOWOUT5 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH5 */

/* PWML6 register */
#define CONF_PWMLOWOUT6 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH6 */

/* PWML7 register */
#define CONF_PWMLOWOUT7 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH7 */

/* PWML8 register */
#define CONF_PWMLOWOUT8 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH8 */

/* PWML9 register */
#define CONF_PWMLOWOUT9 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH9 */

/* PWML10 register */
#define CONF_PWMLOWOUT10 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH10 */

/* PWML11 register */
#define CONF_PWMLOWOUT11 \
    (0x0F) /* Output PWM Duty-cycle Setting Lower bits for CH11 */

#define CONF_EN__1 (0xFFF) /* Enable all channels of CONF_EN0 and CONF_EN1 */
#define CONF_EN__0 (0x000) /* Disable all channels of CONF_EN0 and CONF_EN1 */

/* CONF_EN0 register */
#define CONF_EN0__1 (0xFF)   /* Enable all channels of CONF_EN0 */
#define CONF_EN0__0 (0x00)   /* Disable all channels of CONF_EN0 */
#define CONF_ENCH7__1 (0x80) /* Channel 7 enable */
#define CONF_ENCH7__0 (0x00) /* Channel 7 disable */
#define CONF_ENCH6__1 (0x40) /* Channel 6 enable */
#define CONF_ENCH6__0 (0x00) /* Channel 6 disable */
#define CONF_ENCH5__1 (0x20) /* Channel 5 enable */
#define CONF_ENCH5__0 (0x00) /* Channel 5 disable */
#define CONF_ENCH4__1 (0x10) /* Channel 4 enable */
#define CONF_ENCH4__0 (0x00) /* Channel 4 disable */
#define CONF_ENCH3__1 (0x08) /* Channel 3 enable */
#define CONF_ENCH3__0 (0x00) /* Channel 3 disable */
#define CONF_ENCH2__1 (0x04) /* Channel 2 enable */
#define CONF_ENCH2__0 (0x00) /* Channel 2 disable */
#define CONF_ENCH1__1 (0x02) /* Channel 1 enable */
#define CONF_ENCH1__0 (0x00) /* Channel 1 disable */
#define CONF_ENCH0__1 (0x01) /* Channel 0 enable */
#define CONF_ENCH0__0 (0x00) /* Channel 0 disable */

/* CONF_EN1 register */
#define CONF_EN1__1 (0x0F)    /* Enable all channels of CONF_EN1 */
#define CONF_EN1__0 (0x00)    /* Disable all channels of CONF_EN1 */
#define CONF_ENCH11__1 (0x08) /* Channel 11 enable */
#define CONF_ENCH11__0 (0x00) /* Channel 11 disable */
#define CONF_ENCH10__1 (0x04) /* Channel 10 enable */
#define CONF_ENCH10__0 (0x00) /* Channel 10 disable */
#define CONF_ENCH9__1 (0x02)  /* Channel 9 enable */
#define CONF_ENCH9__0 (0x00)  /* Channel 9 disable */
#define CONF_ENCH8__1 (0x01)  /* Channel 8 enable */
#define CONF_ENCH8__0 (0x00)  /* Channel 8 disable */

#define CONF_DIAGEN__1 \
    (0xFFF) /* Enable diagnostics for all channels of CONF_DIAGEN0 and CONF_DIAGEN1 */
#define CONF_DIAGEN__0 \
    (0x000) /* Disable diagnostics for all channels of CONF_DIAGEN0 and CONF_DIAGEN1 */

/* CONF_DIAGEN0 register */
#define CONF_DIAGEN0__1 \
    (0xFF) /* Enable diagnostics for all channels of CONF_DIAGEN0 */
#define CONF_DIAGEN0__0 \
    (0x00) /* Disable diagnostics for all channels of CONF_DIAGEN0 */
#define CONF_DIAGENCH7__1 (0x80) /* Channel 7 diagnostics enable */
#define CONF_DIAGENCH7__0 (0x00) /* Channel 7 diagnostics disable */
#define CONF_DIAGENCH6__1 (0x40) /* Channel 6 diagnostics enable */
#define CONF_DIAGENCH6__0 (0x00) /* Channel 6 diagnostics disable */
#define CONF_DIAGENCH5__1 (0x20) /* Channel 5 diagnostics enable */
#define CONF_DIAGENCH5__0 (0x00) /* Channel 5 diagnostics disable */
#define CONF_DIAGENCH4__1 (0x10) /* Channel 4 diagnostics enable */
#define CONF_DIAGENCH4__0 (0x00) /* Channel 4 diagnostics disable */
#define CONF_DIAGENCH3__1 (0x08) /* Channel 3 diagnostics enable */
#define CONF_DIAGENCH3__0 (0x00) /* Channel 3 diagnostics disable */
#define CONF_DIAGENCH2__1 (0x04) /* Channel 2 diagnostics enable */
#define CONF_DIAGENCH2__0 (0x00) /* Channel 2 diagnostics disable */
#define CONF_DIAGENCH1__1 (0x02) /* Channel 1 diagnostics enable */
#define CONF_DIAGENCH1__0 (0x00) /* Channel 1 diagnostics disable */
#define CONF_DIAGENCH0__1 (0x01) /* Channel 0 diagnostics enable */
#define CONF_DIAGENCH0__0 (0x00) /* Channel 0 diagnostics disable */

/* CONF_DIAGEN1 register */
#define CONF_DIAGEN1__1 \
    (0x0F) /* Enable diagnostics for all channels of CONF_DIAGEN1 */
#define CONF_DIAGEN1__0 \
    (0x00) /* Disable diagnostics for all channels of CONF_DIAGEN1 */
#define CONF_DIAGENCH11__1 (0x08) /* Channel 11 diagnostics enable */
#define CONF_DIAGENCH11__0 (0x00) /* Channel 11 diagnostics disable */
#define CONF_DIAGENCH10__1 (0x04) /* Channel 10 diagnostics enable */
#define CONF_DIAGENCH10__0 (0x00) /* Channel 10 diagnostics disable */
#define CONF_DIAGENCH9__1 (0x02)  /* Channel 9 diagnostics enable */
#define CONF_DIAGENCH9__0 (0x00)  /* Channel 9 diagnostics disable */
#define CONF_DIAGENCH8__1 (0x01)  /* Channel 8 diagnostics enable */
#define CONF_DIAGENCH8__0 (0x00)  /* Channel 8 diagnostics disable */

/* CONF_MISC0 register */
#define CONF_AUTOSS__1 \
    (0x80) /* Auto single-LED short-circuit configuration enable */
#define CONF_AUTOSS__0 \
    (0x00) /* Auto single-LED short-circuit configuration disable */
#define CONF_LDO__4V4 (0x40) /* LDO output voltage 4.4V */
#define CONF_LDO__5V0 (0x00) /* LDO output voltage 5V */
#define CONF_LDO__1 (0x40)   /* LDO output voltage 4.4V */
#define CONF_LDO__0 (0x00)   /* LDO output voltage 5V */
#define CONF_EXPEN__1 (0x10) /* PWM exponetinal dimming enable */
#define CONF_EXPEN__0 (0x00) /* PWM exponetinal dimming disable */

/* CONF_MISC1 register */
#define CONF_PWMFREQ__20K8 (0xF0) /* PWM frequency 20.8 kHz */
#define CONF_PWMFREQ__9K6 (0xE0)  /* PWM frequency 9.6 kHz */
#define CONF_PWMFREQ__7K8 (0xD0)  /* PWM frequency 7.8 kHz */
#define CONF_PWMFREQ__5K9 (0xC0)  /* PWM frequency 5.9 kHz */
#define CONF_PWMFREQ__4K0 (0xB0)  /* PWM frequency 4 kHz */
#define CONF_PWMFREQ__2K0 (0xA0)  /* PWM frequency 2 kHz */
#define CONF_PWMFREQ__1200 (0x90) /* PWM frequency 1200 Hz */
#define CONF_PWMFREQ__1000 (0x80) /* PWM frequency 1000 Hz */
#define CONF_PWMFREQ__800 (0x70)  /* PWM frequency 800 Hz */
#define CONF_PWMFREQ__600 (0x60)  /* PWM frequency 600 Hz */
#define CONF_PWMFREQ__500 (0x50)  /* PWM frequency 500 Hz */
#define CONF_PWMFREQ__400 (0x40)  /* PWM frequency 400 Hz */
#define CONF_PWMFREQ__350 (0x30)  /* PWM frequency 350 Hz */
#define CONF_PWMFREQ__300 (0x20)  /* PWM frequency 300 Hz */
#define CONF_PWMFREQ__250 (0x10)  /* PWM frequency 250 Hz */
#define CONF_PWMFREQ__200 (0x00)  /* PWM frequency 200 Hz */
#define PWMFREQ__20K8 (0xF0)      /* PWM frequency 20.8 kHz */
#define PWMFREQ__9K6 (0xE0)       /* PWM frequency 9.6 kHz */
#define PWMFREQ__7K8 (0xD0)       /* PWM frequency 7.8 kHz */
#define PWMFREQ__5K9 (0xC0)       /* PWM frequency 5.9 kHz */
#define PWMFREQ__4K0 (0xB0)       /* PWM frequency 4 kHz */
#define PWMFREQ__2K0 (0xA0)       /* PWM frequency 2 kHz */
#define PWMFREQ__1200 (0x90)      /* PWM frequency 1200 Hz */
#define PWMFREQ__1000 (0x80)      /* PWM frequency 1000 Hz */
#define PWMFREQ__800 (0x70)       /* PWM frequency 800 Hz */
#define PWMFREQ__600 (0x60)       /* PWM frequency 600 Hz */
#define PWMFREQ__500 (0x50)       /* PWM frequency 500 Hz */
#define PWMFREQ__400 (0x40)       /* PWM frequency 400 Hz */
#define PWMFREQ__350 (0x30)       /* PWM frequency 350 Hz */
#define PWMFREQ__300 (0x20)       /* PWM frequency 300 Hz */
#define PWMFREQ__250 (0x10)       /* PWM frequency 250 Hz */
#define PWMFREQ__200 (0x00)       /* PWM frequency 200 Hz */
#define CONF_REFRANGE__512 (0x03) /* Reference current ratio 512 */
#define CONF_REFRANGE__256 (0x02) /* Reference current ratio 256 */
#define CONF_REFRANGE__128 (0x01) /* Reference current ratio 128 */
#define CONF_REFRANGE__64 (0x00)  /* Reference current ratio 64 */
#define REFRANGE__512 (0x03)      /* Reference current ratio 512 */
#define REFRANGE__256 (0x02)      /* Reference current ratio 256 */
#define REFRANGE__128 (0x01)      /* Reference current ratio 128 */
#define REFRANGE__64 (0x00)       /* Reference current ratio 64 */

/* CONF_MISC2 register */
#define CONF_FLTIMEOUT__10M0 (0x70)  /* FlexWire timeout 10 ms */
#define CONF_FLTIMEOUT__5M0 (0x60)   /* FlexWire timeout 5 ms */
#define CONF_FLTIMEOUT__2M5 (0x50)   /* FlexWire timeout 2.5 ms */
#define CONF_FLTIMEOUT__1M25 (0x40)  /* FlexWire timeout 1.25 ms */
#define CONF_FLTIMEOUT__500U (0x30)  /* FlexWire timeout 500 us */
#define CONF_FLTIMEOUT__250U (0x20)  /* FlexWire timeout 250 us */
#define CONF_FLTIMEOUT__125U (0x10)  /* FlexWire timeout 125 us */
#define CONF_FLTIMEOUT__1M0 (0x00)   /* FlexWire timeout 1 ms */
#define CONF_ADCLOWSUPTH__40V (0x0F) /* ADC Supply monitor threshold 40V */
#define CONF_ADCLOWSUPTH__38V (0x0E) /* ADC Supply monitor threshold 38V */
#define CONF_ADCLOWSUPTH__36V (0x0D) /* ADC Supply monitor threshold 36V */
#define CONF_ADCLOWSUPTH__34V (0x0C) /* ADC Supply monitor threshold 34V */
#define CONF_ADCLOWSUPTH__32V (0x0B) /* ADC Supply monitor threshold 32V */
#define CONF_ADCLOWSUPTH__30V (0x0A) /* ADC Supply monitor threshold 30V */
#define CONF_ADCLOWSUPTH__28V (0x09) /* ADC Supply monitor threshold 28V */
#define CONF_ADCLOWSUPTH__26V (0x08) /* ADC Supply monitor threshold 26V */
#define CONF_ADCLOWSUPTH__24V (0x07) /* ADC Supply monitor threshold 24V */
#define CONF_ADCLOWSUPTH__22V (0x06) /* ADC Supply monitor threshold 22V */
#define CONF_ADCLOWSUPTH__20V (0x05) /* ADC Supply monitor threshold 20V */
#define CONF_ADCLOWSUPTH__18V (0x04) /* ADC Supply monitor threshold 18V */
#define CONF_ADCLOWSUPTH__16V (0x03) /* ADC Supply monitor threshold 16V */
#define CONF_ADCLOWSUPTH__14V (0x02) /* ADC Supply monitor threshold 14V */
#define CONF_ADCLOWSUPTH__12V (0x01) /* ADC Supply monitor threshold 12V */
#define CONF_ADCLOWSUPTH__10V (0x00) /* ADC Supply monitor threshold 10V */
#define LOWSUPTH__40V (0x0F)         /* ADC Supply monitor threshold 40V */
#define LOWSUPTH__38V (0x0E)         /* ADC Supply monitor threshold 38V */
#define LOWSUPTH__36V (0x0D)         /* ADC Supply monitor threshold 36V */
#define LOWSUPTH__34V (0x0C)         /* ADC Supply monitor threshold 34V */
#define LOWSUPTH__32V (0x0B)         /* ADC Supply monitor threshold 32V */
#define LOWSUPTH__30V (0x0A)         /* ADC Supply monitor threshold 30V */
#define LOWSUPTH__28V (0x09)         /* ADC Supply monitor threshold 28V */
#define LOWSUPTH__26V (0x08)         /* ADC Supply monitor threshold 26V */
#define LOWSUPTH__24V (0x07)         /* ADC Supply monitor threshold 24V */
#define LOWSUPTH__22V (0x06)         /* ADC Supply monitor threshold 22V */
#define LOWSUPTH__20V (0x05)         /* ADC Supply monitor threshold 20V */
#define LOWSUPTH__18V (0x04)         /* ADC Supply monitor threshold 18V */
#define LOWSUPTH__16V (0x03)         /* ADC Supply monitor threshold 16V */
#define LOWSUPTH__14V (0x02)         /* ADC Supply monitor threshold 14V */
#define LOWSUPTH__12V (0x01)         /* ADC Supply monitor threshold 12V */
#define LOWSUPTH__10V (0x00)         /* ADC Supply monitor threshold 10V */

/* CONF_MISC3 register */
#define CONF_ODIOUT__64 \
    (0xF0) /* On-demand diagnostics output current IOUT = 64/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__57 \
    (0xE0) /* On-demand diagnostics output current IOUT = 57/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__53 \
    (0xD0) /* On-demand diagnostics output current IOUT = 53/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__49 \
    (0xC0) /* On-demand diagnostics output current IOUT = 49/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__45 \
    (0xB0) /* On-demand diagnostics output current IOUT = 45/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__41 \
    (0xA0) /* On-demand diagnostics output current IOUT = 41/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__37 \
    (0x90) /* On-demand diagnostics output current IOUT = 37/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__33 \
    (0x80) /* On-demand diagnostics output current IOUT = 33/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__29 \
    (0x70) /* On-demand diagnostics output current IOUT = 29/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__25 \
    (0x60) /* On-demand diagnostics output current IOUT = 25/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__21 \
    (0x50) /* On-demand diagnostics output current IOUT = 21/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__17 \
    (0x40) /* On-demand diagnostics output current IOUT = 17/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__13 \
    (0x30) /* On-demand diagnostics output current IOUT = 13/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__9 \
    (0x20) /* On-demand diagnostics output current IOUT = 9/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__5 \
    (0x10) /* On-demand diagnostics output current IOUT = 5/64 * I(FULL_RANGE) */
#define CONF_ODIOUT__1 \
    (0x00) /* On-demand diagnostics output current IOUT = 1/64 * I(FULL_RANGE) */
#define CONF_ODPW__5M0 (0x0F)  /* On-demand diagnostics pulse-width 5 ms */
#define CONF_ODPW__3M0 (0x0E)  /* On-demand diagnostics pulse-width 3 ms */
#define CONF_ODPW__2M0 (0x0D)  /* On-demand diagnostics pulse-width 2 ms */
#define CONF_ODPW__1M5 (0x0C)  /* On-demand diagnostics pulse-width 1.5 ms */
#define CONF_ODPW__1M2 (0x0B)  /* On-demand diagnostics pulse-width 1.2 ms */
#define CONF_ODPW__1M0 (0x0A)  /* On-demand diagnostics pulse-width 1 ms */
#define CONF_ODPW__800U (0x09) /* On-demand diagnostics pulse-width 800 us */
#define CONF_ODPW__500U (0x08) /* On-demand diagnostics pulse-width 500 us */
#define CONF_ODPW__300U (0x07) /* On-demand diagnostics pulse-width 300 us */
#define CONF_ODPW__200U (0x06) /* On-demand diagnostics pulse-width 200 us */
#define CONF_ODPW__150U (0x05) /* On-demand diagnostics pulse-width 150 us */
#define CONF_ODPW__80U (0x04)  /* On-demand diagnostics pulse-width 80 us */
#define CONF_ODPW__50U (0x03)  /* On-demand diagnostics pulse-width 50 us */
#define CONF_ODPW__30U (0x02)  /* On-demand diagnostics pulse-width 30 us */
#define CONF_ODPW__20U (0x01)  /* On-demand diagnostics pulse-width 20 us */
#define CONF_ODPW__100U (0x00) /* On-demand diagnostics pulse-width 100 us */

/* CONF_MISC4 register */
#define CONF_WDTIMER__0_FS \
    (0xF0) /* Watchdog timer disabled; direct enter fail-safe state */
#define CONF_WDTIMER__500M0 (0xB0) /* Watchdog timer 500 ms */
#define CONF_WDTIMER__200M0 (0xA0) /* Watchdog timer 500 ms */
#define CONF_WDTIMER__100M0 (0x90) /* Watchdog timer 500 ms */
#define CONF_WDTIMER__50M0 (0x80)  /* Watchdog timer 500 ms */
#define CONF_WDTIMER__20M0 (0x70)  /* Watchdog timer 500 ms */
#define CONF_WDTIMER__10M0 (0x60)  /* Watchdog timer 500 ms */
#define CONF_WDTIMER__5M0 (0x50)   /* Watchdog timer 500 ms */
#define CONF_WDTIMER__2M0 (0x40)   /* Watchdog timer 500 ms */
#define CONF_WDTIMER__1M0 (0x30)   /* Watchdog timer 500 ms */
#define CONF_WDTIMER__500U (0x20)  /* Watchdog timer 500 ms */
#define CONF_WDTIMER__200U (0x10)  /* Watchdog timer 500 ms */
#define CONF_WDTIMER__0_NO_FS \
    (0x00) /* Watchdog timer disabled; do not automatically enter fail-safe state */

/* CONF_MISC5 register */
#define CONF_ADCSHORTTH__40V65 \
    (0xFF) /* ADC short detection threshold 40.6543 V */
#define CONF_ADCSHORTTH__40V50 \
    (0xFE) /* ADC short detection threshold 40.496 V */
#define CONF_ADCSHORTTH__40V34 \
    (0xFD) /* ADC short detection threshold 40.3377 V */
#define CONF_ADCSHORTTH__40V18 \
    (0xFC) /* ADC short detection threshold 40.1794 V */
#define CONF_ADCSHORTTH__40V02 \
    (0xFB) /* ADC short detection threshold 40.0211 V */
#define CONF_ADCSHORTTH__39V86 \
    (0xFA) /* ADC short detection threshold 39.8628 V */
#define CONF_ADCSHORTTH__39V70 \
    (0xF9) /* ADC short detection threshold 39.7045 V */
#define CONF_ADCSHORTTH__39V55 \
    (0xF8) /* ADC short detection threshold 39.5462 V */
#define CONF_ADCSHORTTH__39V39 \
    (0xF7) /* ADC short detection threshold 39.3879 V */
#define CONF_ADCSHORTTH__39V23 \
    (0xF6) /* ADC short detection threshold 39.2296 V */
#define CONF_ADCSHORTTH__39V07 \
    (0xF5) /* ADC short detection threshold 39.0713 V */
#define CONF_ADCSHORTTH__38V91 \
    (0xF4) /* ADC short detection threshold 38.913 V */
#define CONF_ADCSHORTTH__38V75 \
    (0xF3) /* ADC short detection threshold 38.7547 V */
#define CONF_ADCSHORTTH__38V60 \
    (0xF2) /* ADC short detection threshold 38.5964 V */
#define CONF_ADCSHORTTH__38V44 \
    (0xF1) /* ADC short detection threshold 38.4381 V */
#define CONF_ADCSHORTTH__38V28 \
    (0xF0) /* ADC short detection threshold 38.2798 V */
#define CONF_ADCSHORTTH__38V12 \
    (0xEF) /* ADC short detection threshold 38.1215 V */
#define CONF_ADCSHORTTH__37V96 \
    (0xEE) /* ADC short detection threshold 37.9632 V */
#define CONF_ADCSHORTTH__37V80 \
    (0xED) /* ADC short detection threshold 37.8049 V */
#define CONF_ADCSHORTTH__37V65 \
    (0xEC) /* ADC short detection threshold 37.6466 V */
#define CONF_ADCSHORTTH__37V49 \
    (0xEB) /* ADC short detection threshold 37.4883 V */
#define CONF_ADCSHORTTH__37V33 \
    (0xEA) /* ADC short detection threshold 37.33 V */
#define CONF_ADCSHORTTH__37V17 \
    (0xE9) /* ADC short detection threshold 37.1717 V */
#define CONF_ADCSHORTTH__37V01 \
    (0xE8) /* ADC short detection threshold 37.0134 V */
#define CONF_ADCSHORTTH__36V86 \
    (0xE7) /* ADC short detection threshold 36.8551 V */
#define CONF_ADCSHORTTH__36V70 \
    (0xE6) /* ADC short detection threshold 36.6968 V */
#define CONF_ADCSHORTTH__36V54 \
    (0xE5) /* ADC short detection threshold 36.5385 V */
#define CONF_ADCSHORTTH__36V38 \
    (0xE4) /* ADC short detection threshold 36.3802 V */
#define CONF_ADCSHORTTH__36V22 \
    (0xE3) /* ADC short detection threshold 36.2219 V */
#define CONF_ADCSHORTTH__36V06 \
    (0xE2) /* ADC short detection threshold 36.0636 V */
#define CONF_ADCSHORTTH__35V91 \
    (0xE1) /* ADC short detection threshold 35.9053 V */
#define CONF_ADCSHORTTH__35V75 \
    (0xE0) /* ADC short detection threshold 35.747 V */
#define CONF_ADCSHORTTH__35V59 \
    (0xDF) /* ADC short detection threshold 35.5887 V */
#define CONF_ADCSHORTTH__35V43 \
    (0xDE) /* ADC short detection threshold 35.4304 V */
#define CONF_ADCSHORTTH__35V27 \
    (0xDD) /* ADC short detection threshold 35.2721 V */
#define CONF_ADCSHORTTH__35V11 \
    (0xDC) /* ADC short detection threshold 35.1138 V */
#define CONF_ADCSHORTTH__34V96 \
    (0xDB) /* ADC short detection threshold 34.9555 V */
#define CONF_ADCSHORTTH__34V80 \
    (0xDA) /* ADC short detection threshold 34.7972 V */
#define CONF_ADCSHORTTH__34V64 \
    (0xD9) /* ADC short detection threshold 34.6389 V */
#define CONF_ADCSHORTTH__34V48 \
    (0xD8) /* ADC short detection threshold 34.4806 V */
#define CONF_ADCSHORTTH__34V32 \
    (0xD7) /* ADC short detection threshold 34.3223 V */
#define CONF_ADCSHORTTH__34V16 \
    (0xD6) /* ADC short detection threshold 34.164 V */
#define CONF_ADCSHORTTH__34V01 \
    (0xD5) /* ADC short detection threshold 34.0057 V */
#define CONF_ADCSHORTTH__33V85 \
    (0xD4) /* ADC short detection threshold 33.8474 V */
#define CONF_ADCSHORTTH__33V69 \
    (0xD3) /* ADC short detection threshold 33.6891 V */
#define CONF_ADCSHORTTH__33V53 \
    (0xD2) /* ADC short detection threshold 33.5308 V */
#define CONF_ADCSHORTTH__33V37 \
    (0xD1) /* ADC short detection threshold 33.3725 V */
#define CONF_ADCSHORTTH__33V21 \
    (0xD0) /* ADC short detection threshold 33.2142 V */
#define CONF_ADCSHORTTH__33V06 \
    (0xCF) /* ADC short detection threshold 33.0559 V */
#define CONF_ADCSHORTTH__32V90 \
    (0xCE) /* ADC short detection threshold 32.8976 V */
#define CONF_ADCSHORTTH__32V74 \
    (0xCD) /* ADC short detection threshold 32.7393 V */
#define CONF_ADCSHORTTH__32V58 \
    (0xCC) /* ADC short detection threshold 32.581 V */
#define CONF_ADCSHORTTH__32V42 \
    (0xCB) /* ADC short detection threshold 32.4227 V */
#define CONF_ADCSHORTTH__32V26 \
    (0xCA) /* ADC short detection threshold 32.2644 V */
#define CONF_ADCSHORTTH__32V11 \
    (0xC9) /* ADC short detection threshold 32.1061 V */
#define CONF_ADCSHORTTH__31V95 \
    (0xC8) /* ADC short detection threshold 31.9478 V */
#define CONF_ADCSHORTTH__31V79 \
    (0xC7) /* ADC short detection threshold 31.7895 V */
#define CONF_ADCSHORTTH__31V63 \
    (0xC6) /* ADC short detection threshold 31.6312 V */
#define CONF_ADCSHORTTH__31V47 \
    (0xC5) /* ADC short detection threshold 31.4729 V */
#define CONF_ADCSHORTTH__31V31 \
    (0xC4) /* ADC short detection threshold 31.3146 V */
#define CONF_ADCSHORTTH__31V16 \
    (0xC3) /* ADC short detection threshold 31.1563 V */
#define CONF_ADCSHORTTH__31V00 \
    (0xC2) /* ADC short detection threshold 30.998 V */
#define CONF_ADCSHORTTH__30V84 \
    (0xC1) /* ADC short detection threshold 30.8397 V */
#define CONF_ADCSHORTTH__30V68 \
    (0xC0) /* ADC short detection threshold 30.6814 V */
#define CONF_ADCSHORTTH__30V52 \
    (0xBF) /* ADC short detection threshold 30.5231 V */
#define CONF_ADCSHORTTH__30V36 \
    (0xBE) /* ADC short detection threshold 30.3648 V */
#define CONF_ADCSHORTTH__30V21 \
    (0xBD) /* ADC short detection threshold 30.2065 V */
#define CONF_ADCSHORTTH__30V05 \
    (0xBC) /* ADC short detection threshold 30.0482 V */
#define CONF_ADCSHORTTH__29V89 \
    (0xBB) /* ADC short detection threshold 29.8899 V */
#define CONF_ADCSHORTTH__29V73 \
    (0xBA) /* ADC short detection threshold 29.7316 V */
#define CONF_ADCSHORTTH__29V57 \
    (0xB9) /* ADC short detection threshold 29.5733 V */
#define CONF_ADCSHORTTH__29V42 \
    (0xB8) /* ADC short detection threshold 29.415 V */
#define CONF_ADCSHORTTH__29V26 \
    (0xB7) /* ADC short detection threshold 29.2567 V */
#define CONF_ADCSHORTTH__29V10 \
    (0xB6) /* ADC short detection threshold 29.0984 V */
#define CONF_ADCSHORTTH__28V94 \
    (0xB5) /* ADC short detection threshold 28.9401 V */
#define CONF_ADCSHORTTH__28V78 \
    (0xB4) /* ADC short detection threshold 28.7818 V */
#define CONF_ADCSHORTTH__28V62 \
    (0xB3) /* ADC short detection threshold 28.6235 V */
#define CONF_ADCSHORTTH__28V47 \
    (0xB2) /* ADC short detection threshold 28.4652 V */
#define CONF_ADCSHORTTH__28V31 \
    (0xB1) /* ADC short detection threshold 28.3069 V */
#define CONF_ADCSHORTTH__28V15 \
    (0xB0) /* ADC short detection threshold 28.1486 V */
#define CONF_ADCSHORTTH__27V99 \
    (0xAF) /* ADC short detection threshold 27.9903 V */
#define CONF_ADCSHORTTH__27V83 \
    (0xAE) /* ADC short detection threshold 27.832 V */
#define CONF_ADCSHORTTH__27V67 \
    (0xAD) /* ADC short detection threshold 27.6737 V */
#define CONF_ADCSHORTTH__27V52 \
    (0xAC) /* ADC short detection threshold 27.5154 V */
#define CONF_ADCSHORTTH__27V36 \
    (0xAB) /* ADC short detection threshold 27.3571 V */
#define CONF_ADCSHORTTH__27V20 \
    (0xAA) /* ADC short detection threshold 27.1988 V */
#define CONF_ADCSHORTTH__27V04 \
    (0xA9) /* ADC short detection threshold 27.0405 V */
#define CONF_ADCSHORTTH__26V88 \
    (0xA8) /* ADC short detection threshold 26.8822 V */
#define CONF_ADCSHORTTH__26V72 \
    (0xA7) /* ADC short detection threshold 26.7239 V */
#define CONF_ADCSHORTTH__26V57 \
    (0xA6) /* ADC short detection threshold 26.5656 V */
#define CONF_ADCSHORTTH__26V41 \
    (0xA5) /* ADC short detection threshold 26.4073 V */
#define CONF_ADCSHORTTH__26V25 \
    (0xA4) /* ADC short detection threshold 26.249 V */
#define CONF_ADCSHORTTH__26V09 \
    (0xA3) /* ADC short detection threshold 26.0907 V */
#define CONF_ADCSHORTTH__25V93 \
    (0xA2) /* ADC short detection threshold 25.9324 V */
#define CONF_ADCSHORTTH__25V77 \
    (0xA1) /* ADC short detection threshold 25.7741 V */
#define CONF_ADCSHORTTH__25V62 \
    (0xA0) /* ADC short detection threshold 25.6158 V */
#define CONF_ADCSHORTTH__25V46 \
    (0x9F) /* ADC short detection threshold 25.4575 V */
#define CONF_ADCSHORTTH__25V30 \
    (0x9E) /* ADC short detection threshold 25.2992 V */
#define CONF_ADCSHORTTH__25V14 \
    (0x9D) /* ADC short detection threshold 25.1409 V */
#define CONF_ADCSHORTTH__24V98 \
    (0x9C) /* ADC short detection threshold 24.9826 V */
#define CONF_ADCSHORTTH__24V82 \
    (0x9B) /* ADC short detection threshold 24.8243 V */
#define CONF_ADCSHORTTH__24V67 \
    (0x9A) /* ADC short detection threshold 24.666 V */
#define CONF_ADCSHORTTH__24V51 \
    (0x99) /* ADC short detection threshold 24.5077 V */
#define CONF_ADCSHORTTH__24V35 \
    (0x98) /* ADC short detection threshold 24.3494 V */
#define CONF_ADCSHORTTH__24V19 \
    (0x97) /* ADC short detection threshold 24.1911 V */
#define CONF_ADCSHORTTH__24V03 \
    (0x96) /* ADC short detection threshold 24.0328 V */
#define CONF_ADCSHORTTH__23V87 \
    (0x95) /* ADC short detection threshold 23.8745 V */
#define CONF_ADCSHORTTH__23V72 \
    (0x94) /* ADC short detection threshold 23.7162 V */
#define CONF_ADCSHORTTH__23V56 \
    (0x93) /* ADC short detection threshold 23.5579 V */
#define CONF_ADCSHORTTH__23V40 \
    (0x92) /* ADC short detection threshold 23.3996 V */
#define CONF_ADCSHORTTH__23V24 \
    (0x91) /* ADC short detection threshold 23.2413 V */
#define CONF_ADCSHORTTH__23V08 \
    (0x90) /* ADC short detection threshold 23.083 V */
#define CONF_ADCSHORTTH__22V92 \
    (0x8F) /* ADC short detection threshold 22.9247 V */
#define CONF_ADCSHORTTH__22V77 \
    (0x8E) /* ADC short detection threshold 22.7664 V */
#define CONF_ADCSHORTTH__22V61 \
    (0x8D) /* ADC short detection threshold 22.6081 V */
#define CONF_ADCSHORTTH__22V45 \
    (0x8C) /* ADC short detection threshold 22.4498 V */
#define CONF_ADCSHORTTH__22V29 \
    (0x8B) /* ADC short detection threshold 22.2915 V */
#define CONF_ADCSHORTTH__22V13 \
    (0x8A) /* ADC short detection threshold 22.1332 V */
#define CONF_ADCSHORTTH__21V97 \
    (0x89) /* ADC short detection threshold 21.9749 V */
#define CONF_ADCSHORTTH__21V82 \
    (0x88) /* ADC short detection threshold 21.8166 V */
#define CONF_ADCSHORTTH__21V66 \
    (0x87) /* ADC short detection threshold 21.6583 V */
#define CONF_ADCSHORTTH__21V50 \
    (0x86) /* ADC short detection threshold 21.5 V */
#define CONF_ADCSHORTTH__21V34 \
    (0x85) /* ADC short detection threshold 21.3417 V */
#define CONF_ADCSHORTTH__21V18 \
    (0x84) /* ADC short detection threshold 21.1834 V */
#define CONF_ADCSHORTTH__21V03 \
    (0x83) /* ADC short detection threshold 21.0251 V */
#define CONF_ADCSHORTTH__20V87 \
    (0x82) /* ADC short detection threshold 20.8668 V */
#define CONF_ADCSHORTTH__20V71 \
    (0x81) /* ADC short detection threshold 20.7085 V */
#define CONF_ADCSHORTTH__20V55 \
    (0x80) /* ADC short detection threshold 20.5502 V */
#define CONF_ADCSHORTTH__20V39 \
    (0x7F) /* ADC short detection threshold 20.3919 V */
#define CONF_ADCSHORTTH__20V23 \
    (0x7E) /* ADC short detection threshold 20.2336 V */
#define CONF_ADCSHORTTH__20V08 \
    (0x7D) /* ADC short detection threshold 20.0753 V */
#define CONF_ADCSHORTTH__19V92 \
    (0x7C) /* ADC short detection threshold 19.917 V */
#define CONF_ADCSHORTTH__19V76 \
    (0x7B) /* ADC short detection threshold 19.7587 V */
#define CONF_ADCSHORTTH__19V60 \
    (0x7A) /* ADC short detection threshold 19.6004 V */
#define CONF_ADCSHORTTH__19V44 \
    (0x79) /* ADC short detection threshold 19.4421 V */
#define CONF_ADCSHORTTH__19V28 \
    (0x78) /* ADC short detection threshold 19.2838 V */
#define CONF_ADCSHORTTH__19V13 \
    (0x77) /* ADC short detection threshold 19.1255 V */
#define CONF_ADCSHORTTH__18V97 \
    (0x76) /* ADC short detection threshold 18.9672 V */
#define CONF_ADCSHORTTH__18V81 \
    (0x75) /* ADC short detection threshold 18.8089 V */
#define CONF_ADCSHORTTH__18V65 \
    (0x74) /* ADC short detection threshold 18.6506 V */
#define CONF_ADCSHORTTH__18V49 \
    (0x73) /* ADC short detection threshold 18.4923 V */
#define CONF_ADCSHORTTH__18V33 \
    (0x72) /* ADC short detection threshold 18.334 V */
#define CONF_ADCSHORTTH__18V18 \
    (0x71) /* ADC short detection threshold 18.1757 V */
#define CONF_ADCSHORTTH__18V02 \
    (0x70) /* ADC short detection threshold 18.0174 V */
#define CONF_ADCSHORTTH__17V86 \
    (0x6F) /* ADC short detection threshold 17.8591 V */
#define CONF_ADCSHORTTH__17V70 \
    (0x6E) /* ADC short detection threshold 17.7008 V */
#define CONF_ADCSHORTTH__17V54 \
    (0x6D) /* ADC short detection threshold 17.5425 V */
#define CONF_ADCSHORTTH__17V38 \
    (0x6C) /* ADC short detection threshold 17.3842 V */
#define CONF_ADCSHORTTH__17V23 \
    (0x6B) /* ADC short detection threshold 17.2259 V */
#define CONF_ADCSHORTTH__17V07 \
    (0x6A) /* ADC short detection threshold 17.0676 V */
#define CONF_ADCSHORTTH__16V91 \
    (0x69) /* ADC short detection threshold 16.9093 V */
#define CONF_ADCSHORTTH__16V75 \
    (0x68) /* ADC short detection threshold 16.751 V */
#define CONF_ADCSHORTTH__16V59 \
    (0x67) /* ADC short detection threshold 16.5927 V */
#define CONF_ADCSHORTTH__16V43 \
    (0x66) /* ADC short detection threshold 16.4344 V */
#define CONF_ADCSHORTTH__16V28 \
    (0x65) /* ADC short detection threshold 16.2761 V */
#define CONF_ADCSHORTTH__16V12 \
    (0x64) /* ADC short detection threshold 16.1178 V */
#define CONF_ADCSHORTTH__15V96 \
    (0x63) /* ADC short detection threshold 15.9595 V */
#define CONF_ADCSHORTTH__15V80 \
    (0x62) /* ADC short detection threshold 15.8012 V */
#define CONF_ADCSHORTTH__15V64 \
    (0x61) /* ADC short detection threshold 15.6429 V */
#define CONF_ADCSHORTTH__15V48 \
    (0x60) /* ADC short detection threshold 15.4846 V */
#define CONF_ADCSHORTTH__15V33 \
    (0x5F) /* ADC short detection threshold 15.3263 V */
#define CONF_ADCSHORTTH__15V17 \
    (0x5E) /* ADC short detection threshold 15.168 V */
#define CONF_ADCSHORTTH__15V01 \
    (0x5D) /* ADC short detection threshold 15.0097 V */
#define CONF_ADCSHORTTH__14V85 \
    (0x5C) /* ADC short detection threshold 14.8514 V */
#define CONF_ADCSHORTTH__14V69 \
    (0x5B) /* ADC short detection threshold 14.6931 V */
#define CONF_ADCSHORTTH__14V53 \
    (0x5A) /* ADC short detection threshold 14.5348 V */
#define CONF_ADCSHORTTH__14V38 \
    (0x59) /* ADC short detection threshold 14.3765 V */
#define CONF_ADCSHORTTH__14V22 \
    (0x58) /* ADC short detection threshold 14.2182 V */
#define CONF_ADCSHORTTH__14V06 \
    (0x57) /* ADC short detection threshold 14.0599 V */
#define CONF_ADCSHORTTH__13V90 \
    (0x56) /* ADC short detection threshold 13.9016 V */
#define CONF_ADCSHORTTH__13V74 \
    (0x55) /* ADC short detection threshold 13.7433 V */
#define CONF_ADCSHORTTH__13V59 \
    (0x54) /* ADC short detection threshold 13.585 V */
#define CONF_ADCSHORTTH__13V43 \
    (0x53) /* ADC short detection threshold 13.4267 V */
#define CONF_ADCSHORTTH__13V27 \
    (0x52) /* ADC short detection threshold 13.2684 V */
#define CONF_ADCSHORTTH__13V11 \
    (0x51) /* ADC short detection threshold 13.1101 V */
#define CONF_ADCSHORTTH__12V95 \
    (0x50) /* ADC short detection threshold 12.9518 V */
#define CONF_ADCSHORTTH__12V79 \
    (0x4F) /* ADC short detection threshold 12.7935 V */
#define CONF_ADCSHORTTH__12V64 \
    (0x4E) /* ADC short detection threshold 12.6352 V */
#define CONF_ADCSHORTTH__12V48 \
    (0x4D) /* ADC short detection threshold 12.4769 V */
#define CONF_ADCSHORTTH__12V32 \
    (0x4C) /* ADC short detection threshold 12.3186 V */
#define CONF_ADCSHORTTH__12V16 \
    (0x4B) /* ADC short detection threshold 12.1603 V */
#define CONF_ADCSHORTTH__12V00 \
    (0x4A) /* ADC short detection threshold 12.002 V */
#define CONF_ADCSHORTTH__11V84 \
    (0x49) /* ADC short detection threshold 11.8437 V */
#define CONF_ADCSHORTTH__11V69 \
    (0x48) /* ADC short detection threshold 11.6854 V */
#define CONF_ADCSHORTTH__11V53 \
    (0x47) /* ADC short detection threshold 11.5271 V */
#define CONF_ADCSHORTTH__11V37 \
    (0x46) /* ADC short detection threshold 11.3688 V */
#define CONF_ADCSHORTTH__11V21 \
    (0x45) /* ADC short detection threshold 11.2105 V */
#define CONF_ADCSHORTTH__11V05 \
    (0x44) /* ADC short detection threshold 11.0522 V */
#define CONF_ADCSHORTTH__10V89 \
    (0x43) /* ADC short detection threshold 10.8939 V */
#define CONF_ADCSHORTTH__10V74 \
    (0x42) /* ADC short detection threshold 10.7356 V */
#define CONF_ADCSHORTTH__10V58 \
    (0x41) /* ADC short detection threshold 10.5773 V */
#define CONF_ADCSHORTTH__10V42 \
    (0x40) /* ADC short detection threshold 10.419 V */
#define CONF_ADCSHORTTH__10V26 \
    (0x3F) /* ADC short detection threshold 10.2607 V */
#define CONF_ADCSHORTTH__10V10 \
    (0x3E) /* ADC short detection threshold 10.1024 V */
#define CONF_ADCSHORTTH__9V94 \
    (0x3D) /* ADC short detection threshold 9.9441 V */
#define CONF_ADCSHORTTH__9V79 \
    (0x3C) /* ADC short detection threshold 9.7858 V */
#define CONF_ADCSHORTTH__9V63 \
    (0x3B) /* ADC short detection threshold 9.6275 V */
#define CONF_ADCSHORTTH__9V47 \
    (0x3A) /* ADC short detection threshold 9.4692 V */
#define CONF_ADCSHORTTH__9V31 \
    (0x39) /* ADC short detection threshold 9.3109 V */
#define CONF_ADCSHORTTH__9V15 \
    (0x38) /* ADC short detection threshold 9.1526 V */
#define CONF_ADCSHORTTH__8V99 \
    (0x37) /* ADC short detection threshold 8.9943 V */
#define CONF_ADCSHORTTH__8V84 \
    (0x36) /* ADC short detection threshold 8.836 V */
#define CONF_ADCSHORTTH__8V68 \
    (0x35) /* ADC short detection threshold 8.6777 V */
#define CONF_ADCSHORTTH__8V52 \
    (0x34) /* ADC short detection threshold 8.5194 V */
#define CONF_ADCSHORTTH__8V36 \
    (0x33) /* ADC short detection threshold 8.3611 V */
#define CONF_ADCSHORTTH__8V20 \
    (0x32) /* ADC short detection threshold 8.2028 V */
#define CONF_ADCSHORTTH__8V04 \
    (0x31) /* ADC short detection threshold 8.0445 V */
#define CONF_ADCSHORTTH__7V89 \
    (0x30) /* ADC short detection threshold 7.8862 V */
#define CONF_ADCSHORTTH__7V73 \
    (0x2F) /* ADC short detection threshold 7.7279 V */
#define CONF_ADCSHORTTH__7V57 \
    (0x2E) /* ADC short detection threshold 7.5696 V */
#define CONF_ADCSHORTTH__7V41 \
    (0x2D) /* ADC short detection threshold 7.4113 V */
#define CONF_ADCSHORTTH__7V25 \
    (0x2C) /* ADC short detection threshold 7.253 V */
#define CONF_ADCSHORTTH__7V09 \
    (0x2B) /* ADC short detection threshold 7.0947 V */
#define CONF_ADCSHORTTH__6V94 \
    (0x2A) /* ADC short detection threshold 6.9364 V */
#define CONF_ADCSHORTTH__6V78 \
    (0x29) /* ADC short detection threshold 6.7781 V */
#define CONF_ADCSHORTTH__6V62 \
    (0x28) /* ADC short detection threshold 6.6198 V */
#define CONF_ADCSHORTTH__6V46 \
    (0x27) /* ADC short detection threshold 6.4615 V */
#define CONF_ADCSHORTTH__6V30 \
    (0x26) /* ADC short detection threshold 6.3032 V */
#define CONF_ADCSHORTTH__6V14 \
    (0x25) /* ADC short detection threshold 6.1449 V */
#define CONF_ADCSHORTTH__5V99 \
    (0x24) /* ADC short detection threshold 5.9866 V */
#define CONF_ADCSHORTTH__5V83 \
    (0x23) /* ADC short detection threshold 5.8283 V */
#define CONF_ADCSHORTTH__5V67 (0x22) /* ADC short detection threshold 5.67 V */
#define CONF_ADCSHORTTH__5V51 \
    (0x21) /* ADC short detection threshold 5.5117 V */
#define CONF_ADCSHORTTH__5V35 \
    (0x20) /* ADC short detection threshold 5.3534 V */
#define CONF_ADCSHORTTH__5V20 \
    (0x1F) /* ADC short detection threshold 5.1951 V */
#define CONF_ADCSHORTTH__5V04 \
    (0x1E) /* ADC short detection threshold 5.0368 V */
#define CONF_ADCSHORTTH__4V88 \
    (0x1D) /* ADC short detection threshold 4.8785 V */
#define CONF_ADCSHORTTH__4V72 \
    (0x1C) /* ADC short detection threshold 4.7202 V */
#define CONF_ADCSHORTTH__4V56 \
    (0x1B) /* ADC short detection threshold 4.5619 V */
#define CONF_ADCSHORTTH__4V40 \
    (0x1A) /* ADC short detection threshold 4.4036 V */
#define CONF_ADCSHORTTH__4V25 \
    (0x19) /* ADC short detection threshold 4.2453 V */
#define CONF_ADCSHORTTH__4V09 \
    (0x18) /* ADC short detection threshold 4.087 V */
#define CONF_ADCSHORTTH__3V93 \
    (0x17) /* ADC short detection threshold 3.9287 V */
#define CONF_ADCSHORTTH__3V77 \
    (0x16) /* ADC short detection threshold 3.7704 V */
#define CONF_ADCSHORTTH__3V61 \
    (0x15) /* ADC short detection threshold 3.6121 V */
#define CONF_ADCSHORTTH__3V45 \
    (0x14) /* ADC short detection threshold 3.4538 V */
#define CONF_ADCSHORTTH__3V30 \
    (0x13) /* ADC short detection threshold 3.2955 V */
#define CONF_ADCSHORTTH__3V14 \
    (0x12) /* ADC short detection threshold 3.1372 V */
#define CONF_ADCSHORTTH__2V98 \
    (0x11) /* ADC short detection threshold 2.9789 V */
#define CONF_ADCSHORTTH__2V82 \
    (0x10) /* ADC short detection threshold 2.8206 V */
#define CONF_ADCSHORTTH__2V66 \
    (0x0F) /* ADC short detection threshold 2.6623 V */
#define CONF_ADCSHORTTH__2V50 \
    (0x0E) /* ADC short detection threshold 2.504 V */
#define CONF_ADCSHORTTH__2V35 \
    (0x0D) /* ADC short detection threshold 2.3457 V */
#define CONF_ADCSHORTTH__2V19 \
    (0x0C) /* ADC short detection threshold 2.1874 V */
#define CONF_ADCSHORTTH__2V03 \
    (0x0B) /* ADC short detection threshold 2.0291 V */
#define CONF_ADCSHORTTH__1V87 \
    (0x0A) /* ADC short detection threshold 1.8708 V */
#define CONF_ADCSHORTTH__1V71 \
    (0x09) /* ADC short detection threshold 1.7125 V */
#define CONF_ADCSHORTTH__1V55 \
    (0x08) /* ADC short detection threshold 1.5542 V */
#define CONF_ADCSHORTTH__1V40 \
    (0x07) /* ADC short detection threshold 1.3959 V */
#define CONF_ADCSHORTTH__1V24 \
    (0x06) /* ADC short detection threshold 1.2376 V */
#define CONF_ADCSHORTTH__1V08 \
    (0x05) /* ADC short detection threshold 1.0793 V */
#define CONF_ADCSHORTTH__0V92 \
    (0x04) /* ADC short detection threshold 0.921 V */
#define CONF_ADCSHORTTH__0V76 \
    (0x03) /* ADC short detection threshold 0.7627 V */
#define CONF_ADCSHORTTH__0V60 \
    (0x02) /* ADC short detection threshold 0.6044 V */
#define CONF_ADCSHORTTH__0V45 \
    (0x01) /* ADC short detection threshold 0.4461 V */
#define CONF_ADCSHORTTH__0V29 \
    (0x00) /* ADC short detection threshold 0.2878 V */

/* CLR register */
#define CONF_FORCEFS__1 \
    (0x20) /* Force device into Fail-safe state from normal state */
#define CONF_FORCEFS__0 (0x00) /*  */
#define CLR_REG__1 \
    (0x10) /* Clear device register settings to default values */
#define CLR_REG__0 (0x00)       /*  */
#define CONF_FORCEERR__1 (0x08) /* Force error setting register */
#define CONF_FORCEERR__0 (0x00) /*  */
#define CLR_FS__1 \
    (0x04) /* Force the device out of Fail-safe states to normal state */
#define CLR_FS__0 (0x00)    /*  */
#define CLR_FAULT__1 (0x02) /* Clear all fault flags */
#define CLR_FAULT__0 (0x00) /*  */
#define CLR_POR__1 (0x01)   /* Clear POR flag */
#define CLR_POR__0 (0x00)   /*  */
#define CLRREG (0x10) /* Clear device register settings to default values */
#define CLRFS \
    (0x04) /* Force the device out of Fail-safe states to normal state */
#define CLRFAULT (0x02) /* Clear all fault flags */
#define CLRPOR (0x01)   /* Clear POR flag */

/* CONF_LOCK register */
#define CONF_CLRLOCK__1 (0x08) /* CLR register write-protected enabled */
#define CONF_CLRLOCK__0 (0x00) /* CLR register write-protect disabled */
#define CONF_CONFLOCK__1 \
    (0x04) /* Configuration setting register write-protected enabled */
#define CONF_CONFLOCK__0 \
    (0x00) /* Configuration setting register write-protect disabled */
#define CONF_IOUTLOCK__1 \
    (0x02) /* Output current setting register write-protected enabled */
#define CONF_IOUTLOCK__0 \
    (0x00) /* Output current setting register write-protect disabled */
#define CONF_PWMLOCK__1 (0x01) /* PWM Register write-protected enabled */
#define CONF_PWMLOCK__0 (0x00) /* PWM Register write-protect disabled */
#define CLRLOCK (0x08)         /* CLR register write-protected */
#define CONFLOCK (0x04) /* Configuration setting register write-protected */
#define IOUTLOCK (0x02) /* Output current setting register write-protected */
#define PWMLOCK (0x01)  /* PWM Register write-protected */

/* CONF_MISC6 register */
#define CONF_STAYINEEP__1 (0x80) /* EEPROM mode enabled */
#define CONF_STAYINEEP__0 (0x00) /* EEPROM mode disabled */
#define CONF_EEPREADBACK__1 \
    (0x40) /* Allow EEPROM to overwrite configuration registers */
#define CONF_EEPREADBACK__0 (0x00) /*  */
#define CONF_ADCCH__OUT11 (0x1B)   /* Output voltage channel 11 */
#define CONF_ADCCH__OUT10 (0x1A)   /* Output voltage channel 10 */
#define CONF_ADCCH__OUT9 (0x19)    /* Output voltage channel 9 */
#define CONF_ADCCH__OUT8 (0x18)    /* Output voltage channel 8 */
#define CONF_ADCCH__OUT7 (0x17)    /* Output voltage channel 7 */
#define CONF_ADCCH__OUT6 (0x16)    /* Output voltage channel 6 */
#define CONF_ADCCH__OUT5 (0x15)    /* Output voltage channel 5 */
#define CONF_ADCCH__OUT4 (0x14)    /* Output voltage channel 4 */
#define CONF_ADCCH__OUT3 (0x13)    /* Output voltage channel 3 */
#define CONF_ADCCH__OUT2 (0x12)    /* Output voltage channel 2 */
#define CONF_ADCCH__OUT1 (0x11)    /* Output voltage channel 1 */
#define CONF_ADCCH__OUT0 (0x10)    /* Output voltage channel 0 */
#define CONF_ADCCH__MAXOUT (0x05)  /* Maximum channel output voltage */
#define CONF_ADCCH__IREF (0x04)    /* Reference current */
#define CONF_ADCCH__TEMPSNS (0x03) /* Internal temperature sensor */
#define CONF_ADCCH__VLDO (0x02)    /* 5V LDO output voltage */
#define CONF_ADCCH__SUPPLY (0x01)  /* Supply voltage */
#define CONF_ADCCH__REF (0x00)     /* Reference voltage */
#define ADCCHSEL__OUT11 (0x1B)     /* Output voltage channel 11 */
#define ADCCHSEL__OUT10 (0x1A)     /* Output voltage channel 10 */
#define ADCCHSEL__OUT9 (0x19)      /* Output voltage channel 9 */
#define ADCCHSEL__OUT8 (0x18)      /* Output voltage channel 8 */
#define ADCCHSEL__OUT7 (0x17)      /* Output voltage channel 7 */
#define ADCCHSEL__OUT6 (0x16)      /* Output voltage channel 6 */
#define ADCCHSEL__OUT5 (0x15)      /* Output voltage channel 5 */
#define ADCCHSEL__OUT4 (0x14)      /* Output voltage channel 4 */
#define ADCCHSEL__OUT3 (0x13)      /* Output voltage channel 3 */
#define ADCCHSEL__OUT2 (0x12)      /* Output voltage channel 2 */
#define ADCCHSEL__OUT1 (0x11)      /* Output voltage channel 1 */
#define ADCCHSEL__OUT0 (0x10)      /* Output voltage channel 0 */
#define ADCCHSEL__MAXOUT (0x05)    /* Maximum channel output voltage */
#define ADCCHSEL__IREF (0x04)      /* Reference current */
#define ADCCHSEL__TEMPSNS (0x03)   /* Internal temperature sensor */
#define ADCCHSEL__VLDO (0x02)      /* 5V LDO output voltage */
#define ADCCHSEL__SUPPLY (0x01)    /* Supply voltage */
#define ADCCHSEL__REF (0x00)       /* Reference voltage */

/* CONF_MISC7 register */
#define CONF_EXTCLK__1 (0x20) /* Use external clock source for PWM generator */
#define CONF_EXTCLK__0 (0x00) /* Use internal clock source for PWM generator */
#define CONF_SHAREPWM__1 \
    (0x10) /* All channel PWM dutycycle is the same as CH0 */
#define CONF_SHAREPWM__0 \
    (0x00) /* All channel PWM dutycycle is set independently */
#define CONF_READSHADOW__1 (0x02) /* From EEPROM shadow registers */
#define CONF_READSHADOW__0 (0x00) /* From EEPROM */
#define CONF_EEPMODE__1 (0x01)    /* Enable EEPMODE Programming State */
#define CONF_EEPMODE__0 (0x00)    /* Disable EEPMODE Programming State */

/* CONF_MISC8 register */
#define CONF_MASKREF__1 \
    (0x80) /* Reference fault will not be reported to ERR output */
#define CONF_MASKREF__0 \
    (0x00) /* Reference fault will be reported to ERR output */
#define CONF_MASKCRC__1 \
    (0x40) /* CRC fault will not be reported to ERR output */
#define CONF_MASKCRC__0 (0x00) /* CRC fault will be reported to ERR output */
#define CONF_MASKSHORT__1 \
    (0x20) /* Short-circuit fault will not be reported to ERR output */
#define CONF_MASKSHORT__0 \
    (0x00) /* Short-circuit fault will be reported to ERR output */
#define CONF_MASKOPEN__1 \
    (0x10) /* Open-circuit fault will not be reported to ERR output */
#define CONF_MASKOPEN__0 \
    (0x00) /* Open-circuit fault will be reported to ERR output */
#define CONF_MASKTSD__1 \
    (0x08) /* TSD Fault masked to ERR output, output will be shutdown */
#define CONF_MASKTSD__0 (0x00) /* TSD Fault unmasked to ERR output */
#define CONF_EEPPROG__1 (0x04) /* EEPROM burning start in EEPROM mode only */
#define CONF_EEPPROG__0 (0x00) /*  */
#define CONF_SSSTART__1 (0x02) /* Single LED Short diagnostics start */
#define CONF_SSSTART__0 (0x00) /*  */
#define CONF_INVDIAGSTART__1 (0x01) /* Invisible Diagnostics start */
#define CONF_INVDIAGSTART__0 (0x00) /*  */
#define EEPPROG (0x04) /* EEPROM burning start in EEPROM mode only */
#define MASKREF_1 \
    (0x80) /* Reference fault will not be reported to ERR output */
#define MASKREF_0 (0x00)  /* Reference fault will be reported to ERR output */
#define MASKCRC__1 (0x40) /* CRC fault will not be reported to ERR output */
#define MASKCRC__0 (0x00) /* CRC fault will be reported to ERR output */
#define MASKSHORT_1 \
    (0x20) /* Short-circuit fault will not be reported to ERR output */
#define MASKSHORT_0 \
    (0x00) /* Short-circuit fault will be reported to ERR output */
#define MASKOPEN_1 \
    (0x10) /* Open-circuit fault will not be reported to ERR output */
#define MASKOPEN_0 \
    (0x00) /* Open-circuit fault will be reported to ERR output */
#define MASKTSD_1 \
    (0x08) /* TSD Fault masked to ERR output, output will be shutdown */
#define MASKTSD_0 (0x00) /* TSD Fault unmasked to ERR output */
#define MASKREF__1 \
    (0x80) /* Reference fault will not be reported to ERR output */
#define MASKREF__0 (0x00) /* Reference fault will be reported to ERR output */
#define MASKCRC__1 (0x40) /* CRC fault will not be reported to ERR output */
#define MASKCRC__0 (0x00) /* CRC fault will be reported to ERR output */
#define MASKSHORT__1 \
    (0x20) /* Short-circuit fault will not be reported to ERR output */
#define MASKSHORT__0 \
    (0x00) /* Short-circuit fault will be reported to ERR output */
#define MASKOPEN__1 \
    (0x10) /* Open-circuit fault will not be reported to ERR output */
#define MASKOPEN__0 \
    (0x00) /* Open-circuit fault will be reported to ERR output */
#define MASKTSD__1 \
    (0x08) /* TSD Fault masked to ERR output, output will be shutdown */
#define MASKTSD__0 (0x00) /* TSD Fault unmasked to ERR output */

/* CONF_MISC9 register */
#define CONF_EEPGATE__FF (0xFF) /* FF */
#define CONF_EEPGATE__FE (0xFE) /* FE */
#define CONF_EEPGATE__FD (0xFD) /* FD */
#define CONF_EEPGATE__FC (0xFC) /* FC */
#define CONF_EEPGATE__FB (0xFB) /* FB */
#define CONF_EEPGATE__FA (0xFA) /* FA */
#define CONF_EEPGATE__F9 (0xF9) /* F9 */
#define CONF_EEPGATE__F8 (0xF8) /* F8 */
#define CONF_EEPGATE__F7 (0xF7) /* F7 */
#define CONF_EEPGATE__F6 (0xF6) /* F6 */
#define CONF_EEPGATE__F5 (0xF5) /* F5 */
#define CONF_EEPGATE__F4 (0xF4) /* F4 */
#define CONF_EEPGATE__F3 (0xF3) /* F3 */
#define CONF_EEPGATE__F2 (0xF2) /* F2 */
#define CONF_EEPGATE__F1 (0xF1) /* F1 */
#define CONF_EEPGATE__F0 (0xF0) /* F0 */
#define CONF_EEPGATE__EF (0xEF) /* EF */
#define CONF_EEPGATE__EE (0xEE) /* EE */
#define CONF_EEPGATE__ED (0xED) /* ED */
#define CONF_EEPGATE__EC (0xEC) /* EC */
#define CONF_EEPGATE__EB (0xEB) /* EB */
#define CONF_EEPGATE__EA (0xEA) /* EA */
#define CONF_EEPGATE__E9 (0xE9) /* E9 */
#define CONF_EEPGATE__E8 (0xE8) /* E8 */
#define CONF_EEPGATE__E7 (0xE7) /* E7 */
#define CONF_EEPGATE__E6 (0xE6) /* E6 */
#define CONF_EEPGATE__E5 (0xE5) /* E5 */
#define CONF_EEPGATE__E4 (0xE4) /* E4 */
#define CONF_EEPGATE__E3 (0xE3) /* E3 */
#define CONF_EEPGATE__E2 (0xE2) /* E2 */
#define CONF_EEPGATE__E1 (0xE1) /* E1 */
#define CONF_EEPGATE__E0 (0xE0) /* E0 */
#define CONF_EEPGATE__DF (0xDF) /* DF */
#define CONF_EEPGATE__DE (0xDE) /* DE */
#define CONF_EEPGATE__DD (0xDD) /* DD */
#define CONF_EEPGATE__DC (0xDC) /* DC */
#define CONF_EEPGATE__DB (0xDB) /* DB */
#define CONF_EEPGATE__DA (0xDA) /* DA */
#define CONF_EEPGATE__D9 (0xD9) /* D9 */
#define CONF_EEPGATE__D8 (0xD8) /* D8 */
#define CONF_EEPGATE__D7 (0xD7) /* D7 */
#define CONF_EEPGATE__D6 (0xD6) /* D6 */
#define CONF_EEPGATE__D5 (0xD5) /* D5 */
#define CONF_EEPGATE__D4 (0xD4) /* D4 */
#define CONF_EEPGATE__D3 (0xD3) /* D3 */
#define CONF_EEPGATE__D2 (0xD2) /* D2 */
#define CONF_EEPGATE__D1 (0xD1) /* D1 */
#define CONF_EEPGATE__D0 (0xD0) /* D0 */
#define CONF_EEPGATE__CF (0xCF) /* CF */
#define CONF_EEPGATE__CE (0xCE) /* CE */
#define CONF_EEPGATE__CD (0xCD) /* CD */
#define CONF_EEPGATE__CC (0xCC) /* CC */
#define CONF_EEPGATE__CB (0xCB) /* CB */
#define CONF_EEPGATE__CA (0xCA) /* CA */
#define CONF_EEPGATE__C9 (0xC9) /* C9 */
#define CONF_EEPGATE__C8 (0xC8) /* C8 */
#define CONF_EEPGATE__C7 (0xC7) /* C7 */
#define CONF_EEPGATE__C6 (0xC6) /* C6 */
#define CONF_EEPGATE__C5 (0xC5) /* C5 */
#define CONF_EEPGATE__C4 (0xC4) /* C4 */
#define CONF_EEPGATE__C3 (0xC3) /* C3 */
#define CONF_EEPGATE__C2 (0xC2) /* C2 */
#define CONF_EEPGATE__C1 (0xC1) /* C1 */
#define CONF_EEPGATE__C0 (0xC0) /* C0 */
#define CONF_EEPGATE__BF (0xBF) /* BF */
#define CONF_EEPGATE__BE (0xBE) /* BE */
#define CONF_EEPGATE__BD (0xBD) /* BD */
#define CONF_EEPGATE__BC (0xBC) /* BC */
#define CONF_EEPGATE__BB (0xBB) /* BB */
#define CONF_EEPGATE__BA (0xBA) /* BA */
#define CONF_EEPGATE__B9 (0xB9) /* B9 */
#define CONF_EEPGATE__B8 (0xB8) /* B8 */
#define CONF_EEPGATE__B7 (0xB7) /* B7 */
#define CONF_EEPGATE__B6 (0xB6) /* B6 */
#define CONF_EEPGATE__B5 (0xB5) /* B5 */
#define CONF_EEPGATE__B4 (0xB4) /* B4 */
#define CONF_EEPGATE__B3 (0xB3) /* B3 */
#define CONF_EEPGATE__B2 (0xB2) /* B2 */
#define CONF_EEPGATE__B1 (0xB1) /* B1 */
#define CONF_EEPGATE__B0 (0xB0) /* B0 */
#define CONF_EEPGATE__AF (0xAF) /* AF */
#define CONF_EEPGATE__AE (0xAE) /* AE */
#define CONF_EEPGATE__AD (0xAD) /* AD */
#define CONF_EEPGATE__AC (0xAC) /* AC */
#define CONF_EEPGATE__AB (0xAB) /* AB */
#define CONF_EEPGATE__AA (0xAA) /* AA */
#define CONF_EEPGATE__A9 (0xA9) /* A9 */
#define CONF_EEPGATE__A8 (0xA8) /* A8 */
#define CONF_EEPGATE__A7 (0xA7) /* A7 */
#define CONF_EEPGATE__A6 (0xA6) /* A6 */
#define CONF_EEPGATE__A5 (0xA5) /* A5 */
#define CONF_EEPGATE__A4 (0xA4) /* A4 */
#define CONF_EEPGATE__A3 (0xA3) /* A3 */
#define CONF_EEPGATE__A2 (0xA2) /* A2 */
#define CONF_EEPGATE__A1 (0xA1) /* A1 */
#define CONF_EEPGATE__A0 (0xA0) /* A0 */
#define CONF_EEPGATE__9F (0x9F) /* 9F */
#define CONF_EEPGATE__9E (0x9E) /* 9E */
#define CONF_EEPGATE__9D (0x9D) /* 9D */
#define CONF_EEPGATE__9C (0x9C) /* 9C */
#define CONF_EEPGATE__9B (0x9B) /* 9B */
#define CONF_EEPGATE__9A (0x9A) /* 9A */
#define CONF_EEPGATE__99 (0x99) /* 99 */
#define CONF_EEPGATE__98 (0x98) /* 98 */
#define CONF_EEPGATE__97 (0x97) /* 97 */
#define CONF_EEPGATE__96 (0x96) /* 96 */
#define CONF_EEPGATE__95 (0x95) /* 95 */
#define CONF_EEPGATE__94 (0x94) /* 94 */
#define CONF_EEPGATE__93 (0x93) /* 93 */
#define CONF_EEPGATE__92 (0x92) /* 92 */
#define CONF_EEPGATE__91 (0x91) /* 91 */
#define CONF_EEPGATE__90 (0x90) /* 90 */
#define CONF_EEPGATE__8F (0x8F) /* 8F */
#define CONF_EEPGATE__8E (0x8E) /* 8E */
#define CONF_EEPGATE__8D (0x8D) /* 8D */
#define CONF_EEPGATE__8C (0x8C) /* 8C */
#define CONF_EEPGATE__8B (0x8B) /* 8B */
#define CONF_EEPGATE__8A (0x8A) /* 8A */
#define CONF_EEPGATE__89 (0x89) /* 89 */
#define CONF_EEPGATE__88 (0x88) /* 88 */
#define CONF_EEPGATE__87 (0x87) /* 87 */
#define CONF_EEPGATE__86 (0x86) /* 86 */
#define CONF_EEPGATE__85 (0x85) /* 85 */
#define CONF_EEPGATE__84 (0x84) /* 84 */
#define CONF_EEPGATE__83 (0x83) /* 83 */
#define CONF_EEPGATE__82 (0x82) /* 82 */
#define CONF_EEPGATE__81 (0x81) /* 81 */
#define CONF_EEPGATE__80 (0x80) /* 80 */
#define CONF_EEPGATE__7F (0x7F) /* 7F */
#define CONF_EEPGATE__7E (0x7E) /* 7E */
#define CONF_EEPGATE__7D (0x7D) /* 7D */
#define CONF_EEPGATE__7C (0x7C) /* 7C */
#define CONF_EEPGATE__7B (0x7B) /* 7B */
#define CONF_EEPGATE__7A (0x7A) /* 7A */
#define CONF_EEPGATE__79 (0x79) /* 79 */
#define CONF_EEPGATE__78 (0x78) /* 78 */
#define CONF_EEPGATE__77 (0x77) /* 77 */
#define CONF_EEPGATE__76 (0x76) /* 76 */
#define CONF_EEPGATE__75 (0x75) /* 75 */
#define CONF_EEPGATE__74 (0x74) /* 74 */
#define CONF_EEPGATE__73 (0x73) /* 73 */
#define CONF_EEPGATE__72 (0x72) /* 72 */
#define CONF_EEPGATE__71 (0x71) /* 71 */
#define CONF_EEPGATE__70 (0x70) /* 70 */
#define CONF_EEPGATE__6F (0x6F) /* 6F */
#define CONF_EEPGATE__6E (0x6E) /* 6E */
#define CONF_EEPGATE__6D (0x6D) /* 6D */
#define CONF_EEPGATE__6C (0x6C) /* 6C */
#define CONF_EEPGATE__6B (0x6B) /* 6B */
#define CONF_EEPGATE__6A (0x6A) /* 6A */
#define CONF_EEPGATE__69 (0x69) /* 69 */
#define CONF_EEPGATE__68 (0x68) /* 68 */
#define CONF_EEPGATE__67 (0x67) /* 67 */
#define CONF_EEPGATE__66 (0x66) /* 66 */
#define CONF_EEPGATE__65 (0x65) /* 65 */
#define CONF_EEPGATE__64 (0x64) /* 64 */
#define CONF_EEPGATE__63 (0x63) /* 63 */
#define CONF_EEPGATE__62 (0x62) /* 62 */
#define CONF_EEPGATE__61 (0x61) /* 61 */
#define CONF_EEPGATE__60 (0x60) /* 60 */
#define CONF_EEPGATE__5F (0x5F) /* 5F */
#define CONF_EEPGATE__5E (0x5E) /* 5E */
#define CONF_EEPGATE__5D (0x5D) /* 5D */
#define CONF_EEPGATE__5C (0x5C) /* 5C */
#define CONF_EEPGATE__5B (0x5B) /* 5B */
#define CONF_EEPGATE__5A (0x5A) /* 5A */
#define CONF_EEPGATE__59 (0x59) /* 59 */
#define CONF_EEPGATE__58 (0x58) /* 58 */
#define CONF_EEPGATE__57 (0x57) /* 57 */
#define CONF_EEPGATE__56 (0x56) /* 56 */
#define CONF_EEPGATE__55 (0x55) /* 55 */
#define CONF_EEPGATE__54 (0x54) /* 54 */
#define CONF_EEPGATE__53 (0x53) /* 53 */
#define CONF_EEPGATE__52 (0x52) /* 52 */
#define CONF_EEPGATE__51 (0x51) /* 51 */
#define CONF_EEPGATE__50 (0x50) /* 50 */
#define CONF_EEPGATE__4F (0x4F) /* 4F */
#define CONF_EEPGATE__4E (0x4E) /* 4E */
#define CONF_EEPGATE__4D (0x4D) /* 4D */
#define CONF_EEPGATE__4C (0x4C) /* 4C */
#define CONF_EEPGATE__4B (0x4B) /* 4B */
#define CONF_EEPGATE__4A (0x4A) /* 4A */
#define CONF_EEPGATE__49 (0x49) /* 49 */
#define CONF_EEPGATE__48 (0x48) /* 48 */
#define CONF_EEPGATE__47 (0x47) /* 47 */
#define CONF_EEPGATE__46 (0x46) /* 46 */
#define CONF_EEPGATE__45 (0x45) /* 45 */
#define CONF_EEPGATE__44 (0x44) /* 44 */
#define CONF_EEPGATE__43 (0x43) /* 43 */
#define CONF_EEPGATE__42 (0x42) /* 42 */
#define CONF_EEPGATE__41 (0x41) /* 41 */
#define CONF_EEPGATE__40 (0x40) /* 40 */
#define CONF_EEPGATE__3F (0x3F) /* 3F */
#define CONF_EEPGATE__3E (0x3E) /* 3E */
#define CONF_EEPGATE__3D (0x3D) /* 3D */
#define CONF_EEPGATE__3C (0x3C) /* 3C */
#define CONF_EEPGATE__3B (0x3B) /* 3B */
#define CONF_EEPGATE__3A (0x3A) /* 3A */
#define CONF_EEPGATE__39 (0x39) /* 39 */
#define CONF_EEPGATE__38 (0x38) /* 38 */
#define CONF_EEPGATE__37 (0x37) /* 37 */
#define CONF_EEPGATE__36 (0x36) /* 36 */
#define CONF_EEPGATE__35 (0x35) /* 35 */
#define CONF_EEPGATE__34 (0x34) /* 34 */
#define CONF_EEPGATE__33 (0x33) /* 33 */
#define CONF_EEPGATE__32 (0x32) /* 32 */
#define CONF_EEPGATE__31 (0x31) /* 31 */
#define CONF_EEPGATE__30 (0x30) /* 30 */
#define CONF_EEPGATE__2F (0x2F) /* 2F */
#define CONF_EEPGATE__2E (0x2E) /* 2E */
#define CONF_EEPGATE__2D (0x2D) /* 2D */
#define CONF_EEPGATE__2C (0x2C) /* 2C */
#define CONF_EEPGATE__2B (0x2B) /* 2B */
#define CONF_EEPGATE__2A (0x2A) /* 2A */
#define CONF_EEPGATE__29 (0x29) /* 29 */
#define CONF_EEPGATE__28 (0x28) /* 28 */
#define CONF_EEPGATE__27 (0x27) /* 27 */
#define CONF_EEPGATE__26 (0x26) /* 26 */
#define CONF_EEPGATE__25 (0x25) /* 25 */
#define CONF_EEPGATE__24 (0x24) /* 24 */
#define CONF_EEPGATE__23 (0x23) /* 23 */
#define CONF_EEPGATE__22 (0x22) /* 22 */
#define CONF_EEPGATE__21 (0x21) /* 21 */
#define CONF_EEPGATE__20 (0x20) /* 20 */
#define CONF_EEPGATE__1F (0x1F) /* 1F */
#define CONF_EEPGATE__1E (0x1E) /* 1E */
#define CONF_EEPGATE__1D (0x1D) /* 1D */
#define CONF_EEPGATE__1C (0x1C) /* 1C */
#define CONF_EEPGATE__1B (0x1B) /* 1B */
#define CONF_EEPGATE__1A (0x1A) /* 1A */
#define CONF_EEPGATE__19 (0x19) /* 19 */
#define CONF_EEPGATE__18 (0x18) /* 18 */
#define CONF_EEPGATE__17 (0x17) /* 17 */
#define CONF_EEPGATE__16 (0x16) /* 16 */
#define CONF_EEPGATE__15 (0x15) /* 15 */
#define CONF_EEPGATE__14 (0x14) /* 14 */
#define CONF_EEPGATE__13 (0x13) /* 13 */
#define CONF_EEPGATE__12 (0x12) /* 12 */
#define CONF_EEPGATE__11 (0x11) /* 11 */
#define CONF_EEPGATE__10 (0x10) /* 10 */
#define CONF_EEPGATE__0F (0x0F) /* 0F */
#define CONF_EEPGATE__0E (0x0E) /* 0E */
#define CONF_EEPGATE__0D (0x0D) /* 0D */
#define CONF_EEPGATE__0C (0x0C) /* 0C */
#define CONF_EEPGATE__0B (0x0B) /* 0B */
#define CONF_EEPGATE__0A (0x0A) /* 0A */
#define CONF_EEPGATE__09 (0x09) /* 09 */
#define CONF_EEPGATE__08 (0x08) /* 08 */
#define CONF_EEPGATE__07 (0x07) /* 07 */
#define CONF_EEPGATE__06 (0x06) /* 06 */
#define CONF_EEPGATE__05 (0x05) /* 05 */
#define CONF_EEPGATE__04 (0x04) /* 04 */
#define CONF_EEPGATE__03 (0x03) /* 03 */
#define CONF_EEPGATE__02 (0x02) /* 02 */
#define CONF_EEPGATE__01 (0x01) /* 01 */
#define CONF_EEPGATE__00 (0x00) /* 00 */

/* FLAG0 register */
#define FLAG_REF (0x40)    /* Reference fault flag */
#define FLAG_FS (0x20)     /* Fail-safe flag */
#define FLAG_OUT (0x10)    /* Output fault flag */
#define FLAG_PRETSD (0x08) /* Overtemperature pre-shut down flag */
#define FLAG_TSD (0x04)    /* Overtemperature shut down flag */
#define FLAG_POR (0x02)    /* Power-on-reset flag */
#define FLAG_ERR (0x01)    /* Error output flag */

/* FLAG1 register */
#define FLAG_EXTFS (0x20)     /* FS pin voltage indicator */
#define FLAG_PROGREADY (0x10) /* EEPROM burning completion flag */
#define FLAG_ADCLOWSUP (0x08) /* Flag for low supply voltage detection */
#define FLAG_LOWSUP (0x08)    /* Flag for low supply voltage detection */
#define FLAG_ADCDONE (0x04)   /* Flag for ADC conversion completition */
#define FLAG_ODREADY (0x02)   /* Flag for on-demand diagnostics */
#define FLAG_EEPCRC (0x01)    /* Flag for EEPROM CRC check failure */

/* FLAG2 register */
#define ADC_SUPPLY (0xFF) /* ADC conversion output register for supply */

/* FLAG3 register */
#define ADC_OUT (0xFF) /* ADC Conversion output register */

/* FLAG4 register */
#define FLAG_ODDIAGCH7 \
    (0x80) /* Channel 7 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH6 \
    (0x40) /* Channel 6 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH5 \
    (0x20) /* Channel 5 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH4 \
    (0x10) /* Channel 4 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH3 \
    (0x08) /* Channel 3 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH2 \
    (0x04) /* Channel 2 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH1 \
    (0x02) /* Channel 1 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH0 \
    (0x01) /* Channel 0 on-demand diagnostics fault flag bit */

/* FLAG5 register */
#define FLAG_ODDIAGCH11 \
    (0x08) /* Channel 11 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH10 \
    (0x04) /* Channel 10 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH9 \
    (0x02) /* Channel 9 on-demand diagnostics fault flag bit */
#define FLAG_ODDIAGCH8 \
    (0x01) /* Channel 8 on-demand diagnostics fault flag bit */

/* FLAG7 register */
#define CALC_EEPCRC (0xFF) /* EEPROM CRC reference */

/* FLAG8 register */
#define CALC_CONFCRC (0xFF) /* Calculated CRC result for all CONFx registers */

/* FLAG11 register */
#define FLAG_OPENCH7 (0x80) /* Channel 7 open-circuit fault flag bit */
#define FLAG_OPENCH6 (0x40) /* Channel 6 open-circuit fault flag bit */
#define FLAG_OPENCH5 (0x20) /* Channel 5 open-circuit fault flag bit */
#define FLAG_OPENCH4 (0x10) /* Channel 4 open-circuit fault flag bit */
#define FLAG_OPENCH3 (0x08) /* Channel 3 open-circuit fault flag bit */
#define FLAG_OPENCH2 (0x04) /* Channel 2 open-circuit fault flag bit */
#define FLAG_OPENCH1 (0x02) /* Channel 1 open-circuit fault flag bit */
#define FLAG_OPENCH0 (0x01) /* Channel 0 open-circuit fault flag bit */

/* FLAG12 register */
#define FLAG_OPENCH11 (0x08) /* Channel 11 open-circuit fault flag bit */
#define FLAG_OPENCH10 (0x04) /* Channel 10 open-circuit fault flag bit */
#define FLAG_OPENCH9 (0x02)  /* Channel 9 open-circuit fault flag bit */
#define FLAG_OPENCH8 (0x01)  /* Channel 8 open-circuit fault flag bit */

/* FLAG13 register */
#define FLAG_SHORTCH7 (0x80) /* Channel 7 short-circuit fault flag bit */
#define FLAG_SHORTCH6 (0x40) /* Channel 6 short-circuit fault flag bit */
#define FLAG_SHORTCH5 (0x20) /* Channel 5 short-circuit fault flag bit */
#define FLAG_SHORTCH4 (0x10) /* Channel 4 short-circuit fault flag bit */
#define FLAG_SHORTCH3 (0x08) /* Channel 3 short-circuit fault flag bit */
#define FLAG_SHORTCH2 (0x04) /* Channel 2 short-circuit fault flag bit */
#define FLAG_SHORTCH1 (0x02) /* Channel 1 short-circuit fault flag bit */
#define FLAG_SHORTCH0 (0x01) /* Channel 0 short-circuit fault flag bit */

/* FLAG14 register */
#define FLAG_SHORTCH11 (0x08) /* Channel 11 short-circuit fault flag bit */
#define FLAG_SHORTCH10 (0x04) /* Channel 10 short-circuit fault flag bit */
#define FLAG_SHORTCH9 (0x02)  /* Channel 9 short-circuit fault flag bit */
#define FLAG_SHORTCH8 (0x01)  /* Channel 8 short-circuit fault flag bit */

/* EEPIx registers */
#define EEP_IOUT__64 \
    (0x3F) /* Output current setting IOUT = 64/64 * I(FULL_RANGE) */
#define EEP_IOUT__63 \
    (0x3E) /* Output current setting IOUT = 63/64 * I(FULL_RANGE) */
#define EEP_IOUT__62 \
    (0x3D) /* Output current setting IOUT = 62/64 * I(FULL_RANGE) */
#define EEP_IOUT__61 \
    (0x3C) /* Output current setting IOUT = 61/64 * I(FULL_RANGE) */
#define EEP_IOUT__60 \
    (0x3B) /* Output current setting IOUT = 60/64 * I(FULL_RANGE) */
#define EEP_IOUT__59 \
    (0x3A) /* Output current setting IOUT = 59/64 * I(FULL_RANGE) */
#define EEP_IOUT__58 \
    (0x39) /* Output current setting IOUT = 58/64 * I(FULL_RANGE) */
#define EEP_IOUT__57 \
    (0x38) /* Output current setting IOUT = 57/64 * I(FULL_RANGE) */
#define EEP_IOUT__56 \
    (0x37) /* Output current setting IOUT = 56/64 * I(FULL_RANGE) */
#define EEP_IOUT__55 \
    (0x36) /* Output current setting IOUT = 55/64 * I(FULL_RANGE) */
#define EEP_IOUT__54 \
    (0x35) /* Output current setting IOUT = 54/64 * I(FULL_RANGE) */
#define EEP_IOUT__53 \
    (0x34) /* Output current setting IOUT = 53/64 * I(FULL_RANGE) */
#define EEP_IOUT__52 \
    (0x33) /* Output current setting IOUT = 52/64 * I(FULL_RANGE) */
#define EEP_IOUT__51 \
    (0x32) /* Output current setting IOUT = 51/64 * I(FULL_RANGE) */
#define EEP_IOUT__50 \
    (0x31) /* Output current setting IOUT = 50/64 * I(FULL_RANGE) */
#define EEP_IOUT__49 \
    (0x30) /* Output current setting IOUT = 49/64 * I(FULL_RANGE) */
#define EEP_IOUT__48 \
    (0x2F) /* Output current setting IOUT = 48/64 * I(FULL_RANGE) */
#define EEP_IOUT__47 \
    (0x2E) /* Output current setting IOUT = 47/64 * I(FULL_RANGE) */
#define EEP_IOUT__46 \
    (0x2D) /* Output current setting IOUT = 46/64 * I(FULL_RANGE) */
#define EEP_IOUT__45 \
    (0x2C) /* Output current setting IOUT = 45/64 * I(FULL_RANGE) */
#define EEP_IOUT__44 \
    (0x2B) /* Output current setting IOUT = 44/64 * I(FULL_RANGE) */
#define EEP_IOUT__43 \
    (0x2A) /* Output current setting IOUT = 43/64 * I(FULL_RANGE) */
#define EEP_IOUT__42 \
    (0x29) /* Output current setting IOUT = 42/64 * I(FULL_RANGE) */
#define EEP_IOUT__41 \
    (0x28) /* Output current setting IOUT = 41/64 * I(FULL_RANGE) */
#define EEP_IOUT__40 \
    (0x27) /* Output current setting IOUT = 40/64 * I(FULL_RANGE) */
#define EEP_IOUT__39 \
    (0x26) /* Output current setting IOUT = 39/64 * I(FULL_RANGE) */
#define EEP_IOUT__38 \
    (0x25) /* Output current setting IOUT = 38/64 * I(FULL_RANGE) */
#define EEP_IOUT__37 \
    (0x24) /* Output current setting IOUT = 37/64 * I(FULL_RANGE) */
#define EEP_IOUT__36 \
    (0x23) /* Output current setting IOUT = 36/64 * I(FULL_RANGE) */
#define EEP_IOUT__35 \
    (0x22) /* Output current setting IOUT = 35/64 * I(FULL_RANGE) */
#define EEP_IOUT__34 \
    (0x21) /* Output current setting IOUT = 34/64 * I(FULL_RANGE) */
#define EEP_IOUT__33 \
    (0x20) /* Output current setting IOUT = 33/64 * I(FULL_RANGE) */
#define EEP_IOUT__32 \
    (0x1F) /* Output current setting IOUT = 32/64 * I(FULL_RANGE) */
#define EEP_IOUT__31 \
    (0x1E) /* Output current setting IOUT = 31/64 * I(FULL_RANGE) */
#define EEP_IOUT__30 \
    (0x1D) /* Output current setting IOUT = 30/64 * I(FULL_RANGE) */
#define EEP_IOUT__29 \
    (0x1C) /* Output current setting IOUT = 29/64 * I(FULL_RANGE) */
#define EEP_IOUT__28 \
    (0x1B) /* Output current setting IOUT = 28/64 * I(FULL_RANGE) */
#define EEP_IOUT__27 \
    (0x1A) /* Output current setting IOUT = 27/64 * I(FULL_RANGE) */
#define EEP_IOUT__26 \
    (0x19) /* Output current setting IOUT = 26/64 * I(FULL_RANGE) */
#define EEP_IOUT__25 \
    (0x18) /* Output current setting IOUT = 25/64 * I(FULL_RANGE) */
#define EEP_IOUT__24 \
    (0x17) /* Output current setting IOUT = 24/64 * I(FULL_RANGE) */
#define EEP_IOUT__23 \
    (0x16) /* Output current setting IOUT = 23/64 * I(FULL_RANGE) */
#define EEP_IOUT__22 \
    (0x15) /* Output current setting IOUT = 22/64 * I(FULL_RANGE) */
#define EEP_IOUT__21 \
    (0x14) /* Output current setting IOUT = 21/64 * I(FULL_RANGE) */
#define EEP_IOUT__20 \
    (0x13) /* Output current setting IOUT = 20/64 * I(FULL_RANGE) */
#define EEP_IOUT__19 \
    (0x12) /* Output current setting IOUT = 19/64 * I(FULL_RANGE) */
#define EEP_IOUT__18 \
    (0x11) /* Output current setting IOUT = 18/64 * I(FULL_RANGE) */
#define EEP_IOUT__17 \
    (0x10) /* Output current setting IOUT = 17/64 * I(FULL_RANGE) */
#define EEP_IOUT__16 \
    (0x0F) /* Output current setting IOUT = 16/64 * I(FULL_RANGE) */
#define EEP_IOUT__15 \
    (0x0E) /* Output current setting IOUT = 15/64 * I(FULL_RANGE) */
#define EEP_IOUT__14 \
    (0x0D) /* Output current setting IOUT = 14/64 * I(FULL_RANGE) */
#define EEP_IOUT__13 \
    (0x0C) /* Output current setting IOUT = 13/64 * I(FULL_RANGE) */
#define EEP_IOUT__12 \
    (0x0B) /* Output current setting IOUT = 12/64 * I(FULL_RANGE) */
#define EEP_IOUT__11 \
    (0x0A) /* Output current setting IOUT = 11/64 * I(FULL_RANGE) */
#define EEP_IOUT__10 \
    (0x09) /* Output current setting IOUT = 10/64 * I(FULL_RANGE) */
#define EEP_IOUT__9 \
    (0x08) /* Output current setting IOUT = 9/64 * I(FULL_RANGE) */
#define EEP_IOUT__8 \
    (0x07) /* Output current setting IOUT = 8/64 * I(FULL_RANGE) */
#define EEP_IOUT__7 \
    (0x06) /* Output current setting IOUT = 7/64 * I(FULL_RANGE) */
#define EEP_IOUT__6 \
    (0x05) /* Output current setting IOUT = 6/64 * I(FULL_RANGE) */
#define EEP_IOUT__5 \
    (0x04) /* Output current setting IOUT = 5/64 * I(FULL_RANGE) */
#define EEP_IOUT__4 \
    (0x03) /* Output current setting IOUT = 4/64 * I(FULL_RANGE) */
#define EEP_IOUT__3 \
    (0x02) /* Output current setting IOUT = 3/64 * I(FULL_RANGE) */
#define EEP_IOUT__2 \
    (0x01) /* Output current setting IOUT = 2/64 * I(FULL_RANGE) */
#define EEP_IOUT__1 \
    (0x00) /* Output current setting IOUT = 1/64 * I(FULL_RANGE) */

/* EEPI0 register */
#define EEP_IOUT0 (0x3F) /* Output current setting for OUT0 */

/* EEPI1 register */
#define EEP_IOUT1 (0x3F) /* Output current setting for OUT1 */

/* EEPI2 register */
#define EEP_IOUT2 (0x3F) /* Output current setting for OUT2 */

/* EEPI3 register */
#define EEP_IOUT3 (0x3F) /* Output current setting for OUT3 */

/* EEPI4 register */
#define EEP_IOUT4 (0x3F) /* Output current setting for OUT4 */

/* EEPI5 register */
#define EEP_IOUT5 (0x3F) /* Output current setting for OUT5 */

/* EEPI6 register */
#define EEP_IOUT6 (0x3F) /* Output current setting for OUT6 */

/* EEPI7 register */
#define EEP_IOUT7 (0x3F) /* Output current setting for OUT7 */

/* EEPI8 register */
#define EEP_IOUT8 (0x3F) /* Output current setting for OUT8 */

/* EEPI9 register */
#define EEP_IOUT9 (0x3F) /* Output current setting for OUT9 */

/* EEPI10 register */
#define EEP_IOUT10 (0x3F) /* Output current setting for OUT10 */

/* EEPI11 register */
#define EEP_IOUT11 (0x3F) /* Output current setting for OUT11 */

/* EEPP0 register */
#define EEP_PWMOUT0 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH0 */

/* EEPP1 register */
#define EEP_PWMOUT1 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH1 */

/* EEPP2 register */
#define EEP_PWMOUT2 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH2 */

/* EEPP3 register */
#define EEP_PWMOUT3 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH3 */

/* EEPP4 register */
#define EEP_PWMOUT4 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH4 */

/* EEPP5 register */
#define EEP_PWMOUT5 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH5 */

/* EEPP6 register */
#define EEP_PWMOUT6 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH6 */

/* EEPP7 register */
#define EEP_PWMOUT7 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH7 */

/* EEPP8 register */
#define EEP_PWMOUT8 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH8 */

/* EEPP9 register */
#define EEP_PWMOUT9 (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH9 */

/* EEPP10 register */
#define EEP_PWMOUT10 \
    (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH10 */

/* EEPP11 register */
#define EEP_PWMOUT11 \
    (0xFF) /* PWM Dutycycle EEPROM Register Setting for CH11 */

/* EEPM0 register */
#define EEPM0__1 \
    (0xFF) /* Enable fail-safe state 0 setting for all channels of EEPM0 */
#define EEPM0__0 \
    (0x00) /* Disable fail-safe state 0 setting for all channels of EEPM0 */
#define EEP_FS0CH7__1 \
    (0x80) /* Channel 7 setting in fail-safe state 0 enable */
#define EEP_FS0CH7__0 \
    (0x00) /* Channel 7 setting in fail-safe state 0 disable */
#define EEP_FS0CH6__1 \
    (0x40) /* Channel 6 setting in fail-safe state 0 enable */
#define EEP_FS0CH6__0 \
    (0x00) /* Channel 6 setting in fail-safe state 0 disable */
#define EEP_FS0CH5__1 \
    (0x20) /* Channel 5 setting in fail-safe state 0 enable */
#define EEP_FS0CH5__0 \
    (0x00) /* Channel 5 setting in fail-safe state 0 disable */
#define EEP_FS0CH4__1 \
    (0x10) /* Channel 4 setting in fail-safe state 0 enable */
#define EEP_FS0CH4__0 \
    (0x00) /* Channel 4 setting in fail-safe state 0 disable */
#define EEP_FS0CH3__1 \
    (0x08) /* Channel 3 setting in fail-safe state 0 enable */
#define EEP_FS0CH3__0 \
    (0x00) /* Channel 3 setting in fail-safe state 0 disable */
#define EEP_FS0CH2__1 \
    (0x04) /* Channel 2 setting in fail-safe state 0 enable */
#define EEP_FS0CH2__0 \
    (0x00) /* Channel 2 setting in fail-safe state 0 disable */
#define EEP_FS0CH1__1 \
    (0x02) /* Channel 1 setting in fail-safe state 0 enable */
#define EEP_FS0CH1__0 \
    (0x00) /* Channel 1 setting in fail-safe state 0 disable */
#define EEP_FS0CH0__1 \
    (0x01) /* Channel 0 setting in fail-safe state 0 enable */
#define EEP_FS0CH0__0 \
    (0x00) /* Channel 0 setting in fail-safe state 0 disable */

/* EEPM1 register */
#define EEPM1__1 \
    (0xFF) /* Enable fail-safe state 0 setting for all channels of EEPM1 */
#define EEPM1__0 \
    (0x00) /* Disable fail-safe state 0 setting for all channels of EEPM1 */
#define EEP_FS0CH11__1 \
    (0x08) /* Channel 11 setting in fail-safe state 0 enable */
#define EEP_FS0CH11__0 \
    (0x00) /* Channel 11 setting in fail-safe state 0 disable */
#define EEP_FS0CH10__1 \
    (0x04) /* Channel 10 setting in fail-safe state 0 enable */
#define EEP_FS0CH10__0 \
    (0x00) /* Channel 10 setting in fail-safe state 0 disable */
#define EEP_FS0CH9__1 \
    (0x02) /* Channel 9 setting in fail-safe state 0 enable */
#define EEP_FS0CH9__0 \
    (0x00) /* Channel 9 setting in fail-safe state 0 disable */
#define EEP_FS0CH8__1 \
    (0x01) /* Channel 8 setting in fail-safe state 0 enable */
#define EEP_FS0CH8__0 \
    (0x00) /* Channel 8 setting in fail-safe state 0 disable */

/* EEPM2 register */
#define EEPM2__1 \
    (0xFF) /* Enable fail-safe state 1 setting for all channels of EEPM2 */
#define EEPM2__0 \
    (0x00) /* Disable fail-safe state 1 setting for all channels of EEPM2 */
#define EEP_FS1CH7__1 \
    (0x80) /* Channel 7 setting in fail-safe state 1 enable */
#define EEP_FS1CH7__0 \
    (0x00) /* Channel 7 setting in fail-safe state 1 disable */
#define EEP_FS1CH6__1 \
    (0x40) /* Channel 6 setting in fail-safe state 1 enable */
#define EEP_FS1CH6__0 \
    (0x00) /* Channel 6 setting in fail-safe state 1 disable */
#define EEP_FS1CH5__1 \
    (0x20) /* Channel 5 setting in fail-safe state 1 enable */
#define EEP_FS1CH5__0 \
    (0x00) /* Channel 5 setting in fail-safe state 1 disable */
#define EEP_FS1CH4__1 \
    (0x10) /* Channel 4 setting in fail-safe state 1 enable */
#define EEP_FS1CH4__0 \
    (0x00) /* Channel 4 setting in fail-safe state 1 disable */
#define EEP_FS1CH3__1 \
    (0x08) /* Channel 3 setting in fail-safe state 1 enable */
#define EEP_FS1CH3__0 \
    (0x00) /* Channel 3 setting in fail-safe state 1 disable */
#define EEP_FS1CH2__1 \
    (0x04) /* Channel 2 setting in fail-safe state 1 enable */
#define EEP_FS1CH2__0 \
    (0x00) /* Channel 2 setting in fail-safe state 1 disable */
#define EEP_FS1CH1__1 \
    (0x02) /* Channel 1 setting in fail-safe state 1 enable */
#define EEP_FS1CH1__0 \
    (0x00) /* Channel 1 setting in fail-safe state 1 disable */
#define EEP_FS1CH0__1 \
    (0x01) /* Channel 0 setting in fail-safe state 1 enable */
#define EEP_FS1CH0__0 \
    (0x00) /* Channel 0 setting in fail-safe state 1 disable */

/* EEPM3 register */
#define EEPM3__1 \
    (0xFF) /* Enable fail-safe state 1 setting for all channels of EEPM3 */
#define EEPM3__0 \
    (0x00) /* Disable fail-safe state 1 setting for all channels of EEPM3 */
#define EEP_FS1CH11__1 \
    (0x08) /* Channel 11 setting in fail-safe state 1 enable */
#define EEP_FS1CH11__0 \
    (0x00) /* Channel 11 setting in fail-safe state 1 disable */
#define EEP_FS1CH10__1 \
    (0x04) /* Channel 10 setting in fail-safe state 1 enable */
#define EEP_FS1CH10__0 \
    (0x00) /* Channel 10 setting in fail-safe state 1 disable */
#define EEP_FS1CH9__1 \
    (0x02) /* Channel 9 setting in fail-safe state 1 enable */
#define EEP_FS1CH9__0 \
    (0x00) /* Channel 9 setting in fail-safe state 1 disable */
#define EEP_FS1CH8__1 \
    (0x01) /* Channel 8 setting in fail-safe state 1 enable */
#define EEP_FS1CH8__0 \
    (0x00) /* Channel 8 setting in fail-safe state 1 disable */

/* EEPM4 register */
#define EEPM4__1 (0xFF) /* Enable diagnostics for all channels of EEPM4 */
#define EEPM4__0 (0x00) /* Disable diagnostics for all channels of EEPM4 */
#define EEP_DIAGENCH7__1 (0x80) /* Channel 7 diagnostics enable */
#define EEP_DIAGENCH7__0 (0x00) /* Channel 7 diagnostics disable */
#define EEP_DIAGENCH6__1 (0x40) /* Channel 6 diagnostics enable */
#define EEP_DIAGENCH6__0 (0x00) /* Channel 6 diagnostics disable */
#define EEP_DIAGENCH5__1 (0x20) /* Channel 5 diagnostics enable */
#define EEP_DIAGENCH5__0 (0x00) /* Channel 5 diagnostics disable */
#define EEP_DIAGENCH4__1 (0x10) /* Channel 4 diagnostics enable */
#define EEP_DIAGENCH4__0 (0x00) /* Channel 4 diagnostics disable */
#define EEP_DIAGENCH3__1 (0x08) /* Channel 3 diagnostics enable */
#define EEP_DIAGENCH3__0 (0x00) /* Channel 3 diagnostics disable */
#define EEP_DIAGENCH2__1 (0x04) /* Channel 2 diagnostics enable */
#define EEP_DIAGENCH2__0 (0x00) /* Channel 2 diagnostics disable */
#define EEP_DIAGENCH1__1 (0x02) /* Channel 1 diagnostics enable */
#define EEP_DIAGENCH1__0 (0x00) /* Channel 1 diagnostics disable */
#define EEP_DIAGENCH0__1 (0x01) /* Channel 0 diagnostics enable */
#define EEP_DIAGENCH0__0 (0x00) /* Channel 0 diagnostics disable */

/* EEPM5 register */
#define EEPM5__1 (0xFF) /* Enable diagnostics for all channels of EEPM5 */
#define EEPM5__0 (0x00) /* Disable diagnostics for all channels of EEPM5 */
#define EEP_DIAGENCH11__1 (0x08) /* Channel 11 diagnostics enable */
#define EEP_DIAGENCH11__0 (0x00) /* Channel 11 diagnostics disable */
#define EEP_DIAGENCH10__1 (0x04) /* Channel 10 diagnostics enable */
#define EEP_DIAGENCH10__0 (0x00) /* Channel 10 diagnostics disable */
#define EEP_DIAGENCH9__1 (0x02)  /* Channel 9 diagnostics enable */
#define EEP_DIAGENCH9__0 (0x00)  /* Channel 9 diagnostics disable */
#define EEP_DIAGENCH8__1 (0x01)  /* Channel 8 diagnostics enable */
#define EEP_DIAGENCH8__0 (0x00)  /* Channel 8 diagnostics disable */

/* EEPM6 register */
#define EEP_LDO__4V4 (0x40)    /* LDO output voltage 4.4V */
#define EEP_LDO__5V0 (0x00)    /* LDO output voltage 5V */
#define EEP_LDO__1 (0x40)      /* LDO output voltage 4.4V */
#define EEP_LDO__0 (0x00)      /* LDO output voltage 5V */
#define EEP_EXPEN__1 (0x10)    /* PWM exponetinal dimming enable */
#define EEP_EXPEN__0 (0x00)    /* PWM exponetinal dimming disable */
#define EEP_DEVADDR__15 (0x0F) /* Device slave address 15 */
#define EEP_DEVADDR__14 (0x0E) /* Device slave address 14 */
#define EEP_DEVADDR__13 (0x0D) /* Device slave address 13 */
#define EEP_DEVADDR__12 (0x0C) /* Device slave address 12 */
#define EEP_DEVADDR__11 (0x0B) /* Device slave address 11 */
#define EEP_DEVADDR__10 (0x0A) /* Device slave address 10 */
#define EEP_DEVADDR__9 (0x09)  /* Device slave address 9 */
#define EEP_DEVADDR__8 (0x08)  /* Device slave address 8 */
#define EEP_DEVADDR__7 (0x07)  /* Device slave address 7 */
#define EEP_DEVADDR__6 (0x06)  /* Device slave address 6 */
#define EEP_DEVADDR__5 (0x05)  /* Device slave address 5 */
#define EEP_DEVADDR__4 (0x04)  /* Device slave address 4 */
#define EEP_DEVADDR__3 (0x03)  /* Device slave address 3 */
#define EEP_DEVADDR__2 (0x02)  /* Device slave address 2 */
#define EEP_DEVADDR__1 (0x01)  /* Device slave address 1 */
#define EEP_DEVADDR__0 (0x00)  /* Device slave address 0 */

/* EEPM7 register */
#define EEP_PWMFREQ__20K8 (0xF0) /* PWM frequency 20.8 kHz */
#define EEP_PWMFREQ__9K6 (0xE0)  /* PWM frequency 9.6 kHz */
#define EEP_PWMFREQ__7K8 (0xD0)  /* PWM frequency 7.8 kHz */
#define EEP_PWMFREQ__5K9 (0xC0)  /* PWM frequency 5.9 kHz */
#define EEP_PWMFREQ__4K0 (0xB0)  /* PWM frequency 4 kHz */
#define EEP_PWMFREQ__2K0 (0xA0)  /* PWM frequency 2 kHz */
#define EEP_PWMFREQ__1200 (0x90) /* PWM frequency 1200 Hz */
#define EEP_PWMFREQ__1000 (0x80) /* PWM frequency 1000 Hz */
#define EEP_PWMFREQ__800 (0x70)  /* PWM frequency 800 Hz */
#define EEP_PWMFREQ__600 (0x60)  /* PWM frequency 600 Hz */
#define EEP_PWMFREQ__500 (0x50)  /* PWM frequency 500 Hz */
#define EEP_PWMFREQ__400 (0x40)  /* PWM frequency 400 Hz */
#define EEP_PWMFREQ__350 (0x30)  /* PWM frequency 350 Hz */
#define EEP_PWMFREQ__300 (0x20)  /* PWM frequency 300 Hz */
#define EEP_PWMFREQ__250 (0x10)  /* PWM frequency 250 Hz */
#define EEP_PWMFREQ__200 (0x00)  /* PWM frequency 200 Hz */
#define EEP_INTADDR__1 (0x08)    /* Device slave address set by EEP_DEVADDR */
#define EEP_INTADDR__0 \
    (0x00) /* Device slave address set by ADDR2/ADDR1/ADDR0 pins configuration */
#define EEP_OFAF__1 (0x04)       /* One-fails-all-fail */
#define EEP_OFAF__0 (0x00)       /* One-fails-others-on */
#define EEP_REFRANGE__512 (0x03) /* Reference current ratio 512 */
#define EEP_REFRANGE__256 (0x02) /* Reference current ratio 256 */
#define EEP_REFRANGE__128 (0x01) /* Reference current ratio 128 */
#define EEP_REFRANGE__64 (0x00)  /* Reference current ratio 64 */

/* EEPM8 register */
#define EEP_FLTIMEOUT__10M0 (0x70)  /* FlexWire timeout 10 ms */
#define EEP_FLTIMEOUT__5M0 (0x60)   /* FlexWire timeout 5 ms */
#define EEP_FLTIMEOUT__2M5 (0x50)   /* FlexWire timeout 2.5 ms */
#define EEP_FLTIMEOUT__1M25 (0x40)  /* FlexWire timeout 1.25 ms */
#define EEP_FLTIMEOUT__500U (0x30)  /* FlexWire timeout 500 us */
#define EEP_FLTIMEOUT__250U (0x20)  /* FlexWire timeout 250 us */
#define EEP_FLTIMEOUT__125U (0x10)  /* FlexWire timeout 125 us */
#define EEP_FLTIMEOUT__1M0 (0x00)   /* FlexWire timeout 1 ms */
#define EEP_ADCLOWSUPTH__40V (0x0F) /* ADC Supply monitor threshold 40V */
#define EEP_ADCLOWSUPTH__38V (0x0E) /* ADC Supply monitor threshold 38V */
#define EEP_ADCLOWSUPTH__36V (0x0D) /* ADC Supply monitor threshold 36V */
#define EEP_ADCLOWSUPTH__34V (0x0C) /* ADC Supply monitor threshold 34V */
#define EEP_ADCLOWSUPTH__32V (0x0B) /* ADC Supply monitor threshold 32V */
#define EEP_ADCLOWSUPTH__30V (0x0A) /* ADC Supply monitor threshold 30V */
#define EEP_ADCLOWSUPTH__28V (0x09) /* ADC Supply monitor threshold 28V */
#define EEP_ADCLOWSUPTH__26V (0x08) /* ADC Supply monitor threshold 26V */
#define EEP_ADCLOWSUPTH__24V (0x07) /* ADC Supply monitor threshold 24V */
#define EEP_ADCLOWSUPTH__22V (0x06) /* ADC Supply monitor threshold 22V */
#define EEP_ADCLOWSUPTH__20V (0x05) /* ADC Supply monitor threshold 20V */
#define EEP_ADCLOWSUPTH__18V (0x04) /* ADC Supply monitor threshold 18V */
#define EEP_ADCLOWSUPTH__16V (0x03) /* ADC Supply monitor threshold 16V */
#define EEP_ADCLOWSUPTH__14V (0x02) /* ADC Supply monitor threshold 14V */
#define EEP_ADCLOWSUPTH__12V (0x01) /* ADC Supply monitor threshold 12V */
#define EEP_ADCLOWSUPTH__10V (0x00) /* ADC Supply monitor threshold 10V */

/* EEPM9 register */
#define EEP_ODIOUT__64 \
    (0xF0) /* On-demand diagnostics output current IOUT = 64/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__57 \
    (0xE0) /* On-demand diagnostics output current IOUT = 57/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__53 \
    (0xD0) /* On-demand diagnostics output current IOUT = 53/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__49 \
    (0xC0) /* On-demand diagnostics output current IOUT = 49/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__45 \
    (0xB0) /* On-demand diagnostics output current IOUT = 45/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__41 \
    (0xA0) /* On-demand diagnostics output current IOUT = 41/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__37 \
    (0x90) /* On-demand diagnostics output current IOUT = 37/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__33 \
    (0x80) /* On-demand diagnostics output current IOUT = 33/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__29 \
    (0x70) /* On-demand diagnostics output current IOUT = 29/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__25 \
    (0x60) /* On-demand diagnostics output current IOUT = 25/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__21 \
    (0x50) /* On-demand diagnostics output current IOUT = 21/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__17 \
    (0x40) /* On-demand diagnostics output current IOUT = 17/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__13 \
    (0x30) /* On-demand diagnostics output current IOUT = 13/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__9 \
    (0x20) /* On-demand diagnostics output current IOUT = 9/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__5 \
    (0x10) /* On-demand diagnostics output current IOUT = 5/64 * I(FULL_RANGE) */
#define EEP_ODIOUT__1 \
    (0x00) /* On-demand diagnostics output current IOUT = 1/64 * I(FULL_RANGE) */
#define EEP_ODPW__5M0 (0x0F)  /* On-demand diagnostics pulse-width 5 ms */
#define EEP_ODPW__3M0 (0x0E)  /* On-demand diagnostics pulse-width 3 ms */
#define EEP_ODPW__2M0 (0x0D)  /* On-demand diagnostics pulse-width 2 ms */
#define EEP_ODPW__1M5 (0x0C)  /* On-demand diagnostics pulse-width 1.5 ms */
#define EEP_ODPW__1M2 (0x0B)  /* On-demand diagnostics pulse-width 1.2 ms */
#define EEP_ODPW__1M0 (0x0A)  /* On-demand diagnostics pulse-width 1 ms */
#define EEP_ODPW__800U (0x09) /* On-demand diagnostics pulse-width 800 us */
#define EEP_ODPW__500U (0x08) /* On-demand diagnostics pulse-width 500 us */
#define EEP_ODPW__300U (0x07) /* On-demand diagnostics pulse-width 300 us */
#define EEP_ODPW__200U (0x06) /* On-demand diagnostics pulse-width 200 us */
#define EEP_ODPW__150U (0x05) /* On-demand diagnostics pulse-width 150 us */
#define EEP_ODPW__80U (0x04)  /* On-demand diagnostics pulse-width 80 us */
#define EEP_ODPW__50U (0x03)  /* On-demand diagnostics pulse-width 50 us */
#define EEP_ODPW__30U (0x02)  /* On-demand diagnostics pulse-width 30 us */
#define EEP_ODPW__20U (0x01)  /* On-demand diagnostics pulse-width 20 us */
#define EEP_ODPW__100U (0x00) /* On-demand diagnostics pulse-width 100 us */

/* EEPM10 register */
#define EEP_WDTIMER__0_FS \
    (0xF0) /* Watchdog timer disabled; direct enter fail-safe state */
#define EEP_WDTIMER__500M0 (0xB0) /* Watchdog timer 500 ms */
#define EEP_WDTIMER__200M0 (0xA0) /* Watchdog timer 500 ms */
#define EEP_WDTIMER__100M0 (0x90) /* Watchdog timer 500 ms */
#define EEP_WDTIMER__50M0 (0x80)  /* Watchdog timer 500 ms */
#define EEP_WDTIMER__20M0 (0x70)  /* Watchdog timer 500 ms */
#define EEP_WDTIMER__10M0 (0x60)  /* Watchdog timer 500 ms */
#define EEP_WDTIMER__5M0 (0x50)   /* Watchdog timer 500 ms */
#define EEP_WDTIMER__2M0 (0x40)   /* Watchdog timer 500 ms */
#define EEP_WDTIMER__1M0 (0x30)   /* Watchdog timer 500 ms */
#define EEP_WDTIMER__500U (0x20)  /* Watchdog timer 500 ms */
#define EEP_WDTIMER__200U (0x10)  /* Watchdog timer 500 ms */
#define EEP_WDTIMER__0_NO_FS \
    (0x00) /* Watchdog timer disabled; do not automatically enter fail-safe state */
#define EEP_INITTIMER__50U (0x0F)  /* Initialization timer 50 us */
#define EEP_INITTIMER__100U (0x09) /* Initialization timer 100 us */
#define EEP_INITTIMER__200U (0x08) /* Initialization timer 200 us */
#define EEP_INITTIMER__500U (0x07) /* Initialization timer 500 us */
#define EEP_INITTIMER__1M0 (0x06)  /* Initialization timer 1 ms */
#define EEP_INITTIMER__2M0 (0x05)  /* Initialization timer 2 ms */
#define EEP_INITTIMER__5M0 (0x04)  /* Initialization timer 5 ms */
#define EEP_INITTIMER__10M0 (0x03) /* Initialization timer 10 ms */
#define EEP_INITTIMER__20M0 (0x02) /* Initialization timer 20 ms */
#define EEP_INITTIMER__50M0 (0x01) /* Initialization timer 50 ms */
#define EEP_INITTIMER__0 (0x00)    /* Initialization timer 0 s */

/* EEPM11 register */
#define EEP_ADCSHORTTH__40V65 \
    (0xFF) /* ADC short detection threshold 40.6543 V */
#define EEP_ADCSHORTTH__40V50 \
    (0xFE) /* ADC short detection threshold 40.496 V */
#define EEP_ADCSHORTTH__40V34 \
    (0xFD) /* ADC short detection threshold 40.3377 V */
#define EEP_ADCSHORTTH__40V18 \
    (0xFC) /* ADC short detection threshold 40.1794 V */
#define EEP_ADCSHORTTH__40V02 \
    (0xFB) /* ADC short detection threshold 40.0211 V */
#define EEP_ADCSHORTTH__39V86 \
    (0xFA) /* ADC short detection threshold 39.8628 V */
#define EEP_ADCSHORTTH__39V70 \
    (0xF9) /* ADC short detection threshold 39.7045 V */
#define EEP_ADCSHORTTH__39V55 \
    (0xF8) /* ADC short detection threshold 39.5462 V */
#define EEP_ADCSHORTTH__39V39 \
    (0xF7) /* ADC short detection threshold 39.3879 V */
#define EEP_ADCSHORTTH__39V23 \
    (0xF6) /* ADC short detection threshold 39.2296 V */
#define EEP_ADCSHORTTH__39V07 \
    (0xF5) /* ADC short detection threshold 39.0713 V */
#define EEP_ADCSHORTTH__38V91 \
    (0xF4) /* ADC short detection threshold 38.913 V */
#define EEP_ADCSHORTTH__38V75 \
    (0xF3) /* ADC short detection threshold 38.7547 V */
#define EEP_ADCSHORTTH__38V60 \
    (0xF2) /* ADC short detection threshold 38.5964 V */
#define EEP_ADCSHORTTH__38V44 \
    (0xF1) /* ADC short detection threshold 38.4381 V */
#define EEP_ADCSHORTTH__38V28 \
    (0xF0) /* ADC short detection threshold 38.2798 V */
#define EEP_ADCSHORTTH__38V12 \
    (0xEF) /* ADC short detection threshold 38.1215 V */
#define EEP_ADCSHORTTH__37V96 \
    (0xEE) /* ADC short detection threshold 37.9632 V */
#define EEP_ADCSHORTTH__37V80 \
    (0xED) /* ADC short detection threshold 37.8049 V */
#define EEP_ADCSHORTTH__37V65 \
    (0xEC) /* ADC short detection threshold 37.6466 V */
#define EEP_ADCSHORTTH__37V49 \
    (0xEB) /* ADC short detection threshold 37.4883 V */
#define EEP_ADCSHORTTH__37V33 \
    (0xEA) /* ADC short detection threshold 37.33 V */
#define EEP_ADCSHORTTH__37V17 \
    (0xE9) /* ADC short detection threshold 37.1717 V */
#define EEP_ADCSHORTTH__37V01 \
    (0xE8) /* ADC short detection threshold 37.0134 V */
#define EEP_ADCSHORTTH__36V86 \
    (0xE7) /* ADC short detection threshold 36.8551 V */
#define EEP_ADCSHORTTH__36V70 \
    (0xE6) /* ADC short detection threshold 36.6968 V */
#define EEP_ADCSHORTTH__36V54 \
    (0xE5) /* ADC short detection threshold 36.5385 V */
#define EEP_ADCSHORTTH__36V38 \
    (0xE4) /* ADC short detection threshold 36.3802 V */
#define EEP_ADCSHORTTH__36V22 \
    (0xE3) /* ADC short detection threshold 36.2219 V */
#define EEP_ADCSHORTTH__36V06 \
    (0xE2) /* ADC short detection threshold 36.0636 V */
#define EEP_ADCSHORTTH__35V91 \
    (0xE1) /* ADC short detection threshold 35.9053 V */
#define EEP_ADCSHORTTH__35V75 \
    (0xE0) /* ADC short detection threshold 35.747 V */
#define EEP_ADCSHORTTH__35V59 \
    (0xDF) /* ADC short detection threshold 35.5887 V */
#define EEP_ADCSHORTTH__35V43 \
    (0xDE) /* ADC short detection threshold 35.4304 V */
#define EEP_ADCSHORTTH__35V27 \
    (0xDD) /* ADC short detection threshold 35.2721 V */
#define EEP_ADCSHORTTH__35V11 \
    (0xDC) /* ADC short detection threshold 35.1138 V */
#define EEP_ADCSHORTTH__34V96 \
    (0xDB) /* ADC short detection threshold 34.9555 V */
#define EEP_ADCSHORTTH__34V80 \
    (0xDA) /* ADC short detection threshold 34.7972 V */
#define EEP_ADCSHORTTH__34V64 \
    (0xD9) /* ADC short detection threshold 34.6389 V */
#define EEP_ADCSHORTTH__34V48 \
    (0xD8) /* ADC short detection threshold 34.4806 V */
#define EEP_ADCSHORTTH__34V32 \
    (0xD7) /* ADC short detection threshold 34.3223 V */
#define EEP_ADCSHORTTH__34V16 \
    (0xD6) /* ADC short detection threshold 34.164 V */
#define EEP_ADCSHORTTH__34V01 \
    (0xD5) /* ADC short detection threshold 34.0057 V */
#define EEP_ADCSHORTTH__33V85 \
    (0xD4) /* ADC short detection threshold 33.8474 V */
#define EEP_ADCSHORTTH__33V69 \
    (0xD3) /* ADC short detection threshold 33.6891 V */
#define EEP_ADCSHORTTH__33V53 \
    (0xD2) /* ADC short detection threshold 33.5308 V */
#define EEP_ADCSHORTTH__33V37 \
    (0xD1) /* ADC short detection threshold 33.3725 V */
#define EEP_ADCSHORTTH__33V21 \
    (0xD0) /* ADC short detection threshold 33.2142 V */
#define EEP_ADCSHORTTH__33V06 \
    (0xCF) /* ADC short detection threshold 33.0559 V */
#define EEP_ADCSHORTTH__32V90 \
    (0xCE) /* ADC short detection threshold 32.8976 V */
#define EEP_ADCSHORTTH__32V74 \
    (0xCD) /* ADC short detection threshold 32.7393 V */
#define EEP_ADCSHORTTH__32V58 \
    (0xCC) /* ADC short detection threshold 32.581 V */
#define EEP_ADCSHORTTH__32V42 \
    (0xCB) /* ADC short detection threshold 32.4227 V */
#define EEP_ADCSHORTTH__32V26 \
    (0xCA) /* ADC short detection threshold 32.2644 V */
#define EEP_ADCSHORTTH__32V11 \
    (0xC9) /* ADC short detection threshold 32.1061 V */
#define EEP_ADCSHORTTH__31V95 \
    (0xC8) /* ADC short detection threshold 31.9478 V */
#define EEP_ADCSHORTTH__31V79 \
    (0xC7) /* ADC short detection threshold 31.7895 V */
#define EEP_ADCSHORTTH__31V63 \
    (0xC6) /* ADC short detection threshold 31.6312 V */
#define EEP_ADCSHORTTH__31V47 \
    (0xC5) /* ADC short detection threshold 31.4729 V */
#define EEP_ADCSHORTTH__31V31 \
    (0xC4) /* ADC short detection threshold 31.3146 V */
#define EEP_ADCSHORTTH__31V16 \
    (0xC3) /* ADC short detection threshold 31.1563 V */
#define EEP_ADCSHORTTH__31V00 \
    (0xC2) /* ADC short detection threshold 30.998 V */
#define EEP_ADCSHORTTH__30V84 \
    (0xC1) /* ADC short detection threshold 30.8397 V */
#define EEP_ADCSHORTTH__30V68 \
    (0xC0) /* ADC short detection threshold 30.6814 V */
#define EEP_ADCSHORTTH__30V52 \
    (0xBF) /* ADC short detection threshold 30.5231 V */
#define EEP_ADCSHORTTH__30V36 \
    (0xBE) /* ADC short detection threshold 30.3648 V */
#define EEP_ADCSHORTTH__30V21 \
    (0xBD) /* ADC short detection threshold 30.2065 V */
#define EEP_ADCSHORTTH__30V05 \
    (0xBC) /* ADC short detection threshold 30.0482 V */
#define EEP_ADCSHORTTH__29V89 \
    (0xBB) /* ADC short detection threshold 29.8899 V */
#define EEP_ADCSHORTTH__29V73 \
    (0xBA) /* ADC short detection threshold 29.7316 V */
#define EEP_ADCSHORTTH__29V57 \
    (0xB9) /* ADC short detection threshold 29.5733 V */
#define EEP_ADCSHORTTH__29V42 \
    (0xB8) /* ADC short detection threshold 29.415 V */
#define EEP_ADCSHORTTH__29V26 \
    (0xB7) /* ADC short detection threshold 29.2567 V */
#define EEP_ADCSHORTTH__29V10 \
    (0xB6) /* ADC short detection threshold 29.0984 V */
#define EEP_ADCSHORTTH__28V94 \
    (0xB5) /* ADC short detection threshold 28.9401 V */
#define EEP_ADCSHORTTH__28V78 \
    (0xB4) /* ADC short detection threshold 28.7818 V */
#define EEP_ADCSHORTTH__28V62 \
    (0xB3) /* ADC short detection threshold 28.6235 V */
#define EEP_ADCSHORTTH__28V47 \
    (0xB2) /* ADC short detection threshold 28.4652 V */
#define EEP_ADCSHORTTH__28V31 \
    (0xB1) /* ADC short detection threshold 28.3069 V */
#define EEP_ADCSHORTTH__28V15 \
    (0xB0) /* ADC short detection threshold 28.1486 V */
#define EEP_ADCSHORTTH__27V99 \
    (0xAF) /* ADC short detection threshold 27.9903 V */
#define EEP_ADCSHORTTH__27V83 \
    (0xAE) /* ADC short detection threshold 27.832 V */
#define EEP_ADCSHORTTH__27V67 \
    (0xAD) /* ADC short detection threshold 27.6737 V */
#define EEP_ADCSHORTTH__27V52 \
    (0xAC) /* ADC short detection threshold 27.5154 V */
#define EEP_ADCSHORTTH__27V36 \
    (0xAB) /* ADC short detection threshold 27.3571 V */
#define EEP_ADCSHORTTH__27V20 \
    (0xAA) /* ADC short detection threshold 27.1988 V */
#define EEP_ADCSHORTTH__27V04 \
    (0xA9) /* ADC short detection threshold 27.0405 V */
#define EEP_ADCSHORTTH__26V88 \
    (0xA8) /* ADC short detection threshold 26.8822 V */
#define EEP_ADCSHORTTH__26V72 \
    (0xA7) /* ADC short detection threshold 26.7239 V */
#define EEP_ADCSHORTTH__26V57 \
    (0xA6) /* ADC short detection threshold 26.5656 V */
#define EEP_ADCSHORTTH__26V41 \
    (0xA5) /* ADC short detection threshold 26.4073 V */
#define EEP_ADCSHORTTH__26V25 \
    (0xA4) /* ADC short detection threshold 26.249 V */
#define EEP_ADCSHORTTH__26V09 \
    (0xA3) /* ADC short detection threshold 26.0907 V */
#define EEP_ADCSHORTTH__25V93 \
    (0xA2) /* ADC short detection threshold 25.9324 V */
#define EEP_ADCSHORTTH__25V77 \
    (0xA1) /* ADC short detection threshold 25.7741 V */
#define EEP_ADCSHORTTH__25V62 \
    (0xA0) /* ADC short detection threshold 25.6158 V */
#define EEP_ADCSHORTTH__25V46 \
    (0x9F) /* ADC short detection threshold 25.4575 V */
#define EEP_ADCSHORTTH__25V30 \
    (0x9E) /* ADC short detection threshold 25.2992 V */
#define EEP_ADCSHORTTH__25V14 \
    (0x9D) /* ADC short detection threshold 25.1409 V */
#define EEP_ADCSHORTTH__24V98 \
    (0x9C) /* ADC short detection threshold 24.9826 V */
#define EEP_ADCSHORTTH__24V82 \
    (0x9B) /* ADC short detection threshold 24.8243 V */
#define EEP_ADCSHORTTH__24V67 \
    (0x9A) /* ADC short detection threshold 24.666 V */
#define EEP_ADCSHORTTH__24V51 \
    (0x99) /* ADC short detection threshold 24.5077 V */
#define EEP_ADCSHORTTH__24V35 \
    (0x98) /* ADC short detection threshold 24.3494 V */
#define EEP_ADCSHORTTH__24V19 \
    (0x97) /* ADC short detection threshold 24.1911 V */
#define EEP_ADCSHORTTH__24V03 \
    (0x96) /* ADC short detection threshold 24.0328 V */
#define EEP_ADCSHORTTH__23V87 \
    (0x95) /* ADC short detection threshold 23.8745 V */
#define EEP_ADCSHORTTH__23V72 \
    (0x94) /* ADC short detection threshold 23.7162 V */
#define EEP_ADCSHORTTH__23V56 \
    (0x93) /* ADC short detection threshold 23.5579 V */
#define EEP_ADCSHORTTH__23V40 \
    (0x92) /* ADC short detection threshold 23.3996 V */
#define EEP_ADCSHORTTH__23V24 \
    (0x91) /* ADC short detection threshold 23.2413 V */
#define EEP_ADCSHORTTH__23V08 \
    (0x90) /* ADC short detection threshold 23.083 V */
#define EEP_ADCSHORTTH__22V92 \
    (0x8F) /* ADC short detection threshold 22.9247 V */
#define EEP_ADCSHORTTH__22V77 \
    (0x8E) /* ADC short detection threshold 22.7664 V */
#define EEP_ADCSHORTTH__22V61 \
    (0x8D) /* ADC short detection threshold 22.6081 V */
#define EEP_ADCSHORTTH__22V45 \
    (0x8C) /* ADC short detection threshold 22.4498 V */
#define EEP_ADCSHORTTH__22V29 \
    (0x8B) /* ADC short detection threshold 22.2915 V */
#define EEP_ADCSHORTTH__22V13 \
    (0x8A) /* ADC short detection threshold 22.1332 V */
#define EEP_ADCSHORTTH__21V97 \
    (0x89) /* ADC short detection threshold 21.9749 V */
#define EEP_ADCSHORTTH__21V82 \
    (0x88) /* ADC short detection threshold 21.8166 V */
#define EEP_ADCSHORTTH__21V66 \
    (0x87) /* ADC short detection threshold 21.6583 V */
#define EEP_ADCSHORTTH__21V50 (0x86) /* ADC short detection threshold 21.5 V */
#define EEP_ADCSHORTTH__21V34 \
    (0x85) /* ADC short detection threshold 21.3417 V */
#define EEP_ADCSHORTTH__21V18 \
    (0x84) /* ADC short detection threshold 21.1834 V */
#define EEP_ADCSHORTTH__21V03 \
    (0x83) /* ADC short detection threshold 21.0251 V */
#define EEP_ADCSHORTTH__20V87 \
    (0x82) /* ADC short detection threshold 20.8668 V */
#define EEP_ADCSHORTTH__20V71 \
    (0x81) /* ADC short detection threshold 20.7085 V */
#define EEP_ADCSHORTTH__20V55 \
    (0x80) /* ADC short detection threshold 20.5502 V */
#define EEP_ADCSHORTTH__20V39 \
    (0x7F) /* ADC short detection threshold 20.3919 V */
#define EEP_ADCSHORTTH__20V23 \
    (0x7E) /* ADC short detection threshold 20.2336 V */
#define EEP_ADCSHORTTH__20V08 \
    (0x7D) /* ADC short detection threshold 20.0753 V */
#define EEP_ADCSHORTTH__19V92 \
    (0x7C) /* ADC short detection threshold 19.917 V */
#define EEP_ADCSHORTTH__19V76 \
    (0x7B) /* ADC short detection threshold 19.7587 V */
#define EEP_ADCSHORTTH__19V60 \
    (0x7A) /* ADC short detection threshold 19.6004 V */
#define EEP_ADCSHORTTH__19V44 \
    (0x79) /* ADC short detection threshold 19.4421 V */
#define EEP_ADCSHORTTH__19V28 \
    (0x78) /* ADC short detection threshold 19.2838 V */
#define EEP_ADCSHORTTH__19V13 \
    (0x77) /* ADC short detection threshold 19.1255 V */
#define EEP_ADCSHORTTH__18V97 \
    (0x76) /* ADC short detection threshold 18.9672 V */
#define EEP_ADCSHORTTH__18V81 \
    (0x75) /* ADC short detection threshold 18.8089 V */
#define EEP_ADCSHORTTH__18V65 \
    (0x74) /* ADC short detection threshold 18.6506 V */
#define EEP_ADCSHORTTH__18V49 \
    (0x73) /* ADC short detection threshold 18.4923 V */
#define EEP_ADCSHORTTH__18V33 \
    (0x72) /* ADC short detection threshold 18.334 V */
#define EEP_ADCSHORTTH__18V18 \
    (0x71) /* ADC short detection threshold 18.1757 V */
#define EEP_ADCSHORTTH__18V02 \
    (0x70) /* ADC short detection threshold 18.0174 V */
#define EEP_ADCSHORTTH__17V86 \
    (0x6F) /* ADC short detection threshold 17.8591 V */
#define EEP_ADCSHORTTH__17V70 \
    (0x6E) /* ADC short detection threshold 17.7008 V */
#define EEP_ADCSHORTTH__17V54 \
    (0x6D) /* ADC short detection threshold 17.5425 V */
#define EEP_ADCSHORTTH__17V38 \
    (0x6C) /* ADC short detection threshold 17.3842 V */
#define EEP_ADCSHORTTH__17V23 \
    (0x6B) /* ADC short detection threshold 17.2259 V */
#define EEP_ADCSHORTTH__17V07 \
    (0x6A) /* ADC short detection threshold 17.0676 V */
#define EEP_ADCSHORTTH__16V91 \
    (0x69) /* ADC short detection threshold 16.9093 V */
#define EEP_ADCSHORTTH__16V75 \
    (0x68) /* ADC short detection threshold 16.751 V */
#define EEP_ADCSHORTTH__16V59 \
    (0x67) /* ADC short detection threshold 16.5927 V */
#define EEP_ADCSHORTTH__16V43 \
    (0x66) /* ADC short detection threshold 16.4344 V */
#define EEP_ADCSHORTTH__16V28 \
    (0x65) /* ADC short detection threshold 16.2761 V */
#define EEP_ADCSHORTTH__16V12 \
    (0x64) /* ADC short detection threshold 16.1178 V */
#define EEP_ADCSHORTTH__15V96 \
    (0x63) /* ADC short detection threshold 15.9595 V */
#define EEP_ADCSHORTTH__15V80 \
    (0x62) /* ADC short detection threshold 15.8012 V */
#define EEP_ADCSHORTTH__15V64 \
    (0x61) /* ADC short detection threshold 15.6429 V */
#define EEP_ADCSHORTTH__15V48 \
    (0x60) /* ADC short detection threshold 15.4846 V */
#define EEP_ADCSHORTTH__15V33 \
    (0x5F) /* ADC short detection threshold 15.3263 V */
#define EEP_ADCSHORTTH__15V17 \
    (0x5E) /* ADC short detection threshold 15.168 V */
#define EEP_ADCSHORTTH__15V01 \
    (0x5D) /* ADC short detection threshold 15.0097 V */
#define EEP_ADCSHORTTH__14V85 \
    (0x5C) /* ADC short detection threshold 14.8514 V */
#define EEP_ADCSHORTTH__14V69 \
    (0x5B) /* ADC short detection threshold 14.6931 V */
#define EEP_ADCSHORTTH__14V53 \
    (0x5A) /* ADC short detection threshold 14.5348 V */
#define EEP_ADCSHORTTH__14V38 \
    (0x59) /* ADC short detection threshold 14.3765 V */
#define EEP_ADCSHORTTH__14V22 \
    (0x58) /* ADC short detection threshold 14.2182 V */
#define EEP_ADCSHORTTH__14V06 \
    (0x57) /* ADC short detection threshold 14.0599 V */
#define EEP_ADCSHORTTH__13V90 \
    (0x56) /* ADC short detection threshold 13.9016 V */
#define EEP_ADCSHORTTH__13V74 \
    (0x55) /* ADC short detection threshold 13.7433 V */
#define EEP_ADCSHORTTH__13V59 \
    (0x54) /* ADC short detection threshold 13.585 V */
#define EEP_ADCSHORTTH__13V43 \
    (0x53) /* ADC short detection threshold 13.4267 V */
#define EEP_ADCSHORTTH__13V27 \
    (0x52) /* ADC short detection threshold 13.2684 V */
#define EEP_ADCSHORTTH__13V11 \
    (0x51) /* ADC short detection threshold 13.1101 V */
#define EEP_ADCSHORTTH__12V95 \
    (0x50) /* ADC short detection threshold 12.9518 V */
#define EEP_ADCSHORTTH__12V79 \
    (0x4F) /* ADC short detection threshold 12.7935 V */
#define EEP_ADCSHORTTH__12V64 \
    (0x4E) /* ADC short detection threshold 12.6352 V */
#define EEP_ADCSHORTTH__12V48 \
    (0x4D) /* ADC short detection threshold 12.4769 V */
#define EEP_ADCSHORTTH__12V32 \
    (0x4C) /* ADC short detection threshold 12.3186 V */
#define EEP_ADCSHORTTH__12V16 \
    (0x4B) /* ADC short detection threshold 12.1603 V */
#define EEP_ADCSHORTTH__12V00 \
    (0x4A) /* ADC short detection threshold 12.002 V */
#define EEP_ADCSHORTTH__11V84 \
    (0x49) /* ADC short detection threshold 11.8437 V */
#define EEP_ADCSHORTTH__11V69 \
    (0x48) /* ADC short detection threshold 11.6854 V */
#define EEP_ADCSHORTTH__11V53 \
    (0x47) /* ADC short detection threshold 11.5271 V */
#define EEP_ADCSHORTTH__11V37 \
    (0x46) /* ADC short detection threshold 11.3688 V */
#define EEP_ADCSHORTTH__11V21 \
    (0x45) /* ADC short detection threshold 11.2105 V */
#define EEP_ADCSHORTTH__11V05 \
    (0x44) /* ADC short detection threshold 11.0522 V */
#define EEP_ADCSHORTTH__10V89 \
    (0x43) /* ADC short detection threshold 10.8939 V */
#define EEP_ADCSHORTTH__10V74 \
    (0x42) /* ADC short detection threshold 10.7356 V */
#define EEP_ADCSHORTTH__10V58 \
    (0x41) /* ADC short detection threshold 10.5773 V */
#define EEP_ADCSHORTTH__10V42 \
    (0x40) /* ADC short detection threshold 10.419 V */
#define EEP_ADCSHORTTH__10V26 \
    (0x3F) /* ADC short detection threshold 10.2607 V */
#define EEP_ADCSHORTTH__10V10 \
    (0x3E) /* ADC short detection threshold 10.1024 V */
#define EEP_ADCSHORTTH__9V94 \
    (0x3D) /* ADC short detection threshold 9.9441 V */
#define EEP_ADCSHORTTH__9V79 \
    (0x3C) /* ADC short detection threshold 9.7858 V */
#define EEP_ADCSHORTTH__9V63 \
    (0x3B) /* ADC short detection threshold 9.6275 V */
#define EEP_ADCSHORTTH__9V47 \
    (0x3A) /* ADC short detection threshold 9.4692 V */
#define EEP_ADCSHORTTH__9V31 \
    (0x39) /* ADC short detection threshold 9.3109 V */
#define EEP_ADCSHORTTH__9V15 \
    (0x38) /* ADC short detection threshold 9.1526 V */
#define EEP_ADCSHORTTH__8V99 \
    (0x37) /* ADC short detection threshold 8.9943 V */
#define EEP_ADCSHORTTH__8V84 (0x36) /* ADC short detection threshold 8.836 V */
#define EEP_ADCSHORTTH__8V68 \
    (0x35) /* ADC short detection threshold 8.6777 V */
#define EEP_ADCSHORTTH__8V52 \
    (0x34) /* ADC short detection threshold 8.5194 V */
#define EEP_ADCSHORTTH__8V36 \
    (0x33) /* ADC short detection threshold 8.3611 V */
#define EEP_ADCSHORTTH__8V20 \
    (0x32) /* ADC short detection threshold 8.2028 V */
#define EEP_ADCSHORTTH__8V04 \
    (0x31) /* ADC short detection threshold 8.0445 V */
#define EEP_ADCSHORTTH__7V89 \
    (0x30) /* ADC short detection threshold 7.8862 V */
#define EEP_ADCSHORTTH__7V73 \
    (0x2F) /* ADC short detection threshold 7.7279 V */
#define EEP_ADCSHORTTH__7V57 \
    (0x2E) /* ADC short detection threshold 7.5696 V */
#define EEP_ADCSHORTTH__7V41 \
    (0x2D) /* ADC short detection threshold 7.4113 V */
#define EEP_ADCSHORTTH__7V25 (0x2C) /* ADC short detection threshold 7.253 V */
#define EEP_ADCSHORTTH__7V09 \
    (0x2B) /* ADC short detection threshold 7.0947 V */
#define EEP_ADCSHORTTH__6V94 \
    (0x2A) /* ADC short detection threshold 6.9364 V */
#define EEP_ADCSHORTTH__6V78 \
    (0x29) /* ADC short detection threshold 6.7781 V */
#define EEP_ADCSHORTTH__6V62 \
    (0x28) /* ADC short detection threshold 6.6198 V */
#define EEP_ADCSHORTTH__6V46 \
    (0x27) /* ADC short detection threshold 6.4615 V */
#define EEP_ADCSHORTTH__6V30 \
    (0x26) /* ADC short detection threshold 6.3032 V */
#define EEP_ADCSHORTTH__6V14 \
    (0x25) /* ADC short detection threshold 6.1449 V */
#define EEP_ADCSHORTTH__5V99 \
    (0x24) /* ADC short detection threshold 5.9866 V */
#define EEP_ADCSHORTTH__5V83 \
    (0x23) /* ADC short detection threshold 5.8283 V */
#define EEP_ADCSHORTTH__5V67 (0x22) /* ADC short detection threshold 5.67 V */
#define EEP_ADCSHORTTH__5V51 \
    (0x21) /* ADC short detection threshold 5.5117 V */
#define EEP_ADCSHORTTH__5V35 \
    (0x20) /* ADC short detection threshold 5.3534 V */
#define EEP_ADCSHORTTH__5V20 \
    (0x1F) /* ADC short detection threshold 5.1951 V */
#define EEP_ADCSHORTTH__5V04 \
    (0x1E) /* ADC short detection threshold 5.0368 V */
#define EEP_ADCSHORTTH__4V88 \
    (0x1D) /* ADC short detection threshold 4.8785 V */
#define EEP_ADCSHORTTH__4V72 \
    (0x1C) /* ADC short detection threshold 4.7202 V */
#define EEP_ADCSHORTTH__4V56 \
    (0x1B) /* ADC short detection threshold 4.5619 V */
#define EEP_ADCSHORTTH__4V40 \
    (0x1A) /* ADC short detection threshold 4.4036 V */
#define EEP_ADCSHORTTH__4V25 \
    (0x19) /* ADC short detection threshold 4.2453 V */
#define EEP_ADCSHORTTH__4V09 (0x18) /* ADC short detection threshold 4.087 V */
#define EEP_ADCSHORTTH__3V93 \
    (0x17) /* ADC short detection threshold 3.9287 V */
#define EEP_ADCSHORTTH__3V77 \
    (0x16) /* ADC short detection threshold 3.7704 V */
#define EEP_ADCSHORTTH__3V61 \
    (0x15) /* ADC short detection threshold 3.6121 V */
#define EEP_ADCSHORTTH__3V45 \
    (0x14) /* ADC short detection threshold 3.4538 V */
#define EEP_ADCSHORTTH__3V30 \
    (0x13) /* ADC short detection threshold 3.2955 V */
#define EEP_ADCSHORTTH__3V14 \
    (0x12) /* ADC short detection threshold 3.1372 V */
#define EEP_ADCSHORTTH__2V98 \
    (0x11) /* ADC short detection threshold 2.9789 V */
#define EEP_ADCSHORTTH__2V82 \
    (0x10) /* ADC short detection threshold 2.8206 V */
#define EEP_ADCSHORTTH__2V66 \
    (0x0F) /* ADC short detection threshold 2.6623 V */
#define EEP_ADCSHORTTH__2V50 (0x0E) /* ADC short detection threshold 2.504 V */
#define EEP_ADCSHORTTH__2V35 \
    (0x0D) /* ADC short detection threshold 2.3457 V */
#define EEP_ADCSHORTTH__2V19 \
    (0x0C) /* ADC short detection threshold 2.1874 V */
#define EEP_ADCSHORTTH__2V03 \
    (0x0B) /* ADC short detection threshold 2.0291 V */
#define EEP_ADCSHORTTH__1V87 \
    (0x0A) /* ADC short detection threshold 1.8708 V */
#define EEP_ADCSHORTTH__1V71 \
    (0x09) /* ADC short detection threshold 1.7125 V */
#define EEP_ADCSHORTTH__1V55 \
    (0x08) /* ADC short detection threshold 1.5542 V */
#define EEP_ADCSHORTTH__1V40 \
    (0x07) /* ADC short detection threshold 1.3959 V */
#define EEP_ADCSHORTTH__1V24 \
    (0x06) /* ADC short detection threshold 1.2376 V */
#define EEP_ADCSHORTTH__1V08 \
    (0x05) /* ADC short detection threshold 1.0793 V */
#define EEP_ADCSHORTTH__0V92 (0x04) /* ADC short detection threshold 0.921 V */
#define EEP_ADCSHORTTH__0V76 \
    (0x03) /* ADC short detection threshold 0.7627 V */
#define EEP_ADCSHORTTH__0V60 \
    (0x02) /* ADC short detection threshold 0.6044 V */
#define EEP_ADCSHORTTH__0V45 \
    (0x01) /* ADC short detection threshold 0.4461 V */
#define EEP_ADCSHORTTH__0V29 \
    (0x00) /* ADC short detection threshold 0.2878 V */

/* EEPM15 register */
#define EEP_CRC (0xFF) /* CRC reference for all EEPROM register */

#endif /* TPS929121_H_ */
