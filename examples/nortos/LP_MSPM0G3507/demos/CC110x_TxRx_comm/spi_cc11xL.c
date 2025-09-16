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
#include "spi_cc11xL.h"

#define MSPMO_SPI_BEGIN()   DL_GPIO_clearPins(GPIO_SPI_0_PORT, GPIO_SPI_0_CSn_PIN)
#define MSPMO_SPI_END()     DL_GPIO_setPins(GPIO_SPI_0_PORT, GPIO_SPI_0_CSn_PIN)

bool byteSent = false;


//-----------------------------------------------------------------------------
// mspm0SpiCmdStrobe (trxSpiCmdStrobe)
//
// Send command strobe to the radio. Returns status byte read during transfer
// of command strobe.
//
// Input Parameters
//
//  - cmd         - command strobe
//
// Return
//
//  - Status byte
//-----------------------------------------------------------------------------
rfStatus_t mspm0SpiCmdStrobe(uint8_t cmd)
{
    uint8_t status;

    // Pull CS_N low
    MSPMO_SPI_BEGIN();

    // Wait for CHIP_RDYn (MISO/POCI going low)

    // Set POCI to be a GPIO to be able to read it
    DL_GPIO_initDigitalInputFeatures(GPIO_SPI_0_IOMUX_POCI, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    while(DL_GPIO_readPins(GPIO_SPI_0_POCI_PORT, GPIO_SPI_0_POCI_PIN) & GPIO_SPI_0_POCI_PIN);

    // Set it back to being an SPI peripheral
    DL_GPIO_initPeripheralInputFunction(GPIO_SPI_0_IOMUX_POCI, GPIO_SPI_0_IOMUX_POCI_FUNC);

    // Transmit the strobe command
    DL_SPI_transmitData8(SPI_0_INST, cmd);

    // Go to sleep until the byte is sent
    while (!byteSent)
    {
        __WFI();
    }
    byteSent = false;

    status = DL_SPI_receiveData8(SPI_0_INST);

    // Pull CS_N high
    MSPMO_SPI_END();
    return((rfStatus_t)status);
}


//-----------------------------------------------------------------------------
// mspm0ReadWriteBurstSingle (trxReadWriteBurstSingle)
//
// When the address byte is sent to the SPI peripheral (slave), the next byte
// communicated is the data to be written or read. The address byte that holds
// information about read/write -and single/burst-access is provided to this
// function.
// Depending on these two bits this function will write len bytes to the radio
// in burst mode or read len bytes from the radio in burst mode if the burst
// bit is set. If the burst bit is not set, only one data byte is communicated.
//
// NOTE: This function is used in the following way:
//
// MSPMO_SPI_BEGIN();
// Wait for CHIP_RDYn (MISO/POCI going low)
// mspm0ReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len);
// MSPMO_SPI_END();
//
// Input Parameters
//
//  - None
//
// Return
//
//  - None
 //-----------------------------------------------------------------------------
static void mspm0ReadWriteBurstSingle(uint8_t addr,uint8_t *pData, uint16_t len)
{
    uint16_t i;
    uint8_t dummy;

    if(addr & RADIO_READ_ACCESS)
    {
        if(addr & RADIO_BURST_ACCESS)
        {
            for (i = 0; i < len; i++)
            {
                DL_SPI_transmitData8(SPI_0_INST, 0);
                while (!byteSent)
                {
                   __WFI();
                }
                byteSent = false;
                *pData = DL_SPI_receiveData8(SPI_0_INST);
                pData++;
            }
        }
        else // Read single access
        {
            DL_SPI_transmitData8(SPI_0_INST, 0);
            while (!byteSent)
            {
                __WFI();
            }
            byteSent = false;
            *pData = DL_SPI_receiveData8(SPI_0_INST);
        }
    }
    else // RADIO_WRITE_ACCESS
    {
        if(addr & RADIO_BURST_ACCESS)
        {
            for (i = 0; i < len; i++)
            {
                DL_SPI_transmitData8(SPI_0_INST, *pData);
                while (!byteSent)
                {
                    __WFI();
                }
                dummy = DL_SPI_receiveData8(SPI_0_INST);
                byteSent = false;
                pData++;
            }
        }
        else
        {
            DL_SPI_transmitData8(SPI_0_INST, *pData);
            while (!byteSent)
            {
                __WFI();
            }
            dummy = DL_SPI_receiveData8(SPI_0_INST);
            byteSent = false;
        }
    }
    return;
}


//-----------------------------------------------------------------------------
// mspm08BitRegAccess (trx8BitRegAccess)
//
// This function performs a read or write from/to a 8bit register address space.
// The function handles burst and single read/write as specified in addrByte. .
//
// Input Parameters
//
//  - accessType    - Specifies if this is a read or write and if it's a single
//                    or burst access. Bitmask made up of
//                    RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
//                    RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
//  - addrByte      - Address byte of register.
//  - pData         - data array
//  - len           - Length of array to be read(TX)/written(RX)
//
// Return
//
//  - Status Byte
 //-----------------------------------------------------------------------------
rfStatus_t mspm08BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len)
{
    uint8_t chipStatus;

    // Pull CS_N low
    MSPMO_SPI_BEGIN();

    // Wait for CHIP_RDYn (MISO/POCI going low)

    // Set POSI to be a GPIO to be able to read it
    DL_GPIO_initDigitalInputFeatures(GPIO_SPI_0_IOMUX_POCI, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    while(DL_GPIO_readPins(GPIO_SPI_0_POCI_PORT, GPIO_SPI_0_POCI_PIN) & GPIO_SPI_0_POCI_PIN);

    // Set it back to being a SPI peripheral
    DL_GPIO_initPeripheralInputFunction(GPIO_SPI_0_IOMUX_POCI, GPIO_SPI_0_IOMUX_POCI_FUNC);

    // Send register address byte
    DL_SPI_transmitData8(SPI_0_INST, (accessType | addrByte));

    while (!byteSent)
    {
        __WFI();
    }
    byteSent = false;

    // Storing chip status
    chipStatus = DL_SPI_receiveData8(SPI_0_INST);

    mspm0ReadWriteBurstSingle((accessType | addrByte), pData, len);

    // Pull CS_N high
    MSPMO_SPI_END();

    // return the status byte value
    return((rfStatus_t)chipStatus);
}


//-----------------------------------------------------------------------------
// cc11xLSpiReadReg
//
// Reads register(s). If len  = 1: one byte is read
//                    if len != 1: len bytes are read in burst mode
// Input Parameters
//
//  - addr   - address to read
//  - *pData - pointer to data array where read data is stored
//  - len    - numbers of bytes to read starting from addr
//
// Return
//
//  - Status byte
//-----------------------------------------------------------------------------
rfStatus_t cc11xLSpiReadReg(uint8_t addr, uint8_t *pData, uint8_t len)
{
    rfStatus_t status;
    status = mspm08BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS), addr, pData, len);
    return (status);
}


