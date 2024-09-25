/* --COPYRIGHT--,BSD
 * Copyright (c) 2019, Texas Instruments Incorporated
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
//         GUI Interface
//
// API to send and receive data from GUI using GUI Composer
// Texas Instruments, Inc.
// ******************************************************************************

#ifndef __GUI_H__
#define __GUI_H__
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! \brief Maximum string for RX
#ifndef MAX_STR_LEN
#define MAX_STR_LEN 64
#endif

//! \brief Enable GUI Reception of commands
#define GUI_RXCMD_ENABLE (1)

#if (GUI_RXCMD_ENABLE)
//! \brief Callback for RX Commands (called when command is received from GUI)
typedef void (*tGUI_RxCmdCallback)(char *);

//! \brief Structure for RX commands.
typedef struct {
    //!< RX Command (received from GUI)
    char *stringCMD;
    //!< Function executed if command is detected.
    tGUI_RxCmdCallback callback;
} tGUI_RxCmd;
#endif

//! \brief Structure for TX commands.
typedef struct {
    //!< TX Command including sprintf formatting.
    char *formattingStringCMD;
    //!< Parameter sent with TX command.
    void **param;
} tGUI_TxCmd;

//! \brief Initializes the GUI communication and protocol.
//!
//! \return none
extern void GUI_Init(void);

#if (GUI_RXCMD_ENABLE)
//! \brief Initializes structure to receive and process commands
//!
//! \param[in] RxCmdArray is a pointer to the array of RX Commands.
//!             These commands will execute a callback function when received.
//! \param[in] size is the size of RxCmdArray.
//!
//! \return none
extern void GUI_InitRxCmd(const tGUI_RxCmd *RxCmdArray, uint16_t size);
#endif

//! \brief Transmits an array of TX commands.
//!
//! \param[in] TXACmdrray is a pointer to the array of TX commands.
//!             These commands will be sent with corresponding formatted data.
//! \param[in] size is the size of TxCmdArray.
//!
//! \return none
extern void GUI_TransmitData(const tGUI_TxCmd *TXCmdrray, uint16_t size);

//! \brief Transmits a string to GUI.
//!
//! \param[in] str is the string to be sent.
//!
//! \return none
extern void GUI_TransmitStringBlocking(char *str);

#endif  // __GUI_H__
