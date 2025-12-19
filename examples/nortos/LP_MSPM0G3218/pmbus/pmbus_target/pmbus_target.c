/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

/*
 * PMBus_Target
 *
 * This example acts as a PMBus target and responds to controller for the below
 * commands
 *
 *  PMBus CMD          | CMD ID |  Write Type | Read Type
 *  ------------------------------------------------------
 *  Device ID          | 0xAD   |  -          | Block read
 *  Clear Faults       | 0x03   | Send Byte   | -
 *  Vout Mode          | 0x020  | Write Byte  | Read Byte
 *  Vout OV warn Limit | 0x42   | Write Word  | Read Word
 *  MRF ID             | 0x99   | Block Write | Block Read
 *  Read Vout          | 0x8B   | -           | Read Word
 *  ------------------------------------------------------
 *  Extended Byte commands - user defined
 *  ------------------------------------------------------
 *  EXT_BYTE_CMD       | 0x01FF | Write Byte  | Read Byte
 *  EXT_WORD_CMD       | 0x02FF | Write Word  | Read Word
 *  ------------------------------------------------------
 *  User defined commands
 *  ------------------------------------------------------
 *  Process call Byte  | 0xD0   | Write Word  | Read Word
 *  Process call Block | 0xD1   | Block Write | Block Read
 *
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include "ti/pmbus/pmbus.h"
#include "ti_msp_dl_config.h"
#include "pmbus_cmd_handlers.h"
#include "pmbus_cmd_list.h"

static SMBus SMB;
static int8_t extendedCommand = -1;

//*****************************************************************************
//
//! Main Function
//!
//!
//!
//! \return none
//
// *****************************************************************************
int main(void)
{
    SYSCFG_DL_init();

    /* Enable SMBus I2C interrupts  */
    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    /* Enable ADC interrupts    */
    NVIC_ClearPendingIRQ(SMB_ADC_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_ADC_INST_INT_IRQN);

    /* Initialize SMB I2C as PMBus Traget   */
    PMBus_targetInit(&SMB, SMB_I2C_INST, SMB_I2C_TARGET_OWN_ADDR);

    /* Enable PEC   */
    PMBus_enablePEC(&SMB);

    while(1)
    {
        __WFE();
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
void SMB_I2C_INST_IRQHandler(void)
{
    uint16_t cmd;
    SMBus_State SMBState = PMBus_targetProcessInt(&SMB);

    switch(SMBState)
    {
    case SMBus_State_Target_QCMD:
        PMBus_App_quickCommand(&SMB);
        break;
    case SMBus_State_Target_FirstByte:
        cmd = PMBus_getTargetCommand(&SMB);

        if((cmd != PMBUS_COMMAND_EXT) && (cmd != MFR_SPECIFIC_COMMAND_EXT))
        {
            PMBus_App_isCommandValid(&SMB);
        }
        else
        {
            extendedCommand = 1;
        }

        break;
    case SMBus_State_Target_ByteReceived:
        if (extendedCommand == 1)
        {
            PMBus_App_isExtendedCommandValid(&SMB);
            extendedCommand = -1;
        }

        break;
    case SMBus_State_Target_CmdComplete:
        PMBus_App_processCommand(&SMB);
        break;
    case SMBus_State_Target_Error:

        break;
    default :

        break;
    }

    PMBus_processDone(&SMB);
}

//*****************************************************************************
//
//! ADC ISR
//!
//! Reads current ADC result and stores it
//!
//
// *****************************************************************************
void SMB_ADC_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(SMB_ADC_INST))
    {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            outputVoltage = DL_ADC12_getMemResult(SMB_ADC_INST, SMB_ADC_ADCMEM_SMB_ADC_channel_0);
            break;
        default:
            break;
    }
}

