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

#ifndef ADXL355_H_
#define ADXL355_H_

#include "ti_msp_dl_config.h"

// ADXL355 Register Addresses
/** \brief Device ID (AD) register address */
#define ADXL355_DEVID_AD_ADDRESS         0x00
/** \brief Device ID (MST) register address */
#define ADXL355_DEVID_MST_ADDRESS        0x01
/** \brief Part ID register address */
#define ADXL355_PARTID_ADDRESS           0x02
/** \brief Revision ID register address */
#define ADXL355_REVID_ADDRESS            0x03
/** \brief Status register address */
#define ADXL355_STATUS_ADDRESS           0x04
/** \brief FIFO entries register address */
#define ADXL355_FIFO_ENTRIES_ADDRESS     0x05
/** \brief Temperature MSB register address */
#define ADXL355_TEMP2_ADDRESS            0x06
/** \brief Temperature LSB register address */
#define ADXL355_TEMP1_ADDRESS            0x07
/** \brief X-axis data MSB register address */
#define ADXL355_XDATA3_ADDRESS           0x08
/** \brief X-axis data middle byte register address */
#define ADXL355_XDATA2_ADDRESS           0x09
/** \brief X-axis data LSB register address */
#define ADXL355_XDATA1_ADDRESS           0x0A
/** \brief Y-axis data MSB register address */
#define ADXL355_YDATA3_ADDRESS           0x0B
/** \brief Y-axis data middle byte register address */
#define ADXL355_YDATA2_ADDRESS           0x0C
/** \brief Y-axis data LSB register address */
#define ADXL355_YDATA1_ADDRESS           0x0D
/** \brief Z-axis data MSB register address */
#define ADXL355_ZDATA3_ADDRESS           0x0E
/** \brief Z-axis data middle byte register address */
#define ADXL355_ZDATA2_ADDRESS           0x0F
/** \brief Z-axis data LSB register address */
#define ADXL355_ZDATA1_ADDRESS           0x10
/** \brief FIFO data register address */
#define ADXL355_FIFO_DATA_ADDRESS        0x11
// Reserved: 0x12 - 0x1D
/** \brief X-axis offset MSB register address */
#define ADXL355_OFFSET_X_H_ADDRESS       0x1E
/** \brief X-axis offset LSB register address */
#define ADXL355_OFFSET_X_L_ADDRESS       0x1F
/** \brief Y-axis offset MSB register address */
#define ADXL355_OFFSET_Y_H_ADDRESS       0x20
/** \brief Y-axis offset LSB register address */
#define ADXL355_OFFSET_Y_L_ADDRESS       0x21
/** \brief Z-axis offset MSB register address */
#define ADXL355_OFFSET_Z_H_ADDRESS       0x22
/** \brief Z-axis offset LSB register address */
#define ADXL355_OFFSET_Z_L_ADDRESS       0x23
/** \brief Activity enable register address */
#define ADXL355_ACT_EN_ADDRESS           0x24
/** \brief Activity threshold MSB register address */
#define ADXL355_ACT_THRESH_H_ADDRESS     0x25
/** \brief Activity threshold LSB register address */
#define ADXL355_ACT_THRESH_L_ADDRESS     0x26
/** \brief Activity count register address */
#define ADXL355_ACT_COUNT_ADDRESS        0x27
/** \brief Filter register address */
#define ADXL355_FILTER_ADDRESS           0x28
/** \brief FIFO samples register address */
#define ADXL355_FIFO_SAMPLES_ADDRESS     0x29
/** \brief Interrupt map register address */
#define ADXL355_INT_MAP_ADDRESS          0x2A
/** \brief Sync register address */
#define ADXL355_SYNC_ADDRESS             0x2B
/** \brief Range register address */
#define ADXL355_RANGE_ADDRESS            0x2C
/** \brief Power control register address */
#define ADXL355_POWER_CTL_ADDRESS        0x2D
/** \brief Self-test register address */
#define ADXL355_SELF_TEST_ADDRESS        0x2E
/** \brief Reset register address */
#define ADXL355_RESET_ADDRESS            0x2F

// STATUS Register (0x04)
/** \brief Status register: Data ready bit offset */
#define ADXL355_STATUS_DATA_RDY_OFFSET       0
/** \brief Status register: Data ready bit mask */
#define ADXL355_STATUS_DATA_RDY_MASK         (1U << ADXL355_STATUS_DATA_RDY_OFFSET)
/** \brief Status register: FIFO full bit offset */
#define ADXL355_STATUS_FIFO_FULL_OFFSET      1
/** \brief Status register: FIFO full bit mask */
#define ADXL355_STATUS_FIFO_FULL_MASK        (1U << ADXL355_STATUS_FIFO_FULL_OFFSET)
/** \brief Status register: FIFO overrun bit offset */
#define ADXL355_STATUS_FIFO_OVR_OFFSET       2
/** \brief Status register: FIFO overrun bit mask */
#define ADXL355_STATUS_FIFO_OVR_MASK         (1U << ADXL355_STATUS_FIFO_OVR_OFFSET)
/** \brief Status register: Activity bit offset */
#define ADXL355_STATUS_ACT_OFFSET            3
/** \brief Status register: Activity bit mask */
#define ADXL355_STATUS_ACT_MASK              (1U << ADXL355_STATUS_ACT_OFFSET)

