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
#include "i2c_controller.h"

/*============================ EXTERNS =====================================*/

/*====================== DEFINES/MACROS ====================================*/
#define SUPPORT_I2C_RESTART             (true)

/*============= USER DEFINED TYPES / ENUMERATIONS ==========================*/

/*============================ VARIABLES ===================================*/

/* Transmit buffer */
uint8_t TxBuffer[I2C_CONTROLLER_TX_PACKET_SIZE] = {0};

/* Counters for TX length and bytes sent */
uint8_t TxLen;
uint8_t TxCount;

/* Receive buffer */
uint8_t RxBuffer[I2C_CONTROLLER_RX_PACKET_SIZE] = {0};

/* Counters for RX length and bytes sent */
uint8_t RxLen;
uint8_t RxCount;

static bool bTimerBusyFlag;
static bool periodicTimerFlag;

/*====================== FORWARD DECLARATIONS ==============================*/
static void SerializeData(tPacket* pPacket);
static void DeSerializeData(tPacket* pPacket);
static void I2C_Write(tPacket* pPacket);
static uint32_t I2C_Read(tPacket* pPacket);
static void _i2c_write(void);
static void _i2c_read(void);

/*============================ FUNCTIONS ===================================*/
/*
 * Note: Controller passes pin(s) selection to Target.  Target will apply
 * input or output mask to ensure only active IO pins are used.  No need to
 * mask anything from this side.
 */
uint32_t IOx_readPins(tPacket* pPacket)
{
    pPacket->Register = eDIN;
    pPacket->Data.ui32Full = 0x00000000;

    /* Target will mask the result */
    return(I2C_Read(pPacket));
}

void IOx_writePins(tPacket* pPacket, uint32_t Pins)
{
    pPacket->Register = eDOUT;
    pPacket->Data.ui32Full = Pins;
    I2C_Write(pPacket);
}

void IOx_clearPins(tPacket* pPacket, uint32_t Pins)
{
    pPacket->Register = eDOUT_CLEAR;
    pPacket->Data.ui32Full = Pins;
    I2C_Write(pPacket);
}

void IOx_setPins(tPacket* pPacket, uint32_t Pins)
{
    pPacket->Register = eDOUT_SET;
    pPacket->Data.ui32Full = Pins;
    I2C_Write(pPacket);
}

void IOx_togglePins(tPacket* pPacket, uint32_t Pins)
{
    pPacket->Register = eDOUT_TOGGLE;
    pPacket->Data.ui32Full = Pins;
    I2C_Write(pPacket);
}

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


void I2C_Write(tPacket* pPacket)
{
    RxLen              = I2C_CONTROLLER_RX_PACKET_SIZE;
    RxCount            = 0;
    TxLen              = I2C_CONTROLLER_TX_PACKET_SIZE;
    TxCount            = 0;

    serializePacket(pPacket, TxBuffer );

    _i2c_write();
}

/*
 * High level read function.
 * Depending upon the interface this could be
 * I2C_Read, UART_Read, SPI_Read, etc.
 *
 * Data read is returned or can be found in the
 * port structure.
 */
uint32_t I2C_Read(tPacket* pPacket)
{
    /* Send a read request to Target */
    RxLen              = I2C_CONTROLLER_RX_PACKET_SIZE;
    RxCount            = 0;
    TxLen              = I2C_CONTROLLER_TX_PACKET_SIZE;
    TxCount            = 0;

    serializePacket(pPacket, TxBuffer);

    _i2c_read();

    deSerializePacket(pPacket, RxBuffer);

    return(pPacket->Data.ui32Full);

}

/*==================================================
 * _i2c_write()
 *
 * Performs the register level write actions.
 * Params In: void
 * Returns  : void
 *
 * Outgoing data is stored in TxBuffer[]
 */
