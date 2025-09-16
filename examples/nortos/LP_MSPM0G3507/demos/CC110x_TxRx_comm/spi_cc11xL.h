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

#ifndef SPI_CC11XL_H
#define SPI_CC11XL_H

typedef uint8_t rfStatus_t;

typedef struct
{
    uint8_t addr;
    uint8_t data;
} registerSetting_t;

// Strobe Commands
#define CC11xL_SRES             0x30    // SRES             - Reset chip
#define CC11xL_SFSTXON          0x31    // SFSTXON          - Enable and calibrate frequency synthesizer
#define CC11xL_SXOFF            0x32    // SXOFF            - Turn off crystal oscillator
#define CC11xL_SCAL             0x33    // SCAL             - Calibrate frequency synthesizer and turn it off
#define CC11xL_SRX              0x34    // SRX              - Enable RX. Perform calibration if enabled
#define CC11xL_STX              0x35    // STX              - Enable TX. If in RX state, only enable TX if CCA passes
#define CC11xL_SIDLE            0x36    // SIDLE            - Exit RX / TX, turn off frequency synthesizer
#define CC11xL_SPWD             0x39    // SPWD             - Enter power down mode when CSn goes high
#define CC11xL_SFRX             0x3A    // SFRX             - Flush the RX FIFO buffer
#define CC11xL_SFTX             0x3B    // SFTX             - Flush the TX FIFO buffer
#define CC11xL_SNOP             0x3D    // SNOP             - No operation. Returns status byte

// Status Registers
#define CC11xL_PARTNUM          0x30    // PARTNUM          - Chip ID
#define CC11xL_VERSION          0x31    // VERSION          - Chip ID
#define CC11xL_FREQEST          0x32    // FREQEST          – Frequency Offset Estimate from demodulator
#define CC11xL_LQI              0x33    // LQI              – Demodulator estimate for Link Quality
#define CC11xL_RSSI             0x34    // RSSI             – Received signal strength indication
#define CC11xL_MARCSTATE        0x35    // MARCSTATE        – Main Radio Control State Machine state
#define CC11xL_RESERVED_0X36    0x36    // RESERVED_0X36    – Reserved register
#define CC11xL_RESERVED_0X37    0x37    // RESERVED_0X37    – Reserved register
#define CC11xL_PKTSTATUS        0x38    // PKTSTATUS        – Current GDOx status and packet status
#define CC11xL_RESERVED_0X39    0x39    // RESERVED_0X39    – Reserved register
#define CC11xL_TXBYTES          0x3A    // TXBYTES          – Underflow and number of bytes
#define CC11xL_RXBYTES          0x3B    // RXBYTES          – Overflow and number of bytes
#define CC11xL_RESERVED_0X3C    0x3C    // RESERVED_0X3C    – Reserved register
#define CC11xL_RESERVED_0X3D    0x3D    // RESERVED_0X3D    – Reserved register

