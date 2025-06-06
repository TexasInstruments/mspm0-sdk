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
#include "i2c_target.h"
#include "ti_msp_dl_config.h"

/*============================ EXTERNS =====================================*/

/*====================== TYPEDEFS/DEFINES/MACROS ===========================*/

/*============================ VARIABLES ===================================*/
bool g_bMessageReady = false;

/* Data sent to the Controller */
uint8_t TxBuffer[I2C_TARGET_TX_MAX_PACKET_SIZE] = {0};

/* Counters for TX length and bytes sent */
uint8_t TxLen;
uint8_t TxCount;

/* Data received from Controller */
uint8_t RxBuffer[I2C_TARGET_RX_MAX_PACKET_SIZE] = {0};

/* Counters for RX length and bytes sent */
uint8_t RxLen;
uint8_t RxCount;

/*====================== FORWARD DECLARATIONS ==============================*/

/*============================ FUNCTIONS ===================================*/
void deSerializePacket(tPacket* pPacket, uint8_t* pBuffer)
{
    pPacket->PortNum  = *pBuffer++;
    pPacket->Register = *pBuffer++;
    pPacket->Data.B3  = *pBuffer++;
    pPacket->Data.B2  = *pBuffer++;
    pPacket->Data.B1  = *pBuffer++;
    pPacket->Data.B0  = *pBuffer;
}

void serializePacket(tPacket* pPacket, uint8_t* pBuffer)
{
    *pBuffer++ = pPacket->PortNum;
    *pBuffer++ = pPacket->Register;
    *pBuffer++ = pPacket->Data.B3;
    *pBuffer++ = pPacket->Data.B2;
    *pBuffer++ = pPacket->Data.B1;
    *pBuffer = pPacket->Data.B0;
}

void I2C_TARGET_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_TARGET_INST)) {

        case DL_I2C_IIDX_TARGET_START:

            if(gI2cTargetStatus == I2C_STATUS_IDLE)
            {
                if(!DL_I2C_isTargetClockStretchingEnabled(I2C_TARGET_INST))
                    DL_I2C_enableTargetClockStretching(I2C_TARGET_INST);

                RxCount = 0;
                RxLen   = I2C_TARGET_RX_PACKET_SIZE;
                gI2cTargetStatus = I2C_STATUS_RX_INPROGRESS;
                g_bMessageReady = false;

            }/* START followed by RESTART -> Prepare data for controller to read */
            else if(gI2cTargetStatus == I2C_STATUS_RX_INPROGRESS)
            {
                TxCount = 0;
                TxLen   = I2C_TARGET_TX_PACKET_SIZE;
                /* Flush TX FIFO to refill it */
                DL_I2C_flushTargetTXFIFO(I2C_TARGET_INST);
                gI2cTargetStatus = I2C_STATUS_TX_INPROGRESS;
                g_bMessageReady = true;
            }
            break;

        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in user buffer */
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_TARGET_INST) != true) {
                if (RxCount < RxLen) {
                    RxBuffer[RxCount++] = DL_I2C_receiveTargetData(I2C_TARGET_INST);
                }
                else
                {
                    /* Prevent overflow and just ignore data */
                    DL_I2C_receiveTargetData(I2C_TARGET_INST);
                }
            }
            break;

        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:
            /* Fill TX FIFO from user buffer */
            if (TxCount < TxLen) {
                TxCount += DL_I2C_fillTargetTXFIFO(
                I2C_TARGET_INST, &TxBuffer[TxCount], (TxLen - TxCount));
            }
            else
            {
                 /* Fill FIFO with 0x00 if more data is requested than expected gTxLen */
                while (DL_I2C_transmitTargetDataCheck(I2C_TARGET_INST, 0x00) != false);

            }
            break;

        case DL_I2C_IIDX_TARGET_STOP:


            if(gI2cTargetStatus == I2C_STATUS_RX_INPROGRESS)
            {
                /* Come here if TARGET READ complete */
                g_bMessageReady = true;

                gI2cTargetStatus = I2C_STATUS_IDLE;
            }
            /* Come here if TARGET WRITE complete */
            else if(gI2cTargetStatus == I2C_STATUS_TX_INPROGRESS)
            {
                g_bMessageReady = false;

                while (!(DL_I2C_getTargetStatus(I2C_TARGET_INST) & DL_I2C_TARGET_STATUS_BUS_BUSY)){};
                DL_I2C_flushTargetTXFIFO(I2C_TARGET_INST);

                gI2cTargetStatus = I2C_STATUS_IDLE;
            }

            break;

        case DL_I2C_IIDX_TARGET_RX_DONE:
        case DL_I2C_IIDX_TARGET_TX_DONE:
        case DL_I2C_IIDX_TARGET_RXFIFO_FULL:
        case DL_I2C_IIDX_TARGET_TXFIFO_EMPTY:
        case DL_I2C_IIDX_TARGET_GENERAL_CALL:
        case DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE:
        case DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE:
        default:
            break;
    }
}

