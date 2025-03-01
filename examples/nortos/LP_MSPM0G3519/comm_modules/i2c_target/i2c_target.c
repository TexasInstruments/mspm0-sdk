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
#include "ti/comm_modules/i2c/target/i2c_comm_target.h"

I2C_Instance gI2C;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);

    I2C_init(&gI2C);

    /* Enabled to prevent stale bytes being sent on the next I2C packet */
    DL_I2C_enableTargetTXWaitWhenTXFIFOStale(I2C_0_INST);

    while(1)
    {
        I2C_checkForCommand(&gI2C);
    }
}

void I2C_0_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_0_INST)) {

        /* START is included to support Repeated Start condition */
        case DL_I2C_IIDX_TARGET_START:
        case DL_I2C_IIDX_TARGET_STOP:
            /* Stop Condition of I2C Read Command */
            if(DL_DMA_getTransferSize(DMA,DMA_CH_RX_CHAN_ID) < MAX_BUFFER_SIZE)
            {
                DL_DMA_disableChannel(DMA, DMA_CH_RX_CHAN_ID);
                gI2C.status = I2C_STATUS_RX_BUFFERING;
            }
            /* Stop Condition of I2C Write Command */
            else
            {
                DL_DMA_disableChannel(DMA, DMA_CH_TX_CHAN_ID);
            }
            break;

        case DL_I2C_IIDX_TARGET_TXFIFO_EMPTY:
            /* Clear Stale data from Tx FIFO */
            if(DL_I2C_getTargetStatus(I2C_0_INST) & DL_I2C_TARGET_STATUS_STALE_TX_FIFO)
            {

               DL_I2C_startFlushTargetTXFIFO(I2C_0_INST);
               while (DL_I2C_getTargetStatus(I2C_0_INST) & DL_I2C_TARGET_STATUS_STALE_TX_FIFO) {
                    ;
               }
               DL_I2C_stopFlushTargetTXFIFO(I2C_0_INST);
            }

            /* Enable DMA Tx Channel if Response frame is prepared */
            if(gI2C.status == I2C_STATUS_TX_PREPARED)
            {
                DMA_TX_init(&gI2C,MAX_RESP_SIZE);
            }
            break;

        default:
            break;
    }
}

