## Example Summary

The following example configures ADC0 to be triggered by a TimerG instance 1s
after the timer has been started while the device remains Stop mode.
LED1 is set if ADC conversion is greater than 1.5V using window comparator
feature.

This example also showcases how to configure ADC0 in repeat single mode
triggered by a timer event.

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA22 | Standard Output |
| GPIOA | PA2 | Standard Output |
| SYSCTL |  |  |
| TIMG14 |  |  |
| ADC0 | PA25 | ADC12 Channel 2 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | GPIOA | PA22 | J1_8 | N/A |
| PA2 | GPIOA | PA2 | J2_13 | N/A |
| PA25 | ADC0 | A2 | J1_2 | N/A |
| PA20 | DEBUGSS | SWCLK | J2_11 | <ul><li>PA20 is used by SWD during debugging<br><ul><li>`J101 13:14 ON` Connect to XDS-110 SWCLK while debugging<br><li>`J101 13:14 OFF` Disconnect from XDS-110 SWCLK if using pin in application</ul></ul> |
| PA19 | DEBUGSS | SWDIO | J2_17 | <ul><li>PA19 is used by SWD during debugging<br><ul><li>`J101 11:12 ON` Connect to XDS-110 SWDIO while debugging<br><li>`J101 11:12 OFF` Disconnect from XDS-110 SWDIO if using pin in application</ul></ul> |

### Device Migration Recommendations
This project was developed for a superset device included in the LP_MSPM0C1104 LaunchPad. Please
visit the [CCS User's Guide](https://software-dl.ti.com/msp430/esd/MSPM0-SDK/latest/docs/english/tools/ccs_ide_guide/doc_guide/doc_guide-srcs/ccs_ide_guide.html#sysconfig-project-migration)
for information about migrating to other MSPM0 devices.

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0C1104 User's Guide](https://www.ti.com/lit/slau908).

## Example Usage
Compile, load and run the example. Provide a valid voltage to A0_2
pin and monitor LED1 (Voltage source for ADC is excepted to be between
0v and MCU supply voltage).

On powerup, the ADC pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.

## Important Note
Early experimental samples have a known issue where the user is unable to
reconnect their debugger connection after the device has gone into STOP or
STANDBY mode. Please review advisory DEBUGSS_02 to see the steps to regain
the ability to connect the device to the debugger. This information is located
in the "MSPM0G and MSPM0L Families: Functional Differences With Early Samples"
document.
