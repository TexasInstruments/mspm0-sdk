
## Example Summary

The following example configures TimerG0 in combined capture mode (configure to
measure a PWM signal's period and duty cycle). TimerG7 is used to generate a the
PWM signal. This examples configures timers in down counting mode.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMG7 | PB15 | Counter Compare Pin 0 |
| TIMG0 | PA12 | Counter Compare Pin 0 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB15 | TIMG7 | CCP0 | J2_17 | N/A |
| PA12 | TIMG0 | CCP0 | J4_32/J26_1 | R64 connects J26_1 (default) |
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
Connect a jumper cable between PA12 (GPIO_TIMER_CAPTURE_C0_PIN) and PB15
(GPIO_PWM_C0_PIN).
Compile, load and run the example.
The application will capture two full periods before hitting a SW breakpoint.
This ensures that the timers are fully synchronized (please refer the TRM for
additional details). At the breakpoint, the user can inspect the values of
pwmPeriod and pwmDuty.
The value of pwmDuty should be set to ~50.
The value of pwmPeriod should be set to ~1000.