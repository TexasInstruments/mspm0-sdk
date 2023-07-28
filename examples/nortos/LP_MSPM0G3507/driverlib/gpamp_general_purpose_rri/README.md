## Example Summary

GPAMP is configured in general purpose mode with rail-to-rail configuration
enabled by default.
The inputs and output are configured for easy connection to external circuitry.

## Peripherals & Pin Assignments

| Peripheral | Pin | Function |
| --- | --- | --- |
| SYSCTL |  |  |
| GPAMP | PA26 | GPAMP Non-inverting input Pin |
| GPAMP | PA18 | GPAMP Inverting input Pin |
| GPAMP | PA22 | GPAMP Output Pin |
| DEBUGSS | PA20 | Debug Clock |
| DEBUGSS | PA19 | Debug Data In Out |

## BoosterPacks, Board Resources & Jumper Settings

Visit [LP_MSPM0G3507](https://www.ti.com/tool/LP-MSPM0G3507) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA26 | GPAMP | IN+ | J1_5 | J9 1:2/2:3 Disconnect/Connect from thermistor RT1<br>J18 ON/OFF  Connect/Disconnect from Light Sensor D1 |
| PA18 | GPAMP | IN- | J3_26/J3_29 | J15 2:3 Connect to J3_29<br>R58 connects to J3_26 (default)<br>R23 connects to J101 and J8 for BSL_Invoke (default)<br>J8 OFF Disconnect S1 for BSL_Invoke (with pull-down)<br>J101 17:18 OFF Disconnect from XDS-110 BSL_Invoke<br>R66/R67 can be soldered for thermistor RT1  |
| PA22 | GPAMP | OUT | J3_24 | J16 ON/OFF: Connect/Disconnect from Light sensor D1<br>RC Filter R83/C52 not soldered by default |
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

Connect GPAMP_IN+ and GPAMP_IN- to external circuitry.
Compile, load and run the example.
GPAMP_OUT will be the output of the Op-Amp.

On powerup, the GPAMP pin(s) used in this example are by default set to
the correct analog mode. Therefore, calls to
DL_GPIO_initPeripheralAnalogFunction for pinmuxing those pins are not needed.

## Application Design Details

This example enables users to implement a custom analog signal chain by
connecting external circuitry to the device.
Developers can use SysConfig to modify the GPAmp default configuration.
