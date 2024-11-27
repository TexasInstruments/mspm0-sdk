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

#include <expander.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"


extern volatile tDataType ioExpander;
uint8_t gTxCount;

//System Init
void systemInit(void)
{
    //GPIO init
    DL_GPIO_init(GPIO_GRP_0_PIN_0_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_1_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_2_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_3_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_4_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_5_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_6_IOMUX);
    DL_GPIO_init(GPIO_GRP_0_PIN_7_IOMUX);

    //Interrupt init
#ifdef UART
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
#endif
#ifdef I2C
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
#endif
#ifdef SPI
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
#endif

}

// GPIO Init
void DL_GPIO_init(uint32_t pincmIndex)
{
    IOMUX->SECCFG.PINCM[pincmIndex] =
        (IOMUX_PINCM_INENA_ENABLE | IOMUX_PINCM_PC_CONNECTED | ((uint32_t) 0x00000001));
}

// GPIO direction
void gpioDirectionSet(void)
{
    uint8_t gpiodirection;

    gpiodirection = ioExpander.rxDataPkt[2];

    if(gpiodirection & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }

    if((gpiodirection >> 1) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }

    if((gpiodirection >> 2) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }

    if((gpiodirection >> 3) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }

    if((gpiodirection >> 4) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }

    if((gpiodirection >> 5) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }

    if((gpiodirection >> 6) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }

    if((gpiodirection >> 7) & 0x1)
    {
        DL_GPIO_enableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
    else
    {
        DL_GPIO_disableOutput(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
}

// GPIO output level
void gpioOutputCtl(void)
{
    uint8_t gpiostate;
    gpiostate = ioExpander.rxDataPkt[2];

    if(gpiostate & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
    }

    if((gpiostate >> 1) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_1_PIN);
    }

    if((gpiostate >> 2) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_2_PIN);
    }

    if((gpiostate >> 3) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_3_PIN);
    }

    if((gpiostate >> 4) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_4_PIN);
    }

    if((gpiostate >> 5) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_5_PIN);
    }

    if((gpiostate >> 6) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_6_PIN);
    }

    if((gpiostate >> 7) & 0x1)
    {
        DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_7_PIN);
    }
}

// GPIO Status read
uint8_t DL_GPIO_readPinStatus(uint32_t pins)
{
    if(DL_GPIO_readPins(GPIO_GRP_0_PORT,pins))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// detect GPIO input voltage level
void gpioStateRead(void)
{
    uint8_t gpioState_0, gpioState_1, gpioState_2,gpioState_3, gpioState_4, gpioState_5, gpioState_6,gpioState_7;
    uint8_t u8ClearBuff[4]={0};
    gpioState_0 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_0_PIN);
    gpioState_1 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_1_PIN);
    gpioState_2 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_2_PIN);
    gpioState_3 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_3_PIN);
    gpioState_4 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_4_PIN);
    gpioState_5 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_5_PIN);
    gpioState_6 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_6_PIN);
    gpioState_7 = DL_GPIO_readPinStatus(GPIO_GRP_0_PIN_7_PIN);

    ioExpander.txDataPkt[0]=0x5A;
    ioExpander.txDataPkt[1]=eGpioInp;
    ioExpander.txDataPkt[2]=((gpioState_7)<<7)+((gpioState_6)<<6)+((gpioState_5)<<5)+((gpioState_4)<<4)+((gpioState_3)<<3)+((gpioState_2)<<2)+((gpioState_1)<<1)+gpioState_0;
    ioExpander.txDataPkt[3] = checkSum(ioExpander.txDataPkt,3);
    ioExpander.txPktStatus = eTrue;

    if(ioExpander.commType == eSPI)
    {
        //Save the first byte into TX buffer
        DL_SPI_transmitData8(SPI_0_INST, ioExpander.txDataPkt[gTxCount]);
        //Wait SPI finish and Clear SPI TX FIFO
        while(DL_SPI_isBusy(SPI_0_INST));
        DL_SPI_fillTXFIFO8(SPI_0_INST, u8ClearBuff,4);
    }

    if(ioExpander.commType == eUART)
    {
        uartTxData(ioExpander.txDataPkt,4);
    }
}

