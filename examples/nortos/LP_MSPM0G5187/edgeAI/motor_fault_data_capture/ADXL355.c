/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include "ADXL355.h"

/** \brief Variables to hold latest X, Y, Z axis data from sensor */
int32_t xData, yData, zData;

/**
 * @brief Write one byte to sensor register via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @param data Data byte
 */
void SPI_writeByte(sensorStruct sensor, uint8_t addr, uint8_t data)
{
    uint8_t dataTx[2];

dataTx[0] = (addr << 1) | ADXL355_SPI_WRITE_BIT;
    dataTx[1] = data;

    DL_GPIO_clearPins(sensor.csPort, sensor.csPin);

    DL_SPI_fillTXFIFO8(sensor.spiReg, dataTx, 2);
    while (DL_SPI_isBusy(sensor.spiReg));

    DL_SPI_drainRXFIFO8(sensor.spiReg, dataTx, 2);

    DL_GPIO_setPins(sensor.csPort, sensor.csPin);
}

/**
 * @brief Read one byte from sensor register via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @return Data byte read from sensor
 */
uint8_t SPI_readByte(sensorStruct sensor, uint8_t addr)
{
    uint8_t dataTx[2];
    uint8_t dataRx[2];
    uint8_t dummy[4];

    dataTx[0] = (addr << 1) | ADXL355_SPI_READ_BIT;
    dataTx[1] = 0x00;

    DL_SPI_drainRXFIFO8(sensor.spiReg, dummy, 4);

    DL_GPIO_clearPins(sensor.csPort, sensor.csPin);

    DL_SPI_fillTXFIFO8(sensor.spiReg, dataTx, 2);
    while (DL_SPI_isBusy(sensor.spiReg));

    dataRx[0] = DL_SPI_receiveDataBlocking8(sensor.spiReg);
    dataRx[1] = DL_SPI_receiveDataBlocking8(sensor.spiReg);

    DL_GPIO_setPins(sensor.csPort, sensor.csPin);

    return dataRx[1];
}

/**
 * @brief Write multiple bytes to sensor via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @param dataTx Pointer to data buffer to send
 * @param length Number of bytes to send
 */
void SPI_write(sensorStruct sensor, uint8_t addr, uint8_t *dataTx, uint8_t length)
{
    uint8_t addrByte = (addr << 1) | ADXL355_SPI_WRITE_BIT;

    DL_GPIO_clearPins(sensor.csPort, sensor.csPin);

    DL_SPI_fillTXFIFO8(sensor.spiReg, &addrByte, 1);
    while (DL_SPI_isBusy(sensor.spiReg));

    DL_SPI_receiveDataBlocking8(sensor.spiReg);

    while(length > 0)
    {
        DL_SPI_fillTXFIFO8(sensor.spiReg, dataTx, 1);
        while (DL_SPI_isBusy(sensor.spiReg));

        DL_SPI_receiveDataBlocking8(sensor.spiReg);

        dataTx++;
        length--;
    }

    DL_GPIO_setPins(sensor.csPort, sensor.csPin);
}

/**
 * @brief Read multiple bytes from sensor via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @param dataRx Pointer to buffer to store received data
 * @param length Number of bytes to read
 */
void SPI_read(sensorStruct sensor, uint8_t addr, uint8_t *dataRx, uint8_t length)
{
    uint8_t dataTx = (addr << 1) | ADXL355_SPI_READ_BIT;
    uint8_t dummy[4];

    DL_SPI_drainRXFIFO8(sensor.spiReg, dummy, 4);

    DL_GPIO_clearPins(sensor.csPort, sensor.csPin);

    DL_SPI_fillTXFIFO8(sensor.spiReg, &dataTx, 1);
    while (DL_SPI_isBusy(sensor.spiReg));

    DL_SPI_receiveDataBlocking8(sensor.spiReg);

    while(length != 0)
    {
        uint8_t dummy = 0x00;
        DL_SPI_fillTXFIFO8(sensor.spiReg, &dummy, 1);
        while (DL_SPI_isBusy(sensor.spiReg));

        *dataRx++ = DL_SPI_receiveDataBlocking8(sensor.spiReg);

        length--;
    }

    DL_GPIO_setPins(sensor.csPort, sensor.csPin);
}

/**
 * @brief Configure the ADXL355 sensor
 * @param sensor Sensor configuration
 */
void ADXL355_config(sensorStruct sensor)
{
    SPI_writeByte(sensor, ADXL355_RESET_ADDRESS, ADXL355_RESET_CODE);

    SPI_writeByte(sensor, ADXL355_POWER_CTL_ADDRESS, ADXL355_POWER_CTL_MEASURE_MODE);

    SPI_writeByte(sensor, ADXL355_RANGE_ADDRESS, ADXL355_RANGE_8G);
}

/**
 * @brief Disable data ready interrupt from the ADXL355 sensor
 * @param sensor Pointer to sensor configuration
 */
void ADXL355_disableInterrupts(sensorStruct *sensor)
{   
    sensor->sensorMode = SENSOR_MODE_IDLE;
    DL_GPIO_disableInterrupt(sensor->readyPort, sensor->readyPin);
    DL_GPIO_clearInterruptStatus(sensor->readyPort, sensor->readyPin);
}

/**
 * @brief Enable data ready interrupt from the ADXL355 sensor
 * @param sensor Pointer to sensor configuration
 */
void ADXL355_enableInterrupts(sensorStruct *sensor)
{
    sensor->sensorMode = SENSOR_MODE_COLLECT_DATA;
    DL_GPIO_clearInterruptStatus(sensor->readyPort, sensor->readyPin);
    DL_GPIO_enableInterrupt(sensor->readyPort, sensor->readyPin);
}

/**
 * @brief Read acceleration data from the ADXL355 sensor
 * @param sensor Sensor configuration
 */
void ADXL355_getData(sensorStruct sensor)
{
    uint8_t dataRx[9];

    SPI_read(sensor, ADXL355_XDATA3_ADDRESS, dataRx, 9);

    xData = (dataRx[0] << 12) | (dataRx[1] << 4) | (dataRx[2] >> 4);
    yData = (dataRx[3] << 12) | (dataRx[4] << 4) | (dataRx[5] >> 4);
    zData = (dataRx[6] << 12) | (dataRx[7] << 4) | (dataRx[8] >> 4);

    if(xData & 0x80000)
    {
        xData |= 0xFFF00000;
    }
    if(yData & 0x80000)
    {
        yData |= 0xFFF00000;
    }
    if(zData & 0x80000)
    {
        zData |= 0xFFF00000;
    }
    xData = (xData >> 4);
    yData = (yData >> 4);
    zData = (zData >> 4);
}



