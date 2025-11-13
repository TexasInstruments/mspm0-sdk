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

Custom_Element gSPI_RX_Element;
Custom_Element gSPI_TX_Element;
uint8_t gSpiReceiveGroup[SPI_RX_SIZE];
uint8_t gSpiTransmitGroup[SPI_TX_SIZE];
bool gSpiTxflag, gSpiRxflag, gSpiDirection;

bool getSpiRxMsg(SPI_Regs *spi, uint8_t *SpiReceiveGroup)
{
    static uint16_t num, data_length;
    static bool flag1;
    uint8_t temp;
    temp = DL_SPI_receiveData8(spi);
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
            /* if SPI_ID_LENGTH = 4, 7th byte is length
             * if SPI_ID_LENGTH = 1, 4th byte is length
             * if SPI_ID_LENGTH = 0, 3th byte is length
             * */
        case (2+SPI_ID_LENGTH):
            data_length = temp; //7th byte is length
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into SpiReceiveGroup */
        SpiReceiveGroup[num] = temp;
        num++;
        if(num>=(data_length + 3 + SPI_ID_LENGTH))
        {
            /* Returns true when the group is fully received */
            flag1 =false;
            num = 0;
            return true;
        }
    }
    return false;
}

bool processSpiRxMsg(uint8_t *SpiReceiveGroup, Custom_Element *SPI_RX_Element)
{
    /* The format of SpiReceiveGroup:
     * if SPI_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get origin id, indicating the origin of the message */
    /* SPI does not have id, set default value*/
    SPI_RX_Element->origin_id = DEFAULT_SPI_ORIGIN_ID;
    /* Get Destination id, indicating the destination of the message */
    /* if SPI_ID_LENGTH = 4, id = SpiReceiveGroup[2]<<24 + SpiReceiveGroup[3]<<16 +
     * SpiReceiveGroup[4]<<8 + SpiReceiveGroup[5]
     * if SPI_ID_LENGTH = 1, id = SpiReceiveGroup[2]
     * if SPI_ID_LENGTH = 0, id = default value;
     * */
    if(SPI_ID_LENGTH)
    {
        id = 0;
        for(count=0;count<SPI_ID_LENGTH;count++)
        {
            id += SpiReceiveGroup[count+2]<<((SPI_ID_LENGTH-1-count)<<3);
        }
    }
    else
    {
        id = DEFAULT_SPI_DESTINATION_ID;
    }
    SPI_RX_Element->destination_id = id;

    /* Get data length */
    /* if SPI_ID_LENGTH = 4, data length = SpiReceiveGroup[6]
     * if SPI_ID_LENGTH = 1, data length = SpiReceiveGroup[3]
     * if SPI_ID_LENGTH = 0, data length = SpiReceiveGroup[2]
     * */
    SPI_RX_Element->dlc = SpiReceiveGroup[SPI_ID_LENGTH + 2];
    /* Get data */
    /* if SPI_ID_LENGTH = 4, data start from SpiReceiveGroup[7]
     * if SPI_ID_LENGTH = 1, data start from SpiReceiveGroup[4]
     * if SPI_ID_LENGTH = 0, data start from SpiReceiveGroup[3]
     * */
    for(count=0;count<(SPI_RX_Element->dlc);count++)
    {
        SPI_RX_Element->data[count] = SpiReceiveGroup[SPI_ID_LENGTH + 3 +count];
    }
    return true;
}

bool readSpiRxMsg(SPI_Regs *spi, uint16_t length)
{
    static uint16_t SpiTransmitcount;

    if(false == DL_SPI_isTXFIFOFull(spi))
    {
        /* Put SPI_RX_DEFAULT_CHAR to SPI FIFO */
        DL_SPI_transmitData8(spi, SPI_RX_DEFAULT_CHAR);
        /* Check if all the bytes have been sent */
        SpiTransmitcount++;
        if(SpiTransmitcount >= length)
        {
            gSpiRxflag = false;
            SpiTransmitcount = 0;
        }
    }
    return true;
}

bool sendSpiTxMsg(SPI_Regs *spi, uint8_t *SpiTransmitGroup, uint16_t length)
{
    static uint16_t SpiTransmitcount;

    if(false == DL_SPI_isTXFIFOFull(spi))
    {
        /* Put one byte from SpiTransmitGroup to SPI FIFO */
        DL_SPI_transmitData8(spi, SpiTransmitGroup[SpiTransmitcount]);
        /* Check if all the bytes have been sent */
        SpiTransmitcount++;
        if(SpiTransmitcount >= length)
        {
            gSpiTxflag = false;
            SpiTransmitcount = 0;
        }
    }
    return true;
}

