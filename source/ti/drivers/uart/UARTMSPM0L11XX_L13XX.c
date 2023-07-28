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
/* clang-format off */
#include <string.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/uart/UARTMSPM0L11XX_L13XX.h>

static bool UARTMSP_enable(UART_Handle handle, UART_Params *params);
static void UARTMSP_disable(UART_Handle handle);
static void UARTMSP_txChar(UART_Handle handle);

const UARTSupport_Fxns UARTMSPSupportFxns =
{
    .enable  = UARTMSP_enable,
    .disable = UARTMSP_disable,
    .txChar  = UARTMSP_txChar,
    .read    = UART_readFullFeatured,
    .write   = UART_writeFullFeatured,
};

/*
 *  ======== UARTMSP_eventCallback ========
 *  A dummy event callback function in case user didn't provide one
 */
void __WEAK UARTMSP_eventCallback(UART_Handle handle, uint32_t event, uint32_t data, void *userArg)
{
}

/*!
 *  @brief      Initialization helper for buffer support.
 *
 *  @param[in]  object    reference to the buffer related metadata.
 */
__STATIC_INLINE void UARTMSP_initBuffers(UART_Buffers_Object *object)
{
    RingBuf_construct(&object->rxBuf, object->rxBufPtr, object->rxBufSize);
    RingBuf_construct(&object->txBuf, object->txBufPtr, object->txBufSize);

    object->rxSem = SemaphoreP_createBinary(1);
    object->txSem = SemaphoreP_createBinary(1);
}

/* Default UART parameters structure */
const UART_Params UART_defaultParams =
{
    UART_Mode_BLOCKING,
    UART_Mode_BLOCKING,
    NULL,
    NULL,
    NULL,
    0,
    UART_ReadReturnMode_PARTIAL,
    9600,
    UART_DataLen_8,
    UART_StopBits_1,
    UART_Parity_NONE,
    NULL,
};

/*
 * ======== UARTMSP_enable ========
 */
static bool UARTMSP_enable(UART_Handle handle, UART_Params *params)
{
    uint32_t divratio = 0;
    uint32_t uartClk  = 0;
    uint32_t clkFreq  = 0;

    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);
    UART_Object *uartObject                = UART_Obj_Ptr(handle);
    UART_Buffers_Object *buffersObject = UART_buffersObject(uartObject);
    UART_EventCallback_Object *eventCallbackObj = UART_eventCallbackObject(uartObject);


    /* Set the event mask to 0 if the callback is NULL to simplify checks, load default eventCallback function */
    if ((uartObject->eventsSupported == true) && (eventCallbackObj->eventCallback == NULL))
    {
        eventCallbackObj->eventCallback = UARTMSP_eventCallback;
        eventCallbackObj->eventMask     = 0;
    }

    if (uartObject->buffersSupported == true)
    {
        UARTMSP_initBuffers(buffersObject);
    }

    DL_GPIO_initPeripheralOutputFunction(hwAttrs->txPin, hwAttrs->txPinFunction);
    DL_GPIO_initPeripheralInputFunction(hwAttrs->rxPin, hwAttrs->rxPinFunction);

    /* Reset and enable power to UART instance */
    DL_UART_reset(hwAttrs->regs);
    DL_UART_enablePower(hwAttrs->regs);

    DL_UART_ClockConfig clockConfig =
    {
        .clockSel    = hwAttrs->clockSource,
        .divideRatio = hwAttrs->clockDivider,
    };
    DL_UART_setClockConfig(hwAttrs->regs, &clockConfig);

    switch (clockConfig.clockSel)
    {
        case DL_UART_CLOCK_BUSCLK:
            uartClk = 32000000;  /* 32Mhz */
            break;
        case DL_UART_CLOCK_MFCLK:
            uartClk = 4000000;  /* 4Mhz */
            break;
        case DL_UART_CLOCK_LFCLK:
            uartClk = 32000;  /* 32Khz */
            break;
    }

    divratio = clockConfig.divideRatio + 1;
    /* roundup the integer division using the equation x/y = (x+(y-1))/y */
    clkFreq = (uint32_t)((uartClk + (divratio - 1)) / divratio);

    DL_UART_Config config =
    {
        .mode        = hwAttrs->mode,
        .direction   = hwAttrs->direction,
        .flowControl = hwAttrs->flowControl,
        .parity      = (DL_UART_PARITY) params->parityType,
        .wordLength  = (DL_UART_WORD_LENGTH) params->dataLength,
        .stopBits    = (DL_UART_STOP_BITS) params->stopBits,
    };
    DL_UART_init(hwAttrs->regs, &config);
    DL_UART_configBaudRate(hwAttrs->regs, clkFreq, params->baudRate);
    DL_UART_enableFIFOs(hwAttrs->regs);
    DL_UART_setRXFIFOThreshold(hwAttrs->regs, hwAttrs->rxIntFifoThr);
    DL_UART_setTXFIFOThreshold(hwAttrs->regs, hwAttrs->txIntFifoThr);
    DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_RX);
    HwiP_enableInterrupt(hwAttrs->irq);  /* NVIC_EnableIRQ */
    DL_UART_enable(hwAttrs->regs);

    return true;
}

