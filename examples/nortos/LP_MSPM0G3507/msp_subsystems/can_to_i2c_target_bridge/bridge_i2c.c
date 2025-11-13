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
#include "ti_msp_dl_config.h"
#include "user_define.h"

Custom_Element gI2C_RX_Element;
Custom_Element gI2C_TX_Element;
uint8_t gI2CReceiveGroup[I2C_RX_SIZE];
uint8_t gI2CTransmitGroup[I2C_TX_SIZE];
uint32_t gTxLen, gTxCount;
bool gI2CTxflag;

bool getI2CRxMsg_controller(uint8_t *I2CReceiveGroup)
{
    static uint16_t num, data_length;
    static bool flag1;
    uint8_t temp;
    temp = DL_I2C_receiveControllerData(I2C_INST);
    switch(num){
        case 0:
            if(temp == 0x55) //First byte is 0x55
            {
                flag1 = true;
            }
            else
            {
                flag1 = false;
            }
            break;
        case 1:
            if(temp == 0xAA) //Second byte is 0xAA
            {
                flag1 = true;
            }
            else
            {
                flag1 = false;
                num = 0;
            }
            break;
            /* if I2C_ID_LENGTH = 4, 7th byte is length
             * if I2C_ID_LENGTH = 1, 4th byte is length
             * if I2C_ID_LENGTH = 0, 3th byte is length
             * */
        case (2+I2C_ID_LENGTH):
            data_length = temp; //7th byte is length
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into I2CReceiveGroup */
        I2CReceiveGroup[num] = temp;
        num++;
        if(num>=(data_length + 3 + I2C_ID_LENGTH))
        {
            /* Returns true when the group is fully received */
            flag1 =false;
            num = 0;
            return true;
        }
    }
    return false;
}

bool getI2CRxMsg_target(uint8_t *I2CReceiveGroup)
{
    static uint16_t num, data_length;
    static bool flag1;
    uint8_t temp;
    temp = DL_I2C_receiveTargetData(I2C_INST);
    switch(num){
        case 0:
            if(temp == 0x55) //First byte is 0x55
            {
                flag1 = true;
            }
            else
            {
                flag1 = false;
            }
            break;
        case 1:
            if(temp == 0xAA) //Second byte is 0xAA
            {
                flag1 = true;
            }
            else
            {
                flag1 = false;
                num = 0;
            }
            break;
            /* if I2C_ID_LENGTH = 4, 7th byte is length
             * if I2C_ID_LENGTH = 1, 4th byte is length
             * if I2C_ID_LENGTH = 0, 3th byte is length
             * */
        case (2+I2C_ID_LENGTH):
            data_length = temp; //7th byte is length
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into I2CReceiveGroup */
        I2CReceiveGroup[num] = temp;
        num++;
        if(num>=(data_length + 3 + I2C_ID_LENGTH))
        {
            /* Returns true when the group is fully received */
            flag1 =false;
            num = 0;
            return true;
        }
    }
    return false;
}

bool processI2CRxMsg_controller(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element, uint8_t address)
{
    /* The format of I2CTransmitGroup is
     * if I2C_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get origin id, indicating the origin of the message */
    /* message from I2C slave has 1 byte address*/
    I2C_RX_Element->origin_id = address;
    /* Get Destination id, indicating the destination of the message */
    /* if I2C_ID_LENGTH = 4, id = I2CReceiveGroup[2]<<24 + I2CReceiveGroup[3]<<16 +
     * I2CReceiveGroup[4]<<8 + I2CReceiveGroup[5]
     * if I2C_ID_LENGTH = 1, id = I2CReceiveGroup[2]
     * if I2C_ID_LENGTH = 0, id = default value;
     * */
    if(I2C_ID_LENGTH)
    {
        id = 0;
        for(count=0;count<I2C_ID_LENGTH;count++)
        {
            id += I2CReceiveGroup[count+2]<<((I2C_ID_LENGTH-1-count)<<3);
        }
    }
    else
    {
        id = DEFAULT_I2C_DESTINATION_ID;
    }
    I2C_RX_Element->destination_id = id;

    /* Get data length */
    /* if I2C_ID_LENGTH = 4, data length = I2CReceiveGroup[6]
     * if I2C_ID_LENGTH = 1, data length = I2CReceiveGroup[3]
     * if I2C_ID_LENGTH = 0, data length = I2CReceiveGroup[2]
     * */
    I2C_RX_Element->dlc = I2CReceiveGroup[I2C_ID_LENGTH + 2];
    /* Get data */
    /* if I2C_ID_LENGTH = 4, data start from I2CReceiveGroup[7]
     * if I2C_ID_LENGTH = 1, data start from I2CReceiveGroup[4]
     * if I2C_ID_LENGTH = 0, data start from I2CReceiveGroup[3]
     * */
    for(count=0;count<(I2C_RX_Element->dlc);count++)
    {
        I2C_RX_Element->data[count] = I2CReceiveGroup[I2C_ID_LENGTH + 3 +count];
    }
    return true;
}

