/*
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
  */

/*
  *  ============ DAC8Driver.c =============
  *  Example source file for simple DAC task to be handled with
  *  the RTC scheduler.
  *
  *  NOTE: This file exists only to demonstrate the functionality of
  *  the scheduler module.
  *
  */

#include "DAC8Driver.h"

static volatile bool gDACFlag = false;
volatile uint8_t gDACValue = 0;

void setDACFlag()
{
    gDACFlag = true;
}

bool getDACFlag()
{
    return gDACFlag;
}

void resetDACFlag()
{
    gDACFlag = false;
}

/* This function is used by the scheduler when the DAC8 task is called */
void runDACTask()
{
    /* Update DAC8, sweep from 0-255 in 8 bit increments, then back to 0 */
    if (gDACValue <= 255)
        gDACValue += 8;
    else
        gDACValue = 0;

    DL_COMP_setDACCode0(COMP_0_INST, gDACValue);
}
