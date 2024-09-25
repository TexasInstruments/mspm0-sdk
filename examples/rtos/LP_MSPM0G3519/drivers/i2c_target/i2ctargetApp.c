/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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
 *  ======== i2cTarget.c ========
 */
#include <stddef.h>
#include <stdint.h>
/* For sleep() */
#include <unistd.h>
/* For memcpy() */
#include <string.h>

/* Driver Header files */
#include <ti/display/DisplayUart.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2CTarget.h>
#include "cmdinterface.h"

/* Driver configuration */
#include "ti_drivers_config.h"

#define TASKSTACKSIZE 640

typedef enum {
    STATE_WAIT_FOR_CMD = 1, /*!< I2C Target is waiting for command byte */
    STATE_PROCESS_ARGS = 2, /*!< I2C Target is processing command arguments */
    STATE_PROCESS_PAYLOAD = 3, /*!< I2C Target is processing command payload */
    STATE_CMD_DONE = 4 /*!< I2C Target has completed processing command */
} ProtocolState;

#define BUFFER_SIZE 32
typedef struct {
    uint8_t id;        /*!< Command ID */
    uint8_t argCount;  /*!< Number of arguments in the command */
    uint8_t dataCount; /*!< Number of data bytes */
    uint8_t args[4];   /*!< Command arguments (4 bytes supported) */
    uint8_t argIdx;    /*!< Internal. Current argument index */
    uint8_t *dataPtr;  /*!< Pointer to data */
    uint16_t dataIdx;  /*!< Internal. Current data index. */
} Command;

/* Constants */
static const char dataString[] = "Hello, I am the target!";

/* Local variables */
uint8_t rxBuffer[BUFFER_SIZE];
uint8_t txBuffer[BUFFER_SIZE];
ProtocolState protocolState;
Command cmd;

/* Function prototypes */
static int_fast16_t i2cTargetCallback(
    I2CTarget_Handle handle, I2CTarget_Event event, uint8_t *val);
static int cmdHandler(I2CTarget_Handle handle, uint8_t *data);
static int cmdCtlSetStatus(I2CTarget_Handle handle, uint8_t *data);
static int cmdCtlGetStatus(I2CTarget_Handle handle, uint8_t *data);
static int cmdCtlWriteBlock(I2CTarget_Handle handle, uint8_t *data);
static int cmdCtlReadBlock(I2CTarget_Handle handle, uint8_t *data);
/************************************************/
static uint8_t targetAddress;
static Display_Handle display;

static void i2cErrorHandler(
    I2C_Transaction *transaction, Display_Handle display);
/*****************************************/
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
    I2CTarget_Handle i2cHandle;
    I2CTarget_Params i2cParams;

    /* Call driver init functions */
    Display_init();
    GPIO_init();
    I2C_init();

    /* Configure the LED0 and LED1*/
    GPIO_setConfig(CONFIG_GPIO_LED_0,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH | CONFIG_GPIO_LED_0_IOMUX);
    GPIO_setConfig(CONFIG_GPIO_LED_1,
        GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW | CONFIG_GPIO_LED_1_IOMUX);

    /* Open the UART display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1) {
            ;
        }
    }

    Display_printf(display, 0, 0, "Starting the I2CTarget example\n");

    /* Open I2CTarget driver */
    I2CTarget_Params_init(&i2cParams);
    i2cParams.eventCallbackFxn = i2cTargetCallback;
    i2cParams.targetAddress    = TGT_ADDRESS;
    i2cHandle = I2CTarget_open(CONFIG_I2C_TARGET_0, &i2cParams);
    if (i2cHandle == NULL) {
        Display_printf(display, 0, 0, "Error Initializing I2CTarget\n");
        while (1) {
            ;
        }
    } else {
        Display_printf(display, 0, 0, "I2C Target Initialized!\n");
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_0_ON);

    /* Initialize state machine */
    protocolState = STATE_WAIT_FOR_CMD;

    /* Start driver, it will now act on I2C traffic */
    I2CTarget_start(i2cHandle);

    while (1) {
        sleep(1);
    }
}

