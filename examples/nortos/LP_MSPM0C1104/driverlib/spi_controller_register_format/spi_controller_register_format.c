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
 * CMD_WRITE_TYPE_X are example write commands/register addresses the
 * Controller sends to the Peripheral. The Peripheral will initialize itself
 * to receive gCmdWriteTypeXBuffer example buffers.
 */
#define CMD_WRITE_TYPE_0 (3)
#define CMD_WRITE_TYPE_1 (4)
#define CMD_WRITE_TYPE_2 (5)

/*
 * CMD_READ_TYPE_X are example read commands/register addresses the Controller
 * sends to the Peripheral. The Peripheral will send example
 * gCmdReadTypeXBuffer buffers in response.
 */
#define CMD_READ_TYPE_0 (0)
#define CMD_READ_TYPE_1 (1)
#define CMD_READ_TYPE_2 (2)

/*
 * TYPE_x_LENGTH defines the length expected from the command, not including
 *  the command itself.
 * For READ commands, it represents the bytes received from the SPI Peripheral.
 * For WRITE commands, it represents the bytes sent to the SPI Peripheral.
 */
#define TYPE_0_LENGTH (1)
#define TYPE_1_LENGTH (2)
#define TYPE_2_LENGTH (6)

/* Maximum buffer size defined for this example */
#define MAX_BUFFER_SIZE (20)

/* Dummy data sent when receiving data from SPI Peripheral */
#define DUMMY_DATA (0xFF)

/* State machine to keep track of the current SPI Controller mode */
typedef enum SPI_ControllerModeEnum {
    IDLE_MODE,
    TX_REG_ADDRESS_WRITE_MODE,
    TX_REG_ADDRESS_READ_MODE,
    WRITE_DATA_MODE,
    READ_DATA_MODE,
    TIMEOUT_MODE
} SPI_Controller_Mode;

/*
 * gCmdWriteTypeXBuffer are example buffers initialized in the
 * Controller, they will be sent by the Controller to the Peripheral.
 */
uint8_t gCmdWriteType0Buffer[TYPE_0_LENGTH] = {0x11};
uint8_t gCmdWriteType1Buffer[TYPE_1_LENGTH] = {8, 9};
uint8_t gCmdWriteType2Buffer[TYPE_2_LENGTH] = {'A', 'B', 'C', 'D', 'E', 'F'};

/*
 * gCmdReadTypeXBuffer are example buffers initialized in the
 * Peripheral, they will be sent by the Peripheral to the Controller.
 */
uint8_t gCmdReadType0Buffer[TYPE_0_LENGTH] = {0};
uint8_t gCmdReadType1Buffer[TYPE_1_LENGTH] = {0};
uint8_t gCmdReadType2Buffer[TYPE_2_LENGTH] = {0};

/* Buffer used to receive data in the ISR */
uint8_t gRxBuffer[MAX_BUFFER_SIZE] = {0};
/* Buffer used to transmit data in the ISR */
uint8_t gTxBuffer[MAX_BUFFER_SIZE] = {0};

/* Used to track the state of the software state machine */
volatile SPI_Controller_Mode gControllerMode = IDLE_MODE;
/* Number of bytes left to receive */
volatile uint8_t gRxByteCount = 0;
/* Index of the next byte to be received in gRxBuffer */
volatile uint8_t gRxIndex = 0;
/* Number of bytes left to transfer */
volatile uint8_t gTxByteCount = 0;
/* Index of the next byte to be transmitted in gTxBuffer */
volatile uint8_t gTxIndex = 0;


/* Local function prototypes */
static void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);
static void SPI_Controller_writeReg(
    uint8_t writeCmd, uint8_t *data, uint8_t count);
static void SPI_Controller_readReg(uint8_t readCmd, uint8_t count);

int main(void)
{
    SYSCFG_DL_init();

    /* Enable interrupts */
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);

    /* Set LED to indicate start of transfer */
    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, (GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN));


    /*
     * Send Read Type 2 Command to Peripheral device.
     * Copy received data to gCmdReadType2Buffer.
     */
    SPI_Controller_readReg(CMD_READ_TYPE_2, TYPE_2_LENGTH);
    CopyArray(gRxBuffer, gCmdReadType2Buffer, TYPE_2_LENGTH);


    /*
     * Send Read Type 1 Command to Peripheral device.
     * Copy received data to gCmdReadType1Buffer.
     */
    SPI_Controller_readReg(CMD_READ_TYPE_1, TYPE_1_LENGTH);
    CopyArray(gRxBuffer, gCmdReadType1Buffer, TYPE_1_LENGTH);


    /*
     * Send Read Type 0 Command to Peripheral device.
     * Copy received data to gCmdReadType0Buffer.
     */
    SPI_Controller_readReg(CMD_READ_TYPE_0, TYPE_0_LENGTH);
    CopyArray(gRxBuffer, gCmdReadType0Buffer, TYPE_0_LENGTH);


    /* Send Write Type 2 Command to Peripheral device */
    SPI_Controller_writeReg(
        CMD_WRITE_TYPE_2, gCmdWriteType2Buffer, TYPE_2_LENGTH);


    /* Send Write Type 1 Command to Peripheral device */
    SPI_Controller_writeReg(
        CMD_WRITE_TYPE_1, gCmdWriteType1Buffer, TYPE_1_LENGTH);


    /* Send Write Type 0 Command to Peripheral device */
    SPI_Controller_writeReg(
        CMD_WRITE_TYPE_0, gCmdWriteType0Buffer, TYPE_0_LENGTH);


    /* If write and read were successful, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT,
            (GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN));
        delay_cycles(12000000);
    }
}

void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_TX:
            switch (gControllerMode) {
                case IDLE_MODE:
                case TIMEOUT_MODE:
                    break;
                case TX_REG_ADDRESS_READ_MODE:
                case READ_DATA_MODE:
                    if (gTxByteCount) {
                        /*  Send dummy data to get read more bytes */
                        DL_SPI_transmitData8(SPI_0_INST, DUMMY_DATA);
                        gTxByteCount--;
                    }
                    break;
                case TX_REG_ADDRESS_WRITE_MODE:
                    gControllerMode = WRITE_DATA_MODE;
                case WRITE_DATA_MODE:
                    if (gTxByteCount) {
                        /* Transmit data until all expected data is sent */
                        DL_SPI_transmitData8(
                            SPI_0_INST, gTxBuffer[gTxIndex++]);
                        gTxByteCount--;
                    } else {
                        /* Transmission is done, reset state machine */
                        gControllerMode = IDLE_MODE;
                    }
                    break;
            }
            break;
        case DL_SPI_IIDX_RX:
            switch (gControllerMode) {
                case IDLE_MODE:
                case TIMEOUT_MODE:
                    break;
                case TX_REG_ADDRESS_READ_MODE:
                    /* Ignore data and change state machine to read data */
                    DL_SPI_receiveData8(SPI_0_INST);
                    gControllerMode = READ_DATA_MODE;
                    break;
                case READ_DATA_MODE:
                    if (gRxByteCount) {
                        /* Receive data until all expected data is read */
                        gRxBuffer[gRxIndex++] =
                            DL_SPI_receiveData8(SPI_0_INST);
                        gRxByteCount--;
                    }
                    if (gRxByteCount == 0) {
                        /* All data is received, reset state machine */
                        gControllerMode = IDLE_MODE;
                    }
                    break;
                case TX_REG_ADDRESS_WRITE_MODE:
                case WRITE_DATA_MODE:
                    /* Ignore the data while transmitting */
                    DL_SPI_receiveData8(SPI_0_INST);
                    break;
            }
            break;
        default:
            break;
    }
}