bool processSpiTxMsg(uint8_t *SpiTransmitGroup, Custom_Element *SPI_TX_Element)
{
    /* The format of SpiTransmitGroup:
     * if SPI_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if SPI_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;

    /* Set Destination id*/
    /* SPI does not have id, no action with Destination id*/

    /* Fill in header*/
    SpiTransmitGroup[0] = 0x55;
    SpiTransmitGroup[1] = 0xAA;

    /* Fill in ID area*/
    /* In this demo, ID area is filled with origin id to indicate the origin of message
     * Users can modify the ID area as needed */
    /* if SPI_ID_LENGTH = 4,
     * SpiTransmitGroup[2] = (uint8_t)((SPI_TX_Element->id >> 24) & 0x000000ff);
     * SpiTransmitGroup[3] = (uint8_t)((SPI_TX_Element->id >> 16) & 0x000000ff);
     * SpiTransmitGroup[4] = (uint8_t)((SPI_TX_Element->id >> 8) & 0x000000ff);
     * SpiTransmitGroup[5] = (uint8_t)(SPI_TX_Element->id & 0x000000ff);
     * if SPI_ID_LENGTH = 1,
     * SpiTransmitGroup[2] = (uint8_t)(SPI_TX_Element->id & 0x000000ff);
     * if SPI_ID_LENGTH = 0, no id area;
     * */
    if(SPI_ID_LENGTH)
    {
        for(count=0;count<SPI_ID_LENGTH;count++)
        {
            SpiTransmitGroup[count+2]= (uint8_t)((SPI_TX_Element->origin_id >> ((SPI_ID_LENGTH-1-count)<<3)) &0x000000ff);
        }
    }
    /* Fill in length area */
    /* if SPI_ID_LENGTH = 4, data length is SpiTransmitGroup[6]
     * if SPI_ID_LENGTH = 1, data length is SpiTransmitGroup[3]
     * if SPI_ID_LENGTH = 0, data length is SpiTransmitGroup[2]
     * */
    SpiTransmitGroup[SPI_ID_LENGTH + 2] = SPI_TX_Element->dlc;
    /* Fill in data area */
    /* if SPI_ID_LENGTH = 4, data start from SpiTransmitGroup[7]
     * if SPI_ID_LENGTH = 1, data start from SpiTransmitGroup[4]
     * if SPI_ID_LENGTH = 0, data start from SpiTransmitGroup[3]
     * */
    for(count=0; count < SPI_TX_Element->dlc; count++)
    {
        SpiTransmitGroup[SPI_ID_LENGTH + 3 + count] =  SPI_TX_Element->data[count];
    }
    return true;
}

uint16_t gGetSpiRxMsg_Count;
bool getSpiRxMsg_transparent(SPI_Regs *spi, uint8_t *SpiReceiveGroup)
{
    uint8_t temp;
    temp = DL_SPI_receiveData8(spi);
    if(gGetSpiRxMsg_Count < SPI_RX_SIZE)
    {
        /* Log data into SpiReceiveGroup */
        SpiReceiveGroup[gGetSpiRxMsg_Count] = temp;
        gGetSpiRxMsg_Count++;
        return false;
    }
    else
    {
        /* Returns true when the group is full */
        return true;
    }
}

bool processSpiRxMsg_transparent(uint8_t *SpiReceiveGroup, Custom_Element *SPI_RX_Element)
{
    /* The format of SPI:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;

    /* Get origin id, indicating the origin of the message */
    /* SPI does not have id, set default value*/
    SPI_RX_Element->origin_id = DEFAULT_SPI_ORIGIN_ID;
    /* Get Destination id, indicating the destination of the message */
    /* No Destination id in Transparent transmission, id = default value;
     * */
    SPI_RX_Element->destination_id = DEFAULT_SPI_DESTINATION_ID;

    /* Get data length */
    SPI_RX_Element->dlc = gGetSpiRxMsg_Count;
    /* Get data */
    for(count=0;count<(SPI_RX_Element->dlc);count++)
    {
        SPI_RX_Element->data[count] = SpiReceiveGroup[count];
    }
    return true;
}

void clearSpiTransmitGroup(uint8_t *SpiTransmitGroup)
{
    uint16_t count;
    for(count=0;count<(SPI_TX_SIZE);count++)
    {
        SpiTransmitGroup[count] = SPI_RX_DEFAULT_CHAR;
    }
}
bool processSpiTxMsg_transparent(uint8_t *SpiTransmitGroup, Custom_Element *SPI_TX_Element)
{
    /* The format of SPI:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;
    clearSpiTransmitGroup(SpiTransmitGroup);
    /* Fill in data area */
    for(count=0; count < SPI_TX_Element->dlc; count++)
    {
        SpiTransmitGroup[count] =  SPI_TX_Element->data[count];
    }
    return true;
}
