## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is being considered obsolete. These terms will be replaced with "Controller" and "Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller.

This example uses the CD/CS3 line as a signal to distinguish between Command and Data information.
- C/D level low: command information
- C/D level high: data information

When a value is written into the CTL1.CDMODE bits the C/D line will go low for the given numbers of byte which
are sent by the SPI, starting with the next value to be transmitted. After the number of bytes are transmitted the
C/D will be set to high automatically.

After all transmits have been performed, the LED will toggle to indicate completion.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOA | Open-Drain Output | PA0 | Red LED1 | Populate Jumper(s): J1[1:2] |
| SYSCTL |  |  |  |  |
| SPI0 | SPI SCLK (Clock) | PB3 | J1_7 |  |
| SPI0 | SPI PICO (Peripheral In, Controller Out) | PB2 | J2_15 |  |
| SPI0 | SPI POCI (Peripheral Out, Controller In) | PA13 | J2_14 |  |
| SPI0 | SPI CS1 (Chip Select 1) | PA12 | J3_25 |  |
| SPI0 | SPI CS3/CD (Chip Select 3/Command Data) | PA8 | J1_4 |  |
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
- Controller CD   -> Peripheral CD

Compile, load and run the example. The SPI will automatically start
to transmit and receive data.

## Application Design Details
The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
