## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller.
This example is intended to be used with the spi_peripheral_fifo_dma_interrupts example running on another device.

This example sets up the SPI to trigger the DMA Channel 0 to do a transmit data transfer and the DMA Channel 1 to do a receive data transfer.

The DMA Channel 0 is configured in single transfer mode, using the static SPI TX DMA event as the DMA trigger.
When the SPI TXINT is set (which indicates that the SPI is ready to transmit data), this triggers the DMA to transfer the data from a user buffer to the SPI TXFIFO. This transfer will start as soon as the channel is enabled, because the SPI TXINT should be set when the SPI is enabled.
The DMA Channel 1 is configured in single transfer mode, using the static SPI RX DMA event as the DMA trigger.
When data is received and the SPI RXINT is set, this triggers the DMA to transfer the data from the SPI RXFIFO to a user buffer.

After SPI_PACKET_SIZE bytes have been transferred by the DMA, this will trigger the DL_SPI_IIDX_DMA_DONE_TX interrupt indicating that the DMA is done transferring all expected bytes from the buffer to the TXFIFO.
The SPI Controller will then initiate the transaction. The SPI will transmit the data that is in the TXFIFO until it is empty. When the TXFIFO is empty, this will trigger the DL_SPI_IIDX_TX_EMPTY interrupt indicating that the transfer is complete.

When the SPI Controller receives SPI_PACKET_SIZE bytes from the SPI Peripheral, this will trigger the DL_SPI_IIDX_DMA_DONE_RX interrupt indicating that the DMA is done transferring all expected bytes from the RXFIFO to the buffer.

The SPI controller will then check the received bytes and compare against the expected bytes from the spi_peripheral_fifo_dma_interrupts example and set the LED and USER_TEST high upon a successful compare.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Output | PA15 | J4_38 |  |
| SYSCTL |  |  |  |  |
| SPI0 | SPI SCLK (Clock) | PA25 | J1_7 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PA10 | J2_15 |  |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PA9 | J2_14 |  |
| SPI0 | SPI CS0 (Chip Select 0) | PA8 | J2_13 |  |
| EVENT |  |  |  |  |
| DMA |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_4 |  |
| BOARD | Debug Data In Out | PA19 | J14_2 |  |

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
MSPM0 LaunchPad, please visit the LP-MSPM0H3216 User's Guide.

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example.
Ensure that the SPI Peripheral example is running **before** starting the SPI Controller example.
Once the example is started, the SPI Controller will automatically start to transmit data.
The example will set the LED high upon successful completion and reception of expected data.
USER_TEST GPIO pin will mimic the behavior of the LED pin on the BoosterPack header and can be used to verify the LED behavior.

The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
