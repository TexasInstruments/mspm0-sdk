/* --COPYRIGHT--,BSD
 * Copyright (c) 2020, Texas Instruments Incorporated
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
//*****************************************************************************
//         Generic GUI Demo using MSP430
//
// API to communicate with GUI and abstract transport layer protocol
// Texas Instruments, Inc.
// ******************************************************************************

#include <stdbool.h>
#include <stdint.h>

#ifndef INCLUDE_GUICOMM_H_
#define INCLUDE_GUICOMM_H_

//! ***** Functions to send data to GUI *****

//! \brief Sends a 32-bit integer.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendInt(char *cmd, uint8_t cmdLength, uint32_t val);

//! \brief Sends a 32-bit unsigned integer.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendUInt(char *cmd, uint8_t cmdLength, uint32_t val);

//! \brief Sends an uint8_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendUInt8(char *cmd, uint8_t cmdLength, uint8_t val);

//! \brief Sends an int8_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendInt8(char *cmd, uint8_t cmdLength, int8_t val);

//! \brief Sends an uint16_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendUInt16(char *cmd, uint8_t cmdLength, uint16_t val);

//! \brief Sends an int16_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendInt16(char *cmd, uint8_t cmdLength, int16_t val);

//! \brief Sends an uint32_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendUInt32(char *cmd, uint8_t cmdLength, uint32_t val);

//! \brief Sends an int32_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendInt32(char *cmd, uint8_t cmdLength, int32_t val);

//! \brief Sends an uint32_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendUInt64(char *cmd, uint8_t cmdLength, uint64_t val);

//! \brief Sends an int32_t value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendInt64(char *cmd, uint8_t cmdLength, int64_t val);

//! \brief Sends a bool value to the GUI.
//!
//! \return 0 if success, -1 if failure
extern int GUIComm_sendBool(char *cmd, uint8_t cmdLength, bool val);

//! \brief Reads a uint8_t value received from the GUI
//!
//! \return The received uint8_t value
extern uint8_t GUIComm_ReadUInt8(char *string);

//! \brief Reads a uint16_t value received from the GUI
//!
//! \return The received uint16_t value
extern uint16_t GUIComm_ReadUInt16(char *string);

//! \brief Reads a uint32_t value received from the GUI
//!
//! \return The received uint32_t value
extern uint32_t GUIComm_ReadUInt32(char *string);

//! \brief Reads a uint64_t value received from the GUI
//!
//! \return The received uint64_t value
extern uint64_t GUIComm_ReadUInt64(char *string);

//! \brief Reads a int8_t value received from the GUI
//!
//! \return The received int8_t value
extern int8_t GUIComm_ReadInt8(char *string);

//! \brief Reads a int16_t value received from the GUI
//!
//! \return The received int16_t value
extern int16_t GUIComm_ReadInt16(char *string);

//! \brief Reads a int32_t value received from the GUI
//!
//! \return The received int32_t value
extern int32_t GUIComm_ReadInt32(char *string);

//! \brief Reads a int64_t value received from the GUI
//!
//! \return The received int64_t value
extern int64_t GUIComm_ReadInt64(char *string);

//! \brief Reads a bool value received from the GUI
//!
//! \return The received bool value
extern bool GUIComm_ReadBool(char *string);

//! \brief Reads a float value received from the GUI
//!
//! \return The received float value
extern float GUIComm_ReadFloat(char *string);

//! \brief Reads a double value received from the GUI
//!
//! \return The received double value
extern double GUIComm_ReadDouble(char *string);

#endif /* INCLUDE_GUICOMM_H_ */
