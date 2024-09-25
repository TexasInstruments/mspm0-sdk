#include "ti_msp_dl_config.h"
#include "user_define.h"

#define I2C_TARGET_ADDRESS (0x48)

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
        case 6:
            data_length = temp; //7th byte is length
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into SpiReceiveGroup */
        I2CReceiveGroup[num] = temp;
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
        case 6:
            data_length = temp; //7th byte is length
            break;
        default:
            break;
    }
    if(flag1 == true)
    {
        /* After detecting '0x55 0xAA', log data into SpiReceiveGroup */
        I2CReceiveGroup[num] = temp;
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

bool processI2CRxMsg(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element)
{
    uint16_t count;
    uint32_t id;
    uint8_t length;

    /* Get device address */
    id = I2CReceiveGroup[2]<<24;
    id += I2CReceiveGroup[3]<<16;
    id += I2CReceiveGroup[4]<<8;
    id += I2CReceiveGroup[5];
    I2C_RX_Element->id = id;
    /* Get data length */
    I2C_RX_Element->dlc = I2CReceiveGroup[6];
    /* Get data */
    for(count=0;count<(I2C_RX_Element->dlc);count++)
    {
        I2C_RX_Element->data[count] = I2CReceiveGroup[7+count];
    }
    return true;
}

/*
 * controller connect to target and write bytes
 */
bool sendI2CTxMsg_controller(uint8_t *I2CTransmitGroup)
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
        I2C_INST, I2C_TARGET_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, gTxLen);
    return true;
}

/*
 * controller connect to target and read bytes
 */
bool readI2CRxMsg_controller(uint16_t length)
{
     /* Send a read request to Target */
     while (!(
         DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
         ;
     DL_I2C_startControllerTransfer(
         I2C_INST, I2C_TARGET_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_RX, length);
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

bool processI2CTxMsg(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element)
{
    /* The format of I2CTransmitGroup is
     * 55 AA ID1 ID2 ID3 ID4 Length Data1 Data2 ...
     * */
    /* Fill in header */
    I2CTransmitGroup[0] = 0x55;
    I2CTransmitGroup[1] = 0xAA;
    /* Fill in ID area */
    I2CTransmitGroup[2] = (uint8_t)((I2C_TX_Element->id >> 24) & 0x000000ff);
    I2CTransmitGroup[3] = (uint8_t)((I2C_TX_Element->id >> 16) & 0x000000ff);
    I2CTransmitGroup[4] = (uint8_t)((I2C_TX_Element->id >> 8) & 0x000000ff);
    I2CTransmitGroup[5] = (uint8_t)(I2C_TX_Element->id & 0x000000ff);
    /* Fill in data length */
    I2CTransmitGroup[6] = I2C_TX_Element->dlc;
    /* Fill in data area */
    for(uint16_t num = 0; num < I2C_TX_Element->dlc; num++)
    {
        I2CTransmitGroup[7+num] =  I2C_TX_Element->data[num];
    }
    return true;
}