// FIFO_ENTRIES Register (0x05)
/** \brief FIFO entries register: Number of samples mask */
#define ADXL355_FIFO_ENTRIES_MASK            (0x7F)

// ACT_EN Register (0x24)
/** \brief Activity enable register: X-axis bit offset */
#define ADXL355_ACT_EN_X_OFFSET              0
/** \brief Activity enable register: X-axis bit mask */
#define ADXL355_ACT_EN_X_MASK                (1U << ADXL355_ACT_EN_X_OFFSET)
/** \brief Activity enable register: Y-axis bit offset */
#define ADXL355_ACT_EN_Y_OFFSET              1
/** \brief Activity enable register: Y-axis bit mask */
#define ADXL355_ACT_EN_Y_MASK                (1U << ADXL355_ACT_EN_Y_OFFSET)
/** \brief Activity enable register: Z-axis bit offset */
#define ADXL355_ACT_EN_Z_OFFSET              2
/** \brief Activity enable register: Z-axis bit mask */
#define ADXL355_ACT_EN_Z_MASK                (1U << ADXL355_ACT_EN_Z_OFFSET)

// ACT_THRESH_H/L (0x25/0x26)
// 16-bit threshold value, H: [15:8], L: [7:0]

// ACT_COUNT Register (0x27)
// 8-bit activity count

// FILTER Register (0x28)
/** \brief Filter register: ODR bit offset */
#define ADXL355_FILTER_ODR_OFFSET            0
/** \brief Filter register: ODR bit mask */
#define ADXL355_FILTER_ODR_MASK              (7U << ADXL355_FILTER_ODR_OFFSET)
/** \brief Filter register: External sample bit offset */
#define ADXL355_FILTER_EXT_SAMPLE_OFFSET     4
/** \brief Filter register: External sample bit mask */
#define ADXL355_FILTER_EXT_SAMPLE_MASK       (1U << ADXL355_FILTER_EXT_SAMPLE_OFFSET)

// FIFO_SAMPLES Register (0x29)
/** \brief FIFO samples register: Watermark level mask */
#define ADXL355_FIFO_SAMPLES_MASK            (0x7F)

// INT_MAP Register (0x2A)
/** \brief Interrupt map register: INT1 DRDY bit offset */
#define ADXL355_INT_MAP_INT1_DRDY_OFFSET     0
/** \brief Interrupt map register: INT1 DRDY bit mask */
#define ADXL355_INT_MAP_INT1_DRDY_MASK       (1U << ADXL355_INT_MAP_INT1_DRDY_OFFSET)
/** \brief Interrupt map register: INT1 FIFO full bit offset */
#define ADXL355_INT_MAP_INT1_FIFO_FULL_OFFSET 1
/** \brief Interrupt map register: INT1 FIFO full bit mask */
#define ADXL355_INT_MAP_INT1_FIFO_FULL_MASK  (1U << ADXL355_INT_MAP_INT1_FIFO_FULL_OFFSET)
/** \brief Interrupt map register: INT1 activity bit offset */
#define ADXL355_INT_MAP_INT1_ACT_OFFSET      3
/** \brief Interrupt map register: INT1 activity bit mask */
#define ADXL355_INT_MAP_INT1_ACT_MASK        (1U << ADXL355_INT_MAP_INT1_ACT_OFFSET)
/** \brief Interrupt map register: INT2 DRDY bit offset */
#define ADXL355_INT_MAP_INT2_DRDY_OFFSET     4
/** \brief Interrupt map register: INT2 DRDY bit mask */
#define ADXL355_INT_MAP_INT2_DRDY_MASK       (1U << ADXL355_INT_MAP_INT2_DRDY_OFFSET)
/** \brief Interrupt map register: INT2 FIFO full bit offset */
#define ADXL355_INT_MAP_INT2_FIFO_FULL_OFFSET 5
/** \brief Interrupt map register: INT2 FIFO full bit mask */
#define ADXL355_INT_MAP_INT2_FIFO_FULL_MASK  (1U << ADXL355_INT_MAP_INT2_FIFO_FULL_OFFSET)
/** \brief Interrupt map register: INT2 activity bit offset */
#define ADXL355_INT_MAP_INT2_ACT_OFFSET      7
/** \brief Interrupt map register: INT2 activity bit mask */
#define ADXL355_INT_MAP_INT2_ACT_MASK        (1U << ADXL355_INT_MAP_INT2_ACT_OFFSET)

// SYNC Register (0x2B)
// Used for synchronization (see datasheet for details)

// RANGE Register (0x2C)
/** \brief Range register: Range bit offset */
#define ADXL355_RANGE_RANGE_OFFSET           0
/** \brief Range register: Range bit mask */
#define ADXL355_RANGE_RANGE_MASK             (3U << ADXL355_RANGE_RANGE_OFFSET)
/** \brief Range register: 2G setting */
#define ADXL355_RANGE_2G                     (1U << ADXL355_RANGE_RANGE_OFFSET)
/** \brief Range register: 4G setting */
#define ADXL355_RANGE_4G                     (2U << ADXL355_RANGE_RANGE_OFFSET)
/** \brief Range register: 8G setting */
#define ADXL355_RANGE_8G                     (3U << ADXL355_RANGE_RANGE_OFFSET)

