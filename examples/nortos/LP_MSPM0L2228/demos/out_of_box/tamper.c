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
//  tamper.c
//
//  An RTC time stamp will be taken when a tamper event is detected. The tamper
//  event is caused by a rising edge on the tamperIO pin, PA7. The RTC value
//  will be displayed on the LCD screen.
//*****************************************************************************

#include "ti_msp_dl_config.h"

#include "demo_mode_declaration.h"
#include "tamper.h"

#include "callbacks_mpack.h"

bool tamperClean;

/* Flag to track the status of LED2 */
static volatile bool gCheckTSEVT = false;

/* Flag to determine when to update the GUI LED */
static volatile bool updateGUILED = false;

static void setupExample(void);
static void cleanupExample(void);
static void updateLCD(void);

static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableTamper) {
        GUIComm_sendBool("tamperEnable", STR_LEN_TWELVE, true);
        bEnableTamper = true;
    }

    tamperClean = false;

    /* Initialize peripherals */
    tamper_init();

    /* Allow interrupt to propagate to the CPU */
    NVIC_EnableIRQ(RTC_A_INT_IRQn);

    /* Start the timer that controls the LED blink frequency */
    DL_RTC_A_enableClockControl(RTC_A);
}

void tamper_runExample(void)
{
    setupExample();

    while (gDemoMode == TAMPER_MODE) {
        while (!gCheckTSEVT && gDemoMode == TAMPER_MODE) {
            __WFE();
        }

        if (DL_RTC_A_getTimeStampEventCause(
                RTC_A, DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_0) ==
            DL_RTC_A_TIME_STAMP_EVENT_CAUSE_TIO_0) {
            updateLCD();
            gCheckTSEVT = false;
        }
    }
    cleanupExample();
}

void LFSS_IRQHandler()
{
    switch (DL_RTC_A_getPendingInterrupt(RTC_A)) {
        case DL_RTC_A_IIDX_TSEVT:
            gCheckTSEVT = true;
        default:
            break;
    }
}

void updateLCD()
{
    uint8_t HourH, HourL, MinH, MinL, SecH, SecL = 0;

    HourH = (RTC_A->TSHOUR & RTC_TSHOUR_HOURHIGHBCD_MASK) >> 12;
    HourL = (RTC_A->TSHOUR & RTC_TSHOUR_HOURLOWBCD_MASK) >> 8;
    MinH  = (RTC_A->TSMIN & RTC_TSMIN_MINHIGHBCD_MASK) >> 12;
    MinL  = (RTC_A->TSMIN & RTC_TSMIN_MINLOWBCD_MASK) >> 8;
    SecH  = (RTC_A->TSSEC & RTC_TSSEC_SECHIGHBCD_MASK) >> 12;
    SecL  = (RTC_A->TSSEC & RTC_TSSEC_SECLOWBCD_MASK) >> 8;

    /* Updates the GUI */
    GUIComm_sendUInt8("tHourH", STR_LEN_SIX, HourH);
    GUIComm_sendUInt8("tHourL", STR_LEN_SIX, HourL);
    GUIComm_sendUInt8("tMinH", STR_LEN_FIVE, MinH);
    GUIComm_sendUInt8("tMinL", STR_LEN_FIVE, MinL);
    GUIComm_sendUInt8("tSecH", STR_LEN_FIVE, SecH);
    GUIComm_sendUInt8("tSecL", STR_LEN_FIVE, SecL);

    LCD_showChar(LCD, HourH + 48, gLCDPinPosition1);
    LCD_showChar(LCD, HourL + 48, gLCDPinPosition2);
    LCD_showChar(LCD, MinH + 48, gLCDPinPosition3);
    LCD_showChar(LCD, MinL + 48, gLCDPinPosition4);
    LCD_showChar(LCD, SecH + 48, gLCDPinPosition5);
    LCD_showChar(LCD, SecL + 48, gLCDPinPosition6);
}

static void cleanupExample(void)
{
    /* Reset */
    tamperClean = true;
    DL_RTC_A_disableInterrupt(RTC_A, DL_RTC_A_INTERRUPT_TSEVT);
    DL_LFSS_reset(LFSS);
    DL_LCD_reset(LCD);
    DL_LCD_disablePower(LCD);

    /* Disable and clear any pending timer interrupts at the NVIC level */
    NVIC_DisableIRQ(RTC_A_INT_IRQn);
    NVIC_ClearPendingIRQ(RTC_A_INT_IRQn);

    bEnableTamper = false;
    GUIComm_sendBool("tamperEnable", STR_LEN_TWELVE, false);
    gCheckTSEVT = false;
}

void NMI_Handler()
{
    switch (DL_SYSCTL_getPendingNonMaskableInterrupt()) {
        case DL_SYSCTL_NMI_IIDX_VBAT_ON:
            if (tamperClean) {
                tamperClean = false;
            } else {
                while (1)
                    ;
            }
            break;
        case DL_SYSCTL_NMI_IIDX_VBAT_OFF:
            if (tamperClean) {
                ;
            } else {
                while (1)
                    ;
            }
            break;
        default:
            break;
    }
}
