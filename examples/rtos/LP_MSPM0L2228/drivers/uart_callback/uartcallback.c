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

/*
 *  ======== UARTecho.c ========
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <semaphore.h>

#include "ti_drivers_config.h"

static sem_t appSema;
static volatile size_t numBytesRead;

uint8_t gBuffer[CONFIG_UART_BUFFER_LENGTH] = {0};
static volatile UART_Handle uart;
UART_Params uartParams;
void callbackFxn(UART_Handle handle, void *buffer, size_t count, void *userArg,
    int_fast16_t status);
void callbackFxnTx(UART_Handle handle, void *buffer, size_t count,
    void *userArg, int_fast16_t status);
/*
 *  ======== callbackFxn ========
 */
void callbackFxn(UART_Handle handle, void *buffer, size_t count, void *userArg,
    int_fast16_t status)
{
    if (status != UART_STATUS_SUCCESS) {
        /* RX error occur in UART_read() */
        while (1) {
        }
    }

    numBytesRead = count;
    sem_post(&appSema);
}

/*
 *  ======== callbackFxnTx ========
 */
void callbackFxnTx(UART_Handle handle, void *buffer, size_t count,
    void *userArg, int_fast16_t status)
{
    if (status != UART_STATUS_SUCCESS) {
        /* RX error occured in UART_read() */
        while (1) {
        }
    }
    /* if TX DMA is enabled and semaphore is not release by rx callback then enable below function */
    /* sem_post(&appSema); */
}
void uartInit(void)
{
    /* Initialise it with default parameters */
    UART_Params_init(&uartParams);
    uartParams.readMode     = UART_Mode_CALLBACK;
    uartParams.baudRate     = CONFIG_UART_BAUD_RATE;
    uartParams.readCallback = callbackFxn;

    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL) {
        /* UART_open failed */
        __BKPT();
    }
}

/*
 *  ======== mainThread ========
 */
void mainThread(void *arg0)
{
    size_t rdCount          = 0;
    size_t wrCount          = 0;
    const char echoPrompt[] = "Echoing characters:\r\n";
    int32_t semStatus;
    uint32_t status   = UART_STATUS_SUCCESS;
    static char input = 0;

    /* Create semaphore */
    semStatus = sem_init(&appSema, 0, 0);

    if (semStatus != 0) {
        /* Error creating semaphore */
        while (1) {
        }
    }

    uartInit();

    /* Turn on user LED to indicate successful initialization */
    DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);

    UART_write(uart, &echoPrompt, sizeof(echoPrompt), NULL);

    /* Loop forever echoing */
    while (1) {
        numBytesRead = 0;

        /* Pass NULL for bytesRead since it's not used in this example */
        status = UART_read(uart, &input, 1, NULL);
        if (uartParams.readMode == UART_Mode_BLOCKING) {
            numBytesRead = sizeof(input);
        }
        if (status != UART_STATUS_SUCCESS) {
            /* UART_read() failed */
            while (1) {
            }
        }

        /* Do not write until read callback executes */
        /* if using buffer mode for both TX and RX then comment out below line */
        sem_wait(&appSema);

        if (numBytesRead > 0) {
            status = UART_write(uart, &input, 1, NULL);

            if (status != UART_STATUS_SUCCESS) {
                /* UART_write() failed */
                while (1) {
                }
            }
        }
    }
}