static void _i2c_write(void)
{
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;

    /*
     * Fill the FIFO
     * The FIFO is 8-bytes deep, and this function will return number
     * of bytes written to FIFO */
    TxCount = DL_I2C_fillControllerTXFIFO(I2C_CONTROLLER_INST, &TxBuffer[0], TxLen);

    /* Enable TXFIFO trigger interrupt if there are more bytes to send */
    if (TxCount < TxLen) {
        DL_I2C_enableInterrupt(
                I2C_CONTROLLER_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(
                I2C_CONTROLLER_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }

    /*
     * Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE)){};

        DL_I2C_startControllerTransfer(
               I2C_CONTROLLER_INST, I2C_TARGET_IOX_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, TxLen);

    /* Wait until the Controller sends all bytes */
    while ((gI2cControllerStatus != I2C_STATUS_TX_COMPLETE) &&
           (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFE();
    }

    while (DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

    /* Check if there was an error */
    if (DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        gI2cControllerStatus = I2C_STATUS_ERROR;
    }
}


#if (SUPPORT_I2C_RESTART==true)
/*
 * DriverLib doesn't appear to have a function to handle repeat start.
 * Copied/modified from dl_i2c.h to create a repeat start function.
 */
__STATIC_INLINE void DL_I2C_repeatedStartControllerTransfer(I2C_Regs *i2c,
    uint32_t targetAddr, DL_I2C_CONTROLLER_DIRECTION direction, uint32_t length)
{
    /* Specify target address and read/write mode */
    DL_Common_updateReg(&i2c->MASTER.MSA,
        ((targetAddr << I2C_MSA_SADDR_OFS) | (uint32_t) direction),
        (I2C_MSA_SADDR_MASK | I2C_MSA_DIR_MASK ));

    /* NO STOP bit is generated after burst length number of bytes transferred */
    DL_Common_updateReg(&i2c->MASTER.MCTR,
        (((uint32_t) length << I2C_MCTR_MBLEN_OFS) | I2C_MCTR_BURSTRUN_ENABLE |
            I2C_MCTR_START_ENABLE | I2C_MCTR_RD_ON_TXEMPTY_ENABLE | I2C_MCTR_STOP_ENABLE),
        (I2C_MCTR_MBLEN_MASK | I2C_MCTR_BURSTRUN_MASK | I2C_MCTR_START_MASK | I2C_MCTR_STOP_MASK | I2C_MCTR_RD_ON_TXEMPTY_MASK));
}
#endif

/*===================================================================
 * _i2c_read()
 *
 * Performs the register level actions.
 * Params In: void
 * Returns  : Success/Fail
 *
 * The incoming data is stored in RxBuffer[] which is available
 * for further processing.
 *
 */
static void _i2c_read(void)
{
    /*
     * Fill the FIFO
     * The FIFO is 8-bytes deep, and this function will return number
     * of bytes written to FIFO */
    TxCount = DL_I2C_fillControllerTXFIFO(I2C_CONTROLLER_INST, &TxBuffer[0], TxLen);

    /* Enable TXFIFO trigger interrupt if there are more bytes to send */
    if (TxCount < TxLen) {
        DL_I2C_enableInterrupt(
                I2C_CONTROLLER_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(
                I2C_CONTROLLER_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }

    /* Update Status */
    gI2cControllerStatus = I2C_STATUS_RX_STARTED;
    while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE)){};

#if (SUPPORT_I2C_RESTART==true)
        DL_I2C_repeatedStartControllerTransfer(
                I2C_CONTROLLER_INST, I2C_TARGET_IOX_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_RX, RxLen);
#else
        DL_I2C_startControllerTransfer(
               I2C_CONTROLLER_INST, I2C_TARGET_IOX_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_RX, RxLen);
#endif

    /* Wait until the Controller sends all bytes */
    while ((gI2cControllerStatus != I2C_STATUS_RX_COMPLETE) &&
           (gI2cControllerStatus != I2C_STATUS_ERROR)) {
        __WFE();
    }

    /* Wait for bus to not be BUSY */
    while (DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

    /* Check if there was an error */
    if (DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        gI2cControllerStatus = I2C_STATUS_ERROR;
    }
}

/*===================================================================
 * I2C Interrupt Handler
 */
void I2C_CONTROLLER_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_CONTROLLER_INST)) {

    case DL_I2C_IIDX_CONTROLLER_RX_DONE:
        gI2cControllerStatus = I2C_STATUS_RX_COMPLETE;
        break;
    case DL_I2C_IIDX_CONTROLLER_TX_DONE:
        DL_I2C_disableInterrupt(
                I2C_CONTROLLER_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
        gI2cControllerStatus = I2C_STATUS_TX_COMPLETE;
        break;
    case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
        gI2cControllerStatus = I2C_STATUS_RX_INPROGRESS;
        /* Receive all bytes from target */
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_CONTROLLER_INST) != true) {
            if (RxCount < RxLen) {
                RxBuffer[RxCount++] =
                    DL_I2C_receiveControllerData(I2C_CONTROLLER_INST);
            } else {
                /* Ignore and remove from FIFO if the buffer is full */
                DL_I2C_receiveControllerData(I2C_CONTROLLER_INST);
            }
        }
        break;
    case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:
        gI2cControllerStatus = I2C_STATUS_TX_INPROGRESS;
        /* Fill TX FIFO with next bytes to send */
        if (TxCount < TxLen) {
            TxCount += DL_I2C_fillControllerTXFIFO(
                    I2C_CONTROLLER_INST, &TxBuffer[TxCount], TxLen - TxCount);
        }
        break;
        /* Not used for this example */
    case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
    case DL_I2C_IIDX_CONTROLLER_NACK:
        if ((gI2cControllerStatus == I2C_STATUS_RX_STARTED) ||
            (gI2cControllerStatus == I2C_STATUS_TX_STARTED)) {
            /* NACK interrupt if I2C Target is disconnected */
            gI2cControllerStatus = I2C_STATUS_ERROR;
        }
    case DL_I2C_IIDX_CONTROLLER_RXFIFO_FULL:
    case DL_I2C_IIDX_CONTROLLER_TXFIFO_EMPTY:
    case DL_I2C_IIDX_CONTROLLER_START:
    case DL_I2C_IIDX_CONTROLLER_STOP:
    case DL_I2C_IIDX_CONTROLLER_EVENT1_DMA_DONE:
    case DL_I2C_IIDX_CONTROLLER_EVENT2_DMA_DONE:
    default:
        break;
    }
}

/* For debugging CONTROLLER on the Launchpad*/
void I2C_errorTrap(void)
{
    while(1)
    {
        DL_GPIO_togglePins(GPIO_B_PORT, GPIO_B_LED_RGB_RED_PIN);
        msecDelayBlocking(100);
        bTimerBusyFlag = false;
    }

}
