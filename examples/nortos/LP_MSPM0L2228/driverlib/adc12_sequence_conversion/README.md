## Example Summary

The following example configures ADC0 to trigger sequence of conversions on 4 different channels and stores the results in 4 different buffer arrays.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| ADC0 | PA27 | ADC12 Channel 0 Pin |
| ADC0 | PA22 | ADC12 Channel 7 Pin |
| ADC0 | PA24 | ADC12 Channel 3 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L2228](https://www.ti.com/tool/LP-MSPM0L2228) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA25 | ADC0 | A2 | J7_31 | N/A |
| PA27 | ADC0 | A0 | J7_33 | N/A |
| PA22 | ADC0 | A7 | J3_6 | N/A |
| PA24 | ADC0 | A3 | J3_2 | N/A |
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

Compile, load and run the example. Connect vaild inputs ranging from 0-VCC to the ADC channels, floating pins are not guaranteed to read as 0.
