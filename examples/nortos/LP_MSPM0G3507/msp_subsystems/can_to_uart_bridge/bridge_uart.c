#include "ti_msp_dl_config.h"
#include "user_define.h"

Custom_Element gUART_RX_Element;
Custom_Element gUART_TX_Element;
uint8_t gUartReceiveGroup[UART_RX_SIZE];
uint8_t gUartTransmitGroup[UART_TX_SIZE];
bool gUartTxflag;

bool getUartRxMsg(uint8_t *UartReceiveGroup)
{
    static uint16_t num, data_length;
    static bool flag1;
    uint8_t temp;
    temp = DL_UART_Main_receiveData(UART_0_INST);
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
        case 6:
            data_length = temp; //7th byte is length
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into UartReceiveGroup */
        UartReceiveGroup[num] = temp;
        num++;
        if(num>=(data_length+7))
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
    /* The format of UartReceiveGroup is
     * 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * */
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get device address */
    id = UartReceiveGroup[2]<<24;
    id += UartReceiveGroup[3]<<16;
    id += UartReceiveGroup[4]<<8;
    id += UartReceiveGroup[5];
    UART_RX_Element->id = id;
    /* Get data length */
    UART_RX_Element->dlc = UartReceiveGroup[6];
    /* Get data */
    for(count=0;count<(UART_RX_Element->dlc);count++)
    {
        UART_RX_Element->data[count] = UartReceiveGroup[7+count];
    }
    return true;
}

bool sendUartTxMsg(uint8_t *UartTransmitGroup, uint16_t length)
{
    static uint16_t UartTransmitCount;
    /* Put one byte from UartTransmitGroup to UART FIFO */
    DL_UART_transmitData(UART_0_INST, UartTransmitGroup[UartTransmitCount]);

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
    /* The format of UartTransmitGroup is
     * 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * */

    /* Fill in header*/
    gUartTransmitGroup[0] = 0x55;
    gUartTransmitGroup[1] = 0xAA;
    /* Fill in ID area*/
    UartTransmitGroup[2] = (uint8_t)((UART_TX_Element->id >> 24) & 0x000000ff);
    UartTransmitGroup[3] = (uint8_t)((UART_TX_Element->id >> 16) & 0x000000ff);
    UartTransmitGroup[4] = (uint8_t)((UART_TX_Element->id >> 8) & 0x000000ff);
    UartTransmitGroup[5] = (uint8_t)(UART_TX_Element->id & 0x000000ff);
    /* Fill in length area */
    UartTransmitGroup[6] = (uint8_t)(UART_TX_Element->dlc);
    /* Fill in data area */
    for(uint16_t num = 0; num < UART_TX_Element->dlc; num++)
    {
        UartTransmitGroup[7+num] =  UART_TX_Element->data[num];
    }
    return true;
}

void transmitPacketBlocking(uint8_t *buffer, uint32_t size)
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
        DL_UART_Main_transmitDataBlocking(UART_0_INST, buffer[i]);
    }
}


