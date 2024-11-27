/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * NOTE 1:  This project provides a simple POSIX demo where a POSIX thread is
 * used to control an LED.
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * basic demo. Generic functions, such FreeRTOS hook functions, and functions
 * required to configure the hardware, are defined in main.c.
 ******************************************************************************
 *
 * The thread is created in main(). This thread sleeps for 1sec,
 * causing the LED to toggle every 1sec.
 */

/* Standard includes */
#include <stdio.h>

/* POSIX header files */
#include <unistd.h>

/* TI includes for driver configuration */
#include "ti_msp_dl_config.h"

/* 1 second delay */
uint32_t task_delay = 1;

void *Thread(void *arg0)
{
    for (;;) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        sleep(task_delay);
    }
}
