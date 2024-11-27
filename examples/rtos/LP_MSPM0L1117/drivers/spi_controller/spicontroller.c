/*
 * Copyright (c) 2018-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *  ======== spicontroller.c ========
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define THREADSTACKSIZE (1024)

#define SPI_MSG_LENGTH 32
#define CONTROLLER_MSG ("Hello from controller, msg#: ")

#define MAX_LOOP (10)

static Display_Handle display;

unsigned char controllerRxBuffer[SPI_MSG_LENGTH];
unsigned char controllerTxBuffer[SPI_MSG_LENGTH];

/* Semaphore to block controller until peripheral is ready for transfer */
sem_t controllerSem;

/*
 *  ======== peripheralReadyFxn ========
 *  Callback function for the GPIO interrupt on CONFIG_SPI_PERIPHERAL_READY.
 */
void peripheralReadyFxn(uint_least8_t index)
{
    sem_post(&controllerSem);
}

/*
 *  ======== controllerThread ========
 *  Controller SPI sends a message to peripheral while simultaneously receiving a
 *  message from the peripheral.
 */
void *controllerThread(void *arg0)
{
    SPI_Handle controllerSpi;
    SPI_Params spiParams;
    SPI_Transaction transaction;
    uint8_t i;
    bool transferOK;
    int32_t status;

    /*
     * CONFIG_SPI_CONTROLLER_READY & CONFIG_SPI_PERIPHERAL_READY are GPIO pins connected
     * between the controller & peripheral.  These pins are used to synchronize
     * the controller & peripheral applications via a small 'handshake'.  The pins
     * are later used to synchronize transfers & ensure the controller will not
     * start a transfer until the peripheral is ready.  These pins behave
     * differently between spicontroller & spiperipheral examples:
     *
     * spicontroller example:
     *     * CONFIG_SPI_CONTROLLER_READY is configured as an output pin.  During the
     *       'handshake' this pin is changed from low to high output.  This
     *       notifies the peripheral the controller is ready to run the application.
     *       Afterwards, the pin is used by the controller to notify the peripheral it
     *       has opened CONFIG_SPI_CONTROLLER.  When CONFIG_SPI_CONTROLLER is opened, this
     *       pin will be pulled low.
     *
     *     * CONFIG_SPI_PERIPHERAL_READY is configured as an input pin. During the
     *       'handshake' this pin is read & a high value will indicate the peripheral
     *       ready to run the application.  Afterwards, a falling edge interrupt
     *       will be configured on this pin.  When the peripheral is ready to perform
     *       a transfer, it will pull this pin low.
     *
     * Below we set CONFIG_SPI_CONTROLLER_READY & CONFIG_SPI_PERIPHERAL_READY initial
     * conditions for the 'handshake'.
     */
    GPIO_setConfig(
        CONFIG_SPI_CONTROLLER_READY, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW |
                                         CONFIG_SPI_CONTROLLER_READY_IOMUX);
    GPIO_setConfig(CONFIG_SPI_PERIPHERAL_READY,
        GPIO_CFG_INPUT | GPIO_CFG_HIZ_ON | CONFIG_SPI_PERIPHERAL_READY_IOMUX |
            GPIO_CFG_IN_INT_FALLING);

    Display_printf(display, 0, 0, "Starting the SPI controller example");

    /*
     * Handshake - Set CONFIG_SPI_CONTROLLER_READY high to indicate controller is ready
     * to run.  Wait CONFIG_SPI_PERIPHERAL_READY to be high.
     */
    GPIO_write(CONFIG_SPI_CONTROLLER_READY, 1);
    while (GPIO_read(CONFIG_SPI_PERIPHERAL_READY) == 0) {
    } /* it stays here until peripheral ready becomes 1/high */

    /* Handshake complete; now configure interrupt on CONFIG_SPI_PERIPHERAL_READY */
    GPIO_setConfig(
        CONFIG_SPI_PERIPHERAL_READY, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(CONFIG_SPI_PERIPHERAL_READY, peripheralReadyFxn);
    GPIO_enableInt(CONFIG_SPI_PERIPHERAL_READY);

    /*
     * Create synchronization semaphore; the controller will wait on this semaphore
     * until the peripheral is ready.
     */
    status = sem_init(&controllerSem, 0, 0);
    if (status != 0) {
        Display_printf(display, 0, 0, "Error creating controllerSem\n");

        while (1) {
        }
    }

    /* Open SPI as controller (default) */
    SPI_Params_init(&spiParams);
    spiParams.frameFormat = SPI_MOTO4_POL0_PHA0;
    /* See device-specific technical reference manual for supported speeds */
    spiParams.bitRate = 500000;
    controllerSpi     = SPI_open(CONFIG_SPI_CONTROLLER, &spiParams);
    if (controllerSpi == NULL) {
        Display_printf(display, 0, 0, "Error initializing controller SPI\n");
        while (1) {
        }
    } else {
        Display_printf(display, 0, 0, "Controller SPI initialized\n");
    }

    /*
     * Controller has opened CONFIG_SPI_CONTROLLER; set CONFIG_SPI_CONTROLLER_READY LOW to
     * inform the peripheral.
     */
    GPIO_write(CONFIG_SPI_CONTROLLER_READY, 0);

    /* Copy message to transmit buffer */
    strncpy((char *) controllerTxBuffer, CONTROLLER_MSG, SPI_MSG_LENGTH);

    for (i = 0; i < MAX_LOOP; i++) {
        /*
         * Wait until peripheral is ready for transfer; peripheral will pull
         * CONFIG_SPI_PERIPHERAL_READY low.
         */
        sem_wait(&controllerSem);

        /* Initialize controller SPI transaction structure */
        controllerTxBuffer[sizeof(CONTROLLER_MSG) - 1] = (i % 10) + '0';
        memset((void *) controllerRxBuffer, 0, SPI_MSG_LENGTH);
        transaction.count = SPI_MSG_LENGTH;
        transaction.txBuf = (void *) controllerTxBuffer;
        transaction.rxBuf = (void *) controllerRxBuffer;

        /* Toggle user LED, indicating a SPI transfer is in progress */
        GPIO_toggle(CONFIG_GPIO_LED_0);

        /* Perform SPI transfer */
        transferOK = SPI_transfer(controllerSpi, &transaction);
        if (transferOK) {
            Display_printf(
                display, 0, 0, "Controller received: %s", controllerRxBuffer);
        } else {
            Display_printf(
                display, 0, 0, "Unsuccessful controller SPI transfer");
        }

        /* Sleep for a bit before starting the next SPI transfer  */
        sleep(2);
    }

    SPI_close(controllerSpi);

    /* Example complete - set pins to a known state */
    GPIO_disableInt(CONFIG_SPI_PERIPHERAL_READY);
    GPIO_setConfig(
        CONFIG_SPI_PERIPHERAL_READY, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);
    GPIO_write(CONFIG_SPI_CONTROLLER_READY, 0);

    Display_printf(display, 0, 0, "\nDone");

    return (NULL);
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    pthread_t thread0;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    /* Call driver init functions. */
    Display_init();
    GPIO_init();
    SPI_init();

    /* Configure the LED pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH | CONFIG_GPIO_LED_0_IOMUX);

    /* Open the display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        /* Failed to open display driver */
        while (1) {
        }
    }

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Create application threads */
    pthread_attr_init(&attrs);

    detachState = PTHREAD_CREATE_DETACHED;
    /* Set priority and stack size attributes */
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1) {
        }
    }

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1) {
        }
    }

    /* Create controller thread */
    priParam.sched_priority = 1;
    pthread_attr_setschedparam(&attrs, &priParam);

    retc = pthread_create(&thread0, &attrs, controllerThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {
        }
    }

    return (NULL);
}
