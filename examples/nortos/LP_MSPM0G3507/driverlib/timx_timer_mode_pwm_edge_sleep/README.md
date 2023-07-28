## Example Summary

The following example configures TimerG0 to output a pair of edge aligned PWM
signals. The device is configured to wait in SLEEP mode.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG0 | PA12 | Counter Compare Pin 0 |
| TIMG0 | PA13 | Counter Compare Pin 1 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA12 | TIMG0 | CCP0 | J4_32/J26_1 | R64 connects J26_1 (default) |
| PA13 | TIMG0 | CCP1 | J4_32/J26_3 | R65 connects J26_3 (default) |
| PA20 | DEBUGSS | SWCLK | N/A | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | N/A | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0G3507 User's Guide](https://www.ti.com/lit/slau846).

## Example Usage
Compile, load and run the example.
TimerG0 will output a pair of 62.5Hz edge aligned PWM signals on PA12 and PA13.
PA12 is configured at a 75% duty cycle.
PA13 is configured at a 50% duty cycle.
Monitor the PWM signals on PA12 and PA13.
