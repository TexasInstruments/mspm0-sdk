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
volatile dali_303_TimerCounter g303_TimerCounter;
volatile dali_occupancySensorVar gOccupancySensorVar1;

PIR_configRegister gPIR_configRegister;

volatile bool gReceive = false;

volatile DALI_GPIO dali_gpio;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(TIMER_DALI_RX_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_DALI_TX_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_FR_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_303_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_PIR_SERIN_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_PIR_INT_IRQN);

    /* DALI Control Device Initialization */
    DALI_ControlDevice_init();

    /* User can modify these values for configuring the PIR sensor */
    gPIR_configRegister.threshold       = 127;
    gPIR_configRegister.blindTime       = 1;
    gPIR_configRegister.pulseCounter    = 0;
    gPIR_configRegister.windowTime      = 0;
    gPIR_configRegister.operationMode   = 2;
    gPIR_configRegister.signalSource    = 0;
    gPIR_configRegister.hpfCutOff       = 0;
    gPIR_configRegister.countMode       = 0;

    /* PIR Initialization */
    DALI_303_init(&gPIR_configRegister);

    while(1)
    {
        DALI_ControlDevice_receive();

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

/* TIMER 303 ISR (10ms Interval) */
void TIMER_303_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_303_INST))
    {
        case DL_TIMER_IIDX_ZERO:
            if(g303_TimerCounter.deadtimeCounter == 1)
            {
                gOccupancySensorVar1.isDeadtimeOn = false;
            }
            
            if(g303_TimerCounter.holdCounter == 1)
            {
                gControlDeviceVar1.instance[PIR_INSTANCE_INDEX].inputValue = VACANT_NO_MOVEMENT;
            }

            if(g303_TimerCounter.reportCounter == 1)
            {
                DALI_ControlDevice_EventMessages(PIR_INSTANCE_INDEX);
                g303_TimerCounter.reportCounter = DALI_303_getCounterValue((uint32_t)DALI_303_REPORT_INCREMENT * (uint32_t)gOccupancySensorVar1.tReport);
            }
            
            if(g303_TimerCounter.blindTimeCounter == 1)
            {
                gControlDeviceVar1.instance[PIR_INSTANCE_INDEX].inputValue = OCCCUPIED_NO_MOVEMENT;
                g303_TimerCounter.holdCounter = DALI_303_getCounterValue((uint32_t)DALI_303_HOLD_INCREMENT * (uint32_t)gOccupancySensorVar1.tHold);
            }
            
            if(g303_TimerCounter.deadtimeCounter > 0)
                g303_TimerCounter.deadtimeCounter--;
                
            if(g303_TimerCounter.reportCounter > 0)
                g303_TimerCounter.reportCounter--;
            
            if(g303_TimerCounter.holdCounter > 0)
                g303_TimerCounter.holdCounter--;

            if(g303_TimerCounter.blindTimeCounter > 0)
                g303_TimerCounter.blindTimeCounter--;


            if(g303_TimerCounter.deadtimeCounter == 0  && 
               g303_TimerCounter.holdCounter == 0 &&
               g303_TimerCounter.reportCounter == 0 )
               {
                DL_Timer_stopCounter(TIMER_303_INST);
               }
            break;

        default:
            break;
    }
}

/* ISR to capture Interrupts from PIR */
void GROUP1_IRQHandler(void)
{
    uint32_t gpioA = DL_GPIO_getEnabledInterruptStatus(
        GPIO_PIR_DIRECT_LINK_PORT, GPIO_PIR_DIRECT_LINK_PIN);

    if ((gpioA & GPIO_PIR_DIRECT_LINK_PIN) ==
                 GPIO_PIR_DIRECT_LINK_PIN) {
        DALI_303_PIR_readInput();
    }

    return;
}
