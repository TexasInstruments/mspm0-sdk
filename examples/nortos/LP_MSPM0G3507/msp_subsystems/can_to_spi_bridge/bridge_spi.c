#include "ti_msp_dl_config.h"
#include "user_define.h"

Custom_Element gSPI_RX_Element;
Custom_Element gSPI_TX_Element;
uint8_t gSpiReceiveGroup[SPI_RX_SIZE];
uint8_t gSpiTransmitGroup[SPI_TX_SIZE];
bool gSpiTxflag;

bool getSpiRxMsg(uint8_t *SpiReceiveGroup)
{
    static uint16_t num, data_length;
    static bool flag1;
    uint8_t temp;
    temp = DL_SPI_receiveData8(SPI_0_INST);
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
        /* After detecting '0x55 0xAA', log data into SpiReceiveGroup */
        SpiReceiveGroup[num] = temp;
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

bool processSpiRxMsg(uint8_t *SpiReceiveGroup, Custom_Element *SPI_RX_Element)
{
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get device address */
    id = SpiReceiveGroup[2]<<24;
    id += SpiReceiveGroup[3]<<16;
    id += SpiReceiveGroup[4]<<8;
    id += SpiReceiveGroup[5];
    SPI_RX_Element->id = id;
    /* Get data length */
    SPI_RX_Element->dlc = SpiReceiveGroup[6];
    /* Get data */
    for(count=0;count<(SPI_RX_Element->dlc);count++)
    {
        SPI_RX_Element->data[count] = SpiReceiveGroup[7+count];
    }
    return true;
}

bool sendSpiTxMsg(uint8_t *SpiTransmitGroup, uint16_t length)
{
    static uint16_t SpiTransmitcount;

    if(false == DL_SPI_isTXFIFOFull(SPI_0_INST))
    {
        /* Put one byte from SpiTransmitGroup to SPI FIFO */
        DL_SPI_transmitData8(SPI_0_INST, SpiTransmitGroup[SpiTransmitcount]);
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
    /* The format of SpiTransmitGroup is
     * 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ... '\n'
     * */

    /* Fill in header */
    SpiTransmitGroup[0] = 0x55;
    SpiTransmitGroup[1] = 0xAA;
    /* Fill in ID area */
    SpiTransmitGroup[2] = (uint8_t)((SPI_TX_Element->id >> 24) & 0x000000ff);
    SpiTransmitGroup[3] = (uint8_t)((SPI_TX_Element->id >> 16) & 0x000000ff);
    SpiTransmitGroup[4] = (uint8_t)((SPI_TX_Element->id >> 8) & 0x000000ff);
    SpiTransmitGroup[5] = (uint8_t)(SPI_TX_Element->id & 0x000000ff);
    /* Fill in data length */
    SpiTransmitGroup[6] = SPI_TX_Element->dlc;
    /* Fill in data area */
    for(uint16_t num = 0; num < SPI_TX_Element->dlc; num++)
    {
        SpiTransmitGroup[7+num] =  SPI_TX_Element->data[num];
    }
    return true;
}
