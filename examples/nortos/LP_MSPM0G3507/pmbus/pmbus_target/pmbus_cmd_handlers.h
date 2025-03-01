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

#ifndef __PMBUS_CMD_HANDLERS_H__
#define __PMBUS_CMD_HANDLERS_H__

//*****************************************************************************
//
//! \addtogroup pmbus
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/* Include files */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ti/smbus/smbus.h"

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/*! Number of commands implemented in application (except quick command) */
#define CMD_LIST_LENGTH     (0x0A)
/*! Expected number of bytes to read a command */
#define EXPECTED_PMBUS_READ_CMD_RX_BYTE_COUNT   1
/*! Expected number of bytes to read a extended command */
#define EXPECTED_PMBUS_READ_EXTENDED_CMD_RX_BYTE_COUNT  2
/*! Expected number of bytes to write a command */
#define EXPECTED_PMBUS_WRITE_BYTE_CMD_RX_BYTE_COUNT 2
/*! Default MFR ID */
#define MFR_ID_DEFAULT      "MFR_ID"
/*! Default device ID */
#define DEVICE_ID           "MSPM0"
/*! Store the voltage reading from ADC  */
extern uint16_t outputVoltage;

//*****************************************************************************
//
//! Quick Command
//!
//! This demo toggles LED0 when a Quick Command is received
//! Note that we can detect QuickCommand(W), but not QuickCommand(R)
//! Format: S + Target_Addr+R/W*+A + P
//!
//
// *****************************************************************************
extern void PMBus_App_quickCommand(SMBus *SMB);

//*****************************************************************************
//
//! Validates a command
//!
//! This function is called when a command is received and the target needs to
//! check if the command is valid or not.
//! The function checks a list of valid commands (PMBus_cmdList) in order to
//! validate the current command, and if the command is found, it gets its
//! associated handler.
//!
//
// *****************************************************************************
extern void PMBus_App_isCommandValid(SMBus *SMB);

//*****************************************************************************
//
//! Validates an extended command
//!
//! This function is called when an extended command is received and the target
//! needs to check if the command is valid or not.
//! The function checks a list of valid commands (PMBus_cmdList) in order to
//! validate the current command, and if the command is found, it gets its
//! associated handler.
//!
//
// *****************************************************************************
extern void PMBus_App_isExtendedCommandValid(SMBus *SMB);

//*****************************************************************************
//
//! Command complete callback
//!
//! This function is called when a received packet is ready to process
//! after a STOP or RE-START.
//! The function executes the corresponding function if it was found by
//! Demo_isCmd_valid().
//! This demo implements a list of commands in PMBus_cmdList but the function
//! can be customized according to particular needs (i.e. switch statement or
//! hardcoded response).
//!
//
// *****************************************************************************
extern void PMBus_App_processCommand(SMBus *SMB);

extern int8_t PMBus_clearFaults_hdlr(SMBus* SMB);

extern int8_t PMBus_deviceId_hdlr(SMBus *SMB);

extern int8_t PMBus_voutMode_hdlr(SMBus *SMB);

extern int8_t PMBus_voutOVWarnLimit_hdlr(SMBus *SMB);

extern int8_t PMBus_MFRID_hdlr(SMBus *SMB);

extern int8_t PMBus_readVout_hdlr(SMBus *SMB);

extern int8_t PMBus_extByteCmd_hdlr(SMBus *SMB);

extern int8_t PMBus_extWordCmd_hdlr(SMBus *SMB);

extern int8_t PMBus_processCall_hdlr(SMBus *SMB);

extern int8_t PMBus_blockProcessCall_hdlr(SMBus *SMB);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif //__PMBUS_CMD_HANDLERS_H__
