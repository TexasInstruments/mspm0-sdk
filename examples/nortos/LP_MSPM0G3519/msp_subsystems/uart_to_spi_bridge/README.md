## Example Summary

The following example bridges incoming UART transmissions to 
to SPI peripherals. The UART is confgured at 9600bps to 
transmit and receive data. The SPI is configured for 8MHz transmissions. 
The device remains in SLEEP mode while waiting for an interrupt.
A specific UART data format is expected for proper operation. <br>
UART data packet: <br>
[Start Byte] [Read or Write indicator] [Message Length N] [D0] [D1]..[DN]

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| UART0 | RX Pin | PA11 | J4_33 | Populate Jumper(s): J15[2:1] |
| UART0 | TX Pin | PA10 | J4_34 | Populate Jumper(s): J14[2:1] |
| SPI1 | SPI SCLK (Clock) | PB9 | J4_39 |  |
| SPI1 | SPI PICO (Peripheral In, Controller Out) | PB8 | J2_15 |  |
| SPI1 | SPI POCI (Peripheral Out, Controller In) | PB7 | J2_14 |  |
| SPI1 | SPI CS0 (Chip Select 0) | PB6 | J2_18 |  |
| BOARD | Debug Clock | PA20 | J19_16 |  |
| BOARD | Debug Data In Out | PA19 | J19_14 |  |

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
To use this application feed proper UART data packet to device, and attach SPI peripheral.
Compile, load and run the example.
After receiving UART packet, device will automatically translate to SPI peripheral, or provide error code.
