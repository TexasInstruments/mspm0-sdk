
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

#include "bsl_spi.h"
#include "boot_config.h"

/*
 * Global variables
 */

uint8_t BSL_Buffer_Status;
uint8_t* BSL_Back_Buf;
uint8_t* BSL_Active_Buf;
uint8_t* BSL_Buf1;
uint8_t* BSL_Buf2;
uint16_t BSL_RXBufferSize;
uint16_t BSL_maxBufferSize;
uint8_t* BSL_drainFIFO;

volatile BSL_RX_states BSL_RX_state;
volatile uint32_t BSL_PI_dataPointer;
volatile uint32_t BSL_PI_checksum;
volatile uint16_t BSL_PI_RxDataCnt;
volatile uint16_t BSL_activePluginType;
volatile uint8_t BSL_PI_RxData;

/*
 * Static Function declarations
 */

/*
 * @brief       Interrupt handler for UART interrupts
 */
void BSL_PI_SPI_ISR(void);
/*
 * @brief       Sends one byte of data
 *
 * @param[in]   data    One byte of data to be transferred
 *
 * @return      none
 */
void BSL_PI_sendByte(uint8_t data);
/*
 * @brief       Receive one byte of data
 *
 * @param[in]   none
 *
 * @return      One byte of data received
 */
uint8_t SPI_receiveByte(void);
/*
 * @brief       Initializes data buffer
 *
 * Initializes the buffer variables and all the global variables
 * needed for Plugin operation.
 *
 * @param[in]   buffer      Pointer to the SRAM data buffer sent from the
 *                          BSL core.
 * @param[in]   bufferSize  1/2 of the SRAM memory size available to use as
 *                          data buffer.
 * @return      none
 */
void BSL_initBuffers(uint8_t* buffer, uint16_t bufferSize);
/*
 * @brief       Calculates the CRC32 for the given data
 *
 * This function calculates the CRC32 for the given data using the CRC module
 * present in the device. It doesn't need to configure CRC, since it would have
 * been already taken care by ROM BSL.
 *
 * ROM BSL configures CRC modules with following specifications:
 * 1. CRC32-ISO3309 polynomial
 * 2. Bit reversed configuration
 * 3. Initial seed - 0xFFFFFFFF
 *
 * @param[in]   data        Pointer to the data byte array for which CRC should
 *                          be calculated
 * @param[in]   dataSize    Size of the data transferred for calculation
 *
 * @return      32 bit CRC value
 */
uint32_t BSL_calculateCRC(uint8_t* data, uint16_t dataSize);
/*
 * @brief       Process the commands which needs handling in interface layer
 *
 * @param[in]   dataBuffer  Pointer to the data buffer which contains the packet
 *                          received
 *
 * @return      none
 */
void BSL_PI_interpretPICommand(const uint8_t* dataBuffer);

void BSL_initBuffers(uint8_t* buffer, uint16_t bufferSize)
{
    /*
     * Two buffers are configured for BSL communication, in which one buffer
     * will be used for receiving the packets, while other buffer contains the
     * last received command packet shared with BSL core for processing.
     */
    BSL_Buf1 = buffer;
    BSL_Buf2 = &buffer[bufferSize];

    BSL_Active_Buf = BSL_Buf1;
    BSL_Back_Buf   = BSL_Buf2;

    BSL_Buffer_Status = BSL_BUF1_ACTIVE;

    BSL_maxBufferSize = bufferSize;

    /* Initialize buffer pointers */
    BSL_RXBufferSize   = (uint16_t) 0;
    BSL_PI_dataPointer = (uint32_t) 0;
    BSL_PI_checksum    = (uint32_t) 0;
    BSL_PI_RxDataCnt   = (uint16_t) 0;

    /* Reset the Receive state */
    BSL_RX_state = RX_idle;
}

