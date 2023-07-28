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
#include "drv8411aevm-gui.h"

DRV8411A_Instance drv8411a;

int main(void)
{
    SYSCFG_DL_init();

    /* Initialize the HAL module */
    HAL_init();

    /* Assign the pins specific for the DRV */
    drv8411a.ain1               = HAL_GPIO_OUT_PIN_0;
    drv8411a.ain2               = HAL_GPIO_OUT_PIN_1;
    drv8411a.bin1               = HAL_GPIO_OUT_PIN_2;
    drv8411a.bin2               = HAL_GPIO_OUT_PIN_3;
    drv8411a.nfault             = HAL_GPIO_IN_PIN_0;
    drv8411a.aipropi            = HAL_ADC_CHANNEL_0;
    drv8411a.bipropi            = HAL_ADC_CHANNEL_1;
    drv8411a.vref               = HAL_DAC_CHANNEL_0;
    drv8411a.indexerTimer       = HAL_TIM_CHANNEL_0;
    drv8411a.ADCTriggerTimer    = HAL_TIM_CHANNEL_1;
    
    /* Initialize the DRV module */
    DRV8411A_init(&drv8411a);

    /* Initialize the GUI module */
    gui_init(&drv8411a);

    /* Enable the GUI plot timer interrupt */
    NVIC_EnableIRQ(GUI_PLOT_UPDATE_TIMER_INST_INT_IRQN);

    /* Set the priority for interrupts */
    NVIC_SetPriority(GENERIC_ADC_0_INST_INT_IRQN, 2);
    NVIC_SetPriority(GENERIC_TIM_CHANNEL_0_INST_INT_IRQN, 1);
    NVIC_SetPriority(GUI_PLOT_UPDATE_TIMER_INST_INT_IRQN, 3);

    while(1)
    {
        /* Handles the GUI changes */
        gui_loop(&drv8411a);

        /* Checks for any faults */
        gui_checkFaults(&drv8411a);

        /* Converts the Phase currents for the GUI */
        gui_convertPhaseCurrent(&drv8411a);
    }
}

void GENERIC_ADC_0_INST_IRQHandler(void)
{
    DL_ADC12_IIDX pendIrq = HAL_processADCIRQ(GENERIC_ADC_0_INST);
}

void GENERIC_TIM_CHANNEL_0_INST_IRQHandler(void)
{
    DRV8411A_setComm(&drv8411a);
}

void GUI_PLOT_UPDATE_TIMER_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(GUI_PLOT_UPDATE_TIMER_INST))
    {
        case DL_TIMER_IIDX_ZERO:
            gui_setUpdatePlotVarFlag();
            break;
        default:
            break;
    }
}
