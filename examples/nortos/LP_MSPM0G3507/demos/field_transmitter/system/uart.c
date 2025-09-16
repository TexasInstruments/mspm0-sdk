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

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ti_msp_dl_config.h"

#include "adc.h"
#include "adc_condition.h"
#include "out_condition.h"
#include "output.h"
#include "system/system.h"
#include "system/uart.h"

static char gUartRxBuffer[32];
static char UartRxPos = 0;
static char gUartTxBuffer[128];
static char UartTxPos = 0;
static char UartTxLen = 0;
bool UartFlag         = false;
static enum status_enum help_cmd(char *cmd);

struct command {
    char *name;
    char *help;
    enum status_enum (*function)(char *cmd);
};

static struct command commands[6];
#if 0
static void uart_puts(char *str){
    while(*str != 0)
        DL_UART_Main_transmitDataBlocking(UART_CMD_INST, *(str++)); //todo: write non blocking with wfi
}
#endif

/**
 * @brief fills the command list structure with correct function pointers
 *
 */
void uart_cmd_init(void)
{
    int i = 0;

    commands[i].name     = "sys";
    commands[i].function = system_cmd;
    commands[i++].help   = "configure system";
    commands[i].name     = "adc";
    commands[i].function = adc_cmd;
    commands[i++].help   = "configure adc subsystem";
    commands[i].name     = "out";
    commands[i].function = output_cmd;
    commands[i++].help   = "configure output subsystem";
    commands[i].name     = "cin";
    commands[i].function = condition_adc_cmd;
    commands[i++].help   = "configure adc data conditioning system";
    commands[i].name     = "cout";
    commands[i].function = condition_out_cmd;
    commands[i++].help   = "configure output data conditioning system";
    commands[i].name     = "help";
    commands[i].function = help_cmd;
    commands[i++].help   = "show this message";
}

static enum status_enum help_cmd(char *cmd)
{
    int i;
    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        uart_printf("%-7s - 0x%x - %s\r\n", commands[i].name,
            commands[i].function, commands[i].help);
    }
    return STATUS_OK;
}

void uart_handle_cmd(void)
{
    int i;
    bool found = false;
    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (!strcmp(commands[i].name, strtok(gUartRxBuffer, " "))) {
            // command found, call it
            uart_printf("ret: %d\r\n",
                commands[i].function(
                    gUartRxBuffer + strlen(commands[i].name) + 1));
            found = true;
            break;
        }
    }
    if (found == false) {  //unknown command
        uart_printf("command not found - try help\r\n");
    }
    memset(gUartRxBuffer, 0, sizeof(gUartRxBuffer));
}

/**
 * Printf to UART
 * @param format format to print
 */
void uart_printf(const char *format, ...)
{
    //char buffer[128];
    va_list args;

    while (UartTxLen != 0)  // there is still something sending, wait...
        asm(" wfi");

    va_start(args, format);
    UartTxLen = vsnprintf(gUartTxBuffer, sizeof(gUartTxBuffer), format, args);
    UartTxPos = 0;
    DL_UART_Main_enableInterrupt(UART_CMD_INST, DL_UART_MAIN_INTERRUPT_TX);
    va_end(args);
}

void UART_CMD_INST_IRQHandler(void)
{
    char data;
    switch (DL_UART_Main_getPendingInterrupt(UART_CMD_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            data = DL_UART_Main_receiveData(UART_CMD_INST);
            DL_UART_Main_transmitData(UART_CMD_INST, data);
            if (UartRxPos < sizeof(gUartRxBuffer)) {
                gUartRxBuffer[UartRxPos++] = data;
            }
            if (data == '\r' || data == '\n') {
                while (DL_UART_isTXFIFOFull(UART_CMD_INST))
                    ;
                DL_UART_Main_transmitData(UART_CMD_INST, '\n');
                gUartRxBuffer[UartRxPos - 1] = 0;
                UartFlag =
                    true;  // this flag is checked outside of IRQ context in system_cyclic and triggers uart_handle_cmd
                UartRxPos = 0;
            }
            break;
        case DL_UART_MAIN_IIDX_TX:
            DL_UART_Main_transmitData(
                UART_CMD_INST, gUartTxBuffer[UartTxPos++]);
            if (UartTxPos == UartTxLen) {
                DL_UART_Main_disableInterrupt(
                    UART_CMD_INST, DL_UART_MAIN_INTERRUPT_TX);
                UartTxLen = 0;
            }
        default:
            break;
    }
}

void uart_init(void)
{
    NVIC_ClearPendingIRQ(UART_CMD_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_CMD_INST_INT_IRQN);
}
