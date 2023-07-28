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
#include "drv8323rs-gui.h"

DRV8323RS_Instance drv8323rs;

FOC_Instance foc;

int main(void)
{
    SYSCFG_DL_init();

    /* Initialize the HAL module */
    HAL_init();

    /* Assign the pins specific for the DRV */
    drv8323rs.enable  = HAL_GPIO_OUT_PIN_0;
    drv8323rs.nfault  = HAL_PWM_FAULT_0;
    drv8323rs.spi     = HAL_SPI_CHANNEL_0;
    drv8323rs.spiCS   = HAL_SPI_CS_2;
    drv8323rs.vsenvm  = HAL_ADC_CHANNEL_1;
    drv8323rs.isena   = HAL_ADC_CHANNEL_2;
    drv8323rs.isenb   = HAL_ADC_CHANNEL_0;

    /* Initialize the DRV module */
    DRV8323RS_init(&drv8323rs);
    
    /* Initialize the GUI module */
    GUI_init(&foc, &drv8323rs);

    /* Assign the pins specific for FOC */
    foc.pwmAHal = HAL_PWM_CHANNEL_1;
    foc.pwmBHal = HAL_PWM_CHANNEL_2;
    foc.pwmCHal = HAL_PWM_CHANNEL_0;
    
    /* Initialize the FOC module */
    FOC_init(&foc);
    
    /* Set the priority for interrupts */
    NVIC_SetPriority(GENERIC_ADC_0_INST_INT_IRQN, 1);
    NVIC_SetPriority(GENERIC_ADC_1_INST_INT_IRQN, 1);

    while (1)
    {
        /* Handles the GUI changes */
        GUI_loop(&foc, &drv8323rs);

        /* Gets the GUI variables for current and voltage */
        GUI_getVars(&drv8323rs);

        /* Checks for any faults */
        GUI_checkFaults(&foc, &drv8323rs);

        /* Handling the SPI register request from GUI */
        GUI_spiCommands(&drv8323rs);
    }
}

void GENERIC_ADC_1_INST_IRQHandler(void)
{
    /* HAL layer collects the ADC data if HAL channel is defined in the
     HAL layer. The api also returns the pending IRQ, the user can use this 
     for any custom application */
    DL_ADC12_IIDX pendIrq = HAL_processADCIRQ(GENERIC_ADC_1_INST);
}

void GENERIC_ADC_0_INST_IRQHandler(void)
{
    uint16_t vmAdc, iaAdc, ibAdc;
    /* HAL layer collects the ADC data if HAL channel is defined in the
    HAL layer. The api also returns the pending IRQ, the user can use this 
    for any custom application */
    DL_ADC12_IIDX pendIrq = HAL_processADCIRQ(GENERIC_ADC_0_INST);
    vmAdc = DRV8323RS_getVMRaw(&drv8323rs);
    iaAdc = DRV8323RS_getIARaw(&drv8323rs);
    ibAdc = DRV8323RS_getIBRaw(&drv8323rs);
    FOC_loop(&foc, vmAdc, iaAdc, ibAdc);
}
