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
//*****************************************************************************
// Blood pressure monitor demo
//
// Texas Instruments, Inc.
//*****************************************************************************

#include <cuff.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/HAL.h>
#include "MSP_GUI/GUIComm_mpack.h"
#include "MSP_GUI/GUI_mpack.h"
#include "callbacks_mpack.h"
#include "demo_mode_declaration.h"
#include "ti_msp_dl_config.h"

/* Macros for command string lengths */
#define STR_LEN_TWO 2
#define STR_LEN_FIVE 5
#define STR_LEN_SEVEN 7
#define STR_LEN_EIGHT 8

//! \brief RX Command structure.
//!         The corresponding callback will be called when the command is
//!         received from GUI.
//! Note: shorter command names take less time to process
const tGUI_RxCmd GUI_RXCommands[] = {
    {"startGUI", callback_startGUI},
    {"IC", callback_inflateCuff},
    {"DC", callback_deflateCuff},
    {"clear", callback_clearGUI},
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

    SYSCFG_DL_init();

    /* Begin ADC readings of cuff pressure and oscillations */
    ADC_init();
    NVIC_EnableIRQ(ADC_INST_INT_IRQN);

    /* Start timer that triggers ADC sampling */
    DL_TimerG_startCounter(TIMER_ADC_INST);

    /* Turn off motor and valve high (cuff deflating) */
    DL_GPIO_clearPins(GPIO_PINS_PORT, GPIO_PINS_MOTOR_PIN);
    DL_GPIO_setPins(GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN);

    /* Wait for ACK from GUI */
    __WFI();

    while (1) {
        if (startupFlag) {
            /* Reset peripherals, used when multiple cycles of code run */
            SYSCFG_DL_init();
            /* Enable ADC */
            ADC_init();
            NVIC_EnableIRQ(ADC_INST_INT_IRQN);
            DL_TimerG_startCounter(
                TIMER_ADC_INST); /* Start timer that triggers ADC sampling */
            NVIC_EnableIRQ(UART_INST_INT_IRQN);
            DL_UART_enable(UART_INST);
            startupFlag = 0;
        }
        switch (gDemoMode) {
            case CUFF_ACTIVE_MODE:
                run_activeMode();
                break;
            case START_GUI:
                delay_cycles(16000000);
                setupExample();
                delay_cycles(16000000);  // Delays for ADC
                break;
            case IDLE:
                break;
            case DEFLATE:
                NVIC_DisableIRQ(ADC_INST_INT_IRQN);
                NVIC_ClearPendingIRQ(ADC_INST_INT_IRQN);
                /* Deflate cuff */
                DL_GPIO_clearPins(GPIO_PINS_PORT, GPIO_PINS_MOTOR_PIN);
                DL_GPIO_setPins(GPIO_PINS_PORT, GPIO_PINS_VALVE_PIN);
                gDemoMode = IDLE;
                break;
            case CLEAR:
                cleanupExample();
                gDemoMode = IDLE;
                break;
            default:
                break;
        }
    }
}
