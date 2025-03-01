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
| GPIOA | Standard Output | PA0 | Red LED1 | Populate Jumper(s): J4[1:2] |
| GPIOA | Standard Input | PA14 | S2 |  |
| SYSCTL |  |  |  |  |
| SPI0 | SPI SCLK (Clock) | PA25 | J1_7 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PA10 | J2_15 |  |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PA9 | J2_14 |  |
| SPI0 | SPI CS0 (Chip Select 0) | PA8 | J2_13 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J13_4 | Populate Component(s): J13 |
| BOARD | Debug Data In Out | PA19 | J13_2 | Populate Component(s): J13 |

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

Compile, load and run the example. Ensure the SPI Peripheral example is running
on the device before the SPI Controller example.
Press S2 to send a byte.
The Blue LED will toggle after data is received.
The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
