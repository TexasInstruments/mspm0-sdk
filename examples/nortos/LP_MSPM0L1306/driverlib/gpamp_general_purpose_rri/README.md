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

Visit [LP_MSPM0L1306](https://www.ti.com/tool/LP-MSPM0L1306) for LaunchPad information, including user guide and hardware files.

| Pin | Peripheral | Function | LaunchPad Pin | LaunchPad Settings |
| --- | --- | --- | --- | --- |
| PA26 | GPAMP | IN+ | J4_38 | J12 OFF: Disconnect LED2 Red |
| PA18 | GPAMP | IN- | J3_26 | J11 OFF: Disconnect S1 button (with pull-down)<br>J101 17:18 OFF: Disconnect from XDS-110 BSL_Invoke |
| PA22 | GPAMP | OUT | J1_8 | J6 ON/OFF: Connect/disconnect from photodiode D1<br>R95/C57 can be soldered for thermistor RT1<br>J15 ON: Connects Thermistor RT1, if needed |
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
