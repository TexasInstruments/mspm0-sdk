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

#include "PGA460.h"
#include "ti_msp_dl_config.h"

double distance = 0;

int main(void)
{
    SYSCFG_DL_init();

    //**********************initThresholds **********************//
    //Register: 0x5F~0x7F
    initThresholds(1);  // 1:50% detection threshold

    //**********************defaultPGA460************************//
    //Register: 0x00~0x2B
    defaultPGA460(0);  // 0: Murata MA58MF14-7N

    //**********************PGA460 Function**********************//
    while (1)  //run measurement code repeatedly
    {
        ultrasonicCmd(
            0, 1);  // run preset 1 (short distance) burst+listen for 1 object
        delay_cycles(
            4800000);  //delay 200ms (24MHz) wait for distance detection finish
        pullUltrasonicMeasResult();  // Read the ultrasonic measurement result data based on the last busrt and/or listen command issued.
        distance = printUltrasonicMeasResult(0);  // convert time to distance
        uartSend(distance);  // send distance to COM through UART
    }
}
