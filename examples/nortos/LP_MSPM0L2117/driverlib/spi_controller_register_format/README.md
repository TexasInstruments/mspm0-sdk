## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller.
This example can be used with the spi_peripheral_register_format example running on another device.
This example uses "register format" to send commands to a SPI Peripheral. The first byte transmitted by the SPI Controller is the command/register address.
The SPI Controller can send two different types of commands: CMD_WRITE_TYPE_X and CMD_READ_TYPE_X.

When the Controller sends CMD_WRITE_TYPE_X commands, the SPI Controller will then send data to the Peripheral to write to its registers. The Peripheral will initialize itself to receive gCmdWriteTypeXBuffer example buffers.
After all the data is received by the Peripheral, the received data will be stored in its corresponding gCmdWriteTypeXBuffer.

When the Controller sends CMD_READ_TYPE_X commands, the SPI Controller will read data from the Peripheral. The Peripheral will send example gCmdReadTypeXBuffer buffers in response.
After all the data is received by the Controller, the received data will be stored in its corresponding gCmdReadTypeXBuffer.

The Controller will go to sleep in between transactions.

After all commands have been performed, the LED will toggle to indicate success.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the BoosterPack
header and can be used to verify the LED behavior.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J2[1:2] |
| GPIOA | Standard Output | PA15 | J3_25 |
| SYSCTL |  |  |  |  |
| UC8 | SPI PICO (Peripheral In, Controller Out) | PB8 | J2_15 |  |
| UC8 | SPI POCI (Peripheral Out, Controller In) | PB7 | J2_14 |  |
| UC8 | SPI SCLK (Clock) | PA17 | J1_7 |  |
| UC8 | SPI CS0 (Chip Select 0) | PB6 | J2_13 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J14_15 |  |
| BOARD | Debug Data In Out | PA19 | J14_13 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the LP-MSPM02117 User's Guide.

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first

Ensure the SPI Peripheral is running before this Controller example.
Compile, load and run the example.
The SPI will automatically start to transmit and receive data.

The LED will toggle after completion to indicate success.
USER_TEST_PIN GPIO will mimic the behavior of the LED pin on the BoosterPack
header and can be used to verify the LED behavior.
