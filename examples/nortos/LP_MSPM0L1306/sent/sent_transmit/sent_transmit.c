/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include "SENT/SENT.h"
#include "ti_msp_dl_config.h"

/* Flag for when to update timer period value for next nibble in frame */
volatile bool gUpdateSENT;

int main(void)
{
    SENT_config_obj config;
    SENT_Handler sentHandler;
    SENT_PACK sentData;
    SENT_PACK newData;
    bool toogleData;

    SYSCFG_DL_init();

    bool initStat;

    /* Initialize first packet of data */
    sentData.status = 0x3;
    sentData.d0     = 0x1;
    sentData.d1     = 0x2;
    sentData.d2     = 0x3;
    sentData.d3     = 0x4;
    sentData.d4     = 0x5;
    sentData.d5     = 0x6;

    /* Initialize second packet of data */
    newData.status = 0xf;
    newData.d0     = 0xf;
    newData.d1     = 0xf;
    newData.d2     = 0xf;
    newData.d3     = 0xf;
    newData.d4     = 0xf;
    newData.d5     = 0xf;

    /* Configure frame format parameters */
    config.tickDuration           = 3;
    config.pauseConfig.mode       = SENT_PAUSE_MODE_FIXED_FRAME;
    config.pauseConfig.numOfTicks = 500;
    config.numDataNibbles         = SENT_NUM_DATA_NIBBLES_6;

    initStat = SENT_init((SENT_config_obj*) &config, &sentHandler);

    gUpdateSENT = false;
    toogleData  = true;

    SENT_updateData(&sentHandler, &sentData);

    SENT_startTransfer(&sentHandler);

    if (initStat == false) {
        /* Initialization error occurred, trap execution */
        while (1)
            ;
    } else {
        gUpdateSENT = false;
        toogleData  = true;

        SENT_updateData(&sentHandler, &sentData);

        SENT_startTransfer(&sentHandler);

        /* Turn on LED when transfer starts */
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

        while (1) {
            while (gUpdateSENT == false) {
                __WFE();
            }
            gUpdateSENT = false;

            SENT_updateState(&sentHandler);

            /* Toggle between sending packets of data */
            if (sentHandler.sentState == SENT_STATE_PAUSE) {
                if (toogleData == true) {
                    SENT_updateData(&sentHandler, &newData);
                    toogleData = false;
                } else {
                    SENT_updateData(&sentHandler, &sentData);
                    toogleData = true;
                }
            }
        }
    }
}
