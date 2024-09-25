## Example Summary

The following example expands the number of I2C controllers by utilizing the 3 I2C peripherals on the MSPM0L2228. The I2C is configured for 400 kHz transmissions. 
The device remains in SLEEP mode while waiting for an interrupt.
A specific I2C data format is expected for proper operation. <br>

I2C data packet: <br>
[I2C Address] [I2C Read or Write Indicator] [I2C Controller Indicator] [Number of Data Bytes N] [D0] [D1]..[DN]

Please see accompanying sub-system document for more information. [I2C Expander Sub-System](https://www.ti.com/lit/slaeep1)

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA9 | Standard Output |
| SYSCTL |  |  |
| I2C2 | PA16 | I2C Serial Data line (SDA) |
| I2C2 | PA15 | I2C Serial Clock line (SCL) |
| I2C1 | PA10 | I2C Serial Data line (SDA) |
| I2C1 | PA11 | I2C Serial Clock line (SCL) |
| I2C0 | PA0 | I2C Serial Data line (SDA) |
| I2C0 | PA1 | I2C Serial Clock line (SCL) |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA9 | GPIOA | PA9 | J3_3 | N/A |
| PA16 | I2C2 | SDA | J3_24 | N/A |
| PA15 | I2C2 | SCL | J3_25 | N/A |
| PA10 | I2C1 | SDA | J3_30 | <ul><li>PA10 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J3_30 and disconnect XDS-110:<br> `J14 9:10 OFF` Disconnects XDS-110 backchannel to UART TX</ul></ul> |
| PA11 | I2C1 | SCL | J3_29 | <ul><li>PA11 can be connected to XDS-110 backchannel or boosterpack connector:<br><ul><li>To use on boosterpack connector J3_29 and disconnect XDS-110:<br> `J14 7:8 OFF` Disconnects XDS-110 backchannel to UART TX</ul></ul> |
| PA0 | I2C0 | SDA | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA1 | I2C0 | SCL | J3_9 | <ul><li>PA1 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J13 1:2` Use 3.3V pull-up<br><li>`J13 2:3` Use 5V pull-up</ul></ul> |
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
To use this application send proper data packet to the device and connect I2C peripheral(s).
Compile, load and run the example.
After receiving the I2C data packet, the device will automatically send I2C communication on the corresponding interface and trigger a GPIO signal to request a read from the expander. The returned data is formatted such that the first byte of data signifies if there was an ACK or NACK during transmission.