/*
 *  ======== i2cTargetCallback ========
 */
static int_fast16_t i2cTargetCallback(
    I2CTarget_Handle handle, I2CTarget_Event event, uint8_t *val)
{
    int retCode = I2CTarget_STATUS_SUCCESS;
    if (event == I2CTarget_Event_WRITE_REQUESTED) {
        /* Do nothing. */
        return retCode;
    }
    if (event == I2CTarget_Event_WRITE_RECEIVED) {
        /* Controller has sent a byte to us, send to command handler for processing */
        retCode = cmdHandler(handle, val);
    }

    if ((event == I2CTarget_Event_READ_REQUESTED) ||
        (event == I2CTarget_Event_READ_PROCESSED)) {
        if (protocolState == STATE_WAIT_FOR_CMD) {
            /* Protocol expects 1 write from controller (CMD), but we got read.
             * Send dummy data.
             */
            *val = 0xff;
            /* We should always return success. */
            retCode = I2CTarget_STATUS_SUCCESS;
        } else {
            retCode = cmdHandler(handle, val);
        }
    }

    if (protocolState == STATE_CMD_DONE) {
        /* Command complete, reset state machine. */
        protocolState = STATE_WAIT_FOR_CMD;
    }

    if (event == I2CTarget_Event_STOP) {
        /* Stop condition, reset state machine. */
        protocolState = STATE_WAIT_FOR_CMD;
    }

    return retCode;
}

/*
 *  ======== cmdHandler ========
 */
static int cmdHandler(I2CTarget_Handle handle, uint8_t *data)
{
    int retCode = 0; /* Assume success */
    if (protocolState == STATE_WAIT_FOR_CMD) {
        /* The data argument contains command byte */
        cmd.id = *data;

        /* Reset variables */
        cmd.argCount  = 0;
        cmd.argIdx    = 0;
        cmd.dataCount = 0;
        cmd.dataIdx   = 0;

        /* Toggle LED to indicate activity */
        GPIO_toggle(CONFIG_GPIO_LED_1);
    }

    if (cmd.id == CMD_CTL_GET_STATUS) {
        /* Controller wants to read a byte from us */
        retCode = cmdCtlGetStatus(handle, data);
    } else if (cmd.id == CMD_CTL_READ_BLOCK) {
        /* Controller wants to read a block of data from us */
        retCode = cmdCtlReadBlock(handle, data);
    } else if (cmd.id == CMD_CTL_SET_STATUS) {
        /* Controller wants to write a byte to us */
        retCode = cmdCtlSetStatus(handle, data);
    } else if (cmd.id == CMD_CTL_WRITE_BLOCK) {
        /* Controller wants to write a block of data to us */
        retCode = cmdCtlWriteBlock(handle, data);
    } else {
        /* Unsupported command */
        return -1;
    }

    return retCode;
}

/*
 *  ======== cmdCtlSetStatus ========
 */
static int cmdCtlSetStatus(I2CTarget_Handle handle, uint8_t *data)
{
    switch (protocolState) {
        case STATE_WAIT_FOR_CMD:
            /* Update state. We expect to receive data the next time around. */
            protocolState = STATE_PROCESS_PAYLOAD;
            break;
        case STATE_PROCESS_ARGS:
            /* No argument processing */
            break;
        case STATE_PROCESS_PAYLOAD:
            /* Data contains the byte to receive */
            rxBuffer[0]   = *data;
            protocolState = STATE_CMD_DONE;
            break;
        case STATE_CMD_DONE:
            /* We do not expect to get here */
            break;
        default:
            break;
    }
    return 0;
}

/*
 *  ======== cmdCtlGetStatus ========
 */
