## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller.
This example can be used with the spi_peripheral_fifo_dma_interrupts example running on another device.

This example sets up the SPI to trigger the DMA Channel 0 to do a transmit data transfer and the DMA Channel 1 to do a receive data transfer.

The DMA Channel 0 is configured in single transfer mode, using the static SPI TX DMA event as the DMA trigger.
When the SPI TXINT is set (which indicates that the SPI is ready to transmit data), this triggers the DMA to transfer the data from a user buffer to the SPI TXFIFO. This transfer will start as soon as the channel is enabled, because the SPI TXINT should be set when the SPI is enabled.
The DMA Channel 1 is configured in single transfer mode, using the static SPI RX DMA event as the DMA trigger.
When data is received and the SPI RXINT is set, this triggers the DMA to transfer the data from the SPI RXFIFO to a user buffer.

After SPI_PACKET_SIZE bytes have been transferred by the DMA, this will trigger the DL_SPI_IIDX_DMA_DONE_TX interrupt indicating that the DMA is done transferring all expected bytes from the buffer to the TXFIFO.
The SPI Controller will then initiate the transaction. The SPI will transmit the data that is in the TXFIFO until it is empty. When the TXFIFO is empty, this will trigger the DL_SPI_IIDX_TX_EMPTY interrupt indicating that the transfer is complete.

When the SPI Controller receives SPI_PACKET_SIZE bytes from the SPI Peripheral, this will trigger the DL_SPI_IIDX_DMA_DONE_RX interrupt indicating that the DMA is done transferring all expected bytes from the RXFIFO to the buffer.


## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| SPI1 | PB9 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PB6 | SPI CS0 (Chip Select 0) |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB9 | SPI1 | SCLK | J1_7 | N/A |
| PB8 | SPI1 | MOSI | J2_15 | N/A |
| PB7 | SPI1 | MISO | J2_14 | N/A |
| PB6 | SPI1 | CS0 | J2_13 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example.
Ensure that the SPI Peripheral example is running **before** starting the SPI Controller example.
Once the example is started, the SPI Controller will automatically start to transmit data.

The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