// POWER_CTL Register (0x2D)
/** \brief Power control register: Measure bit offset */
#define ADXL355_POWER_CTL_MEASURE_OFFSET     0
/** \brief Power control register: Measure bit mask */
#define ADXL355_POWER_CTL_MEASURE_MASK       (1U << ADXL355_POWER_CTL_MEASURE_OFFSET)
/** \brief Power control register: Measurement mode */
#define ADXL355_POWER_CTL_MEASURE_MODE       (0U << ADXL355_POWER_CTL_MEASURE_OFFSET)
/** \brief Power control register: Temp off bit offset */
#define ADXL355_POWER_CTL_TEMP_OFF_OFFSET    1
/** \brief Power control register: Temp off bit mask */
#define ADXL355_POWER_CTL_TEMP_OFF_MASK      (1U << ADXL355_POWER_CTL_TEMP_OFF_OFFSET)
/** \brief Power control register: Standby bit offset */
#define ADXL355_POWER_CTL_STANDBY_OFFSET     2
/** \brief Power control register: Standby bit mask */
#define ADXL355_POWER_CTL_STANDBY_MASK       (1U << ADXL355_POWER_CTL_STANDBY_OFFSET)

// SELF_TEST Register (0x2E)
/** \brief Self-test register: Self-test bit offset */
#define ADXL355_SELF_TEST_ST_OFFSET          0
/** \brief Self-test register: Self-test bit mask */
#define ADXL355_SELF_TEST_ST_MASK            (1U << ADXL355_SELF_TEST_ST_OFFSET)

// RESET Register (0x2F)
/** \brief Reset register: Write 0x52 to reset */
#define ADXL355_RESET_CODE                   0x52

// Operations
/** \brief SPI operation: Set bit to indicate read operation */
#define ADXL355_SPI_READ_BIT                 0x01
/** \brief SPI operation: Set bit to indicate write operation */
#define ADXL355_SPI_WRITE_BIT                0x00

// OFFSET registers: 16 bits per axis, H: [15:8], L: [7:0]

// Data registers (XDATA, YDATA, ZDATA): 20 bits per axis, 3 bytes each

// TEMP2/TEMP1: Temperature output, TEMP2 [11:4], TEMP1 [3:0]

/** \brief Sensor operating modes */
typedef enum
{
    SENSOR_MODE_IDLE = 0,      /**< Idle mode */
    SENSOR_MODE_COLLECT_DATA   /**< Data collection mode */
} SENSOR_MODE;

/** \brief Sensor configuration structure */
typedef struct
{
    UNICOMM_Inst_Regs * spiReg;      /**< SPI register pointer */
    GPIO_Regs * csPort;              /**< Chip select port */
    uint32_t csPin;                  /**< Chip select pin */
    GPIO_Regs *readyPort;            /**< Data ready port */
    uint32_t readyPin;               /**< Data ready pin */
    SENSOR_MODE sensorMode;          /**< Sensor operating mode */
} sensorStruct;

/** \brief Variables to hold latest X, Y, Z axis data from sensor */
extern int32_t xData, yData, zData;

/**
 * @brief Write one byte to sensor register via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @param data Data byte
 */
void SPI_writeByte(sensorStruct sensor, uint8_t addr, uint8_t data);


/**
 * @brief Read one byte from sensor register via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @return Data byte read from sensor
 */
uint8_t SPI_readByte(sensorStruct sensor, uint8_t addr);


/**
 * @brief Write multiple bytes to sensor via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @param dataTx Pointer to data buffer to send
 * @param length Number of bytes to send
 */
void SPI_write(sensorStruct sensor, uint8_t addr, uint8_t *dataTx, uint8_t length);


/**
 * @brief Read multiple bytes from sensor via SPI
 * @param sensor Sensor configuration
 * @param addr Register address
 * @param dataRx Pointer to buffer to store received data
 * @param length Number of bytes to read
 */
void SPI_read(sensorStruct sensor, uint8_t addr, uint8_t *dataRx, uint8_t length);


/**
 * @brief Configure the ADXL355 sensor
 * @param sensor Sensor configuration
 */
void ADXL355_config(sensorStruct sensor);


/**
 * @brief Disable data ready interrupt from the ADXL355 sensor
 * @param sensor Pointer to sensor configuration
 */
void ADXL355_disableInterrupts(sensorStruct *sensor);


/**
 * @brief Enable data ready interrupt from the ADXL355 sensor
 * @param sensor Pointer to sensor configuration
 */
void ADXL355_enableInterrupts(sensorStruct *sensor);


/**
 * @brief Read acceleration data from the ADXL355 sensor
 * @param sensor Sensor configuration
 */
void ADXL355_getData(sensorStruct sensor);


#endif /* ADXL355_H_ */
