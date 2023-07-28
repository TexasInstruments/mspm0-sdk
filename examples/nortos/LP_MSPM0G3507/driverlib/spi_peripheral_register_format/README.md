## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Peripheral.
This example can be used with the spi_controller_register_format example running on another device. Ensure that the SPI Peripheral is running before starting the SPI Controller example.
This example receives "register format" commands from the SPI Controller. The first byte transmitted by the SPI Controller is the command/register address.
The SPI Controller can send two different types of commands: CMD_WRITE_TYPE_X and CMD_READ_TYPE_X.

When the Controller sends CMD_WRITE_TYPE_X commands, the SPI Controller will then send data to the Peripheral to write to its registers. The Peripheral will initialize itself to receive gCmdWriteTypeXBuffer example buffers.
After all the data is received by the Peripheral, the received data will be stored in its corresponding gCmdWriteTypeXBuffer.

When the Controller sends CMD_READ_TYPE_X commands, the SPI Controller will read data from the Peripheral. The Peripheral will send example gCmdReadTypeXBuffer buffers in response.
After all the data is received by the Controller, the received data will be stored in its corresponding gCmdReadTypeXBuffer.

The Peripheral will go to SLEEP in between transactions.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| SPI1 | PB9 | SPI SCLK (Clock) |
| SPI1 | PB8 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PB7 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PB6 | SPI CS0 (Chip Select 0) |
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
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first

Compile, load and run the example before running the Controller example.
The SPI will automatically start to transmit and receive data responding to
requests from the SPI Controller.
