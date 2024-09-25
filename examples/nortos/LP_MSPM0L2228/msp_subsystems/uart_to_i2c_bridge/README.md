## Example Summary

The following example bridges incoming UART transmissions to 
to I2C peripherals. The UART is confgured at 9600bps to 
transmit and receive data. The I2C is configured for 100kHz transmissions. 
The device remains in SLEEP mode while waiting for an interrupt.
A specific UART data format is expected for proper operation. <br>
UART data packet: <br>
[Start Byte] [I2C Address] [I2C Read or Write indicator] [Message Length N] [D0] [D1]..[DN]

Please see accompanying sub-system document for more information. [UART to I2C Sub-system](https://www.ti.com/lit/spradd0)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| I2C0 | PA10 | I2C Serial Data line (SDA) |
| I2C0 | PA11 | I2C Serial Clock line (SCL) |
| UART0 | PA1 | RX Pin |
| UART0 | PA0 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA10 | I2C0 | SDA | J3_30 | <ul><li>PA10 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J3_30 and disconnect XDS-110:<br> `J14 9:10 OFF` Disconnects XDS-110 backchannel to UART TX</ul></ul> |
| PA11 | I2C0 | SCL | J3_29 | <ul><li>PA11 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J3_29 and disconnect XDS-110:<br> `J14 7:8 OFF` Disconnects XDS-110 backchannel to UART TX</ul></ul> |
| PA1 | UART0 | RX | J3_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J13 1:2` Use 3.3V pull-up<br><li>`J13 2:3` Use 5V pull-up</ul></ul> |
| PA0 | UART0 | TX | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J14 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J14 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

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
To use this application feed proper UART data packet to device, and attach I2C peripheral.
Compile, load and run the example.
After receiving UART packet, device will automatically translate to I2C peripheral, or provide error code.
