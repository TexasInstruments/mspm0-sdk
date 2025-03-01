/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#include <ti/dali/dali_gpio_comm.h>
#include <ti/dali/dali_target_comm.h>
#include "ti_msp_dl_config.h"

#define DATA_SIZE 2

volatile bool gReceive = false;

volatile dali_controlGearVar gControlVar1;
volatile dali_Rx gDALI_Rx;

uint8_t receivedData[DATA_SIZE] = {0};

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_DALI_RX_INST_INT_IRQN);
    /*DALI initialization
    wait for query regarding address and send address to controller*/
    DALI_Target_init();
    while(true){
        /* Check if there are any failures, and if so, address immediately */
        DALI_Target_failure();
        /* Receive data from master */
        DALI_Target_receive(&receivedData[0]);
        /* Transmit a response if required */
        DALI_Target_transmit();
        /* Check if information needs to be updated/backed up to flash */
        DALI_Target_flashUpdate();

    }
}

void TIMER_DALI_RX_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_DALI_RX_INST))
    {
        /* Rising Edge */
        case DL_TIMER_IIDX_CC1_UP:
            /* Reset Timer Counter */
            DL_TimerG_setTimerCount(TIMER_DALI_RX_INST, 0);

            /* Check if index is within array bounds */
            if(gDALI_Rx.captIndex < MAX_CAPTURE_BITS_LENGTH)
            {
                gDALI_Rx.captBits[gDALI_Rx.captIndex] = 0;
                gDALI_Rx.captBitTimings[gDALI_Rx.captIndex++] = DL_TimerG_getCaptureCompareValue(TIMER_DALI_RX_INST,DL_TIMER_CC_1_INDEX) \
                                                                - DL_TimerG_getCaptureCompareValue(TIMER_DALI_RX_INST,DL_TIMER_CC_0_INDEX);
            }
            break;

        /* Falling Edge */
        case DL_TIMER_IIDX_CC0_UP:
            /* Setting Trigger for Rising Edge */
            DL_TimerG_setCaptureCompareCtl(TIMER_DALI_RX_INST,
            DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_NONE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE),
            DL_TIMER_CC_1_INDEX);

            /* Check if index is within array bounds */
            if(gDALI_Rx.captIndex < MAX_CAPTURE_BITS_LENGTH)
            {
                gDALI_Rx.captBits[gDALI_Rx.captIndex] = 1;
                gDALI_Rx.captBitTimings[gDALI_Rx.captIndex++] = DL_TimerG_getCaptureCompareValue(TIMER_DALI_RX_INST,DL_TIMER_CC_0_INDEX);
            }
            break;

        /* Stop Condition */
        case DL_TIMER_IIDX_LOAD:
            DL_TimerG_stopCounter(TIMER_DALI_RX_INST);

            /* Verify if it's a proper frame */
            if(DALI_RX_decodeFrame())
            {
                gReceive = true;
                receivedData[0] = gDALI_Rx.dataArr[0];
                receivedData[1] = gDALI_Rx.dataArr[1];
            }
            else
            {
                /* Handle Rx Error Case */
            }

            gDALI_Rx.captIndex = 0;

            /* Re-Configure ZCOND to start the timer for Rising edge */
            DL_TimerG_setCaptureCompareCtl(TIMER_DALI_RX_INST,
            DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_TRIG_RISE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE),
            DL_TIMER_CC_1_INDEX);
            break;
        
        default:
            break;
    }
}