/*
 * ======== UARTMSP_disable ========
 */
static void UARTMSP_disable(UART_Handle handle)
{
    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);
    UART_Object *uartObject                = UART_Obj_Ptr(handle);
    UART_Buffers_Object *buffersObject = UART_buffersObject(uartObject);

    HwiP_disableInterrupt(hwAttrs->irq);
    DL_UART_disableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_RX |
                                                DL_UART_INTERRUPT_TX |
                                                DL_UART_INTERRUPT_EOT_DONE);
    DL_UART_clearInterruptStatus(hwAttrs->regs, 0xFFFF);
    DL_UART_disable(hwAttrs->regs);
    uartObject->readInUse  = false;
    uartObject->writeInUse = false;
    SemaphoreP_delete(buffersObject->rxSem);
    SemaphoreP_delete(buffersObject->txSem);

    return;
}

/*
 * ======== UARTMSP_txChar ========
 */
static void UARTMSP_txChar(UART_Handle handle)
{
    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);

    /* enable transmit interrupts */
    DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_TX);

}

/*
 *  ======== UART2CC26X2_getRxData ========
 *  Must be called with HWI disabled.
 */
__STATIC_INLINE size_t UARTMSP_getRxData(UART_Handle handle, size_t size)
{
    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);
    UART_Object *uartObject                = UART_Obj_Ptr(handle);
    UART_Buffers_Object *buffersObject = UART_buffersObject(uartObject);

    size_t consumed = 0;
    uint8_t data;

    while (!(DL_UART_isRXFIFOEmpty(hwAttrs->regs)) && size)
    {
        data = DL_UART_receiveData(hwAttrs->regs);
        RingBuf_put(&buffersObject->rxBuf, data);
        ++consumed;
        --size;
    }

    return (consumed);
}

/*
* =============UARTMSP_TxCallback============
* This function should be called from ISR
*/
void UARTMSP_TxCallback(UART_Handle handle)
{
    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);
    UART_Object *uartObject            = UART_Obj_Ptr(handle);
    UART_Buffers_Object *buffersObject = UART_buffersObject(uartObject);
    UART_Callback_Object *callbackObj  = UART_callbackObject(uartObject);

    do
    {
        uint8_t txData;
        txData = *(callbackObj->writeBuf + callbackObj->bytesWritten);
        if (callbackObj->writeSize == callbackObj->bytesWritten)
        {
            /* no more data left to transmit, disable TX interrupt, enable
                * end of transmission interrupt for low power support.
                */
            DL_UART_disableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_TX);
            DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_EOT_DONE);

            break;
        }
        /* pulled a byte of data out, prime the transmitter */
        DL_UART_transmitData(hwAttrs->regs, txData);
        callbackObj->bytesWritten++;

    } while (DL_UART_isTXFIFOFull(hwAttrs->regs) == false);

}

/*
 * ======== UARTMSP_interruptHandler ========
 */
