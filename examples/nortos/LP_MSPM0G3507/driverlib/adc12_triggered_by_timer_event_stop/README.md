## Example Summary

The following example configures ADC1 to be triggered by a TimerG instance 1s
after the timer has been started while the device remains Stop mode.
LED1 is set if ADC conversion is greater than 1.5V using window comparator
feature.

This example also showcases how to configure ADC1 in repeat single mode
triggered by a timer event.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| SYSCTL |  |  |
| TIMG0 |  |  |
| ADC1 | PB19 | ADC12 Channel 6 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J19 1:2` Use 3.3V pull-up<br><li>`J19 2:3` Use 5V pull-up</ul><br><li>PA0 can be connected to LED1<br><ul><li>`J4 ON` Connect to LED1<br><li>`J4 OFF` Disconnect from LED1</ul></ul> |
| PB19 | ADC1 | A6 | J3_23 | N/A |
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
Compile, load and run the example. Provide a valid voltage to A0_2
and monitor LED1 (Voltage source for ADC is excepted to be between
0v and MCU supply voltage).

On powerup, the ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