/*
 *  Controller sends a command to the Peripheral device to write data sent
 *  from the Controller.
 *  After sending the command, the Controller mode will be in
 *  TX_REG_ADDRESS_WRITE_MODE. If there are remaining bytes to transmit, the
 *  Controller will move to TX_DATA_MODE and it will continue to transmit
 *  the remaining bytes. Data sent by the Peripheral will be ignored during the
 *  transmission of the data. After all data is transmitted, the Controller
 *  will move back to IDLE_MODE.
 *
 *  writeCmd  The write command/register address to send to the Peripheral.
 *            Example: CMD_WRITE_TYPE_0
 *  data      The buffer containing the data to send to the Peripheral to write.
 *            Example: gCmdWriteType2Buffer
 *  count     The length of data to read. Example: TYPE_0_LENGTH
 */
static void SPI_Controller_writeReg(
    uint8_t writeCmd, uint8_t *data, uint8_t count)
{
    gControllerMode = TX_REG_ADDRESS_WRITE_MODE;

    /* Copy data to gTxBuffer */
    CopyArray(data, gTxBuffer, count);

    gTxByteCount = count;
    gRxByteCount = count;
    gRxIndex     = 0;
    gTxIndex     = 0;

    /*
     * TX interrupts are disabled and RX interrupts are enabled by default.
     * TX interrupts will be enabled after sending the command, and they will
     * trigger after the FIFO has more space to send all subsequent bytes.
     */
    DL_SPI_clearInterruptStatus(SPI_0_INST, DL_SPI_INTERRUPT_TX);
    DL_SPI_transmitData8(SPI_0_INST, writeCmd);
    DL_SPI_enableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_TX);

    /* Go to sleep until all data is transmitted */
    while (gControllerMode != IDLE_MODE) {
        __WFI();
    }

    /* Disable TX interrupts after the command is complete */
    DL_SPI_disableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_TX);
    DL_SPI_clearInterruptStatus(SPI_0_INST, DL_SPI_INTERRUPT_TX);
}

/*
 *  Controller sends a command to the Peripheral device to read data as
 *  specified by the command and send it to the Controller.
 *  After sending the command, the Controller will be in
 *  TX_REG_ADDRESS_READ_MODE. After receiving the first byte from the
 *  Peripheral, if there are more bytes to receive, then the Controller will
 *  move to RX_DATA_MODE. The Controller will transmit DUMMY_DATA to
 *  receive 'count' number of bytes from the Peripheral.
 *  After 'count' number of bytes have been received, the Controller will move
 *  back to IDLE_MODE. The received data will be available in gRxBuffer.
 *
 *  readCmd   The read command/register address to send to the Peripheral.
 *            Example: CMD_WRITE_TYPE_0
 *  count     The length of data to read. Example: TYPE_0_LENGTH
 */
static void SPI_Controller_readReg(uint8_t readCmd, uint8_t count)
{
    gControllerMode = TX_REG_ADDRESS_READ_MODE;
    gRxByteCount    = count;
    gTxByteCount    = count;
    gRxIndex        = 0;
    gTxIndex        = 0;

    /*
     * TX interrupts are disabled and RX interrupts are enabled by default.
     * TX interrupts will be enabled after sending the command, and they will
     * trigger after the FIFO has more space to send all subsequent bytes.
     */
    DL_SPI_clearInterruptStatus(SPI_0_INST, DL_SPI_INTERRUPT_TX);
    DL_SPI_transmitData8(SPI_0_INST, readCmd);
    DL_SPI_enableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_TX);

    /* Go to sleep until all data is received */
    while (gControllerMode != IDLE_MODE) {
        __WFI();
    }

    /* Disable TX interrupts after the command is complete */
    DL_SPI_disableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_TX);
    DL_SPI_clearInterruptStatus(SPI_0_INST, DL_SPI_INTERRUPT_TX);
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
