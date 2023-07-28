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

/* The length of the commands to send */
#define CMD_TYPE_0_LENGTH (1)
#define CMD_TYPE_1_LENGTH (3)
#define CMD_TYPE_2_LENGTH (5)

/* The length of the data to send */
#define DATA_TYPE_0_LENGTH (0)
#define DATA_TYPE_1_LENGTH (3)
#define DATA_TYPE_2_LENGTH (10)

/* The different commands to send to the Peripheral */
uint8_t gCmdType0Buffer[CMD_TYPE_0_LENGTH] = {0x11};
uint8_t gCmdType1Buffer[CMD_TYPE_1_LENGTH] = {7, 8, 9};
uint8_t gCmdType2Buffer[CMD_TYPE_2_LENGTH] = {0xA, 0xB, 0xC, 0xD, 0xE};

/* The different data to send to the Peripheral */
uint8_t gDataType1Buffer[DATA_TYPE_1_LENGTH] = {0x44, 0x55, 0x66};
uint8_t gDataType2Buffer[DATA_TYPE_2_LENGTH] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

/*
 *  Helper function to transmit commands from the Controller to the Peripheral.
 *  This sets the CD line LOW, indicating that a command is being transmitted.
 *  The CD line will stay LOW for the number of command bytes specified, and
 *  then the CD line will automatically be set HIGH.
 *
 *  cmd          The command to send to the Peripheral.
 *               Example: CMD_TYPE_0
 *  cmdLength    The number of command bytes to send.
 *               Example: CMD_TYPE_0_LENGTH
 */
void SPI_Controller_transmitCommand(uint8_t *cmd, uint8_t cmdLength)
{
    /* SPI must be idle prior to setting a new value of CDMODE */
    while (DL_SPI_isBusy(SPI_0_INST))
        ;

    DL_SPI_setControllerCommandDataModeConfig(SPI_0_INST, cmdLength);

    int i = 0;
    for (i = 0; i < cmdLength; i++) {
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
        DL_SPI_transmitData8(SPI_0_INST, cmd[i]);
    }
}

/*
 *  Helper function to transmit data from the Controller to the Peripheral.
 *  This function assumes that the command has already been transmitted, so
 *  the CD line should already be set HIGH indicating that data is being
 *  transmitted.
 *
 *  data         The data to send to the Peripheral.
 *               Example: DATA_TYPE_0
 *  dataLength   The number of data bytes to send.
 *               Example: DATA_TYPE_0_LENGTH
 */
void SPI_Controller_transmitData(uint8_t *data, uint8_t dataLength)
{
    int i = 0;
    for (i = 0; i < dataLength; i++) {
        while (DL_SPI_isBusy(SPI_0_INST))
            ;
        DL_SPI_transmitData8(SPI_0_INST, data[i]);
    }
}

int main(void)
{
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    /* Transmit Command Type 0. This has no associated data to transmit. */
    SPI_Controller_transmitCommand(gCmdType0Buffer, CMD_TYPE_0_LENGTH);

    /* Transmit Command Type 1 and Data Type 1.  */
    SPI_Controller_transmitCommand(gCmdType1Buffer, CMD_TYPE_1_LENGTH);
    SPI_Controller_transmitData(gDataType1Buffer, DATA_TYPE_1_LENGTH);

    /* Transmit Command Type 2 and Data Type 2.  */
    SPI_Controller_transmitCommand(gCmdType2Buffer, CMD_TYPE_2_LENGTH);
    SPI_Controller_transmitData(gDataType2Buffer, DATA_TYPE_2_LENGTH);

    /* After all data is transmitted, toggle LED */
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}
