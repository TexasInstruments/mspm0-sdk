## Example Summary

COMP is configured with external inputs and outputs enabling the analog output
filter and hysteresis.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| VREF |  |  |
| COMP0 | PA11 | COMP Output Pin |
| COMP0 | PA26 | COMP Positive Channel 0 Pin |
| COMP0 | PA27 | COMP Negative Channel 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA11 | COMP0 | OUT | J2_11/J4_35 | J14 1:2 Connect to J2_11<br>J14 2:3 Connect to J4_35 |
| PA26 | COMP0 | IN0+ | J4_38 | J12 OFF: Disconnect LED2 Red |
| PA27 | COMP0 | IN0- | J4_37 | J13 OFF: Disconnect LED2 Blue |
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

Connect COMP0_IN0+ and COMP0_IN0- to external circuitry.
Compile, load and run the example.
Check output COMP0_OUT based on inputs using an oscilloscope.

On powerup, the COMP pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
