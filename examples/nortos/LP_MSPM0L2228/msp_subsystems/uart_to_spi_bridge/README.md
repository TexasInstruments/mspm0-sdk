## Example Summary

The following example bridges incoming UART transmissions to 
to SPI peripherals. The UART is confgured at 9600bps to 
transmit and receive data. The SPI is configured for 8MHz transmissions. 
The device remains in SLEEP mode while waiting for an interrupt.
A specific UART data format is expected for proper operation. <br>
UART data packet: <br>
[Start Byte] [Read or Write indicator] [Message Length N] [D0] [D1]..[DN]

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA1 | RX Pin |
| UART0 | PA0 | TX Pin |
| SPI1 | PA17 | SPI SCLK (Clock) |
| SPI1 | PA18 | SPI PICO (Peripheral In, Controller Out) |
| SPI1 | PA16 | SPI POCI (Peripheral Out, Controller In) |
| SPI1 | PA13 | SPI CS0 (Chip Select 0) |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA1 | UART0 | RX | J3_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J13 1:2` Use 3.3V pull-up<br><li>`J13 2:3` Use 5V pull-up</ul></ul> |
| PA0 | UART0 | TX | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA17 | SPI1 | SCLK | J3_7 | N/A |
| PA18 | SPI1 | MOSI | J3_23 | <ul><li>PA18 can be connected to S2 button to VCC with external pull-down<br><ul><li>`J1 OFF` Disconnect S2 button and external pull-down</ul><br><li>PA18 can be connected to XDS-110 BSL_Invoke<br><ul><li>`J14 17:18 OFF` Disconnect pin to XDS-110 BSL Invoke</ul></ul> |
| PA16 | SPI1 | MISO | J3_24 | N/A |
| PA13 | SPI1 | CS0 | J3_27 | <ul><li>PA13 has a capacitor to be used for LCDCAP0 functionality by default in Launchpad<br><ul><li>Remove `C50` if needed to use pin for other functions</ul></ul> |
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
To use this application feed proper UART data packet to device, and attach SPI peripheral.
Compile, load and run the example.
After receiving UART packet, device will automatically translate to SPI peripheral, or provide error code.
