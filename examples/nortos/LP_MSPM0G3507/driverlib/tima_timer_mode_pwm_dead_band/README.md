## Example Summary

The following example configures TimerA in PWM edge align mode and
outputs 4 PWM signal. The pair of signals on C0 and C1 are complementary
signals with deadband insertion. The pair of signals on C2 and C3 are also
complementary signals with dead band insertion.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| TIMA0 | PA0 | Counter Compare Pin 0 |
| TIMA0 | PA22 | Counter Compare Complementary Pin 0 |
| TIMA0 | PB20 | Counter Compare Pin 1 |
| TIMA0 | PB13 | Counter Compare Complementary Pin 1 |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | TIMA0 | CCP0 | J27_9 | This pin is 5V tolerant open-drain and requires pull-up.<br>J4 ON/OFF: Connect/Disconnect LED1<br>J19 1:2 3.3V Pull-up<br>J19 2:3 5V Pull-up |
| PA22 | TIMA0 | CCP0_CMPL | J3_24 | J16 OFF: Disconnect from Light sensor D1<br>RC Filter R83/C52 not soldered by default |
| PB20 | TIMA0 | CCP1 | J4_36 | N/A |
| PB13 | TIMA0 | CCP1_CMPL | J4_35 | N/A |
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
Compile, load and run the example. Monitor the PWM signals on PA0, PA22, PB20,
and PB13.
The pair of signals on PA0 and PA22 are complementary signals with deadband
insertion.
The second pair of signals on PB20 and PB13 are also complementary signals with
deadband insertion.
