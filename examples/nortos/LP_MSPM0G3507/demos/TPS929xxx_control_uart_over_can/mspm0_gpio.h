/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 */

#ifndef MSPM0_GPIO_H_
#define MSPM0_GPIO_H_

#include "ti_msp_dl_config.h"

//PA0
#define LED_ERR_ON \
    DL_GPIO_clearPins(GPIO_GRP_0_LED1_PORT, GPIO_GRP_0_LED1_PIN);
#define LED_ERR_OFF DL_GPIO_setPins(GPIO_GRP_0_LED1_PORT, GPIO_GRP_0_LED1_PIN);
//PB27
#define LED_RUN_ON DL_GPIO_setPins(GPIO_GRP_0_LED2_PORT, GPIO_GRP_0_LED2_PIN);
#define LED_RUN_OFF \
    DL_GPIO_clearPins(GPIO_GRP_0_LED2_PORT, GPIO_GRP_0_LED2_PIN);

extern uint8_t buttonS2Pressed;
extern uint8_t buttonS1Pressed;

#endif /* MSPM0_GPIO_H_ */
