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

#include "ti_msp_dl_config.h"

/*
* Initializing the variables that will help count through what sequences
* have turned on and also the different timer values. The first timer 
* will be set from SysConfig but the rest are configured here.
*/
int gPinCount = 0;
bool gStartUp = true;
/* Set this to true to turn all the pins off at the same time */
bool gQuickOff = false;
/* Divide the desired time by 7.81ms to get the period count for the arrays */
int gTimerUp[4] = {64, 51, 77, 102};
int gTimerDown[4] = {13, 25, 38, 51};

void counting(void);
void pinToggle(void);

int main(void)
{
    SYSCFG_DL_init();
    DL_GPIO_clearPins(GPIO_OUT_PORT, GPIO_OUT_PIN_0_PIN | GPIO_OUT_PIN_1_PIN | GPIO_OUT_PIN_2_PIN | GPIO_OUT_PIN_3_PIN);

    NVIC_EnableIRQ(GPIO_INPUT_INT_IRQN);
    NVIC_EnableIRQ(TIME_SEQUENCE_INST_INT_IRQN);


    while (1){
        __WFI();
    }    
}

void GROUP1_IRQHandler(void)
{
    /* Get the interrupt status of the input pin */
    uint32_t gpioInterruptStatus = DL_GPIO_getEnabledInterruptStatus(GPIO_INPUT_PORT,
            GPIO_INPUT_USER_INPUT_1_PIN);

    if ((gpioInterruptStatus & GPIO_INPUT_USER_INPUT_1_PIN) ==
            GPIO_INPUT_USER_INPUT_1_PIN) {
        counting();
    }
    else{
        ; /* If there is no trigger on the input pin, do nothing */
    }
}

void TIME_SEQUENCE_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIME_SEQUENCE_INST)) {
        case DL_TIMER_IIDX_ZERO:
            pinToggle();
            break;
        default:
            break;
    }
}

void counting(void)
{
    if (gStartUp == true){
        DL_TimerG_stopCounter(TIME_SEQUENCE_INST);
        DL_TimerG_setLoadValue(TIME_SEQUENCE_INST, gTimerUp[gPinCount]);
        DL_TimerG_startCounter(TIME_SEQUENCE_INST);
    } else {
        DL_TimerG_stopCounter(TIME_SEQUENCE_INST);
        DL_TimerG_setLoadValue(TIME_SEQUENCE_INST, gTimerDown[gPinCount]);
        DL_TimerG_startCounter(TIME_SEQUENCE_INST);
    }
}

void pinToggle(void)
{
    /* Check to see if the pins have been set high */
    if (gStartUp == true){
        if (gPinCount == 0){
            DL_GPIO_setPins(GPIO_OUT_PORT, GPIO_OUT_PIN_0_PIN);
        } else if (gPinCount == 1){
            DL_GPIO_setPins(GPIO_OUT_PORT, GPIO_OUT_PIN_1_PIN);
        } else if (gPinCount == 2){
            DL_GPIO_setPins(GPIO_OUT_PORT, GPIO_OUT_PIN_2_PIN);
        } else if (gPinCount == 3){
            DL_GPIO_setPins(GPIO_OUT_PORT, GPIO_OUT_PIN_3_PIN);
        } else {
            gStartUp = false;
            gPinCount = 0;
            return;
        }
        gPinCount += 1;
    /* Otherwise they are being turned off */
    } else {
        /* Check to see if the pins are being turned off all at once */
        if (gQuickOff == true){
            DL_GPIO_clearPins(GPIO_OUT_PORT, GPIO_OUT_PIN_0_PIN | GPIO_OUT_PIN_1_PIN | GPIO_OUT_PIN_2_PIN | GPIO_OUT_PIN_3_PIN);
            gStartUp = true;
        /* Otherwise they are turned off in sequence */
        } else {
            if (gPinCount == 0){
                DL_GPIO_clearPins(GPIO_OUT_PORT, GPIO_OUT_PIN_0_PIN);
            } else if (gPinCount == 1){
                DL_GPIO_clearPins(GPIO_OUT_PORT, GPIO_OUT_PIN_1_PIN);
            } else if (gPinCount == 2){
                DL_GPIO_clearPins(GPIO_OUT_PORT, GPIO_OUT_PIN_2_PIN);
            } else if (gPinCount == 3){
                DL_GPIO_clearPins(GPIO_OUT_PORT, GPIO_OUT_PIN_3_PIN);
            } else {
                gStartUp = true;
                gPinCount = 0;
                return;
            }
            gPinCount += 1;
        }
    }
    counting();
}
