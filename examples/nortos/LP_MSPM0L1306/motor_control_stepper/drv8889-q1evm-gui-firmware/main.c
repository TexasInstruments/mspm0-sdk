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
#include "drv8889-q1evm-gui.h"

DRV8889Q1_Instance drv8889q1;

int main(void)
{
    SYSCFG_DL_init();

    /* Initialize the HAL module */
    HAL_init();

    /* Assign the pins specific for the DRV */
    drv8889q1.dir               = HAL_GPIO_OUT_PIN_0;
    drv8889q1.drvOff            = HAL_GPIO_OUT_PIN_1;
    drv8889q1.nsleep            = HAL_GPIO_OUT_PIN_2;
    drv8889q1.nfault            = HAL_GPIO_IN_PIN_0;
    drv8889q1.vref              = HAL_DAC_CHANNEL_0;
    drv8889q1.step              = HAL_PWM_CHANNEL_0;
    drv8889q1.spi               = HAL_SPI_CHANNEL_0;
    drv8889q1.spiCS             = HAL_SPI_CS_2;

    /* Initialize the DRV module */
    DRV8889Q1_init(&drv8889q1);

    /* Initialize the GUI module */
    gui_init(&drv8889q1);

    /* Set the priority for interrupts */
    NVIC_SetPriority(GENERIC_PWM_CHANNEL_0_INST_INT_IRQN, 1);

    while(1)
    {
        /* Handles the GUI changes */
        gui_loop(&drv8889q1);

        /* Checks for any faults */
        gui_checkFaults(&drv8889q1);

        /* Checks for the reverse on fault condition */
        gui_CheckStallReverse(&drv8889q1);

        /* Handling the SPI register request from GUI */
        gui_spiCommands(&drv8889q1);
    }
}

void GENERIC_PWM_CHANNEL_0_INST_IRQHandler(void)
{
    DRV8889Q1_checkStep(&drv8889q1);
}
