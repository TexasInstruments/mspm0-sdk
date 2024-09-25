/*
 * Copyright (c) 2022, Texas Instruments Incorporated
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
//*****************************************************************************
//
//  lcd.h
//
//*****************************************************************************

#ifndef LCD_H_
#define LCD_H_

#include "ti_msp_dl_config.h"

#define ASCII_DIGIT_OFFSET (48)
#define ASCII_LETTER_OFFSET (65)

typedef struct {
    uint32_t pin1;
    uint32_t pin2;
    uint32_t pin3;
    uint32_t pin4;
} LCD_pin;

extern LCD_pin gLCDPinPosition1;
extern LCD_pin gLCDPinPosition2;
extern LCD_pin gLCDPinPosition3;
extern LCD_pin gLCDPinPosition4;
extern LCD_pin gLCDPinPosition5;
extern LCD_pin gLCDPinPosition6;

extern char alphabetUpper[26][4];
extern char digit[10][4];

void LCD_showChar(LCD_Regs*, char, LCD_pin);

#endif /* LCD_H_ */
