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


#include <stdint.h>

#include "ti_msp_dl_config.h"

#include <ti/smbus/smbus.h>
#include "SMBUS_Demo.h"

//
// Macros and definitions
//
#define TARGET_ADDRESS 0x43          /*! Address of target device */
#define SMB_DEFAULT_PEC_ENABLED      /*! Define to enable PEC by default */

//
//  Global variables
//
static SMBus sSMBController;                             /*! SMBus Controller Struct  */
static SMBus_State sSMBState;                            /*! SMBus state          */
static uint8_t Command_Buff[SMB_MAX_PACKET_SIZE + 4];    /*! Buffer for command \
                                                             to target   */
static uint8_t Resp_Buff[SMB_MAX_PACKET_SIZE + 4];       /*! Buffer for response \
                                                             from target device    */
/*! String used to test block write*/
static char const TestString[255] = "This string makes use of the 255 byte block write in SMBus 3.0. Previously, only 32 bytes could be transfered with SMBus 2.0.";

/*! Step-by-step Tests performed by this demo */
enum Tests
{
    QUICK_CMD_TEST = 0,     /*! Test Quick Command                      */
    SEND_BYTE_TEST,         /*! Test Send Byte command                  */
    RECEIVE_BYTE_TEST,      /*! Test Receive Byte command               */
    WRITE_BYTE_TEST,        /*! Test Write Byte Command                 */
    READ_BYTE_TEST,         /*! Test Read Byte command                  */
    READ_WORD16_TEST,       /*! Test Read 2 Byte Word command           */
    READ_WORD32_TEST,       /*! Test Read 4 Byte Word command           */
    READ_WORD64_TEST,       /*! Test Read 8 Byte Word command           */
    PROCESS_CALL_TEST,      /*! Test Process Call command               */
    BLOCK_WRITE_TEST,       /*! Test Block Write command                */
    BLOCK_READ_TEST,        /*! Test Block Read command                 */
    BLOCK_PROC_CALL_TEST,   /*! Test BlockWBlockR Process Call command  */
    END_OF_TESTS            /*! delimiter                               */
} Tests_e;

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Controller demo testing all SMBus 2.0 protocols
//!   Each command is sent to target one-by-one
//!   Check response using logic analyzer or set breakpoints to debug
//!
//! \return None
//
// *****************************************************************************
int main(void)
{
    uint8_t ret, RespLen;
    bool RXExpected;

    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    SMBus_controllerInit(&sSMBController, SMB_I2C_INST, (32000000));

 #ifdef SMB_DEFAULT_PEC_ENABLED
    SMBus_enablePEC(&sSMBController);       // Call if PEC is enabled by default
 #endif

    Tests_e = QUICK_CMD_TEST;    // Start with Quick Command test


    while(1)
    {
        switch(Tests_e)
        {

        case QUICK_CMD_TEST:
            // Send QuickCommand (W), no data nor command is needed
            //  Target is expected to simply toggle LED1
            ret = SMBus_controllerQuickCommand(&sSMBController,  // SMB struct
                                           TARGET_ADDRESS,       // Target Addr
                                           true);                // QuickCMD W
            RXExpected = false;       // Response not expected
            break;
        case SEND_BYTE_TEST:
            // Send Sendbyte, command is needed
            //  Target will change its register pointer:
            //  0x00 = Reg0 = SMBus Status
            //  0x01 = Reg1 = SMBus Control
            //  0x02 = Reg2 = Port Input
            //  0x03 = Reg3 = Demo version  <- Used in demo
            ret = SMBus_controllerSendByte(&sSMBController,      // SMB struct
                                       TARGET_ADDRESS,           // Target Addr
                                       0x03);                    // SMB Command
            RXExpected = false;      // No response expected
            break;
        case RECEIVE_BYTE_TEST:
            // Send Receive byte, no data nor command is needed
            //  Target is expected to send the contents of its register
            //  pointer (set by Send_Byte command)
            ret = SMBus_controllerReceiveByte(&sSMBController,  // SMB struct
                                          TARGET_ADDRESS,       // Target Addr
                                          Resp_Buff);           // Response
            RXExpected = true;      // Response expected (version)
            break;
        case WRITE_BYTE_TEST:
            // Send Write byte, Command and data are needed
            //  Target will write directly to the corresponding register
            //  0x10 = Reg0 = SMBus Status
            //  0x11 = Reg1 = SMBus Control
            //  0x12 = Reg2 = Port Output   <- Used in demo
            //  0x13 = Reg3 = Demo version
            Command_Buff[0] = 0xAA;         // Write 0xAA to the port
            ret = SMBus_controllerWriteByteWord(&sSMBController, // SMB struct
                                            TARGET_ADDRESS,      // Target Addr
                                            0x12,                // SMB Command
                                            &Command_Buff[0],    // DataPtr
                                            1);                  // 1 byte data
            RXExpected = false;      // No response expected
            break;
       case READ_BYTE_TEST:
            // Send Read byte, Command is needed
            //  Target will read directly from the corresponding register
            //  0x20 = Reg0 = SMBus Status
            //  0x21 = Reg1 = SMBus Control
            //  0x22 = Reg2 = Port input    <- Used in demo
            //  0x23 = Reg3 = Demo version
            ret = SMBus_controllerReadByteWord(&sSMBController,  // SMB struct
                                           TARGET_ADDRESS,       // Target Addr
                                           0x22,                 // SMB Command
                                           Resp_Buff,            // ResponsePtr
                                           1);                   // 1 byte expected
            RXExpected = true;      // Response expected (port input)
            break;
        case READ_WORD16_TEST:
            // Send Read Word 16, Command is needed
            //  0x30 = Target will send contents of latest ADC conversion
            ret = SMBus_controllerReadByteWord(&sSMBController,  // SMB struct
                                           TARGET_ADDRESS,       // Target Addr
                                           0x30,                 // SMB Command
                                           Resp_Buff,            // ResponsePtr
                                           2);                   // 2 bytes expected
            RXExpected = true;      // Response expected (ADC result)
            break;
        case READ_WORD32_TEST:
            // Send Read Word 32, Command is needed
            //  0x31 = Target will send contents of GPIOA.DOUT31_0
            ret = SMBus_controllerReadByteWord(&sSMBController,  // SMB struct
                                           TARGET_ADDRESS,       // Target Addr
                                           0x31,                 // SMB Command
                                           Resp_Buff,            // ResponsePtr
                                           4);                   // 2 bytes expected
            RXExpected = true;      // Response expected
            break;
        case READ_WORD64_TEST:
            // Send Read Word 64, Command is needed
            //  0x32 = Target will send transaction count
            ret = SMBus_controllerReadByteWord(&sSMBController,  // SMB struct
                                           TARGET_ADDRESS,       // Target Addr
                                           0x32,                 // SMB Command
                                           Resp_Buff,            // ResponsePtr
                                           8);                   // 2 bytes expected
            RXExpected = true;      // Response expected
            break;
        case PROCESS_CALL_TEST:
            // Send Process Call, Command and 2 bytes of data are needed
            //  0x40 = Multiply 2 bytes
            Command_Buff[0] = 0x02;         // Multiplier 1
            Command_Buff[1] = 0x10;         // Multiplier 2
            ret = SMBus_controllerProcessCall(&sSMBController,   // SMB struct
                                          TARGET_ADDRESS,        // Target Addr
                                          0x40,                  // SMB Command
                                          &Command_Buff[0],      // DataPtr
                                          Resp_Buff);            // ResponsePtr
            RXExpected = true;      // Response expected (result of multiplication)
            break;
        case BLOCK_WRITE_TEST:
            // Send Block Write with CMD, Len and Data
            //  0x50 = Write a string
            ret = SMBus_controllerWriteBlock(&sSMBController,        // SMB struct
                                         TARGET_ADDRESS,             // Target Addr
                                         0x50,                       // SMB Command
                                         (uint8_t *) &TestString[0], // DataPtr
                                         sizeof(TestString));        // Data Len
            RXExpected = false;      // No response expected
            break;
        case BLOCK_READ_TEST:
            // Send Read Block, Command is needed
            //  0x51 = Read a string from target
            ret = SMBus_controllerReadBlock(&sSMBController,    // SMB struct
                                        TARGET_ADDRESS,         // Target Addr
                                        0x51,                   // SMB Command
                                        Resp_Buff);             // ResponsePtr
            RXExpected = true;      // Response expected (string)
            break;
        case BLOCK_PROC_CALL_TEST:
            // Send Block Write with CMD, Men and Data
            //  0x60 = Sum all bytes
            Command_Buff[0] = 0x01;         // Data1
            Command_Buff[1] = 0x02;         // Data2
            Command_Buff[2] = 0x03;         // Data3
            ret = SMBus_controllerProcessCallBlock(&sSMBController, // SMB struct
                                               TARGET_ADDRESS,      // Target Addr
                                               0x60,                // SMB Command
                                               &Command_Buff[0],    // DataPtr
                                               3,                   // Data Len
                                               Resp_Buff);          // ResponsePtr
            RXExpected = true;      // Response expected (CRC)
            break;
        default:
            Tests_e = QUICK_CMD_TEST;   // Unexpected case, restart demo
            break;
        }

        // After sending the command, wait for completion and check response
        if(ret == SMBUS_RET_OK)
        {
            // If the command was sent to target, wait for completion
            if(SMBus_controllerWaitUntilDone (&sSMBController,
                                          DEMO_TIMEOUT) != SMBUS_RET_OK)
            {
                ret = DEMO_TIMEOUT_ERROR;
                // Timeout detected in App but not by SCL line, restart interface
                SMBus_controllerReset(&sSMBController);
            }
            else
            {
                ret = sSMBState;
            }

            // If we are waiting for a response, check if we got it
            if((ret == SMBus_State_OK) && (RXExpected == true))
            {
                // Get the length of payload for response
                RespLen = SMBus_getRxPayloadAvailable(&sSMBController);

                if(RespLen >= 1)
                {
                    ret = DEMO_NO_ERROR_DATA_AVAIL; // Data Received OK
                }
                else
                {
                    ret = DEMO_RX_ERROR;             // RX Data size Error
                }
            }
        }

        sSMBController.status.u8byte = 0x00; // Clear status
        Tests_e++;                       // Run next test
        if(Tests_e >= END_OF_TESTS)
        {
            Tests_e = QUICK_CMD_TEST;
            while(1);
        }
        delay_cycles(40000);
    }
}

//*****************************************************************************
//
//! I2C SMBus ISR
//!
//! Handles Controller interrupts
//!
//! \return  None
//
// *****************************************************************************
void SMB_I2C_INST_IRQHandler(void) {
    sSMBState = SMBus_controllerProcessInt(&sSMBController);
}
