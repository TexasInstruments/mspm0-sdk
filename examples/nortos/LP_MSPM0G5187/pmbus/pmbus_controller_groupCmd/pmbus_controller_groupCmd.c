/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

/*
 * ======== main.c ========
 * PMBus_controller
 *
 * This example communicates with the target devices using PMBus group command
 * showing basic calls of PMBusLib functions.
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include <ti/pmbus/pmbus.h>

#include "ti_msp_dl_config.h"

/*! SMBus Controller Struct  */
SMBus SMB;

//*****************************************************************************
//
//! Main Function
//!
//!  This example communicates with the target devices using PMBus group command
//!  showing basic calls of PMBusLib functions. The LED toggles if all
//!  communications work correctly. The LED is set on if there is a failure.
//! \return none
//
// *****************************************************************************

int8_t ret;
int main(void)
{
    uint32_t timeout = 800000; /* 10ms */

    uint8_t commands[2];
    uint8_t **txData;
    uint8_t *txSize;
    uint8_t *target;

    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    /* Initialize PMBus interface */
    PMBus_controllerInit(&SMB,
                         SMB_I2C_INST,
                         (uint32_t)CPUCLK_FREQ);

    /*
     * If desired, enable PEC. The target device will accept commands with or
     * without PEC, but it will only check PEC if it's sent/requested
     */
    PMBus_enablePEC(&SMB);

    txSize=( uint8_t *)malloc(sizeof( uint8_t)*2);
    txData=( uint8_t **)malloc(sizeof( uint8_t*)*2);
    txData[1]=( uint8_t *)malloc(sizeof( uint8_t)*1);
    txData[0]=( uint8_t *)malloc(sizeof( uint8_t)*2);
    target = ( uint8_t *)malloc(sizeof( uint8_t)*2);

    /* PMBus command for each target */
    commands[0] = PMB_VOUT_OV_WARN_LIMIT;
    commands[1] = PMB_VOUT_MODE;
    /* TX data for each target  */
    txData[0][0] = 0xCD;
    txData[0][1] = 0xAB;
    txData[1][0] = 0x01;
    /* TX size for each target  */
    txSize[0] = 2;
    txSize[1] = 1;
    /* Both target address  */
    target[0] = 0x1A;
    target[1] = 0x1B;

    ret = PMBus_cmdGroupCommand(&SMB,
                          target,
                          commands,
                          (uint8_t**)txData,
                          txSize,
                          2,                /* Number of targets    */
                          timeout);
    while(1)
    {
        if (ret == PMBUS_RET_OK)
        {
            /* toggle LED0 if command execution is  successful */
            DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_D0_PIN);
            delay_cycles(16000000); /* 200ms */
        }
        else
        {
            /* Set LED0 if data command execution fails */
            DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_D0_PIN);
        }
    } 
}

//
// Events
//
//*****************************************************************************
//
//! I2C Event
//!
//! Event called when I2C TX, RX, Start, Stop and errors are detected
//!
//
// *****************************************************************************
void SMB_I2C_INST_IRQHandler(void) {

    PMBus_controllerProcessInt(&SMB);
}
