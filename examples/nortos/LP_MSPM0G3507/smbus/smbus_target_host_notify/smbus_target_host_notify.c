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
 *+----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details on example usage.
 * ---------------------------------------------------------------------------*/


#include "ti/smbus/smbus.h"
#include "SMBus_Commands_List.h"
#include "ti_msp_dl_config.h"

#if (SMB_TARGET_SUPPORTS_HOST_NOTIFY == false)
    #error "This example requires Host Notify function. Enable in smbus.h"
#endif

//
// Macros and definitions
//
#define SMB_DEFAULT_PEC_ENABLED         /*! Define to enable PEC by default */

//
//  Global variables
//
//  Variables used for SMBus functionality
static SMBus sSMBusTarget;                       /*! SMBus Structure           */
static uint8_t au8RxBuff[SMB_MAX_PACKET_SIZE];  /*! SMB Reception Buffer      */
static uint8_t au8TxBuff[SMB_MAX_PACKET_SIZE];  /*! SMB Transmission Buffer   */

static int8_t i8current_handler;                /*! Current command handler   */
static volatile bool bTriggerHostNotify = false;
static uint16_t au16Counter =0;
//
//  Function prototypes
//
static int8_t Demo_isCmd_valid(uint8_t cmd);
static int8_t Demo_CmdComplete(SMBus *SMB);

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Target demo testing Host Notify function.
//! The device recognizes "WRITE_BYTE" command 0x12 and toggles LED.
//! If the LaunchPad button is pressed, a Host Notify message will be sent to
//! the Controller.
//!
//! \return None
//
// *****************************************************************************
int main(void)
{

    SYSCFG_DL_init();
    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    SMBus_targetInit(&sSMBusTarget, SMB_I2C_INST);
    SMBus_targetSetAddress(&sSMBusTarget, SMB_I2C_TARGET_OWN_ADDR);

    // Set the RX and TX buffers for SMBus layers
    SMBus_targetSetRxBuffer(&sSMBusTarget, au8RxBuff, sizeof(au8RxBuff));
    SMBus_targetSetTxBuffer(&sSMBusTarget, au8TxBuff, sizeof(au8TxBuff));

    SMBus_targetEnableInt(&sSMBusTarget);

    #ifdef SMB_DEFAULT_PEC_ENABLED
    SMBus_enablePEC(&sSMBusTarget);  // Enable Packet Error Check
    #endif

    /* Enable GPIO Interrupt to trigger Alert Notify */
    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    while(1)
    {
        if (bTriggerHostNotify == true)
        {
            /*
             * This example sends an incrementing counter for host notify
             */
            au8TxBuff[0] = (au16Counter&0xFF);
            au8TxBuff[1] = (au16Counter&0xFF00)>>8;
            au16Counter++;
            SMBus_targetHostAlert(&sSMBusTarget,
                                 SMB_I2C_TARGET_OWN_ADDR,
                                 au8TxBuff);
            bTriggerHostNotify = false;
        }
        __WFE();
    }
}

//*****************************************************************************
//
//! Control LED
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command controls the LED
//!
//! Format:
//!     S + Target_Addr+W+A + CMD+A + VAL+A + {PEC+A} + P
//!    CMD = 0x12
//!     VAL = 0 to clear LED, 1 or any to set it
//!
//! \return  SMBUS_RET_OK if pointer was updated correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Control_LED_hdlr(SMBus * SMB){
    if (SMB->nwk.rxBuffPtr[0] == 0x12)
    {
        if (SMB->nwk.rxBuffPtr[1] == 0x00)
        {
            DL_GPIO_clearPins(SMB_GPIO_PORT, SMB_GPIO_LED_DEMO_PIN);
        }
        else
        {
            DL_GPIO_setPins(SMB_GPIO_PORT, SMB_GPIO_LED_DEMO_PIN);
        }
        return(SMBUS_RET_OK);
    }
    else
    {
        return(SMBUS_RET_ERROR);    /* Invalid command */
    }
}

