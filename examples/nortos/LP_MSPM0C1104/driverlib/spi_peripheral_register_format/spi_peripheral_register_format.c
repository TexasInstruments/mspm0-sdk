/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

/*
 * CMD_WRITE_TYPE_X are example write commands the Controller sends to the
 * Peripheral. The Peripheral will initialize itself to receive
 * gCmdWriteTypeXBuffer example buffers.
 */
#define CMD_WRITE_TYPE_0 (3)
#define CMD_WRITE_TYPE_1 (4)
#define CMD_WRITE_TYPE_2 (5)

/*
 * CMD_READ_TYPE_X are example read commands the Controller sends to the
 * Peripheral. The Peripheral will send example gCmdReadTypeXBuffer
 * buffers in response.
 */
#define CMD_READ_TYPE_0 (0)
#define CMD_READ_TYPE_1 (1)
#define CMD_READ_TYPE_2 (2)

/*
 * TYPE_x_LENGTH defines the length expected from the command, not including
 *  the command itself.
 * For READ commands, it represents the bytes sent back to the Controller.
 * For WRITE commands, it represents the bytes received from the Controller.
 */
#define TYPE_0_LENGTH (1)
#define TYPE_1_LENGTH (2)
#define TYPE_2_LENGTH (6)

/* Maximum buffer size defined for this example */
#define MAX_BUFFER_SIZE (20)

/* State machine to keep track of the current SPI Peripheral mode */
typedef enum SPI_PeripheralModeEnum {
    IDLE_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    TIMEOUT_MODE
} SPI_PeripheralMode;

/*
 * gCmdWriteTypeXBuffer are example buffers initialized in the
 * Controller, they will be sent by the Controller to the Peripheral.
 */
uint8_t gCmdWriteType0Buffer[TYPE_0_LENGTH] = {0};
uint8_t gCmdWriteType1Buffer[TYPE_1_LENGTH] = {0};
uint8_t gCmdWriteType2Buffer[TYPE_2_LENGTH] = {0};

/*
 * gCmdReadTypeXBuffer are example buffers initialized in the
 * Peripheral, they will be sent by the Peripheral to the Controller.
 */
uint8_t gCmdReadType0Buffer[TYPE_0_LENGTH] = {12};
uint8_t gCmdReadType1Buffer[TYPE_1_LENGTH] = {0x15, 0x16};
uint8_t gCmdReadType2Buffer[TYPE_2_LENGTH] = {'1', '2', '3', '4', '5', '6'};

/* Buffer used to receive data in the ISR */
uint8_t gRxBuffer[MAX_BUFFER_SIZE] = {0};
/* Buffer used to transmit data in the ISR */
uint8_t gTxBuffer[MAX_BUFFER_SIZE] = {0};

/* The Register Address/Command to use*/
volatile uint8_t gReceivedCommand = 0;
/* Used to track the state of the software state machine */
volatile SPI_PeripheralMode gPeripheralMode = RX_REG_ADDRESS_MODE;
/* Number of bytes left to receive */
volatile uint8_t gRxByteCount = 0;
/* Index of the next byte to be received in gRxBuffer */
volatile uint8_t gRxIndex = 0;
/* Number of bytes left to transfer */
volatile uint8_t gTxByteCount = 0;
/* Index of the next byte to be transmitted in gTxBuffer */
volatile uint8_t gTxIndex = 0;

/* Local function prototypes */
static void SPI_Peripheral_processCmd(uint8_t cmd);
static void SPI_Peripheral_transactionDone(uint8_t cmd);
static void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);

int main(void)
{
    SYSCFG_DL_init();

    /* Enable interrupts */
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);

    while (1) {
        __WFI();
    }
}

