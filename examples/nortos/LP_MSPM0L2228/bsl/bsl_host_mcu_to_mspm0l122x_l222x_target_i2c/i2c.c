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
#include <bsl_i2c.h>
#include <i2c.h>
#include "stdint.h"
#include "ti_msp_dl_config.h"

uint8_t test_d;
uint8_t I2C_writeBuffer(uint8_t *pData, uint8_t ui8Cnt)
{
    uint8_t gTxCount;

    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_0_INST, pData, ui8Cnt);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_startControllerTransfer(I2C_0_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_TX, ui8Cnt);
    while (gTxCount < ui8Cnt) {
        while (DL_I2C_getRawInterruptStatus(
                   I2C_0_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_EMPTY) == 0)
            ;
        gTxCount += DL_I2C_fillControllerTXFIFO(
            I2C_0_INST, pData + gTxCount, ui8Cnt - gTxCount);
    }
    /* Poll until the Controller writes all bytes */
    while (DL_I2C_getControllerStatus(I2C_0_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_0_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* LED will remain high if there is an error */
        return 1;
    }
    // res = DL_UART_receiveDataBlocking(UART_0_INST);
    return 0;
}

void I2C_readBuffer(uint8_t *pData, uint8_t ui8Cnt)
{
    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(I2C_0_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_RX, ui8Cnt);

    /*
     * Receive all bytes from target. LED will remain high if not all bytes
     * are received
     */
    for (uint8_t i = 0; i < ui8Cnt; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_0_INST))
            ;
        *pData = DL_I2C_receiveControllerData(I2C_0_INST);
        pData++;
    }
}

//uint8_t test_d;

uint8_t Status_check(void)
{
    i2c_error_t i2c_ack;
    uint8_t buf[1] = {0xBB};
    i2c_ack        = I2C_writeBuffer(buf, 1);
    I2C_readBuffer(BSL_RX_buffer, 1);
    return BSL_RX_buffer[0];
}
