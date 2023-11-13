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
//*****************************************************************************
//        GUI HAL for MSPM0L1306 using UART
//
// Driver to send and receive data from GUI using MSPM0L1306 UART
// Texas Instruments, Inc.
//*****************************************************************************
#include <ti/gui_composer/HAL.h>
#include "callbacks_mpack.h"
#include "ti_msp_dl_config.h"

tGUICommRXCharCallback RxByteISRCallback;

/* Variable to track the current running demo */
volatile demoMode gDemoMode = IDLE;

/* Counter to increment when S2 pressed */
static volatile uint16_t count = 0;

void HAL_System_Init(void)
{
    SYSCFG_DL_init();

    DL_SYSCTL_disableSleepOnExit();

    /* Enable UART interrupts */
    NVIC_ClearPendingIRQ(UART_INST_NVIC_IRQn);
    NVIC_EnableIRQ(UART_INST_NVIC_IRQn);

    /* Enable GPIO interrupts */
    NVIC_EnableIRQ(SWITCHES_USER_SWITCH_1_INT_IRQN);
}

void HAL_GUI_Init(tGUICommRXCharCallback RxByteCallback)
{
    /* Store callback for ISR RX Byte */
    RxByteISRCallback = RxByteCallback;
}

void HAL_GUI_TransmitCharBlocking(char character)
{
    DL_UART_transmitDataBlocking(UART_INST, character);
}

void UART_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            if (RxByteISRCallback != NULL) {
                RxByteISRCallback(DL_UART_Main_receiveDataBlocking(UART_INST));
            }
            break;
        default:
            break;
    }
}

/* Pressing S2 starts the next example */
void GPIOA_IRQHandler(void)
{
    /*
     * This IRQ is triggered when S2 is pressed on the LaunchPad.
     */
    switch (DL_GPIO_getPendingInterrupt(GPIOA)) {
        case SWITCHES_USER_SWITCH_1_IIDX:
            /*
             * At the start, the demo is operating in IDLE mode.
             * Each time S2 is pressed, the counter will increment by 1.
             * The counter will keep incremented until it reaches 10, in
             * which case it will reset back to 0.
             */
            if (count > MAX_COUNT) {
                count = 0;
            }
            GUIComm_sendUInt("counter", STR_LEN_SEVEN, count);
            count++;
            break;
        default:
            break;
    }
}
