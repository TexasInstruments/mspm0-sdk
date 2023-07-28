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

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

#include "ti_drivers_config.h"

uint8_t gBuffer[CONFIG_UART_BUFFER_LENGTH] = {0};

UART_Handle uart;
UART_Params uartParams;

/*
 *  ======== mainThread ========
 */
void mainThread(void *arg0)
{
    size_t rdCount = 0;
    size_t wrCount = 0;

    UART_Params_init(&uartParams);
    uartParams.baudRate = CONFIG_UART_BAUD_RATE;
    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL) {
        // UART_open failed
        __BKPT();
    }

    /* Turn on user LED to indicate successful initialization */
    DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);

    /* Loop forever echoing */
    while (1) {
        UART_read(uart, gBuffer, CONFIG_UART_BUFFER_LENGTH, &rdCount);
        UART_write(uart, gBuffer, CONFIG_UART_BUFFER_LENGTH, &wrCount);
    }
}
