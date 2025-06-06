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
//******************************************************************************
//
//  button_counter.c
//
//  To increment counter as switch pressed on LaunchPad.
//
//******************************************************************************

#include "ti_msp_dl_config.h"
#include "demo_mode_declaration.h"
#include "callbacks_mpack.h"
#include "button_counter.h"

static void setupExample(void);
static void cleanupExample(void);

/* Counter to increment when S2 pressed */
static volatile uint16_t count = 0;


static void setupExample(void)
{
    /*
     * If the user changed examples using the switch, make sure that the
     * enable slider button is also updated
     */
    if (!bEnableButton) {
        GUIComm_sendBool("buttonEnable", STR_LEN_TWELVE, true);
        bEnableButton = true;
    }

    bEnableButton = true;
    count = 0;
    GUIComm_sendUInt("counter", STR_LEN_SEVEN, count);

    /* Enable Button Interrupt */
    NVIC_EnableIRQ(SWITCHES_USER_SWITCH_1_INT_IRQN);

}

void button_counter_runExample(void)
{
    setupExample();

    while (gDemoMode == BUTTON_COUNTER_MODE) {

    }

    cleanupExample();
}

static void cleanupExample(void)
{
    /* Disable Button Interrupt */
    NVIC_DisableIRQ(SWITCHES_USER_SWITCH_1_INT_IRQN);
    NVIC_ClearPendingIRQ(SWITCHES_USER_SWITCH_1_INT_IRQN);

    GUIComm_sendBool("buttonEnable", STR_LEN_TWELVE, false);
    bEnableButton = false;
}


/* Pressing S2 starts the example */
void GPIOA_IRQHandler(void)
{
    /*
     * This IRQ is triggered when S2 is pressed on the LaunchPad.
     */
    switch (DL_GPIO_getPendingInterrupt(GPIOA)) {
        case SWITCHES_USER_SWITCH_1_IIDX:
            /*
             * At the start, the demo is operating in IDLE mode.
             * Each time S2 is pressed, the counter will increment by 1.
             * The counter will keep incremented until it reaches 10, in
             * which case it will reset back to 0.
             */
            count++;
            if (count > MAX_COUNT) {
                count = 0;
            }

            GUIComm_sendUInt("counter", STR_LEN_SEVEN, count);
            break;
        default:
            break;
    }
}