bool processI2CRxMsg_target(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element)
{
    /* The format of I2CTransmitGroup is
     * if I2C_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get origin id, indicating the origin of the message */
    /* message received by I2C slave does not have origin id, set default value*/
    I2C_RX_Element->origin_id = DEFAULT_I2C_ORIGIN_ID;
    /* Get Destination id, indicating the destination of the message */
    /* if I2C_ID_LENGTH = 4, id = I2CReceiveGroup[2]<<24 + I2CReceiveGroup[3]<<16 +
     * I2CReceiveGroup[4]<<8 + I2CReceiveGroup[5]
     * if I2C_ID_LENGTH = 1, id = I2CReceiveGroup[2]
     * if I2C_ID_LENGTH = 0, id = default value;
     * */
    if(I2C_ID_LENGTH)
    {
        id = 0;
        for(count=0;count<I2C_ID_LENGTH;count++)
        {
            id += I2CReceiveGroup[count+2]<<((I2C_ID_LENGTH-1-count)<<3);
        }
    }
    else
    {
        id = DEFAULT_I2C_DESTINATION_ID;
    }
    I2C_RX_Element->destination_id = id;

    /* Get data length */
    /* if I2C_ID_LENGTH = 4, data length = I2CReceiveGroup[6]
     * if I2C_ID_LENGTH = 1, data length = I2CReceiveGroup[3]
     * if I2C_ID_LENGTH = 0, data length = I2CReceiveGroup[2]
     * */
    I2C_RX_Element->dlc = I2CReceiveGroup[I2C_ID_LENGTH + 2];
    /* Get data */
    /* if I2C_ID_LENGTH = 4, data start from I2CReceiveGroup[7]
     * if I2C_ID_LENGTH = 1, data start from I2CReceiveGroup[4]
     * if I2C_ID_LENGTH = 0, data start from I2CReceiveGroup[3]
     * */
    for(count=0;count<(I2C_RX_Element->dlc);count++)
    {
        I2C_RX_Element->data[count] = I2CReceiveGroup[I2C_ID_LENGTH + 3 +count];
    }
    return true;
}

/*
 * controller connect to target and write bytes
 */
