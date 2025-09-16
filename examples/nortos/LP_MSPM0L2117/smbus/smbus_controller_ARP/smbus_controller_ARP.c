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
#include <string.h>

//
// Macros and definitions
//
#define TARGET_ADDRESS 0x61          /*! Address of target Default Device Address */
#define SMB_DEFAULT_PEC_ENABLED      /*! Define to enable PEC by default */

#define SMB_MAX_PAYLOAD_SIZE                    (255)
#define SMB_MAX_PACKET_SIZE                     (SMB_MAX_PAYLOAD_SIZE + 3)

/*! Software timeout used to wait for SMBus completion */
#define DEMO_TIMEOUT                100000

//
//  Global variables
//
static SMBus sSMBController;                             /*! SMBus Controller Struct  */
static SMBus_State sSMBState;                            /*! SMBus state          */
static uint8_t Command_Buff[SMB_MAX_PACKET_SIZE + 4];    /*! Buffer for command \ */
static uint8_t Resp_Buff[SMB_MAX_PACKET_SIZE + 4];       /*! Buffer for response \
                                                             from target device    */

#define TARGET_OWN_ADDRESS 0x0A
#define PREPARE_TO_ARP 0x01
#define RESET_DEVICE 0x02
#define GET_UDID 0x03
#define ASSIGN_ADDR 0x04
#define RESET_DEVICE_DIRECT (0x0 | TARGET_ADDRESS << 1)
#define GET_UDID_DIRECT (0x01 | TARGET_ADDRESS << 1)

typedef enum
{
    PREPARE_TO_ARP_CMD = 0x01,
    RESET_DEVICE_CMD   = 0x02,
    GET_UDID_CMD       = 0x03,
    ASSIGN_ADDR_CMD    = 0x04,
    GET_UDID_DIRECT_CMD    = 0x05,
    RESET_DIRECT_CMD       = 0x06,
    READ_BLOCK_DEBUG_CMD  = 0x07
}ARPCommand;

uint8_t targetOwnAdd = TARGET_OWN_ADDRESS;
uint8_t deviceID = 0;
tSMBusUDID deviceUDIDs[5];
uint8_t targetAddr[10] = {0};

volatile bool sendARPCommand = false;
volatile ARPCommand cmd = GET_UDID;
volatile uint16_t debugCmd = 0x04;

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

    while (1) {

        if(sendARPCommand)
        {

            if(cmd == GET_UDID_CMD)
            {
                sendARPCommand =  false;
                // Controller Read Block Command to get UDIDs
                ret = SMBus_controllerReadBlock(&sSMBController,    // SMB struct
                                            TARGET_ADDRESS,         // Target Addr
                                            GET_UDID,                    // ARP Command
                                            Resp_Buff);             // ResponsePtr
                RXExpected = true;      // Response expected (string)
            }
            else if(cmd == PREPARE_TO_ARP_CMD)
           {
                sendARPCommand =  false;
               // Controller Read Block Command to get UDIDs

                ret = SMBus_controllerSendByte(&sSMBController,        // SMB struct
                                                TARGET_ADDRESS,             // Target Addr
                                                PREPARE_TO_ARP_CMD);        // Data
           }
            else if(cmd == RESET_DEVICE_CMD)
           {
                sendARPCommand =  false;
               // Controller Read Block Command to get UDIDs

                ret = SMBus_controllerSendByte(&sSMBController,        // SMB struct
                                                TARGET_ADDRESS,             // Target Addr
                                                RESET_DEVICE_CMD);        // Data
           }
            else if(cmd == RESET_DIRECT_CMD)
           {
                sendARPCommand =  false;
               // Controller Read Block Command to get UDIDs

                ret = SMBus_controllerSendByte(&sSMBController,        // SMB struct
                                                TARGET_ADDRESS,             // Target Addr
                                                ((targetOwnAdd << 1) & 0xFE));        // Data
           }
            else if(cmd == GET_UDID_DIRECT_CMD)
            {
                sendARPCommand =  false;
                // Controller Read Block Command to get UDIDs
                ret = SMBus_controllerReadBlock(&sSMBController,    // SMB struct
                                            TARGET_ADDRESS,         // Target Addr
                                            ((targetOwnAdd << 1 | 0x01) & 0xFF),                    // ARP Command
                                            Resp_Buff);             // ResponsePtr
                RXExpected = true;      // Response expected (string)
            }
            else if(cmd == ASSIGN_ADDR_CMD)
            {
                sendARPCommand = false;
               //Assign Address ...
                cmd = ASSIGN_ADDR;
                memcpy(Command_Buff,&deviceUDIDs[deviceID-1],sizeof(deviceUDIDs[0]));
                Command_Buff[16] = ((targetOwnAdd << 1 | 0x01) & 0xFF);

                ret = SMBus_controllerWriteBlock(&sSMBController,        // SMB struct
                                             TARGET_ADDRESS,             // Target Addr
                                             ASSIGN_ADDR,                       // SMB Command
                                             &Command_Buff[0],           // DataPtr
                                             17);        // Data Len
                RXExpected = false;      // No response expected
            }
            else if(cmd == READ_BLOCK_DEBUG_CMD)
            {
                sendARPCommand = false;

                ret = SMBus_controllerReadBlock(&sSMBController,    // SMB struct
                                                        TARGET_ADDRESS,         // Target Addr
                                                        debugCmd,                   // SMB Command
                                                        Resp_Buff);             // ResponsePtr
                RXExpected = true;      // Response expected (string)
            }

            /* ------------------------------------------------------------------------- */

            // After sending the command, wait for completion and check response
            if(ret == SMBUS_RET_OK)
            {
                // If the command was sent to target, wait for completion
                if(SMBus_controllerWaitUntilDone (&sSMBController,
                                              DEMO_TIMEOUT) != SMBUS_RET_OK)
                {
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
                        //Target has responded with its UDID, send the command Again
                        if((RespLen == 17 && cmd == GET_UDID_DIRECT_CMD) ||(RespLen == 17 && cmd == GET_UDID_CMD))
                        {
                            memcpy(&deviceUDIDs[deviceID],Resp_Buff,sizeof(deviceUDIDs[0]));
                            targetAddr[deviceID++] = Resp_Buff[16];

                            //Change it
                            cmd = ASSIGN_ADDR;
                            sendARPCommand = false;
                        }
                    }
                    else
                    {
                        sendARPCommand = false;
                    }
                }
            }

        }
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
