## Example Summary

The following example bridges incoming UART transmissions to 
to I2C peripherals. The UART is confgured at 9600bps to 
transmit and receive data. The I2C is configured for 100kHz transmissions. 
The device remains in SLEEP mode while waiting for an interrupt.
A specific UART data format is expected for proper operation. <br>
UART data packet: <br>
[Start Byte] [I2C Address] [I2C Read or Write indicator] [Message Length N] [D0] [D1]..[DN]

Please see accompanying sub-system document for more information. [UART to I2C Sub-system](https://www.ti.com/lit/spradd0)

## Peripherals, Pin Functions, MCU Pins, Launchpad Pins
| Peripheral | Function | MCU Pin | Launchpad Pin | Launchpad Settings |
| --- | --- | --- | --- | --- |
| SYSCTL |  |  |  |  |
| I2C0 | I2C Serial Data line (SDA) | PA0 | J1_10 |  |
| I2C0 | I2C Serial Clock line (SCL) | PA1 | J1_9 |  |
| UART0 | RX Pin | PA11 | J4_33 | Populate Jumper(s): J12[2:3] |
| UART0 | TX Pin | PA10 | J4_34 | Populate Jumper(s): J10[2:3] |
| BOARD | Debug Clock | PA20 | J22_16 |  |
| BOARD | Debug Data In Out | PA19 | J22_14 |  |

Visit [LP_MSPM0L1117](https://www.ti.com/tool/LP-MSPM0L1117) for LaunchPad information, including user guide and hardware files.

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1117 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1117 User's Guide](https://www.ti.com/lit/slau953).

## Example Usage
To use this application feed proper UART data packet to device, and attach I2C peripheral.
Compile, load and run the example.
After receiving UART packet, device will automatically translate to I2C peripheral, or provide error code.
