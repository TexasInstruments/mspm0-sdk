## Example Summary

The following example configures the UART1 at 9600bps waiting to echo received characters.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART1 | PA9 | RX Pin |
| UART1 | PA8 | TX Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |
| GPIOA | PA0 | Open-Drain Output |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA9 | UART1 | RX | J1_3 | J19 7:8 Disconnect XDS-110 BackChannel UART RX<br>J1:3 Connect XDS-110 BackChannel UART on J19 |
| PA8 | UART1 | TX | J1_4 | J19 9:10 Disconnect XDS-110 BackChannel UART TX<br>J1:4 Connect XDS-110 BackChannel UART on J19 |
| PA0 | GPIOA | PA0 | J5_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J2 1:2` Use 3.3V pull-up<br><li>`J2 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J6 ON` Connect to LED1<br><li>`J6 OFF` Disconnect from LED1</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | J19 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J19 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3519 web page](https://www.ti.com/tool/LP-MSPM0G3519).


### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#non-sysconfig-compatible-project-migration)
for information about migrating to other MSPM0 devices.

## Example Usage

Compile, load and run the example.
Connect to terminal if using Launchpad's back-channel UART, or connect to
external device using BoosterPack.
The UART will wait to receive data and respond with echo.

## Application Design Details

* This example shows how to initialize the UART1 driver in blocking read and write mode and echo characters back to a console.
* A single thread, mainThread, reads a character from configured UART and writes it back.
