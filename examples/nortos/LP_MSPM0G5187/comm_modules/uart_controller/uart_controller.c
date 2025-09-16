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

#include "ti/comm_modules/uart/controller/uart_comm_controller.h"
#include "ti_msp_dl_config.h"

UART_Instance gUART;

volatile UART_CommandInfo gCommand;

UART_ResponseInfo gResponse;

volatile bool gSendCommand = false;

/* Data to send to Target */
uint8_t gTxData[MAX_DATA_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
    0x1F};

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    UART_init(&gUART);

    /* Initialize gResponse fields
     * received is set as true because command can be sent only if the
     * previous command gets a response
     */
    gResponse.received = true;
    gResponse.dataSize = 0;

    /* Default CommandInfo Object */
    UART_CommandInfo CommandInfo_Obj;

    CommandInfo_Obj.targetAddr  = 0xAD;
    CommandInfo_Obj.commandType = READ_COMMAND;
    CommandInfo_Obj.memAddr     = 0x20207C00;
    CommandInfo_Obj.dataArray   = &gTxData[0];
    CommandInfo_Obj.dataSize    = 4;
    CommandInfo_Obj.crcEnable   = true;

    gCommand = CommandInfo_Obj;

    while (1) {
        /* Decodes the received data in Rx Buffer for response */
        UART_checkForResponse(&gUART, &gResponse);

        /* Checks whether UART Status is idle and response is received for the previous command */
        if (gSendCommand == true && gUART.status == UART_STATUS_IDLE &&
            gResponse.received == true) {
            gResponse.received = false;

            UART_sendCommand(&gUART, (UART_CommandInfo *) &gCommand);

            gSendCommand = false;
        }
    }
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX: {
            uint8_t rxData = DL_UART_Main_receiveData(UART_0_INST);
            UART_storeRxData(&gUART, rxData);
            break;
        }
        default:
            break;
    }
}