//
// Local functions
//
//*****************************************************************************
//
//! Validates a command
//!
//! \param cmd  Received command
//!
//! This function is called when a command is received and the target needs to
//! check if the command is valid or not.
//! The function checks a list of valid commands (SMB_CMD_List) in order to
//! validate the current command, and if the command is found, it gets its
//! associated handler.
//!
//! \return  SMBUS_RET_OK if command was found
//!          SMBUS_RET_ERROR if command is invalid
//
// *****************************************************************************
static int8_t Demo_isCmd_valid(uint8_t cmd)
{
    uint8_t i;
    int8_t ret;
    i8current_handler = -1; // Handler is invalid by default

    for(i = 0; i < (CMD_LIST_LENGTH); i++)
    {
        if((cmd | SMB_CMD_List[i].mask) ==
           (SMB_CMD_List[i].cmd | SMB_CMD_List[i].mask))
        {
            // If the command is valid, save the handler which will be executed
            // if the command is received completely in Demo_CmdComplete
            i8current_handler = i;
            ret = (SMB_CMD_List[i].length == SMBUS_BLOCK_LENGTH)?SMBUS_RET_OK_BLOCK:SMBUS_RET_OK_FIXED;
            return ret;
        }
    }

    return(SMBUS_RET_ERROR);
}

//*****************************************************************************
//
//! Returns the length associated with a given command
//!
//! \param cmd  Received command
//!
//! This function is called when a command is valid and the target needs to
//! set the correct length for the PECCNT
//! The function returns the length for the previously processed command, and
//! must be called after Demo_isCmd_valid
//!
//! \return  Length of command if command was found
//!          0 if command is not found
//
// *****************************************************************************
static uint16_t Demo_getCurrentCmdLength(void){
    if(i8current_handler >= 0){
        return SMB_CMD_List[i8current_handler].length;
    } else {
        return 0;
    }
}

//*****************************************************************************
//
//! Command complete callback
//!
//! \param SMB  SMBus structure which received the command
//!
//! This function is called when a received packet is ready to process
//! after a STOP or RE-START.
//! The function executes the corresponding function if it was found by
//! Demo_isCmd_valid().
//! This demo implements a list of commands in SMB_CMD_List but the function
//! can be customized according to particular needs (i.e. switch statement or
//! hardcoded response).
//!
//! \return  SMBUS_RET_OK if command executed successfully
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
static int8_t Demo_CmdComplete(SMBus *SMB)
{
    int8_t ret;
    // i8current_handler is set by Demo_isCmd_valid when a command is received
    if(i8current_handler != -1)
    {
        // If the command was found as valid, call the corresponding handler
        ret = SMB_CMD_List[i8current_handler].handler(SMB);
        i8current_handler = -1;   // Invalidate the handler after execution
    }
    return(ret);
}

//
// Events
//
//*****************************************************************************
//
//! I2C Event
//!
//! Event called by HAL when I2C TX, RX, Start, Stop and errors are detected
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
void SMB_I2C_INST_IRQHandler (void)
{
    int8_t command_ret;
    // Check the state of SMBus
    switch(SMBus_targetProcessInt(&sSMBusTarget))
    {
    case SMBus_State_Target_QCMD:
        /* Not implemented */
        break;
    case SMBus_State_Target_CmdComplete:
        // If any other command besides Quick Command is detected, process it
        if(Demo_CmdComplete(&sSMBusTarget) == SMBUS_RET_ERROR)
        {
            SMBus_targetReportError(&sSMBusTarget, SMBus_ErrorCode_Packet);
            __BKPT(0);
        }
        break;
    case SMBus_State_Target_FirstByte:
        // Validate the command here
        command_ret = Demo_isCmd_valid(SMBus_targetGetCommand(&sSMBusTarget));

        if(command_ret != SMBUS_RET_OK_FIXED &&
           command_ret != SMBUS_RET_OK_BLOCK)
        {
            SMBus_targetReportError(&sSMBusTarget, SMBus_ErrorCode_Cmd);
            DL_GPIO_togglePins(SMB_GPIO_PORT, SMB_GPIO_LED_ERROR_PIN);
            __NOP();
        }
        else if (command_ret == SMBUS_RET_OK_BLOCK) {
            SMBus_targetReportBlock(&sSMBusTarget);
        }
        else if (command_ret == SMBUS_RET_OK_FIXED) {
            SMBus_targetReportLength(&sSMBusTarget, Demo_getCurrentCmdLength());
        }
        break;
    case SMBus_State_DataSizeError:
    case SMBus_State_PECError:
    case SMBus_State_Target_Error:
    case SMBus_State_TimeOutError:
        // This demo simply updates the Status register when an error is
        // detected. The Host can check this register
        break;
    default:
        break;
    }
    // Clear flags to be ready for next packet
    SMBus_processDone(&sSMBusTarget);
}


//*****************************************************************************
//
//! GROUP1 ISR
//!
//! Used to check GPIO Interrupt
//!     USER_SWITCH_1 will trigger a Host Notify
//!
//! \return  None
//
// *****************************************************************************
void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case  GPIO_SWITCHES_USER_SWITCH_1_IIDX:
                    /* Trigger a Host Notify alert when button is pressed */
                    bTriggerHostNotify = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}


