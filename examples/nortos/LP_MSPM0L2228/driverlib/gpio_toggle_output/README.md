## Example Summary

Toggles three GPIO pins using HW toggle register.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA23 | Standard Output |
| GPIOB | PB10 | Standard Output |
| GPIOB | PB9 | Standard Output |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA23 | GPIOA | PA23 | N/A | <ul><li>PA23 can be connected to VCC using R8 or to GND using C9<br><ul><li>`R8` and `C9` are not soldered by default allowing the pin to be used for other functions</ul><br><li>PA23 can be connected to LED4 Blue<br><ul><li>`J4 ON` Connect to LED4 Blue<br><li>`J4 OFF` Disconnect from LED4 Blue</ul></ul> |
| PB10 | GPIOB | PB10 | N/A | <ul><li>PB10 can be connected to LED4 Red<br><ul><li>`J5 ON` Connect to LED4 Red<br><li>`J5 OFF` Disconnect from LED4 Red</ul></ul> |
| PB9 | GPIOB | PB9 | N/A | <ul><li>PB9 can be connected to LED4 Green<br><ul><li>`J6 ON` Connect to LED4 Green<br><li>`J6 OFF` Disconnect from LED4 Green</ul></ul> |
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
Compile, load and run the example.
RGB LEDs will toggle with red being opposite of blue and green.
