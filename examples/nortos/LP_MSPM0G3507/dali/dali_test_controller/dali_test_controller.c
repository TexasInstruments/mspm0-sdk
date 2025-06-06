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

#include "ti_msp_dl_config.h"


#include <ti/dali/dali_103/dali_cd_comm.h>

volatile dali_controlDeviceVar gControlDeviceVar1;
volatile TickCounter gTickCounter;
volatile bool gReceive = false;
volatile DALI_GPIO dali_gpio;

volatile userVars gUserVar;

volatile bool gSendConfigEvent;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(TIMER_DALI_RX_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_DALI_TX_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_FR_INST_INT_IRQN);

    /* DALI Control Device Initialization */
    DALI_ControlDevice_init();

    /* Initialising gUserVar with Reset Command */
    /* Broadcast Address Scheme */
    gUserVar.addrByte = 0xFF;
    gUserVar.instanceByte = 0xFE;
    gUserVar.opcodeByte = 0x10;
    gUserVar.isSendTwiceCmd = true;


    while(1)
    {
        if(gUserVar.toSend)
        {
            DALI_ControlDevice_testTransmit();
            gUserVar.toSend = false;
        }

        DALI_ControlDevice_transmit();
    }

}

/* TIMER FR ISR (10ms Interval) */
void TIMER_FR_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_FR_INST))
    {
        case DL_TIMER_IIDX_ZERO:
            if(gTickCounter.resetCounter == 1)
            {
                gControlDeviceVar1.resetState = false;
            }
            
            if(gTickCounter.quiescentMode == 1)
            {
                gControlDeviceVar1.quiescentMode = DISABLED;
            }

            if(gTickCounter.initialisationCounter == 1)
            {
                gControlDeviceVar1.initialisationState = DISABLED;
            }
            
            if(gTickCounter.quiescentMode > 0)
                gTickCounter.quiescentMode--;
                
            if(gTickCounter.resetCounter > 0)
                gTickCounter.resetCounter--;
            
            if(gTickCounter.initialisationCounter > 0)
                gTickCounter.initialisationCounter--;

            if(gTickCounter.quiescentMode == 0  && 
            gTickCounter.initialisationCounter == 0 &&
            gTickCounter.resetCounter == 0 )
            {
                DL_Timer_stopCounter(TIMER_FR_INST);
            }
            break;

        default:
            break;
    }
}

