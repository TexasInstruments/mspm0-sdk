/* --COPYRIGHT--,BSD
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/

/* Include files */
#include "ti_msp_dl_config.h"
#include "pmbus_cmd_handlers.h"
#include "pmbus_cmd_list.h"
#include <string.h>
#include <stdbool.h>
#include "ti/pmbus/pmbus.h"

/*! Store the array index of the handler */
int8_t currentHandler = -1;
/*! Store the voltage reading from ADC  */
uint16_t outputVoltage;

/*
 * @brief Send 1 byte of data
 * @param[in] SMB   The SMBus structure
 * @param[in] data  One byte of data to be sent
 */
static void PMBus_sendByte(SMBus* SMB, uint8_t data)
{
    SMB->nwk.txBuffPtr[0] = data;
    SMB->nwk.txLen = 1;
}

/*
 * @brief Send 2 bytes of data
 * @param[in] SMB   The SMBus structure
 * @param[in] data  Two bytes of data to be sent
 */
static void PMBus_sendWord(SMBus* SMB, uint16_t data)
{
    SMB->nwk.txBuffPtr[0] = (data & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((data & 0xFF00) >> 8);
    SMB->nwk.txLen = 2;
}

/*
 * @brief Send a string of data
 * @param[in] SMB     The SMBus structure
 * @param[in] string  Pointer to the string
 * @param[in] length  Length of the string
 */
static void PMBus_sendString(SMBus* SMB, const char *string, uint32_t length)
{
    if ((length + 1) > SMB_MAX_PAYLOAD_SIZE)
    {
        return;
    }

    /*
     * Follow the block read command structure. The 1st data byte sent is the
     * byte count/payload length.
     */
    SMB->nwk.txBuffPtr[0] = length;

    memcpy(&SMB->nwk.txBuffPtr[1], string, length);

    /* Plus 1 to account for string length being sent as well */
    SMB->nwk.txLen = length + 1;
}

void PMBus_App_quickCommand(SMBus *SMB)
{
    DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_0_PIN);
}

void PMBus_App_isCommandValid(SMBus *SMB)
{
    uint16_t cmd = PMBus_getTargetCommand(SMB);
    uint8_t i;
    currentHandler = -1; /* Handler is invalid by default   */

    for(i = 0; i < (CMD_LIST_LENGTH); i++)
    {
        if (cmd == PMBus_cmdList[i].cmd)
        {
            /*
             * If the command is valid, save the handler which will be executed
             * if the command is received completely in Demo_CmdComplete
             */
            if (PMBus_cmdList[i].handler != NULL)
            {
                /* Command handler must be non-NULL value */
                currentHandler = i;

                if (PMBus_cmdList[i].length == SMBUS_BLOCK_LENGTH)
                {
                    PMBus_targetReportBlock(SMB);
                }
                else
                {
                    PMBus_targetReportLength(SMB, PMBus_cmdList[i].length);
                }
                break;
            }
        }
    }
}

void PMBus_App_isExtendedCommandValid(SMBus *SMB)
{
    uint16_t cmd = PMBus_getTargetExtendedCommand(SMB);
    uint8_t i;
    currentHandler = -1; /* Handler is invalid by default   */

    for(i = 0; i < (CMD_LIST_LENGTH); i++)
    {
        if (cmd == PMBus_cmdList[i].cmd)
        {
            /*
             * If the command is valid, save the handler which will be executed
             * if the command is received completely in Demo_CmdComplete
             */
            if (PMBus_cmdList[i].handler != NULL)
            {
                /* Command handler must be non-NULL value */
                currentHandler = i;

                if (PMBus_cmdList[i].length == SMBUS_BLOCK_LENGTH)
                {
                    PMBus_targetReportBlock(SMB);
                }
                else
                {
                    PMBus_targetReportLength(SMB, PMBus_cmdList[i].length);
                }
            }
        }
    }
}

void PMBus_App_processCommand(SMBus *SMB)
{
    int8_t ret;
    /* currentHandler is set by Demo_isCmd_valid when a command is received */
    if(currentHandler != -1)
    {
        /* If the command was found as valid, call the corresponding handler */
        ret = PMBus_cmdList[currentHandler].handler(SMB);
        currentHandler = -1;   /* Invalidate the handler after execution    */
    }
}

int8_t PMBus_clearFaults_hdlr(SMBus* SMB)
{
    /* This is a Write only command. It contains no data to write,
     * hence there is no validity check done for that packet */

    /* No functionality implemented in this example */
    return SMBUS_RET_OK;
}

int8_t PMBus_deviceId_hdlr(SMBus *SMB)
{
    int8_t returnStatus = SMBUS_RET_OK;
    char tempString[] = DEVICE_ID;
    uint32_t stringLen;
    /* Subtract 1 from sizeof result to ignore end of string character */
    stringLen = sizeof(tempString)-1;

    /* In response to a read request, send just the device ID */
    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        PMBus_sendString(SMB, tempString, stringLen);
    }
    /* In response to a write command, return an SMBUS error */
    else
    {
        returnStatus = SMBUS_RET_ERROR;
    }

    return(returnStatus);
}

