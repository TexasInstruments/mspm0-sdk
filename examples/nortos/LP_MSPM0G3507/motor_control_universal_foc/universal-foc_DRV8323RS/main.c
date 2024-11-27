/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
#include "main.h"
#include "ISR.h"
#include "focPeriphInit.h"
#include "uart_comm.h"
#include "gateDriver.h"

/*! @brief Enable gate driver read test */
_Bool gdReadTestEn = 0;

/*! @brief Data read from gate driver */
volatile uint16_t regData = 0;

/*! @brief Register address to read from gate driver
* Note that reading the registers are only applicable
* to gate driver with communication interface like SPI
*/
volatile int16_t regAddr = 0x06;

int main(void)
{
    SYSCFG_DL_init();

    appInputReset(); /* Reset all the Application Input variables to Zero. */

    appReset(g_pMC_App); /* Reset all the Application Data variables to Zero */

    userInputsInit(); /* Initialize the Default Motor Parameters */

    appConfig(); /* Loads the application configurations */

    focPeriphInit();  /* Does foc application specific Peripheral configurations */

    while (1)
    {
        if(gdReadTestEn)
        {
            regData =  gateDriverRegRead(regAddr);
        }
        UART_checkForCommand(pUART);

        updateConfigs();
    }

}
