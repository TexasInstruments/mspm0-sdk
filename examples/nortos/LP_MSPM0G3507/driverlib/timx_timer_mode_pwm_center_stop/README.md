## Example Summary

The following example configures TimerG0 to output a pair of center aligned PWM
signals. The device is configured to wait in STOP mode.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG0 | PA12 | Capture/Compare Pin 0 |
| TIMG0 | PA13 | Capture/Compare Pin 1 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA12 | TIMG0 | CCP0 | J4_32/J26_1 | <ul><li>PA12 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R64` is populated by default and connects pin to `J26_1`</ul></ul> |
| PA13 | TIMG0 | CCP1 | J4_32/J26_3 | <ul><li>PA13 can be connected to CAN/LIN connector in addition to boosterpack connector:<br><ul><li>To use on J26 CAN/LIN connector:<br>  `R65` is populated by default and connects pin to `J26_3`</ul></ul> |
| PA20 | DEBUGSS | SWCLK | N/A | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 15:16 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 15:16 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | N/A | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0G3507 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau873).

## Example Usage
Compile, load and run the example.
TimerG0 will output a pair of 2.73Hz center aligned PWM signals on
PA12 and PA13.
PA12 is configured at a 75% duty cycle.
PA13 is configured at a 50% duty cycle.
Monitor the PWM signals on PA12 and PA13.