static int cmdCtlGetStatus(I2CTarget_Handle handle, uint8_t *data)
{
    switch (protocolState) {
        case STATE_WAIT_FOR_CMD:
            /* Update state. We'll send data the next time around. */
            protocolState = STATE_PROCESS_PAYLOAD;
            break;
        case STATE_PROCESS_ARGS:
            /* No command processing */
            break;
        case STATE_PROCESS_PAYLOAD:
            /* Give controller the byte value it wants to read. */
            *data         = rxBuffer[0]; /* Return the last received data */
            protocolState = STATE_CMD_DONE;
            break;
        case STATE_CMD_DONE:
            /* We do not expect to get here */
            break;
        default:
            break;
    }

    return 0;
}

/*
 *  ======== cmdCtlWriteBlock ========
 */
static int cmdCtlWriteBlock(I2CTarget_Handle handle, uint8_t *data)
{
    switch (protocolState) {
        case STATE_WAIT_FOR_CMD:
            /* Command byte received. Command has 3 additional argument bytes */
            cmd.argCount  = 3;
            protocolState = STATE_PROCESS_ARGS;
            break;
        case STATE_PROCESS_ARGS:
            /* Reading the command arguments */
            if (cmd.argIdx < cmd.argCount) {
                cmd.args[cmd.argIdx++] = *data;

                if (cmd.argIdx == cmd.argCount) {
                    /* All arguments read, prepare for sending response */
                    uint8_t startOffset = cmd.args[0];
                    uint16_t count =
                        ((uint16_t) cmd.args[1] << 8) | (cmd.args[2]);

                    cmd.dataPtr   = &rxBuffer[startOffset];
                    cmd.dataCount = count;
                    protocolState = STATE_PROCESS_PAYLOAD;
                }
            }
            break;
        case STATE_PROCESS_PAYLOAD:
            if (cmd.dataIdx < cmd.dataCount) {
                cmd.dataPtr[cmd.dataIdx++] = *data;
                if (cmd.dataIdx == cmd.dataCount) {
                    protocolState = STATE_CMD_DONE;
                }
            } else {
                protocolState = STATE_CMD_DONE;
            }
            break;
        case STATE_CMD_DONE:
            /* We do not expect to get here */
            break;
        default:
            break;
    }

    return 0;
}

/*
 *  ======== cmdCtlReadBlock ========
 */
static int cmdCtlReadBlock(I2CTarget_Handle handle, uint8_t *data)
{
    switch (protocolState) {
        case STATE_WAIT_FOR_CMD:
            /* Prepare for upcoming command arguments */
            cmd.argCount  = 3;
            protocolState = STATE_PROCESS_ARGS;
            break;
        case STATE_PROCESS_ARGS:
            /* Read command arguments */
            if (cmd.argIdx < cmd.argCount) {
                /* Store argument */
                cmd.args[cmd.argIdx++] = *data;

                if (cmd.argIdx == cmd.argCount) {
                    /* All arguments read, prepare for upcoming command data.
                     * args[0] = startOffset ("address")
                     * args[1] = byteCount (MSB)
                     * args[2] = byteCount (LSB)
                     */
                    uint8_t startOffset = cmd.args[0];
                    uint16_t count =
                        ((uint16_t)(cmd.args[1]) << 8) | cmd.args[2];

                    /* Copy response to TX buffer */
                    memcpy(txBuffer, dataString, count);

                    cmd.dataPtr   = &txBuffer[startOffset];
                    cmd.dataCount = count;
                    protocolState = STATE_PROCESS_PAYLOAD;
                }
            }
            break;
        case STATE_PROCESS_PAYLOAD:
            /* Feed data to controller */
            if (cmd.dataIdx < cmd.dataCount) {
                *data = cmd.dataPtr[cmd.dataIdx++];
                if (cmd.dataIdx == cmd.dataCount) {
                    protocolState = STATE_CMD_DONE;
                }
            } else {
                protocolState = STATE_CMD_DONE;
            }
            break;
        case STATE_CMD_DONE:
            /* We do not expect to get here */
            break;
        default:
            break;
    }

    return 0;
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