uint16_t BSL_PI_SPI_init(uint8_t* buffer, uint16_t bufferSize)
{
    BSL_initBuffers(buffer, bufferSize);

    /* Reset and enable power to peripheral */
    DL_SPI_reset(BSL_SPI);
    DL_SPI_enablePower(BSL_SPI);

    /* Initialize GPIOs */
    DL_GPIO_initPeripheralInputFunction(
        BSL_SPI_GPIO_IOMUX_SCLK, BSL_SPI_GPIO_FUNCTION);
    DL_GPIO_initPeripheralInputFunction(
        BSL_SPI_GPIO_IOMUX_COPI, BSL_SPI_GPIO_FUNCTION);
    DL_GPIO_initPeripheralInputFunction(
        BSL_SPI_GPIO_IOMUX_CS, BSL_SPI_GPIO_FUNCTION);
    DL_GPIO_initPeripheralOutputFunction(
        BSL_SPI_GPIO_IOMUX_CIPO, BSL_SPI_GPIO_FUNCTION);

    /* Configure SPI in Slave mode */
    DL_SPI_Config config;
    DL_SPI_ClockConfig clockConfig;

    config.mode          = DL_SPI_MODE_PERIPHERAL;
    config.frameFormat   = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0;
    config.parity        = DL_SPI_PARITY_NONE;
    config.dataSize      = DL_SPI_DATA_SIZE_8;
    config.bitOrder      = DL_SPI_BIT_ORDER_MSB_FIRST;
    config.chipSelectPin = DL_SPI_CHIP_SELECT_0;

    clockConfig.clockSel    = DL_SPI_CLOCK_BUSCLK;
    clockConfig.divideRatio = DL_SPI_CLOCK_DIVIDE_RATIO_1;

    DL_SPI_init(BSL_SPI, &config);
    DL_SPI_setClockConfig(BSL_SPI, &clockConfig);

    DL_SPI_enablePeripheralAlignDataOnChipSelect(BSL_SPI);
    DL_SPI_setPeripheralReceiveTimeout(BSL_SPI, 0);

    /* Configure FIFO */
    DL_SPI_setFIFOThreshold(BSL_SPI, DL_SPI_RX_FIFO_LEVEL_ONE_FRAME,
        DL_SPI_TX_FIFO_LEVEL_ONE_FRAME);

    /* Configure interrupt */
    /* Initialize interrupts in NVIC */
    NVIC_ClearPendingIRQ(BSL_SPI_INTERRUPT_NUM);
    NVIC_EnableIRQ(BSL_SPI_INTERRUPT_NUM);

    /* Initialize interrupts for SPI */
    DL_Interrupt_registerInterrupt(
        (uint32_t) BSL_SPI_INTERRUPT_NUM, BSL_PI_SPI_ISR);
    DL_SPI_enableInterrupt(BSL_SPI, DL_SPI_INTERRUPT_RX);

    /* Enable SPI */
    DL_SPI_enable(BSL_SPI);

    return FLASH_PLUGIN_VERSION_SPI;
}

