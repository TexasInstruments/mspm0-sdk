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

//*****************************************************************************
// the includes
//*****************************************************************************
#include "drv8317hevm_gui.h"

//*****************************************************************************
//local variables
//*****************************************************************************
drv8317_Instance    drv8317;
Halltrap_Instance   hallTrap;

int main(void)
{
    /* initialize the modules   */
    SYSCFG_DL_init();
    HAL_init();

    drv8317.drvoff      = HAL_GPIO_OUT_01;
    drv8317.nsleep      = HAL_GPIO_OUT_02;
    drv8317.Vsen        = HAL_ADC_CHAN_0;
    drv8317.VsenA       = HAL_ADC_CHAN_6;
    drv8317.VsenB       = HAL_ADC_CHAN_2;
    drv8317.VsenC       = HAL_ADC_CHAN_5;
    drv8317.IsenA       = HAL_ADC_CHAN_1;
    drv8317.IsenB       = HAL_ADC_CHAN_3;
    drv8317.IsenC       = HAL_ADC_CHAN_4;
    drv8317.faultIn     = HAL_FAULT_INPUT_1;

    hallTrap.hallA          = HAL_GPIO_IN_01;
    hallTrap.hallB          = HAL_GPIO_IN_02;
    hallTrap.hallC          = HAL_GPIO_IN_03;
    hallTrap.phaseA         = HAL_PWM_02;
    hallTrap.phaseB         = HAL_PWM_03;
    hallTrap.phaseC         = HAL_PWM_01;
    hallTrap.captureInput   = HAL_CAPTURE_TIMER_01;

    drv8317_init(&drv8317);
    Halltrap_Init(&hallTrap);

    /* Enable DRV   */
    drv8317_enable(&drv8317);

    /* set GUI parameters   */
    GUI_setupGuiValues(&drv8317);

    /* enable interrupt to increment or decrement the dutycycle */
    HAL_enablePWMInterrupt(HAL_PWM_03);

    HAL_clearTimerFault(drv8317.faultIn);
    Halltrap_disablePWM(&hallTrap);

    while(1)
    {
        GUI_changeCheck(&drv8317, &hallTrap);
    }
}

void PWM_0_INST_IRQHandler(void)
{
    GUI_accMotor(&hallTrap);
    guiSpeedTimeout++;

    if(guiSpeedTimeout >= firmVar.pwmFreq)
    {
        guiMotorSpeed = 0;
    }
}

void GROUP1_IRQHandler(void)
{
    if(motorState == GUI_MOTOR_RUN)
    {
        Halltrap_PWMUpdate(&hallTrap, firmVar.motorDirection, firmVar.pulseWidth);
    }
    else
    {
        Halltrap_stopMotor(&hallTrap, firmVar.motorBraketype);
    }
}

void GEN_ADC_CHAN_0_INST_IRQHandler(void)
{
    HAL_ADC_IIDX pend_irq = HAL_processADCIRQ(GEN_ADC_CHAN_0_INST);
    guiADCStatus |= GUI_ADC0_DATA_READY;
}

void GEN_ADC_CHAN_1_INST_IRQHandler(void)
{
    HAL_ADC_IIDX pend_irq = HAL_processADCIRQ(GEN_ADC_CHAN_1_INST);
    guiADCStatus |= GUI_ADC1_DATA_READY;
}

void CAPTURE_0_INST_IRQHandler(void)
{
    guiMotorSpeed = Halltrap_calculateMotorSpeed(&hallTrap, guiMotorPoles);
    guiSpeedTimeout = 0;
}