uint8_t checkSum(volatile uint8_t* data, int len)
{
    uint16_t checksum = 0;
    for (int i = 0; i < len; i++) {
        checksum += data[i];
    }
    return checksum;
}

void receivePacket(uint8_t u8Data)
{
    switch (ioExpander.prtclType)
    {
        case eNone:
            if (u8Data == 0x5a)
            {
                ioExpander.rxDataPkt[0] = u8Data;
                ioExpander.u8RTCheckSum = 0x5a;
                ioExpander.prtclType = eCommd;
            }
            break;
        case eCommd:
            ioExpander.rxDataPkt[1] = u8Data;
            ioExpander.u8RTCheckSum += u8Data;
            ioExpander.prtclType = eData;
            break;
        case eData:
            ioExpander.rxDataPkt[2] = u8Data;
            ioExpander.u8RTCheckSum += u8Data;
            ioExpander.prtclType = eCheck;
            break;
        case eCheck:
            ioExpander.rxDataPkt[3] = u8Data;
            if(u8Data == ioExpander.u8RTCheckSum)
            {
                ioExpander.pktTyp = ioExpander.rxDataPkt[1];
            }
            else
            {
                ioExpander.pktTyp = eNoPkt;
            }
            ioExpander.prtclType = eNone;
            break;
        default:
            break;
     }
}

//UART TX
void uartTxData(volatile uint8_t* u8Data,uint16_t Len)
{
    uint16_t u16Cnt;

    for(u16Cnt=0; u16Cnt<Len; u16Cnt++)
    {
        DL_UART_Main_transmitData(UART_0_INST, u8Data[u16Cnt]);
        while (DL_UART_isBusy(UART_0_INST));
    }

    ioExpander.txPktStatus = eFalse;
}

//I2C INT
void I2C_INST_IRQHandler(void)
{
    static bool dataRx = false;

    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            /* Initialize RX or TX after Start condition is received */
            gTxCount = 0;
            /* Flush TX FIFO to refill it */
            DL_I2C_flushTargetTXFIFO(I2C_INST);
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true)
            {
                receivePacket(DL_I2C_receiveTargetData(I2C_INST));
            }
            ioExpander.commType = eI2C;
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_EMPTY:
            /* Fill TX FIFO if there are more bytes to send */
            if(ioExpander.txPktStatus == eTrue)
            {
                if (gTxCount < TX_PACKET_LEN)
                {
                    gTxCount += DL_I2C_fillTargetTXFIFO(
                        I2C_INST, (uint8_t *)&ioExpander.txDataPkt[gTxCount], (TX_PACKET_LEN - gTxCount));
                }

                if (gTxCount == TX_PACKET_LEN)
                {
                    ioExpander.txPktStatus = eFalse;
                }
            }
            break;
        default:
            break;
    }
}

//UART INT
void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            receivePacket(DL_UART_Main_receiveDataBlocking(UART_0_INST));
            ioExpander.commType = eUART;
            break;
        default:
            break;
    }
}

//SPI INT
void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
            if(ioExpander.txPktStatus == eFalse)
            {
                while (DL_SPI_isRXFIFOEmpty(SPI_0_INST) != true)
                {
                    receivePacket(DL_SPI_receiveData8(SPI_0_INST));
                }
                ioExpander.commType = eSPI;
            }
            else
            {
                DL_SPI_receiveData8(SPI_0_INST);
                gTxCount++;
                DL_SPI_transmitData8(SPI_0_INST, ioExpander.txDataPkt[gTxCount]);
                if(gTxCount>=(TX_PACKET_LEN-1))
                {
                    gTxCount = 0;
                    ioExpander.txPktStatus = eFalse;
                }
            }
            break;
        default:
            break;
    }
}
