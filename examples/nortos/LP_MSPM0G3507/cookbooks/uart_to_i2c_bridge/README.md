## Example Summary

The following example bridges incoming UART transmissions to 
to I2C peripherals. The UART is confgured at 9600bps to 
transmit and receive data. The I2C is configured for 100kHz transmissions. 
The device remains in SLEEP mode while waiting for an interrupt.
A specific UART data format is expected for proper operation. <br>
UART data packet: <br>
[Start Byte] [I2C Address] [I2C Read or Write indicator] [Message Length N] [D0] [D1]..[DN]

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | I2C0 | SDA | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 OFF: Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA1 | I2C0 | SCL | J28_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J20 1:2 3.3V Pull-up<br>J20 2:3 5V Pull-up |
| PA11 | UART0 | RX | J4_33/J26_6 | J22 1:2 Connect XDS-110 BackChannel UART on J101<br>J22 2:3 Connect to J4_33<br>R63 connects J26_6 for LIN (default)<br>J101 7:8 Connect XDS-110 BackChannel UART RX |
| PA10 | UART0 | TX | J4_34/J26_5 | J21 1:2 Connect XDS-110 BackChannel UART on J101<br>J21 2:3 Connect to J4_34<br>R62 connects J26_5 for LIN (default)<br>J101 9:10 Connect XDS-110 BackChannel UART TX |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 web page](https://www.ti.com/tool/LP-MSPM0G3507).

## Example Usage
To use this application feed proper UART data packet to device, and attach I2C peripheral.
Compile, load and run the example.
After receiving UART packet, device will automatically translate to I2C peripheral, or provide error code.