uint32_t BSL_PI_SPI_receive(void)
{
    uint32_t receivedPacketAddr = (uint32_t) 0;
    /*
     * Since same data buffers are used between the ROM and flash plugins,
     * BSL_activePluginType helps to check if the data received is through
     * Flash plugin or not.
    */
    if (BSL_activePluginType == FLASH_PLUGIN_VERSION_SPI) {
        /*
         * If the last received command packet is processed, and BSL core is ready
         * to accept a new data packet, the state will move from BLOCKED to IDLE.
         */
        if (BSL_RX_state == RX_blocked) {
            BSL_RX_state = RX_idle;
        }
        /*
         * Once the packet is started to arrive, the RX state will be changed to
         * RECEIVING and the same state is retained until complete packet is
         * received.
         */
        while (BSL_RX_state == RX_receiving) {
            if (BSL_PI_dataPointer == (uint32_t) 0) {
                if (BSL_PI_RxDataCnt > (uint16_t) 0) {
                    /* Check if first byte in the packet matches the Header byte */
                    if (BSL_Back_Buf[0] != BSL_PACKET_HEADER_BYTE) {
                        BSL_PI_sendByte(BSL_ERROR_HEADER_INCORRECT);
                        BSL_RX_state = RX_idle;
                    } else {
                        BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
                    }
                }
            }
            /* Next two bytes are buffer length */
            else if (BSL_PI_dataPointer == (uint32_t) 1) {
                if (BSL_PI_RxDataCnt > (uint16_t) 1) {
                    BSL_RXBufferSize   = (uint16_t) BSL_Back_Buf[1];
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
                }
            } else if (BSL_PI_dataPointer == (uint32_t) 2) {
                if (BSL_PI_RxDataCnt > (uint16_t) 2) {
                    uint16_t temp = (uint16_t) BSL_Back_Buf[2] << (uint16_t) 8;
                    BSL_RXBufferSize |= temp;

                    /*
                     * Check if the packet size received is valid
                     * else return the error
                     */
                    if (BSL_RXBufferSize == (uint16_t) 0) {
                        BSL_PI_sendByte(BSL_ERROR_PACKET_SIZE_ZERO);
                        BSL_RX_state = RX_idle;
                    }
                    if ((BSL_RXBufferSize + BSL_PI_WRAPPER_SIZE) >
                        BSL_maxBufferSize) {
                        BSL_PI_sendByte(BSL_ERROR_PACKET_SIZE_TOO_BIG);
                        BSL_RX_state = RX_idle;
                    }
                    BSL_PI_dataPointer =
                        (uint32_t) BSL_RXBufferSize + (uint32_t) 3;
                }
            }
            /* Skip over the buffer data and go to the 32-bit checksum at the end */
            /* Start with the low checksum byte */
            else if (BSL_PI_dataPointer ==
                     ((uint32_t) BSL_RXBufferSize + (uint32_t) 3)) {
                if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 3)) {
                    BSL_PI_checksum = (uint32_t)
                        BSL_Back_Buf[BSL_RXBufferSize + (uint16_t) 3];
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
                }
            } else if (BSL_PI_dataPointer ==
                       ((uint32_t) BSL_RXBufferSize + (uint32_t) 4)) {
                if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 4)) {
                    BSL_PI_checksum =
                        BSL_PI_checksum |
                        (BSL_Back_Buf[BSL_RXBufferSize + (uint16_t) 4] << 8);
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
                }
            } else if (BSL_PI_dataPointer ==
                       ((uint32_t) BSL_RXBufferSize + (uint32_t) 5)) {
                if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 5)) {
                    BSL_PI_checksum =
                        BSL_PI_checksum |
                        (BSL_Back_Buf[BSL_RXBufferSize + (uint16_t) 5] << 16);
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
                }
            } else if (BSL_PI_dataPointer ==
                       ((uint32_t) BSL_RXBufferSize + (uint32_t) 6)) {
                if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 6)) {
                    BSL_PI_checksum =
                        BSL_PI_checksum |
                        (BSL_Back_Buf[BSL_RXBufferSize + (uint16_t) 6] << 24);
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;

                    BSL_RX_state = RX_blocked;

                    uint8_t* BSL_core_data_start = &BSL_Back_Buf[3];

                    /* Verify checksum */
                    if (BSL_calculateCRC(BSL_core_data_start,
                            BSL_RXBufferSize) == BSL_PI_checksum) {
                        /* Ping pong the buffers */
                        if (BSL_Buffer_Status == BSL_BUF1_ACTIVE) {
                            BSL_Back_Buf      = BSL_Buf1;
                            BSL_Active_Buf    = BSL_Buf2;
                            BSL_Buffer_Status = BSL_BUF2_ACTIVE;

                        } else {
                            BSL_Back_Buf      = BSL_Buf2;
                            BSL_Active_Buf    = BSL_Buf1;
                            BSL_Buffer_Status = BSL_BUF1_ACTIVE;
                        }
                        /*
                         * Return the packet address to the BSL core for processing
                         */
                        BSL_PI_sendByte(BSL_ACK);
                        receivedPacketAddr = (uint32_t) BSL_Active_Buf;

                        BSL_PI_interpretPICommand(BSL_Active_Buf);
                    } else {
                        /* Checksum failed */
                        BSL_PI_sendByte(BSL_ERROR_CHECKSUM_INCORRECT);
                        BSL_RX_state = RX_idle;
                    }
                }
            } else {
                /* Do nothing */
            }
        }
    }

    return receivedPacketAddr;
}

uint8_t BSL_PI_SPI_send(uint8_t* data, uint16_t len)
{
    uint8_t status    = BSL_PI_SUCCESS;
    uint32_t checksum = (uint32_t) 0;
    uint16_t i;
    uint8_t* BSL_core_data_start = &data[3];
    uint16_t dataSize            = (uint16_t) data[1];
    dataSize                     = dataSize | (data[2] << 8U);

    /* Calculate CRC for BSL Core Response and data */
    checksum = BSL_calculateCRC(BSL_core_data_start, dataSize);

    /* Append the 32-bit CRC result to the response packet */
    data[len]                = (uint8_t) checksum & (uint8_t) 0xFF;
    uint8_t temp             = checksum >> 8U;
    data[len + (uint16_t) 1] = temp & (uint8_t) 0xFF;
    temp                     = checksum >> 16U;
    data[len + (uint16_t) 2] = temp & (uint8_t) 0xFF;
    data[len + (uint16_t) 3] = (checksum >> 24U);

    /* Transmit the packet */
    for (i = (uint16_t) 0; i < (BSL_PI_CRC_SIZE + len); i++) {
        while (!(DL_SPI_getRawInterruptStatus(BSL_SPI, DL_SPI_INTERRUPT_TX)))
            ;
        /* Waits until FIFO has space to fill the data */
        DL_SPI_transmitDataBlocking8(BSL_SPI, data[i]);
        while (DL_SPI_isBusy(BSL_SPI))
            ;
    }
    /* Waits until the data is completely transmitted */
    while (DL_SPI_isBusy(BSL_SPI))
        ;

    /*Clear TX FIFO*/
    for (int i = 0; i < 4; i++) {
        BSL_SPI->TXDATA = 0x00;
    }

    /*Read maximum of 4 bytes from the RX FIFO*/
    DL_SPI_drainRXFIFO8(BSL_SPI, BSL_drainFIFO, (uint32_t) 00000004);

    return status;
}

