## Example Summary

This example demonstrates how to simultaneously service three GPIO interrupts.
The corresponding LED will be toggled for each GPIO interrupt.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA12 | High-Speed Input |
| GPIOA | PA13 | High-Speed Input |
| GPIOB | PB27 | Standard Output |
| GPIOB | PB26 | Standard Output |
| GPIOB | PB12 | Standard Input |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA12 | GPIOA | PA12 | J4_32/J26_1 | R64 connects J26_1 (default) |
| PA13 | GPIOA | PA13 | J4_32/J26_3 | R65 connects J26_3 (default) |
| PB27 | GPIOB | PB27 | J27_10 | J7 ON/OFF Connect/Disconnect LED2 Green |
| PB26 | GPIOB | PB26 | J27_8 | J6 ON/OFF Connect/Disconnect LED2 Red |
| PB12 | GPIOB | PB12 | J2_19 | N/A |
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
The LEDs will toggle based on the serviced GPIO.

For demo purposes, to perform simultaneous interrupts connect the input pins to the same input signal.
