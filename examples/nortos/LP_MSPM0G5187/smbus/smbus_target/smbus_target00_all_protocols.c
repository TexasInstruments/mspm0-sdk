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
 * Please refer to the Examples Guide for more details.
 * ---------------------------------------------------------------------------*/


#include "ti/smbus/smbus.h"
#include "SMBus_Commands_List.h"
#include "ti_msp_dl_config.h"

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
//  Variables used for demo
static volatile uint16_t au16ADC_shadow;        /*! ADC value                 */
/*! String written/read on block Read/Write commands */
static uint8_t au8String_buf[SMB_MAX_PAYLOAD_SIZE] =
    "Default string for SMBusMSP Demo";
static const uint8_t u8Version = 0x16;           /*! Demo u8Version            */
static int8_t i8current_handler;                /*! Current command handler   */
static uint8_t scratchpad_byte;
static int64_t trxnCnt = 0;


//
//  Function prototypes
//
static int8_t Demo_isCmd_valid(uint8_t cmd);
static uint16_t Demo_getCurrentCmdLength(void);
static int8_t Demo_CmdComplete(SMBus *SMB);

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Target demo testing all SMBus 2.0 protocols
//! Waits for command from the SMBus Controller and processes it.
//! The commands supported are:
//! | Function         | CMD   |Type              | Description
//! | Quick_Led_toggle |  -    |QUICK_COMMAND     | Toggle LED
//! | Read_Reg_Ptr     |  -    |RECEIVE_BYTE      | Read Register pointer contents
//! | Set_Reg_Ptr      | 00-03 |SEND_BYTE         | Change Register pointer
//! | Write_Reg        | 10-13 |WRITE_BYTE        | Write Register
//! | Read_Reg         | 20-23 |READ_BYTE         | Read Register
//! | Read_ADC_Ch      | 30    |READ_WORD         | Read ADC
//! | Mult_Bytes       | 40    |PROCESS_CALL      | Multiplies 2 bytes
//! | Write_String     | 50    |BLOCK_WRITE       | Write a string
//! | Read_String      | 51    |BLOCK_READ        | Read the string
//! | Calc_CRC         | 60    |BLW_BLR_PROC_CALL | Multiplies 2 bytes
//!
//! A timer triggered ADC gets continuous samples automatically
//!
//!
//! \return None
//
// *****************************************************************************
int main(void)
{

    SYSCFG_DL_init();
    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    NVIC_ClearPendingIRQ(SMB_ADC_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_ADC_INST_INT_IRQN);

    SMBus_targetInit(&sSMBusTarget, SMB_I2C_INST);
    SMBus_targetSetAddress(&sSMBusTarget, SMB_I2C_TARGET_OWN_ADDR);

    // Set the RX and TX buffers for SMBus layers
    SMBus_targetSetRxBuffer(&sSMBusTarget, au8RxBuff, sizeof(au8RxBuff));
    SMBus_targetSetTxBuffer(&sSMBusTarget, au8TxBuff, sizeof(au8TxBuff));

    SMBus_targetEnableInt(&sSMBusTarget);

    #ifdef SMB_DEFAULT_PEC_ENABLED
    SMBus_enablePEC(&sSMBusTarget);  // Enable Packet Error Check
    #endif

    while(1)
    {
        __WFE();
    }
}

