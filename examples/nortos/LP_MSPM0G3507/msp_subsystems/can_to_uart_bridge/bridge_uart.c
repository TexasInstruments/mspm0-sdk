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

Custom_Element gUART_RX_Element;
Custom_Element gUART_TX_Element;
uint8_t gUartReceiveGroup[UART_RX_SIZE];
uint8_t gUartTransmitGroup[UART_TX_SIZE];

bool gUartTxflag;

bool getUartRxMsg(UART_Regs *uart, uint8_t *UartReceiveGroup)
{
    static uint16_t num, data_length;
    static bool flag1;
    uint8_t temp;
    temp = DL_UART_Main_receiveData(uart);
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
        /* if UART_ID_LENGTH = 4, 7th byte is length
         * if UART_ID_LENGTH = 1, 4th byte is length
         * if UART_ID_LENGTH = 0, 3th byte is length
         * */
        case (2+UART_ID_LENGTH):
            data_length = temp;
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into UartReceiveGroup */
        UartReceiveGroup[num] = temp;
        num++;
        if(num>=(data_length + 3 + UART_ID_LENGTH))
        {
            /* Returns true when the group is fully received */
            flag1 =false;
            num = 0;
            return true;
        }
    }
    return false;
}

bool processUartRxMsg(uint8_t *UartReceiveGroup, Custom_Element *UART_RX_Element)
{
    /* The format of UartReceiveGroup:
     * if UART_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if UART_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if UART_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get origin id, indicating the origin of the message */
    /* UART does not have id, set default value */
    UART_RX_Element->origin_id = DEFAULT_UART_ORIGIN_ID;
    /* Get Destination id, indicating the destination of the message */
    /* if UART_ID_LENGTH = 4, id = UartReceiveGroup[2]<<24 + UartReceiveGroup[3]<<16 +
     * UartReceiveGroup[4]<<8 + UartReceiveGroup[5]
     * if UART_ID_LENGTH = 1, id = UartReceiveGroup[2]
     * if UART_ID_LENGTH = 0, id = default value;
     * */
    if(UART_ID_LENGTH)
    {
        id = 0;
        for(count=0;count<UART_ID_LENGTH;count++)
        {
            id += UartReceiveGroup[count+2]<<((UART_ID_LENGTH-1-count)<<3);
        }
    }
    else
    {
        id = DEFAULT_UART_DESTINATION_ID;
    }
    UART_RX_Element->destination_id = id;

    /* Get data length */
    /* if UART_ID_LENGTH = 4, data length = UartReceiveGroup[6]
     * if UART_ID_LENGTH = 1, data length = UartReceiveGroup[3]
     * if UART_ID_LENGTH = 0, data length = UartReceiveGroup[2]
     * */
    UART_RX_Element->dlc = UartReceiveGroup[UART_ID_LENGTH + 2];
    /* Get data */
    /* if UART_ID_LENGTH = 4, data start from UartReceiveGroup[7]
     * if UART_ID_LENGTH = 1, data start from UartReceiveGroup[4]
     * if UART_ID_LENGTH = 0, data start from UartReceiveGroup[3]
     * */
    for(count=0;count<(UART_RX_Element->dlc);count++)
    {
        UART_RX_Element->data[count] = UartReceiveGroup[UART_ID_LENGTH + 3 +count];
    }
    return true;
}


bool sendUartTxMsg(UART_Regs *uart, uint8_t *UartTransmitGroup, uint16_t length)
{
    static uint16_t UartTransmitCount;
    /* Put one byte from UartTransmitGroup to UART FIFO */
    DL_UART_transmitData(uart, UartTransmitGroup[UartTransmitCount]);

    /* Check if all the bytes have been sent */
    UartTransmitCount++;
    if(UartTransmitCount >= length)
    {
        gUartTxflag = false;
        UartTransmitCount = 0;
    }
    return true;
}

