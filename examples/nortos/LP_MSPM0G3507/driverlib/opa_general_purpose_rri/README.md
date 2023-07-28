## Example Summary

OPA is configured in general purpose mode with rail-to-rail configuration
enabled by default.
The inputs and output are configured for easy connection to external circuitry.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| OPA1 | PB19 | OPA Non-inverting input 0 Pin |
| OPA1 | PA17 | OPA Inverting input 1 Pin |
| OPA1 | PA16 | OPA Output Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PB19 | OPA1 | IN0+ | J3_23 | RC filter R78/C49 not soldered by default |
| PA17 | OPA1 | IN1- | J3_28 | RC Filter R82/C51 not soldered by default |
| PA16 | OPA1 | OUT | J3_29 | J15 1:2 Connect to J3_29 |
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

Connect OPA0_IN0+ and OPA1_IN1- to external circuitry.
Compile, load and run the example.
OPA1_OUT will be the output of the Op-Amp.

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
