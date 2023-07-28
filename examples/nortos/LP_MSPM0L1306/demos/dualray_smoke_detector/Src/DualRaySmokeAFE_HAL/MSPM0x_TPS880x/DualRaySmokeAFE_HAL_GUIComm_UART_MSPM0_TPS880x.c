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
//*****************************************************************************
#include <Src/DualRaySmokeAFE_HAL/DualRaySmokeAFE_HAL_Config_Private.h>
#include <Src/include/DualRaySmokeAFE_Config_Common.h>
#include <Src/include/DualRaySmokeAFE_GUI.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>

tGUICommRXCharCallback RxByteISRCallback;

void DualRaySmokeAFE_HAL_GUI_Init(tGUICommRXCharCallback RxByteCallback)
{
    // Baudrate set to fixed 9600 in config.c

    // Store callback for ISR RX Byte
    RxByteISRCallback = RxByteCallback;

    NVIC_EnableIRQ(GUI_UART_INST_INT_IRQN);  // Enable Interrupt
}

void DualRaySmokeAFE_HAL_GUI_TransmitCharBlocking(char character)
{
    DL_UART_transmitDataBlocking(GUI_UART_INST, character);
}

void GUI_UART_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(GUI_UART_INST)) {
        case DL_UART_MAIN_IIDX_RX:

            if (RxByteISRCallback != NULL) {
                if (RxByteISRCallback(DL_UART_Main_receiveDataBlocking(
                        GUI_UART_INST)) == true) {
                    DL_SYSCTL_disableSleepOnExit();  // exit LPM
                }
            }
            break;
        default:
            break;
    }
}