bool processUartTxMsg(uint8_t *UartTransmitGroup, Custom_Element *UART_TX_Element)
{
    /* The format of UartTransmitGroup:
     * if UART_ID_LENGTH = 4, format is 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * if UART_ID_LENGTH = 1, format is 55 AA ID Length Data1 Data2 ...
     * if UART_ID_LENGTH = 0, format is 55 AA Length Data1 Data2 ...
     * */
    uint16_t count;

    /* Set Destination id*/
    /* UART does not have id, no action with Destination id*/

    /* Fill in header*/
    UartTransmitGroup[0] = 0x55;
    UartTransmitGroup[1] = 0xAA;

    /* Fill in ID area*/
    /* In this demo, ID area is filled with origin id to indicate the origin of message
     * Users can modify the ID area as needed */
    /* if UART_ID_LENGTH = 4,
     * UartTransmitGroup[2] = (uint8_t)((UART_TX_Element->id >> 24) & 0x000000ff);
     * UartTransmitGroup[3] = (uint8_t)((UART_TX_Element->id >> 16) & 0x000000ff);
     * UartTransmitGroup[4] = (uint8_t)((UART_TX_Element->id >> 8) & 0x000000ff);
     * UartTransmitGroup[5] = (uint8_t)(UART_TX_Element->id & 0x000000ff);
     * if UART_ID_LENGTH = 1,
     * UartTransmitGroup[2] = (uint8_t)(UART_TX_Element->id & 0x000000ff);
     * if UART_ID_LENGTH = 0, no id area;
     * */
    if(UART_ID_LENGTH)
    {
        for(count=0;count<UART_ID_LENGTH;count++)
        {
            UartTransmitGroup[count+2]= (uint8_t)((UART_TX_Element->origin_id >> ((UART_ID_LENGTH-1-count)<<3)) &0x000000ff);
        }
    }
    /* Fill in length area */
    /* if UART_ID_LENGTH = 4, data length is UartTransmitGroup[6]
     * if UART_ID_LENGTH = 1, data length is UartTransmitGroup[3]
     * if UART_ID_LENGTH = 0, data length is UartTransmitGroup[2]
     * */
    UartTransmitGroup[UART_ID_LENGTH + 2] = UART_TX_Element->dlc;
    /* Fill in data area */
    /* if UART_ID_LENGTH = 4, data start from UartTransmitGroup[7]
     * if UART_ID_LENGTH = 1, data start from UartTransmitGroup[4]
     * if UART_ID_LENGTH = 0, data start from UartTransmitGroup[3]
     * */
    for(count=0; count < UART_TX_Element->dlc; count++)
    {
        UartTransmitGroup[UART_ID_LENGTH + 3 + count] =  UART_TX_Element->data[count];
    }
    return true;
}


void transmitPacketBlocking(UART_Regs *uart, uint8_t *buffer, uint32_t size)
{
    uint32_t i;
    for (i = 0; i < size; i++) {
        /*
         * The transmitter will automatically check that the CTS signal is
         * asserted (low) before transmitting the next byte. If CTS is
         * de-asserted (high), the byte will not be transmitted out of the TX
         * FIFO. Therefore block if the TX FIFO is full before attempting
         * to fill it.
         */
        DL_UART_Main_transmitDataBlocking(uart, buffer[i]);
    }
}

uint16_t gGetUartRxMsg_Count;
bool getUartRxMsg_transparent(UART_Regs *uart, uint8_t *UartReceiveGroup)
{
    uint8_t temp;
    temp = DL_UART_Main_receiveData(uart);
    if(gGetUartRxMsg_Count < UART_RX_SIZE)
    {
        /* Log data into UartReceiveGroup */
        UartReceiveGroup[gGetUartRxMsg_Count] = temp;
        gGetUartRxMsg_Count++;
        return false;
    }
    else
    {
        /* Returns true when the group is full */
        return true;
    }
}

bool processUartRxMsg_transparent(uint8_t *UartReceiveGroup, Custom_Element *UART_RX_Element)
{
    /* The format of Uart:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;

    /* Get origin id, indicating the origin of the message */
    /* UART does not have id, set default value*/
    UART_RX_Element->origin_id = DEFAULT_UART_ORIGIN_ID;
    /* Get Destination id, indicating the destination of the message */
    /* No Destination id in Transparent transmission, id = default value;
     * */
    UART_RX_Element->destination_id = DEFAULT_UART_DESTINATION_ID;

    /* Get data length */
    UART_RX_Element->dlc = gGetUartRxMsg_Count;
    /* Get data */
    for(count=0;count<(UART_RX_Element->dlc);count++)
    {
        UART_RX_Element->data[count] = UartReceiveGroup[count];
    }
    return true;
}

bool processUartTxMsg_transparent(uint8_t *UartTransmitGroup, Custom_Element *UART_TX_Element)
{
    /* The format of Uart:
         * Transparent transmission - Data1 Data2 ...*/
    uint16_t count;

    /* Fill in data area */
    for(count=0; count < UART_TX_Element->dlc; count++)
    {
        UartTransmitGroup[count] =  UART_TX_Element->data[count];
    }
    return true;
}
