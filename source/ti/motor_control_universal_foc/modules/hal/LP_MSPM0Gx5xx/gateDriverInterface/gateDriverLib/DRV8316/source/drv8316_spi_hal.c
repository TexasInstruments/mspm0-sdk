/*
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
 */

#include "drv8316_spi_hal.h"
#include "main.h"
#include "gateDriver.h"

#if defined DRV8316 || defined DRV8316_DRIVER

/*! @brief Flag to represent spi is busy */
volatile static _Bool spiTransactionInProgress = FALSE;

_Bool findParity(int32_t x)
{
    _Bool parity;

    uint32_t y = x ^ (x >> 1);
    y = y ^ (y >> 2);
    y = y ^ (y >> 4);
    y = y ^ (y >> 8);
    y = y ^ (y >> 16);

    /* Rightmost bit of y holds the parity value */
    /* if (y&1) is 1 then parity is odd else even */
    if (y & 1)
    {
        parity = 1;
    }
    else
    {
        parity = 0;
    }
    return parity;
}

void HAL_writeSPI16(uint16_t data)
{
    /* clear the RX buffer  */
    while (!DL_SPI_isRXFIFOEmpty(DRV8316_SPI_INST))
    {
        DL_SPI_receiveData16(DRV8316_SPI_INST);
    }

    DL_GPIO_clearPins(DRV8316_SPI_NSCS_PORT, DRV8316_SPI_NSCS_PIN);
    DL_SPI_transmitData16(DRV8316_SPI_INST, data);
    while (DL_SPI_isBusy(DRV8316_SPI_INST))
    {
        /* This is expected to be empty*/
    };
    DL_GPIO_setPins(DRV8316_SPI_NSCS_PORT, DRV8316_SPI_NSCS_PIN);
}

uint16_t HAL_readSPI16(void)
{
    while (DL_SPI_isBusy(DRV8316_SPI_INST))
    {
        /* This is expected to be empty*/
    }
    return DL_SPI_receiveData16(DRV8316_SPI_INST);
}


uint16_t drv8316SPIRead(volatile uint16_t prevReadVal, volatile int16_t regAddr)
{
    uint16_t readVal = prevReadVal;

    if(!spiTransactionInProgress)
    {
        spiTransactionInProgress = TRUE;

        volatile uint16_t data, ctrlWord;
        volatile uint16_t parityVal = 0x00;

        /* In DRV8316 the 6bit address is located at bit position 14:9 */
        /* While reading the Read/write bit is set to 1, hence set for Odd parity of address */
        regAddr = ((regAddr << 1) & 0x07E) | (1 << 7);

        /* While Writing the Read/write bit is set to 1, hence Compute for Even parity of Address + R/W bit */
        parityVal =  findParity(regAddr <<8);

        regAddr = regAddr | (parityVal & (0x01));

        ctrlWord = regAddr << 8;
        HAL_writeSPI16(ctrlWord);

        data = HAL_readSPI16();

        /* DRV8316 with a 8 bit register width data and Status bits from Location 8- 15*/
        readVal = data & 0x0FF;

        spiTransactionInProgress = FALSE;
    }
    return readVal;
}

void drv8316SPIWrite(int16_t regAddr, uint16_t data)
{
    if(!spiTransactionInProgress)
    {
        spiTransactionInProgress = TRUE;

        uint16_t
        ctrlWord,
        data_MSB,
        data_LSB,
        parityVal;

        /* In DRV8316 the 6bit address is located at bit position 14:9 */

        regAddr = ((regAddr << 1) & 0x07E);

        /* While Writing the Read/write bit is set to 0, hence Compute for Even parity of address + data */
        parityVal =  findParity( (regAddr <<8) | data);

        /* create the MSB and LSB content of Hex value */
        data_LSB = (data & 0x00FF);
        data_MSB =  regAddr | (parityVal & (0x01));

        ctrlWord = data_MSB << 8 | data_LSB;
        HAL_writeSPI16(ctrlWord);

        spiTransactionInProgress = FALSE;
    }
}

#endif
