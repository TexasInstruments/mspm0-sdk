## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Peripheral.
This example can be used with the spi_controller_repeated_fifo_dma_interrupts example running on another device.
This example sets up the SPI to receive data from another SPI Controller.

This example sets up the DMA Channel 0 to do a receive data transfer.

The DMA Channel 0 is configured in single transfer mode, using the static SPI RX DMA event as the DMA trigger.
When data is received and the SPI RXINT is set, this triggers the DMA to transfer the data from the SPI RXFIFO to a user buffer.

The SPI Controller must first initiate the transaction. When the SPI Peripheral receives SPI_PACKET_SIZE bytes, this will trigger the DL_SPI_IIDX_DMA_DONE_RX interrupt indicating that the DMA is done transferring all expected bytes from the RXFIFO to the buffer.
During the transaction, the SPI Peripheral will transmit the data that is in the TXFIFO until it is empty. When the TXFIFO is empty, this will trigger the DL_SPI_IIDX_TX_EMPTY interrupt indicating that the transfer is complete.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | Pin | Launchpad Pin |
| --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 |
| GPIOA | Standard Output | PA15 | J3_30 |
| SPI1 | SPI SCLK (Clock) | PB31 | J1_7 |
| SPI1 | SPI PICO (Peripheral In, Controller Out) | PB8 | J2_15 |
| SPI1 | SPI POCI (Peripheral Out, Controller In) | PB7 | J2_14 |
| SPI1 | SPI CS0 (Chip Select 0) | PB6 | J2_18 |
| BOARD | Debug Clock | PA20 | J19_16 |
| BOARD | Debug Data In Out | PA19 | J19_14 |

### Device Migration Recommendations
This project was developed for a superset device included in the MSPM0 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0G3519 User's Guide.

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example.

Ensure that the SPI Peripheral example is running **before** starting the SPI Controller example.

Once the example is started, the SPI Peripheral will wait to receive data. Every time data is received, LED0 will toggle. The USER_TEST pin will toggle with LED1. When the Controller initiates a transfer with the Peripheral, the Peripheral will transmit a data packet of {'x', 0x2, 0x3, 0x4}, where 'x' starts at '0' and should increment every time the Peripheral example sends a new data packet.

An optional SW breakpoint can be uncommented in the application to check the results of the received data.
If this example is used with the spi_controller_repeated_multibyte_fifo_dma_interrupts example, the expected data that will be received in gRxPacket is {'M', 'S', 'P', 'x'}, where 'x' starts at '0' and should increment every time the Controller example sends a new data packet.

The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
