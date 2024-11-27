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
#include "drv8323_spi_hal.h"
#include "main.h"

#ifdef DRV8323

/*! @brief Flag to represent spi is busy */
volatile static _Bool spiTransactionInProgress = FALSE;

void HAL_writeSPI16(uint16_t data)
{
    /* clear the RX buffer  */
    while (!DL_SPI_isRXFIFOEmpty(DRV8323_SPI_INST))
    {
        DL_SPI_receiveData16(DRV8323_SPI_INST);
    }

    /* DL_GPIO_clearPins(DRV8323_SPI_NSCS_PORT, DRV8323_SPI_NSCS_PIN); */ /* To be Used in 3 wire SPI mode */
    DL_SPI_transmitData16(DRV8323_SPI_INST, data);
    while (DL_SPI_isBusy(DRV8323_SPI_INST))
    {
        /* This is expected to be empty*/
    };
    /* DL_GPIO_setPins(DRV8323_SPI_NSCS_PORT, DRV8323_SPI_NSCS_PIN); */ /* To be used in 3 wire SPI mode */
}

uint16_t HAL_readSPI16(void)
{
    while (DL_SPI_isBusy(DRV8323_SPI_INST))
    {
        /* This is expected to be empty*/
    }
    return DL_SPI_receiveData16(DRV8323_SPI_INST);
}

uint16_t drv8323SPIRead(volatile uint16_t prevReadVal, volatile int16_t regAddr)
{
    uint16_t readVal = prevReadVal;

    if(!spiTransactionInProgress)
    {
        spiTransactionInProgress = TRUE;

        volatile uint16_t data, ctrlWord;

        /* While reading the Read/write bit is set to 1 */
        regAddr = ((regAddr << 3) & 0x078) | (1 << 7);

        ctrlWord = regAddr << 8;
        HAL_writeSPI16(ctrlWord);

        data = HAL_readSPI16();

        /* DRV10x with a 10 bit register width data */
        readVal = data & 0x07FF;

        spiTransactionInProgress = FALSE;
    }

    return readVal;
}

void drv8323SPIWrite(int16_t regAddr, uint16_t data)
{

    if(!spiTransactionInProgress)
    {
        spiTransactionInProgress = TRUE;

        uint16_t
        ctrlWord,
        data_MSB ,
        data_LSB;

        /* Shift the register address by 3 bits to place it in
         * 14:11 position of MSB */
        regAddr = ((regAddr << 3) & 0x078);

        /* Extract MSB and LSB content of Hex value */
        data_LSB = (data & 0x00FF);
        data_MSB = ((data >> 8) & 0x07) | regAddr;

        ctrlWord = data_MSB << 8 | data_LSB;
        HAL_writeSPI16(ctrlWord);

        spiTransactionInProgress = FALSE;
    }
}

#endif
