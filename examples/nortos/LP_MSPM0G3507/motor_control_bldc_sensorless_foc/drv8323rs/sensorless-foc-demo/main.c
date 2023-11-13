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
#include "foc.h"

DRV8323RS_Instance drv8323rs;

FOC_Instance foc;

/* @brief Defines the ADC reset status */
#define ADC_STATUS_RESET                  (0x00)

/* @brief Defines the status when adc data is ready */
#define ADC0_DATA_READY                   (0x01)

/* @brief Defines the status when adc data is ready */
#define ADC1_DATA_READY                   (0x02)

/* @brief Defines the status when adc data is ready */
#define ADC_READY                         (0x03)

/** @brief Stores the status of ADC data availability   */
uint8_t ADCStatus = ADC_STATUS_RESET;

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
    drv8323rs.isenc   = HAL_ADC_CHANNEL_3;

    /* Initialize the DRV module */
    DRV8323RS_init(&drv8323rs);

    /* Assign the pins specific for FOC */
    foc.hal.pwmAHal     = HAL_PWM_CHANNEL_1;
    foc.hal.pwmBHal     = HAL_PWM_CHANNEL_2;
    foc.hal.pwmCHal     = HAL_PWM_CHANNEL_0;
    foc.hal.adcTrigHal  = HAL_PWM_CHANNEL_3;
    foc.hal.isenAComp   = HAL_COMP_CHANNEL_0;
    foc.hal.isenBComp   = HAL_COMP_CHANNEL_1;
    foc.hal.isenCComp   = HAL_COMP_CHANNEL_2;
    foc.hal.ipdTimer    = HAL_CAPTURE_CHANNEL_0;

    /* Set the drv handle in the foc instance */
    foc.drv_handle = (void *)&drv8323rs;

    /* Initialize the FOC module */
    FOC_init(&foc);

    /* Enabling the interrupts used by DRV module */
    HAL_enableADCInterrupt(drv8323rs.vsenvm);
    HAL_enableADCInterrupt(drv8323rs.isena);
    HAL_enableADCInterrupt(drv8323rs.isenb);

    /* Enable interrupts */
    NVIC_EnableIRQ(GENERIC_TIM_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GENERIC_PWM_0_INST_INT_IRQN);

    /* Set the priority for interrupts */
    NVIC_SetPriority(GENERIC_CAPTURE_0_INST_INT_IRQN, 0);
    NVIC_SetPriority(GENERIC_ADC_0_INST_INT_IRQN, 1);
    NVIC_SetPriority(GENERIC_ADC_1_INST_INT_IRQN, 1);
    NVIC_SetPriority(GENERIC_PWM_0_INST_INT_IRQN, 1);
    NVIC_SetPriority(GENERIC_TIM_0_INST_INT_IRQN, 2);

    while (1)
    {
        /* Handling the SPI register request from GUI */
        GUI_spiCommands(&drv8323rs);

        /* Check and update the parameters */
        FOC_paramsUpdateProcess(&foc, &userVar);
    }
}

void GENERIC_ADC_1_INST_IRQHandler(void)
{
    ADCStatus |= ADC1_DATA_READY;

    /* HAL layer collects the ADC data if HAL channel is defined in the
     HAL layer. The api also returns the pending IRQ, the user can use this 
     for any custom application */
    DL_ADC12_IIDX pendIrq = HAL_processADCIRQ(GENERIC_ADC_1_INST);
}

void GENERIC_ADC_0_INST_IRQHandler(void)
{
    ADCStatus |= ADC0_DATA_READY;

    /* HAL layer collects the ADC data if HAL channel is defined in the
    HAL layer. The api also returns the pending IRQ, the user can use this 
    for any custom application */
    DL_ADC12_IIDX pendIrq = HAL_processADCIRQ(GENERIC_ADC_0_INST);
}

void GENERIC_PWM_0_INST_IRQHandler(void)
{
    uint16_t vmAdc, iaAdc, ibAdc, icAdc;
    if(ADCStatus == ADC_READY)
    {
        vmAdc = DRV8323RS_getVMRaw(&drv8323rs);
        iaAdc = DRV8323RS_getIARaw(&drv8323rs);
        ibAdc = DRV8323RS_getIBRaw(&drv8323rs);
        icAdc = DRV8323RS_getICRaw(&drv8323rs);
        FOC_loop(&foc, vmAdc, iaAdc, ibAdc, icAdc);
        ADCStatus = ADC_STATUS_RESET;
    }
}

void GENERIC_TIM_0_INST_IRQHandler(void)
{
    FOC_CONTROL_run(&foc);
}

void GENERIC_CAPTURE_0_INST_IRQHandler(void)
{
    IPD_ISR(&ipd, &foc.hal);
}
