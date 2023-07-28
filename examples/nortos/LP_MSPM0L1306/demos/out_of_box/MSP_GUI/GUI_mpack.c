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
//
// Texas Instruments, Inc.

#include "GUI_mpack.h"
#include <ti/gui_composer/HAL.h>
#include <ti/gui_composer/mpack/mpack.h>

static char txString[MAX_STR_LEN];  // String to transmit data
static char rxString[MAX_STR_LEN];  // String to receive data

static const tGUI_RxCmd *RxCmdsArray;  // Pointer to array of
// RX commands
static uint16_t RxCmdsArraySize;  // Size of RX commands

/* Variables used for messagepack communication */
char data[50];
mpack_writer_t writer;
size_t size;

/**** Local Function Prototypes************************************************/
static bool GUI_ParseString(void);
static bool GUI_RxCharHALCallback(char data);

/**** Functions ***************************************************************/
void mpack_flush_callback(
    mpack_writer_t *writer, const char *buffer, size_t count)
{
    /* Set the delimiter at the end and then transmit data */
    data[count] = '\n';
    GUI_TransmitStringBlocking(data, (count + 1));
}

void GUI_Init(void)
{
    /* Initialize the RX callback */
    HAL_GUI_Init(GUI_RxCharHALCallback);

    /*
     * Initialize mpack flush callback, which is called when we transmit a
     * messagepack message
     */
    mpack_writer_flush_t flush = &mpack_flush_callback;
    mpack_writer_init(&writer, data, sizeof(data));
    mpack_writer_set_flush(&writer, flush);
}

void GUI_InitRxCmd(const tGUI_RxCmd *CommandArray, uint16_t size)
{
    RxCmdsArray     = CommandArray;
    RxCmdsArraySize = size;
}

void GUI_TransmitStringBlocking(char *str, size_t count)
{
    uint16_t i;

    for (i = 0; i < count; i++) {
        HAL_GUI_TransmitCharBlocking(str[i]);
    }
}

void GUI_TransmitData(const tGUI_TxCmd *TXCmdrray, uint16_t size)
{
    uint16_t i;

    for (i = 0; i < size; i++) {
        sprintf(
            txString, TXCmdrray[i].formattingStringCMD, *TXCmdrray[i].param);
        GUI_TransmitStringBlocking(txString, sizeof(txString));
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
    static bool rxInProgress = false;
    static uint16_t charCnt  = 0;
    bool ret                 = false;

    if (!rxInProgress) /* Initial receive character */
    {
        if ((data != '\n')) {
            rxInProgress      = true;
            charCnt           = 0;
            rxString[charCnt] = data;
        }
    } else /* Receive in progress */
    {
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
            /* String receive complete */
            if (GUI_ParseString() == true) {
                ret = true; /* Wake-up MCU */
            }
        }
    }
    return ret;
}

//! \brief Parses a string looking for command
//!
//! \return true to wake-up MCU when a command is received, false to stay in LPM
static bool GUI_ParseString(void)
{
    mpack_reader_t reader;
    mpack_tag_t tag;

    int i, j;
    int containerSize;

    mpack_reader_init_data(&reader, rxString, strlen(rxString));
    tag = mpack_read_tag(&reader);

    /* Initial type should be a mpack map */
    if (mpack_tag_type(&tag) == mpack_type_map) {
        /* Map tag count specifies the number of key:value pairs, so to read
         * each individual key and value, we must double the count
         */
        containerSize = (mpack_tag_map_count(&tag) * 2);
        for (i = 0; i < containerSize; i++) {
            /* Read the key, should be type string */
            tag = mpack_read_tag(&reader);
            if (mpack_tag_type(&tag) == mpack_type_str) {
                char buffer[8];  // todo length should be some max string value

                /* Read the actual string to get the command */
                mpack_read_bytes(&reader, buffer, mpack_tag_str_length(&tag));
                for (j = 0; j < RxCmdsArraySize; j++) {
                    /* Check if it is a valid command */
                    if (strncmp(buffer, RxCmdsArray[j].stringCMD,
                            mpack_tag_str_length(&tag)) == 0) {
                        if (RxCmdsArray[j].callback != NULL) {
                            /* Read the value and call the correct callback */
                            tag = mpack_read_tag(&reader);
                            RxCmdsArray[j].callback(&tag, &reader);
                        }
                        break; /* Got a positive match, can stop checking */
                    }
                }
            }
        }
    }

    /* If reader is corrupted, return error */
    if (mpack_reader_destroy(&reader) != mpack_ok) {
        return false;
    }
    return true;
}
