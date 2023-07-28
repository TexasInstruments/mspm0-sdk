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
//         GUI Composer Simple JSON Demo using MSP430
//
// Texas Instruments, Inc.
// ******************************************************************************

#include "ti_msp_dl_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/HAL.h>
#include <ti/gui_composer/IQMathLib/QmathLib.h>
#include "MSP_GUI/GUIComm_JSON.h"
#include "MSP_GUI/GUI_JSON.h"
#include "callbacks_JSON.h"

/* Macros for command string lengths */
#define STR_LEN_TWO 2
#define STR_LEN_SEVEN 7

/* Global variable use to track the of state of GUI and counter values */
volatile uint8_t u8Counter;
volatile uint16_t u16Counter;
volatile uint32_t u32Counter;
volatile _q
    qCounter; /* Q8 is used because the variable qCounter ranges 0.0-100.0 */
volatile bool bUpdateGUI;
extern volatile bool bEnableSwitch;
extern volatile _q qIncrement;

//! \brief RX Command structure.
//!         The corresponding callback will be called when the command is
//!         received from GUI.
//! Note: shorter command names take less time to process
const tGUI_RxCmd GUI_RXCommands[] = {
    {"bEnable", callback_boolEnable},
    {"u16Data", callback_QMathData},
};

int main(void)
{
    /* Initialize peripherals */
    HAL_System_Init();

    /* Initialize GUI layer */
    GUI_Init();

    /* Initialize the RX command callbacks */
    GUI_InitRxCmd(&GUI_RXCommands[0],
        (sizeof(GUI_RXCommands) / sizeof(GUI_RXCommands[0])));

    bUpdateGUI = false; /* Update GUI first time */
    u8Counter =
        50; /* Counter8 will be updated on SW1 press in increments of 50 */
    u16Counter =
        5000; /* Counter16 will be updated on button press in increments of 5000 */
    u32Counter =
        10000; /* Counter32 will be updated on timer in increments of 10000 */
    bEnableSwitch = true; /* Switch enables updating QMath */
    qCounter      = _Q(
        0.5); /* QMath counter updated on timer based on boolean in increments of qIncrement */
    qIncrement = _Q(0.5);

    /* Send default value of variables */
    GUIComm_sendUInt8("c1", STR_LEN_TWO, u8Counter);
    GUIComm_sendUInt16("c2", STR_LEN_TWO, u16Counter);
    GUIComm_sendUInt32("c3", STR_LEN_TWO, u32Counter);
    GUIComm_sendInt16("c4", STR_LEN_TWO, (int16_t) qCounter);
    GUIComm_sendInt16("u16Data", STR_LEN_SEVEN, (int16_t) qIncrement);
    GUIComm_sendBool("bEnable", STR_LEN_SEVEN, bEnableSwitch);

    /* Wait for ACK from GUI */
    __WFI();

    while (1) {
        /* Send updates to GUI when there is a periodic timer interrupt */
        while (bUpdateGUI == true) {
            GUIComm_sendUInt8("c1", STR_LEN_TWO, u8Counter);
            GUIComm_sendUInt16("c2", STR_LEN_TWO, u16Counter);
            GUIComm_sendUInt32("c3", STR_LEN_TWO, u32Counter);
            if (bEnableSwitch == true) {
                GUIComm_sendInt16("c4", STR_LEN_TWO, (int16_t) qCounter);
            }

            bUpdateGUI = false;
        }

        if (bUpdateGUI == false) {
            __WFE();
        }
    }
}

void TIMER_INST_IRQHandler(void)
{
    /* Update 32-bit counter */
    u32Counter += 10000;
    bUpdateGUI = true;
    /* If switch is enabled, update qCounter */
    if (bEnableSwitch == true) {
        qCounter += qIncrement;

        /* Reset counter when value is over 100.0 */
        if (qCounter > _Q(100.0)) {
            qCounter = 0;
        }
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case DL_INTERRUPT_GROUP1_IIDX_GPIOB:
            /* When USER_SWITCH_1 is pressed, update 8-bit counter */
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES1_PORT)) {
                case SWITCHES1_USER_SWITCH_1_IIDX:
                    u8Counter += 50;
                    break;
                default:
                    break;
            }
            break;
        case DL_INTERRUPT_GROUP1_IIDX_GPIOA:
            /* When USER_SWITCH_2 is pressed, update 16-bit counter */
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES2_PORT)) {
                case SWITCHES2_USER_SWITCH_1_IIDX:
                    u16Counter += 5000;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