bool BSL_PI_SPI_deinit(void)
{
    /* Reset the SPI */
    DL_SPI_reset(BSL_SPI);

    /* Disable and Unregister the interrupt */
    DL_Interrupt_unregisterInterrupt((uint32_t) BSL_SPI_INTERRUPT_NUM);
    NVIC_DisableIRQ(BSL_SPI_INTERRUPT_NUM);

    return true;
}

void BSL_PI_sendByte(uint8_t data)
{
    while (!(DL_SPI_getRawInterruptStatus(BSL_SPI, DL_SPI_INTERRUPT_TX)))
        ;
    DL_SPI_transmitDataBlocking8(BSL_SPI, data);
    while (DL_SPI_isBusy(BSL_SPI))
        ;
}

uint8_t SPI_receiveByte()
{
    while (1) {
        if (DL_SPI_isRXFIFOEmpty(BSL_SPI) == false) {
            BSL_PI_RxData = DL_SPI_receiveData8(BSL_SPI);
            return BSL_PI_RxData;
        }
    }
}

void BSL_PI_SPI_ISR(void)
{
    uint8_t receivedData;
    switch (DL_SPI_getPendingInterrupt(BSL_SPI)) {
        case DL_SPI_IIDX_RX:
            receivedData = SPI_receiveByte();

            /*
             * If a byte is received when RX is in IDLE state, it is treated as
             * a new command packet and buffer parameters are initialized
             * to receive the packet.
             */
            if ((BSL_RX_state == RX_idle) &&
                ((receivedData == BSL_PACKET_HEADER_BYTE) ||
                    (BSL_activePluginType == FLASH_PLUGIN_VERSION_SPI))) {
                BSL_activePluginType = FLASH_PLUGIN_VERSION_SPI;
                BSL_PI_RxDataCnt     = (uint16_t) 1;
                BSL_PI_dataPointer   = (uint32_t) 0;
                BSL_Back_Buf[0]      = receivedData;
                BSL_RX_state         = RX_receiving;
            }
            /*
             * Whenever a new byte is received, after a packet reception is
             * started, add it to the data buffer
             */
            else if (BSL_RX_state == RX_receiving) {
                if (BSL_PI_RxDataCnt < (BSL_maxBufferSize + (uint16_t) 4)) {
                    BSL_Back_Buf[BSL_PI_RxDataCnt] = receivedData;
                    BSL_PI_RxDataCnt = BSL_PI_RxDataCnt + (uint16_t) 1;
                }
            }
            /* When RX state machine is in any other state (BLOCKED),
             * packet is dropped */
            else {
            }
            break;
        default:
            break;
    }
}

uint32_t BSL_calculateCRC(uint8_t* data, uint16_t dataSize)
{
    uint16_t bufferIndex;

    /*
     * @note: CRC module would have been configured by the ROM BSL with
     * required specification.
     */

    /* Set the Seed value to reset the calculation */
    DL_CRC_setSeed32(CRC, MSPM0_BSL_CRC_SEED);

    /* Feed the data to CRC module */
    for (bufferIndex = (uint16_t) 0; bufferIndex < dataSize; bufferIndex++) {
        DL_CRC_feedData8(CRC, data[bufferIndex]);
    }

    /* Return the 32 bit result */
    return DL_CRC_getResult32(CRC);
}

void BSL_PI_interpretPICommand(const uint8_t* dataBuffer)
{
    uint8_t command = dataBuffer[3];

    /* Check if command is non-blocking */
    if (command == CMD_PROGRAM_DATA_FAST) {
        /*
         * When a non-blocking command is received, set the RX state to IDLE
         * So that the state machine continuous to receive the next command
         * packet without waiting for the current command to be processed by the
         * BSL core
         * */
        BSL_RX_state = RX_idle;
    } else {
        /* Command is blocking */
        BSL_RX_state = RX_blocked;
    }
}
