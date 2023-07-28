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
//         GUI Communication Layer
//
// API to communicate with GUI using JSON protocol
// Texas Instruments, Inc.**

#include "ti_msp_dl_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/HAL.h>
#include "GUIComm_JSON.h"
#include "GUI_JSON.h"

static void itoa(int32_t value, char *result);

static const char cmdTemplate[] = "{\"\":}\n";
static char cATXString[20];

//These values are used to concatenate the command and data to the command template
// when using the GUIComm_send functions
#define CMD_INDEX 1
#define DATA_INDEX 3
#define CMD_TEMPLATE_LENGTH 6
#define BASE 10

//! \brief Converts a number into a string
//!
//! \return none
void itoa(int32_t value, char *result)
{
    char *ptr = result, *ptr1 = result, tmp_char;
    uint32_t tmp_value;

    tmp_value = labs(value);
    do {
        *ptr++ = "0123456789"[tmp_value % BASE];
        tmp_value /= BASE;
    } while (tmp_value);

    // Apply negative sign
    if (value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--   = *ptr1;
        *ptr1++  = tmp_char;
    }
}

int GUIComm_sendUInt(char *cmd, uint8_t cmdLength, uint32_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendInt(char *cmd, uint8_t cmdLength, uint32_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendUInt8(char *cmd, uint8_t cmdLength, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendInt8(char *cmd, uint8_t cmdLength, int8_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendInt16(char *cmd, uint8_t cmdLength, int16_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendUInt16(char *cmd, uint8_t cmdLength, uint16_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendUInt32(char *cmd, uint8_t cmdLength, uint32_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendInt32(char *cmd, uint8_t cmdLength, int32_t val)
{
    uint8_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            itoa(val, cATXString);
            GUI_TransmitStringBlocking(cATXString);
        }
    }

    return 0;
}

int GUIComm_sendBool(char *cmd, uint8_t cmdLength, bool val)
{
    uint16_t i;

    for (i = 0; i < CMD_TEMPLATE_LENGTH; i++) {
        HAL_GUI_TransmitCharBlocking(cmdTemplate[i]);
        if (i == CMD_INDEX) {
            GUI_TransmitStringBlocking(cmd);
        } else if (i == DATA_INDEX) {
            if (val == true) {
                GUI_TransmitStringBlocking("true");
            } else {
                GUI_TransmitStringBlocking("false");
            }
        }
    }

    return 0;
}

uint8_t GUIComm_ReadUInt8(char *string)
{
    return (uint8_t) strtoul(string, NULL, 10);
}

uint16_t GUIComm_ReadUInt16(char *string)
{
    return (uint16_t) strtoul(string, NULL, 10);
}

uint32_t GUIComm_ReadUInt32(char *string)
{
    return (uint32_t) strtoul(string, NULL, 10);
}

uint64_t GUIComm_ReadUInt64(char *string)
{
    return (uint64_t) strtoul(string, NULL, 10);
}

int8_t GUIComm_ReadInt8(char *string)
{
    return (int8_t) strtol(string, NULL, 10);
}

int16_t GUIComm_ReadInt16(char *string)
{
    return (int16_t) strtol(string, NULL, 10);
}

int32_t GUIComm_ReadInt32(char *string)
{
    return (int32_t) strtol(string, NULL, 10);
}

int64_t GUIComm_ReadInt64(char *string)
{
    return (int64_t) strtol(string, NULL, 10);
}

bool GUIComm_ReadBool(char *string)
{
    if (strncmp(string, "true", 4) == 0) {
        return true;
    } else {
        return false;
    }
}

float GUIComm_ReadFloat(char *string)
{
    return strtof(string, NULL);
}

double GUIComm_ReadDouble(char *string)
{
    return strtod(string, NULL);
}
