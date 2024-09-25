## Example Summary

The following example configures TimerG0 in combined capture mode (configure to
measure a PWM signal's period and duty cycle). TimerG1 is used to generate a the
PWM signal.

**Note**: Example requires Rev E3 or later of the MSPM0C1104 LaunchPad.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG8 | PA0 | Capture/Compare Pin 0 |
| TIMG14 | PA23 | Capture/Compare Pin 0 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0C1104](https://www.ti.com/tool/LP-MSPM0C1104) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | TIMG8 | CCP0 | J1_10 | <ul><li>PA0 is 5V tolerant open-drain so it requires pull-up<br><ul><li>`J20 1:2` Use 3.3V pull-up<br><li>`J20 2:3` Use 5V pull-up</ul></ul> |
| PA23 | TIMG14 | CCP0 | J2_12 | N/A |
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
Connect a jumper cable between PA12 (GPIO_TIMER_CAPTURE_C0_PIN) and PB15
(GPIO_PWM_C0_PIN).
Compile, load and run the example.
The application will capture two full periods before hitting a SW breakpoint.
This ensures that the timers are fully synchronized (please refer the TRM for
additional details). At the breakpoint, the user can inspect the values of
pwmPeriod and pwmDuty.
The value of pwmDuty should be set to ~50.
The value of pwmPeriod should be set to ~1000.
