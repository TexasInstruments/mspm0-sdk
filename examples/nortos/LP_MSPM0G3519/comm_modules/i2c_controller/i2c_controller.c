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


#include "ti_msp_dl_config.h"
#include "ti/comm_modules/i2c/controller/i2c_comm_controller.h"

I2C_Instance gI2C;

volatile I2C_CommandInfo gCommand;

I2C_ResponseInfo gResponse;

volatile bool gSendCommand = false;

/* Data to send to target */
uint8_t gTxData[MAX_DATA_SIZE] = {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF, \
                                  0x00,0x11,0x22,0x33,0x44,0x55, \
                                  0x66,0x77,0x88,0x99,0x12,0x23, \
                                  0x34,0x45,0x56,0x67,0x78,0x89};

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(I2C_INST_INT_IRQN);

    I2C_init(&gI2C);

    /* Default CommandInfo Object */
    I2C_CommandInfo CommandInfo_Obj;

    CommandInfo_Obj.targetAddr = DEF_TARGET_ADDR;
    CommandInfo_Obj.commandType = READ_COMMAND;
    CommandInfo_Obj.addr = 0x20207C00;
    CommandInfo_Obj.dataArray = &gTxData[0];
    CommandInfo_Obj.dataSize = 4;
    CommandInfo_Obj.crcEnable = false;

    gCommand = CommandInfo_Obj;

    while(1)
    {
        if(gSendCommand == true)
        {
            /* Frames a command packet and sends it to Target through I2C Write Command */
            I2C_sendCommand(&gI2C, (I2C_CommandInfo *) &gCommand);

            gI2C.status = I2C_STATUS_RX_STARTED;
            gI2C.rxMsg.ptr = 0;

            /* Issues I2C Read Command to get the response from Target */
            I2C_getResponse(&gI2C,gCommand.targetAddr);

            gSendCommand = false;
        }
    }
}

void I2C_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) 
    {
        case DL_I2C_IIDX_CONTROLLER_RX_DONE:
            gI2C.rxMsg.len = gI2C.rxMsg.ptr;
            I2C_decodeResponse(&gI2C,&gResponse);
            gI2C.status = I2C_STATUS_RX_COMPLETE;
            break;

        case DL_I2C_IIDX_CONTROLLER_TX_DONE:
            DL_I2C_disableInterrupt(
                I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
            gI2C.status = I2C_STATUS_TX_COMPLETE;
            break;

        case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
            gI2C.status = I2C_STATUS_RX_INPROGRESS;
            /* Store bytes received from target in Rx Msg Buffer */
            while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST) != true) {
                if (gI2C.rxMsg.ptr < MAX_BUFFER_SIZE) {
                    gI2C.rxMsg.buffer[gI2C.rxMsg.ptr++] =
                        DL_I2C_receiveControllerData(I2C_INST);
                } else {
                    /* Ignore and remove from FIFO if the buffer is full */
                    DL_I2C_receiveControllerData(I2C_INST);
                }
            }
            break;

        case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
            gI2C.status = I2C_STATUS_TX_INPROGRESS;
            /* Fill TX FIFO with bytes to send */
            if (gI2C.txMsg.ptr < gI2C.txMsg.len) {
                gI2C.txMsg.ptr += DL_I2C_fillControllerTXFIFO(
                    I2C_INST, &gI2C.txMsg.buffer[gI2C.txMsg.ptr], gI2C.txMsg.len - gI2C.txMsg.ptr);
            }
            break;

        case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        case DL_I2C_IIDX_CONTROLLER_NACK:
            gI2C.status = I2C_STATUS_ERROR;
            break; 
        default:
            break;
    }
}
