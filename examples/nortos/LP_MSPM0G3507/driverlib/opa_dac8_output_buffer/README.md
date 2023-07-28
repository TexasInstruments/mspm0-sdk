## Example Summary

OPA is configured in output buffer configuration for the 8-bit DAC.The 8-bit
DAC uses VDDA as its reference voltage.
The output is configured for easy connection to external circuitry.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| VREF | PA23 | VREF Positive (VREF+) |
| VREF | PA21 | VREF Negative (VREF-) |
| COMP0 |  |  |
| OPA0 | PA22 | OPA Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA23 | VREF | VREF+ | N/A | C8 to GND soldered by default<br>C7 to GND can be soldered for custom VREF capacitor<br>Solder R22 to connect to J28_10 |
| PA21 | VREF | VREF- | J28_8 | R3 to GND soldered by default<br>Solder R10 to connect to J28_8 |
| PA22 | OPA0 | OUT | J3_24 | J16 ON/OFF: Connect/Disconnect from Light sensor D1<br>RC Filter R83/C52 not soldered by default |
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
OPA0_OUT will output DAC8 value.

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