int8_t   VMode;
int8_t PMBus_voutMode_hdlr(SMBus *SMB)
{
    int8_t   returnStatus = SMBUS_RET_OK;

    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        /* Send Vmode as a response */
        PMBus_sendByte(SMB, VMode);
    }
    else
    {
        /* Handle the write condition   */
        VMode = SMB->nwk.rxBuffPtr[1];
    }

    return (returnStatus);
}

uint16_t ovWarnLimit;
int8_t PMBus_voutOVWarnLimit_hdlr(SMBus *SMB)
{
    int8_t returnStatus = SMBUS_RET_OK;

    /* If we need to respond to a read command, RX index should be as expected for just a command */
    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        /* Send ovFaultLimit as a response */
        PMBus_sendWord(SMB, ovWarnLimit);
    }
    else
    {   /* Handle the write condition   */
        ovWarnLimit = (SMB->nwk.rxBuffPtr[2] << 8) | SMB->nwk.rxBuffPtr[1];
    }
    return returnStatus;
}

int8_t PMBus_MFRID_hdlr(SMBus *SMB)
{
    int8_t returnStatus = SMBUS_RET_OK;

    /* In response to a read request, send mfrID */
    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        PMBus_sendString(SMB, MFR_ID_DEFAULT, sizeof(MFR_ID_DEFAULT)-1);
    }
    else
    {
        /* Write command is not handled in this example */
    }
    return(SMBUS_RET_OK);
}

int8_t PMBus_readVout_hdlr(SMBus *SMB)
{
    int8_t returnStatus = SMBUS_RET_OK;

    /* If this is a read command, return the voutVolts value */
    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        PMBus_sendWord(SMB, outputVoltage);
    }
    else
    {
        /* This command is read only, so if not reading return an error */
        returnStatus = SMBUS_RET_ERROR;
    }
    return returnStatus;
}

uint8_t extByteValue = 0;
int8_t PMBus_extByteCmd_hdlr(SMBus *SMB)
{
    int8_t returnStatus = SMBUS_RET_OK;

    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_EXTENDED_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        /* Handle the read command  */
        PMBus_sendByte(SMB, extByteValue);
    }
    else
    {
        /* Handle the write command */
        extByteValue = SMB->nwk.rxBuffPtr[2];
    }
    return returnStatus;
}


uint16_t extWordValue = 0;
int8_t PMBus_extWordCmd_hdlr(SMBus *SMB)
{
    int8_t returnStatus = SMBUS_RET_OK;

    if ((SMB->nwk.rxIndex == EXPECTED_PMBUS_READ_EXTENDED_CMD_RX_BYTE_COUNT) &&
        (SMB->nwk.eState == SMBus_NwkState_TX_Resp))
    {
        /* Handle the read command  */
        PMBus_sendWord(SMB, extWordValue);
    }
    else
    {
        /* Handle the write command */
        extWordValue = (uint16_t)(((uint16_t)SMB->nwk.rxBuffPtr[3] << 8) |  (uint16_t)SMB->nwk.rxBuffPtr[2]);
    }
    return returnStatus;
}

uint16_t mult;
int8_t PMBus_processCall_hdlr(SMBus *SMB)
{
    /*
     * This handler does multiplication operation on received two bytes
     * and sends the result to controller.
     */
    int8_t returnStatus = SMBUS_RET_OK;

    /* Multiply the 2 received values   */
    mult = SMB->nwk.rxBuffPtr[1] * SMB->nwk.rxBuffPtr[2];
    /* Set the response as a word   */
    SMB->nwk.txBuffPtr[0] = (mult & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((mult & 0xFF00) >> 8);
    SMB->nwk.txLen = 2;

    return returnStatus;
}

uint16_t sum;
uint16_t length;
int8_t PMBus_blockProcessCall_hdlr(SMBus *SMB)
{
    /*
     * This handler does addition operation on received bytes and sends
     * the result to controller.
     */

    int8_t returnStatus = SMBUS_RET_OK;

    length = SMB->nwk.rxIndex - 2; /* -1 for command byte, -1 for byte count */

    /* Ensure length matches the byte_count as described in the packet */
    if (length != SMB->nwk.rxBuffPtr[1])
    {
        return (SMBUS_RET_ERROR);
    }

    sum = 0;
    for (int j = 0; j < length; j++)
    {
        sum += SMB->nwk.rxBuffPtr[j+2];
    }
    SMB->nwk.txBuffPtr[0] = 2;    /* Response length is 2 bytes */
    SMB->nwk.txBuffPtr[1] = (sum & 0x00FF);
    SMB->nwk.txBuffPtr[2] = ((sum & 0xFF00) >> 8);
    SMB->nwk.txLen = 3; /* Number of bytes to send (including COUNT_TX) */

    return returnStatus;
}
