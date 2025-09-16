## Example Summary

Note: The use of "Master" and "Slave", along with "MOSI/MISO" terminology is
being considered obsolete. These terms will be replaced with "Controller" and
"Peripheral", and "PICO/POCI" respectively.

The following example configures the SPI as a Controller in internal loopback
mode to transmit and receive data.

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| GPIOB | Standard Output | PB13 | LED2 Blue | Populate Jumper(s): J11[2:1] |
| SYSCTL |  |  |  |  |
| UC2 | SPI SCLK (Clock) | PA11 | J4_33 |  |
| UC2 | SPI PICO (Peripheral In, Controller Out) | PA14 | J3_30 |  |
| UC2 | SPI POCI (Peripheral Out, Controller In) | PA10 | J4_34 |  |
| UC2 | SPI CS0 (Chip Select 0) | PA12 | J3_28 |  |
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
Compile, load and run the example. The SPI will automatically start
to transmit and receive data.
The SPI is initialized with the following configuration:
- SPI Controller
- Motorola 4 Wire with Polarity 0, Phase 0
- No parity
- 8 bits per transfer
- MSB first
LED1 and USER_TEST pin are toggled after each transaction. A SW breakpoint is
set in the application to allow the user to check the value of the received
data.
