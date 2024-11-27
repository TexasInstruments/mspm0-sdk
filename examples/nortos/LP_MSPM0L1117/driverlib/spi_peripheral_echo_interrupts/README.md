## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is
being considered obsolete. These terms will be replaced with "Controller" and
"Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Peripheral.
This example can be used with the spi_controller_echo_interrupts example running
 on another device.

The user must set the mode on the SPI Peripheral to the same mode
as the SPI Controller to ensure correct data communication between devices.
The example will set the SPI to Motorola 4 Wire Mode by default.

When the SPI Peripheral receives data, it will echo this data back to the SPI
Controller.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J1[1:2] |
| GPIOA | Standard Output | PA25 | J1_2 |  |
| SYSCTL |  |  |  |  |
| SPI0 | SPI SCLK (Clock) | PB3 | J1_7 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PB2 | J2_15 |  |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PA13 | J2_14 |  |
| SPI0 | SPI CS1 (Chip Select 1) | PA12 | J3_25 |  |
| EVENT |  |  |  |  |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |

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

Compile, load and run the example. Ensure the SPI Peripheral example is
running on the device before the SPI Controller example.
Send bytes from SPI Controller to SPI Peripheral.
The LED and USER_TEST pin will toggle after data is received.
The SPI is initialized with the following configuration:
- SPI Peripheral
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
