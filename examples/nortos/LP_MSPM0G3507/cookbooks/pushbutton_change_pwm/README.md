## Example Summary

This example demonstrates using the two switches on the MSPM0G3507 Launchpad to change the period and duty cycle on TIMA1 PWM output channels CC0 and CC1. Pressing switch S1 changes the PWM period of the timer. Pressing S2 changes the duty cycle on CC1 output. This example also demonstrates the use of TIMA0 configured as a one-shot timer and interrupts for debouncing the switches.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA18 | Standard with Wake Input |
| GPIOB | PB22 | Standard Output |
| GPIOB | PB27 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB21 | Standard Input with internal pull-up |
| SYSCTL |  |  |
| TIMA1 | PA10 | Counter Compare Pin 0 |
| TIMA1 | PA11 | Counter Compare Pin 1 |
| TIMA0 |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | GPIOA | PA18 | J3_26/J3_29 | J15 2:3 Connect to J3_29<br>R58 connects to J3_26 (default)<br>R23 connects to J101 and J8 for BSL_Invoke (default)<br>J8 ON/OFF Connect/Disconnect S1 for BSL_Invoke (with pull-down)<br>J101 17:18 Connect/Disconnect from XDS-110 BSL_Invoke<br>R66/R67 for thermistor RT1 not soldered by default |
| PB22 | GPIOB | PB22 | J27_5 | J5 ON/OFF Connect/Disconnect LED2 Blue |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB21 | GPIOB | PB21 | J27_4 | Connected to S2 button (no pull resistor) |
| PA10 | TIMA1 | CCP0 | J4_34/J26_5 | J21 2:3 Connect to J4_34<br>R62 connects J26_5 (default) |
| PA11 | TIMA1 | CCP1 | J4_33/J26_6 | J22 2:3 Connect to J4_33<br>R63 connects J26_6 (default) |
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
Compile, load, and run the example. At startup, TIMA1 is loaded with a PWM period of 1ms (1000Hz) and 75% duty cycle on CC0 and 25% duty cycle on CC1.  Pressing switch S1 will cycle through the following PWM periods: 1ms, 2ms, 4ms, and 0.5ms.    These changes are reflected on both CC0 and CC1 outputs. Pressing switch S2 will cycle through the following PWM duty cycles: 75%, 90%, 25%, and 50%

