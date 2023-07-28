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
//         Dual Ray Smoke Sensor GUI Interface
//
// API to send and receive data from Dual Ray Smoke GUI
// ******************************************************************************

#ifndef __DUALRAYSMOKEAFE_GUI_H__
#define __DUALRAYSMOKEAFE_GUI_H__
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! \brief Maximum string for RX
#define MAX_STR_LEN 256

//! \brief Callback for RX Commands (called when command is received from GUI)
typedef void (*tDualRaySmokeAFE_GUI_RxCmdCallback)(char *);

//! \brief Structure for RX commands.
typedef struct {
    //!< RX Command (received from GUI)
    char *stringCMD;
    //!< Function executed if command is detected.
    tDualRaySmokeAFE_GUI_RxCmdCallback callback;
} tDualRaySmokeAFE_GUI_RxCmd;

//! \brief Structure for TX commands.
typedef struct {
    //!< TX Command including sprintf formatting.
    char *formattingStringCMD;
    //!< Parameter sent with TX command.
    void **param;
} tDualRaySmokeAFE_GUI_TxCmd;

//! \brief Initializes the GUI communication and protocol.
//!
//! \param[in] RxCmdArray is a pointer to the array of RX Commands.
//!             These commands will execute a callback function when received.
//! \param[in] size is the size of RxCmdArray.
//!
//! \return none
extern void DualRaySmokeAFE_GUI_Init(
    const tDualRaySmokeAFE_GUI_RxCmd *RxCmdArray, uint16_t size);

//! \brief Transmits an array of TX commands.
//!
//! \param[in] TXACmdrray is a pointer to the array of TX commands.
//!             These commands will be sent with corresponding formatted data.
//! \param[in] size is the size of TxCmdArray.
//!
//! \return none
extern void DualRaySmokeAFE_GUI_TransmitData(
    const tDualRaySmokeAFE_GUI_TxCmd *TXCmdrray, uint16_t size);

//! \brief Transmits a string to GUI.
//!
//! \param[in] str is the string to be sent.
//!
//! \return none
extern void DualRaySmokeAFE_GUI_TransmitStringBlocking(char *str);

extern const tDualRaySmokeAFE_GUI_TxCmd GUI_TXCmdData[];

#endif  // __DUALRAYSMOKEAFE_GUI_H__