//-----------------------------------------------------------------------------
// cc11xLSpiWriteReg
//
// writes register(s). If len  = 1: one byte is written
//                     if len != 1: len bytes at *data is written in burst mode.
// Input Parameters
//
//  - addr   - register address to write
//  - *pData - pointer to data array that is written
//  - len    - number of bytes written
//
// Return
//
//  - Status byte
//-----------------------------------------------------------------------------
rfStatus_t cc11xLSpiWriteReg(uint8_t addr, uint8_t *pData, uint8_t len)
{
    rfStatus_t status;
    status = mspm08BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),addr, pData, len);
    return (status);
}


//-----------------------------------------------------------------------------
// cc11xLSpiWriteTxFifo
//
// Writes provided data to TX FIFO
//
// Input Parameters
//
//  - *pData - pointer to data array that is written to TX FIFO
//  - len    - Length of data array to be written
//
// Return
//
//  - Status byte
//-----------------------------------------------------------------------------
rfStatus_t cc11xLSpiWriteTxFifo(uint8_t *pData, uint8_t len)
{
    rfStatus_t status;
    status = mspm08BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),CC11xL_FIFO, pData, len);
    return (status);
}


//-----------------------------------------------------------------------------
// cc11xLSpiReadRxFifo
//
// Reads RX FIFO values to pData array
//
// Input Parameters
//
//  - *pData - pointer to data array where RX FIFO bytes are saved
//  - len    - number of bytes to read from the RX FIFO
//
// Return
//
//  - Status byte
//-----------------------------------------------------------------------------
rfStatus_t cc11xLSpiReadRxFifo(uint8_t *pData, uint8_t len)
{
    rfStatus_t status;
    status = mspm08BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),CC11xL_FIFO, pData, len);
    return (status);
}


//-----------------------------------------------------------------------------
// SPI_0_INST_IRQHandler
//
// ISR called when SPI has finished transfers and changed into IDLE mode
//-----------------------------------------------------------------------------
void SPI_0_INST_IRQHandler(void)
{
    if(DL_SPI_getPendingInterrupt(SPI_0_INST) == DL_SPI_IIDX_IDLE)
    {
        byteSent = true;
    }
}