void UARTMSP_interruptHandler(UART_Handle handle)
{
    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);
    UART_Object *uartObject                = UART_Obj_Ptr(handle);
    UART_Buffers_Object *buffersObject = UART_buffersObject(uartObject);
    UART_Callback_Object *callbackObj = UART_callbackObject(uartObject);
    UART_EventCallback_Object *eventCallbackObj = UART_eventCallbackObject(uartObject);

    int space;
    static unsigned char *dstAddr;
    uint16_t count = 0;
    uint32_t event;

    /* Get all enabled interrupt status */
    uint32_t status = DL_UART_getEnabledInterruptStatus(hwAttrs->regs, 0x1FFFF);
    bool rxPost = false;
    bool txPost = false;

    if ((status & (UART_RX_ERRS)) && uartObject->eventsSupported)
    {
        uartObject->overrunActive = false;
        uartObject->overrunCount  = 0;
        if (status & DL_UART_INTERRUPT_OVERRUN_ERROR)
        {
            /*
             *  Overrun error occurred, get what we can.
             */
            UARTMSP_getRxData(handle, RingBuf_space(&buffersObject->rxBuf));
            /* Throw away the rest in order to clear the overrun */
            while (!DL_UART_isRXFIFOEmpty(hwAttrs->regs))
            {
                volatile uint8_t data = DL_UART_receiveData(hwAttrs->regs);
                /* discard the data */
                (void) data;
            }
            ++uartObject->overrunCount;
            if (uartObject->overrunActive == false)
            {
                uartObject->overrunActive = true;
            }
        }

        event = status & (UART_RX_ERRS);

        if (event)
        {
            eventCallbackObj->eventCallback(handle, event, uartObject->overrunCount, callbackObj->userArg);
        }
    }

    if (status & DL_UART_INTERRUPT_RX)
    {

        /* receive characters as long as we can */
        while (DL_UART_isRXFIFOEmpty(hwAttrs->regs) == false)
        {
            /* data in the RX FIFO, pull it out of the hardware */
            volatile uint8_t rxData = DL_UART_receiveData(hwAttrs->regs);
            /* try to push the data into the software RX buffer */
            if (RingBuf_put(&buffersObject->rxBuf, rxData) == -1)
            {
                /* no space left in RX buffer, overrun */
            }

            if (callbackObj->readCallback != NULL)
            {
                /* get the data from ring buffer to the application provided buffer */
                count += RingBuf_getn(&buffersObject->rxBuf, callbackObj->readBuf + count, callbackObj->readSize - count);
                if(callbackObj->readSize == count)
                {
                    count = 0; /* reset the counter */
                    uartObject->readInUse = false;
                    callbackObj->readCallback(handle, (void*)callbackObj->readBuf, callbackObj->readSize,callbackObj->userArg, UART_STATUS_SUCCESS);
                }
            }

            rxPost = true; /* release the semaphore */
        }
    }
    /* transmit a character if we have pending TX data */
    if (status & DL_UART_INTERRUPT_TX)
    {
        if (callbackObj->writeCallback != NULL)
        {
            UARTMSP_TxCallback(handle);
        }
        else
        {
            /* because we had an active interrupt flag, we should always have
            * room for at least one byte of transmit data
            */
            do
            {
                uint8_t txData;
                if (RingBuf_get(&buffersObject->txBuf, &txData) == -1)
                {
                    /* no more data left to transmit, disable TX interrupt, enable
                    * end of transmission interrupt for low power support.
                    */
                    DL_UART_disableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_TX);
                    DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_EOT_DONE);
                    break;
                }
                /* pulled a byte of data out, prime the transmitter  */
                DL_UART_transmitData(hwAttrs->regs, txData);

            } while (DL_UART_isTXFIFOFull(hwAttrs->regs) == false);

        }
    }

    if (status & DL_UART_INTERRUPT_EOT_DONE)
    {
        /* transmission complete, disable interrupt and remove low power
         * inhibit
         */
        DL_UART_disableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_EOT_DONE);
        txPost                 = true;
    }

    if (rxPost)
    {
        SemaphoreP_post(buffersObject->rxSem);
    }
    if (txPost)
    {
        SemaphoreP_post(buffersObject->txSem);
    }
}


/*
 *  ======== UARTMSPSupport_enableInts ========
 *  Function to enable receive, receive timeout, and error interrupts
 */
void UARTMSP_enableInts(UART_Handle handle)
{
    UARTMSP_HWAttrs *hwAttrs           = UART_HWAttrs_Ptr(handle);
    UART_Object *uartObject                = UART_Obj_Ptr(handle);
    UART_EventCallback_Object *eventCallbackObj = UART_eventCallbackObject(uartObject);

    if (eventCallbackObj->eventCallback)
    {
        if (eventCallbackObj->eventMask & DL_UART_INTERRUPT_OVERRUN_ERROR)
        {
            DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_OVERRUN_ERROR);
        }
        if (eventCallbackObj->eventMask & DL_UART_INTERRUPT_BREAK_ERROR)
        {
            DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_BREAK_ERROR);
        }
        if (eventCallbackObj->eventMask & DL_UART_INTERRUPT_PARITY_ERROR)
        {
            DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_PARITY_ERROR);
        }
        if (eventCallbackObj->eventMask & DL_UART_INTERRUPT_FRAMING_ERROR)
        {
            DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_FRAMING_ERROR);
        }
        if (eventCallbackObj->eventMask & DL_UART_INTERRUPT_RX_TIMEOUT_ERROR)
        {
            DL_UART_enableInterrupt(hwAttrs->regs, DL_UART_INTERRUPT_RX_TIMEOUT_ERROR);
        }
    }
}
/* clang-format on */
