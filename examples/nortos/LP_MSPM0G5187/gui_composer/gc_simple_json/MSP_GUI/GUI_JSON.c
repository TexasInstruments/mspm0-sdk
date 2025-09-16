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
// API to send and receive data from GUI using JSON protocol
//
// Texas Instruments, Inc.

#include "GUI_JSON.h"
#include <ti/gui_composer/HAL.h>
#include <ti/gui_composer/jsmn/jsmn.h>

/**** Local Variables *********************************************************/
static char txString[MAX_STR_LEN];  // String to transmit data
#if (GUI_RXCMD_ENABLE)
static char rxString[MAX_STR_LEN];     // String to receive data
static jsmn_parser p;                  // parses JSON data
static jsmntok_t t[64];                // We expect no more than 64 tokens
static const tGUI_RxCmd *RxCmdsArray;  // Pointer to array of
                                       // RX commands
static uint16_t RxCmdsArraySize;       // Size of RX commands
#endif

/**** Local Function Prototypes************************************************/
#if (GUI_RXCMD_ENABLE)
static bool GUI_ParseString(void);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
#endif
static bool GUI_RxCharHALCallback(char data);

/**** Functions ***************************************************************/
void GUI_Init(void)
{
    HAL_GUI_Init(GUI_RxCharHALCallback);
    RxCmdsArraySize = 0;
    RxCmdsArray     = NULL;
}

#if (GUI_RXCMD_ENABLE)
void GUI_InitRxCmd(const tGUI_RxCmd *CommandArray, uint16_t size)
{
    RxCmdsArray     = CommandArray;
    RxCmdsArraySize = size;
}
#endif

void GUI_TransmitStringBlocking(char *str)
{
    uint16_t i;

    for (i = 0; i < strlen(str); i++) {
        if (str[i] != 0) {
            HAL_GUI_TransmitCharBlocking(str[i]);
        }
    }
}

void GUI_TransmitData(const tGUI_TxCmd *TXCmdrray, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++) {
        sprintf(
            txString, TXCmdrray[i].formattingStringCMD, *TXCmdrray[i].param);
        GUI_TransmitStringBlocking(txString);
    }
}

/**** Local Functions**********************************************************/
//! \brief Callback function for GUI HAL. Called when byte is received.
//!
//! \param[in] data is the byte received from GUI Comm interface.
//!
//! \return true to wake-up MCU, false to stay in LPM
static bool GUI_RxCharHALCallback(char data)
{
#if (GUI_RXCMD_ENABLE)
    static bool rxInProgress = false;
    static uint16_t charCnt  = 0;
    bool ret                 = false;

    if (!rxInProgress) {
        if ((data != '\n')) {
            rxInProgress      = true;
            charCnt           = 0;
            rxString[charCnt] = data;
        }
    } else {  // in progress
        charCnt++;
        if ((data != '\n')) {
            if (charCnt >= MAX_STR_LEN) {
                rxInProgress = false;
            } else {
                rxString[charCnt] = data;
            }
        } else {
            rxInProgress      = false;
            rxString[charCnt] = '\0';
            // String receive complete
            if (GUI_ParseString() == true) {
                ret = true;  // wake-up MCU
            }
        }
    }
    return ret;
#else
    return true;
#endif
}

#if (GUI_RXCMD_ENABLE)
//! \brief Parses a string looking for JSON objects
//!
//! \return true to wake-up MCU when a command is received, false to stay in LPM
static bool GUI_ParseString(void)
{
    int i;
    int r;
    int j;
    bool ret = false;

    jsmn_init(&p);

    r = jsmn_parse(
        &p, rxString, strlen(rxString), t, sizeof(t) / sizeof(t[0]));

    // Loop over keys of JSON object
    if (RxCmdsArray != NULL) {
        for (i = 1; i < r; i++) {
            for (j = 0; j < RxCmdsArraySize; j++) {
                if (jsoneq(rxString, &t[i], RxCmdsArray[j].stringCMD) == 0) {
                    if (RxCmdsArray[j].callback != NULL) {
                        RxCmdsArray[j].callback(rxString + t[i + 1].start);
                    }
                    i++;
                    ret = true;
                }
            }
        }
    }

    return ret;
}

//! \brief Compare JSON keys
//!
//! \param[in] json is the JSON string.
//! \param[in] tok is the JSON token.
//! \param[in] s is the string to check for equality.
//!
//! \return 0 if string found. -1 if not.
static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
#endif
