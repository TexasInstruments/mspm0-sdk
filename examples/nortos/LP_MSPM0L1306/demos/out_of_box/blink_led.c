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
//  blink_led.c
//
//  Toggle LED2 at a frequency that is configurable by the accompanying GUI.
//  By default, LED2 is set to green and toggles every 500ms.
//*****************************************************************************

#include "ti_msp_dl_config.h"

#include "blink_led.h"
#include "demo_mode_declaration.h"

#include "callbacks_mpack.h"

/* Used to calculate new toggle frequency based on GUI input */
#define CLOCK_FREQ (15686)

/* Flag to track the status of LED2 */
static volatile bool ledOn = false;

/* Flag to determine when to update the GUI LED */
static volatile bool updateGUILED = false;

static void setupExample(void);
static void cleanupExample(void);

static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableBlinkLED) {
        GUIComm_sendBool("blinkEnable", STR_LEN_ELEVEN, true);

        bEnableBlinkLED = true;
    }

    /* LED2 is off */
    ledOn = false;

    updateGUILED = false;

    /* The GUI slider widget should start at the default of 0.5 sec */
    GUIComm_sendBool("blinkTick", STR_LEN_NINE, (int16_t) _Q(0.5));

    /* Initialize peripherals */
    blink_led_init();

    /* Allow interrupt to propagate to the CPU */
    NVIC_EnableIRQ(BLINK_LED_TIMER_INST_INT_IRQN);

    /* Start the timer that controls the LED blink frequency */
    DL_TimerG_startCounter(BLINK_LED_TIMER_INST);
}

void blink_led_runExample(void)
{
    setupExample();

    while (gDemoMode == BLINK_LED_MODE) {
        while (!updateGUILED) {
            __WFE();
        }

        /* LED2 was toggled in the IRQ. Update status and send to the GUI */
        ledOn ^= 0x1;
        GUIComm_sendBool("ledOn", STR_LEN_FIVE, ledOn);

        updateGUILED = false;
    }

    cleanupExample();
}

void BLINK_LED_TIMER_INST_IRQHandler(void)
{
    /*
     * This IRQ is triggered whenever the timer counts down to zero
     * (zero event). The IRQ checks whether the user changed the LED
     * blink rate (only configurable in the GUI), recalculates the
     * timer counter value based on the new blink rate, and toggles the LED.
     */
    switch (DL_TimerG_getPendingInterrupt(BLINK_LED_TIMER_INST)) {
        case DL_TIMER_IIDX_ZERO:

            /* Check whether the user changed the blink rate in the GUI */
            if (bChangeBlinkFreq) {
                /*
                 * Stop counter to prevent reading LOAD value while it's
                 * being set.
                 */
                DL_TimerG_stopCounter(BLINK_LED_TIMER_INST);

                /*
                 * Set new blink frequency based on GUI input.
                 * Subtract 1 due to N+1 ticks
                 */
                uint16_t count = (CLOCK_FREQ * fBlinkLEDDelay) - 1;

                DL_Timer_setLoadValue(BLINK_LED_TIMER_INST, count);
                DL_TimerG_startCounter(BLINK_LED_TIMER_INST);

                /* Reset flag */
                bChangeBlinkFreq = false;
            }

            DL_GPIO_togglePins(GPIO_RGB_PORT, GPIO_RGB_GREEN_PIN);

            updateGUILED = true;

            break;
        default:
            break;
    }
}

static void cleanupExample(void)
{
    /* Reset and power off timer peripheral */
    DL_TimerG_reset(BLINK_LED_TIMER_INST);
    DL_TimerG_disablePower(BLINK_LED_TIMER_INST);

    /* Disable and clear any pending timer interrupts at the NVIC level */
    NVIC_DisableIRQ(BLINK_LED_TIMER_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(BLINK_LED_TIMER_INST_INT_IRQN);

    /* Clear the LED pin and update the GUI status LED */
    DL_GPIO_clearPins(GPIO_RGB_PORT, GPIO_RGB_GREEN_PIN);
    GUIComm_sendBool("ledOn", STR_LEN_FIVE, false);

    /* Stop clock used by this example */
    DL_SYSCTL_disableMFCLK();

    /* Reset flags and GUI widgets */
    ledOn        = false;
    updateGUILED = false;

    GUIComm_sendBool("blinkEnable", STR_LEN_ELEVEN, false);
    bEnableBlinkLED = false;
}
