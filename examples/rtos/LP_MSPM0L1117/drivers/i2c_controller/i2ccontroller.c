/*
 * Copyright (c) 2018-2024, Texas Instruments Incorporated
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
 *  ======== i2ccontroller.c ========
 */
#include <stddef.h>
#include <stdint.h>
/* For sleep() */
#include <unistd.h>
/* For memcpy() */
#include <string.h>

/* Driver Header files */
#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* Example command interface */
#include "cmdinterface.h"

#define BUFFER_SIZE 32
uint8_t txBuffer[BUFFER_SIZE];
uint8_t rxBuffer[BUFFER_SIZE];

volatile uint8_t cmd = 0;
static Display_Handle display;
static const char dataString[] = "Hi, I am the controller";

static void i2cErrorHandler(
    I2C_Transaction *transaction, Display_Handle display);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    I2C_Handle i2cHandle;
    I2C_Params i2cParams;
    I2C_Transaction i2cTransaction;
    bool status;

    /* Call driver init functions */
    Display_init();
    GPIO_init();
    I2C_init();

    /* Configure the LED and if applicable, the TMP_EN pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH | CONFIG_GPIO_LED_0_IOMUX);

    /* Open the UART display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1) {
        }
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_LED_ON);
    Display_printf(display, 0, 0, "Starting the i2ccontroller example\n");

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2cHandle         = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2cHandle == NULL) {
        Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1) {
        }
    } else {
        Display_printf(display, 0, 0, "I2C Initialized!\n");
    }

    /* Common I2C transaction setup */
    i2cTransaction.targetAddress = TGT_ADDRESS;
    i2cTransaction.readBuf       = rxBuffer;
    i2cTransaction.writeBuf      = txBuffer;
    /**********************************Get Status*******************************************************/
    /* Send GET_STATUS until the transaction is acknowledged by the
     * I2C target (another device running the "i2ctarget" example).
     */
    i2cTransaction.writeCount = 1; /* cmd */
    txBuffer[0]               = CMD_CTL_GET_STATUS;
    i2cTransaction.readCount  = 1; /* response */
    Display_printf(display, 0, 0, "Sending GET_STATUS until target responds");
    do {
        status = I2C_transfer(i2cHandle, &i2cTransaction);
        if (!status) {
            /* Transaction not successful. Sleep a while and try again. */
            sleep(1);
        } else {
            /* Do nothing, we accept any data from the target, so we do not
             * check the returned status in i2cTransaction.readBuf[0]. */
        }
    } while (!status);

    Display_printf(display, 0, 0, "Target responded!");
    /****************************************************************************************************/
    uint16_t dataSize   = sizeof(dataString);
    uint8_t statusValue = 0;
    while (1) {
        /* Send SET_STATUS command */
        txBuffer[0]               = CMD_CTL_SET_STATUS;
        txBuffer[1]               = statusValue;
        i2cTransaction.writeCount = 2;
        i2cTransaction.readCount  = 0;
        Display_printf(display, 0, 0, "Sending SET_STATUS = %d", statusValue);
        status = I2C_transfer(i2cHandle, &i2cTransaction);
        if (!status) {
            i2cErrorHandler(&i2cTransaction, display);
        }

        sleep(1);
        GPIO_toggle(CONFIG_GPIO_LED_0);

        /* Send GET_STATUS command */
        txBuffer[0]               = CMD_CTL_GET_STATUS;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readCount  = 1;
        Display_printf(display, 0, 0, "Sending GET_STATUS");
        status = I2C_transfer(i2cHandle, &i2cTransaction);
        if (!status) {
            i2cErrorHandler(&i2cTransaction, display);
        } else {
            Display_printf(
                display, 0, 0, "GET_STATUS returned %d", rxBuffer[0]);
        }

        sleep(1);
        GPIO_toggle(CONFIG_GPIO_LED_0);

        /* Send WRITE_BLOCK command */
        /* [0]: CMD
         * [1]: Start offset in target's buffer ("address")
         * [2]: byteCount (MSB)
         * [3]: byteCount (LSB)
         * [4 through (<byteCount>+3)] Data
         */
        txBuffer[0] = CMD_CTL_WRITE_BLOCK;
        txBuffer[1] = 0;
        txBuffer[2] = (dataSize & 0xFF00) >> 8;
        txBuffer[3] = (dataSize & 0x00FF);
        memcpy(&txBuffer[4], dataString, dataSize);
        i2cTransaction.writeCount = 4 + dataSize;
        i2cTransaction.readCount  = 0;

        Display_printf(
            display, 0, 0, "Sending WRITE_BLOCK '%s'", &txBuffer[4]);
        status = I2C_transfer(i2cHandle, &i2cTransaction);
        if (!status) {
            i2cErrorHandler(&i2cTransaction, display);
        }

        sleep(1);
        GPIO_toggle(CONFIG_GPIO_LED_0);

        /* Send READ_BLOCK command, reading back  */
        /* [0]: CMD
         * [1]: Start offset in target's buffer ("address")
         * [2]: byteCount (MSB)
         * [3]: byteCount (LSB)
         * [4 through (<byteCount>+3)] Data
         */
        txBuffer[0] = CMD_CTL_READ_BLOCK;
        txBuffer[1] = 0;
        txBuffer[2] = (dataSize & 0xFF00) >> 8;
        txBuffer[3] = (dataSize & 0x00FF);

        i2cTransaction.writeCount = 4;
        i2cTransaction.readCount  = dataSize;
        Display_printf(display, 0, 0, "Sending READ_BLOCK");
        status = I2C_transfer(i2cHandle, &i2cTransaction);
        if (!status) {
            i2cErrorHandler(&i2cTransaction, display);
        } else {
            Display_printf(
                display, 0, 0, "READ_BLOCK returned '%s'\n", rxBuffer);
        }

        statusValue++;

        sleep(1);
        GPIO_toggle(CONFIG_GPIO_LED_0);
    }
}

/*
 *  ======== i2cErrorHandler ========
 */
static void i2cErrorHandler(
    I2C_Transaction *transaction, Display_Handle display)
{
    switch (transaction->status) {
        case I2C_STATUS_TIMEOUT:
            Display_printf(display, 0, 0, "I2C transaction timed out!");
            break;
        case I2C_STATUS_CLOCK_TIMEOUT:
            Display_printf(display, 0, 0, "I2C serial clock line timed out!");
            break;
        case I2C_STATUS_ADDR_NACK:
            Display_printf(display, 0, 0,
                "I2C target address 0x%x not"
                " acknowledged!",
                transaction->targetAddress);
            break;
        case I2C_STATUS_DATA_NACK:
            Display_printf(display, 0, 0, "I2C data byte not acknowledged!");
            break;
        case I2C_STATUS_ARB_LOST:
            Display_printf(
                display, 0, 0, "I2C arbitration to another controller!");
            break;
        case I2C_STATUS_INCOMPLETE:
            Display_printf(
                display, 0, 0, "I2C transaction returned before completion!");
            break;
        case I2C_STATUS_BUS_BUSY:
            Display_printf(display, 0, 0, "I2C bus is already in use!");
            break;
        case I2C_STATUS_CANCEL:
            Display_printf(display, 0, 0, "I2C transaction cancelled!");
            break;
        case I2C_STATUS_INVALID_TRANS:
            Display_printf(display, 0, 0, "I2C transaction invalid!");
            break;
        case I2C_STATUS_ERROR:
            Display_printf(display, 0, 0, "I2C generic error!");
            break;
        default:
            Display_printf(display, 0, 0, "I2C undefined error case!");
            break;
    }
}
