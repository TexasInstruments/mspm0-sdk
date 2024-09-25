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

#ifndef __SMBUS_COMMAND_LIST_H__
#define __SMBUS_COMMAND_LIST_H__

//
// Include files
//
#include <stdlib.h>
#include <stdint.h>
#include "ti/smbus/smbus.h"

//
// Macros and definitions
//
/*! Number of commands implemented in application (except quick command) */
#define CMD_LIST_LENGHT 1
/*! Handler called when a command matches one in our list */
typedef int8_t (*Cmd_handler_t)(SMBus *);

/*! SMBus command structure*/
typedef struct
{
    uint8_t cmd;                /*! Command address */
    uint8_t mask;               /*! Mask (1 means don't care) */
    Cmd_handler_t handler;      /*! Handler called if command is received */
}t_SMB_Cmd;

//
// Global variables
//
//
extern const t_SMB_Cmd SMB_CMD_List[];

//
// Function prototypes
//
extern int8_t Control_LED_hdlr(SMBus *);

#endif //__SMBUS_COMMAND_LIST_H__
