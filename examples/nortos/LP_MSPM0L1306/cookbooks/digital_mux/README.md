## Example Summary

Uses GPIO port interrupts to emulate a digital mux. Application code assigns output based on states of pins PA26 and PA27.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| GPIOA | PA0 | Open-Drain Output |
| GPIOA | PA3 | Standard Input |
| GPIOA | PA7 | Standard Input |
| GPIOA | PA4 | Standard Input |
| GPIOA | PA23 | Standard Input |
| GPIOA | PA26 | Standard Input |
| GPIOA | PA27 | Standard Input |
| SYSCTL |  |  |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA0 | GPIOA | PA0 | J1_10 | This pin is 5V tolerant open-drain and requires pull-up.<br>J2 ON/OFF: Connect/Disconnect LED1<br>J10 1:2 5V Pull-up<br>J10 2:3 3.3V Pull-up |
| PA3 | GPIOA | PA3 | J2_19 | N/A |
| PA7 | GPIOA | PA7 | J2_18 | N/A |
| PA4 | GPIOA | PA4 | J2_14 | N/A |
| PA23 | GPIOA | PA23 | J2_12 | C8/C11 to GND not soldered by default |
| PA26 | GPIOA | PA26 | J4_38 | J12 ON/OFF: Connect/Disconnect LED2 Red |
| PA27 | GPIOA | PA27 | J4_37 | J13 ON/OFF: Connect/Disconnect LED2 Blue |
| PA20 | DEBUGSS | SWCLK | J2_13 | J101 15:16 ON: Connect to XDS-110 SWCLK (debug) |
| PA19 | DEBUGSS | SWDIO | J2_17 | J101 13:14 ON: Connect to XDS-110 SWDIO (debug) |

### Low-Power Recommendations
TI recommends to terminate unused pins by setting the corresponding functions to
GPIO and configure the pins to output low or input with internal
pullup/pulldown resistor.

SysConfig allows developers to easily configure unused pins by selecting **Board**→**Configure Unused Pins**.

For more information about jumper configuration to achieve low-power using the
MSPM0 LaunchPad, please visit the [LP-MSPM0L1306 User's Guide](https://www.ti.com/lit/slau869).

## Example Usage
Compile, load and run the example.
Use logic analyzer to see output change based on inputs or watch the LEDs to see different output based on PA26 and PA27.
