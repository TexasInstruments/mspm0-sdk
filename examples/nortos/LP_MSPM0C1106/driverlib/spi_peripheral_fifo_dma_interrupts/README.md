## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Peripheral.
This example can be used with the spi_controller_fifo_dma_interrupts
example running on another device.
This example sets up the SPI to receive data from another SPI Controller.

This example sets up the SPI to trigger the DMA Channel 0 to do a transmit data transfer and the DMA Channel 1 to do a receive data transfer.

The DMA Channel 0 is configured in single transfer mode, using the static SPI TX DMA event as the DMA trigger.
When the SPI TXINT is set (which indicates that the SPI is ready to transmit data), this triggers the DMA to transfer the data from a user buffer to the SPI TXFIFO. This transfer will start as soon as the channel is enabled, because the SPI TXINT should be set when the SPI is enabled. After SPI_PACKET_SIZE bytes have been transferred by the DMA, this will trigger the DL_SPI_IIDX_DMA_DONE_TX interrupt indicating that the DMA is done transferring all expected bytes from the buffer to the TXFIFO. However the SPI Peripheral will not actually start transmitted data until the SPI Controller initiates a transaction.
The DMA Channel 1 is configured in single transfer mode, using the static SPI RX DMA event as the DMA trigger.
When data is received and the SPI RXINT is set, this triggers the DMA to transfer the data from the SPI RXFIFO to a user buffer.

The SPI Controller must first initiate the transaction. When the SPI Peripheral receives SPI_PACKET_SIZE bytes, this will trigger the DL_SPI_IIDX_DMA_DONE_RX interrupt indicating that the DMA is done transferring all expected bytes from the RXFIFO to the buffer.
During the transaction, the SPI Peripheral will transmit the data that is in the TXFIFO until it is empty. When the TXFIFO is empty, this will trigger the DL_SPI_IIDX_TX_EMPTY interrupt indicating that the transfer is complete.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA15 | Standard Output |
| SYSCTL |  |  |
| SPI0 | SPI SCLK (Clock) | PA25 | J1_7 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PA10 | J2_15 |  |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PA9 | J2_14 |  |
| SPI0 | SPI CS0 (Chip Select 0) | PA8 | J2_13 |  |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example.
Ensure that the SPI Peripheral example is running **before** starting the SPI Controller example.
Once the example is started, the SPI Peripheral will wait to receive data. Once data is successfully received, the LED will turn on.

The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
