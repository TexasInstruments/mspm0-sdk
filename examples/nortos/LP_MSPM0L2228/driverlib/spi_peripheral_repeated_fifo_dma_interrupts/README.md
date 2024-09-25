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

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA21 | Standard Output |
| SYSCTL |  |  |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PA27 | SPI CS1 (Chip Select 1) |
| EVENT |  |  |
| DMA |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA21 | GPIOA | PA21 | J3_8 | <ul><li>This pin can be used for testing purposes in boosterpack connector<ul><li>Pin can be reconfigured for general purpose as necessary</ul></ul><ul><li>PA21 can be connected to GND when used for VREF-<br><ul><li>`R4` is not soldered by default allowing the pin to be used for other functions</ul></ul> |
| PA17 | SPI1 | SCLK | J3_7 | N/A |
| PB8 | SPI1 | MOSI | J7_15 | <ul><li>PB8 is connected to S3 button to GND with no external pull resistor<br><ul><li>Press `S3` button to connect pin to GND<br><li>Don't use `S3` button if not needed by application</ul></ul> |
| PB7 | SPI1 | MISO | J7_14 | N/A |
| PA27 | SPI1 | CS1_MISO1 | J7_33 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L2228 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 web page](https://www.ti.com/tool/LP-MSPM0L2228).

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example.

Ensure that the SPI Peripheral example is running **before** starting the SPI Controller example.

Once the example is started, the SPI Peripheral will wait to receive data. Every time data is received, LED0 will toggle. The USER_TEST pin will toggle with LED1.  When the Controller initiates a transfer with the Peripheral, the Peripheral will transmit a data packet of {'x', 0x2, 0x3, 0x4}, where 'x' starts at '0' and should increment every time the Peripheral example sends a new data packet.

An optional SW breakpoint can be uncommented in the application to check the results of the received data.
If this example is used with the spi_controller_repeated_multibyte_fifo_dma_interrupts example, the expected data that will be received in gRxPacket is {'M', 'S', 'P', 'x'}, where 'x' starts at '0' and should increment every time the Controller example sends a new data packet.

The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
