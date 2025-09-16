## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is
being considered obsolete. These terms will be replaced with "Controller" and
"Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller.
This example can be used with the spi_peripheral_echo_interrupts example
running on another device.

The user must set the mode on the SPI Peripheral to the same mode as the
SPI Controller to ensure correct data communication between devices.
The example will set the SPI to Motorola 4 Wire Mode by default.

By pressing S1 on the LaunchPad, this will initiate a data transfer from the
SPI Controller to the SPI Peripheral. The received data will be incremented,
and the device will transmit this data the next time that S1 is pressed.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Standard with Wake Input | PA18 | J101_18 |  |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| SYSCTL |  |  |  |  |
| UC2 | SPI SCLK (Clock) | PB18 | J1_7 |  |
| UC2 | SPI PICO (Peripheral In, Controller Out) | PB17 | J2_15 |  |
| UC2 | SPI POCI (Peripheral Out, Controller In) | PB19 | J2_14 |  |
| UC2 | SPI CS1 (Chip Select 1) | PB24 | J2_18 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J101_16 |  |
| BOARD | Debug Data In Out | PA19 | J101_14 |  |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G5187 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G5187 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Make the following connections between the SPI Controller and SPI Peripheral:
- Controller SCLK -> Peripheral SCLK
- Controller PICO -> Peripheral PICO
- Controller POCI <- Peripheral POCI
- Controller CS   -> Peripheral CS

Compile, load and run the example. Ensure the SPI Peripheral example is running
on the device before the SPI Controller example.
Press S1 to send a byte.
The Blue LED will toggle after data is received.
The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