void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
            switch (gPeripheralMode) {
                case (RX_REG_ADDRESS_MODE):
                    /* Received a command from SPI Controller, now process it */
                    gReceivedCommand = DL_SPI_receiveData8(SPI_0_INST);
                    SPI_Peripheral_processCmd(gReceivedCommand);
                    break;
                case (RX_DATA_MODE):
                    /* Receive data until all expected data is read */
                    gRxBuffer[gRxIndex++] = DL_SPI_receiveData8(SPI_0_INST);
                    gRxByteCount--;
                    if (gRxByteCount == 0) {
                        /* All data is received, reset state machine */
                        gPeripheralMode = RX_REG_ADDRESS_MODE;
                        SPI_Peripheral_transactionDone(gReceivedCommand);
                    }
                    break;
                case (TX_DATA_MODE):
                    /* Read RXFIFO to empty it out, but it is dummy data
                     * so do not save it
                     */
                    DL_SPI_receiveData8(SPI_0_INST);
                    if (gTxByteCount > 0) {
                        /* Transmit data until all expected data is sent */
                        DL_SPI_transmitData8(
                            SPI_0_INST, gTxBuffer[gTxIndex++]);
                        gTxByteCount--;
                    }
                    if (gTxByteCount == 0) {
                        /* Transmission is done, reset state machine */
                        gPeripheralMode = RX_REG_ADDRESS_MODE;
                        SPI_Peripheral_transactionDone(gReceivedCommand);
                    }
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

/*
 * Process the received command from the SPI Controller and initialize the
 *  software state machine accordingly
 *
 *  cmd   The command/register address received
 */
static void SPI_Peripheral_processCmd(uint8_t cmd)
{
    gRxIndex     = 0;
    gTxIndex     = 0;
    gRxByteCount = 0;
    gTxByteCount = 0;

    switch (cmd) {
        case (CMD_READ_TYPE_0):
            /* Prepare to transmit gCmdReadType0Buffer data */
            gPeripheralMode = TX_DATA_MODE;
            gTxByteCount    = TYPE_0_LENGTH;
            /* Copy data to gTxBuffer */
            CopyArray(gCmdReadType0Buffer, gTxBuffer, TYPE_0_LENGTH);
            /* Send first byte */
            DL_SPI_transmitData8(SPI_0_INST, gTxBuffer[gTxIndex++]);
            gTxByteCount--;
            break;
        case (CMD_READ_TYPE_1):
            /* Prepare to transmit gCmdReadType1Buffer data */
            gPeripheralMode = TX_DATA_MODE;
            gTxByteCount    = TYPE_1_LENGTH;
            /* Copy data to gTxBuffer */
            CopyArray(gCmdReadType1Buffer, gTxBuffer, TYPE_1_LENGTH);
            /* Send first byte */
            DL_SPI_transmitData8(SPI_0_INST, gTxBuffer[gTxIndex++]);
            gTxByteCount--;
            break;
        case (CMD_READ_TYPE_2):
            /* Prepare to transmit gCmdReadType2Buffer data */
            gPeripheralMode = TX_DATA_MODE;
            gTxByteCount    = TYPE_2_LENGTH;
            /* Copy data to gTxBuffer */
            CopyArray(gCmdReadType2Buffer, gTxBuffer, TYPE_2_LENGTH);
            /* Send first byte */
            DL_SPI_transmitData8(SPI_0_INST, gTxBuffer[gTxIndex++]);
            gTxByteCount--;
            break;
        case (CMD_WRITE_TYPE_0):
            /* Prepare to receive gCmdReadType0Buffer data */
            gPeripheralMode = RX_DATA_MODE;
            gRxByteCount    = TYPE_0_LENGTH;
            break;
        case (CMD_WRITE_TYPE_1):
            /* Prepare to receive gCmdReadType1Buffer data */
            gPeripheralMode = RX_DATA_MODE;
            gRxByteCount    = TYPE_1_LENGTH;
            break;
        case (CMD_WRITE_TYPE_2):
            /* Prepare to receive gCmdReadType2Buffer data */
            gPeripheralMode = RX_DATA_MODE;
            gRxByteCount    = TYPE_2_LENGTH;
            break;
        default:
            break;
    }
}
/*
 * Function called after the command is completed
 *
 *  cmd   The command/register address which was completed
 */
static void SPI_Peripheral_transactionDone(uint8_t cmd)
{
    /* Toggle GPIOs to signify the successful completion of a transaction */
    DL_GPIO_togglePins(
        GPIO_LEDS_PORT, (GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN));

    switch (cmd) {
        case (CMD_READ_TYPE_0):
            /* gCmdReadType0Buffer data was sent */
            break;
        case (CMD_READ_TYPE_1):
            /* gCmdReadType1Buffer data was sent */
            break;
        case (CMD_READ_TYPE_2):
            /* gCmdReadType2Buffer data was sent */
            break;
        case (CMD_WRITE_TYPE_0):
            /* Write received data to gCmdWriteType0Buffer */
            CopyArray(gRxBuffer, gCmdWriteType0Buffer, TYPE_0_LENGTH);
            break;
        case (CMD_WRITE_TYPE_1):
            /* Write received data to gCmdWriteType1Buffer */
            CopyArray(gRxBuffer, gCmdWriteType1Buffer, TYPE_1_LENGTH);
            break;
        case (CMD_WRITE_TYPE_2):
            /* Write received data to gCmdWriteType2Buffer */
            CopyArray(gRxBuffer, gCmdWriteType2Buffer, TYPE_2_LENGTH);
            break;
        default:
            break;
    }
}

/*
 * Copies an array from source to destination
 *
 *  source   Pointer to source array
 *  dest     Pointer to destination array
 *  count    Number of bytes to copy
 *
 */
static void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++) {
        dest[copyIndex] = source[copyIndex];
    }
}
