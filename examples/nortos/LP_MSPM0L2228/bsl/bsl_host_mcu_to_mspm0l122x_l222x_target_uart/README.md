## Example Summary

This demo shows how to program a MSPM0L2228 MCU using a LP-MSPM0L2228 as host by BSL UART interface.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOB | PB8 | Standard Input with internal pull-up |
| GPIOB | PB5 | Standard Output(To control NRST pin) |
| GPIOB | PB4 | Standard Output(To control invoke pin) |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA23 | Standard Output |
| SYSCTL |  |  |
| UART0 | PB1 | RX Pin |
| UART0 | PB0 | TX Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB8 | GPIOB | PB8 | N/A | <ul><li>PB8 is connected to S3 button to GND with no external pull resistor<br><ul><li>Press `S3` button to connect pin to GND<br><li>Don't use `S3` button if not needed by application</ul></ul> |
| PB5 | GPIOB | PB5 | J7_12 | N/A |
| PB4 | GPIOB | PB4 | J7_11 | N/A |
| PA0 | GPIOA | PA0 | J3_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J12 1:2` Use 3.3V pull-up<br><li>`J12 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED3<br><ul><li>`J2 ON` Connect to LED3<br><li>`J2 OFF` Disconnect from LED3</ul></ul> |
| PA23 | GPIOA | PA23 | N/A | <ul><li>PA23 can be connected to LED4 Blue<br><ul><li>`J4 ON` Connect to LED4 Blue<br><li>`J4 OFF` Disconnect from LED4 Blue</ul></ul> |
| PB1 | UART0 | RX | J7_38 | N/A |
| PB0 | UART0 | TX | J7_37 | N/A |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J14 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J14 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
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
MSPM0 LaunchPad, please visit the [LP-MSPM0L2228 User's Guide](https://www.ti.com/lit/slau928).

## Example Usage

Connect the hardware that descriped in the document. Compile, load and run the example.
Push the S3 button to start program MSPM0L2228.