//*****************************************************************************
//
//! Quick Command
//!
//! \param Addr_RW  Received address + R/W bit
//!
//! This demo toggles LED0 when a Quick Command is received
//! Note that we can detect QuickCommand(W), but not QuickCommand(R)
//!   Format:
//!     S + Target_Addr+R/W*+A + P
//!
//! \return SMBUS_RET_OK
//
// *****************************************************************************
int8_t SMBus_App_Quickcommand(uint8_t Addr_RW)
{
    DL_GPIO_togglePins(SMB_GPIO_PORT, SMB_GPIO_LED_DEMO_PIN);
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Write Register Pointer
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command is received on a SEND_BYTE
//! The contents of this register pointer will be sent when a RECEIVE_BYTE
//! command is sent by the host.
//! Format:
//!   S + Target_Addr+W+A + CMD +A + {PEC+A} + P
//!   CMD = 0x00-0x03, sets pointer to Register0-3
//!       Reg0 = SMBus Status
//!       Reg1 = SMBus Control
//!       Reg2 = Scratchpad Byte
//!       Reg3 = Demo u8Version
//!
//! \return  SMBUS_RET_OK if pointer was updated correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Write_Reg_Ptr_hdlr(SMBus * SMB){
    // Change the register pointer to the new address
    // Implemented registers are 0-3

    switch((SMB->nwk.rxBuffPtr[0]) & 0x03)
    {
    case 0x00:
        // Point to SMBus status register
        SMB->nwk.recByteTxPtr = &(SMB->status.u8byte);
        break;
    case 0x01:
        // Point to Control Reg
        SMB->nwk.recByteTxPtr = &(SMB->ctrl.u8byte);
        break;
    case 0x02:
        // Point to address byte
        SMB->nwk.recByteTxPtr = &scratchpad_byte;
        break;
    case 0x03:
        // u8Version
        SMB->nwk.recByteTxPtr = (uint8_t *) &u8Version;
        break;
    default:
        // Invalid register
        return(SMBUS_RET_ERROR);
    }

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Write Register
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command writes a register directly when a WRITE_BYTE command is
//! received. Note that some bits in some registers are read-only.
//!
//! Format:
//!     S + Target_Addr+W+A + CMD+A + VAL+A + {PEC+A} + P
//!    CMD = 0x10-0x13 Writes Register 0-3
//!        Reg0 = SMBus Status  (clear only)
//!        Reg1 = SMBus Control (some bits are read-only)
//!        Reg2 = Scratchpad Byte
//!        Reg3 = Demo u8Version (read-only)
//!     VAL = New value being written to register
//!
//! \return  SMBUS_RET_OK if pointer was updated correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Write_Reg_hdlr(SMBus * SMB){
    // Implemented registers are 0-3, some are Read-only
    switch((SMB->nwk.rxBuffPtr[0]) & 0x03)
    {
    case 0x00:
        // Clear Status Register
        SMB->status.u8byte =
            SMBus_targetClearStatusReg(SMB, SMB->nwk.rxBuffPtr[1]);
        break;
    case 0x01:
        // Writes to Control Register (only to r/w bits)
        SMB->ctrl.u8byte =
            SMBus_targetWriteCtrlReg(SMB, SMB->nwk.rxBuffPtr[1]);
        break;
    case 0x02:
        // Write to Scratchpad Byte
        scratchpad_byte = SMB->nwk.rxBuffPtr[1];
        break;
    case 0x03:
        // Register is read-only, ignore request
        break;
    default:
        // Invalid register
        return(SMBUS_RET_ERROR);
    }
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Reads Register
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command reads a register directly when a READ_BYTE command is
//! received.
//! Format:
//!   S + Target_Addr+W+A + CMD+A +
//!   Sr + Target_Addr+R+A + VAL+A + {PEC+A} + P
//!   CMD = 0x20-0x23 Reads Register 0-3
//!       Reg0 = SMBus Status
//!       Reg1 = SMBus Control
//!       Reg2 = PxIN
//!       Reg3 = Demo u8Version
//!   VAL = Value sent as response from MSPM0
//!
//!
//! \return  SMBUS_RET_OK if register read correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_Reg_hdlr(SMBus * SMB){
    // Change the register pointer to the new address
    // Implemented registers are 0-7
    switch((SMB->nwk.rxBuffPtr[0]) & 0x03)
    {
    case 0x00:
        // Point to SMBus status register
        SMB->nwk.txBuffPtr[0] = SMB->status.u8byte;
        break;
    case 0x01:
        // Point to Control Reg
        SMB->nwk.txBuffPtr[0] = SMB->ctrl.u8byte;
        break;
    case 0x02:
        // Read Scratchpad Byte
        SMB->nwk.txBuffPtr[0] = scratchpad_byte;
        break;
    case 0x03:
        // u8Version
        SMB->nwk.txBuffPtr[0] = u8Version;
        break;
    default:
        // Invalid register
        return(SMBUS_RET_ERROR);
    }

    SMB->nwk.txLen = 1;
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Reads ADC
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command reads the latest result from ADC conversions using READ_WORD
//  protocol.
//! ADC conversions are performed automatically by the device, using timer
//! as a trigger.
//!   Format:
//!    S + Target_Addr+W+A + CMD+A +
//!    Sr + Target_Addr+R+A + VAL_L+A + VAL_H+A + {PEC+A} + P
//!   CMD = 0x30
//!   VAL_L = Low byte of ADC
//!   VAL_H = High byte of ADC
//!
//!
//! \return  SMBUS_RET_OK if ADC read successfully
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_ADC_Ch_hdlr(SMBus * SMB){
    // Get the ADC value which is updated by ADC Timer

    uint16_t temp;

    temp = au16ADC_shadow;

    // Send 2 bytes of ADC as response
    SMB->nwk.txBuffPtr[0] = (temp & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((temp & 0xFF00) >> 8);
    SMB->nwk.txLen = 2;

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Reads GPIOA
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command reads GPIO output values using READ_WORD32 protocol.
//!
//!
//!
//! \return  SMBUS_RET_OK if GPIOA read successfully
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_GPIOA_hdlr(SMBus * SMB){
    uint32_t portval = GPIOA->DOUT31_0;

    SMB->nwk.txBuffPtr[0] = (portval & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((portval & 0xFF00) >> 8);
    SMB->nwk.txBuffPtr[2] = ((portval & 0xFF0000) >> 16);
    SMB->nwk.txBuffPtr[3] = ((portval & 0xFF000000) >> 24);
    SMB->nwk.txLen = 4;

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Reads transaction counter
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command reads the targets transaction counter using READ_WORD64.
// The transaction counter is incremented after a command is sent to the target.
//!
//!
//!
//! \return  SMBUS_RET_OK if GPIOA read successfully
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_Transaction_Cnt_hdlr(SMBus *SMB){

    SMB->nwk.txBuffPtr[0] = (trxnCnt & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((trxnCnt & 0xFF00) >> 8);
    SMB->nwk.txBuffPtr[2] = ((trxnCnt & 0xFF0000) >> 16);
    SMB->nwk.txBuffPtr[3] = ((trxnCnt & 0xFF000000) >> 24);
    SMB->nwk.txBuffPtr[4] = ((trxnCnt & 0xFF00000000) >> 32);
    SMB->nwk.txBuffPtr[5] = ((trxnCnt & 0xFF0000000000) >> 40);
    SMB->nwk.txBuffPtr[6] = ((trxnCnt & 0xFF000000000000) >> 48);
    SMB->nwk.txBuffPtr[7] = ((trxnCnt & 0xFF00000000000000) >> 56);
    SMB->nwk.txLen = 8;

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Multiply 2 bytes
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command gets 2 bytes and returns the result of their multiplication
//!   using PROCESS_CALL format.
//!   Format:
//!     S + Target_Addr+W+A + CMD+A + VAL_1+A + VAL_2+A +
//!      Sr + Target_Addr+R+A + RES_L+A + RES_H+A + {PEC+A} + P
//!   CMD = 0x40
//!   VAL_1 = First byte (multiplicand)
//!   VAL_2 = Second byte (multiplier)
//!   RES_L = Low byte of result of VAL_1*VAL_2
//!   RES_H = High byte of result of VAL_1*VAL_2
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Mult_Bytes_hdlr(SMBus * SMB){
    uint16_t mult;

    // Multiply the 2 received values
    mult = SMB->nwk.rxBuffPtr[1] * SMB->nwk.rxBuffPtr[2];
    // Set the response as a word
    SMB->nwk.txBuffPtr[0] = (mult & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((mult & 0xFF00) >> 8);
    SMB->nwk.txLen = 2;

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Write a string
//!
//! \param SMB  SMBus structure which received the command
//!
//! Gets a block of data and stores in string using BLOCK_WRITE protocol.
//!   Format:
//!      S + Target_Addr+W+A + CMD+A + COUNT+A + DATA1+A +...+ DATAN+A + {PEC+A} +P
//!   CMD = 0x50
//!   COUNT = Number of bytes being received
//!   DATA1-DATAN = Received values 1-N
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Write_String_hdlr(SMBus * SMB){
    uint8_t i;
    // Simply write the received data to local buffer
    for(i = 0; i < SMB->nwk.rxBuffPtr[1]; i++)
    {
        au8String_buf[i] = SMB->nwk.rxBuffPtr[2 + i];
    }

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Read a string
//!
//! \param SMB  SMBus structure which received the command
//!
//! Returns a string using BLOCK_READ protocol.
//!   Format:
//!    S + Target_Addr+W+A + CMD+A +
//!    Sr + Target_Addr+R+A + COUNT+A + DATA1+A +...+ DATAN+A + {PEC+A} +P
//!   CMD = 0x51
//!   COUNT = Number of bytes being sent (32)
//!   DATA1-DATAN = Sent values 1-N
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_String_hdlr(SMBus * SMB){
    uint8_t i;

    // Set the lenght of payload
    SMB->nwk.txBuffPtr[0] = SMB_MAX_PAYLOAD_SIZE;
    // Now send the data
    for(i = 0; i < SMB_MAX_PAYLOAD_SIZE; i++)
    {
        SMB->nwk.txBuffPtr[i + 1] = au8String_buf[i];
    }
    // Number of bytes to send (including LEN)
    SMB->nwk.txLen = (SMB_MAX_PAYLOAD_SIZE + 1);
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Calculate CRC
//!
//! \param SMB  SMBus structure which received the command
//!
//! Calculates the CRC of a packet using BLOCKWRITE_BLOCKREAD PROCESS CALL
//!  protocol.
//!   Format:
//!    S + Target_Addr+W+A + CMD+A + COUNT+A + DATA1+A +...+ DATA2+A +
//!    Sr + Target_Addr+R+A + COUNT_TX+A + RES_L+A +RES_H+A + {PEC+A} + P
//!   CMD = 0x60
//!   COUNT = Number of received bytes
//!   DATA1-DATAN = Received values 1-N
//!    COUNT_TX = Number of bytes to transmit (2)
//!    RES_L = Low Byte of CRC Result
//!    RES_H = High Byte of CRC Result
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Calc_Sum_hdlr(SMBus * SMB){

    uint16_t acc = 0;

    uint16_t payload_length = SMB->nwk.rxIndex - 2; // - 1 for command byte, - 1 for byte count

    // Ensure payload length matches the byte_count as described in the packet "preamble"
    if (payload_length != SMB->nwk.rxBuffPtr[1])
    {
        return (SMBUS_RET_ERROR);
    }

    for (int ii = 0; ii < payload_length; ii++) {
        acc += SMB->nwk.rxBuffPtr[ii+1];
    }
    SMB->nwk.txBuffPtr[0] = 2;    // Response length is 2 bytes
    SMB->nwk.txBuffPtr[1] = (acc & 0x00FF);
    SMB->nwk.txBuffPtr[2] = ((acc & 0xFF00) >> 8);
    SMB->nwk.txLen = 3; // Number of bytes to send (including COUNT_TX)

    return(SMBUS_RET_OK);
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
        // If a Quick command was detected, execute function (if any)
        SMBus_App_Quickcommand(sSMBusTarget.nwk.currentAddr & 0x01);
        break;
    case SMBus_State_Target_CmdComplete:
        // Update application transaction counter
        trxnCnt++;

        // If any other command besides Quick Command is detected, process it
        if(Demo_CmdComplete(&sSMBusTarget) == SMBUS_RET_ERROR)
        {
            SMBus_targetReportError(&sSMBusTarget, SMBus_ErrorCode_Packet);
            DL_GPIO_togglePins(SMB_GPIO_PORT, SMB_GPIO_LED_ERROR_PIN);
            __NOP();
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
        DL_GPIO_setPins(SMB_GPIO_PORT, SMB_GPIO_LED_ERROR_PIN);
        delay_cycles(100);
        DL_GPIO_clearPins(SMB_GPIO_PORT, SMB_GPIO_LED_ERROR_PIN);
        __NOP();
        break;
    case SMBus_State_TimeOutError:
        // This demo simply updates the Status register when an error is
        // detected. The Host can check this register
        DL_GPIO_togglePins(SMB_GPIO_PORT, SMB_GPIO_LED_ERROR_PIN);
        __NOP();
        break;
    default:
        break;
    }
    // Clear flags to be ready for next packet
    SMBus_processDone(&sSMBusTarget);
}


//*****************************************************************************
//
//! ADC ISR
//!
//! Reads current ADC result and stores it
//!
//! \param   value has the result of latest ADC conversion
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
void SMB_ADC_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(SMB_ADC_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            au16ADC_shadow = DL_ADC12_getMemResult(SMB_ADC_INST, SMB_ADC_ADCMEM_SMB_ADC_channel_0);
            break;
        default:
            break;
    }
}
