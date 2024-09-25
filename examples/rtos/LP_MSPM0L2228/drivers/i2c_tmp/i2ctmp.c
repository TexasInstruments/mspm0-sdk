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
 *  ======== i2ctmp.c ========
 */
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/display/DisplayUart.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* Temperature result registers */
#define TMP11X_RESULT_REG 0x0000

/* I2C target addresses */
#define TMP11X_BASSENSORS_ADDR 0x48
#define TMP116_LAUNCHPAD_ADDR 0x49

/* Number of supported sensor iterations */
#define TMP_COUNT 2
uint8_t gBuffer[CONFIG_UART_BUFFER_LENGTH] = {0};

/*
 * Data structure containing currently supported I2C TMP sensors.
 * Sensors are ordered by descending preference.
 */
static const struct {
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[TMP_COUNT] = {{TMP11X_BASSENSORS_ADDR, TMP11X_RESULT_REG, "11X"},
    {TMP116_LAUNCHPAD_ADDR, TMP11X_RESULT_REG, "116"}};

static uint8_t targetAddress;
static Display_Handle display;

static void i2cErrorHandler(
    I2C_Transaction *transaction, Display_Handle display);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    uint16_t sample;
    int16_t temperature;
    uint8_t txBuffer[1];
    uint8_t rxBuffer[2];
    int8_t i;
    I2C_Handle i2c;
    I2C_Params i2cParams;
    I2C_Transaction i2cTransaction;

    /* Call driver init functions */
    Display_init();
    GPIO_init();
    I2C_init();

    /* Configure the LED and if applicable, the TMP_EN pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH | CONFIG_GPIO_LED_0_IOMUX);
#ifdef CONFIG_GPIO_TMP_EN
    GPIO_setConfig(CONFIG_GPIO_TMP_EN,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH | CONFIG_GPIO_TMP_EN_IOMUX);
    /* Allow the sensor to power on */
    sleep(1);
#endif

    /* Open the UART display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1) {
        }
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_LED_ON);
    Display_printf(display, 0, 0, "Starting the i2ctmp example\n");

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2c               = I2C_open(CONFIG_I2C_TMP, &i2cParams);
    if (i2c == NULL) {
        Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1) {
        }
    } else {
        Display_printf(display, 0, 0, "I2C Initialized!\n");
    }

    /* Common I2C transaction setup */
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 0;

    /*
     * Determine which I2C sensors are present by querying known I2C
     * target addresses.
     */
    for (i = TMP_COUNT - 1; i >= 0; i--) {
        i2cTransaction.targetAddress = sensors[i].address;
        txBuffer[0]                  = sensors[i].resultReg;

        if (I2C_transfer(i2c, &i2cTransaction)) {
            targetAddress = sensors[i].address;
            Display_printf(display, 0, 0,
                "Detected TMP%s sensor with target"
                " address 0x%x",
                sensors[i].id, sensors[i].address);
        } else {
            i2cErrorHandler(&i2cTransaction, display);
        }
    }

    /* If we never assigned a target address */
    if (targetAddress == 0) {
        Display_printf(display, 0, 0, "Failed to detect a sensor!");
        I2C_close(i2c);
        while (1) {
        }
    }

    Display_printf(display, 0, 0, "\nUsing last known sensor for samples.");
    i2cTransaction.targetAddress = targetAddress;

    /* Take 20 samples and print them out onto the console */
    for (sample = 0; sample < 20; sample++) {
        i2cTransaction.readCount = 2;
        if (I2C_transfer(i2c, &i2cTransaction)) {
            /*
             * Extract degrees C from the received data;
             * see TMP sensor datasheet
             */
            temperature = (rxBuffer[0] << 8) | (rxBuffer[1]);
            temperature *= 0.0078125;

            Display_printf(
                display, 0, 0, "Sample %u: %d C", sample, temperature);
        } else {
            i2cErrorHandler(&i2cTransaction, display);
        }

        /* Sleep for 1 second */
        sleep(1);
    }
    Display_printf(display, 0, 0, "I2C closed!");
    I2C_close(i2c);
    return (NULL);
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