bool sendI2CTxMsg_controller(uint8_t *I2CTransmitGroup, uint8_t address)
{
    /*
     * Fill the FIFO
     *  The FIFO is 8-bytes deep, and this function will return number
     *  of bytes written to FIFO */
    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_INST, &I2CTransmitGroup[0], gTxLen);
    /* Enable TXFIFO trigger interrupt if there are more bytes to send */
    if (gTxCount < gTxLen) {
        DL_I2C_enableInterrupt(
            I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    } else {
        DL_I2C_disableInterrupt(
            I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
    }
    /*
     * Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
    DL_I2C_startControllerTransfer(
        I2C_INST, address, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);
    return true;
}

/*
 * controller connect to target and read bytes
 */
bool readI2CRxMsg_controller(uint16_t length, uint8_t address)
{
     /* Send a read request to Target */
     while (!(
         DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
         ;
     DL_I2C_startControllerTransfer(
         I2C_INST, address, DL_I2C_CONTROLLER_DIRECTION_RX, length);
     return true;
}

bool sendI2CTxMsg_target(uint8_t *I2CTransmitGroup)
{
    /* Enable TXFIFO trigger interrupt to put multi-bytes into FIFO */
    DL_I2C_enableInterrupt(I2C_INST, DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER);
    /* Use IO to trigger the controller to read from target */
    DL_GPIO_clearPins(GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN);
    delay_cycles(100);
    DL_GPIO_setPins(GPIO_SWITCHES_PORT, GPIO_SWITCHES_USER_SWITCH_1_PIN);
    return true;
}

uint8_t processI2CTxMsg_controller(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element)
{
    /* The format of I2CTransmitGroup is
     * if I2C_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;
    uint8_t id;

    /* Set Destination id*/
    /* message sent by I2C master have address*/
    id = I2C_TX_Element->destination_id;

    /* Fill in header */
    I2CTransmitGroup[0] = 0x55;
    I2CTransmitGroup[1] = 0xAA;
    /* Fill in ID area */
    /* In this demo, ID area is filled with origin id to indicate the origin of message
     * Users can modify the ID area as needed */
    /* if I2C_ID_LENGTH = 4,
     * I2CTransmitGroup[2] = (uint8_t)((I2C_TX_Element->id >> 24) & 0x000000ff);
     * I2CTransmitGroup[3] = (uint8_t)((I2C_TX_Element->id >> 16) & 0x000000ff);
     * I2CTransmitGroup[4] = (uint8_t)((I2C_TX_Element->id >> 8) & 0x000000ff);
     * I2CTransmitGroup[5] = (uint8_t)(I2C_TX_Element->id & 0x000000ff);
     * if I2C_ID_LENGTH = 1,
     * I2CTransmitGroup[2] = (uint8_t)(I2C_TX_Element->id & 0x000000ff);
     * if I2C_ID_LENGTH = 0, no id area;
     * */
    if(I2C_ID_LENGTH)
    {
        for(count=0;count<I2C_ID_LENGTH;count++)
        {
            I2CTransmitGroup[count+2]= (uint8_t)((I2C_TX_Element->origin_id >> ((I2C_ID_LENGTH-1-count)<<3)) &0x000000ff);
        }
    }
    /* Fill in length area */
    /* if I2C_ID_LENGTH = 4, data length is I2CTransmitGroup[6]
     * if I2C_ID_LENGTH = 1, data length is I2CTransmitGroup[3]
     * if I2C_ID_LENGTH = 0, data length is I2CTransmitGroup[2]
     * */
    I2CTransmitGroup[I2C_ID_LENGTH + 2] = I2C_TX_Element->dlc;
    /* Fill in data area */
    /* if I2C_ID_LENGTH = 4, data start from I2CTransmitGroup[7]
     * if I2C_ID_LENGTH = 1, data start from I2CTransmitGroup[4]
     * if I2C_ID_LENGTH = 0, data start from I2CTransmitGroup[3]
     * */
    for(count=0; count < I2C_TX_Element->dlc; count++)
    {
        I2CTransmitGroup[I2C_ID_LENGTH + 3 + count] =  I2C_TX_Element->data[count];
    }
    return id;
}

bool processI2CTxMsg_target(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element)
{
    /* The format of I2CTransmitGroup is
     * if I2C_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if I2C_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;

    /* Set Destination id*/
    /* message sent by I2C slave does not have Destination id, no action with Destination id*/

    /* Fill in header */
    I2CTransmitGroup[0] = 0x55;
    I2CTransmitGroup[1] = 0xAA;
    /* Fill in ID area */
    /* In this demo, ID area is filled with origin id to indicate the origin of message
     * Users can modify the ID area as needed */
    /* if I2C_ID_LENGTH = 4,
     * I2CTransmitGroup[2] = (uint8_t)((I2C_TX_Element->id >> 24) & 0x000000ff);
     * I2CTransmitGroup[3] = (uint8_t)((I2C_TX_Element->id >> 16) & 0x000000ff);
     * I2CTransmitGroup[4] = (uint8_t)((I2C_TX_Element->id >> 8) & 0x000000ff);
     * I2CTransmitGroup[5] = (uint8_t)(I2C_TX_Element->id & 0x000000ff);
     * if I2C_ID_LENGTH = 1,
     * I2CTransmitGroup[2] = (uint8_t)(I2C_TX_Element->id & 0x000000ff);
     * if I2C_ID_LENGTH = 0, no id area;
     * */
    if(I2C_ID_LENGTH)
    {
        for(count=0;count<I2C_ID_LENGTH;count++)
        {
            I2CTransmitGroup[count+2]= (uint8_t)((I2C_TX_Element->origin_id >> ((I2C_ID_LENGTH-1-count)<<3)) &0x000000ff);
        }
    }
    /* Fill in length area */
    /* if I2C_ID_LENGTH = 4, data length is I2CTransmitGroup[6]
     * if I2C_ID_LENGTH = 1, data length is I2CTransmitGroup[3]
     * if I2C_ID_LENGTH = 0, data length is I2CTransmitGroup[2]
     * */
    I2CTransmitGroup[I2C_ID_LENGTH + 2] = I2C_TX_Element->dlc;
    /* Fill in data area */
    /* if I2C_ID_LENGTH = 4, data start from I2CTransmitGroup[7]
     * if I2C_ID_LENGTH = 1, data start from I2CTransmitGroup[4]
     * if I2C_ID_LENGTH = 0, data start from I2CTransmitGroup[3]
     * */
    for(count=0; count < I2C_TX_Element->dlc; count++)
    {
        I2CTransmitGroup[I2C_ID_LENGTH + 3 + count] =  I2C_TX_Element->data[count];
    }
    return true;
}

uint16_t gGetI2cRxMsg_Count;
bool getI2CRxMsg_controller_transparent(uint8_t *I2CReceiveGroup)
{
    uint8_t temp;
    temp = DL_I2C_receiveControllerData(I2C_INST);

    if(gGetI2cRxMsg_Count < I2C_RX_SIZE)
    {
        /* log data into I2CReceiveGroup */
        I2CReceiveGroup[gGetI2cRxMsg_Count] = temp;
        gGetI2cRxMsg_Count++;
        return false;
    }
    else
    {
        /* Returns true when the group is full */
        return true;
    }
}

bool getI2CRxMsg_target_transparent(uint8_t *I2CReceiveGroup)
{
    uint8_t temp;
    temp = DL_I2C_receiveTargetData(I2C_INST);

    if(gGetI2cRxMsg_Count < I2C_RX_SIZE)
    {
        /* log data into I2CReceiveGroup */
        I2CReceiveGroup[gGetI2cRxMsg_Count] = temp;
        gGetI2cRxMsg_Count++;
        return false;
    }
    else
    {
        /* Returns true when the group is full */
        return true;
    }
}

bool processI2CRxMsg_controller_transparent(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element, uint8_t address)
{
    /* The format of I2C:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get origin id, indicating the origin of the message */
    /* message from I2C slave has 1 byte address*/
    I2C_RX_Element->origin_id = address;
    /* Get Destination id, indicating the destination of the message */
    /* No Destination id in Transparent transmission, id = default value; */
    I2C_RX_Element->destination_id = DEFAULT_I2C_DESTINATION_ID;

    /* Get data length */
    I2C_RX_Element->dlc = gGetI2cRxMsg_Count;
    /* Get data */
    for(count=0;count<(I2C_RX_Element->dlc);count++)
    {
        I2C_RX_Element->data[count] = I2CReceiveGroup[count];
    }
    return true;
}

bool processI2CRxMsg_target_transparent(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element)
{
    /* The format of I2C:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get origin id, indicating the origin of the message */
    /* message received by I2C slave does not have origin id, set default value*/
    I2C_RX_Element->origin_id = DEFAULT_I2C_ORIGIN_ID;
    /* Get Destination id, indicating the destination of the message */
    /* No Destination id in Transparent transmission, id = default value; */
    I2C_RX_Element->destination_id = DEFAULT_I2C_DESTINATION_ID;

    /* Get data length */
    I2C_RX_Element->dlc = gGetI2cRxMsg_Count;
    /* Get data */
    for(count=0;count<(I2C_RX_Element->dlc);count++)
    {
        I2C_RX_Element->data[count] = I2CReceiveGroup[count];
    }
    return true;
}

uint8_t processI2CTxMsg_controller_transparent(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element)
{
    /* The format of I2C:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;
    uint8_t id;
    /* Set Destination id*/
    /* message sent by I2C master have address*/
    id = I2C_TX_Element->destination_id;
    /* Fill in data area */
    for(count=0; count < I2C_TX_Element->dlc; count++)
    {
        I2CTransmitGroup[count] =  I2C_TX_Element->data[count];
    }
    return id;
}

bool processI2CTxMsg_target_transparent(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element)
{
    /* The format of I2C:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;
    /* Fill in data area */
    for(count=0; count < I2C_TX_Element->dlc; count++)
    {
        I2CTransmitGroup[count] =  I2C_TX_Element->data[count];
    }
    return true;
}
