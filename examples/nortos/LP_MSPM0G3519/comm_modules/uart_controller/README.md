## Example Summary

This example implements a UART controller using the MSP Communication modules - UART. This example demonstrates sending commands to a UART Target to read from or write to its memory address and shows the response.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| UART0 | PA11 | RX Pin |
| UART0 | PA10 | TX Pin |
| CRC |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3519](https://www.ti.com/tool/LP-MSPM0G3519) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | UART0 | RX | J4_33 | <ul><li>PA11 can be used as UART RX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J19:<br>  `J15 1:2`: Connects XDS-110 backchannel UART RX to `J19`<br>  `J19 7:8` Connects XDS-110 backchannel to UART RX<br><li>To use UART on boosterpack connector:<br>  `J15 2:3`: Connects UART RX to `J4_33`<br></ul> |
| PA10 | UART0 | TX | J4_34 | <ul><li>PA10 can be used as UART TX connected to XDS-110 backchannel or to boosterpack connector:<br><ul><li>To use backchannel UART on J19:<br>  `J14 1:2`: Connects XDS-110 backchannel UART TX to `J19`<br>  `J19 9:10` Connects XDS-110 backchannel to UART TX<br><li>To use UART on boosterpack connector:<br>  `J14 2:3`: Connects UART TX to `J4_34`<br></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J19 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J19 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J19 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J19 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3519 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

## Example Usage
Connect UART Rx and Tx of Controller to UART Tx and Rx of Target respectively. Compile, load and run the example.

Send command to Target by setting gSendCommand as true and check response through gResponse from expressions.

Please refer to MSPM0 Communication Modules - UART User Guide for more details.
