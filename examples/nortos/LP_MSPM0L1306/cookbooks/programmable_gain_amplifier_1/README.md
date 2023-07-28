## Example Summary

OPA is configured in non-inverting PGA configuration. 
The ADC is configured to sample OPA output. 
The Window comparator mode fo the ADC is engaged to detect high and low ADC value boundaries. 
When hitting these boundaries, the OPA gain is adjusted to keep input singal in optimal bands of resolution. 
The inputs are configured for easy connection to external circuitry.

Refer to the documentation of this design at www.ti.com/lit/slaae80

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 |  |  |
| OPA1 | PA18 | OPA Non-inverting input 0 Pin |
| OPA1 | PA16 | OPA Output Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA18 | OPA1 | IN0+ | J3_26 | J11 OFF: Disconnect S1 button (with pull-down)<br>J101 17:18 OFF: Disconnect from XDS-110 BSL_Invoke |
| PA16 | OPA1 | OUT | J3_24 | RC Filter R19/C10 not soldered by default |
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

Connect OPA1_IN0+ to external circuitry. This example has default
non-inverting gain of 2. To avoid saturation on OPA1_OUT input signal amplitude
should be between 0 and VCC/2 V.

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