// Register Addresses
#define CC11xL_IOCFG2           0x00    // IOCFG2           - GDO2 output pin configuration
#define CC11xL_IOCFG1           0x01    // IOCFG1           - GDO1 output pin configuration
#define CC11xL_IOCFG0           0x02    // IOCFG1           - GDO0 output pin configuration
#define CC11xL_FIFOTHR          0x03    // FIFOTHR          - RX FIFO and TX FIFO thresholds
#define CC11xL_SYNC1            0x04    // YNC1             - Sync word, high byte
#define CC11xL_SYNC0            0x05    // SYNC0            - Sync word, low byte
#define CC11xL_PKTLEN           0x06    // PKTLEN           - Packet length
#define CC11xL_PKTCTRL1         0x07    // PKTCTRL1         - Packet automation control
#define CC11xL_PKTCTRL0         0x08    // PKTCTRL0         - Packet automation control
#define CC11xL_ADDR             0x09    // ADDR             - Device address
#define CC11xL_RESERVED_0X0A    0x0A    // RESERVED_0X0A    - Reserved register
#define CC11xL_FSCTRL1          0x0B    // FSCTRL1          - Frequency synthesizer control
#define CC11xL_FSCTRL0          0x0C    // FSCTRL0          - Frequency synthesizer control
#define CC11xL_FREQ2            0x0D    // FREQ2            - Frequency control word, high byte
#define CC11xL_FREQ1            0x0E    // FREQ1            - Frequency control word, middle byte
#define CC11xL_FREQ0            0x0F    // FREQ0            - Frequency control word, low byte
#define CC11xL_MDMCFG4          0x10    // MDMCFG4          - Modem configuration
#define CC11xL_MDMCFG3          0x11    // MDMCFG3          - Modem configuration
#define CC11xL_MDMCFG2          0x12    // MDMCFG2          - Modem configuration
#define CC11xL_MDMCFG1          0x13    // MDMCFG1          - Modem configuration
#define CC11xL_MDMCFG0          0x14    // MDMCFG1          - Modem configuration
#define CC11xL_DEVIATN          0x15    // DEVIATN          - Modem deviation setting
#define CC11xL_MCSM2            0x16    // MCSM2            - Main Radio Control State Machine configuration
#define CC11xL_MCSM1            0x17    // MCSM1            - Main Radio Control State Machine configuration
#define CC11xL_MCSM0            0x18    // MCSM0            - Main Radio Control State Machine configuration
#define CC11xL_FOCCFG           0x19    // FOCCFG           - Frequency Offset Compensation configuration
#define CC11xL_BSCFG            0x1A    // BSCFG            - Bit Synchronization configuration
#define CC11xL_AGCCTRL2         0x1B    // AGCCTRL2         - AGC control
#define CC11xL_AGCCTRL1         0x1C    // AGCCTRL1         - AGC control
#define CC11xL_AGCCTRL0         0x1D    // AGCCTRL0         - AGC control
#define CC11xL_RESERVED_0X20    0x20    // RESERVED_0X20    - Reserved register
#define CC11xL_FREND1           0x21    // FREND1           - Front end RX configuration
#define CC11xL_FREND0           0x22    // FREDN0           - Front end TX configuration
#define CC11xL_FSCAL3           0x23    // FSCAL3           - Frequency synthesizer calibration
#define CC11xL_FSCAL2           0x24    // FSCAL2           - Frequency synthesizer calibration
#define CC11xL_FSCAL1           0x25    // FSCAL1           - Frequency synthesizer calibration
#define CC11xL_FSCAL0           0x26    // FSCAL0           - Frequency synthesizer calibration
#define CC11xL_RESERVED_0X29    0x29    // RESERVED_0X29    - Reserved register
#define CC11xL_RESERVED_0X2A    0x2A    // RESERVED_0X2A    - Reserved register
#define CC11xL_RESERVED_0X2B    0x2B    // RESERVED_0X2B    - Reserved register
#define CC11xL_TEST2            0x2C    // TEST2            - Various test settings
#define CC11xL_TEST1            0x2D    // TEST1            - Various test settings
#define CC11xL_TEST0            0x2E    // TEST0            - Various test settings
#define CC11xL_PA_TABLE0        0x3E    // PA_TABLE0        - PA control settings table

#define RADIO_BURST_ACCESS      0x40
#define RADIO_SINGLE_ACCESS     0x00
#define RADIO_READ_ACCESS       0x80
#define RADIO_WRITE_ACCESS      0x00

// Burst Write Registers
#define CC11xL_PA_TABLE0        0x3E    // PA_TABLE0 - PA control settings table
#define CC11xL_FIFO             0x3F    // FIFO  - Transmit FIFO

rfStatus_t mspm0SpiCmdStrobe(uint8_t cmd);
static void mspm0ReadWriteBurstSingle(uint8_t addr,uint8_t *pData, uint16_t len);
rfStatus_t mspm08BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len);
rfStatus_t cc11xLSpiReadReg(uint8_t addr, uint8_t *pData, uint8_t len);
rfStatus_t cc11xLSpiWriteReg(uint8_t addr_t, uint8_t *pData, uint8_t len);
rfStatus_t cc11xLSpiWriteTxFifo(uint8_t *pData, uint8_t len);
rfStatus_t cc11xLSpiReadRxFifo(uint8_t *pData, uint8_t len);

#endif
