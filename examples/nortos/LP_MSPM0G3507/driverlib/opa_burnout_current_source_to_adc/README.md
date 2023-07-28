## Example Summary

OPA is configured in buffer mode using connecting positive input to ADC.
The burnout current source is then enabled to detect a possible short or open
condition.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| ADC0 | PA22 | ADC12 Channel 7 Pin |
| OPA0 | PA26 | OPA Non-inverting input 0 Pin |
| EVENT |  |  |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA22 | ADC0 | A7 | J3_24 | J16 ON/OFF: Connect/Disconnect from Light sensor D1<br>R83/C52 can be soldered for RC filter |
| PA26 | OPA0 | IN0+ | J1_5 | J9 1:2/2:3 Disconnect/Connect from thermistor RT1<br>J18 ON/OFF  Connect/Disconnect from Light Sensor D1 |
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

Connect OPA0_IN0+ to external circuitry.
Compile, load and run the example.
Use debugger to check ADC result with and without burnout current source.

## Application Design Details

This example shows an implementation of OpAmp in buffer configuration with the
capability to check for open or short connection using the OPA integrated
burnout current source.

On powerup, the OPA pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.
