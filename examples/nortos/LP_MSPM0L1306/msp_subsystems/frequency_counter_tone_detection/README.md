## Example Summary

The following example configures the on-board comparator, TimerG0, and TimerG4 capturing analog signals to periodically convert to digital signals. An LED is toggled to indicate whether the input signal is within the desired frequency range. This example configures timers in down counting mode. 

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA13 | Standard Output |
| GPIOA | PA3 | Standard Output |
| GPIOA | PA7 | Standard Output |
| SYSCTL |  |  |
| TIMG4 |  |  |
| TIMG0 |  |  |
| COMP0 | PA11 | COMP Output Pin |
| COMP0 | PA27 | COMP Negative Channel 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA13 | GPIOA | PA13 | J4_39 | <ul><li>PA13 can be connected to LED2 Green<br><ul><li>`J3 ON` Connect to LED2 Green<br><li>`J3 OFF` Disconnect from LED2 Green</ul></ul> |
| PA3 | GPIOA | PA3 | J2_19 | N/A |
| PA7 | GPIOA | PA7 | J2_18 | N/A |
| PA11 | COMP0 | OUT | J2_11/J4_35 | <ul><li>PA11 can be connected to two locations in boosterpack connector:<br><ul><li>`J14 1:2`: Connect pin to `J2_11`<br><li>`J14 2:3`: Connect pin to `J4_35`</ul></ul> |
| PA27 | COMP0 | IN0- | J4_37 | <ul><li>PA27 can be connected to LED2 Blue<br><ul><li>`J13 OFF` Disconnect from LED2 Blue</ul></ul> |
| PA20 | DEBUGSS | SWCLK | J2_13 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0L1306 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Connect input signal to PA27 (COMP_0 IN0-). Configure capture period and/or frequency range settings in `main.c`. Within sysconfig, the period between captures can be adjusted (TIMER > Desired Timer Period) as well as offset voltage (COMP > Input Code Through DACCODE0). Compile, load and run the example. LED2 toggles ON when the frequency is within desired range and toggles OFF when the frequency is outside of the desired range.

