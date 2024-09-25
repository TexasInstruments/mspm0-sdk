/* --COPYRIGHT--,BSD
 * Copyright (c) 2022, Texas Instruments Incorporated
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

#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/IQMathLib/QmathLib.h>
#include "MSP_GUI/GUIComm_mpack.h"

#include "demo_mode_declaration.h"

#ifndef INCLUDE_CALLBACKS_H_
#define INCLUDE_CALLBACKS_H_

#define STR_LEN_FIVE 5
#define STR_LEN_SIX 6
#define STR_LEN_SEVEN 7
#define STR_LEN_EIGHT 8
#define STR_LEN_NINE 9
#define STR_LEN_ELEVEN 11

#define MAX_COUNT 10

/* Defined in callbacks_mpack.c */
extern volatile bool bEnableBlinkLED;
extern volatile bool bChangeBlinkFreq;
extern volatile float fBlinkLEDDelay;

/* Functions to receive data from GUI */
extern void callback_blinkEnable(mpack_tag_t *tag, mpack_reader_t *reader);
extern void callback_setBlinkDelay(mpack_tag_t *tag, mpack_reader_t *reader);

#endif /* INCLUDE_CALLBACKS_H_ */
